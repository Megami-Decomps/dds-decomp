# Alpha-marker focused diagnostics

Tested source base: `31a8e6ce863fdfb05d19d6055d285e57ef1d6764`.

Unsigned pulse baseline: native and candidate 912 bytes; 34 differing words. 31 words differ only in recognized GPR fields. Two words at offsets 164 and 168 have non-register-bit differences; one at offset 148 remains unclassified. Register substitutions include A0 to S5 and the retained plain marker's S0/S1 and S2/S3 swaps. This classification does not establish the compiler cause.

Signed pulse conversion control: 872 bytes and 198 differing words. Rejected as a matching route: it removes 40 bytes present in the native function. Kept separately for reproducibility. The active candidate restores the unsigned baseline.

Both checks preserved all 164 older C functions with zero data/context issues and restored the source unit. No full retail image build, development build, or runtime test was performed; no new matching body is claimed.

Runs: [baseline classification](https://github.com/Megami-Decomps/dds-decomp/actions/runs/37906969783), [signed control](https://github.com/Megami-Decomps/dds-decomp/actions/runs/37907348926).
