# cc1probe_gdb.py -- gdb-Python decision logger for the vendor ee-gcc 2.96-ee-001003-1 cc1.
#
# Loaded by cc1probe.sh through `gdb -x`.  It only READS inferior memory/registers; every
# hook is a Python breakpoint whose stop() returns False, so the compiler is never altered
# (cc1probe.sh verifies that by diffing the -o assembly + -da dumps against an unhooked run).
#
# Addresses are for the exact binary with sha256 d11ca9e2...c8f1 (checked by cc1probe.sh).
# Struct layouts come from the binary's own STABS (gdb `ptype`), asserted below.
import gdb
import os
import struct

LOG = open(os.environ["CC1PROBE_LOG"], "w", buffering=1)
MODES = set(filter(None, os.environ.get("CC1PROBE_MODES", "sched").split(",")))
ONLY_FUNC = os.environ.get("CC1PROBE_FUNC") or None
MAXRANK = int(os.environ.get("CC1PROBE_MAXLINES", "0"))   # 0 = unlimited

inf = gdb.selected_inferior()


def rd(a, n):
    return inf.read_memory(a, n).tobytes()


def u32(a):
    return struct.unpack("<I", rd(a, 4))[0]


def s32(a):
    return struct.unpack("<i", rd(a, 4))[0]


def s16(a):
    return struct.unpack("<h", rd(a, 2))[0]


def s64(a):
    return struct.unpack("<q", rd(a, 8))[0]


def reg(name):
    return int(gdb.selected_frame().read_register(name)) & 0xFFFFFFFF


def sreg(name):
    v = reg(name)
    return v - (1 << 32) if v & 0x80000000 else v


def arg(i):
    """i-th cdecl arg at a function-ENTRY breakpoint (frame not yet built)."""
    return u32(reg("esp") + 4 + 4 * i)


def sarg(i):
    v = arg(i)
    return v - (1 << 32) if v & 0x80000000 else v


def cstr(a, limit=200):
    if a == 0:
        return "(null)"
    out = b""
    while len(out) < limit:
        chunk = rd(a + len(out), 16)
        i = chunk.find(b"\0")
        if i >= 0:
            out += chunk[:i]
            break
        out += chunk
    return out.decode("latin1")


# ---- global addresses (symbols / STABS of this exact binary) -----------------------------
A = dict(
    h_i_d=0x824E25C, reload_completed=0x82714A4, last_scheduled_insn=0x824E2E8,
    clock_var=0x824E3F4, qty=0x8249550, qty_phys_sugg=0x8249558, qty_phys_num_sugg=0x8249560,
    qty_phys_copy_sugg=0x8249554, qty_phys_num_copy_sugg=0x824955C,
    allocno=0x82495F4, reg_allocno=0x82495F0, reg_n_info=0x8269FB0, reg_renumber=0x826E6B4,
    rtx_name=0x820CE90, rtx_format=0x820DE64, mode_name=0x820D720, reg_names=0x823E720,
    reg_class_names=0x8219ED8, current_function_decl=0x82536BC,
    expr_hash_table=0x8245CC0, expr_hash_table_size=0x8245CBC, pre_insert_map=0x8245DDC,
    pre_delete_map=0x8245DE0, edge_list=0x8245DE4,
)
HID_SZ = 40          # struct haifa_insn_data
QTY_SZ = 40          # struct qty
ALLOCNO_SZ = 100     # struct allocno


def check_layouts():
    """Assert the layout assumptions against the binary's own STABS (cheap, run once)."""
    exp = {
        "sizeof(struct haifa_insn_data)": HID_SZ, "sizeof(struct qty)": QTY_SZ,
        "sizeof(struct allocno)": ALLOCNO_SZ, "sizeof(rtunion)": 8,
        "(long)&((struct haifa_insn_data*)0)->reg_weight": 36,
        "(long)&((struct haifa_insn_data*)0)->luid": 8,
        "(long)&((struct haifa_insn_data*)0)->priority": 12,
        "(long)&((struct haifa_insn_data*)0)->cost": 32,
        "(long)&((struct qty*)0)->first_reg": 20, "(long)&((struct qty*)0)->mode": 32,
        "(long)&((struct allocno*)0)->live_length": 16,
        "(long)&((struct allocno*)0)->hard_reg_conflicts": 20,
        "(long)&((struct allocno*)0)->hard_reg_preferences": 36,
        "(long)&((struct allocno*)0)->hard_reg_copy_preferences": 52,
        "(long)&((struct allocno*)0)->hard_reg_full_preferences": 68,
        "(long)&((struct allocno*)0)->regs_someone_prefers": 84,
        "(long)&((struct rtvec_def*)0)->elem": 4,
        "(long)&((struct reg_info_def*)0)->range_copy": 0,   # bitfield: just must parse
    }
    for e, v in exp.items():
        try:
            got = int(gdb.parse_and_eval(e))
        except gdb.error:
            continue
        if "range_copy" in e:
            continue
        assert got == v, "layout drift: %s = %d, expected %d" % (e, got, v)
    for sym, addr in (("h_i_d", A["h_i_d"]), ("qty", A["qty"]), ("allocno", A["allocno"]),
                      ("reload_completed", A["reload_completed"]),
                      ("last_scheduled_insn", A["last_scheduled_insn"]),
                      ("reg_renumber", A["reg_renumber"]), ("clock_var", A["clock_var"])):
        got = int(gdb.parse_and_eval("(long)&%s" % sym))
        assert got == addr, "symbol %s at %#x, expected %#x" % (sym, got, addr)


# ---- output ------------------------------------------------------------------------------
NLINES = [0]


def out(s):
    NLINES[0] += 1
    if MAXRANK and NLINES[0] > MAXRANK:
        return
    LOG.write(s + "\n")


# ---- RTL pretty printer (reads inferior memory with the binary's own rtx tables) --------
_names = {}


def rtx_name(code):
    if code not in _names:
        _names[code] = cstr(u32(A["rtx_name"] + 4 * code))
    return _names[code]


_fmts = {}


def rtx_fmt(code):
    if code not in _fmts:
        _fmts[code] = cstr(u32(A["rtx_format"] + 4 * code))
    return _fmts[code]


def mode_str(m):
    return cstr(u32(A["mode_name"] + 4 * m)) if m else ""


def reg_name(n):
    return cstr(u32(A["reg_names"] + 4 * n)) if 0 <= n < 79 else "r%d" % n


def rtx(r, depth=0):
    if r == 0:
        return "0"
    if depth > 7:
        return "..."
    w = u32(r)
    code, mode = w & 0xFFFF, (w >> 16) & 0xFF
    name = rtx_name(code)
    ms = mode_str(mode)
    if name == "reg":
        n = u32(r + 4)
        return "(reg%s %s)" % (":" + ms if ms else "", reg_name(n) if n < 79 else "%d" % n)
    if name == "const_int":
        return "%d" % s64(r + 4)
    if name == "symbol_ref":
        return '"%s"' % cstr(u32(r + 4))
    if name in ("label_ref",):
        t = u32(r + 4)
        return "(label_ref %d)" % u32(t + 4)
    if name == "code_label":
        return "(code_label %d)" % u32(r + 4)
    if name == "const_double":
        return "(const_double%s)" % (":" + ms if ms else "")
    fmt = rtx_fmt(code)
    parts = []
    for i, ch in enumerate(fmt):
        fa = r + 4 + 8 * i
        if ch == "e":
            parts.append(rtx(u32(fa), depth + 1))
        elif ch == "E":
            v = u32(fa)
            if v == 0:
                parts.append("[]")
            else:
                n = s32(v)
                parts.append("[" + " ".join(rtx(u32(v + 4 + 4 * k), depth + 1) for k in range(n)) + "]")
        elif ch == "i":
            parts.append("%d" % s32(fa))
        elif ch == "w":
            parts.append("%d" % s64(fa))
        elif ch in "sS":
            parts.append('"%s"' % cstr(u32(fa)))
        elif ch == "u":
            t = u32(fa)
            parts.append("uid%d" % u32(t + 4) if t else "0")
    return "(%s%s %s)" % (name, ":" + ms if ms else "", " ".join(parts))


