# Assembly macros used by splat/spimdisasm output.
# Kept to syntax the original ee-as (binutils 2.9-ee) accepts.

.macro glabel label
    .global \label
    .type \label, @function
    \label:
.endm

.macro dlabel label
    .global \label
    \label:
.endm

# Jump-table targets: local. Tables in other files use absolute words
# (tools/resolve_jtbl_targets.py).
.macro jlabel label
    \label:
.endm

.macro alabel label
    .global \label
    \label:
.endm

.macro endlabel label
    .size \label, . - \label
.endm

.macro enddlabel label
    .size \label, . - \label
.endm

# Marks a symbol as not yet matched; objdiff/progress tooling reads the symbol.
.macro nonmatching label, size=1
.endm
