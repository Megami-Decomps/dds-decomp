# DDS2 mantra-category availability

`mnuRebuildMantraCategoryAvailability` (0x00315A50, 420 code bytes) clears the
17 active category flags, enables the initial model-state categories, then
visits occupied party slots and profiles 1..175. A next-category neighbor
unlocks that category when the party member owns the current profile. All six
neighbors are visited; the ownership test uses the current profile, not the
neighbor's ID.

The reconstruction preserves Halcyon Drift's source, Cedar Spark/Luna's
zero-based model-state/next-category relationship, and TrackEdit/KiB/H2-2's
later traversal and ABI investigations.

The countdown roster still visits `party[0]` through `party[4]`; indexed
neighbors preserve the complete native array traversal. On the current owner,
this ordinary form leaves only two initial-byte-store register differences.
Correcting the old false-void memset declaration to its real pointer-returning
SDK contract closes those words. The same ABI correction moves four words in
`prfBuildRawSkillList`; reading its eight skills directly from the actual
profile-array owner restores that peer without changing its compaction.

A qualified stock-compiler control establishes the loop-index lifetime. The
single `index` is pseudo 84 in `00.rtl`, used first for the category countdown
and then for the profile scan. `20.greg` assigns it register 17, matching both
native scans. Splitting it into two genuine indices assigns the category
pseudo 84 register 3 and the profile pseudo 85 register 20; the body grows from 420
to 428 bytes. This is reuse of a same-kind loop index, not unrelated values or
a dummy use. The actual memset result is represented as a set of v0 in expansion;
the independent byte-one pseudo 93 is allocated v1, matching retail.

The final complete owner gates at 98 match / 0 differ. Ordinary ASM-inclusive
compilation and `ee_gcc_probe.py --strict-assemble` agree in complete normalized
assembly and every non-debug ELF section; the split-index control has the same
parity. The local receipt retains 22 stock RTL passes, 23 extracted target
artifacts, complete candidates and source hashes. No compiler, flag, header,
owner representation, dummy value or qualifier was changed.

The combined local integration preserves both retail SHA-1 checks, both
development payloads, and all 415 tooling tests.
