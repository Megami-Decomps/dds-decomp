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

Maintained source may give a local slot a meaningful name. An `alias`
declaration is source metadata and emits no instruction or data; it only maps
an identifier to the encoded slot:

```text
locals int=6 float=0
alias treasure_kind = local_int[0]
alias item_id = local_int[1]
alias item_quantity = local_int[2]
alias choice = local_int[5]

# ...
  treasure_kind = READ_TREASURE_TABLE_VALUE(CONTENT_KIND)
  item_id = READ_TREASURE_TABLE_VALUE(ITEM_ID)
  item_quantity = READ_TREASURE_TABLE_VALUE(ITEM_QUANTITY)
  choice = MESSAGE_SELECTION_REQUEST_AND_POLL(selection(TAKARA_SEL))
```

Aliases work as assignment targets and in every expression position. They
must refer to a declared integer or float local, and one slot has at most one
name. Indexed forms such as `local_int[3]` remain valid for unknown or
deliberately unnamed state. Parsing and rendering existing source preserves
its aliases, while disassembling a binary alone leaves locals indexed because
the names are not stored in FLW0.

When a profiled call's result is pushed immediately, canonical source keeps it
as an expression through its exact consumer. This works even when older values
are already pending on the VM stack:

```text
  WAIT_FOR_TASK_REMOVAL(CREATE_POLYGON_MOVIE(670, 1))
  if (ACTION_WINDOW_REQUEST_AND_POLL_DIRECT(6) == 1) {
    # ...
  }
```

The nested form lowers calls in the original order and emits one `PUSHREG` for
each captured result. A label on the result push, a non-adjacent result read,
or an unknown command retains explicit source instead of moving the value
across an uncertain boundary.

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
      segment-start
      text-attribute 3 6
      font "から"
      newline
      stream-end
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

`newline` emits byte `0x0A`. The semantic message view also names the paired
renderer behavior established by the native handlers: `segment-start`,
`stream-end`, `conditional-newline`, `token SLOT`, `font-slot SLOT`, and
`text-attribute SLOT VALUE`. Text-attribute slots 1 through 3 are kept neutral
until their display roles are established; the operands are the decoded values
stored by the renderer. `control` preserves every other complete DDS control
sequence. The lead byte determines its length, so the assembler can reject a
truncated sequence. `bytes` remains available inside a page, option, or speaker
when a stream does not fit those forms. NUL terminators, record
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

An adjacent literal target for a profiled event command uses the maintained
event script's resource name:

```text
  PUSHEVENT e602
  COMM CALL_EVENT

  PUSHIS 633
  COMM SUBMIT_EVENT

  PUSHEVENT e610
  PUSHIS 258
  COMM SUBMIT_EVENT_WITH_SELECTION
```

`PUSHEVENT` also assembles to `PUSHIS`. The selected game profile binds names
to the event-resource IDs present in its maintained corpus. This mirrors the runtime,
which formats event ID 602 as
`/event/e600/e602/scr/e602.bf`. The command profile records which handler
argument carries the event resource. The disassembler emits the name only when
that argument is a direct literal and the target is maintained for the selected
game. This covers `CALL_EVENT` and the selected-event argument of
`SUBMIT_EVENT_WITH_SELECTION`. The first argument of that command,
`SUBMIT_EVENT`, and `SUBMIT_EVENT_IMMEDIATE` are encounter request IDs: their
native path uses the value as an index into the 1,024-row `ENCOUNT.TBL`. They
remain numeric in BF source because they belong to the encounter domain, not
the event-resource domain. Dynamic values and references to absent event
resources stay numeric.

The reading view carries the same evidence as `CALL_EVENT(event(e602))` or
`SUBMIT_EVENT_WITH_SELECTION(258, event(e610))`. Request IDs stay ordinary
integers, as in `SUBMIT_EVENT(633)` and the first selection argument.

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

The DDS1 and DDS2 profiles contain a reviewed command set. Each ID, handler,
stack read, and result write was checked against the applicable PS2 command
table and native implementation; shared commands were checked in both games:

