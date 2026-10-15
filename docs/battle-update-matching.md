# DDS1 battle update: source ownership and native callback tails

DDS1 `effBattleUpdate` at `0x00160D88`, in `src/dds1/effect/effBattle.c`,
matches all 2,044 native bytes (511 words), with the native 592-byte frame.
The complete owner passes 19/19 C functions, retaining all 18 existing peers.
This is one new matching function. The DDS2 counterpart remains a separately
tracked nonmatching draft and is not changed by this commit.

## Lineage and replay

The complete reconstruction continues Silver / Obsidian / KiB / Sable / Brass /
Evening lineage. The actual recovered donor is
[`battle.patch` at b269b5a212f78c5386965780040f86b4bb4fcba0](https://github.com/Megami-Decomps/dds-collab/blob/b269b5a212f78c5386965780040f86b4bb4fcba0/handoffs/evening-notes-20261014/battle.patch).
PiM completed the trace-backed source ownership, workspace and callback-tail
matching. This does not claim fresh authorship of the donor algorithm.

[Checkpoint001](https://github.com/Megami-Decomps/dds-collab/tree/c8e28fc73d15521f86d5706419915956199dab56/drafts/pim-battle-update-20261015/checkpoint001)
preserves exact donor replay and stages001–004.
[Checkpoint002](https://github.com/Megami-Decomps/dds-collab/tree/eca118e697cf6c0a7f462651d3722e090f377c77/drafts/pim-battle-update-20261015/checkpoint002)
preserves the complete reviewed007 reconstruction and compact005–007 causal
history. Each uses immutable source hashes and strict replay guards.

## Sufficient source properties

- The three reciprocal transports use the existing operand-owned VU primitive.
  Producer and consumer share the selected GPR; arithmetic, VF2/VF10 and matrix
  row behavior are unchanged. This is not a claim that the sanctioned older
  transport was independently proved behaviorally wrong.
- Existing vector/matrix declarations reproduce native aligned workspace homes:
  vector at `SP+0x00`; base, override, final, alternate, opposite, basis and
  output matrices at `0x10`, `0x50`, `0x90`, `0xD0`, `0x110`, `0x150`, `0x190`.
  The vector occupies 16 bytes and each matrix 64 bytes, without overlap.
- Four count arguments use the canonical unsigned parent byte at `entry+9`,
  at the original post-callback/post-model read epochs. Retained request
  acquisition occurs in the actual flags-gated request phase; the earlier
  DDS1 position query still receives the same ephemeral `entry+8` address.
- Four dynamic output stores use the narrowly audited existing legacy store
  primitive. The actual switch order reproduces all five native callback2
  tail groups: `{0,4,1}`, `{2,6}`, `{7}`, `{5,8,9}`, `{10}`. Every case selects
  the original matrix and preserves empty case 3/default behavior.
- Existing index/count declarations precede the captured frame/color scalars,
  reproducing all six scalar stack homes. Complete tint arms follow native
  fade-first fallthrough. The early direct-color load is annulled on the fade
  path, which reloads color after the fade callback.
- Initial list/unit assignments follow the frame/flags captures, with list
  before unit, and remain before every call. The same-epoch late flags read is
  consumed directly by the tint predicate instead of joining an earlier
  quantity. These properties recover native scheduling and reload registers.
- The final DDS1-only step holds the real nonvolatile `u16` eligibility flags
  once inside `kind != 0`, before the kind 3 test. Native and emitted code use
  the same single `LHU` in the ordinary branch delay slot and the same four
  mask consumers. No call, store or volatile access intervenes. Kind 0 skips
  the capture, and all reject/accept paths retain their native behavior.

These are trace-backed, independently reviewed source properties, not a claim
of unique original spelling. No artificial uses, clobbers, register variables,
compiler changes or broad permutation search were introduced.

## Four-store contract

`VU0_STORE_MATRIX_UNCLOBBERED` is used only for the four dynamic matrixOutput
stores in cases 5/8/9/10. Each writes all 64 bytes of the 16-byte-aligned
`SP+0x190..0x1CF` buffer from VF28–VF31, followed by the opaque callback2
pointer consumer. There is no C initializer, scalar readback or cached C value
in this interval, and every reuse fully overwrites it. Other stores retain
their prior forms. This exact pinned-compiler/site audit does not establish a
general memory barrier or license broader unclobbered-store conversion.

The observed top-level volatile asm flushes pure address CSE; callback A1 is
rematerialized after the stores. The existing header's address-retention
comment does not describe this specific emitted behavior. No shared macro or
header is changed here.

## Qualification

Frozen qualification pin: `c621892ad89602509f7ab17b5023c58d111c6ba9`.
Exact owner SHA-256:
`b8041a8dd2d77c651451024eaca6204cbab158b2340f8a6a14caf702f831a74e`.
The full-owner checker passes 19/19; the target jump table also passes.
Correct-title ordinary and stock `-da` compilation agree for every allocated
section, raw relocation section and function metadata record. Independent
source/native/emitted review binds the exact source, including inherited
workspace, pointer, callback and fresh-read contracts.

Current-context production qualification at
`bca74980f46379dcde1c8b58abc4a363a1a9e972` passes the untouched baseline 18/0
and candidate 19/0. Every allocated section, raw relocation section and function
metadata record agrees with frozen014, including the intervening transitive
header changes. The exact source hash above is unchanged.

The DDS1 retail image reproduces expected SHA1
`6d18898e2724bf1d145392766e8ba1e678487419`. The DDS1 development image has
zero changed payload words. Both target builds complete successfully, and an
immediate repeated build is a no-op. The tracked source change is confined to
the DDS1 owner. Publication preserves unrelated concurrent changes under an
owner/dependency guard and nonforce expected-head update; it does not wait
for unrelated CI. No game or compiler binaries accompany this source.
