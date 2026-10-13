#!/bin/bash
# cc1probe.sh <file.c> [--sched] [--alloc] [--cse] [--reorg] [--jump] [--gcse] [--rcse] [--reload]
#             [--ifcvt] [--loops] [--all] [--func NAME] [--as-unit src/<v>/<dir>/<unit>.c]
#             [--version dds1|dds2] [--out DIR] [--dumpbase PATH] [--cflag FLAG]... [--maxlines N]
#             [--no-verify] [--nohook] [--quiet]
#
# Read-only diagnostic. Runs the REAL, unmodified vendor cc1 on a C file under gdb, by way of
# tools/cc.sh (so the compiler, pinned i386 loader, ASLR handling, canonical scratch names,
# per-unit cflags and cc1 argv are exactly cc.sh's), with `-da` dumps, and logs the compiler's
# own decision inputs at chosen internal functions:
#   --sched  sched1/sched2: every rank_for_schedule comparison (uids, priorities, register
#            weights, which tie-break tier decided it, who issues first), each PICK, the ready
#            list before/after SCE's mips_sched_reorder
#   --alloc  local-alloc: qty_compare/qty_compare_1/qty_sugg_compare[_1] (priority terms),
#            find_free_reg (class/mode/suggestions -> hard reg); global-alloc: allocno_compare
#            (range_copy tier, priority), global find_reg (conflicts/preferences -> reg)
#   --cse    cse_insn per insn (REWRITTEN = the insn was changed) + preferrable() cost comparisons
#   --jump   jump.c cross-jumping/tail merging: find_cross_jump (insn pair, matched run), do_cross_jump
#   --gcse   gcse.c PRE/cprop: expression table, LCM insert/delete verdicts, PRE deletions + copies
#   --rcse   post-reload CSE (reload_cse_simplify_set): which set insns were rewritten
#   --reload choose_reload_regs/allocate_reload_reg: per-insn reloads, spill-reg ring,
#            last_spill_reg round-robin, chosen reload regs; alter_reg stack-slot assignment
#   --ifcvt  ifcvt.c (the `ce` passes): noce_process_if_block shapes, noce_try_* successes,
#            find_if_case_1/2, dead_or_predicable hoists
#   --loops  SCE mips_r5900_lengthen_loops: R5900 short-loop nop padding
#   --reorg  dbr_schedule: mostly_true_jump, eligible_for_delay, fill_slots_from_thread,
#            redundant_insn, fill_simple/eager_delay_slots
#   --all    every mode above.  --func NAME restricts the log to one function (strongly advised).
#
# Output: $OUT/decisions.log (header + decisions), $OUT/candidate.s, $OUT/rtl.NN.* (-da dumps);
# $OUT/baseline/ holds the unhooked run it is compared with. Default OUT is
# ${TMPDIR:-/tmp}/cc1probe/<file>. Exit status is 0 only if the hooked run's assembly and every
# -da dump are byte-identical to the unhooked run (the header and stderr line say `verify: OK`).
# Nothing in the repo, the vendor binary or the build is modified, and the build does not use this
# tool. Scratch trees live under $TMPDIR (see docs/cc1probe.md). Requires gdb with Python.
#
# The whole script lives in one function so bash has parsed all of it before running anything:
# editing this file while a run is in progress can no longer corrupt that run.
main() {
set -u
tools=$(cd "$(dirname "$0")" && pwd)
REPO=$(cd "$tools/.." && pwd)
export TMPDIR=${TMPDIR:-/tmp}
mkdir -p "$TMPDIR"

in= out= modes= func= as_unit= version=dds1 dumpbase= verify=1 maxlines=0 quiet=0 nohook=0
cflags=()
while [ $# -gt 0 ]; do
    case $1 in
        --sched) modes=$modes,sched ;;
        --alloc) modes=$modes,alloc ;;
        --cse) modes=$modes,cse ;;
        --reorg) modes=$modes,reorg ;;
        --jump) modes=$modes,jump ;;
        --gcse) modes=$modes,gcse ;;
        --rcse) modes=$modes,rcse ;;
        --reload) modes=$modes,reload ;;
        --loops) modes=$modes,loops ;;
        --ifcvt) modes=$modes,ifcvt ;;
        --all) modes=sched,alloc,cse,reorg,jump,gcse,rcse,reload,loops,ifcvt ;;
        --func) func=$2; shift ;;
        --as-unit) as_unit=$2; shift ;;
        --version) version=$2; shift ;;
        --out) out=$2; shift ;;
        --dumpbase) dumpbase=$2; shift ;;
        --cflag) cflags+=("$2"); shift ;;
        --maxlines) maxlines=$2; shift ;;
        --no-verify) verify=0 ;;
        --nohook) nohook=1 ;;
        --quiet) quiet=1 ;;
        -h|--help) sed -n 2,40p "$0"; exit 0 ;;
        -*) echo "unknown option $1" >&2; exit 2 ;;
        *) in=$1 ;;
    esac
    shift
