#!/usr/bin/env python3
"""Re-cut a run of C units at the boundaries in a list: `python3 tools/resplit.py dds1 config/dds1/sdf_units.txt`.

Each list line is `0x<vram> <unit>`, `-` for code that keeps its old unit. Run it on a tree configured
for the old layout (it reads the retail asm), then `configure.py --force-split` and check every unit.
"""
import argparse
import bisect
import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from shared_funcs import TOKENS, blocks, declared_name  # noqa: E402
from split_unit import ROW, retail_references, split_source  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
VERSIONS = json.loads((ROOT / "config/versions.json").read_text())
ROM = 0xFF000  # both ELFs load .text at file offset + 0xFF000
TEXT_ROW = re.compile(r"^      - \[0x([0-9A-Fa-f]+), (c|asm|hasm), ([^\]]+)\](.*)$", re.M)
DATA_ROW = re.compile(r"^      - \[0x([0-9A-Fa-f]+), \.(rodata|lit4|sdata|data), ([^\]]+)\](.*)$", re.M)
BSS_ROW = re.compile(r"^(      - \{ start: 0x[0-9A-Fa-f]+, type: \.s?bss, vram: )0x([0-9A-Fa-f]+), name: ([^ }]+) \}(.*)$", re.M)
ALIAS = re.compile(r"\b(\w+)\s*\([^()]*\)\s*(?:__asm__|asm)\s*\(\s*\"(\w+)\"\s*\)")
LABEL = re.compile(r"^(?:dlabel|glabel|jlabel) (\w+)\n\s*/\* [0-9A-F]+ ([0-9A-F]{8})", re.M)
REF = re.compile(r"/\* [0-9A-F]+ ([0-9A-F]{8}) [0-9A-F]{8} \*/.*?\b\w+?_([0-9A-F]{8})\b|"
                 r"/\* [0-9A-F]+ ([0-9A-F]{8}) [0-9A-F]{8} \*/.*?%\w+\((\w+)\)")


def read_list(path):
    rows = []
    for line in Path(path).read_text().splitlines():
        line = line.split("#")[0].strip()
        if line:
            addr, unit = line.split()
            rows.append((int(addr, 16), unit))
    return rows


def plan(segments, targets):
    """New text segments as (start, kind, name, [(old name, lo, hi)]), and warnings."""
    ends = [s[0] for s in segments[1:]] + [None]
    old = [(s, k, n, e) for (s, k, n), e in zip(segments, ends)]
    lo, hi = targets[0][0], targets[-1][0]
    if targets[-1][1] != "-":
        sys.exit("the list must end with a `-` row")
    target_at = lambda a: next((u for t, u in reversed(targets) if t <= a), None) if lo <= a < hi else None
    cuts = sorted({s for s, *_ in old} | {t for t, _ in targets})
    out, warnings = [], []
    for i, start in enumerate(cuts):
        end = cuts[i + 1] if i + 1 < len(cuts) else None
        s, kind, name, oend = next(o for o in old if o[0] <= start and (o[3] is None or start < o[3]))
        unit = target_at(start)
        if kind != "c" or name.startswith("hand/"):
            if unit and unit != "-":
                warnings.append(f"{unit}: {kind} segment {name} at 0x{start:08X} stays its own segment")
            out.append([start, kind, name, []])
            continue
        piece = (name, start, end)
        prev = out[-1] if out else None
        if unit and unit != "-":
            if prev and prev[1] == "c" and not prev[2].startswith("hand/") and target_at(prev[0]) == unit:
                prev[3].append(piece)
                continue
            if any(o[2] == unit for o in out):
                fallback = name if start == s else f"game/code_{start:08X}"
                warnings.append(f"{unit}: C at 0x{start:08X} after an asm segment becomes {fallback}")
                unit = fallback
            out.append([start, "c", unit, [piece]])
        else:
            if prev and prev[1] == "c" and prev[3] and prev[3][-1][0] == name and target_at(prev[0]) in (None, "-") \
                    and (target_at(start) == target_at(prev[0])):
                prev[3].append(piece)
                continue
            out.append([start, "c", name if start == s else f"game/code_{start:08X}", [piece]])
    # Collapse consecutive pieces of one old unit.
    for seg in out:
        merged = []
        for p in seg[3]:
            if merged and merged[-1][0] == p[0] and merged[-1][2] == p[1]:
                merged[-1] = (p[0], merged[-1][1], p[2])
            else:
                merged.append(p)
        seg[3] = merged
    return out, warnings


