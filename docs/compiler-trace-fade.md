# Camera track fades: native interpolation and the allocation frontier

`btlScaleCameraTrackValues` remains nonmatching. The last qualified paired
reconstruction differs in **five of 904 words**, with unchanged **3,616-byte**
size in each game. This note explains an admitted source premise and
qualified compiler observations, not an exact implementation or new matching
credit. The function remains `INCLUDE_ASM` in production.

| Game | Address | Current owner |
|---|---|---|
| DDS1 | 0x001EBE88 | `src/dds1/game/code_001DEFE0.c` |
| DDS2 | 0x001FC5E0 | `src/dds2/game/code_001EC868.c` |

Credits: Iron, algorithm; Copper, canonical owner and interface; Slate, flags and
callback work; PiM, complete reconstruction and paired qualification; Harbor,
allocator diagnosis and negative workspace controls; Luna, importer diagnosis.
The native-operand audit and independent semantic review build on that work.

## Zero-endpoint interpolation

The sine envelopes interpolate from zero to each current output component:

```c
(output[component] - 0.0f) * ((sdfSin(angle) + 1.0f) * 0.5f) + 0.0f
```

This is the ordinary endpoint formula `(end - start) * weight + start`, with a
real zero start value. Modes 0x10 and 0x20 additionally multiply the result by
0.5 after the endpoint addition. The reconstruction retains the separate calls,
component reads and stores, and existing conditions; it does not add a clamp.

An earlier source-admission assessment rejected these expressions as dummy zero
arithmetic without tracing their native operands and consumers. That exclusion
was mistaken. In **each game**, all 16 component evaluations explicitly define
positive zero with `mtc1 $0, $f4`, subtract it from the component, multiply by the
sine weight, add it back, and store the result. F4 is not overwritten between its
definition and those uses. These are 16 live subtractions and 16 live additions,
not operations deleted by the compiler merely to leave an allocation effect.

The following DDS2 addresses map every component. Corresponding DDS1 addresses
are lower by 0x10758; the zero arithmetic instruction words are identical in the
twins.

| Mode | Component | Subtract zero | Add zero | Store result |
|---|---:|---|---|---|
| 0x04 | 0 | 0x001FC830 | 0x001FC83C | 0x001FC840 |
| 0x04 | 1 | 0x001FC8C0 | 0x001FC8CC | 0x001FC8D0 |
| 0x04 | 2 | 0x001FC950 | 0x001FC95C | 0x001FC960 |
| 0x04 | 3 | 0x001FC9D8 | 0x001FC9E8 | 0x001FC9EC |
| 0x08 | 0 | 0x001FCB10 | 0x001FCB1C | 0x001FCB20 |
| 0x08 | 1 | 0x001FCBA0 | 0x001FCBAC | 0x001FCBB0 |
| 0x08 | 2 | 0x001FCC30 | 0x001FCC3C | 0x001FCC40 |
| 0x08 | 3 | 0x001FCCB8 | 0x001FCCC8 | 0x001FCCCC |
| 0x10 | 0 | 0x001FCDF0 | 0x001FCDFC | 0x001FCE04 |
| 0x10 | 1 | 0x001FCE88 | 0x001FCE94 | 0x001FCE9C |
| 0x10 | 2 | 0x001FCF20 | 0x001FCF2C | 0x001FCF34 |
| 0x10 | 3 | 0x001FCFB0 | 0x001FCFC0 | 0x001FCFC8 |
| 0x20 | 0 | 0x001FD0F0 | 0x001FD0FC | 0x001FD104 |
| 0x20 | 1 | 0x001FD188 | 0x001FD194 | 0x001FD19C |
| 0x20 | 2 | 0x001FD220 | 0x001FD22C | 0x001FD234 |
| 0x20 | 3 | 0x001FD2B0 | 0x001FD2C0 | 0x001FD2C8 |

Matched C in `src/dds2/game/code_001B2AF8.c` corroborates this formula style:

- `btlUpdateAnalysisPanelWindowGeometry` computes sine-weighted extents from zero
  to 40 and zero to 34, retaining `+ 0.0f`. Native additions at 0x001B9F68 and
  0x001B9FEC feed actual panel geometry.
- `btlUpdatePhasePanelSecondaryAnimation` computes a sine-weighted position from
  zero to 392 in two states, also retaining `+ 0.0f`. Native additions at
  0x001BCDB8 and 0x001BCF10 feed the stored position.

