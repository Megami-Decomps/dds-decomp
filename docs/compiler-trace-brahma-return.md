# Return-tail merging: an observed negative boundary

DDS2 `func_00223350` remains `INCLUDE_ASM`. Its qualified 2,180-byte camera
candidate differs from retail in one branch destination, not in its result or
side effects. This case records why another equivalent success label is not,
by itself, a source correction. It does not claim a match or authorize dead
state, extra calls, compiler flags, or an assembly branch patch.

The method complements the [decision atlas](compiler-decision-atlas.md) and
[live observer guidance](compiler-live-observer.md). In particular, inspect the
actual candidate's deciding pass rather than substituting an observation of a
similar earlier variant.

## Source and qualification boundary

The source is PiM/red thing's
[checkpoint012](https://github.com/Megami-Decomps/dds-collab/tree/bf237ff8b49ad733fb0318ab65321b013b6d6bbe/drafts/pim-dds2-brahma-camera-20261013/checkpoint012),
continuing Basalt's reconstruction and Slate's admission work. All eleven packet
files were checked against its manifest. Current-source admission at
`395a394efd078f92140cf69d19b0d54d1ce4c13c` changed only the already-renamed
`func_001EFEE8` identifier to `btlBuildRandomizedActionCameraPosePair` in the
prototype and two calls. Fourteen retained data fragments were independently
recovered from the local retail split and matched the packet's hashes.

The candidate uses existing `BtlLinkedCommand`, `ActionStateLink`, and `BtlUnit`
owners, the real signed selector interpretations, and finite camera-provider
interfaces. It retains all eleven literal camera presets, native publications,
and the otherwise unused-result `btlGetRuntime` call. No production definition
or shared header was changed.

Qualification of that exact source established:

- Whole-owner checker: 264 matched C definitions and one differing target.
- Native-address diagnostic unit link: all 277 neighboring bodies and all 278
  function starts/extents exact, including definitions retained in assembly.
- Complete owner `.text`: 76,092 bytes, one differing word at owner offset
  `0xF01C`; resolved SHA-256
  `af59d13d02b624769e44c9a9bd6d2667b334d877f5e15ff87b1f4757caa6b1bf`.
- All owned `.rodata`, `.lit4`, and `.sdata` bytes exact, as are all 49 direct
  target call destinations and their order.
- Target size 2,180 bytes, frame `0x70`, one difference among 545 words.

The diagnostic link is not a whole-game build or hosted-CI claim. These counts
belong to the stated source pin, not an arbitrarily newer translation unit.

The differing word is at target offset `0x614`:

| Source | Word | Destination |
| --- | --- | --- |
| Retail | `0x1000FFB6` | group-camera success at `0x00223840` |
| Candidate | `0x10000092` | final success at `0x00223BB0` |

Both destinations return one. Equivalence does not count as byte matching.

## Observation must preserve the compilation

This run used case-specific late-jump hooks layered on the public read-only
observer runner. Those hooks and the complete trace are retained locally, not
included in the linked checkpoint packet or exposed as a public observer mode.
The packet reproduces the ordinary source candidate; reproducing this live
observation additionally requires that case-specific instrumentation and locally
supplied compiler/retail inputs. This note is not a self-contained live-replay
package.

The live run used the unmodified pinned compiler with SHA-256
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.
Hooks and layouts came from that executable's actual symbols and disassembly;
the original instruction bytes were checked at first use. They are not portable
addresses for another compiler build.

The observer froze 460 source/header/runtime/generated-input files and produced
1,724 gated events. Every one of the 28 declared baseline/observed artifacts,
including assembly, object, RTL dumps, and logs, was byte-identical. A separate
ordinary `tools/cc.sh` compilation agreed with both objects in all six allocated
sections and all four raw relocation sections. Filename/debug metadata from
independently named ordinary output was excluded only from that latter object
comparison; it was not excluded from same-path live-observation parity.

This validates the observation, not the candidate as a match. The analysis
below concerns this actual checkpoint012 run; earlier control variants are not
interchangeable evidence. Event and RTL UID numbers are specific to this run.

## First accepted tail, then jump threading

The deciding events occur in late jump pass 6, with cross-jumping and no-op move
handling enabled after reload.

1. Events 361–367 compare the group return jump UID1177 with epilogue label
   UID2038, using minimum match length one. The accepted tail is UID1176 versus
   final-success UID2030. At the predecessor-chain gate, an existing match is
   already present, so execution goes directly to the accepted-match block and
   `do_cross_jump`. This decision does not rank that success tail against a
   longer predecessor tail.
2. Events 452–458 similarly merge the single-resource branch's common success
   return, UID1529/UID1528, into final-success UID2030.
3. At event 898, `follow_jumps` redirects the actual publication edge UID1492
   from its previous join UID1525 to final-success label UID2027. Its preceding
   real flags store UID1490 is unchanged. This is the branch at offset `0x614`.

The compiler-created `USE(const0)` marker is also visible at genuine
call-to-label boundaries in this source, for example UID2087 before join
UID1148. Its presence does not justify introducing a source-side dummy use or
barrier. A proposed correction has to explain a real value or control-flow
boundary before the deciding pass.

The useful causal distinction is therefore not “the candidate names the wrong
success label.” That label is subsequently threaded away after the first
accepted tail merge. For this observed mechanism, the next supported hypothesis would need a
genuine pre-jump distinction that changes the merge or threading decision while
preserving the native publications and remaining instruction graph. This trace
does not establish a unique source correction.

## One direct-completion correction was rejected

The observed intermediate success join motivated one ordinary experiment:
return one immediately after the single-resource publication, and let the
alternate helper-call path reach its own return. It removes a real source join
without adding data, calls, predicates, or dummy state.

It did not solve the problem. The target became 2,164 bytes with 134 differing
words among 541 common words; its first difference moved to `0x34`. The 264
neighboring C definitions still matched. The original checkpoint was retained,
and no further return/label enumeration was inferred from this negative.

The [qualification and release](https://github.com/Megami-Decomps/dds-collab/issues/16#issuecomment-6152310903)
therefore leaves production assembly intact. Reopen this frontier when there is
new source/representation evidence for a pre-jump distinction, not merely a
smaller textual edit or another semantically equivalent return.

Credits: Basalt, Slate, PiM/red thing, and KiB. The result of this continuation is a
qualified causal boundary, with zero new matching credit.
