# Functions shared with the Persona 4 decomp

Question: which functions matched in the Persona 4 decomp (SLUS_217.82) also
exist in Digital Devil Saga 1 (SLUS_209.74), and does P4's C carry over?

## Method (2026-09-26)

1. **Pairing.** Both executables were imported into romwright (`build/romwright`,
   `build/romwright-p4`, P4 names from its `config/symbol_addrs.txt`), semantic
   signatures were indexed, and `romwright-cli diff dds1 --reference build/romwright-p4`
   matched 1,280 DDS functions to P4 functions (149 exact shape, 36 exact masked,
   rest likely/tentative). 491 of the pairs point at P4 functions that P4 has
   matched in C.
2. **Recompile.** Each of those 491 P4 C bodies was compiled with DDS's toolchain
   (ee-gcc 2.96 `cc1 -O2`, original ee-as `-G8`) inside P4's own header context,
   then compared with the DDS bytes. Relocation fields were masked.
3. **Byte sweep.** P4 functions P4 builds with gcc itself (CRI, `src/sce`) were
   also compared byte-for-byte with every DDS function.

## Result

The two games use different compilers (P4 game code is MWCC; DDS is ee-gcc
2.96) and different engines, so most pairs are only structural look-alikes.
Of the 491 pairs, 325 recompile to different code and 125 are MWCC `asm`
bodies in P4, so there is no C to try. **41 recompile byte-identical.**

| Kind | Count | Notes |
|---|---:|---|
| Non-trivial C (more than 4 instructions) | 16 | sdkTask, scr common commands, itfMesManager, effMiscRand, and more |
| Inline-VU0-asm C (effMisc) | 5 | Bytes come from the asm text, not the compiler |
| `call + sync` wrappers (kernel) | 4 | In the SDK/libkernl range |
| 4 instructions or fewer | 16 | Getters and returns; weak evidence |

The 86 P4 syscall stubs that are byte-identical only give names. romwright's
SDK signatures name those already. The byte sweep found no real CRI or
`src/sce` code: DDS does not link CRI middleware. Its 5 to 16 instruction hits
are generic gcc shapes, with one P4 function "matching" several unrelated DDS
functions.

## Imported into the build

The game-region transfers are in `src/dds1/` and are linked. Units named after
Nocturne's `__FILE__` strings replaced the P4 unit names (see `docs/tu-names.md`).
The retail SHA-1 still matches.

| DDS1 | Size | Persona 4 origin | Unit here |
|---|---:|---|---|
| `func_001019C8` | 0x8C | `func_00452490`, Kernel/sdkTask.c | `kernel/dds3KernelCore.c` |
| `kwlnTaskGetTimer` 0x101A60 | 0x8 | `kwlnTaskGetTimer`, Kernel/sdkTask.c | `kernel/dds3KernelCore.c` |
| `scrCommand_SCR_GET_TIMER` 0x10DD38 | 0x5C | Script/scrCommonCommand.c | `script/scrCommonCommand.c` |
| `scrCommand_SCR_EXISTS` 0x10EEA8 | 0x48 | Script/scrCommonCommand.c | `script/scrCommonCommand.c` |
| `func_0011CE88` | 0x30 | `func_00298d70` | `game/code_0011CE88.c` |
| `func_0014F098` | 0x30 | `func_002993c0`, Script/scrCommonCommand.c | `game/code_0014F098.c` |
| `func_0014F498` | 0x30 | `func_001eb2a0` | `game/code_0014F098.c` |
| `func_0019D160` | 0x48 | `func_00278d50`, itfMesManager.c | `interface/itfMesManager.c` |
| `func_0019D920` | 0x34 | `func_0027a340`, itfMesManager.c | `interface/itfMesManager.c` |
| `func_0019DA50` | 0x50 | `func_0027a4d0`, itfMesManager.c | `interface/itfMesManager.c` |
| `func_0019DB40` | 0x48 | `func_0027a580`, itfMesManager.c | `interface/itfMesManager.c` |
| `func_001F60E8` | 0x28 | `func_001789d0` | `game/code_001F60E8.c` |
| `func_0029A810` | 0x30 | `func_00492e30` | `game/code_0029A810.c` |
| `func_002DD8B8`, `func_002DDC50` | 0x2C, 0x48 | effMisc.c (VU0 asm) | `game/code_002DD8B8.c` |
| `effMiscQuatMultiplyVU`, `effMiscNormalizeVU`, `func_002E7D98` | | effMisc.c (VU0 asm) | `game/code_002E7C20.c` |
| `effMiscRand` 0x2E8340 | 0x54 | effMisc.c | `game/code_002E7C20.c` |

P4's global names were replaced by the DDS symbols at the same relocation
sites. For example, P4's `iGpffffb9ec` becomes DDS `D_003BA800`. The link
checks this, because relocation masking in the comparison alone would accept a
wrong symbol.

Not imported yet: the 4 kernel wrappers and the SDK-range stubs (0x30B108 and
up), which sit inside `sdk/libkernl`, and the trivial game-range stubs
(0x100538, 0x132B60, 0x196BB0, 0x1FB1A8/E0/F0, 0x2055F0, 0x2A8018, 0x2B4B98,
0x2EFEE0). DDS2 was not swept against P4 directly; `tools/shared_funcs.py port`
copied every transfer above except `code_0014F098.c` (its DDS2 copies are not
contiguous) into the identical DDS2 functions.

## Unit names

P4's `sdkTask` functions fall inside Nocturne's `dds3KernelCore.c` range, and
the P4 `scrCommonCommand`/`itfMesManager` functions extend Nocturne's units of
the same name. P4's `effMisc` functions sit among the `sdf*` engine files in
DDS, so their units stay unnamed (`code_<vram>`) until evidence names them.
