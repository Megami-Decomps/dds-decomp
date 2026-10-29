# Original kernel names on native layouts

The scheduler, administration state and dispatch-row types correspond to
`dds3ProcessID_t`, `dds3AdminiData_t` and `dds3AdminiList_t` in HD's `kernel.h`.
The [HD type metadata](https://github.com/Megami-Decomps/smt3-decomp/blob/main/config/smt3/hd_types.txt)
establishes these original type/member names and header ownership. Native
DDS offsets and readers establish field correspondence; HD layouts are not
copied. The scheduler remains 0x50 bytes, the administration state 0x24 and
the three-pointer dispatch row 0x0C. All existing native C types stay intact.
In particular `workdata` remains a u32, and the shutdown callback remains void.

| Native type | Previous member | Original member |
| --- | --- | --- |
| dds3ProcessID_t | nameSum | namehash |
| dds3ProcessID_t | flags | flag |
| dds3ProcessID_t | unk24 | starttime |
| dds3ProcessID_t | runUpdateCount | runtime |
| dds3ProcessID_t | startDelayTicks | starttimer |
| dds3ProcessID_t | destroyDelayTicks | shutdowntimer |
| dds3ProcessID_t | update | processfunc |
| dds3ProcessID_t | destroy | shutdownfunc |
| dds3ProcessID_t | userValue | workdata |
| dds3ProcessID_t | listNext | next |
| dds3ProcessID_t | listPrev | prev |
| dds3ProcessID_t | childList | child |
| dds3ProcessID_t | nextSibling | sibling |
| dds3AdminiData_t | flags | Stat |
| dds3AdminiData_t | value | RunTimer |
| dds3AdminiData_t | activeMode | NowSequence |
| dds3AdminiData_t | requestedMode | NextSequence |
| dds3AdminiData_t | historyIndex | HistoryPointer |
| dds3AdminiData_t | signedHistory | SequenceHistory |
| dds3AdminiData_t | unsignedHistory | FlagHistory |
| dds3AdminiData_t | ownedRequestData | CallData |
| dds3AdminiData_t | requestSizeLowByte | CallDataSize |
| dds3AdminiData_t | modeTransitionDelay | CallCount |
| dds3AdminiList_t | entry | start |
| dds3AdminiList_t | destroy | end |
| dds3AdminiList_t | cleanup | check |

The scheduler checksum, state bits, queue links, parent/child links, callback
invocation and public data/time accessors establish the field roles. The word
at 0x24 is returned by `dds3GetProcessStartTime`; no remaster-only fields are
introduced. The administration request path copies its payload, stores the
next sequence, counts down `CallCount`, activates the sequence and records
its history. Mode exit stores the callback result plus one in that same
countdown byte. The completion path increments `RunTimer`. These operations
match Nocturne's native roles, beyond sharing offsets. Padding retains its
existing spelling and size.

Frame setup, scene packet setup and frame rendering correspond to
`dds3FirstProcess`, `dds3DrawFirstProcess` and `dds3DrawProcess`. Their native
rendering details differ by game. `dds3GlobalWorkInit` registers the GBWK
callback `dds3GlobalWorkMainProc`, which increments runtime/playtime;
`dds3GlobalWorkDestroy` kills GBWK and releases the backing allocation. HD's
original `dds3GlobalWork.c` has these same lifecycle roles. Debug pad editors
and fade-color setup retain their native operations with the HD names.

The entry point has the original assembler name `main` and C name
`sdfDefaultMain`, as in Nocturne. The C identifier avoids GCC's special
implicit `__main` insertion; the existing explicit call is preserved. The
development wrapper now wraps `main` as well.

## Administrator mode values

The native table is at 0x003297D0 in DDS1 and 0x003847D0 in DDS2. Handler-proven
rows share the original enumerator name with Nocturne, retaining DDS values.
The complete Call/Exit/Check triples are compared, not just an empty handler.
Field starts the supplied field scene, Event chooses the explicit/pending
event and controls the sky task, and Camp creates/destroys camp tasks and
checks their acknowledged exit state. Those roles establish rows 5, 6 and 7.

| Original enum suffix | DDS1/2 | Nocturne |
| --- | --- | --- |
| DEBUG | 0 | 0 |
| BOOTMC | 1 | 1 |
| TITLE | 2 | 2 |
| LOAD | 3 | 3 |
| ENDSAVE | 4 | 5 |
| FIELD | 5 | 6 |
| EVENT | 6 | 7 |
| CAMP | 7 | 8 |
| RECOVER | 8 | 12 |
| SAVE | 9 | 13 |
| RESULT2 | 10 | 14 |
| STAFF | 12 | 16 |
| CONFIG | 13 | 17 |
| SAVE_TEST | 21 | 31 |
| SOUND_TEST | 23 | 35 |
| BILLBOARD_TEST | 24 | 36 |
| FONT_TEST | 25 | 37 |

Each suffix above is prefixed `DDS3_ADMINI_LIST_`. SAVE and SAVE_TEST have
the CallTerminal/ExitTerminal/CheckTerminal and corresponding test handlers
in both games. DDS battle row 14 retains `DDS3_ADMIN_MODE_BATTLE`: its
CheckNewBattleTest pointer alone does not establish identity with a whole
Nocturne battle row. Unresolved rows receive no guessed enumerator names.
