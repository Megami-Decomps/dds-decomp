#!/usr/bin/env python3
"""Run decomp-permuter on a draft compiled inside its real unit.

    tools/perm_unit.py import src/dds1/game/code_00202178.c func_00203BA8 draft.c
    ../decomp-permuter/permuter.py build/permuter/dds1_func_00203BA8 -j8 --best-only --stack-diffs --stop-on-zero

`import` writes build/permuter/<version>_<func>/. Each candidate is spliced
back into the unit in place of the function's INCLUDE_ASM (draft.c may carry
declarations ahead of the function; they stay in place), compiled with
tools/cc.sh under the unit's canonical name and flags, and reduced to an
object holding only that function, so small-data and no-throw facts are the
unit's own. Other functions' bodies become prototypes when that leaves the
base's code unchanged, keeping inline helpers and anything the function
names. The target is the unit's INCLUDE_ASM body through the same path, and
both sides are dumped with relocations folded in, so a draft that matches
scores 0.

Only passes that keep source ordinary are enabled (NEAT_WEIGHTS). A permuter
hit is a hint, not landable code: reduce it to the smallest ordinary change
and verify the full build (docs/CONTRIBUTING.md).
Use --stack-diffs: the permuter otherwise ignores stack-offset differences,
so a zero score can still hide different local layouts. Independently check
the reduced source with tools/check_unit.py in the complete unit.

Needs a decomp-permuter checkout (DECOMP_PERMUTER, default ../decomp-permuter
next to the repository) and pycparser/toml for the Python running `import`.
"""
from __future__ import annotations

import argparse
import base64
import importlib.util
import json
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
AS = ROOT / "tools/compilers/ee-gcc2.96/ee/bin/as"
OBJDUMP = ROOT / "tools/bin/mips-ps2-decompals-objdump"

NEAT_WEIGHTS = {
    "perm_temp_for_expr": 40, "perm_expand_expr": 20, "perm_reorder_stmts": 15,
    "perm_reorder_decls": 10, "perm_cast_simple": 8, "perm_refer_to_var": 5,
    "perm_float_literal": 1, "perm_randomize_internal_type": 12, "perm_split_assignment": 10,
    "perm_ins_block": 4, "perm_struct_ref": 8, "perm_condition": 6, "perm_commutative": 10,
    "perm_add_sub": 3, "perm_inequalities": 5, "perm_compound_assignment": 5,
    "perm_chain_assignment": 5, "perm_long_chain_assignment": 2, "perm_inline": 3,
    "perm_remove_var": 6, "perm_reverse_cond": 6,
    # Never landable: masks, xor/mult zero, dummy commas, self/duplicate
    # assignments, padding variables, extern/prototype type changes, ...
    "perm_add_mask": 0, "perm_xor_zero": 0, "perm_randomize_external_type": 0,
    "perm_randomize_function_type": 0, "perm_sameline": 0, "perm_empty_stmt": 0,
    "perm_mult_zero": 0, "perm_factor_mult": 0, "perm_factor_shift": 0,
    "perm_dummy_comma_expr": 0, "perm_add_self_assignment": 0, "perm_remove_ast": 0,
    "perm_duplicate_assignment": 0, "perm_pad_var_decl": 0, "perm_var_cond_block": 0,
    "perm_alias_array": 0,
}


def find_def(text: str, fn: str) -> tuple[int, int]:
    """(start, end) of fn's definition; start is at the start of its line."""
    for m in re.finditer(r"^[^\n;{}#]*?\b" + re.escape(fn) + r"\s*\(", text, re.M):
        i, depth = m.end(), 1
        while depth:
            depth += {"(": 1, ")": -1}.get(text[i], 0)
            i += 1
        while text[i] in " \t\n":
            i += 1
        if text[i] != "{":
            continue  # a prototype
        depth = 0
        while True:
            depth += {"{": 1, "}": -1}.get(text[i], 0)
            if depth == 0:
                break
            i += 1
        i += 1
        if i < len(text) and text[i] == "\n":
            i += 1
        return m.start(), i
    raise SystemExit(f"no definition of {fn}")


