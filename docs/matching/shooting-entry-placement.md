# Shooting-entry placement

DDS2 `mnuPlaceShootingEntryObject` at `0x00319A58` is exact C: 1,004 instruction
bytes in `game/code_00316E08` (the next symbol follows four alignment bytes later).
The complete owner passes with 33 matches and zero differences.

The reconstruction uses the existing work-entry, shooting-context, model-node,
model-instance, registry and progress records. Origin coordinates are captured
before the step-state getter; the object is read afterward. Integer coordinates
are converted after the floating addition. Rotations retain signed conversion,
and the countdown test retains signed remainder semantics. Callback boundaries
and fresh post-callback reads are preserved. Keyed motion still runs when the
entry has no object.

## Flag access and decoding

The final instance path needs two ordinary source properties together:

- Take the address of the existing `MenuWorkFlagBits` member, without a cast or
  alternate layout: `MenuWorkFlagBits *flags = &entry->flagsBits`.
- Decode its actual `updated` field before testing the half-count blink state.

The half-count predicate means `halfRemainingCountReached` is set and
`halfRemainingTicks` is zero, exactly `(word & 0x3FE0) == 0x20`. `updated` is bit 1.
These are the named fields written by the existing menu updater.

The model instance comes from `itfClaimFreeModelInstance`; the work entry comes
from a separate 0x1C20-byte pool. Clearing the instance flags cannot alter the
work flags. There is no callback between this clear and the field reads. The
member pointer therefore describes the real object, rather than inventing a
non-aliasing view to steer compilation. This follows the existing union-member
access idiom in `docs/idioms.md`.

The qualified EE GCC trace shows why both properties matter. Access through the
union word gives the flags read alias set zero, adding a false anti-dependence
to the instance store. Taking the actual member address restores its own alias
set. With eager update decoding, the scheduler then selects the native initial
load order; the final update-bit mask fills the condition's delay slot.
Eager decoding alone and a member snapshot alone do not produce the match.

Normal, stock-dump and read-only observed compilations have identical complete
owner payloads. The observer uses the owner's existing compiler options; none
were changed. Both retail images retain their SHA-1, both development builds
have zero changed payload words, and all 415 tooling tests pass.
