#include "kwln.h"
#include "mnu_list.h"
#include "kwln_task_state.h"
#include "evt_world.h"
#include "sdf_resource.h"
#include "dat_state.h"
#include "kwln_task_lifecycle.h"
#include "mnu_mantra_position_api.h"

#define EVT_PARTY_SLOT_COUNT 5

#define MNU_MANTRA_POSITION_RECORD_BYTES 0x20

#define MNU_MANTRA_SELECTION_WORK_BYTES 0x16C

#define PRF_ATTRIBUTE_SLOT_COUNT 5

#define PRF_ATTRIBUTE_MULTIPLE_SENTINEL 5

extern s32 mnuMantraPanelPositionTable;

extern EvtLoadedRecord *mnuMantraNodePositionTable;

extern s32 func_0026CD50(u32);

typedef struct EvtMantraNodePositionRecord {
    union {
        u32 packedHeader; /* Flags and signed ID also have a native word view. */
        struct {
            u16 flags;
            s16 id;
        };
    };
    s16 firstKey;  /* 0x04 */
    s16 secondKey; /* 0x06 */
    struct EvtMantraNodePositionRecord *neighbors[6];
} EvtMantraNodePositionRecord; /* 0x20 */

INCLUDE_ASM(const s32, "game/code_0026CD50", func_0026CD50);

extern s32 fileQueueDefaultCallbackRequest(const char *path);
extern s32 fileIsRequestReadyInCurrentMode(s32 request);
extern u32 fileGetLoadedDataAddress(s32 request);
extern s32 fileGetResourceHandle(s32 request);
extern void filePollEntryCleanup(s32 request);
extern void func_00315A50(void);
void mnuLoadMantraNodePositionTable(u32 resourceId);

/* Load the mantra node-position table synchronously and release the request. */
void func_0026CE90(void) {
    s32 request;

    evtPrintDeveloperConsoleMessage("Mantra Table Load [/facility/data/mtrMantraBlockData.tbl]\n");
    request = fileQueueDefaultCallbackRequest("/facility/data/mtrMantraBlockData.tbl");
    while (!fileIsRequestReadyInCurrentMode(request)) {
    }
    mnuLoadMantraNodePositionTable(fileGetLoadedDataAddress(request));
    func_00315A50();
    sdfReleaseResourceAllocation(fileGetResourceHandle(request));
    filePollEntryCleanup(request);
}

/* Replace the node-position resource, releasing an existing table first. */
void mnuLoadMantraNodePositionTable(u32 resourceId) {
    if (mnuMantraNodePositionTable != 0) {
        mnuReleaseMantraNodePositionTable();
    }
    mnuMantraNodePositionTable = (EvtLoadedRecord *)func_0026CD50(resourceId);
}

/* Release the retained node-position handle and clear the global table address. */
void mnuReleaseMantraNodePositionTable(void) {
    sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(mnuMantraNodePositionTable->handle));
    mnuMantraNodePositionTable = 0;
}

/* Return a 32-byte record address for a signed halfword index; callers sign-extend the index at the call
 * (func_0028DC08 +0x34), and this body re-extends it (sll/sra 16). */
struct MantraNodePos;

struct MantraNodePos *mnuGetMantraNodePositionRecord(s16 index) {
    return (struct MantraNodePos *)(mnuMantraNodePositionTable->recordsAddress + index * 32);
}

/* Find the first position matching two signed-halfword, staggered-grid keys.
 * The second coordinate's odd bit adds five to the first key; both scale by ten.
 * A loaded table is required; a failed search returns address zero. */
