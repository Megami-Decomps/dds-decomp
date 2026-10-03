# Field interaction tables (`.INF`)

`tools/inf.py` converts the fixed-size field interaction tables used by both
DDS games to compact, editable `.infasm` source. The tracked corpus contains
one table for every maintained field script: 24 in DDS1 and 22 in DDS2.

```sh
python3 tools/inf.py disassemble --messages f004.bfasm f004.inf f004.infasm
python3 tools/inf.py assemble --messages f004.bfasm f004.infasm f004.inf
python3 tools/inf.py verify f004.inf
```

Every tracked table assembles to its retail SHA-1. `ninja dds1 dds2` includes
the field-data checks; they can also be run alone with
`ninja dds1-field-data dds2-field-data`.

## Binary layout

An INF file has no header. It is always `0x3b80` bytes and consists of four
fixed arrays:

| Offset | Count | Row size | Source directive | Runtime role |
|---:|---:|---:|---|---|
| `0x0000` | 8 | `0x54` | `pack` | Maps five area/event hits to an interaction set |
| `0x02a0` | 40 | `0x20` | `view` | Player state, position, and camera actions |
| `0x07a0` | 40 | `0x14` | `ex` | Optional extra actions with four parameters |
| `0x0ac0` | 40 | `0x138` | `set` | Flag selectors and message/selection graphs |

A pack row begins with a signed 32-bit set index followed by five hits. Each
hit is an area byte and a 15-byte, zero-padded event name. A view row contains
two signed 32-bit values followed by 12-byte player-position and camera names.
An `ex` row contains five signed 32-bit values: its action ID and four
parameters.

Each interaction set contains:

| Offset | Count | Row size | Contents |
|---:|---:|---:|---|
| `0x00` | 1 | `0x10` | kind, area, action, event-hit value, 12-byte event name |
| `0x10` | 4 | `0x04` | signed 16-bit flag ID and two one-byte branch targets |
| `0x20` | 20 | `0x0e` | row kind, message ID, four branch targets, two flags, view and `ex` selectors |

The paired runtimes establish the message-row fields directly. Row kind `0`
displays a message, kind `1` displays a selection, and kind `-1` performs only
the attached transition/actions. The four `go` bytes select the next result
for the four possible selection outcomes. Values `10..29` address message rows
`0..19`; value `100` enters the warp handoff. Other control values stay
numeric because their full meanings vary by interaction.

The two signed flag fields on a destination row are applied as off/on
mutations. A nonzero view selector applies the corresponding view row. A
nonzero `ex` selector applies its extra-action row. DDS stores the view
selector in one byte; values with the high bit set are preserved numerically.

## Source form

Source begins with `inf 1`. It is defined relative to the canonical template
present in every tracked retail table, so unchanged rows are omitted:

- eight packs with set index `0` and five `{area=0, event="01eve_01"}` hits;
- forty `{player=1, motion=0, position="01pos_01", camera="01cam_01"}` views;
- forty zeroed `ex` rows;
- forty inactive sets with kind/area `255`, action/event-hit `1`, event
  `"01eve_01"`, zeroed flag selectors, and default message rows.

Declarations replace individual template rows. Missing flag and message rows
inside a declared set retain their defaults.

```text
inf 1

set 1 kind=event area=0 action=1 event_hit=2 event="01d_03"
  flag 0 id=23 off=2 on=warp
  flag 1 id=15 off=3 on=row0
  row 0 kind=message message=@DONT_OPEN go=0,0,0,0 flag_off=0 flag_on=0 view=0 ex=0
  row 1 kind=action message=-1 go=warp,0,0,0 flag_off=0 flag_on=2060 view=0 ex=0
end
```

Set kinds `npc` and `event` encode the verified values `0` and `1`. Message
row kinds `action`, `message`, and `selection` encode `-1`, `0`, and `1`.
`row0..row19` and `warp` encode the verified graph targets; numeric values are
accepted everywhere and are used whenever no stable semantic name exists.

`message=@NAME` resolves a unique dialog name from the paired BF source passed
with `--messages`. If a message bank cannot be decoded safely, or a name is
duplicated or not source-safe, the disassembler writes the numeric message ID.
The assembler rejects an unresolved `@NAME` rather than guessing.

Pack, view, and extra-action overrides use the same exact model:

```text
pack 2 set=4
  hit 3 area=1 event="01eve_03"
end

view 3 player=0 motion=2 position="01pos_04" camera="01cam_02"
ex 4 id=2 params=10,1,0,0
```

Strings must be printable ASCII and fit their fixed fields. Integers are
range-checked at assembly time, duplicate declarations are rejected, and
binary decoding rejects nonzero string padding. The format remains fixed-size;
archive extraction and reinsertion are separate concerns.

Run the codec and complete-corpus regression tests with:

```sh
python3 tools/test_inf.py
```

## Placement and scene linkage

For every nondefault set, the start row's `action` byte equals the `AAA` part
of an existing ordinary `fNNN_AAA` field area, while its event string names a
type-10 FLD2 placement. This gives a strict cross-resource identity for 258 of
the 284 nondefault sets:
141 of 143 in DDS1 and 117 of 141 in DDS2. There are no duplicate placement
matches within an area. The other 26 sets remain explicit unlinked identities;
the linker does not fall back to matching the same name elsewhere in a field.

`tools/fld_scene.py` exposes the relationship in composed GLB scenes. A linked
placement carries `ddsInteractions`, with the complete nondefault flag and
message graph plus any default row reached by an explicit branch. BF message
names are included only when the paired source gives a unique symbol for that
numeric message ID. Raw control values remain alongside typed row, warp, and
completion targets.
