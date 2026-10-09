# Callback-fresh selector ownership hypothesis

The native discriminator is reported as a 35 / unsigned-below-36 / 34 tree. The preserved C instead has only two explicit values, because it conflates case 36 and default. This experiment retains an explicit case 36 and removes that default label, leaving the existing rank operands live across callback-driven peer iterations.

This is one concrete semantic hypothesis: unsupported callback-mutated kinds may retain the last rank comparison rather than reselecting through phaseCount. It is not an ordering search. Native successor and reaching-definition validation is still required. The getter's purity and initialization before any mutating clear callback must also be verified. Do not publish this candidate as matching or correct based on compiler similarity alone.

The complete readable experimental body is carried-selector-body.c. The original candidate.patch remains unchanged for reproduction. diagnose.py applies exactly one checked selector-label deletion after replaying that patch; all unrelated source is preserved. Credit Basalt, Obsidian and PiM.
