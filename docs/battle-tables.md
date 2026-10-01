# Battle table source

`tools/battle_tbl.py` converts the core battle data tables used by both games
to editable `.tblasm` source. It currently supports `ENCOUNT.TBL`, `UNIT.TBL`,
`SKILL.TBL`, `AICALC.TBL`, and `MSG.TBL`:

```sh
python3 tools/battle_tbl.py disassemble ENCOUNT.TBL encount.tblasm
python3 tools/battle_tbl.py assemble encount.tblasm ENCOUNT.TBL
python3 tools/battle_tbl.py disassemble UNIT.TBL unit.tblasm
python3 tools/battle_tbl.py assemble unit.tblasm UNIT.TBL
python3 tools/battle_tbl.py disassemble SKILL.TBL skill.tblasm
python3 tools/battle_tbl.py assemble skill.tblasm SKILL.TBL
python3 tools/battle_tbl.py disassemble AICALC.TBL aicalc.tblasm
python3 tools/battle_tbl.py assemble aicalc.tblasm AICALC.TBL
python3 tools/battle_tbl.py disassemble MSG.TBL msg.tblasm
python3 tools/battle_tbl.py assemble msg.tblasm MSG.TBL
ninja dds1-battle-data dds2-battle-data
```

The tracked DDS1 and DDS2 sources reassemble to the retail SHA-1s. The normal
`dds1` and `dds2` targets include those checks.

## Physical container

Each supported `.TBL` contains consecutive segments. A segment is a little-endian
`u32` byte length followed by that many payload bytes; the next header begins
at a 16-byte boundary. Retail alignment bytes are zero. `ENCOUNT.TBL` has six
segments:

| Segment | DDS1 size | DDS2 size | Contents |
|---:|---:|---:|---|
| 0 | `0xa000` | `0xa000` | 1,024 encounter formations |
| 1 | `0x3040` | `0x3040` | 16 default-zone maps |
| 2 | `0x10600` | `0xaf00` | Field encounter zones and weighted pools |
| 3 | `0x80` | `0x80` | 16 conditional zone overrides |
| 4 | `0x3040` | `0x3040` | 16 battle-background maps |
| 5 | `0x880` | `0x700` | Eight field visual-selection rows |

The different segment-2 sizes are structural. DDS1 has 128 zone rows, four
weighted pools per row, and a `0x20c` row size. DDS2 has 112 zone rows, three
pools per row, and a `0x190` row size. The selection code in each executable
uses those exact strides. The visual rows likewise use profile-specific
headers and group counts.

`UNIT.TBL` has five segments:

| Segment | DDS1 size | DDS2 size | Contents |
|---:|---:|---:|---|
| 0 | `0x1a40` | `0x1c40` | 16 party-unit templates |
| 1 | `0x4c0` | `0x4c0` | 16 normal-form affinity rows |
| 2 | `0x4c0` | `0x4c0` | 16 alternate-form affinity rows |
| 3 | `0x7200` | `0x7200` | 384 enemy templates |
| 4 | `0x7200` | `0x7200` | 384 enemy affinity rows |

The party row grows from `0x1a4` bytes in DDS1 to `0x1c4` in DDS2. The
retail templates only author the common core through the five starting stats;
the remaining skill, profile, and game-specific tail storage is zero. The
source still models that storage so later edits remain exact.

## Encounter source

The source omits zero records and reconstructs the fixed tables from the
selected profile. An encounter names its enemy slots and battle setup values:

```text
battle-table 1 kind=encounter profile=dds1

encounter 606 voice=2 enemies=109,109 backgrounds=228,1 flags=0x4 bgm=31 event=606
```

Default-zone and background maps use separate vocabulary for the same physical
selector shape:

```text
default-map 0 map=22
  entry 0 zone=1 flag_a=100 zone_a=2 flag_b=101 zone_b=3
end

background-map 0 map=22
  entry 0 background=131073 flag_a=100 background_a=4
end
```

A zone records its three conditions, the selector for each truth combination,
and its weighted encounter pools:

```text
zone 4 backgrounds=203,2 bgm=5
  condition 0 kind=1 value=32
  routes abc=0 ab=0 ac=0 bc=0 a=1 b=1 c=1 none=0
  pool 0 threshold=30
    slot 0 encounter=33 weight=60 modifier=-3 next_roll=70
  end
end
```

The renderer keeps fields numeric when their gameplay meaning is not stable.
Those fields use offset-based names such as `unknown_03` and `unknown_06`.
The visual-selection tail is represented as fixed headers and groups because
its native consumers establish the record boundaries but do not yet justify
names for every value. This keeps the source exact without presenting guesses
as recovered semantics.

## Unit source

Party templates expose their starting resources, level, and five core stats:

