#include "common.h"
#include "eff.h"
#include "itf.h"
#include "btl_state.h"
#include "btl_ui.h"
#include "sdf.h"
#include "btl_action.h"

typedef struct BattlePanelEdgeWork {
    u8 pad00[0x31];
    s8 edgePhase;
    u8 pad32[0x1E];
    f32 corners[4][4];
} BattlePanelEdgeWork;

extern f32 D_00415F80[4];

extern s32 func_001ABB10(BtlUnit *, s32);

typedef struct UiQuadColor {
    s32 red;
    s32 green;
    s32 blue;
    s32 alpha;
} UiQuadColor;

extern const UiQuadColor D_00414D50;

extern u32 D_004367F8;

extern u32 D_004367CC;

extern s32 dds3FindEntryIndex();

extern s32 btlGetIndexedPartyEntryRecord(s32);

extern s32 func_00101820(u32);

extern s8 D_003B4CF8[13];

extern u32 sndTestMessageResourceIndex;

extern s32 sndTestMessageTexture;

extern s32 itfLoadTextureFromAsset(u32);

extern u64 D_004366E8;

extern s32 btlRuntime;

extern s32 D_00435DE0;

extern s32 D_00435DF0;

extern s32 D_00435DE4;

extern s32 D_00435DF8;

extern s32 datItemSkillRecords;

extern s32 btlGetRuntime(void);

extern BattleCmdPanel *btlCommandPanelWork;

extern s32 datGameState;

extern u32 btlMahenPanelTaskNameRef;

extern s64 kwlnTaskGetTaskByName();

extern s64 kwlnTaskIsRegistered(s64);

extern u32 kwlnTaskGetUserValue();

extern u32 btlAnalyzPanelTaskNameRef;

extern u32 D_004367E0;

extern u32 D_004367D8;

extern u32 D_004367C8;

extern u32 D_004367C4;

extern u32 D_00438F4C;

extern u32 D_00438F50;

extern s8 D_0037F530[];

extern u8 D_003B4DC0[];

extern s32 D_00435E00;

extern void *D_003B4E88[];
typedef struct EncBgEntry {
    s32 unk00;
    s16 unk04;
    s16 unk06;
    s16 unk08;
    s16 unk0A;
} EncBgEntry;

typedef struct EncBgRow {
    s32 unk00;
    EncBgEntry entries[64];
} EncBgRow;

extern EncBgRow *D_00435E14;

extern s32 fldConsumeNextSceneRequest(s32 *, s32 *);


typedef struct ActorEntrySlot {
    s16 code;
    s16 unk02;
    s16 countdown;
} ActorEntrySlot;

typedef struct UiObject {
    u8 unk_00[0x110];
    u32 flags;
    u32 actionFlags;
    u8 unk_118[8];
    u16 entryMask;
    u8 entryDataTail[2];
    u16 index;
    u16 currentValue;
    u16 maximumValue;
    u8 unk_12A[4];
    u16 statusFlags;
    u16 conditionFlags;
    u8 pad_132[2];
    u16 stat134;
    u8 pad_136[0x1AE];
    u8 kind;
    u8 pad_2E5;
    ActorEntrySlot entrySlots[7];
    s32 selectedEntryIndex;
    u32 marker;
    u8 pad_318[0x4C];
    struct UiObject *next;
} UiObject;

/* Actor-task record; the separate unit-data list uses UiObject. */
typedef struct SceneTask {
    s32 state;
    u8 pad04[4];
    u32 flags;
    u8 pad0C[0xC];
    UiObject *actor;
    u8 pad1C[0x134];
    s32 actions[8]; /* 0x150 */
    u8 pad170[8];
    struct SceneTask *next; /* 0x178 */
} SceneTask;

typedef struct BattleItemDrop {
    u16 id;
    u8 count;
    u8 pad03;
} BattleItemDrop;

typedef struct BattleController {
    u8 pad_000[0x214];
    s32 frame;
    u32 flags;
    u32 flags21C;
    u8 pad_220[0x28];
    SceneTask *taskHead; /* 0x248 */
    UiObject *actors;
    u8 pad_250[0x1E];
    u8 mode; /* 0x26E: mode 3 scales defeat experience in DDS2. */
    u8 pad26F;
    u16 variant; /* 0x270 */
    u8 pad_272[0x22];
    s32 adjustmentRecordIndex; /* 0x294 */
    s32 adjustmentGroupIndex;  /* 0x298 */
    s32 adjustmentEntryIndex;  /* 0x29C */
    u8 pad_2A0[0x24];
    s32 drawTask;
    u8 pad2C8[0x14];
    BattleItemDrop itemDrops[3];
    s32 moneyEarned; /* 0x2E8 */
    u8 pad2EC[4];
    s32 experienceEarned; /* 0x2F0 */
    s32 epEarned; /* 0x2F4 */
    u8 pad2F8[4];
    u16 specialEnemyDefeats; /* 0x2FC: defeated enemy kinds 100 through 103. */
    u8 pad2FE[0x3DE];
    s32 (*commandRangeOverride)(UiObject *, s32);
} BattleController;

typedef struct BattleEffect {
    u32 flags;
    u8 pad_004[0xA0];
    u32 resourceHandles[3];
} BattleEffect;

extern s32 D_003B4F70[];

extern s32 datCommandRecords;
extern u32 datCalculateCommandBaseValue(s32, s32);

extern BattleTrackedTaskWork *btlTrackedTaskHandles;

extern s32 btlGetTrackedTaskHandle(s32);

extern s32 D_003B6928[];

extern s32 datEnemyRecords;

extern s32 datCommandSelectors;

extern char D_00415158[];

extern void btlBossDebugPrintf(const char *, ...);
extern SdfMemBlock *sdfAllocGeneralBlock(s32);
extern u32 sdfResourceRetainAddress(SdfMemBlock *);
extern void sdfReleaseResourceAllocation(SdfMemBlock *);

extern s8 effSharedRandomState[];

extern void *D_003B4E40[];

extern s32 D_003B4F78[];

extern s32 D_003B4F74[];

extern f32 D_003B4E28[];

extern s32 btlCheckSpecialAbility(s32, s32);

extern s32 mdlFlagTest(s32);

extern u32 mnuGetPartyEntryCurrentId(s32);

extern s8 *datAffinityRecords;

extern s32 btlDoesEnabledStatusMatchCurrentId(s32, u32);

extern s32 evtCheckValueThreshold(s32, s32);

extern void mdlFlagSet(s32);

extern void mdlFlagClear(s32);

extern char D_00415638[]; /* "btl:hunt mp rec[%d]\n" */

extern u8 *datBattleSceneRecords;

/* Native per-species AI record, shared layout with the action evaluator. */
typedef struct AiSlot {
    u8 weight;
    u8 pad1;
    u16 actionId;
    u32 actionArg;
} AiSlot;

typedef struct AiSpecies {
    u8 unk00;
    u8 pad01[0x3F];
    AiSlot slot[25];
    u8 pad108[0x54];
} AiSpecies;

extern AiSpecies *datEnemyAiRecords;

extern s32 func_001B32F8(s32, s32 *);

extern char D_00415840[]; /* "btl:endure=%d%%[ratio=%.2f]\n" */
extern s32 fldAreaState[];
extern u8 D_003B4EC8[];
extern char D_004159A0[];

extern BattleSelectionWork *btlLinkedSelectionTaskBuffer;

typedef struct SndMessageNode {
    struct SndMessageNode *previous;
    struct SndMessageNode *next;
    s32 message;
    void *object;
} SndMessageNode;

typedef struct SoundQueue {
    s32 unk00;
    s32 allocation;
    s32 unk08;
    u16 drawFlags;
    u16 unk0E;
    SndMessageNode *head;
    SndMessageNode *tail;
} SoundQueue;

extern SoundQueue itfMesWork;

extern void itfMesDestroyWindow(s32 arg0);

extern void sdfTexReleaseReferenceViaHandler(s32 arg0);

extern s32 sndUpdateTestMsgTask(void);

typedef struct SndDev {
    u8 unk0[0x10];
    void (*submitPacket)(void *, s32);
} SndDev;

extern SndDev D_003805A8;

extern u8 D_003B4D08[];

extern u8 D_003B4D18[];

extern u8 D_003B4D28[];

extern s32 sdfAllocPacketAligned(s32 size);

extern void sdfInitPacketList(s32 mem);

extern void itfSendTablePacket(s32 arg0, s32 arg1, s32 arg2);

extern void itfQueueTextureBoundQuadPacket(void *arg0, void *arg1, void *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

typedef struct UiPanelPlacement {
    UiSprite *frame;
    UiSprite *sprite;
    UiSprite *overlay;
    s32 bounds[4]; /* x, y, right, bottom */
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 fadeLimit;
} UiPanelPlacement;
typedef struct UiTexRef { u8 pad0[4]; s32 unk4; s32 unk8; s32 unkC; } UiTexRef;
typedef struct UiPos { s32 x; s32 y; s32 unk8; UiTexRef *chain; } UiPos;
typedef struct UiPanelOrigin { s32 x; s32 y; UiTexRef *tex; } UiPanelOrigin;
typedef struct SndSeqSelect {
    u8 pad0[8];
    s32 seq;
    u8 padC[6];
    s16 current;
    s16 saved;
    s16 count;
    u8 pad18[0xA];
    s16 entryCount;
} SndSeqSelect;
typedef struct UiPanel {
    u32 flags;
    u8 pad4[8];
    s32 unkC;
    s16 index;
    s16 state;
    UiPanelOrigin origin;
    s32 pad20;
    UiPos pos;
    u8 pad34[0xC];
    SndSeqSelect selection;
    u8 pad64[0x40];
    UiPanelPlacement place;
    u8 padD0[0x100];
    BtlFade fade;
} UiPanel;
extern s32 func_0019DBA8();
extern UiSprite *func_001A1858(s32, u32);
extern void itfSetPanelLayoutAndNotify();
extern void itfPanelUpdateValuesAndNotify();

typedef struct UiCursor {
    s32 x;
    s32 y;
    s32 unk8;
    s32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    u8 unk14;
    s8 unk15;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
} UiCursor;

extern void itfAdvancePanelLayoutAndNotify(UiSprite *sprite, s32 a, s32 b, s32 c, s32 d, s32 e);
extern void itfMesOffsetNodeChain(UiTexRef *node, s32 dx, s32 dy);


extern void itfMesSetRowItemFlag();
extern void sndSetSequenceVolumePan();
extern void sndStepSequenceIndex(SndSeqSelect *sel, s32 dir);
typedef struct SndPad {
    u8 pad00[0x21];
    s8 confirm;
    u8 pad22[4];
    s8 prev;
    s8 next;
    u8 pad28[9];
    s8 unk31;
    u8 pad32[2];
    s8 coarseDown;
    s8 coarseUp;
    s8 unk36;
    s8 unk37;
    s8 fineDown;
    u8 pad39;
    s8 fineUp;
} SndPad;
extern SndPad D_0037F510;
extern s32 func_001A6AB8();
extern void itfResetBattleFadeState(s32, s32);

typedef struct UiSurface {
    u8 pad00[0x10];
    void (*submit)(struct UiSurface *, s32);
    u8 pad14[0xC];
} UiSurface;
typedef struct UiOwnerRef { u8 pad0[0xC]; struct UiPanel *owner; } UiOwnerRef;
extern UiSurface kwlnDrawSurfaces[];
extern UiOwnerRef *D_003B4778[];
extern void itfBuildAndSubmitPanelPacket(UiSprite *sprite, UiSurface *surface);
extern void func_001A7798(UiSprite *sprite);

/* Party/enemy entry; the HP/MP/status prefix is shared with DDS1. */
typedef struct BtlEntry {
    u16 flags;
    u8 pad2[4];
    u16 hp;
    u16 maxHp; /* 0x08: cached skill-adjusted maximum */
    u16 mp;
    u16 maxMp; /* 0x0C: cached skill-adjusted maximum */
    u16 status;
    u8 pad10[4];
    u16 unk14;
    u8 unk16[5];
    u8 pad1B[0x191];
    u16 unk1AC;
    u16 unk1AE;
    u16 unk1B0;
    u8 pad1B2[0x12];
} BtlEntry;

#define BTL_ENTRY_STATUS_MASK 0x7FFF
extern s32 datComputeSkillBoostedMaxHp();
extern s32 datComputeSkillBoostedMaxMp();

/* Enable context rendering for every font object in the linked chain. */
void frFontEnableNodeContextModes(s32 fontObject) {
    for (; fontObject != 0; fontObject = *(s32 *)(fontObject + 0x24)) {
        frFontEnableContextMode(fontObject);
    }
}

void itfMesInitializePanelPlacementSprite(UiPanel *panel) {
    UiPos *pos = &panel->pos;
    UiPanelPlacement *place = &panel->place;
    s32 top;
    if (place->sprite == 0) {
        place->sprite = func_001A1858(6, itfMesWork.allocation);
        if (place->frame != 0) {
            place->sprite->unk20 = panel->origin.tex->unk4 + func_0019DBA8(0, panel->origin.tex);
        }
    }
    top = pos->y + place->bounds[1];
    itfSetPanelLayoutAndNotify(place->sprite, pos->x + place->bounds[0], top, pos->x + place->bounds[2], pos->y + place->bounds[3], panel->unkC);
    place->sprite->screenY = top;
    itfPanelUpdateValuesAndNotify(place->sprite, place->unk1C, place->unk20, place->unk24, 0);
    panel->flags = (panel->flags & ~0x300) | 0x100;
}

void itfMesCreatePanelOriginFrameWhenVisible(UiPanel *panel) {
    UiPanelOrigin *origin = &panel->origin;
    UiPanelPlacement *place = &panel->place;
    if (origin->tex != 0 && !(panel->flags & 0x10000)) {
        if (place->frame == 0) {
            s32 width = origin->tex->unkC * 16;
            place->frame = func_001A1858(7, itfMesWork.allocation);
            itfSetPanelLayoutAndNotify(place->frame, origin->x - 0x2D0, origin->y - 0x68, origin->x + width + 0x2D0, origin->y + 0x110, panel->unkC);
            itfPanelUpdateValuesAndNotify(place->frame, 0x7F, 0x7F, 0x7F, 0);
        }
        panel->flags = (panel->flags & ~0x3000) | 0x1000;
    } else if (place->frame != 0) {
        panel->flags |= 0x3000;
    }
}

void itfResetCursorPositionAndState(u32 *arg0, s32 arg1) {
    if (arg1 != 0) {
        *arg0 = 0x280;
        arg0[1] = 0xa10;
    }
    arg0[2] = 0;
    *(u16 *)(arg0 + 3) = 0xffff;
}

void itfMesResetCursorState(UiCursor *cur, s32 resetPos) {
    if (resetPos != 0) {
        cur->x = 0x4B0;
        cur->y = 0xAF8;
    }
    cur->unkC = 0;
    cur->unk10 = 0;
    cur->unk11 = 0;
    cur->unk16 = 0;
    cur->unk18 = 0;
    cur->unk1A = 0;
    cur->unk12 = 0;
    cur->unk13 = 0;
    cur->unk14 = 0;
    cur->unk15 = -0x80;
    cur->unk8 = 0;
}

void itfInitializeCursorResetState(s32 object) {
    *(u32 *)(object + 0) = 0x560;
    *(u32 *)(object + 4) = 0xC88;
    *(s32 *)(object + 8) = 0;
    *(s32 *)(object + 0xC) = 0;
    *(s16 *)(object + 0x10) = 0;
    *(s16 *)(object + 0x12) = -1;
    *(s16 *)(object + 0x14) = -1;
    *(s16 *)(object + 0x16) = 0;
    *(s32 *)(object + 0x18) = 0;
    *(s32 *)(object + 0x1C) = 0;
    *(s16 *)(object + 0x20) = 0;
    *(s16 *)(object + 0x22) = 0;
}

extern void func_001A6078();

void itfResetWindowResourceBlock(s32 *object) {
    object[0] = 0;
    object[1] = 0;
    object[2] = 0;
    func_001A6078(object, 0, 0);
}

/* Paired arrays: a nonzero slot marker owns the handle at the same index. */
typedef struct UiResourceSlots {
    s32 markers[32];
    SdfMemBlock *handles[32];
} UiResourceSlots;

/* Clear 32 words, from the end back toward the beginning of the buffer. */
void itfClearDrawStateWords(UiResourceSlots *slots) {
    s32 remaining;
    u32 *word;

    word = (u32 *)&slots->markers[31];
    remaining = 0x1f;
    do {
        remaining = remaining - 1;
        *word = 0;
        word = word + -1;
    } while (-1 < remaining);
}

void itfResetBattleFadeState(s32 arg0, s32 arg1) {
    if (arg1 == 0) {
        *(u8 *)arg0 = 0;
    }
    *(u16 *)(arg0 + 2) = 0;
    *(u16 *)(arg0 + 6) = 0;
    *(u16 *)(arg0 + 4) = 0x40;
    *(u32 *)(arg0 + 8) = 0;
}

void btlSetFadePhaseAlphaTimer(s32 arg0, s16 arg1, s16 arg2, s16 arg3) {
    *(s16 *)(arg0 + 2) = arg1;
    *(s16 *)(arg0 + 4) = arg2;
    *(s16 *)(arg0 + 6) = arg3;
}

void btlReleaseEffectResourceHandles(BattleEffect *effect) {
    u32 *handles = effect->resourceHandles;
    if (handles[0] != 0) {
        itfPanelReleasePrimitiveResources(handles[0]);
        handles[0] = 0;
    }
    if (handles[1] != 0) {
        itfPanelReleasePrimitiveResources(handles[1]);
        handles[1] = 0;
    }
    if (handles[2] != 0) {
        itfPanelReleasePrimitiveResources(handles[2]);
        handles[2] = 0;
    }
    effect->flags &= ~0xF00;
}

/* Release the handles in the second half for occupied entries in the first. */
void itfReleaseUiResourceSlotHandles(UiResourceSlots *slots) {
    s32 remaining;
    s32 *entries = slots->markers;

    remaining = 0x1f;
    do {
        if (*entries != 0) {
            sdfReleaseResourceAllocation(slots->handles[entries - slots->markers]);
            *entries = 0;
        }
        remaining = remaining - 1;
        entries = entries + 1;
    } while (-1 < remaining);
}

u16 *txtFormatNumberU16(s32 value, u16 *out) {
    s32 digits[10];
    s32 count = 0;
    s32 i;
    do {
        digits[count] = value % 10;
        value = value / 10;
        count++;
    } while (value > 0 && count < 10);
    for (i = count - 1; i >= 0; i--) {
        *out++ = (digits[i] << 8) - 0x6F80;
    }
    *out = 0;
    return out;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A6078);

void itfUpdateBattleDisplayAndFadeIndicator(u32 arg0) {
    itfMesUpdatePanelFades();
    func_001A6350(arg0);
    func_001A6528(arg0);
    btlUpdateFadeIndicator(arg0);
}

void itfMesUpdatePanelFades(UiPanel *panel) {
    UiPanelPlacement *place = &panel->place;
    s32 *sprite;
    s32 transition;

    /* The panel fade word is at +0x38 in each sprite record. */
    sprite = (s32 *)place->sprite;
    transition = panel->flags & 0x300;
    switch (transition) {
    case 0x100:
        sprite[14] += 24;
        if (sprite[14] >= place->fadeLimit || panel->state == 3) {
            sprite[14] = place->fadeLimit;
            panel->flags = (panel->flags & ~0x307) | 0x203;
        }
        break;
    case 0x300:
        sprite[14] -= 8;
        if (sprite[14] <= 0 || panel->state == 3) {
            sprite[14] = 0;
            panel->flags &= ~0x300;
        }
        break;
    }

    sprite = (s32 *)place->frame;
    transition = panel->flags & 0x3000;
    switch (transition) {
    case 0x1000:
        sprite[14] += 32;
        if (sprite[14] >= 200) {
            sprite[14] = 200;
            panel->flags = (panel->flags & ~0x3000) | 0x2000;
        }
        break;
    case 0x3000:
        sprite[14] -= 32;
        if (sprite[14] <= 0) {
            sprite[14] = 0;
            panel->flags &= ~0x3000;
            itfPanelReleasePrimitiveResources(sprite);
            place->frame = NULL;
        }
        break;
    }

    sprite = (s32 *)place->overlay;
    transition = panel->flags & 0xC00;
    switch (transition) {
    case 0x400:
        sprite[14] += 24;
        if (sprite[14] >= place->fadeLimit) {
            sprite[14] = place->fadeLimit;
            panel->flags = (panel->flags & ~0xC07) | 0x803;
        }
        break;
    case 0xC00:
        sprite[14] -= 8;
        if (sprite[14] <= 0) {
            sprite[14] = 0;
            panel->flags &= ~0xC00;
            itfPanelReleasePrimitiveResources(sprite);
            place->overlay = NULL;
        }
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A6350);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A6528);

void itfMesShiftPanelVertically(UiPanel *panel, s32 dy) {
    UiPanelOrigin *origin = &panel->origin;
    UiPos *pos = &panel->pos;
    UiPanelPlacement *place = &panel->place;
    pos->y += dy;
    itfMesOffsetNodeChain(pos->chain, 0, dy);
    if (place->sprite != 0) {
        itfAdvancePanelLayoutAndNotify(place->sprite, 0, dy, 0, 0, 0);
    }
    if (place->frame != 0) {
        itfAdvancePanelLayoutAndNotify(place->frame, 0, dy, 0, dy, 0);
    }
    if (origin->tex != 0) {
        origin->y += dy;
        itfMesOffsetNodeChain(origin->tex, 0, dy);
    }
}

s32 sndSeqSelectPoll(s32 obj) {
    SndSeqSelect *sel = (SndSeqSelect *)(obj + 0x40);
    s32 dir = 0;
    s32 index;
    if (D_0037F510.prev & 2) {
        if (sel->current != 0) {
            dir = -1;
        } else if (D_0037F510.prev < 0) {
            dir = -1;
        }
    } else if (D_0037F510.next & 2) {
        if (sel->current != sel->count - 1 || D_0037F510.next < 0) {
            dir = 1;
        }
    }
    if (dir != 0) {
        sndStepSequenceIndex(sel, dir);
        itfResetBattleFadeState(obj + 0x1D0, 1);
    }
    if (D_0037F510.confirm < 0) {
        sndSetSequenceVolumePan(8, 0x7F, 0x3F);
        return 1;
    }
    if (sel->entryCount > 0 && (index = func_001A6AB8(sel)) >= 0) {
        sndSetSequenceVolumePan(8, 0x7F, 0x3F);
        if (index != sel->current) {
            itfMesSetRowItemFlag(sel->seq, sel->current, sel->count, 0);
            itfMesSetRowItemFlag(sel->seq, index, sel->count, 1);
            sel->current = index;
            sel->saved = index;
        }
        return 1;
    }
    return 0;
}


void sndStepSequenceIndex(SndSeqSelect *sel, s32 dir) {
    s32 index = sel->current;
    itfMesSetRowItemFlag(sel->seq, index, sel->count, 0);
    if (dir < 0) {
        index--;
        if (index < 0) {
            index = sel->count - 1;
        }
    } else {
        index++;
        if (index >= sel->count) {
            index = 0;
        }
    }
    itfMesSetRowItemFlag(sel->seq, index, sel->count, 1);
    sel->current = index;
    sel->saved = index;
    sndSetSequenceVolumePan(1, 0x7F, 0x3F);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A6AB8);


void btlUpdateFadeIndicator(u8 *obj) {
    BtlFade *fade = (BtlFade *)(obj + 0x1D0);
    s32 minimumAlpha;
    if (fade->kind != 0) {
        if (fade->timer > 0) {
            fade->timer -= 8;
        }
        switch (fade->phase) {
        case 0:
            fade->alpha += 8;
            if (fade->alpha >= 0xFF) {
                fade->phase = 1;
                fade->alpha = 0xFF;
                fade->timer = 0x80;
            }
            break;
        case 1:
            fade->alpha -= 8;
            minimumAlpha = (fade->kind & 1) ? 0x20 : 0x40;
            if (fade->alpha <= minimumAlpha) {
                fade->alpha = minimumAlpha;
                fade->phase = 0;
            }
            break;
        }
    }
}

extern void itfMesRenderActivePanelSprites(UiPanel *);
extern s32 frFontDrawGlyphInDefaultMode(s32);
extern s32 frFontDrawGlyphWithSharedFlags(s32, s32);
extern void itfDrawSoundSelectorFadeLayers(UiPanel *);
extern void func_001A7120(UiPanel *);
extern void func_001A6E88(UiPanel *);

void itfUpdateSoundSelectorPanel(UiPanel *panel) {
    u32 flags = panel->flags;
    SndSeqSelect *selection;
    s32 glyph;

    itfMesWork.drawFlags &= ~2;
    itfMesRenderActivePanelSprites(panel);
    glyph = (s32)panel->origin.tex;
    if (!(flags & 0x10000) && glyph != 0) {
        frFontDrawGlyphInDefaultMode(glyph);
    }
    glyph = (s32)panel->pos.chain;
    if (!(flags & 0x20000) && (flags & 7) >= 3) {
        if (frFontDrawGlyphInDefaultMode(glyph) > 0) {
            if ((panel->flags & 7) != 4) {
                panel->fade.unk08 = 0;
                panel->flags = (panel->flags & ~7) | 4;
            }
        }
    }
    glyph = panel->selection.seq;
    if (!(flags & 0x40000)) {
        flags &= 0x38;
        if (flags >= 0x18 && frFontDrawGlyphWithSharedFlags(glyph, 1) > 0) {
            if (flags == 0x18) {
                selection = &panel->selection;
                if (selection->current == -1) {
                    selection->current = 0;
                    selection->saved = 0;
                }
                itfMesSetRowItemFlag(selection->seq, selection->current, selection->count, 1);
                panel->flags = (panel->flags & ~0x38) | 0x20;
                panel->fade.kind = 2;
            }
        }
    }
    if (panel->fade.kind & 1) {
        itfDrawSoundSelectorFadeLayers(panel);
    }
    if (panel->fade.kind & 2) {
        if (panel->state == 3) {
            func_001A7120(panel);
        } else {
            func_001A6E88(panel);
        }
    }
}

void itfMesRenderActivePanelSprites(UiPanel *panel) {
    UiPanelPlacement *place;
    if (panel->flags & 0x80000) {
        return;
    }
    place = &panel->place;
    if ((panel->flags & 0x300) >= 0x100) {
        if (panel->state != 3) {
            itfBuildAndSubmitPanelPacket(place->sprite, &kwlnDrawSurfaces[panel->index]);
        }
        itfMesWork.drawFlags |= 2;
        if (place->overlay != 0) {
            itfBuildAndSubmitPanelPacket(place->overlay, &kwlnDrawSurfaces[panel->index]);
        }
    }
    if (D_003B4778[0] != 0 && D_003B4778[0]->owner == panel && place != 0) {
        func_001A7798(place->sprite);
    }
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A6E88);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A7120);