| Source name | ID | Stack values consumed | Verified handler behavior |
|---|---:|---:|---|
| `MESSAGE_REQUEST_AND_POLL` | `0x000` | 1 | Starts a message entry or waits for its current work |
| `ACTIVATE_MESSAGE_PANEL` | `0x001` | 0 | Activates the current message panel |
| `FINISH_SCRIPT_MESSAGE_WINDOW` | `0x002` | 0 | Waits for and finishes the active script message window |
| `MESSAGE_SELECTION_REQUEST_AND_POLL` | `0x003` | 1 | Builds or waits for a selection and returns the selected value |
| `TEST_MODEL_FLAG` | `0x007` | 1 | Tests a model flag and returns the result |
| `SET_MODEL_FLAG` | `0x008` | 1 | Sets a model flag |
| `CLEAR_MODEL_FLAG` | `0x009` | 1 | Clears a model flag |
| `RANDOM_ONE_TO` | `0x00A` | 1 | Returns a random integer from one through the supplied limit |
| `WAIT_FOR_TIMER_START` | `0x00D` | 0 | Waits until the current command timer becomes nonzero |
| `WAIT_FOR_TIMER_LIMIT` | `0x00E` | 1 | Waits until the command timer reaches a limit |
| `SCREEN_FADE_A` | `0x00F` | 2 | Starts the selected screen fade when its timer reaches zero |
| `SCREEN_FADE_B` | `0x010` | 2 | Starts the selected screen fade-in when its timer reaches zero |
| `ADD_EFFECT_UNIT_TO_WORLD` | `0x012` | 1 | Adds the selected player object or effect-unit ID to the active world |
| `START_CAMERA_PATH_MOVE` | `0x013` | 2 | Starts a camera path move by binding a motion resource to a selected camera object |
| `CREATE_LINKED_CAMERA_VIEWER` | `0x015` | 2 | Creates the linked-camera viewer object and returns its fixed object ID |
| `DESTROY_WORLD_UNIT` | `0x016` | 1 | Removes a selected staged unit from the world |
| `START_UNIT_MOVE_TO_POSITION_OBJECT` | `0x017` | 5 | Starts a staged unit move toward a selected position object |
| `ADD_FLAGGED_EFFECT_UNIT_TO_WORLD` | `0x019` | 1 | Adds the selected player object, or marks and adds an effect-unit ID |
| `SET_CONTROLLER_VIBRATION` | `0x01A` | 3 | Sets the `SMALL_MOTOR` or `LARGE_MOTOR` vibration strength and duration |
| `FADE_BACKGROUND_IN` | `0x01F` | 1 | Starts the background fade-in with the supplied duration |
| `READ_SOLAR_PHASE` | `0x027` | 0 | Returns the current solar phase |
| `SUBMIT_EVENT_WITH_SELECTION` | `0x028` | 2 | Starts ENCOUNT.TBL encounter arg0 and selects event resource arg1 after it |
| `RESET_DRAW_EFFECTS` | `0x043` | 0 | Clears draw transitions and effect enables |
| `RETURN_TO_TITLE` | `0x046` | 0 | Requests the title scene |
| `WAIT_FOR_UNIT_MOTION` | `0x049` | 1 | Waits until the selected unit's motion is idle or in its timed mode |
| `ATTACH_WORLD_OBJECT_TO_SOURCE_VECTOR` | `0x04A` | 2 | Attaches a world object to a source vector |
| `SET_UNIT_VALUE` | `0x04B` | 2 | Writes the selected unit's 16-bit value at offset `0xBC` |
| `ACTION_WINDOW_REQUEST_AND_POLL` | `0x05E` | 1 | Requests or polls the current action-window message and returns `-1`, `0`, or `1` |
| `RESTORE_CAMERA_NODE_MODE` | `0x060` | 0 | Resets the player scene-object state and restores camera node mode |
| `RELEASE_CURRENT_OBJECT` | `0x061` | 0 | Releases the current field object and refreshes field state |
| `CALL_EVENT` | `0x066` | 1 | Calls the event resource in arg0 and clears named processes |
| `SUBMIT_EVENT` | `0x067` | 1 | Starts the ENCOUNT.TBL encounter indexed by arg0 with no selected event |
| `READ_CURRENT_WORLD_OBJECT_ID` | `0x068` | 0 | Returns the current world object's ID, or `-1` when absent |
| `CLEAR_UNIT_LOW_FLAG` | `0x069` | 1 | Clears the selected unit's low flag bit |
| `SET_UNIT_LOW_FLAG` | `0x06A` | 1 | Sets the selected unit's low flag bit |
| `MOVE_OBJECT_ALONG_PATH` | `0x06B` | 3 | Binds the selected object to a path and applies its movement mode |
| `WAIT_FOR_OBJECT_PATH` | `0x06C` | 1 | Waits for the selected object's path state to finish |
| `CHANGE_ITEM_COUNT` | `0x070` | 2 | Adjusts the selected item or flag count and applies its range limit |
| `SET_MESSAGE_WINDOW_GEOMETRY` | `0x071` | 3 | Applies three geometry values to the current message window |
| `PREPARE_UNIT_MOTION_STATE` | `0x073` | 5 | Looks up an event unit and applies four motion-state values |
| `START_UNIT_PATH_FOLLOW` | `0x08B` | 7 | Starts a staged unit following a selected path resource |
| `READ_SECONDARY_WORLD_ID_VALUE` | `0x094` | 1 | Looks up a named secondary-world ID and returns its value or zero |
| `DEFER_BATTLE_EXIT` | `0x097` | 3 | Records a field and WAP battle-exit event for deferred dispatch; the first value is diagnostic |
| `REQUEST_CURRENT_GROUP_SEQUENCE` | `0x098` | 2 | Starts a named sequence in the current field group and clears named processes |
| `RESET_FIELD_EFFECTS` | `0x099` | 0 | Resets field draw, sway, sky, and fade state |
| `DESTROY_WORLD_EFFECT_OBJECT` | `0x09B` | 1 | Removes the selected world effect object |
| `COPY_EFFECT_OBJECT_TRANSFORM_FROM_SOURCE` | `0x09D` | 2 | Copies a source vector's position and rotation to an effect object |
| `FOCUS_CAMERA_ON_OBJECT` | `0x0A3` | 1 | Focuses the field camera on the selected object's current position |
| `UPDATE_FIELD_LOOK_AT_SEGMENT` | `0x0A4` | 0 | Rebuilds the field camera's look-at segment and clears its highlight state |
| `CREATE_SCRIPT_TASK` | `0x0A5` | 2 | Creates a task at a local procedure with a relative priority and returns its handle |
| `DESTROY_REGISTERED_TASK` | `0x0A6` | 1 | Destroys a registered task and its hierarchy; an absent handle is ignored |
| `WAIT_FOR_TASK_REMOVAL` | `0x0A7` | 1 | Waits until a task ID leaves the task queues |
| `CREATE_POLYGON_MOVIE` | `0x0AA` | 2 | Creates an EventViewer task and returns its task ID |
| `SETUP_FADE_FRAMES` | `0x0AB` | 2 | Configures the fade frame range used by the field effect sequence |
| `SET_SOLAR_OVERLAY_MODE` | `0x0C3` | 1 | Selects the solar-overlay opacity mode |
| `WAIT_FOR_CAMP_TASK` | `0x0C8` | 1 | Starts or waits for the selected camp task |
| `BIND_MODEL_MOTION_SOUND` | `0x0C9` | 2 | Binds motion sound to a model and returns the resolved model ID |
| `CREATE_EVENT_TEXTURE_TASK` | `0x0CC` | 2 | Creates a texture task from an event resource and returns its task handle |
| `CREATE_FLAGGED_EFFECT_OBJECT` | `0x0CD` | 1 | Creates and flags an effect object from a resource name, returning its object ID or zero |
| `SET_EFFECT_MODEL_CUT` | `0x0CE` | 2 | Selects one of an effect model's three event cuts |
| `SET_EFFECT_MODEL_ROTATION` | `0x0CF` | 4 | Applies three angle values to an effect model |
| `CREATE_EVENT_BED_EFFECT` | `0x0D0` | 2 | Creates and flags a BED effect from an event resource, returning its object ID or zero |
| `ATTACH_EFFECT_TO_PATH` | `0x0D1` | 2 | Attaches an effect object to a path object |
| `WAIT_FOR_EFFECT_PATH` | `0x0D2` | 1 | Waits for an effect object's path movement to finish |
| `CREATE_MG1_EFFECT` | `0x0D3` | 1 | Creates and flags an MG1 effect from a resource name, returning its object ID or zero |
| `CREATE_EVENT_MG1_EFFECT` | `0x0D4` | 2 | Creates and flags an MG1 effect from an event resource, returning its object ID or zero |
| `CREATE_MG2_EFFECT` | `0x0D5` | 1 | Creates and flags an MG2 effect from a resource name, returning its object ID or zero |
| `CREATE_EVENT_MG2_EFFECT` | `0x0D6` | 2 | Creates and flags an MG2 effect from an event resource, returning its object ID or zero |
| `SET_MG1_EFFECT_POINTS` | `0x0D7` | 3 | Sets the two control objects used by an MG1 effect |
| `SET_MG2_EFFECT_POINTS` | `0x0D8` | 5 | Sets the four control objects used by an MG2 effect |
| `START_EVENT_BGM` | `0x0D9` | 2 | Starts an event BGM from its sound ID and fade value |
| `SET_WORLD_NODE_BASE_MODE` | `0x0DB` | 1 | Applies the selected base-mode value to world nodes |
| `DESTROY_EFFECT_OBJECT` | `0x0E3` | 1 | Destroys the selected effect object |
| `START_CAMP_TASK_IF_ABSENT` | `0x0F7` | 1 | Starts the selected camp task when it is not already running |
| `CAMP_TASK_READY` | `0x0F8` | 1 | Returns whether the selected camp task is ready |
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
| `START_AND_WAIT_FOR_STREAM_SOUND` | `0x11B` | 1 | Starts the selected stream sound and waits through its startup state |
| `ADVANCE_STREAM_SOUND_STATE` | `0x11C` | 0 | Advances the stream-sound transition state |
| `RESET_STREAM_PLAYBACK` | `0x11F` | 0 | Resets the stream playback commands |
| `WAIT_FOR_STREAM_IDLE` | `0x121` | 0 | Waits until stream playback is idle |
| `CLEAR_WORLD_OBJECT_STATE_FLAGS` | `0x124` | 1 | Clears both transient state flags on a selected world object |
| `ADD_PARTY_CURRENCY` | `0x139` | 1 | Adds a clamped amount to party currency |
| `APPLY_PARTY_TRAP_EFFECT` | `0x13A` | 1 | Applies a named HP-loss or status effect to the party |
| `SET_MESSAGE_RANGE` | `0x13C` | 2 | Sets the active message range for the current window |
| `SUBMIT_EVENT_IMMEDIATE` | `0x166` | 1 | Starts the ENCOUNT.TBL encounter indexed by arg0 immediately |
| `QUEUE_WORLD_OBJECT_PENDING_VALUE` | `0x1E0` | 2 | Arms a selected world object with a pending value |
| `CLEAR_WORLD_OBJECT_PENDING_VALUE` | `0x1E1` | 1 | Clears a selected world object's pending value and starts its reset timer |
| `CLEAR_PROCESS_CONTROL_FLAG` | `0x1E7` | 0 | Clears the script-process control flag |
| `CONSUME_FIELD_SKILL_END_NOTICE` | `0x1F1` | 1 | Clears and reports a pending Lightoma, Liftoma, Riberama, or Estoma expiration notice |
| `START_ARCHIVE_SOUND` | `0x1F3` | 2 | Starts an archive sound with the default volume and pan |
| `STOP_ARCHIVE_SOUND` | `0x1F4` | 2 | Stops an archive sound selected by bank and sound ID |
| `REVEAL_AUTOMAP_RECTANGLE` | `0x1F6` | 5 | Marks a rectangular range of cells in the current field's automap bitmap |
| `READ_SUCTION_WARP_VALUE` | `0x1FA` | 1 | Returns a state, object, map-entry, or motion value for the selected suction warp |
| `READ_BARRIER_VALUE` | `0x1FB` | 1 | Returns a model flag, object, completion flag, or map entry for the selected barrier |
| `APPLY_ROOM_MODE_GROUP_ZERO` | `0x1FC` | 4 | Applies DDS2 room-object transition modes from group zero |
| `APPLY_ROOM_MODE_GROUP_ONE` | `0x1FD` | 4 | Applies DDS2 room-object transition modes from group one |
| `READ_CURRENT_SCENE_SELECTION_RESOURCE` | `0x1FE` | 0 | Returns the resource attached to the selected scene entry, or zero |
| `FIND_FIELD_EFFECT_BY_NAME` | `0x1FF` | 1 | Finds a field effect by its type-5 name and returns its handle |
| `READ_ELEVATOR_TABLE_VALUE` | `0x200` | 1 | Returns a value from the selected elevator-destination row |
| `RUN_FIELD_DESTINATION_TRANSITION` | `0x201` | 1 | Runs the field transition selected by a destination index |
| `SET_CURRENT_TASK_SCENE` | `0x202` | 0 | Selects the scene attached to the current field task |
| `NO_OP_FIELD_TRANSITION` | `0x203` | 0 | Preserves a retail field-transition command whose paired handlers do nothing |
| `APPLY_CURRENT_TASK_ENTRY_TRIGGER` | `0x204` | 0 | Applies the actor-entry trigger attached to the current field task |
| `ADVANCE_FIELD_INTERACTION` | `0x205` | 2 | Selects and applies the next field-interaction row for a line and choice |
| `READ_FIELD_INTERACTION_VALUE` | `0x206` | 2 | Returns the row type, message ID, or flag from a field-interaction row |
| `READ_FIELD_INTERACTION_KIND` | `0x207` | 0 | Classifies the current field interaction for the shared action-window loop |
| `READ_LADDER_TABLE_VALUE` | `0x208` | 1 | Returns the direction, object IDs, or action-window mode for the selected ladder warp |
| `CONFIGURE_ELEVATOR_CAMERA_MOVE` | `0x209` | 2 | Configures elevator camera movement between two destination indices |
| `RESET_ELEVATOR_CAMERA_MOVE_TRACKING` | `0x20A` | 0 | Resets elevator camera-move tracking |
| `POLL_ELEVATOR_MOVE_STATE` | `0x20B` | 0 | Returns the tracked elevator camera-move state and starts its final move when ready |
| `READ_DOOR_WARP_VALUE` | `0x20C` | 1 | Returns the motion duration or fade mode for the selected door warp |
| `APPLY_CURRENT_TASK_RECORD_ENTRY` | `0x20D` | 0 | Applies the record entry attached to the current field task |
| `START_CURRENT_FIELD_INTERACTION_EVENT` | `0x20E` | 0 | Starts the script event selected by the current field interaction |
| `RUN_FIELD_TRANSITION_SELECTOR` | `0x20F` | 1 | Runs the field transition selected by its script ID; the shared `HEAL_FACILITY` and `SAVE_POINT` IDs are named |
| `START_SCENE_BGM` | `0x210` | 1 | Stores the scene BGM selector and starts the resolved scene track |
| `PLAY_FIELD_SE_VOLUME_PAN` | `0x214` | 1 | Plays a field sound effect through its volume-and-pan path |
| `PLAY_FIELD_SE` | `0x215` | 1 | Plays a field sound effect |
| `RELEASE_CURRENT_BGM` | `0x216` | 0 | Releases the current field BGM handle |
| `LOAD_ARCHIVE_SOUND_BANK_AND_WAIT` | `0x217` | 1 | Requests an archive sound bank and waits until it is resident |
| `READ_WARP_EFFECT_MODE` | `0x219` | 0 | Returns the warp-effect selector; both DDS implementations return zero |
| `RESET_FIELD_AFTER_EVENT` | `0x21A` | 0 | Resets DDS2 field state after an event |
| `ACTION_WINDOW_REQUEST_AND_POLL_DIRECT` | `0x21D` | 1 | Requests or polls an action-window message without the actor-entry precheck and returns `-1`, `0`, or `1` |
| `APPLY_FIELD_MODEL_LIGHTING` | `0x21E` | 1 | Applies the active field-lighting state to the selected world unit |

