# Recycled alias storage in EE GCC's address-taken scalar expansion

## Finding

The pinned EE GCC 2.96 compiler can assign a context-dependent alias set to an
address-taken scalar before the first CSE pass. In the inspected case this is
not a source type difference: the compiler converts the scalar's existing
`REG` RTL node into a `MEM`, then obtains the new memory alias set by reading
that same node's previously unused alias storage.

The observed full-unit compilation of DDS1
`field/fldFileResolver::fldRefreshAreaStateFromPosition` gives its scalar
classifier output the alias set `0x1B00000000`. The `-DSKIP_ASM` diagnostic
compilation of the identical C gives it alias set zero. Both values are present
before optimization, and both conversion paths have been observed in the
unmodified vendor compiler.

Compiler SHA-256:

```
d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1
```

The source is based on public repository commit
`4d9c3d9c5260590e6b976b6b23da4310017a84aa`. The complete candidate SHA-256 is:

```
4bbd5d075295ef99cc15b0569d76db8e734ce63ce472f2e4ceb69f54a33e6e32
```

This candidate is **not a match**. The observation diagnoses one compile-context
failure; it is neither a source closure nor an instruction to change the
production compiler.

## Actual conversion and return path

These are addresses in the pinned i386 `cc1`, not game addresses.

1. `gen_mem_addressof` enters at `0x08093DA8`. Its existing scalar RTL node is
   `REG:SF 85`; its future alias-storage words at `+0x0C/+0x10` already contain
   low word zero, high word 27 in the full-unit run.
2. At `0x08093DFB`, the function changes that node's code from `REG` (`0x45`)
   to `MEM` (`0x4A`). It installs an `ADDRESSOF` expression at `+0x04`, referring
   to replacement scalar register 366. It has not initialized the two alias
   words.
3. At `0x08093E59`, it calls `get_alias_set` with the scalar declaration.
4. `get_alias_set`'s `VAR_DECL` path sees that declaration's now-`MEM` RTL.
   At `0x080ED0A3`, it loads the alias words from `DECL_RTL +0x10/+0x0C` into
   `EDX:EAX`. Observation confirms that this is the **same RTL node** changed
   in step 2.
5. Returning to `0x08093E5F`, `EDX:EAX` is `27:0`. Instructions there copy
   those values back into the same node's alias-storage words.

The relevant instructions are:

```asm
# gen_mem_addressof
08093DFB: movw $0x4a,(%ebx)
08093E00: mov  %esi,0x4(%ebx)
...
08093E59: push %edi
08093E5A: call get_alias_set
08093E5F: mov  %eax,0xc(%ebx)
08093E62: mov  %edx,0x10(%ebx)

# get_alias_set's existing DECL_RTL MEM path
080ED096: mov  0x54(%edi),%eax
...
080ED09D: cmpw $0x4a,(%eax)
...
080ED0A3: mov  0x10(%eax),%edx
080ED0A6: mov  0xc(%eax),%eax
```

Unlike this in-place conversion, ordinary `gen_rtx_MEM` at `0x080DCA60`
explicitly initializes both alias words to zero at `0x080DCA72` and
`0x080DCA79`. This distinction explains why a broad claim that every memory
node is uninitialized would be incorrect.

### Compact observed evidence

For the full compilation, the 20 bytes beginning at the scalar node are:

```
Before conversion: 45000f08 55000000 1a020000 00000000 1b000000
Before alias call: 4a000f80 20bfd32f 1a020000 00000000 1b000000
```

The pointer word is run-specific. Its identity, not its numeric address, is
the evidence: `get_alias_set` reads the same node. The resulting 64-bit alias
value is `0x0000001B00000000` (`115964116992`). In the `-DSKIP_ASM` run, the
same unused storage is zero before conversion and the same circular path
returns zero. The RTL dumps agree with the observed values.

No observer instruction writes these fields or changes compiler decisions.
The hooks only read registers/memory and log them.

## Effect on the actual candidate

The output scalar is a genuine `f32` passed by address to
`fldClassifyPositionInZoneWithMargin`. The provider writes one scalar. Its
contract does not establish an array or a larger aggregate.

At expansion, the full candidate has `mem/f:SF(addressof(reg366), alias
0x1B00000000)`. In the otherwise identical `-DSKIP_ASM` diagnostic it has alias
zero. Initial CSE then treats the intervening `point[]` writes differently:

- In the full run, the repeated margin reads at UIDs 1989, 2004, 2014, 2175,
  and 2187 disappear in pass `03.cse`. The artificial nonzero alias set is
  distinct from the ordinary float-array accesses.
- With alias zero, those reads survive initial CSE. After `ADDRESSOF`
  elimination and later CSE, two reads remain: UID 1989 in the support arm
  and UID 2175 in the other collision arm.
- The full target is 2984 bytes; the diagnostic target is 2992 bytes. Those
  two extra `lwc1` instructions reproduce DDS1 retail's corresponding
  stack-margin reloads at `0x00137B60` and `0x00137C64`. DDS1 retail is still
  3008 bytes, and the candidate retains other differences.

