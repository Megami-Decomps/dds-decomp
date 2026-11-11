# DDS2 terminal panel row drawing

`mnuDrawTerminalPanelListRows` at `00294D50` is 356 native bytes. Its
complete owner, `src/dds2/game/code_00294730.c`, verifies 22 matches and
zero differences, including owned data.

## Source and native contract

The renderer captures the first texture set from the current terminal, then
draws the frame. Only after that callback does it test the current window
list count. An empty or negative count skips the row phase; otherwise a
`do` loop draws one band per current row. The list and count are read again
after each row callback. The footer still uses its fixed table coordinates,
without adding the input X/Y offsets.

The row uses the existing four-halfword `BrsIconRecord` owner. Its actual
X, Y and icon-ID field addresses are formed in the guarded row phase;
values are read again on every iteration. Native `00294DEC` retains the
Y-field address, table + `0x4E`, in `s3`. Native `00294DF0..E08`
materializes X at table + `0x4C` and ID at table + `0x4A` each iteration.
No table value is cached across a callback. The counter is initialized to
zero before frame drawing, as the native `00294D6C` does.

The source introduces no side effect, additional call, pointer cast,
qualifier on the underlying storage, compiler constraint, or dead value.
`const s16 *` restricts only writes through these read-only local pointers;
the table itself remains mutable. Original source spelling is unknown.

## Qualified ordinary compiler evidence

The compiler is the pinned ee-gcc 2.96 binary with SHA-256
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.
Compile the complete unit through `tools/cc.sh`, with its normal per-unit
flags and assembly fallbacks intact, once normally and once with
`-da -dumpbase <output>/rtl`. Keep assembly with `DDS_KEEP_S`. The two
assembly files agree after normalizing only the random scratch-directory
component of `.file`; no debugger or instrumentation is needed.

In the accepted whole-unit stock dumps:

- `00.rtl` gives X/Y/ID real address pseudos 118/120/122 (definitions
  95/98/101). They are all defined only after the initial count guard.
- `08.gcse` represents these as the table base plus `0x4C/0x4E/0x4A`.
- `09.loop` recognizes a normal 29-instruction loop. This is not a phony
  loop or a source form that disables loop optimization.
- `20.greg` allocates Y pseudo120 to `s3`, sharing the table-base home
  after that base's last use. Instruction 98 adds `0x4E` once. The real
  X and ID address values are rematerialized by instructions 246..249 in
  the loop as the two native high/low pairs.

The older direct-array `for` draft emits 340 bytes, with 55 differing
words and 16 native bytes uncovered. Merely changing its loop syntax
leaves that residual unchanged. Giving all three field pointers a scope
before the initial guard reaches the correct 356-byte size but changes
41 words through different address lifetimes and allocation. The accepted
guarded phase preserves the native lifetimes without artificial uses.

## Verification

Use the normal project gates:

```sh
python tools/check_unit.py src/dds2/game/code_00294730.c
ninja
python tools/run_tests.py
```

DDS1 source and shared headers are unchanged. All comparison artifacts
remain local; no retail binaries or compiler binaries are included in the
source change.
