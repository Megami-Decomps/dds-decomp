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
