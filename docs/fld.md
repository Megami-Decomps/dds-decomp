# Relocatable field resources (`FLD2`)

`tools/fld.py` converts decompressed `FLD2` resources to editable `.fldasm`
source and back. `FLD2` holds the spatial side of a field area: collision and
automap meshes, cameras, event triggers, named placements, and related
resources.

```sh
python3 tools/fld.py disassemble f011_001.f2 f011_001.fldasm
python3 tools/fld.py assemble \
  --scripts src/dds1/scripts/field/f011.bfasm \
  --warps src/dds1/data/field/f011.wapasm \
  src/dds1/data/field/f011_001.fldasm f011_001.f2
python3 tools/fld.py verify f011_001.f2
```

The tracked DDS1 corpus contains all 591 unique version-23 sources. DDS2 begins
with `f011_001` and uses the same codec; its complete source batch follows
separately. `ninja dds1-field-data dds2-field-data` assembles the tracked
sources and checks every output against its retail SHA-1 alongside the INF and
WAP field data.

`field_fld2_links` in `config/versions.json` selects areas whose event, actor,
and destination identities have also been verified against the maintained BF
and WAP sources. These stricter joins are explicit because field resources can
refer to labels outside the same numbered BF, and WAP identities do not always
belong to one area FLD2. Adding a source to the list turns those joins into a
build error rather than a warning.

`tools/fld_corpus.py` reads loose and LB-contained payloads directly from a
disc image you own. With no output options it verifies canonical byte-exact
round trips. Supplying both paths writes the deduplicated source set and its
build manifest only after the complete pass succeeds:

```sh
python3 tools/fld_corpus.py dds1 /path/to/game.iso
python3 tools/fld_corpus.py dds1 /path/to/game.iso \
  --output-dir src/dds1/data/field \
  --manifest config/dds1/field_fld2.sha1
```

The complete disc profile contains 591 unique version-23 sources in DDS1 and
621 in DDS2. All reconstruct exactly. DDS2 also has 20 byte-identical loose
copies of archived payloads, which share their source names. Two exceptional
DDS1 files use version 21, and one `.f2` file has another format; the importer
reports these without treating them as version 23.

## Object and relocation model

An `FLD2` file has a `0x40`-byte header, a data region, and a packed
relocation stream. Pointers in the data region are file-relative byte offsets.
At load time, the game walks the relocation stream and adds the allocation
base to each pointer word.

The relocation stream stores deltas between pointer-word indices. Small
deltas use one byte, larger deltas use two or three bytes, and a low-bit code
represents a run of consecutive pointer words. The assembler derives this
stream from source labels and emits the same canonical encoding used by the
retail files. Moving a labeled object therefore updates every known and
unknown pointer that targets it.

The main object graph is:

```text
header -> resource type rows -> resource descriptors -> typed payloads
                                      |                 -> raw payloads
                                      -> name, transform, area, links
```

A resource descriptor identifies its type and serial and points to optional
name, transform, area, link, scene block, and type-specific data objects.
Source preserves the physical order of all objects because order is part of
the retail binary.

```text
type id=10 count=10 resources=@type_10_resources

label type_10_resources
resource serial=14 flags=0 type=10 name=@r_01d_01_name reserved=0 \
  transform=@r_01d_01_transform area=@r_01d_01_area \
  link=null sblock=null data=@r_01d_01_data
```

Typed directives express recovered structures. `bytes`, `zeros`, `u32`,
`s32`, and `f32` retain unresolved data, while `pointer @label` preserves a
relocation inside such a region. Raw data can therefore coexist with typed
objects without freezing their offsets.

## Recovered resource types

| Type | Source form | Contents currently recovered |
|---:|---|---|
| `3` | `collision`, `vertex`, `face` | Collision, automap, and placement meshes |
| `4` | `camera` | Named camera field of view (four-byte payload) |
| `6` | `event` | Event flags and field-script procedure name |
| `9` | `motion`, `motion_curve`, typed values, `keys` | Keyed vector, quaternion, scalar, and light motion curves |
| `10` | `placement`, `special_point` | Named positions, actors, events, facilities, and hunt markers |

Each transform is `0x30` bytes: four position floats, four rotation floats,
and four scale floats. Each collision object also starts with a `0x30`-byte
header. Its header owns vertex and face counts and pointers; it is followed by
`0x10`-byte four-float vertices and `0x24`-byte faces.

A face records four vertex indices plus the field controls exposed in source:

```text
face attributes=0x00000800 move_floor=0 sound=0 stop=0 place=0 \
  automap=1,1 vertices=0,1,3,2 \
  encounter_type=0 encounter=0 special=0,0
```

The assembler rejects an out-of-range vertex index, except for the retail
`0xffffffff` triangle sentinel. Event placements likewise must refer to an
event resource present in the same file.

Placement kind 8 carries an eight-byte `special_point` payload. Its first word
selects a save terminal, heal terminal, or indexed hunt marker; the second is
the terminal or marker id. Both PS2 runtimes create `SAVE_UNIT` and
`HEAL_UNIT` objects for the first two values. The third stores an indexed pair
of transform vectors, and its retail resources are consistently named as hunt
markers across both games.

## Motion and path curves

Type-9 resources contain one or more independently keyed curves. The track
table names each value kind and points to its curve:

```text
motion tracks=vector3:@camera_position_curve,quaternion:@camera_rotation_curve

label camera_position_curve
motion_curve count=2 values=@camera_position_values \
  keys=@camera_position_keys word_0c=1

label camera_position_values
vector3 9794.154296875 -756.4210205078125 -4633.36083984375
vector3 9794.154296875 -756.4210205078125 -4633.36083984375

label camera_position_keys
keys 0 270
```

The PS2 runtime linearly interpolates `vector3` tracks and normalized-linearly
interpolates `quaternion` tracks. `scalar` values are single-float curves.
`light` values contain ten floats per key and feed the runtime's light-path
sampler. A curve's values and keys have matching indices; keys must increase
strictly. The final serialized curve word is always one in the tracked data,
but its purpose is not established, so source retains it as `word_0c`.

## Links to scripts and warp data

An area is useful as source when its identities agree across files. During a
linked assembly, the tool checks three joins:

- Every type-6 event label names a procedure in the field's `.bfasm` source.
- Every WAP actor in the area names a type-10 placement.
- Every WAP transition into the area names an existing placement and, when
  supplied, an existing type-4 camera.

These are exact symbol joins. The tool does not infer a similar spelling or
silently leave a missing target unresolved.

For example, the `001_01eve_01` string in the DDS1 `f011_001` event resource
resolves to the procedure of the same name in `f011.bfasm`, and its `01d_01`
door placement resolves the matching actor row in `f011.wapasm`.

## Archive boundary

Most field resources are stored as compressed blocks inside `.LB` archives.
The `.fldasm` source describes the decompressed `FLD2` object. `tools/lb.py`
then places that output into the paired resource archive while retaining the
other blocks from an extracted retail base. See [`lb.md`](lb.md) for the LB
container, compression codec, archive source, and exact build targets.

Run the codec, relocation, semantic-link, and tracked-source tests with:

```sh
python3 tools/test_fld.py
```
