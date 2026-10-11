# Investigating an unexplained near-match

A useful investigation recovers a missing source fact and predicts a compiler
decision. An exact instruction sequence is the final test; it does not by
itself establish that a guessed type, file boundary or dependency is genuine.
The [decision atlas](compiler-decision-atlas.md) routes a known symptom to a
pass, [compiler diagnostics](compiler-diagnostics.md) describes the tools, and
[idioms](idioms.md) records confirmed source examples. This guide explains how
to discover a rule when those examples do not yet answer the problem.

## Start with a current, complete candidate

Read the retail body, its callers, providers and data producers before using a
decompiler draft. Preserve callback freshness, signed comparisons, update
order, unused parameters, resource ownership and all exits. A four-byte size
difference can accompany hundreds of different words; it is not necessarily a
one-instruction problem. Record both native/emitted extents and the compared
instruction residual.

Refresh the official source and coordination state. An old parked draft may
now have a real matched donor, a corrected primary owner or a newly recovered
provider. Reuse the complete strongest source with contributor credit. A park
records the failed inputs at that time, not a permanent exclusion of the
function.

For a cross-title donor, compare the complete native instruction streams,
operands, branch targets, globals and callees. Identical opcode sequences are
a discovery filter, not proof of identical behavior. Translate real layout
differences using the target title's existing owners. A same-sized payload in
both games need not have the same origin or field offsets.

Also inspect matched siblings in the same title and translation unit. An exact
opcode-twin filter misses related algorithms with different state transitions.
DDS1 `func_00267FF0` became exact by adapting the matched level-animation
`func_00267850` to the established `profileAnimation` rows. The target retains
its own initialization coordinates, completion ramp and fade; the donor supplies
the natural direct-array accesses, signed clamps and shared first state machine.
Compare every changed path against retail before transferring a source shape.

## Classify the uncertainty before changing source

| Question | Evidence that can resolve it |
| --- | --- |
| Is an operation or path missing? | Retail CFG, jump tables, actual callees and every observable side effect |
| Is the interface wrong? | Native incoming/outgoing registers, provider definition, callers, return use and old-style declarations |
| Is a field or owner wrong? | Allocation/clear size, constructor stores, consumers, stride, signed operations and project-compiler offset checks |
| Are memory accesses dependent? | RTL memory modes, alias sets, address overlap and scheduler dependence edges |
| Did a loop transform differently? | Entry edges, loop notes, invariant decisions and giv grouping in pass 09 |
| Did values receive different register homes? | Definition/use chains, local allocation, global priority, conflicts and preferences |
| Did scheduling or delay donation choose differently? | Scheduler input RTL and verbose choices, followed by the pass-29 sequence |
| Did the build emit or attribute data incorrectly? | Owned section extents, relocations, generated includes and final linked bytes |

If the current uncertainty is a provider contract, investigate that provider
before trying another temporary in the caller. A false call argument creates
early register uses and can later look like an allocation problem. Likewise,
a field incorrectly stored as an integer can create a memory dependency that
later looks like a scheduler tie.

## Find the first causal difference

Keep a baseline and one source change justified by the current evidence.
Capture both in the canonical unit context; focused standalone compilation
can hide context-sensitive neighbors. For a scratch source, the probe's
`--as-unit` option preserves the real unit path:

```sh
python3 tools/ee_gcc_probe.py src/dds1/field/fldFileResolver.c \
  --function func_001281E0 --out-dir /tmp/scene-baseline
python3 tools/ee_gcc_probe.py /tmp/scene-candidate.c \
  --as-unit src/dds1/field/fldFileResolver.c \
  --function func_001281E0 --out-dir /tmp/scene-probe
python3 tools/ee_gcc_why.py /tmp/scene-baseline /tmp/scene-probe \
  --function func_001281E0
```

Ignore unrelated pseudo renumbering; follow the first changed operation,
address, memory mode or control edge. Later changes can be consequences:
different pass-00 addresses feed CSE, CSE changes references, allocation
changes scheduler inputs, and scheduling changes delay-slot candidates.
`ee_gcc_why.py` compares candidate probes, not historical retail RTL. It proves
what the source experiment changed; native evidence is still needed to relate
that change to the original program.

