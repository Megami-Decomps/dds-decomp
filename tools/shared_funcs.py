#!/usr/bin/env python3
"""Functions shared by DDS1 and DDS2: map them, share names, mirror units, port C.

    python3 tools/shared_funcs.py map [--nocturne SLPM_652.42] [--us SLUS_209.11]
    python3 tools/shared_funcs.py names
    python3 tools/shared_funcs.py units [--write] [--from dds1]
    python3 tools/shared_funcs.py port [--from dds1]
    python3 tools/shared_funcs.py clones dds1

Pairs come from `romwright-cli diff dds2 --reference build/romwright --reference-name dds1
--json` (build/dds1_diff_dds2.json) and are kept only when the code is byte-identical
after relocation masking.

map    writes build/shared/dds1_dds2.txt: every identical pair, whether it is in address
       order ("order": longest increasing run plus same-count gaps), and the identical
       Nocturne debug-build / US Nocturne function when those diffs exist.
names  copies hand-curated names (the block above romwright's in symbol_addrs.txt) to the
       other version through in-order pairs, into a generated "shared" block.
units  lists the source version's C units that have no counterpart in the other version,
       with the other version's range when every function maps in order; --write splits
       the other version's asm chunk.
port   replaces the other version's INCLUDE_ASM with the source version's C for each
       decompiled function whose counterpart is identical. Symbol references are
       translated through the relocations of that pair, so the result rebuilds the same
       bytes even when romwright matched one of several identical copies.
clones the same within one version: an INCLUDE_ASM function whose masked code equals
       a decompiled function of the same game gets that C with its own symbols.
"""
import argparse
import bisect
import collections
import json
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

from pairing import RETAIL, ROOT, STORE, Build, address_pairs, diff_pairs, identical, ordered_pairs, va_to_off

