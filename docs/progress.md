# Reading progress

- `python tools/progress.py` reports current **source coverage** after splitting:
  game functions and their retail code bytes that no longer use `INCLUDE_ASM`.
  This inventory does not compile or independently verify the functions.
- [decomp.dev](https://decomp.dev/Megami-Decomps/dds-decomp) uses the generated **objdiff comparison reports**.
  Exact matching credits only functions with a 100% comparison result; fuzzy
  matching also gives partial credit for similar instructions. These are
  different measures from source coverage.
- The primary reports cover **Atlus game/engine EE code**, the C reconstruction
  target. Their overall totals and **Atlus game/engine** category contain the
  same units, including unfinished game functions. The headline and code-byte
  badges measure matching bytes; the separate function badges measure matching
  function counts. A short function and a large function contribute equally
  only to the latter.
- **Sony SDK / C runtime** code was linked from prebuilt libraries and remains
  assembly. **VU1 microcode (binary)** is the binary `.vutext` program, which
  splat exposes as a text unit rather than individual EE functions. These are
  outside the primary game-code denominator and `tools/progress.py`, but stay
  in the full-binary audit reports and local objdiff configurations.
- The reports do not currently set objdiff's **complete/linked** metadata.
  A zero there is not a measurement of source coverage or build success.

Objdiff bases are compiled separately with `-DSKIP_ASM`. ee-gcc 2.96 can select
different instructions when the surrounding source or compiler pathnames
change, so an accepted C function can score below 100% in that comparison.
Relocatable objects can also represent the same linked address or literal with
different relocation kinds. `tools/check_unit.py` checks against the linked
retail executable and recognizes these matches. See
[the matching workflow](CONTRIBUTING.md#3-verify) and
[compiler context](idioms.md#code-that-changes-with-unrelated-text-context).
The report generator corrects only explicitly configured relocation-only
cases after resolving every source instruction against that same executable.
Its checked source-object/fallback partition keeps assembly functions in the
denominator with zero C credit. Other context discrepancies remain visible;
use the unit checks and retail checksum build when validating a match.

`ninja report` generates both views: `build/<v>/report.json` is the primary
game-code report, and `build/<v>/report.all.json` retains game, SDK/runtime,
and VU1 units with their separate categories. The root `report.json` combines
both games' full-binary reports. CI publishes the primary files as
`dds1_report` and `dds2_report` for decomp.dev and the full-binary files in the
separate **build-audit** artifact on [the build run](https://github.com/Megami-Decomps/dds-decomp/actions/workflows/build.yml). Objdiff computes
each report's scope and raw totals. `tools/reconcile_report.py` applies the
bounded linked-word corrections to the primary, audit, and combined reports.
