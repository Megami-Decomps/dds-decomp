#!/usr/bin/env python3
"""Bounded, read-only camera peer-selector probe. Never prints native words."""
import argparse
import json
import struct
import sys
from dataclasses import dataclass
from pathlib import Path

MASK = (1 << 64) - 1
BASE = 0x001FA480
SIZE = 5256
# Synthetic addresses, not recovered game addresses.
ACTION = 0x50000000
SCRIPT = 0x51000000
LINK = 0x52000000
REFERENCE = 0x53000000
PEERS = (0x54000000, 0x54001000)
RUNTIME = 0x55000000
POSE = 0x56000000
STACK = 0x70000000
RETURN = 0x7FFF0000

class Blocked(Exception):
    pass

def need(condition, reason):
    if not condition:
        raise Blocked(reason)

def signed(value, bits):
    value &= (1 << bits) - 1
    return value - (1 << bits) if value & (1 << (bits - 1)) else value

@dataclass(frozen=True)
class V:
    value: object = None
    tags: frozenset = frozenset()

def val(n, *tags):
    return V(n & MASK, frozenset(tags))

def number(v):
    need(v.value is not None, "unknown_value_used")
    return v.value

def combine(n, a, b=V(0)):
    return V(n & MASK, a.tags | b.tags)

def float_bits(f):
    return struct.unpack("<I", struct.pack("<f", f))[0]

def bits_float(n):
    return struct.unpack("<f", struct.pack("<I", n & 0xFFFFFFFF))[0]

def direct_target(pc, word):
    return ((pc + 4) & 0xF0000000) | ((word & 0x03FFFFFF) << 2)