done
[ -n "$in" ] && [ -f "$in" ] || { echo "usage: $0 <file.c> [--sched] [--alloc] [--cse] [--reorg] ... (see -h)" >&2; exit 2; }
[ -n "$modes" ] || modes=,sched
modes=${modes#,}
case $in in /*) ;; *) in=$PWD/$in ;; esac

cc1="$REPO/tools/compilers/ee-gcc2.96/lib/gcc-lib/ee/2.96-ee-001003-1/cc1"
want=d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1
[ -f "$cc1" ] || { echo "no compiler at $cc1 (run tools/download_tools.py)" >&2; exit 3; }
[ "$(sha256sum "$cc1" | cut -d' ' -f1)" = "$want" ] || { echo "cc1 sha256 mismatch: the probe addresses are for $want" >&2; exit 3; }
command -v gdb >/dev/null || { echo "gdb (with Python) is required" >&2; exit 3; }

# The loader cc.sh will use (DDS_I386_LIBDIR, else tools/glibc32). Only needed so the hooked run can
# start the same loader under gdb; the unhooked baseline is plain tools/cc.sh.
real_libdir=${DDS_I386_LIBDIR:-}
[ -z "$real_libdir" ] && [ -f "$REPO/tools/glibc32/libc.so.6" ] && real_libdir="$REPO/tools/glibc32"

[ -n "$out" ] || out=$TMPDIR/cc1probe/$(basename "${in%.c}")
mkdir -p "$out"; out=$(cd "$out" && pwd)
case $out$dumpbase in *[[:space:]]*) echo "output path must not contain spaces: $out" >&2; exit 2 ;; esac
[ -n "$dumpbase" ] || dumpbase=$out/rtl
rm -rf "$out/baseline" "$out/loader"; mkdir -p "$out/baseline"
rm -f "$out"/rtl.[0-9][0-9].* "$out/candidate.s"

# ---- mirror of the repo root with the SAME absolute path length: tools/cc.sh runs with cwd=<root>
# and cc1's output can depend on file-name lengths through heap layout. cc.sh is run from inside the
# mirror (its root is where it is found), so it writes its scratch dirs there and never in the repo.
want_len=${#REPO}
base=${CC1PROBE_MIRROR_BASE:-$TMPDIR/.c1p/}
pad=$((want_len - ${#base}))
[ $pad -ge 1 ] || { echo "cannot build a mirror path as long as $REPO under $base; set TMPDIR to a shorter directory or CC1PROBE_MIRROR_BASE (a path ending in / that is shorter than $want_len characters)" >&2; exit 3; }
# Key the mirror by the repo path as well as its length, so clones with equal-length paths never
# share a mirror; re-point any stale link left by an earlier run.
key=$(printf '%s' "$REPO" | sha1sum | cut -d' ' -f1)
if [ $pad -gt ${#key} ]; then
    mname=$key$(printf 'm%.0s' $(seq 1 $((pad - ${#key}))))
else
    mname=${key:0:$pad}
fi
mir=$base$mname
mkdir -p "$mir"
for d in tools include src config build; do
    [ -e "$REPO/$d" ] || continue
    if [ -L "$mir/$d" ]; then
        [ "$(readlink "$mir/$d")" = "$REPO/$d" ] || ln -sfn "$REPO/$d" "$mir/$d"
    elif [ ! -e "$mir/$d" ]; then
        ln -s "$REPO/$d" "$mir/$d"
    else
        echo "mirror entry $mir/$d is not a link; remove it or set CC1PROBE_MIRROR_BASE" >&2; exit 3
    fi
done

canon=${as_unit:-}
case $in in "$REPO"/src/*/*.c) [ -z "$canon" ] && canon=${in#"$REPO"/} ;; esac
[ -n "$canon" ] || canon=src/$version/game/$(basename "$in")

# One compile of $in through tools/cc.sh (inside the mirror). $1 = where to keep the assembly.
run_cc() {
    ( cd "$mir" && DDS_VERSION=$version DDS_AS_UNIT=$canon DDS_KEEP_S=$1 \
        tools/cc.sh -da -dumpbase "$dumpbase" "${cflags[@]}" "$in" -o "$2" )
}
same() {  # byte compare, ignoring the random scratch-dir names tools/cc.sh puts in `.file` and dump headers
    cmp -s <(sed -E 's#\.[0-9a-f]{2,4}/#./#g' "$1") <(sed -E 's#\.[0-9a-f]{2,4}/#./#g' "$2")
}
collect() {  # collect <dest>: move the dumps there (unless dest holds them already)
    for f in "$dumpbase".[0-9][0-9].*; do
        [ -e "$f" ] || continue
        [ "$(dirname "$f")" = "$1" ] || mv "$f" "$1/"
    done
}

# ---- 1) unhooked baseline: tools/cc.sh itself
run_cc "$out/baseline/candidate.s" "$out/baseline/cc.o" >"$out/baseline/cc.stdout" 2>"$out/baseline/cc.stderr" </dev/null
base_rc=$?
collect "$out/baseline"
[ $base_rc -eq 0 ] || { echo "baseline tools/cc.sh failed rc=$base_rc"; sed -n 1,20p "$out/baseline/cc.stderr"; exit 4; }

if [ "$nohook" = 1 ]; then   # plain compile only: assembly + dumps copied to $out, no gdb
    cp "$out"/baseline/candidate.s "$out"/baseline/rtl.[0-9][0-9].* "$out"/ 2>/dev/null
    echo "# nohook: $out/candidate.s" >&2; exit 0
fi

# ---- 2) hooked run: the same tools/cc.sh command, with DDS_I386_LIBDIR pointing at a stub "loader" that
# starts the real one under gdb for cc1 (and just execs it for ee-as).
cat >"$out/probe.gdb" <<EOF
set pagination off
set confirm off
# --args quotes cc.sh's multiword -D values; the shell must interpret those quotes.
set startup-with-shell on
set disable-randomization on
unset environment LINES
unset environment COLUMNS
add-symbol-file "$cc1" -o 0
starti
thbreak *0x8078158
continue
python
import sys, os
sys.dont_write_bytecode = True
sys.path.insert(0, "$tools")
import cc1probe_gdb as P
P.install()
gdb.events.exited.connect(lambda e: open(os.environ["CC1PROBE_RC"], "w").write(str(getattr(e, "exit_code", "?"))))
end
continue
EOF
if [ -n "$real_libdir" ]; then
    real_prefix="\"$real_libdir/ld-linux.so.2\" --library-path \"$real_libdir\""
else
    real_prefix=
fi
mkdir -p "$out/loader"
cat >"$out/loader/ld-linux.so.2" <<EOF
#!/bin/sh
# argv from tools/cc.sh: --library-path <this dir> <program> args...
shift 2
prog=\$1; shift
case \$prog in
    */cc1) exec gdb -batch -nx -q -x "$out/probe.gdb" --args $real_prefix "\$prog" "\$@" >"$out/gdb.stdout" 2>"$out/gdb.stderr" </dev/null ;;
    *) exec $real_prefix "\$prog" "\$@" ;;