def skip_literal(t: str, i: int) -> int:
    q, i = t[i], i + 1
    while t[i] != q:
        i += 2 if t[i] == "\\" else 1
    return i + 1


def prototypes_only(t: str, body: str) -> str:
    """t with other functions' definitions turned into prototypes, except inline
    helpers and functions body names (an earlier definition makes calls to a
    function no-throw, which changes delay-slot filling)."""
    used = set(re.findall(r"\b[A-Za-z_]\w*\b", body))
    i, depth, stmt = 0, 0, 0
    while i < len(t):
        c = t[i]
        if t.startswith("//", i):
            i = t.index("\n", i)
            continue
        if t.startswith("/*", i):
            i = t.index("*/", i) + 2
            continue
        if c in "\"'":
            i = skip_literal(t, i)
            continue
        if c == "#" and depth == 0 and (i == 0 or t[i - 1] == "\n"):
            while True:
                j = t.index("\n", i)
                if t[j - 1] != "\\":
                    break
                i = j + 1
            i = stmt = j + 1
            continue
        if c == "{" and depth == 0:
            head = re.sub(r"/\*.*?\*/|//[^\n]*", "", t[stmt:i], flags=re.S).strip()
            if head.endswith(")") and "=" not in head.split("(")[0]:
                names = [w for w in re.findall(r"(\w+)\s*\(", head) if w != "__attribute__"]
                j, d = i, 0
                while True:
                    if t[j] in "\"'":
                        j = skip_literal(t, j)
                        continue
                    if t.startswith("/*", j):
                        j = t.index("*/", j) + 2
                        continue
                    if t.startswith("//", j):
                        j = t.index("\n", j)
                        continue
                    d += {"{": 1, "}": -1}.get(t[j], 0)
                    if d == 0:
                        break
                    j += 1
                keep = not names or names[0] in used or re.search(r"\binline\b|__inline", head)
                if keep:
                    i = stmt = j + 1
                else:
                    pre = t[:i].rstrip()
                    t = pre + ";\n" + t[j + 1:]
                    i = stmt = len(pre) + 2
                continue
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0:
                stmt = i + 1
        elif c == ";" and depth == 0:
            stmt = i + 1
        i += 1
    return t


def hide_asm(src: str) -> str:
    """Each `__asm__ [volatile] (...)` becomes a permuter b64literal pragma, which
    pycparser accepts and the permuter prints back verbatim."""
    out, i = [], 0
    for m in re.finditer(r"\b__asm__\b", src):
        if m.start() < i:
            continue
        mm = re.match(r"\s*(volatile\s*)?\(", src[m.end():])
        if not mm:
            continue
        k, depth = m.end() + mm.end(), 1
        while depth:
            if src[k] in "\"'":
                k = skip_literal(src, k)
                continue
            depth += {"(": 1, ")": -1}.get(src[k], 0)
            k += 1
        while src[k] in " \t":
            k += 1
        if src[k] == ";":
            k += 1
        text = src[m.start():k].replace("\n", " ")
        out += [src[i:m.start()], "\n#pragma _permuter b64literal " + base64.b64encode(text.encode()).decode() + "\n"]
        i = k
    return "".join(out + [src[i:]])


def strip_other_code(s: str, fn: str) -> str:
    """A unit's cc1 assembly without other functions' code (compiled bodies and
    INCLUDE_ASM includes); data, .extern and section state are kept."""
    out, skip = [], None
    for line in s.split("\n"):
        m = re.fullmatch(r"\s*\.ent\s+(\S+)", line)
        if m and m.group(1) != fn:
            skip = m.group(1)
            continue
        if skip:
            if re.fullmatch(r"\s*\.end\s+" + re.escape(skip), line):
                skip = None
            continue
        m = re.search(r'\.include\s+"build/eeasm/asm/[^"]*/nonmatchings/[^"]*/([^/"]+)\.s"', line)
        if m and m.group(1) != fn and not m.group(1).startswith(("jtbl_", "D_")):
            continue
        out.append(line)
    return "\n".join(out) + "\n"


