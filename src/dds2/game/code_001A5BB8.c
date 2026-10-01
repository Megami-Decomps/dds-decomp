#include "common.h"

extern u32 D_004367F8;

extern u32 D_004367CC;

extern s32 dds3FindEntryIndex();

extern s32 btlGetIndexedPartyEntryRecord(s32);

extern s32 func_00101820(u32);

extern s8 D_003B4CF8[13];

extern u32 D_00436640;

extern s32 D_00436644;

extern s32 itfLoadTextureFromAsset(u32);

extern u64 D_004366E8;

extern s32 btlRuntime;

extern s32 D_00435DE0;

extern s32 D_00435DF0;

extern s32 D_00435DE4;

extern s32 D_00435DF8;

extern s32 D_00435E38;

extern s32 func_001AA6F8(void);

extern u8 *btlCommandPanelWork;

extern s32 D_00435DD0;

extern u32 D_004367E4;

extern s64 func_00101740();

extern s64 kwlnTaskIsRegistered(s64);

extern u32 kwlnTaskGetUserValue();

extern u32 D_004367E8;

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

typedef struct BtlSlotRecord {
    u8 pad_00[0x84];
    u32 word[7];
} BtlSlotRecord;

typedef struct BtlSlotOwner {
    u8 pad_00[0x18];
    BtlSlotRecord *records;
} BtlSlotOwner;

typedef struct BattleController {
    u8 pad_000[0x214];
    s32 frame;
    u32 flags;
    u32 flags21C;
    u8 pad_220[0x2C];
    UiObject *actors;
    u8 pad_250[0x74];
    s32 drawTask;
} BattleController;

typedef struct BattleEffect {
    u32 flags;
    u8 pad_004[0xA0];
    u32 resourceHandles[3];
} BattleEffect;

extern s32 D_003B4F70[];

extern s32 D_00435E20;

extern s32 btlTrackedTaskHandles;

extern s32 btlGetTrackedTaskHandle(s32);

extern s32 D_003B6928[];

extern s32 D_00435DEC;

extern s32 D_00435E1C;

extern char D_00415158[];

extern s32 btlBossDebugPrintf(const char *, ...);

extern s8 D_0037F550[];

extern void *D_003B4E40[];

extern s32 D_003B4F78[];

extern s32 D_003B4F74[];

extern f32 D_003B4E28[];

extern s32 btlCheckSpecialAbility(s32, s32);

extern s32 mdlFlagTest(s32);

extern u32 mnuGetPartyEntryCurrentId(s32);

extern s8 *D_00435E24;

extern s32 btlDoesEnabledStatusMatchCurrentId(s32, u32);

extern s32 evtCheckValueThreshold(s32, s32);

extern void mdlFlagSet(s32);

extern void mdlFlagClear(s32);

extern char D_00415638[]; /* "btl:hunt mp rec[%d]\n" */

typedef struct BtlRates {
    u8 pad0[0xE8];
    f32 unkE8;
    u8 padEC[0x5C];
    f32 hpRate;
    u8 pad4[0x124];
    f32 mpRate;
} BtlRates;

extern BtlRates *D_00435E2C;

extern u8 *D_00435E04;

extern u8 *D_00435DF4;

extern s32 func_001B32F8(s32, s32 *);

extern char D_00415840[]; /* "btl:endure=%d%%[ratio=%.2f]\n" */

typedef struct ActorClassIds {
    s16 values[8];
} ActorClassIds;

extern const ActorClassIds D_00415B08;

typedef struct ActorClassPairTable {
    u32 values[14];
} ActorClassPairTable;

extern const ActorClassPairTable D_00415B18;

extern u32 D_00438F48;

typedef struct SndMessageNode {
    u32 flags;
    struct SndMessageNode *next;
    s32 message;
} SndMessageNode;

