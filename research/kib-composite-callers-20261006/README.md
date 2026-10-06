# Composite slot draw callers: preserved nonmatch

KiB, 2026-10-06. Research only. The two production functions remain ASM.

- DDS1 src/dds1/game/code_0029C530.c::func_002AF6F8
- DDS2 src/dds2/game/code_002DE248.c::func_002F2AE8
- Native size: 1,556 bytes / 389 words each
- Candidate: 388 words, 163 positional word differences in each title
- Complete canonical units: 520 match / 1 differ and 657 match / 1 differ

The snippets replace only the named INCLUDE_ASM and include owner-types.h. That file copies complete existing menu-owner types, refines FileKeyBlock's real +0x54 blend mode, and includes the separately landed complete composite descriptor. It is a research input, not permission to install duplicate production owners. Coherent shared-header reuse and provider validation are prerequisites for integration.

The containing source baseline is 49e91a2c840a0fb577879348655efa6ac3ed919b. Checks used current headers from that source and the pinned cc1 SHA256 d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1, normal canonical paths/flags. Run tools/check_unit.py UNIT --source FULL_CANDIDATE; the files here are insertion snippets.

Independent native/source review found no missing semantic operation. The non-NOP opcode-class counts agree exactly; the one-word gap is an alignment NOP associated with the transformed input address. Native forms that address before the loop; the candidate forms it inside.

A qualified normal-versus-diagnostic RTL trace identifies 09.loop rejecting the invariant address (life 1, savings 1, not desirable). It is not reload rematerialization. An explicit real pointer can hoist it, but creates a ten-node global interference clique across shared branch-counter roles and rematerializes baseColor, whereas the native code needs nine saved roles. Natural countdown, guarded-loop and separate-counter probes did not recover native allocation. No artificial lifetime, dummy use, added clobber, register pinning, or flag change was accepted.

Important semantics retained: initial count/slots/key snapshots; later record reloads after calls; inactive-slot cursor advancement; old-UV sampling before state advance; u16 texture periods; direct-path unsigned conversion versus transformed-path signed conversion; real branch-specific color scratch objects. Remaining differences include GPR roles and FP scheduling beyond the missing alignment NOP.

No retail binaries or generated assembly are included, and no match or retail-build success is claimed for these candidates.