def permuter_dir() -> Path:
    return Path(os.environ.get("DECOMP_PERMUTER", ROOT.parent / "decomp-permuter")).resolve()


def cmd_import(a: argparse.Namespace) -> None:
    unit = Path(a.unit).resolve().relative_to(ROOT).as_posix()
    version = unit.split("/")[1]
    fn = a.func
    out = Path(a.out or ROOT / "build/permuter" / f"{version}_{fn}").resolve()
    out.mkdir(parents=True, exist_ok=True)
    s = (ROOT / unit).read_text()
    m = re.search(r'INCLUDE_ASM\([^,]+,\s*"[^"]+",\s*' + re.escape(fn) + r"\);\n", s)
    if not m:
        raise SystemExit(f"{fn} is not INCLUDE_ASM in {unit}")
    d = Path(a.draft).read_text()
    da, db = find_def(d, fn)
    template = s[:m.start()] + d[:da] + "@@PERM@@\n" + d[db:] + s[m.end():]
    full = s[:m.start()] + d + s[m.end():]
    (out / "template.c").write_text(template)
    (out / "meta.json").write_text(json.dumps({"version": version, "unit": unit, "func": fn,
                                               "include_asm": m.group(0).strip()}))

    cpp = ["cpp", "-P", "-undef", "-nostdinc", "-DPERMUTER", "-D__GNUC__=2", "-D__GNUC_MINOR__=96",
           "-Dmips", "-D__mips__", "-D_R5900", "-D__R5900__", "-D_LANGUAGE_C", "-DLANGUAGE_C",
           "-D__SIZE_TYPE__=unsigned int", "-D__PTRDIFF_TYPE__=int", f"-DVERSION_{version.upper()}",
           f"-I{ROOT / 'include'}", f"-I{ROOT / 'src'}", f"-I{(ROOT / unit).parent}",
           "-D_Static_assert(x, y)=", "-D__attribute__(x)=", "-D__inline__=inline", "-D__inline=inline",
           "-D__volatile__=volatile", "-D__const=const", "-D__extension__=", "-D__restrict=",
           "-D__signed__=signed", "-Dasm=__asm__", "-"]
    src = hide_asm(subprocess.run(cpp, input=full, capture_output=True, text=True, check=True).stdout)
    perm = permuter_dir()
    spec = importlib.util.spec_from_file_location("permuter_import", perm / "import.py")
    imp = importlib.util.module_from_spec(spec)
    sys.path.insert(0, str(perm))
    spec.loader.exec_module(imp)
    base, _ = imp.prune_source(src, True, fn)
    from src.ast_util import process_pragmas
    (out / "base.c").write_text(base)

    me = Path(__file__).resolve()
    (out / "compile.sh").write_text(f'#!/bin/sh\nexec python3 "{me}" cc "{out}" "$1" "$3"\n')
    (out / "compile.sh").chmod(0o755)
    subprocess.run([sys.executable, me, "cc", out, "TARGET", out / "target.o"], check=True)

    fast = prototypes_only(template, full[find_def(full, fn)[0]:find_def(full, fn)[1]])
    dumps = []
    # base.c keeps parser-safe asm pragmas; validate the decoded source that
    # the permuter actually sends to the compiler, not ignored GCC pragmas.
    with tempfile.TemporaryDirectory(dir=out) as t:
        check_source = Path(t) / "base.c"
        check_source.write_text(process_pragmas(base))
        for name in ("fast", "full"):
            if name == "fast":
                (out / "template_fast.c").write_text(fast)
            else:
                (out / "template_fast.c").unlink()
            subprocess.run([out / "compile.sh", check_source, "-o", out / f"check_{name}.o"], check=True)
            dumps.append(dump(out / f"check_{name}.o")[3:])
    if dumps[0] == dumps[1] and len(dumps[0]) > 3:
        (out / "template_fast.c").write_text(fast)
    print("prototype-only unit:", "same code, used" if dumps[0] == dumps[1] else "differs, full unit used")

    settings = [f'func_name = "{fn}"', 'compiler_type = "gcc"',
                f'objdump_command = "python3 {me} objdump"', "", "[weight_overrides]"]
    settings += [f"{k} = {v}" for k, v in NEAT_WEIGHTS.items()]
    (out / "settings.toml").write_text("\n".join(settings) + "\n")
    print(out)


