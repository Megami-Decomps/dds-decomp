# Field map icons: real texture endpoints and the owned DDS2 texture

The three native routines are DDS1 `fldAutoMapDrawIconOfsRF` at `0x001428C0`
(828 bytes), DDS2 `func_00145A08` at `0x00145A08` (832 bytes), and DDS2
`func_00145D48` at `0x00145D48` (912 bytes). The last takes an additional
alpha argument. The reconstruction preserves the earlier retained source's
geometry, callback order, aligned corner arrays and shared VU helpers.

## Local constraints on the final UV endpoints

Each icon loads signed texture coordinates and dimensions from the established
`FldProjectedSprite` record. The final right and bottom endpoints are
`textureU + textureWidth` and `textureV + textureHeight`, in sixteenth-pixel
units. Each is initialized unconditionally before `SetRotateMatrixY` and is
used twice in the real `sdfPrimLinkFt4` arguments. No later statement reads
those endpoints after the draw call. The intervening matrix routine preserves
callee-saved integer registers; the VU helpers do not clobber them.

Two local bindings express the observed compiler allocation dependency:
`textureURight` in `$16` and `textureVBottom` in `$18`. They apply to the
actual endpoint results, not to raw dimensions, a dummy copy or an extended
lifetime. There is no additional operation, memory effect, qualifier, assembly
instruction or consumer. These are compiler-allocation reconstructions, not
recovered original spelling or hardware requirements.

The qualified stock-compiler trace explains the affected quantities. In the
ordinary DDS1 control, pass `19.lreg` places U/V pseudos 92/93 in registers
18/16 and endpoint pseudos 96/97 in 17/19. The dimension-load and shift
pseudos 142/146 share the right endpoint's register 17; 152/156 similarly
share register 19 with the bottom endpoint. The ordinary source therefore
has exactly 20 differing register words, with unchanged 828-byte extent.

The endpoint constraints first appear as truthful hard-register destinations
in pass `00.rtl`, UIDs 209 and 214. By `19.lreg`, the width load/shift receives
16 and the height load/shift receives 18, while U/V receive 19/17. The two
endpoint sums consume those dying dimension values without extra copies.
This reproduces all native words. Removing only one endpoint constraint
leaves 11 words different in each target; the other constraint alone is not
sufficient. Binding the earlier raw dimensions instead changes floating-point
allocation too and is not the accepted form.

Ordinary alternatives retained separate or in-place bounds, computed geometry
before UV coordinates, and reused the index only after taking the actual
sprite pointer. None closed the residual. The accepted source keeps the
original distinct coordinate and dimension roles.

## DDS2 texture ownership

The old retained scalar external `D_00389978` is an interior field, not a
standalone small-data pointer. `fldGlobalBase` is at `0x00389770` and
`fieldTextures` starts at offset `0x1E8`. Each resource is 12 bytes and its
texture pointer is at offset 8. Thus `fieldTextures[2].texture` is precisely
`0x00389978`, at owner offset `0x208`.

The matched resource initializer stores the third `sdfTexSet` result at this
field (native `0x0014BDC4`), and the matched release routine reads, releases
and clears it (`0x0014BE20`, `0x0014BE2C`, `0x0014BE34`). Existing field
renderers read the same member. Both DDS2 icons now use that established
member, removing the false scalar extern. The old scalar declaration chose a
GP-relative access and omitted one instruction; the owned field produces the
native address pair without a section attribute or invented array.

## Alpha accumulation before packing

The alpha variant uses one real color accumulator through its actual stages:
unsigned conversion of the scaled sine, addition of 128, signed multiplication
and division by 128, signed clamp to 0..255, and final shift into the alpha
byte. Conversion and addition are separate ordinary assignments. The native
unsigned conversion's high-range branch is retained, as is signed scaling.
No assumption about sine range removes either conversion arm.

The prior combined local `pulsed` expression left 16 native words different.
Using the existing accumulator for the alpha stages leaves five conversion
register words different until conversion and addition are separately
assigned. This exposes the real conversion destination before the two
unsigned-conversion arms join. It adds no arithmetic or side effect and
requires no additional constraint.

## Validation boundaries

All three targets have exact retail words and extents. Both complete affected
owners and their data must pass, not only the targets. Qualified ordinary and
RTL-dump captures compare full assembly, every allocated section's bytes and
metadata, symbol and relocation contracts, auxiliary metadata and ELF ABI.
The endpoint-removal, texture-owner-removal and conversion-combination
controls are retained with the same qualification. Independent semantic
review and both complete retail builds and repository test suites remain
required before publication.

In the frozen DDS2 comparison, both final conversion arms define color pseudo
101 in pass `00.rtl` (UIDs 51 and 59), followed by its high-bit OR and the
addition (UID 66). The combined-expression control instead defines temporary
111 and assigns color only from `111 + 128`. Pass `20.greg` places color in
21 in both sources, but the control's temporary in 3. That predicts all five
residual words. In `29.dbr`, both forms already use the annulled conversion
branch; the separate form's addition donor uses 21 as both source and
destination, reproducing the native operands. Splitting the expression is
not claimed to change that branch's annul bit.

## Integrated result

Independent semantic review approved the frozen DDS1 source SHA-256
`cc1ac453d2171cfb22e4530d0964cd533f76695fffa51b89e1df40e1decd0409`
and DDS2 source SHA-256
`39b007df9b0325a34c2d54b7bdab0247946142d1aca777111eb34e811cf78adf`.
The complete owners pass 141/0 and 189/0. Both full retail executable SHA-1
checks pass, all 442 tooling tests in 117 batches pass, and all 80 retained-C
units compile. The three icons contribute 2,572 exact bytes. Private binaries,
compiler captures and controls are not part of the source commit.