s32 mnuFindMantraNodePositionRecord(s32 firstCoordinate, s32 secondCoordinate) {
    EvtLoadedRecord *loaded = mnuMantraNodePositionTable;
    s16 firstKey;
    s16 secondKey;
    EvtMantraNodePositionRecord *record;
    s32 index;

    firstCoordinate = (s16)firstCoordinate;
    secondCoordinate = (s16)secondCoordinate;
    firstKey = (s16)(firstCoordinate * 10 + ((secondCoordinate & 1) * 5));
    secondKey = (s16)(secondCoordinate * 10);
    record = (EvtMantraNodePositionRecord *)loaded->recordsAddress;

    for (index = 0; index < (s32)loaded->recordCount; index++) {
        if (record->firstKey == firstKey && record->secondKey == secondKey) {
            return (s32)record;
        }
        record = (EvtMantraNodePositionRecord *)((u8 *)record + MNU_MANTRA_POSITION_RECORD_BYTES);
    }
    return 0;
}

/* Return the loaded node-position count; the table must already exist. */
u32 mnuGetMantraNodePositionRecordCount(void) {
    return mnuMantraNodePositionTable->recordCount;
}

/* Replace the panel-position resource, releasing an existing table first. */
void mnuLoadMantraPanelPositionTable(u32 resourceId) {
    if (mnuMantraPanelPositionTable != 0) {
        mnuReleaseMantraPanelPositionTable();
    }
    mnuMantraPanelPositionTable = func_0026CD50(resourceId);
}

/* Release the retained panel-position handle and clear the global table address. */
void mnuReleaseMantraPanelPositionTable(void) {
    sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(((EvtLoadedRecord *)mnuMantraPanelPositionTable)->handle));
    mnuMantraPanelPositionTable = 0;
}

/* Return a 32-byte panel record using the same signed low-halfword index convention. */
struct MantraNodePos *mnuGetMantraPanelPositionRecord(s16 index) {
    return (struct MantraNodePos *)(((EvtLoadedRecord *)mnuMantraPanelPositionTable)->recordsAddress +
                                    ((index << 0x10) >> 0xb));
}

extern void func_0026D168(void *, s32, s32);

typedef struct EvtMantraWork {
    struct SdfMemBlock *allocation;
    u32 capacity;
    void *entries;
    u8 data[0x160];
} EvtMantraWork; /* 0x16C bytes */

/* Allocate/zero the selection work and point its entries at its inline storage.
 * The existing initializer is called only for nonzero initialValue. */
EvtMantraWork *evtAllocateMantraSelectionWork(s32 initialValue, s32 mode) {
    struct SdfMemBlock *allocation = sdfAllocGeneralBlock(MNU_MANTRA_SELECTION_WORK_BYTES);
    EvtMantraWork *work = (EvtMantraWork *)sdfMemoryGetBlockAddress(allocation);

    memset(work, 0, MNU_MANTRA_SELECTION_WORK_BYTES);
    work->allocation = allocation;
    work->capacity = 0xB0;
    work->entries = work->data;
    if (initialValue != 0) {
        func_0026D168(work, initialValue, mode);
    }
    return work;
}

void evtReleaseMantraSelectionWork(EvtMantraWork *work) {
    if (work != NULL) {
        sdfReleaseResourceAllocation(work->allocation);
    }
}

extern s32 mnuGetActiveMantraModelFlagState(void);

extern void evtPrintDeveloperConsoleMessage(const char *, ...);

extern u32 scrGetSelectedScriptEntryId(DatPartyRecord *);

extern s32 func_00316020(DatPartyRecord *, u16);

extern s32 func_00314B00(DatPartyRecord *, u16);

extern u32 ptyGetProfileRecordCap(u16);

extern u32 ptyGetProfileRecordValue(DatPartyRecord *, u16);

extern s32 func_00315C68(u32, u32, DatPartyRecord *, u16, u32 *);

extern s32 func_00315FA0(u32, DatPartyRecord *, u16);

extern s32 func_0028F128(u16, s8);

extern s32 func_0026D590(u16, s32);

extern const char D_00425098[];