VERSIONS = json.loads((ROOT / "config/versions.json").read_text())
PAIR_DIFF = ROOT / "build/dds1_diff_dds2.json"
OUT = ROOT / "build/shared/dds1_dds2.txt"
ROW = re.compile(r"^\s*(\S+)\s*=\s*0x([0-9A-Fa-f]+)\s*;(.*)$")
PLACEHOLDER = re.compile(r"^(func|D|jtbl|FUN|sub|entry)_?[0-9A-Fa-f]*$")
SYM_TOKEN = re.compile(r"^(?:func|D)_([0-9A-F]{8})$")
RW_BEGIN = "// BEGIN romwright"
SH_BEGIN = "// BEGIN shared (tools/shared_funcs.py names; curate names in either version's block above)"
SH_END = "// END shared"
INCLUDE_ASM = re.compile(r'^INCLUDE_ASM\([^,]+,\s*"[^"]+",\s*(\w+)\);$', re.M)
TOKENS = re.compile(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|\b[A-Za-z_]\w*\b', re.S)
DEF = re.compile(r"^[A-Za-z_][\w\s\*]*?\b(\w+)\s*\([^;]*$", re.M)
TYPE_DECL = re.compile(r"\s*(?:/\*.*?\*/\s*)*(?:typedef|struct|union|enum)\b", re.S)
FIRST_BODY = re.compile(r"^(?:INCLUDE_ASM\(|[A-Za-z_][\w \t\*]*\b\w+\s*\([^;{]*\)\s*\{?[ \t]*$)", re.M)
UNIT = re.compile(r"^(\s+- \[)0x([0-9A-Fa-f]+), (\w+), ([^\]]+)\](.*)$")


class Game:
    def __init__(self, version):
        self.version = version
        self.build = Build(ROOT / RETAIL[version], STORE, version)
        self.gp = int(VERSIONS[version]["gp"], 16)
        self.sym_path = ROOT / "config" / version / "symbol_addrs.txt"
        self.yaml = ROOT / "config" / version / f"{VERSIONS[version]['serial']}.yaml"
        self.by_name, self.by_addr, self.curated = {}, {}, {}
        section = "curated"
        for line in self.sym_path.read_text().splitlines():
            if line.startswith(RW_BEGIN):
                section = "romwright"
            elif line.startswith(SH_BEGIN):
                section = "shared"
            elif line.startswith("// END"):
                section = "curated"
            elif m := ROW.match(line):
                name, addr, rest = m.group(1), int(m.group(2), 16), m.group(3)
                self.by_name[name] = addr
                self.by_addr.setdefault(addr, name)
                if section == "curated" and not PLACEHOLDER.match(name):
                    self.curated[addr] = (name, rest.strip())

    def address_of(self, token):
        if token in self.by_name:
            return self.by_name[token]
        m = SYM_TOKEN.match(token)
        return int(m.group(1), 16) if m else None

    def name_at(self, addr):
        if addr in self.by_addr:
            return self.by_addr[addr]
        return f"func_{addr:08X}" if addr in self.build.sizes else f"D_{addr:08X}"


def load_pairs(d1, d2):
    raw = diff_pairs(PAIR_DIFF, a_is_target=False)
    ordered = ordered_pairs(d1.build, d2.build, raw)
    loose = {a: b for a, b in raw.items() if a not in ordered and identical(d1.build, a, d2.build, b)}
    return ordered, loose


def oriented(args):
    d1, d2 = Game("dds1"), Game("dds2")
    ordered, loose = load_pairs(d1, d2)
    if args.src == "dds1":
        return d1, d2, ordered, loose
    return d2, d1, {b: a for a, b in ordered.items()}, {b: a for a, b in loose.items()}


def cmd_map(args):
    d1, d2 = Game("dds1"), Game("dds2")
    ordered, loose = load_pairs(d1, d2)
    extra = {}
    for label, elf, store, prog, diff in (
            ("noct", args.nocturne, "build/romwright-noct/project.sqlite", "noct", "build/noct_diff_dds1.json"),
            ("noctus", args.us, "build/romwright-noctus/project.sqlite", "noctus", "build/noctus_diff_dds1.json")):
        if elf:
            other = Build(elf, ROOT / store, prog)
            extra[label] = {a: b for a, b in diff_pairs(ROOT / diff, a_is_target=True).items()
                            if identical(d1.build, a, other, b)}
    OUT.parent.mkdir(parents=True, exist_ok=True)
    rows = sorted([(a, b, "order") for a, b in ordered.items()] + [(a, b, "moved") for a, b in loose.items()])
    with OUT.open("w") as f:
        f.write("# DDS1/DDS2 functions with byte-identical code (relocations masked); tools/shared_funcs.py\n")
        f.write("# dds1      dds2        size  kind   " + "  ".join(f"{k:10s}" for k in extra) + "\n")
        for a, b, kind in rows:
            cols = "  ".join(f"0x{extra[k][a]:08X}" if a in extra[k] else "-         " for k in extra)
            f.write(f"0x{a:08X}  0x{b:08X}  {d1.build.sizes[a]:5d}  {kind:5s}  {cols}\n".rstrip() + "\n")
    print(f"dds1 {len(d1.build.sizes)} / dds2 {len(d2.build.sizes)} functions; identical: "
          f"{len(ordered)} in order, {len(loose)} moved or duplicate copies")
    for k, m in extra.items():
        both = sum(1 for a in m if a in ordered or a in loose)
        print(f"  {k}: {len(m)} dds1 functions identical, {both} of them also in dds2")
    print(f"-> {OUT.relative_to(ROOT)}")


def data_votes(src, dst, pairs):
    votes = collections.defaultdict(collections.Counter)
    for a, b in pairs.items():
        for x, y in address_pairs(src.build, a, dst.build, b, src.gp, dst.gp):
            votes[x][y] += 1
    return votes


def cmd_names(_args):
    d1, d2 = Game("dds1"), Game("dds2")
    ordered, _ = load_pairs(d1, d2)
    for src, dst, pairs in ((d1, d2, ordered), (d2, d1, {b: a for a, b in ordered.items()})):
        votes = None
        rows, conflicts = [], []
        for addr, (name, attrs) in sorted(src.curated.items()):
            if addr in pairs:
                target = pairs[addr]
            else:
                votes = votes or data_votes(src, dst, pairs)
                if addr not in votes or addr in src.build.sizes:
                    continue
                (target, n), *rest = votes[addr].most_common()
                if rest and rest[0][1] * 2 >= n:
                    conflicts.append(f"{name}: ambiguous data address in {dst.version}")
                    continue
            have = dst.curated.get(target, (None,))[0]
            if have == name:
                continue
            if have:
                conflicts.append(f"{name} ({src.version}) vs {have} ({dst.version}) at 0x{target:08X}")
                continue
            if name in dst.by_name and dst.by_name[name] != target:
                conflicts.append(f"{name}: already at 0x{dst.by_name[name]:08X} in {dst.version}")
                continue
            rows.append((name, target, f"{name} = 0x{target:08X}; {attrs}  // {src.version} 0x{addr:08X}".rstrip()))
        write_shared_block(dst, rows)
        print(f"{src.version} -> {dst.version}: {len(rows)} names")
        for c in conflicts:
            print(f"  conflict: {c}")


def write_shared_block(game, rows):
    """Replace the shared block, keep romwright's placeholder rows off named addresses, and
    rename the version's C references to match."""
    old, kept, section = {}, [], None
    for line in game.sym_path.read_text().splitlines():
        if line.startswith(SH_BEGIN):
            section = "shared"
            continue
        if section == "shared":
            if line.startswith(SH_END):
                section = None
            elif m := ROW.match(line):
                old[int(m.group(2), 16)] = m.group(1)
            continue
        kept.append(line)
    new = {target: name for name, target, _ in rows}
    placeholder = {addr: f"func_{addr:08X}" for addr in set(old) | set(new)}
    rw_start = next(i for i, line in enumerate(kept) if line.startswith(RW_BEGIN))
    rw_end = next(i for i, line in enumerate(kept) if i > rw_start and line.startswith("// END romwright"))
    rw = {}
    for line in kept[rw_start + 1:rw_end]:
        if m := ROW.match(line):
            rw[int(m.group(2), 16)] = line
    for addr in new:
        rw.pop(addr, None)
    for addr in set(old) - set(new):
        if addr in game.build.sizes:
            rw.setdefault(addr, f"{placeholder[addr]} = 0x{addr:08X}; // type:func")
    block = [SH_BEGIN, *(r for _, _, r in rows), SH_END] if rows else []
    lines = kept[:rw_start] + block + [kept[rw_start]] + [rw[a] for a in sorted(rw)] + kept[rw_end:]
    game.sym_path.write_text("\n".join(lines) + "\n")

    renames = {}
    for addr, name in new.items():
        for before in (placeholder[addr], old.get(addr)):
            if before and before != name:
                renames[before] = name
    for addr in set(old) - set(new):
        renames[old[addr]] = placeholder[addr]
    if renames:
        pattern = re.compile(r"\b(" + "|".join(map(re.escape, renames)) + r")\b")
        for path in (ROOT / "src" / game.version).rglob("*.c"):
            text = path.read_text()
            changed = pattern.sub(lambda m: renames[m.group(1)], text)
            if changed != text:
                path.write_text(changed)


def yaml_units(game):
    rows = []
    for i, line in enumerate(game.yaml.read_text().splitlines()):
        if m := UNIT.match(line):
            rows.append((int(m.group(2), 16) + 0xFF000, m.group(3), m.group(4), i))
    return rows


def unit_ranges(game):
    """.text units in yaml order -> [(start_vram, end_vram, kind, name, yaml_line)]."""
    code, text_end = [], None
    for row in yaml_units(game):
        if row[1] in ("asm", "c"):
            code.append(row)
        elif code:
            text_end = row[0]
            break
    ends = [r[0] for r in code[1:]] + [text_end]
    return [(s, e, kind, name, line) for (s, kind, name, line), e in zip(code, ends)]


def cmd_units(args):
    src, dst, ordered, _ = oriented(args)
    dst_units = unit_ranges(dst)
    dst_names = {u[3] for u in dst_units}
    edits = []
    for s, e, kind, name, _ in unit_ranges(src):
        if kind != "c" or name in dst_names or name.startswith("sdk/"):
            continue
        funcs = src.build.entries[bisect.bisect_left(src.build.entries, s):bisect.bisect_left(src.build.entries, e)]
        mapped = [ordered.get(f) for f in funcs]
        if None in mapped:
            print(f"{name}: {mapped.count(None)} of {len(funcs)} functions have no in-order {dst.version} copy")
            continue
        lo = mapped[0]
        i = bisect.bisect_left(dst.build.entries, lo)
        if dst.build.entries[i:i + len(mapped)] != mapped:
            print(f"{name}: {dst.version} copies are not contiguous")
            continue
        hi = dst.build.entries[i + len(mapped)]
        host = next(u for u in dst_units if u[0] <= lo < u[1])
        if host[2] != "asm" or hi > host[1]:
            print(f"{name}: {dst.version} 0x{lo:08X}-0x{hi:08X} crosses {host[3]}")
            continue
        new = f"game/code_{lo:08X}" if name.startswith("game/code_") else name
        print(f"{name}: {dst.version} {new} 0x{lo:08X}-0x{hi:08X} (inside {host[3]})")
        edits.append((host, lo, hi, new, name))
    if not args.write or not edits:
        return
    lines = dst.yaml.read_text().splitlines()
    for host, lo, hi, new, name in sorted(edits, key=lambda x: -x[0][4]):
        s, e, _, host_name, idx = host
        repl = [] if lo == s else [lines[idx]]
        repl.append(f"      - [0x{lo - 0xFF000:X}, c, {new}]  # mirrors {src.version} {name} (tools/shared_funcs.py)")
        if hi < e:
            repl.append(f"      - [0x{hi - 0xFF000:X}, asm, game/code_{hi:08X}]")
        lines[idx:idx + 1] = repl
    dst.yaml.write_text("\n".join(lines) + "\n")
    print(f"wrote {len(edits)} units to {dst.yaml.relative_to(ROOT)}; run python3 configure.py")


def blocks(text):
    """Top-level items: each declaration, definition, INCLUDE_ASM or preprocessor line,
    with any comment lines directly above it. Several declarations on consecutive lines
    are separate items, so porting picks only the ones a function uses."""
    out, cur, depth = [], [], 0

    def flush():
        if cur:
            out.append("\n".join(cur))
            cur.clear()

    for line in text.split("\n"):
        stripped = line.strip()
        if depth == 0 and not stripped:
            flush()
            continue
        if depth == 0 and stripped.startswith("#"):
            flush()
            out.append(line)
            continue
        cur.append(line)
        depth += line.count("{") - line.count("}")
        code = re.sub(r"\s*(/\*.*?\*/|//.*)$", "", stripped)  # a trailing comment
        if depth == 0 and code.endswith((";", "}")):
            flush()
    flush()
    return out

def translate(text, src, dst, amap):
    """Rewrite symbol identifiers through amap; comments and literals stay verbatim."""
    missing = []

    def sub(m):
        tok = m.group(0)
        if not (tok[0].isalpha() or tok[0] == "_"):
            return tok
        addr = src.address_of(tok)
        if addr is None:
            return tok
        if addr in amap:
            if addr in src.build.sizes and amap[addr] not in dst.build.sizes:
                missing.append(tok)  # a function whose counterpart is no function start
                return tok
            return dst.name_at(amap[addr])
        near = [a for a in amap if addr < a < addr + 0x800 and a not in src.build.sizes]
        if near and addr not in src.build.sizes:
            a = min(near)
            return dst.name_at(amap[a] - (a - addr))
        missing.append(tok)
        return tok

    return TOKENS.sub(sub, text), missing



STRING = re.compile(r'"((?:[^"\\\n]|\\.)*)"')


def c_unescape(s):
    return s.encode("latin-1").decode("unicode_escape").encode("latin-1")


def c_escape(b):
    out = []
    for c in b:
        ch = chr(c)
        out.append({"\n": "\\n", "\t": "\\t", "\"": "\\\"", "\\": "\\\\"}.get(ch)
                   or (ch if 32 <= c < 127 else f"\\x{c:02x}"))
    return "".join(out)


def translate_strings(body, src, dst, amap):
    """A string literal the function writes itself sits at some retail address the
    relocations pair with the counterpart's own string, which may differ (clones
    that print different messages). Returns the body with the counterpart's
    strings, or None when a literal cannot be located."""
    def cstr(build, va):
        off = va_to_off(build.segs, va)
        end = build.data.index(b"\0", off)
        return build.data[off:end]

    def sub(m):
        want = c_unescape(m.group(1))
        for x, y in amap.items():
            if x in src.build.sizes or y in dst.build.sizes:
                continue
            try:
                if cstr(src.build, x) == want:
                    return '"' + c_escape(cstr(dst.build, y)) + '"'
            except (ValueError, KeyError, IndexError):
                continue
        raise LookupError(m.group(0))

    try:
        return STRING.sub(sub, body)
    except LookupError:
        return None


def declared_name(decl):
    """Identifier a top-level declaration introduces: the typedef/struct name for type
    definitions (whose bodies may hold function-pointer members), the name before '('
    for prototypes, else the last identifier before ';'."""
    code = re.sub(r"/\*.*?\*/|//[^\n]*", "", decl, flags=re.S).strip()
    if m := re.match(r"#define\s+(\w+)", code):
        return m.group(1)
    if code.startswith(("typedef", "struct", "union", "enum")):
        if "{" not in code and (m := re.search(r"\(\s*\*\s*(\w+)\s*\)\s*\(", code)):
            return m.group(1)  # typedef void (*Fn)(...);
        tail = code.rsplit("}", 1)[-1] if "}" in code else code
        names = re.findall(r"[A-Za-z_]\w*", tail.rsplit(";", 1)[0])
        if names:
            return names[-1]
        m = re.match(r"(?:typedef\s+)?(?:struct|union|enum)\s+(\w+)", code)
        return m.group(1) if m else None
    head = code.split("(", 1)[0] if "(" in code else code.rsplit(";", 1)[0]
    names = re.findall(r"[A-Za-z_]\w*", head)
    return names[-1] if names else None


def cmd_port(args):
    src, dst, ordered, loose = oriented(args)
    port(src, dst, {a: [b] for a, b in {**loose, **ordered}.items()})


def cmd_clones(args):
    """Same-game twins: an asm function whose relocation-masked code equals a
    decompiled function of the same version gets that function's C."""
    game = Game(args.version)
    b = game.build
    by_code = collections.defaultdict(list)
    for fa in b.sizes:
        if b.sizes[fa] >= 16:
            by_code[tuple(b.code(fa))].append(fa)
    asm_names = set()
    defined = {}
    for path in (ROOT / "src" / game.version).rglob("*.c"):
        text = path.read_text()
        asm_names.update(INCLUDE_ASM.findall(text))
        for blk in blocks(text):
            if "{" in blk and (m := DEF.search(blk)) and not re.fullmatch(r"[^{]*\{\s*\}", blk.strip()):
                defined[game.address_of(m.group(1))] = m.group(1)
    pairs = {}
    for group in by_code.values():
        done = [a for a in group if a in defined]
        todo = [a for a in group if game.name_at(a) in asm_names]
        if done and todo:
            pairs[done[0]] = todo
    print(f"{sum(map(len, pairs.values()))} asm functions have a decompiled twin")
    port(game, game, pairs)


ported_names = {}  # unit -> the functions this run gave C (the only ones finish() may revert)
originals = {}  # unit -> its text before this run


def port(src, dst, pairs):
    """Replace dst's INCLUDE_ASM for each target in pairs[src address] with src's C."""
    dst_files, dst_defined = {}, set()
    for path in (ROOT / "src" / dst.version).rglob("*.c"):
        text = path.read_text()
        for name in INCLUDE_ASM.findall(text):
            dst_files[name] = path
        dst_defined.update(m.group(1) for b in blocks(text) if "{" in b and (m := DEF.search(b)))
    ported, skipped = collections.Counter(), []
    for path in sorted((ROOT / "src" / src.version).rglob("*.c")):
        pre, funcs = [], []
        for b in blocks(path.read_text()):
            if (b.startswith("#") and not b.startswith("#define")) or "INCLUDE_ASM" in b:
                continue
            m = DEF.search(b) if "{" in b else None
            (funcs if m else pre).append((m.group(1) if m else None, b))
        for fname, body in funcs:
            if re.fullmatch(r"[^{]*\{\s*\}", body.strip()):
                continue  # splat writes `jr $ra` stubs as empty C in every version
            addr = src.address_of(fname)
            rel = path.relative_to(ROOT)
            if addr not in pairs:
                if src is not dst:
                    skipped.append(f"{rel}:{fname}: no identical {dst.version} function")
                continue
            for target in pairs[addr]:
                port_one(src, dst, path, pre, fname, body, addr, target, dst_files, dst_defined, ported, skipped)
    finish(dst, ported, skipped)


def port_one(src, dst, path, pre, fname, body, addr, target, dst_files, dst_defined, ported, skipped):
    """Port one function; returns nothing, records the outcome in ported/skipped."""
    rel = path.relative_to(ROOT)
    dname = dst.name_at(target)
    dpath = dst_files.get(dname)
    if dpath is None:
        if dname not in dst_defined:
            skipped.append(f"{rel}:{fname}: {dname} is not in a {dst.version} C unit (see `units`)")
        return
    amap = {}
    for x, y in address_pairs(src.build, addr, dst.build, target, src.gp, dst.gp):
        amap.setdefault(x, y)
    amap[addr] = target
    new_body, missing = translate(body, src, dst, amap)
    new_body = translate_strings(new_body, src, dst, amap) if STRING.search(new_body) else new_body
    if new_body is None:
        skipped.append(f"{rel}:{fname}: a string literal has no retail counterpart")
        return
    used = set(TOKENS.findall(body))
    chosen = set()
    while True:
        more = {i for i, (_, decl) in enumerate(pre) if i not in chosen and declared_name(decl) in used}
        if not more:
            break
        chosen |= more
        for i in more:
            used |= set(TOKENS.findall(pre[i][1]))
    new_pre = []
    for i in sorted(chosen):
        tdecl, miss = translate(pre[i][1], src, dst, amap)
        missing += miss
        new_pre.append(tdecl)
    if missing:
        skipped.append(f"{rel}:{fname}: cannot translate {sorted(set(missing))}")
        return
    text = dpath.read_text()
    pattern = re.compile(rf'^INCLUDE_ASM\([^,]+,\s*"[^"]+",\s*{dname}\);$', re.M)
    text = pattern.sub(lambda _: new_body, text, count=1)
    add = [d for d in new_pre if d not in text]
    # A changed type definition replaces the destination's old one in place, so
    # everything declared after it still sees it first.
    for d in [d for d in add if TYPE_DECL.match(d) and "{" in d]:
        old_def = next((b for b in blocks(text) if TYPE_DECL.match(b) and "{" in b
                        and declared_name(b) == declared_name(d)), None)
        if old_def:
            text = text.replace(old_def, d, 1)
            add.remove(d)
    if add:
        # A source declaration supersedes the destination's older one of the same
        # name (e.g. a global retyped from void* to a struct pointer).
        # Types are left alone: a forward typedef and its struct share a name.
        names = {declared_name(d) for d in add if not TYPE_DECL.match(d)}
        text = "\n\n".join(b for b in blocks(text)
                           if "{" in b or TYPE_DECL.match(b) or b.startswith("#include")
                           or "INCLUDE_ASM" in b or declared_name(b) not in names) + "\n"
        # After the declarations already there (earlier ports' types may be
        # what these depend on), before the first function or INCLUDE_ASM.
        first = FIRST_BODY.search(text)
        at = first.start() if first else len(text)
        text = text[:at] + "\n\n".join(add) + "\n\n" + text[at:]
    before = dpath.read_text()
    originals.setdefault(dpath.relative_to(ROOT), before)
    dpath.write_text(text)
    # A port that breaks the destination's compile (conflicting prototype,
    # arity) is undone at once, so one bad function never blocks the unit.
    if not compiles(dpath, dst.version):
        dpath.write_text(before)
        skipped.append(f"{rel}:{fname}: does not compile in {dpath.relative_to(ROOT)}; not ported")
        return
    ported[dpath.relative_to(ROOT)] += 1
    ported_names.setdefault(dpath.relative_to(ROOT), set()).add(dname)


def finish(dst, ported, skipped):
    """Check every unit that received C; revert whatever does not match."""
    # Relocation-masked pairing ignores plain immediates (li 0x15 vs li 0x19), so a
    # pair is not proof; every ported function must compile to its own retail bytes.
    for rel in sorted(ported):
        path = ROOT / rel
        r = subprocess.run([sys.executable, str(ROOT / "tools/check_unit.py"), str(path), "-v"],
                           capture_output=True, text=True)
        bad = re.findall(r"^(?:DIFF|OVER|SHARED rodata of|PAD rodata of|TWICE) (\w+)\b", r.stdout, re.M)
        if r.returncode and not [n for n in bad if n in ported_names.get(rel, ())]:
            skipped.append(f"{rel}: check_unit failed without per-function DIFF; left for review")
        text = path.read_text()
        for name in [n for n in bad if n in ported_names.get(rel, ())]:
            block = next((b for b in blocks(text) if "{" in b and (m := DEF.search(b)) and m.group(1) == name), None)
            if block is None:
                skipped.append(f"{rel}:{name}: flagged by check_unit but not a C definition here; left for review")
                continue
            unit = rel.relative_to(Path("src") / dst.version).with_suffix("").as_posix()
            text = text.replace(block, f'INCLUDE_ASM(const s32, "{unit}", {name});', 1)
            ported[rel] -= 1
            skipped.append(f"{rel}:{name}: ported C differs from {dst.version} retail; reverted")
        path.write_text(text)
        # A ported declaration can change how the unit's other functions compile.
        if ported[rel] and subprocess.run([sys.executable, str(ROOT / "tools/check_unit.py"), str(path)],
                                          capture_output=True).returncode:
            path.write_text(originals[rel])
            skipped.append(f"{rel}: still not clean after reverting; all {ported[rel]} ports undone")
            ported[rel] = 0
    for p, n in sorted(ported.items()):
        if n:
            print(f"ported {n} to {p}")
    for s in skipped:
        print(f"skip {s}")
    if ported:
        print("run python3 configure.py --no-split && ninja")


def compiles(path, version):
    env = dict(os.environ, DDS_VERSION=version)
    with tempfile.TemporaryDirectory() as tmp:
        return subprocess.run([str(ROOT / "tools/cc.sh"), "-DSKIP_ASM", str(path), "-o", f"{tmp}/u.o"],
                              capture_output=True, env=env).returncode == 0



def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    m = sub.add_parser("map")
    m.add_argument("--nocturne", help="Nocturne debug-build SLPM_652.42 (needs build/noct_diff_dds1.json)")
    m.add_argument("--us", help="US Nocturne SLUS_209.11 (needs build/noctus_diff_dds1.json)")
    sub.add_parser("names")
    c = sub.add_parser("clones")
    c.add_argument("version", choices=("dds1", "dds2"))
    for name in ("units", "port"):
        p = sub.add_parser(name)
        p.add_argument("--from", dest="src", choices=("dds1", "dds2"), default="dds1")
        if name == "units":
            p.add_argument("--write", action="store_true", help="split the other version's asm chunks")
    args = ap.parse_args()
    if not PAIR_DIFF.exists():
        sys.exit(f"{PAIR_DIFF.relative_to(ROOT)} missing; see the module docstring")
    {"map": cmd_map, "names": cmd_names, "units": cmd_units, "port": cmd_port,
     "clones": cmd_clones}[args.cmd](args)


if __name__ == "__main__":
    main()
