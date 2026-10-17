# Seven-function recovery qualification, 2026-10-15

> Historical qualification on base bad6a472d. The October 17 integration retains upstream mapper/battle bodies and adds only the endpoint and PCP pairs (4 functions / 9,616 bytes). Earlier scalar effect-link and single-target workarounds below are not retained. See [current integration](recovery-integration-2026-10-17.md).

Status: all five current complete-owner checks pass. Both retail-image SHA1 gates pass, both development images report zero changed payload words, and the immediate four-target rebuild is a no-op. The pristine upstream baseline also passed before these changes.

## Scope

Base: `bad6a472ddeaa98f9e12dcb0f7f8fec910be9be7`.

Seven previously restored functions were still assembly-backed on that upstream base:

- DDS1 `func_001EB368`, DDS2 `func_001FBAC0`: 2,844 bytes each
- DDS1 `func_001F0CA0`, DDS2 `func_00201828`: 828 bytes each
- DDS1/DDS2 `effPcpBlockSetUpdate`: 1,964 bytes each
- DDS2 `effBattleUpdate`: 2,028 bytes

Total: **7 functions / 13,300 native bytes**. Upstream DDS1 `effBattleUpdate` and all other distinct upstream implementations remain retained.

## Provenance versus fresh qualification

The recovered bodies came from preserved October 14 source snapshots. Each recovery group's manifest records the recovered file hash; `current-seven-targets-oct15/MANIFEST.json` binds the selected body hashes and initial current-owner candidates. Earlier source hashes, compiler traces, owner counts and image receipts describe their historical contexts. They are not current-context qualification, and source preservation alone does not establish a match.

Fresh complete-owner checks after the integration prerequisites and equivalent DDS2 camera resolution report:

- DDS1 camera owner: 259 match / 0 differ
- DDS2 camera owner: 242 match / 0 differ
- DDS1/DDS2 PCP owners: 368 match / 0 differ each
- DDS2 battle-effect owner: 19 match / 0 differ

These include the seven recovered targets. The final image gates and immediate no-op check also pass; see the accompanying current qualification receipt. This is native matching reconstruction, not a gameplay or cross-compiler portability claim.

## Required integration contracts

Endpoint returns are signed status values. The DDS2 bounds provider returns an `f32` in f0 and takes the native nine arguments, including two signed-byte selectors. Its declaration is localized beside the new consumer. The shared cursor fills formerly opaque fields without changing its 0x130-byte extent or existing offsets.

The mapper requires complete scalar effect-link ownership: `u16 flags`, two padding bytes and `s32 referenceCount`, totaling eight bytes. Both existing packed observations copy the complete record into a local `u64` through `memcpy`, at their original read points. The previous union representation shared physical offsets but did not reproduce the qualified scalar alias behavior. The record-size/reference-offset and actor-size/alignment checks remain intact.

The typed PCP updater also requires removing obsolete integer casts from its two existing `effPcpRotateFireIds` callers. Both casts were removed and both complete owners rechecked at 368/0.

## Equivalent DDS2 camera merge choice

Composing the newer upstream single-target camera source with the restored owner left only the already-existing `func_001F2308` context mismatch: 764 emitted bytes rather than 752. Its source body was unchanged. The merge retains the previously qualified DDS2 `func_001EDC38` reconstruction instead of changing the unrelated leftmost implementation or compiler settings. All 96 initializer words were compared bit-for-bit with current native `D_00417F30`; the obsolete external-bank declaration and duplicate rodata emission were removed. The grouped-camera type remains beside its remaining first user. No distinct peer functionality was discarded.

This equivalent single-target function is already matched upstream and adds **no new matching credit**. The subsequent complete DDS2 owner passed 242/0. The evidence establishes this source-context resolution; it does not by itself prove a particular allocator or heap mechanism.

## Credit

- Endpoints: PiM checkpoint011, incorporating Basalt and Silver; earlier DDS1 work by PiM, Silver and Brass. [Retained checkpoint](https://github.com/Megami-Decomps/dds-collab/tree/c9a759ce56d18f4f03dd36059a9f10b15a70827d/drafts/pim-dds2-endpoint-20261011/checkpoint011).
- Mappers: Garnet Tern / PiM, Halcyon Drift, and Harbor Maple. [Released paired handoff](https://github.com/Megami-Decomps/dds-collab/tree/efc0b5e13d96a73ef46c496f2163e97d3528abbe/handoffs/harbor-maple-target-flags-20261014).
- Battle updater: Silver Helix, Obsidian, KiB, Sable Forge, Brass Osprey and Evening Notes. [Immutable battle handoff](https://github.com/Megami-Decomps/dds-collab/blob/b269b5a212f78c5386965780040f86b4bb4fcba0/handoffs/evening-notes-20261014/battle.patch).
- PCP: Deep Quasar, KiB, Silver Helix, Amber, Brass Osprey, Sable Forge and Evening Notes. [Immutable PCP handoff](https://github.com/Megami-Decomps/dds-collab/blob/b269b5a212f78c5386965780040f86b4bb4fcba0/handoffs/evening-notes-20261014/pcp-block-set-dds2.patch).

## Remaining unrecovered scope

Four functions / **4,504 bytes** from the earlier ledger remain outside this recovery:

- DDS1 `func_001B4F10`: 1,120 bytes
- DDS2 `func_001BFB58`: 1,152 bytes
- DDS1 `func_001FDA78`: 1,116 bytes
- DDS2 `func_0020FA98`: 1,116 bytes

No recovery, current matching qualification or new contribution is claimed for those four.
