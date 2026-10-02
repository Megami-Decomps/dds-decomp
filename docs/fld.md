# Relocatable field resources (`FLD1` and `FLD2`)

`tools/fld.py` converts decompressed field resources to editable source and
back. `FLD1` holds field models, texture-list references, effects, lights, and
some shared collision and motion data. `FLD2` holds collision and automap
meshes, cameras, event triggers, named placements, and related resources. Both
formats use the same object and relocation layer.

```sh
python3 tools/fld.py disassemble f011_001.f2 f011_001.fldasm
python3 tools/fld.py assemble \
  --scripts src/dds1/scripts/field/f011.bfasm \
  --warps src/dds1/data/field/f011.wapasm \
  src/dds1/data/field/f011_001.fldasm f011_001.f2
python3 tools/fld.py verify f011_001.f2
python3 tools/fld.py disassemble f011_001.f1 f011_001.f1asm
python3 tools/fld.py assemble f011_001.f1asm f011_001.f1
```

The tracked FLD2 corpora contain all 591 unique DDS1 and 621 unique DDS2
version-23 sources. The paired `f011_001.f1asm` sources establish exact FLD1
assembly for both games and are inserted into the tracked LB archives.
`ninja dds1-field-data dds2-field-data` checks every output against its retail
SHA-1 alongside the INF and WAP field data.

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
python3 tools/fld_corpus.py dds1 /path/to/game.iso --kind fld1
```

The complete disc profile contains 591 unique version-23 sources in DDS1 and
621 in DDS2. All reconstruct exactly. DDS2 also has 20 byte-identical loose
copies of archived payloads, which share their source names. Two exceptional
DDS1 files use version 21, and one `.f2` file has another format; the importer
reports these without treating them as version 23.

For FLD1, the importer verifies 554 version-23 source identities in DDS1 and
568 in DDS2. DDS1 has two version-21 files and one unrelated `.f1` payload;
DDS2 has 20 byte-identical loose/archive occurrences. The paired sources are
tracked first because most of the roughly 830 MiB decompressed corpus is model
data whose inner structures are still being recovered.

## Object and relocation model

An `FLD1` or `FLD2` file has a `0x40`-byte header, a data region, and a packed
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

FLD1 additionally uses these typed forms:

| Type | Source form | Contents currently recovered |
|---:|---|---|
| `2` | `model_resource`, `model_items`, `model_item`, `model_bounds`, `model_assets`, `model_asset`, `model_draw_set`, `model_draw_list`, `model_draw`, mesh directives | Field model hierarchy, material assets, render commands, and VIF mesh streams |
| `5` | `texture_list` | Referenced field texture-list filename |
| `11` | `effect` | Effect kind, resource selector, size, and parameters |
| `12` | `light` | Animation mode, radii, softness, bias, diffuse RGB, and ambient RGB |

Types `3` and `9` use the same collision and motion forms in both files.

Each type-2 resource owns a model-item list and refers to its shared asset and
motion definitions:

```text
model_resource items=@md_01all_02_items \
  assets=@md_01all_02_assets motion=@md_01all_02_motion

label md_01all_02_items
model_items count=131
model_item node_id=0 parent=-1 rotation=0,-1.57079637,-0 \
  position=0,0,0,1 scale=1,1,1,0 bounds=null commands=null
model_item node_id=2 parent=1 rotation=0,0.210236,0 \
  position=-54240.1171875,30,-290.3564453125,1 scale=1,1,1,0 \
  bounds=@md_01all_02_node_2_bounds \
  commands=@md_01all_02_node_2_draws

label md_01all_02_node_2_bounds
model_bounds minimum=-2138.26513671875,0.0000457763671875,-7423.814453125 \
  maximum=4551.91259765625,103.9683609008789,6058.49365234375

label md_01all_02_node_2_draws
model_draw_set lists=@md_01all_02_node_2_draws_list_0
label md_01all_02_node_2_draws_list_0
model_draw_list selector=2 draws=@md_01all_02_node_2_draws_list_0_draw_0
label md_01all_02_node_2_draws_list_0_draw_0
model_draw asset=1 qwords=7 \
  packet=@md_01all_02_node_2_draws_list_0_draw_0_packet

