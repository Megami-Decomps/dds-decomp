# Camera-track transition gate allocation reconstruction

This change reconstructs `btlScaleCameraTrackValues` in DDS1 (0x001EBE88)
and DDS2 (0x001FC5E0), 3,616 bytes per game. It uses the narrow localized
register-constraint allowance in `docs/CONTRIBUTING.md`. It is **not a claim
that the original source declared registers**, and it is not an ordinary-C-only
match.

## Real values and the localized constraint

The final 0x80 transition test loads an initialized `u8` from the current
camera parameter record's `transitionFlags`. That byte's real consumer is a
32-bit bitwise mask. The mask's real consumer is the conditional branch into
the cubic-settle calculation.

Two block-scoped register declarations specify the byte's native `$3` home and
the mask's native `$2` home. There is no empty inline-asm statement, emitted
instruction template, dummy input, artificial arithmetic, extra load/store,
new call, or fabricated output. The byte dies at the mask; the mask dies at the
branch. No call or hardware-register operation intervenes while either value
is live. The cubic code subsequently reuses the same registers normally.

Without these constraints, GCC 2.96 coalesces the final byte and its mask into
one local quantity. The register pressure at this gate then also assigns the
preceding record-index shift to `$3` instead of `$2`. That shift is copied into
a preceding timing branch's delay slot. One local allocation choice therefore
changes five instruction words, while the function size stays 3,616 bytes.

The public historical audit in `docs/compiler-trace-fade.md` documents the
same five-word frontier and the real zero-endpoint interpolation operations.
The rehosted baseline reproduces that frontier with current names and headers.

## Reduction controls

All controls below compile in the same complete DDS2 owner and preserve the
same target size. These are reconstruction controls, not proof of historical
source spelling or proof that every ordinary-C formulation has been tried.

| Control | Different target words |
| --- | ---: |
| Ordinary reconstructed baseline | 5 |
| Explicit ordinary byte local | 5 |
| Explicit ordinary mask local | 5 |
| Only byte bound to `$3` | 2 |
| Only mask bound to `$2` | 2 |
| Both local bindings, no asm statements | 0 |

The two bindings are independently necessary in this localized formulation.
Early-clobber and empty-asm experiments were diagnostic controls and were
removed; they are not part of the proposed implementation. Existing real
operations, evaluation order, callbacks, and memory effects are unchanged.

## Compiler causality and verification

The actual pinned compiler's pass-00 RTL represents the two declared real
values in hard registers. In the final 19.lreg gate, the QI load into `$3`
feeds the SI `& 128` result in `$2`, which feeds the real branch. `REG_DEAD`
marks `$3` on the mask and `$2` on the branch; neither is in the successor's
live-out register set. This directly establishes the localized lifetime and
why coalescing no longer aliases the byte and mask.

Against the ordinary baseline, the complete DDS2 owner's allocated code
changes only at target offsets +A2C, +CEC, +CF0, +CF4, and +CF8. The function
symbol map and every other allocated section are unchanged. These are exactly
the five historical register-operand differences; there are no relocation
changes at them.

The exact landing sources pass the complete ordinary owners and owned-data
checks: DDS1 **258 matching, zero differing**; DDS2 **240 matching, zero
differing**. In each title, the ordinary and `-da` compilations have identical
allocated sections and relocation contracts (DDS1: 2,833; DDS2: 3,084), and
identical assembly apart from debug-path directives. The formerly documented
DDS1 dump perturbation does not reproduce with this current landing owner.

The integrated sources also pass both retail checksum builds and both
relocated development builds. The complete tooling suite passes **442 tests
in 117 batches**, and the retained-C compile gate passes **84 units, zero
failures**. Independent semantic and compiler-causality review approved the
minimal two-binding form under the documented exception. This remains an
explicit allocation reconstruction, not evidence of original source text.