Write a prediction before the next edit, for example:

> The constructor and consumers imply two coordinate elements. Replacing the
> two scalar members with that genuine array should change the pass-00 address
> split and remove the extra shared row base, while preserving every offset.

Check that predicted intermediate effect as well as the final diff. If the
first pass does not change, the experiment did not test the proposed cause.

## Turn a symptom into a source fact

Several confirmed cases illustrate how to connect compiler evidence to an
independently supported contract:

| Observation | Recovered fact | Validation beyond the final instruction diff |
| --- | --- | --- |
| Same `abs.s`, different motion of nearby constants | Game module uses the existing `ffabsf` asm helper | Paired `func_00152560` / `func_0015A150`, larger `func_001726E8`, and a kernel builtin counterexample |
| Indexed displacement exceeds the apparent scalar remainder | Nested record levels contribute separate offset splits | `BtlState.debug`, its actual extent/offset guards, and multiple related accesses |
| X/Y pointers share differently despite equal byte offsets | Real coordinate arrays differ from two scalar members | `BattleActorPanelEntry.position[2]`, both title consumers and pass-00 address RTL |
| A store/reload has unexpected dependencies | Access through a union component differs from access through its actual active member type | Existing `BattleLinkedEffectState` member, lifecycle/mode evidence and scheduler edges |
| Loop constants remain inside the body | A loop is skipped, becomes phony, or fails a cost test | Actual pass-09 entry/notes and invariant decisions; these are three distinct causes |
| An annulled branch changes beside a call | Earlier same-unit C body supplies nothrow information to reorg | Visibility experiment plus independent file-ownership evidence and every affected unit |
| Callee registers agree but caller argument setup differs | Mixed integer/FPU parameter order affects expansion | Both provider definitions and all callers of C70/D88 draw setters |
| A saved-register pair changes after a semantic rename | GCSE symbol-name hashing changed PRE pseudo creation order | First changed GCSE expression and exact sibling users of `effDefaultRandomState` |

The right question is which source fact could have produced the compiler
inputs, not which edit makes the final word look right. A visibility stub can
localize a nothrow effect but cannot prove a file boundary. A spelling change
can diagnose hashing but cannot justify a name. A flag-disable probe can
identify an optimization without justifying a production flag.

### Battle selection: distinguish membership, completion and commit

DDS1 `game/code_001A9780::func_001C0650` initially had a matching scan prefix
but a different success tail. Returning directly from the matching iteration
exposes the old-ID/target-ID XOR inside the loop. The actual loop-motion
comparison accepts that invariant: threshold 64 × savings 1 × lifetime 1 = 64
against an instruction count of 16. Spelling a conditional return instead does
not avoid it; the jump pass synthesizes the XOR before loop optimization.

Breaking out of the scan and returning afterward keeps the XOR outside the
loop, but an unconditional `missing = 1` in each iteration creates a different
hoist. The failure flag becomes live across callbacks, adds a spill and enlarges
the frame. A guarded diagnostic rejecting only that movable restores the
native frame, establishing a question for the ordinary source rather than a
production compiler change.

The scan's actual membership predicate provides the supported correction:

```c
missing = unit->partyRecord.unitId != targetId;
if (missing == 0) {
    break;
}
```

The flag now records meaningful changing state and controls the successful
exit. Exhaustion reaches the retry path; successful completion computes its
result and then commits the selected entry outside the inner scan:

```c
s32 changed = oldId != targetId;
work->entry = index;
return changed;
```

This closes the 616-byte body. Read-only traces confirm neither inner-loop
hoist remains. Computing the result before committing also changes the actual
target/index allocator comparison from keys 17307/16000 to 16071/17142,
restoring the native register assignment. No callback lies between those
completion operations. The correction follows their genuine value roles;
these numbers are evidence for this input, not a register-placement recipe.

The real earlier selector definition also supplies `REG_EH_REGION 0` to the
later `func_001C08B8` call. With short-circuit OR pairs for adjacent same-action
inputs, the input handler is 1340 bytes exact. Those pairs preserve the native
mode 4/5 key priorities and suppressed later reads. The complete owner passes
198 match/0 differ, including data, and both retail hashes pass under the
unchanged compiler. The signed cursor row/column contract was qualified
separately in both actual consumer units.

