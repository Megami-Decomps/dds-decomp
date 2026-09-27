#!/usr/bin/env python3
"""Function/data naming with provenance.

    python3 tools/names.py harvest dds1            # evidence candidates -> build/dds1/name_candidates.tsv
    python3 tools/names.py apply dds1 names.tsv    # rows: <old name|0xADDR> <new> <evidence|inferred> <note>

Every applied name becomes a curated row in config/<v>/symbol_addrs.txt (above the
generated blocks, so it wins) and a line in config/<v>/name_sources.txt:

    0x002CF76C  sdfAddHandler  evidence  string "sdfAddHandler : unknown type %d"
    0x00216CC8  mdlManagerSetFlag  inferred  stores arg to mdl->flags

`evidence` means the binary itself names the function: a message inside it that
carries its own name (`sdfAddHandler : unknown type %d`). Names from other
decomps (Persona 3/4), task labels and our own choices are `inferred`: someone
chose them, following the module-prefix camelCase convention of the
evidence names; revise freely. C references in src/<v> are renamed; unmatched
functions need `python3 configure.py --force-split <v>` so asm files follow.
DDS2 picks names up from DDS1 through `tools/shared_funcs.py names`.

harvest lists strings that name a function (`name : message`, `name()`,
`call name()`, bare module-prefixed identifiers) with the function that
references each one. It proposes nothing on its own: the referencing function
may be a caller or a task registrar, so each row needs a look at the code.
"""
import argparse
import concurrent.futures
import re
import sqlite3
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
RW = str(Path.home() / "ventris/target/release/romwright-cli")
STORE = ROOT / "build/romwright-c"
ROW = re.compile(r"^\s*(\S+)\s*=\s*0x([0-9A-Fa-f]+)\s*;(.*)$")
IDENT = re.compile(r"^[A-Za-z_]\w*$")
CURATED_END = "// BEGIN "  # first generated block (shared or romwright)
# Atlus identifiers: lowercase module prefix, then CamelCase (sdfAddHandler, evtLipsExecFunction).
MODULE_IDENT = r"[a-z][a-z0-9]{1,5}[A-Z]\w{2,}"
NAMING = [
    ("self", re.compile(rf"^\W*({MODULE_IDENT})\s*(?:\(\)|:)")),
    ("call", re.compile(rf"\bcall ({MODULE_IDENT})\(\)")),
    ("bare", re.compile(rf"^({MODULE_IDENT})$")),
]


def symbols(version):
    path = ROOT / "config" / version / "symbol_addrs.txt"
    rows = {}
    for line in path.read_text().splitlines():
        if m := ROW.match(line):
            rows.setdefault(int(m[2], 16), (m[1], m[3].strip()))
    return path, rows


def cmd_harvest(args):
    _, rows = symbols(args.version)
    db = sqlite3.connect(STORE / "project.sqlite")
    pid = db.execute("select id from programs where name = ?", (args.version,)).fetchone()[0]
    funcs = sorted((int(e, 16), s) for e, s in db.execute(
        "select entry, size from functions where program_id = ?", (pid,)))
    starts = [a for a, _ in funcs]

    def owner(addr):
        import bisect
        i = bisect.bisect_right(starts, addr) - 1
        return funcs[i][0] if i >= 0 and addr < funcs[i][0] + funcs[i][1] else None

    out = subprocess.run([RW, "strings", args.version, "--project", str(STORE)],
                         capture_output=True, text=True, check=True).stdout
    found = []
    for line in out.splitlines():
        m = re.match(r"^([0-9a-f]{8})\s+\[\w+\]\s+(.*)$", line)
        if not m:
            continue
        for kind, rx in NAMING:
            if n := rx.search(m[2]):
                found.append((int(m[1], 16), m[2], kind, n[1]))
                break

    def refs(item):
        addr = item[0]
        text = subprocess.run([RW, "xrefs", args.version, "--to", f"{addr:08x}", "--project", str(STORE)],
                              capture_output=True, text=True).stdout
        users = {owner(int(m[1], 16)) for m in re.finditer(r"^([0-9a-f]{8}) --", text, re.M)}
        return item, sorted(u for u in users if u is not None)

    lines = ["# func\tcurrent\tcandidate\tkind\tstring"]
    with concurrent.futures.ThreadPoolExecutor(8) as pool:
        for (saddr, text, kind, cand), users in pool.map(refs, found):
            for u in users or [None]:
                cur = rows.get(u, (f"func_{u:08X}",))[0] if u is not None else "-"
                where = f"0x{u:08X}" if u is not None else "-"
                lines.append(f"{where}\t{cur}\t{cand}\t{kind}\t{text!r}")
    dest = ROOT / "build" / args.version / "name_candidates.tsv"
    dest.parent.mkdir(parents=True, exist_ok=True)
    dest.write_text("\n".join(lines) + "\n")
    print(f"{len(found)} naming strings, {len(lines) - 1} rows -> {dest.relative_to(ROOT)}")