esac
EOF
chmod +x "$out/loader/ld-linux.so.2"
export CC1PROBE_LOG="$out/decisions.raw" CC1PROBE_RC="$out/gdb.rc" CC1PROBE_MODES=$modes
export CC1PROBE_FUNC=$func CC1PROBE_MAXLINES=$maxlines
rm -f "$CC1PROBE_RC"
DDS_I386_LIBDIR="$out/loader" run_cc "$out/candidate.s" "$out/cc.o" >"$out/cc.stdout" 2>"$out/cc.stderr" </dev/null
hooked_rc=$?
collect "$out"
rc=$(cat "$CC1PROBE_RC" 2>/dev/null || echo none)
rm -rf "$out/loader"

status=OK
if [ "$verify" = 1 ]; then
    if [ $hooked_rc -ne 0 ]; then status="HOOKED tools/cc.sh EXIT CODE $hooked_rc (see $out/cc.stderr, $out/gdb.stderr)"; fi
    if [ "$rc" != "0" ] && [ "$rc" != "00" ] && [ "$rc" != "none" ]; then status="HOOKED RUN EXIT CODE $rc"; fi
    for f in "$out/baseline"/candidate.s "$out/baseline"/rtl.[0-9][0-9].*; do
        b=$(basename "$f")
        same "$f" "$out/$b" || { status="OUTPUT DIFFERS from unhooked run: $b"; break; }
    done
    [ "$rc" = none ] && status="gdb did not report exit (see $out/gdb.stderr)"
fi
{
    echo "# cc1probe  file=$in  unit=$canon  modes=$modes${func:+  func=$func}"
    echo "# cc1 sha256=$want  gdb-run exit=$rc  hooked-vs-unhooked outputs: $status"
    echo "# dumps: $dumpbase.NN.*   assembly: $out/candidate.s"
} >"$out/decisions.hdr"
cat "$out/decisions.hdr" "$out/decisions.raw" >"$out/decisions.log"
[ "$quiet" = 1 ] || cat "$out/decisions.log"
echo "# log: $out/decisions.log  ($(wc -l <"$out/decisions.raw") lines)  verify: $status" >&2
[ "$status" = OK ] || [ "$verify" = 0 ]
}
main "$@"
exit $?