Both are existing C implementations, not proposed donor drafts. A read-only
comparison of their complete 1,400-byte and 1,796-byte bodies in the built retail
image against native instructions found zero differing words. They corroborate
the endpoint style; the target's own native code proves both its subtraction and
addition. They do not prove that the target used any particular original macro.

This is not permission to invent identity arithmetic. The endpoint has an
algorithmic role, the operations are native, and their results have real
consumers. Preserve the floating-point sequence rather than substituting an
identity over real numbers or relying on untested exceptional-value behavior.
The audit does not claim a particular signed-zero or NaN result on hardware.
Original source spelling and names remain unknown; even exact bytes would not
establish source authenticity.

## Qualification boundaries

The historical DDS2 observation used PiM's complete reconstruction in its full
owner, with the existing sine-function signature and assembler flags
`-EL -G8 -g`. Its original-QEMU baseline and observed run matched all 28 archived
artifacts across **28,726 events**. Target bytes, all 48 relocation contracts and
entire literal sections also matched the production-context object. These are
historical control results, not a new exact-match qualification.

The [historical qualification receipt](compiler-evidence/dds2-fade-allocation/qualification.json)
records the compiler observation. Its five differing words are listed below;
none belongs to a floating-point interpolation chain. Thus all mapped zero
operations survived in that qualified candidate's final code. This conclusion
does not require inventing a particular intermediate folding-pass explanation.
Event identifiers and pseudo numbers are run-specific, not stable interfaces.

The paired source-audit checkpoint, rehosted under the canonical function, field
and sine-provider names, reproduces the same five words in each game. Complete
ordinary owner gates report 257 matching siblings in DDS1 and 238 in DDS2, with
only the target differing.
DDS2's full ordinary and stock-`-da` assembly, allocated sections and relocation
signatures agree. Both games have target-byte, 48-relocation-contract and literal
section parity. **DDS1 does not have full-owner ordinary/stock parity**: `-da`
perturbs unrelated `btlAdvanceSkillCameraCursor`, and the same change reproduces
in the unmodified baseline. The ordinary owner gate still keeps that sibling
exact. DDS2 is therefore the primary owner for causal compiler observation.

These results describe the frozen source-audit checkpoint, not a fresh
qualification of later owner revisions. They establish a faithful near-match
baseline, not a complete-image or gameplay qualification of the reconstructed
fade implementation. Any integration requires requalifying the complete owners.

## One allocation issue, five changed words

Offsets are relative to `btlScaleCameraTrackValues`; words use decoded numeric
instruction order. The paired source-audit checkpoint reproduces this inventory.

| Offset | Candidate | Native | Effect |
|---|---|---|---|
| +A2C | 000519C0 | 000511C0 | Delay-slot shift writes $3 instead of $2 |
| +CEC | 000519C0 | 000511C0 | Common-tail shift writes $3 instead of $2 |
| +CF0 | 00734021 | 00534021 | Address $8 consumes the changed shift register |
| +CF4 | 91020020 | 91030020 | Flags byte goes to $2 instead of $3 |
| +CF8 | 30420080 | 30620080 | Mask reads $2 instead of $3; result remains $2 |

None carries a relocation. The +A28 timing-false branch skips +CEC and targets
+CF0. Its +A2C delay slot supplies the same offset computation. The historical
`rtl.29.dbr` observation identifies a copied shift in that delay slot while the
original remains on the common path: one allocation choice affects both paths.

The historical final 0x80 gate and matching 0x10/0x20 gates make identical local
allocation decisions. Each byte has `REG_DEAD` on its mask instruction. The QI
byte and SI mask coalesce into a four-reference quantity, lifetime 8–12, and
receive $2. The two-reference shift, lifetime 4–6, receives $3. Expanded ranges
overlap: mask [6,13), shift [2,8); $2 is blocked at indices 6–7 when allocating
the shift, while $3 is available.

This rules out a special missing byte-death note or different local coalescing
eligibility at the final gate. The final index and record base die here, while
earlier gates retain them; the final record pointer also lives longer into the
cubic loop. Those genuine differences were already represented and did not
change the observed choices. Native and candidate both place the address in $8.

Native wants byte $3, mask $2 and shift $2. Recovering that allocation still
requires an ordinary source constraint and qualified compiler evidence. The
zero-endpoint audit removes an unsupported source exclusion; it does not itself
resolve this residual. Dummy uses, artificial lifetime extensions and unrelated
behavior changes remain forbidden. No new control outcome is claimed here.