```text
battle-table 1 kind=unit profile=dds1

party 3 flags=0x7 unit=3 hp=35 max_hp=34 mp=9 max_mp=9 level=1 stats=5,5,2,3,3
```

The optional `affinity_source` field selects an enemy-affinity row for linked
or special forms; a zero value uses the party unit's own row.

Enemy templates connect the encounter enemy IDs to combat stats, skill IDs,
rewards, drops, and basic-attack behavior:

```text
enemy 6 race=1 level=30 hp=220 max_hp=220 mp=288 max_mp=288 growth=6 stats=30,25,30,25,18 skills=22,84,85,50,164 macca=900 experience=221 atma_points=300
```

Affinity rows hold 19 packed `u32` values. Their low halfword is the numeric
rate used by battle calculations; the high halfword contains behavior flags.
Plain rates stay decimal and packed values use hexadecimal so that distinction
is visible without assigning unverified names to the flags:

```text
party-affinity 1 values=100,100,0x80000078,50,100,100,100,100,100,100,100,100,100,100,100,100
```

Fields whose consumers establish a type but not a stable gameplay name retain
offset-based names. Unknown byte spans use fixed-length hexadecimal values.
The assembler validates every row count, list width, integer range, segment
size, and alignment byte before writing a table. Corpus tests also join every
nonzero encounter enemy ID to a populated enemy template in the paired game.

## Skill source

`SKILL.TBL` connects skill IDs to battle behavior, costs, targeting, power,
requirements, item records, and shared calculation constants. DDS1 has seven
segments and DDS2 has eight:

| Segment | DDS1 | DDS2 | Contents |
|---:|---:|---:|---|
| 0 | 608 × `0x02` | 672 × `0x02` | action attribute and auxiliary value by skill ID |
| 1 | 512 × `0x38` | 544 × `0x38` | executable action records |
| 2 | 85 × `0x10` | 117 × `0x10` | requirements for skill IDs `0x1ab` and above |
| 3 | `0x300` | `0x400` | calculation coefficients |
| 4 | 16 × `0x14` | 16 × `0x14` | party-unit battle defaults |
| 5 | 192 × `0x08` | 256 × `0x08` | item and event entries |
| 6 | — | 64 × `0x06` | DDS2 profile stat bonuses |
| 6 / 7 | 16 × 16 skills | 48 × 24 skills | named requirement groups |

The action record exposes the fields used directly by battle and menu code,
including use and effect types, cost, targeting, hit behavior, HP and MP
effects, ailments, support effects, and magic scaling. Multi-byte fields are
split at their actual boundaries, so hit type and hit level, for example, can
be edited independently:

```text
action 1 use=2 effect_type=1 cost_type=2 cost=3 target_area=2 hit_type=1 hit_level=99 hits_min=1 hits_max=1 hp_type=1 hp_power=30 effect_percent=100 ailment_level=100 magic_base=20 magic_limit=31000
```

High skill IDs carry three tagged requirements. The source writes the tags as
`skill`, `attribute-mask`, `unit-mask`, or `group`; `any` is an unconditional
slot and `none` is an unused slot. Group references resolve to the skill lists
at the end of the same file:

```text
requirement 0x1b3 conditions=skill:530,skill:533,group:12 count=3
group 12 skills=46,47,48
```

Floating-point constants use decimal values when they round-trip exactly to
the stored `f32`. Non-float words in the coefficient pool use `bits=0x...`.
The DDS2-only profile rows expose five stat bonuses and their tier. The corpus
checks validate group references and every skill ID stored by party and enemy
unit records, including passive IDs that have an attribute row but no
executable action row.

## Battle text and message banks

`MSG.TBL` connects battle IDs to the text shown by battle and menu code. Its
fixed-width tables contain affinity summaries, tribe names, enemy descriptions,
enemy names, item names, actor names, race names, and skill names. DDS1 has
two additional label tables whose placeholder-only contents remain neutral in
source. DDS2 instead has a 48-row skill-family name table used by an affinity
menu path.

| Table | DDS1 | DDS2 | Row width |
|---|---:|---:|---:|
| Affinity descriptions | 256 | 256 | `0x2d` |
| Tribe names | 98 | 176 | `0x13` |
| Enemy descriptions | 384 | 384 | `0xbd` |
| Enemy names | 384 | 384 | `0x11` |
| Item names | 192 | 256 | `0x19` |
| Actor names | 32 | 32 | `0x11` |
| Race names | 16 | 32 | `0x07` |
| Skill names | 624 | 672 | `0x11` |
| DDS1 token labels / DDS2 skill-family names | 64 | 48 | `0x11` / `0x21` |
| DDS1 reserved labels | 256 | — | `0x11` |

Every fixed row appears explicitly, including retail placeholders and empty
strings. Quoted text makes spaces and punctuation editable while the assembler
enforces ASCII, row capacity, NUL termination, and zero padding:

```text
enemy-name 1 "Laksmi"
item-name 1 "Ration"
skill-name 1 "Agi"
```

The final four segments are complete MSG1 banks for item help, skill help,
status help, and default-command help. They live beside the table as ordinary
`.msgasm` sources and use the same exact message language as other MSG1 data:

```text
message-bank items file=msg-items.msgasm
message-bank skills file=msg-skills.msgasm
message-bank status-help file=msg-status-help.msgasm
message-bank command-help file=msg-command-help.msgasm
```

DDS1 contains 192, 607, 210, and 9 dialogs in those banks; DDS2 contains 256,
672, 254, and 9. The table codec validates each complete MSG1 layout and
reassembles every bank canonically. Corpus checks also join the 384 enemy name
and description rows to `UNIT.TBL`, the item-name rows to the SKILL item domain,
and the skill-name rows to the full SKILL ID domain.

The maintained battle banks use the native- and corpus-verified semantic
control view.
For example, `segment-start`, `font-slot 1`, `text-attribute 1 4`, `token 0`,
and `conditional-newline` replace their encoded `F1`/`F2` byte sequences.
Controls whose behavior is still unknown remain explicit `control` rows.

## Battle AI and formulas

`AICALC.TBL` holds the enemy decision tables, shared calculation words, and
the FLW0 programs that implement enemy AI and battle formulas:

| Segment | DDS1 | DDS2 | Contents |
|---:|---:|---:|---|
| 0 | 384 × `0x15c` | 384 × `0x15c` | Per-enemy decision data and weighted actions |
| 1 | `0xa6c` | `0xc14` | Calculation words consumed by battle formulas |
| 2 | — | 32 × `0x20` | DDS2 weighted value tables |
| 2 / 3 | `0x2f9fe` | `0x38138` | Enemy-AI FLW0 program |
| 3 / 4 | `0x219c` | `0x257c` | Battle-formula FLW0 program |

Each enemy has three decision tiers. A tier evaluates three packed predicates
and uses their truth pattern to read one of eight route bytes. Retail routes
normally select one of seven groups or use `8` to continue to another tier.
Each group contains five `{weight, action, effect}` choices. The source makes
those boundaries explicit while retaining numeric selector IDs where the
predicate or effect handler has not earned a stable gameplay name:

```text
enemy-ai 2 script=ai_ishisu_zako
  decision 0 predicates=50:2,22:2,25:0 routes=0,8,8,2,8,8,8,8
  choice 0 0 weight=100 action=special:0
  choice 1 0 weight=50 action=skill:86
  choice 5 0 weight=100 action=preset:1:4
end
```

A packed predicate or effect writes `selector:argument`; the physical word
uses its high 10 bits for the native handler selector and its low 22 bits for
the argument. Action values distinguish direct `skill` IDs, six native
`preset` families, DDS2 `weighted` tables, and built-in `special` actions.
Direct skill actions are checked against the paired `SKILL.TBL`. Script
references use the actual procedure names from the AI program rather than
bare table indices.

DDS2's separate weighted tables contain eight `{value, weight}` entries. Zero
entries are omitted from source:

```text
weighted-table 2
  entry 0 value=42 weight=20
  entry 1 value=39 weight=20
  entry 2 value=90 weight=20
end
```

The calculation segment remains an indexed word table because its consumers
assign meaning to ranges rather than one uniform record type. Words that are
ordinary finite `f32` values use decimals; other bit patterns stay explicit.

The two embedded programs live beside the table as `aicalc-ai.bfasm` and
`aicalc-formulas.bfasm`. They use the same exact symbolic and structured FLW0
source as field and event scripts. DDS1 contributes 72 named AI procedures and
29 formula procedures; DDS2 contributes 89 and 32. Assembly resolves their
labels, rebuilds both containers, inserts them into the table, and verifies the
complete retail hash.

The formula program also names the native calculation context rather than
leaving its dispatch IDs as `COMM` operands. For example, hit-rate code can
refer directly to `CALC_SOURCE_LEVEL()`, `CALC_TARGET_STAT(3)`, and
`CALC_ACTION_HIT_LEVEL()`, then return through `CALC_SET_RESULT(...)`.
Lookup curves whose exact gameplay role remains uncertain retain neutral
`CALC_LEVEL_FACTOR_*` names.

The AI programs use a separate battle-command vocabulary derived from each
game's native dispatch table and handlers. It covers action and target
selection, HP and party-state queries, history counters, scene transitions,
and camera operations. This names 2,909 of 3,434 calls in DDS1 and 3,414 of
3,801 calls in DDS2, allowing common code to read as conditions such as
`AI_UNIT_HP_AT_OR_BELOW_RATE(25)` and actions such as
`AI_SELECT_SKILL(1)`. Calls whose handler role is still ambiguous remain as
numeric `COMM` instructions; the profile does not infer names from usage
alone.
