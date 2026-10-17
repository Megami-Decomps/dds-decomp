# Recovery integration, October 17, 2026

Initially integrated onto `7411c3a5018c05ea3d34eb7a3e4ef25800c78102`, then rebased onto `f1ece955e6024c49b19c369b50ed59c9b431b236` after concurrent upstream changes. Both retail/dev gates and immediate no-op were repeated successfully; the changed DDS1 camera owner again passed 269/0.

## New matching contribution

Four functions, **9,616 native bytes**:
- DDS1 `func_001EB368` and DDS2 `func_001FBAC0`: 2,844 bytes each.
- Both `effPcpBlockSetUpdate`: 1,964 bytes each.

Retained upstream implementations of five previously recovered bodies: both target-effect mappers, DDS2 `effBattleUpdate` (`8868355cd`), and counter callbacks (`883d432b3`, `fd8527688`). These receive no duplicate credit. Earlier recovery documents and receipts remain historical evidence, not current-owner qualification.

## Integration and verification

Kept the current shared cursor layout, canonical field/provider names, mapper bodies, packed effect-link representation, and DDS2 random-target camera body/bank. The prior scalar effect-link migration and single-target workaround were discarded. Endpoints retain truthful s32 returns and the projected pair provider's f32 return and nine-argument contract.

Fresh complete-owner checks: DDS1 camera 269/0, DDS2 camera 252/0, both PCP owners 368/0. Initial checks exposed a stale cursor type and old generated labels; these were corrected before final qualification.

`configure.py dds1 dds2 --force-split` completed; generated source churn was restored to the intended source bytes before building. `ninja -j2 dds1 dds2 dds1-dev dds2-dev` exited 0, both original retail SHA1 gates passed, and both development audits report zero changed payload words. An immediate repeat reported no work. Checker tests (59) and observer tests (56) all pass.

Default targets exclude unavailable retail base archives (32 DDS1, 26 DDS2); no complete archive or gameplay qualification is claimed. The [current receipt](compiler-evidence/recovery-integration-2026-10-17/qualification.json) binds source and log hashes. Original image SHA1 values are included there.

## Credit

Endpoint work: PiM, Basalt, Silver and Brass. PCP: Deep Quasar, KiB, Silver Helix, Amber, Brass Osprey, Sable Forge and Evening Notes. Preserve the detailed attribution and immutable handoff links in the earlier recovery documents. Upstream match authors retain credit for the five implementations kept from main.

The integration also carries the qualified compiler decision-tier observer, nine-entry command bounds, fade allocation case study, and the missing-target checker regression fix. No private nonmatching candidate source is published.

Final publication base: `5d04a1d5c438c95106286ad1dc723201bb3e390b`. Both retail/dev targets passed again after this clean rebase. Automatic declaration-placement churn was restored before the final successful rebuild and immediate no-op.