extern u8 D_003B49F8[];
extern u8 D_003B49E8[];
extern s32 D_003B4A08[];

/* Draw the sound selector frame, its fade layer and the expanding timer outline. */
void itfDrawSoundSelectorFadeLayers(UiPanel *object) {
    s32 bounds[4];
    BtlFade *fade = &object->fade;
    s32 packet;
    s32 expansion;
    UiSurface *surface;

    bounds[0] = 0x1AA0;
    bounds[1] = 0xC60;
    bounds[2] = 0x1BD0;
    bounds[3] = 0xD58;
    packet = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(packet);
    D_003B4A08[3] = 0xFF;
    itfQueueTextureBoundQuadPacket(bounds, D_003B49F8, D_003B4A08, object->unkC,
                                  itfMesWork.allocation, 0, packet);
    D_003B4A08[3] = fade->alpha;
    itfQueueTextureBoundQuadPacket(bounds, D_003B49E8, D_003B4A08, object->unkC,
                                  itfMesWork.allocation, 0, packet);
    if (fade->timer > 0) {
        expansion = 0x80 - fade->timer;
        bounds[0] -= expansion * 2;
        bounds[1] -= expansion;
        bounds[2] += expansion * 2;
        bounds[3] += expansion;
        D_003B4A08[3] = fade->timer;
        itfSendTablePacket(packet, 1, 0);
        itfQueueTextureBoundQuadPacket(bounds, D_003B49E8, D_003B4A08, object->unkC,
                                      itfMesWork.allocation, 0, packet);
        itfSendTablePacket(packet, 0, 0);
    }
    surface = &kwlnDrawSurfaces[object->index];
    surface->submit(surface, packet);
}

typedef struct SndQueueNode {
    u32 unk0;
    struct SndQueueNode *next;
    u32 unk8;
    u32 value;
} SndQueueNode;

extern SndQueueNode *D_00452950[];

s32 sndVisitQueuedResources(void) {
    SndQueueNode *node;
    for (node = D_00452950[0]; node != 0; node = node->next) {
        itfUpdateBattleDisplayAndFadeIndicator(node->value);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A76C8);

void sndFlushMessageQueue(void) {
    SndMessageNode *node = itfMesWork.head;
    s32 message;
    while (node != 0) {
        message = node->message;
        node = node->next;
        itfMesDestroyWindow(message);
    }
    sdfTexReleaseReferenceViaHandler(itfMesWork.allocation);
    itfMesWork.allocation = 0;
}

extern SndDev D_00380708;
extern s32 D_003B4A18[];
extern u8 D_00436630[5];
extern u8 D_00436638[5];
extern void itfEmitQuadListA(void *, void *, u8 *, u8 *, s32, u32, s32);

void func_001A7798(UiSprite *sprite) {
    s32 vertices[4][2] = {
        {sprite->left, sprite->top},
        {sprite->right, sprite->top},
        {sprite->right, sprite->bottom},
        {sprite->left, sprite->bottom}
    };
    s32 packet;

    packet = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(packet);
    itfEmitQuadListA(vertices, D_003B4A18, D_00436630, D_00436638, 5, 0xFFFFFF, packet);
    D_00380708.submitPacket(&D_00380708, packet);
}

void itfAdjustPanelBoundsWithPad(UiPanelPlacement *object, s32 mode) {
    UiSprite *sprite = object->sprite;
    s32 *bounds;
    s32 dx;
    s32 dy;

    if (sprite != NULL) {
        bounds = object->bounds;
        if (D_0037F510.coarseDown & 2) {
            dx = -16;
        } else {
            dx = ((u8)D_0037F510.coarseUp << 3) & 0x10;
        }
        if (D_0037F510.unk36 & 2) {
            dy = -8;
        } else {
            dy = ((u8)D_0037F510.unk37 << 2) & 8;
        }
        if (D_0037F510.unk31 != 0) {
            dx *= 8;
            dy *= 8;
        }
        if (dx != 0 || dy != 0) {
            switch (mode) {
            case 0:
                itfAdvancePanelLayoutAndNotify(sprite, dx, dy, 0, 0, 0);
                bounds[0] += dx;
                bounds[1] += dy;
                break;
            case 1:
                itfAdvancePanelLayoutAndNotify(sprite, 0, 0, dx, dy, 0);
                bounds[2] += dx;
                bounds[3] += dy;
                break;
            case 2:
                itfAdvancePanelLayoutAndNotify(sprite, dx, dy, dx, dy, 0);
                bounds[0] += dx;
                bounds[1] += dy;
                bounds[2] += dx;
                bounds[3] += dy;
                break;
            }
        }
    }
}

typedef struct SndPadStepTarget {
    u8 pad00[0x38];
    s32 value; /* 0x38 */
} SndPadStepTarget;

typedef struct SndPadStepper {
    u8 pad00[4];
    SndPadStepTarget *target; /* 0x04 */
    u8 pad08[0x20];
    s32 index; /* 0x28 */
} SndPadStepper;

/* Step the stepper's index by pad input: one per press, ten with the fast modifier held; mirror it into the target. */
void sndStepIndexByPad(SndPadStepper *stepper) {
    s32 step;

    if (D_0037F510.coarseDown & 2) {
        step = -1;
    } else {
        step = (D_0037F510.coarseUp & 2) > 0;
    }
    if (D_0037F510.unk36 & 2) {
        step = -1;
    } else if (D_0037F510.unk37 & 2) {
        step = 1;
    }
    if (D_0037F510.unk31 != 0) {
        step *= 10;
    }
    if (step != 0) {
        s32 index = (stepper->index + step) & 0xFF;

        stepper->index = index;
        if (stepper->target != NULL) {
            stepper->target->value = index;
        }
    }
}

SndMessageNode *func_001A7A98(SndMessageNode *node) {
    s32 index = 0;
    s32 count = itfMesWork.unk00;

    if (count <= 0) {
        return NULL;
    }
    for (;;) {
        if (node != NULL) {
            node = node->previous;
        }
        if (node == NULL) {
            node = itfMesWork.tail;
        }
        if (((UiPanel *)node->object)->place.sprite != NULL) {
            break;
        }
        index++;
        if (count < index) {
            node = NULL;
            break;
        }
    }
    return node;
}

void itfQueueOffsetTexturedRect(s32 *bounds, s32 *region, s32 x, s32 y,
                   s32 alpha, s32 texture, s32 command) {
    s32 positions[4];
    s32 uv[4];
    UiQuadColor color = D_00414D50;

    positions[0] = (bounds[0] + x) << 4;
    positions[1] = (bounds[1] + y) << 3;
    positions[2] = (bounds[2] + x) << 4;
    positions[3] = (bounds[3] + y) << 3;
    uv[0] = region[0] << 4;
    uv[1] = region[1] << 4;
    uv[2] = (region[0] + region[2]) << 4;
    uv[3] = (region[1] + region[3]) << 4;
    color.alpha = alpha;
    itfQueueTextureBoundQuadPacket(positions, uv, &color, 0, texture, 0, command);
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00414D50);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00414D60);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A7C08);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A81F0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A85E0);

extern s32 D_00435CBC;

extern s32 kwlnTaskCreate(s32 name, s32 arg1, s32 arg2, s32 arg3, void *update, void *destroy, void *data);

extern void itfPrintTestMessageCallback();

extern s32 itfUpdateTestMessageResourceInput();

extern u8 D_003B4A28[];

extern s32 scrCreateTaskForProcessId();

extern void itfMesSetWindowCallbackAddress();

void sndCreateTestMsgTasks(void) {
    D_00435CBC = 0x80FFFFFF;
    sndCycleTestMessageResource();
    itfMesSetWindowCallbackAddress(*(s32 *)(kwlnTaskGetUserValue(scrCreateTaskForProcessId(0x3E8, D_003B4A28, 0)) + 0xCC), itfPrintTestMessageCallback);
    kwlnTaskCreate((s32)"TestMsgMngC", 0x3EF, 0, 0, itfUpdateTestMessageResourceInput, 0, 0);
    kwlnTaskCreate((s32)"TestMsgMngD", 0x2AFE, 0, 0, sndUpdateTestMsgTask, 0, 0);
}

void sndCycleTestMessageResource(void) {
    if (sndTestMessageTexture != 0) {
        sdfTexReleaseReferenceViaHandler(sndTestMessageTexture);
        sndTestMessageTexture = 0;
    }
    sndTestMessageResourceIndex = (sndTestMessageResourceIndex + 1) & 3;
    if (sndTestMessageResourceIndex != 3) {
        sndTestMessageTexture = itfLoadTextureFromAsset(*(u32 *)(D_003B4CF8 + sndTestMessageResourceIndex * 4));
    }
}

s32 itfUpdateTestMessageResourceInput(void) {
    if (D_0037F530[0] < 0) {
        sndCycleTestMessageResource();
    }
    return 0;
}

s32 sndUpdateTestMsgTask(void) {
    s32 mem;
    if ((sndTestMessageTexture != 0) && (sndTestMessageResourceIndex != 3)) {
        mem = sdfAllocPacketAligned(0x20);
        sdfInitPacketList(mem);
        itfSendTablePacket(mem, 0, 0);
        itfQueueTextureBoundQuadPacket(D_003B4D08, D_003B4D18, D_003B4D28, 0xFFF, sndTestMessageTexture, 0, mem);
        D_003805A8.submitPacket(&D_003805A8, mem);
        return 0;
    }
    return 0;
}

extern s32 func_0035B6E0();

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00414EB0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00414EC8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00414EE0);

void itfPrintTestMessageCallback(void) {
    func_0035B6E0("********* AAAA ********\n");
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A8938);

s32 itfStepFloatWithPad(f32 *value, f32 minimum, f32 maximum, f32 coarseStep, f32 fineStep) {
    f32 current = *value;
    s32 changed = 0;

    if (D_0037F510.coarseUp < 0) {
        if (current < maximum) {
            current += coarseStep;
        } else {
            current = minimum;
        }
        changed = 1;
    } else if (D_0037F510.coarseUp & 2) {
        if (current < maximum) {
            current += coarseStep;
            changed = 1;
        }
    } else if (D_0037F510.coarseDown < 0) {
        if (minimum < current) {
            current -= coarseStep;
        } else {
            current = maximum;
        }
        changed = 1;
    } else if (D_0037F510.coarseDown & 2) {
        if (minimum < current) {
            current -= coarseStep;
            changed = 1;
        }
    } else if (fineStep != 0.0f) {
        if (D_0037F510.fineUp < 0) {
            if (current < maximum) {
                current += fineStep;
                if (maximum < current) {
                    current = maximum;
                }
            } else {
                current = minimum;
            }
            changed = 1;
        } else if (D_0037F510.fineUp & 2) {
            if (current < maximum) {
                current += fineStep;
                if (maximum < current) {
                    current = maximum;
                }
                changed = 1;
            }
        } else if (D_0037F510.fineDown < 0) {
            if (minimum < current) {
                current -= fineStep;
                if (current < minimum) {
                    current = minimum;
                }
            } else {
                current = maximum;
            }
            changed = 1;
        } else if (D_0037F510.fineDown & 2) {
            if (minimum < current) {
                current -= fineStep;
                if (current < minimum) {
                    current = minimum;
                }
                changed = 1;
            }
        }
    }
    if (changed != 0) {
        *value = current;
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A8BD0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A9130);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A9580);

u64 *btlCreateGsTestRegisterPacket(u64 owner, s32 alternative) {
    u64 *entry = (u64 *)sdfConsFinalizePacketHeader(sdfAllocPacketAligned(0x30), 0x30);
    entry[4] = owner;
    entry[5] = alternative ? 0x48 : 0x47;
    return entry;
}

u64 *btlCreateGsAlphaRegisterPacket(u64 owner, s32 alternative) {
    u64 *entry = (u64 *)sdfConsFinalizePacketHeader(sdfAllocPacketAligned(0x30), 0x30);
    entry[4] = owner;
    entry[5] = alternative ? 0x43 : 0x42;
    return entry;
}

extern s32 D_00438F3C;
extern UiQuadColor D_003B4D80;
extern f32 sdfSinPoly(f32);
extern s32 sdfCreateResetPacketList(void);
extern void sdfAppendPacket(s32, u64 *);
extern u64 *func_001A9580(s32, s32, s32, s32, s32, u32, u32);

void itfDrawPulsingTestOverlay(s32 surfaceIndex) {
    u32 color = 0;
    s32 alpha;
    s32 i;
    u8 *component;
    s32 list;
    UiSurface *surface;
    f32 phase;

    phase = (f32)(D_00438F3C % 4096) * (1.0f / 4096.0f);
    phase = phase * 6.2831852f + 1.5707963f + 0.78539815f;
    alpha = (s32)((sdfSinPoly(phase) + 1.0f) * 0.5f * D_003B4D80.alpha);
    component = (u8 *)&D_003B4D80;
    for (i = 0; i != 3; i++, component += 4) {
        color |= *component << (i * 8);
    }
    color |= alpha << 24;
    list = sdfCreateResetPacketList();
    sdfAppendPacket(list, btlCreateGsTestRegisterPacket(0x33001, 0));
    sdfAppendPacket(list, btlCreateGsAlphaRegisterPacket(6, 0));
    sdfAppendPacket(list, func_001A9580(0x7000, 0x7900, 0xFEFFFF, 0x2000, 0xE00, color, color));
    surface = &kwlnDrawSurfaces[surfaceIndex];
    surface->submit(surface, list);
}

void btlResetRuntimeSequenceCounter(void) {
    D_004366E8 = 1;
}

u64 btlAdvanceRuntimeSequenceCounter(void) {
    s64 value = D_004366E8 + 1;

    if (value < 0) {
        value = 1;
        D_004366E8 = value;
    } else {
        D_004366E8 = value;
    }
    return value;
}

void btlClearModelFlagRange(void) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = 0xbe0;
    do {
        temp_v0 = temp_v1 + 1;
        mdlFlagClear(temp_v1);
        temp_v1 = temp_v0;
    } while (temp_v0 < 0xc00);
}

extern s32 btlUpdateFadeColor(void);

extern s32 btlUpdateAutoMusic(void);

extern s32 btlUpdateTintAndWorldLight(void);

extern s32 func_00205160(void);

extern s32 btlUpdateScene(void);

extern s32 func_001D3ED8(void);

extern s32 btlUpdateActionSeqs(void);

extern s32 func_001E7648(void);

extern s32 func_0020D108(void);

extern s32 btlSweepFinishedTasks(void);

extern s32 func_001E9130(void);

extern s32 btlExitWhenAudioAndTasksIdle(void);

s32 btlUpdateActiveBattleFrame(void) {
    if (btlRuntime == 0) {
        return 0;
    }
    if (((BattleController *)btlRuntime)->flags & 1) {
        btlUpdateFadeColor();
        btlUpdateAutoMusic();
        btlUpdateTintAndWorldLight();
        func_00205160();
        btlUpdateScene();
        func_001D3ED8();
        btlUpdateActionSeqs();
        func_001E7648();
        func_0020D108();
        btlSweepFinishedTasks();
        func_001E9130();
        ((BattleController *)btlRuntime)->frame = ((BattleController *)btlRuntime)->frame + 1;
    } else {
        btlExitWhenAudioAndTasksIdle();
    }
    return 0;
}

extern s32 btlTickFieldSwayAndTint(void);

extern s32 btlReleaseRainSoundTransition(void);

extern s32 btlSweepFloorModelLists(void);

extern s32 btlUpdateActorModelColorAndLinks(void);

extern s32 func_0022AC10(void);

extern s32 func_0020D110(void);

extern s32 fldInitializeBattleSceneFlow(void);

extern s32 btlClearDeferredTasks(void);

s32 btlUpdateBattleFieldPresentation(void) {
    BattleController *battle = (BattleController *)btlRuntime;
    if (battle == 0) {
        return 0;
    }
    if (battle->flags & 1) {
        btlTickFieldSwayAndTint();
        btlReleaseRainSoundTransition();
        btlSweepFloorModelLists();
        btlUpdateActorModelColorAndLinks();
        func_0022AC10();
        func_0020D110();
        fldInitializeBattleSceneFlow();
        btlClearDeferredTasks();
    }
    return 0;
}

void func_001A9AA8(void) {
}

extern char D_004366F0[];

extern void func_001A9AA8();

extern s32 btlUpdateBattleFieldPresentation();

extern s32 btlUpdateActiveBattleFrame();

void btlCreateDrawTasks(void) {
    BattleController *battle = (BattleController *)btlRuntime;
    s32 draw;
    draw = kwlnTaskCreate((s32)D_004366F0, 0x3F9, 0, 0, btlUpdateActiveBattleFrame, func_001A9AA8, 0);
    battle->drawTask = draw;
    func_00101968(draw, kwlnTaskCreate((s32)"battle_draw", 0x2B0E, 0, 0, btlUpdateBattleFieldPresentation, 0, 0));
}

void btlDestroyDrawTaskAtPriorityWhenPresent(void) {
    s64 task;

    task = func_00101820(0x3f9);
    if (task != 0) {
        BattleController *battle = (BattleController *)btlRuntime;
        kwlnTaskDestroyWithHierarchy(battle->drawTask, 1);
        return;
    }
}

typedef struct ItfMesTable ItfMesTable;

typedef struct ItfMesEntry {
    u32 itemList;
    ItfMesTable *table;
} ItfMesEntry;

typedef struct ItfMesSub {
    u8 unk00[0x18];
    u32 entryCount;
    u8 unk1C[4];
    ItfMesEntry entries[1];
} ItfMesSub;

typedef struct BattleInitState {
    u8 pad000[0x214];
    u32 tick;
    u8 pad218[4];
    u32 commandRestrictFlags;
    u8 pad220[4];
    u32 unk224;
    u8 pad228[0x20];
    u32 listHeads[6];
    u8 pad260[0x1C];
    u8 endCode;
    u8 pad27D[0x2F];
    u16 backgroundA;
    u16 backgroundB;
    u32 unk2B0;
    u8 unk2B4;
    u8 pad2B5[0x20F];
    u8 unk4C4;
    u8 pad4C5[3];
    f32 unk4C8;
    s32 messageWindows[3];
    u8 pad4D8[0xE0];
    u8 unk5B8;
    u8 pad5B9[3];
    u32 unk5BC;
} BattleInitState;

extern SdfMemBlock *D_004366E0;
extern ItfMesSub *D_00435E78;
extern ItfMesSub D_003858D8;
extern ItfMesSub D_00385228;
extern u16 mnuMovieTaskState;
extern u32 func_001003F8(void);
extern u32 func_001B5600(void);
extern void effMiscSeedRandom(void *, u32);
extern s32 itfMesCreateWindow(ItfMesSub *);
extern void btlResetActorEntryState(void);
extern s32 mdlSetViewerSlotResourceHandles(s32, s32, s32, s32, s32);
extern char D_004366F8[];
extern s32 D_003B4D90[];
extern s32 D_003B4DA8[];

