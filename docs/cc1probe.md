# cc1probe: a read-only decision log of the real compiler

`tools/cc1probe.sh` runs the vendor ee-gcc 2.96 `cc1` under `gdb` and logs the
inputs of its own decisions: why the scheduler issued one instruction first,
which pseudo the allocator colored first and which register it chose, why CSE
replaced a register with a constant, why ifcvt hoisted an insn, which reload
register was picked, and which instruction reorg put in a delay slot. It is a
diagnostic for the [trace-justified rule](CONTRIBUTING.md): it shows which
property the compiler needed. It does not search for source, and nothing in
the build depends on it.

`tools/cc1probe_gdb.py` holds the hooks. They only read the compiler's memory:
each hook is a Python breakpoint whose `stop()` returns False.

Related tools: [`ee_gcc_probe.py` / `ee_gcc_observe.py`](compiler-live-observer.md)
(QEMU, 35 verified hooks, allocation and early operand choices) and the
[RTL diagnostics](compiler-diagnostics.md). cc1probe is the wider net (nine
decision families, run under `gdb`); use whichever answers the question. The
RTL dumps it keeps (`rtl.NN.*`) are the ordinary `-da` dumps.

## Requirements

- the downloaded compiler with SHA-256
  `d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`
  (`tools/download_tools.py`); the hook addresses are for exactly this binary
  and the script refuses any other;
- `gdb` with Python (tested with GDB 17.2); no other dependency;
- the pinned 32-bit loader exactly as for `tools/cc.sh` (`tools/glibc32` or
  `DDS_I386_LIBDIR`).

## Usage

```sh
tools/cc1probe.sh src/dds1/game/code_001411F0.c --func fldTitle --sched --alloc --cse --reorg
tools/cc1probe.sh /tmp/candidate.c --as-unit src/dds1/game/code_001411F0.c \
    --version dds1 --func fldTitle --all --out /tmp/fldTitle-candidate
```

| Option | Meaning |
| --- | --- |
| `--func NAME` | log only that function (always give it; a whole unit is 100k+ lines) |
| `--sched` | sched1/sched2: every `rank_for_schedule` comparison, the tier that decided it (priority, register weight, cost, luid), each issue (`PICK`), the ready list around SCE's `mips_sched_reorder` |
| `--alloc` | local-alloc quantity comparisons and `find_free_reg` choices; global-alloc `allocno_compare` and `find_reg` (conflicts, preferences, chosen register) |
| `--cse` | `cse_insn` per insn (`REWRITTEN` marks an insn CSE changed) and every `preferrable` cost comparison |
| `--reload` | `choose_reload_regs` per insn, the spill-register ring and `last_spill_reg`, the chosen reload registers, and `alter_reg` stack-slot assignment |
| `--rcse` | post-reload CSE: which insns `reload_cse_simplify_set` rewrote |
| `--ifcvt` | `noce_process_if_block` shapes, `noce_try_*` successes, `find_if_case_1/2`, `dead_or_predicable` hoists with the merged block's insns |
| `--jump` | cross-jumping: `find_cross_jump` pairs and matched runs, `do_cross_jump` |
| `--gcse` | PRE/cprop expression table, insert/delete verdicts, deletions and copies |
| `--reorg` | `estimate_probability` (the REG_BR_PROB note each conditional jump gets, with the rule that produced it: loop-exit 9001, pointer EQ/NE 3999/6001, opcode table, default 5000), then `mostly_true_jump` (shows the note value it read, or `none(fallback heuristics)`), `eligible_for_delay`, `fill_slots_from_thread`, `redundant_insn`, the simple and eager fill passes |
| `--loops` | the R5900 short-loop nop padding |
| `--all` | every mode |
| `--as-unit src/<v>/<dir>/<unit>.c` | compile a scratch file as that unit (names and per-unit cflags as the build; same as `DDS_AS_UNIT`) |
| `--version dds1\|dds2` | the INCLUDE_ASM tree (default `dds1`) |
| `--out DIR` | output directory (default `${TMPDIR:-/tmp}/cc1probe/<file>`; no spaces) |
| `--cflag FLAG` | extra cc1 flag, repeatable (for experiments such as `-fno-optimize-sibling-calls`) |
| `--maxlines N` | stop logging after N lines |
| `--nohook` | only the unhooked compile (assembly and `-da` dumps), no gdb |
| `--no-verify` | skip the byte comparison (the exit status then ignores it) |

Output in `OUT`: `decisions.log` (a header plus the decisions, one section per
pass such as `=== sched1 (pre-reload) function fldTitle ===`), `candidate.s`,
`rtl.NN.*` (the `-da` dumps of the hooked run) and `baseline/` (the unhooked
run). Instruction uids in the log are the uids in the `rtl.NN.*` dumps of the
same run; use them to find the insns a line talks about.

Sample lines (fldTitle):

