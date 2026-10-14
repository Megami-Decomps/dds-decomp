#include "common.h"
#include "mnu_callback_list.h"
#include "mnu_work.h"

extern MenuResourceRecord *mnuResourceRecords;

extern s32 mnuResourceRecordCount;

extern void (*sdfTickCallback)(void);

extern MenuProgressParameters D_0045C860;

extern MenuRuntimeCallback mnuRuntimeRecordInitializationCallback;

extern void mnuFreeOptionalBlock(u32);

extern void *memcpy(void *, const void *, u32);

extern void *memset(void *, s32, u32);

/* Cadence phase and accumulated progress at mnuStepCounterState (8 bytes). */

extern MenuProgressState mnuStepCounterState;

extern u16 mnuStepCounterThreshold[];

extern void func_0035A880(SdfListNode *);

/* Remove the matching ID entry and invoke its list listener. */
SdfListNode *mnuRemoveResourceNodeById(MnuCallbackList *list, u32 id) {
    SdfListNode *selectedNode = mnuFindResourceNodeById(list, id);
    if (selectedNode != 0) {
        return dds3RemoveListNodeAndNotify(list, selectedNode);
    }
    return NULL;
}

/* Notify and free every node, then reset the list. */
void mnuClearResourceList(MnuCallbackList *list) {
    SdfListNode *node;

    if (list != NULL) {
        node = list->head;
        if (node != NULL) {
            do {
                SdfListNode *current = node;
                node = node->next;
                list->onRemove(current->index, (u32)current->value);
                func_0035A880(current);
            } while (node != NULL);
        }
        list->tail = NULL;
        list->head = NULL;
        list->count = 0;
    }
}

typedef struct SdfLink {
    u8 pad00[8];
    struct SdfLink *prev;
    struct SdfLink *next;
} SdfLink;

typedef struct SdfLinkList {
    u32 pad00;
    SdfLink *prev;
    SdfLink *next;
} SdfLinkList;

/* Rewire non-null nodes and endpoints; neighbor writes precede saving links. */
void sdfLinkListExchangeNodes(SdfLinkList *list, SdfLink *firstNode, SdfLink *secondNode) {
    SdfLink *savedNext;
    SdfLink *savedPrev;

    if (firstNode != NULL && secondNode != NULL) {
        if (firstNode->prev != NULL) {
            firstNode->prev->next = secondNode;
        }
        if (firstNode->next != NULL) {
            firstNode->next->prev = secondNode;
        }
        if (secondNode->prev != NULL) {
            secondNode->prev->next = firstNode;
        }
        if (secondNode->next != NULL) {
            secondNode->next->prev = firstNode;
        }
        savedNext = firstNode->next;
        firstNode->next = secondNode->next;
        savedPrev = firstNode->prev;
        firstNode->prev = secondNode->prev;
        secondNode->next = savedNext;
        secondNode->prev = savedPrev;
        if (firstNode->next == NULL) {
            list->prev = firstNode;
        }
        if (firstNode->prev == NULL) {
            list->next = firstNode;
        }
        if (secondNode->next == NULL) {
            list->prev = secondNode;
        }
        if (secondNode->prev == NULL) {
            list->next = secondNode;
        }
    }
}

SdfListNode *mnuFindResourceNodeByValue(MnuCallbackList *list, u32 value) {
    SdfListNode *node = list->head;
    if (node == NULL) {
        return NULL;
    }
    do {
        if (node->key == value) {
            break;
        }
        node = node->next;
    } while (node != NULL);
    return node;
}

SdfListNode *mnuFindResourceNodeById(MnuCallbackList *list, u32 id) {
    SdfListNode *node = list->head;
    if (node == NULL) {
        return NULL;
    }
    do {
        if (node->index == id) {
            break;
        }
        node = node->next;
    } while (node != NULL);
    return node;
}

SdfListNode *mnuFindResourceNodeByHandle(MnuCallbackList *list, u32 handle) {
    SdfListNode *node = list->head;
    if (node == NULL) {
        return NULL;
    }
    do {
        if ((u32)node->value == handle) {
            break;
        }
        node = node->next;
    } while (node != NULL);
    return node;
}