typedef struct SoundQueue {
    s32 unk00;
    s32 allocation;
    s32 unk08;
    u16 drawFlags;
    u16 unk0E;
    SndMessageNode *head;
    u32 unk14;
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

extern void func_001A0CA0(void *arg0, void *arg1, void *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

typedef struct UiSprite { u8 pad0[0x20]; s32 unk20; s32 unk24; } UiSprite;
typedef struct UiPanelPlacement {
    UiSprite *frame;
    UiSprite *sprite;
    UiSprite *overlay;
    s32 offsetX;
    s32 offsetY;
    s32 rightX;
    s32 bottomY;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
} UiPanelPlacement;
typedef struct UiTexRef { u8 pad0[4]; s32 unk4; s32 unk8; s32 unkC; } UiTexRef;
typedef struct UiPos { s32 x; s32 y; s32 unk8; UiTexRef *chain; } UiPos;
typedef struct UiPanelOrigin { s32 x; s32 y; UiTexRef *tex; } UiPanelOrigin;
typedef struct UiPanel {
    u32 flags;
    u8 pad4[8];
    s32 unkC;
    s16 index;
    s16 state;
    UiPanelOrigin origin;
    s32 pad20;
    UiPos pos;
    u8 pad34[0x70];
    UiPanelPlacement place;
} UiPanel;
extern s32 func_0019DBA8();
extern UiSprite *func_001A1858();
extern void itfSetPanelLayoutAndNotify();
extern void func_001A1A50();

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

extern void itfMesSetRowItemFlag();
extern void sndSetSequenceVolumePan();
extern void sndStepSequenceIndex(SndSeqSelect *sel, s32 dir);
typedef struct SndPad {
    u8 pad00[0x21];
    s8 confirm;
    u8 pad22[4];
    s8 prev;
    s8 next;
} SndPad;
extern SndPad D_0037F510;
extern s32 func_001A6AB8();
extern void itfResetBattleFadeState(s32, s32);

typedef struct UiSurface { u8 pad0[0x20]; } UiSurface;
typedef struct UiOwnerRef { u8 pad0[0xC]; struct UiPanel *owner; } UiOwnerRef;
extern UiSurface D_0037FB48[];
extern UiOwnerRef *D_003B4778[];
extern void itfBuildAndSubmitPanelPacket(UiSprite *sprite, UiSurface *surface);
extern s32 func_001A7798(UiSprite *sprite);

typedef struct BtlEntry {
    u16 flags;
    u8 pad2[4];
    u16 hp;
    u8 pad8[2];
    u16 mp;
    u8 padC[2];
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
extern s32 func_001197C0();
extern s32 func_001198C0();

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
    top = pos->y + place->offsetY;
    itfSetPanelLayoutAndNotify(place->sprite, pos->x + place->offsetX, top, pos->x + place->rightX, pos->y + place->bottomY, panel->unkC);
    place->sprite->unk24 = top;
    func_001A1A50(place->sprite, place->unk1C, place->unk20, place->unk24, 0);
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
            func_001A1A50(place->frame, 0x7F, 0x7F, 0x7F, 0);
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
    s32 handles[32];
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
            func_003297C8(entries[0x20]);
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
    func_001A6160();
    func_001A6350(arg0);
    func_001A6528(arg0);
    btlUpdateFadeIndicator(arg0);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A6160);

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

typedef struct BtlFade {
    u8 kind;
    u8 pad1;
    s16 phase;
    s16 alpha;
    s16 timer;
} BtlFade;

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

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A6BF8);

void itfMesRenderActivePanelSprites(UiPanel *panel) {
    UiPanelPlacement *place;
    if (panel->flags & 0x80000) {
        return;
    }
    place = &panel->place;
    if ((panel->flags & 0x300) >= 0x100) {
        if (panel->state != 3) {
            itfBuildAndSubmitPanelPacket(place->sprite, &D_0037FB48[panel->index]);
        }
        itfMesWork.drawFlags |= 2;
        if (place->overlay != 0) {
            itfBuildAndSubmitPanelPacket(place->overlay, &D_0037FB48[panel->index]);
        }
    }
    if (D_003B4778[0] != 0 && D_003B4778[0]->owner == panel && place != 0) {
        func_001A7798(place->sprite);
    }
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A6E88);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A7120);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A74F0);

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

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A7798);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A7878);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A7A08);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A7A98);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A7B00);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00414D50);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00414D60);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A7C08);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A81F0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A85E0);

extern s32 D_00435CBC;

extern s32 kwlnTaskCreate(s32 name, s32 arg1, s32 arg2, s32 arg3, void *update, void *destroy, void *data);

extern void func_001A8918();

extern s32 func_001A8848();

extern u8 D_003B4A28[];

extern s32 scrCreateTaskForProcessId();

extern void itfMesSetWindowCallbackAddress();

void sndCreateTestMsgTasks(void) {
    D_00435CBC = 0x80FFFFFF;
    sndCycleTestMessageResource();
    itfMesSetWindowCallbackAddress(*(s32 *)(kwlnTaskGetUserValue(scrCreateTaskForProcessId(0x3E8, D_003B4A28, 0)) + 0xCC), func_001A8918);
    kwlnTaskCreate((s32)"TestMsgMngC", 0x3EF, 0, 0, func_001A8848, 0, 0);
    kwlnTaskCreate((s32)"TestMsgMngD", 0x2AFE, 0, 0, sndUpdateTestMsgTask, 0, 0);
}

void sndCycleTestMessageResource(void) {
    if (D_00436644 != 0) {
        sdfTexReleaseReferenceViaHandler(D_00436644);
        D_00436644 = 0;
    }
    D_00436640 = (D_00436640 + 1) & 3;
    if (D_00436640 != 3) {
        D_00436644 = itfLoadTextureFromAsset(*(u32 *)(D_003B4CF8 + D_00436640 * 4));
    }
}

s32 func_001A8848(void) {
    if (D_0037F530[0] < 0) {
        sndCycleTestMessageResource();
    }
    return 0;
}

s32 sndUpdateTestMsgTask(void) {
    s32 mem;
    if ((D_00436644 != 0) && (D_00436640 != 3)) {
        mem = sdfAllocPacketAligned(0x20);
        sdfInitPacketList(mem);
        itfSendTablePacket(mem, 0, 0);
        func_001A0CA0(D_003B4D08, D_003B4D18, D_003B4D28, 0xFFF, D_00436644, 0, mem);
        D_003805A8.submitPacket(&D_003805A8, mem);
        return 0;
    }
    return 0;
}

extern s32 func_0035B6E0();

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00414EB0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00414EC8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00414EE0);

void func_001A8918(void) {
    func_0035B6E0("********* AAAA ********\n");
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A8938);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A8A70);

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

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A9798);

void func_001A9910(void) {
    D_004366E8 = 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", btlAdvanceRuntimeSequenceCounter);

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

extern s32 func_001E7960(void);

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
        func_001E7960();
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

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A9B80);

INCLUDE_ASM(const s32, "game/code_001A5BB8", btlExitWhenAudioAndTasksIdle);

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

void btlResetActorEntryState(void) {
    s32 context = btlRuntime;
    s32 *entries = (s32 *)(D_00435DD0 + 0xC18);
    u32 i;
    *(s32 *)(context + 0x2E8) = 0;
    *(s32 *)(context + 0x2EC) = 0;
    *(s32 *)(context + 0x2F0) = 0;
    *(s32 *)(context + 0x2F4) = 0;
    *(s32 *)(context + 0x2F8) = 0;
    *(u16 *)(context + 0x2FC) = 0;
    for (i = 0; i < 5; i++) {
        *entries = 0;
        entries = (s32 *)((u8 *)entries + 0x1C4);
    }
    memset((void *)(btlRuntime + 0x2DC), 0, 12);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AA400);

