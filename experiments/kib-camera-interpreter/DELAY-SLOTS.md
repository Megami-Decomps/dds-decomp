# Actor-status delay-slot decision

This observation watches one source role in the original 570-difference camera
interpreter candidate: the actor status 0x80000 guard and the first argument to
its immediately guarded position call. It does not assume a register-home
change would be a valid source correction.

The existing RTL isolates the difference to delayed-branch scheduling. In the
candidate, the increment from the shared advancement path is copied into an
annulled branch's delay slot. Native instead puts the actor argument move into
an ordinary branch's delay slot. The resulting four-byte displacement also
changes later jump-table-target alignment.

The pinned compiler's implementation first tries the owned fallthrough path
for this branch probability. Therefore the concrete question is why the actor
argument move was not selected, rather than whether the target-path increment
was given a higher generic priority.

## Bounded observation

`tools/ee_gcc_delay_role_tracer.py` resolves the unique source guard and call
occurrence into initial RTL, then follows their actual UIDs into `28.mach`.
The watch records exact current branch, argument, call and target-chain
identities. It rejects missing or changed source, dump, instruction or compiler
identities. At delayed-branch entry, the live input must match the watch.

The selected branch alone enables resource and eligibility hooks. Events cover
fallthrough/target ownership, interference checks, trap checks, machine delay
eligibility, and the opposing path's conservative live-register calculation.
Live-mask provenance includes cache/recomputation routes and bounded scan
transitions. These are observations of the candidate compiler, not native RTL.
All hooks verify the pinned executable's original instruction bytes.

The existing observer runs the unchanged compiler with the existing verified
Unix transport. Ordinary compilation, frozen native control, QEMU baseline and
observed target bytes/relocations must agree. The complete declared artifact
inventories and contents must agree too. The mode adds no debugger writes,
compiler options, cost overrides, socket fallback or guest instrumentation.

The driver accepts `--mode delay`; default `--mode cse` preserves the previous
CSE observation. The case builder accepts only those two explicit watch kinds
and adds the selected observer module to its frozen input closure. The hosted
workflow selects the delay mode for this experiment.

## Validation boundary

Before live execution: 30 synthetic delay-watch/decoder/gating/summary tests,
35 existing lineage tests, peer-probe self-tests, and all 20 static hook byte
signatures passed locally. A watch was constructed successfully from the
ordinary pinned probe. This establishes offline checks only; a successful
paired live run is required before using a reported decision as evidence.

Raw compiler patterns, pointers, masks, objects and retail inputs remain private.
The published summary contains only checked role decisions, resource-route
metadata and bounded source-derived UID transitions. No new game-code match,
full retail-image qualification or runtime test is claimed by this tool change.
