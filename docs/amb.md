# Automap resources (`AMB`)

`tools/amb.py` converts standalone field automaps to editable source and back:

```sh
python3 tools/amb.py disassemble f011.amb f011.ambasm
python3 tools/amb.py assemble f011.ambasm f011.amb
python3 tools/amb.py verify f011.amb
```

The tracked corpus contains all 32 DDS1 and 22 DDS2 resources. Together they
define 1,446 named map areas, 3,780 sub-blocks, 1,625 icons, 10,739 model
nodes, and 6,143 meshes. The normal `dds1-field-data` and `dds2-field-data`
targets assemble every source and check its retail SHA-1.

`tools/amb_corpus.py` verifies the complete AMB set directly from a disc image
you own. Supplying both output options imports the sources and manifest only
after every file has reconstructed exactly:

```sh
python3 tools/amb_corpus.py dds1 /path/to/game.iso
python3 tools/amb_corpus.py dds1 /path/to/game.iso \
  --output-dir src/dds1/data/field \
  --manifest config/dds1/field_amb.sha1
```

## Object graph

An AMB starts with a `0x20`-byte `ATMP` header. Its data region is followed by
the same packed relocation stream used by FLD1 and FLD2. The header relocates
three words: the end of the data region, the start of the data region, and the
area table. The assembler derives the data sizes and complete relocation stream
from labels.

```text
header -> areas -> name
                -> sub-blocks -> name
                              -> icons -> position
                              -> minimum/maximum bounds
                -> model root -> node hierarchy -> bounds and draw graph
                              -> material assets  -> VIF mesh packets
                -> position
```

The recovered source names the area and sub-block tables, fixed-width names,
model roots, icon records, positions, bounds, node indices, and floors:

```text
header version=3 magic=ATMP areas=@areas count=1

label areas
area name=@area_001_name sblocks=@area_001_sblocks count=3 \
  model=@area_001_model position=@area_001_position

label area_001_sblocks
sblock name=@area_001_s01_name node=0 icons=@area_001_s01_icons count=1 \
  floor=0 bounds=@area_001_s01_bound_min,@area_001_s01_bound_max

label area_001_s01_icons
icon type=5 position=@area_001_s01_icon_1_position
```

Some zero-count icon lists retain a non-null pointer, and some sub-blocks omit
one or both bounds. Source preserves those distinctions because they are part
of the original object graph.

The model root uses the same SDF node, asset, draw, and mesh structures as
FLD1 model resources, without the FLD1 resource wrapper or motion playbook.
AMB source therefore shares the established model vocabulary:

```text
label area_001_geometry
model_items count=8
model_item node_id=0 parent=-1 rotation=0,-1.57079637,-0 \
  position=0,-0,0,1 scale=1,1,1,0 bounds=null commands=null
model_item node_id=4 parent=3 rotation=-0,-0,-0 \
  position=39100,-0,-16238.109375,1 scale=1,1,1,0 \
  bounds=@area_001_node_4_bounds commands=@area_001_node_4_draws

label area_001_node_4_draws_list_0_draw_0_packet
mesh_header triangles=2 vertices=4 controls=0x0048,0x0060
mesh_triangles
triangle 0,1,2,0 3,2,1,0
mesh_positions
position -1300,0,-5299.9814453125 -1300,0,-6100 \
  1100,0,-5299.9814453125 1100,0,-6100
mesh_program address=12
```

The complete corpus contains 4,883 material assets, 5,399 draws and packets,
35,528 triangles, and 60,629 vertices. Every object is typed: canonical source
has no residual `bytes` or generic `pointer` directives. Names remain neutral
for the two mesh control halfwords and asset selectors whose rendering roles
are not yet established.

Moving any labeled object updates the complete pointer graph. The codec rejects
malformed extents, noncanonical relocation streams, misplaced relocations,
invalid fixed strings, overlapping typed objects, invalid hierarchy or asset
references, inconsistent draw sizes, and malformed VIF mesh packets.

## Runtime field identity

Ordinary field rooms and automap rows have an exact index relationship. Loading
`fNNN_AAA.LB` sets the active field to `NNN` and the active automap area index
to `AAA - 1`. The shared `fNNN_000.LB` archive supplies the base `fNNN.AMB`;
the runtime installs its area table without looking up the area's string name
or testing its coordinates. The names remain useful presentation data, but are
not the room-to-area key.

On a collision face with attribute `0x800`, the first `automap` byte is a
one-based sub-block selector. The runtime records selector `N` as discovery bit
`N - 1`, searches that same zero-based row in the active AMB area, and returns
the row's signed `floor + 1`. A missing row takes the native default return of
`1`. The second byte is retained as the separate numeric `upperName` channel;
it is not treated as another sub-block identity.

The maintained ordinary field sources link 464 of 554 DDS1 rooms and 477 of
568 DDS2 rooms to an in-range base AMB row. DDS1 has 86 fields without a base
map and four out-of-range room indices; DDS2 has 84 and seven. The complete
collision corpus contains 2,956 DDS1 and 2,109 DDS2 discovery-tagged faces.
Of these, 2,547 and 1,986 resolve to a sub-block, 344 and 18 take the native
default-floor path, and 65 and 105 belong to resources without a proven active
automap row. The latter split into 63/102 faces in `k*` resources and 2/3 in
out-of-range ordinary rooms; the ordinary rooms without a base map contain no
tagged faces. These cases remain explicit in graph output.

## Scene export

`tools/amb_scene.py` turns the decoded graph into a self-contained glTF 2.0
GLB. It accepts either a retail binary or tracked source:

```sh
python3 tools/amb_scene.py f024.amb f024.glb --meters-per-unit 0.01
python3 tools/amb_scene.py src/dds1/data/field/f024.ambasm f024.glb \
  --area 003 --meters-per-unit 0.01
python3 tools/amb_scene.py src/dds1/data/field/f024.ambasm f024-area-0.glb \
  --area-index 0 --meters-per-unit 0.01
```

The export retains every selected area's native model hierarchy, transforms,
indexed meshes, vertex channels, draw selectors, and unresolved mesh controls.
Area positions, sub-block names, floors, bounds, model-node links, and icon
types are glTF metadata, along with the complete neutral asset fields. The
matching root node is identified where the area position repeats its transform.
Icons also receive a reusable octahedral marker;
`--icon-marker-size 0` omits that diagnostic geometry. Native axes are
preserved, and `--meters-per-unit` applies one explicit scale to geometry,
transforms, bounds, positions, and markers.

Area names and zero-based indices are alternative selectors. Index selection
is useful when reproducing the field runtime's `AAA - 1` choice; the composed
field exporter applies it automatically.

All 54 retail AMBs export without an alternate parser: the scene exporter
consumes the same decoded model graph used by disassembly and reuses the FLD1
SDF-to-glTF consumer. The complete pass emits 1,446 areas, 13,810 glTF nodes,
3,938 meshes, and 1,625 icon markers.
