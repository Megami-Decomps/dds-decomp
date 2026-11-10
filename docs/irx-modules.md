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

DDS's sound driver is a later revision of SMT3's. Its symbols are SMT3's,
mapped onto DDS's file by aligning the two `.text` sections, and
`src/irx/SDFSDMAN.c` keeps SMT3's text for every function whose code is the
same in both. Of the 72 functions, 22 are SMT3's matching C, 13 are SMT3's
drafts under `NON_MATCHING`, and 37 are `INCLUDE_ASM` of DDS's own code
because DDS changed them:

`sdfSdManInit`, `sdfSndPostError`, `sdfSndUpdateVoiceMap`,
`sdfSndGetVolTable`, `sdfSndMidiCbMessage`, `sdfSndSeqThread`,
`sdfSndStopAll`, `sdfSndInitSynth`, `sdfSndFindSlotById`,
`sdfSndBgmPlay`, `sdfSndSaveResumePoint`, `sdfSndBgmResume`,
`sdfSndBgmStop`, `sdfSndHsynLoad`, `sdfSndSePlay`,
`sdfSndUpdate3dVoice`, `sdfSndSePlay3d`, `sdfSndIsBankLoaded`,
`sdfSndLoadSmg`, `sdfSndSetBankAddr`, `sdfSndStopByGroup`,
`sdfSndUnloadBank`, `p2sdIrxSpuIopMap`, `sdfSndFadeOutById`,
`sdfSndAllocBgmSlot`, `sdfSndAllocSeSlot`, `sdfSndSetSeqVolume`,
`sdfSndFadeOutAll`, `sdfSndFadeInAll`, `sdfSndLoadVolTables`,
`sdfSndStoreVolTables`, `sdfAdpcmFadeOut`, `sdfAdpcmStop`,
`sdfAdpcmFillBuffer`, `sdfAdpcmSetVolume`, `sdfAdpcmStreamPlay`,
`sdfAdpcmStreamInit`.

What changed, as far as the differences show:

- 13 bank kinds instead of 7 (`SDF_SND_KINDS` in `include/irx/sdfsnd.h`), so
  the bank tables, the work area's `stat` and `volTable`, and the volume tables
  grew. Kinds 3 and 4 have 32-entry volume tables, kinds 5 to 12 two entries
  each; the MIDI slot names in `.rodata` run `A1 A2 E1 E2 F1 F2 G1 G2 H1 H2 I1
  I2 AD`.
- `sdfSndSlot_t` is 0x44 bytes, 0x34 in SMT3; the extra 16 bytes sit at the
  end, since the fields SMT3 knows keep their offsets.
- one bank-loaded flag instead of two: DDS uses the one SMT3 calls
  `sdfSndBank1Loaded`.
- the BGM paths take `CpuSuspendIntr` locks SMT3 does not.
- the ADPCM stream player lost most of its logging (the fade, stop, mono,
  stereo and frequency messages, and `sdfAdpcmFreqNames`).

The import tables are identical. ATRAC3 is not here: DDS decodes its `.at3`
streams on the EE (`libat3dec.c`, `/soundat3/` in the ELF's strings), so this
driver only sees what the EE hands it.