REGIONS = {}
MARKER = re.compile(r"^.*(?:/\*@@(\S+?)@@\*/|#include \"@@(\S+?)@@\").*$", re.M)


def collapse(unit, text):
    """Each top-level #if region as one line split_source can carry: its INCLUDE_ASM line
    followed by the identifiers the region uses, or an #include placeholder."""
    out, cur, depth = [], [], 0
    for line in text.split("\n"):
        s = line.strip()
        if depth == 0 and not re.match(r"#\s*if", s):
            out.append(line)
            continue
        cur.append(line)
        if re.match(r"#\s*if", s):
            depth += 1
        elif re.match(r"#\s*endif", s):
            depth -= 1
            if depth == 0:
                key = f"{unit}#{len(REGIONS)}"
                REGIONS[key] = "\n".join(cur)
                asm = [l.strip() for l in cur if re.match(r"INCLUDE_ASM\(", l.strip())]
                if asm:
                    words = sorted({w for w in TOKENS.findall(REGIONS[key]) if re.fullmatch(r"[A-Za-z_]\w*", w)})
                    out.append(f"{asm[-1]} /*@@{key}@@*/ " + " ".join(words))
                else:
                    out.append(f'#include "@@{key}@@"')
                cur = []
    return "\n".join(out)


def expand(text):
    return MARKER.sub(lambda m: REGIONS[m[1] or m[2]], text)


def cut(version, unit, text, names, points):
    """The unit's C cut at each address in points: [(lo, text)], lo of the first is None."""
    text = collapse(unit, text)
    refs = retail_references(version, unit)
    parts, rest, name = [], text, unit
    for p in points:
        result, counts, new = split_source(rest, names, p, None, name, refs)
        parts.append((counts["head"], result["head"]))
        rest, name = result["mid"], new["mid"]
        parts[-1] = parts[-1] + (p,)
    parts.append((None, rest, None))
    return parts, name


def data_definition(b):
    """Name a top-level block defines as data, or None."""
    code = re.sub(r"/\*.*?\*/|//[^\n]*", "", b, flags=re.S).strip()
    if not code.endswith(";") or re.match(r"(extern|typedef|struct|union|enum|#|INCLUDE_)\b", code):
        return None
    if "(" in code.split("=", 1)[0]:
        return None
    return declared_name(b)


def rehome_definitions(texts):
    """Move each data definition to the first part whose code uses it, at that part's end."""
    parts = [blocks(t) for t in texts]
    moves = []
    for i, part in enumerate(parts):
        for b in part:
            name = data_definition(b)
            if not name:
                continue
            users = [j for j, other in enumerate(parts)
                     if any(name in TOKENS.findall(o) for o in other
                            if o is not b and not o.lstrip().startswith("extern"))]
            if users and i not in users:
                moves.append((i, users[0], b))
    for i, j, b in moves:
        parts[i].remove(b)
        parts[j].append(b)
    if not moves:
        return texts
    return ["\n\n".join(p) + "\n" for p in parts]


def normalise(block):
    return re.sub(r"\s+", " ", block).strip()


ENUM = re.compile(r"\benum\b[^{;]*\{([^}]*)\}", re.S)


def value(text):
    text = re.sub(r"\s+", "", text or "")
    try:
        return int(text, 0)
    except ValueError:
        return text


def enum_values(text):
    """name -> value text of every enumerator in text's enum definitions."""
    out = {}
    for body in ENUM.findall(re.sub(r"/\*.*?\*/|//[^\n]*", "", text, flags=re.S)):
        for entry in body.split(","):
            if m := re.match(r"\s*(\w+)\s*(?:=\s*(.*?))?\s*$", entry, re.S):
                out[m[1]] = value(m[2])
    return out


