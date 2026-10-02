# EE GCC decision atlas

This atlas maps a matching symptom to the earliest compiler decision that can
cause it. It is a routing guide, not a list of syntax tricks. First compare two
truthful source forms with the [compiler diagnostics](compiler-diagnostics.md),
then use the row for the first changed target-function pass. A late assembly
difference does not prove that a late pass caused it.

`ee_gcc_why.py` explains what changed between two compiler probes. It does not
compare a candidate's RTL directly with retail RTL, so its result establishes
causality for the source experiment, not by itself the origin of a retail
mismatch.

| Decision family | Evidence to require | Truthful lever to test | Stop rule |
| --- | --- | --- | --- |
| Type and ABI lowering (pass 00) | Signed/unsigned operations, argument and return registers, callee definition and callers | Correct parameter, return, field or expression type; real prototype | Stop when the proposed type conflicts with the interface or leaves pass 00 unchanged |
| CFG and branch topology (passes 00-12) | Candidate and retail block edges, compare order, branch polarity, shared returns and jump-table entries | Natural `switch`, separate cases, accumulator or early-return structure supported by behavior | Stop when CFGs agree and the residual begins in allocation or scheduling |
| CSE and translation-unit context (pass 03) | First changed expression, symbol/alias inputs and a whole-unit comparison | A real type, prototype, global declaration or translation-unit boundary | Stop on declaration/name/address perturbations with no semantic or unit evidence |
| Pseudo liveness and allocation (passes 13, 19, 20) | Def/use RTL, live ranges, local dispositions, global order, conflicts and preferences | End a real lifetime earlier, remove a redundant use, or preserve a real expression instead of naming a temporary | Stop when the needed lifetime or use change would misstate behavior, or identical inputs still select differently |
| Memory dependence and sched1 (pass 17) | Aliasing/dependence edges and verbose ready-set priorities | Correct member types, source order or a real dependency | Park when independent instructions stay tied and the desired order has no truthful dependency |
| Sched2 and delay donation (passes 25-29) | Verbose ready choices, donor eligibility and the pass-29 delay sequence | Real argument expression/type, call visibility or prototype that changes donor availability | Stop when the same donor is already eligible and truthful forms do not change the choice |
| Assembly/object emission | Compiler assembly agrees but object words differ | Project assembler compatibility fix with an independently characterized rule | Do not distort C to compensate for an assembler-only difference |

## High-value order of operations

1. Confirm that the candidate behavior, types and translation-unit identity are
   credible.
2. Find the first semantic divergence between two controlled probes.
3. Inspect the evidence named in the corresponding row before editing C again.
4. Test one semantic lever. Record whether it changed the predicted pass.
5. Keep a source idiom only after an exact natural-C result transfers to another
   function. Otherwise record a justified park and move on.

A correct park is useful output. It prevents repeated searches for a source
lever when the visible compiler inputs do not support one.

## Allocation decisions in this compiler

Passes 19 and 20 together report the inputs that explain the global allocator's
ordering. Its numeric priority is:

```text
floor_log2(references) * references * hard_register_width * 10000 / live_length
```

The comparator also contains a dormant first tier for compiler-created
`range_copy` pseudos. The only code that sets that bit is reached through the
unreferenced `live_range` entry: the shipped compiler has no code or runtime
data reference to that entry, and its address occurs only in debug/symbol
records. Ordinary project compilations therefore have no such tier and use the
numeric score directly. Do not use the dormant bit to explain an apparent
ordering inversion. The score is truncated to an integer, higher values
allocate first and exact ties use the lower allocno number. Hard-register width
comes from pass 20's parenthesized width and is normally one; it is not the byte
size printed by pass 19. `calls_crossed`, pointer and user-variable flags are
useful allocation context, but are not part of this ordering formula. Local
allocation happens before this global order and uses its own related quantity
model.

`ee_gcc_allocations.py` pairs pass 19 with pass 20 and prints
`refs/live/width/priority`, local assignments, global attempt order, conflicts,
preferences and final dispositions. These facts narrow an allocation problem;
they do not map pseudo numbers to source variables or replay every hard-register
rejection. Establish identity from the pseudo's definition and uses in RTL.

### Interpreting preferences

Treat pass 20's preference set as the allocator's surviving input, not as a
record of how the preference arose. A hard-register preference can come
directly from a `set` involving that register (including a pseudo already given
a local hard-register home). Preference expansion can then union normal and
full preferences across a conflict-free `set` whose dying input is another
global allocno; copy preferences propagate only for a direct `set` from one
register to another. Before allocation, pruning removes fixed, conflicting,
call-used and out-of-class registers. Pass 20 does not distinguish these origins
or show preferences that pruning removed.

The allocator also tries to leave room for lower-priority conflicting allocnos
by initially avoiding registers they prefer. This is a soft reservation, not a
hard conflict: if the first search fails, the allocator retries without that
reservation. Consequently, a final home outside the printed preference set can
be ordinary first-fit after conflicts and reservations when no usable
preference wins; it is not evidence of a hidden source-level preference.

Use the final home only as the end of a causal chain. First identify the
pseudo, then compare its defining `set`, death points, conflicts and surviving
preferences. Test a source change only when it expresses a real lifetime,
copy, or expression relationship and predicts which of those inputs will
change. If the relevant lifetimes, local dispositions, global order, conflicts
and preferences stay the same, or the required change would invent a use or
lifetime, stop rather than permuting declarations around the selected
register.

### Saved-role feasibility gate

Use the number and lifetime of meaningful source roles as an early stop test
for allocation residuals. If retail keeps several values separately preserved,
but a behaviorally correct candidate has already combined, ended, or omitted
those roles by passes 19 and 20, declaration order and later scheduling cannot
manufacture the missing allocation topology.

Require all of the following evidence before applying the gate:

- retail assembly visibly gives the values separate roles across the relevant
  calls or control-flow edges;
- the candidate's emitted data flow and pass-19/20 report have fewer distinct
  live roles at the allocation boundary; and
- source-location records, where available, agree with that reading.

Then test at most one real source fact capable of creating the missing
lifetime: a correct type or prototype, a caller/callee contract, a genuine
later use, or an ownership/alias boundary. If no such fact is supported, or it
does not change the predicted allocation input, park the function. Renaming or
reordering declarations, adding dummy uses, and trying to repair the final
delay slot are not evidence for a missing role.

This is a one-way feasibility test, not a proof of original source. STABS homes
are final compiler hints, not pseudo identities or location timelines. Missing,
duplicate, or ambiguous records make the result inconclusive. A candidate with
the right number of roles can still differ because of conflicts, preferences,
reload, or scheduling; continue with the ordinary allocation report in that
case.

The transferable source shapes discovered so far are catalogued in
[Matching C idioms](idioms.md). Treat their stated preconditions as part of the
idiom: a shape that worked for one mechanism is not a generic permutation rule.

## Evidence standard for new mechanisms

A reusable compiler claim should include:

- the earliest pass where a controlled source change takes effect;
- the decision inputs visible in that pass;
- one natural source fact that changes the expected input;
- an exact result on the discovery case and a separately chosen transfer case;
- a negative condition that tells the next worker when to stop.

Parser fixtures and familiar examples validate tooling, but do not count as a
held-out transfer. Measure acceleration with new released blockers: time to the
correct mechanism, number of compile hypotheses, exact matches, correct parks
and false actionable diagnoses.
