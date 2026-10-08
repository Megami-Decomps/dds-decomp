#include "common.h"
#include "sdf.h"
#include "btl.h"
#include "btl_state.h"
#include "btl_resource.h"
#include "btl_command.h"
#include "btl_ui.h"

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



extern const char *D_004367CC;

extern u32 kwlnTaskGetUserValue(void *);

extern void *kwlnTaskGetTaskByName(const char *name);

extern s32 btlGetRuntime(void);


extern u8 btlHasRequiredActorStatusBits(BtlUnit *node);


extern const char *btlCommandPanelTaskNameRef;
extern void func_001C3A38(BattleActorPanelWork *, s8);
extern void btlUpdateActorSlotStates(u8 *, s8);
extern void func_001C3DB0(ActionStateLink *, BattleActorPanelWork *, s8);

void func_001C35F0(ActionStateLink *actor, s8 mode, s8 value) {
    s32 count;
    u8 slot;
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *node = battle->units;
    void *task;
    BattleActorPanelWork *work;

    if (battle->battleFlags & 0x8000) {
        if (kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef) != NULL) {
            return;
        }
    }
    count = 0;
    slot = 0;
    for (; node != NULL; node = node->nextActor) {
        if (btlHasRequiredActorStatusBits(node) != 0) {
            slot = node->lookupId;
            if (actor->unit->owner == node->owner) {
                break;
            }
            count++;
        }
    }
    if (count >= 3) {
        return;
    }
    task = kwlnTaskGetTaskByName(D_004367CC);
    if (task == NULL) {
        return;
    }
    work = (BattleActorPanelWork *)kwlnTaskGetUserValue(task);
    func_001C3A38(work, mode);
    btlUpdateActorSlotStates((u8 *)work, 0);
    work->activeEntries[slot].presentation.presentationState = 2;
    work->activeEntries[slot].presentation.presentationValue = value;
    if (mode == 0) {
        func_001C3DB0(actor, work, 0);
    } else if (mode == 2) {
        func_001C3DB0(actor, work, 1);
    }
}
void btlUpdateActorSlotPresentationState(BtlUnit *object, s8 mode, s8 value) {
    s32 count = 0;
    u8 slot = 0;
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *node = battle->units;
    u8 *entry;
    UiSlotRow *slotEntry;
    void *task;
    s32 offset;

    for (; node != 0; node = node->nextActor) {
        if (btlHasRequiredActorStatusBits(node) != 0) {
            slot = node->lookupId;
            if (object->owner == node->owner) {
                break;
            }
            count++;
        }
    }
    if (count < 3) {
        task = kwlnTaskGetTaskByName(D_004367CC);
        if (task != 0) {
            entry = (u8 *)kwlnTaskGetUserValue(task);
            if (mode != 2) {
                func_001C3A38((BattleActorPanelWork *)entry, mode);
            }
            offset = slot * 0x290 + 0x10;
            slotEntry = (UiSlotRow *)(entry + offset);
            slotEntry->state = 2;
            slotEntry->value = value;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C3850);

void btlResetActorSlotPresentationValue(BtlUnit *object, BattleSceneObject *sceneObject) {
    s32 count = 0;
    u8 slot = 0;
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *node = battle->units;
    u8 *entry;
    s32 offset;
    for (; node != 0; node = node->nextActor) {
        if (btlHasRequiredActorStatusBits(node) != 0) {
            slot = object->lookupId;
            if (object->owner == node->owner) {
                break;
            }
            count++;
        }
    }
    if (count < 3) {
        entry = (u8 *)kwlnTaskGetUserValue(kwlnTaskGetTaskByName(D_004367CC));
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

extern f32 sdfSinPoly(f32);

INCLUDE_RODATA(const s32, "game/code_001C35F0", D_00416840);

INCLUDE_RODATA(const s32, "game/code_001C35F0", D_00416858);

INCLUDE_RODATA(const s32, "game/code_001C35F0", D_00416870);

void func_001C4520(BtlUnit *unit, BattleActorPanelWork *work, s32 slot, s8 reserve) {
    s32 i;

    switch (reserve == 0 ? work->activeEntries[slot].presentation.presentationState :
                           work->reserveEntries[slot].presentation.presentationState) {
    case 1:
        for (i = 0; i < 8; i++) {
            if (reserve == 0) {
                work->activeEntries[slot].presentation.highlightPhase[i] =
                    (work->activeEntries[slot].presentation.highlightPhase[i] + 8) % 360;
                work->activeEntries[slot].presentation.highlightLevel[i] =
                    (sdfSinPoly(((work->activeEntries[slot].presentation.highlightPhase[i] + 90) % 360) /
                               180.0f * 3.14159f) + 1.0f) * 0.5f * 64.0f + 16.0f;
            } else {
                work->reserveEntries[slot].presentation.highlightPhase[i] =
                    (work->reserveEntries[slot].presentation.highlightPhase[i] + 8) % 360;
                work->reserveEntries[slot].presentation.highlightLevel[i] =
                    (sdfSinPoly(((work->reserveEntries[slot].presentation.highlightPhase[i] + 90) % 360) /
                               180.0f * 3.14159f) + 1.0f) * 0.5f * 64.0f + 16.0f;
            }
        }
        break;
    case 2:
        break;
    case 3:
        for (i = 0; i < 8; i++) {
            if (reserve == 0) {
                if (work->activeEntries[slot].presentation.highlightLevel[i] != 0) {
                    work->activeEntries[slot].presentation.highlightLevel[i]--;
                }
            } else if (work->reserveEntries[slot].presentation.highlightLevel[i] >= 32) {
                work->reserveEntries[slot].presentation.highlightLevel[i] -= 32;
            } else {
                work->reserveEntries[slot].presentation.highlightLevel[i] = 0;
            }
        }
        break;
    case 0:
    case 4:
        for (i = 0; i < 8; i++) {
            if (reserve == 0) {
                if (work->activeEntries[slot].presentation.highlightLevel[i] >= 32) {
                    work->activeEntries[slot].presentation.highlightLevel[i] -= 32;
                } else {
                    work->activeEntries[slot].presentation.highlightLevel[i] = 0;
                }
            } else if (work->reserveEntries[slot].presentation.highlightLevel[i] >= 32) {
                work->reserveEntries[slot].presentation.highlightLevel[i] -= 32;
            } else {
                work->reserveEntries[slot].presentation.highlightLevel[i] = 0;
            }
        }
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C4900);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C4C58);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C50A0);

INCLUDE_RODATA(const s32, "game/code_001C35F0", D_00416898);

void func_001C53A0(BtlUnit *unusedUnit, BattleActorPanelWork *work, s32 slot) {
    s32 i;

    switch (work->activeEntries[slot].presentation.transitionState) {
    case 1:
    case 2: {
        for (i = 0; i < 8; i++) {
            work->activeEntries[slot].presentation.trianglePhase[i] =
                (work->activeEntries[slot].presentation.trianglePhase[i] + 8) % 360;
            work->activeEntries[slot].presentation.triangleAlpha[i] =
                (u32)((sdfSinPoly((f32)((work->activeEntries[slot].presentation.trianglePhase[i] + 90) % 360) /
                                  180.0f * 3.14159f) + 1.0f) * 0.5f * 64.0f + 16.0f);
        }
        break;
    }
    case 3:
        for (i = 0; i < 8; i++) {
            if (work->activeEntries[slot].presentation.triangleAlpha[i] != 0) {
                work->activeEntries[slot].presentation.triangleAlpha[i]--;
            }
        }
        break;
    case 0:
    case 4: {
        s32 index;
        for (i = 7, index = 0; i >= 0; i--, index++) {
            u8 alpha = work->activeEntries[slot].presentation.triangleAlpha[index];
            work->activeEntries[slot].presentation.triangleAlpha[index] =
                alpha < 32 ? 0 : alpha - 32;
        }
        break;
    }
    }
}

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C5610);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C5868);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C5D10);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C6010);


