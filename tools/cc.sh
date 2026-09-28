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
run() {
    if [ -n "$DDS_I386_LIBDIR" ]; then
        "$DDS_I386_LIBDIR/ld-linux.so.2" --library-path "$DDS_I386_LIBDIR" "$@"
    else
        "$@"
    fi
}
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT
cd "$root"
# Per-unit flags of the original build (config/<v>/cflags.txt), as configure.py uses.
case $in in
    "$root"/src/*/*.c)
        rel=${in#"$root"/src/*/}; rel=${rel%.c}
        unit_flags=$(sed -n "s|^$rel[[:space:]]\{1,\}\([^#]*\).*|\1|p" "config/$version/cflags.txt" 2>/dev/null)
        flags="$flags $unit_flags" ;;
esac
run "$ee/lib/gcc-lib/ee/2.96-ee-001003-1/cc1" \
    -D__GNUC__=2 -D__GNUC_MINOR__=96 -D__GNUC_PATCHLEVEL__=0 \
    -Dmips -DMIPSEL -DR5900 -D_mips -D_MIPSEL -D_R5900 -D__ee__ \
    -D__mips__ -D__MIPSEL__ -D__R5900__ -D__mips -D__MIPSEL -D__R5900 -D__OPTIMIZE__ \
    -D__LANGUAGE_C -D_LANGUAGE_C -DLANGUAGE_C \
    "-D__SIZE_TYPE__=unsigned int" "-D__PTRDIFF_TYPE__=int" -D__LONG_MAX__=9223372036854775807L \
    -U__mips -D__mips=3 -D__mips64 -D__mips_eabi -D__mips_single_float \
    -Iinclude -Isrc "-DASM_ROOT=\"build/eeasm/asm/$version/nonmatchings/\"" "-DVERSION_$(echo $version | tr a-z A-Z)" \
    -quiet -O2 $flags "$in" -o "$tmp/out.s"
run "$ee/ee/bin/as" -EL -G8 -Iinclude -o "$out" "$tmp/out.s"
