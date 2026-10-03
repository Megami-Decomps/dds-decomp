# Field actor and warp tables (`.WAP`)

`tools/wap.py` converts the field actor, door, elevator, and transition tables
used by both DDS games to compact, editable `.wapasm` source. The tracked
corpus contains one table for every maintained field: 24 in DDS1 and 22 in
DDS2.

```sh
python3 tools/wap.py disassemble \
  --scripts f004.bfasm --interactions f004.infasm \
  f004.wap f004.wapasm
python3 tools/wap.py assemble \
  --scripts f004.bfasm --interactions f004.infasm \
  f004.wapasm f004.wap
python3 tools/wap.py verify f004.wap
```

Every tracked table assembles to its retail SHA-1. `ninja dds1 dds2` checks
the WAP corpus along with the executables, scripts, and INF tables;
`ninja dds1-field-data dds2-field-data` checks the field data alone.

## Physical profiles

WAP has no binary header. Its file size selects one of three layouts:

| Source profile | Elevator rows | Actor/warp rows | File size | Retail use |
|---|---:|---:|---:|---|
| `legacy` | `5 × 0x20` | `256 × 0x64` | `0x64a0` | DDS1 `f009` |
| `dds1` | `5 × 0x20` | `256 × 0x6c` | `0x6ca0` | Other DDS1 fields |
| `dds2` | `8 × 0x20` | `256 × 0x6c` | `0x6d00` | DDS2 fields |

The legacy profile is the layout inherited from Nocturne. DDS extends each
actor/warp row by eight bytes, and DDS2 also expands the elevator table from
five rows to eight. The DDS-only tail is live runtime data, so the profiles
are explicit rather than inferred from a game name.

## Elevator rows

An elevator row is a signed little-endian structure:

| Offset | Type | Source field | Runtime role |
|---:|---|---|---|
| `0x00` | `s32` | `area` | Area/table selector |
| `0x04` | `s16` | `sound` | Elevator sound effect |
| `0x06` | `s16` | `floor_count` | Number of floor choices |
| `0x08` | `6 × s16` | `floors` | Logical floor identifiers |
| `0x14` | `6 × s16` | `blocks` | Matching automap block identifiers |

The common retail row is `{area=1, sound=0, floor_count=1}` with zeroed floor
and block arrays. Source only declares rows that differ from it:

```text
elevator 0 area=52 floor_count=3 floors=3,8,9,0,0,0 blocks=2,3,2,0,0,0
```

## Actor and transition rows

Each of the 256 repeated rows has the following common `0x64`-byte layout:

| Offset | Type | Source field | Established role |
|---:|---|---|---|
| `0x00` | `u8` | `kind` | Actor/door behavior dispatch |
| `0x01` | `u8` | `flag_mode` | Flag-gate mode |
| `0x02` | `s16` | `flag` | Optional model/event flag gate |
| `0x04` | `s16` | `area` | One-based area selector |
| `0x06` | `char[12]` | `name` | Actor, door, or event-trigger name |
| `0x12` | `3 × s16` | `scene args` | Kind-dependent selectors |
| `0x18` | `char[12]` | `scene primary` | Primary scene-object name |
| `0x24` | `char[12]` | `scene secondary` | Secondary scene-object name |
| `0x30` | `u8` | `warp type` | Transition dispatch |
| `0x31` | `u8` | `warp attributes` | Transition state bits |
| `0x32` | `3 × s16` | `warp args` | Type-dependent transition arguments |
| `0x38` | `char[12]` | `warp position` | Destination position resource |
| `0x44` | `u8` | `camera mode` | Destination camera behavior |
| `0x45` | `u8` | `camera table` | Room/camera table selector |
| `0x46` | `char[12]` | `camera name` | Destination camera resource |
| `0x52` | `u8` | `after bgm` | Destination BGM selector |
| `0x53` | `u8` | `after footstep` | Destination footstep selector |
| `0x54` | `u8` | `after flag` | Post-transition action flags |
| `0x55` | `char[15]` | `after script` | Post-transition procedure name |

The actor kind selects one of the procedures bundled with each field script.
The executable uses the same dispatch in both games (kind `12` is only present
in the tracked DDS1 data):

| Value | Source name | Runtime selection |
|---:|---|---|
| `1` | `door` | Named door actor and ordinary transfer |
| `2` | `run_warp` | Running field transition (`runwarp_label`) |
| `3` | `hole_warp` | Falling transition (`anawarp_label`) |
| `4` | `jump_warp` | Jumping transition (`tobiwarp_label`) |
| `5` | `ladder` | Ladder interaction (`hasigo_label`) |
| `6` | `elevator_exit` | Elevator return selected by elevator and floor |
| `7` | `side_exit` | Return selected by cached exit mode and selector |
| `8` | `battle_exit` | Post-battle return selected by event number |
| `9` | `special_warp` | Explicit warp selected by numeric ID |
| `10` | `warp` | Generic scripted transition (`warp_label`) |
| `11` | `suction_warp` | Suction transition (`suikomi_label`) |
| `12` | `barrier` | Barrier interaction (`baria_label`) |

