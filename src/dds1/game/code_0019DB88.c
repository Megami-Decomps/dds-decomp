#include "common.h"
#include "pcp_vu0.h"

extern void btlUpdateFadeIndicator(u8 *);

typedef struct SoundQueue {
    s32 unk00;
    s32 allocation;
    s32 unk08;
    u16 drawFlags;
    u16 unk0E;
    struct SoundQueueNode *head;
    u32 unk14;
} SoundQueue;

extern SoundQueue itfMesWork;

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

extern s32 func_00195ED8();

extern UiSprite *func_00199828();

extern void itfSetPanelLayoutAndNotify();

extern void func_00199A20();

typedef struct SoundSeq {
    s32 unk00;
    s32 unk04;
    s32 seq;
    s32 unk0C;
    s16 unk10;
    s16 current;
    s16 saved;
    s16 count;
    u8 unk18[0xA];
    s16 entryCount;
} SoundSeq;

typedef struct SndPad {
    u8 pad00[0x21];
    s8 confirm;
    s8 edge22;
    s8 edge23;
    u8 pad24[2];
    s8 prev;
    s8 next;
} SndPad;

typedef struct UiSurface { u8 pad0[0x20]; } UiSurface;

typedef struct UiOwnerRef { u8 pad0[0xC]; struct UiPanel *owner; } UiOwnerRef;

extern SndPad D_00324510;

extern UiSurface D_00324B48[];

extern UiOwnerRef *D_00357D88[];

extern void itfAdvancePanelLayoutAndNotify(UiSprite *sprite, s32 a, s32 b, s32 c, s32 d, s32 e);

extern void itfBuildAndSubmitPanelPacket(UiSprite *sprite, UiSurface *surface);

extern void itfMesOffsetNodeChain(UiTexRef *node, s32 dx, s32 dy);

extern s32 func_0019F770(UiSprite *sprite);

extern s32 func_0019EA88();

extern void itfMesSetRowItemFlag(s32 sequence, s32 index, s32 count, s32 selected);

extern void sndSetSequenceVolumePan(s32 sequence, s32 volume, s32 pan);

extern s8 D_00324530[];

extern u32 kwlnTaskGetUserValue(s64);

extern u32 D_00358308[];

extern u32 D_003BB240;

extern s32 D_003BB244;

extern s32 itfLoadTextureFromAsset(u32);

extern u8 D_003D6EB0[0x18];

/* Message-window nodes are linked through the word at offset 0x24. */
typedef struct MessageNodeLink {
    u8 pad00[0x24];
    s32 next;
} MessageNodeLink;

void frFontEnableNodeContextModes(s32 node) {
    for (; node != 0; node = ((MessageNodeLink *)node)->next) {
        frFontEnableContextMode(node);
    }
}

void itfMesInitializePanelPlacementSprite(UiPanel *panel) {
    UiPos *pos = &panel->pos;
    UiPanelPlacement *place = &panel->place;
    s32 spriteTop;
    if (place->sprite == 0) {
        place->sprite = func_00199828(6, itfMesWork.allocation);
        if (place->frame != 0) {
            place->sprite->unk20 = panel->origin.tex->unk4 + func_00195ED8(0, panel->origin.tex);
        }
    }
    spriteTop = pos->y + place->offsetY;
    itfSetPanelLayoutAndNotify(place->sprite, pos->x + place->offsetX, spriteTop, pos->x + place->rightX, pos->y + place->bottomY, panel->unkC);
    place->sprite->unk24 = spriteTop;
    func_00199A20(place->sprite, place->unk1C, place->unk20, place->unk24, 0);
    panel->flags = (panel->flags & ~0x300) | 0x100;
}

void itfMesCreatePanelOriginFrameWhenVisible(UiPanel *panel) {
    UiPanelOrigin *origin = &panel->origin;
    UiPanelPlacement *place = &panel->place;
    if (origin->tex != 0 && !(panel->flags & 0x10000)) {
        if (place->frame == 0) {
            s32 width = origin->tex->unkC * 16;
            place->frame = func_00199828(7, itfMesWork.allocation);
            itfSetPanelLayoutAndNotify(place->frame, origin->x - 0x2D0, origin->y - 0x68, origin->x + width + 0x2D0, origin->y + 0xF0, panel->unkC);
            func_00199A20(place->frame, 0x7F, 0x7F, 0x7F, 0);
        }
        panel->flags = (panel->flags & ~0x3000) | 0x1000;
    } else if (place->frame != 0) {
        panel->flags |= 0x3000;
    }
}

