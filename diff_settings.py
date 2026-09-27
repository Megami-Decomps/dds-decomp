# asm-differ settings (https://github.com/simonlindholm/asm-differ).
#
#   python3 ../asm-differ/diff.py -mwo func_0019D958            # dds1
#   DDS_VERSION=dds2 python3 ../asm-differ/diff.py -mwo func_...
#
# The built image is a byte-for-byte copy of the retail ELF file, so the ELF
# itself is the "rom": the map's load addresses are file offsets into both.
import json
import os
from pathlib import Path

ROOT = Path(__file__).resolve().parent


def apply(config, args):
    version = os.environ.get("DDS_VERSION", "dds1")
    serial = json.loads((ROOT / "config" / "versions.json").read_text())[version]["serial"]
    config["arch"] = "mipsee"
    config["baseimg"] = f"orig/{version}/{serial}"
    config["myimg"] = f"build/{version}/{serial}"
    config["mapfile"] = f"build/{version}/{serial}.map"
    config["source_directories"] = [f"src/{version}", "include", f"asm/{version}"]
    config["objdump_executable"] = "tools/bin/mips-ps2-decompals-objdump"
    config["make_command"] = ["ninja"]
    config["build_dir"] = f"build/{version}"
