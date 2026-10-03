#include "common.h"

typedef struct UiSlotEntry {
    u8 pad00[0x18];
    s8 state; /* 0x18 */
    u8 pad19[0x277];
} UiSlotEntry; /* 0x290 */

typedef struct UiSlotRow {
    u8 pad00[0x10];
    u8 state;
    u8 value;
} UiSlotRow;

typedef struct UiInputState {
    u8 pad00[0x3C];
    u32 flags; /* 0x3C */
} UiInputState;



extern u32 D_004367CC;

extern u32 kwlnTaskGetUserValue(void *);

extern void *func_00101740(const char *name);

extern s32 btlGetRuntime(void);

typedef struct UiSceneNode {
    u8 pad00[0x108];
    s64 key;
    u8 pad110[0xC];
    u8 slot;
    u8 pad11D[0x247];
    struct UiSceneNode *next;
} UiSceneNode;

extern s32 btlHasRequiredActorStatusBits(UiSceneNode *node);


INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C35F0);
void btlUpdateActorSlotPresentationState(UiSceneNode *object, s8 mode, s8 value) {
    s32 count = 0;
    u8 slot = 0;
    UiSceneNode *node = *(UiSceneNode **)(btlGetRuntime() + 0x24C);
    u8 *entry;
    UiSlotRow *slotEntry;
    void *task;
    s32 offset;

    for (; node != 0; node = node->next) {
        if (btlHasRequiredActorStatusBits(node) != 0) {
            slot = node->slot;
            if (object->key == node->key) {
                break;
            }
            count++;
        }
    }
    if (count < 3) {
        task = func_00101740(D_004367CC);
        if (task != 0) {
            entry = (u8 *)kwlnTaskGetUserValue(task);
            if (mode != 2) {
                func_001C3A38(entry, mode);
            }
            offset = slot * 0x290 + 0x10;
            slotEntry = (UiSlotRow *)(entry + offset);
            slotEntry->state = 2;
            slotEntry->value = value;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C3850);

void btlResetActorSlotPresentationValue(UiSceneNode *object) {
    s32 count = 0;
    u8 slot = 0;
    UiSceneNode *node = *(UiSceneNode **)(btlGetRuntime() + 0x24C);
    u8 *entry;
    s32 offset;
    for (; node != 0; node = node->next) {
        if (btlHasRequiredActorStatusBits(node) != 0) {
            slot = object->slot;
            if (object->key == node->key) {
                break;
            }
            count++;
        }
    }
    if (count < 3) {
        entry = (u8 *)kwlnTaskGetUserValue(func_00101740(D_004367CC));
        offset = slot * 0x290 + 0x10;
        entry += offset;
        *(u8 *)(entry + 0x10) = 2;
        *(u8 *)(entry + 0x11) = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C3A38);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C3BB0);

void btlUpdateActorSlotStates(u8 *context, s8 mode) {
    u8 *entry = context + 0x80;
    s32 modeZeroState = 3;
    s32 modeNonzeroState = 4;
    s32 i = 2;
    do {
        s32 state = entry[0xC];
        if (state == 1 || state == 2) {
            entry[0xC] = mode == 0 ? modeZeroState : modeNonzeroState;
        }
        i--;
        entry += 0x290;
    } while (i >= 0);
}

void btlAdvancePendingSceneSlotStates(u8 *scene) {
    UiSlotEntry *entry = (UiSlotEntry *)(scene + 0xE0);
    s32 i;

    for (i = 0; i < 3; i++, entry++) {
        if (entry->state == 1) {
            entry->state = 5;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C3DB0);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C3EC0);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C43F8);

INCLUDE_RODATA(const s32, "game/code_001C35F0", D_00416840);

INCLUDE_RODATA(const s32, "game/code_001C35F0", D_00416858);

INCLUDE_RODATA(const s32, "game/code_001C35F0", D_00416870);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C4520);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C4900);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C4C58);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C50A0);

INCLUDE_RODATA(const s32, "game/code_001C35F0", D_00416898);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C53A0);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C5610);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C5868);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C5D10);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C6010);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C6320);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C6648);

INCLUDE_RODATA(const s32, "game/code_001C35F0", D_004168C8);

INCLUDE_RODATA(const s32, "game/code_001C35F0", D_004168D8);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C68D0);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C6B98);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C7020);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C7760);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C7BA8);

/* Tracked-task state: the word at +0x3C holds status flags, and its low byte is read as the signed status code. */
typedef struct BtlTrackedState {
    u8 pad00[0x3C];
    union {
        u32 flags;
        s8 status;
    } word; /* 0x3C */
    s32 counter;
    s32 threshold;
} BtlTrackedState;
extern BtlTrackedState *btlTrackedTaskHandles;
extern u32 btlCommandPanelTaskNameRef;

s32 btlGetNamedTaskPairStatusOrUnavailable(void) {
    s64 first;
    s64 second;

    if (btlTrackedTaskHandles != 0) {
        first = func_00101740(btlCommandPanelTaskNameRef);
        second = func_00101740(D_004367CC);
        if (first == 0 && second == 0) {
            return -128;
        }
        if (btlTrackedTaskHandles->word.flags & 0x100) {
            return 0;
        }
        return btlTrackedTaskHandles->word.status;
    }
    return -128;
}

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C7DB8);

extern s32 btlAreLinkedSceneCountersAtThreshold(void);
extern void func_001C35F0(s32, s32, s32);

s32 func_001C7F10(void) {
    BtlTrackedState *flow;
    void *task;
    s32 counter;

    if (btlTrackedTaskHandles != NULL) {
        task = func_00101740(btlCommandPanelTaskNameRef);
        flow = btlTrackedTaskHandles;
        switch (flow->word.status) {
        case 1:
            flow->counter++;
            if (flow->counter >= flow->threshold) {
                flow->word.status = 2;
            }
            break;
        case 3:
            counter = flow->counter + 1;
            flow->counter = counter;
            if (counter < flow->threshold) {
                break;
            }
            counter = counter <= 0 ? 0 :
                (counter < flow->threshold ? counter : flow->threshold);
            flow->counter = counter;
            if (btlAreLinkedSceneCountersAtThreshold() != 0) {
                if (task != NULL) {
                    func_001C35F0(*(s32 *)(kwlnTaskGetUserValue(task) + 0x2C), 2, 0);
                }
                btlTrackedTaskHandles->word.status = 0;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_001C35F0", D_00416920);

INCLUDE_RODATA(const s32, "game/code_001C35F0", D_00416938);

INCLUDE_RODATA(const s32, "game/code_001C35F0", D_00416948);