def insn_uid(i):
    return u32(i + 4) if i else 0


def insn_text(i):
    """INSN/JUMP_INSN/CALL_INSN: pattern at fld[3]."""
    if i == 0:
        return "(null)"
    code = u32(i) & 0xFFFF
    nm = rtx_name(code)
    t = ("%s %s" % (nm, rtx(u32(i + 4 + 8 * 3)))) if nm in ("insn", "jump_insn", "call_insn") else rtx(i)
    t = t.replace("\n", "\\n").replace("\t", " ")
    return t if len(t) <= 240 else t[:237] + "..."


# ---- haifa helpers -----------------------------------------------------------------------
def hid(uid):
    return u32(A["h_i_d"]) + HID_SZ * uid


def h_prio(u): return s32(hid(u) + 12)
def h_luid(u): return s32(hid(u) + 8)
def h_cost(u): return s16(hid(u) + 32)
def h_rw(u): return s16(hid(u) + 36)
def h_depend(u): return u32(hid(u))


def list_len(l):
    n = 0
    while l and n < 100000:
        n += 1
        l = u32(l + 12)
    return n


def dep_kind_of(last_uid, cand_insn):
    """kind of the INSN_DEPEND(last)->cand link: data / anti / output / none."""
    l = h_depend(last_uid)
    while l:
        if u32(l + 4) == cand_insn:
            k = (u32(l) >> 16) & 0xFF
            return {0: "data", 1: "anti", 2: "output"}.get(k, "kind%d" % k)
        l = u32(l + 12)
    return "none"


def cur_func():
    try:
        d = u32(A["current_function_decl"])
        n = u32(d + int(gdb.parse_and_eval("(long)&((union tree_node*)0)->decl.name")))
        return cstr(u32(n + int(gdb.parse_and_eval("(long)&((union tree_node*)0)->identifier.pointer"))))
    except Exception as e:     # noqa
        return "?"


STATE = dict(func="?", on=True, pass_name="", blk=-1, seen=set())


def wanted():
    return STATE["on"]


def begin_func(tag):
    STATE["func"] = cur_func()
    STATE["on"] = ONLY_FUNC is None or STATE["func"] == ONLY_FUNC
    STATE["seen"] = set()
    WTAB.clear()
    STATE["blk"] = -1
    STATE["pass_name"] = tag
    if ONLY_FUNC is not None:
        # with --func, breakpoints of the other hooks are only armed while the wanted function is compiled
        for h in HOOKS:
            if h.enabled != STATE["on"]:
                h.enabled = STATE["on"]
    if STATE["on"]:
        out("")
        out("=== %s  function %s ===" % (tag, STATE["func"]))


def tag():
    return STATE["pass_name"]


def is_regdest(x):
    """register_operand(SET_DEST, VOIDmode) for the cases that occur here: REG or SUBREG of REG."""
    if not x:
        return False
    nm = rtx_name(u32(x) & 0xFFFF)
    if nm == "reg":
        return True
    if nm == "subreg":
        return rtx_name(u32(u32(x + 4)) & 0xFFFF) == "reg"
    return False


def weight_expl(insn):
    """Re-derive find_insn_reg_weight (0x816D71C) for one insn:
       +1 per SET/CLOBBER whose destination is a register (top level or inside a PARALLEL),
       -1 per REG_DEAD / REG_UNUSED note.   Returns (weight, sets, [notes])."""
    sets = 0
    pat = u32(insn + 4 + 8 * 3)
    pn = rtx_name(u32(pat) & 0xFFFF)
    if pn in ("set", "clobber"):
        sets = 1 if is_regdest(u32(pat + 4)) else 0
    elif pn == "parallel":
        vec = u32(pat + 4)
        for k in range(s32(vec) if vec else 0):
            e = u32(vec + 4 + 4 * k)
            if rtx_name(u32(e) & 0xFFFF) in ("set", "clobber") and is_regdest(u32(e + 4)):
                sets += 1
    notes = []
    n = u32(insn + 4 + 8 * 6)
    while n:
        kind = (u32(n) >> 16) & 0xFF
        if kind in (1, 10):
            notes.append(("DEAD " if kind == 1 else "UNUSED ") + rtx(u32(n + 4)))
        n = u32(n + 12)
    return sets - len(notes), sets, notes


WTAB = {}


def hk_regweight_table():
    """At the epilogue of find_insn_reg_weight(b) (0x816D822, ebp still valid): re-derive and
    record each insn's weight WHILE its REG_DEAD/REG_UNUSED notes still exist (the scheduler
    strips them before ranking starts, so they cannot be read later)."""
    if not wanted():
        return
    ebp = reg("ebp")
    head, tail = u32(ebp - 8), u32(ebp - 4)
    stop = u32(tail + 4 + 8 * 2)          # NEXT_INSN(tail)
    i = head
    n = 0
    while i and i != stop and n < 100000:
        if rtx_name(u32(i) & 0xFFFF) in ("insn", "jump_insn", "call_insn"):
            uid = insn_uid(i)
            w, sets, notes = weight_expl(i)
            WTAB[uid] = (s16(hid(uid) + 36), w, sets, notes)
        i = u32(i + 4 + 8 * 2)
        n += 1


def show_insn(uid, insn):
    """print 'uidN = text' (+ the register-weight derivation) the first time a uid is seen."""
    key = (tag(), uid)
    if key in STATE["seen"]:
        return
    STATE["seen"].add(key)
    ex = ""
    if uid in WTAB:
        stored, w, sets, notes = WTAB[uid]
        ex = "   [regw=%d = %d reg-sets - %d dead/unused notes (%s)%s]" % (
            stored, sets, len(notes), ", ".join(notes) or "-",
            "" if w == stored else "; MODEL-MISMATCH model says %d" % w)
    out("    uid%-4d = %s%s" % (uid, insn_text(insn), ex))


_LINES = {}


def caller_line():
    """source line (from the binary's STABS line table) of the call site that invoked this function"""
    ret = u32(reg("esp"))
    if ret not in _LINES:
        try:
            if not (0x8049ab0 <= ret < 0x81e830c):
                _LINES[ret] = "libc"
            else:
                sal = gdb.find_pc_line(ret - 1)
                _LINES[ret] = "%s:%d" % (os.path.basename(sal.symtab.filename), sal.line) if sal.symtab else "%#x" % ret
        except Exception:
            _LINES[ret] = "%#x" % ret
    return _LINES[ret]


# ---- scheduler hooks ----------------------------------------------------------------------
RANK = dict(stage=0, a=0, b=0, ra=0, rb=0)
STAGE_NAME = ["priority", "reg_weight", "interblock_bb", "spec_val", "prob_val",
              "last_insn_class", "dependents", "luid"]


def hk_schedule_insns():
    rc = u32(A["reload_completed"])
    begin_func("sched2 (post-reload)" if rc else "sched1 (pre-reload)")


def hk_schedule_block():
    if not wanted():
        return
    STATE["blk"] = sarg(0)
    out("--- %s block bb=%d  n_insns=%d ---" % (tag(), sarg(0), sarg(1)))


def hk_rank_entry():
    if not wanted():
        return
    pa, pb = arg(0), arg(1)
    a, b = u32(pa), u32(pb)       # tmp2 = *x = A ; tmp = *y = B
    RANK.update(stage=0, a=a, b=b, via=caller_line())


def mk_stage(n):
    def f():
        if wanted() and RANK["stage"] < n:
            RANK["stage"] = n
    return f


