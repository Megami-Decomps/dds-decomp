# DDS2 camera interpreter matching

Active target: `func_001FA480`, native5256B. Current work follows grounded source contracts and source-linked compiler instrumentation.

- `current-body.c` and `candidate.patch`: current primary-owner candidate, including the two native-proven8-byte peer-status reads.
- `historical-candidate.patch`: preserved historical5248B/600-difference baseline.
- `STATUS-OWNER.md`: exact source correction and evidence.
- `diagnose.py`: complete owning-unit comparison, older-function preservation and source restoration. A nonmatch intentionally exits1.
- `peer_probe.py`: bounded private-native semantics using synthetic inputs, aggregate output only.
- `trace.py`, `roles.json`, and additive compiler tools: parity-gated source-role and CSE instrumentation under development.

Diagnostic branches do not replace canonical game source. Ordinary Build on these branches tests unchanged canonical code. Only the focused diagnostic temporarily installs the complete candidate. No full-retail or runtime qualification is claimed for a nonmatch.

Credit Basalt, Obsidian and PiM for the reconstruction and prior compiler work; KiB / redthing1's dot continues matching.
