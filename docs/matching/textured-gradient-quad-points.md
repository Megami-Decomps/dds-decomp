# DDS2 textured gradient quad points

`evtSubmitTexturedGradientQuadPoints` at `0x00109028` is ordinary matching C,
544 native bytes. This continues tge-was-taken's retained reconstruction in
`a62382e057`; it does not claim the recovered source spelling is original.
The function is DDS2-only and has no direct caller to establish a different
interleaving of its separate integer and floating-point argument streams.
Its existing signature is retained.

## Coordinate lifetime

Scale each of the eight independent float screen coordinates before assembling
the primitive-call arguments. X uses `16.0f`, Y uses `8.0f`. The primitive
arguments then convert each scaled coordinate to `s32` and add the integer
origin, `0x7000` for X and `0x7900` for Y. In particular, the origin is not
added before conversion, and the pixel value is not converted before scaling.

All eight scaled parameters have real consumers. An independent ordinary
control with eight signed fixed-point locals also matches, following the
nearby matched rectangle emitter's representation. In-place scaling is the
smaller form. The old nested argument expressions left 25 of 136 instruction
words different at the same 544-byte extent.

Texture coordinates still use the original shifted left/top plus shifted
width/height construction. Primitive vertices are TL/TR/BL/BR, paired with
colors 0/1/3/2. The draw tag allocation, texture link, primitive link, and current
surface callback remain in their original order. No parameter type, data
owner, external declaration, compiler flag, constraint, or assembly body was
introduced or changed.

## Stock compiler qualification

The final full owner, original control, and fixed-point-local control each
have ordinary/probed assembly equality after normalizing only the scratch
`.file` path. Their allocated section bytes/metadata and allocated relocation
contracts agree, including 1,157 text and 20 readonly relocations. The final
owner has six allocated sections. The probes use the normal pinned ee-gcc
wrapper with stock RTL dumps; no debugger or instrumentation is involved.

In final `00.rtl`, the eight incoming float values are pseudos 84 through 91.
Instructions 76, 81, 86, 91, 96, 101, 106 and 111 scale these actual values;
the first integer conversion is instruction 129. This separates coordinate
preparation from primitive argument construction. Ordinary scheduling then
produces the native saved-FPR multiplication, conversion, and stack-argument
order without additional work or register restrictions.

## Verification

- Complete DDS2 `kernel/dds3KernelDraw` owner: **104 match, 0 differ**, including
  owned data and relocation-aware comparisons.
- Both complete retail targets pass their configured SHA-1 checks.
- **445 tests in 118 batches: OK**; `git diff --check` is clean.
- Independent semantic review approves both ordinary coordinate forms and
  prefers the minimal in-place scaling.

The compact receipt is
`docs/compiler-evidence/textured-gradient-quad-points/qualification.json`.
Full sources, controls, plain objects, stock dumps and logs remain in the local
investigation evidence. Nonmatching stat-gauge, menu-row and model-link
experiments from the same investigation are not part of this change.