/* Default callback-list removal/destruction listener ignores both supplied words. */
void mnuCallbackNodeNoOp(u32 unusedFirstWord, u32 unusedSecondWord) {
}

MenuProgressParameters *mnuGetResourceProgressParameters(void) {
    return &D_0045C860;
}

void mnuCopyResourceProgressParameters(MenuProgressParameters *parameters) {
    memcpy(&D_0045C860, parameters, sizeof(MenuProgressParameters));
}

MenuProgressState *mnuGetResourceProgressStepState(void) {
    return &mnuStepCounterState;
}

/* Change the cadence without resetting its phase or accumulated progress. */
void mnuSetResourceProgressCadence(u8 cadenceLimit, u8 progressStep) {
    mnuStepCounterState.cadenceLimit = cadenceLimit;
    mnuStepCounterState.progressStep = progressStep;
}

/* Clear all state bytes before installing the new cadence. */
void mnuResetProgressLimitAndStep(u8 cadenceLimit, u8 progressStep) {
    memset(&mnuStepCounterState, 0, 8);
    mnuStepCounterState.cadenceLimit = cadenceLimit;
    mnuStepCounterState.progressStep = progressStep;
}

/* Only a cadence boundary adds progress and checks the completion threshold. */
s32 mnuAdvanceCursorStepUntilThreshold(void) {
    if (++mnuStepCounterState.cadenceCount >= mnuStepCounterState.cadenceLimit) {
        mnuStepCounterState.cadenceCount = 0;
        mnuStepCounterState.progress += mnuStepCounterState.progressStep;
        if (mnuStepCounterState.progress >= mnuStepCounterThreshold[0]) {
            return 1;
        }
    }
    return 0;
}

/* Reset the counters while preserving the configured limit and step. */
void mnuResetResourceProgressCounters(void) {
    mnuStepCounterState.cadenceCount = 0;
    mnuStepCounterState.progress = 0;
}

void mnuBindResourceRecordTable(MenuResourceRecord *records, s32 count) {
    mnuResourceRecords = records;
    mnuResourceRecordCount = count;
}

/* The externally owned table stores 28-byte records. */
MenuResourceRecord *mnuGetResourceRecordByIndex(s32 recordIndex) {
    return &mnuResourceRecords[recordIndex];
}

s32 mnuTriggerProgressResourceRecords(MenuProgressState *progressState) {
    s32 i;

    for (i = 0; i < mnuResourceRecordCount; i++) {
        MenuRegistry *registry = mnuGetMenuRecordRegistryEntry(mnuResourceRecords[i].tag);
        MenuResourceRecord *record = (MenuResourceRecord *)(u32)((u32)(i * sizeof(MenuResourceRecord)) + (u32)mnuResourceRecords);
        s32 margin = registry->parameterIndex == 0x22 ? 0x96 : 0;

        if (!(record->flags & 1) &&
            (s32)progressState->progress + margin + 0x20 >= record->progress) {
            MenuWorkEntry *entry;

            record->flags |= 1;
            entry = mnuFindUnusedWorkEntry();
            if (entry != 0) {
                mnuInitializeRegistryWorkEntry(
                    entry, record->tag, i, record->x,
                    (s32)progressState->progress - record->progress,
                    (f32)record->angleDegrees * 3.1415926f / 180.0f);
                entry->flags |= 0x10;
                if (registry->parameterIndex == 0x22) {
                    entry->control.word =
                        ((entry->control.word | 0x100) & 0xFFFE01FF) | 0x19000;
                }
            }
        }
    }
    return 0;
}

/* Default post-initialization callback leaves the runtime record unchanged. */
void mnuRuntimeRecordInitializationNoOp(MenuRuntimeRecord *record) {
}

void mnuSetRuntimeRecordInitializationCallback(MenuRuntimeCallback callback) {
    mnuRuntimeRecordInitializationCallback = callback;
}

void mnuReleaseResourceBlockCallback(u32 unusedFirstWord, u32 resourceWord) {
    if (resourceWord != 0) {
        mnuFreeOptionalBlock(resourceWord);
        return;
    }
}

INCLUDE_SDATA(const s32, "game/code_00320FD0", mnuRuntimeRecordInitializationCallback);

