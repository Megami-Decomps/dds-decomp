# DDS2 fade evaluator: five-word allocation frontier

This is a qualified compiler observation, not a matching implementation or new matching credit. DDS2 `func_001FC5E0` remains nonmatching: **5/904 words, 3,616 bytes**, frame 48. No production C replacement accompanies this case study.

## Provenance and reproduction boundary

The source is PiM's [complete checkpoint005 reconstruction](https://github.com/Megami-Decomps/dds-collab/blob/a7c4049cd235892a9e03c9714c981e2f905bd2de/drafts/pim-paired-fade-20261011/checkpoint005/source/dds2_preferred.c), incorporated into the complete current `src/dds2/game/code_001EC5F0.c` owner by replacing only the target ASM marker and supplying the truthful existing sine-function declaration. Other owner functions remain unchanged. See the [pinned donor receipt](https://github.com/Megami-Decomps/dds-collab/blob/28b781620d97456a87cbbf765b192e25909fc6ca/handoffs/harbor-maple-retirement-20261015/proof.json) and [Harbor's allocation evidence](https://github.com/Megami-Decomps/dds-collab/blob/28b781620d97456a87cbbf765b192e25909fc6ca/handoffs/harbor-maple-retirement-20261015/fade-allocator-decisions.txt).

Credits: Iron, algorithm; Copper, canonical owner/interface; Slate, flags/callback work; PiM, complete reconstruction and paired qualification; Harbor, allocator diagnosis and negative workspace controls; Luna, importer diagnosis. This case independently reproduces and extends that evidence with matching-gate controls.

A focused `tools/check_unit.py` check reproduced the five words. A separate production-context `tools/cc.sh` compile retained assembler `-EL -G8 -g`; omitting `-g` is a known importer mismatch, documented in the [pinned assembler correction](https://github.com/Megami-Decomps/dds-collab/blob/28b781620d97456a87cbbf765b192e25909fc6ca/handoffs/harbor-maple-retirement-20261015/permuter-assembler-debug-flag.patch). The allocation observer used the unchanged complete owner, `-da` pass dumps, canonical relative filename lengths, and hashed inputs. Its original-QEMU baseline and observed run matched all 28 archived artifacts across 28,726 events. Additionally, the target's bytes, 48 relocation contracts, and entire literal sections matched the production-context object. This does not claim a whole-image build or gameplay qualification.

The [qualification receipt](compiler-evidence/dds2-fade-allocation/qualification.json) records hashes and event IDs. To reproduce, construct that complete owner from the pinned body, check the recorded owner/body hashes, run the current allocation observer interface with an environment-specific frozen configuration, and require both same-path observer equality and production-context parity. Absolute-path configurations and raw traces are intentionally excluded. Event IDs are run-specific; locate corresponding RTL operations rather than assuming IDs survive another owner/tool revision.

## One allocation issue, five changed words

Offsets are relative to `func_001FC5E0`; words use decoded numeric instruction order.

| Offset | Candidate | Native | Effect |
|---|---|---|---|
| +A2C | 000519C0 | 000511C0 | Delay-slot shift writes $3 instead of $2 |
| +CEC | 000519C0 | 000511C0 | Common-tail shift writes $3 instead of $2 |
| +CF0 | 00734021 | 00534021 | Address $8 consumes changed shift register |
| +CF4 | 91020020 | 91030020 | Flags byte goes to $2 instead of $3 |
| +CF8 | 30420080 | 30620080 | Mask reads $2 instead of $3; result remains $2 |

None of these words carries a relocation. The +A28 timing-false branch skips +CEC and targets +CF0. Its +A2C delay slot therefore supplies the same offset computation. Dump `rtl.29.dbr` shows sequence 2682 wrapping branch 1650 with copied shift 2084; the original shift remains on the common path. It is one allocation choice propagated through two paths.

## Matching-gate control

The final 0x80 gate and matching 0x10/0x20 gates make identical local decisions. Each byte has `REG_DEAD` on its mask instruction. QI byte and SI mask coalesce into a four-reference quantity, lifetime 8–12, and receive $2. The two-reference shift, lifetime 4–6, receives $3. The allocator's expanded ranges overlap: mask [6,13), shift [2,8). Actual mask observations show $2 blocked at indices 6–7 during shift allocation, with $3 available.

This falsifies the proposed distinguishing explanation that the final gate has a special missing byte-death note or different local coalescing eligibility. It does not falsify Harbor's allocation diagnosis, which is reproduced.

The final index and record-base die here; earlier gates retain them. The final record pointer also lives longer into the cubic loop. Those legitimate differences are already represented and do not change the observed local choices. Native and candidate both put the final address in $8.

## Missing constraint

Native wants an uncombined byte $3/mask $2 with shift $2. No real additional flags use or cross-block masked-result use has been established that would require this separation. Extending lifetimes, introducing fake uses, or repeating the already-negative broad workspaces is not justified. The current source remains parked pending a source-backed ownership/control-flow constraint; allocator understanding alone does not recover that source.
