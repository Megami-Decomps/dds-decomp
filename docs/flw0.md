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

Select a command profile when native command names are known for the game. The
profile works with both physical and symbolic source:

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
bytes; use version 1 for those files. It also accepts a short type-4 descriptor
when `PUSHTYPE5` references account for the complete physical string pool, as
described below.

### Embedded messages

The `messages msg1` block is editable source for the BMD/`MSG1` bank embedded
in section 3. It exposes dialog names and kinds, page or option boundaries,
speaker references, and the separate speaker table:

```text
messages msg1
  message MSG_START_00 speaker=0
    page
      control f2 08 ff ff
      control f2 07 07 ff
      font "から"
      newline
      control f1 04
    endpage
  endmessage
  select CHOICE
    option
      text "Yes"
    endoption
    option
      text "No"
    endoption
  endselect
  speaker 0
    font "人修羅"
  endspeaker
end
```

`text` holds printable single-byte ASCII. `font` holds characters from the
verified font-0 map and emits the corresponding two-byte glyph codes. The game
uses a 128-column font index beginning at code `0x8080`; it is not Shift-JIS.
When two codes map to the same Unicode character, one spelling is preferred
and the other remains an explicit `glyphs` directive, so disassembly and
assembly remain byte-exact. `glyphs` is also the fallback for a code missing
from the current map. DDS2 reuses the mapped code range; sequel-only or
otherwise unmapped codes remain explicit `glyphs` values.

`newline` emits byte `0x0A`, and `control` preserves a complete DDS control
sequence. The lead byte determines the control length, so the assembler can
reject a truncated sequence. `bytes` remains available inside a page, option,
or speaker when a stream does not fit those forms. NUL terminators, record
offsets, text lengths, alignment, and the packed relocation table are derived.
Changing message text in symbolic source therefore moves every later record
and pointer automatically. A selection may declare `ext`, `pattern`,
`reserved`, or `trailing` only when its physical record uses those fields.
`trailing=00`, for example, retains one extra byte after the final option
terminator.

Code that immediately passes a message-bank index to the profiled message
command uses the message declaration name instead of a numeric index:

```text
  PUSHMSG MSG_START_00
  COMM MESSAGE_REQUEST_AND_POLL
```

`PUSHMSG` assembles to the VM's ordinary `PUSHIS` instruction. Its operand is
the named dialog's current declaration-order index, so moving a dialog within
the bank also updates direct code references to it. The disassembler emits the
pseudo-instruction only for the adjacent `PUSHIS` and
`MESSAGE_REQUEST_AND_POLL` pattern, with a unique source-safe dialog name.
Other integer pushes remain numeric, including computed or ambiguous message
references.

Physical version-1 sources use the same records after an `msg1` marker inside
their type-3 section. Their section size remains fixed. Across the tracked
corpus, this form covers all 36 nonempty banks: 179 dialogs, 257 message pages,
16 selection options, and 27 speaker strings. Banks outside the verified
layout fall back to local raw bytes rather than receiving a partial decode.

### String symbols

Section 4 is an ordered byte pool. A `string` declaration emits its ASCII text
and terminating NUL, and binds its symbol to the first emitted byte:

```text
code
main:
  PUSHTYPE5 camera
  PUSHTYPE5 camera_motion
  END
end

strings
  string camera "cam01"
  string camera_motion "cam01_MOTION"
  zero 32
end
```

Changing an earlier string moves later symbols and updates their encoded
`PUSHTYPE5` operands automatically. Equal text at different offsets remains as
separate declarations; the assembler never interns it. The disassembler only
creates a declaration when a referenced offset begins a printable,
NUL-terminated ASCII string. Other operands remain numeric and the bytes stay
in local `bytes` or `zero` directives.

The DDS1 event corpus has 315 such instructions in 13 files. Every operand
lands at the start of a distinct valid string, so all 315 are symbolic. Five
physical version-1 files address records beyond the type-4 descriptor's logical
length. Their section line carries an explicit physical `extent`, for example:

```text
section 4 type=4 stride=0x1 count=48 offset=0x1243 extent=0x174
```

`count` remains the exact retail descriptor value; `extent` says how many
physical bytes the following source directives own. This exposes the
descriptor/physical-length mismatch without splitting the readable pool into
unrelated top-level preservation records.

Symbolic source represents the same layout with `count` on the strings block:

```text
strings count=32
  string Camera01_MOTION "Camera01_MOTION"
end
```

The count stays equal to the retail type-4 descriptor while the directives own
the complete physical pool. The symbolic disassembler uses this form only when
referenced, NUL-terminated strings account for every byte through the end of
the file. Unreferenced trailing data still requires the physical format. All
22 tracked DDS2 field scripts satisfy this stronger condition.

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

The DDS1 and DDS2 profiles contain a reviewed shared command set. Each ID,
handler, stack read, and result write was checked against both PS2 command
tables and both implementations:

