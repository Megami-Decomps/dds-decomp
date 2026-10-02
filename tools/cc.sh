#!/bin/sh
# Compile one C file exactly like the build does (ee-gcc 2.96 cc1 -O2 -> ee-as),
# for the permuter and quick experiments:
#   tools/cc.sh [extra cc1 flags] in.c -o out.o
# DDS_VERSION selects the INCLUDE_ASM tree (default dds1); DDS_I386_LIBDIR as
# for configure.py when the system has no 32-bit loader.
set -e
root=$(cd "$(dirname "$0")/.." && pwd)
in= out= flags=
while [ $# -gt 0 ]; do
    case $1 in
        -o) out=$2; shift 2 ;;
        *.c) in=$1; shift ;;
        *) flags="$flags $1"; shift ;;
    esac
done
[ -n "$in" ] && [ -n "$out" ] || { echo "usage: $0 [flags] in.c -o out.o" >&2; exit 2; }
case $in in /*) ;; *) in="$PWD/$in" ;; esac
case $out in /*) ;; *) out="$PWD/$out" ;; esac
version=${DDS_VERSION:-dds1}
ee="$root/tools/compilers/ee-gcc2.96"
cc1="$ee/lib/gcc-lib/ee/2.96-ee-001003-1/cc1"
# Pinned 32-bit glibc (tools/download_tools.py) unless overridden.
[ -z "$DDS_I386_LIBDIR" ] && [ -f "$root/tools/glibc32/libc.so.6" ] && DDS_I386_LIBDIR="$root/tools/glibc32"
# download_tools.py gives GCC's garbage collector a deterministic arena under
# normal ASLR. Keep setarch as a fallback for a manually installed compiler.
aslr=
patched_sha=d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1
if ! grep -qx "$patched_sha" "$cc1.aslr-fixed" 2>/dev/null; then
    [ -z "$DDS_ALLOW_ASLR" ] && command -v setarch >/dev/null && aslr="setarch $(uname -m) -R"
fi
run() {
    if [ -n "$DDS_I386_LIBDIR" ]; then
        $aslr "$DDS_I386_LIBDIR/ld-linux.so.2" --library-path "$DDS_I386_LIBDIR" "$@"
    else
        $aslr "$@"
    fi
}
tmp=$(mktemp -d)
cleanup() { rm -rf "$tmp" ${srcdir:+"$root/$srcdir"} ${outdir:+"$root/$outdir"}; }
trap cleanup EXIT
cd "$root"
# ee-gcc 2.96's code can depend on the lengths of the file names cc1 is given
# (they shift its heap, and CSE hashes heap addresses). Compile a unit under
# names exactly as long as the build's: `src/<v>/<dir>/<unit>.c` in and
# `build/<v>/src/<v>/<dir>/<unit>.o.s` out, in scratch directories named
# `.xx`/`.xxxx` so the lengths match. DDS_AS_UNIT=src/... compiles another
# file (an experiment) as if it were that unit.
canon=${DDS_AS_UNIT:-}
case $in in "$root"/src/*/*.c) [ -z "$canon" ] && canon=${in#"$root"/} ;; esac
cc_in=$in
cc_out=$tmp/out.s
if [ -n "$canon" ]; then
    rest=${canon#src/}
    while :; do srcdir=.$(od -An -N1 -tx1 /dev/urandom | tr -d ' \n'); mkdir "$srcdir" 2>/dev/null && break; done
    while :; do outdir=.$(od -An -N2 -tx1 /dev/urandom | tr -d ' \n'); mkdir "$outdir" 2>/dev/null && break; done
    mkdir -p "$srcdir/$(dirname "$rest")" "$outdir/$version/src/$(dirname "$rest")"
    cp "$in" "$srcdir/$rest"
    cc_in=$srcdir/$rest
    cc_out=$outdir/$version/src/${rest%.c}.o.s
    # Per-unit flags of the original build (config/<v>/cflags.txt), as configure.py uses.
    rel=${rest#*/}; rel=${rel%.c}
    unit_flags=$(sed -n "s|^$rel[[:space:]]\{1,\}\([^#]*\).*|\1|p" "config/$version/cflags.txt" 2>/dev/null)
    flags="$flags $unit_flags"
fi
run "$cc1" \
    -D__GNUC__=2 -D__GNUC_MINOR__=96 -D__GNUC_PATCHLEVEL__=0 \
    -Dmips -DMIPSEL -DR5900 -D_mips -D_MIPSEL -D_R5900 -D__ee__ \
    -D__mips__ -D__MIPSEL__ -D__R5900__ -D__mips -D__MIPSEL -D__R5900 -D__OPTIMIZE__ \
    -D__LANGUAGE_C -D_LANGUAGE_C -DLANGUAGE_C \
    "-D__SIZE_TYPE__=unsigned int" "-D__PTRDIFF_TYPE__=int" -D__LONG_MAX__=9223372036854775807L \
    -U__mips -D__mips=3 -D__mips64 -D__mips_eabi -D__mips_single_float \
    -Iinclude -Isrc "-DASM_ROOT=\"build/eeasm/asm/$version/nonmatchings/\"" "-DVERSION_$(echo $version | tr a-z A-Z)" \
    -quiet -O2 $flags "$cc_in" -o "$cc_out"
# DDS_KEEP_S=path keeps the compiler's assembly (check_unit measures inline asm).
if [ -n "$DDS_KEEP_S" ]; then cp "$cc_out" "$DDS_KEEP_S"; fi
run "$ee/ee/bin/as" -EL -G8 -g -Iinclude -o "$out" "$cc_out"