/* Populate mantra selection flags from rank, profile state and node rules. */
void func_0026D168(void *selection, s32 unitAddress, s32 mode) {
    EvtMantraWork *work = selection;
    DatPartyRecord *unit = (DatPartyRecord *)unitAddress;
    s32 rank;
    u32 selected;
    u16 *entry;
    EvtMantraNodePositionRecord *node;
    s32 i = 0;

    rank = mnuGetActiveMantraModelFlagState();
    evtPrintDeveloperConsoleMessage(D_00425098, rank);
    selected = scrGetSelectedScriptEntryId(unit);
    entry = work->entries;
    node = (EvtMantraNodePositionRecord *)mnuMantraNodePositionTable->recordsAddress;
    for (; i < (s32)mnuMantraNodePositionTable->recordCount; i++, node++, entry++) {
        *entry = 0;
        if (node->id != 0) {
            s32 rankMet;
            if (rank < ((s32)(node->packedHeader << 24) >> 28)) {
                rankMet = 0;
            } else {
                rankMet = 1;
            }
            if (func_00316020(unit, node->id) == 0) {
                goto unavailable;
            } else {
                if (selected == node->id) {
                    *entry |= 0x400;
                }
                if (func_00314B00(unit, node->id)) {
                    *entry |= 0x200;
                }
                if (ptyGetProfileRecordCap(node->id) == ptyGetProfileRecordValue(unit, node->id)) {
                    *entry |= 0x100;
                }
                if ((node->packedHeader & 0xF) == 3) {
                    if (func_00315C68(0, 4, unit, node->id, 0) == 0) {
                        goto unavailable;
                    }
                    if (mode != 0) {
                        if (func_0028F128(node->id, (s8)unit->unitId) == 0) {
                            goto unavailable;
                        }
                        *entry = (*entry & 0xFFF0) | 1;
                    } else {
                        *entry = (*entry & 0xFFF0) | 1;
                    }
                } else if (func_00315FA0(0, unit, node->id) != 0 || rankMet) {
                    switch ((s32)(node->packedHeader << 28) >> 28) {
                    case 2:
                        if (func_0026D590(node->id, mode)) {
                            *entry |= 0x800;
                        }
                        if (func_00315C68(1, 2, unit, node->id, 0)) {
                            if ((*entry >> 8) & 8) {
                                if (node->packedHeader & 0x100) {
                                    *entry = (*entry & 0xFFF0) | 0x101;
                                } else {
                                    *entry = (*entry & 0xFFF0) | 1;
                                }
                            } else {
                                *entry = (*entry & 0xFFF0) | 2;
                            }
                        } else {
                            *entry = (*entry & 0xFFF0) | 2;
                        }
                        if ((node->packedHeader & 0x100) && ((*entry >> 8) & 8)) {
                            *entry = (*entry & 0xFFF0) | 0x101;
                        }
                        break;
                    case 1:
                    case 4:
                        if (func_00315FA0(1, unit, node->id)) {
                            *entry = (*entry & 0xFFF0) | 1;
                        } else if (func_00315C68(1, 2, unit, node->id, 0)) {
                            *entry = (*entry & 0xFFF0) | 1;
                        } else if ((*entry >> 8) & 2) {
                            *entry = (*entry & 0xFFF0) | 1;
                        } else {
                            *entry = (*entry & 0xFFF0) | 2;
                        }
                        break;
                    }
                } else {
                    goto unavailable;
                }
            }
        }
        continue;
    unavailable:
        *entry = (*entry & 0xFFF0) | 3;
    }
}

extern u32 ptyGetProfileRecordCap(u16 scriptId);

/* The shared value provider owns a DatPartyRecord, not an integer address. */
extern u32 ptyGetProfileRecordValue(DatPartyRecord *work, u16 scriptId);

