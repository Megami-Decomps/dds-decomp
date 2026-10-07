# Common emitter resource initializer: two-word scheduling frontier

KiB / redthing1, 2026-10-07. These are nonmatching research patches, not production replacements or matching credit.

Apply the patches to upstream `e81394cc2f4bbfc8a494ad72365a8c44462ee8e8`. The two candidate patches extend the existing emitter owner and replace only DDS1 `func_00153740` / DDS2 `func_0015B330`. `provider-contracts.patch` gives the existing ParTable definitions their real opaque tag and corrects the DDS1 unused capacity parameter from the DDS2/native donor. The DDS1 candidate also restores effRetainResource's native return value. No shared header changes are needed.

Each target is 480 bytes / 120 words. Both candidates have exactly two differing words: at +0x154, the candidate loads particle count (`lw a1,+0x20`) where retail loads history capacity (`lhu v0,+0x34`); at +0x15C those same instructions are reversed. Everything else agrees, including all four kind branches, calls and the full partial track-parameter construction. Complete candidate units qualify at 114 matches / 1 difference and 113 / 1. Prerequisite-only units are 114 / 0 and 113 / 0; both tagged parManager units are 16 / 0, and the DDS1 cell provider is 55 / 0.

Qualification used the normal canonical wrapper, pinned cc1 SHA256 `d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`, actual retail executables and unchanged production flags. The retained local checkout base is 5a65092384eeedbdd6b01117f22ede86739e2f70 with the actual current headers supplied privately; published patches contain normal header names. No full retail pass is claimed for these nonmatches.

The qualified RTL probe explains the remaining ordering. In sched1, capacity insn 434 precedes count insn 485. In sched2, the SI count load has alias-set 1 anti-dependencies to every SI parameter store, whereas the HI capacity load has alias-set 2 and only the history-length consumer. Count consequently moves earlier. A single natural local track-count snapshot was tested; it instead moves count into the branch delay and leaves four differing words. It is not retained here. Independent owner/callee/CFG review found no supported alternate storage type or lifetime that resolves the two-word residual. This does not prove source impossibility.

Important recovered contracts:

- Kind 1 retains the address of the real BillObj* slot at +0xF4. A later instantiate path can replace that slot; the table renderer dereferences it on each draw. Passing a temporary or caching the pointer would be incorrect.
- The existing tagged ParColorRamp owns 0x44 bytes beginning at emitter +0x48. This unit passes that opaque provider object through the existing 0x4C byte region and never declares a second layout.
- Kind 4 writes only the tail of the full 0x34-byte EffTrackPolyParams. This is a manually fed endpoint list: reset/push/color/draw/release use the owned data and never inspect the model/id/range prefix. The separate model-viewer path initializes all thirteen words and calls the model-driven updater. Do not zero the native untouched prefix, shorten the record, or route this list through effTrackPolyUpdate.
- Signed billboard mode and unsigned submission bucket share the same native halfword. The actual conversions are retained.

To reproduce, apply the patches in an isolated checkout and use `python tools/check_unit.py src/dds1/game/code_00151F58.c -v` and its DDS2 counterpart. Keep production ASM until a grounded correction passes the complete units and retail gates.