void func_001A9B80(void) {
    SdfMemBlock *allocation;
    u32 seed;
    u8 *runtime;

    btlResetRuntimeSequenceCounter();
    allocation = sdfAllocGeneralBlock(0xFD4);
    D_004366E0 = allocation;
    runtime = (u8 *)sdfResourceRetainAddress(allocation);
    btlRuntime = (s32)runtime;
    memset(runtime, 0, 0xFD4);

    ((BattleInitState *)btlRuntime)->tick = 0;
    ((BattleInitState *)btlRuntime)->unk224 = 0;
    ((BattleInitState *)btlRuntime)->listHeads[0] = 0;
    ((BattleInitState *)btlRuntime)->listHeads[1] = 0;
    ((BattleInitState *)btlRuntime)->listHeads[2] = 0;
    ((BattleInitState *)btlRuntime)->listHeads[3] = 0;
    ((BattleInitState *)btlRuntime)->listHeads[4] = 0;
    ((BattleInitState *)btlRuntime)->listHeads[5] = 0;
    mnuMovieTaskState = 2;
    ((BattleInitState *)btlRuntime)->unk4C4 = 0x1E;
    {
        BattleInitState *state = (BattleInitState *)btlRuntime;
        state->endCode = 0;
        state->unk4C8 = 1.0f;
    }
    ((BattleInitState *)btlRuntime)->unk5B8 = 0;
    ((BattleInitState *)btlRuntime)->unk5BC = 0x80808080;
    ((BattleInitState *)btlRuntime)->backgroundA = 0xC9;
    ((BattleInitState *)btlRuntime)->backgroundB = 1;
    ((BattleInitState *)btlRuntime)->unk2B0 = 0;
    ((BattleInitState *)btlRuntime)->unk2B4 = func_001B5600();
    seed = func_001003F8();
    effMiscSeedRandom(effSharedRandomState, seed);
    btlResetActorEntryState();

    ((BattleInitState *)btlRuntime)->messageWindows[0] =
        itfMesCreateWindow(D_00435E78);
    ((BattleInitState *)btlRuntime)->messageWindows[1] =
        itfMesCreateWindow(&D_003858D8);
    ((BattleInitState *)btlRuntime)->messageWindows[2] =
        itfMesCreateWindow(&D_00385228);
    sndResetTransition();
    func_002050D0();
    btlResetDeferredTaskQueue();
    btlResetToInitialScene();
    fldClearSceneSlotsAndGroups();
    btlResetFieldColorAndSweepFlags();
    btlRefreshSoundEntries();
    btlRetainButtonTexture();
    func_001CFC40();
    func_0020D118();
    btlClearModelFlagRange();
    mdlFlagClear(0x82B);

    if (mdlFlagTest(0x290) == 0) {
        mdlSetViewerSlotResourceHandles(0, 1, 0, (s32)D_003B4D90, 0);
        btlBossDebugPrintf(D_004366F8, D_003B4D90);
    } else {
        mdlSetViewerSlotResourceHandles(0, 1, 0, (s32)D_003B4DA8, 0);
        btlBossDebugPrintf(D_004366F8, D_003B4DA8);
    }
}

extern void itfMesDestroyWindowIfPresent(s32);
extern char D_00414F48[];
extern char D_00414F68[];
extern char D_00414F80[];
extern char D_00414F98[];
extern char D_00414FB0[];
extern s32 sndHasOccupiedNodeSlots(void);
extern s32 btlCountRegisteredTasks(void);

s32 btlExitWhenAudioAndTasksIdle(void) {
    BattleInitState *state;

    if (btlRuntime == 0) {
        btlBossDebugPrintf(D_00414F48);
        return 0;
    }
    if (sndHasOccupiedNodeSlots() != 0) {
        btlBossDebugPrintf(D_00414F68);
        return -1;
    }
    if (btlCountRegisteredTasks() != 0) {
        btlBossDebugPrintf(D_00414F80);
        btlFlagTasksForUpdate();
        return -1;
    }
    btlReleaseBossData();
    btlClearTaskLists();
    btlDestroyAllActionSeqs();
    btlDestroyAllUnits();
    btlClearPendingSoundList();
    btlReleaseEventAssets();
    btlGetCurrentSceneRecordValue();
    btlFreeFieldBlocks();
    btlStopRainSoundTransition();
    btlClearTintAndEnableCamera();
    fldDestroySceneTasksAndBuffers();
    btlReleaseButtonTexture();
    sndFreeBattleSoundEntries();
    sndClearList();
    brsTaskTryDestroy();
    btlClearSoundAndModelResources();
    btlDestroyDrawTaskAtPriorityWhenPresent();
    itfMesDestroyWindowIfPresent(((BattleInitState *)btlRuntime)->messageWindows[2]);
    itfMesDestroyWindowIfPresent(((BattleInitState *)btlRuntime)->messageWindows[1]);
    itfMesDestroyWindowIfPresent(((BattleInitState *)btlRuntime)->messageWindows[0]);
    btlAdvanceTitleStateWithAudioCleanup();
    kwlnCancelConfiguredFadeFrames();
    evtDestroySelectionState();
    effResetSlots();
    evtSetSolarOverlayFullyTransparent();
    itfMesClearFlags(1);

    state = (BattleInitState *)btlRuntime;
    if (state->commandRestrictFlags & 0x8000) {
        state->commandRestrictFlags &= ~0x8000;
        sdfSceneProjectionParameters.farZ = 65536.0f;
        btlBossDebugPrintf(D_00414F98);
    }

    sdfReleaseResourceAllocation(D_004366E0);
    D_004366E0 = 0;
    btlRuntime = 0;
    btlBossDebugPrintf(D_00414FB0);
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00414F48);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00414F68);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00414F80);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00414F98);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00414FB0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A9F30);

void btlLoadInputIconsAndSystemSounds(void) {
    btlOpenButtonIconResource();
    func_001CFB48();
    sndLoadSysEffLb();
}

u8 btlIsRuntimeAllocated(void) {
    return btlRuntime != 0;
}

s32 btlIsCurrentActorFullyMarked(void) {
    if (btlIsRuntimeAllocated() == 0) {
        return 0;
    }
    return (((BattleController *)btlRuntime)->flags & 0x06000000) == 0x06000000;
}

s32 btlHasPendingRuntimeActivity(void) {
    s32 state;
    if (btlIsRuntimeAllocated() == 0) {
        return 0;
    }
    state = btlRuntime;
    if (*(s32 *)(state + 0x248) != 0) {
        return 1;
    }
    if ((*(s32 *)(state + 0x1EC) & 2) != 0) {
        return 1;
    }
    return *(u32 *)(state + 0x718) != 0;
}

/* The persistent party record is 0x1C4 bytes, distinct from a battle actor. */
typedef struct BtlPartyEntry {
    u32 header; /* 0x00: the reset path clears the full packed state word. */
    u16 displayId;
    u16 currentHp;
    u16 maxHp;
    u16 pad0A;
    u32 words[0x6E];
} BtlPartyEntry;

void btlResetActorEntryState(void) {
    s32 context = btlRuntime;
    BtlPartyEntry *entries = (BtlPartyEntry *)(datGameState + 0xC18);
    u32 i;
    *(s32 *)(context + 0x2E8) = 0;
    *(s32 *)(context + 0x2EC) = 0;
    *(s32 *)(context + 0x2F0) = 0;
    *(s32 *)(context + 0x2F4) = 0;
    *(s32 *)(context + 0x2F8) = 0;
    *(u16 *)(context + 0x2FC) = 0;
    for (i = 0; i < 5; i++) {
        entries->header = 0;
        entries++;
    }
    memset((void *)(btlRuntime + 0x2DC), 0, 12);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AA400);

s32 btlResolveQueuedSceneRequestParameters(s32 *outCode, s32 *outParameter) {
    s32 buffer[2];
    s16 i;

    if (fldConsumeNextSceneRequest(&buffer[0], &buffer[1]) == 0) {
        return 0;
    }
    for (i = 0; i < 0x10; i++) {
        if (D_00435E14[i].unk00 + 0xC8 != buffer[0]) {
            continue;
        }
        *outCode = D_00435E14[i].unk00 + 0xC8;
        if (D_00435E14[i].entries[buffer[1]].unk08 != -1 &&
            mdlFlagTest(D_00435E14[i].entries[buffer[1]].unk08) != 0) {
            *outParameter = D_00435E14[i].entries[buffer[1]].unk0A;
            return 1;
        }
        if (D_00435E14[i].entries[buffer[1]].unk04 != -1 &&
            mdlFlagTest(D_00435E14[i].entries[buffer[1]].unk04) != 0) {
            *outParameter = D_00435E14[i].entries[buffer[1]].unk06;
            return 1;
        }
        *outParameter = D_00435E14[i].entries[buffer[1]].unk00;
        return 1;
    }
    return 0;
}

s32 btlGetRuntime(void) {
    return btlRuntime;
}

/* Read current HP from a unit-entry address. */
u16 btlReadCurrentUnitHp(s32 entryAddress) {
    return ((BtlEntry *)entryAddress)->hp;
}

/* Read current MP from a unit-entry address. */
u16 btlReadCurrentUnitMp(s32 entryAddress) {
    return ((BtlEntry *)entryAddress)->mp;
}

void btlComputeProfileMaxHp(void) {
    ptyComputeMaxHp();
}

void btlComputeProfileMaxMp(void) {
    ptyComputeMaxMp();
}

s32 btlComputeSkillAdjustedMaxHp(void *stats) {
    return datComputeSkillBoostedMaxHp(stats);
}

s32 btlComputeSkillAdjustedMaxMp() {
    return datComputeSkillBoostedMaxMp();
}

void btlAdjustUnitHp(void) {
    datAdjustCurrentHp();
}

void btlAdjustUnitMp(void) {
    datAdjustCurrentMp();
}

/* Cache the skill-adjusted maximum and return current HP clamped to it.
 * The comparison uses the full-width result, not the u16 cache. */
u16 btlRefreshUnitMaximumHpAndClampCurrentHp(s32 entryAddress) {
    u16 currentHp = btlReadCurrentUnitHp(entryAddress);
    u32 maxHp = btlComputeSkillAdjustedMaxHp(entryAddress);
    ((BtlEntry *)entryAddress)->maxHp = maxHp;
    if (maxHp < currentHp) {
        ((BtlEntry *)entryAddress)->hp = maxHp;
    }
    return ((BtlEntry *)entryAddress)->hp;
}

/* Cache the skill-adjusted maximum and return current MP clamped to it.
 * The comparison uses the full-width result, not the u16 cache. */
u16 btlRefreshUnitMaximumMpAndClampCurrentMp(s32 entryAddress) {
    u16 currentMp = btlReadCurrentUnitMp(entryAddress);
    u32 maxMp = btlComputeSkillAdjustedMaxMp(entryAddress);
    ((BtlEntry *)entryAddress)->maxMp = maxMp;
    if (maxMp < currentMp) {
        ((BtlEntry *)entryAddress)->mp = maxMp;
    }
    return ((BtlEntry *)entryAddress)->mp;
}

/* Return the low 15 status bits; do not expose the stored high bit. */
u16 btlReadUnitStatusMask(s32 entryAddress) {
    return ((BtlEntry *)entryAddress)->status & BTL_ENTRY_STATUS_MASK;
}

void func_001AA850(void) {
    sdfRaisePackedChannelValue();
}

void func_001AA868(void) {
    datClearUnitStatusBits();
}

/* Set the actor's selected entry index. */
void btlSetActorSelectedEntryIndex(UiObject *actor, u32 index) {
    actor->selectedEntryIndex = index;
}

/* No selected entry is represented by -1. */
void btlClearActorSelectedEntryIndex(UiObject *actor) {
    actor->selectedEntryIndex = -1;
}


INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AA898);

s32 btlGetActorEntryData(UiObject *actor) {
    s32 entry;

    if ((actor->flags & 0x400) == 0) {
        entry = btlGetIndexedPartyEntryRecord(actor->kind);
        return entry;
    }
    return (s32)&actor->entryMask;
}

s32 btlGetCurrentPartyEntryRecord(void) {
    s32 temp_v0;

    temp_v0 = dds3FindEntryIndex();
    return datGameState + temp_v0 * 0x1c4 + 0xa60;
}

s32 btlGetIndexedPartyEntryRecord(s32 index) {
    return datGameState + index * 0x1c4 + 0xa60;
}

void btlSyncPlayerWork(UiObject *actor) {
    BtlEntry *src = (BtlEntry *)&actor->entryMask;
    BtlEntry *dst = (BtlEntry *)btlGetIndexedPartyEntryRecord(actor->kind);
    s32 maxHp;
    s32 maxMp;
    if (src->flags & 0x1000) {
        dst->flags |= 0x1000;
    } else {
        dst->flags &= ~0x1000;
    }
    if (src->flags & 0x4000) {
        dst->flags |= 0x4000;
    } else {
        dst->flags &= ~0x4000;
    }
    dst->unk14 = src->unk14;
    maxHp = datComputeSkillBoostedMaxHp(dst);
    maxMp = datComputeSkillBoostedMaxMp(dst);
    dst->hp = src->hp < maxHp ? src->hp : maxHp;
    dst->mp = src->mp < maxMp ? src->mp : maxMp;
    memcpy(dst->unk16, src->unk16, 5);
    dst->status = src->status & 0x7FFF;
    dst->unk1AC = src->unk1AC;
    dst->unk1AE = src->unk1AE;
    dst->unk1B0 = src->unk1B0;
    btlBossDebugPrintf("btl:player work set[%p]\n", actor);
}

s32 btlFindPartyEntryIndexForActor(UiObject *object) {
    return dds3FindEntryIndex(object->index);
}

void func_001AABD8(void) {
}

UiObject *btlFindActiveActorByKind(s32 kind) {
    UiObject *unit;
    u32 flags;
    for (unit = ((BattleController *)btlGetRuntime())->actors; unit != 0; unit = unit->next) {
        flags = unit->flags;
        if (flags & 1) {
            if (flags & 0x200) {
                if (kind == unit->kind) {
                    return unit;
                }
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AAC50);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AB160);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AB510);

s32 btlGetEntryFlagsUnlessDisabled(s32 entry) {
    if ((*(u16 *)entry & 4) != 0) {
        return 0;
    }
    return *(s32 *)(datEnemyRecords + *(u16 *)(entry + 4) * 76);
}

void func_001AB8C0(void) {
    datGetClampedProfileAdjustedStat();
}

void func_001AB8D8(void) {
    datGetStatWithStatusOverride();
}

s32 btlApplyCommandAbilityMultiplier(s32 battler, s32 command) {
    u32 value = datCalculateCommandBaseValue(battler, command);
    f32 scale;

    if (value == 0) {
        return 0;
    }
    scale = 1.0f;
    switch (*(u8 *)(datCommandRecords + command * 56 + 3)) {
    case 1:
        if (btlCheckSpecialAbility(battler, 0x254)) {
            scale = datAbilityParameters[0x254 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        break;
    case 2:
        if (btlCheckSpecialAbility(battler, 0x255)) {
            scale = datAbilityParameters[0x255 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        break;
    }
    value = (s32)((f32)value * scale);
    return value == 0 ? 1 : value;
}

s32 btlGetSlotValueAdjustedForSpecialAbility(s32 battler, s32 slot) {
    s32 value = datAffinityRecords[slot * 16 - 0x1aa4];
    if (btlDoesEnabledStatusMatchCurrentId(battler + 0x120, 0xe4) && (u32)value >= 2) {
        value--;
    }
    return value;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ABA40);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ABB10);

s32 btlGetCombinedPartyCommandPower(BtlPartyEntry *base, UiObject *first, UiObject *second,
                  UiObject *third, s32 command) {
    BtlPartyEntry snapshot = *base;
    s32 totalMaxHp = 0;
    s32 count = 0;
    s32 average;

    if (first != NULL) {
        totalMaxHp = first->maximumValue;
        count = 1;
    }
    if (second != NULL) {
        totalMaxHp += second->maximumValue;
        count++;
    }
    if (third != NULL) {
        totalMaxHp += third->maximumValue;
        count++;
    }
    average = totalMaxHp / count;
    snapshot.maxHp = average;
    snapshot.currentHp = average;
    return btlApplyCommandAbilityMultiplier((s32)&snapshot, command);
}

s32 func_001ABDE8(BtlUnit *base, BtlUnit *first, BtlUnit *second,
                  BtlUnit *third, s32 command) {
    BtlUnit snapshot = *base;
    s32 totalMaxHp = 0;
    s32 count = 0;

    if (first != NULL) {
        if ((first->flags & 0x100) == 0) {
            return 3;
        }
        if ((first->conditionFlags & 0x2A0E) != 0) {
            return 3;
        }
        totalMaxHp = first->maxHp;
        count = 1;
    }
    if (second != NULL) {
        if ((second->flags & 0x100) == 0) {
            return 3;
        }
        if ((second->conditionFlags & 0x2A0E) != 0) {
            return 3;
        }
        count++;
        totalMaxHp += second->maxHp;
    }
    if (third != NULL) {
        if ((third->flags & 0x100) == 0) {
            return 3;
        }
        if ((third->conditionFlags & 0x2A0E) != 0) {
            return 3;
        }
        count++;
        totalMaxHp += third->maxHp;
    }
    snapshot.maxHp = totalMaxHp / count;
    return func_001ABB10(&snapshot, command);
}

s8 btlGetActorIndexedSignedValue(UiObject *object, s32 index) {
    if (index == 0 && (object->flags & 0x400) != 0) {
        return *(s8 *)(datEnemyRecords + object->index * 76 + 0x46);
    }
    return *(s8 *)(datCommandSelectors + index * 2);
}

extern void btlResolveUnitValueWithOverride();

void func_001ABF50(s32 battler) {
    btlResolveUnitValueWithOverride(battler + 0x120);
}

struct DatUnitStatus;
extern s32 datGetEffectiveAffinity(struct DatUnitStatus *, s32);

void btlResolveUnitValueWithOverride(s32 arg0, s32 arg1) {
    s32 (*hook)(s32, s32) = *(s32 (**)(s32, s32))(btlGetRuntime() + 0x6B8);
    if (hook != 0) {
        if (hook(arg0, arg1) != -1) {
            return;
        }
    }
    datGetEffectiveAffinity((struct DatUnitStatus *)arg0, arg1);
}

s32 btlGetSideIndexedActorStatusTable(s32 arg0, s32 arg1) {
    if (arg0 == 0) {
        return D_00435DE0 + arg1 * 0x270;
    }
    return D_00435DF0 + arg1 * 0x270;
}

s32 btlSelectSharedOrIndexedTransformParameters(s32 arg0, s32 arg1) {
    if (arg0 == 0) {
        return (s32)D_003B4DC0;
    }
    return D_00435E00 + arg1 * 24;
}

s32 btlSelectSideIndexedActorParameterTable(s32 arg0, s32 arg1) {
    if (arg0 == 0) {
        return D_00435DE4 + arg1 * 0x74;
    }
    return D_00435DF8 + arg1 * 0x74;
}

s32 btlGetLoggedIndexedCommandItem(s32 index) {
    u16 item = *(u16 *)(datItemSkillRecords + index * 8 + 2);
    btlBossDebugPrintf(D_00415158, index, item);
    return item;
}

u16 btlGetActorBedAssetIdFromIndex(s32 arg0) {
    return *(u16 *)(arg0 * 8 + datItemSkillRecords + 2);
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415158);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AC0F8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AC360);



extern s32 btlFindEligibleTargetForMultiActorCommand(s32 arg0, BtlIndexList *arg1);
INCLUDE_ASM(const s32, "game/code_001A5BB8", btlFindEligibleTargetForMultiActorCommand);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AC648);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AC750);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ACD10);

u32 btlEncodeActorIndexAsSelectionMask(u32 id) {
    u32 mask;
    switch (id) {
    case 0xFFFFFFFF: mask = 0x1; break;
    case 0: mask = 0x2; break;
    case 1: mask = 0x4; break;
    case 2: mask = 0x8; break;
    case 3: mask = 0x10; break;
    case 4: mask = 0x20; break;
    case 5: mask = 0x40; break;
    case 6: mask = 0x80; break;
    case 7: mask = 0x100; break;
    case 8: mask = 0x200; break;
    case 9: mask = 0x400; break;
    case 10: mask = 0x800; break;
    case 11: mask = 0x1000; break;
    case 12: mask = 0x2000; break;
    case 13: mask = 0x4000; break;
    case 14: mask = 0x8000; break;
    case 15: mask = 0x10000; break;
    case 16: mask = 0x20000; break;
    case 17: mask = 0x40000; break;
    case 18: mask = 0x80000; break;
    default: mask = 0; break;
    }
    return mask;
}

void func_001AD090(void) {
    datFlagToElementIndex();
}

extern s32 datUnitHasSkill();

extern s32 evtGetMirroredSolarPhase(void);

s32 btlCheckSpecialAbility(s32 arg0, s32 ability) {
    if (datUnitHasSkill(arg0) == 0) {
        return 0;
    }
    switch (ability) {
    case 0x227:
        return evtGetMirroredSolarPhase() == 8;
    case 0x228:
        return evtGetMirroredSolarPhase() == 0;
    default:
        return 1;
    }
}

typedef struct DatSkillOwner {
    u16 flags;
    u16 unk2;
    u16 partyIndex;
    u8 unk6[0x1C];
    u16 skills[0x18];
} DatSkillOwner;

/* The 0x4C-byte enemy table supplies skills and all three reward quantities. */
typedef struct DatEnemyRecord {
    u32 flags;            /* 0x00 */
    u8 pad04;
    u8 level;             /* 0x05 */
    u8 pad06[0x12];
    u16 skills[8];        /* 0x18 */
    s32 money;            /* 0x28 */
    u16 unk2C;
    u16 experience;       /* 0x2E */
    u16 huntExperience;   /* 0x30 */
    u8 pad32[0x1A];
} DatEnemyRecord;


s32 func_001AD118(DatSkillOwner *unit, s32 skill) {
    s32 i;

    if (!(unit->flags & 0x20)) {
        for (i = 0; i < 8; i++) {
            if (unit->skills[i] == skill) {
                return 1;
            }
        }
    } else {
        for (i = 0; i < 8; i++) {
            if (((DatEnemyRecord *)datEnemyRecords)[unit->partyIndex].skills[i] == skill) {
                return 1;
            }
        }
    }
    return 0;
}

s32 btlDoesEnabledStatusMatchCurrentId(s32 status, u32 value) {
    if (*(u16 *)status & 0x20) {
        return 0;
    }
    return mnuGetPartyEntryCurrentId(status) == value;
}

s32 btlSelectActorAction(s32 state) {
    s32 selection = func_001AD310(state, 1);
    if (selection == 0) {
        selection = (effMiscRand((s32)effSharedRandomState) & 1) ? 2 : 9;
    }
    return selection;
}

s32 btlIsEventThresholdSatisfiedForEntry(s32 arg) {
    s32 id = arg & 0xFFFF;
    switch (id) {
    case 0x8A:
        if (evtCheckValueThreshold(0x8A, 1) != 0 || evtCheckValueThreshold(0x93, 1) != 0) {
            return 1;
        }
        break;
    case 0x8B:
        if (evtCheckValueThreshold(0x8B, 1) != 0 || evtCheckValueThreshold(0x94, 1) != 0) {
            return 1;
        }
        break;
    }
    if ((u32)((id + 0xFF80) & 0xFFFF) < 0x20 && evtCheckValueThreshold(id, 1) != 0) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AD310);

