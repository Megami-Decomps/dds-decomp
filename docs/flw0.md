# DDS BF/FLW0 scripts

`tools/flw0.py` reads DDS BF script containers and converts them to a small,
editable assembly format. Its preserve-layout writer keeps the original header
values, section descriptors, offsets, gaps, padding, and trailing bytes. An
untouched source file therefore assembles to the same bytes as its input.

Disassemble a BF file:

```sh
python3 tools/flw0.py disassemble event.bf event.bfasm
```

Assemble it again:

```sh
python3 tools/flw0.py assemble event.bfasm rebuilt.bf
```

The tool can also inspect a container or verify its binary codec directly:

```sh
python3 tools/flw0.py inspect event.bf
python3 tools/flw0.py verify event.bf
```

## Source format

The source is deliberately close to the VM. It records the physical layout
once, then gives known sections readable forms. A small synthetic script looks
like this:

```text
flw0 1

header word00=0x00000000 declared_size=0x000000a4 word0c=0x00000000 int_locals=0 float_locals=0 word18=0x00000000 word1c=0x00000000 physical_size=0x194

section 0 type=0 stride=0x20 count=1 offset=0x70
  proc "sample" pc=0 reserved=0x00000000
end

section 1 type=1 stride=0x20 count=0 offset=0x90
end

section 2 type=2 stride=0x4 count=5 offset=0x90
  0000: PROC 0x0000  # "sample"
  0001: PUSHIS 0x002a
  0002: COMM 0x0001
  0003: PUSHREG
  0004: END
end

section 3 type=3 stride=0x1 count=0 offset=0xa4
end

section 4 type=4 stride=0x1 count=240 offset=0xa4
  zero 240
end
```

Procedure and jump-label table rows use `proc` and `label`. Code uses the DDS
FlowScript opcode names established by the recovered VM; opcode 34 keeps the
conservative name `PUSHTYPE5` while its payload type remains uncertain. The
displayed word address is checked by the assembler, so a missing extended
operand cannot silently shift the rest of the program.

Instruction operands are exact unsigned bit fields in the disassembly.
`PUSHIS` also accepts signed decimal input, such as `PUSHIS -1`, and encodes it
as a 16-bit two's-complement value. Procedure and jump target names after `#`
are explanatory comments in this first format; the numeric table index remains
the assembled operand.

Raw message, string, and unknown sections use `bytes HEX`. An all-zero region
uses the shorter `zero SIZE`. Unusual procedure or label rows fall back to
`row HEX`, and bytes outside declared section extents use `preserve` with an
absolute offset. These escapes are local: understood code and table rows stay
readable even when another part of the file is opaque.

The header exposes the signed integer and float local counts used by the DDS
VM. Other fields retain offset-based names when their purpose is not established.

## Editing boundary

The current writer preserves the existing physical layout. It supports edits
whose encoded data still matches the descriptor sizes and offsets. It rejects
missing bytes, changed section lengths, inconsistent overlaps, invalid code
addresses, and out-of-range operands.

Growing a section requires a distinct relayout mode and archive update. Keeping
that operation separate makes ordinary low-level edits predictable and makes
exact reconstruction straightforward to verify.

Run the synthetic regression tests with:

```sh
python3 tools/test_flw0.py
```
