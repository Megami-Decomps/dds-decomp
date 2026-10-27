# Triangle-height interpolation

`fldCamCalcSlopeOffset` is exact C in both titles:

- DDS1: `0x00122A00`, 428 bytes, owner `game/code_00122030`.
- DDS2: `0x00124A10`, 428 bytes, owner `game/code_00124040`.

The reconstruction retains the existing two-edge projection algorithm and its
nine coordinate snapshots. The final cross-edge interpolation computes two
real quantities: a vertical correction and the returned height. For a collapsed
projected Z span, the correction is zero; otherwise it is the projected height
difference scaled by the query's displacement across the span.

## Source property that closes the match

Define the final height in both arms of the ordinary `if`, and return that height
at the common continuation. Initialize zero correction only inside the
collapsed-span arm. A correction-only common return lets EE GCC if-convert the
zero assignment, changing its lifetime. Separate direct returns lose the final
height's shared allocation preferences. A conditional expression obtains those
preferences but places the arms in the opposite order.

In the exact EE GCC 2.96 RTL capture, `14.combine` retains both final-height
assignments and `15.ce` performs no conversion of this conditional. By
`20.greg`, the projected heights use f2/f4 and the query's Z displacement uses
f8, matching retail. The zero correction and final result use f0. This also
preserves retail's order of the two final edge divisions and its fallback-first
control-flow layout. The prior reconstructed early-return form was ten words
off; no unrelated source, compiler flags, or layout changes were needed.

The interval test remains strict and ordered: `-0.001f < width < 0.001f`.
Unordered values therefore follow the general path. The collapsed path still
adds its positive-zero correction to the baseline height, preserving retail's
signed-zero behavior; it does not simply return the baseline. The general path
retains the original subtraction, multiplication, division, and addition order.
Both variables hold consumed algorithmic results, not dummy register ties.

## Verification

Both complete owners pass `check_unit.py`: 84 matches and zero differences each
(including the new function). Both retail images retain their pinned SHA-1,
both development images report zero changed payload words, and the tooling
suite passes all 415 tests. The source change replaces only the two assembly
includes; this note records the compiler property rather than claiming recovery
of the original source spelling.

An ordinary RTL capture can be reproduced with `tools/ee_gcc_probe.py` on either
owner, selecting `fldCamCalcSlopeOffset` and its canonical `--as-unit` path.
