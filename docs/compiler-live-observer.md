# Read-only EE GCC live observer

`tools/ee_gcc_observe.py` records decisions made by the real, unmodified EE GCC
2.96 compiler. It fills gaps in the [RTL diagnostics](compiler-diagnostics.md):
local coalescing quantities, actual register-search exclusions, the origin and
pruning of global preferences, and early destructive-target operand choices.
Use it after the ordinary dumps have narrowed a specific question.

The tool uses Python 3's standard library and a locally installed i386 QEMU.
No compiler, game, runtime-library or emulator binary is distributed here.
The compiler must have SHA-256:

```text
d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1
```

The decoder is specific to this little-endian i386 executable. The original
hook/layout inventory is in [ee-gcc-observer-hooks.json](ee-gcc-observer-hooks.json);
the current guards, including ten comparator-tier hooks, are in
[`tools/_ee_gcc_observer.py`](../tools/_ee_gcc_observer.py).
All 45 static hooks verify eight original instruction bytes on their
first actual breakpoint hit, before decoding state. The initial QEMU stop can
be inside the loader, before the compiler text is mapped. Executable symbols, STABS records and disassembly
identify the hook locations; corresponding vendor C source is unavailable.
An arbitrary compiler version cannot safely use these addresses.

## What it observes

The `allocation` mode gates observation to one named function and records:

- scheduler comparisons, instruction priorities and actual issue choices;
- local `combine_regs` inputs, death-note/class checks and the resulting
  coalesced quantity;
- local quantity comparisons, live exclusions and physical-register choices;
- global `set_preference`, expansion and pruning changes;
- global candidate exclusions, preference choices and the committed register.

### Comparator decision tiers

Allocation traces also identify the branch that returned from the global
allocno comparator and the scheduler's rank comparator. Each call retains its
own stack context. Join entry, checkpoint and exit events by `call_id` within
the trace; `seq` is the ordered event number, and `phase` identifies the pass.
Comparator results are signed 32-bit return values, not physical-register
assignments or final instruction selections.

The three global hooks emit:

- `allocno_compare_enter`: `pair` contains the two allocno snapshots in argument
  order, including their existing allocation fields and a `range_copy` bit
  decoded from the corresponding pseudo's register info. `priorities` starts
  as JSON `null`.
- `allocno_compare_priorities`: `priorities` records the signed left and right
  scores held in EBX and EAX at the comparison checkpoint.
- `allocno_compare_exit`: repeats the entry `pair`, records the observed
  `priorities` and signed `result`, and adds `deciding_tier`. The tier is
  `range_copy` when the priority checkpoint was bypassed, `priority` when the
  scores differ, and `allocno_number` when the scores tie. `null` means the
  score checkpoint was not reached, not that both scores were zero.

The seven scheduler checkpoints emit `rank_stage` with `call_id`, `stage`,
`left_uid` and `right_uid`. `rank_exit` adds the greatest stage reached and its
`deciding_tier` to the existing signed `result`:

- 0: `priority` (no checkpoint reached)
- 1: `reg_weight`
- 2: `interblock_bb`
- 3: `spec_val`
- 4: `prob_val`
- 5: `last_insn_class`
- 6: `dependents`
- 7: `luid`

Stages describe control-flow checkpoints, not the number of comparisons made.
Branches can skip checkpoints, so a later stage means preceding tiers did not
decide that call; it does not prove every preceding tier was evaluated and tied.
Use the matching `rank_enter` instruction snapshots and `schedule_insn` issue
events to interpret the comparison. Reaching `luid` establishes the final
ordering tie-break for that comparator call; it does not alone identify the
source expression responsible for the instruction order.

The `operands` mode records early single-precision (`SF`) `expand_binop` inputs,
actual canonicalization swaps, operand/target identity and results. It tracks
nested operations but retains only SF records. This mode stops collecting at
the target's expansion end; allocation mode stops collecting after its
postreload scheduler pass. The rest of the original translation unit still
compiles. A missing target or missing completion gate is a failure.

The [camera case study](compiler-trace-camera-case.md) illustrates how a real
source lifetime can produce an allocator preference, and why another residual
must instead be traced to early destructive-target selection. Neither a final
register home nor a printed preference alone identifies the source mechanism.

## Freeze a case

Create a locally reviewed JSON configuration, following
[ee-gcc-observer-case.example.json](ee-gcc-observer-case.example.json). Replace
all placeholder paths and hashes with your installed inputs. The example is a
schema guide, not a runnable project configuration.

The JSON must contain:

- `cwd`: the unchanged compiler working directory;
- `environment`: the complete environment, with no implicit host inheritance;
  only `PATH`, `HOME`, `LANG`, `LC_ALL`, `TZ`, `TMPDIR`, `DDS_I386_LIBDIR`,
  `DDS_VERSION` and `DDS_AS_UNIT` are accepted;
