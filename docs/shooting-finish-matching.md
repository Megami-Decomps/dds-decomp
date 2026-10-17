# Shooting entry finish callback

DDS2 `func_00319388` in `game/code_00316E08` is a 1,744-byte finish/deactivation
callback. It handles player reset, score/effect presentation, runtime pickup
creation, and model/instance retirement. The debug message “ADD ITEM” refers
to spawning a runtime pickup, not an immediate inventory award.

## Source and interface recovery

The reconstruction uses the canonical shooting, work-entry, registry and model
records. The matched `mnuCreateShootingEntryObject` supplies the complementary
ownership path; existing shooting reset functions supply the model frame-step
contract. Credit the PurA/PurD and redthing1 interface work, including creator
naming commit `204cdb395`, and the matched runtime-record rotation idiom.
No previous target-specific C was found in the current tree or coordination
history. An exhaustive remote Git-history search was unavailable; this is not
an assertion that no earlier draft ever existed.

Three distinctions matter before considering instruction scheduling:

- Include `sdf_trig.h`. Without the real float prototypes, implicit declarations
  introduce float-to-double argument conversions and integer-return handling.
- Verify constants with the pinned compiler. Its conversion emits the native
  half-pi `0x3FC90FDA` from `1.5707963f` and frame step `0x3E999999` from `0.3f`.
  Longer host-rounded spellings in the first draft emitted one ULP too low.
- Keep the raw integer timed Y separate from its two screen-space uses. The
  popup and projected point each consume `timedY + originY`. Prematurely
  combining those roles changed reference counts, saved-register allocation
  and code size. Restoring the distinction reduced the residual to six words
  at the native extent. Natural X-then-Y origin capture removed two more.

The popup casts after adding its float X origin; projection casts X before
adding its integer origin. Callback-fresh flag, tag, angle and object reads,
invalid-round detachment, signed byte/halfword accesses, and target decrement
on a null object are retained. Existing SDK vector macros are unchanged.

## Mixed-bank argument order

The final four words concerned the call to `mnuStartPositionedEffectBurst`.
The recovered shared contract is:

```c
s32 mnuStartPositionedEffectBurst(MnuEffectList *list,
    MnuEffectPositionStep *step, s32 baseDelay,
    f32 x, f32 y, f32 z, s32 id);
```

The provider receives list/step/delay/id in A0–A3 and XYZ in F12–F14. These
separate banks do not establish cross-bank source ordering by themselves.
With id before XYZ, pass 00 expanded its signed-halfword load first. Both
schedulers had all four argument loads ready, issued id/X/Y/Z in that order,
and delay reorganization donated Z to the call. XYZ before id reproduces the
native signed-halfword load in the delay slot.

The definition, shared declaration and sole native caller were updated
consistently. All arguments remain register-passed; no varargs, wrapper,
caller-only alternate prototype or assembly change is involved. This follows
[the mixed-parameter-order idiom](idioms.md#mixed-integer-and-floating-parameter-order).
It establishes a consistent matching API, not a uniquely proven historical
source declaration; the retail ELF supplies no usable debug signature.

## Validation

On base `c2afce37f7effd5ce9f4455f5d83ecb90ebcb028`:

- Caller unit: 32 match, 0 differ, including all 1,744 target bytes.
- Provider unit: 46 match, 0 differ with the shared contract.
- Explicit DDS1/DDS2 retail and dev targets pass; both retail SHA1 checks pass.
- Rebuilt DDS2 dev audit has zero changed payload words; DDS1 is unchanged.
- Immediate four-target rebuild is a no-op; no generated source churn.

Net new source coverage is one function, 1,744 bytes. The already-matched
provider receives no additional matching credit.
