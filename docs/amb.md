# Automap resources (`AMB`)

`tools/amb.py` converts standalone field automaps to editable source and back:

```sh
python3 tools/amb.py disassemble f011.amb f011.ambasm
python3 tools/amb.py assemble f011.ambasm f011.amb
python3 tools/amb.py verify f011.amb
```

The tracked corpus contains all 32 DDS1 and 22 DDS2 resources. Together they
define 1,446 named map areas, 3,780 sub-blocks, and 1,625 icons. The normal
`dds1-field-data` and `dds2-field-data` targets assemble every source and check
its retail SHA-1.

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
                -> model root -> geometry and material object graphs
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

The nested geometry and material formats are not yet fully classified. Their
bytes remain explicit, while each relocated word is emitted as
`pointer @label`. This keeps unknown structures lossless and relocatable:

```text
label area_001_geometry
bytes 08000000000000000000000000000000
pointer @loc_00400
pointer @loc_00418
```

Moving any labeled object updates typed and unresolved pointers alike. The
codec rejects malformed extents, noncanonical relocation streams, misplaced
relocations, invalid fixed strings, overlapping typed objects, and pointer
targets inside typed records that source could not represent.