- `compiler`, `assembler`, `qemu`: absolute executable paths matching the
  command arrays;
- `command`, `assembler_command`: original QEMU invocations as argv arrays;
- `function`: the exact compiler function name;
- `inputs`: path-to-SHA-256 mappings covering the compiler, original QEMU,
  assembler, source, every included header, loader, runtime libraries and other
  relevant dependencies;
- `logs`: exact paths for `cc1_stdout`, `cc1_stderr`, `as_stdout`, `as_stderr`;
- `artifacts`: explicit file paths/archive names and narrow single-directory
  globs/archive directories for every assembly, object, RTL dump and log;
- optional `timeout_seconds` (default 1800) and `transport` (default Unix).

File paths in JSON are resolved relative to the JSON file, unless absolute.
Command-array arguments and environment strings are never rewritten: paths in
those arguments must already be correct for the frozen working directory.
There is no Python-harness import, shell expansion or command-template engine.
The JSON is an instruction to run local programs, not a sandbox: review it
before use. Keys outside that nine-key compiler/runtime allowlist are rejected
before an archive is created; nothing is silently filtered. In particular,
credential-like or unrelated keys, `LD_PRELOAD`, `LD_LIBRARY_PATH`, `QEMU_GDB`
and `QEMU_SET_ENV` are rejected. Use the explicit original QEMU/loader argv for
library selection rather than adding ambient environment overrides. Public
fixtures contain only fixed synthetic paths and conventional locale values.
Do not place credentials in allowed values or commit a private case
configuration or its generated receipts.

Hash the complete input closure. The tool can verify declared inputs but cannot
prove that a manually written list covers every include or loader dependency.
Include paths, source paths, output paths and dumpbase can affect this compiler;
do not move them between baseline and observed runs. Use a dedicated case and
run it serially. Keep normal builds and other diagnostic harnesses away from
that working directory while the case runs.

```sh
python3 tools/ee_gcc_observe.py --config /path/to/case.json \
  --output /path/to/new-run --dry-run
python3 tools/ee_gcc_observe.py --config /path/to/case.json \
  --output /path/to/new-run --mode allocation
# A separate new archive is required for an operand observation.
python3 tools/ee_gcc_observe.py --config /path/to/case.json \
  --output /path/to/new-operand-run --mode operands
```

`--dry-run` validates the schema, input hashes, i386 image, and current artifact
specifications without launching tools, opening sockets, or writing files. It
does not establish debugger availability or output equality. There is no flag
that disables the compiler hash check or permits debugger writes.

## Local debugger transport

