# Type names carried from SMT3

<!-- rename_refs: keep -->

The third pass after [SMT3 names](smt3-names.md) for functions and
[SMT3 data names](smt3-data-names.md) for globals. This one renames structs,
unions and typedefs to the names the SMT3HD symbol recovery carries.

## How a type is identified

The function pass left both repos calling thousands of functions by the same
name. A function with one name takes one parameter list, so the type at a given
position names the same thing in each repo. Every such position is a vote.

`smt3-decomp/tools/dds_type_names.py` reads the Nocturne database's prototypes
and this repo's own declarations and definitions, pairs the functions by name,
and votes per position. The same method is what smt3-decomp used on its own
invented types, recorded in its `docs/type-recovery.md`.

A pairing is accepted only when:

1. The parameter counts match and the pointer depth at that position agrees.
2. The winning candidate has at least four votes and at least four times the
   runner-up's. Two types used in the same positions cannot be told apart this
   way, so neither is taken.
3. The SMT3 name is a type in its database, not a parameter name this parser
   mistook for one. The same guard rejects a lowercase candidate on the DDS
   side, since types here are CamelCase.
4. The two declared extents agree, where both are known. This repo asserts a
   struct's size in its header, so that check is real wherever the assertion
   exists.
5. The SMT3 name does not carry an owner, as IDA writes for a nested type:
   `sdf_PacDecode2_sdfPac2DecodeCb_t` names a field's type, not a type.
6. The name is not already taken by a different type here, and no two of this
   repo's types claim one name.
7. The name is not listed in `config/smt3_type_exclude.txt`.

## What landed

59 types in each game, the same set, since `include/` is shared. The strongest,
by agreeing positions:

| This repo | Recovered name | Votes |
|---|---|---|
| `EffWorldNode` | `dds3Basic_t` | 164 |
| `KwlnTask` | `dds3ProcessID_t` | 67 |
| `MdlCtx` | `dds3ModelHandle_t` | 49 |
| `SdfTex` | `sdfTexHandle_t` | 45 |
| `ScrData` | `scrProcessWork_s` | 43 |
| `SdfListHead` | `sdfDrawTag_t` | 40 |
| `Motion` | `sdfMotionCtrlr_t` | 32 |
| `SdfModel` | `sdfModelRootNode_t` | 32 |
| `EvtUnit` | `evtUnitHandle_t` | 31 |
| `SdfAsset` | `sdfMat_t` | 24 |

Both games built byte-identical on the first attempt. Unlike the function and
data passes, a type rename cannot tip a matched function: the name does not
survive preprocessing, and only the layout reaches the compiler.

A few recovered names are terse where ours were descriptive, `_FRQ` for
`FrFontGlyph` being the clearest case. They are kept because the premise of
these passes is that the original name wins; a reader who wants the description
has the struct's own comments.

## What this does not cover

- Member names. The recovery carries them too, and they are the larger prize,
  but a member rename touches every field access rather than a declaration, so
  it wants its own pass and its own evidence.
- Enums. Their names and values are in the corpus; nothing here reads them yet.
- The 169 types whose votes were too few, too close, or absent. The generator
  writes every candidate with its status to `type_pairs_<v>.tsv`, so the
  near-misses are a worklist rather than a loss.
- Types this repo declares per game, where a layout differs between DDS1 and
  DDS2. Nothing in this pass needed to split or merge a type.