def hk_rank_exit():
    if not wanted():
        return
    rc = sreg("eax")
    a, b, st = RANK["a"], RANK["b"], RANK["stage"]
    ua, ub = insn_uid(a), insn_uid(b)
    show_insn(ua, a)
    show_insn(ub, b)
    pa, pb = h_prio(ua), h_prio(ub)
    wa, wb = h_rw(ua), h_rw(ub)
    st_name = STAGE_NAME[st]
    detail = ""
    if st == 0:
        detail = "prio(A)-prio(B)"
    elif st == 1:
        detail = "regw(B)-regw(A)  (smaller reg_weight wins)"
    elif st == 5:
        last = u32(A["last_scheduled_insn"])
        lu = insn_uid(last)
        detail = "last_scheduled=uid%d dep(last->A)=%s dep(last->B)=%s" % (
            lu, dep_kind_of(lu, a), dep_kind_of(lu, b))
    elif st == 6:
        detail = "#dependents A=%d B=%d" % (list_len(h_depend(ua)), list_len(h_depend(ub)))
    elif st == 7:
        detail = "luid A=%d B=%d (original order)" % (h_luid(ua), h_luid(ub))
    # rank>0: A sorts after B => A is at the END of the ready list => issued FIRST.
    who = "A first" if rc > 0 else ("B first" if rc < 0 else "tie")
    out("  rank A=uid%-4d B=uid%-4d prio %d/%d regw %d/%d cost %d/%d  => %+d decided@%s %s -> %s  [via %s]"
        % (ua, ub, pa, pb, wa, wb, h_cost(ua), h_cost(ub), rc, st_name, detail, who, RANK.get("via")))


def ready_desc(base, n):
    items = []
    for k in range(n):
        i = u32(base + 4 * k)
        u = insn_uid(i)
        items.append("uid%d(p%d,w%d)" % (u, h_prio(u), h_rw(u)))
    return items


def hk_schedule_insn():
    """schedule_insn(insn, ready, n_ready, clock): the insn was just popped from the list end."""
    if not wanted():
        return
    insn, ready, n_ready, clock = arg(0), arg(1), sarg(2), sarg(3)
    u = insn_uid(insn)
    where = caller_line()
    show_insn(u, insn)
    rem = ready_desc(ready, max(0, min(n_ready, 64)))
    rem.reverse()      # best-first among the remaining
    out("  PICK uid%-4d (prio %d regw %d cost %d luid %d) clock=%d   still ready (next-best first): %s  [from %s]"
        % (u, h_prio(u), h_rw(u), h_cost(u), h_luid(u), clock, " ".join(rem) or "-", where))


MIPS_REORDER = []


def hk_reorder_entry():
    if not wanted():
        MIPS_REORDER.append(None)
        return
    ready, n = arg(2), sarg(3)
    before = [insn_uid(u32(ready + 4 * k)) for k in range(max(0, min(n, 64)))]
    MIPS_REORDER.append((ready, n, before, caller_line()))


def hk_reorder_exit():
    ent = MIPS_REORDER.pop() if MIPS_REORDER else None
    if not ent or not wanted():
        return
    ready, n, before, where = ent
    after = [insn_uid(u32(ready + 4 * k)) for k in range(len(before))]
    if len(before) >= 2 or after != before:
        out("  mips_sched_reorder ready list (last = issued first): %s -> %s"
            % (" ".join("uid%d" % u for u in before),
               "UNCHANGED" if after == before else "REORDERED " + " ".join("uid%d" % u for u in after))
            + "  [from %s]" % where)


# ---- local-alloc hooks --------------------------------------------------------------------
def qty_at(q):
    return A_qty() + QTY_SZ * q


def A_qty():
    return u32(A["qty"])


def qty_str(q):
    b = qty_at(q)
    n_refs, birth, death, size, calls = (s32(b), s32(b + 4), s32(b + 8), s32(b + 12), s32(b + 16))
    first_reg, cls, alt, mode = s32(b + 20), s32(b + 24), s32(b + 28), s32(b + 32)
    return ("q%d{reg%d refs=%d life=[%d,%d]=%d size=%d calls=%d class=%s mode=%s}"
            % (q, first_reg, n_refs, birth, death, death - birth, size, calls,
               cstr(u32(A["reg_class_names"] + 4 * cls)), mode_str(mode)))


QC = {}


def hk_local_alloc():
    begin_func("local-alloc")


def hk_block_alloc():
    if not wanted():
        return
    out("--- local-alloc block b=%d ---" % sarg(0))


def hk_qty_compare_1_entry():
    if not wanted():
        return
    p1, p2 = arg(0), arg(1)
    QC.update(q1=s32(p1), q2=s32(p2))


def hk_qty_cmp_pri_1():       # at 0x813ea3e: ecx = pri(q2), eax = pri(q1)
    if wanted():
        QC.update(pri1=sreg("eax"), pri2=sreg("ecx"))


def hk_qty_compare_1_exit():
    if not wanted():
        return
    q1, q2 = QC["q1"], QC["q2"]
    rc = sreg("eax")
    pri1, pri2 = QC.get("pri1"), QC.get("pri2")
    how = "priority" if (pri1 is not None and pri2 != pri1) else "qty-number (priorities equal)"
    out("  qty_compare_1 %s vs %s  pri=%s/%s  => %+d decided@%s -> %s"
        % (qty_str(q1), qty_str(q2), pri1, pri2, rc, how,
           "q%d allocated first" % (q1 if rc < 0 else q2) if rc else "tie"))
    QC["pri1"] = QC["pri2"] = None


def hk_qty_compare_entry():
    if not wanted():
        return
    QC.update(q1=sarg(0), q2=sarg(1), pri1=None, pri2=None)


def hk_qty_cmp_pri_0():       # 0x813e961: ebx = pri(q2), eax = pri(q1)
    if wanted():
        QC.update(pri1=sreg("eax"), pri2=sreg("ebx"))


def hk_qty_compare_exit():
    if not wanted():
        return
    q1, q2 = QC["q1"], QC["q2"]
    rc = sreg("eax")
    out("  qty_compare   %s vs %s  pri=%s/%s  => %+d" % (qty_str(q1), qty_str(q2),
                                                          QC.get("pri1"), QC.get("pri2"), rc))


def sugg_key(q):
    cn = s16(u32(A["qty_phys_num_copy_sugg"]) + 2 * q)
    if cn:
        return cn, "copy_sugg"
    return 79 * s16(u32(A["qty_phys_num_sugg"]) + 2 * q), "num_sugg*79"


def hk_sugg_entry(byptr):
    def f():
        if not wanted():
            return
        if byptr:
            QC.update(sq1=s32(arg(0)), sq2=s32(arg(1)))
        else:
            QC.update(sq1=sarg(0), sq2=sarg(1))
    return f


def hk_sugg_exit(name):
    def f():
        if not wanted():
            return
        q1, q2 = QC["sq1"], QC["sq2"]
        k1, w1 = sugg_key(q1)
        k2, w2 = sugg_key(q2)
        rc = sreg("eax")
        how = "suggestion count" if k1 != k2 else ("priority (suggestion keys tie)")
        out("  %s %s vs %s  suggestion-key %d(%s)/%d(%s) => %+d decided@%s"
            % (name, qty_str(q1), qty_str(q2), k1, w1, k2, w2, rc, how))
    return f


FFR = []


def hk_ffr_entry():
    if not wanted():
        FFR.append(None)
        return
    cls, mode, q, acc, just, born, dead = [sarg(i) for i in range(7)]
    FFR.append((cls, mode, q, acc, just, born, dead))


def hk_ffr_exit():
    ent = FFR.pop() if FFR else None
    if not ent or not wanted():
        return
    cls, mode, q, acc, just, born, dead = ent
    r = sreg("eax")
    b = qty_at(q)
    ns = s16(u32(A["qty_phys_num_sugg"]) + 2 * q)
    nc = s16(u32(A["qty_phys_num_copy_sugg"]) + 2 * q)
    sugg = regset(u32(A["qty_phys_sugg"]) + 16 * q)
    csugg = regset(u32(A["qty_phys_copy_sugg"]) + 16 * q)
    out("  find_free_reg %s class=%s mode=%s accept_call_clobbered=%d just_try_suggested=%d "
        "born=%d dead=%d\n      suggested(n=%d)=%s copy_suggested(n=%d)=%s => %s"
        % (qty_str(q), cstr(u32(A["reg_class_names"] + 4 * cls)), mode_str(mode), acc, just,
           born, dead, ns, sugg, nc, csugg, reg_name(r) if r >= 0 else "NONE(-1)"))


# ---- global-alloc hooks -------------------------------------------------------------------
AC = {}


def allocno_at(v):
    return u32(A["allocno"]) + ALLOCNO_SZ * v


def range_copy_flag(regno):
    ri = u32(u32(A["reg_n_info"]) + 16 + 4 * regno)       # varray data @+16 -> struct reg_info_def*
    return (rd(ri + 0x24, 1)[0] >> 1) & 1


