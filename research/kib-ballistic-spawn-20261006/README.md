# Ballistic packet spawn pair: preserved nonmatch

KiB, 2026-10-06. Research snippets only; production remains ASM.

DDS1 code_00151F58.c::func_001547D8 and DDS2 code_00159B48.c::func_0015C3C8 are each 2,048 bytes / 512 words. Both candidates preserve native size and have **22 differing words**, solely the general/delay RNG address high halves exchanging GPR20/GPR21. Whole canonical units are 113 match/1 differ and 112 match/1 differ. There is no matching credit or retail-build success claim for these candidates.

Insert the title snippet at its INCLUDE_ASM in source at 92f582e81a25a3d60c3b1f8c6d83f653724e93aa. Existing EffBallisticEmitter, EffEmitterHead and EffPacket are reused without new views. The normal current eff/sdf/pcp_vu0/ee_mmi header composition was checked. Pinned cc1 SHA-256: d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1. Use tools/check_unit.py with the canonical unit and a full --source candidate.

## Source facts recovered

The randomized mode 0 cone draws from the delay RNG, independently of the general RNG used for position/speed. Their order is preserved. Mode 2 prewarms at least once even for nonpositive frameCount: advance position, damp all XYZ, then add Y gravity. It stores negative initial velocity and negative gravity, not final simulated velocity. Sharing the actual jitter intermediate recovers ten FP words; expressing vector damping followed by acceleration recovers twenty-one more.

Independent paired semantic review accepted mode/default behavior, cached parameters, signedness, both twelve-vector authored tables, macro contracts and inherited W behavior. Do not initialize packet W or substitute a forced-point transform: native matrix multiplication actually consumes that retained component.

## Qualified compiler cause

Supplied, fresh normal and diagnostic target assembly are identical. In the actual pinned compiler, PRE creates general high pseudo 432 before delay pseudo 434. Current symbol spellings hash to expression buckets 50 and 91, traversed in that order. The global allocator's truncated priorities tie:

- General: 12 references, reported lifetime 41728, width 1: trunc(3*12/41728*10000)=8
- Delay: 10 references, reported lifetime 35328, width 1: trunc(3*10/35328*10000)=8

Creation order breaks the tie. Both encounter occupied saved registers 16..19, so general receives 20 and delay 21; native uses the reverse. This is not reload rematerialization.

The providers use the same four-u32 RNG state. The current delay-state name is explicitly inferred in name_sources.txt; original declarations/spellings remain unrecovered. No declaration reorder, symbol-name search, selective type change, artificial use, compiler repair or flag change was accepted merely to reverse the tie. A future correction needs independent source evidence. This diagnosis does not claim that matching source is impossible.

No retail binaries or generated assembly are included.
