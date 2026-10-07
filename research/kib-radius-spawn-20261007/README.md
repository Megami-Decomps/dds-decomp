# Bounded-radius particle spawn: allocation frontier

KiB / redthing1, 2026-10-07. Nonmatching research patches; no production replacement or matching credit.

Apply the paired patches to upstream `e81394cc2f4bbfc8a494ad72365a8c44462ee8e8`. They reuse the ordinary existing EffTemplatePacketList fields already landed with the exact radius updater pair, and replace only DDS1 `func_00157280` / DDS2 `func_0015EE70`.

Each target is 848 bytes / 212 words. Both candidates retain 27 different words. Complete units are 114 matches / 1 difference and 113 / 1; the newly matched 928-byte updater remains exact with its spawn helper visible in C. These are native executable comparisons, not fuzzy scores. No full retail pass is claimed for these nonmatches.

The qualified current candidate emits the right sphere/planar behavior, random stream order, full vector stores, negative unsigned-modulo spawn delay, magnitude, speed/spin jitter and kind dispatch. Sphere mode uses three independent radius draws, not one shared draw. Planar phase divides by the unsigned particle count and multiplies the signed packet index. W lanes remain native, without invented zero/one initialization.

One real scalar value shared between the phase computation and later jitter sampling recovered nine floating-point words. The remaining differences concern the persistent general RNG high half versus packet index homes and common-tail scheduling. In the DDS1 qualified trace, index pseudo 85 has 4 references, live length 154, priority 519 and receives r20; the PRE-created general RNG high pseudo 225 has 4 references, live length 152, priority 526 and receives r19. Retail uses the opposite two homes. The quantities conflict and no actual additional lifetime has been established to reverse their order. Preserve this measured boundary rather than inventing a use or renaming a symbol to change allocation.

The rejected broad union of EffEmitterHead and EffTemplatePacketList altered an existing scale helper's alias behavior; it is not part of these patches. No flags, compiler changes, fake volatile, register bindings, source bypasses or enumerated variants are used.

Qualification uses the canonical normal wrapper and pinned cc1 SHA256 `d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`. The retained local checkout base is 5a65092384eeedbdd6b01117f22ede86739e2f70 with qualified current source/header overlays; these portable patches use ordinary repository includes. Reproduce with the complete canonical unit checks against the actual original executables.
