#!/usr/bin/env python3
"""Rename identifiers in the files tools/names.py leaves alone: headers, docs, config.

    python3 tools/rename_refs.py renames.tsv [more.tsv ...]

Rows are <old>\t<new>. Whole-word replacement over include/**/*.h, docs/**/*.md,
README.md and the config text that mentions functions; symbol_addrs.txt and the
splat yaml are never touched, and in name_sources.txt only the note column is.
Run it once per batch on a tree the batch has not touched yet: a chain such as
A->B, B->C applied twice turns A into C. A file containing the line marker below
is left alone, for a doc that records old names on purpose. Never add declarations to a header to cover a rename: an
extra declaration can flip a CONTEXT-sensitive function (docs/idioms.md)."""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
TARGETS = [
    (ROOT / "include", "*.h"),
    (ROOT / "docs", "*.md"),
    (ROOT / "config", "cflags.txt"),
    (ROOT / "config", "not_functions.txt"),
    (ROOT / "config", "devbuild.json"),
    (ROOT / "config", "shared_names_exclude.txt"),
    (ROOT / "README.md", None),
    (ROOT / "permuter_settings.toml", None),
]
NOTES = [(ROOT / "config", "name_sources.txt")]
# A provenance row that names the old name on purpose ("was X") must keep it.
KEEP = "SMT3HD name via "
SKIP_FILE = "<!-- rename_refs: keep -->"


def read_map(paths):
    """First mapping of a name wins; a later file may disagree only on a name the
    two games had assigned to different functions, and the first file settles it."""
    renames = {}
    for path in paths:
        for line in Path(path).read_text().splitlines():
            if not line.strip() or line.startswith("#"):
                continue
            old, new = line.split("\t")[:2]
            if renames.get(old, new) != new:
                print(f"{old}: keeping {renames[old]}, ignoring {new}", file=sys.stderr)
                continue
            renames[old] = new
    return renames


def rewrite(path, pattern, renames, notes_only):
    text = path.read_text()
    if SKIP_FILE in text:
        return 0
    out = []
    for line in text.split("\n"):
        if KEEP in line:
            out.append(line)
            continue
        if notes_only:
            cols = line.split("\t")
            if len(cols) >= 4:
                cols[3] = pattern.sub(lambda m: renames[m[1]], cols[3])
            out.append("\t".join(cols))
            continue
        out.append(pattern.sub(lambda m: renames[m[1]], line))
    changed = "\n".join(out)
    if changed != text:
        path.write_text(changed)
        return 1
    return 0


def main():
    args = sys.argv[1:]
    if not args:
        sys.exit(__doc__)
    renames = read_map(args)
    pattern = re.compile(r"\b(" + "|".join(map(re.escape, sorted(renames, key=len, reverse=True))) + r")\b")
    touched = 0
    for base, glob in TARGETS:
        for path in (base.rglob(glob) if glob else ([base] if base.exists() else [])):
            touched += rewrite(path, pattern, renames, False)
    for base, glob in NOTES:
        for path in base.rglob(glob):
            touched += rewrite(path, pattern, renames, True)
    print(f"{len(renames)} renames, {touched} files changed")


if __name__ == "__main__":
    main()