Inspect interacting transformations together: fixing one success join can
expose another hoist. A diagnostic can identify the missing obligation; native
membership and completion phases must supply the ordinary C that satisfies it.

### Console indexing: inspect the target recognizer

DDS2 `sdf/sdfDevCons::func_0033CE08` retained four differing words after its
control flow and value lifetimes matched. The tab path fused row multiplication
and column addition into `mul_acc_si_r5900`; its later split used a separate
multiply scratch register. The default path kept the retail plain multiply.

The target's `r5900_madd_profitable_p` explains that asymmetry: it rejects this
fusion when the addition's `LOG_LINKS` includes an instruction defining the
addend. The tab path had already used the column in division, losing that
direct definition link; the default path had not. Preparing the initial cell
offset before deriving the tab count preserves the link and produces the exact
372-byte body. These calculations read console state without an intervening
store or call. Read-only observation preserved all compiler and assembler
artifacts, and the complete owner and both retail builds passed.
The same source also matches DDS1 `sdf/sdfDevCons::func_002E3F58`, with its
owning unit and both retail builds passing.

This example justifies that independent calculation boundary. It does not
justify changing arbitrary statement order or disabling multiply/add fusion.

### Camera success: recover both result joins

DDS2 `game/code_00227288::func_00228B08` retained six flag-update/return
instruction differences. Before local allocation, scheduling placed the literal
return value in `$v0` between the flag load and OR/store. The flag quantity's
live interval therefore conflicted with `$v0` and took `$v1`. A boolean return
of the updated flags removed that conflict but left a normalizing instruction
where retail loads literal 1.

The native graph has two shared exits: reset/default return 0; camera setup
and linked-target success return 1. Ordinary `handled` and `unhandled` labels
recover those joins. The flag update then occupies its own block before the
handled return constant, freeing `$v0` during the update. This reproduces all
608 bytes; the complete owner and both retail builds pass. Read-only allocation
observation preserves all compiler/assembler outputs and confirms the removed
conflict. Sharing only the success return changes the reset branch topology.

Inspect the actual exit graph before introducing a computed return dependency.
Equal return values alone do not establish identical control flow or lifetimes.

### World-pool initialization: keep the published array owner

DDS2 `game/code_0010FB00::func_0010FE50` initializes the free entry chain.
The retained source unconditionally wrote adjacent indices and repaired the
last next link after traversal. Retail instead branches for the first previous
sentinel and last next sentinel inside the loop. Recovering those two boundaries
restores the native 264-byte extent, leaving five differing words around the
retained-address publication and loop entry.

A cached entry-array cursor uses the same quantity for publication and traversal.
Direct `info->entries[index]` accesses through the canonical `WorldInfo` field
introduce a separate pointer quantity in PRE GCSE. Later CSE forwards the stored
address, but the return value and loop pointer keep distinct allocation intervals:
retail publishes `$v0` directly and captures the loop base in `$a1` after its
guard. This closes all 66 words. Both complete source forms use the actual
8-byte `WorldValueEntry`; neither requires a new view or artificial dependency.
Read-only allocator observation preserves the complete ordinary output;
the full unit and both retail builds pass.

An explicit positive guard around a post-tested cached-cursor loop keeps the
five-word residual. Direct owner access in neighboring
`dds3RemoveCurrentWorldValueEntry` is byte-neutral and leaves seven differing
words. The rule is to inspect the actual PRE and pointer quantities, rather
than replace cached cursors indiscriminately.

### Camp visual sampling: inspect the last pointer use

`mnuSampleCampPanelVisualTrack` in DDS1 differed by four words because the
first scheduling pass promoted the selected key's second scalar load. The
mode, payload and two scalar loads had equal priority and no load-to-load
edges. The second scalar load carried `REG_DEAD` for the key pointer, giving
it register weight zero; the other three loads had weight one.

A diagnostic change of that single weight reproduced the entire 504-byte
native function. The ordinary C correction captures the payload and scalar
values before the mode, making the mode load the pointer's final use. GCC
then schedules the mode first and reproduces the native order exactly. All
four reads still occur before the first output write or interpolation call.
No extra read, pointer use, dependency, barrier or compiler option is added.

