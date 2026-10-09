# Alpha-marker focused diagnostics

Tested source base: `31a8e6ce863fdfb05d19d6055d285e57ef1d6764`.

## Current retained source

`candidate.inc` is the improved, still-nonmatching DDS2 `func_00145D48` reconstruction: native and candidate are both 912 bytes, with 20 differing words. All 20 differences are recognized general-purpose register-field substitutions. Every other compared instruction bit agrees. The remaining register-pair substitutions are the same pattern recorded for the retained plain-marker candidates. This is a residual classification, not a proof of its source-level cause.

All 164 older C functions remain exact, retaining their full-object sizes. There are zero data, context, or other issues. The diagnostic restores the original translation unit after testing. No full retail image build, development build, runtime test, or new exact-match credit is claimed.

## Evidence-backed improvement

The initial unsigned-pulse reconstruction was 912 bytes with 34 differing words. A candidate-only compiler probe reproduced the ordinary target's bytes and all 17 relocations before its source-expression anchors and allocation metadata were used. It showed that pulse calculation and packed color were separate working values. Keeping a single signed color variable through the original pulse scaling/clamping and unsigned packing reduced the residual to 20 words without changing statement order. The unsigned float conversion remains intact.

Improved source: commit `1d0565f8bbc54c943fe44eb40a0860c30f8d59a7`.

## Rejected control

Changing only pulse truncation to a signed float conversion produced 872 bytes and 198 differing words. It removes 40 bytes present in the native function and is rejected as a matching route. `signed-pulse-control.inc` preserves that experiment separately. Both it and the original baseline preserved the 164 older C functions and had no data/context issues.

## Runs

- [Initial residual classification](https://github.com/Megami-Decomps/dds-decomp/actions/runs/37906969783)
- [Rejected signed conversion](https://github.com/Megami-Decomps/dds-decomp/actions/runs/37907348926)
- [Candidate probe parity and source anchors](https://github.com/Megami-Decomps/dds-decomp/actions/runs/37909566865)
- [Improved source, 20 differing words](https://github.com/Megami-Decomps/dds-decomp/actions/runs/37910069227)
- [All 20 residual words classified](https://github.com/Megami-Decomps/dds-decomp/actions/runs/37910590950)

The failing focused matching jobs intentionally report a nonmatch. A successful candidate-only instrumentation job is not retail-build or runtime proof. No original instruction words or raw compiler dumps are uploaded by these experiments.