/* The first message-window glyph block starts at ItfMesState +0x14. */
typedef struct UiGlyphBlock {
    u32 x;
    u32 y;
    void *glyphChain;
    u16 unk0C;
    u16 pad0E;
} UiGlyphBlock;

void itfResetCursorPositionAndState(u32 *cursorWords, s32 resetPosition) {
    UiGlyphBlock *cursor = (UiGlyphBlock *)cursorWords;
    if (resetPosition != 0) {
        cursor->x = 0x280;
        cursor->y = 0xa10;
    }
    cursor->glyphChain = 0;
    cursor->unk0C = 0xffff;
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

/* Distinct from UiCursor: this reset block has four trailing halfwords. */
typedef struct UiCursorResetBlock {
    u32 x;
    u32 y;
    s32 unk08;
    s32 unk0C;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s32 unk18;
    s32 unk1C;
    s16 unk20;
    s16 unk22;
} UiCursorResetBlock;

void itfInitializeCursorResetState(UiCursorResetBlock *cursor) {
    cursor->x = 0x560;
    cursor->y = 0xC48;
    cursor->unk08 = 0;
    cursor->unk0C = 0;
    cursor->unk10 = 0;
    cursor->unk12 = -1;
    cursor->unk14 = -1;
    cursor->unk16 = 0;
    cursor->unk18 = 0;
    cursor->unk1C = 0;
    cursor->unk20 = 0;
    cursor->unk22 = 0;
}

/* Paired resource slots: each occupied marker owns the handle at its index. */
typedef struct UiResourceSlots {
    s32 markers[32];
    s32 handles[32];
} UiResourceSlots;

/* Message-window resource block at ItfMesState +0xA4. */
typedef struct UiWindowResourceBlock {
    u32 unk00;
    void *resource;
    u32 handle;
    u8 pad0C[0x1C];
    u32 unk28;
} UiWindowResourceBlock;

void itfResetWindowResourceBlock(u32 *object) {
    UiWindowResourceBlock *block = (UiWindowResourceBlock *)object;
    block->unk00 = 0;
    block->resource = 0;
    block->handle = 0;
    func_0019E048((s32)object, 0, 0);
}

void itfClearDrawStateWords(s32 words) {
    s32 remaining;
    u32 *cursor;

    cursor = (u32 *)&((UiResourceSlots *)words)->markers[31];
    remaining = 0x1f;
    do {
        remaining = remaining - 1;
        *cursor = 0;
        cursor = cursor + -1;
    } while (-1 < remaining);
}

typedef struct BtlFade {
    u8 kind;
    u8 pad1;
    s16 phase;
    s16 alpha;
    s16 timer;
    u32 unk08;
} BtlFade;

/* Both the sequence selector and fade belong to the same sound UI object. */
typedef struct SoundUiState {
    u8 pad00[0x40];
    SoundSeq selection;
    u8 pad64[0x16C];
    BtlFade fade;
} SoundUiState;

void itfResetBattleFadeState(s32 fadeAddress, s32 preserveKind) {
    if (preserveKind == 0) {
        ((BtlFade *)fadeAddress)->kind = 0;
    }
    ((BtlFade *)fadeAddress)->phase = 0;
    ((BtlFade *)fadeAddress)->timer = 0;
    ((BtlFade *)fadeAddress)->alpha = 0x40;
    ((BtlFade *)fadeAddress)->unk08 = 0;
}

void btlSetFadePhaseAlphaTimer(s32 fadeAddress, s16 phase, s16 alpha, s16 timer) {
    ((BtlFade *)fadeAddress)->phase = phase;
    ((BtlFade *)fadeAddress)->alpha = alpha;
    ((BtlFade *)fadeAddress)->timer = timer;
}

/* Effects carry three releaseable primitive handles at 0xA4. */
typedef struct BattleEffect {
    u32 flags;
    u8 pad04[0xA0];
    u32 resourceHandles[3];
} BattleEffect;

void btlReleaseEffectResourceHandles(u8 *effect) {
    u32 *handles = ((BattleEffect *)effect)->resourceHandles;
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
    ((BattleEffect *)effect)->flags &= ~0xF00;
}

void itfReleaseUiResourceSlotHandles(s32 *resourceFlags) {
    s32 remaining;

    remaining = 0x1f;
    do {
        if (*resourceFlags != 0) {
            func_002D0918(((UiResourceSlots *)resourceFlags)->handles[0]);
            *resourceFlags = 0;
        }
        remaining = remaining - 1;
        resourceFlags = resourceFlags + 1;
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

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019E048);

void itfUpdateBattleDisplayAndFadeIndicator(u32 object) {
    func_0019E130();
    func_0019E320(object);
    func_0019E4F8(object);
    btlUpdateFadeIndicator(object);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019E130);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019E320);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019E4F8);

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
    SoundSeq *sel = &((SoundUiState *)obj)->selection;
    s32 dir = 0;
    s32 index;
    if (D_00324510.prev & 2) {
        if (sel->current != 0) {
            dir = -1;
        } else if (D_00324510.prev < 0) {
            dir = -1;
        }
    } else if (D_00324510.next & 2) {
        if (sel->current != sel->count - 1 || D_00324510.next < 0) {
            dir = 1;
        }
    }
    if (dir != 0) {
        sndStepSequenceIndex(sel, dir);
        itfResetBattleFadeState((s32)&((SoundUiState *)obj)->fade, 1);
    }
    if (D_00324510.confirm < 0) {
        sndSetSequenceVolumePan(8, 0x7F, 0x3F);
        return 1;
    }
    if (sel->entryCount > 0 && (index = func_0019EA88(sel)) >= 0) {
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

void sndStepSequenceIndex(SoundSeq *obj, s32 dir) {
    s32 cur = obj->current;
    u16 maxv;
    itfMesSetRowItemFlag(obj->seq, cur, obj->count, 0);
    if (dir < 0) {
        cur = cur - 1;
        if (cur < 0) {
            cur = obj->count - 1;
        }
        maxv = obj->count;
    } else {
        cur = cur + 1;
        if (cur >= obj->count) {
            cur = 0;
        }
        maxv = obj->count;
    }
    itfMesSetRowItemFlag(obj->seq, cur, (s16)maxv, 1);
    obj->current = cur;
    obj->saved = cur;
    sndSetSequenceVolumePan(1, 0x7F, 0x3F);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019EA88);


void btlUpdateFadeIndicator(u8 *obj) {
    BtlFade *fade = &((SoundUiState *)obj)->fade;
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

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019EBC8);

void itfMesRenderActivePanelSprites(UiPanel *panel) {
    UiPanelPlacement *place;
    if (panel->flags & 0x80000) {
        return;
    }
    place = &panel->place;
    if ((panel->flags & 0x300) >= 0x100) {
        if (panel->state != 3) {
            itfBuildAndSubmitPanelPacket(place->sprite, &D_00324B48[panel->index]);
        }
        itfMesWork.drawFlags |= 2;
        if (place->overlay != 0) {
            itfBuildAndSubmitPanelPacket(place->overlay, &D_00324B48[panel->index]);
        }
    }
    if (D_00357D88[0] != 0 && D_00357D88[0]->owner == panel && place != 0) {
        func_0019F770(place->sprite);
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019EE58);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019F0F8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019F4C8);

typedef struct SoundQueueNode {
    s32 unk00;
    struct SoundQueueNode *next; /* 0x04 */
    s32 window;                  /* 0x08: released when flushing the message queue */
    u32 object;                  /* 0x0C: updated by the sound-queue visitor */
} SoundQueueNode;

s32 sndVisitQueuedResources(void) {
    SoundQueueNode *node = *(SoundQueueNode **)D_003D6EB0;
    while (node != 0) {
        itfUpdateBattleDisplayAndFadeIndicator(node->object);
        node = node->next;
    }
    return 0;
}

extern void itfMesDestroyWindow(s32 window);

extern void sdfTexReleaseReferenceViaHandler(s32 texture);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019F6A0);

