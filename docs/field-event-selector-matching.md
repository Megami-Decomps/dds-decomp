# INF event-selector resolution

`fldResolveEventSelectorTarget` reconstructs DDS1 `0013CBA8` and DDS2
`0013F790`, 772 native bytes each. The shared name is inferred from the
operation, not recovered from a debug symbol.

## Data and control flow

The resolver follows the current INF set's signed selector graph. Selectors
below ten use the flag-selector rows; higher selectors use the message rows.
A flag test may change the selected set, so the result path reads the current
set index again. Serialized target bytes and `eventHit` are sign-extended even
though their canonical storage fields are unsigned bytes.

Zero and 101–105 are direct terminal targets. Target 100 resolves the current
room resource's event name, checks the resulting event kind, and, when needed,
looks up the actor and dispatches its current classification. The actor-stat
case selects 104 for zero and 105 otherwise. Results 106 and 107 can be
synthesized by these dispatches; they are not additional direct-terminal graph
cases.

Other targets link to another flag selector or message. A message whose signed
kind is -1 continues resolution. Otherwise, only unsigned kinds below two
enter the final event-kind switch. Unknown kinds return zero. The source adds
no range checks, cycle limits, fallback lookups, or different provider behavior.
The DDS2 actor-name provider has title-specific behavior, including a path
that leaves the classification unchanged; the resolver intentionally ignores
the provider's return and reads the existing global classification afterward.

## Source reconstruction and compiler behavior

A hand-maintained `for (;;)` loop initially gave the argument, intermediate
selector and row index different lifetimes from native. Expressing the actual
linked graph traversal as tail recursion restores the compiler's argument
replacement and shared backedges. The pinned compiler eliminates the recursive
calls; the result has the native constant stack frame and no stack growth.

The direct terminal cases return the recovered target separately. Combining
all terminal labels into one identity range changes dispatch lowering. The
actor-stat case uses explicit zero/nonzero returns, preserving the native
104-default/105-override conditional move. These source distinctions introduce
no extra operations or artificial dependencies.

The initial loop source emitted 724 bytes. Explicit terminal dispatch emitted
780 bytes. Tail-recursive graph resolution restored 772 bytes and left five
instruction differences; preserving each terminal's result provenance removed
two, and the explicit actor-stat branch removed the final three.

## Validation

The two native bodies are instruction-isomorphic after exact symbol and branch
translation: 193 function words each, plus a separate alignment nop, and three
eight-entry jump tables. Frozen complete-owner checks passed 241/0 for DDS1 and
82/0 for DDS2 before integration. All existing compiled functions and owned
read-only data remained exact.

Independent review covers callback-fresh global reads, signed byte and
halfword interpretation, all three jump tables, special-target fallthrough and
caller-valid graph bounds. The maintained INF corpus contains 46 tables and
1,840 sets; traversal from selector 1 under both flag outcomes finds no invalid
selector/target or cycle. This corpus check is not a claim about arbitrary
malformed INF data.

The publication gate checks both original retail hashes and both development
links, including unchanged development payload words. No instruction padding, compiler flag change, register
pinning, fabricated read, or inline assembly is used.
