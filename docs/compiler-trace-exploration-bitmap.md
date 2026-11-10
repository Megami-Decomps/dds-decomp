# DDS2 exploration bitmap: current-area owner pointer

`fldMarkStageExplorationBitmapRegion` in
`src/dds2/game/code_0011F208.c` occupies 244 retail bytes at `0x00123DE8`.
It marks a rectangular region in the exploration bitmap, with ten bytes per
row. DDS2 looks up the stage index using the supplied area, discards that
result, and obtains the stage's row offset using the current global area.
Those distinct arguments and both calls are preserved.

## Ordinary source and compiler cause

The ordinary reconstruction already reproduces the complete loop. Its sole
remaining cause of mismatch is the current-area load before the second call.
The native sequence materializes `&fldGlobalBase`, then loads its signed
`area` member at byte offset `0x10`. Ordinary C combines that address into a
relocated load of `fldGlobalBase + 0x10`. This removes an instruction before
the loop and shifts the intervening code until alignment absorbs the change:
the emitted extent remains 244 bytes, with nine differing words.

The pinned real compiler's `14.combine` dump identifies the transformation.
In the otherwise identical constraint-removal control, the original owner
address definition is deleted and the remaining high/low pair refers directly
to the member address. In the candidate, the initialized owner pointer remains
an actual register operand, and the load still addresses `owner + 16`.

The source uses one localized empty tied operand on that pointer:

```c
work = &fldGlobalBase;
__asm__("" : "+r"(work));
rowOffset = fldFindStageCoordinateRowOffset(work->area, floor);
```

This is a mechanistic compiler reconstruction under the "Localized compiler
constraints on real values" rule, not recovered original spelling or a
hardware register requirement. There is no selected hard register. The pointer
is initialized after the index call, used immediately for the real area load,
and dead at the row-offset call. The empty template leaves its tied value
unchanged and emits no instruction. It adds no memory access, arithmetic,
clobber, qualifier, dummy consumer, or artificial lifetime extension.

Independent semantic review confirms the real typed owner and its asserted
member offset, initialized input/output, call-free lifetime, unchanged signed
field load, and unchanged bitmap accesses and loop behavior.

The DDS1 twin does not load `fldGlobalBase` here: it selects a separate
800-byte map using the returned stage index. This constraint therefore does
not transfer to DDS1, which remains unmodified and nonmatching.

## Qualified evidence

The canonical compiler SHA-256 is
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.
For both the final candidate and its otherwise identical constraint-removal
control, ordinary and `-da` diagnostic compilations agree across:

- Normalized assembly for the entire owner.
- All six allocated sections' contents and metadata.
- All 610 non-file symbol contracts.
- All 666 relocation contracts.

The final complete owner has 59 matching functions and zero differences,
including its owned-data checks. Removing only the constraint leaves 58
matches and the one nine-word residual. This causal control is separate from
instrumentation neutrality and native exactness.

Private native binaries and compiler dumps are not part of the source change.

## Full validation

Both complete retail builds pass their exact SHA-1 gates:

- DDS1: `6d18898e2724bf1d145392766e8ba1e678487419`.
- DDS2: `9be91ee1b4a535a4cb6ec89237b5a6ba41be2add`.

The repository suite passes all 442 tests in 117 batches.
All 83 retained-C units compile, and `git diff --check` passes.
