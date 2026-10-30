# Trail billboard alpha stores

`billAdvanceTrailInstances` matches the 1,960-byte native routine in both games:

| Game | Address | Owner |
| --- | --- | --- |
| DDS1 | `0x002A22B8` | `src/dds1/game/code_0029C530.c` |
| DDS2 | `0x002E4320` | `src/dds2/game/code_002DE248.c` |

## Source reconstruction

The trail's odd and even entries use different center-alpha flags and side-alpha scales. Keep the two side-color stores with the calculation in their existing parity branch:

```c
if (item & 1) {
    colors[3] = packed | 0xFF000000;
    packed = (u32)(scale * 128.0f) << 24;
    colors[2] = packed;
    colors[4] = packed;
} else {
    colors[3] = packed | 0x40000000;
    packed = (u32)(scale * 255.0f) << 24;
    colors[2] = packed;
    colors[4] = packed;
}
entry->age = age + 1;
```

Previously the last two color stores were factored after the branch. The change introduces no predicate, call, extra execution, dummy value, register binding, or assembly constraint. Exactly one branch executes, with the same calculations, addresses, values, store order, and subsequent age update. Buffer initialization remains unchanged. Existing hardware-helper operations are unchanged.

This is a supported ordinary-C reconstruction, not a claim that the original source used these exact identifiers or formatting.

## Qualified compiler evidence

Paired ordinary and RTL-dump compilations reproduce their respective target assembly exactly. The retained factored source has 18 differing words. The branch-local stores produce an exact match without changing buffer initialization.

The compiler's `19.lreg` reports these pointer reference and lifetime metrics:

| Source | Color pointer (pseudo 97) | Vertex pointer (pseudo 98) |
| --- | --- | --- |
| Factored stores | 19 references across 335 instructions | 19 references across 329 instructions |
| Branch-local stores | 23 references across 337 instructions | 19 references across 331 instructions |

These are compiler metrics, not counts of runtime operations. The branch-local source still executes only two side-color stores.

In `20.greg`, the factored source allocates pseudo 98 before 97, placing vertices in `s3` and colors in `s4`. The reconstructed source allocates 97 before 98, placing colors in `s3` and vertices in `s4`, as native does. Both store pairs remain present through `25.sched2`; only the merged pair remains by `27.jump2`. The final native sequence writes side colors at offsets `0x10` and `0x08`, then computes and stores the next age.

A control that merely reverses buffer initialization produces the native register assignments but exchanges the two initial buffer loads and still misorders the age calculation. Combining that reversal with branch-local stores leaves the two load differences. Restoring the original initialization order removes them. This distinguishes the supported source-use graph from a register-order workaround.

## Verification scope

The complete affected C owners pass strict function and owned-data checks: 564 functions for DDS1 and 713 for DDS2. No data layout, shared header, compiler option, or binary input is changed. Ordinary/probed allocated-section and relocation-contract comparisons are retained with the investigation receipts. Both games' retail and development builds passed, the retail SHA1s match the originals, all 442 tests in 117 batches passed, and all 84 retained-C units compiled without failure.
