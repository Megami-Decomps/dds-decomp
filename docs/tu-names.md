# Unit names and ranges

DDS1 and DDS2 retail and prototype builds have no symbols and no `__FILE__`
strings. The names come from Shin Megami Tensei III: Nocturne, built from the same
engine with the same compiler (ee-gcc 2.96 + ee-as: `sd` callee saves, `move` as
`addu`).

## Sources

| Build | ELF | Date | Use |
|---|---|---|---|
| Nocturne debug build (JP) | `SLPM_652.42` | Dec 26 2002 | 172 `__FILE__` paths, 1,478 code references |
| Nocturne (USA) | `SLUS_209.11` | Sep 3 2004 | Second hop: 3,921 functions byte-identical to DDS1 |
| Nocturne Tsutaya (JP) | `SLPM_652.59` | Dec 13 2002 | Nothing: stripped copy of the debug build |

No function references two files and no two files' reference spans overlap
in the debug build, so each file owns every function between its first and
last reference.

## Projection rules (`tools/nocturne_tus.py`)

- A label moves between builds only through a romwright pair whose code is
  byte-identical after relocation masking (function size at least 16 bytes).
- US Nocturne gets the debug build's labels, keeps each file's largest run, and
  refills spans that do not overlap. Debug-build labels win on conflict.
- In DDS, each file's largest run is its range. It is a lower bound: the file
  can extend into the neighbouring `game/code_<vram>` chunks.

```sh
RW=~/ventris/target/release/romwright-cli
$RW import SLPM_652.42 --name noct --project build/romwright-noct
$RW import SLUS_209.11 --name noctus --project build/romwright-noctus
# plus index-semantics for both, then:
$RW diff dds1 --reference build/romwright-noct --json --project build/romwright > build/noct_diff_dds1.json
$RW diff dds1 --reference build/romwright-noctus --json --project build/romwright > build/noctus_diff_dds1.json
$RW diff noct --reference build/romwright-noctus --json --project build/romwright-noct > build/noctus_diff_noct.json
python3 tools/nocturne_tus.py dds1 --nocturne SLPM_652.42 --us SLUS_209.11
```

## Units in the splat configs

Both games use the same 65 names. Directory = Nocturne's source directory
(`../effect/src/effPCPMisc.c` -> `effect/effPCPMisc`); the `sdf*` engine files
have no directory in Nocturne and live in `sdf/`.

DDS links its files in a different order than Nocturne; whole blocks moved
(`newdata/datCalc`, `model/mdlManager`, `file/`, `mc/`). A projected run was kept
if it has at least 3 functions or its Nocturne order agrees with both neighbours.
Dropped as likely mispairs, in both games: `newbattle/nbSound`, `newbattle/nbMisc`,
`newbattle/nbAiScript`, `effect/effBattleSystem`. DDS rewrote most of
`newbattle/`, so it has no named units.

Ranges widened beyond the projection, using the Persona 4 transfer
(`docs/p4-transfer.md`):

| Unit | DDS1 | DDS2 | Evidence |
|---|---|---|---|
| `script/scrCommonCommand` | to 0x10EEF0 | to 0x10F118 | P4 `scrCommonCommand.c` functions after the Nocturne run; same file on both sides of the gap |
| `interface/itfMesManager` | to 0x19DB88 | to 0x1A5BB8 | P4 `itfMesManager.c` functions after the Nocturne run |

DDS2 ends were found through the identical DDS1 functions (constant offset).