void func_001AD5B0(u16 item) {
    BattleController *controller = (BattleController *)btlGetRuntime();

    if (item != 0) {
        s32 found = 0;
        u16 i;

        for (i = 0; i < 3; i++) {
            if (controller->itemDrops[i].id == item) {
                found = 1;
                controller->itemDrops[i].count++;
                break;
            }
        }
        if (!found) {
            for (i = 0; i < 3; i++) {
                if (controller->itemDrops[i].id == 0) {
                    controller->itemDrops[i].id = item;
                    controller->itemDrops[i].count = 1;
                    break;
                }
            }
        }
    }
}

extern s32 func_001B39E8(u32);
extern s32 btlCalculateEnemyExperienceReward(u8 *, u8 *);
extern s32 btlGetEnemyMoney(u8 *, u8 *);
extern char D_004151E8[];
extern char D_00415208[];
extern char D_00415220[];
extern char D_00415238[];
extern char D_00415250[];

void btlAccumulateEnemyDefeatRewards(BtlUnit *enemy) {
    BattleController *controller = (BattleController *)btlGetRuntime();
    DatEnemyRecord *record = &((DatEnemyRecord *)datEnemyRecords)[enemy->mode];
    s32 level = func_001B39E8(4);
    s32 enemyLevel = record->level;
    s32 allowance = datBattleParameters->rewardLevelAllowance;
    s32 reward;
    s32 amount;
    s32 item;
    s32 kind;

    if (level > 62) {
        level = 62;
    }
    if (level >= enemyLevel + allowance && datBattleParameters->rewardDivisor != 0.0f) {
        reward = (s32)(record->unk2C / datBattleParameters->rewardDivisor);
        btlBossDebugPrintf(D_004151E8, reward, level, enemyLevel, allowance,
                          datBattleParameters->rewardDivisor);
    } else {
        reward = record->unk2C;
    }
    if (record->flags & 0x2000) {
        reward *= 100;
    }
    if (controller->mode == 3) {
        reward = (s32)(reward * datBattleParameters->majinRewardScale);
        btlBossDebugPrintf(D_00415208, reward, datBattleParameters->majinRewardScale);
    }
    controller->experienceEarned += reward;
    if ((enemy->flags64 & 0x200800000ULL) == 0) {
        amount = btlCalculateEnemyExperienceReward(NULL, (u8 *)enemy);
        controller->epEarned += amount;
        btlBossDebugPrintf(D_00415220, controller->epEarned, amount);
    }
    if ((enemy->stateFlags & 0x400) == 0) {
        amount = btlGetEnemyMoney(NULL, (u8 *)enemy);
        controller->moneyEarned += amount;
        btlBossDebugPrintf(D_00415238, controller->moneyEarned, amount);
    }
    item = func_001AD310((s32)enemy, 0);
    if (item != 0) {
        func_001AD5B0(item);
    }
    kind = enemy->mode;
    if (kind < 104) {
        if (kind >= 100) {
            controller->specialEnemyDefeats++;
        }
    }
    enemy->stateFlags |= 1;
    btlBossDebugPrintf(D_00415250, enemy);
}

