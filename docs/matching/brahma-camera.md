# Brahma camera: mutually exclusive policies and their handled exit

DDS2 `btlSelectBrahmaActorCategoryCamera` (2180 bytes at `0x00223350`)
selects fixed Brahma presets for category flag 4, or a linked-actor camera
policy otherwise. Write those as `if (...) { ... } else { ... }`, followed
by their common `return 1`. The same policy structure appears in the
matched Vritra dispatcher in this owner. The two category inputs retain the
canonical `s32` callback signature and use real `s8` locals, as in the
matched Seto dispatcher; no provider prototype change is needed.

A control with an early handled return after the flag-4 switch was only
2172 bytes. Its first switch default acquired the shared return block,
preventing retail's first delay-slot pair. The ordinary strict compiler
probe shows the exact distinction: before reorg (`28.mach`), the early
return control has the shared handled label and `v0 = 1` immediately after
jump UID345. In the mutually exclusive source, that location instead has
jump UID346 to the later handled join (label UID2181, return UID1196).
In `29.dbr`, reorg copies `v0 = 1` into UID345's comparison delay slot and
puts the saved `$16` reload in UID346's delay slot. All 545 native words,
owned strings/literals, and all 275 existing C peers then match.

The unmatched two-way random cases genuinely report handled while leaving
the existing camera untouched; the final three-way random default reports
unhandled. This is ordinary source control flow, not a redundant write,
invented guard, or register reservation.
