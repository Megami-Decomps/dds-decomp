# Reading progress

## The headline: verified production C

The [decomp.dev tracker](https://decomp.dev/Megami-Decomps/dds-decomp) and README
badges measure **production-exact, source-owned game code**. A function receives
its full retail byte size only when the normal production build proves that
an eligible C function supplies those exact linked bytes. Unfinished
`INCLUDE_ASM` functions stay in the denominator and receive zero C credit,
even when their assembly is byte-identical. Function-count badges use the same
ownership rule; a short and a large function each count once there.

The primary reports cover **Atlus game/engine EE code**, the C reconstruction
target. SDK/runtime libraries and binary VU1 microcode are excluded from this
denominator. They remain in the full-binary audit reports and local objdiff
projects, under separate categories.

## Why the old numbers disagreed

`python tools/progress.py` inventories retail functions no longer named by an
`INCLUDE_ASM` directive. It does not compile or verify C. This source coverage
is useful, but cannot establish a match by itself.

Local objdiff bases are compiled separately with `-DSKIP_ASM`. Removing the
assembly changes compiler context; ee-gcc 2.96 can then select different
instructions. Relocatable objects can also encode the same final address or
literal with different relocation representations. GCC additionally gives
nested C functions local suffixes such as `.0`, which can prevent name-based
pairing. The production function can be exact in all these cases.

Raw objdiff **exact** credit requires a function's entire comparison score to
be 100%. A small comparison difference therefore removes the function's whole
size from exact progress, not just the differing instructions. **Fuzzy** credit
is a separate, partially weighted diagnostic.

Earlier reports reconciled only the functions explicitly listed in a manual
manifest. That fixed a bounded set of relocation differences but left the
same systematic problem elsewhere. The production-proof pipeline replaces
that exception list for every game-code unit automatically. Source coverage
and production-exact coverage should agree when every inventoried function
has eligible, complete production proof. They are not equated by assumption:
assembly-heavy C wrappers can be ineligible, and missing or mismatched proof
fails report generation.

The first published increase from this change is an **accounting correction
for already reconstructed, verified code**, not new decompilation work.
Historical tracker points used the older comparison rule; the raw reports
remain available to inspect that difference.

## What the proof checks

`tools/production_report.py` requires all of the following:

1. A receipt written by the successful production compile binds its object,
   compiler assembly, source, headers, assembly includes and per-unit flags.
   A receipt written by the successful link binds those inputs, the linked
   ELF and its map. Changed or missing inputs invalidate the evidence.
2. Compiler-generated `.ent` declarations outside `#APP` blocks identify C
   providers. Every other function must have an actual assembly include in
   that compilation. Together they must exactly partition the report's
   functions and sizes, and all linked game-code units must be present in
   the report. Configured games are supplied by the build separately from the
   raw report, so omitting an entire game also fails validation. The existing `check_unit` ASMBODY policy and documented
   `libvu0`/`vu0 routine` exceptions also apply.
3. Each production object's function span must match the unit's link-map
   contribution, retail symbol address, size, and linked ELF symbol. Nested
   `.N` names are accepted only when this full identity is unambiguous.
4. Every linked function's bytes match retail. The complete file-backed load
   image, including data and literal pools, must also equal the pinned retail
   executable. The linker resolves relocations; the reporter never masks
   instruction bits or maintains a second partial relocation implementation.

A checksum pass alone is insufficient: an all-assembly build still earns zero
C credit. Missing, stale, ambiguous or mismatching evidence is a build error,
not permission to promote a function. This also means publishing reports
requires the matching production build and locally available retail inputs.
`ninja objdiff` and the raw report targets remain available for comparison
work without production-proof publication.

## Reports and diagnostics

`ninja report` generates:

- `build/<v>/report.json`: primary game-code report, published to decomp.dev as
  `dds1_report` or `dds2_report`.
- `build/<v>/report.all.json`: game, SDK/runtime and VU1 audit report.
- `report.json`: combined full-binary report for the configured games.
- Each report's `.raw` file: untouched SKIP_ASM objdiff comparison output.
- Each report's `.proof.json` file: per-function C/assembly ownership,
  production identity, exact result and original raw comparison score.

The standard objdiff report schema, function sizes, denominators and category
membership are preserved. Published game-function comparison fields represent
production-exact C credit (100 or 0); the raw files retain the original fuzzy
scores. Data comparison measures remain raw objdiff diagnostics and do not
measure C ownership. Objdiff's complete/linked metadata is not set; a zero
there does not describe source coverage or build success.

CI includes the raw reports and non-binary function-proof summaries in the
**build-audit** artifact on the [build run](https://github.com/Megami-Decomps/dds-decomp/actions/workflows/build.yml).
Compile/link receipts and private retail or generated binary/assembly files
are local build evidence and are not uploaded.

For reconstruction checks, continue to use
[`check_unit` and the retail checksum build](CONTRIBUTING.md#3-verify).