The `scene` group names arguments according to the selected kind. Door rows
use `motion`, `secondary_motion`, and `sound`; elevator exits use `elevator`
and `floor`; side exits use `exit_mode` and `selector`; battle exits use
`event`; and special warps use `id`. Ladder rows expose `direction` and the
`direct_prompt` flag; their primary and secondary resources resolve the source
vector and effect unit. Suction rows expose `state_selector`, `map_entry`, and
`motion`, with the same two resources resolving the source vector and effect
unit. Barrier rows select a separate barrier definition with `barrier`. A
generic `args=A,B,C` triple remains available for unlabelled payloads.

Warp types `field`, `elevator`, `facility`, and `event` encode the verified
values `0..3`. Field transfers name their destination `field` and `area`,
elevator transfers name their `table` and `floor`, and event transfers name
their `event` and optional `alternate_field`.

Facility transfers dispatch on their first argument. The native transition
handler stores the second argument as the initial selection for a shop or as
the terminal slot used by the other actions. Every action applies the third
argument as a floor flag before it enters the menu system:

| Value | Source action | Second argument | Runtime action |
|---:|---|---|---|
| `0` | `shop` | `selection` | Open the shop scene at its initial selection |
| `3` | `terminal` | `slot` | Open the full terminal |
| `4` | `save` | `slot` | Open the terminal's save function |
| `5` | `heal` | `slot` | Open the terminal's healing function |

The terminal slot is also used to select the matching `side_exit` when play
returns to the field. A generic `args=A,B,C` triple remains available for an
unrecognized facility action or a noncanonical payload. The grouped source
therefore reads according to the native dispatch while keeping unresolved
values visible:

```text
entry 5 kind=door area=1 name=@01d_03
  scene motion=1 secondary_motion=2 sound=65 primary="md_01d_03" secondary="md_01d_03b"
  warp field=23 area=1
  camera table=81
end

entry 18 kind=battle_exit
  scene event=606
  warp area=2 position="02pos_03"
  after flag=2 script=@battle_return
end

entry 22
  warp type=facility action=terminal slot=8 floor_flag=3
end
```

The common inactive entry has zero control fields, empty actor/scene names,
position `"01pos_01"`, camera `"01cam_01"`, BGM and footstep selectors `1`,
and no after action. Entry declarations replace that template, and omitted
groups retain it.

The extended profiles append an eight-byte `tail`. Its control byte has two
verified actions: bit 0 applies a field flag using tail arguments, and bit 1
calls the paired parameter helper. The remaining high control bits supply the
flag value. The arguments stay numeric until their shared helper is recovered:

```text
  tail control=5 args=0,0,23,0,0,0,0
```

Fixed strings normally use zero padding. A small set of DDS2 rows retains
nonzero bytes after the first string terminator; those bytes are ignored by
the runtime but are part of the retail file. Canonical source records the
residue explicitly as `script_padding=...` so assembly remains exact without
presenting it as a live procedure name.

## Cross-file symbols

With paired BF and INF source, the disassembler writes `name=@SYMBOL` when an
entry name exactly matches an INF interaction target and `script=@SYMBOL` when
an after-script name exactly matches a BF procedure. Assembly verifies both
links against the paired source and rejects a missing target. Other names stay
quoted strings; no fuzzy or cross-field guess is made.

`tools/fld_scene.py --warps` projects these rows onto the matching FLD2
placement nodes in a composed GLB. Each node retains every owned conditional
transition and its typed destination. The field wrapper reports unlinked named
rows explicitly, which makes incomplete or exceptional resource relationships
inspectable without weakening the exact WAP or FLD2 codecs.

## Field-world graph

`tools/field_graph.py` builds a deterministic whole-game navigation graph from
the tracked WAP and FLD2 identities. JSON keeps the complete transition
metadata for other tools; Graphviz DOT gives a direct visual representation:

```sh
python3 tools/field_graph.py src/dds1/data/field dds1-field-world.json
python3 tools/field_graph.py src/dds1/data/field dds1-field-world.dot \
  --format dot
python3 tools/field_graph.py src/dds1/data/field dds1-field-world-full.json \
  --include-interactions --include-events
python3 tools/field_graph.py src/dds1/data/field f004_001-interactions.dot \
  --format dot --interaction-area f004_001
python3 tools/field_graph.py src/dds1/data/field f011-events.dot \
  --format dot --event-field f011
```