/* Readiness requires each active actor to have cleared transient action flags. */
s32 btlAllActiveUnitsReady(void) {
    UiObject *unit;
    u32 flags;
    for (unit = ((BattleController *)btlGetRuntime())->actors; unit != 0; unit = unit->next) {
        flags = unit->flags;
        if (flags & 1) {
            if (flags & 0x400) {
                if (flags & 0xC0) {
                    return 0;
                }
                if (flags & 0x20) {
                    if (!(unit->actionFlags & 1)) {
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

typedef struct BattleAdjustmentEntry {
    u16 sceneIndex;
    u16 weight;
    s8 value;
    u8 unk05;
} BattleAdjustmentEntry;

typedef struct BattleAdjustmentGroup {
    s32 interval;
    BattleAdjustmentEntry entries[20];
} BattleAdjustmentGroup;

typedef struct BattleAdjustmentRecord {
    u8 pad00[8];
    u32 conditions[3];
    u8 variantCodes[8];
    BattleAdjustmentGroup groups[3];
} BattleAdjustmentRecord;

extern BattleAdjustmentRecord *D_00435E0C;

f32 func_001AD978(void) {
    BattleController *runtime = (BattleController *)btlGetRuntime();
    s32 adjustment = D_00435E0C[runtime->adjustmentRecordIndex]
                         .groups[runtime->adjustmentGroupIndex]
                         .entries[runtime->adjustmentEntryIndex]
                         .value;

    if (adjustment < -3) {
        adjustment = -3;
    } else if (adjustment > 3) {
        adjustment = 3;
    }
    return datBattleParameters->adjustmentScale[adjustment + 3];
}

u32 func_001ADA10(void) {
    s32 controller;

    controller = btlGetRuntime();
    return *(u32 *)(controller + 0x278);
}

extern u32 effMiscRandMod(s32, s32);

/* Return a random eligible actor task, or 0 if no candidate is available. */
s32 btlChooseAvailableUnit(void) {
    s32 candidates[16];
    s32 count = 0;
    SceneTask *node;
    UiObject *unit;
    u32 flags;
    for (node = ((BattleController *)btlGetRuntime())->taskHead; node != 0; node = node->next) {
        if (!(node->flags & 8)) {
            continue;
        }
        unit = node->actor;
        flags = unit->flags;
        if (flags & 1) {
            if (flags & 0x200) {
                if (flags & 2) {
                    if (!(flags & 0xE0)) {
                        candidates[count] = (s32)node;
                        count++;
                    }
                }
            }
        }
    }
    if (count == 0) {
        return 0;
    }
    return candidates[effMiscRandMod(0, count)];
}

s32 btlCountAvailableUnits(void) {
    UiObject *unit;
    s32 count = 0;
    u32 flags;
    for (unit = ((BattleController *)btlGetRuntime())->actors; unit != 0; unit = unit->next) {
        flags = unit->flags;
        if (flags & 1) {
            if (flags & 0x200) {
                if (!(flags & 0xE0)) {
                    count++;
                }
            }
        }
    }
    return count;
}

/* Count ready scene actors plus eligible party entries stored in script state. */
s32 btlCountAvailableParticipants(void) {
    UiObject *unit;
    u8 *entry;
    s32 count = 0;
    s32 i;
    u32 flags;
    for (unit = ((BattleController *)btlGetRuntime())->actors; unit != 0; unit = unit->next) {
        flags = unit->flags;
        if (flags & 1) {
            if (flags & 0x200) {
                if (!(flags & 0xE0)) {
                    count++;
                }
            }
        }
    }
    entry = (u8 *)(datGameState + 0xA60);
    for (i = 4; i >= 0; i--) {
        if (((BtlEntry *)entry)->flags & 1) {
            if (!(((BtlEntry *)entry)->flags & 2)) {
                if (!(((BtlEntry *)entry)->status & 0x4000)) {
                    count++;
                }
            }
        }
        entry += 0x1C4;
    }
    return count;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ADBD0);

void btlClearAllActorEntrySlots(u32 arg0) {
    u32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    do {
        temp_v0 = temp_v1 + 1;
        btlClearActorEntrySlot(arg0, temp_v1);
        temp_v1 = temp_v0;
    } while (temp_v0 < 7);
}

s32 btlActorEntryIsExpired(UiObject *unit, s32 index) {
    if (unit->entrySlots[index].code == 0) {
        return 0;
    }
    return unit->entrySlots[index].countdown < 1;
}

typedef struct EntryPair {
    s16 first;
    s16 second;
    s16 initialValue;
    s16 countdown;
} EntryPair;

extern EntryPair D_003B4DF0[];

s32 btlMatchActorEntryCode(UiObject *unit, s32 index) {
    s16 value = unit->entrySlots[index].code;
    if (D_003B4DF0[index].first != 0) {
        if (D_003B4DF0[index].first == value) {
            return 1;
        }
    }
    if (D_003B4DF0[index].second != 0) {
        if (D_003B4DF0[index].second == value) {
            return 2;
        }
    }
    return 0;
}

void func_001ADD30(UiObject *unit, s32 index, s16 delta) {
    s16 code = unit->entrySlots[index].code;
    code += delta;

    if (code > D_003B4DF0[index].first) {
        code = D_003B4DF0[index].first;
    }
    if (code < D_003B4DF0[index].second) {
        code = D_003B4DF0[index].second;
    }
    if (code != 0) {
        unit->entrySlots[index].unk02 = D_003B4DF0[index].initialValue;
        unit->entrySlots[index].countdown = D_003B4DF0[index].countdown;
    }
    unit->entrySlots[index].code = code;
}

void btlSetActorEntryCode(UiObject *unit, s32 index, u16 code) {
    unit->entrySlots[index].code = code;
}

void btlClearActorEntrySlot(UiObject *unit, s32 index) {
    unit->entrySlots[index].code = 0;
    unit->entrySlots[index].unk02 = -1;
    unit->entrySlots[index].countdown = -1;
}

s16 btlGetActorEntryCode(UiObject *unit, s32 index) {
    return unit->entrySlots[index].code;
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004151E8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415208);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415220);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415238);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415250);

f32 btlGetActorEntryMultiplier(UiObject *unit, u32 index, s8 includeCharge) {
    f32 factor;
    s32 stage;

    stage = btlGetActorEntryCode(unit, index);
    factor = 1.0f;
    switch (index) {
    case 3:
        if (unit->flags & 0x200) {
            factor = (datBattleParameters->partyEntryScaleA + 3)[-stage];
        } else {
            factor = (datBattleParameters->enemyEntryScaleA + 3)[-stage];
        }
        break;
    case 2:
        if (unit->flags & 0x200) {
            factor = (datBattleParameters->partyEntryScaleB + 3)[-stage];
        } else {
            factor = (datBattleParameters->enemyEntryScaleB + 3)[-stage];
        }
        break;
    case 0:
    case 1:
        if (unit->flags & 0x200) {
            factor = (datBattleParameters->partyEntryScaleA + 3)[stage];
        } else {
            factor = (datBattleParameters->enemyEntryScaleA + 3)[stage];
        }
        break;
    case 4:
        if (unit->flags & 0x200) {
            factor = (datBattleParameters->partyEntryScaleB + 3)[stage];
        } else {
            factor = (datBattleParameters->enemyEntryScaleB + 3)[stage];
        }
        break;
    }
    if (index == 0 && includeCharge != 0 &&
        btlGetActorEntryCode(unit, 5) > 0) {
        if (btlActorEntryIsExpired(unit, 5) != 0) {
            factor *= 2.25f;
            btlBossDebugPrintf("btl:BUTURIx2\n");
        }
    }
    if (index == 1 && includeCharge != 0 &&
        btlGetActorEntryCode(unit, 6) > 0) {
        if (btlActorEntryIsExpired(unit, 6) != 0) {
            factor *= 2.25f;
            btlBossDebugPrintf("btl:MAGICx2\n");
        }
    }
    return factor;
}

void func_001ADFE0(UiObject *unit, u32 flags, s16 delta) {
    if (flags == 0) {
        return;
    }
    if (flags & 1) {
        func_001ADD30(unit, 0, delta);
    }
    if (flags & 2) {
        func_001ADD30(unit, 0, -delta);
    }
    if (flags & 4) {
        func_001ADD30(unit, 1, delta);
    }
    if (flags & 8) {
        func_001ADD30(unit, 1, -delta);
    }
    if (flags & 0x10) {
        func_001ADD30(unit, 2, delta);
    }
    if (flags & 0x20) {
        func_001ADD30(unit, 2, -delta);
    }
    if (flags & 0x40) {
        func_001ADD30(unit, 3, delta);
    }
    if (flags & 0x80) {
        func_001ADD30(unit, 3, -delta);
    }
    if (flags & 0x100) {
        func_001ADD30(unit, 4, delta);
    }
    if (flags & 0x200) {
        func_001ADD30(unit, 4, -delta);
    }
    if (flags & 0x400) {
        func_001ADD30(unit, 5, delta);
    }
    if (flags & 0x2000) {
        func_001ADD30(unit, 6, delta);
    }
    if (flags & 0x800) {
        if (btlGetActorEntryCode(unit, 0) > 0) {
            btlSetActorEntryCode(unit, 0, 0);
        }
        if (btlGetActorEntryCode(unit, 1) > 0) {
            btlSetActorEntryCode(unit, 1, 0);
        }
        if (btlGetActorEntryCode(unit, 2) > 0) {
            btlSetActorEntryCode(unit, 2, 0);
        }
        if (btlGetActorEntryCode(unit, 3) > 0) {
            btlSetActorEntryCode(unit, 3, 0);
        }
        if (btlGetActorEntryCode(unit, 4) > 0) {
            btlSetActorEntryCode(unit, 4, 0);
        }
    }
    if (flags & 0x1000) {
        if (btlGetActorEntryCode(unit, 0) < 0) {
            btlSetActorEntryCode(unit, 0, 0);
        }
        if (btlGetActorEntryCode(unit, 1) < 0) {
            btlSetActorEntryCode(unit, 1, 0);
        }
        if (btlGetActorEntryCode(unit, 2) < 0) {
            btlSetActorEntryCode(unit, 2, 0);
        }
        if (btlGetActorEntryCode(unit, 3) < 0) {
            btlSetActorEntryCode(unit, 3, 0);
        }
        if (btlGetActorEntryCode(unit, 4) < 0) {
            btlSetActorEntryCode(unit, 4, 0);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", btlLowestSetPairIndex);

void btlTickActorEntryCountdowns(UiObject *unit) {
    s16 *entry = &unit->entrySlots[0].countdown;
    u32 i;
    for (i = 0; i < 7; i++) {
        if (*entry >= 0) {
            if (*entry == 0) {
                *entry = -1;
            }
            *entry = *entry - 1;
        }
        entry += 3;
    }
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AE3A8);

s32 func_001AE678(u8 *actor, s32 attr) {
    u32 value = 100;

    if (((BattleController *)btlGetRuntime())->flags21C & 0x20000) {
        return value;
    }
    switch (attr) {
    case 0:
    case 1:
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x25C)) {
            value = (u32)(datAbilityParameters[0x25C - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 2:
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x25D)) {
            value = (u32)(datAbilityParameters[0x25D - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 3:
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x25E)) {
            value = (u32)(datAbilityParameters[0x25E - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 4:
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x25F)) {
            value = (u32)(datAbilityParameters[0x25F - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 5:
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x260)) {
            value = (u32)(datAbilityParameters[0x260 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 6:
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x261)) {
            value = (u32)(datAbilityParameters[0x261 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 8:
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x262)) {
            value = (u32)(datAbilityParameters[0x262 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 9:
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x263)) {
            value = (u32)(datAbilityParameters[0x263 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    }
    return value;
}

s32 btlHasMappedSpecialAbilityForSlot(s32 unit, u32 slot) {
    if (((BattleController *)btlGetRuntime())->flags21C & 0x20000) {
        return 0;
    }
    if (slot < 2 && btlCheckSpecialAbility(unit + 0x120, 0x26F)) {
        return 1;
    }
    if (slot == 8 && btlCheckSpecialAbility(unit + 0x120, 0x271)) {
        return 1;
    }
    if (slot == 9 && btlCheckSpecialAbility(unit + 0x120, 0x272)) {
        return 1;
    }
    if (slot == 10 && btlCheckSpecialAbility(unit + 0x120, 0x264)) {
        return 1;
    }
    if (slot == 11 && btlCheckSpecialAbility(unit + 0x120, 0x265)) {
        return 1;
    }
    if (slot == 12 && btlCheckSpecialAbility(unit + 0x120, 0x266)) {
        return 1;
    }
    if (slot == 13 && btlCheckSpecialAbility(unit + 0x120, 0x267)) {
        return 1;
    }
    if (slot == 14 && btlCheckSpecialAbility(unit + 0x120, 0x268)) {
        return 1;
    }
    if (slot < 15) {
        if (slot >= 10 && btlCheckSpecialAbility(unit + 0x120, 0x273)) {
            return 1;
        }
    }
    if (slot < 7) {
        if (slot >= 2 && btlCheckSpecialAbility(unit + 0x120, 0x277)) {
            return 1;
        }
    }
    if (slot != 7) {
        if (btlCheckSpecialAbility(unit + 0x120, 0x269)) {
            return 1;
        }
        if (btlDoesEnabledStatusMatchCurrentId(unit + 0x120, 0xF7)) {
            return 1;
        }
    }
    if (slot == 3 && btlDoesEnabledStatusMatchCurrentId(unit + 0x120, 0xF2)) {
        return 1;
    }
    return 0;
}

s32 btlHasSpecialAbility274(s32 unit, u32 slot) {
    if (((BattleController *)btlGetRuntime())->flags21C & 0x20000) {
        return 0;
    }
    if (slot < 2 && btlCheckSpecialAbility(unit + 0x120, 0x274)) {
        return 1;
    }
    return 0;
}

s32 btlHasEnabledSpecialAbilityForSlot(s32 unit, u32 slot) {
    if (((BattleController *)btlGetRuntime())->flags21C & 0x20000) {
        return 0;
    }
    if (slot == 8 && btlCheckSpecialAbility(unit + 0x120, 0x275)) {
        return 1;
    }
    if (slot == 9 && btlCheckSpecialAbility(unit + 0x120, 0x276)) {
        return 1;
    }
    if (slot < 7) {
        if (slot >= 2 && btlCheckSpecialAbility(unit + 0x120, 0x278)) {
            return 1;
        }
    }
    if (slot == 8 && btlDoesEnabledStatusMatchCurrentId(unit + 0x120, 0xF0)) {
        return 1;
    }
    if (slot == 9 && btlDoesEnabledStatusMatchCurrentId(unit + 0x120, 0xF1)) {
        return 1;
    }
    return 0;
}

f32 func_001AEC18(s32 unit) {
    s32 stats = unit + 0x120;
    s32 maximum;
    s32 percentage;

    if (btlCheckSpecialAbility(stats, 0x27A) == 0) {
        return 1.0f;
    }
    maximum = btlComputeSkillAdjustedMaxHp((void *)stats);
    percentage = (s32)((f32)btlReadCurrentUnitHp(stats) / (f32)maximum * 100.0f);
    if (percentage < 6) {
        return 3.0f;
    }
    if (percentage < 11) {
        return 2.2f;
    }
    if (percentage < 16) {
        return 1.7f;
    }
    if (percentage < 21) {
        return 1.4f;
    }
    if (percentage < 26) {
        return 1.2f;
    }
    if (percentage < 31) {
        return 1.1f;
    }
    return 1.0f;
}

f32 btlGetClampedBattleTableValue(void) {
    u16 index = *(u16 *)(btlGetRuntime() + 0x47c);
    if (index > 4) {
        index = 4;
    }
    return D_003B4E28[index];
}

s32 sndGetResourceForIndex(s32 index) {
    s8 resource = *(s8 *)(datCommandSelectors + index * 2);
    if (resource < 0) {
        return 0;
    }
    return (s32)D_003B4E40[resource];
}

void *btlGetIndexedUiResource(UiObject *object) {
    return D_003B4E88[object->index];
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004152F8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415308);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415318);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AED98);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AEEA8);

void btlSyncModelFlagFromEventThresholds(void) {
    if (evtCheckValueThreshold(0x53, 1) || evtCheckValueThreshold(0x54, 1)) {
        mdlFlagSet(0xa20);
    } else {
        mdlFlagClear(0xa20);
    }
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AF0B0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AF4A0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AFF38);

typedef struct BtlHitResult {
    s32 amount;
    u8 pad04[0x28];
} BtlHitResult;

typedef struct BtlTargetResult {
    u8 hitCount;
    u8 pad01[7];
    u32 kind;
    u8 pad0C[4];
    u8 skipped;
    u8 pad11[3];
    u8 blocked;
    u8 pad15[7];
    BtlHitResult hits[32];
} BtlTargetResult;

s32 btlSumOtherTargetHitAmounts(u8 *action) {
    BtlTargetResult *result = *(BtlTargetResult **)(action + 0x88);
    u32 count = btlGetIndexListCount(*(BtlIndexList **)(action + 0x60));
    u32 i;
    u32 j;
    s32 total = 0;

    for (i = 0; i < count; i++, result++) {
        if (result->skipped != 0 || result->blocked != 0) {
            continue;
        }
        switch (result->kind) {
        case 2:
        case 4:
        case 0x10000:
        case 0x20000:
        case 0x40000:
            break;
        default:
            if (*(BtlUnit **)(action + 0x18) !=
                btlGetIndexListEntry(*(BtlIndexList **)(action + 0x60), i)) {
                for (j = 0; j < result->hitCount; j++) {
                    total += result->hits[j].amount;
                }
            }
            break;
        }
    }
    return total;
}

s32 btlTestActorStatusPredicate(u8 *unit) {
    s32 (*hook)(u8 *) = *(s32 (**)(u8 *))(btlGetRuntime() + 0x698);
    if (hook != 0) {
        if (hook(unit) != 0) {
            return 1;
        }
    }
    return (((UiObject *)unit)->statusFlags & 0x2806) != 0;
}

s32 btlComputeStatusPenaltyFifth(UiObject *object) {
    u16 status;
    s32 amount;

    status = object->statusFlags & 0x7fff;
    amount = 0;
    if ((status == 0x80) || (status == 0x400)) {
        amount = (s32)-(u32)object->maximumValue / 5;
    }
    return amount;
}

extern char D_00415440[]; /* "btl:fear ratio[%d]\n" */

extern s32 btlRollAiBucket();

s32 btlRollFearChance(s32 unused, u8 *unit, s32 flagsA, s32 flagsB) {
    s32 threshold;
    if (((BtlState *)btlGetRuntime())->unk220 & 0x80) {
        return 0;
    }
    if (((UiObject *)unit)->actionFlags & 8) {
        return 0;
    }
    if (!(flagsA & 1)) {
        return 0;
    }
    if (((UiObject *)unit)->statusFlags & 1) {
        return 0;
    }
    threshold = 0x1E;
    if (!(flagsB & 2)) {
        threshold = !(flagsB & 4) ? 0 : 0x28;
    }
    btlBossDebugPrintf(D_00415440, threshold);
    return btlRollAiBucket() < threshold;
}

extern s32 btlGetActionRecordLookupValue(s32);
extern s32 fldCountSceneSlots(void);

/* Combine both contributions; groups of three or more suppress the 30% case. */
INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415440);

s32 btlRollAllFearChance(s32 unused, UiObject *unit, u32 flags,
                  s32 unusedFlags, u8 useSelectedAction) {
    s32 actionThreshold;
    s32 statusThreshold;
    s32 threshold;
    u32 category;

    if (((BtlState *)btlGetRuntime())->unk220 & 0x80) {
        return 0;
    }
    actionThreshold = 0;
    statusThreshold = 0;
    if (useSelectedAction != 0) {
        if (unit->selectedEntryIndex == -1) {
            return 0;
        }
        category = btlGetActionRecordLookupValue(unit->selectedEntryIndex);
        switch (category) {
            case 0x20000:
            case 0x40000:
                actionThreshold = 40;
                break;
            case 0x10000:
                actionThreshold = 30;
                if (fldCountSceneSlots() >= 3) {
                    actionThreshold = 0;
                }
                break;
        }
    }
    if (flags & 0x60000) {
        statusThreshold = 40;
    } else if (flags & 0x10000) {
        statusThreshold = 30;
        if (fldCountSceneSlots() >= 3) {
            statusThreshold = 0;
        }
    }
    threshold = statusThreshold < actionThreshold ? actionThreshold : statusThreshold;
    btlBossDebugPrintf("btl:fear all ratio[%d]\n", threshold);
    return btlRollAiBucket() < threshold;
}

f32 func_001B0B20(void) {
    return 1.5f;
}

typedef struct EventModeSlot {
    s8 stat;
    s8 kind;
} EventModeSlot;

typedef struct EventRosterStat {
    s16 base;
    u8 alternateA;
    u8 alternateB;
    f32 multiplier;
    u8 pad08[6];
    u8 rangeMin;
    u8 rangeMax;
    u8 pad10[4];
} EventRosterStat;

typedef struct EventStatRecord {
    u8 pad00[0x11];
    u8 stat11;
    u8 pad12[2];
    u8 rangeMin;
    u8 rangeMax;
    u8 pad16[2];
    s16 stat18;
    u8 pad1A[2];
    s16 stat1C;
    u8 pad1E[7];
    u8 stat25;
    u8 pad26[7];
    u8 stat2D;
    u8 pad2E[6];
    s16 stat34;
    s16 stat36;
} EventStatRecord;


extern s32 datRosterDetails;

u8 func_001B0B30(UiObject *unit, s32 command) {
    BattleController *controller = (BattleController *)btlGetRuntime();
    s32 result;
    s32 minimum;
    s32 maximum;

    if (controller->commandRangeOverride != NULL) {
        result = controller->commandRangeOverride(unit, command);
        if (result > 0) {
            return result;
        }
    }
    if (command == 0) {
        return 1;
    }
    if (((EventModeSlot *)datCommandSelectors)[command].kind == 5 &&
        (unit->flags & 0x200)) {
        minimum = ((EventRosterStat *)datRosterDetails)[unit->index].rangeMin;
        maximum = ((EventRosterStat *)datRosterDetails)[unit->index].rangeMax;
    } else {
        minimum = ((EventStatRecord *)datCommandRecords)[command].rangeMin;
        maximum = ((EventStatRecord *)datCommandRecords)[command].rangeMax;
    }
    if (minimum < maximum) {
        result = minimum + effMiscRandMod(0, maximum - minimum + 1);
    } else {
        result = minimum;
    }
    return result;
}

u8 btlGetActorDisplayByteWithDefault(UiObject *object, s32 index) {
    if (index == 0) {
        if ((object->flags & 0x400) != 0) {
            return *(u8 *)(datEnemyRecords + object->index * 76 + 0x48);
        }
        return 12;
    }
    return 12;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B0C68);

s32 btlMapActionCode(s32 unused, u32 id) {
    switch (id) {
    case 0x31:
    case 0x32:
    case 0x33:
    case 0x185:
        return 0x35;
    case 0x186:
        return 0x1E;
    case 0x193:
        return 0x25;
    default:
        return *(s8 *)(datCommandSelectors + id * 2 + 1) == 2 ? 0x2D : 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B0DB0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B1090);

void btlDistributeRandomTargetHits(UiObject *unit, BtlIndexList *targets,
                   BtlTargetResult *results, s32 command) {
    u8 selected[13];
    BtlIndexList *copy;
    void *previous;
    void *entry;
    u32 targetCount;
    u32 maximumHits;
    u32 hitCount;
    u32 i;
    u32 index;
    u32 resultIndex;
    s32 allowConsecutiveHits;

    targetCount = btlGetIndexListCount(targets);
    allowConsecutiveHits = targetCount < 2;
    maximumHits = targetCount + effMiscRandMod(0, 2);
    if (allowConsecutiveHits) {
        results[0].hitCount = maximumHits;
        return;
    }

    previous = NULL;
    copy = btlAllocateIndexList(ARRAY_COUNT(selected));
    i = 0;
    btlCopyIndexList(copy, targets);
    btlClearIndexList(targets);
    hitCount = func_001B0B30(unit, command);
    memset(selected, 0, sizeof(selected));
    while (i < hitCount) {
        index = effMiscRandMod(0, targetCount);
        entry = btlGetIndexListEntry(copy, index);
        if (!allowConsecutiveHits && entry == previous) {
            index = (index + effMiscRandMod(0, targetCount - 1) + 1) % targetCount;
            entry = btlGetIndexListEntry(copy, index);
        }
        previous = entry;
        if (!selected[index]) {
            btlAppendIndexListEntry(targets, entry);
            resultIndex = btlFindListIndex(targets, entry);
            selected[index] = 1;
            results[resultIndex].hitCount = 1;
        } else {
            resultIndex = btlFindListIndex(targets, entry);
            if (results[resultIndex].hitCount < maximumHits) {
                results[resultIndex].hitCount++;
            }
        }
        i++;
    }
    btlFreeIndexList(copy);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B1350);

s32 btlQueryUnitChannelFlags(s32 first, s32 second, s32 other, s32 variant, s32 mode) {
    s32 flags;
    if (mode != 1) {
        return 0;
    }
    flags = sdfQueryChannelBits(other, first + 0x120, second + 0x120);
    if ((((UiObject *)second)->statusFlags & 8) != 0 && variant == 2) {
        flags |= 8;
    }
    return flags;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B16B0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B17E8);

void func_001B1F78(void) {
}


extern f32 func_001B20C8(u8 *, u8 *, s32);

/* Scale the enemy's normal EP reward; flag 0x2000 multiplies it by 100. */
s32 btlCalculateEnemyExperienceReward(u8 *acquirer, u8 *enemy) {
    s32 result = 0;
    DatEnemyRecord *entry;
    f32 ratio;
    u32 ep;

    if (!(((UiObject *)enemy)->flags & 0x400)) {
        return result;
    }
    if (acquirer != 0 && !(((UiObject *)acquirer)->flags & 0x200)) {
        return result;
    }
    entry = &((DatEnemyRecord *)datEnemyRecords)[((UiObject *)enemy)->index];
    ratio = func_001B20C8(acquirer, enemy, 1);
    ep = (u32)((f32)entry->experience * ratio);
    if (entry->flags & 0x2000) {
        ep *= 100;
    }
    if (acquirer != 0) {
        btlBossDebugPrintf("btl:ep=%d[%d,%.3f]\n", ep, entry->experience, ratio);
    } else {
        btlBossDebugPrintf("btl:ep=%d[%d,%.3f](acquisition)\n", ep, entry->experience, ratio);
    }
    return ep;
}

f32 func_001B20C8(u8 *acquirer, u8 *enemy, s32 rewardKind) {
    s32 level;
    s32 difference;
    f32 factor;

    if (*(u16 *)(datBattleSceneRecords +
                ((BtlState *)btlGetRuntime())->battleMode * 40 + 0x20) & 0x400) {
        btlBossDebugPrintf("btl:ep hosei off\n");
        return 1.0f;
    }
    if (acquirer == NULL) {
        level = func_001B39E8(4);
    } else {
        level = ((UiObject *)acquirer)->stat134;
    }
    if (level > 60) {
        level = 60;
    }
    difference = level - ((UiObject *)enemy)->stat134;
    if (difference > 15) {
        difference = 15;
    } else if (difference < -15) {
        difference = -15;
    }
    factor = datBattleParameters->rewardLevelScale[(15 - difference) * 2 + rewardKind];
    btlBossDebugPrintf("btl:ep[lv=%.3f(%d,%d)]\n",
                       factor, difference, rewardKind);
    return factor;
}

/* Read the money reward with the same eligibility and 100-fold table flag. */
s32 btlGetEnemyMoney(u8 *acquirer, u8 *enemy) {
    s32 result = 0;
    s32 money;
    DatEnemyRecord *entry;
    if (!(((UiObject *)enemy)->flags & 0x400)) {
        return result;
    }
    if (acquirer != 0 && !(((UiObject *)acquirer)->flags & 0x200)) {
        return result;
    }
    entry = &((DatEnemyRecord *)datEnemyRecords)[((UiObject *)enemy)->index];
    money = entry->money;
    if (entry->flags & 0x2000) {
        money *= 100;
    }
    if (acquirer != 0) {
        btlBossDebugPrintf("btl:money=%d\n", money, acquirer);
    } else {
        btlBossDebugPrintf("btl:money=%d(acquisition)\n", money);
    }
    return money;
}

/* Hunt EP uses its own table quantity and the ratio calculator's mode 0. */
s32 btlCalculateHuntEpReward(u8 *arg0, u8 *arg1) {
    DatEnemyRecord *entry = &((DatEnemyRecord *)datEnemyRecords)[((UiObject *)arg1)->index];
    f32 ratio = func_001B20C8(arg0, arg1, 0);
    u32 ep = (u32)((f32)entry->huntExperience * ratio);
    if (entry->flags & 0x2000) {
        ep *= 100;
    }
    btlBossDebugPrintf("btl:ep=%d[%d,%.3f](hunt)\n", ep, entry->huntExperience, ratio);
    return ep;
}

u32 func_001B2380(void) {
    return 0;
}

s32 btlCalculateAbilityRecoveryAmount(u8 *unit) {
    s32 recovery = 0;
    if (btlCheckSpecialAbility((s32)unit + 0x120, 0x26E) != 0) {
        recovery = (s32)(*(u16 *)(unit + 0x12C) * datAbilityParameters[0x26E - BTL_ABILITY_PARAMETER_FIRST_SKILL].value);
    } else if (btlCheckSpecialAbility((s32)unit + 0x120, 0x249) != 0) {
        recovery = (s32)(*(u16 *)(unit + 0x12C) * datAbilityParameters[0x249 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value);
    }
    btlBossDebugPrintf(D_00415638, recovery);
    return recovery;
}

extern s32 btlHasEnemyRecordDefeatExemptionFlag();

s32 btlIsUnitDefeatTriggeredByValueDelta(u8 *unit, s32 delta) {
    if (btlGetEntryFlagsUnlessDisabled((s32)unit + 0x120) & 4) {
        return 0;
    }
    if (btlHasEnemyRecordDefeatExemptionFlag(unit) != 0) {
        return 0;
    }
    if ((((UiObject *)unit)->statusFlags & 0x7FFF) == 0x4000) {
        return 1;
    }
    if (!(*(u32 *)(btlGetRuntime() + 0x218) & 0x80)) {
        return 0;
    }
    return ((UiObject *)unit)->currentValue + delta < 1;
}

s32 btlIsCurrentValueBelowQuarterThreshold(UiObject *object) {
    return object->currentValue * 100 / object->maximumValue < 26;
}

s32 btlWouldUiValueFallBelowQuarter(UiObject *object, s32 delta) {
    s32 value = object->currentValue + delta;
    if (value <= 0) {
        return 1;
    }
    return value * 100 / object->maximumValue < 26;
}

s32 btlBothSidesActive(UiObject *unit) {
    UiObject *actor;
    s32 a;
    s32 b;
    if (btlIsUnitDefeatTriggeredByValueDelta((u8 *)unit, 0) != 0) {
        return 0;
    }
    if (unit->flags & 0x60) {
        return 0;
    }
    a = 0;
    b = 0;
    for (actor = ((BattleController *)btlGetRuntime())->actors; actor != 0; actor = actor->next) {
        if (actor->flags & 1) {
            if (!(actor->flags & 0xE0)) {
                if (actor->flags & 0x200) {
                    a++;
                }
                if (actor->flags & 0x400) {
                    b++;
                }
            }
        }
    }
    if (a != 0 && b != 0) {
        return 1;
    }
    return 0;
}

s32 btlIsUiObjectIndexAllowed(UiObject *object) {
    if ((object->flags & 0x400) != 0) {
        if (object->index >= 0x100) {
            return 0;
        }
    }
    return 1;
}

s32 btlIsUnitStatusFlagClear(UiObject *object) {
    if ((object->statusFlags & 0x1000) != 0) {
        return 0;
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415638);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B2630);

s32 btlSelectedEntryHitsElement(s32 arg0, UiObject *unit, s32 arg2) {
    u32 kind;
    s32 mask;
    u32 power;
    if (unit->selectedEntryIndex <= 0) {
        return 0;
    }
    btlGetRuntime();
    kind = btlGetActorIndexedSignedValue(arg0, arg2);
    mask = btlEncodeActorIndexAsSelectionMask(kind);
    power = *(u16 *)(unit->selectedEntryIndex * 0x38 + datCommandRecords + 0x2E);
    if (power == 0) {
        return 0;
    }
    if (kind >= 0x10 && (kind < 0x12 || kind == -1)) {
        return 0;
    }
    if (power >= 0x21) {
        return 0;
    }
    return (D_003B4F78[power * 3] & mask) != 0;
}

s32 btlGetActionRecordLookupValue(s32 arg0) {
    u16 temp_v0;

    temp_v0 = *(u16 *)(datCommandRecords + arg0 * 56 + 0x2e);
    return D_003B4F70[temp_v0 * 3];
}

s32 btlTestSelectedItemCategoryMask(UiObject *unit, s32 arg) {
    s32 index = unit->selectedEntryIndex;
    u16 kind;
    if (index == -1) {
        return 0;
    }
    kind = *(u16 *)(datCommandRecords + index * 56 + 0x2E);
    return (D_003B4F78[kind * 3] & btlEncodeActorIndexAsSelectionMask(arg)) != 0;
}

s32 fldGetSelectedUnitStat(UiObject *unit) {
    s32 index = unit->selectedEntryIndex;
    if (index == -1) {
        return 0;
    }
    return btlGetActionRecordLookupValue(index);
}

s32 btlGetSelectedUnitProperty(UiObject *unit) {
    s32 index = unit->selectedEntryIndex;
    u16 property;
    if (index == -1) {
        return 0;
    }
    property = *(u16 *)(datCommandRecords + index * 56 + 0x2E);
    return D_003B4F74[property * 3];
}

s32 btlCompareSkippedAndActiveTargetCounts(BtlIndexList *targets, BtlTargetResult *results) {
    s32 i = 0;
    u32 sides = 0;
    BattleController *controller = (BattleController *)btlGetRuntime();
    u32 count = btlGetIndexListCount(targets);
    s32 skipped;
    UiObject *actor;

    for (; i < count; i++) {
        sides |= ((UiObject *)btlGetIndexListEntry(targets, i))->flags & 0x600;
    }
    skipped = 0;
    for (i = 0; i < count; i++, results++) {
        if (results->skipped != 0) {
            skipped++;
        }
    }
    count = 0;
    for (actor = controller->actors; actor != NULL; actor = actor->next) {
        if (actor->flags & 1) {
            if (actor->flags & 0xE0) {
                continue;
            }
            if (actor->flags & sides) {
                count++;
            }
        }
    }
    return skipped == count;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B2AF8);

s32 btlIsSelectedActorStatusAndRecordClear(u8 *unit) {
    s32 work = btlGetRuntime();
    if (*(s8 *)(datBattleSceneRecords + *(s32 *)(work + 0x2A0) * 40) != 0) {
        return 0;
    }
    if (((UiObject *)unit)->statusFlags & 0x2A0F) {
        return 0;
    }
    return datEnemyAiRecords[((UiObject *)unit)->index].unk00 == 0;
}

/* Return true when neither actor status nor its entry flags contain bit 0x40. */
s32 btlAreUnitStatusAndEntryFlagsClear(s32 actor) {
    if ((((UiObject *)actor)->statusFlags & 0x40) != 0) {
        return 0;
    }
    return (btlGetEntryFlagsUnlessDisabled(actor + 0x120) & 0x40) < 1;
}

extern s32 ptyMatchAffinityPermutation(s32 *actors, s32 affinity);

s32 btlFindCommandPartnersByAffinity(UiObject *unit, s32 command, void **first, void **second) {
    s32 statAddresses[3];
    u32 requiredCount = 0;
    BattleController *controller = (BattleController *)btlGetRuntime();
    s32 *requirementCursor;
    UiObject *candidate;
    UiObject *partner;
    u32 i;

    requirementCursor = (s32 *)(datAffinityRecords + command * 16 - 0x1AB0);
    for (i = 0; i < 3; i++) {
        if (*requirementCursor++ != -1) {
            requiredCount++;
        }
    }
    if (requiredCount < 2) {
        return 0;
    }

    statAddresses[0] = (s32)&unit->entryMask;
    for (candidate = controller->actors; candidate != NULL; candidate = candidate->next) {
        u32 flags = candidate->flags;

        if ((flags & 1) == 0) {
            continue;
        }
        if ((flags & 0x400) == 0) {
            continue;
        }
        if ((candidate->statusFlags & 0x2A0E) != 0) {
            continue;
        }
        if (unit == candidate) {
            continue;
        }
        statAddresses[1] = (s32)&candidate->entryMask;
        if (requiredCount == 3) {
            for (partner = controller->actors; partner != NULL; partner = partner->next) {
                u32 partnerFlags = partner->flags;

                if ((partnerFlags & 1) == 0) {
                    continue;
                }
                if ((partnerFlags & 0x400) == 0) {
                    continue;
                }
                if ((partner->statusFlags & 0x2A0E) != 0) {
                    continue;
                }
                if (unit == partner || candidate == partner) {
                    continue;
                }
                statAddresses[2] = (s32)&partner->entryMask;
                if (ptyMatchAffinityPermutation(statAddresses, command) == 0) {
                    continue;
                }
                if (first != NULL) {
                    *first = candidate;
                }
                if (second != NULL) {
                    *second = partner;
                }
                return 1;
            }
        } else {
            statAddresses[2] = 0;
            if (ptyMatchAffinityPermutation(statAddresses, command) != 0) {
                if (first != NULL) {
                    *first = candidate;
                }
                if (second != NULL) {
                    *second = NULL;
                }
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B2F50);

s32 btlHasAdjacentActorRecordStatus(UiObject *object) {
    u8 *status = (u8 *)(datEnemyRecords + object->index * 76 + 0x3E);
    u32 i;
    for (i = 0; i < 2; i++) {
        if (*status++ != 0) {
            return 1;
        }
    }
    return 0;
}

/* Return 1 when bit 26 is clear, preserving the original signed word shift. */
u32 btlIsActorHighStateFlagClear(s32 actor) {
    return (((s32)((UiObject *)actor)->flags >> 0x1a) ^ 1U) & 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B3200);

s32 func_001B32F8(s32 object, s32 *choices) {
    s32 count = 0;
    u32 i;
    u16 *entry = (u16 *)(object + 0x142);
    u16 value;

    for (i = 0; i < 0x18; i++) {
        value = entry[i];
        if (value < 0xA0) {
            continue;
        }
        if (value >= 0xA6) {
            if (value >= 0xB9) {
                continue;
            }
            if (value < 0xB5) {
                continue;
            }
        }
        if (func_001ABB10((BtlUnit *)object, value) != 0) {
            continue;
        }
        if (choices != NULL) {
            choices[count] = value;
        }
        count++;
    }
    return count;
}

u8 btlHasAvailableOption(u32 arg0) {
    s64 temp_v0;

    temp_v0 = func_001B32F8(arg0, 0);
    return temp_v0 != 0;
}

s32 btlChooseRandomAvailableOption(s32 object) {
    s32 choices[24];
    s32 count = func_001B32F8(object, choices);
    if (count != 0) {
        return choices[effMiscRandMod(0, count)];
    }
    return -1;
}

s32 btlChooseEligibleSkill(s32 object) {
    s32 choices[24];
    s32 count = 0;
    u32 i;
    u16 *ids = (u16 *)(object + 0x142);
    for (i = 0; i < 24; i++) {
        u32 id = *ids++;
        if (id != 0) {
            if (id < 0x2A0) {
                s32 category = *(s8 *)(datCommandSelectors + id * 2 + 1);
                if (category != 2) {
                    if (category != 4) {
                        if ((*(u8 *)(datCommandRecords + id * 56 + 1) & 2) != 0) {
                            if (id < 0xAB || (id >= 0xAD && id != 0xBF)) {
                                choices[count++] = id;
                            }
                        }
                    }
                }
            }
        }
    }
    if (count == 0) {
        return -1;
    }
    return choices[effMiscRandMod(0, count)];
}

f32 btlGetActionCategoryMultiplier(s32 unit, s32 unused, s32 index) {
    s32 mode = *(u16 *)(index * 0x38 + datCommandRecords + 0x16);
    f32 rate;
    if (mode < 0xE) {
        rate = 1.0f;
        if (mode >= 0xC) {
            return rate;
        }
    }
    if (index == 0 && btlCheckSpecialAbility(unit + 0x120, 0x23D) != 0) {
        rate = datAbilityParameters[0x23D - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
    } else {
        rate = 0.0f;
    }
    return rate;
}

f32 btlGetActionCategoryGateAsFloat(s32 unused0, s32 unused1, s32 index) {
    s32 category = *(u16 *)(datCommandRecords + index * 56 + 0x1A);
    if (category < 14) {
        if (category >= 12) {
            return 1.0f;
        }
    }
    return 0.0f;
}

f32 btlGetActionRecordPercentAsFraction(s32 unused0, s32 unused1, s32 index) {
    return (f32)*(u16 *)(datCommandRecords + index * 56 + 0x22) / 100.0f;
}

s32 btlAdjustPointsForCombatFlags(s32 unused, u32 flags, u32 otherFlags, u32 value, s32 index) {
    s32 entry;
    u16 code;
    if (flags & 0x20000) {
        return 0x1194;
    }
    if (flags & 0x40000) {
        return 0x1194;
    }
    if (flags & 0x10000) {
        return value + 100;
    }
    if (flags & 2) {
        return value + 100;
    }
    if (flags & 4) {
        entry = index * 56 + datCommandRecords;
        code = *(u16 *)(entry + 0x16);
        if (code != 8 && code != 10 && *(u8 *)(entry + 2) != 2 &&
            *(s32 *)(entry + 0x30) != 4) {
            return value + 100;
        }
    }
    if (otherFlags & 4) {
        return value >> 1;
    }
    if (otherFlags & 2) {
        return value >> 1;
    }
    return value;
}

s32 btlGetCommandResultKindFromFlags(u32 flags, u32 otherFlags, s32 index) {
    s32 entry;
    u16 code;
    if (flags & 0x20000) {
        return 1;
    }
    if (flags & 0x40000) {
        return 1;
    }
    if (flags & 0x10000) {
        return 1;
    }
    if (flags & 2) {
        return 1;
    }
    if (flags & 4) {
        entry = index * 56 + datCommandRecords;
        code = *(u16 *)(entry + 0x16);
        if (code != 8 && code != 10 && *(u8 *)(entry + 2) != 2 &&
            *(s32 *)(entry + 0x30) != 4) {
            return 1;
        }
    }
    if (otherFlags & 4) {
        return 3;
    }
    if (otherFlags & 2) {
        return 2;
    }
    return 1;
}

s32 btlAverageMaximumValueForMask(s32 mask, s8 skipDown) {
    UiObject *unit;
    s32 total = 0;
    s32 count = 0;
    u32 flags;
    for (unit = ((BattleController *)btlGetRuntime())->actors; unit != 0; unit = unit->next) {
        flags = unit->flags;
        if (flags & 1) {
            if (skipDown == 0 || !(flags & 0x20)) {
                if (unit->entryMask & mask) {
                    count++;
                    total += unit->maximumValue;
                }
            }
        }
    }
    if (count > 0) {
        return total / count;
    }
    return 1;
}

void btlAverageFilteredMaximumForMask(u32 arg0) {
    btlAverageMaximumValueForMask(arg0, 1);
}

void btlAverageAllMaximumForMask(u32 arg0) {
    btlAverageMaximumValueForMask(arg0, 0);
}

s32 btlAverageCurrentValueForMask(s32 mask, s8 skipDown) {
    UiObject *unit;
    s32 total = 0;
    s32 count = 0;
    u32 flags;
    for (unit = ((BattleController *)btlGetRuntime())->actors; unit != 0; unit = unit->next) {
        flags = unit->flags;
        if (flags & 1) {
            if (skipDown == 0 || !(flags & 0x20)) {
                if (unit->entryMask & mask) {
                    count++;
                    total += unit->currentValue;
                }
            }
        }
    }
    if (count > 0) {
        return total / count;
    }
    return 1;
}

void btlAverageFilteredCurrentForMask(u32 arg0) {
    btlAverageCurrentValueForMask(arg0, 1);
}

void btlAverageAllCurrentForMask(u32 arg0) {
    btlAverageCurrentValueForMask(arg0, 0);
}

s32 btlAverageMaskedActorStat(s32 mask, s8 skipDown) {
    UiObject *unit;
    s32 total = 0;
    s32 count = 0;
    u32 flags;
    for (unit = ((BattleController *)btlGetRuntime())->actors; unit != 0; unit = unit->next) {
        flags = unit->flags;
        if (flags & 1) {
            if (skipDown == 0 || !(flags & 0x20)) {
                if (unit->entryMask & mask) {
                    count++;
                    total += unit->stat134;
                }
            }
        }
    }
    if (count > 0) {
        return total / count;
    }
    return 1;
}

s32 func_001B39E8(u32 mask) {
    return btlAverageMaskedActorStat(mask, 1);
}

void func_001B3A00(u32 arg0) {
    btlAverageMaskedActorStat(arg0, 0);
}

s32 btlSumOrAverageActorAttribute(u32 mask, s32 attribute, s8 skipDown) {
    s32 sum = 0;
    s32 count = 0;
    UiObject *unit = ((BattleController *)btlGetRuntime())->actors;
    for (; unit != 0; unit = unit->next) {
        u32 flags = unit->flags;
        if ((flags & 1) != 0) {
            if (skipDown == 0 || (flags & 0x20) == 0) {
                if ((unit->entryMask & mask) != 0) {
                    s32 value = datGetStatWithStatusOverride((s32)&unit->entryMask, attribute);
                    count++;
                    sum += value;
                }
            }
        }
    }
    if (count >= 2) {
        sum /= count;
    }
    return sum;
}

void btlAverageFilteredActorAttribute(u32 arg0, u32 arg1) {
    btlSumOrAverageActorAttribute(arg0, arg1, 1);
}

void __udivdi3(u32 arg0, u32 arg1) {
    btlSumOrAverageActorAttribute(arg0, arg1, 0);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B3B28);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B3BE0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B3DD8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B4040);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004157A0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B4210);

void btlClearUnitStatusMask(void) {
    UiObject *unit;
    u32 flags;
    for (unit = ((BattleController *)btlGetRuntime())->actors; unit != 0; unit = unit->next) {
        flags = unit->flags;
        if (flags & 1) {
            if (flags & 0x200) {
                unit->flags = flags & ~0x1000;
                unit->entryMask &= 0xEFFF;
            }
        }
    }
}

s32 btlHasSpecialAbilityOrModelFlag(s32 object) {
    if (btlCheckSpecialAbility(object, 0x279)) {
        return 1;
    }
    return mdlFlagTest(0x820) != 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B4600);

s32 btlRollActorEligibilityWithAbilityOverride(u8 *unit) {
    s32 (*hook)(u8 *) = *(s32 (**)(u8 *))(btlGetRuntime() + 0x6A4);
    if (hook != 0 && hook(unit) == 0) {
        return 0;
    }
    if (((UiObject *)unit)->actionFlags & 0x2000) {
        return 0;
    }
    if ((((UiObject *)unit)->statusFlags & 0x7FFF) == 0x4000) {
        return 0;
    }
    if (btlCheckSpecialAbility((s32)unit + 0x120, 0x251) != 0) {
        return 1;
    }
    btlBossDebugPrintf(D_00415840, 5, 1.0);
    return btlRollAiBucket() < 5;
}

s32 btlHasEnemyRecordDefeatExemptionFlag(UiObject *object) {
    if ((object->flags & 0x400) == 0) {
        return 0;
    }
    return ((s32)((DatEnemyRecord *)datEnemyRecords)[object->index].flags & 0x100) > 0;
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415840);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B4828);

typedef struct BtlAiActionEntry {
    s32 abilityId;
    s32 actionCode;
    u8 unk_08;
    u8 pad_09[3];
} BtlAiActionEntry;

extern BtlAiActionEntry D_003B5100[];

s32 func_001B4918(UiObject *unit, UiObject *target) {
    s32 result;
    s32 threshold;
    u32 i;

    if (btlIsUnitDefeatTriggeredByValueDelta((u8 *)unit, 0) != 0) {
        return -1;
    }

    result = 0;
    for (i = 0; i < 5; i++) {
        if (btlCheckSpecialAbility((s32)unit + 0x120, D_003B5100[i].abilityId) == 0) {
            continue;
        }
        if (D_003B5100[i].unk_08 != 0) {
            if ((unit->flags & 0x200) == 0) {
                continue;
            }
            if ((target->flags & 0x400) == 0) {
                continue;
            }
            if ((unit->flags & 0x1000) == 0) {
                if ((unit->entryMask & 0x10) == 0) {
                    continue;
                }
            }
            if ((unit->statusFlags & 0x40) != 0) {
                continue;
            }
            if ((btlGetEntryFlagsUnlessDisabled((s32)target + 0x120) & 0x40) != 0) {
                continue;
            }
        }
        threshold = (s32)(datAbilityParameters[D_003B5100[i].abilityId -
                                             BTL_ABILITY_PARAMETER_FIRST_SKILL].value *
                          100.0f);
        if (btlRollAiBucket() < threshold) {
            result = D_003B5100[i].actionCode;
            break;
        }
    }
    return result != 0 ? result : -1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B4AA0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B4EB8);

u32 func_001B5268(void) {
    btlGetRuntime();
    return 0xffffffff;
}

void btlRestoreUnitMinimumValueAndClearStatus(UiObject *object, s32 resource) {
    object->flags &= ~0x20;
    object->statusFlags &= ~0x4080;
    *(u32 *)(resource + 0x28) &= ~1;
    *(u32 *)(resource + 0x28) &= ~2;
    if (object->currentValue == 0) {
        object->currentValue = 1;
    }
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B52D8);

s32 btlCanUseActorCommandForModelEntry(s32 object, s32 other, s32 offset, s32 index) {
    s32 (*predicate)(s32, s32, s32);
    s32 table;
    predicate = *(s32 (**)(s32, s32, s32))(btlGetRuntime() + 0x6cc);
    if (predicate != 0 && !predicate(object, other, index)) {
        return 0;
    }
    if (index == 0x91) {
        return 0;
    }
    table = btlGetSideIndexedActorStatusTable(*(s32 *)(object + 0xc4), *(s32 *)(object + 0xc8));
    if (*(s16 *)(table + offset * 20 + 0x2c) != 2) {
        return 0;
    }
    if (index != 0 && *(u8 *)(datCommandRecords + index * 56 + 8) != 0) {
        return 0;
    }
    return 1;
}

s32 btlIsActorModeActionCodeAllowed(s32 object) {
    s32 value;
    if (*(s32 *)(object + 0xdc) != 1) {
        return 1;
    }
    value = *(s32 *)(object + 0xe0);
    if (value == 0x39 || value == 0x126) {
        return 0;
    }
    return 1;
}

typedef struct BtlWeightEntry {
    u16 id;
    u8 weight;
    u8 pad3;
} BtlWeightEntry;

typedef struct BtlWeightTable {
    BtlWeightEntry entries[8];
} BtlWeightTable;

extern BtlWeightTable *D_00435E40;

u32 btlPickWeightedEntry(u16 index) {
    u32 offset = index * sizeof(BtlWeightTable);
    BtlWeightEntry *counted = (BtlWeightEntry *)(offset + (u32)D_00435E40);
    BtlWeightEntry *entry;
    s32 total = 0;
    s32 sum;
    s32 roll;
    s32 i;
    for (i = 7; i >= 0; i--, counted++) {
        if (counted->id != 0) {
            total += counted->weight;
        }
    }
    roll = effMiscRandMod(0, total);
    sum = 0;
    entry = (BtlWeightEntry *)(offset + (u32)D_00435E40);
    for (i = 0; i < 8; i++, entry++) {
        if (entry->id != 0) {
            if (roll < sum + entry->weight) {
                return entry->id;
            }
            sum += entry->weight;
        }
    }
    return 0;
}

u32 func_001B5600(void) {
    u32 i;

    if (mdlFlagTest(0x818) != 0) {
        return 0;
    }
    for (i = 0; i < 0x12; i++) {
        u8 area = D_003B4EC8[i * 2];
        if (area == fldAreaState[4]) {
            u8 zone = D_003B4EC8[i * 2 + 1];
            if (zone == fldAreaState[5] + 1) {
                btlBossDebugPrintf(D_004159A0, area, zone);
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004159A0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B5688);

s32 btlHasHighPriorityState(void) {
    BattleSelectionWork *table;
    s32 index;
    if (btlCommandPanelWork->state == 2) {
        table = btlLinkedSelectionTaskBuffer;
        index = table->selectedRow;
        if (table->rowFade[index - 1] >= 0x80) {
            if (table->positions[index - 1].x >= 0xB) {
                return 1;
            }
        }
    }
    return 0;
}

s32 btlCountFlaggedSceneActors(void) {
    BattleController *controller = (BattleController *)btlGetRuntime();
    UiObject *unit;
    s32 count = 0;
    for (unit = controller->actors; unit != 0; unit = unit->next) {
        if ((*(u64 *)&unit->flags & 0x321) == 0x301) {
            if (!(unit->statusFlags & 0x800)) {
                count++;
            }
        }
    }
    return count;
}


extern void *sdfAllocAndClearQuadwords(s32);
extern s16 btlGetActorIdForClass(s8);
extern void btlGetActorClassPair(s8, u32 *, u32 *);

void btlInitializeCommandPanelSlotTables(void) {
    u32 tableA[5] = {0x40, 0x30, 0x20, 0x10, 0};
    u32 tableB[5] = {0, 0x10, 0x20, 0x30, 0x40};
    s32 i;

    btlCommandPanelWork = sdfAllocAndClearQuadwords(0xCC);
    btlCommandPanelWork->state = 1;
    btlCommandPanelWork->classIndex = 0;
    btlCommandPanelWork->labelEntry =
        btlGetActorIdForClass(btlCommandPanelWork->classIndex);
    btlGetActorClassPair(btlCommandPanelWork->classIndex,
                         &btlCommandPanelWork->classX,
                         &btlCommandPanelWork->classY);
    for (i = 0; i < 5; i++) {
        btlCommandPanelWork->slotsA[i].unk01 = 0;
        btlCommandPanelWork->slotsA[i].fadeValue = tableB[i];
        btlCommandPanelWork->slotsB[i].unk01 = 1;
        btlCommandPanelWork->slotsB[i].fadeValue = tableA[i];
    }
}

s16 btlGetActorIdForClass(s8 classId) {
    s16 table[8] = {0x13, 0x17, 0x15, 0x16, 0x18, 0x14, 0x14, 0x14};
    return table[classId];
}

void btlGetActorClassPair(s8 classId, u32 *first, u32 *second) {
    u32 pairs[14] = {0x19, 4, 0x27, 4, 0x27, 4, 0x35, 4, 0x43, 4, 0x51, 4, 0x5F, 4};
    *first = pairs[classId * 2];
    *second = pairs[classId * 2 + 1];
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B5A20);

typedef struct BattleCmdPanelGridTable {
    u8 values[30];
} BattleCmdPanelGridTable;

extern const BattleCmdPanelGridTable D_00415B80;
extern const BattleCmdPanelGridTable D_00415BA0;

void btlPopulateCommandPanelGrid(void) {
    BattleCmdPanelGridTable table1 = D_00415B80;
    BattleCmdPanelGridTable table2 = D_00415BA0;
    s32 i;

    for (i = 0; i < 5; i++) {
        btlCommandPanelWork->slotsA[i].hidden =
            table1.values[btlCommandPanelWork->classIndex * 5 + i];
        btlCommandPanelWork->slotsB[i].hidden =
            table2.values[btlCommandPanelWork->classIndex * 5 + i];
    }
}


/* Ramp both five-row banks by panel state; narrow each update before clamping. */
void btlUpdateCommandPanelRowFades(void) {
    s32 i;

    switch (btlCommandPanelWork->state) {
    case 1:
        for (i = 0; i < 5; i++) {
            btlCommandPanelWork->slotsA[i].fadeValue += 0x10;
            btlCommandPanelWork->slotsA[i].fadeValue = btlCommandPanelWork->slotsA[i].fadeValue <= 0 ? 0 :
                btlCommandPanelWork->slotsA[i].fadeValue > 0xF0 ? 0xF0 : btlCommandPanelWork->slotsA[i].fadeValue;
            btlCommandPanelWork->slotsB[i].fadeValue += 0x10;
            btlCommandPanelWork->slotsB[i].fadeValue = btlCommandPanelWork->slotsB[i].fadeValue <= 0 ? 0 :
                btlCommandPanelWork->slotsB[i].fadeValue > 0xF0 ? 0xF0 : btlCommandPanelWork->slotsB[i].fadeValue;
        }
        break;
    case 2:
        for (i = 0; i < 5; i++) {
            btlCommandPanelWork->slotsA[i].fadeValue -= 0x10;
            btlCommandPanelWork->slotsA[i].fadeValue = btlCommandPanelWork->slotsA[i].fadeValue <= 0x80 ? 0x80 :
                btlCommandPanelWork->slotsA[i].fadeValue > 0xF0 ? 0xF0 : btlCommandPanelWork->slotsA[i].fadeValue;
            btlCommandPanelWork->slotsB[i].fadeValue -= 0x10;
            btlCommandPanelWork->slotsB[i].fadeValue = btlCommandPanelWork->slotsB[i].fadeValue <= 0x80 ? 0x80 :
                btlCommandPanelWork->slotsB[i].fadeValue > 0xF0 ? 0xF0 : btlCommandPanelWork->slotsB[i].fadeValue;
        }
        break;
    case 3:
        for (i = 0; i < 5; i++) {
            btlCommandPanelWork->slotsA[i].fadeValue -= 0x10;
            btlCommandPanelWork->slotsA[i].fadeValue = btlCommandPanelWork->slotsA[i].fadeValue <= 0 ? 0 :
                btlCommandPanelWork->slotsA[i].fadeValue > 0x80 ? 0x80 : btlCommandPanelWork->slotsA[i].fadeValue;
            btlCommandPanelWork->slotsB[i].fadeValue -= 0x10;
            btlCommandPanelWork->slotsB[i].fadeValue = btlCommandPanelWork->slotsB[i].fadeValue <= 0 ? 0 :
                btlCommandPanelWork->slotsB[i].fadeValue > 0x80 ? 0x80 : btlCommandPanelWork->slotsB[i].fadeValue;
        }
        break;
    case 4:
        for (i = 0; i < 5; i++) {
            btlCommandPanelWork->slotsA[i].fadeValue -= btlTrackedTaskHandles->status.bytes.fadeStep;
            btlCommandPanelWork->slotsA[i].fadeValue = btlCommandPanelWork->slotsA[i].fadeValue <= 0 ? 0 :
                btlCommandPanelWork->slotsA[i].fadeValue > 0x80 ? 0x80 : btlCommandPanelWork->slotsA[i].fadeValue;
            btlCommandPanelWork->slotsB[i].fadeValue -= btlTrackedTaskHandles->status.bytes.fadeStep;
            btlCommandPanelWork->slotsB[i].fadeValue = btlCommandPanelWork->slotsB[i].fadeValue <= 0 ? 0 :
                btlCommandPanelWork->slotsB[i].fadeValue > 0x80 ? 0x80 : btlCommandPanelWork->slotsB[i].fadeValue;
        }
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B6180);

typedef struct BattleCornerFadeDefaults {
    s16 values[20];
} BattleCornerFadeDefaults;
typedef struct BattleCornerPhaseDefaults {
    u32 values[12];
} BattleCornerPhaseDefaults;
extern const BattleCornerFadeDefaults D_00415C30;
extern const BattleCornerPhaseDefaults D_00415C58;

void func_001B6438(void) {
    BattleCornerFadeDefaults limits = D_00415C30;
    BattleCornerPhaseDefaults phases = D_00415C58;
    s32 i;
    switch (btlCommandPanelWork->state) {
    case 1:
        btlCommandPanelWork->labelFade += 0x10;
        btlCommandPanelWork->labelFade = btlCommandPanelWork->labelFade <= 0 ? 0 :
            btlCommandPanelWork->labelFade > 0x80 ? 0x80 : btlCommandPanelWork->labelFade;
        btlCommandPanelWork->classFade += 0x10;
        btlCommandPanelWork->classFade = btlCommandPanelWork->classFade <= 0 ? 0 :
            btlCommandPanelWork->classFade > 0x80 ? 0x80 : btlCommandPanelWork->classFade;
        for (i = 0; i < 4; i++) {
            btlCommandPanelWork->cornerFade[i] += 0x10;
            btlCommandPanelWork->cornerFade[i] = btlCommandPanelWork->cornerFade[i] <= 0 ? 0 :
                btlCommandPanelWork->cornerFade[i] >= limits.values[i] ? limits.values[i] : btlCommandPanelWork->cornerFade[i];
            btlCommandPanelWork->cornerPhase[i] = phases.values[i];
        }
        if (btlCommandPanelWork->labelFade >= 0x80 && btlCommandPanelWork->slotsB[0].fadeValue >= 0xF0)
            btlCommandPanelWork->state = 2;
        break;
    case 2: {
        btlCommandPanelWork->labelFade -= 8;
        btlCommandPanelWork->labelFade = btlCommandPanelWork->labelFade <= 0x80 ? 0x80 :
            btlCommandPanelWork->labelFade >= 0x100 ? 0xFF : btlCommandPanelWork->labelFade;
        btlCommandPanelWork->classFade -= 0x10;
        btlCommandPanelWork->classFade = btlCommandPanelWork->classFade <= 0x80 ? 0x80 :
            btlCommandPanelWork->classFade >= 0x100 ? 0xFF : btlCommandPanelWork->classFade;
        for (i = 0; i < 4; i++) {
            btlCommandPanelWork->cornerPhase[i] = (btlCommandPanelWork->cornerPhase[i] + 8) % 360;
            btlCommandPanelWork->cornerFade[i] =
                (u32)((sdfSinPoly((f32)((btlCommandPanelWork->cornerPhase[i] + 90) % 360) /
                                  180.0f * 3.14159f) + 1.0f) * 0.5f * 64.0f + 128.0f);
        }
        break;
    }
    case 3:
        btlCommandPanelWork->labelFade -= 0x10;
        btlCommandPanelWork->labelFade = btlCommandPanelWork->labelFade <= 0 ? 0 :
            btlCommandPanelWork->labelFade > 0x80 ? 0x80 : btlCommandPanelWork->labelFade;
        btlCommandPanelWork->classFade -= 0x10;
        btlCommandPanelWork->classFade = btlCommandPanelWork->classFade <= 0 ? 0 :
            btlCommandPanelWork->classFade > 0x80 ? 0x80 : btlCommandPanelWork->classFade;
        for (i = 0; i < 4; i++) {
            btlCommandPanelWork->cornerFade[i] -= 0x10;
            btlCommandPanelWork->cornerFade[i] = btlCommandPanelWork->cornerFade[i] <= 0 ? 0 :
                btlCommandPanelWork->cornerFade[i] > 0x80 ? 0x80 : btlCommandPanelWork->cornerFade[i];
        }
        if (btlCommandPanelWork->labelFade <= 0) btlCommandPanelWork->state = 0;
        break;
    case 4:
        btlCommandPanelWork->labelFade -= btlTrackedTaskHandles->status.bytes.fadeStep;
        btlCommandPanelWork->labelFade = btlCommandPanelWork->labelFade <= 0 ? 0 :
            btlCommandPanelWork->labelFade > 0x80 ? 0x80 : btlCommandPanelWork->labelFade;
        btlCommandPanelWork->classFade -= btlTrackedTaskHandles->status.bytes.fadeStep;
        btlCommandPanelWork->classFade = btlCommandPanelWork->classFade <= 0 ? 0 :
            btlCommandPanelWork->classFade > 0x80 ? 0x80 : btlCommandPanelWork->classFade;
        for (i = 0; i < 4; i++) {
            btlCommandPanelWork->cornerFade[i] -= btlTrackedTaskHandles->status.bytes.fadeStep;
            btlCommandPanelWork->cornerFade[i] = btlCommandPanelWork->cornerFade[i] <= 0 ? 0 :
                btlCommandPanelWork->cornerFade[i] > 0x80 ? 0x80 : btlCommandPanelWork->cornerFade[i];
        }
        if (btlCommandPanelWork->labelFade <= 0) btlCommandPanelWork->state = 0;
        break;
    }
}

typedef struct BtlResBlock {
    SdfMemBlock *unk0;
    s32 nameA;
    s32 nameB;
    s32 nameC;
    EffectSlotSet *resA;
    EffectSlotSet *resB;
    EffectSlotSet *resC;
    s32 unk1C;
} BtlResBlock;
extern BtlResBlock *btlResourceBlock;

extern void func_00306C28(s32, s32, s32, u32 *, s32, EffectSlotSet *, s32, s32);

typedef struct BattlePanelColors {
    u32 values[4];
} BattlePanelColors;
extern const BattlePanelColors D_00415CE8;

typedef struct BattleClassLabelOffsets {
    s32 values[8][2];
} BattleClassLabelOffsets;
extern const BattlePanelColors D_00415C88;
extern const BattleClassLabelOffsets D_00415C98;
extern void func_001B6180(void);

void func_001B6A20(void) {
    BattlePanelColors colors = D_00415C88;
    BattleClassLabelOffsets offsets = D_00415C98;
    if (btlCommandPanelWork->state < 5) {
        if (btlCommandPanelWork->state > 0) {
            colors.values[0] = btlCommandPanelWork->cornerFade[0] | 0x80808000;
            colors.values[1] = btlCommandPanelWork->cornerFade[1] | 0x80808000;
            colors.values[2] = btlCommandPanelWork->cornerFade[2] | 0x80808000;
            colors.values[3] = btlCommandPanelWork->cornerFade[3] | 0x80808000;
            func_00306C28(0x50, 0x968, 0, colors.values, 0, btlResourceBlock->resA, 0x1E, 0x53);
            func_001B6180();
            colors.values[0] = btlCommandPanelWork->classFade | 0x80808000;
            colors.values[1] = colors.values[0];
            colors.values[2] = colors.values[0];
            colors.values[3] = colors.values[0];
            func_00306C28((btlCommandPanelWork->classX + 5) << 4,
                         (btlCommandPanelWork->classY + 0x12D) << 3,
                         0, colors.values, 0, btlResourceBlock->resA, 0x1A, 0x53);
            func_00306C28((btlCommandPanelWork->classX + 0x37) << 4,
                         (btlCommandPanelWork->classY + 0x12D) << 3,
                         0, colors.values, 0, btlResourceBlock->resA, 0x1B, 0x53);
            colors.values[0] = btlCommandPanelWork->labelFade | 0x80808000;
            colors.values[1] = colors.values[0];
            colors.values[2] = colors.values[0];
            colors.values[3] = colors.values[0];
            {
                s32 x = btlCommandPanelWork->classX + 5;
                s32 y = btlCommandPanelWork->classY + 0x12D;
                func_00306C28((x + offsets.values[btlCommandPanelWork->classIndex][0]) << 4,
                             (y + offsets.values[btlCommandPanelWork->classIndex][1]) << 3,
                             0, colors.values, 0, btlResourceBlock->resA, (s16)btlCommandPanelWork->labelEntry, 0x53);
            }
        }
    }
}

void btlReleaseAndClearChipBlock(void) {
    sdfReleaseChipBlock(btlCommandPanelWork);
    btlCommandPanelWork = 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B6CA8);



/* 0x2C-byte action record shared by the initializer, updater and renderer. */
typedef struct BattleMirroredSpriteRecord {
    s8 active;
    u8 pad01;
    s16 slot;
    f32 scale;
    s32 restoredWidth;
    s32 restoredHeight;
    s32 width;
    s32 height;
    s32 x;
    s32 y;
    s32 secondX;
    s32 frame;
    s8 alpha;
    u8 pad29[3];
} BattleMirroredSpriteRecord;

void func_001B6FC0(s32 unused, BattleMirroredSpriteRecord *records, s32 count) {
    if (count > 0) {
        BattleMirroredSpriteRecord *record = records;
        s32 remaining = count;
        do {
            switch (record->active) {
            case 1: {
                s32 scale = (s32)record->scale;
                s32 x = record->x;
                s32 y = record->y;
                s32 width = btlResourceBlock->resA->workEntries[record->slot].sourceWidth;
                s32 height;
                s32 scaledWidth;
                s32 scaledHeight;
                record->restoredWidth = width;
                scale = scale >> 1;
                height = btlResourceBlock->resA->workEntries[record->slot].sourceHeight;
                scaledWidth = width * scale;
                scaledHeight = height * scale;
                record->active = 2;
                record->restoredHeight = height;
                x -= (scaledWidth - width) >> 1;
                y -= (scaledHeight - height) >> 1;
                record->secondX = x;
                record->y = y;
                record->width = scaledWidth << 4;
                record->height = scaledHeight << 3;
                break;
            }
            case 2:
                record->width -= 0x800;
                record->height -= 0x800;
                if (record->width <= 0x1000) record->active = 0;
                break;
            }
            remaining--;
            record++;
        } while (remaining != 0);
    }
}





void func_001B70B8(s32 unused, BattleMirroredSpriteRecord *records, s32 count) {
    BattlePanelColors colors = D_00415CE8;
    if (count > 0) {
        BattleMirroredSpriteRecord *record = records;
        s32 remaining = count;
        do {
            if (record->active != 0) {
                u32 color;
                btlResourceBlock->resA->workEntries[record->slot].width = record->width;
                btlResourceBlock->resA->workEntries[record->slot].height = record->height;
                color = record->alpha | 0x80808000;
                colors.values[0] = color;
                colors.values[1] = color;
                colors.values[2] = color;
                colors.values[3] = color;
                func_00306C28(record->x << 4, record->y << 3, 0, colors.values, 0, btlResourceBlock->resA, record->slot, 0x53);
                func_00306C28(record->secondX << 4, record->y << 3, 0, colors.values, 0, btlResourceBlock->resA, record->slot, 0x53);
                btlResourceBlock->resA->workEntries[record->slot].width = record->restoredWidth << 4;
                btlResourceBlock->resA->workEntries[record->slot].height = record->restoredHeight << 3;
            }
            record++;
        } while (--remaining != 0);
    }
}

void btlInitializeActionRecordWithScale(s32 arg0, s16 arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5) {
    *(u8 *)(arg0 + 0) = 1;
    *(u8 *)(arg0 + 0x28) = arg4 * 8 + 0x18;
    *(u16 *)(arg0 + 2) = arg1;
    *(f32 *)(arg0 + 4) = arg5;
    *(u32 *)(arg0 + 0x18) = arg2;
    *(u32 *)(arg0 + 0x1c) = arg3;
    *(u32 *)(arg0 + 0x24) = 0;
}

extern u8 D_00436800;

extern u8 btlResourceBlockLoaded;

extern s32 sdfReadNamedResource(const char *, void *, s32);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415B80);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415BA0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415BC0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415BD0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415C00);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415C30);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415C58);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415C88);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415C98);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415CD8);

const BattlePanelColors D_00415CE8 = {{0x80808080, 0x80808080, 0x80808080, 0x80808080}};

void btlPanelResourcesLoad(void) {
    u8 params[16];
    SdfMemBlock *handle;
    BtlResBlock *block;
    if (D_00436800 == 0) {
        handle = sdfAllocGeneralBlock(0x28);
        block = (BtlResBlock *)sdfResourceRetainAddress(handle);
        btlResourceBlock = block;
        block->unk0 = handle;
        block->resA = 0;
        block->resB = 0;
        block->unk1C = 0;
        btlResourceBlock->nameA = sdfReadNamedResource("/battle/panel/batle_01.spr", params, 0);
        btlResourceBlock->nameB = sdfReadNamedResource("/battle/panel/batle_02.spr", params, 0);
        btlResourceBlock->nameC = sdfReadNamedResource("/battle/panel/battle_03.spr", params, 0);
        btlResourceBlockLoaded = 0;
    }
    D_00436800 = 1;
}

typedef struct BtlWorkRes {
    u8 pad[0x4D8];
    EffectSlotSet *resA;
    EffectSlotSet *resB;
    EffectSlotSet *resC;
} BtlWorkRes;

extern EffectSlotSet *func_00305148();

void btlLoadResourceBlock(void) {
    BtlWorkRes *work = (BtlWorkRes *)btlGetRuntime();
    if (btlResourceBlockLoaded == 0) {
        btlResourceBlock->resA = func_00305148(btlResourceBlock->nameA, 0);
        btlResourceBlock->resB = func_00305148(btlResourceBlock->nameB, 0);
        btlResourceBlock->resC = func_00305148(btlResourceBlock->nameC, 0);
        work->resA = btlResourceBlock->resA;
        work->resB = btlResourceBlock->resB;
        btlResourceBlockLoaded = 1;
    }
}

extern s32 effDestroyResourceSlotSet(EffectSlotSet *);

void btlReleaseResourceBlock(void) {
    BtlWorkRes *work = (BtlWorkRes *)btlGetRuntime();
    if (btlResourceBlockLoaded != 0) {
        effDestroyResourceSlotSet(btlResourceBlock->resA);
        btlResourceBlock->resA = 0;
        effDestroyResourceSlotSet(btlResourceBlock->resB);
        btlResourceBlock->resB = 0;
        effDestroyResourceSlotSet(btlResourceBlock->resC);
        btlResourceBlock->resC = 0;
        work->resA = 0;
        work->resB = 0;
        work->resC = 0;
        btlResourceBlockLoaded = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B73E8);

/* Clear each task's eight opaque words, forward for variant 1 and backward otherwise. */
void btlClearTaskActorSlots(void) {
    BattleController *work = (BattleController *)btlGetRuntime();
    SceneTask *node;
    UiObject *owner;
    s32 *slot;
    s32 *reverse;
    s32 i;
    for (node = work->taskHead; node != 0; node = node->next) {
        owner = node->actor;
        if (owner != 0) {
            if (work->variant == 1) {
                if (owner->flags & 0x200) {
                    for (i = 0, slot = &node->actions[0]; i < 8; i++) {
                        *slot = 0;
                        slot++;
                    }
                }
            } else if (owner->flags & 0x400) {
                for (i = 7, reverse = &node->actions[7]; i >= 0; i--) {
                    *reverse = 0;
                    reverse--;
                }
            }
        }
    }
}

typedef struct ActorSlotOrder {
    u8 pad00[0xC];
    s32 entries[12];
} ActorSlotOrder;

typedef struct ActorOrder12 {
    s32 entries[12];
} ActorOrder12;

typedef struct ActorOrder6 {
    s32 entries[6];
} ActorOrder6;

extern ActorSlotOrder *D_00438F58[2];
extern const ActorOrder12 D_00415D58;
extern const ActorOrder6 D_00415D88;

void func_001B75F8(s32 selector, s32 count) {
    ActorOrder12 primaryOrder = D_00415D58;
    ActorOrder6 secondaryOrder = D_00415D88;
    s32 *order = selector != 0 ? primaryOrder.entries : secondaryOrder.entries;
    ActorSlotOrder *destination;
    s32 i;

    i = 0;
    if (count > 0) {
        destination = D_00438F58[selector];
        do {
            destination->entries[i] = order[i];
            i++;
        } while (i < count);
    }
}

s32 func_001B76F0(void) {
    s32 runtime;
    s32 fadeCounts[4];

    runtime = btlGetRuntime();
    return fldCountSceneFadeKinds(runtime, fadeCounts);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B7718);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B7830);

void btlClearSharedBattleStateWords(void) {
    u32 *puVar1;
    s32 temp_v0;

    temp_v0 = 3;
    puVar1 = (u32 *)(datGameState + 0x16f00);
    do {
        temp_v0 = temp_v0 - 1;
        *puVar1 = 0;
        puVar1 = puVar1 + 1;
    } while (-1 < temp_v0);
}

/* The reset routine clears all four words of the shared battle flag bank. */
typedef struct BattleFlagSaveState {
    u8 pad00000[0x16F00];
    u32 sharedBattleFlags[4];
} BattleFlagSaveState;

s32 func_001B7940(s32 id, s32 operation) {
    s32 selector;
    s32 offset;
    u32 word;
    u32 bit;

    id = (u16)id;
    offset = id - 0x1AB;
    selector = (s8)operation;
    if (offset != 0) {
        word = (u32)offset >> 5;
        bit = offset & 0x1F;
    } else {
        word = 0;
        bit = 0;
    }
    switch (selector) {
    case 0:
        ((BattleFlagSaveState *)datGameState)->sharedBattleFlags[word] |= 1 << bit;
        break;
    case 1:
        ((BattleFlagSaveState *)datGameState)->sharedBattleFlags[word] &= ~(1 << bit);
        break;
    default:
        return ((((BattleFlagSaveState *)datGameState)->sharedBattleFlags[word] & (1 << bit)) != 0);
    }
    return 1;
}

void func_001B7A00(void) {
    s32 i;
    for (i = 3; i >= 0; i--) {
        D_003B6928[i] = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B7A38);

extern u32 D_004367B8;

s64 btlSetTaskPhase2(void) {
    s64 task = kwlnTaskGetTaskByName(D_004367B8);
    if (task != 0) {
        *(s32 *)kwlnTaskGetUserValue(task) = 2;
        return 1;
    }
    return task;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B7B20);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B7BF0);

u32 btlIsNamedBattleTaskRegistered(void) {
    s64 temp_v0;

    temp_v0 = btlGetTrackedTaskHandle(10);
    if (temp_v0 != 0) {
        temp_v0 = kwlnTaskIsRegistered(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = kwlnTaskGetTaskByName(btlMahenPanelTaskNameRef);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

void func_001B7E08(void) {
    u8 *puVar1;
    s64 temp_v0;

    temp_v0 = kwlnTaskGetTaskByName(btlMahenPanelTaskNameRef);
    if (temp_v0 != 0) {
        puVar1 = (u8 *)kwlnTaskGetUserValue(temp_v0);
        *puVar1 = 2;
    }
}

typedef struct BtlAnalysisPanelWork {
    s8 state;
    u8 pad01[3];
    s32 unk04;
    s32 duration;
    s32 frame;
    u8 pad10[0x18];
    s32 x;
    s32 y;
    s8 page;
    u8 pad31[0x6F];
} BtlAnalysisPanelWork;

extern u32 btlHasRegisteredAnalysisPanelTask(void);
extern u32 btlHasRegisteredGuidePanelTask(void);
extern u32 btlHasRegisteredSkillNamePanelTask(void);
extern u32 btlHasRegisteredAphNamePanelTask(void);
extern void btlSetTrackedTaskHandle(s32, s32);
extern s32 func_001B9158(s32);
extern void btlReleaseTaskAndRefreshCursorIfFlagged(s32);

s32 btlCreateAnalysisPanelTask(s32 entry, s32 duration) {
    BattleController *context = (BattleController *)btlGetRuntime();
    BtlAnalysisPanelWork *data;
    s32 task;

    if (btlHasRegisteredAnalysisPanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(11), 0);
    }
    if (btlIsNamedBattleTaskRegistered() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(10), 0);
    }
    if (btlHasRegisteredGuidePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(9), 0);
    }
    if (btlHasRegisteredSkillNamePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(1), 0);
    }
    if (btlHasRegisteredAphNamePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(0), 0);
    }
    data = sdfAllocAndClearQuadwords(sizeof(*data));
    data->state = 0;
    data->page = 0;
    data->x = -20;
    data->y = 133;
    data->duration = duration;
    data->unk04 = entry;
    data->frame = 0;
    task = kwlnTaskCreate(btlAnalyzPanelTaskNameRef, 0x2B0E, 1, 1, func_001B9158,
                          btlReleaseTaskAndRefreshCursorIfFlagged, data);
    func_00101968(context->drawTask, task);
    btlSetTrackedTaskHandle(11, task);
    context->flags &= ~0x100000;
    return 1;
}

u32 btlHasRegisteredAnalysisPanelTask(void) {
    s64 temp_v0;

    temp_v0 = btlGetTrackedTaskHandle(0xb);
    if (temp_v0 != 0) {
        temp_v0 = kwlnTaskIsRegistered(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = kwlnTaskGetTaskByName(btlAnalyzPanelTaskNameRef);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

s32 btlGetRegisteredTaskValueOrDefault(void) {
    if (btlHasRegisteredAnalysisPanelTask() == 0) {
        return 0x80;
    }
    return *(s8 *)kwlnTaskGetUserValue(kwlnTaskGetTaskByName(btlAnalyzPanelTaskNameRef));
}

u32 func_001B8038(void) {
    u8 *puVar1;
    s64 temp_v0;

    temp_v0 = kwlnTaskGetTaskByName(btlAnalyzPanelTaskNameRef);
    if (temp_v0 != 0) {
        puVar1 = (u8 *)kwlnTaskGetUserValue(temp_v0);
        *puVar1 = 2;
    }
    return 1;
}

typedef struct BtlGuidePanelWork {
    s8 state;
    u8 pad01[0x1B];
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
} BtlGuidePanelWork;

extern s32 func_001BB5C0(s64);
extern void btlReleaseMessageWindowTask(void);
extern u32 btlHasRegisteredGuidePanelTask(void);
extern u32 btlHasRegisteredSkillNamePanelTask(void);
extern u32 btlHasRegisteredAphNamePanelTask(void);
extern void btlSetTrackedTaskHandle(s32, s32);

s32 btlCreateGuidePanelTask(s32 arg0, s32 arg1) {
    BattleController *context = (BattleController *)btlGetRuntime();
    BtlGuidePanelWork *data;
    s32 task;

    if (btlHasRegisteredGuidePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(9), 0);
    }
    if (btlHasRegisteredSkillNamePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(1), 0);
    }
    if (btlHasRegisteredAphNamePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(0), 0);
    }
    if (btlIsNamedBattleTaskRegistered() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(10), 0);
    }
    data = sdfAllocAndClearQuadwords(sizeof(*data));
    data->state = 0;
    data->unk1C = 0x18;
    data->unk20 = 0x60;
    data->unk24 = arg0;
    data->unk28 = arg1;
    task = kwlnTaskCreate(D_004367E0, 0x2B0E, 1, 1, func_001BB5C0,
                          btlReleaseMessageWindowTask, data);
    func_00101968(context->drawTask, task);
    btlSetTrackedTaskHandle(9, task);
    return 1;
}

void func_001B81B0(void) {
    u8 *puVar1;
    s64 temp_v0;

    temp_v0 = kwlnTaskGetTaskByName(D_004367E0);
    if (temp_v0 != 0) {
        puVar1 = (u8 *)kwlnTaskGetUserValue(temp_v0);
        *puVar1 = 2;
    }
}

u32 btlHasRegisteredGuidePanelTask(void) {
    s64 temp_v0;

    temp_v0 = btlGetTrackedTaskHandle(9);
    if (temp_v0 != 0) {
        temp_v0 = kwlnTaskIsRegistered(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = kwlnTaskGetTaskByName(D_004367E0);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

extern u32 btlCommandPanelTaskNameRef;

typedef struct BtlPhaseTask {
    s32 phase;
} BtlPhaseTask;

void btlSetTaskPhase5(void) {
    s64 task = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
    if (task != 0) {
        ((BtlPhaseTask *)kwlnTaskGetUserValue(task))->phase = 5;
        btlCommandPanelWork->state = 3;
    }
}

void btlSetTrackedTaskDisplayMode(s32 mode) {
    s32 task = btlGetTrackedTaskHandle(7);
    s32 *state;
    if (task != 0) {
        state = (s32 *)kwlnTaskGetUserValue(task);
        state[2] = mode;
        if (mode == 0) {
            state[1] = 1;
            state[5] = 0x80;
        } else {
            state[1] = 4;
            state[5] = 0xFF;
            state[6] = 0xFF;
        }
    }
}

void btlInvalidateSceneFadeCounts(void) {
    btlGetRuntime();
    kwlnTaskGetUserValue(btlGetTrackedTaskHandle(7));
    btlTrackedTaskHandles->fadeKindsCached = 0;
}

u32 btlHasRegisteredPsechgPanelTask(void) {
    s64 temp_v0;

    temp_v0 = btlGetTrackedTaskHandle(6);
    if (temp_v0 != 0) {
        temp_v0 = kwlnTaskIsRegistered(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = kwlnTaskGetTaskByName(D_004367D8);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B8368);

u32 btlHasRegisteredSkillNamePanelTask(void) {
    s64 temp_v0;

    temp_v0 = btlGetTrackedTaskHandle(1);
    if (temp_v0 != 0) {
        temp_v0 = kwlnTaskIsRegistered(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = kwlnTaskGetTaskByName(D_004367C8);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B8580);

u32 btlHasRegisteredAphNamePanelTask(void) {
    s64 temp_v0;

    temp_v0 = btlGetTrackedTaskHandle(0);
    if (temp_v0 != 0) {
        temp_v0 = kwlnTaskIsRegistered(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = kwlnTaskGetTaskByName(D_004367C4);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

typedef struct MsgQueueTaskData {
    s32 task;
    s32 id;
    s32 unk08;
    s32 counter;
    s32 value;
    s16 fade;
} MsgQueueTaskData;

extern const BattlePanelColors D_004165C0;
extern s32 itfMesMeasureEntryItem(s32, s32, s32);
extern void itfMesBlk24MoveTo(s32, s32, s32);

extern void *sdfAllocAndClearQuadwords(s32);
extern s32 btlDrawTimedDialogTask(s64);
extern void btlReleaseDialogTaskData(s32);
extern s32 btlGetTrackedTaskHandle(s32);

s32 btlReplaceDialogTasksAndQueueMessage(s32 arg0, s32 arg1) {
    s32 context = btlGetRuntime();
    s32 task = btlGetTrackedTaskHandle(0);
    MsgQueueTaskData *data;

    if (btlHasRegisteredAphNamePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy(task, 0);
    }
    data = (MsgQueueTaskData *)sdfAllocAndClearQuadwords(0x40);
    if (btlHasRegisteredSkillNamePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(1), 0);
    }
    if (btlHasRegisteredGuidePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(9), 0);
    }
    if (btlIsNamedBattleTaskRegistered() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(10), 0);
    }
    data->id = arg0;
    data->unk08 = arg1;
    data->value = 0x2D;
    task = kwlnTaskCreate((s32)D_004367C4, 0x2B0E, 1, 1, btlDrawTimedDialogTask,
                          btlReleaseDialogTaskData, data);
    func_00101968(*(s32 *)(context + 0x2C4), task);
    data->task = task;
    btlSetTrackedTaskHandle(0, task);
    return 1;
}

/* Query one of the fourteen task slots retained by the command UI. */
s32 btlGetTrackedTaskHandle(s32 slotIndex) {
    s32 *handle;

    handle = &btlTrackedTaskHandles->handles[slotIndex];
    return *handle;
}

void btlSetTrackedTaskHandle(s32 slotIndex, s32 taskHandle) {
    s32 *handle;

    handle = &btlTrackedTaskHandles->handles[slotIndex];
    *handle = taskHandle;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B88F8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B8A80);


extern void itfMesCleanupWindow(s32, s32);

typedef struct MesWindowSet {
    u32 unk0;
    u32 handle[9];
    u8 pad28[0x20];
    u16 active[9][2];
} MesWindowSet;

void itfMesCloseAllWindows(s32 handle) {
    MesWindowSet *set;
    s32 i;
    btlGetRuntime();
    set = (MesWindowSet *)kwlnTaskGetUserValue(handle);
    for (i = 0; i < 9; i++) {
        if (set->active[i][0] != 0) {
            itfMesCleanupWindow(set->handle[i], 0);
            itfMesDestroyWindowIfPresent(set->handle[i]);
        }
    }
    sdfReleaseChipBlock(set);
    btlSetTrackedTaskHandle(0xA, 0);
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415D58);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415D88);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415DA0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415DC8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415DF0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B8E68);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B9158);

extern void btlInitCursorAndApplyAction(s32, s32, s32);

void btlReleaseTaskAndRefreshCursorIfFlagged(s32 handle) {
    u8 *work = (u8 *)btlGetRuntime();
    u8 *actor;
    sdfReleaseChipBlock(kwlnTaskGetUserValue(handle));
    btlSetTrackedTaskHandle(0xB, 0);
    actor = *(u8 **)(work + 0x184);
    if (*(u16 *)(*(u8 **)(actor + 0x18) + 0x12E) & 0x80) {
        btlInitCursorAndApplyAction((s32)work + 0x70, (s32)work + 0x70, (s32)actor);
    }
    *(u32 *)(work + 0x218) |= 0x100000;
}

/* Advance one corner toward the panel boundary before moving to the next edge. */
s32 btlAdvancePanelCornerPhase(s32 task, BattlePanelEdgeWork *work) {
    f32 center[4];

    memcpy(center, D_00415F80, sizeof(center));

    switch (work->edgePhase) {
    case 0:
        work->corners[0][1] += 80.0f;
        work->corners[0][1] =
            work->corners[0][1] <= center[1] - 174.0f ? center[1] - 174.0f :
            center[1] + 174.0f <= work->corners[0][1] ? center[1] + 174.0f :
            work->corners[0][1];
        if (center[1] + 174.0f <= work->corners[0][1]) {
            work->edgePhase++;
        }
        break;
    case 1:
        work->corners[2][0] += 80.0f;
        work->corners[2][0] =
            work->corners[2][0] <= center[0] - 200.0f ? center[0] - 200.0f :
            center[0] + 200.0f <= work->corners[2][0] ? center[0] + 200.0f :
            work->corners[2][0];
        if (center[0] + 200.0f <= work->corners[2][0]) {
            work->edgePhase++;
        }
        break;
    case 2:
        work->corners[1][1] -= 80.0f;
        work->corners[1][1] =
            work->corners[1][1] <= center[1] - 174.0f ? center[1] - 174.0f :
            center[1] + 174.0f <= work->corners[1][1] ? center[1] + 174.0f :
            work->corners[1][1];
        if (work->corners[1][1] <= center[1] - 174.0f) {
            work->edgePhase++;
        }
        break;
    case 3:
        work->corners[3][0] -= 80.0f;
        work->corners[3][0] =
            work->corners[3][0] <= center[0] - 200.0f ? center[0] - 200.0f :
            center[0] + 200.0f <= work->corners[3][0] ? center[0] + 200.0f :
            work->corners[3][0];
        if (work->corners[3][0] <= center[0] - 200.0f) {
            work->edgePhase++;
        }
        break;
    default:
        return 1;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415F80);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415F90);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B9C70);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BA1E8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415FC0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415FD0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416000);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416030);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416060);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BAB90);

typedef struct BtlPanelStrip {
    s32 x;
    s32 y;
    s32 texture;
} BtlPanelStrip;

extern u32 btlSetSlotLowByteClamped(EffectSlotSet *, s32, s32, s32);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004160F0);

void btlDrawThreePanelSpriteStrips(s32 unused, s32 x, s32 y, s32 delta) {
    BtlPanelStrip strips[3] = { {0, 0, 0x40}, {5, 0, 0x41}, {0x6B, 0, 0x42} };
    u32 colors[4] = { 0x80808080, 0x80808080, 0x80808080, 0x80808080 };
    s32 i, j;
    BtlPanelStrip *strip;

    for (strip = strips, i = 0; i < 3; i++, strip++) {
        for (j = 0; j < 4; j++) {
            colors[j] = btlSetSlotLowByteClamped(btlResourceBlock->resA, strip->texture, j, delta);
        }
        func_00306C28((x + strip->x) << 4, (y + strip->y) << 3,
                     0, colors, 0, btlResourceBlock->resA, strip->texture, 0x53);
    }
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416138);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004162F8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BB1E8);

u32 btlSetSlotLowByteClamped(EffectSlotSet *owner, s32 group, s32 slot, s32 delta) {
    u32 word = owner->workEntries[group].savedColors[slot];
    u32 limit;
    u32 value;
    if (delta > 0) {
        limit = value = word & 0xFF;
        if ((u32)delta < value) {
            value = delta;
        }
    } else {
        limit = word & 0xFF;
        value = 0;
    }
    delta = value;
    if (limit > 0x80) {
        if (delta >= 0x80) {
            delta = limit;
        }
    }
    return (word & ~0xFF) | delta;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BB5C0);

void btlReleaseMessageWindowTask(void) {
    u8 *window = (u8 *)kwlnTaskGetUserValue();
    itfMesCleanupWindow(*(s32 *)(window + 0x24), 0);
    sdfReleaseChipBlock(window);
    btlSetTrackedTaskHandle(9, 0);
}

u32 func_001BB8D0(UiObject *object, s32 current, s32 total, s8 mode) {
    u32 color;

    if (mode == 1 && (object->flags & 0x20) != 0) {
        color = 0x4F4E3E40;
    } else if ((object->statusFlags & 0x4800) != 0) {
        color = 0x4F4E3E40;
    } else if (current * 2 >= total) {
        color = 0xA09DC380;
    } else if (current <= 0) {
        color = 0x4F4E3E40;
    } else {
        u32 nearColor = 0xC8747380;
        color = 0xD1BA7180;
        if (current * 4 < total) {
            color = nearColor;
        }
    }
    return ((color >> 24) | ((color & 0xFF00) << 8)) |
           ((color << 24) | ((color >> 8) & 0xFF00));
}

u8 btlHasRequiredActorStatusBits(s32 arg0) {
    return (~*(u64 *)(arg0 + 0x110) & 0x201) == 0;
}

void func_001BB988(s32 unit, u16 a, u16 b, u16 c) {
    u8 *data = *(u8 **)(*(s32 *)(unit + 0x2C) + 0x18);
    *(u16 *)(data + 0x2CC) = a;
    *(u16 *)(data + 0x2CE) = b;
    *(u16 *)(data + 0x2D0) = c;
}

s32 btlCountEligibleLinkedActors(u8 *scene) {
    u8 *actor = *(u8 **)(scene + 0x24C);
    s32 count = 0;
    while (actor != 0) {
        if ((*(u64 *)(actor + 0x110) & 0x201) == 0x201 &&
            (*(u16 *)(actor + 0x120) & 2) != 0) {
            count++;
        }
        actor = *(u8 **)(actor + 0x364);
    }
    return count;
}

extern s32 D_004367DC;

extern void func_001BBA80();

extern void btlReleaseRegisteredChildTaskWork();

extern void *sdfAllocAndClearQuadwords(s32);

void btlStartRegisteredChildTask(void) {
    u8 *work = (u8 *)btlGetRuntime();
    s32 task = kwlnTaskCreate(D_004367DC, 0x2B0E, 1, 1, func_001BBA80, btlReleaseRegisteredChildTaskWork, sdfAllocAndClearQuadwords(0x20));
    func_00101968(*(s32 *)(work + 0x2C4), task);
    btlSetTrackedTaskHandle(7, task);
}

void func_001BBA60(s32 arg0) {
    *(u32 *)(arg0 + 4) = 1;
    *(u32 *)(arg0 + 12) = 0x80;
    *(u32 *)(arg0 + 16) = 0;
    *(u32 *)(arg0 + 0) = 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BBA80);

void btlReleaseRegisteredChildTaskWork(void) {
    sdfReleaseChipBlock(kwlnTaskGetUserValue());
    btlSetTrackedTaskHandle(7, 0);
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004163A0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004163B0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004163C0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004163F0);

/* B8368 allocates 0x138 bytes. The strip renderer uses the first three rows;
 * the later panel updater handles the other five points and their fades. */
typedef struct BattlePhasePanelWork {
    s32 frames;
    s32 mode;
    s8 phase;
    u8 pad09[0xF];
    s32 waitCounter; /* 0x18: delay before the first slide */
    u8 pad1C[0x1C];
    BattleSelectionPosition current[8]; /* 0x38 */
    BattleSelectionPosition saved[8];   /* 0x78 */
    s32 fade[8][4];                    /* 0xB8 */
} BattlePhasePanelWork;

typedef char BattlePhasePanelWork_size_must_be_0x138[
    (sizeof(BattlePhasePanelWork) == 0x138) ? 1 : -1];
typedef char BattlePhasePanelWork_waitCounter_offset_check[
    ((u32)&((BattlePhasePanelWork *)0)->waitCounter == 0x18) ? 1 : -1];
typedef char BattlePhasePanelWork_current_offset_check[
    ((u32)&((BattlePhasePanelWork *)0)->current == 0x38) ? 1 : -1];
typedef char BattlePhasePanelWork_saved_offset_check[
    ((u32)&((BattlePhasePanelWork *)0)->saved == 0x78) ? 1 : -1];
typedef char BattlePhasePanelWork_fade_offset_check[
    ((u32)&((BattlePhasePanelWork *)0)->fade == 0xB8) ? 1 : -1];

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BC138);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416428);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BC618);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416448);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416458);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416468);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BC8A8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416498);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BCFB0);

