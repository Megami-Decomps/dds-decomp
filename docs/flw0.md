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

Add `--semantic` to produce editable expressions and statements wherever the
instruction sequence has one exact lowering:

```sh
python3 tools/flw0.py disassemble --symbolic --semantic --profile dds1 event.bf event.bfasm
```

Use `--structured` to additionally recover exact canonical branches and loops:

```sh
python3 tools/flw0.py disassemble --symbolic --structured --profile dds1 event.bf event.bfasm
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

### Hybrid semantic code

The version-2 code block may mix VM instructions with semantic statements.
This is an exact source format rather than a reading view: every expression,
assignment, call, and structured block expands to a fixed instruction
sequence. For example:

```text
code
main:
  PROC main
  CLEAR_PROCESS_CONTROL_FLAG()
  local_int[0] = MESSAGE_SELECTION_REQUEST_AND_POLL(selection(CHOICE))
  result = CREATE_SCRIPT_TASK(procedure(worker), -1)
  WAIT_FOR_TASK_REMOVAL(result)
  return
end
```

Native call arguments use handler order. Argument zero is the VM stack top, so
the compiler emits the arguments in reverse source order. Binary expressions
follow the same rule. A 16-bit integer is written directly; `int32(value)`
preserves a `PUSHI`, and `float32(value, bits=0xNNNNNNNN)` preserves the exact
bits of a `PUSHF`. The typed constructors `message`, `selection`, `event`,
`procedure`, and `string` lower to their existing symbolic pseudo-instructions.

Assignments may target `global_int`, `global_float`, `local_int`, or
`local_float`. A result-producing native call can be assigned directly, or it
can be named as `result` and read later:

```text
  local_int[3] = READ_CURRENT_WORLD_OBJECT_ID()
  result = CREATE_POLYGON_MOVIE(670, 1)
  WAIT_FOR_TASK_REMOVAL(result)
```

A profiled command may give a small integer argument a command-specific name:

```text
  local_int[2] = READ_TREASURE_TABLE_VALUE(ITEM_QUANTITY)
```

Here `ITEM_QUANTITY` lowers exactly to `PUSHIS 2`. The name is valid only in
that argument of `READ_TREASURE_TABLE_VALUE`, which keeps unrelated integer
domains separate. The disassembler uses a name only for a direct profiled
`PUSHIS`; computed selectors, wider `PUSHI` values, and values outside the
reviewed set remain ordinary expressions or numbers.

Canonical branches and loops use ordinary blocks while retaining the labels
already required by the exact jump table:

```text
again:
  while (local_int[0] < 10) {
    local_int[0] = local_int[0] + 1
  }
done:

  if (local_int[0] != 0) {
    SET_MODEL_FLAG(7)
  } else {
otherwise:
    CLEAR_MODEL_FLAG(7)
  }
after_if:
```

A `while` needs its back-edge label immediately before the block and its exit
label immediately after it. An `if` needs its end label immediately after the
block; an `else` begins with the false-arm label. These labels preserve the
original table identities and make the lowering explicit without exposing the
branch opcodes in the body.

Assembly remains valid anywhere in the same code block. The semantic
disassembler lifts only linear instruction runs with a complete, verified
stack contract. It flushes pending values before labels and leaves unknown
commands, malformed words, and ambiguous stack state as instructions. This is
the escape hatch that keeps partial decompilation exact.

The structured disassembler additionally replaces canonical branch and
back-edge sequences with the blocks above. It retains the linear semantic form
when a region has an external entry, crossing control flow, an unknown
condition, or a jump-table alias whose exact operand cannot be expressed by
the block boundary. Structured output is therefore subject to the same exact
assemble check as linear semantic output.

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

Selections use a distinct typed reference before the profiled selection
command:

```text
  PUSHSELECT CHOICE
  COMM MESSAGE_SELECTION_REQUEST_AND_POLL
```

`PUSHSELECT` also assembles to `PUSHIS`, but its symbol must name a `select`
record. A message declaration is rejected even when it has a valid source
name. The disassembler uses this form only for an adjacent literal passed to
`MESSAGE_SELECTION_REQUEST_AND_POLL`; dynamic, ambiguous, and raw-bank
references remain numeric. The command waits for the selection to finish and
returns the selected value through the VM result register.

An adjacent literal target for the profiled event command uses the maintained
event script's resource name:

```text
  PUSHEVENT e602
  COMM CALL_EVENT