void sndFlushMessageQueue(void) {
    SoundQueueNode *node = itfMesWork.head;
    s32 window;
    while (node != 0) {
        window = node->window;
        node = node->next;
        itfMesDestroyWindow(window);
    }
    sdfTexReleaseReferenceViaHandler(itfMesWork.allocation);
    itfMesWork.allocation = 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019F770);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019F850);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019F9E0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019FA70);

extern u32 kwlnTaskCreate(u32, u32, u32, u32, s32 (*)(s64), void (*)(s64), u32);

extern u8 D_00358038[];

extern char D_003A14F0[]; /* "TestMsgMngC" */

extern char D_003A1500[]; /* "TestMsgMngD" */

extern s32 D_003BA8EC;

extern s32 scrCreateTaskForProcessId(s32, u8 *, s32);

extern void itfMesSetWindowCallbackAddress(s32, void (*)(void));

extern void sndCycleTestMessageResource(void);

extern s32 func_0019FBD8(void);

extern void func_0019FCA8(void);

extern s32 sndUpdateTestMsgTask(void);

/* The process task stores its message-window handle at offset 0xCC. */
typedef struct TestMessageTaskData {
    u8 pad00[0xCC];
    s32 window;
} TestMessageTaskData;

void sndCreateTestMsgTasks(void) {
    D_003BA8EC = 0x80FFFFFF;
    sndCycleTestMessageResource();
    itfMesSetWindowCallbackAddress(((TestMessageTaskData *)kwlnTaskGetUserValue(scrCreateTaskForProcessId(0x3E8, D_00358038, 0)))->window, func_0019FCA8);
    kwlnTaskCreate((u32)D_003A14F0, 0x3EF, 0, 0, (s32 (*)(s64))func_0019FBD8, 0, 0);
    kwlnTaskCreate((u32)D_003A1500, 0x2AFE, 0, 0, (s32 (*)(s64))sndUpdateTestMsgTask, 0, 0);
}