The useful source question is which real access ends the pointer lifetime.
Check its death note and scheduler weight before changing the capture order;
the intervened compiler output is a causal test, while the unchanged compiler
and complete-unit check establish the source match.

### File transfer: distinguish publication from later metadata reads

DDS1 `game/code_0029C530::func_002B6778` retained two adjacent instruction
differences: retail prepares the resolver argument before loading the transfer
mode. The resolver has one argument; the mode belongs to the request's
halfword field. Read-only priority and ready-clock hooks established that the
argument move was ready alongside the metadata loads but had lower priority.
A guarded compiler intervention proved that this choice explained the gap,
without qualifying as a source match.

Publishing the retained output pointer by copying its representation into the
actual pointer field gives the compiler conservative memory dependencies.
Capturing the real payload after retain and before that publication follows
the native read epoch. Reading its size and then its mode makes the mode the
last actual pointer input, retaining the native register homes and scheduler
choice. Together these changes reproduce all 828 bytes. Copying the mode field
instead changes other dependencies and register homes and remains nonexact.

This is a valid C encoding of pointer publication, not evidence that the
original spelling literally used `memcpy`, nor evidence that the separately
allocated payload and request alias. Preserve the real allocation, retain,
resolver and fresh post-resolver size-read boundaries. Onyx/Obsidian supplied
the retained reconstruction; Peregrine Forge closed the compiler gap, with
independent Luna review.

The related queue-duplication coordinator `func_002B6C00` had the same two
argument/option-load differences after its own complete native audit. Its
allocation/retain path also captures the queued payload before publishing the
output pointer. Applying the same real publication and final-input boundary
closes its original 980-byte body. The function retains its own header copy,
sector-leader linking, resource-name copy, linked-job count and cleanup paths;
this is not a counterpart port. Luna Field supplied this closure, with Luna
Event's independent provider/epoch review and Peregrine Forge integration.
Preserve Ember, Quartz, Azure, Tidal and Sable's retained source lineage.

The DDS2 counterparts `func_002FD900` (912 bytes) and `func_002FDDD8`
(1068 bytes) close with the same publication and final metadata-input boundary.
The retained duplication draft reproduced the two swapped words at offsets
`+0x288` and `+0x28C`; applying this correction leaves all 683 functions in
its owner exact. Preserve DDS2's `0x74`-byte defaults copy rather than DDS1's
`0x3C`, its `effQueueEffectFileJob` provider, and the fresh global payload-size
read after the resolver. A qualified ordinary/probe comparison preserves the
complete assembly. This transfer credits Brass's retained DDS2 draft and
Peregrine's investigation alongside the donor reconstruction lineage above;
KiB integrated and independently checked the native read epochs.

### Grid placement: compute each selected coordinate pair in its own arm

DDS2 `game/code_00285CE8::func_00285E98` merged its selected Y-direction byte
across the axis-selection branch. Read-only allocation observation traced a
`$v1` preference from the byte's sign-extension result to that merged quantity;
retail uses `$v0` in each arm. Computing the real X and Y coordinates together
inside each axis arm removes that merge and reproduces all 576 bytes. The
12-byte direction table, signed component arithmetic, depth/countdown fields
and two separate random-call epochs remain those of the native function.

The complete owners and both retail builds pass for these two corrections.
This observation supports the branch-specific coordinate computation; it does
not justify adding uses to change register preferences. Preserve PiM, Silver
and Sable's reconstruction lineage; Luna Field supplied the source closure,
with independent Luna Event review and Peregrine Forge integration.

## Investigate a mechanism that is not documented yet

1. **Reduce the question, preserving the cause.** Identify the smallest
   related access, call or loop that exhibits the changed compiler decision.
   Keep the real types, declaration visibility and relevant context. A toy
   that loses the symptom is an informative negative control, not a solution.
2. **Inspect the decision itself.** Use the matching compiler's dump and, when
   needed, its implementation or existing instrumentation. For a scheduler
   tie, obtain the actual input notes and ready choices. For a record access,
   inspect the field-offset split before later folding obscures it. Avoid
   reconstructing a whole optimizer when one concrete branch or cost test
   answers the question.