DDS2 retail lacks the corresponding two margin reloads. This observation
therefore also warns against assuming that every cross-title instruction
change represents a source-level algorithm change. It does not establish
which heap contents the original development compiler had.

## Qualification and stability

Two independently qualified, read-only QEMU RSP captures were made:

- `observed-alias-full-3`: 10 events; **28 of 28 artifacts byte-identical**
  between ordinary and observed execution.
- `observed-alias-skip`: 10 events; **28 of 28 artifacts byte-identical**.

Each qualification includes complete emitted assembly, object, all 22 RTL
pass dumps, and compiler/assembler output logs. The runner verifies compiler
and input hashes before and after, uses the same working directory,
environment and compiler arguments, and pins all hook instruction bytes.
The full observed baseline's extracted target assembly also equals the
separate ordinary `ee_gcc_probe` capture of the preferred candidate exactly.
This establishes reproducibility for the frozen inputs. It does not promise
that a changed translation-unit context will retain the same spare words.

## Reproduce or investigate another scalar

Keep the exact source and dependency closure frozen. Use the existing
`tools/ee_gcc_observe.py` QEMU-RSP runner with a locally reviewed configuration;
retain its ordinary-versus-observed artifact equality gate. No GDB installation
or compiler modification is required.

The originally captured `observe-alias.py` has SHA-256
`88e80bbcf0e9ba3774e32b007c52910dc51a27d4771108681a4b572f784be671`.

The [published portable tracer](compiler-evidence/scalar-alias/observe_alias.py) subclasses
`_ee_gcc_observer.GatedGlobalTracer` and gates `gen_mem_addressof` entry on the
current target function. Its only hooks are:

```
08093DA8 gen_mem_addressof entry: original RTL, declaration, raw 20 bytes
08093DFB before REG-to-MEM rewrite: original alias-storage words
08093E59 before get_alias_set: rewritten RTL and unchanged alias storage
080ED0A3 get_alias_set reads existing DECL_RTL: node identity and alias words
08093E5F after get_alias_set: returned EDX:EAX
08093E65 after alias writes: final node storage
08075764 rest_of_compilation: finish the expansion observation
```

Use a separate frozen configuration adding only `-DSKIP_ASM` for the context
control. That flag is a diagnostic already used by `check_unit`, not a
proposed production setting. Verify both configurations independently:

```sh
python docs/compiler-evidence/scalar-alias/observe_alias.py --config observer.json --output observed-full
python docs/compiler-evidence/scalar-alias/observe_alias.py --config observer-skip.json --output observed-skip
```

The [compact evidence bundle](compiler-evidence/scalar-alias/README.md) includes
the portable tracer, complete observed events and both 28-artifact
qualification/hash reports. The full nonmatching candidate is not included
in this documentation batch; its hash pins the capture, but this bundle is
not a standalone source-level reproducer of that exact translation unit.
A machine-local configuration must be regenerated with the recipient's verified
paths and hashes rather than copied blindly.

## Limits and source guidance

- Do not infer that two game objects really alias, or that a source type is
  incorrect, from these context-dependent numeric alias values.
- Do not add padding, dummy declarations, heap perturbations, arbitrary
  identifier changes, compiler flags, `volatile`, invented unions or
  one-element arrays to obtain a desired spare value.
- A naturally supported aggregate or changed value lifetime may avoid this
  conversion, but needs independent producer/consumer and native evidence.
  The scalar classifier output alone supplies no such evidence.
- The native/diagnostic reload agreement diagnoses this compiler mechanism.
  Only ordinary source under the unchanged compiler, complete-owner checks
  and the linked retail-image gates can establish a match.

Recovery and observation: KiB, with independent semantic and compiler review.

## Address-taken integer stage: separate current-context observation

A later DDS2 observation extends the *observed RTL symptom* to an address-taken
integer. It does not repeat the live REG-to-MEM conversion observation above.
The compiler remains the same pinned cc1. The target is
`game/code_00136EF8::fldStartDeferredActorTransition` at `0x00141CF0`.

At repository commit `143ec511aac362e16c72eb5f47381ad20e93ab85`, the owner has SHA-256
`e78e7ac1c923b4810d478d21725d792de70ef91fea18a1c6f3b9a785e620526c`.
The experimental complete owner, replacing only the preceding room geometry
binder's INCLUDE_ASM with recovered C, has SHA-256
`7eeb765403b34b8e44a5d08e7d240a82207905bb56dcab96f68c9b64254fce2d`.
No source change was made to the transition function. The experimental binder
is not a match and is not included in this compiler-note evidence.

Four ordinary compilations distinguish a baseline issue from the experiment:

- Current baseline full and SKIP_ASM: identical transition instructions, 616B.
- Binder-added SKIP_ASM: same instructions, 616B.
- Binder-added full: four differing instruction words after relocation normalization, still 616B.

