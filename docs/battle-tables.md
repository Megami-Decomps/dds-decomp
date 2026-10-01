# Battle table source

`tools/battle_tbl.py` converts the core battle data tables used by both games
to editable `.tblasm` source. It currently supports `ENCOUNT.TBL` and
`UNIT.TBL`:

```sh
python3 tools/battle_tbl.py disassemble ENCOUNT.TBL encount.tblasm
python3 tools/battle_tbl.py assemble encount.tblasm ENCOUNT.TBL
python3 tools/battle_tbl.py disassemble UNIT.TBL unit.tblasm
python3 tools/battle_tbl.py assemble unit.tblasm UNIT.TBL
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