def header_enums(includes, seen=None):
    """Enumerators the included headers define, following their own includes."""
    seen = set() if seen is None else seen
    out = {}
    for line in includes:
        m = re.match(r'#include "([^"]+)"', line)
        path = ROOT / "include" / m[1] if m else None
        if not path or path in seen or not path.exists():
            continue
        seen.add(path)
        text = path.read_text()
        out.update(header_enums(re.findall(r'^#include "[^"]+"', text, re.M), seen))
        out.update(enum_values(text))
    return out


def drop_enumerators(b, known, conflicts, unit):
    """b without the enumerators known already defines with the same value; None if empty."""
    m = re.match(r"(\s*(?:/\*.*?\*/\s*)*(?:typedef\s+)?enum\b[^{]*\{)(.*)(\}[^}]*)$", b, re.S)
    if not m or "/*" in m[2] or "//" in m[2]:
        return b
    entries = [e for e in m[2].split(",") if e.strip()]
    keep = []
    for e in entries:
        n = re.match(r"\s*(\w+)\s*(?:=\s*(.*?))?\s*$", e, re.S)
        if n and n[1] in known:
            if n[2] is None or known[n[1]] != value(n[2]):
                conflicts.append(f"{unit}: enumerator {n[1]} defined twice")
                keep.append(e)
            continue
        keep.append(e)
    if keep == entries:
        return b
    if not keep and not re.search(r"\w", m[3].strip("};\n ")) and "typedef" not in m[1]:
        return None
    return m[1] + ",".join(keep) + ("\n" if keep else "") + m[3]


def join(texts, unit):
    """Concatenate C parts: includes once at the top, a block already seen dropped."""
    includes, body, seen, defined, conflicts = [], [], set(), {}, []
    for text in texts:
        for b in blocks(text):
            if b.startswith("#include"):
                if b not in includes:
                    includes.append(b)
    known = header_enums(includes)
    for text in texts:
        for b in blocks(text):
            if b.startswith("#include"):
                continue
            if ENUM.search(b) and not b.startswith("#"):
                b = drop_enumerators(b, known, conflicts, unit)
                if b is None:
                    continue
                known.update(enum_values(b))
            key = normalise(b)
            if b.startswith("#") and not b.startswith("#define"):
                body.append(b)
                continue
            if key in seen:
                continue
            seen.add(key)
            if "{" in b and re.match(r"\s*(?:/\*.*?\*/\s*)*(?:typedef|struct|union|enum)\b", b, re.S):
                name = declared_name(b)
                if name and name in defined and defined[name] != key:
                    conflicts.append(f"{unit}: two definitions of {name}")
                defined.setdefault(name, key)
            body.append(b)
    includes = [expand(b) for b in includes]
    body = [expand(b) for b in body]
    body = [re.sub(r'(INCLUDE_(?:ASM|RODATA|SDATA)\([^,]+, ")[^"]+(")', rf"\g<1>{unit}\g<2>", b) for b in body]
    return "\n".join(includes) + "\n\n" + "\n\n".join(body) + "\n", conflicts


def data_symbols(version):
    """Address-sorted (address, name) of every data label splat wrote."""
    found = {}
    for path in (ROOT / "asm" / version / "data").rglob("*.s"):
        for n, a in LABEL.findall(path.read_text()):
            found.setdefault(int(a, 16), n)
    return sorted(found.items())


