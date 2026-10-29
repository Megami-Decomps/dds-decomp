# Battle action sequence flag allocation

## DDS1 skill action scheduler

`func_001CB410` in `src/dds1/game/code_001C8890.c` reconstructs the
6,400-byte retail function at `0x001CB410`. Its retained implementation is
promoted without changing its task ordering, shared layouts or sequence IDs.
The complete owner verifies as **79 matches, zero differences**.

### A real Boolean, not a 64-bit allocation workaround

`categoryEnabled` is initialized to one, optionally cleared for a kind-2
command without qualifying operands, and later gates category lookup. It has
no 64-bit value or storage requirement, so it is an `s32` rather than the
retained draft's `u64`.

With the ordinary narrow Boolean, the pinned compiler changes the first
flag-decision region. Stock RTL keeps the trailing-sound and trailing-effect
assignments as literal ones through `19.lreg`. In `20.greg`, global
allocation/reload uses the live category flag's constant equivalence instead
of independently materializing those constants. The subsequent delay-slot
pass folds the category clear into a branch-likely path, shortening the
function by 16 bytes. Retail instead materializes one independently at
`0x001CB4EC` and keeps the distinct branch/clear sequence.

A single empty tied `+r` operand at the kind-2 policy decision preserves
that allocation boundary. Its input is the initialized category flag, and
its output is the same register value: the empty template does not invent
a value, emit an instruction, touch memory, add a call or clobber, or extend
the flag's real lifetime. The later category lookup remains its actual
consumer. This narrow use was independently reviewed under the localized
compiler-constraint rule. It is an allocation reconstruction, not evidence
of the original spelling or a hardware-required register.

Ordinary and stock RTL-dump compilations of the complete owner were compared.
Their assembly and objects agree after normalizing only the two random
scratch-directory characters in source-path metadata. Allocated sections
and relocation contents agree without alteration. The deciding evidence is
the change from independent constants in `19.lreg` to constant-equivalent
register stores in `20.greg`, followed by the altered branch scheduling.

### Owner preservation and verification

The retained-only callee declarations are scoped to this function. Making
them visible before earlier functions changes an existing tail-call decision;
local scope keeps those functions unchanged. The three shared string imports
at `D_003A3648`, `D_003A3658` and `D_003A3670` remain in their native owner.

Verification uses the pinned compiler and runtime, the complete `check_unit`
owner check, both retail checksum builds, and the tooling suite. The final
source passes 79 matches with no differences, both original executable
checksums, and 442 tests in 117 batches. The checker's `CONSTRAINT` notice is
a review requirement, not a matching or semantic exemption.

## DDS2 counterpart

`func_001D7230` in `src/dds2/game/code_001D4438.c` is 6,728 bytes.
Its complete owner verifies as **81 matches, zero differences**. It uses
the same reviewed narrow Boolean boundary and preserves both shared string
imports. Three retained-draft omissions are corrected from retail evidence:

- `0x001D7A3C` through `0x001D7A58` supplies `-1` for each non-reflected,
  non-`0x40000` group, rather than retaining the preceding group's selector.
- `0x001D8530` through `0x001D8558` includes hit kind 2 alongside kind 4
  when scheduling effect-counter kind 3.
- `0x001D8AC4` reads the actor's `pendingFlags` at offset 8, not `flags`
  at offset 12.

The last five differing words were spill addresses. Whole-owner stock RTL
showed GCSE/PRE creating temporaries for the last-target expression and
the group's flag address, then reload assigning their spill slots in that
order. Grouping the actual operand pointer with its group, and runtime task
pointers together, gives the native temporary ordering and spill addresses
`0x13C` and `0x140`. These are ordinary declarations of existing live values,
all before the first statement; no initializer, evaluation, scope, or
observable effect moves. The grouping is a trace-supported reconstruction,
not a claim about the original source's declaration order.

The exact DDS2 owner and isolated Boolean-boundary and declaration-order
controls were each compiled ordinarily and with stock RTL dumps. Their
allocated sections and relocation sections are byte-identical. In the
declaration-order control, the last-target expression's hash precedes the
flag-address hash; grouping the related pointers reverses that ordering,
and the two PRE temporaries exchange their reload spill slots. This identifies
the compiler mechanism rather than treating declaration shuffling as a
general matching technique.

Direct typed command-record array reads also preserve the original access
sequence while avoiding an unnecessary temporary pointer. Neither the
checker nor the compiler's production options are changed.
