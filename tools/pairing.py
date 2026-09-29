"""ELF, romwright-store and function-pairing helpers shared by the cross-build tools."""
import bisect
import json
import sqlite3
import struct
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
RETAIL = {"dds1": "orig/dds1/SLUS_209.74", "dds2": "orig/dds2/SLUS_211.52"}
STORE = ROOT / "build/romwright/project.sqlite"
# Opcodes whose low 16 bits are a relocatable immediate (lui, addiu, ori, loads/stores, lq/sq, ...).
IMM_OPS = {0x0F, 0x09, 0x0C, 0x0D, 0x19, 0x1E, 0x1F, 0x20, 0x21, 0x23, 0x24, 0x25, 0x27, 0x28,
           0x29, 0x2B, 0x2C, 0x2D, 0x31, 0x34, 0x35, 0x37, 0x39, 0x3C, 0x3D, 0x3F}
# Immediate users that combine with a preceding lui into an address (%lo).
LO_OPS = {0x09, 0x19, 0x1E, 0x1F, 0x20, 0x21, 0x23, 0x24, 0x25, 0x27, 0x28, 0x29, 0x2B, 0x31,
          0x35, 0x37, 0x39, 0x3D, 0x3F}

GPR_RT_WRITERS = {0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x18, 0x19, 0x1A, 0x1B, 0x1E,
                  0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x37}


def load_segments(elf):
    phoff = struct.unpack_from("<I", elf, 0x1C)[0]
    count = struct.unpack_from("<H", elf, 0x2C)[0]
    segs = []
    for i in range(count):
        p_type, p_off, p_vaddr, _, p_filesz = struct.unpack_from("<5I", elf, phoff + i * 32)
        if p_type == 1:
            segs.append((p_vaddr, p_off, p_filesz))
    return segs


def va_to_off(segs, va):
    for vaddr, off, size in segs:
        if vaddr <= va < vaddr + size:
            return off + va - vaddr
    return None


def off_to_va(segs, off):
    for vaddr, foff, size in segs:
        if foff <= off < foff + size:
            return vaddr + off - foff
    return None


def function_sizes(db, program):
    con = sqlite3.connect(db)
    pid = con.execute("select id from programs where name=?", (program,)).fetchone()[0]
    return {int(e, 16): s for e, s in con.execute(
        "select entry,size from functions where program_id=?", (pid,))}


def masked(code):
    words = []
    for i in range(0, len(code) - 3, 4):
        w = struct.unpack_from("<I", code, i)[0]
        op = w >> 26
        if op in (2, 3):
            w &= 0xFC000000
        elif op in IMM_OPS:
            w &= 0xFFFF0000
        words.append(w)
    return words


class Build:
    def __init__(self, elf, store, program):
        self.data = Path(elf).read_bytes()
        self.segs = load_segments(self.data)
        self.sizes = function_sizes(store, program)
        self.entries = sorted(self.sizes)

    def words(self, va):
        off = va_to_off(self.segs, va)
        return struct.unpack_from(f"<{self.sizes[va] // 4}I", self.data, off)

    def code(self, va):
        off = va_to_off(self.segs, va)
        return masked(self.data[off:off + self.sizes[va]])


def identical(a, fa, b, fb, min_size=16):
    """Same size and same code with relocations masked. Tiny functions (stubs, returns) match
    by chance, so callers without positional evidence keep the 16-byte floor."""
    size = a.sizes.get(fa)
    return size is not None and size >= min_size and b.sizes.get(fb) == size and a.code(fa) == b.code(fb)


def diff_pairs(path, a_is_target):
    """romwright diff JSON -> {a: b}."""
    out = {}
    for m in json.loads(Path(path).read_text())["matches"]:
        t, r = m["target"]["offset"], m["reference"]["offset"]
        a, b = (t, r) if a_is_target else (r, t)
        out[a] = b
    return out


def ordered_pairs(a, b, pairs):
    """Identical pairs kept in address order on both sides (longest increasing run), then
    same-count gaps between consecutive anchors paired by position when identical."""
    seq = sorted((x, y) for x, y in pairs.items() if identical(a, x, b, y))
    tails, tail_idx, prev = [], [], [-1] * len(seq)
    for i, (_, y) in enumerate(seq):
        j = bisect.bisect_left(tails, y)
        if j == len(tails):
            tails.append(y)
            tail_idx.append(i)
        else:
            tails[j] = y
            tail_idx[j] = i
        prev[i] = tail_idx[j - 1] if j else -1
    chain = []
    k = tail_idx[-1] if tail_idx else -1
    while k != -1:
        chain.append(seq[k])
        k = prev[k]
    chain.reverse()
    out = dict(chain)
    for (x0, y0), (x1, y1) in zip(chain, chain[1:]):
        ga = a.entries[bisect.bisect_right(a.entries, x0):bisect.bisect_left(a.entries, x1)]
        gb = b.entries[bisect.bisect_right(b.entries, y0):bisect.bisect_left(b.entries, y1)]
        if len(ga) == len(gb):
            for x, y in zip(ga, gb):
                if identical(a, x, b, y, min_size=4):
                    out[x] = y
    return out


def address_pairs(a, fa, b, fb, gp_a, gp_b):
    """Addresses the two identical functions reference at the same instruction: jal/j targets,
    lui/%lo pairs and $gp-relative accesses -> [(addr_a, addr_b)]."""
    wa, wb = a.words(fa), b.words(fb)
    out = []
    hi_a, hi_b = {}, {}
    for x, y in zip(wa, wb):
        op, rs, rt = x >> 26, (x >> 21) & 31, (x >> 16) & 31
        if op in (2, 3):  # jal, and j for tail calls
            out.append(((fa & 0xF0000000) | (x & 0x3FFFFFF) << 2, (fb & 0xF0000000) | (y & 0x3FFFFFF) << 2))
            continue
        if op == 0x0F:
            hi_a[rt], hi_b[rt] = (x & 0xFFFF) << 16, (y & 0xFFFF) << 16
            continue
        if op in LO_OPS:
            sa = (x & 0xFFFF) - 0x10000 if x & 0x8000 else x & 0xFFFF
            sb = (y & 0xFFFF) - 0x10000 if y & 0x8000 else y & 0xFFFF
            if rs == 28:
                out.append(((gp_a + sa) & 0xFFFFFFFF, (gp_b + sb) & 0xFFFFFFFF))
            elif rs in hi_a:
                out.append(((hi_a[rs] + sa) & 0xFFFFFFFF, (hi_b[rs] + sb) & 0xFFFFFFFF))
        # A write to a GPR ends any %hi pending in it (loads and ALU immediates write rt;
        # R-type and MMI write rd), except `addu/daddu rd, hi, index`, which keeps the symbol
        # base for a following %lo access. Stores, branches and FPU/COP loads write no GPR.
        if op in GPR_RT_WRITERS:
            hi_a.pop(rt, None)
            hi_b.pop(rt, None)
        elif op in (0x00, 0x1C):
            rd = (x >> 11) & 31
            base = rs if rs in hi_a else rt if rt in hi_a else None
            if op == 0 and x & 0x3F in (0x21, 0x2D) and base is not None:
                hi_a[rd], hi_b[rd] = hi_a[base], hi_b[base]
            else:
                hi_a.pop(rd, None)
                hi_b.pop(rd, None)
    return out
