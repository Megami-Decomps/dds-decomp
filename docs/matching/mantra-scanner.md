# DDS2 mantra source scanner

`mnuScanMantraSourceCandidates` at `0x0028F9A0` is 868 bytes. It captures the
selected party record, updates the first source record's rank state, then checks
the remaining 111 records for event, party-category, and selected-party
requirements. The nine-byte member-category map and four diagnostic strings
are ordinary local initializer/string data.

The reconstruction builds on the earlier Garnet, MantraPurist, PurBoss, and H1
scanner investigations and KiB's retained typed-member reconstruction. The
selected value is loaded through the address of the existing
`MenuListNode.partyRecord` union member; this expresses the actual pointer slot,
not a fabricated alias or a cached list across provider calls.

The final seven-word residual was an initial local-allocation difference.
Capturing the selected party before assigning `record = object->menu.records`
changes the serialized order of two same-cycle operations in `sched1`: list
load before record-base setup, rather than the reverse. The list's allocation
interval grows while the cursor's remains short. The subsequent `lreg` pass
then assigns list `$4`, cursor `$3`, initial flags `$2`, and initializer high
address `$5`; the local nine-byte copy reuses `$4`, as retail does. A strict
EE GCC 2.96 probe preserves all 21 RTL dumps and assembles the exact result.
The qualified observer agrees with the ordinary compile's assembly and raw
object. Its `sched1` trace breaks the priority-5/weight-1 tie by instruction
order; the local allocator then measures list lifetime 8 versus cursor lifetime
6 (two references each), giving cursor priority 3333 versus list priority 2500.
This is ordinary independent setup ordering with a measured compiler effect;
no extra load, qualifier, dummy value, or register constraint is introduced.