Fresh full and SKIP_ASM `-da` captures produce transition assembly identical to
the corresponding ordinary captures after local-label-number normalization.
The resulting first executable difference is in `03.cse`, not merely a late
conditional-move preference.

The source quantity is a genuine `s32 stage`. An earlier branch passes `&stage`
to `dds3AdminSubmitModeRequest(0x1C, &stage, 4, 0)`. The affected later branch is:

```c
stage = command;
if (entry->variant != 0) {
    stage = entry->variant;
}
room = entry->warpEntry;
if (fldAreaState.area != stage) {
    fldReleaseCurrentBgm();
}
```

Before CSE, `stage` is `mem/f:SI(addressof(reg177), alias)`:

- Full alias: `3447816126786961408`.
- SKIP_ASM alias: zero.

Both values already appear in expansion (`00.rtl`) and persist through
`02.jump`. Both contexts then contain the same stage-selection operations:

1. UID374 stores command pseudo84 to stage.
2. UID377 reloads `entry->variant`, a signed halfword at offset `0x32`, alias17.
3. UID379 and UID381 sign-extend that reload into pseudo183.
4. UID382 tests pseudo183 against zero.
5. On the assignment arm, UIDs387/389/391 perform a second variant load and
   sign-extension into pseudo185; UID393 stores pseudo185 to stage.

There is no intervening provider call in this sequence. Initial CSE in the
full context removes UIDs377/379/381, changes UID382 to use the prior variant
pseudo172, and stores pseudo172 in UID393. Both contexts also fold the second
load/sign-extension at UIDs387/389/391: full reuses pseudo172, while SKIP_ASM
reuses pseudo183 for UID393. The prior variant pseudo172 was loaded and
sign-extended by UIDs328/330/332 before UID374. SKIP_ASM retains the reload and pseudo183 across the alias-zero stage store,
consistent with conservative alias invalidation. The address,
mode, and alias17 of the variant load are the same in both contexts.

This establishes the first alias-sensitive optimization divergence. It is
consistent with the previously observed recycled alias-storage mechanism;
the integer conversion itself has **not** yet been dynamically observed here.
There is no evidence of new unit-visible provider knowledge or a legitimate
source-type change. This is not evidence that the source's stack scalar and
actor field actually alias, and no compiler memory was patched to test a
counterfactual.

The downstream four-word, relocation-normalized difference relative to the function start is:

```
+0x170  move v0,s6       -> move v0,a0
+0x1A4  movn v0,a0,a0    -> movz v0,s6,a0
+0x1A8  sw v0,164(sp)    -> lh s1,52(s0)
+0x1B0  lh s1,52(s0)     -> sw v0,164(sp)
```

The raw full/SKIP object bytes also differ at +0x34 and +0x1BC: these words carry
`R_MIPS_26 .text` addends for unchanged local call targets. Those two relocation
addend differences are excluded from the four instruction differences above.

The command-default MOVN and variant-default MOVZ forms implement the same
conditional selection, but differ from each other and therefore cannot pass
the same retail instruction comparison. The subsequent spill/load scheduling
also differs. No source workaround, neighbor edit, checker masking, or
production flag was applied.

### Actual capture commands

Working directory: `/workspace/scratch/a4cc6a43d59f/dds-direct-recovery`.
`W=/workspace/scratch/a4cc6a43d59f/dds-recovered-inputs/room-binder-recovery-oct14`.
For each `mode=full` and `mode=skip`, `flags` is empty or `-DSKIP_ASM`, respectively:

```sh
TMPDIR="$W/transition-rtl-$mode-tmp" \
DDS_VERSION=dds2 DDS_AS_UNIT=src/dds2/game/code_00136EF8.c \
DDS_KEEP_S="$W/transition-rtl-$mode.s" \
DDS_I386_LIBDIR=/workspace/scratch/a4cc6a43d59f/dds-host-runtime \
tools/cc.sh $flags -da -dumpbase "$W/transition-rtl-$mode" \
  "$W/candidate-preferred-dds2.c" -o "$W/transition-rtl-$mode.o" \
  > "$W/transition-rtl-$mode.log" 2>&1
```

Ordinary captures used the same wrapper and source, without `-da -dumpbase`,
with separate `transition-candidate-$mode` actual assembly/object/log paths.
Baseline captures substituted `src/dds2/game/code_00136EF8.c` and used separate
`transition-baseline-$mode` output paths. Each TMPDIR existed before invocation.
The wrapper retained the original unit's filename lengths and standard flags.

The compact evidence consists of stage RTL excerpts, the four-word delta, and
hash manifest. Complete raw dumps and nonmatching source remain local for
review, not part of this proposed documentation addition.

Compact captured evidence: [integer-stage-alias](compiler-evidence/integer-stage-alias/manifest.json). The manifest identifies the unchanged published base and input hashes; it does not make the omitted experimental source independently reproducible.