def references(version, names):
    """(pc, target address) of every retail code reference to a data symbol."""
    out = []
    for path in (ROOT / "asm" / version).rglob("*.s"):
        if path.relative_to(ROOT / "asm" / version).parts[0] in ("data", "nonmatchings", "matchings"):
            continue
        for m in REF.finditer(path.read_text()):
            if m.group(1):
                out.append((int(m.group(1), 16), int(m.group(2), 16)))
            elif m.group(4) in names:
                out.append((int(m.group(3), 16), names[m.group(4)]))
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("version")
    ap.add_argument("list")
    ap.add_argument("-n", "--dry-run", action="store_true", help="print the plan only")
    args = ap.parse_args()
    v = args.version
    yaml_path = ROOT / "config" / v / f"{VERSIONS[v]['serial']}.yaml"
    yaml = yaml_path.read_text()
    segments = [(int(m[1], 16) + ROM, m[2], m[3]) for m in TEXT_ROW.finditer(yaml)]
    targets = read_list(args.list)
    new, warnings = plan(segments, targets)
    changed = [s for s in new if s[1] == "c" and (len(s[3]) != 1 or s[3][0][0] != s[2] or
                                                  s[3][0][1] != next(o[0] for o in segments if o[2] == s[3][0][0]))]
    touched_old = {p[0] for s in changed for p in s[3]}
    for w in warnings:
        print("note:", w)
    for s in new:
        if s[1] == "c" and s[3] and any(p[0] in touched_old for p in s[3]):
            print(f"0x{s[0]:08X} {s[2]:28} " + " + ".join(f"{p[0]}[0x{p[1]:08X}]" for p in s[3]))
    if args.dry_run:
        return

    names = {n: int(a, 16) for n, a in ROW.findall((ROOT / "config" / v / "symbol_addrs.txt").read_text())}
    src = ROOT / "src" / v
    # Cut every touched old unit at the starts of the new units inside it.
    pieces = {}
    for unit in sorted(touched_old):
        text = (src / f"{unit}.c").read_text()
        local = dict(names)
        for fn, alias in ALIAS.findall(text):
            if alias in names:
                local[fn] = names[alias]
        start = next(o[0] for o in segments if o[2] == unit)
        points = sorted({p[1] for s in new for p in s[3] if p[0] == unit and p[1] != start})
        parts, _ = cut(v, unit, text, local, points)
        los = [start] + points
        texts = rehome_definitions([text_part for _, text_part, _ in parts])
        for lo, text_part in zip(los, texts):
            pieces[(unit, lo)] = text_part
    conflicts = []
    written = set()
    for s in new:
        if s[1] != "c" or not any(p[0] in touched_old for p in s[3]):
            continue
        text, bad = join([pieces[(p[0], p[1])] for p in s[3]], s[2])
        conflicts += bad
        path = src / f"{s[2]}.c"
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text)
        written.add(s[2])
    for unit in touched_old - written:
        (src / f"{unit}.c").unlink()

    # Owner of each old unit's code, by address: the new unit holding that address.
    starts = [(s[0], s[2]) for s in new if s[1] == "c"]
    order = {n: i for i, (_, n) in enumerate(starts)}
    owner_at = lambda pc: next((n for a, n in reversed(starts) if a <= pc), None)
    old_new = {u: sorted({s[2] for s in new for p in s[3] if p[0] == u}, key=order.get) for u in touched_old}

    # Data rows: rename, and split a row whose old unit's code went to several units.
    syms = data_symbols(v)
    refs = None
    row_starts = sorted(int(r, 16) for r in re.findall(r"^\s+- (?:\[|\{ start: )0x([0-9A-Fa-f]+),", yaml, re.M))
    rows = [(m.start(), m.end(), int(m[1], 16), m[2], m[3], m[4]) for m in DATA_ROW.finditer(yaml)]
    edits = []
    for s0, s1, rom, section, unit, rest in rows:
        if unit not in touched_old:
            continue
        targets_of = old_new[unit]
        if len(targets_of) == 1:
            edits.append((s0, s1, [(rom, targets_of[0])], section))
            continue
        lo_v = rom + ROM
        hi_v = next(r for r in row_starts if r > rom) + ROM
        if refs is None:
            refs = references(v, {n: a for a, n in syms})
        # As split_rodata.py: a symbol belongs to the first part referencing it, owners
        # must not decrease along the row, and the rest stay with the part before them.
        inside = [a for a, _ in syms if lo_v <= a < hi_v]
        owner = {}
        for pc, target in refs:
            if lo_v <= target < hi_v and owner_at(pc) in targets_of:
                sym = max((a for a in inside if a <= target), default=None)
                if sym is not None:
                    k = targets_of.index(owner_at(pc))
                    owner[sym] = min(owner.get(sym, k), k)
        owned = [a for a in inside if a in owner]
        tails, tail_at, parent = [], [], {}
        for a in owned:
            i = bisect.bisect_right(tails, owner[a])
            parent[a] = tail_at[i - 1] if i else None
            if i == len(tails):
                tails.append(owner[a])
                tail_at.append(a)
            else:
                tails[i], tail_at[i] = owner[a], a
        keep, a = set(), tail_at[-1] if tail_at else None
        while a is not None:
            keep.add(a)
            a = parent[a]
        out, current = [(rom, targets_of[0])], 0
        for a in inside:
            k = owner.get(a) if a in keep else None
            if k is None or k <= current:
                continue
            start = a
            if section == "sdata" and start % 4:
                start = next((b for b in inside if b > start and b % 4 == 0), start)
            if section == "rodata" and any(n.startswith("jtbl_") for b, n in syms if b == start) and start % 16:
                start = next((b for b in inside if b > start and b % 16 == 0), start)
            if start == lo_v:
                out = []
            out.append((start - ROM, targets_of[k]))
            current = k
        edits.append((s0, s1, out, section))
    for s0, s1, out, section in reversed(edits):
        yaml = yaml[:s0] + "\n".join(f"      - [0x{r:X}, .{section}, {t}]" for r, t in out) + yaml[s1:]
    # Consecutive rows of one section and one owner merge into the first.
    lines, last = [], None
    for line in yaml.split("\n"):
        m = DATA_ROW.match(line)
        key = (m[2], m[3]) if m else None
        if key and key == last:
            continue
        last = key if m else (last if not line.strip().startswith("- [") else None)
        lines.append(line)
    yaml = "\n".join(lines)

    def bss(m):
        unit = m[3]
        if unit in touched_old:
            targets_of = old_new[unit]
            if len(targets_of) > 1:
                conflicts.append(f"{unit} {m[1].split('type: ')[1].split(',')[0]}: split between {targets_of}; "
                                 f"kept with {targets_of[0]}")
            unit = targets_of[0]
        return f"{m[1]}0x{m[2]}, name: {unit} }}{m[4]}"
    yaml = BSS_ROW.sub(bss, yaml)

    # Text rows: the old rows from the first touched segment to the last, replaced.
    text_rows = list(TEXT_ROW.finditer(yaml))
    first = min(int(m[1], 16) for m in text_rows if m[3] in touched_old) + ROM
    last = max((s[0] for s in new if s[1] == "c" and any(p[0] in touched_old for p in s[3])))
    keep_rows = [m for m in text_rows if first <= int(m[1], 16) + ROM <= last]
    comment = {m[3]: m[4] for m in keep_rows}
    rows_out = []
    for s in new:
        if first <= s[0] <= last:
            note = comment.get(s[2], "") if s[1] != "c" or s[2] not in written else (
                "  # SMT3 unit (sdf-decomp tools/twins.py runs)" if s[2].startswith("sdf/") else "")
            rows_out.append(f"      - [0x{s[0] - ROM:X}, {s[1]}, {s[2]}]{note}")
    yaml = yaml[:keep_rows[0].start()] + "\n".join(rows_out) + yaml[keep_rows[-1].end():]
    yaml_path.write_text(yaml)

    # Unit names elsewhere: an old unit whose code went to one new unit is renamed.
    renames = {u: t[0] for u, t in old_new.items() if len(t) == 1 and t[0] != u}
    if renames:
        pattern = re.compile(r"(?<![\w/])(" + "|".join(map(re.escape, sorted(renames, key=len, reverse=True)))
                             + r")(?![\w/])")
        for path in [ROOT / "config" / v / "cflags.txt", ROOT / "config" / v / "devbuild.json"]:
            if path.exists():
                path.write_text(pattern.sub(lambda m: renames[m[1]], path.read_text()))
    for u, t in old_new.items():
        if len(t) > 1:
            print(f"split: {u} -> {', '.join(t)}")
    for c in conflicts:
        print("CONFLICT:", c)
    print(f"{len(written)} units written, {len(touched_old - written)} removed")


if __name__ == "__main__":
    main()