INCLUDE_ASM(const s32, "game/code_001A5BB8", btlResolveQueuedSceneRequestParameters);

s32 func_001AA6F8(void) {
    return btlRuntime;
}

u16 func_001AA700(s32 arg0) {
    return *(u16 *)(arg0 + 6);
}

u16 func_001AA708(s32 arg0) {
    return *(u16 *)(arg0 + 10);
}

void func_001AA710(void) {
    func_001188F0();
}

void func_001AA728(void) {
    func_001189D0();
}

s32 func_001AA740(void *stats) {
    return func_001197C0(stats);
}

s32 func_001AA758() {
    return func_001198C0();
}

void func_001AA770(void) {
    datMoveCursorX();
}

void func_001AA788(void) {
    datMoveCursorY();
}

u16 btlRefreshUnitMaximumHpAndClampCurrentHp(s32 object) {
    u16 maximum = func_001AA700(object);
    u32 value = func_001AA740(object);
    *(u16 *)(object + 8) = value;
    if (value < maximum) {
        *(u16 *)(object + 6) = value;
    }
    return *(u16 *)(object + 6);
}

u16 btlRefreshUnitMaximumMpAndClampCurrentMp(s32 object) {
    u16 maximum = func_001AA708(object);
    u32 value = func_001AA758(object);
    *(u16 *)(object + 12) = value;
    if (value < maximum) {
        *(u16 *)(object + 10) = value;
    }
    return *(u16 *)(object + 10);
}

u16 func_001AA840(s32 arg0) {
    return *(u16 *)(arg0 + 0xe) & 0x7fff;
}

void func_001AA850(void) {
    sdfRaisePackedChannelValue();
}

void func_001AA868(void) {
    datClearUnitStatusBits();
}

void btlSetActorSelectedEntryIndex(UiObject *actor, u32 value) {
    // GCC needs integer-address arithmetic to retain the original store shape.
    *(u32 *)((s32)actor + 0x310) = value;
}

void btlClearActorSelectedEntryIndex(UiObject *actor) {
    *(u32 *)((s32)actor + 0x310) = 0xffffffff;
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
    return D_00435DD0 + temp_v0 * 0x1c4 + 0xa60;
}

s32 btlGetIndexedPartyEntryRecord(s32 index) {
    return D_00435DD0 + index * 0x1c4 + 0xa60;
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
    maxHp = func_001197C0(dst);
    maxMp = func_001198C0(dst);
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
    for (unit = ((BattleController *)func_001AA6F8())->actors; unit != 0; unit = unit->next) {
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
    return *(s32 *)(D_00435DEC + *(u16 *)(entry + 4) * 76);
}

void func_001AB8C0(void) {
    func_00119A10();
}

void func_001AB8D8(void) {
    func_00119A78();
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", btlApplyCommandAbilityMultiplier);

s32 btlGetSlotValueAdjustedForSpecialAbility(s32 battler, s32 slot) {
    s32 value = D_00435E24[slot * 16 - 0x1aa4];
    if (btlDoesEnabledStatusMatchCurrentId(battler + 0x120, 0xe4) && (u32)value >= 2) {
        value--;
    }
    return value;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ABA40);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ABB10);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ABCC0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ABDE8);

s8 btlGetActorIndexedSignedValue(UiObject *object, s32 index) {
    if (index == 0 && (object->flags & 0x400) != 0) {
        return *(s8 *)(D_00435DEC + object->index * 76 + 0x46);
    }
    return *(s8 *)(D_00435E1C + index * 2);
}

extern void btlResolveUnitValueWithOverride();

void func_001ABF50(s32 battler) {
    btlResolveUnitValueWithOverride(battler + 0x120);
}

extern void func_00119C78(s32, s32);

void btlResolveUnitValueWithOverride(s32 arg0, s32 arg1) {
    s32 (*hook)(s32, s32) = *(s32 (**)(s32, s32))(func_001AA6F8() + 0x6B8);
    if (hook != 0) {
        if (hook(arg0, arg1) != -1) {
            return;
        }
    }
    func_00119C78(arg0, arg1);
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
    u16 item = *(u16 *)(D_00435E38 + index * 8 + 2);
    btlBossDebugPrintf(D_00415158, index, item);
    return item;
}

u16 btlGetActorBedAssetIdFromIndex(s32 arg0) {
    return *(u16 *)(arg0 * 8 + D_00435E38 + 2);
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415158);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AC0F8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AC360);

INCLUDE_ASM(const s32, "game/code_001A5BB8", btlFindEligibleTargetForMultiActorCommand);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AC648);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AC750);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ACD10);

INCLUDE_ASM(const s32, "game/code_001A5BB8", btlEncodeActorIndexAsSelectionMask);

void func_001AD090(void) {
    func_00119F68();
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

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AD118);

s32 btlDoesEnabledStatusMatchCurrentId(s32 status, u32 value) {
    if (*(u16 *)status & 0x20) {
        return 0;
    }
    return mnuGetPartyEntryCurrentId(status) == value;
}