```text
  rank A=uid66   B=uid68   prio 5/5 regw 1/1 cost 1/1  => +1 decided@luid luid A=11 B=12 (original order) -> A first
  qty_compare_1 q1{reg101 refs=4 life=[4,28]=24 ...} vs q2{reg226 refs=2 life=[8,54]=46 ...}  pri=3333/434 => -2899 decided@priority -> q1 allocated first
  allocno_compare a0{reg84 refs=9 live=12 ...} vs a1{reg85 refs=57 live=113 ...}  pri=22500/25221 => +2721 decided@priority -> a1 colored first
  fill_slots_from_thread@reorg.c:2367(insn=uid26 thread=uid66 ... likely=1 thread_if_true=1 ...)
      cse_insn uid480 REWRITTEN: insn (set (reg:SI 153) (ashift:SI (reg:SI 150) 7))
```

## It does not change the build, and says so itself

`tools/cc1probe.sh` compiles through `tools/cc.sh`, so the compiler path, pinned
loader, ASLR handling, per-unit cflags, canonical scratch names and cc1 argv are
cc.sh's, not a copy. It runs cc.sh from a symlink mirror of the repository
root under `$TMPDIR/.c1p/` (the mirror has the same path length as the
repository, because ee-gcc's output can depend on file-name lengths). The
mirror name starts with a hash of the repository path, so two clones whose
paths have equal length get separate mirrors, and links left by an earlier
run are re-pointed. cc.sh's scratch directories are therefore created in the
mirror, never in the working tree. If your `TMPDIR` is so long that the mirror cannot be as long as the
repository path, the script says so; set a shorter `TMPDIR` or
`CC1PROBE_MIRROR_BASE`.

Every run first compiles the same command **unhooked** (plain `tools/cc.sh`),
then again with cc1 under gdb, and compares the two byte for byte (assembly
and every `-da` dump, ignoring only the random scratch-directory names cc.sh
puts into `.file` and dump headers). The log header and the last stderr line
state the result:

```text
# cc1 sha256=d11ca9e2...  gdb-run exit=0  hooked-vs-unhooked outputs: OK
# log: .../decisions.log  (32658 lines)  verify: OK
```

The exit status is 0 only for `verify: OK`. A log whose header says anything
else must not be cited. The baseline assembly is also what `tools/cc.sh` gives
for the unit (checked for `fldTitle`'s unit `src/dds1/game/code_001411F0.c`).

## Citing a trace for the trace-justified rule

[CONTRIBUTING.md](CONTRIBUTING.md) accepts a form the trace says the compiler
needed when ordinary C provides it and every other matched function stays
exact. Record these four items in a comment or the landing notes:

1. **Command** and `verify: OK`: for example
   `tools/cc1probe.sh <unit> --func <f> --sched --out ...`, hooked == unhooked.
2. **Deciding pass and line**: the pass section and the log line, with the uids
   (for example "sched1: `rank A=uid213 B=uid403 ... decided@priority -> A first`").
3. **Property P** in one sentence about the compiler's state, not about a
   source line: "the const-3 set must issue before the `.z` store" or "`found`
   must be unknown to cse1 at `i = found`". The trace shows what the compiler
   needed, not which line the original had.
4. **The ordinary-C form** that gives the compiler P, why a person would write
   it, and the whole-unit `check_unit.py` result.

A trace is also useful when it rules a form out. A log that shows the deciding
tier (for example a `luid` tie that follows source order) says a different
source order can settle it; a log that shows the fold is in an earlier pass
says a later-pass change cannot.

## Reading notes

- sched1 order, not final order, decides reload's spill-ring walk: `reload`
  allocates in the stream sched1 produced (sched2 runs afterwards), so a
  different temp register can be a sched1 tie.
- REG_BR_PROB notes are emitted before reorg by `estimate_probability`. The
  `estimate_probability` section of a `--reorg` log shows each note when it is
  made; the `mostly_true_jump` lines show the value read at reorg time, which can
  differ because jump optimization may invert a branch (3999 after a 6001 on the
  same jump, for example). reorg maps the note to a prediction in
  `mostly_true_jump`: ≥9000 gives 2, ≥5000 gives 1, ≥1000 gives 0, below that
  -1. The prediction chooses which thread `fill_eager_delay_slots` searches
  first (1 or 2: the branch target first; 0 or -1: the fall-through first), and
  a prediction of 2 also enables extra compensation searches. It does not by
  itself decide the branch form: whether the slot is filled, and whether the
  branch becomes `beql`/`bnel` with an annulled slot, depends on the slot insn's
  safety against the other path (resources, trapping, liveness). So `likely=0`
  can still end as a branch-likely, and prediction 2 can end as an ordinary
  branch with a safe move in the slot. The `fill_slots_from_thread(...)` line
  and its `=> delay_list` name the donor insns but not the annul bit; the final
  assembly (or the RTL `/u` flag) is authoritative for the branch form. The
  pass-1 9001 note is the loop-exit rule: a cold block outside the
  loop's members, inside its first..last interval, whose predecessor is inside
  the loop. The emit-time reasons are reconstructed from the compiled code, so
  treat them as hints and check the note value itself.
- The hooks decode structures with layouts taken from the binary's STABS; the
  script asserts them on startup, so a different compiler build fails loudly
  rather than printing nonsense.
- A hooked run is slow (30 s for a mid-size unit with four modes). Restrict
  with `--func` and `--maxlines`.
