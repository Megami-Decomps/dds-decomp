# Event-viewer object-target selection

`evtViewCmdSetObjectTarget` assigns the selected world's object name to the
pending timeline key, reapplies the current timeline frame, and pops the viewer
history. The matched routines are DDS1 `0x00232108` and DDS2 `0x0024CE18`, each
736 bytes, in `event/evtEventViewer.c`.

## Preserve the actual selection and callback boundaries

The object cursor starts as NULL after checking the pending key. It walks the
world's named objects, skipping slot 3, and retains the object at the requested
ordinal. NULL remains the selection when the selector is zero or the traversal
finds no matching object. The subsequent command and group types choose the
signed byte or halfword receiving its name index, with -1 for no selection.

For group type 9, the routine first finds the old target and ends its unit
transition. The selected object is a distinct live value across that lookup
and those calls. It then supplies its name to `evtEventViewerAddName`; there is
no further selected-object consumer after that name call on the same path.
The reconstruction preserves these real definitions and uses. It does not
cache the viewer's input index across the name callback or introduce a new
memory effect.

## Use the existing byte-parameter member

Types 20 and 21 select a signed-byte name index beginning at key offset `0x0C`.
The existing `EvtRuntimeChild.parameterBytes` array begins at `0x08`, so the
store is expressed as:

```c
entry->parameterBytes[viewer->inputB + 4] = nameIndex;
```

This is the same array used by the matched `evtViewerDrawPendingNodeRow`
consumer, which reads `parameterBytes[4 + index]`. Its `mnuDrawInfoWindowB`
producer offers two rows for type 20 and four for type 21. The viewer starts
`inputB` at zero and `dds3SelectMenu` updates that cursor. The established
indices therefore address bytes 4..5 or 4..7 of the parameter array.

The earlier `entry->p0C.sb[viewer->inputB]` spelling addresses the same bytes
for those indices. It is not being replaced because the established UI path
necessarily exceeds that four-byte subarray. Both forms retain the same signed
byte value and assignment conversion; the array form also agrees with the
existing renderer's representation.

The qualified compiler captures expose the addressing distinction at RTL
expansion. The nested-member form first builds `entry + 12` and then adds the
input index. The parameter-array form evaluates `inputB + 4` and the existing
array base `entry + 8`. Constant folding leaves the same displacement but
preserves the native index-before-base operand order. With the selected-object
home fixed, these forms differ in exactly two native words, at function offsets
`+0x160` and `+0x17C`. No cast, alternate layout, qualifier, extra operation, or
flat word view of a typed record is involved.

## Keep the final store arms explicit

The final command case uses the same NULL/non-NULL store arms as the preceding
name-assignment cases. Exactly one store executes, and only a non-NULL object
causes a name-intern call. The stock compiler combines the two final halfword
stores in pass `27.jump2`; this source retains the native null-first branch and
single joined store through delay-slot processing.

The earlier ternary has a different final branch and store placement. A joined
32-bit temporary shrinks the function to 728 bytes, while a joined signed
halfword temporary introduces sign extension instead of the native layout.
Separate real store arms retain the complete 736-byte shape without dead code
or a branch-shaping constraint.

## Localize the selected-object register home

Both native routines keep the actual object cursor and selected object in the
callee-saved `s3` register. Ordinary C gives the same operation sequence and
extent but chooses a different three-register allocation. Qualified stock
EE GCC 2.96 traces identify these ordinary pseudos in both games:

| Value | Pseudo | Weighted references | Live length | Global priority | Home |
| --- | ---: | ---: | ---: | ---: | --- |
| World slot | 91 | 17 | 70 | 9714 | s0 |
| Object cursor / selection | 88 | 33 | 184 | 8967 | s1 |
| Named-object ordinal | 90 | 16 | 74 | 8648 | s2 |
| Viewer | 86 | 27 | 149 | 7248 | s3 |
| Pending key | 87 | 14 | 128 | 3281 | s4 |

Each has hard-register width one. The object sorts before the ordinal and
viewer in global allocation. Their overlapping lifetimes prevent them from
sharing those homes. The natural cursor-loop and separate-selection controls
do not recover the native allocation without changing the instruction shape.

The matching candidate therefore gives only the real object local a fixed
callee-saved home:

```c
register dds3Basic_t *object __asm__("s3");
```

Every reaching read is initialized by NULL, a real slot head, or an actual
`next` link. The binding applies to that existing local's lifetime, including
the old-target lookup and transition calls, and adds no consumer or extension
of its lifetime. No intervening call clobbers this callee-saved register.

The final trace represents the same definitions and uses directly in hard
`s3`. That gives the overlapping ordinal and viewer actual `s3` conflicts;
their unchanged priorities and live lengths select `s1` and `s2`, respectively,
as in native. The pending key remains in `s4`. Removing only this binding keeps
the 736-byte extent but changes 47 instruction words in each game, all from the
saved-register allocation. This is an explicit reconstruction of an allocation
dependency, not evidence of original source spelling or a hardware requirement
for `s3`.

## Qualified evidence and validation

The final candidate and binding-removal control in each game were compiled as
complete owners through the repository's normal compiler wrapper and unit
flags. The pinned compiler SHA-256 is
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.

All four guarded candidate/control ordinary-versus-RTL-capture comparisons
agree in assembly, allocated section bytes and metadata, complete symbols,
resolved relocation records, auxiliary nondebug sections, and ELF ABI. The
separate nested-byte-member control is qualified the same way.

The promotion-ready sources remove only this target's preprocessor guard and
assembly fallback. Both unguarded sources were independently recaptured and
pass the same complete ordinary-versus-RTL qualification. They also reproduce
the guarded candidates' normalized complete-owner assembly and all compared
object contracts exactly. The guarded binding-removal captures therefore remain
valid semantic controls. The source and immutable captures are retained
privately; the build does not use the diagnostics.

The complete candidate owners verify 98 matches and zero differences in DDS1,
and 102 matches and zero differences in DDS2. Binding-removal owners verify
97/1 and 101/1, with the only mismatch being this target. No owned-data changes
are required. Independent semantic review and the integrated full-build and
test gates are recorded with the landing change.

Integrated validation passed after independent semantic approval: complete
owners remain 98/0 and 102/0; both full retail executable SHA-1 checks pass;
all 442 tooling tests in 117 batches pass; and all 80 retained-C units compile.
The approved guardless source SHA-256 values are
`61f55885c92e4fd2c61058ad1bbc2924d235f07bdcd1b76d18efb5f028fae53d`
(DDS1) and
`a267ba5562b507906f2ca2a8fe71b690bebb904aa953677f015339423ce2b404`
(DDS2). Private binaries, traces and controls are not included in the source
commit.