extern s32 btlGetNamedTaskPairStatusOrUnavailable();

extern void func_001BC138();

extern void func_001BC618();

extern void func_001BC8A8();

extern void func_001BCFB0();

s32 btlUpdatePhaseGatedTaskUntilTimeout(void) {
    BattlePhasePanelWork *state = (BattlePhasePanelWork *)kwlnTaskGetUserValue();
    s32 frame;
    if ((u32)((btlGetNamedTaskPairStatusOrUnavailable() - 1) & 0xFF) < 2U) {
        return 0;
    }
    func_001BC138(state);
    if (btlTrackedTaskHandles->phaseGate == 0) {
        func_001BC618(state);
    }
    func_001BC8A8(state);
    if (btlTrackedTaskHandles->phaseGate == 0) {
        func_001BCFB0(state);
    }
    frame = state->frames + 1;
    state->frames = frame;
    return frame < 0x32 ? 0 : -1;
}

void btlReleasePsechgPanelWork(void) {
    sdfReleaseChipBlock(kwlnTaskGetUserValue());
    btlSetTrackedTaskHandle(6, 0);
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004164B8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004164C8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BD6E8);

void btlReleaseBattleScratchBlocks(void) {
    sdfReleaseChipBlock(D_00438F4C);
    sdfReleaseChipBlock(D_00438F50);
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416520);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BD9A0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416560);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BDF20);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BE9E8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BEBD0);

