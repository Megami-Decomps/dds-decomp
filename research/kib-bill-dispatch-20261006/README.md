# Billboard dispatcher nonmatches

These are preserved C candidates, not matched implementations. Production keeps both INCLUDE_ASM bodies.

- DDS1: src/dds1/effect/billManager.c::func_001515E8
- DDS2: src/dds2/effect/billManager.c::func_001591D8
- Each native body is 696 bytes / 174 words; each candidate differs in 24 words
- All 22 existing C bodies remain exact in each complete unit

Context: Megami-Decomps/dds-decomp commit 7c091b095334ec939c6b8c792c9b5e8ef7e4dead. Replace only the corresponding INCLUDE_ASM with the body in dds1.c or dds2.c. The canonical owners and typed evaluator/renderers are already defined in that context. Then run tools/check_unit.py on the full unit; a retail build is not expected to pass with this candidate installed.

The native mode/loop semantics are recovered, including delayed plural entries, temporary child X/Y, single and paired draws, and restoration of instance color/length. Separate plural saved values preserve native cleanup but choose different saved-register homes. A shared saved-state diagnostic recovers those homes but merges plural cleanup with the ordinary final store, shortening the function to 172 words and changing the zero-entry exit. An explicit early return does not prevent it; a guarded top-tested loop adds an unwanted initial test. Naming the actual animation-entry pointer fixes the separate address-expression difference. The now-exact evaluator and correct distinct record formats leave the same 24-word dispatcher frontier.

No flags, fake volatile/barriers, pinned registers, dummy uses, or source-order search were adopted. The next useful step needs a supported value/control-flow distinction or a precise compiler-pass explanation for the independent cleanup and shared register homes.
