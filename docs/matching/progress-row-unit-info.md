# DDS2 progress-row unit information

`brsDrawProgressRowUnitInfo` (`0x0029E220`, 600 bytes) draws the level text and
unit icon using existing work/row/unit contracts. Both placement arrays use
**one** `BrsRowPosition { s16 x, y; }` element type; their real row counts are
inferred from initialization. The frame table is a genuine signed-byte array.
No nominal type split, external anonymous array overlay, or dead-code exception
is used. Inferred complete array types give the literal block copies separate
alias sets in the pinned compiler (see the existing inferred-bounds idiom).

The unit pointer is captured before drawing. Disabled-state tint affects text,
not the separately captured sprite palette. Position values are acquired before
formatting; the glyph is created, drawn and queued for the current draw buffer. The sprite resource is
rechecked after these callbacks and unit ID is read again for frame selection.

## Bounded small-data ownership correction

The old YAML attributed file offsets `0x3389C0..0x338AE8` to `code_0029BC58`,
although native consumers span several subsequent text units. The correction
adds only two boundaries:

- `0x3389D0`: start `code_0029DF18` small data.
- `0x338A30`: start the next `code_002A1820` small-data range.

The preceding `%2d`/`%7d` formats at native `0x004379C0/C8` are used by
`code_0029BC58`. The next 96 bytes belong to the current unit:

| Native address | Object / consumer |
|---|---|
| `4379D0/D8` | Two 8-byte level/icon placement templates |
| `4379E0` | Eight signed-byte icon frames |
| `4379E8` | Unit-info `%2d` literal and alignment |
| `4379F0` | Existing level-details `%7d` format |
| `4379F8/FC` | Title sound task and initialization guard |
| `437A00/08/10` | Sound/voice path prefixes and title task name |
| `437A18/1C` | Title audio initialization state |
| `437A20/28` | Double-buffer addresses/staging pointer initialized here, also consumed by the next unit |
| `437A2C` | Alternating sound-buffer index used here |

The next owner's first exclusive state is at `437A30/34/38/3C`; its shared
`titleWork` follows at `437A40`. The preexisting coarse suffix after this next
start remains unsplit; this patch does not claim all later title/movie objects
originate in that next unit. Existing ASM providers move intact, retaining every
shared global and byte. Only the target's three small templates and format
become C-owned; the 16-byte gray palette at `428590` is likewise target-owned.
No expected binary, symbol address, symbol size, or runtime contract changes.

Native consumers and neighboring boundaries—not a matching score—justify the
96-byte extent. Whole gates cover previous/current/next units; linked checks
cover the target, palette, corrected extent and entire original 296-byte range.

## Provenance

This is a native-first reconstruction. Earlier nominally different position
row types produced an exact diagnostic that was reverted; that workaround is
not retained. The final source uses one actual coordinate type and the ordinary
inferred-array-bounds idiom, with an independently audited data-owner boundary.

Earlier Raikaru/Lite2-2 (board comments 6242085801/6242266921) established
renderer baselines; Q3 explored aggregate copies; PurBoss (6259307919 and
6259416088) preserved the truthful single-type frontier and qualified copy
scheduler observations after the nominal-type reversion (6259149291). Their
work is credited without treating the reverted diagnostic as acceptable source.
