# DDS2 AI action: resolved-unit transfer

`btlExecuteAiTableAction` in `src/dds2/game/code_00211360.c` occupies
140 retail bytes at `0x00211EA8`.

The reconstruction uses one localized compiler allocation constraint on the
actual unit pointer. It is not recovered original-source spelling or a
hardware register requirement. The applicable rule is "Localized compiler
constraints on real values" in `CONTRIBUTING.md`.

## Ordinary source and real values

The resolver writes a command slot and result kind to a local output pair at
stack offsets zero and four. The matched DDS1 `btlRunAiAction` uses the same
`u16`/`s32` result-pair layout. The two scalar snapshots have real consumers:
the slot goes to the unit's signed halfword `actionSlot` and the task's
word-sized `skillId`; the result goes to the task's `phase`. The handler index
and low 22-bit argument remain live across the resolver call.

The parameter `id` is already `u16`. Its ordinary integer promotion suffices
for the resolver; no redundant full-range mask is needed. The compiled native
argument narrowing remains present.

Using ordinary snapshots restores the complete 140-byte native extent and
leaves only four differing words: the result and unit-pointer loads exchange
positions at `+0x38/+0x40`, and the unit halfword and task phase stores exchange
positions at `+0x4C/+0x50`. All other source-owned functions in the complete
owner remain exact.

## Qualified compiler cause

The pinned compiler's ordinary RTL dumps explain both scheduling stages.
In the ordinary two-snapshot source, sched1 gives the result and unit loads
equal priority, weight and forward-dependent count; the earlier result-load
UID wins. Its phase store then becomes ready before the unit store.

Acquiring the unit pointer before the result snapshot fixes those orders in
sched1. However, local allocation now gives the shorter result live range v0
and the longer unit range v1. The later handler-address construction reuses
v0, so sched2's hard-register dependencies again prioritize the result load
and postpone the unit store into the indirect call's delay slot.

The final source binds only the actual unit-pointer transfer to v0:

```c
{
    register BtlUnit *unit __asm__("$2") = task->unit;
    resultKind = lookup.result;
    unit->partyRecord.actionSlot = commandSlot;
}
```

The pointer is defined after the resolver returns, has one actual memory-store
consumer, does not escape, and is dead before the handler call. There is no
extra load, store, arithmetic, fake consumer, false qualifier or lifetime
extension. The slot's existing conversion to the signed halfword field is
unchanged. Both task stores and the handler invocation retain their original
roles and values.

The otherwise identical plain-pointer removal control is nonmatching. The
binding is causal, not an annotation on an already matching source.

## Verification

Independent semantic review confirms the initialized pointer, immediate real
consumer, call-free lifetime, unchanged field types and memory effects. The
ordinary and diagnostic compilations are compared across normalized assembly,
all allocated section bytes and metadata, and complete relocation contracts.
This instrumentation-neutrality check is distinct from native exactness.

The final tracked owner passes `tools/check_unit.py`: 91 matches, zero
differences, including its owned data. Both complete retail builds pass:

- DDS1 SHA-1: `6d18898e2724bf1d145392766e8ba1e678487419`.
- DDS2 SHA-1: `9be91ee1b4a535a4cb6ec89237b5a6ba41be2add`.
- Repository tests: 442 tests in 117 batches, all pass.
- Retained-C check: 83 units, zero failures.

The change adds 140 source-owned exact DDS2 bytes. Private native files,
binaries and compiler dumps are not part of this source/documentation change.
