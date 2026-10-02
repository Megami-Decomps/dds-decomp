# Field environment tables

`tools/npl.py` and `tools/sky.py` reconstruct the fixed tables that control
field character palettes and lighting. Both source formats describe a complete
default record followed by the indices that differ from it. This reflects the
retail data—most of each table repeats one record—while every meaningful or
unclassified byte remains reproducible.

## NPL palettes

An NPL row is eight bytes:

| Offset | Size | Source field |
|---:|---:|---|
| `0x00` | 3 | `primary`, RGB |
| `0x03` | 3 | `secondary`, RGB |
| `0x06` | 1 | `texture`, two packed four-bit coordinates |
| `0x07` | 1 | `direction` |

Ordinary field files contain 64 rows. DDS1 `F009.NPL` is the one 32-row
profile. The row count is part of the source header:

```text
npl 1 count=64

default primary=#737373 secondary=#333333 texture=15,0 direction=0
palette 1 primary=#736666 secondary=#2c2c2c texture=15,0 direction=50
```

## SKY light sets

A SKY file contains 256 records of `0xe0` bytes. The field renderer reads the
fade and sway state, a fixed-point draw color, three directional lights, the
background color, and character-lighting values directly from the selected
record. The four-component draw vector is written as logical `X,Y,Z,W`; the
codec handles the retail `X,Z,Y,W` storage order.

Each source record is complete:

```text
sky 1

default
  state type=0 display=0 fade=0 sway=0
  draw_vector 255,0,19999,20000
  draw_color 64,80,96
  light 0 color=0.7,0.7,0.7 direction=0.5,0.3,0.5
  light 1 color=0.0,0.0,0.0 direction=1.0,0.0,0.0
  light 2 color=0.0,0.0,0.0 direction=0.0,0.0,0.0
  background 0.2,0.2,0.2
  unit_color_a 0.7,0.7,0.7
  unit_direction 0.5,0.3,0.5
  reserved 0000000000000000000000000000803f0000000000000000000000000000000000000000000000000000000000000000
  unit_color_b 0.2,0.2,0.2
end
```

The `reserved` field covers the unclassified `0xa4..0xd3` region. It is kept
visible and exact rather than assigned guessed semantics. Float values use the
shortest decimal that reproduces their IEEE-754 single-precision bits;
`bits:0xXXXXXXXX` is available for values that cannot be written safely as a
decimal.

## Build and corpus verification

The normal build assembles every tracked source and checks it against the
manifest:

```sh
python3 configure.py --no-split
ninja dds1-field-data dds2-field-data
```

The corpus importer validates the game executable, reads the loose resources
from `DDS3.IMG`, round-trips all of them in memory, and can regenerate the
tracked source and manifests from a disc image you own:

```sh
python3 tools/field_environment_corpus.py dds1 /path/to/game.iso
python3 tools/field_environment_corpus.py dds2 /path/to/game.iso
```

Standalone operations are also available:

```sh
python3 tools/npl.py disassemble F011.NPL
python3 tools/npl.py assemble f011.nplasm F011.NPL
python3 tools/sky.py disassemble F011.SKY
python3 tools/sky.py assemble f011.skyasm F011.SKY
```
