# Indexed one-skin packets: retained payload-count allocation

`sdfModelSetOneSkinDrawPacket` occupies 384 native bytes in both games:
DDS1 `0x002D7FB0`, DDS2 `0x00330E60`, both in `sdf/sdfModel.c`.
The candidate emits the asset reference, one seven-quadword frame-workspace
reference for every node ID, a count/VIF payload, and the command's payload
reference. The two retail bodies differ only in relocated call destinations.

## Ordinary source and allocation dependency

The ordinary retained reconstruction emits all 384 bytes but has nine words
different. Retail retains the packed asset/count word and then its low-half
payload count in `s6`, and the loop-invariant `0x6C07C000` VIF UNPACK code
in `s7`. Ordinary allocation exchanges these two homes. The mismatches are
limited to the corresponding saves, two-part constant construction, word
load, asset-index shift, halfword mask, UNPACK OR, and final count store.

The candidate has one localized initialized binding:

```c
register u32 packed __asm__("$22") = command->assetIndexAndCount;
```

This actual word supplies `packed >> 16` to `sdfModelSetMatRefPacket`, is
masked with `0xFFFF`, and supplies the final payload tag's halfword count.
There is no added read, dummy consumer, artificial lifetime, false type or
qualifier, empty asm statement, register-bound constant, or one-off helper.
This is an allocation reconstruction, not recovered source spelling or a
hardware requirement.

Bounded ordinary controls were retained rather than promoted:

- Separate asset/count halfwords and a separate payload-count local each
  changed the frame/scheduling and emitted 376 rather than 384 bytes.
- Naming the UNPACK constant or using signed loop/count locals retained the
  same nine-word exchange. Naming the actual frame byte offset changed the
  frame/scheduling and emitted 376 bytes.
- Indexed node IDs emitted 384 bytes but retained fourteen differing words.
- Keeping a complete encoded UNPACK cursor eliminated real constant/OR
  operations and emitted only 360 bytes. This was rejected as not native.

The removal controls differ from the final candidates only by replacing the
binding declaration with `u32 packed`. Both complete owners have 23 matches
and this one difference. Both bound candidates have 24 matches, zero
differences, including the complete native 384-byte target.

## Qualified actual-compiler records

Pinned compiler SHA-256:
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.
Ordinary compiler `-da` dumps were captured through `tools/ee_gcc_probe.py`
with each game's explicit `--version` and canonical
`--as-unit src/<game>/sdf/sdfModel.c`, then independently qualified against
normal production-wrapper compilation of the identical source.
No debugger execution or unqualified hooked trace is cited here.

Definitions and consumers identify the values, rather than guessing from
physical register numbers:

- In the removal controls, pass 00 UID 17 loads command `+4` into pseudo 88;
  UID 55 masks that same pseudo. The final halfword store UID 192 consumes
  it. Pass 19 records five references over 61 live instructions, crossing
  three calls, for a derived allocation priority of 1639.
- The ordinary UNPACK code is pseudo 110. The loop pass moves its constant
  definition from UID 124 to UID 237 in the preheader. The OR that supplies
  `packet->secondVifCode` consumes it. Pass 19 records five references over
  58 live instructions, crossing two calls, priority 1724.
- Pass 20's actual global allocation order places constant pseudo 110 before
  packed pseudo 88. Their dispositions are respectively hard registers 22
  and 23. Other source values occupy `s0` through `s5`.
- With the binding, pass 00 UID 17 loads the real word directly into hard
  register 22. Pass 19 UID 44 shifts it into `a1`, UID 57 retains its low
  halfword, and UID 194 stores the final count and marks hard register 22
  dead. There is no extra value consumer after that operation.
- In the bound compilation, the UNPACK constant becomes pseudo 111, with the
  same five references / 58-instruction lifetime / priority 1724. It has a
  hard-register-22 conflict, and pass 20 assigns it hard register 23. The
  packed source variable is no longer a global pseudo competing for a home.

DDS1 and DDS2 produce these same records. This demonstrates the allocation
property supplied by the binding; it does not establish original spelling.

Each candidate and its corresponding removal control was compiled normally
and with `-da`. All four pairs agree in normalized **whole-owner** assembly,
all five allocated sections' bytes and semantic headers, all 183 symbols,
all 109 resolved relocation contracts, both nondebug auxiliary sections,
and ELF ABI metadata. Both compiler and assembler exited successfully in
all runs. Normalization removes only canonical-length random wrapper paths;
section extents, flags, alignment and relocation targets/types remain checked.

## Value, memory, and ABI boundaries

The initializer loads the actual unsigned packed word on every reaching path.
The upper half is the asset index and the lower half is the final DMA payload
quadword count. Both values have real consumers. The bound value remains live
across the material-reference call and every node-search/workspace-allocation
call, then dies at the final count store. Register `s6` is callee-saved in the
EE ABI; the candidate retains the native `sd`/`ld` pair for the caller's value.

Direct retail callee inspection in both games finds:

- `sdfModelSetMatRefPacket` does not touch `s6`; its tail destination
  `sdfMatMakePacket` is a leaf that also does not touch it.
- `sdfModelSearchNode` is a leaf using caller-saved GPRs and does not touch `s6`.
- `sdfModelAllocTmPacket` saves/uses/restores `s0` only and does not touch `s6`;
  the optional allocation path calls the normal ABI `sdfChipAllocDirect`.
  That routine saves/uses/restores `s0`/`s1`, and invokes the existing allocator
  callback interface and interrupt helpers under their normal callee-saved ABI.

`SdfIndexedNodeCommandView` is the specific indexed-skin command record:
packed word at `+4`, hardware address word at `+8`, full-width node count at
`+0xC`, followed by unsigned halfword IDs at `+0x10`. The definition already
existed in the retained C. The candidate makes the read-only input and ID
cursor const, verifies the count/ID offsets with native-compiler assertions,
and makes the kind-2 dispatch's concrete command conversion explicit. It does
not reinterpret a runtime model node as a command.

`SdfSkinCountVifBlock` describes the actual separate four-word wire payload
between the closing DMA CNT and the final DMA REF. It contains the UNPACK
code `0x6001C000`, node count, ITOP `0x0400000E`, and MSCAL `0x14000000`, with
native size `0x10`. The cast of `packet + 1` is a boundary into this distinct
payload, not an alternate field view of an existing DMA tag. Tag rows retain
the canonical `SdfDmaTagHeader` type. There is no added owned data object.

The loop remains a do-while, including its unconditional first node lookup.
The unsigned equality/count behavior and `unpackAddress += 7` remain unchanged.
The frame parameter uses the existing signed `SdfGraphBufferIndex` contract
and the established two-buffer `0x80` workspace stride. Calls, masks, stores,
VIF opcodes and return pointer retain retail order and effects. No missing-ID,
zero-count, allocation-failure, or out-of-domain buffer behavior is repaired.

## Review and validation status

Independent semantic review passed for both games. Both integrated complete
owners have 24 matches and zero differences. Both full retail builds reproduce
the original executable hashes, all 442 repository tests pass, and all 81
retained-C units compile. Private candidate binaries, retail
extracts, and compiler dumps are diagnostic artifacts, not source deliverables.