| Source name | ID | Stack values consumed | Verified handler behavior |
|---|---:|---:|---|
| `MESSAGE_REQUEST_AND_POLL` | `0x000` | 1 | Starts a message entry or waits for its current work |
| `ACTIVATE_MESSAGE_PANEL` | `0x001` | 0 | Activates the current message panel |
| `FINISH_SCRIPT_MESSAGE_WINDOW` | `0x002` | 0 | Waits for and finishes the active script message window |
| `SET_MODEL_FLAG` | `0x008` | 1 | Sets a model flag |
| `CLEAR_MODEL_FLAG` | `0x009` | 1 | Clears a model flag |
| `WAIT_FOR_TIMER_LIMIT` | `0x00E` | 1 | Waits until the command timer reaches a limit |
| `SCREEN_FADE_A` | `0x00F` | 2 | Starts the selected screen fade when its timer reaches zero |
| `RESET_DRAW_EFFECTS` | `0x043` | 0 | Clears draw transitions and effect enables |
| `RETURN_TO_TITLE` | `0x046` | 0 | Requests the title scene |
| `CALL_EVENT` | `0x066` | 1 | Submits an event request and clears named processes |
| `PREPARE_UNIT_MOTION_STATE` | `0x073` | 5 | Looks up an event unit and applies four motion-state values |
| `READ_SECONDARY_WORLD_ID_VALUE` | `0x094` | 1 | Looks up a named secondary-world ID and returns its value or zero |
| `RESET_FIELD_EFFECTS` | `0x099` | 0 | Resets field draw, sway, sky, and fade state |
| `WAIT_FOR_TASK_REMOVAL` | `0x0A7` | 1 | Waits until a task ID leaves the task queues |
| `CREATE_POLYGON_MOVIE` | `0x0AA` | 2 | Creates an EventViewer task and returns its task ID |
| `CLEAR_PROCESS_CONTROL_FLAG` | `0x1E7` | 0 | Clears the script-process control flag |

The assembler resolves these names to numeric operands. `COMM 0xNNNN` remains
valid for commands outside the reviewed profile. A name is rejected when the
source has no profile or the selected profile does not define it. Profiles are
kept separate because the same command ID can differ between engine versions;
for example, DDS1 `0x1E7` consumes no stack values and does not have Nocturne
HD's two-argument behavior.

The reviewed set names 2,231 of 3,881 native calls in the DDS1 event corpus and
14,659 of 38,850 calls in the complete DDS2 corpus. It also makes the adjacent
message-command pattern safe to recognize, producing 188 symbolic DDS1 message
references and 1,910 symbolic DDS2 references. Every other command and every
dynamic or ambiguous message operand remains numeric.

## Tracked corpora

The repository tracks 104 DDS1 event scripts and all 140 DDS2 BF programs:
117 event scripts, 22 field scripts, and the battle negotiation script. Of the
DDS2 sources, 126 use symbolic version 2. Fourteen global event programs use
the lossless physical form because they have a distinct four-section layout.
The 66 nonempty DDS2 message banks contain 2,437 dialog records, 2,673 message
pages, 1,149 selection options, and 177 speaker strings. Fifty-nine banks use
editable MSG1 records; the seven banks outside the verified MSG1 layouts keep
their bytes locally.

Assemble one source directly with:

```sh
python3 tools/flw0.py assemble src/dds1/scripts/event/e670.bfasm e670.bf
```

After configuring the repository, assemble and verify either corpus with:

```sh
ninja dds1-scripts
ninja dds2-scripts
```

The normal `ninja dds1` and `ninja dds2` targets include their corresponding
checks. Expected output hashes live in `config/dds1/event_scripts.sha1` and
`config/dds2/scripts.sha1`, so verification does not require the original BF
files.

## Reading view

The exact assembly remains deliberately close to the VM. Use `view` when you
want to read its stack operations as expressions and profiled calls:

```sh
python3 tools/flw0.py view src/dds1/scripts/event/e670.bfasm
```

Tracked source supplies its own command profile. Pass one explicitly when
viewing an older or external source that does not declare one:

```sh
python3 tools/flw0.py view --profile dds1 event.bfasm
```

The output keeps one PC-anchored statement for every instruction. For example,
the task creation and wait in `e670` become:

```text
  0004: push 1
  0005: push 670
  0006: result = CREATE_POLYGON_MOVIE(1, 670)
  0007: push result
  0008: WAIT_FOR_TASK_REMOVAL(result)
```

This is a derived reading aid, not another source format. An unprofiled native
command is printed numerically and invalidates the inferred stack; later
self-contained pushes can still form known arguments. Procedure and jump-label
entries also start with unknown stack state. Unsupported instructions stay
visible at their original PC instead of being guessed away. Float literals
retain their exact bit pattern. Every one of the 315 `PUSHTYPE5` operands in the
tracked corpus lands on a NUL-delimited ASCII run in section 4, so the view
shows both the byte offset and that observed text while retaining the
conservative `type5_ref` name. The current profile resolves 1,334 of the
corpus's 3,881 native-command instructions and reaches every tracked event
file. Binary expressions follow the VM's verified order: the top stack value
is the left operand and the next value is the right operand.

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

Unknown sections and unrecognized message banks use `bytes HEX`; string pools
use it locally for data that cannot be represented by a `string` declaration.
An all-zero region uses the shorter `zero SIZE`. Unusual procedure or label
rows fall back to `row HEX`, and other bytes outside declared section extents
use `preserve` with an absolute offset. These escapes are local: understood
code and table rows stay readable even when another part of the file is
opaque.

The header exposes the signed integer and float local counts used by the DDS
VM. Other fields retain offset-based names when their purpose is not established.

## Physical editing boundary

The current writer preserves the existing physical layout. It supports edits
whose encoded data still matches the descriptor sizes, offsets, and any
explicit physical string extent. It rejects missing bytes, changed extents,
inconsistent overlaps, invalid code addresses, and out-of-range operands.

Use symbolic version 2 when an edit changes section size. Archive insertion is
outside this tool; the assembler produces the rebuilt BF file.

Run the synthetic regression tests with:

```sh
python3 tools/test_flw0.py
```
