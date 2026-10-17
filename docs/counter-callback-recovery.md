# Paired counter-display callback recovery

> Historical local recovery. The October 17 integration retains upstream counter matches 883d432b3 and fd8527688; these add no new matching credit. See [current integration](recovery-integration-2026-10-17.md) for fresh gates.

DDS1 `func_001FDA78` and DDS2 `func_0020FA98` each match 1,116 native bytes. Complete owners pass 59/0 and 65/0. Both retail checksum gates pass, both development images report zero changed payload words, and the immediate four-target rebuild is a no-op.

## Source and ownership

The counter variant uses its constructor's 0x2C allocation, existing sprite layouts and existing provider ABIs. Dimensions are captured before initialization callbacks; owner, resource and elapsed-tick reads retain their later native observation points. Slot selection is bounded to 0–3, and initialized scatter indices to 0–11. Four corner words are initialized before submission.

Elapsed time and its conversions remain unsigned. The source captures the actual linear rise before unsigned modulo subtraction. Late phase conversion occurs before scaling; RGB masking remains inside the final color assignment, preserving its real temporary lifetime. Four ascending corner assignments reproduce the native stores. No padding, forced register or artificial use is introduced.

DDS1 retains its scalar-status gate followed by a packed observation, while DDS2 uses the distinct single packed gate. Scatter radius, screen bias, table and draw pass remain title-specific. Complete-record status access and the inherited VU pipeline rely on the qualified EE compiler output; this is not a cross-compiler portability or gameplay-validation claim.

## Evidence and credit

The first reconstruction produced 90 differing words in DDS1 and 151 in DDS2. Applying documented source-phase constraints left 19 allocation differences in both. Keeping RGB masking within the final assignment closed both functions without changing their operations or read epochs. Fresh whole-owner and image checks establish the current result; historical qualification is not substituted for these checks.

This recovery preserves earlier work and attribution associated with historical commit `f8342aba864265e2af0e00805282b6d19727b47e` and the existing owner/header contributors. It does not claim sole authorship or byte-for-byte recovery of the missing historical source. See [current qualification](compiler-evidence/counter-callback-recovery/qualification.json).