def cmd_apply(args):
    sym_path, rows = symbols(args.version)
    by_name = {name: addr for addr, (name, _) in rows.items()}
    sources = ROOT / "config" / args.version / "name_sources.txt"
    provenance = {}
    if sources.exists():
        for line in sources.read_text().splitlines():
            if line.strip() and not line.startswith("#"):
                provenance[int(line.split()[0], 16)] = line
    renames, curated, errors = {}, {}, []
    for n, line in enumerate(Path(args.file).read_text().splitlines(), 1):
        if not line.strip() or line.startswith("#"):
            continue
        old, new, kind, *note = line.split("\t") if "\t" in line else line.split(None, 3)
        addr = int(old, 16) if old.startswith("0x") else by_name.get(old)
        if addr is None or addr not in rows:
            errors.append(f"line {n}: unknown symbol {old}")
            continue
        if kind not in ("evidence", "inferred") or not IDENT.match(new):
            errors.append(f"line {n}: bad kind or name: {kind} {new}")
            continue
        if by_name.get(new, addr) != addr:
            errors.append(f"line {n}: {new} already names 0x{by_name[new]:08X}")
            continue
        current, attrs = rows[addr]
        if current != new:
            renames[current] = new
        by_name.pop(current, None)
        by_name[new] = addr
        rows[addr] = (new, attrs)
        curated[addr] = f"{new} = 0x{addr:08X}; {attrs}".rstrip()
        provenance[addr] = f"0x{addr:08X}\t{new}\t{kind}\t{' '.join(note).strip()}"
    for e in errors:
        print(e, file=sys.stderr)
    if errors and not args.force:
        sys.exit("nothing applied (use --force to apply the valid rows)")

    # symbol_addrs: drop every other row at a renamed address, insert/replace curated rows.
    lines = sym_path.read_text().splitlines()
    end = next(i for i, line in enumerate(lines) if line.startswith(CURATED_END))
    head, tail = lines[:end], lines[end:]
    keep = lambda line: not ((m := ROW.match(line)) and int(m[2], 16) in curated)
    head, tail = [l for l in head if keep(l)], [l for l in tail if keep(l)]
    marker = "// Names from tools/names.py (provenance: name_sources.txt)"
    if marker not in head:
        head.append(marker)
    at = head.index(marker) + 1
    block = sorted([l for l in head[at:] if ROW.match(l)] + list(curated.values()),
                   key=lambda l: int(ROW.match(l)[2], 16))
    head = head[:at] + block
    sym_path.write_text("\n".join(head + tail) + "\n")

    sources.write_text("# addr\tname\tevidence|inferred\tnote\n" +
                       "\n".join(provenance[a] for a in sorted(provenance)) + "\n")
    if renames:
        pattern = re.compile(r"\b(" + "|".join(map(re.escape, renames)) + r")\b")
        for path in (ROOT / "src" / args.version).rglob("*.c"):
            text = path.read_text()
            changed = pattern.sub(lambda m: renames[m[1]], text)
            if changed != text:
                path.write_text(changed)
    print(f"{len(curated)} names applied, {len(renames)} renamed in src/{args.version}")


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    h = sub.add_parser("harvest")
    h.add_argument("version")
    a = sub.add_parser("apply")
    a.add_argument("version")
    a.add_argument("file")
    a.add_argument("--force", action="store_true", help="apply valid rows despite errors")
    args = ap.parse_args()
    {"harvest": cmd_harvest, "apply": cmd_apply}[args.cmd](args)


if __name__ == "__main__":
    main()