/* 1 when some active party member other than unit `skipId` has `scriptId` at its profile record cap. */
s32 ptyAnyActivePartyMemberAtProfileCap(u16 scriptId, u16 skipId) {
    s32 i;

    for (i = 0; i < EVT_PARTY_SLOT_COUNT; i++) {
        DatPartyRecord *slot;

        if (datGameState->party[i].flags & 1) {
            slot = &datGameState->party[i];
            if (slot->unitId != skipId) {
                if (ptyGetProfileRecordCap(scriptId) == ptyGetProfileRecordValue(slot, scriptId)) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* Like func_0026D7E8, but a mode of 1 rejects the node and the profile caps alone decide. */
s32 func_0026D590(u16 entry, s32 mode) {
    EvtMantraNodePositionRecord *record;
    s8 satisfiedNeighbors;
    s32 partyIndex;

    if (func_0028F128(entry, 0) != 0) {
        return 1;
    }
    if (mode == 1) {
        return 0;
    }
    record = (EvtMantraNodePositionRecord *)mnuGetMantraNodePositionRecord((s16)entry);
    if (record == NULL) {
        return 0;
    }

    satisfiedNeighbors = 0;
    for (partyIndex = 0; partyIndex < EVT_PARTY_SLOT_COUNT; partyIndex++) {
        s32 slot;

        if ((u16)(datGameState->party[partyIndex].flags & 1) != 0) {
            DatPartyRecord *party = &datGameState->party[partyIndex];
            for (slot = 0; slot < 6; slot++) {
                EvtMantraNodePositionRecord **neighborSlot = &record->neighbors[slot];
                EvtMantraNodePositionRecord *neighbor = *neighborSlot;

                if (neighbor != NULL) {
                    if ((neighbor->packedHeader & 0xF) == 1) {
                        u32 cap = ptyGetProfileRecordCap((u16)(*neighborSlot)->id);

                        if (cap == ptyGetProfileRecordValue(party, (u16)(*neighborSlot)->id)) {
                            satisfiedNeighbors |= 1 << slot;
                        }
                    } else {
                        satisfiedNeighbors |= 1 << slot;
                    }
                } else {
                    satisfiedNeighbors |= 1 << slot;
                }
            }
            if (satisfiedNeighbors == 0x3F) {
                return 1;
            }
        }
    }
    return satisfiedNeighbors == 0x3F;
}

typedef struct MantraNodeStates {
    u8 pad00[8];
    u16 *cells; /* 0x08: per-node state halfwords, indexed by record id */
} MantraNodeStates;

void func_0026D710(MantraNodeStates *states, s32 unused, s16 entry) {
    EvtMantraNodePositionRecord *record = (EvtMantraNodePositionRecord *)mnuGetMantraNodePositionRecord(entry);
    u16 *origin = &states->cells[record->id];
    s32 slot;

    *origin |= 0x100;
    for (slot = 0; slot < 6; slot++) {
        EvtMantraNodePositionRecord *neighbor = record->neighbors[slot];

        if (neighbor != NULL) {
            u16 *cell = &states->cells[neighbor->id];
            s32 kind = neighbor->packedHeader & 0xF;

            if (kind == 1 || kind == 4 || ((neighbor->packedHeader & 0x10F) == 2 && ((*cell >> 8) & 8))) {
                if ((*cell & 0xF) == 2) {
                    *cell = (*cell & 0xFFF0) | 1;
                }
            }
        }
    }
}

extern s32 func_0026DB48(DatPartyRecord *context, u8 entry);

extern s32 func_0026DB90(DatPartyRecord *context);

/* Return true when all six neighbor conditions are satisfied across active party members. */
s32 func_0026D7E8(u16 entry) {
    EvtMantraNodePositionRecord *record;
    s8 satisfiedNeighbors;
    s32 partyIndex;

    if (func_0028F128(entry, 0) != 0) {
        return 1;
    }
    record = (EvtMantraNodePositionRecord *)mnuGetMantraNodePositionRecord((s16)entry);
    if (record == NULL) {
        return 0;
    }

    satisfiedNeighbors = 0;
    for (partyIndex = 0; partyIndex < EVT_PARTY_SLOT_COUNT; partyIndex++) {
        s32 slot;

        if ((u16)(datGameState->party[partyIndex].flags & 1) != 0) {
            DatPartyRecord *party = &datGameState->party[partyIndex];
            for (slot = 0; slot < 6; slot++) {
                EvtMantraNodePositionRecord **neighborSlot = &record->neighbors[slot];
                EvtMantraNodePositionRecord *neighbor = *neighborSlot;

                if (neighbor != NULL) {
                    if ((neighbor->packedHeader & 0xF) == 1) {
                        if (func_0026DB90(party) != 0) {
                            if (func_0026DB48(party, (u8)(*neighborSlot)->id) != 0) {
                                satisfiedNeighbors |= 1 << slot;
                            }
                        } else {
                            u32 cap = ptyGetProfileRecordCap((u16)(*neighborSlot)->id);

                            if (cap == ptyGetProfileRecordValue(party, (u16)(*neighborSlot)->id)) {
                                satisfiedNeighbors |= 1 << slot;
                            }
                        }
                    } else {
                        satisfiedNeighbors |= 1 << slot;
                    }
                } else {
                    satisfiedNeighbors |= 1 << slot;
                }
            }
            if (satisfiedNeighbors == 0x3F) {
                return 1;
            }
        }
    }
    return satisfiedNeighbors == 0x3F;
}

/* The sixth work stores the combined availability state for the five profiles. */
void func_0026D988(EvtMantraWork **selectionWorks) {
    EvtMantraNodePositionRecord *node;
    u16 *combinedEntry;
    s32 recordCount;
    s32 remaining;
    s32 profileIndex;

    node = (EvtMantraNodePositionRecord *)mnuMantraNodePositionTable->recordsAddress;
    combinedEntry = selectionWorks[5]->entries;
    recordCount = (s32)mnuGetMantraNodePositionRecordCount();
    if (recordCount <= 0) {
        return;
    }
    remaining = recordCount;
    do {
        if (node->id != 0) {
            profileIndex = 0;
            while (selectionWorks[profileIndex] != NULL && profileIndex < 5) {
                u16 *profileEntries = selectionWorks[profileIndex]->entries;
                if ((u16)((profileEntries[node->id] >> 8) & 1)) {
                    *combinedEntry = (*combinedEntry & 0xFF0F) | 0x10;
                    break;
                }
                if (((*combinedEntry & 0xF0) >> 4) != 1) {
                    *combinedEntry = (*combinedEntry & 0xFF0F) | 0x20;
                }
                profileIndex++;
            }
        }
        remaining--;
        combinedEntry++;
        node++;
    } while (remaining != 0);
}

extern u32 prfGetIndexedProfileByte(u16, s32);

/* Scan the low-halfword entry id's five attributes into caller-owned summary words.
 * A nonzero summary[0] makes the next nonzero attribute report sentinel five.
 * Zero is treated as empty even after storing attribute index zero; neither
 * summary word is initialized here, and summary[1] keeps the latest value. */
void prfSummarizeNonzeroEntryAttributes(s32 entryId, s32 *summary) {
    s32 attributeIndex;
    s32 attributeValue;

    for (attributeIndex = 0; attributeIndex < PRF_ATTRIBUTE_SLOT_COUNT; attributeIndex++) {
        attributeValue = prfGetIndexedProfileByte(entryId & 0xFFFF, attributeIndex);
        if (attributeValue != 0) {
            summary[1] = attributeValue;
            if (summary[0] == 0) {
                summary[0] = attributeIndex;
            } else {
                summary[0] = PRF_ATTRIBUTE_MULTIPLE_SENTINEL;
                return;
            }
        }
    }
}

void func_0026DB20(void) {
}

extern void scrClearEntryFlag(DatPartyRecord *context, u16 entryId, u32 bit);

void func_0026DB28(DatPartyRecord *context, u8 entry) {
    scrClearEntryFlag(context, entry, 0xf);
}

INCLUDE_RODATA(const s32, "game/code_0026CD50", D_00425098);