class VM:
    """Only the scalar operations needed on the peer paths are supported."""
    def __init__(self, words, read_native, calls, runtime_site, clear_site,
                 cursor, gp, selected_kind=34, phase=0):
        self.words = words
        self.read_native = read_native
        self.calls = calls
        self.runtime_site = runtime_site
        self.clear_site = clear_site
        self.cursor = cursor
        self.selected_kind = selected_kind
        self.phase = phase
        self.r = [V() for _ in range(32)]
        self.r[0] = val(0)
        self.r[4] = val(ACTION, "action")
        self.r[5] = val(POSE, "pose")
        self.r[6] = val(SCRIPT, "script")
        self.r[28] = val(gp)
        self.r[29] = val(STACK)
        self.r[31] = val(RETURN)
        self.f = [None] * 32
        self.fcc = None
        self.pc = BASE
        self.cells = {}
        self.epoch = 0
        self.active_peer = None
        self.rank = []
        self.clears = []
        self.runtime_count = 0
        self.kind_reads = []
        self.phase_reads = []
        self.second_seen = False
        self.finished = False
        self.put(ACTION + 0x114, 4, val(LINK, "link"))
        self.put(LINK + 0x18, 4, val(REFERENCE, "reference"))
        self.put(RUNTIME + 0x24C, 4, val(PEERS[0], "peer.0"))
        for i, p in enumerate(PEERS):
            self.put(p + 0x110, 8, val(0x201))
            self.put(p + 0x364, 4,
                     val(PEERS[1], "peer.1") if i == 0 else val(0))
        self.put(SCRIPT, 4, val(34, "kind"))
        self.put(SCRIPT + 8, 4, val(float_bits(0.0)))
        self.put(SCRIPT + 12, 4, val(float_bits(10.0)))
        self.put(SCRIPT + 16, 4, val(48, "terminal_kind"))
        self.put(cursor + 2, 2, val(1))
        self.put(cursor + 0x13, 1, val(phase))

    def put(self, address, width, value):
        # An overlapping write must not leave an older wider/narrower cell live.
        for start, span in list(self.cells):
            if address < start + span and start < address + width:
                del self.cells[start, span]
        self.cells[address, width] = value

    def word(self, pc):
        need(BASE <= pc < BASE + 4 * len(self.words)
             and (pc - BASE) % 4 == 0, "control_left_function")
        return self.words[(pc - BASE) // 4]

    def read(self, address, width):
        if address == SCRIPT and width == 4:
            self.kind_reads.append((self.epoch, self.runtime_count))
        if address == self.cursor + 0x13 and width == 1:
            self.phase_reads.append(self.epoch)
        for i, p in enumerate(PEERS):
            if address in (p + 0x110, p + 0x11C):
                self.active_peer = i
                self.second_seen |= i == 1
            if address == p + 0x11C and width == 1:
                return val(0 if i == 0 else 2, "id.peer.%d" % i)
        if address == REFERENCE + 0x11C and width == 1:
            return val(1, "id.reference.%d" % self.epoch)
        if (address, width) in self.cells:
            return self.cells[address, width]
        for (start, span), v in self.cells.items():
            if start <= address and address + width <= start + span:
                shift = address - start
                # The model knows each GPR's low 64 bits only.
                need(shift + width <= 8, "unsupported_subcell_read")
                if v.value is None:
                    return v
                return V((v.value >> (shift * 8)) & ((1 << (width * 8)) - 1),
                         v.tags)
        need(not 0x50000000 <= address <= RETURN,
             "unmodeled_synthetic_memory")
        data = self.read_native(address, width)
        need(data is not None and len(data) == width, "unmapped_native_data")
        return val(int.from_bytes(data, "little"), "native_data")

    def write_reg(self, reg, value):
        if reg:
            self.r[reg] = value
        self.r[0] = val(0)

    @staticmethod
    def is_control(w):
        op, fn = w >> 26, w & 63
        rs = (w >> 21) & 31
        return (op in (1, 2, 3, 4, 5, 6, 7, 20, 21, 22, 23)
                or (op == 0 and fn in (8, 9))
                or (op == 17 and rs == 8))

    def data(self, pc):
        w = self.word(pc)
        need(not self.is_control(w), "control_in_delay_slot")
        op = w >> 26
        rs, rt, rd = (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
        sa, fn = (w >> 6) & 31, w & 63
        imm = w & 0xFFFF
        simm = signed(imm, 16)
        a, b = self.r[rs], self.r[rt]
        if op == 0:
            if fn in (10, 11):
                take = number(b) == 0 if fn == 10 else number(b) != 0
                if take:
                    self.write_reg(rd, a)
                return
            if fn in (0, 2, 3, 4, 6, 7, 56, 58, 59, 60, 62, 63):
                shift = number(a) & 31 if fn in (4, 6, 7) else sa
                if fn in (60, 62, 63):
                    shift += 32
                bits = 64 if fn >= 56 else 32
                x = number(b) & ((1 << bits) - 1)
                if fn in (0, 4, 56, 60):
                    n = x << shift
                elif fn in (3, 7, 59, 63):
                    n = signed(x, bits) >> shift
                else:
                    n = x >> shift
                if bits == 32:
                    n = signed(n, 32)
                self.write_reg(rd, V(n & MASK, b.tags))
                return
            x, y = number(a), number(b)
            if fn in (33, 45):
                n = x + y
            elif fn in (35, 47):
                n = x - y
            elif fn == 36:
                n = x & y
            elif fn == 37:
                n = x | y
            elif fn == 38:
                n = x ^ y
            elif fn == 39:
                n = ~(x | y)
            elif fn in (42, 43):
                n = (signed(x, 64) < signed(y, 64) if fn == 42 else x < y)
                left = sorted(t for t in a.tags if t.startswith("id."))
                right = sorted(t for t in b.tags if t.startswith("id."))
                if left or right:
                    need(fn == 43 and left and right,
                         "rank_comparison_not_unsigned_pair")
                    self.rank.append({
                        "site": pc, "peer": self.active_peer,
                        "left": left, "right": right, "result": bool(n),
                    })
            else:
                raise Blocked("unsupported_special_operation")
            if fn in (33, 35):
                n = signed(n, 32)
            self.write_reg(rd, combine(n, a, b))
            return
        if op == 28:
            need(fn == 0x29 and sa == 0x12, "unsupported_mmi_operation")
            self.write_reg(rd, combine(number(a) | number(b), a, b))
            return
        if op in (8, 9, 24, 25, 10, 11, 12, 13, 14, 15):
            if op == 15:
                out = val(signed(imm << 16, 32))
            else:
                x = number(a)
                if op in (8, 9, 24, 25):
                    n = x + simm
                    if op in (8, 9):
                        n = signed(n, 32)
                elif op == 10:
                    n = signed(x, 64) < simm
                elif op == 11:
                    n = x < (simm & MASK)
                elif op == 12:
                    n = x & imm
                elif op == 13:
                    n = x | imm
                else:
                    n = x ^ imm
                out = V(n & MASK, a.tags)
            self.write_reg(rt, out)
            return
        loads = {32: (1, True), 33: (2, True), 35: (4, True),
                 36: (1, False), 37: (2, False), 39: (4, False),
                 55: (8, False), 30: (16, False)}
        stores = {40: 1, 41: 2, 43: 4, 63: 8, 31: 16}
        if op in loads:
            width, sign = loads[op]
            v = self.read((number(a) + simm) & MASK, width)
            if v.value is not None:
                n = signed(v.value, width * 8) if sign else v.value
                v = V(n & MASK, v.tags)
            self.write_reg(rt, v)
            return
        if op in stores:
            self.put((number(a) + simm) & MASK, stores[op], b)
            return
        if op == 49:
            self.f[rt] = number(self.read((number(a) + simm) & MASK, 4))
            return
        if op == 57:
            need(self.f[rt] is not None, "unknown_float_used")
            self.put((number(a) + simm) & MASK, 4, val(self.f[rt]))
            return
        if op == 17:
            fs, ft, fd = rd, rt, sa
            if rs == 0:
                need(self.f[fs] is not None, "unknown_float_used")
                self.write_reg(rt, val(signed(self.f[fs], 32)))
                return
            if rs == 4:
                self.f[fs] = number(b) & 0xFFFFFFFF
                return
            need(self.f[fs] is not None, "unknown_float_used")
            if rs == 20 and fn == 32:
                self.f[fd] = float_bits(float(signed(self.f[fs], 32)))
                return
            need(rs == 16, "unsupported_float_format")
            x = bits_float(self.f[fs])
            if fn == 6:
                self.f[fd] = self.f[fs]
                return
            need(self.f[ft] is not None, "unknown_float_used")
            y = bits_float(self.f[ft])
            if fn in (50, 52, 54, 60, 62):
                self.fcc = x == y if fn == 50 else x < y if fn in (52, 60) else x <= y
                return
            if fn == 0:
                self.f[fd] = float_bits(x + y)
                return
            raise Blocked("unsupported_float_operation")
        raise Blocked("unsupported_operation")

    def callback(self, pc, target):
        name = self.calls.get(target)
        need(name is not None, "unexpected_call")
        result = V()
        if name == "runtime":
            need(pc == self.runtime_site, "wrong_runtime_site")
            self.runtime_count += 1
            result = val(RUNTIME, "runtime")
        elif name == "clear":
            need(pc == self.clear_site, "wrong_clear_site")
            p = number(self.r[4])
            need(p in PEERS, "clear_argument_not_peer")
            i = PEERS.index(p)
            self.clears.append(i)
            if i == 0:
                need(self.epoch == 0, "first_peer_cleared_twice")
                self.epoch = 1
                self.put(SCRIPT, 4, val(self.selected_kind, "kind"))
        elif name == "memset":
            dst, byte, count = (number(self.r[i]) for i in (4, 5, 6))
            need(STACK - 0x1000 <= dst < STACK and byte == 0 and count == 16,
                 "unexpected_memset_contract")
            for off in range(0, 16, 4):
                self.put(dst + off, 4, val(0))
            result = val(dst)
        for i in list(range(1, 16)) + [24, 25]:
            self.r[i] = V(None, frozenset({"caller_clobbered"}))
        self.r[2] = result
        self.r[0] = val(0)
        # No floating state is modeled as surviving a call.
        self.f = [None] * 32
        self.fcc = None

    def step(self):
        pc = self.pc
        w = self.word(pc)
        if not self.is_control(w):
            self.data(pc)
            self.pc += 4
            return
        op, fn = w >> 26, w & 63
        rs, rt = (w >> 21) & 31, (w >> 16) & 31
        imm = signed(w & 0xFFFF, 16)
        taken, likely, target, call = True, False, None, False
        if op in (2, 3):
            target = direct_target(pc, w)
            call = op == 3
        elif op == 0:
            need(fn == 8, "indirect_call_not_supported")
            target = number(self.r[rs])
        elif op == 17:
            need(self.fcc is not None, "unknown_float_condition")
            taken = self.fcc == bool(rt & 1)
            likely = bool(rt & 2)
            target = pc + 4 + 4 * imm
        else:
            x = number(self.r[rs])
            if op in (4, 5, 20, 21):
                y = number(self.r[rt])
                taken = x == y if op in (4, 20) else x != y
                likely = op >= 20
            elif op in (6, 7, 22, 23):
                taken = signed(x, 64) <= 0 if op in (6, 22) else signed(x, 64) > 0
                likely = op >= 20
            elif op == 1:
                need(rt in (0, 1, 2, 3), "unsupported_regimm_branch")
                taken = signed(x, 64) < 0 if rt in (0, 2) else signed(x, 64) >= 0
                likely = rt in (2, 3)
            target = pc + 4 + 4 * imm
        if call:
            self.r[31] = val(pc + 8)
        if taken or not likely:
            self.data(pc + 4)
        if call:
            self.callback(pc, target)
            self.pc = pc + 8
        elif taken and target == RETURN:
            self.finished = True
        else:
            self.pc = target if taken else pc + 8

    def run(self):
        for _ in range(3000):
            if self.finished:
                return
            self.step()
        raise Blocked("execution_bound_exceeded")

def selftest():
    def enc(op, rs=0, rt=0, imm=0):
        return (op << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)
    def machine(words):
        m = VM(words, lambda a, n: None, {}, 0, 0, 0x10000, 0)
        m.r[1] = val(1)
        return m
    # Untaken BEQL annuls its delay slot; ordinary BEQ executes it.
    for op, expected in ((20, None), (4, 7)):
        words = [enc(op, 0, 1, 2), enc(9, 0, 2, 7),
                 enc(9, 0, 3, 9), (31 << 21) | 8, 0]
        m = machine(words)
        m.run()
        assert m.r[2].value == expected
        assert m.r[3].value == 9
    # Taken branch-likely executes its delay slot and skips fallthrough.
    words = [enc(20, 0, 0, 2), enc(9, 0, 2, 7),
             enc(9, 0, 3, 9), (31 << 21) | 8, 0]
    m = machine(words)
    m.run()
    assert m.r[2].value == 7 and m.r[3].value is None
    # Signed and unsigned register comparisons differ for -1 versus 1.
    for fn, expected in ((42, 1), (43, 0)):
        m = machine([(2 << 21) | (3 << 16) | (4 << 11) | fn])
        m.r[2], m.r[3] = val(-1), val(1)
        m.data(BASE)
        assert m.r[4].value == expected
    for op, expected in ((10, 1), (11, 0)):
        m = machine([enc(op, 2, 4, 1)])
        m.r[2] = val(-1)
        m.data(BASE)
        assert m.r[4].value == expected
    # Unknown branch operands must fail, never select a guessed successor.
    m = machine([enc(4, 2, 0, 1), 0, 0])
    try:
        m.step()
    except Blocked as exc:
        assert exc.args[0] == "unknown_value_used"
    else:
        raise AssertionError("unknown branch was accepted")
    # Modeled calls invalidate caller-clobbered integer and floating state.
    m = machine([0])
    m.calls[123] = "runtime"
    m.runtime_site = BASE
    m.r[8] = val(7)
    m.r[16] = val(11)
    m.f[0], m.f[31], m.fcc = 0, 0, True
    m.callback(BASE, 123)
    assert m.r[8].value is None and m.r[16].value == 11
    assert all(x is None for x in m.f) and m.fcc is None
    # Low subranges of an owned wider cell preserve little-endian values.
    m.put(STACK - 32, 8, val(0x1234567800000201))
    assert m.read(STACK - 32, 4).value == 0x201
    assert m.read(STACK - 28, 4).value == 0x12345678
    # A partial write cannot leave a stale wider cell available.
    m.put(STACK - 32, 4, val(4))
    assert m.read(STACK - 32, 4).value == 4
    assert (STACK - 32, 8) not in m.cells
    try:
        m.read(STACK - 28, 4)
    except Blocked as exc:
        assert exc.args[0] == "unmodeled_synthetic_memory"
    else:
        raise AssertionError("stale sibling subrange survived partial write")

    # EE ordered comparison encodings, including unordered NaN inputs.
    for fn in (52, 54, 60, 62):
        for x, y, expected in ((1.0, 2.0, True), (2.0, 1.0, False),
                               (float("nan"), 1.0, False)):
            m = machine([(17 << 26) | (16 << 21) | (1 << 16) | fn])
            m.f[0], m.f[1] = float_bits(x), float_bits(y)
            m.data(BASE)
            assert m.fcc == expected

def inspect(repo):
    sys.path.insert(0, str(repo / "tools"))
    import check_unit as cu
    data = (repo / cu.RETAIL["dds2"]).read_bytes()
    segments = cu.load_segments(data)
    def read_native(address, width):
        off = cu.va_to_off(segments, address)
        end = cu.va_to_off(segments, address + width - 1)
        if off is None or end != off + width - 1:
            return None
        return data[off:off + width]
    raw = read_native(BASE, SIZE)
    need(raw is not None and len(raw) == SIZE, "target_extent_unavailable")
    words = struct.unpack("<%dI" % (SIZE // 4), raw)
    syms = cu.symbols("dds2")
    runtime = cu.address("btlGetRuntime", syms)
    clear = cu.address("btlClearUnitDefeatCandidate", syms)
    memset = cu.address("memset", syms)
    cursor = cu.address("D_003BD7D0", syms)
    need(all(x is not None for x in (runtime, clear, cursor)),
         "required_symbols_unresolved")
    direct = [(BASE + 4 * i, direct_target(BASE + 4 * i, w))
              for i, w in enumerate(words) if w >> 26 == 3]
    need(len(direct) == 52, "static_call_count_changed")
    getters = [pc for pc, target in direct if target == runtime]
    need(len(getters) == 2, "runtime_call_structure_changed")
    runtime_site = getters[1]
    following = [(pc, target) for pc, target in direct if pc > runtime_site]
    need(following and following[0][1] == clear,
         "peer_clear_call_structure_changed")
    clear_site = following[0][0]
    calls = {runtime: "runtime", clear: "clear"}
    if memset is not None:
        calls[memset] = "memset"
    gp = int(cu.VERSIONS["dds2"]["gp"], 16)
    getter = (repo / "src/dds2/game/code_001A5BB8.c").read_text()
    import re
    need(re.search(r"\bs32\s+btlGetRuntime\s*\(\s*void\s*\)\s*"
                   r"\{\s*return\s+btlRuntime\s*;\s*\}", getter),
         "getter_contract_changed")
    rows = []
    shared_rank_site = None
    for kind in (34, 35, 36, 33, 37, -1, 0, 48):
        for phase in (0, 1):
            vm = VM(words, read_native, calls, runtime_site, clear_site,
                    cursor, gp, kind, phase)
            vm.run()
            need(vm.runtime_count == 1 and vm.second_seen,
                 "peer_fixture_not_reached")
            need(vm.clears.count(0) == 1, "seed_callback_not_unique")
            need(any(epoch == 1 and count == 1 for epoch, count in vm.kind_reads),
                 "fresh_inner_selector_not_observed")
            first = [r for r in vm.rank if r["peer"] == 0]
            second = [r for r in vm.rank if r["peer"] == 1]
            need(len(first) == 1 and len(second) <= 1,
                 "rank_site_not_unambiguous")
            need(first[0]["left"] == ["id.peer.0"]
                 and first[0]["right"] == ["id.reference.0"]
                 and first[0]["result"], "seed_rank_contract_changed")
            if shared_rank_site is None:
                shared_rank_site = first[0]["site"]
            need(first[0]["site"] == shared_rank_site,
                 "rank_site_changed_between_fixtures")
            if second:
                need(second[0]["site"] == shared_rank_site,
                     "second_rank_has_distinct_site")
                left, right = second[0]["left"], second[0]["right"]
                ids = set(left + right)
                if ids == {"id.peer.0", "id.reference.0"}:
                    source = "carried_previous_peer_definitions"
                elif ids == {"id.peer.1", "id.reference.1"}:
                    source = "fresh_second_peer_definitions"
                else:
                    source = "mixed_definition_epochs"
            else:
                left, right, source = [], [], "rank_test_bypassed"
            rows.append({
                "callback_kind": kind, "phase_count": phase,
                "rank_source": source, "left_roles": left, "right_roles": right,
                "phase_read_after_callback": 1 in vm.phase_reads,
                "second_peer_cleared": 1 in vm.clears,
            })
    class StatusObserved(Exception):
        pass

    class StatusWidthVM(VM):
        def read(self, address, width):
            if address == PEERS[0] + 0x110:
                need(width in (4, 8), "unexpected_status_width")
                self.observed_status_width = width
                raise StatusObserved()
            return super().read(address, width)

    widths = []
    for entry_kind in (34, 35, 36, 37, 38):
        getter_site = getters[0] if entry_kind in (37, 38) else getters[1]
        vm = StatusWidthVM(words, read_native, calls, getter_site, clear_site,
                           cursor, gp)
        vm.put(SCRIPT, 4, val(entry_kind, "kind"))
        try:
            vm.run()
        except StatusObserved:
            widths.append(dict(entry_kind=entry_kind,
                               first_peer_status_load_bytes=vm.observed_status_width))
        else:
            raise Blocked("status_load_not_reached")


    class FirstPeerCleared(Exception):
        pass

    class StatusReuseVM(VM):
        def __init__(self, *args, **kwargs):
            super().__init__(*args, **kwargs)
            self.status_reads = []

        def read(self, address, width):
            value = super().read(address, width)
            base = PEERS[0] + 0x110
            if address < base + 8 and base < address + width:
                need(base <= address and address + width <= base + 8,
                     "unexpected_status_overlap")
                self.status_reads.append({"offset": address - base, "bytes": width})
            return value

        def callback(self, pc, target):
            clearing = self.calls.get(target) == "clear"
            if clearing:
                need(number(self.r[4]) == PEERS[0],
                     "first_clear_argument_not_first_peer")
            super().callback(pc, target)
            if clearing:
                raise FirstPeerCleared()

    first_following = [(pc, target) for pc, target in direct if pc > getters[0]]
    need(first_following and first_following[0][1] == clear,
         "first_peer_clear_call_structure_changed")
    reuse = StatusReuseVM(words, read_native, calls, getters[0],
                          first_following[0][0], cursor, gp, selected_kind=37)
    reuse.put(SCRIPT, 4, val(37, "kind"))
    reuse.put(PEERS[0] + 0x110, 8, val(0x221))
    try:
        reuse.run()
    except FirstPeerCleared:
        pass
    else:
        raise Blocked("first_peer_clear_not_reached")
    need(reuse.runtime_count == 1 and reuse.clears == [0],
         "first_peer_clear_fixture_not_unique")
    need(reuse.status_reads
         and reuse.status_reads[0] == {"offset": 0, "bytes": 8},
         "first_peer_status_width_changed")


    class ReferenceStatusWidthVM(VM):
        def read(self, address, width):
            if address == REFERENCE + 0x110:
                self.observed_reference_status_width = width
                raise StatusObserved()
            return super().read(address, width)

    reference_widths = []
    for entry_kind in (17, 18, 27):
        vm = ReferenceStatusWidthVM(words, read_native, calls,
                                    runtime_site, clear_site, cursor, gp)
        vm.put(SCRIPT, 4, val(entry_kind, "kind"))
        try:
            vm.run()
        except StatusObserved:
            need(vm.runtime_count == 0 and not vm.clears,
                 "reference_status_after_unexpected_callback")
            reference_widths.append({
                "entry_kind": entry_kind,
                "first_reference_status_load_bytes":
                    vm.observed_reference_status_width,
            })
        else:
            raise Blocked("reference_status_load_not_reached")

    return {
        "native_reference_status_widths": reference_widths,
        "opcode37_status_reads_before_first_clear": reuse.status_reads,
        "status": "bounded_native_semantics_observed",
        "native_status_widths": widths,
        "selftests": "passed", "static_calls": 52, "runtime_sites": 2,
        "fixtures": rows,
        "limits": "Synthetic two-peer paths only; no source-match claim.",
    }

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--repo", type=Path, default=Path.cwd())
    parser.add_argument("--selftest", action="store_true")
    args = parser.parse_args()
    try:
        selftest()
        if args.selftest:
            print(json.dumps({"status": "selftests_passed"}))
        else:
            print(json.dumps(inspect(args.repo.resolve()), sort_keys=True))
        return 0
    except Blocked as exc:
        print(json.dumps({"status": "probe_blocked", "stage": exc.args[0]}))
        return 2
    except Exception:
        print(json.dumps({"status": "probe_blocked",
                          "stage": "unexpected_probe_failure"}))
        return 2

if __name__ == "__main__":
    sys.exit(main())
