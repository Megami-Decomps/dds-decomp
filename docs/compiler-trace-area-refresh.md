# Area-position refresh: resolved-vector phases and contact-store allocation

DDS2 `fldRefreshAreaStateFromPosition` (`0x00139EC0`, 3,000 bytes) is reconstructed as C with two narrowly scoped local register bindings. There is **no empty inline asm** in these new constraints. This is allocation reconstruction, not a claim about the original source spelling or a hardware requirement.

## Ordinary source reconstruction

The moving-support path accesses the actual indexed collision record directly. There is no intervening call or write to the table, index, or record between the replaced pointer uses. Repeated-address CSE then produces the native offset-plus-base operand order; no byte-pointer arithmetic or false record layout is used.

Carried coordinates are committed to the private position vector in X, Z, Y order. A qualified live allocator trace of the earlier candidate showed Y's seven references over a 58-instruction lifetime narrowly outranking X's five references over 42 instructions. Committing the real adjusted height last supplies the native lifetime and restores the native floating register allocation. No extra value use or storage was added.

The three real area-state resets occur before axis computation, with the source color reset preceding the encounter reset. The live scheduler trace showed that the final consumer of the area-base address receives `reg_weight = -1`; equal-weight choices then use source LUID order. This explains why merely spelling native instruction order is insufficient. These are independent nonvolatile fields; the intervening absolute-value operations have no memory effects.

Each solid branch captures `resolvedClearance` **after** its actual floor/wall clamp. That one resolved distance is used to reconstruct all three coordinates and, for floors, test the margin. The separately required original contact value is preserved. In the prior qualified RTL, the first multiplication already used promoted clearance pseudo 1137, but the later multiplication reloaded the addressable output after the position-X store. Capturing the resolved geometric quantity avoids that reload without fake aliasing, qualifiers, or a synthetic lifetime. The query's independent return status remains ignored: classification still uses its output clearance, including the original NaN-sensitive comparisons.

The movement threshold has native bits `0x3DCCCCCC` (approximately `0.09999999404`). The source spells it `0.09999999404f`; pinned cc1 also emits those same bits for `0.1f` because its decimal float conversion truncates. This spelling is not a semantic correction or evidence of the original source literal. The existing documented native snapshot-store oddities and final unused length calculation are preserved, not introduced by this change. DDS1 has a different layout and is qualified separately; this note does not claim identical twin behavior.

## Two real contact-store values

At each actual solid-contact store:

```c
{
    register u32 contactActive __asm__("$2") = 1;
    fldMatterTable[index].hitflag = contactActive;
}
```

The initialized word value is exactly the real contact flag. The destination's `u16` conversion is truthful and lossless for 1. Each binding starts at its actual definition, has only the immediate halfword-store consumer, and ends there. No call crosses either lifetime; there is no dummy consumer, global binding, invented clobber, extra memory effect, or artificially extended lifetime.

Native definitions at `0x0013A804` and `0x0013A8E8` load 1 into `$2`; the corresponding real stores at `0x0013A810` and `0x0013A8F4` write it to collision-record `+0xCA`.

In the qualified exact RTL, local allocation retains SI definitions UID 2722 and UID 2988 and their HI stores UID 2741 and UID 3007. Both stores have `REG_DEAD` for `$2`; global allocation preserves the same real uses. In the qualified ordinary unbound control, those stores instead consume `$19` (the loop's live 1) and `$7` (the current classifier result), respectively. Their values happen to equal 1 on those paths, but the native independent literal definitions disappear. The narrow bindings prevent that substitution.

Ablations from the exact candidate:

| Control | Emitted bytes | Differing words |
| --- | ---: | ---: |
| Both bindings retained | 3000 | 0 |
| Floor binding removed | 2992 | 195 |
| Wall binding removed | 3000 | 45 |
| Both removed | 2992 | 195 |

Counts include shifted instruction/branch layouts, not just independent operand changes. Earlier empty-tied-operand experiments are not in the final source; using a uniform real word-sized flag in each branch removed their need.

## Qualification and review

The exact candidate and the unbound causal control were each compiled normally and with `-da`. Each pair agrees in every allocated section (`.text`, `.data`, `.bss`, `.lit4`, `.sdata`), all 404 allocated-section relocation contracts, and assembly after removing debug-path directives. The full affected owner reports 27 matching functions and zero differing functions.

The earlier diagnostic live compiler observation recorded 26,330 events and 28 byte-equal artifacts. Its complete allocated sections and 404 relocation contracts independently equal its ordinary full-context compile. That observation explains the carried-coordinate and reset decisions; the final exact-source RTL, rather than historical diagnostic UIDs, establishes the contact bindings.

An independent semantic reviewer checked direct-record accesses, private coordinate ordering, distinct reset stores, post-clamp snapshots, NaN behavior, and both initialized immediate contact-store bindings against native instructions and CONTRIBUTING's localized-constraint conditions. They requested that the old header not imply that DDS1 is an already qualified identical twin; the landing comment now states that DDS1 is qualified separately. Integration gates passed: the complete affected owner and owned data remain exact (27 matches, zero differences); both games' retail and development builds pass, retail SHA1s match the originals, all 442 tests in 117 batches pass, and all 84 retained-C units compile without failure.
