# Alpha-marker current-main replay and owner audit

Contributor: redthing1 / KiB. Continues the held alpha claim [6073746784](https://github.com/Megami-Decomps/dds-collab/issues/14#issuecomment-6073746784), building on Quartz/Spark's reviewed rotated-marker reconstruction and KiB's signed-color improvement. Historical retained source: `25b363b8d79774ce107726301149cc0e1d97652b`, `experiments/kib-alpha-marker/candidate.inc`. Paired-marker negative findings were read from dds-collab `cc61e6513c8a716dab4bcbc62c06c1ddfa20f94b` before this continuation.

## Result

On canonical main `cbc821ea4259005436e3b76da0359a154972f1d3`, both historical and corrected candidates remain 912 executable bytes, with 20 of 228 words different. All differences are recognized GPR-field-only substitutions at offsets 292 through 728. The substitutions remain s0/s1 and s2/s3, with no differing non-register bits. This is a nonmatch; keep the build's INCLUDE_ASM. No new matching credit, image/development qualification, or runtime claim.

Baseline complete owning unit: 168 match / 0 differ. Both candidate complete owning units: 168 older functions match / only the target differs. All 168 older full-object function sizes remain unchanged; owned data and build context are clean. Compared production and ordinary RTL-probe target bytes and all 17 target relocations are equal. Historical and corrected target bytes/relocations are also equal. A separate `-gstabs` probe likewise preserves target bytes/relocations.

These are local checks with the pinned compiler and assembler through the verified private QEMU execution prefix, not a new native-i386 qualification. The prefix changes no compiler/assembler bytes or compiler flags. No private executable, retail input, assembly, object, adapter or dump is included in this source-only packet.

## One justified correction

The retained source predates current packet-owner contracts. The correction:

- Converts the integer allocator result once into `SdfListHead *packetList`.
- Gives the local `sdfQueueTexturedQuad` declaration its actual `SdfListHead *` first argument.
- Passes the existing `SdfPoolNode *appendTarget` directly to the append callback.
- Removes redundant packet-list casts while preserving both display-table getter calls.

Evidence: `include/sdf_packet_builders.h`, `include/sdf.h`'s `SdfPoolNode.append`, `src/dds2/game/code_0032C278.c`'s quad provider, and `src/dds2/field/fldFileResolver.c`'s `fldGetDisplayTableRow` provider. No shared header/provider or allocator return-type changes were made. Local declaration exactly preserves the current public interface without broadening this body-only scope.

This is a truthful contract correction, not an allocation fix. The ordinary pass comparison finds only pointer classification on the packet-list pseudo in life/lreg metadata. Target assembly is unchanged. All four UV local assignments and their use/liveness metrics remain unchanged.

## Recovered saved-register semantic roles

Retail and candidate both preserve the same four meaningful UV values across the rotation call:

| Role | Native | Candidate | Candidate named pseudo |
| --- | --- | --- | --- |
| Scaled U | s3 | s2 | r93 |
| Scaled V | s1 | s0 | r94 |
| Scaled width, then U-right | s0 | s1 | r97 endpoint |
| Scaled height, then V-bottom | s2 | s3 | r98 endpoint |

Retail's dimensions are consumed by float geometry conversion before the endpoint additions reuse those homes. Native metadata loads at 0x145E64–0x145ED8 and quad argument stores establish these identities; the canonical 16-byte `FldProjectedSprite` owner and neighboring exact projected-sprite consumer independently support all six signed-halfword fields. No missing field, owner union, extra width, narrowing, signedness or endpoint operation was found.

Candidate pass-00 and later def/use evidence follows raw U r134 → scaled r138 → named r93; V r144 → r148 → r94; width r154 → r158 → right r97; and height r164 → r168 → bottom r98. Width/height named copies disappear at CSE. Local allocation already shares each of these chains into a single home. STABS independently reports the four named final homes above. It does not report a lifetime timeline or local-quantity membership by itself.

At pass 19 the named U/V/right/bottom pseudo records have references/lifetime 4/91, 4/78, 3/74, 3/83 respectively; each crosses one call and is locally allocated. These are per-pseudo statistics, not local-quantity priority totals. Do not mistake global order for the cause of their homes. The full local-quantity totals from the earlier plain-marker observer remain historical evidence, not a freshly observed alpha trace.

The remaining roles agree: color is s5, packet list s4, initial alpha s0, initial index s1, and append target later reuses s0. Every native saved semantic role is present in the candidate. The viewer initializer's missing/distorted ownership-role mechanism does not transfer to this case.

## Bounded stopping conclusion

The independently audited contracts reveal no additional source-level grouping/separation or ownership correction. The packet correction leaves precisely the existing frontier. No new unexplained observation warrants repeating the earlier source controls or requesting more instrumentation merely to rediscover known priority data.

Did not repeat signed-pulse, endpoint-accumulator, raw-field-snapshot, deferred-endpoint, aggregate-storage, viewport or screen-stage controls. Deferred endpoint calculation is especially unsupported because native additions precede the matrix call. No declaration/statement order search, artificial lifetime/use, alias/storage trick, register pinning, false signature, compiler flag change or new assembly was attempted.

Reopen only on a concrete new source fact or independently supported pass mechanism capable of changing actual local allocation inputs. Preserve the corrected readable source as a nonmatching draft meanwhile.

## Reproduction

With the canonical pinned setup and private retail inputs, apply `candidate.patch` to the tested base, then run:

    ninja build/dds2/src/dds2/game/code_001442D0.o
    python tools/check_unit.py src/dds2/game/code_001442D0.c --func func_00145D48
    python tools/check_unit.py src/dds2/game/code_001442D0.c -v

Expected target: 20/228 words different, 912 bytes; whole unit: 168 match / 1 differ. `candidate.inc` contains the same body and finite declarations for later selective restoration. `receipt.json` contains whitelisted aggregate counts only.