label md_01all_02_node_2_draws_list_0_draw_0_packet
mesh_header triangles=2 vertices=3 controls=0x1878,0x0360
mesh_triangles
triangle 0,1,2,0 0,2,1,0
mesh_positions
position 10,0,-20 20,0,-20 10,0,-10
mesh_texcoords
texcoord 0,0 1,0 0,1
mesh_colors
color 128,128,128,128 128,128,128,128 128,128,128,128
mesh_program address=12
vif_nops count=1
```

The runtime creates one draw node per `model_item`. `node_id` is its lookup
identity, `parent` selects the parent draw node (`-1` is a root), the three
rotation values are Euler angles converted to a quaternion, and position and
scale become its local transform. `model_bounds` stores two local XYZ box
corners used by clipping. The `commands` pointer is compiled into the two
rendering slots for that node.

A `model_draw_set` is the node's null-terminated sequence of command lists.
Each `model_draw_list` supplies a rendering selector and an ordered sequence of
draws. The retail version-23 field corpus uses opcode 1 for every one of these
draws: `asset` selects the model's material state, `qwords` gives the exact
size of the referenced packet, and the packet contains one or more meshes.

Each mesh starts with its triangle and vertex counts plus two preserved control
words. Triangle records hold three vertex indices and a fourth byte whose role
is not yet named. Positions are XYZ floats. A mesh may then carry XYZ normals,
either two-float texture coordinates or an unclassified four-float vertex
attribute, and RGBA bytes. `mesh_program` selects the VU entry address used to
draw that mesh. The assembler derives the VIF UNPACK commands, element counts,
VU addresses, formats, and stream flags from this structure. `vif_nops`
preserves the zero words that align the complete packet to its 16-byte draw
boundary.

`packet_data` remains accepted for older source files containing this known
packet profile. Newly disassembled source uses the mesh form.

The variable-length asset table is also structural source:

```text
model_assets count=24
model_asset index=0 word_01=0x80969696 word_02=0x80969696
model_asset index=1 word_02=0x20808080 resource_04=0 \
  values_08=0,0,1,0.5,0 word_10=0x207f7f7f resource_20=0,1 \
  values_40=0,0,0.5,1,0 scalar_200=0.9
```

An asset's optional field suffix is its serialized flag bit. `resource_04`
indexes the resource list supplied by the model owner; `resource_20` stores
that index with its associated 16-bit value. The assembler derives the flag mask
from the fields present. It also derives the zero reserved words and checks
asset indices, command pointers, packet sizes, and relocation sites.

All 7,125 type-2 resources and 98,442 model items in the two version-23 disc
corpora use this same profile. Together they contain 161,220 asset entries,
86,571 draw sets, 94,520 command lists, and 140,113 typed draws. The draw
packets contain 904,770 meshes, 12,648,890 triangles, and 30,426,410 vertices.
All DDS1 meshes select program address 12. DDS2 has 503,825 at address 12 and
221 at address 16.
Their fixed VIF commands, control and padding words are derived or checked by
the parser.

Model motion is a playbook shared by every animated node and asset in one model:

```text
model_motion_playbook clip_count=1 bindings=5 clips=@motion_clips
model_motion_binding family=node selector=translation target=11
model_motion_binding family=node selector=scale target=11
model_motion_binding family=node selector=quaternion target=11
model_motion_binding family=asset selector=2 target=46
model_motion_binding family=asset selector=2 target=47

label motion_clips
model_motion_clip_table clips=@motion_clip_0

label motion_clip_0
model_motion_clip duration=60 reserved=0
model_motion_track format=vector3 frames=0
motion_vector3 140135.203125,-309.505859375,12446.6611328125
model_motion_track format=vector3 frames=0
motion_vector3 1.0399997234344482,1.0,0.9999998807907104
model_motion_track format=quaternion_s16 frames=0
motion_quaternion_s16 0,-356,0,4080
```

The high and low halves of each binding command select its target family and
channel. Node channels have names because their runtime effects are established;
asset selectors remain numeric where the property name is not. Each non-null
clip contains one size-prefixed key track per binding, in binding order. The
assembler derives the track size, key count, stride, frame padding, packed
command word, and all playbook relocations. It checks node and asset targets
against the model tables.

Track payloads use the shape required by their binding: XYZ floats, packed
signed-16 quaternions, byte flags, RGBA bytes, one float, or five floats. Frame
values remain unsigned 16-bit values, matching the retail evaluator. Most are
ordered normally, but 46 tracks start with high values such as `65535` before
zero; source preserves those values and does not impose a false monotonicity
rule.

Across the two version-23 disc corpora, the 7,125 playbooks contain 33,877
bindings and tracks with 165,446 keys. Every playbook has one clip, every clip
reserved word is zero, and all nine observed binding/stride profiles agree with
the retail dispatch handlers.

### Model export

The recovered hierarchy, draw graph, mesh packets, and node motion can be
exported directly from source to one self-contained glTF 2.0 binary:

```sh
python3 tools/fld_model.py src/dds2/data/field/f011_001.f1asm model.glb \
  --texture-bundle extracted/f011_001.tbn \
  --resource md_01all_01 --meters-per-unit 0.01 --frames-per-second 30
python3 tools/fld_model.py extracted/f011_001.LB model.glb \
  --meters-per-unit 0.01 --frames-per-second 30