An area node records whether its FLD2 source is present. A field-transition
edge records its source and target areas, actor, WAP entry, actor kind, raw and
typed destination, flag gate, post-transition state, and DDS tail. Missing
source or target areas remain explicit and render as dashed nodes/edges in
DOT. The graph includes only WAP type `field` rows with concrete source and
target areas. Elevator, facility, event, and unresolved transition types stay
in WAP and composed-scene metadata because they do not prove an area-to-area
edge by themselves.

The tracked DDS1 graph contains 579 area nodes and 1,477 field transitions;
1,437 edges have a present source and 1,450 have a present target. DDS2 has
580 nodes and 1,252 transitions, with 1,226 present sources and 1,233 present
targets. Conditional alternatives are separate edges rather than collapsed.

`--include-interactions` upgrades the JSON schema to `dds-field-world-2` and
adds the exact INF state graph. Each interaction set records its ordinary area,
actor, kind, and whether that actor resolves to exactly one type-10 placement.
Flag selectors, message/action rows, completion, warp, and unresolved numeric
controls become stable nodes. Off/on branches and all four row choices become
edges, while message symbols, flag mutations, view actions, and extra actions
stay on their owning row nodes.

A set containing a `warp` target also joins to every same-area, same-actor WAP
row. These handoffs preserve conditional alternatives and every destination
type. Concrete field destinations point to area nodes; elevator, facility,
event, and zero-area destinations remain typed terminal handoffs. There is no
field-wide actor-name fallback.

Across both games the extended graph contains 284 interaction sets, 333 flag
selectors, 453 state rows, and 2,478 encoded state edges. The exact INF-to-WAP
join resolves 167 of 172 sets that contain a warp target, retaining 247 WAP
alternatives. Of those alternatives, 221 reach concrete field areas. DDS1
resolves all 95 warp-bearing sets; DDS2 resolves 72 of 77, with the five
unresolved sets remaining explicit.

`--interaction-area fNNN_AAA` renders a focused DOT graph instead of the full
world view. It groups each actor's state machine, labels unique BF messages,
shows flag and choice edges, and connects warp nodes to their exact WAP
destinations. Unlinked placements, unknown controls, and missing target areas
are dashed. This option enables interaction loading automatically; the default
command remains the fast version-1 world graph.

`--include-events` upgrades the JSON schema to `dds-field-world-5` and adds
the linked field/event execution layer. Each nonnegative kind-1 placement keeps
its exact FLD2 event resource and becomes an entry edge only when the label
exists in the `fNNN.bf` owned by that field's shared archive. The graph does not
use a global label or numeric-prefix fallback.

Procedure nodes expose their runtime index, source name, code-word extent, and
aggregated native-command vocabulary. Exact `CALL`, `JUMP`, and
`CREATE_SCRIPT_TASK` targets form local execution edges. Literal `CALL_EVENT`,
`SUBMIT_EVENT`, and `SUBMIT_EVENT_WITH_SELECTION` targets form external edges.
When the maintained `eNNN.bfasm` exists, the edge enters procedure 0, matching
the native path: admin mode 6 selects either its explicit payload or the
pending positive selection, formats the corresponding event BF resource path,
and creates the process at procedure index 0. The graph then continues through
that script's local and external edges. Each edge retains its command and
dispatch kind; selection submissions also retain the request ID. Dynamic or
invalid targets remain explicit unresolved records with their dispatch command.

Reachability begins at concrete field placements and crosses both local and
maintained event-script edges recursively. `--event-field fNNN` renders that
complete closure for one field, including event procedures and subsequent
event chains rather than stopping at resource-name leaves.

Literal `DEFER_BATTLE_EXIT` calls join the requested field and event to the
exact `battle_exit` row in that field's WAP table. The resulting edge keeps all
matching WAP entries and points to their concrete destination area when they
agree. This resolves 63 of 64 DDS1 sites and all 88 DDS2 sites; all 151 resolved
destinations have maintained field source. The remaining DDS1 request from
`f002` asks the `f031` table for battle-exit event 657, which is absent, so it
remains an explicit unresolved edge.

Across DDS1, 526 placement links seed 510 field procedures and reach 595 of
1,528 field procedures plus 12 maintained event scripts. Those event chains
raise the reachable closure to 607 procedures. DDS2 has 675 placement links,
669 entry procedures, 712 of 1,711 reachable field procedures, and 12
procedures across 11 maintained event scripts, for a 724-procedure closure.

The complete graph contains 107 DDS1 and 121 DDS2 distinct external event
edges (115 and 122 call sites). Of these, 48 and 38 originate in event scripts;
19 DDS1 and 10 DDS2 sites use the selected-event form. All 29 selected targets
are maintained and typed. The 24 DDS1 and 22 DDS2 immediate submissions use
dynamic values and remain explicit unresolved sites. Of the deferred battle
exits, 29 DDS1 and 34 DDS2 sites leave placement-reachable procedures.

Run the codec and complete-corpus regression tests with:

```sh
python3 tools/test_wap.py
```