The archive-sound commands build the same `0x30000000 | bank << 16 | sound`
identity in both games. Start sends the volume/pan command with the retail
defaults, stop sends the single-track stop command, and the bank command waits
until the requested archive is resident. The automap command forwards a map
row, origin, width, and height to the bitmap marker, which only sets discovery
bits. The model-lighting command resolves the selected world unit and applies
the active field-light state. Together these seven contracts replace 509 DDS1
and 569 DDS2 numeric calls.

The field-staging commands cover the common camera and unit choreography path.
The camera command binds a type-`0x10` motion resource to a selected type-`4`
camera object, while the focus command copies a selected object's position into
the active field-camera focus. The unit commands remove a selected type-`5`
unit or start motion toward a type-`0x11` position object or along a type-`0x10`
path. Together these five contracts replace 459 DDS1 and 288 DDS2 numeric
calls across the complete tracked corpora.

The standard profiles also contain the paired battle-runtime commands used by
the negotiation script and AICALC programs:

| Source name | ID | Stack values consumed | Verified handler behavior |
|---|---:|---:|---|
| `AI_COUNTER_REACHED_LIMIT` | `0x0DF` | 1 | Tests the native battle counter against a limit and returns the result |
| `AI_SELECT_ACTION_BY_KIND` | `0x0E2` | 2 | Selects an action kind and value in the battle command context |
| `TRACE_BATTLE_RETREAT` | `0x0E4` | 0 | Emits the retail `BTL_TAIKYO` battle-debug trace |
| `TRACE_BATTLE_ALL_RETREAT` | `0x0E5` | 0 | Emits the retail `BTL_ALLTAIKYO` battle-debug trace |
| `AI_RESET_COMMAND_CONTEXT` | `0x0E6` | 0 | Resets the battle command context and sets its state flag |
| `AI_SELECT_LOWEST_HP_TARGET_BLOCKING_ELEMENT` | `0x0E7` | 1 | Selects the lowest-HP target that blocks the supplied element |
| `AI_MOVE_CAMERA` | `0x0F4` | 7 | Starts a seven-value battle camera move and schedules context reset |
| `TRACE_BATTLE_CAMERA_ORIGINAL` | `0x0F5` | 0 | Emits the retail `BTL_CAM_ORG` battle-debug trace |
| `AI_ENABLE_COMMAND_STATE_FLAG` | `0x0F6` | 0 | Sets bit one in the battle command-context flags |
| `AI_QUEUE_ACTOR_COMMAND_SOUND` | `0x0FA` | 0 | Queues the actor command-sound tasks |
| `TRACE_BATTLE_CAMERA_TWO_SHOT` | `0x0FB` | 0 | Emits the retail `BTL_CAM_2SHOT` battle-debug trace |
| `TRACE_BATTLE_CAMERA_OBSTRUCTION` | `0x0FC` | 0 | Emits the retail `BTL_CAM_BOUGAI` battle-debug trace |