Unix-domain sockets are preferred. Current
[QEMU user-mode documentation](https://www.qemu.org/docs/master/user/main.html)
describes `-g endpoint` with a Unix pathname. The runner conservatively requires
that advertised help interface before it supplies a path, creates the socket
in a private temporary directory, and fails if the host denies Unix sockets.
It never automatically retries with TCP. This help check is a compatibility
gate, not proof that a rejected build lacks Unix support: the pinned older
QEMU parses Unix paths despite advertising `-g port`. In the original execution
environment, an independent Unix-socket attempt reached the host socket call
and was denied with `EPERM`. The public Unix path has offline coverage; its
conservative help gate rejects that pinned build, and the live qualification
uses the explicit fallback below. Do not use a numeric debugger port with the
unmodified pinned QEMU: it binds to all interfaces.

For the precisely pinned older emulator, `ee_gcc_qemu_loopback.py` can create a
separate local-only copy:

```sh
python3 tools/ee_gcc_qemu_loopback.py --input /path/to/original/qemu-i386 \
  --output /path/to/new/qemu-i386-loopback > /path/to/qemu-copy-receipt.json
```

It accepts only original SHA-256
`3378bb95493e33dc6cb215a8d89c4aeb58d12201b817413a4c0449ffa5abb443`.
At file offset `0x318798`, instruction bytes
`c7 44 24 34 00 00 00 00` become `c7 44 24 34 7f 00 00 01`.
Exactly two bytes change: the host debugger's bind address becomes 127.0.0.1.
The resulting copy must hash to
`ba9a200e82870b336c942bc38cf898265bb5ca0bf268ea55da1982aad738c7c1`.
The helper never overwrites an existing file or changes the installed emulator.
For background on the host bind behavior, see
[QEMU's user-mode gdbstub](https://gitlab.com/qemu-project/qemu/-/blob/v10.0.0/gdbstub/user.c).
That source tag is an analogue, not exact build provenance for the pinned binary.

Select this fallback explicitly in the case:

```json
"transport": {"kind": "loopback-copy", "qemu": "/path/to/qemu-i386-loopback"}
```

Both QEMU paths and their exact hashes must be in `inputs`. On Linux the runner
checks `/proc/net/tcp` and `/proc/net/tcp6` before connecting. The listener must
be exactly IPv4 127.0.0.1, with an inode owned by the specific QEMU process the
runner just spawned. It saves those rows in `listener.txt` and ownership evidence in
`listener-owner.json`. An occupied/reused
port, missing ownership evidence, another bind address, or a denied socket
fails the run. There is no all-interface fallback and no retry around access
restrictions. A successful connection alone is not the listener check.

## Equality and archive contract

Each invocation creates a new archive and runs a fresh untraced baseline with
the original QEMU, then the observed compile. The assembler uses the original
QEMU in both runs. Both use exactly the same cwd, environment, compiler argv,
input/output paths and dumpbase. Only the observed host QEMU debugger launch
changes. Input hashes are checked before and after each compiler/assembler
stage.

The runner copies existing declared artifacts into `preexisting/`, then clears
only those managed outputs before each run. This prevents stale dumps from
masquerading as newly emitted output. An exact file must exist, and every glob
must match at least one file. Input/output overlap, duplicate destinations,
missing files and unequal inventories are errors. On a recoverable run failure,
partial managed outputs are archived under `failed/` and pre-run outputs are
restored. A forced host shutdown or filesystem failure may require recovery
from `preexisting/`; an incomplete run has no qualified receipt.

The archive contains:

- `case.json` and `tool-hashes.json`: invocation/input and observer provenance;
- `baseline/` and `observed/`: complete declared output sets;
- `events.jsonl`: ordered runtime observations with phase and call identifiers;
- `observed-command.json`, `hook-checks.json` and, for TCP, `listener.txt`
  and `listener-owner.json`;
- `equivalence.json`: artifact inventories and both hashes for every file,
  equality flags, event count and mode.

Equality requires identical inventories and identical raw bytes for every
artifact. Nothing normalizes paths, labels, instruction order or registers.
A nonzero exit, missing target or any changed output disqualifies the trace.
The final observed files remain in the case directory after success. Full-unit
assembly/RTL equality also covers any target excerpts drawn from those files;
the generic runner does not create additional extracted-function copies.

The debugger protocol is allowlisted: status/capability queries, register and
memory reads, QEMU virtual breakpoint insertion/removal, single-step and
continue. It rejects guest register/memory writes and unknown packets before
sending them, including when Python runs with `-O`. Virtual breakpoints affect
execution control, not guest compiler text. The compiler file remains untouched.
Only the QEMU process spawned by this invocation is killed on error.

## Offline checks and interpretation

```sh
python3 -m unittest discover -s tests -p 'test_ee_gcc_observe.py'
python3 -m unittest discover -s tests -p 'test_ee_gcc_observer_tiers.py'
```

These tests need no vendor inputs or live debugger. They cover protocol
framing/decoding, write rejection, optimized-Python safety, fixed-i386 decoding,
hash/configuration validation, hook guards, missing targets, artifact inventory
mismatches, stale-file recovery, socket denial/ownership and fail-closed QEMU
patching. Mocked patch tests check the byte recipe; a real supported QEMU is
still required for a live qualification.

The focused tier tests use synthetic i386 memory and register state to cover
all eight scheduler tiers, skipped checkpoints, signed values, range-copy and
allocno-number decisions, nested call association, opcode guards and orphan
events. Synthetic coverage does not establish live coverage of any branch or
replace the frozen-case equality checks.

This is diagnostic evidence about a candidate compilation. Byte equality proves
that the observer did not change the declared artifacts of that frozen case;
it does not prove the candidate matches retail or uniquely recovers the
original C source. Keep the project's normal whole-unit and retail build checks.
Use the [decision atlas](compiler-decision-atlas.md) to choose a supported source
hypothesis, and stop when the needed lifetime, preference or expression would
invent behavior. This tool does not search compiler flags or source permutations.

## Decision-tier qualification (2026-10-15)

The reconstructed public implementation passes 56 tests (17 new and 39 existing). A fresh staff-renderer fixture produced 16,712 events with all 28 observed/baseline artifacts byte-identical. Independent ordinary output agrees in all six allocated sections and 239 relocations; assembly differs only in the exact first source `.file` directive. Live coverage includes allocator-number and scheduler LUID decisions; range-copy decisions and scheduler stages 3/4 remain synthetic-test coverage only. The invoking shell had a post-observation script-edit error, separately recorded rather than treated as a successful shell run. See [qualification receipt](compiler-evidence/observer-decision-tiers/qualification.json).