void sndCycleTestMessageResource(void) {
    if (D_003BB244 != 0) {
        sdfTexReleaseReferenceViaHandler(D_003BB244);
        D_003BB244 = 0;
    }
    D_003BB240 = (D_003BB240 + 1) & 3;
    if (D_003BB240 != 3) {
        D_003BB244 = itfLoadTextureFromAsset(D_00358308[D_003BB240]);
    }
}

s32 func_0019FBD8(void) {
    if (D_00324530[0] < 0) {
        sndCycleTestMessageResource();
    }
    return 0;
}

typedef struct SndDev {
    u8 unk0[0x10];
    void (*unk10)(void *, s32);
} SndDev;

extern SndDev D_003255A8;

extern u8 D_00358318[];

extern u8 D_00358328[];

extern u8 D_00358338[];

extern s32 sdfAllocPacketAligned(s32 size);

extern void sdfInitPacketList(s32 mem);

extern void itfSendTablePacket(s32 packet, s32, s32);

extern void func_00198C70(void *, void *, void *, s32, s32, s32, s32);

s32 sndUpdateTestMsgTask(void) {
    s32 mem;
    if ((D_003BB244 != 0) && (D_003BB240 != 3)) {
        mem = sdfAllocPacketAligned(0x20);
        sdfInitPacketList(mem);
        itfSendTablePacket(mem, 0, 0);
        func_00198C70(D_00358318, D_00358328, D_00358338, 0xFFF, D_003BB244, 0, mem);
        D_003255A8.unk10(&D_003255A8, mem);
        return 0;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A14F0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1500);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1510);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1528);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1540);

void func_0019FCA8(void) {
    func_003003F0("********* AAAA ********\n");
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019FCC8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019FE00);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019FF60);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB230);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB238);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB240);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB244);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB248);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB250);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB258);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB260);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB268);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB270);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB278);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB280);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB288);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB290);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB298);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB2A0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB2A8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB2B0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB2B4);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB2B8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB2C0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB2C8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB2D0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB2D8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB2E0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", btlRuntime);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB2E8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB2F0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB2F8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB300);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB308);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB310);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB318);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB320);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB328);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB330);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB338);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB340);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB348);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB350);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB358);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB360);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB368);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB370);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB378);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB380);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB388);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB390);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB398);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3A0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3A4);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3A8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3AC);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3B0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3B4);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3B8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3BC);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3C0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3C4);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3C8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3CC);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3D0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3D4);

INCLUDE_SDATA(const s32, "game/code_0019DB88", btlTrackedTaskHandles);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3DC);

INCLUDE_SDATA(const s32, "game/code_0019DB88", btlCommandPanelWork);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3E4);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3E5);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3E8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3F0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3F8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB400);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB408);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB410);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB418);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB420);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB428);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB430);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB438);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB440);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB448);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB450);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB458);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB460);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB468);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB470);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB478);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB480);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB488);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB494);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB496);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB498);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB4A0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB4A8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB4B0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB4B8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB4C0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB4C8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB4D0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB4D8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB4E0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB4E8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB4F0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB4F8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB500);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB508);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB510);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB518);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB520);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB528);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB530);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB538);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB540);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB548);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB550);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB558);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB560);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB568);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB570);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB578);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB580);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB588);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB590);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB598);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB5A0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB5A8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB5B0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB5B8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB5C0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB5C8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB5D0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB5D8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB5E0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB5E8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", btlDeferredTaskTail);

INCLUDE_SDATA(const s32, "game/code_0019DB88", btlDeferredTaskHead);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB5F8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB600);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB608);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB610);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB618);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB620);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB628);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB630);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB638);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB640);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB648);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB650);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB658);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB660);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB664);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB668);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB670);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB678);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB680);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB690);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB694);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB698);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB6A0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB6A8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB6B0);

