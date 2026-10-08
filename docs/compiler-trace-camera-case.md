# Camera matching through live compiler decisions

The 2,212-byte DDS1 camera function `func_001ECCA8` reached an exact match after
two source changes supported by native lifetimes and live compiler decisions.
The first recovered a real call-argument preference; the second preserved a
meaningful distinction between dimensionless progress and physical distance.
Neither changed the compiler, pinned a register, or replaced scalar code with
assembly.

This case extends the [compiler diagnostics](compiler-diagnostics.md) and
[decision atlas](compiler-decision-atlas.md). Existing allocation summaries show
surviving preferences and final homes. Live hooks establish where a preference
came from, what pruning removed, which candidates were rejected, and when an
operand order changed.

## Keep the proof contexts separate

| Context | Result |
| --- | --- |
| Preserved diagnostic source | 577 functions matched, 0 differed, including 576 older matches |
| Current-source prerequisite | 580 matched, 0 differed |
| Current-source body | 581 matched, 0 differed; data clean |

The current-source prerequisite is
[`727bb644`](https://github.com/Megami-Decomps/dds-decomp/commit/727bb644ed44059db6ba41e3f9342a88f799df3a).
The body is
[`09a02ce4`](https://github.com/Megami-Decomps/dds-decomp/commit/09a02ce4e6c0e25721b9d093c600aadf773b7de6).
It reuses the `exponentialRange` and `quadraticRange` owners already landed in
[`42567f73`](https://github.com/Megami-Decomps/dds-decomp/commit/42567f73b267e5ce89e8facb7dbaa4812df08cf9),
rather than duplicating the earlier proposed owner types. Complete-file
readback verified source blob `3f2b53cb26fa330e2628a9ce09507be9070a727a`.

The observer is bound to compiler SHA-256
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.
The hook map was derived from the executable's symbols and STABS layouts;
the observer reads live state using those pinned layouts. The exact vendor C
source is unavailable. Addresses and field layouts must not be reused
with a different compiler image.

The allocator and early-expansion traces each reproduced all 51 compiler
artifacts of their corresponding same-path ordinary compilation byte for byte.
Candidate runs additionally retained an identical source snapshot. This parity
includes the whole-unit assembly/object and every captured RTL pass; an artifact
inventory mismatch is a failure, not an omitted comparison.

These checks validate observation. They do not replace `check_unit`, the normal
whole-unit/context/data gates, or the project's retail-identity build checks.
No hosted-CI claim is made by these local receipts.

## Start with a semantically qualified frontier

The starting near-match already had the native size and frame, the complete
opcode-family sequence, and all 121 VU arithmetic instructions in order. Its 16
remaining words involved scalar floating registers, including two multiply
operand orientations.

That frontier used an existing historical SDK VU-store idiom. It required an
independent audit of scalar readback, call boundaries, vector stores, and loop
backedges before using it as the starting point. In particular, scalar reads
after VU stores and output-vector replacements retained the required native
order. This case does **not** make weaker memory clobbers a matching recipe.
Changing an asm contract requires its own semantic evidence; allocation
tracing cannot justify an incorrect memory effect.

## A real preference was missing from the scalar web

The original candidate's `blend` value was global pseudo90 in the preserved
trace. It had 42 references, live length43, hard-register width1, and priority
48837. It was allocated first.

The live preference hooks observed:

1. A return-value copy introduced a preference for f0.
2. A comparison introduced a preference for fcc0.
3. Pruning removed f0 because of interference and fcc0 because it was outside
   the floating-register class.
4. The actual search rejected f0 and f1 as occupied, then accepted f2.

The used-register mask did not exclude f12, but the scalar had no surviving
preference for it. Separate pseudos for the initial factor *did* have genuine
f12 call-argument copy preferences and were assigned f12.

Thus the final f2/f12 difference was not evidence of a mysterious register
choice or an arbitrary tie. The relevant source question was whether the
initial factor and later step result represented one reusable scalar role.

Native code supported that question. In modes10–12, the initial factor loaded
from `value[0]` entered f12, was stored to the real motion parameter, and was
used by an optional initializer. The step result later overwrote f12. Mode13
had the same lifetime shape. The initial value did not need to survive that
later result.

### Source change: preserve the real argument relationship

At the two relevant sites, the source captured `value[0]` in the existing
factor local, used it for the motion-parameter store and optional initializer,
and then overwrote it with the existing step result. This added no operation,
unused copy, fixed-register declaration, or changed mathematical order.

The ordinary compiler reduced the residual from **16 words to 2**, retaining
2,212 bytes and all older matches.

The second trace verified the predicted cause:

- The real p90-to-f12 argument copy introduced preference44.
- Pruning reduced preferences `{32,44,67}` to `{44}`.
- The allocator selected the surviving copy preference and committed f12.

The preferred register arose from an actual ABI argument, not a diagnostic
write or an invented use. A similar-looking source capture without that real
call relationship would not have the same justification.

## The two remaining words came from an earlier decision

The remaining words were the operand order of two scalar multiplies. The
source already wrote physical length times dimensionless progress, but the
compiler reversed those inputs during expansion.

The live `expand_binop` entry was:

| Role | RTL identity in this trace |
| --- | --- |
| Operand0: physical length | p89 |
| Operand1: dimensionless progress | p90 |
| Destination | p90 |

The destination was exactly the same RTL object as operand1. The pinned
compiler took its commutative-operand swap at `0x080c38a2`, putting that
operand first. Both swaps occurred before pass00, long before hard-register
allocation.

This explains why merely reversing the expression's written order or changing
its final register was the wrong next step. The meaningful input was the
expression's destination identity.

### Source change: give physical distance its own result

The follow-up retained the real factor relationship and made the product's
physical-distance result distinct from dimensionless progress at those two
sites. It kept the order `length * progress`, the existing negation and
branches, and all stores and calls. It did not change the unrelated mode8
calculation.

The final trace showed destination p91, distinct from both operands. Neither
multiply took the swap path; length stayed first.

The later allocator still reused the right physical register naturally:

- Both inputs died at the product sites in RTL.
- Preference expansion gave the physical-distance result normal/full
  preferences derived from the related dying value, without turning the
  arithmetic into a copy preference.
- Pruning retained f12 for the result.
- The physical-distance result was allocated first, at priority42000, and
  chose f12 through that normal preference.
- Progress later chose f12 through its copy preference. Their lifetimes did
  not interfere, so both values could share one physical register.

Distinct *early expression identities* and shared *later physical storage*
are compatible. The source need not pretend that dimensionless progress and
world distance are the same value just because retail reuses f12.

The ordinary source result matched exactly, and the current-source transfer
passed the separate 581/0 proof above. This is a demonstrated reconstruction,
not proof of a unique original source spelling.

## Local coalescing needs a different unit of analysis

A separate live diagnostic exposed a complementary pitfall. Individual
pseudos with two references each appeared tied, but local allocation had
combined a dying load/AND/OR chain into one quantity. That quantity had six
references and lifetime16, while the competing literal quantity had two
references and lifetime8. The actual scores were7500 versus2500, and the
comparator returned-5000.

Inspect the *coalesced quantity*, not only each pseudo's printed reference
count. Capture `combine_regs` acceptance, death-note and register-class checks,
quantity members, and the later hard-register exclusion mask. This is distinct
from global preference creation and pruning.

Do not manufacture a lifetime, use, or copy to change either score. If no real
source role changes the observed input, record that limit and stop the source
hypothesis.

## A bounded workflow

1. Qualify behavior, types, ABI, and memory effects first.
2. Tie the first residual instruction to its defining/using RTL pseudos.
3. Determine whether local or global allocation decides the home. Use the
   quantity for local allocation and real preference origins for global
   allocation.
4. For operand orientation, compare expansion inputs and destination identity
   before blaming late scheduling.
5. Freeze paths, argv, environment, source, headers, and compiler. Require
   ordinary-versus-observed output equality.
6. Test one source fact that predicts a specific changed decision.
7. Check the original whole unit and independently transfer to the current
   source before publishing.

The tools must not write compiler registers or memory to select a desired
answer. Output-altering diagnostics, if performed separately, are ineligible
for matching regardless of how many native words they resemble. Only observational traces with ordinary-output parity support the conclusions
here.