def allocno_str(v):
    b = allocno_at(v)
    reg_, size, calls, refs, ll = [s32(b + 4 * k) for k in range(5)]
    return "a%d{reg%d refs=%d live=%d size=%d calls=%d range_copy=%d}" % (
        v, reg_, refs, ll, size, calls, range_copy_flag(reg_))


def hk_global_alloc():
    begin_func("global-alloc")


def hk_allocno_cmp_entry():
    if not wanted():
        return
    AC.update(v1=s32(arg(0)), v2=s32(arg(1)), pri1=None, pri2=None)


def hk_allocno_pri():         # 0x81410f5: ebx = pri(v1), eax = pri(v2)
    if wanted():
        AC.update(pri1=sreg("ebx"), pri2=sreg("eax"))


def hk_allocno_cmp_exit():
    if not wanted():
        return
    v1, v2 = AC["v1"], AC["v2"]
    rc = sreg("eax")
    p1, p2 = AC.get("pri1"), AC.get("pri2")
    if p1 is None:
        how = "range_copy flag (SCE live-range split)"
    elif p1 != p2:
        how = "priority"
    else:
        how = "allocno-number (priorities equal)"
    out("  allocno_compare %s vs %s  pri=%s/%s => %+d decided@%s -> %s"
        % (allocno_str(v1), allocno_str(v2), p1, p2, rc, how,
           "a%d colored first" % (v1 if rc < 0 else v2) if rc else "tie"))
    AC["pri1"] = AC["pri2"] = None


