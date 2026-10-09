# Grounded status-width correction

Base: `9e573143fa0ea1e013781dc904138e7c68273990`.

Integrate primary status owner from `907b59e6c2e16982fc7e6367021094d6414288b3`. Five scalar reads now use `status.flags`. The two peer eligibility tests use the existing `btlUnitStatusPair(unit)` accessor for the complete 8-byte owner. No extra view, cast, signature or storage is introduced.

The bounded native probe in [run37922593023](https://github.com/Megami-Decomps/dds-decomp/actions/runs/37922593023) confirms an 8-byte first peer-status read for entry opcodes34–38. Mask0x201 still selects only low bits0 and9; upper stateFlags bits do not affect the predicate. Later0x20 width is not established by that probe and remains scalar. Callback-fresh reads and original phase fallback are preserved.

All ten historical patch contexts match uniquely on this base. `current-body.c` is the complete readable candidate. `historical-candidate.patch` retains the previous5248B/600-difference packet; `candidate.patch` contains the seven changed lines for the current owner. No new match or recovered unpublished PiM delta is claimed. Credit Basalt, Obsidian and PiM.

The no-default counterfactual is rejected by bounded native semantics, and compiled5344B against5256B native. Its separate source is retained only as negative evidence.
