# Building the IRX modules

The IOP modules the games load from `USERIRX/` and `SOUNDIRX/` build
byte-identical from source, with their own compiler, linker script and
container packer. The build is separate from the EE build and touches none of
it.

```
python tools/download_tools.py          # brings in iop-gcc 2.8.1
mkdir -p orig/irx                       # copy the seven .IRX files here
python tools/irx_split.py               # asm from orig/irx/*.IRX
python tools/irx_configure.py
ninja -f build-irx.ninja
```

`orig/irx/` takes the files from either disc: DDS1 and DDS2 ship all seven byte
for byte the same, so there is one config per module under `config/irx/` and
one source file per module under `src/irx/`, not one per game. Like the ELFs,
the retail files are never committed. Each module's yaml holds its SHA-1, and
`ninja` ends in a `verify` step per module that fails on any byte of
difference, so a green build is the proof.

| Module | Origin | Same as SMT3 |
| --- | --- | --- |
| `SDFIOP.IRX` | Atlus, file server | yes |
| `SDFIOPFL.IRX` | Atlus, file loader | yes |
| `SDFSDMAN.IRX` | Atlus, sound driver | no |
| `MODMSIN.IRX` | Sony, message queue | yes |
| `MODMIDI.IRX` | Sony, MIDI sequencer | yes |
| `MODHSYN.IRX` | Sony, SPU2 synthesiser | yes |
| `LIBSD.IRX` | Sony, SPU2 device library | yes |

The tools, the six shared modules' C and their names come from smt3-decomp,
which is authoritative for them: a change to shared text goes there first and is
copied here. Its `docs/irx-modules.md` covers the container, the toolchain and
the splitting, `docs/irx-idioms.md` what iop-gcc 2.8.1 wants, and
`docs/sdf-irx-modules.md` what the modules do.

`tools/check_irx_nonmatching.py` compiles every module's C with `NON_MATCHING`
using the host compiler, so a guarded draft cannot carry an error unseen. CI
runs it; the matching build needs the retail files and runs locally.

## SDFSDMAN

DDS's sound driver is a later revision of SMT3's. `src/irx/SDFSDMAN.c` keeps
SMT3's text and names wherever the code is the same, and the differences are
listed in the file's header comment.