def regset(b):
    words = struct.unpack("<4I", rd(b, 16))
    regs = [reg_name(i) for i in range(79) if (words[i // 32] >> (i % 32)) & 1]
    return "{" + " ".join(regs) + "}"


GFR = []


def hk_gfind_entry():
    if not wanted():
        GFR.append(None)
        return
    num, losers, alt, acc, retry = arg(0), arg(1), sarg(2), sarg(3), sarg(4)     # num = ALLOCNO number
    b = allocno_at(num)
    GFR.append((s32(b),))
    out("  global find_reg(allocno a%d) alt_regs_p=%d accept_call_clobbered=%d retrying=%d  %s" % (
        num, alt, acc, retry, allocno_str(num)))
    out("      conflicts=%s" % regset(b + 20))
    out("      preferences=%s  copy_prefs=%s" % (regset(b + 36), regset(b + 52)))
    out("      full_prefs=%s  someone_prefers=%s  losers(reload)=%s" % (
        regset(b + 68), regset(b + 84), regset(losers) if losers else "{}"))


def hk_gfind_exit():
    ent = GFR.pop() if GFR else None
    if not ent or not wanted():
        return
    regno = ent[0]
    r = s16(u32(A["reg_renumber"]) + 2 * regno)
    out("      => reg_renumber[reg%d] = %s" % (regno, reg_name(r) if r >= 0 else "-1 (not allocated)"))



# ---- jump.c: cross-jumping / tail merging ---------------------------------------------------
CJ = []


def hk_jump_begin():
    begin_func("jump_optimize_1 (cross_jump=%d noop_moves=%d after_regscan=%d mark_labels_only=%d)"
               % (sarg(1), sarg(2), sarg(3), sarg(4)))


def hk_fcj_entry():
    if not wanted():
        CJ.append(None)
        return
    e1, e2, minimum, f1, f2 = arg(0), arg(1), sarg(2), arg(3), arg(4)
    CJ.append((e1, e2, minimum, f1, f2))


def hk_fcj_exit():
    """find_cross_jump is void in this (2000-trunk) jump.c: it reports a match through *f1/*f2 (both stay 0 if the
    common tail is shorter than `minimum` insns or is the whole of one block)."""
    ent = CJ.pop() if CJ else None
    if not ent or not wanted():
        return
    e1, e2, minimum, f1, f2 = ent
    r1, r2 = (u32(f1) if f1 else 0), (u32(f2) if f2 else 0)
    if r1:
        out("  find_cross_jump(e1=uid%d e2=uid%d minimum=%d) => MATCH: common tail starts at uid%d (side 1) / uid%d (side 2)"
            % (insn_uid(e1), insn_uid(e2), minimum, insn_uid(r1), insn_uid(r2)))
        out("      e1: %s" % insn_text(e1))
        out("      e2: %s" % insn_text(e2))
        out("      tail1 first insn uid%d: %s" % (insn_uid(r1), insn_text(r1)))
        out("      tail2 first insn uid%d: %s" % (insn_uid(r2), insn_text(r2)))
    else:
        out("  find_cross_jump(e1=uid%d e2=uid%d minimum=%d) => no usable common tail" % (insn_uid(e1), insn_uid(e2), minimum))


def hk_do_cross_jump():
    if not wanted():
        return
    insn, newj, newl = arg(0), arg(1), arg(2)
    out("  do_cross_jump: jump uid%d (%s) is merged into the code at uid%d; split label before uid%d: %s" % (
        insn_uid(insn), insn_text(insn), insn_uid(newl), insn_uid(newj), insn_text(newj)))


# ---- gcse.c: PRE / cprop --------------------------------------------------------------------
def sbit(sb, k):
    if not sb:
        return False
    n_bits = u32(sb)
    if k >= n_bits:
        return False
    word = struct.unpack("<Q", rd(sb + 12 + 8 * (k // 64), 8))[0]
    return bool((word >> (k % 64)) & 1)


def sb_nbits(sb):
    return u32(sb) if sb else 0


def expr_table():
    base = u32(A["expr_hash_table"])
    size = u32(A["expr_hash_table_size"])
    d = {}
    for i in range(min(size, 100000)):
        e = u32(base + 4 * i)
        while e:
            d[s32(e + 12)] = e
            e = u32(e + 16)
    return d


def occr_list(o):
    items = []
    while o and len(items) < 30:
        items.append("uid%d%s%s" % (insn_uid(u32(o + 4)), "(deleted)" if rd(o + 8, 1)[0] else "",
                                    "(copied)" if rd(o + 9, 1)[0] else ""))
        o = u32(o)
    return items


def hk_gcse_main():
    begin_func("gcse")


def hk_pre_pass():
    if wanted():
        out("--- gcse: one_pre_gcse_pass(pass=%d) ---" % sarg(0))


def hk_cprop_pass():
    if wanted():
        out("--- gcse: one_cprop_pass ---")


def hk_hoist_pass():
    if wanted():
        out("--- gcse: one_code_hoisting_pass ---")


def hk_pre_data_exit():
    """compute_pre_data has run pre_edge_lcm: dump the expression table and the LCM verdicts."""
    if not wanted():
        return
    tab = expr_table()
    ins = u32(A["pre_insert_map"])
    dele = u32(A["pre_delete_map"])
    el = u32(A["edge_list"])
    nedges = s32(el + 4) if el else 0
    out("  PRE expression table (%d expressions):" % len(tab))
    for k in sorted(tab):
        e = tab[k]
        out("    expr#%d %s" % (k, rtx(u32(e))))
        out("        antic occurrences: %s   avail occurrences: %s   reaching_reg: %s" % (
            " ".join(occr_list(u32(e + 20))) or "-", " ".join(occr_list(u32(e + 24))) or "-",
            rtx(u32(e + 28)) if u32(e + 28) else "(none yet)"))
    n_ins = n_del = 0
    for ei in range(nedges):
        sb = u32(ins + 4 * ei) if ins else 0
        for k in sorted(tab):
            if sbit(sb, k):
                edge = u32(u32(el + 8) + 4 * ei)
                src = u32(edge + 8)
                dst = u32(edge + 12)
                out("  LCM INSERT expr#%d on edge bb%s -> bb%s" % (
                    k, s32(src + 32) if src else "ENTRY", s32(dst + 32) if dst else "EXIT"))
                n_ins += 1
    nb = s32(A["n_basic_blocks"]) if False else int(gdb.parse_and_eval("n_basic_blocks"))
    for bi in range(nb):
        sb = u32(dele + 4 * bi) if dele else 0
        for k in sorted(tab):
            if sbit(sb, k):
                out("  LCM DELETE expr#%d (redundant) in bb%d" % (k, bi))
                n_del += 1
    out("  LCM verdict: %d insertion(s), %d deletion(s)" % (n_ins, n_del))


def hk_insert_end_bb():
    if not wanted():
        return
    expr, bb, pre = arg(0), arg(1), sarg(2)
    out("  insert_insn_end_bb(expr#%d %s, bb%d, pre=%d)" % (
        s32(expr + 12), rtx(u32(expr)), s32(bb + 32), pre))


def hk_insert_on_edge():
    if not wanted():
        return
    ret = u32(reg("esp"))
    if not (0x80F436C <= ret < 0x80F461E):      # only the call from pre_edge_insert
        return
    insn, edge = arg(0), arg(1)
    src, dst = u32(edge + 8), u32(edge + 12)
    out("  insert_insn_on_edge (PRE): bb%s -> bb%s : %s" % (
        s32(src + 32) if src else "ENTRY", s32(dst + 32) if dst else "EXIT", insn_text(insn)))


def hk_delete_insn_pre():
    if not wanted():
        return
    ret = u32(reg("esp"))
    if 0x80F48FC <= ret < 0x80F4BD0:
        w = "pre_delete"
    elif 0x80F4BD0 <= ret < 0x80F4CF4:
        w = "pre_gcse"
    else:
        return
    i = arg(0)
    out("  %s: delete_insn uid%d: %s   (replaced by copy from the PRE reaching register)" % (
        w, insn_uid(i), insn_text(i)))


def hk_validate_change():
    if not wanted():
        return
    ret = u32(reg("esp"))
    if not (0x80F48FC <= ret < 0x80F4BD0):      # only the call(s) from pre_delete
        return
    insn, loc, new = arg(0), arg(1), arg(2)
    old = u32(loc)
    out("  pre_delete: uid%d: SET_SRC %s replaced by PRE reaching register %s   (insn: %s)" % (
        insn_uid(insn), rtx(old), rtx(new), insn_text(insn)))


def hk_pre_copy():
    if not wanted():
        return
    e, i = arg(0), arg(1)
    out("  pre_insert_copy_insn(expr#%d, uid%d: %s)" % (s32(e + 12), insn_uid(i), insn_text(i)))


TR = []


def hk_try_replace_entry():
    if not wanted():
        TR.append(None)
        return
    ret = u32(reg("esp"))
    TR.append((arg(0), arg(1), arg(2), ret))


def hk_try_replace_exit():
    ent = TR.pop() if TR else None
    if not ent or not wanted():
        return
    frm, to, insn, ret = ent
    out("  cprop try_replace_reg(%s -> %s) in uid%d => %s" % (rtx(frm), rtx(to), insn_uid(insn),
                                                                 "replaced" if reg("eax") else "refused"))



# ---- post-reload CSE (reload_cse_regs: cselib based) ------------------------------------------
RCS = []


def hk_rcse_regs():
    begin_func("reload_cse_regs (post-reload cselib CSE)")


def hk_rcse_entry():
    if not wanted():
        RCS.append(None)
        return
    st, insn = arg(0), arg(1)
    RCS.append((st, insn, insn_text(insn)))


def hk_rcse_exit():
    ent = RCS.pop() if RCS else None
    if not ent or not wanted():
        return
    st, insn, before = ent
    after = insn_text(insn)
    r = sreg("eax")
    if after != before:
        out("  reload_cse_simplify_set uid%d: %s\n      ==> CHANGED to %s  (ret %d)" % (insn_uid(insn), before, after, r))
    else:
        out("  reload_cse_simplify_set uid%d: %s   (unchanged, ret %d)" % (insn_uid(insn), before, r))



RC = []


def hk_rtxcost_entry():
    if not wanted():
        RC.append(None)
        return
    ret = u32(reg("esp"))
    if 0x81602EC <= ret < 0x8160B5C:
        RC.append(("reload_cse_simplify_set", arg(0), sarg(1)))
    else:
        RC.append(None)


def hk_rtxcost_exit():
    ent = RC.pop() if RC else None
    if not ent or not wanted():
        return
    who, x, outer = ent
    out("      rtx_cost[%s](%s, outer_code=%s) = %d" % (who, rtx(x), rtx_name(outer), sreg("eax")))



RCO = []


def hk_rcso_entry():
    if not wanted():
        RCO.append(None)
        return
    insn = arg(0)
    RCO.append((insn, insn_text(insn)))


def hk_rcso_exit():
    ent = RCO.pop() if RCO else None
    if not ent or not wanted():
        return
    insn, before = ent
    after = insn_text(insn)
    r = sreg("eax")
    out("  reload_cse_simplify_operands uid%d: %s%s  (ret %d)" % (
        insn_uid(insn), before, ("\n      ==> CHANGED to " + after) if after != before else "   (unchanged)", r))



# ---- reload.c / reload1.c: spill-register and reload-register choice ---------------------------
RLT = ["INPUT", "OUTPUT", "INSN", "INPUT_ADDRESS", "INPADDR_ADDRESS", "OUTPUT_ADDRESS", "OUTADDR_ADDRESS",
       "OPERAND_ADDRESS", "OPADDR_ADDR", "OTHER", "OTHER_ADDRESS"]
RLD = 0x826E6E0          # struct reload rld[]  (76 bytes each)
RLD_SZ = 76
SPILL_REGS = 0x824AB80   # short spill_regs[]
N_SPILLS = 0x824A6FC
LAST_SPILL_REG = 0x824AC38
RELOAD_REG_USED_AT_ALL = 0x824B600


def spill_list():
    n = s32(N_SPILLS)
    return n, [s16(SPILL_REGS + 2 * k) for k in range(n)]


def rld_str(r):
    b = RLD + RLD_SZ * r
    inn, outn = u32(b), u32(b + 4)
    cls = cstr(u32(A["reg_class_names"] + 4 * s32(b + 8)))
    return "reload#%d in=%s out=%s class=%s mode=%s %s opnum=%d reg_rtx=%s" % (
        r, rtx(inn) if inn else "-", rtx(outn) if outn else "-", cls, mode_str(s32(b + 20)),
        RLT[s32(b + 68)] if 0 <= s32(b + 68) < len(RLT) else s32(b + 68), s32(b + 48),
        rtx(u32(b + 44)) if u32(b + 44) else "(none)")


CHR = []


def hk_choose_reload_regs():
    """one block per insn that needs reloads: the reloads, the spill-reg ring and last_spill_reg BEFORE choosing"""
    if not wanted():
        CHR.append(None)
        return
    chain = arg(0)
    insn = u32(chain + 16)
    n = s32(chain + 48)
    nsp, sp = spill_list()
    last = s32(LAST_SPILL_REG)
    out("  choose_reload_regs uid%d: %s" % (insn_uid(insn), insn_text(insn)))
    out("      spill_regs ring (n_spills=%d): %s   last_spill_reg(index)=%d  => next candidate index %d = %s" % (
        nsp, " ".join(reg_name(r) for r in sp), last, (last + 1) % nsp if nsp else -1,
        reg_name(sp[(last + 1) % nsp]) if nsp else "-"))
    for r in range(n):
        out("      " + rld_str(r))
    CHR.append((chain, insn, n))


def hk_choose_exit():
    ent = CHR.pop() if CHR else None
    if not ent or not wanted():
        return
    chain, insn, n = ent
    out("      => chosen: " + "; ".join(
        "reload#%d -> %s" % (r, rtx(u32(RLD + RLD_SZ * r + 44)) if u32(RLD + RLD_SZ * r + 44) else "(no reg)")
        for r in range(n)) + "   last_spill_reg(index) now %d" % s32(LAST_SPILL_REG))


ARR = []


def hk_alloc_reload_entry():
    if not wanted():
        ARR.append(None)
        return
    r, last_reload = sarg(1), sarg(2)
    ARR.append((r, s32(LAST_SPILL_REG)))


def hk_alloc_reload_exit():
    ent = ARR.pop() if ARR else None
    if not ent or not wanted():
        return
    r, last_before = ent
    ok = sreg("eax")
    b = RLD + RLD_SZ * r
    used = regset(RELOAD_REG_USED_AT_ALL)
    out("        allocate_reload_reg(reload#%d) start index %d => %s ; reload_reg_used_at_all=%s" % (
        r, last_before, ("reg %s (spill index now %d)" % (rtx(u32(b + 44)) if u32(b + 44) else "?", s32(LAST_SPILL_REG))) if ok else "FAILED",
        used))



# ---- R5900 short-loop erratum padding (SCE mips.c mips_r5900_lengthen_loops, called from machine_dependent_reorg2)
def hk_lengthen_loops():
    begin_func("mips_r5900_lengthen_loops (R5900 short-loop nop padding)")


def hk_loop_pad():
    if not wanted():
        return
    ret = u32(reg("esp"))
    if not (0x81CCE48 <= ret < 0x81CCFE8):
        return
    pat, insn = arg(0), arg(1)
    out("  R5900 short-loop erratum: nop padding emitted before uid%d: %s" % (insn_uid(insn), insn_text(insn)))



# ---- reload1.c alter_reg: which pseudo gets which stack slot, in which order --------------------
AR = []


def hk_alter_reg_entry():
    if not wanted():
        AR.append(None)
        return
    AR.append((sarg(0), sarg(1)))


def hk_alter_reg_exit():
    ent = AR.pop() if AR else None
    if not ent or not wanted():
        return
    i, frm = ent
    base = u32(int(gdb.parse_and_eval("(long)&reg_equiv_memory_loc")))
    m = u32(base + 4 * i) if base else 0
    if m:
        out("  alter_reg(pseudo %d, from_reg %d) -> stack slot %s" % (i, frm, rtx(m)))



# ---- ifcvt.c (the `ce` passes): which block shapes are converted / hoisted -----------------------
IFC = {}


def bb_insns(bb, limit=6):
    if not bb:
        return "(none)"
    head, end = u32(bb), u32(bb + 4)
    items = []
    i = head
    n = 0
    while i and n < limit:
        if rtx_name(u32(i) & 0xFFFF) in ("insn", "jump_insn", "call_insn"):
            items.append("uid%d: %s" % (insn_uid(i), insn_text(i)[:110]))
        if i == end:
            break
        i = u32(i + 4 + 8 * 2)
        n += 1
    return "bb%d [%s]" % (s32(bb + 32), "; ".join(items))


def hk_ce_begin():
    begin_func("ifcvt (if_convert)")


def hk_dop_entry():
    if not wanted():
        IFC.setdefault('dop', []).append(None)
        return
    test_bb, merge_bb, other_bb, new_dest, rev = arg(0), arg(1), arg(2), arg(3), sarg(4)
    IFC.setdefault('dop', []).append((test_bb, merge_bb, other_bb))
    out("  dead_or_predicable(test=%s | merge=%s | other=%s | reversep=%d)" % (
        "bb%d" % s32(test_bb + 32), bb_insns(merge_bb), "bb%d" % s32(other_bb + 32), rev))


def hk_dop_exit():
    st = IFC.get('dop') or []
    ent = st.pop() if st else None
    if ent and wanted():
        out("      => dead_or_predicable %s" % ("SUCCEEDED (insns hoisted into the test block)" if reg("eax") else "refused"))


def mk_noce_try(name):
    def f():
        if wanted() and reg("eax"):
            out("      %s succeeded" % name)
    return f


def hk_find_if_case(n):
    def entry():
        if wanted():
            IFC.setdefault('c%d' % n, []).append((arg(0), arg(1), arg(2)))
    def ex():
        st = IFC.get('c%d' % n) or []
        ent = st.pop() if st else None
        if ent and wanted():
            out("  find_if_case_%d(test=%s) => %s" % (n, "bb%d" % s32(ent[0] + 32), "converted" if reg("eax") else "no"))
    return entry, ex


def hk_noce_entry():
    if not wanted():
        IFC.setdefault('noce', []).append(None)
        return
    t, th, el, jn = arg(0), arg(1), arg(2), arg(3)
    IFC.setdefault('noce', []).append((t, th, el, jn))
    out("  noce_process_if_block(test=%s | then=%s | else=%s)" % (bb_insns(t, 3), bb_insns(th), bb_insns(el) if el else "(none)"))


def hk_noce_exit():
    st = IFC.get('noce') or []
    ent = st.pop() if st else None
    if ent and wanted():
        out("      => noce_process_if_block %s" % ("CONVERTED" if reg("eax") else "no"))


# ---- CSE ----------------------------------------------------------------------------------
PF = []


def hk_cse_main():
    begin_func("cse2 (after loop)" if arg(2) else "cse1")


CSEI = []


def hk_cse_insn():
    if not wanted():
        CSEI.append(None)
        return
    i = arg(0)
    t = insn_text(i)
    CSEI.append((i, t))
    out("  cse_insn uid%d: %s" % (insn_uid(i), t))


def hk_cse_insn_exit():
    ent = CSEI.pop() if CSEI else None
    if not ent or not wanted():
        return
    i, before = ent
    after = insn_text(i)
    if after != before:
        out("      cse_insn uid%d REWRITTEN: %s" % (insn_uid(i), after))


def hk_pref_entry():
    if not wanted():
        PF.append(None)
        return
    PF.append((sarg(0), sarg(1), sarg(2), sarg(3), caller_line()))


def hk_pref_exit():
    ent = PF.pop() if PF else None
    if not ent or not wanted():
        return
    ca, ra, cb, rb, where = ent
    f = lambda c, r: "none" if c == 0x7FFFFFFF else "cost=%d regcost=%d" % (c, r)
    rc = sreg("eax")
    out("    preferrable@%s(A: %s | B: %s) => %d (%s)" % (
        where, f(ca, ra), f(cb, rb), rc, "A preferred" if rc < 0 else "B preferred" if rc > 0 else "tie"))


# ---- reorg --------------------------------------------------------------------------------
MT = []


def hk_dbr():
    begin_func("reorg/dbr_schedule")



# ---- predict.c estimate_probability (REG_BR_PROB emission) + the note mostly_true_jump reads ----------
EST = {"tag": None, "on": False}
EST_TAGS = {
    0x81d3c0e: "chain-flag test false -> 0",
    0x81d3c1a: "chain-flag test true -> 10000",
    0x81d3cc0: "opcode switch, cond op1 != const0 -> 0",
    0x81d3cca: "opcode switch, cond op1 == const0 -> 10000",
    0x81d3c57: "POINTER heuristic: EQ with a REG_POINTER operand",
    0x81d3c90: "POINTER heuristic: NE with a REG_POINTER operand",
    0x81d3cdb: "opcode heuristic -> 3999",
    0x81d3cff: "opcode heuristic -> 6001",
    0x81d3d06: "no heuristic matched -> 5000",
}


def est_wanted():
    return ONLY_FUNC is None or cur_func() == ONLY_FUNC


def hk_est_entry():
    EST["on"] = est_wanted()
    EST["tag"] = None
    if EST["on"]:
        out("=== estimate_probability (REG_BR_PROB notes)  function %s ===" % cur_func())


def mk_est_tag(a):
    def f():
        if a in (0x81d3cdb, 0x81d3cff) and (EST["tag"] or "").startswith("POINTER"):
            EST["tag"] += " -> %s" % ("3999" if a == 0x81d3cdb else "6001")
            return
        EST["tag"] = EST_TAGS[a]
    f.__name__ = "hk_est_tag_%x" % a
    return f


def hk_est_reset():
    EST["tag"] = None


def hk_est_pass1():
    if not EST["on"]:
        return
    ji = reg("ebx")
    out("  estimate_probability pass1 uid%d: REG_BR_PROB=9001 [loop-exit rule, predict.c: cold non-member block inside the loop's first..last interval, pred source inside the natural loop nodes]" % insn_uid(ji))


def hk_est_emit():
    if not EST["on"]:
        return
    ji = u32(reg("ebp") - 0x10)
    out("  estimate_probability uid%d cond=%s: REG_BR_PROB=%d [%s]" % (
        insn_uid(ji), insn_text(ji)[:140], sreg("eax"), EST["tag"] or "?"))


def reg_br_prob(ji):
    """REG_BR_PROB value of a jump insn, or None. Reads are bounded and validated: rtx fields are 8 bytes wide
    in this binary (EXPR_LIST datum at +4, next at +12), and a note chain can hold INSN_LIST links."""
    try:
        if not ji or rtx_name(u32(ji) & 0xFFFF) not in ("insn", "jump_insn", "call_insn"):
            return None
        n = u32(ji + 0x34)
        guard = 0
        while n and guard < 64:
            if rtx_name(u32(n) & 0xFFFF) == "expr_list" and (u32(n) >> 16) & 0xFF == 0x10:
                c = u32(n + 4)
                if c and rtx_name(u32(c) & 0xFFFF) == "const_int":
                    return s32(c + 4)
                return None
            n = u32(n + 12)
            guard += 1
    except Exception:
        return None
    return None


def hk_mtj_entry():
    if not wanted():
        MT.append(None)
        return
    MT.append((arg(0), arg(1)))


def hk_mtj_exit():
    ent = MT.pop() if MT else None
    if not ent or not wanted():
        return
    ji, cond = ent
    v = sreg("eax")
    prob = reg_br_prob(ji)
    out("  mostly_true_jump uid%d cond=%s REG_BR_PROB=%s => %d (%s)" % (
        insn_uid(ji), rtx(cond), prob if prob is not None else "none(fallback heuristics)", v,
        {-1: "unlikely-taken(-1)", 0: "no-opinion(0)", 1: "weakly-taken(1)", 2: "strongly-taken(2)"}.get(v, "?")))


def hk_fill_simple():
    if wanted():
        out("  fill_simple_delay_slots(non_jumps_p=%d)" % sarg(0))


def hk_fill_eager():
    if wanted():
        out("  fill_eager_delay_slots()")


ELIG = []


def hk_elig_entry():
    if not wanted():
        ELIG.append(None)
        return
    ELIG.append((arg(0), sarg(1), arg(2), sarg(3)))


def hk_elig_exit():
    ent = ELIG.pop() if ELIG else None
    if not ent or not wanted():
        return
    d, slot, c, flags = ent
    out("    eligible_for_delay(delay_insn=uid%d slot=%d candidate=uid%d: %s flags=%#x) => %d" % (
        insn_uid(d), slot, insn_uid(c), insn_text(c), flags & 0xFFFFFFFF, sreg("eax")))



# ---- resource.c mark_target_live_regs (halcyon-drift): live set reorg computes at a thread ----
MTLR = []


def hk_mtlr_entry():
    if not wanted():
        MTLR.append(None)
        return
    MTLR.append((arg(1), arg(2), caller_line()))


def hk_mtlr_exit():
    ent = MTLR.pop() if MTLR else None
    if not ent or not wanted():
        return
    target, res, where = ent
    words = [u32(res + 4 + 4 * i) for i in range(3)]
    live = []
    for i in range(96):
        if (words[i // 32] >> (i % 32)) & 1:
            live.append(("$%d" % i) if i < 32 else ("$f%d" % (i - 32)) if i < 64 else ("hr%d" % i))
    out("    mark_target_live_regs@%s(target=uid%d) => mem=%d live={%s}" % (
        where, insn_uid(target) if target else -1, u32(res) & 0xFF, " ".join(live)))


FDSR = []


def hk_fdsr_entry():
    if not wanted():
        return
    out("      find_dead_or_set_registers(start=uid%d jump_count=%d)" % (insn_uid(arg(0)) if arg(0) else -1, sarg(3)))


CT = []


def hk_ct_entry():
    CT.append(arg(0) if wanted() else None)


def hk_ct_exit():
    i = CT.pop() if CT else None
    if i is None or not wanted():
        return
    if reg("eax"):
        out("      can_throw(uid%d: %s) => %d" % (insn_uid(i), insn_text(i)[:80], reg("eax")))


CR = []


def hk_cr_entry():
    if not wanted():
        CR.append(None)
        return
    def rn(x):
        try:
            return rtx(x)
        except Exception:
            return "?"
    CR.append((rn(arg(0)), rn(arg(1)), insn_uid(arg(4)) if arg(4) else -1, sarg(5)))


def hk_cr_exit():
    ent = CR.pop() if CR else None
    if not ent or not wanted():
        return
    out("    combine_regs(used=%s set=%s insn=uid%d already_dead=%d) => %d" % (ent + (reg("eax"),)))

FST = []


def hk_fst_entry():
    if not wanted():
        FST.append(None)
        return
    insn, cond, thread, opp, likely, tit, own, slots, pfilled, dl = (
        arg(0), arg(1), arg(2), arg(3), sarg(4), sarg(5), sarg(6), sarg(7), arg(8), arg(9))
    FST.append((insn,))
    out("  fill_slots_from_thread@%s(insn=uid%d thread=uid%d opposite=uid%d likely=%d thread_if_true=%d "
        "own_thread=%d slots_to_fill=%d slots_filled=%d)" % (
            caller_line(), insn_uid(insn), insn_uid(thread), insn_uid(opp), likely, tit, own, slots,
            s32(pfilled) if pfilled else -1))
    t = thread
    n = 0
    while t and n < 4:
        out("      thread[%d] uid%d: %s" % (n, insn_uid(t), insn_text(t)))
        t = u32(t + 4 + 8 * 2)
        n += 1


def hk_fst_exit():
    ent = FST.pop() if FST else None
    if not ent or not wanted():
        return
    dl = reg("eax")
    items = []
    while dl and len(items) < 8:
        items.append("uid%d" % insn_uid(u32(dl + 4)))
        dl = u32(dl + 12)
    out("      => delay_list = [%s]" % " ".join(items))


RED = []


def hk_red_entry():
    if not wanted():
        RED.append(None)
        return
    RED.append((arg(0), arg(1)))


def hk_red_exit():
    ent = RED.pop() if RED else None
    if not ent or not wanted():
        return
    i, tgt = ent
    r = reg("eax")
    out("    redundant_insn(uid%d: %s, target=uid%d) => %s" % (
        insn_uid(i), insn_text(i), insn_uid(tgt), ("uid%d" % insn_uid(r)) if r else "0"))


# ---- installation -------------------------------------------------------------------------
HOOKS = []
BEGIN_FNS = set()      # filled after the hk_* begin functions are defined


class Hook(gdb.Breakpoint):
    def __init__(self, addr, fn):
        gdb.Breakpoint.__init__(self, "*0x%x" % addr, internal=True)
        self.fn = fn
        if fn.__name__ not in BEGIN_FNS:
            HOOKS.append(self)

    def stop(self):
        try:
            self.fn()
        except Exception as e:      # never let a decoder bug alter or abort the compile
            LOG.write("!! hook %s failed: %r\n" % (self.fn.__name__, e))
        return False


def rets(sym):
    txt = gdb.execute("disassemble %s" % sym, to_string=True)
    return [int(l.split()[0], 16) for l in txt.splitlines() if "\tret" in l]


def rets_range(lo, hi):
    txt = gdb.execute("disassemble 0x%x,0x%x" % (lo, hi), to_string=True)
    return [int(l.split()[0], 16) for l in txt.splitlines() if "\tret" in l]


def sym(name):
    return int(gdb.parse_and_eval("(long)&%s" % name))


BEGIN_FNS.update(["hk_schedule_insns", "hk_local_alloc", "hk_global_alloc", "hk_cse_main", "hk_dbr", "hk_jump_begin", "hk_gcse_main", "hk_rcse_regs", "hk_lengthen_loops", "hk_ce_begin",
                 "hk_est_entry", "hk_est_reset", "hk_est_pass1", "hk_est_emit"] + ["hk_est_tag_%x" % a for a in EST_TAGS])  # estimate_probability runs before dbr: not gated by the per-function toggle


def install():
    check_layouts()
    if "sched" in MODES:
        Hook(0x817160C, hk_schedule_insns)                  # schedule_insns
        Hook(0x816D822, hk_regweight_table)                 # find_insn_reg_weight epilogue
        Hook(sym("schedule_block"), hk_schedule_block)
        Hook(0x816C6AC, hk_rank_entry)                      # rank_for_schedule
        for n, a in ((1, 0x816C702), (2, 0x816C723), (3, 0x816C784), (4, 0x816C7D1),
                     (5, 0x816C7F0), (6, 0x816CA55), (7, 0x816CAAE)):
            Hook(a, mk_stage(n))
        Hook(0x816CAC1, hk_rank_exit)
        Hook(0x816CACC, hk_schedule_insn)                   # schedule_insn
        Hook(sym("mips_sched_reorder"), hk_reorder_entry)
        for r in rets("mips_sched_reorder"):
            Hook(r, hk_reorder_exit)
    if "alloc" in MODES:
        Hook(sym("combine_regs"), hk_cr_entry)
        for r in rets("combine_regs"):
            Hook(r, hk_cr_exit)
        Hook(sym("local_alloc"), hk_local_alloc)
        Hook(sym("block_alloc"), hk_block_alloc)
        Hook(0x813E96C, hk_qty_compare_1_entry)
        Hook(0x813EA3E, hk_qty_cmp_pri_1)
        Hook(0x813EA52, hk_qty_compare_1_exit)
        Hook(0x813E89C, hk_qty_compare_entry)
        Hook(0x813E961, hk_qty_cmp_pri_0)
        for r in rets("qty_compare"):
            Hook(r, hk_qty_compare_exit)
        Hook(sym("qty_sugg_compare"), hk_sugg_entry(False))
        for r in rets("qty_sugg_compare"):
            Hook(r, hk_sugg_exit("qty_sugg_compare"))
        Hook(sym("qty_sugg_compare_1"), hk_sugg_entry(True))
        for r in rets("qty_sugg_compare_1"):
            Hook(r, hk_sugg_exit("qty_sugg_compare_1"))
        Hook(sym("find_free_reg"), hk_ffr_entry)
        Hook(0x813FBFD, hk_ffr_exit)
        Hook(sym("global_alloc"), hk_global_alloc)
        Hook(sym("allocno_compare"), hk_allocno_cmp_entry)
        Hook(0x81410F5, hk_allocno_pri)
        for r in rets("allocno_compare"):
            Hook(r, hk_allocno_cmp_exit)
        gf = 0x81424DC                                       # global.c find_reg
        Hook(gf, hk_gfind_entry)
        for r in rets_range(0x81424DC, 0x8143854):          # whole of global.c find_reg
            Hook(r, hk_gfind_exit)
    if "jump" in MODES:
        Hook(sym("jump_optimize_1"), hk_jump_begin)
        Hook(sym("find_cross_jump"), hk_fcj_entry)
        for r in rets("find_cross_jump"):
            Hook(r, hk_fcj_exit)
        Hook(sym("do_cross_jump"), hk_do_cross_jump)
    if "gcse" in MODES:
        Hook(sym("gcse_main"), hk_gcse_main)
        Hook(sym("one_pre_gcse_pass"), hk_pre_pass)
        Hook(sym("one_cprop_pass"), hk_cprop_pass)
        Hook(sym("one_code_hoisting_pass"), hk_hoist_pass)
        for r in rets("compute_pre_data"):
            Hook(r, hk_pre_data_exit)
        Hook(sym("insert_insn_end_bb"), hk_insert_end_bb)
        Hook(sym("insert_insn_on_edge"), hk_insert_on_edge)
        Hook(sym("delete_insn"), hk_delete_insn_pre)
        Hook(sym("pre_insert_copy_insn"), hk_pre_copy)
        Hook(sym("validate_change"), hk_validate_change)
        Hook(sym("try_replace_reg"), hk_try_replace_entry)
        for r in rets("try_replace_reg"):
            Hook(r, hk_try_replace_exit)
    if "rcse" in MODES:
        Hook(sym("reload_cse_regs"), hk_rcse_regs)
        Hook(sym("reload_cse_simplify_set"), hk_rcse_entry)
        for r in rets("reload_cse_simplify_set"):
            Hook(r, hk_rcse_exit)
        Hook(sym("reload_cse_simplify_operands"), hk_rcso_entry)
        for r in rets("reload_cse_simplify_operands"):
            Hook(r, hk_rcso_exit)
        Hook(sym("rtx_cost"), hk_rtxcost_entry)
        for r in rets("rtx_cost"):
            Hook(r, hk_rtxcost_exit)
    if "reload" in MODES:
        if "alloc" not in MODES:
            Hook(sym("global_alloc"), hk_global_alloc)         # function-begin marker
        Hook(sym("choose_reload_regs"), hk_choose_reload_regs)
        for r in rets("choose_reload_regs"):
            Hook(r, hk_choose_exit)
        Hook(sym("alter_reg"), hk_alter_reg_entry)
        for r in rets("alter_reg"):
            Hook(r, hk_alter_reg_exit)
        Hook(sym("allocate_reload_reg"), hk_alloc_reload_entry)
        for r in rets("allocate_reload_reg"):
            Hook(r, hk_alloc_reload_exit)
    if "loops" in MODES:
        Hook(sym("mips_r5900_lengthen_loops"), hk_lengthen_loops)
        Hook(sym("emit_insn_before"), hk_loop_pad)
    if "ifcvt" in MODES:
        Hook(sym("if_convert"), hk_ce_begin)
        Hook(sym("dead_or_predicable"), hk_dop_entry)
        for r in rets("dead_or_predicable"):
            Hook(r, hk_dop_exit)
        Hook(sym("noce_process_if_block"), hk_noce_entry)
        for r in rets("noce_process_if_block"):
            Hook(r, hk_noce_exit)
        for nm in ("noce_try_store_flag", "noce_try_cmove", "noce_try_store_flag_constants", "noce_try_store_flag_inc",
                   "noce_try_store_flag_mask", "noce_try_cmove_arith"):
            for r in rets(nm):
                Hook(r, mk_noce_try(nm))
        for n_, nm in ((1, "find_if_case_1"), (2, "find_if_case_2")):
            en, ex = hk_find_if_case(n_)
            Hook(sym(nm), en)
            for r in rets(nm):
                Hook(r, ex)
    if "cse" in MODES:
        Hook(sym("cse_main"), hk_cse_main)
        Hook(sym("cse_insn"), hk_cse_insn)
        for r in rets("cse_insn"):
            Hook(r, hk_cse_insn_exit)
        Hook(sym("preferrable"), hk_pref_entry)
        for r in rets("preferrable"):
            Hook(r, hk_pref_exit)
    if "reorg" in MODES:
        Hook(sym("estimate_probability"), hk_est_entry)
        Hook(0x81d3b08, hk_est_pass1)
        Hook(0x81d3bed, hk_est_reset)                       # pass 2: a condjump with a usable condition starts
        Hook(0x81d3d0b, hk_est_emit)
        for a in EST_TAGS:
            Hook(a, mk_est_tag(a))
        Hook(sym("dbr_schedule"), hk_dbr)
        Hook(sym("fill_simple_delay_slots"), hk_fill_simple)
        Hook(sym("fill_eager_delay_slots"), hk_fill_eager)
        Hook(sym("mostly_true_jump"), hk_mtj_entry)
        for r in rets("mostly_true_jump"):
            Hook(r, hk_mtj_exit)
        Hook(sym("eligible_for_delay"), hk_elig_entry)
        for r in rets("eligible_for_delay"):
            Hook(r, hk_elig_exit)
        Hook(sym("fill_slots_from_thread"), hk_fst_entry)
        Hook(sym("mark_target_live_regs"), hk_mtlr_entry)
        Hook(sym("find_dead_or_set_registers"), hk_fdsr_entry)
        Hook(sym("can_throw"), hk_ct_entry)
        for r in rets("can_throw"):
            Hook(r, hk_ct_exit)
        for r in rets("mark_target_live_regs"):
            Hook(r, hk_mtlr_exit)
        for r in rets("fill_slots_from_thread"):
            Hook(r, hk_fst_exit)
        Hook(sym("redundant_insn"), hk_red_entry)
        for r in rets("redundant_insn"):
            Hook(r, hk_red_exit)
    if ONLY_FUNC is not None:
        for h in HOOKS:
            h.enabled = False
    LOG.write("# cc1probe: hooks installed for modes: %s%s\n" % (
        ",".join(sorted(MODES)), (" func=" + ONLY_FUNC) if ONLY_FUNC else ""))