```

`PUSHEVENT` also assembles to `PUSHIS`. The selected game profile binds names
to the event IDs present in its maintained corpus. This mirrors the runtime,
which formats event ID 602 as
`/event/e600/e602/scr/e602.bf`. The disassembler emits the name only for an
adjacent literal `PUSHIS` followed by `CALL_EVENT` and only when that event
target is maintained for the selected game. Dynamic values and references to
absent targets stay numeric.

The reading view carries the same evidence as `CALL_EVENT(event(e602))` while
leaving an unresolved literal as `CALL_EVENT(10)`.

A literal procedure-table index passed to the script-task command uses a local
procedure symbol:

```text
  PUSHIS 0
  PUSHPROC worker
  COMM CREATE_SCRIPT_TASK
  PUSHREG
  COMM WAIT_FOR_TASK_REMOVAL
```

`PUSHPROC` assembles to `PUSHIS`. Its operand follows the procedure declaration
when procedures are reordered, just like the operands of `CALL`, `JUMP`, and
`PROC`. The disassembler emits it only for an in-range literal immediately
before `CREATE_SCRIPT_TASK`; unrelated integers stay numeric. The other
argument is a priority adjustment relative to the current script task, and the
command returns the created task handle. That handle may be waited on, stored
for later destruction, or intentionally left running.

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

The complete DDS1 corpus has 7,863 such instructions; the original event slice
accounts for 315 instructions in 13 files. Every operand lands at the start of
a valid string, so all 7,863 are symbolic. A physical version-1 source can
preserve a string pool beyond the type-4 descriptor's logical length by giving
the section an explicit physical `extent`, for example:

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
30 affected DDS1 scripts and all 22 DDS2 field scripts satisfy this stronger
condition.

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
| `MESSAGE_SELECTION_REQUEST_AND_POLL` | `0x003` | 1 | Builds or waits for a selection and returns the selected value |
| `TEST_MODEL_FLAG` | `0x007` | 1 | Tests a model flag and returns the result |
| `SET_MODEL_FLAG` | `0x008` | 1 | Sets a model flag |
| `CLEAR_MODEL_FLAG` | `0x009` | 1 | Clears a model flag |
| `WAIT_FOR_TIMER_START` | `0x00D` | 0 | Waits until the current command timer becomes nonzero |
| `WAIT_FOR_TIMER_LIMIT` | `0x00E` | 1 | Waits until the command timer reaches a limit |
| `SCREEN_FADE_A` | `0x00F` | 2 | Starts the selected screen fade when its timer reaches zero |
| `SCREEN_FADE_B` | `0x010` | 2 | Starts the selected screen fade-in when its timer reaches zero |
| `ADD_EFFECT_UNIT_TO_WORLD` | `0x012` | 1 | Adds the selected player object or effect-unit ID to the active world |
| `ADD_FLAGGED_EFFECT_UNIT_TO_WORLD` | `0x019` | 1 | Adds the selected player object, or marks and adds an effect-unit ID |
| `RESET_DRAW_EFFECTS` | `0x043` | 0 | Clears draw transitions and effect enables |
| `RETURN_TO_TITLE` | `0x046` | 0 | Requests the title scene |
| `WAIT_FOR_UNIT_MOTION` | `0x049` | 1 | Waits until the selected unit's motion is idle or in its timed mode |
| `ATTACH_WORLD_OBJECT_TO_SOURCE_VECTOR` | `0x04A` | 2 | Attaches a world object to a source vector |
| `SET_UNIT_VALUE` | `0x04B` | 2 | Writes the selected unit's 16-bit value at offset `0xBC` |
| `ACTION_WINDOW_REQUEST_AND_POLL` | `0x05E` | 1 | Requests or polls the current action-window message and returns `-1`, `0`, or `1` |
| `RESTORE_CAMERA_NODE_MODE` | `0x060` | 0 | Resets the player scene-object state and restores camera node mode |
| `RELEASE_CURRENT_OBJECT` | `0x061` | 0 | Releases the current field object and refreshes field state |
| `CALL_EVENT` | `0x066` | 1 | Submits an event request and clears named processes |
| `READ_CURRENT_WORLD_OBJECT_ID` | `0x068` | 0 | Returns the current world object's ID, or `-1` when absent |
| `CLEAR_UNIT_LOW_FLAG` | `0x069` | 1 | Clears the selected unit's low flag bit |
| `SET_UNIT_LOW_FLAG` | `0x06A` | 1 | Sets the selected unit's low flag bit |
| `MOVE_OBJECT_ALONG_PATH` | `0x06B` | 3 | Binds the selected object to a path and applies its movement mode |
| `SET_MESSAGE_WINDOW_GEOMETRY` | `0x071` | 3 | Applies three geometry values to the current message window |
| `PREPARE_UNIT_MOTION_STATE` | `0x073` | 5 | Looks up an event unit and applies four motion-state values |
| `READ_SECONDARY_WORLD_ID_VALUE` | `0x094` | 1 | Looks up a named secondary-world ID and returns its value or zero |
| `RESET_FIELD_EFFECTS` | `0x099` | 0 | Resets field draw, sway, sky, and fade state |
| `CREATE_SCRIPT_TASK` | `0x0A5` | 2 | Creates a task at a local procedure with a relative priority and returns its handle |
| `DESTROY_REGISTERED_TASK` | `0x0A6` | 1 | Destroys a registered task and its hierarchy; an absent handle is ignored |
| `WAIT_FOR_TASK_REMOVAL` | `0x0A7` | 1 | Waits until a task ID leaves the task queues |
| `CREATE_POLYGON_MOVIE` | `0x0AA` | 2 | Creates an EventViewer task and returns its task ID |
| `SET_SOLAR_OVERLAY_MODE` | `0x0C3` | 1 | Selects the solar-overlay opacity mode |
| `REQUEST_ALTERNATE_FIELD_SEQUENCE` | `0x100` | 2 | Starts an alternate field sequence from a mode and resource name |
| `SET_FIELD_ENVIRONMENT` | `0x101` | 2 | Applies a field-environment selector and value |
| `ENABLE_FIELD_MODELS` | `0x103` | 4 | Enables the selected field model set and applies its transition mode |
| `DISABLE_FIELD_MODELS` | `0x104` | 4 | Disables the selected field model set and applies its transition mode |
| `ENABLE_FIELD_ANIMATION` | `0x105` | 4 | Enables animation for the selected field object set |
| `DISABLE_FIELD_ANIMATION` | `0x106` | 4 | Disables animation for the selected field object set |
| `ENABLE_FIELD_COLLISION` | `0x107` | 3 | Enables collision for a selected field room or room set |
| `DISABLE_FIELD_COLLISION` | `0x108` | 3 | Disables collision for a selected field room or room set |
| `ENABLE_FIELD_MODEL_GROUP` | `0x109` | 3 | Enables a numeric field-model group |
| `DISABLE_FIELD_MODEL_GROUP` | `0x10A` | 3 | Disables a numeric field-model group |
| `ENABLE_FIELD_NPCS` | `0x10E` | 3 | Enables NPC state for a selected field room or room set |
| `DISABLE_FIELD_NPCS` | `0x10F` | 3 | Disables NPC state for a selected field room or room set |
| `SET_FIELD_GIMMICK_DISPLAY` | `0x110` | 4 | Applies a display value to a selected field gimmick entry |
| `ENABLE_FIELD_MAP_ENTRY` | `0x111` | 3 | Enables a selected field-map entry |
| `DISABLE_FIELD_MAP_ENTRY` | `0x112` | 3 | Disables a selected field-map entry |
| `SET_FIELD_CAMERA_TABLE` | `0x113` | 1 | Selects the current field camera-table value |
| `READ_TREASURE_TABLE_VALUE` | `0x114` | 1 | Returns `CONTENT_KIND`, `ITEM_ID`, `ITEM_QUANTITY`, `TRAP_KIND`, or `AMOUNT` from the current room's treasure table |
| `MARK_CURRENT_TREASURE_OPENED` | `0x115` | 0 | Marks the current task's treasure object as opened |
| `TEST_CURRENT_TREASURE_OPENED` | `0x116` | 0 | Returns whether the current task's treasure object is already open |
| `QUEUE_WORLD_OBJECT_PENDING_VALUE` | `0x1E0` | 2 | Arms a selected world object with a pending value |
| `CLEAR_WORLD_OBJECT_PENDING_VALUE` | `0x1E1` | 1 | Clears a selected world object's pending value and starts its reset timer |
| `CLEAR_PROCESS_CONTROL_FLAG` | `0x1E7` | 0 | Clears the script-process control flag |

The assembler resolves these names to numeric operands. `COMM 0xNNNN` remains
valid for commands outside the reviewed profile. A name is rejected when the
source has no profile or the selected profile does not define it. Profiles are
kept separate because the same command ID can differ between engine versions;
for example, DDS1 `0x1E7` consumes no stack values and does not have Nocturne
HD's two-argument behavior.

The reviewed set names 43,600 of 53,389 native calls in the complete DDS1
corpus and 31,095 of 38,839 calls in the complete DDS2 corpus. It also makes
the adjacent message-command pattern safe to recognize, producing 188 symbolic
DDS1 message references in the original event slice, 2,368 across complete
DDS1, and 1,910 symbolic DDS2 references. Every other command and every dynamic
or ambiguous message operand remains numeric.

The selection pattern resolves a further 329 DDS1 and 287 DDS2 references to
typed local `select` declarations. References in raw message banks and values
computed at runtime remain numeric.

Task creation resolves all 807 DDS1 and 463 DDS2 local procedure operands to
`PUSHPROC`. The profile also names all 19 task-destruction calls. The source
keeps explicit `PUSHREG`, local stores, waits, and destruction calls, so each
task handle's lifetime remains visible without folding instructions away.

The same profile information resolves 31 DDS1 and 43 DDS2 event calls to
maintained `eNNN` sources. The remaining 11 DDS1 and four DDS2 literal event
operands have no maintained target and remain numeric.

## Tracked corpora

The repository tracks both complete retail BF corpora. DDS1 has 143 programs:
118 event scripts, 24 field scripts, and the battle negotiation script. DDS2
has 140 programs: 117 event scripts, 22 field scripts, and its battle
negotiation script. DDS1 uses 129 symbolic version-2 sources and 14 physical
version-1 sources; DDS2 uses 126 symbolic and 14 physical sources. The physical
sources are each game's 14 global event programs, which use a distinct
four-section layout.

DDS1 has 72 nonempty message banks. Sixty-seven decode to 2,702 dialog records,
3,109 pages, 1,028 selection options, and 184 speaker strings; five banks retain
local raw fallbacks. DDS2 has 66 nonempty banks, of which 59 decode to 2,437
dialogs, 2,673 pages, 1,149 options, and 177 speakers. Its other seven banks
also retain local raw fallbacks.

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
checks. Expected output hashes live in `config/dds1/scripts.sha1` and
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
  0006: result = CREATE_POLYGON_MOVIE(670, 1)
  0007: push result
  0008: WAIT_FOR_TASK_REMOVAL(result)
```