s32 btlSelectActorAction(s32 state) {
    s32 selection = func_001AD310(state, 1);
    if (selection == 0) {
        selection = (effMiscRand((s32)D_0037F550) & 1) ? 2 : 9;
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

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AD5B0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AD698);

/* Readiness requires each active actor to have cleared transient action flags. */
s32 btlAllActiveUnitsReady(void) {
    UiObject *unit;
    u32 flags;
    for (unit = ((BattleController *)func_001AA6F8())->actors; unit != 0; unit = unit->next) {
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

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AD978);

u32 func_001ADA10(void) {
    s32 controller;

    controller = func_001AA6F8();
    return *(u32 *)(controller + 0x278);
}

extern u32 effMiscRandMod(s32, s32);

s32 btlChooseAvailableUnit(void) {
    s32 candidates[16];
    s32 count = 0;
    u8 *actor;
    u8 *unit;
    u32 flags;
    for (actor = *(u8 **)(func_001AA6F8() + 0x248); actor != 0; actor = *(u8 **)(actor + 0x178)) {
        if (!(*(u32 *)(actor + 8) & 8)) {
            continue;
        }
        unit = *(u8 **)(actor + 0x18);
        flags = *(u32 *)(unit + 0x110);
        if (flags & 1) {
            if (flags & 0x200) {
                if (flags & 2) {
                    if (!(flags & 0xE0)) {
                        candidates[count] = (s32)actor;
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
    for (unit = ((BattleController *)func_001AA6F8())->actors; unit != 0; unit = unit->next) {
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
    for (unit = ((BattleController *)func_001AA6F8())->actors; unit != 0; unit = unit->next) {
        flags = unit->flags;
        if (flags & 1) {
            if (flags & 0x200) {
                if (!(flags & 0xE0)) {
                    count++;
                }
            }
        }
    }
    entry = (u8 *)(D_00435DD0 + 0xA60);
    for (i = 4; i >= 0; i--) {
        if (*(u16 *)entry & 1) {
            if (!(*(u16 *)entry & 2)) {
                if (!(*(u16 *)(entry + 0xE) & 0x4000)) {
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
    u8 pad4[4];
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

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ADD30);

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

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ADE18);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ADFE0);

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

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AE678);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AE8C0);

s32 btlHasSpecialAbility274(s32 unit, u32 slot) {
    if (((BattleController *)func_001AA6F8())->flags21C & 0x20000) {
        return 0;
    }
    if (slot < 2 && btlCheckSpecialAbility(unit + 0x120, 0x274)) {
        return 1;
    }
    return 0;
}

s32 btlHasEnabledSpecialAbilityForSlot(s32 unit, u32 slot) {
    if (((BattleController *)func_001AA6F8())->flags21C & 0x20000) {
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

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AEC18);

f32 btlGetClampedBattleTableValue(void) {
    u16 index = *(u16 *)(func_001AA6F8() + 0x47c);
    if (index > 4) {
        index = 4;
    }
    return D_003B4E28[index];
}

s32 sndGetResourceForIndex(s32 index) {
    s8 resource = *(s8 *)(D_00435E1C + index * 2);
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

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B0760);

s32 btlTestActorStatusPredicate(u8 *unit) {
    s32 (*hook)(u8 *) = *(s32 (**)(u8 *))(func_001AA6F8() + 0x698);
    if (hook != 0) {
        if (hook(unit) != 0) {
            return 1;
        }
    }
    return (*(u16 *)(unit + 0x12E) & 0x2806) != 0;
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
    if (*(u32 *)(func_001AA6F8() + 0x220) & 0x80) {
        return 0;
    }
    if (*(u32 *)(unit + 0x114) & 8) {
        return 0;
    }
    if (!(flagsA & 1)) {
        return 0;
    }
    if (*(u16 *)(unit + 0x12E) & 1) {
        return 0;
    }
    threshold = 0x1E;
    if (!(flagsB & 2)) {
        threshold = !(flagsB & 4) ? 0 : 0x28;
    }
    btlBossDebugPrintf(D_00415440, threshold);
    return btlRollAiBucket() < threshold;
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415440);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B09F0);

f32 func_001B0B20(void) {
    return 1.5f;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B0B30);

u8 btlGetActorDisplayByteWithDefault(UiObject *object, s32 index) {
    if (index == 0) {
        if ((object->flags & 0x400) != 0) {
            return *(u8 *)(D_00435DEC + object->index * 76 + 0x48);
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
        return *(s8 *)(D_00435E1C + id * 2 + 1) == 2 ? 0x2D : 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B0DB0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B1090);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B1168);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B1350);

s32 btlQueryUnitChannelFlags(s32 first, s32 second, s32 other, s32 variant, s32 mode) {
    s32 flags;
    if (mode != 1) {
        return 0;
    }
    flags = sdfQueryChannelBits(other, first + 0x120, second + 0x120);
    if ((*(u16 *)(second + 0x12E) & 8) != 0 && variant == 2) {
        flags |= 8;
    }
    return flags;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B16B0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B17E8);

void func_001B1F78(void) {
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", btlCalculateEnemyExperienceReward);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B20C8);

s32 btlGetEnemyMoney(u8 *acquirer, u8 *enemy) {
    s32 result = 0;
    s32 money;
    u8 *entry;
    if (!(*(u32 *)(enemy + 0x110) & 0x400)) {
        return result;
    }
    if (acquirer != 0 && !(*(u32 *)(acquirer + 0x110) & 0x200)) {
        return result;
    }
    entry = (u8 *)(D_00435DEC + *(u16 *)(enemy + 0x124) * 0x4C);
    money = *(s32 *)(entry + 0x28);
    if (*(u32 *)entry & 0x2000) {
        money *= 100;
    }
    if (acquirer != 0) {
        btlBossDebugPrintf("btl:money=%d\n", money, acquirer);
    } else {
        btlBossDebugPrintf("btl:money=%d(acquisition)\n", money);
    }
    return money;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", btlCalculateHuntEpReward);

u32 func_001B2380(void) {
    return 0;
}

s32 btlCalculateAbilityRecoveryAmount(u8 *unit) {
    s32 recovery = 0;
    if (btlCheckSpecialAbility((s32)unit + 0x120, 0x26E) != 0) {
        recovery = (s32)(*(u16 *)(unit + 0x12C) * D_00435E2C->mpRate);
    } else if (btlCheckSpecialAbility((s32)unit + 0x120, 0x249) != 0) {
        recovery = (s32)(*(u16 *)(unit + 0x12C) * D_00435E2C->hpRate);
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
    if ((*(u16 *)(unit + 0x12E) & 0x7FFF) == 0x4000) {
        return 1;
    }
    if (!(*(u32 *)(func_001AA6F8() + 0x218) & 0x80)) {
        return 0;
    }
    return *(u16 *)(unit + 0x126) + delta < 1;
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
    for (actor = ((BattleController *)func_001AA6F8())->actors; actor != 0; actor = actor->next) {
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
    func_001AA6F8();
    kind = btlGetActorIndexedSignedValue(arg0, arg2);
    mask = btlEncodeActorIndexAsSelectionMask(kind);
    power = *(u16 *)(unit->selectedEntryIndex * 0x38 + D_00435E20 + 0x2E);
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

    temp_v0 = *(u16 *)(D_00435E20 + arg0 * 56 + 0x2e);
    return D_003B4F70[temp_v0 * 3];
}

s32 btlTestSelectedItemCategoryMask(UiObject *unit, s32 arg) {
    s32 index = unit->selectedEntryIndex;
    u16 kind;
    if (index == -1) {
        return 0;
    }
    kind = *(u16 *)(D_00435E20 + index * 56 + 0x2E);
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
    property = *(u16 *)(D_00435E20 + index * 56 + 0x2E);
    return D_003B4F74[property * 3];
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B29F0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B2AF8);

s32 btlIsSelectedActorStatusAndRecordClear(u8 *unit) {
    s32 work = func_001AA6F8();
    if (*(s8 *)(D_00435E04 + *(s32 *)(work + 0x2A0) * 40) != 0) {
        return 0;
    }
    if (*(u16 *)(unit + 0x12E) & 0x2A0F) {
        return 0;
    }
    return *(u8 *)((u8 *)D_00435DF4 + *(u16 *)(unit + 0x124) * 0x15C) == 0;
}

s32 btlAreUnitStatusAndEntryFlagsClear(s32 arg0) {
    if ((*(u16 *)(arg0 + 0x12e) & 0x40) != 0) {
        return 0;
    }
    return (btlGetEntryFlagsUnlessDisabled(arg0 + 0x120) & 0x40) < 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B2D70);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B2F50);

s32 btlHasAdjacentActorRecordStatus(UiObject *object) {
    u8 *status = (u8 *)(D_00435DEC + object->index * 76 + 0x3E);
    u32 i;
    for (i = 0; i < 2; i++) {
        if (*status++ != 0) {
            return 1;
        }
    }
    return 0;
}

u32 btlIsActorHighStateFlagClear(s32 arg0) {
    return ((*(s32 *)(arg0 + 0x110) >> 0x1a) ^ 1U) & 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B3200);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B32F8);

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
                s32 category = *(s8 *)(D_00435E1C + id * 2 + 1);
                if (category != 2) {
                    if (category != 4) {
                        if ((*(u8 *)(D_00435E20 + id * 56 + 1) & 2) != 0) {
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
    s32 mode = *(u16 *)(index * 0x38 + D_00435E20 + 0x16);
    f32 rate;
    if (mode < 0xE) {
        rate = 1.0f;
        if (mode >= 0xC) {
            return rate;
        }
    }
    if (index == 0 && btlCheckSpecialAbility(unit + 0x120, 0x23D) != 0) {
        rate = D_00435E2C->unkE8;
    } else {
        rate = 0.0f;
    }
    return rate;
}

f32 btlGetActionCategoryGateAsFloat(s32 unused0, s32 unused1, s32 index) {
    s32 category = *(u16 *)(D_00435E20 + index * 56 + 0x1A);
    if (category < 14) {
        if (category >= 12) {
            return 1.0f;
        }
    }
    return 0.0f;
}

f32 btlGetActionRecordPercentAsFraction(s32 unused0, s32 unused1, s32 index) {
    return (f32)*(u16 *)(D_00435E20 + index * 56 + 0x22) / 100.0f;
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
        entry = index * 56 + D_00435E20;
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
        entry = index * 56 + D_00435E20;
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
    for (unit = ((BattleController *)func_001AA6F8())->actors; unit != 0; unit = unit->next) {
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

void func_001B3818(u32 arg0) {
    btlAverageMaximumValueForMask(arg0, 1);
}

void func_001B3830(u32 arg0) {
    btlAverageMaximumValueForMask(arg0, 0);
}

s32 btlAverageCurrentValueForMask(s32 mask, s8 skipDown) {
    UiObject *unit;
    s32 total = 0;
    s32 count = 0;
    u32 flags;
    for (unit = ((BattleController *)func_001AA6F8())->actors; unit != 0; unit = unit->next) {
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

void func_001B3900(u32 arg0) {
    btlAverageCurrentValueForMask(arg0, 1);
}

void func_001B3918(u32 arg0) {
    btlAverageCurrentValueForMask(arg0, 0);
}

s32 btlAverageMaskedActorStat(s32 mask, s8 skipDown) {
    UiObject *unit;
    s32 total = 0;
    s32 count = 0;
    u32 flags;
    for (unit = ((BattleController *)func_001AA6F8())->actors; unit != 0; unit = unit->next) {
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

void func_001B39E8(u32 arg0) {
    btlAverageMaskedActorStat(arg0, 1);
}

void func_001B3A00(u32 arg0) {
    btlAverageMaskedActorStat(arg0, 0);
}

s32 btlSumOrAverageActorAttribute(u32 mask, s32 attribute, s8 skipDown) {
    s32 sum = 0;
    s32 count = 0;
    UiObject *unit = ((BattleController *)func_001AA6F8())->actors;
    for (; unit != 0; unit = unit->next) {
        u32 flags = unit->flags;
        if ((flags & 1) != 0) {
            if (skipDown == 0 || (flags & 0x20) == 0) {
                if ((unit->entryMask & mask) != 0) {
                    s32 value = func_00119A78((s32)&unit->entryMask, attribute);
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

void func_001B3AF8(u32 arg0, u32 arg1) {
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
    for (unit = ((BattleController *)func_001AA6F8())->actors; unit != 0; unit = unit->next) {
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
    s32 (*hook)(u8 *) = *(s32 (**)(u8 *))(func_001AA6F8() + 0x6A4);
    if (hook != 0 && hook(unit) == 0) {
        return 0;
    }
    if (*(u32 *)(unit + 0x114) & 0x2000) {
        return 0;
    }
    if ((*(u16 *)(unit + 0x12E) & 0x7FFF) == 0x4000) {
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
    return (*(s32 *)(D_00435DEC + object->index * 76) & 0x100) > 0;
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415840);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B4828);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B4918);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B4AA0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B4EB8);

u32 func_001B5268(void) {
    func_001AA6F8();
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
    predicate = *(s32 (**)(s32, s32, s32))(func_001AA6F8() + 0x6cc);
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
    if (index != 0 && *(u8 *)(D_00435E20 + index * 56 + 8) != 0) {
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

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B5600);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B5688);

s32 btlHasHighPriorityState(void) {
    u8 *table;
    s32 index;
    if (*(s8 *)btlCommandPanelWork == 2) {
        table = (u8 *)D_00438F48;
        index = *(s8 *)table;
        if (*(s16 *)(table + index * 2) >= 0x80) {
            if (*(s32 *)(table + index * 8 + 4) >= 0xB) {
                return 1;
            }
        }
    }
    return 0;
}

s32 btlCountFlaggedSceneActors(void) {
    BattleController *controller = (BattleController *)func_001AA6F8();
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

INCLUDE_ASM(const s32, "game/code_001A5BB8", btlInitializeCommandPanelSlotTables);

s16 btlGetActorIdForClass(s8 classId) {
    ActorClassIds table = D_00415B08;
    return table.values[classId];
}

void btlGetActorClassPair(s8 classId, u32 *first, u32 *second) {
    ActorClassPairTable pairs = D_00415B18;
    *first = pairs.values[classId * 2];
    *second = pairs.values[classId * 2 + 1];
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415AF0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415B08);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415B18);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B5A20);

INCLUDE_ASM(const s32, "game/code_001A5BB8", btlPopulateCommandPanelGrid);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B5E98);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B6180);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B6438);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B6A20);

void btlReleaseAndClearChipBlock(void) {
    sdfReleaseChipBlock(btlCommandPanelWork);
    btlCommandPanelWork = 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B6CA8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B6FC0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B70B8);

void btlInitializeActionRecordWithScale(s32 arg0, s16 arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5) {
    *(u8 *)(arg0 + 0) = 1;
    *(u8 *)(arg0 + 0x28) = arg4 * 8 + 0x18;
    *(u16 *)(arg0 + 2) = arg1;
    *(f32 *)(arg0 + 4) = arg5;
    *(u32 *)(arg0 + 0x18) = arg2;
    *(u32 *)(arg0 + 0x1c) = arg3;
    *(u32 *)(arg0 + 0x24) = 0;
}

typedef struct BtlResBlock {
    s32 unk0;
    s32 nameA;
    s32 nameB;
    s32 nameC;
    s32 resA;
    s32 resB;
    s32 resC;
    s32 unk1C;
} BtlResBlock;

extern u8 D_00436800;

extern u8 D_00436801;

extern BtlResBlock *D_00436804;

extern s32 func_003292A8(s32);

extern BtlResBlock *sdfResourceRetainAddress(s32);

extern s32 func_00343ED0(const char *, void *, s32);

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

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415CE8);

void btlPanelResourcesLoad(void) {
    u8 params[16];
    s32 handle;
    BtlResBlock *block;
    if (D_00436800 == 0) {
        handle = func_003292A8(0x28);
        block = sdfResourceRetainAddress(handle);
        D_00436804 = block;
        block->unk0 = handle;
        block->resA = 0;
        block->resB = 0;
        block->unk1C = 0;
        D_00436804->nameA = func_00343ED0("/battle/panel/batle_01.spr", params, 0);
        D_00436804->nameB = func_00343ED0("/battle/panel/batle_02.spr", params, 0);
        D_00436804->nameC = func_00343ED0("/battle/panel/battle_03.spr", params, 0);
        D_00436801 = 0;
    }
    D_00436800 = 1;
}

typedef struct BtlWorkRes {
    u8 pad[0x4D8];
    s32 resA;
    s32 resB;
    s32 resC;
} BtlWorkRes;

extern s32 func_00305148();

void btlLoadResourceBlock(void) {
    BtlWorkRes *work = (BtlWorkRes *)func_001AA6F8();
    if (D_00436801 == 0) {
        D_00436804->resA = func_00305148(D_00436804->nameA, 0);
        D_00436804->resB = func_00305148(D_00436804->nameB, 0);
        D_00436804->resC = func_00305148(D_00436804->nameC, 0);
        work->resA = D_00436804->resA;
        work->resB = D_00436804->resB;
        D_00436801 = 1;
    }
}

extern s32 effDestroyResourceSlotSet(s32);

void btlReleaseResourceBlock(void) {
    BtlWorkRes *work = (BtlWorkRes *)func_001AA6F8();
    if (D_00436801 != 0) {
        effDestroyResourceSlotSet(D_00436804->resA);
        D_00436804->resA = 0;
        effDestroyResourceSlotSet(D_00436804->resB);
        D_00436804->resB = 0;
        effDestroyResourceSlotSet(D_00436804->resC);
        D_00436804->resC = 0;
        work->resA = 0;
        work->resB = 0;
        work->resC = 0;
        D_00436801 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B73E8);

void btlClearTaskActorSlots(void) {
    u8 *work = (u8 *)func_001AA6F8();
    u8 *actor;
    u8 *owner;
    s32 *slot;
    s32 *reverse;
    s32 i;
    for (actor = *(u8 **)(work + 0x248); actor != 0; actor = *(u8 **)(actor + 0x178)) {
        owner = *(u8 **)(actor + 0x18);
        if (owner != 0) {
            if (*(u16 *)(work + 0x270) == 1) {
                if (*(u32 *)(owner + 0x110) & 0x200) {
                    for (i = 0, slot = (s32 *)(actor + 0x150); i < 8; i++) {
                        *slot = 0;
                        slot++;
                    }
                }
            } else if (*(u32 *)(owner + 0x110) & 0x400) {
                for (i = 7, reverse = (s32 *)(actor + 0x16C); i >= 0; i--) {
                    *reverse = 0;
                    reverse--;
                }
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B75F8);

void func_001B76F0(void) {
    s32 temp_v0;
    s32 buf[4];

    temp_v0 = func_001AA6F8();
    fldCountSceneFadeKinds(temp_v0, buf);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B7718);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B7830);

void btlClearSharedBattleStateWords(void) {
    u32 *puVar1;
    s32 temp_v0;

    temp_v0 = 3;
    puVar1 = (u32 *)(D_00435DD0 + 0x16f00);
    do {
        temp_v0 = temp_v0 - 1;
        *puVar1 = 0;
        puVar1 = puVar1 + 1;
    } while (-1 < temp_v0);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B7940);

void func_001B7A00(void) {
    s32 i;
    for (i = 3; i >= 0; i--) {
        D_003B6928[i] = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B7A38);

extern u32 D_004367B8;

s64 btlSetTaskPhase2(void) {
    s64 task = func_00101740(D_004367B8);
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
        temp_v0 = func_00101740(D_004367E4);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

void func_001B7E08(void) {
    u8 *puVar1;
    s64 temp_v0;

    temp_v0 = func_00101740(D_004367E4);
    if (temp_v0 != 0) {
        puVar1 = (u8 *)kwlnTaskGetUserValue(temp_v0);
        *puVar1 = 2;
    }
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B7E40);

u32 func_001B7FB8(void) {
    s64 temp_v0;

    temp_v0 = btlGetTrackedTaskHandle(0xb);
    if (temp_v0 != 0) {
        temp_v0 = kwlnTaskIsRegistered(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = func_00101740(D_004367E8);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

s32 btlGetRegisteredTaskValueOrDefault(void) {
    if (func_001B7FB8() == 0) {
        return 0x80;
    }
    return *(s8 *)kwlnTaskGetUserValue(func_00101740(D_004367E8));
}

u32 func_001B8038(void) {
    u8 *puVar1;
    s64 temp_v0;

    temp_v0 = func_00101740(D_004367E8);
    if (temp_v0 != 0) {
        puVar1 = (u8 *)kwlnTaskGetUserValue(temp_v0);
        *puVar1 = 2;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B8078);

void func_001B81B0(void) {
    u8 *puVar1;
    s64 temp_v0;

    temp_v0 = func_00101740(D_004367E0);
    if (temp_v0 != 0) {
        puVar1 = (u8 *)kwlnTaskGetUserValue(temp_v0);
        *puVar1 = 2;
    }
}

u32 func_001B81E8(void) {
    s64 temp_v0;

    temp_v0 = btlGetTrackedTaskHandle(9);
    if (temp_v0 != 0) {
        temp_v0 = kwlnTaskIsRegistered(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = func_00101740(D_004367E0);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

extern u32 D_004367BC;

typedef struct BtlPhaseTask {
    s32 phase;
} BtlPhaseTask;

void btlSetTaskPhase5(void) {
    s64 task = func_00101740(D_004367BC);
    if (task != 0) {
        ((BtlPhaseTask *)kwlnTaskGetUserValue(task))->phase = 5;
        *btlCommandPanelWork = 3;
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

void func_001B82E8(void) {
    func_001AA6F8();
    kwlnTaskGetUserValue(btlGetTrackedTaskHandle(7));
    *(u8 *)(btlTrackedTaskHandles + 0x48) = 0;
}

u32 func_001B8320(void) {
    s64 temp_v0;

    temp_v0 = btlGetTrackedTaskHandle(6);
    if (temp_v0 != 0) {
        temp_v0 = kwlnTaskIsRegistered(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = func_00101740(D_004367D8);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B8368);

u32 func_001B8538(void) {
    s64 temp_v0;

    temp_v0 = btlGetTrackedTaskHandle(1);
    if (temp_v0 != 0) {
        temp_v0 = kwlnTaskIsRegistered(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = func_00101740(D_004367C8);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B8580);

u32 func_001B8740(void) {
    s64 temp_v0;

    temp_v0 = btlGetTrackedTaskHandle(0);
    if (temp_v0 != 0) {
        temp_v0 = kwlnTaskIsRegistered(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = func_00101740(D_004367C4);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", btlReplaceDialogTasksAndQueueMessage);

s32 btlGetTrackedTaskHandle(s32 arg0) {
    s32 temp_v0;

    temp_v0 = btlTrackedTaskHandles + arg0 * 4;
    return *(s32 *)temp_v0;
}

void btlSetTrackedTaskHandle(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = btlTrackedTaskHandles + arg0 * 4;
    *(s32 *)temp_v0 = arg1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B88F8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B8A80);

extern void itfMesCleanupWindow(s32, s32);

extern s32 itfMesDestroyWindowIfPresent();

typedef struct MesWindowSet {
    u32 unk0;
    u32 handle[9];
    u8 pad28[0x20];
    u16 active[9][2];
} MesWindowSet;

void itfMesCloseAllWindows(s32 handle) {
    MesWindowSet *set;
    s32 i;
    func_001AA6F8();
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
    u8 *work = (u8 *)func_001AA6F8();
    u8 *actor;
    sdfReleaseChipBlock(kwlnTaskGetUserValue(handle));
    btlSetTrackedTaskHandle(0xB, 0);
    actor = *(u8 **)(work + 0x184);
    if (*(u16 *)(*(u8 **)(actor + 0x18) + 0x12E) & 0x80) {
        btlInitCursorAndApplyAction((s32)work + 0x70, (s32)work + 0x70, (s32)actor);
    }
    *(u32 *)(work + 0x218) |= 0x100000;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B9A90);

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

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BB078);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004160F0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416100);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416128);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416138);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004162F8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BB1E8);

u32 btlSetSlotLowByteClamped(BtlSlotOwner *owner, s32 group, s32 slot, s32 delta) {
    u32 word = owner->records[group].word[slot];
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

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BB8D0);

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
    u8 *work = (u8 *)func_001AA6F8();
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

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BC138);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416428);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BC618);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416448);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416458);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416468);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BC8A8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416498);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BCFB0);

extern s32 func_001C7D48();

extern void func_001BC138();

extern void func_001BC618();

extern void func_001BC8A8();

extern void func_001BCFB0();

s32 btlUpdatePhaseGatedTaskUntilTimeout(void) {
    s32 *state = (s32 *)kwlnTaskGetUserValue();
    s32 frame;
    if ((u32)((func_001C7D48() - 1) & 0xFF) < 2U) {
        return 0;
    }
    func_001BC138(state);
    if (*(s8 *)(btlTrackedTaskHandles + 0x54) == 0) {
        func_001BC618(state);
    }
    func_001BC8A8(state);
    if (*(s8 *)(btlTrackedTaskHandles + 0x54) == 0) {
        func_001BCFB0(state);
    }
    frame = *state + 1;
    *state = frame;
    return frame < 0x32 ? 0 : -1;
}

void func_001BD6B8(void) {
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

void func_001BEEF8(void) {
    sdfReleaseChipBlock(kwlnTaskGetUserValue());
    btlSetTrackedTaskHandle(5, 0);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BEF28);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BF3C8);

void btlFreeRegisteredTaskData(s32 handle) {
    func_001AA6F8();
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

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001BF690);

void btlReleaseDialogTaskData(s32 handle) {
    u8 *window;
    func_001AA6F8();
    window = (u8 *)kwlnTaskGetUserValue(handle);
    itfMesCleanupWindow(*(s32 *)(window + 4), 0);
    sdfReleaseChipBlock(window);
    btlSetTrackedTaskHandle(0, 0);
}

void btlCreateMessageWindow(void) {
    u8 *window;
    func_001AA6F8();
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
    func_001AA6F8();
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
    flags = (s32 *)(D_00435DD0 + 0x16f00);
    for (i = 0; i < 4; i++) {
        if (flags[i]) {
            return 0;
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C0080);

extern s32 func_001C0080(s32);

s64 btlGetTaskState6(void) {
    s64 task = func_00101740(D_004367BC);
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
    s64 task = func_00101740(D_004367BC);
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

extern void dspCloseChannel(void);

s64 btlDestroyTaskC(void) {
    s64 result = func_00101740(D_004367EC);
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
    u8 *work = (u8 *)func_001AA6F8();
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
    s64 result = func_00101740(D_004367F0);
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
    BattleController *battle = (BattleController *)func_001AA6F8();
    sdfReleaseChipBlock(kwlnTaskGetUserValue(handle));
    btlSetTrackedTaskHandle(0xD, 0);
    battle->flags |= 0x100000;
    *(u32 *)(btlTrackedTaskHandles + 0x3C) &= ~0x100;
    evtFinishMessageWindowAndNotify();
}

extern u8 D_004166F0[];

void btlInitSoundSlotTable(void) {
    u8 initial[0x20];
    u8 *allocated;
    u32 *source;
    u32 *destination;
    s16 *state;
    s32 i;
    memcpy(initial, D_004166F0, sizeof(initial));
    allocated = sdfAllocAndClearQuadwords(0x30);
    D_00438F48 = (u32)allocated;
    state = (s16 *)(allocated + 2);
    destination = (u32 *)(allocated + 0x10);
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

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004166F0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C0EF0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416740);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C1300);

void btlReleaseSelectionTaskBuffer(void) {
    if (btlGetTrackedTaskHandle(2) != 0) {
        sdfReleaseChipBlock(D_00438F48);
    }
    btlSetTrackedTaskHandle(2, 0);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C1520);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C16B0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416780);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416798);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C1A68);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001C1F10);

void func_001C2390(void) {
    func_003297C8(*(u32 *)kwlnTaskGetUserValue());
    btlSetTrackedTaskHandle(3, 0);
}

s32 btlAreLinkedSceneCountersAtThreshold(void) {
    s32 base;
    s32 i;
    if (btlGetTrackedTaskHandle(8) == 0) {
        if (btlGetTrackedTaskHandle(2) != 0) {
            base = D_00438F48;
            for (i = 0; i < 4; i++) {
                if (*(s16 *)(base + 2 + i * 2) < 0x80) {
                    return 0;
                }
                if (*(s32 *)(base + 0xC + i * 8) < 11) {
                    return 0;
                }
            }
            if (*(s32 *)(base + 0x18) == *(s32 *)(base + 0x10) + 23) {
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
    func_003297C8(*(u32 *)(kwlnTaskGetUserValue(func_00101740(D_004367CC)) + 0x1200));
    btlSetTrackedTaskHandle(8, 0);
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004167E0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416800);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416820);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00416830);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436630);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436638);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436640);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436644);

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

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367BC);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367C0);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367C4);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367C8);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367CC);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367D0);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367D4);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367D8);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367DC);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367E0);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367E4);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367E8);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367EC);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367F0);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", btlTrackedTaskHandles);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_004367F8);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", btlCommandPanelWork);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436800);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436801);

INCLUDE_SDATA(const s32, "game/code_001A5BB8", D_00436804);

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

