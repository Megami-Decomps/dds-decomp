# Alpha-marker diagnostic (not a matching contribution)

KiB / redthing1, with Quartz credit for the underlying rotated-marker reconstruction.

This isolated branch tests DDS2 func_00145D48 against canonical base 31a8e6ce863fdfb05d19d6055d285e57ef1d6764 using the existing repository CI toolchain and private-retail input route. Main and its workflow are unchanged.

candidate.inc reconstructs the saved initial alpha-marker experiment from the source-only marker checkpoint cc61e6513c8a716dab4bcbc62c06c1ddfa20f94b in dds-collab, with the previously reviewed alpha pulse/clamp branch. The lost local experiment returned exit1 but its diagnostics were not recovered; no comparison to that unseen result is claimed.

The temporary C replacement is installed only after a clean baseline whole-unit check and restored afterward. All compiler/checker stdout and stderr are captured and never uploaded or printed; only explicitly parsed statuses, numeric sizes/counts and identity digests are emitted. No original instruction bytes, assembly, retail images, compiler objects or raw logs are uploaded.

This workflow performs focused target and whole-unit diagnostics, NOT complete retail image/dev-link/runtime qualification. A clean diagnostic is insufficient to publish the body to main. A nonmatching candidate returns a failing job. Caller bounds and the complete native/source review remain required before any matching publication.

diagnose.py and the branch workflow are experiment-only support, not intended for main. No compiler flags, toolchain binaries, credentials or permissions are changed.
