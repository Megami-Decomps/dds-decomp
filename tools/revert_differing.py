#!/usr/bin/env python3
"""Put every C function check_unit.py reports as differing back to INCLUDE_ASM.

    python3 tools/revert_differing.py src/dds1/<dir>/<unit>.c [...]

Used when integrating work: a unit must only contain C that matches. Structs,
prototypes and matching functions are left as they are.
"""
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from shared_funcs import DEF, ROOT, blocks  # noqa: E402

BAD = re.compile(r"^(?:DIFF|OVER) (\w+) ", re.M)


def revert(path: Path) -> list[str]:
    version = path.resolve().relative_to(ROOT / "src").parts[0]
    unit = path.resolve().relative_to(ROOT / "src" / version).with_suffix("").as_posix()
    out = subprocess.run([sys.executable, str(ROOT / "tools/check_unit.py"), str(path)],
                         capture_output=True, text=True).stdout
    bad = set(BAD.findall(out)) - {"jump", "rodata"}
    if not bad:
        return []
    text = path.read_text()
    for block in blocks(text):
        m = DEF.search(block) if "{" in block else None
        if m and m.group(1) in bad:
            text = text.replace(block, f'INCLUDE_ASM(const s32, "{unit}", {m.group(1)});', 1)
    path.write_text(text)
    return sorted(bad)


if __name__ == "__main__":
    for arg in sys.argv[1:]:
        print(arg, revert(Path(arg)))