Use the semantic form to fold stack setup and expression instructions into
their proven consumers:

```sh
python3 tools/flw0.py view --semantic src/dds1/scripts/event/e670.bfasm
```

The same sequence then reads:

```text
  0006: result = CREATE_POLYGON_MOVIE(670, 1)
  0008: WAIT_FOR_TASK_REMOVAL(result)
```

An unused value, or a value that reaches a command with an unknown stack
contract, remains as an explicit `push`. The semantic view therefore removes
VM mechanics only when a known instruction accounts for the value.

The structured form additionally recognizes the compiler's canonical forward
branches and natural loops:

```sh
python3 tools/flw0.py view --structured src/dds1/scripts/battle/nego.bfasm
```

It emits nested `if`, `else`, and `while` blocks and removes only the branch and
back-edge instructions owned by those regions. A region is structured only
when its intervals are properly nested, no procedure or external edge enters
its interior, `if` arms do not cross into each other, and a loop condition has
no visible side effect before its test. Other control flow remains as labels
and gotos inside or beside the recovered blocks.

Across the maintained scripts this recovers 813 DDS1 and 1,263 DDS2 natural
loops, plus 3,522 and 3,497 conditional regions. Of those conditionals, 1,221
DDS1 and 1,068 DDS2 regions include a recovered `else` arm.

This is a derived reading aid, not another source format. An unprofiled native
command is printed numerically and invalidates the inferred stack; later
self-contained pushes can still form known arguments. Analysis starts at each
procedure and follows direct branch edges to a fixed point. At a join it keeps
only the top-of-stack suffix and VM result value on which every incoming path
agrees; different native-result sites remain distinct even though both display
as `result`. Calls, unknown commands, and unsupported instructions are explicit
analysis barriers. Unsupported instructions stay visible at their original PC
instead of being guessed away. Float literals retain their exact bit pattern.
`PUSHTYPE5` references show both the byte offset and observed NUL-delimited
ASCII text where available. Native call arguments follow the handler's
parameter order: argument 0 is popped from the top of the VM stack. Binary
expressions use the same VM order, with the top stack value on the left and the
next value on the right.

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