typedef struct BattlePanelColors {
    u32 values[4];
} BattlePanelColors;

extern const BattlePanelColors D_004168D8;
extern BtlResBlock *btlResourceBlock;
extern void func_00306C28(s32, s32, s32, u32 *, s32, struct EffectSlotSet *, s32, s32);

void func_001C6320(BtlUnit *unit, BattleStatPulse *pulse, s32 x, s32 y, s16 alpha, s32 unused, s32 stat) {
    BattlePanelColors colors = D_004168D8;
    f32 value;
    f32 maximum;
    s32 xOffset;
    s32 yOffset;
    s32 sprite;
    s32 targetProgress;
    s32 remaining;
    s32 i;

    if (!(stat & 1)) {
        value = unit->partyRecord.hp;
        maximum = unit->partyRecord.maxHp;
        xOffset = 57;
        yOffset = 49;
        sprite = 3;
    } else {
        value = unit->partyRecord.mp;
        maximum = unit->partyRecord.maxMp;
        xOffset = 28;
        yOffset = 64;
        sprite = 4;
    }
    if (maximum == 0.0f) maximum = 1.0f;
    targetProgress = (s32)(value / maximum * 39.0f);
    if (pulse->active == 0) {
        pulse->progress++;
        pulse->progress = pulse->progress <= 0 ? 0 : pulse->progress >= targetProgress ? targetProgress : pulse->progress;
        remaining = 39 - pulse->progress;
        remaining = remaining <= 0 ? 0 : remaining >= alpha ? alpha : remaining;
        if (pulse->progress >= targetProgress) {
            pulse->active = 1;
            pulse->phase = 260;
        }
        pulse->alpha = alpha - remaining;
    } else {
        pulse->phase = (pulse->phase + 16) % 360;
        pulse->alpha = (s32)((alpha + 119) * ((sdfSinPoly((f32)((pulse->phase + 90) % 360) / 180.0f * 3.14159f) + 1.0f) * 0.5f) + 8.0f);
        if (pulse->phase >= 160 && pulse->phase <= 180) {
            pulse->active = 0;
            pulse->phase = 0;
            pulse->progress = 0;
        }
    }
    for (i = 0; i < 4; i++) {
        colors.values[i] = (colors.values[i] & 0xFFFFFF00) | pulse->alpha;
    }
    if (!(unit->flags & 0x20) && value != 0.0f && !(unit->partyRecord.status & 0x4800)) {
        func_00306C28((x + xOffset + pulse->progress) << 4,
                     (y + yOffset + pulse->yOffset) << 3, 0, colors.values,
                     0, btlResourceBlock->resA, sprite, 0x53);
    }
}


INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C6648);

INCLUDE_RODATA(const s32, "game/code_001C35F0", D_004168C8);

INCLUDE_RODATA(const s32, "game/code_001C35F0", D_004168D8);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C68D0);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C6B98);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C7020);

INCLUDE_ASM(const s32, "game/code_001C35F0", func_001C7760);


extern SdfPoolNode D_003805A8;
extern s32 sdfAllocPacketAligned(s32 size);
extern void sdfInitPacketList(SdfListHead *list);
extern s32 sdfConsCreateDrawPacket(SdfListHead *list, SdfTex *texture, s32 context);
extern void sdfQueueGouraudTexturedQuad(
    s32 list, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 color0,
    s32 x1, s32 y1, s32 u1, s32 v1, s32 color1,
    s32 x2, s32 y2, s32 u2, s32 v2, s32 color2,
    s32 x3, s32 y3, s32 u3, s32 v3, s32 color3,
    s32 depth, s32 (*allocate)(s32));

/* Draw a textured command-panel quad with independently colored corners. */
s32 btlDrawGouraudTexturedPanelQuad(s32 x0, s32 y0, s32 x1, s32 y1,
                  s32 x2, s32 y2, s32 x3, s32 y3,
                  s32 u, s32 v, s32 width, s32 height,
                  const s32 *colors, s32 texture) {
    SdfListHead *list;
    s32 uFixed;
    s32 vFixed;
    s32 uRight;
    s32 vBottom;

    list = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    sdfConsCreateDrawPacket(list, (SdfTex *)texture, 0);
    uFixed = u * 0x10;
    vFixed = v * 0x10;
    uRight = uFixed + width * 0x10;
    vBottom = vFixed + height * 0x10;
    sdfQueueGouraudTexturedQuad((s32)list, 0x40,
        x0 * 0x10 + 0x7000, y0 * 8 + 0x7900, uFixed, vFixed, colors[0],
        x1 * 0x10 + 0x7000, y1 * 8 + 0x7900, uRight, vFixed, colors[1],
        x2 * 0x10 + 0x7000, y2 * 8 + 0x7900, uFixed, vBottom, colors[2],
        x3 * 0x10 + 0x7000, y3 * 8 + 0x7900, uRight, vBottom, colors[3],
        0xFEFFD0, NULL);
    D_003805A8.append((SdfListHead *)&D_003805A8, list);
    return 1;
}

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
extern const char *btlCommandPanelTaskNameRef;

s32 btlGetNamedTaskPairStatusOrUnavailable(void) {
    void *first;
    void *second;

    if (btlTrackedTaskHandles != 0) {
        first = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
        second = kwlnTaskGetTaskByName(D_004367CC);
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
extern void func_001C35F0(ActionStateLink *, s8, s8);

s32 btlUpdateCommandUiTransition(void) {
    BtlTrackedState *flow;
    void *task;
    s32 counter;

    if (btlTrackedTaskHandles != NULL) {
        task = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
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