```

`--resource` is repeatable and keeps large field files manageable. Without it,
the exporter writes every type-2 model in the FLD1. An LB input supplies its
paired F1 model and TBN texture bundle directly. A loose F1 binary or source
accepts the same bundle with `--texture-bundle`. The GLB embeds only textures
used by exported model assets; see [`tmx.md`](tmx.md) for their physical
format and standalone PNG decoder.

The GLB retains the model
and node hierarchy, resource and item transforms, indexed triangles, positions,
normals, texture coordinates, vertex colors, the unclassified four-float vertex
attribute, material identities, primary texture assignments, and the neutral
mesh control values as glTF extras. DDS vertex RGBA uses 128 as full intensity,
so the exporter expands it to glTF's normalized 255 range. Texture and vertex
alpha select a blending material. A draw without texture coordinates gets an
untextured material variant because glTF requires `TEXCOORD_0` for a bound
texture; its DDS texture identity remains in material metadata. Translation,
scale, Euler-rotation, and packed-quaternion node tracks become glTF animation
channels. Every motion binding and clip remains summarized on the resource
node; asset-property tracks are not converted to animation channels because
their material meanings are not yet established.

The two conversion options are explicit rather than assumed. Their defaults of
one preserve the numeric DDS units and frame values; choose the scale and
playback rate appropriate for the intended viewer. DDS axes are preserved. A
track whose u16 frame values are not increasing is recorded as skipped animation
metadata because glTF requires increasing input times. glTF also forbids
non-finite JSON transforms. The one observed retail model with a NaN static
translation omits that glTF component and records all three original IEEE-754
words in the node extras instead of silently inventing coordinates.

Across the archived field corpus, every one of the 150,694 primary and 3,948
secondary material texture references resolves inside its paired TBN bundle.
This covers 3,656 DDS1 and 3,415 DDS2 model resources.

### Composed field export

`tools/fld_scene.py` combines the visual FLD1 layer with the matching FLD2
world data. An LB archive already contains all three required payloads:

```sh
python3 tools/fld_scene.py extracted/f011_001.LB field.glb \
  --meters-per-unit 0.01 --frames-per-second 30
```

Loose binaries or source use explicit companions:

```sh
python3 tools/fld_scene.py src/dds1/data/field/f011_001.f1asm field.glb \
  --field src/dds1/data/field/f011_001.fldasm \
  --texture-bundle extracted/f011_001.tbn \
  --meters-per-unit 0.01 --frames-per-second 30
```

The resulting GLB contains the textured, animated model hierarchy together
with translucent unlit collision surfaces and transformed nodes for every
named camera and placement. Camera nodes retain their vertical field of view
as metadata. Placements use a shared octahedral marker so doors, event points,
terminals, and other script-facing positions are visible in ordinary glTF
viewers; `--placement-marker-size` sets its radius in native DDS units, or zero
hides the geometry while retaining the nodes and metadata.

Collision quads are triangulated as `(0,1,2)` and `(0,2,3)`; the retail
`0xffffffff` fourth-index sentinel selects a single triangle. Source face
controls remain authoritative in the FLD2 source, while mesh extras record
face and triangle counts. Unit conversion is shared with the model exporter,
so both layers stay in the same coordinate space and DDS axes remain intact.

The scene layer validates all 1,232 supported FLD2 payload occurrences across
both games: 5,533 collision resources and 167,623 output triangles, 1,551
cameras, and 10,222 placements. The paired DDS1 and DDS2 composed fields pass
the Khronos glTF validator without errors or warnings.

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

Faces that override the area's random-encounter zone use a semantic form:

```text
face attributes=0x00002000 move_floor=0 sound=0 stop=0 place=0 \
  automap=0,0 vertices=0,1,3,2 encounter_zone=61 special=0,0
```

The assembler derives the stored `{1, zone}` tag and requires it to agree with
attribute `0x2000`. Both game runtimes read the zone halfword when the player
enters one of these faces. Untagged faces retain the raw zero pair because the
two physical fields still exist.

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

The normal field-data build also links the complete FLD2 corpus to the rebuilt
`ENCOUNT.TBL`. For an `fNNN_AAA` source, the runtime selects default map `NNN`
and entry `AAA`, then applies either of that entry's flag-controlled alternate
zones. A collision face with `encounter_zone=N` overrides that result. The
linker rejects an out-of-range or empty zone, a malformed face tag, and an area
outside the encounter map's 64 entries. This currently closes 419 DDS1 and 452
DDS2 default-area links, plus all 40 DDS1 and 22 DDS2 face overrides.

## Archive boundary

Most field resources are stored as compressed blocks inside `.LB` archives.
The `.fldasm` and `.f1asm` sources describe the decompressed field objects.
`tools/lb.py` then places those outputs into the paired resource archive while
retaining any remaining blocks from an extracted retail base. See
[`lb.md`](lb.md) for the LB container, compression codec, archive source, and
exact build targets.

Run the codec, relocation, semantic-link, and tracked-source tests with:

```sh
python3 tools/test_fld.py
```
