# Field texture bundles (`TBN`, `TXP0`, and `TMX0`)

`tools/tmx.py` decodes the texture bundle paired with a field's FLD1 models.
It accepts either the complete type-9 TBN packet or its embedded TXP0 payload
and writes one RGBA PNG per texture:

```sh
python3 tools/tmx.py extracted/f011_001.tbn textures/f011_001
```

The decoder uses only the Python standard library. It checks every packet
boundary, texture offset, header profile, palette size, mip level, and pixel
payload before writing output.

## Container layout

A field TBN is a normal eight-byte SDF resource packet followed by TXP0. TXP0
contains a texture count and file-relative offsets to ordered type-2 TMX0
packets. Model asset fields `resource_04` and `resource_20` select entries in
that order. The TMX0 image header is `0x38` bytes from its magic through the
start of image data; including the child packet header, pixel data begins at
offset `0x40`.

The complete DDS1 and DDS2 field corpora contain 1,117 bundles and 15,506
textures. One DDS2 bundle is intentionally empty. All 154,642 texture
references in archived FLD1 material assets resolve within the corresponding
bundle.

## Pixel profiles

| GS mode | Payload |
|---|---|
| `PSMCT32` (`0x00`) | Four-byte RGBA pixels |
| `PSMCT24` (`0x01`) | Three-byte RGB pixels; decoded alpha is opaque |
| `PSMCT16` (`0x02`) | Packed RGB5A1 pixels |
| `PSMT8` (`0x13`) | 256-entry RGBA32 palette followed by byte indices |
| `PSMT4` (`0x14`) | 16-entry RGBA32 or RGB5A1 palette followed by low-nibble-first indices |

PSMT8 palettes use the GS CSM1 order, so entries 8–15 and 16–23 in every
32-entry group are exchanged when converted to linear index order. PS2 alpha
runs from 0 through 128; PNG output expands that range to 0 through 255.

Some field textures contain one additional mip level. The TMX0 header records
the count and the payload stores successively halved images after the base
level, sharing the indexed texture's palette. PNG output represents the base
level, while the decoder retains the mip count and texture control word for
model-export metadata.

## Model export

`tools/fld_model.py` accepts an LB archive directly, or a loose F1/F1 source
plus `--texture-bundle`. It follows each model draw through its material asset
to the ordered TBN entry and embeds referenced PNGs in the self-contained GLB.
Secondary texture identities are retained as material metadata until their
blend operation is recovered. See [`fld.md`](fld.md#model-export) for the
complete command and model conversion rules.
