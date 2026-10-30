# Counter-map selection row: ordinary source reconstruction

DDS1 `sdfDrawCounterMapSelectionRow` at `0x002C4C88` matches all 1,712 bytes.
The source is a supported reconstruction, not a claim to recover the unique
original spelling. It uses no added assembly, register bindings, volatile
accesses, artificial memory effects, or dummy consumers.

## Actual drawing operations

The row keeps its pre-callback timer, selection, and active-channel snapshots.
Existing headers describe the separately allocated 0x24-byte timer and
0x10-byte display record; their layouts and ownership are unchanged.

Each tint branch owns one packed color and performs one real plate call.
Coordinate shifts occur at their actual drawing consumers. The two pulsing
quads each supply four genuine unsigned alpha conversions, one per corner,
rather than collapsing them into a single shared packed result. These changes
restore native call and conversion structure without adding runtime calls.

The current-row overlay owns an initialized red channel, `red = 0x80`.
Its `red & 0xFF` operation expresses the same low-byte channel contract as
`PACK`'s `(u8)r`; green and blue remain `0x80`. All four results feed actual
quad colors. The mask is a byte-packing operation, not an unused allocation
operand or a fabricated observable effect.

## Qualified compiler controls

The pinned compiler distinguishes equivalent byte-truncation spellings. In a
negative control using the `u8` cast on the real word-sized red channel,
`10.cse2` retains a zero extension of a QI subregister, even though its
`REG_EQUAL` note contains the corresponding nested expression around constant
128. That form survives to an extra load and byte extraction. With the
explicit byte mask, the corresponding `and:SI` in `03.cse` becomes constant
128 in `10.cse2`. The resulting real channel value is shared by the remaining
corner compositions, reproducing the native RGB packing. The negative control
is not landed, and the compiler itself is unchanged.

After reconstructing those consumers, only 21 entry-block words differed.
Changing the actual current-row predicate from a boolean initializer to
`if (index == mode) isCurrent = 1; else isCurrent = 0;` resolves them.
Both arms produce distinct, meaningful results and every path initializes the
value before callbacks and its final overlay consumer.

The explicit branch is already converted to XOR/unsigned-less-than in
`02.jump`; no extra runtime branch survives. Its compiler-generated comparison
temporary differs from the initializer's temporary. Qualified `19.lreg`
records show the resulting native local homes: timer pointer `v0`, channel
index `a3`, mode/comparison `a2`, and head pointer `t0`, instead of the control's
`v1`, `a2`, `v0`, and `a3`. The predicate-only control retains native extent
but restores exactly the 21-word entry difference.

## Verification scope

The polished source passes the complete C-owner check: 37 functions, zero
differences. Its owned 28-byte read-only table, including all seven resolved
text relocations, and 24-byte small-data region match retail. Ordinary and
dump-enabled full-owner objects agree in every allocated section and normalized
relocation contract; the predicate-only control and cast diagnostic have their
own qualification receipts. Final integration passes both games' retail and
development builds, with original retail SHA1s unchanged, 442 tests in 117
batches, and all 83 remaining retained-C units with zero compilation failures.