def cmd_cc(a: argparse.Namespace) -> None:
    d = Path(a.dir)
    meta = json.loads((d / "meta.json").read_text())
    fn = meta["func"]
    if a.input == "TARGET":
        body = meta["include_asm"] + "\n"
        tpl = d / "template.c"
    else:
        c = Path(a.input).read_text()
        ca, cb = find_def(c, fn)
        body = c[ca:cb]
        tpl = d / "template_fast.c" if (d / "template_fast.c").exists() else d / "template.c"
    src = tpl.read_text().replace("@@PERM@@\n", body, 1)
    with tempfile.TemporaryDirectory(dir=d) as t:
        (Path(t) / "u.c").write_text(src)
        env = {**os.environ, "DDS_VERSION": meta["version"], "DDS_AS_UNIT": meta["unit"],
               "DDS_KEEP_S": f"{t}/u.s"}
        r = subprocess.run([ROOT / "tools/cc.sh", f"{t}/u.c", "-o", f"{t}/u.o"], env=env, cwd=ROOT,
                           capture_output=True, text=True)
        if r.returncode:
            sys.exit(r.stderr[-2000:])
        (Path(t) / "f.s").write_text(strip_other_code((Path(t) / "u.s").read_text(), fn))
        r = subprocess.run([AS, "-EL", "-G8", "-Iinclude", "-o", Path(a.output).resolve(), f"{t}/f.s"],
                           cwd=ROOT, capture_output=True, text=True)
        if r.returncode:
            sys.exit(r.stderr[-2000:])


def dump(o: Path) -> list[str]:
    """objdump -dr with relocations folded into their instructions and symbol
    spelling normalized (retail asm says jtbl_/absolute $gp offsets where cc1
    output has section-relative relocations)."""
    lines = subprocess.run([OBJDUMP, "-dr", o], capture_output=True, text=True, check=True).stdout.split("\n")
    out: list[str] = []
    for line in lines:
        m = re.match(r"\s+[0-9a-f]+: (R_MIPS_\w+)\s+(\S+)", line)
        if m and out:
            kind, sym = m.groups()
            if kind == "R_MIPS_26":
                out[-1] = re.sub(r"[0-9a-f]+ <[^>]*>$|[0-9a-f]+$", sym.split("+")[0], out[-1])
            elif kind in ("R_MIPS_HI16", "R_MIPS_LO16", "R_MIPS_GPREL16"):
                out[-1] = re.sub(r"(,|\t)(-?(0x)?[0-9a-f]+)(\(|$)", r"\1<sym>\4", out[-1])
            continue
        out.append(re.sub(r"-?\d+\(gp\)", "<sym>(gp)", line))
    return out


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("import", help="set up a permuter directory for a draft")
    p.add_argument("unit")
    p.add_argument("func")
    p.add_argument("draft")
    p.add_argument("-o", "--out")
    p = sub.add_parser("cc", help="compile hook used by the directory's compile.sh")
    p.add_argument("dir")
    p.add_argument("input")
    p.add_argument("output")
    p = sub.add_parser("objdump", help="dump hook used by the directory's settings.toml")
    p.add_argument("args", nargs="+")
    a = ap.parse_args()
    if a.cmd == "import":
        cmd_import(a)
    elif a.cmd == "cc":
        cmd_cc(a)
    else:
        print("\n".join(dump(Path(a.args[-1]))))


if __name__ == "__main__":
    main()