3. **Distinguish competing explanations.** Select one supported change whose
   predicted intermediate effect differs between the hypotheses. Changing
   type, control flow and lifetime at once cannot identify the cause.
4. **Require native/source support.** Prove the proposed owner, parameter,
   array, macro or file boundary using independent producers and consumers.
   Desired register homes and instruction order are insufficient evidence.
5. **Test transfer and a counterexample.** Apply the rule to a related real
   target or twin, then check a case where it should not apply. The kernel
   builtin abs case prevents an unjustified global helper replacement.
6. **Publish the result with its limits.** Document exact references, the
   changed pass, prerequisites and stop condition. Keep an unclosed diagnosis
   separate from a confirmed idiom; do not turn one success into a universal
   source rule.

No scripted or enumerated search over statement, declaration or operand
orders is part of this method. Neither are arbitrary symbol spellings, fake
views, invented dependencies, register pinning or `volatile` for scheduling.
If a real dependency remains unknown, investigate its producer rather than
manufacturing one.

## Keep experiments useful and bounded

For VU/MMI operands, check which scalar transfers belong to GCC and which
belong inside the SDK block. DDS2 `sdfVuMultiplyNodeColors` initially differed
by three prologue words: a candidate with an explicit `mfc1` inside the block
gave the work-pointer copy scheduler priority 5 and the scale constant 4.
The SDK-style `"r"` input for the binary32 scale, consumed by `qmtc2`, lets
GCC generate the transfer. After reload its visible dependency raises the
constant's priority to 6, reproducing the native constant-before-pointer order.
The natural C loops and the shared `EE_MMI_RGBA_MULTIPLY_VECTOR` macro then
match all 168 bytes; the full unit and both retail builds are exact.

This applies when the assembly consumes binary32 bits as a GPR operand.
Keep `"f"` and an explicit transfer for a macro that actually owns that
instruction. Do not insert a dependency or move scalar setup into assembly
solely to force a schedule.

Separate real status and decision phases when their lifetimes differ. DDS1
`evtUpdateMainTaskPopupEntry` returns a nonzero panel result, otherwise reads
the dispatch status and, when it is zero, accumulates the missing-task and
active-flag popup conditions. A qualified conditional-move trace showed GCC
materializing the `1` input only after the flag call in the earlier boolean
form. Using the actual dispatch status as the initial decision recovers the
shared constant before the lookup, but reusing one variable for both phases
makes it global and gives it the wrong saved-register home. A separate local
popup decision initialized from the zero dispatch status is allocated first
(priority 6666 versus the constant's 1428); all 156 native bytes then match.
Both observed allocator runs preserve the complete ordinary assembly.

This is a control-flow and value-lifetime result for this callback. It does
not license adding artificial uses or splitting every local. Confirm the
actual native phases and the changed compiler decision before transferring it.

Preserve a compact receipt: public base, complete preferred source/patch,
source hashes, exact commands, native/emitted sizes, first changed pass,
residual, previous contributor credit, successful gates and negative controls.
A residual table without the measured source cannot be resumed reliably.

Stop a hypothesis when its predicted input is absent, its type contradicts
the owner, or motivated forms reproduce the same residual. Stop a candidate
when supported hypotheses are exhausted; retain the complete draft and the
unanswered question. Do not abandon a credible candidate solely because its
first draft differs. Reopen it when a genuine prerequisite changes, rather
than repeat invariant experiments.

Measure yield in newly exact bodies and retail bytes, alongside qualified
parks. A broad sweep that mostly returns negatives still narrows the method's
scope. Small word counts do not promise easy closure, while one real owner
correction can unlock several larger bodies and their twins.

## Qualify the retained source state

Follow [Contributing](CONTRIBUTING.md): whole touched units must be clean and
both retail builds exact. Shared headers require every actual dependent, not
just the new target. Moved/replaced development units also need their
development links. Review data ownership, public diff and contributor credit.

Refresh main and claims immediately before integration. Reuse evidence for
unchanged source/dependency/configuration inputs; refresh checks affected by
actual changes. Administrative work or documentation edits do not invalidate
an unchanged successful source check. A focused match, near-match or matching
subset never replaces the complete acceptance gate.
