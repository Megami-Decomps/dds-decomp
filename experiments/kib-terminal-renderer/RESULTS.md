# Current-context replay result

Run [37911793915](https://github.com/Megami-Decomps/dds-decomp/actions/runs/37911793915), experiment ee0a3b9baeb6d722f100183e5a753b6cc1b0fafe on canonical base e2566fe9b5c1b9d8cff1c5181697ba6ecd2a5ebb, reproduces the retained terminal-renderer frontier:

- Baseline: 41 C functions exact, zero issues.
- Candidate: 1384 bytes versus native 1396 executable bytes, 60 differing words.
- All 41 older C functions remain exact and retain ordinary object sizes.
- One target-associated data issue is reported, consistent with the previously documented shifted jump table; this run did not further classify its entries.
- Zero context or other issues; original source restored.

The retained source was replayed without variants. No improvement or new matching code is claimed. No full-retail/development/runtime test of the candidate was performed. The independent Build workflow compiles the unchanged canonical source, because the candidate is installed only inside the separate diagnostic job; its outcome must not be credited as candidate validation.

Source credit: Spark reconstruction, Slate preserved handoff. Prior CSE/scheduler/delay-slot negative controls remain valid. The unchanged result provides no new justification for repeating them.