The four `TRACE_` handlers call only the battle-debug printer. In particular,
`0x0F5` does not restore camera state. Command `0x0DE` reaches a related
counter predicate through a different packed-action path, but the distinction
between the two paths is not established, so it remains numeric. These
profiles therefore resolve every other native call in both negotiation
scripts.

The assembler resolves these names to numeric operands. `COMM 0xNNNN` remains
valid for commands outside the reviewed profile. A name is rejected when the
source has no profile or the selected profile does not define it. Profiles are
kept separate because the same command ID can differ between engine versions;
for example, DDS1 `0x1E7` consumes no stack values and does not have Nocturne
HD's two-argument behavior.

The warp-table commands use selector names only where the paired handlers and
their script consumers establish the field's role. Barrier selectors `3` and
`4`, elevator payload columns `2` through `13`, and the DDS2-only suction
selector `5` remain numeric because their meanings are incomplete or differ
between the games.

The field-interaction commands read the current `.INF` state machine. Column
`0` is the row type and column `1` is its message ID. Column `2` is a confirmed
row flag, but its full role is not yet established, so it remains numeric.
`READ_WARP_EFFECT_MODE` is retained by the shared warp procedure even though
both DDS handlers are stubs that return zero.

The reviewed set names 51,421 of 53,389 native calls in the complete DDS1
corpus and 37,752 of 38,839 calls in the complete DDS2 corpus. It also makes
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
