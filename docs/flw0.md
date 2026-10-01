# DDS BF/FLW0 scripts

`tools/flw0.py` reads DDS BF script containers and converts them to a small,
editable assembly format. It has two source forms: a physical format for exact
forensic work and a symbolic format for maintained script decompositions.

Disassemble a BF file:

```sh
python3 tools/flw0.py disassemble event.bf event.bfasm
```

Use `--symbolic` to derive source whose layout can change:

```sh
python3 tools/flw0.py disassemble --symbolic event.bf event.bfasm
```

Select a command profile when native command names are known for the game:

```sh
python3 tools/flw0.py disassemble --symbolic --profile dds1 event.bf event.bfasm
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

## Symbolic source

Version 2 is the normal form for scripts kept in the repository. Procedure and
jump-label declarations create symbols; labels in the code block give them
word addresses. Instructions refer to those symbols instead of table indices:

```text
flw0 2

header word00=0 word0c=0 word18=0 word1c=0
locals int=2 float=1

procedure main
procedure helper name="helper_proc"
jump_label finished

code
main:
  PROC main
  CALL helper
  IF finished
helper:
  PROC helper
finished:
  END
end

messages
end

strings
  zero 240
end
```

Declaration order defines the procedure and jump-label table indices. The
assembler derives each table address from its code label, counts the encoded
words, places all five sections consecutively, and computes their descriptors
and the header size field. Adding or removing instructions therefore updates
later addresses and offsets without hand-editing bookkeeping.

The symbolic disassembler accepts the standard DDS five-section layout and
canonical 32-byte name rows. It rejects irregular layouts rather than hiding
bytes; use version 1 for those files. Message and string payloads remain raw
until their internal formats are understood. In the current DDS1 event corpus,
97 of 104 files use the canonical layout and round-trip exactly through this
form; the other seven remain exact through version 1.

### Native command profiles

Native `COMM` IDs belong to a specific game and executable version. A `profile`
directive makes that choice part of the source instead of inferring it from a
path:

```text
flw0 2
profile dds1

# ...
  COMM RESET_DRAW_EFFECTS
  PUSHIS 1
  PUSHIS 670
  COMM CREATE_POLYGON_MOVIE
  PUSHREG
  COMM WAIT_FOR_TASK_REMOVAL
```

The DDS1 profile currently contains the six commands used by `e670`. Their IDs,
handlers, and stack consumption were checked against the DDS1 PS2 command table
and runtime code:

| Source name | ID | Stack values consumed | DDS1 handler behavior |
|---|---:|---:|---|
| `CLEAR_PROCESS_CONTROL_FLAG` | `0x1E7` | 0 | Clears the script-process control flag |
| `RESET_DRAW_EFFECTS` | `0x043` | 0 | Clears draw transitions and effect enables |
| `RESET_FIELD_EFFECTS` | `0x099` | 0 | Resets field draw, sway, sky, and fade state |
| `CREATE_POLYGON_MOVIE` | `0x0AA` | 2 | Creates an EventViewer task and returns its task ID |
| `WAIT_FOR_TASK_REMOVAL` | `0x0A7` | 1 | Waits until a task ID leaves the task queues |
| `RETURN_TO_TITLE` | `0x046` | 0 | Requests the title scene |

The assembler resolves these names to numeric operands. `COMM 0xNNNN` remains
valid for commands outside the reviewed profile. A name is rejected when the
source has no profile or the selected profile does not define it. Profiles are
kept separate because the same command ID can differ between engine versions;
for example, DDS1 `0x1E7` consumes no stack values and does not have Nocturne
HD's two-argument behavior.

The first tracked script is `src/dds1/scripts/event/e670.bfasm`. Assemble it
with the same command as any other source:

```sh
python3 tools/flw0.py assemble src/dds1/scripts/event/e670.bfasm e670.bf
```

## Physical source

Version 1 stays deliberately close to the container. It records physical layout
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

## Physical editing boundary

The current writer preserves the existing physical layout. It supports edits
whose encoded data still matches the descriptor sizes and offsets. It rejects
missing bytes, changed section lengths, inconsistent overlaps, invalid code
addresses, and out-of-range operands.

Use symbolic version 2 when an edit changes section size. Archive insertion is
outside this tool; the assembler produces the rebuilt BF file.

Run the synthetic regression tests with:

```sh
python3 tools/test_flw0.py
```
