# Radial linkage formation

`btlApplyRadialLinkageFormation` is DDS1 `0x001F5D08` and DDS2
`0x00207268`, 460 bytes each. It accepts one target and the command's three
participants, skips null participants and signed selector -1, and applies each
resource-selected radial spacing while preserving the participant's old Y.
DDS1 accepts 85 action rows starting at 0x1AB; DDS2 accepts 117.
The range subtraction is unsigned so out-of-range signed inputs cannot
overflow before the guard.

The existing eight-byte action rows select spacing floats at actor archive
address +0x38 + selector*0x14. `BtlFormationSpacingCell` describes this read
window, without claiming the full archive layout or owning its storage.
The provider returns the archive address as an integer. Computing the selected
byte address before casting preserves that interface and the native addition.
A single participant index selects both the actor and action-row selector.
EE GCC's loop pass generates the native advancing actor pointer; a manually
maintained second cursor changed the preheader scheduling.

## Scoped SDK store audit

These two bodies use the existing input-address-only SDK VU store form.
This does not establish a general memory barrier; the restrictions in
[Thunder Bezier matching](thunder-bezier-matching.md#legacy-store-restriction)
continue to apply.

- The target muzzle vector is fully stored once, before the loop, and read
  exclusively by the two VU loads in each participating iteration. C does
  not inspect its components. No subsequent call receives its address.
- The actor's old position is written by `btlCopyUnitCurrentPosition` on each
  iteration; its Y is read after the actor-muzzle callback.
- The VU pipeline fully stores the new position, then C replaces its Y with
  that old Y. No C read of the new vector precedes this store. The immediate
  `btlSetUnitPosition` call receives and consumes the complete vector before
  the next iteration. No previous new-position value is reused.
- Scalar-to-VU transfers retain their truthful GPR2 clobber. Target and new
  position buffers require 16-byte alignment for the quadword operations.

An output-preserving observation of the actual vendor compiler confirms the
cause: the memory-clobbered store uses a PARALLEL pattern and retains cached
addresses; the SDK input-only store visits top-level volatile ASM_OPERANDS,
flushing local CSE. The native second target-vector address and fresh setter
argument are then rematerialized. Local allocation also shows that the
archive-address sum's first input determines its destination tie. The source
changes preserve genuine vector and address operations; they add no artificial
uses, register bindings, or volatile C objects.

Preserve Jade Osprey, Copper, Amber, and Brass Osprey's preceding DDS2 source,
record and SDK investigations, and Halcyon Drift's compiler guidance.
