# Stable coordinate serialization in the effect owner

`sdfDrawTexturedQuad` in DDS2's `game/code_002DE248` remains an exact
1,168-byte function. This change earns no additional matching-function credit.
It removes a demonstrated compilation-context obstacle to replacing earlier
assembly fallbacks in the same owner.

## Why the read representation matters

The four rotated corners are eight independently address-taken `s32` outputs.
The original `(u32)` casts were valid C. With the pinned ee-gcc 2.96, however,
activating an earlier retained C body changed their internal alias metadata.
In qualified traces, coordinate slots that originally had alias set zero
instead acquired invalid values including `4294967296` and
`3451791411077185536`. At `17.sched`, a coordinate load then lost its dependency
on an earlier packet store; scheduling and allocation diverged from retail.
The whole-owner checker correctly rejected the changed quad even though its
source had not been edited.

One-element output arrays and native-layout XY corner records made this
behavior stable across the two compilation contexts, but both emitted 46
different instruction words. Those controls were not adopted.

## Actual packet representation, not an ordering constraint

`sdfCoordinatePacketWord` copies exactly one live coordinate's representation
into the `u32` value consumed by an XYZ2 lane. It is used for all eight existing
coordinate reads. On the pinned 32-bit two's-complement target, every initialized
`s32` representation is the same bit pattern as its conversion modulo 2^32 to
`u32`. Both operands are still widened to `u64` before the high-lane shift.

Each source object is four bytes and each copy is four bytes. There is no
cross-object eight-byte read, extra consumer, artificial value lifetime,
volatile qualifier, register binding, empty asm, or compiler-option change.
The existing SDK `memcpy` prototype is used. The fixed-size copies inline;
the resulting quad has the same eight coordinate loads and four packed XYZ2
stores, with no helper calls or temporary stores. Other arithmetic, rotation
calls, packet fields, and submission calls are unchanged.

This is an ordinary representation-serialization model. It does not repair
source-level aliasing UB: the previous casts were valid. Nor does recovered
alias set zero prove the historical source used `memcpy`. Existing packed-record
serialization examples in `docs/idioms.md` support the operation's legality,
not a claim about original spelling. Pre-existing overflow and floating-point
range assumptions in the rest of the function are unchanged.

## Verification scope

Two complete-owner contexts were checked independently:

- Production source: 712 matching functions, zero differences.
- An isolated parts-editor candidate: the same 712 matching functions, with
  only that candidate's known 71-word residual. The quad and owned data remain
  exact; no `CONTEXT` diagnostic remains. The editor candidate is not promoted.

Both ordinary and stock RTL-dump compilations were qualified. Entire assemblies
and objects agree except the two random source-filename characters in metadata;
allocated sections and relocations are unchanged. In both contexts, the quad's
entire emitted assembly agrees with the exact production baseline after only
local-label renumbering. Independent review also compared all eight coordinate
loads and four packed stores directly with the original executable.

The copy expansion gives the actual four-byte reads a defined byte-copy alias
contract, avoiding the corrupted escaped-scalar metadata. This explains the
observed dependency recovery without treating a matching score as proof of
source authenticity. Full owner/data checks, both-game retail builds, and the
normal tooling checks remain mandatory when further C bodies are integrated.

The integrated production change passed its complete 712-function owner check,
both retail checksum builds, both development builds, 442 tooling tests in 117
batches, and compilation of all 85 retained-C units. Rechecking the isolated
parts context with the final helper text leaves only its known 71-word residual.