void btlReleaseCmsleffPanelWork(void) {
    sdfReleaseChipBlock(kwlnTaskGetUserValue());
    btlSetTrackedTaskHandle(5, 0);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BEF28);

typedef struct BtlPanelTransitionWork {
    u8 pad00[0x10];
    s32 width;
    u8 pad14[0xC];
    s16 fadeLevels[4];
    BattleSelectionPosition initial[2];
    s8 phase;
    u8 pad39[3];
    BattleSelectionPosition current[2];
} BtlPanelTransitionWork;

extern void evtSetDrawSurfaceIndex(u32);
extern void evtSubmitPrimaryGsTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern void evtSubmitPrimaryAlphaBlendMode(s32);
extern void evtSubmitDefaultDepthGradientRect(s32, s32, s32, s32, s32, s32, s32, s32);

void btlUpdatePanelTransitionGradients(BtlPanelTransitionWork *work) {
    s16 *fade;
    s32 i;

    for (i = 3, fade = work->fadeLevels; i >= 0; i--, fade++) {
        *fade += 0x20;
        *fade = *fade <= 0 ? 0 : *fade > 0x80 ? 0x80 : *fade;
    }
    switch (work->phase) {
    case 0:
        work->current[0].x = work->initial[0].x + 0x10;
        work->current[0].y = work->initial[0].y;
        work->current[1].x = work->initial[1].x - 0x10;
        work->current[1].y = work->initial[1].y;
        work->phase++;
        break;
    case 1:
        work->current[0].x -= 2;
        work->current[0].x =
            work->current[0].x <= work->width - work->width / 2 + 0x105 ? work->width - work->width / 2 + 0x105 :
            work->width - work->width / 2 + 0x115 <= work->current[0].x ? work->width - work->width / 2 + 0x115 :
            work->current[0].x;
        work->current[1].x += 2;
        work->current[1].x =
            work->current[1].x <= 0x82 - work->width / 2 ? 0x82 - work->width / 2 :
            0x92 - work->width / 2 <= work->current[1].x ? 0x92 - work->width / 2 :
            work->current[1].x;
        break;
    }
    if (work->fadeLevels[1] < 0x80) {
        evtSetDrawSurfaceIndex(0x53);
        evtSubmitPrimaryGsTest(1, 1, 0x80, 3, 0, 0, 1, 1);
        evtSubmitPrimaryAlphaBlendMode(1);
        evtSubmitDefaultDepthGradientRect(0x100 - work->width / 2,
            work->current[0].y + 0x1A, work->current[0].x - 0x92, 2,
            (work->fadeLevels[1] << 23) | 0x808080, 0,
            (work->fadeLevels[1] << 23) | 0x808080, 0);
        evtSubmitDefaultDepthGradientRect(work->current[1].x,
            work->current[1].y + 2, work->width / 2 - work->current[1].x + 0x100, 2,
            0, (work->fadeLevels[1] << 23) | 0x808080,
            0, (work->fadeLevels[1] << 23) | 0x808080);
    }
}

