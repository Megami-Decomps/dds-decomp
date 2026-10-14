# Frozen nonmatching compiler diagnostic

These files diagnose a vendor compiler bug. They are not production source and
must not be applied to main as a matching-source patch.

- The frozen nonmatching candidate is not distributed in this documentation
  batch. Its SHA-256 is
  `4bbd5d075295ef99cc15b0569d76db8e734ce63ce472f2e4ceb69f54a33e6e32`.
  The observations below diagnose the mechanism; this is not a standalone
  source-level reproducer of the exact captured translation unit.
- `full-events.jsonl` and `skip-events.jsonl` contain the complete ten-event
  read-only observations. Corresponding equivalence files retain all 28
  ordinary/observed artifact hashes and equality checks.
- Run `observe_alias.py` from the repository root. It uses the existing
  `tools/ee_gcc_observe.py` runner; supply a locally regenerated, reviewed
  configuration with actual tool paths and a frozen input/dependency closure.
  See `docs/compiler-live-observer.md` and the accompanying alias note.
- The published tracer changes only module discovery/formatting from the
  originally captured script. Hook addresses/bytes and recorded operations
  are unchanged. It never writes compiler data or changes compiler flags.

The scalar's spare memory contents depend on translation-unit context. A
changed input or dependency set is not a reproduction of the frozen case.
