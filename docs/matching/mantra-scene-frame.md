# DDS1 mantra scene frame

`mnuDrawMantraSceneFrame` (`0x00251E38`, 3756 bytes) reconstructs the
phase-dependent scene layers using the existing `MenuSceneWork`, metadata,
and typed transition-list contracts. It preserves callback order, fresh
selected-party lookup after list advancement, and the distinct foreground,
entry and pending-strip fades. Phases 10/11 and 12 share actual pending-list
advancement, followed by fixed-opacity navigation.

## Inferred copied-case trace

Phase 2 repeats phase 1's live seven-frame entry-channel calculation even
though this outgoing case does not consume that channel. This is an inferred
copied-case form, **not recovered original source**, retained under the explicit
"Dead code that retail still shows" exception in CONTRIBUTING.

A qualified read-only vendor-cc1 trace shows the unused values disappear in
`13.life`, while their integer frame predicate survives combine and allocation
and disappears in `21.flow2`. Retail retains the resulting integer load and
`mtc1` at `0x002520D8`/`0x002520EC`, and separate CSE regions rematerialize the
unit constant instead of carrying it across all callbacks. Removing the copied
clause loses these properties; an ordinary mutable fade accumulator fixes only
the constant lifetime, not the load. The exact live piecewise clause beside it
provides the required corroboration. No arbitrary clamp or threshold was added.
The frozen control yielded 589 observer events and 28 equal artifacts, with
raw same-path no-dump assembly/object parity and normalized strict RTL parity.

## Live pending-list stage

Each pending arm borrows `&work->sceneTransitionList` after its strip callback,
then passes that live typed pointer to the shared advancement action. This is
ordinary used data, not a dead load or a one-pass block. The branch-local address
definition preserves the native shared strip tail. A qualified separate-actions
control (85 events, 28 equal artifacts) showed late crossjump first attaching
both pending tails to the later active case's navigation/party suffix, before
comparing the pending tails to each other. Factoring the real advancement stage
and retaining its live address acquisition avoids that earlier wrong sharing. The final
qualified observer (81 events, 28 equal artifacts and all 26 ordinary-parity
checks) confirms jump2 directly merges the pending tails from conversion
UIDs 3050/3255. Their live list-address definitions (3072/3277) follow the
strip calls and precede the shared advancement label.

The owned sine literal is `3.1415926f`: the pinned compiler emits native
`0x40490FDA` (whereas `3.1415925f` emits `0x40490FD9`). The switch owns its
compiler-generated jump table and padding; no external replacement data is used.

## Provenance

The final body was reconstructed from native instructions and the matched
same-owner state updater, not recovered from historical draft bytes. Earlier
Raikaru/Q2 (board comment 6256033696), Q3 (6257734713), Q0 (6258960379), and PurFAD
(6262922035) established the fade ordering, inline channel amounts, shared
pending-strip frontier, and latest-first crossjump diagnosis. This work retains
that credit and adds the independently reviewed copied-channel trace and live
list-head borrowing form.
