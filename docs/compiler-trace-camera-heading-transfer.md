# DDS2 camera heading: first scalar-transfer allocation

`fldUpdateCameraHeadingTransition` in `src/dds2/field/fldFileResolver.c`
occupies 1,248 retail bytes at `0x00133F08`. This is not the separate
880-byte camera-reset pair.

The match uses **one localized compiler allocation constraint**. It is not
claimed as recovered original-source spelling or a hardware register
requirement. The contribution rule is "Localized compiler constraints on real
values" in [CONTRIBUTING.md](CONTRIBUTING.md).

## Ordinary source reconstruction

The actual axis initializer is the existing 16-byte `D_004132E0` vector
`{0, 1, 0, 1}`. Its copy follows quaternion initialization, as in retail and
the matched DDS1 camera family. The two real heading fields are published in
angle/target-angle source order, with no intervening callback.

These ordinary changes leave exactly 32 differing words at native extent.
They all concern the floating-register homes of sixteen real scalar transfers:
secondary smoothed position, position and rotation, followed by the local
quaternion snapshot. The loads, stores, callbacks and all other target words
already agree with retail. Whole-vector snapshot and reusable vector-copy
controls do not preserve the native transfer sequence. Reusing one ordinary
scalar through each vector instead collapses transfers into one floating
register, also not native.

## Qualified allocation evidence

The real pinned compiler's local-allocation observation gives the first
ordinary transfer a birth/death interval of 10/12 and a padded search interval
of `[8, 14)`. It has two references, no calls and no register preference.
The first free floating register is f0. The following transfer's padded
interval `[12, 18)` overlaps, so it receives f1; subsequent short transfers
alternate. Retail begins with f1 instead.

The final source names only the first actual smoothed-X transfer:

```c
{
    register f32 smoothedX __asm__("$f1") = source->smoothedPosition[0];
    secondary->smoothedPosition[0] = smoothedX;
}
```

There is one real definition and one immediate real consumer. No instruction,
extra load, store, arithmetic, fabricated consumer, helper call or lifetime
extension is added. The value is dead before either setter call. No other
component is bound, and the remaining fifteen transfer statements are
unchanged. This preserves sequential behavior even when source and destination
are the same pose object.

The otherwise identical removal control declares plain `f32 smoothedX`.
It retains the same 32-word difference. Adding the single binding makes the
whole affected owner exact; the binding is therefore causal, not incidental
syntax attached to an already-matching ordinary source.

## Verification and scope

Both bound and unbound full-owner ordinary builds agree with their respective
instrumented builds in every allocated section, all 2,397 normalized relocation
contracts and assembly after removing debug-path directives. Instrumentation
neutrality is separate from native matching.

The bound owner passes all 160 emitted functions and owned data. Independent
semantic review verifies the initialized value, immediate consumer, unchanged
alias behavior and call-free lifetime. Final validation also passes both games' retail and development build targets,
442 tests in 117 batches, and all 83 retained-C units. Retail SHA1s remain
`6d18898e2724bf1d145392766e8ba1e678487419` (DDS1) and
`9be91ee1b4a535a4cb6ec89237b5a6ba41be2add` (DDS2). No original binaries or
private compiler dumps are part of this source/documentation change.
