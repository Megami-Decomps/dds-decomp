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

The tracked corpus begins with area `f011_001` in each game. Both sources
assemble to their retail SHA-1. `ninja dds1-field-data dds2-field-data`
assembles them, resolves their script and warp symbols, and checks the output
alongside the INF and WAP field data.

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
| `4` | `camera` | Named camera field of view |
| `6` | `event` | Event flags and field-script procedure name |
| `9` | raw data plus labeled pointers | Camera motion/path object graph; layout still unresolved |
| `10` | `placement` | Named positions, doors, and event placements |

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