void btlFreeRegisteredTaskData(s32 handle) {
    btlGetRuntime();
    sdfReleaseChipBlock(kwlnTaskGetUserValue(handle));
    btlSetTrackedTaskHandle(1, 0);
}

extern s8 D_0037F53D[];

void btlToggleModelFlagOnInput(void) {
    if (D_0037F53D[0] < 0) {
        if (mdlFlagTest(0xC0E) != 0) {
            mdlFlagClear(0xC0E);
        } else {
            mdlFlagSet(0xC0E);
        }
    }
}

s32 btlDrawTimedDialogTask(s64 task) {
    BattlePanelColors colors = D_004165C0;
    MsgQueueTaskData *data;
    s32 expired;
    s32 width;
    s32 half;
    s32 i;

    if (btlGetTrackedTaskHandle(0) == 0) {
        return 0;
    }
    data = (MsgQueueTaskData *)kwlnTaskGetUserValue(task);
    if (data->counter++ < data->value) {
        expired = 0;
    } else {
        expired = 1;
    }
    width = itfMesMeasureEntryItem(data->id, data->unk08, 0);
    data->fade = 0x80;
    for (i = 0; i < 4; i++) {
        colors.values[i] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 0x15, i, data->fade);
    }
    half = width / 32;
    func_00306C28(((width >> 4) - half + 0x105) << 4, 0x200, 0,
                  colors.values, 0, btlResourceBlock->resC, 0x17, 0x53);
    btlResourceBlock->resC->workEntries[0x16].width = (width >> 4) << 4;
    func_00306C28((0x100 - half) << 4, 0x200, 0,
                  colors.values, 0, btlResourceBlock->resC, 0x16, 0x53);
    btlResourceBlock->resC->workEntries[0x16].width =
        btlResourceBlock->resC->workEntries[0x16].sourceWidth << 4;
    func_00306C28((0x92 - half) << 4, 0x200, 0,
                  colors.values, 0, btlResourceBlock->resC, 0x15, 0x53);
    if (expired) {
        return -1;
    }
    itfMesBlk24MoveTo(data->id, (0x100 - (width >> 5)) << 4, 0x220);
    evtSetDrawSurfaceIndex(0x53);
    evtSubmitPrimaryGsTest(1, 1, 0x80, 3, 0, 0, 1, 1);
    evtSubmitPrimaryAlphaBlendMode(0);
    itfMesStartEntry(data->id, data->unk08, 0);
    return 0;
}

void btlReleaseDialogTaskData(s32 handle) {
    u8 *window;
    btlGetRuntime();
    window = (u8 *)kwlnTaskGetUserValue(handle);
    itfMesCleanupWindow(*(s32 *)(window + 4), 0);
    sdfReleaseChipBlock(window);
    btlSetTrackedTaskHandle(0, 0);
}

void btlCreateMessageWindow(void) {
    u8 *window;
    btlGetRuntime();
    window = (u8 *)sdfAllocAndClearQuadwords(0x40);
    D_004367F8 = (u32)window;
    *(s32 *)(window + 0x10) = 0x14;
    *(s32 *)(window + 0x18) = 0x1800080;
    *(s32 *)(window + 0x1C) = 0x40800080;
    *(s32 *)(window + 0x20) = 0x40800080;
    *(s32 *)(window + 0x24) = 0x60808080;
    *(s32 *)(window + 0x38) = 0xBB;
    *(s32 *)(window + 0x3C) = 0x196;
    btlSetTrackedTaskHandle(4, 1);
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004165A0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004165B0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004165C0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BF978);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004165E0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004165F0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416600);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416610);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416620);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BFB58);

void btlReleaseRegisteredTaskBuffer(void) {
    btlGetRuntime();
    sdfReleaseChipBlock(D_004367F8);
    D_004367F8 = 0;
    btlSetTrackedTaskHandle(4, 0);
}

s32 func_001C0008(void) {
    s32 i;
    s32 *flags;
    if (mdlFlagTest(0xb8f)) {
        return 0;
    }
    flags = (s32 *)(datGameState + 0x16f00);
    for (i = 0; i < 4; i++) {
        if (flags[i]) {
            return 0;
        }
    }
    return 1;
}

extern u8 D_003B5D10[];

s32 func_001C0080(s32 arg0) {
    s32 count;
    s32 i;

    func_001B7A00();
    count = func_001AC750(arg0, D_003B5D10);
    if (count != 0) {
        for (i = 0; i < count; i++) {
            if (func_001B7940(*(u16 *)(D_003B5D10 + 4 + i * 12), 2) == 0) {
                return 1;
            }
        }
        return 0;
    }
    return count;
}

s64 btlGetTaskState6(void) {
    s64 task = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
    if (task != 0) {
        s32 *state = *(s32 **)(kwlnTaskGetUserValue(task) + 0x2C);
        return func_001C0080(state[6]);
    }
    return task;
}

typedef struct FlagEntry {
    u32 unk0;
    u16 id;
    u8 pad6[6];
} FlagEntry;

extern FlagEntry *fldGetCachedSceneActorNameAndId(s32, u16 *);
extern s32 func_001B7940(s32, s32);

s64 btlClearFlagEntries(void) {
    u16 count;
    s64 task = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
    FlagEntry *entries;
    s32 i;
    if (task != 0) {
        entries = fldGetCachedSceneActorNameAndId(kwlnTaskGetUserValue(task), &count);
        for (i = 0; i < count; i++) {
            func_001B7940(entries[i].id, 0);
        }
        return 1;
    }
    return task;
}

extern u32 D_004367EC;

extern s32 dspCloseChannel(void);

s64 btlDestroyTaskC(void) {
    s64 result = kwlnTaskGetTaskByName(D_004367EC);
    if (result != 0) {
        dspCloseChannel();
        if (btlGetTrackedTaskHandle(0xC) != 0) {
            kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(0xC), 0);
        }
        result = 1;
    }
    return result;
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416650);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C0240);

extern s32 evtFinishMessageWindowAndNotify();

void btlFinishTrackedBattleTaskAndCloseWindow(s32 handle) {
    u8 *work = (u8 *)btlGetRuntime();
    sdfReleaseChipBlock(kwlnTaskGetUserValue(handle));
    btlSetTrackedTaskHandle(0xC, 0);
    *(u32 *)(work + 0x218) |= 0x100000;
    evtFinishMessageWindowAndNotify();
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416680);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C0630);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C0828);

extern u32 D_004367F0;

s64 btlDestroyTaskD(void) {
    s64 result = kwlnTaskGetTaskByName(D_004367F0);
    if (result != 0) {
        dspCloseChannel();
        if (btlGetTrackedTaskHandle(0xD) != 0) {
            kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(0xD), 0);
        }
        result = 1;
    }
    return result;
}

void btlReleaseDialogTaskAndMarkBattleState(s32 handle) {
    BattleController *battle = (BattleController *)btlGetRuntime();
    sdfReleaseChipBlock(kwlnTaskGetUserValue(handle));
    btlSetTrackedTaskHandle(0xD, 0);
    battle->flags |= 0x100000;
    btlTrackedTaskHandles->status.flags &= ~0x100;
    evtFinishMessageWindowAndNotify();
}

extern u8 btlSoundSlotDefaults[];

void btlInitializeSelectionWork(void) {
    u8 initial[0x20];
    BattleSelectionWork *allocated;
    u32 *source;
    s32 *destination;
    s16 *state;
    s32 i;
    memcpy(initial, btlSoundSlotDefaults, sizeof(initial));
    allocated = sdfAllocAndClearQuadwords(0x30);
    btlLinkedSelectionTaskBuffer = allocated;
    state = allocated->rowFade;
    /* Retail copies coordinate pairs with its cursor on each row's Y word. */
    destination = &allocated->positions[0].y;
    source = (u32 *)initial;
    for (i = 3; i >= 0; i--) {
        *state = 0;
        state++;
        destination[-1] = source[0];
        destination[0] = source[1];
        destination += 2;
        source += 2;
    }
    btlSetTrackedTaskHandle(2, 1);
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", btlSoundSlotDefaults);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C0EF0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416740);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C1300);

void btlReleaseSelectionTaskBuffer(void) {
    if (btlGetTrackedTaskHandle(2) != 0) {
        sdfReleaseChipBlock(btlLinkedSelectionTaskBuffer);
    }
    btlSetTrackedTaskHandle(2, 0);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C1520);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C16B0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416780);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416798);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C1A68);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C1F10);

void btlReleaseStwrPanelResource(void) {
    sdfReleaseResourceAllocation(*(SdfMemBlock **)kwlnTaskGetUserValue());
    btlSetTrackedTaskHandle(3, 0);
}

s32 btlAreLinkedSceneCountersAtThreshold(void) {
    BattleSelectionWork *base;
    s32 i;
    if (btlGetTrackedTaskHandle(8) == 0) {
        if (btlGetTrackedTaskHandle(2) != 0) {
            base = btlLinkedSelectionTaskBuffer;
            for (i = 0; i < 4; i++) {
                if (base->rowFade[i] < 0x80) {
                    return 0;
                }
                if (base->positions[i].x < 11) {
                    return 0;
                }
            }
            if (base->positions[1].y == base->positions[0].y + 23) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C2450);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C26E0);

typedef struct BtlSlot {
    u8 pad0[4];
    u8 state;
    u8 pad5[0x28B];
} BtlSlot;

typedef struct BtlSlotBank {
    u8 pad0[8];
    s32 count;
    u8 padC[0x7C4];
    BtlSlot slots[1];
} BtlSlotBank;

void btlSlotBankPromoteStates(BtlSlotBank *bank) {
    s32 i;
    for (i = 0; i < bank->count; i++) {
        BtlSlot *slot = &bank->slots[i];
        s32 state = slot->state;
        if (state == 1 || state == 2) {
            slot->state = 4;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C2A98);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C2EA8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C3168);

void btlReleaseTrackedTaskResource(void) {
    sdfReleaseResourceAllocation(*(SdfMemBlock **)(kwlnTaskGetUserValue(kwlnTaskGetTaskByName(D_004367CC)) + 0x1200));
    btlSetTrackedTaskHandle(8, 0);
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004167E0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416800);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416820);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416830);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436630);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436638);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", sndTestMessageResourceIndex);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", sndTestMessageTexture);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436648);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436650);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436658);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436660);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436668);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436670);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436678);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436680);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436688);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436690);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436698);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004366A0);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004366A8);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004366B0);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004366B4);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004366B8);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004366C0);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004366C8);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004366D0);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004366D8);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004366E0);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", btlRuntime);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004366E8);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004366F0);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004366F8);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436700);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436708);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436710);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436718);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436720);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436728);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436730);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436738);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436740);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436748);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436750);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436758);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436760);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436768);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436770);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436778);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436780);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436788);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436790);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436798);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367A0);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367A8);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367B0);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367B8);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", btlCommandPanelTaskNameRef);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367C0);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367C4);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367C8);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367CC);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367D0);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367D4);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367D8);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367DC);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367E0);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", btlMahenPanelTaskNameRef);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", btlAnalyzPanelTaskNameRef);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367EC);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367F0);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", btlTrackedTaskHandles);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367F8);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", btlCommandPanelWork);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436800);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", btlResourceBlockLoaded);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", btlResourceBlock);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436808);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436810);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436818);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436820);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436828);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436830);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436838);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436840);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436848);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436850);
