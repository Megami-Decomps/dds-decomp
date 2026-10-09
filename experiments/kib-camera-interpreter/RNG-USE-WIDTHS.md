# Separate the RNG result's two use widths

The opcode 9 result has two distinct uses: its low halfword is stored in the
cursor index, while a signed halfword indexes a table. Retaining the raw result
for the store and narrowing only the table index reproduces the native use
relationship without changing either value.

The generated address calculation now uses SLL16/SRA15, which computes twice
the signed low 16 bits. The previous shared `s16` temporary generated
SLL16/SRA16/SLL1. This removes one actual indexing instruction. A later alignment
nop absorbs the size reduction, so the full function size does not change.

Composed with the snapshot join:

- The RNG-to-next-call interval is 19 words, matching native length.
- The following interval is 22 words, matching native length and coarse shape.
- The snapshot restoration interval remains 245 words with the recovered scalar
  allocations.
- The complete candidate is still 5240 bytes with 573 positional differences.
- All 515 older C matches remain exact, with two data issues and zero context
  or other issues.
- Ordinary and probe target bytes and all 262 function-relative relocations
  agree.

The independent reset stores are still scheduled differently from native.
Their destinations, widths, values and completion before the next callee agree.
That observation does not justify rearranging statements to search for a
preferred schedule. No further store-order trial was made.

## Replay and limits

Apply `snapshot-join.patch` to its documented game base, then apply
`snapshot-random.delta.patch` to that owning source. The standard
`current-body.c` / `candidate.patch` diagnostic remains unchanged.

These local measurements use game base
`9e573143fa0ea1e013781dc904138e7c68273990`, diagnostic base
`1bce3c8a32351951f632cd3bde7535dfea3aeccc`, and the same private official-QEMU
prefix adapter described in `SNAPSHOT-LIFETIMES.md`. Full compiler evidence is
retained privately; `snapshot-random-results.json` contains bounded results.
There is no full retail-image or runtime qualification for this nonmatch.

Credit Basalt Finch/team, Obsidian Heron and PiM for the earlier reconstruction.
KiB / redthing1's dot traced and composed the source use-width correction.
