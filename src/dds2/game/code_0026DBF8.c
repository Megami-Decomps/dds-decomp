#include "common.h"
#include "kwln.h"
#include "dat_state.h"

#define MNU_MANTRA_DRAW_ITEM_BYTES 0x24
#define MNU_MANTRA_DRAW_POOL_HEADER_BYTES 0xC
#define MNU_MANTRA_DRAW_ACTIVE_FLAG 1
#define MNU_MANTRA_DRAW_STATE_CLEAR_MASK 0xFFFFF807
#define MNU_MANTRA_DRAW_STATE_SHIFT 3
#define MNU_MANTRA_DRAW_STATE_START_DELAY 1
#define MNU_MANTRA_DRAW_STATE_END_DELAY 2
#define MNU_MANTRA_DRAW_STATE_CREATE 3
#define MNU_MANTRA_DRAW_STATE_RUNNING 4
#define MNU_MANTRA_DRAW_STATE_RELEASE 5
#define MNU_MANTRA_DRAW_STATE_DEACTIVATE 6

#define MNU_MANTRA_COST_TEXT_FLAGS 0xA09DC300
#define MNU_MANTRA_COST_TEXT_BUFFER_BYTES 8
#define MNU_MANTRA_COST_MULTI_DIGIT_X_OFFSET 0x1E0
#define MNU_MANTRA_COST_SINGLE_DIGIT_X_OFFSET 0x1E4
#define MNU_MANTRA_COST_TEXT_Y_OFFSET 0x173
#define MNU_MANTRA_COST_MARKER_CODE 0x76

/* Mantra-menu draw processes and their per-kind animation records.
 * Creation arguments and resulting state pointers occupy separate fields;
 * startup/exit delays bracket frame callbacks and final release. */

extern char D_00425118[];
extern char mnuMantraNumberFormat[]; /* "%d" */
extern char D_004378B0[]; /* "---" */
extern s32 mnuGetMantraSourceValue(u16);
extern f32 sdfSinPoly(f32);
extern u32 frFontDrawTextVariantAAndMeasure(s32, s32, s32, u32, u8, char *, s32, s32);
extern u32 frFontDrawTextVariantBAndMeasure(s32, s32, s32, u32, u8, char *, s32, s32);
extern u32 frFontDrawStyledGlyphChainAndMeasure(s32, s32, s32, u32, u8, const u8 *, s32, s32);
extern u32 frFontQueueTintedGlyphChainAndMeasure(s32, s32, s32, u32, u8, u16, s32, s32, s32, s32);
/* The SDK definition and its declarations use legacy K&R parameters. */
extern void sdfSubmitGsTestOneRegisterPacket();
extern void uiDrawUniformColorRect(u32, u32, u32, u32, u32, u32, u32);

extern u32 mnuAllocateMantraPanelBurstPool(void);

extern u32 mnuAllocateMantraSparkleEmitter(s16);

extern s32 mnuFindMantraDrawItemByKind(u32, u32);

extern void *sdfAllocSizeClassBlock(s32);

void sdfReleaseResourceAllocation(u32 sprite);

void mnuFreeMantraSparkleEmitter(u32 sprite);

void mnuReleaseMantraPanelBurstPool(u32 *obj);
u32 mnuAllocateMantraBackgroundBurstPool(void);
extern void mnuDrawCellScaledGrid();
u32 mnuQueueNextMantraSelection(u32);
u32 mnuQueuePreviousMantraSelection(u32);
extern u32 mnuRegisterMantraDrawItem(u32, u32, s32 (*)(), void (*)(), u32 (*)(), void (*)(), s16, s16, u32);
extern s32 mnuUpdateMantraFadeA();
extern u32 mnuDrawMantraIconList(u32, u32);
typedef struct MantraNodePos {
    u8 pad00[2];
    s16 panelIndex; /* Used to look up the corresponding panel-position record. */
    s16 x;
    s16 y;
    struct MantraNodePos *neighbors[6];
} MantraNodePos;

typedef struct MenuPanelObject MenuPanelObject;
/* Menu selection work starts at +0x240 in the menu object. */
typedef struct MantraMenuWork {
    u8 pad00[0x560];
    MantraNodePos *selectedNode;
    u8 pad564[0x2610];
    MenuPanelObject *panelObject;
} MantraMenuWork;

typedef struct MantraMenu {
    u8 pad00[0x240];
    MantraMenuWork work;
} MantraMenu;
extern MantraNodePos *mnuGetMantraNodePositionRecord(s16);
extern MantraNodePos *mnuGetMantraPanelPositionRecord(s16);
extern u32 scrGetSelectedScriptEntryId(DatPartyRecord *);
extern u32 mnuGetSelectedNodeValue(u8 *);
extern u32 mnuAllocateMantraIconPool(u32);
extern u32 mnuCreateMantraIconListA();
extern u32 mnuCreateMantraIconListB();
extern u32 mnuCreateMantraIconListC();
extern void mnuReleaseMantraFadeData(s32);
s32 mnuUpdateMantraLimitLineFade();
void func_00271510();
u32 func_002712E0(u32 list);
void mnuMantraSetupSlot(u32);
void mnuReleaseMantraFadeDrawData();
typedef struct MantraIconEntry {
    /* 0x0 */ u32 active : 1;
    u32 state : 4;
    u32 leaving : 1;
    u32 shortLoop : 1;
    u32 unk7 : 1;
    u32 variant : 1;
    u32 unk_bits : 23;
    /* 0x4 */ s16 x;
    s16 y;
    /* 0x8 */ s32 timer;
} MantraIconEntry;

typedef struct MantraIconPool {
    u32 unk0;
    MantraIconEntry *entries;
    MantraIconEntry *chainTail;
    MantraIconEntry *current;
    s32 count;
} MantraIconPool;
MantraIconEntry *mnuSpawnMantraIcon(s32, s32, MantraIconPool *, u32);
extern s32 mnuUpdateMantraBackgroundFade();
extern void func_00270848();
extern u32 mnuInitMantraBackgroundDraw();
extern void mnuReleaseMantraBackgroundDraw();
extern s32 mnuUpdateMantraBackgroundMaskFade();
extern s32 mnuDrawMantraBackgroundMaskPulse();
extern u32 mnuInitMantraBackgroundMaskDraw();
extern void mnuReleaseMantraBackgroundMaskDraw();
extern s32 mnuUpdateMantraTitleBlinkFade();
extern void func_00272F08();
extern u32 mnuInitMantraTitleDraw();
extern void mnuReleaseMantraTitleDraw();
extern s32 mnuUpdateMantraInfoPulseFade();
extern void func_00273828();
extern u32 mnuInitMantraInfoDraw();
extern void mnuReleaseMantraInfoDraw();
extern s32 mnuUpdateMantraGaugeFade();
extern s32 mnuDrawMantraGauge();
extern u32 mnuInitMantraGaugeData();
extern void mnuReleaseMantraGaugeData();
extern s32 mnuUpdateMantraRecordPanelFade();
extern void func_00274FF8();
extern u32 mnuCreateTypeOneRecord(void);
extern void mnuReleaseMantraRecordPanelData();
extern u32 mnuMantraSpriteSlots[12];


/* The panel pool allocates 48-byte records shared by animation and sprite controls. */
typedef struct MantraPanelAnimation {
    union {
        u32 flags;
        struct {
            u8 kind;
            u8 control[3];
        } tag;
    };
    s16 startDelay;
    s16 endDelay;
    s32 animationTicks;
    u16 x;
    u16 y;
    u32 visualParameters[3];
    u8 byte1C;
    u8 byte1D;
    s16 frame;
    u8 stateA;
    u8 stateB;
    u8 stateC;
    u8 pad23;
    u32 spriteHandle;
    u32 burstPool;
    s16 id;
    s16 transitionDelay;
} MantraPanelAnimation;

/* Full allocation used by both the record-panel controls and its fade update. */
typedef struct MantraRecordPanelState {
    u16 state;
    u16 flags;
    u16 elapsed;
    u16 cycle;
    f32 scale;
    s32 delay;
    u32 unk10;
    u8 pad14[4];
} MantraRecordPanelState;

typedef struct MantraDisplayNode {
    u8 pad00[4];
    u32 fromValue;
    u32 toValue;
    u16 transitionKind;
    u8 pad0E[2];
    struct MantraDisplayNode *next;
} MantraDisplayNode;

/* One draw process: construction supplies data, update requests completion,
 * draw runs each active frame, and release may also be called without arguments. */
typedef struct MantraDrawItem {
    u32 kind;
    union {
        u32 flags;
        struct {
            u32 active : 1;
            u32 unkBits : 2;
            u32 state : 8;
            u32 released : 1;
            u32 category : 8;
            u32 unkHigh : 12;
        } bits;
    };
    u32 (*create)();
    void (*release)();
    s32 (*update)();
    void (*draw)();
    s16 startDelay;    /* 0x18: delay before construction */
    s16 endDelay;      /* 0x1A: delay before release */
    u32 createArg;     /* 0x1C: constructor argument */
    void *data;       /* 0x20: constructor result */
} MantraDrawItem;

typedef struct MantraDrawPool {
    u32 handle;
    MantraDrawItem *items;
    s32 count;
} MantraDrawPool;


typedef struct MantraCountState {
    u8 pad00[0x4C];
    s32 shown;
    s32 step;
} MantraCountState;

typedef struct MantraCountStateB {
    u8 pad00[0x10];
    s32 shown;
    s32 step;
} MantraCountStateB;

typedef struct MantraFadeData {
    MantraIconPool *iconPool;
    u16 state;
    u16 elapsed;
    f32 scale;
    s16 x;
    s16 y;
} MantraFadeData;

typedef struct MantraPulseFade {
    u16 state;       /* 0x00 */
    u16 elapsed;     /* 0x02 */
    u16 cycle;       /* 0x04 */
    u8 pad06[2];
    f32 scale;       /* 0x08 */
} MantraPulseFade;


typedef struct MantraEffectResource {
    u8 pad00[0x6C];
    u32 handle;
} MantraEffectResource;

typedef struct MantraFadeState {
    /* 0x00 */ u16 state;
    /* 0x02 */ u16 timer;
    /* 0x04 */ s16 x;
    /* 0x06 */ s16 y;
    /* 0x08 */ f32 value;
    /* 0x0C */ u32 activeFlags;
    /* 0x10 */ u32 pendingFlags;
    union {
        u32 countdownWord;
        struct {
            u32 armed : 1;
            u32 countdown : 31;
        } timedFlags;
    };
    /* 0x18 */ s32 clock;
    /* 0x1C */ u16 queuedState;
    /* 0x1E */ u16 delay;
    /* 0x20 */ u32 queuedFlags;
} MantraFadeState;

typedef struct MantraBurstPool MantraBurstPool;

/* Background transition/fade data plus the separate burst-pool reference.
 * The transition word packs variant nibbles followed by a signed delay byte. */
typedef struct MantraBackgroundState {
    u16 state;            /* 0x00 */
    u16 pad02;
    union {
        u32 transitionWord;
        struct {
            u32 currentVariant : 4;
            u32 nextVariant : 4;
            u32 unkBits : 24;
        } variants;
        struct {
            u8 unk4;
            s8 transitionDelay;
            u8 unk6[2];
        } timing;
    };
    u16 timer;            /* 0x08 */
    u16 clock;            /* 0x0A */
    f32 value;            /* 0x0C */
    u32 enabled;          /* 0x10 */
    u32 selectedValue;    /* 0x14 */
    MantraBurstPool *burstPool; /* 0x18 */
} MantraBackgroundState;

typedef struct MantraSourceEntry {
    u32 unk0;
    u32 flags;
    u32 unk8;
    s32 value;
    u32 unk10;
} MantraSourceEntry;

/* One of the two 0x14-byte slots of a mantra source entry. */
typedef struct {
    s32 value;      /* 0x00 */
    u8 pad04[8];
    u32 slotFlags;      /* 0x0C: bit 5 marks this mantra source slot active */
    u8 pad10[4];
} MnuSourceSlot;

/* Slot view of a mantra source entry (0x34 bytes). */
typedef struct {
    u8 pad00[4];
    u32 flags;             /* 0x04 */
    u8 pad08[4];
    MnuSourceSlot slot[2]; /* 0x0C */
} MnuSourceEntrySlots;

typedef struct MantraListState {
    u32 unk0;
    MantraDisplayNode *head;
    u32 entries[8];
    s16 count;
    s16 index;
    u32 unk2C;
} MantraListState;

typedef struct MantraFileEntry {
    struct MantraFileEntry *next;
    u8 pad04[4];
    void *handle;
    u8 pad0C[4];
    u8 kind;
} MantraFileEntry;

typedef struct MantraFileRequest {
    u8 pad00[0x60];
    MantraFileEntry *entries;
} MantraFileRequest;
extern s32 fileRequestIsReady(void *);
extern void func_002C7CE8(void *);
extern u32 func_00305148(void *, u32);
extern u32 kwlnTaskGetUserValue(KwlnTask *task);
extern void *fileQueuePlainDispatchRequest(const char *path);
extern KwlnTask *kwlnTaskCreate(const char *, u32, s32, s32, TaskUpdate,
                                TaskDestroy, u32);
s32 mnuLoadMantraSpriteTask(KwlnTask *task);
extern char D_004250B0[];
extern void mnuDrawMantraSprite(s32, s32, s32, s32, s32, s32, s32);
void func_00284508(u32, u32, u32, u32, u32, u32);
extern char mnuMantraSpriteTaskName[];
extern s32 mnuUpdateMantraUnitPanelFade();
extern void func_00274A70();
extern u32 mnuInitMantraUnitPanelDraw();
extern void mnuReleaseMantraUnitPanelDraw();
void mnuStorePanelEntry(u32, u32);
void effDestroyResourceSlotSet(u32);
extern u32 sdfAllocGeneralBlock(u32);
extern u32 sdfMemoryGetBlockAddress(u32);

s32 mnuGetActiveMantraModelFlagState(void) {
    s32 result = 0;

    if (mdlFlagTest(0x920)) {
        result = 1;
    }
    if (mdlFlagTest(0x921)) {
        result = 2;
    }
    if (mdlFlagTest(0x922)) {
        result = 3;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026DC48);

extern char mnuMantraValueFormat[]; /* "%7d" */

/* Interpolate the panel's numeric value over 20 updates, playing a sound on each changed step. */
void mnuDrawAnimatedMantraValue(u32 x, u32 y, u32 depth, u32 fade, MantraCountState *state, u32 drawArg) {
    char text[16];
    u32 flags = fade | 0xA09DC300;

    mnuDrawMantraSprite(x, y, depth, fade, 0x2E, 0, drawArg);
    if (datGameState->header.currency != state->shown) {
        s32 steps = 20;

        sndSetSequenceVolumePan(0x13, 0x7F, 0x3F);
        state->step++;
        func_0035C860(text, mnuMantraValueFormat, state->shown + (datGameState->header.currency - state->shown) * state->step / steps);
        if (state->step == steps) {
            state->shown = datGameState->header.currency;
            state->step = 0;
        }
    } else {
        func_0035C860(text, mnuMantraValueFormat, datGameState->header.currency);
    }
    frFontDrawTextVariantAAndMeasure(x + 0x193, y + 0x26, depth, flags, 0, text, 0, drawArg);
}

/* Draw a label for a nonzero entry, otherwise use the empty-entry panel art. */
void mnuDrawOptionalMantraLabel(u32 x, u32 y, u32 depth, u32 fade, u16 entryId, u32 drawArg) {
    u8 buffer[0x20];
    u32 flags = (fade & 0xFF) | 0xA09DC300;

    mnuDrawMantraSprite(x, y, depth, fade, 0x4C, 0, drawArg);
    if (entryId != 0) {
        memset(buffer, 0, 0x20);
        func_00314500(entryId, 1, buffer);
        frFontDrawStyledGlyphChainAndMeasure(x + 0x27, y + 0x146, depth, flags, 4, buffer, 0x101, drawArg);
        mnuDrawMantraSprite(x, y, depth, fade, 0x50, 0, drawArg);
    } else {
        mnuDrawMantraSprite(x, y, depth, fade, 0x54, 0, drawArg);
    }
}

extern u8 *frFontGetColoredGlyphResource(void);

/* Render a panel icon when it has an ID, or the corresponding empty art. */
void mnuDrawMantraLabelA(u32 x, u32 y, u32 depth, s32 fade, u32 iconId, u32 drawArg) {
    u8 *handle = frFontGetColoredGlyphResource();
    u32 flags = (u32)(fade * 0.6f) | 0xA09D7D00;

    mnuDrawMantraSprite(x, y, depth, fade, 0x4D, 0, drawArg);
    if (iconId != 0) {
        frFontQueueTintedGlyphChainAndMeasure(x + 0x2A, y + 0x160, depth, flags, 0, iconId & 0xFFFF, handle, 0, 0, drawArg);
    } else {
        mnuDrawMantraSprite(x, y, depth, fade, 0x55, 0, drawArg);
    }
}

extern u32 func_003151D0(u16);

/* Print the entry's count and draw one repeated marker per count unit. */
void mnuDrawMantraDigitRow(u32 x, u32 y, u32 depth, u32 fade, u32 entryId, u32 drawArg) {
    char text[0x20];
    u32 flags = fade | 0xA09D7D00;
    u32 count;
    u32 i;

    mnuDrawMantraSprite(x, y, depth, fade, 0x4E, 0, drawArg);
    if (entryId != 0) {
        count = func_003151D0(entryId);
        if (count != 0) {
            func_0035C860(text, mnuMantraNumberFormat, count);
            frFontDrawTextVariantBAndMeasure(x + 0x82, y + 0x17C, depth, flags, 4, text, 0, drawArg);
            for (i = 0; i < count; i++) {
                mnuDrawMantraSprite(x, y, depth, fade, 0x51, 0, drawArg);
                x += 0xF;
            }
        }
    } else {
        mnuDrawMantraSprite(x, y, depth, fade, 0x56, 0, drawArg);
    }
}

void mnuDrawMantraCostCounter(u32 x, u32 y, u32 depth, s32 fade, u32 entryId, u32 showCost, u32 drawArg) {
    char text[0x20];
    u32 flags = fade | 0xA09DC300;

    mnuDrawMantraSprite(x, y, depth, fade, 0x4F, 0, drawArg);
    if (entryId != 0) {
        mnuDrawMantraSprite(x, y, depth, fade, 0x53, 0, drawArg);
        if (showCost != 0) {
            func_0035C860(text, mnuMantraNumberFormat, mnuGetMantraSourceValue(entryId));
            frFontDrawTextVariantBAndMeasure(x + 0x9A, y + 0x190, depth, flags, 4, text, 0, drawArg);
        } else {
            func_0035C860(text, D_004378B0);
            frFontDrawStyledGlyphChainAndMeasure(x + 0xA2, y + 0x18C, depth, (u32)(fade * 0.5f) | 0xA09D7D00, 0, text, 0x80000000, drawArg);
        }
    } else {
        mnuDrawMantraSprite(x, y, depth, fade, 0x57, 0, drawArg);
    }
}

extern MantraSourceEntry *scrGetEntryDescriptor(u16);

/* Pick the value of the first active slot, preferring slot 0. */
s32 mnuGetMantraSourceValue(u16 index) {
    s32 result = 0;
    MnuSourceEntrySlots *entry = (MnuSourceEntrySlots *)scrGetEntryDescriptor(index);
    s32 slot = 0;

    if (entry->flags & 0x20) {
        slot = 0;
    } else if (entry->slot[0].slotFlags & 0x20) {
        slot = 1;
    } else {
        return result;
    }
    return entry->slot[slot].value;
}

void mnuMergeMantraSpriteSlots(u32 *values) {
    s32 i;
    for (i = 0; i < 12; i++) {
        if (values[i] != 0) {
            mnuMantraSpriteSlots[i] = values[i];
        }
    }
}

void mnuReleaseFirstMantraSpriteSlots(void) {
    s32 i;
    u32 *slot = mnuMantraSpriteSlots;
    for (i = 1; i >= 0; i--, slot++) {
        if (*slot != 0) {
            effDestroyResourceSlotSet(*slot);
        }
        *slot = 0;
    }
}

INCLUDE_SDATA(const s32, "game/code_0026DBF8", mnuMantraValueFormat);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", mnuMantraNumberFormat);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378B0);

void mnuReleaseMiddleMantraSpriteSlots(void) {
    s32 slots[2] = {2, 3};
    u32 i;

    for (i = 0; i < 2; i++) {
        if (mnuMantraSpriteSlots[slots[i]] != 0) {
            effDestroyResourceSlotSet(mnuMantraSpriteSlots[slots[i]]);
        }
        mnuMantraSpriteSlots[slots[i]] = 0;
    }
}

void mnuStartMantraSpriteLoad(void) {
    if (mnuMantraSpriteSlots[4] == 0) {
        u32 request = (u32)fileQueuePlainDispatchRequest(D_004250B0);
        kwlnTaskCreate(mnuMantraSpriteTaskName, 0x402, 1, 1,
                       mnuLoadMantraSpriteTask, NULL, request);
    }
}

s32 mnuHasMantraSpriteTaskFinished(void) {
    if (mnuMantraSpriteSlots[4] != 0) {
        return 1;
    }
    return kwlnTaskGetTaskByName(mnuMantraSpriteTaskName) == 0;
}

void mnuReleaseMantraSpriteSlots(void) {
    s32 i;
    u32 *slot = mnuMantraSpriteSlots;
    for (i = 11; i >= 0; i--, slot++) {
        if (*slot != 0) {
            effDestroyResourceSlotSet(*slot);
        }
        *slot = 0;
    }
}

s32 mnuLoadMantraSpriteTask(KwlnTask *task) {
    MantraFileRequest *request = (MantraFileRequest *)kwlnTaskGetUserValue(task);
    s32 result = fileRequestIsReady(request);
    if (result != 0) {
        MantraFileEntry *entry = request->entries;
        s32 i;
        for (i = 4; entry != 0; entry = entry->next, i++) {
            if (entry->kind == 1) {
                mnuMantraSpriteSlots[i] = func_00305148(entry->handle, 0);
                sdfReleaseResourceAllocation((u32)entry->handle);
            }
        }
        func_002C7CE8(request);
        result = -1;
    }
    return result;
}

typedef struct MnuSpriteWork {
    u8 pad00[0xC];
    s32 width;
    s32 height;
    u8 pad14[0x10];
    f32 rotation;
    u8 pad28[0x54];
    s32 nativeWidth;
    s32 nativeHeight;
    u8 pad84[0x1C];
} MnuSpriteWork;

typedef struct MnuSpriteTexture {
    u8 pad00[0x18];
    u32 flags;
    u8 pad1C[0x64];
} MnuSpriteTexture;

typedef struct MnuSpriteResource {
    u8 pad00[0x10];
    MnuSpriteTexture *textures;
    u8 pad14[4];
    MnuSpriteWork *sprites;
} MnuSpriteResource;

extern s32 D_003CE9D0[][4];
extern void func_00306CD0(s32, s32, s32, u32, s32, u32, s32, s32);

void mnuDrawMantraSprite(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex,
                   s32 flags, s32 context) {
    if (flags & 0x10000) {
        ((MnuSpriteResource *)mnuMantraSpriteSlots[D_003CE9D0[placementIndex][0]])
            ->textures[D_003CE9D0[placementIndex][1]].flags |= 1;
    }
    if (flags & 0x20000) {
        ((MnuSpriteResource *)mnuMantraSpriteSlots[D_003CE9D0[placementIndex][0]])
            ->textures[D_003CE9D0[placementIndex][1]].flags |= 2;
    }
    func_00306CD0((x + D_003CE9D0[placementIndex][2]) << 4,
                  (y + D_003CE9D0[placementIndex][3]) << 3,
                  z, (u32)((f32)(alpha << 8) * 0.0078125f), flags,
                  mnuMantraSpriteSlots[D_003CE9D0[placementIndex][0]],
                  D_003CE9D0[placementIndex][1], context);
    if (flags & 0x10000) {
        ((MnuSpriteResource *)mnuMantraSpriteSlots[D_003CE9D0[placementIndex][0]])
            ->textures[D_003CE9D0[placementIndex][1]].flags &= ~1;
    }
    if (flags & 0x20000) {
        ((MnuSpriteResource *)mnuMantraSpriteSlots[D_003CE9D0[placementIndex][0]])
            ->textures[D_003CE9D0[placementIndex][1]].flags &= ~2;
    }
}


void mnuDrawMantraScaledRotatedCenteredSprite(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex,
                   s32 flags, s32 context, f32 scale, f32 rotation) {
    s32 width;
    s32 height;

    ((MnuSpriteResource *)mnuMantraSpriteSlots[D_003CE9D0[placementIndex][0]])
        ->sprites[D_003CE9D0[placementIndex][1]].rotation = rotation;
    width = ((MnuSpriteResource *)mnuMantraSpriteSlots[D_003CE9D0[placementIndex][0]])
        ->sprites[D_003CE9D0[placementIndex][1]].width =
        (s32)(scale * (f32)((MnuSpriteResource *)mnuMantraSpriteSlots[D_003CE9D0[placementIndex][0]])
            ->sprites[D_003CE9D0[placementIndex][1]].nativeWidth) << 4;
    height = ((MnuSpriteResource *)mnuMantraSpriteSlots[D_003CE9D0[placementIndex][0]])
        ->sprites[D_003CE9D0[placementIndex][1]].height =
        (s32)(scale * (f32)((MnuSpriteResource *)mnuMantraSpriteSlots[D_003CE9D0[placementIndex][0]])
            ->sprites[D_003CE9D0[placementIndex][1]].nativeHeight) << 3;
    func_00306CD0(((x + D_003CE9D0[placementIndex][2]) << 4) - (width >> 1),
                  ((y + D_003CE9D0[placementIndex][3]) << 3) - (height >> 1),
                  z, (u32)((f32)(alpha << 8) * 0.0078125f), flags,
                  mnuMantraSpriteSlots[D_003CE9D0[placementIndex][0]],
                  D_003CE9D0[placementIndex][1], context);
    /* Reload the slot after drawing before restoring its native dimensions. */
    ((MnuSpriteResource *)mnuMantraSpriteSlots[D_003CE9D0[placementIndex][0]])
        ->sprites[D_003CE9D0[placementIndex][1]].rotation = 0.0f;
    ((MnuSpriteResource *)mnuMantraSpriteSlots[D_003CE9D0[placementIndex][0]])
        ->sprites[D_003CE9D0[placementIndex][1]].width =
        ((MnuSpriteResource *)mnuMantraSpriteSlots[D_003CE9D0[placementIndex][0]])
            ->sprites[D_003CE9D0[placementIndex][1]].nativeWidth << 4;
    ((MnuSpriteResource *)mnuMantraSpriteSlots[D_003CE9D0[placementIndex][0]])
        ->sprites[D_003CE9D0[placementIndex][1]].height =
        ((MnuSpriteResource *)mnuMantraSpriteSlots[D_003CE9D0[placementIndex][0]])
            ->sprites[D_003CE9D0[placementIndex][1]].nativeHeight << 3;
}

void mnuDrawMantraScaledCenteredSprite(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex,
                   s32 flags, s32 context, f32 scale) {
    s32 width;
    s32 height;

    width = ((MnuSpriteResource *)mnuMantraSpriteSlots[D_003CE9D0[placementIndex][0]])
        ->sprites[D_003CE9D0[placementIndex][1]].width =
        (s32)(scale * (f32)((MnuSpriteResource *)mnuMantraSpriteSlots[D_003CE9D0[placementIndex][0]])
            ->sprites[D_003CE9D0[placementIndex][1]].nativeWidth) << 4;
    height = ((MnuSpriteResource *)mnuMantraSpriteSlots[D_003CE9D0[placementIndex][0]])
        ->sprites[D_003CE9D0[placementIndex][1]].height =
        (s32)(scale * (f32)((MnuSpriteResource *)mnuMantraSpriteSlots[D_003CE9D0[placementIndex][0]])
            ->sprites[D_003CE9D0[placementIndex][1]].nativeHeight) << 3;
    func_00306CD0(((x + D_003CE9D0[placementIndex][2]) << 4) - (width >> 1),
                  ((y + D_003CE9D0[placementIndex][3]) << 3) - (height >> 1),
                  z, (u32)((f32)(alpha << 8) * 0.0078125f), flags,
                  mnuMantraSpriteSlots[D_003CE9D0[placementIndex][0]],
                  D_003CE9D0[placementIndex][1], context);
    /* Reload the slot after drawing before restoring its native dimensions. */
    ((MnuSpriteResource *)mnuMantraSpriteSlots[D_003CE9D0[placementIndex][0]])
        ->sprites[D_003CE9D0[placementIndex][1]].width =
        ((MnuSpriteResource *)mnuMantraSpriteSlots[D_003CE9D0[placementIndex][0]])
            ->sprites[D_003CE9D0[placementIndex][1]].nativeWidth << 4;
    ((MnuSpriteResource *)mnuMantraSpriteSlots[D_003CE9D0[placementIndex][0]])
        ->sprites[D_003CE9D0[placementIndex][1]].height =
        ((MnuSpriteResource *)mnuMantraSpriteSlots[D_003CE9D0[placementIndex][0]])
            ->sprites[D_003CE9D0[placementIndex][1]].nativeHeight << 3;
}

void mnuDrawMantraRotatedSprite(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex,
                   s32 flags, s32 context, f32 rotation) {
    ((MnuSpriteResource *)mnuMantraSpriteSlots[D_003CE9D0[placementIndex][0]])
        ->sprites[D_003CE9D0[placementIndex][1]].rotation = rotation;
    func_00306CD0((x + D_003CE9D0[placementIndex][2]) << 4,
                  (y + D_003CE9D0[placementIndex][3]) << 3,
                  z, (u32)((f32)(alpha << 8) * 0.0078125f), flags,
                  mnuMantraSpriteSlots[D_003CE9D0[placementIndex][0]],
                  D_003CE9D0[placementIndex][1], context);
    ((MnuSpriteResource *)mnuMantraSpriteSlots[D_003CE9D0[placementIndex][0]])
        ->sprites[D_003CE9D0[placementIndex][1]].rotation = 0.0f;
}

s32 mnuDrawMantraSineFade(u32 x, u32 y, u32 depth, s32 frame, s32 amount, u32 drawArg) {
    f32 wave = (f32)frame / 60.0f;
    s32 shown;

    wave = sdfSinPoly(wave * 3.14159265f);
    mnuDrawMantraSprite(x, y, depth, amount, 0x25, 0, drawArg);
    shown = (f32)amount * (wave * 0.5f + 0.5f);
    mnuDrawMantraSprite(0, 0, depth, shown, 0x26, 0, drawArg);
    mnuDrawMantraSprite(0, 0, depth, shown, 0x28, 0, drawArg);
    if (frame < 60) {
        return 0;
    }
    return 1;
}

/* Shared by the badge and its two marker renderers; only observed fields are named. */
typedef struct MantraCostRecord {
    u8 pad00[4];
    u16 iconIndex;
    u8 pad06[0xE];
    u16 cost;
} MantraCostRecord;

/* Draw a button marker and a variable-width cost, shifting one-digit values right. */
INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004250B0);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", mnuMantraSpriteTaskName);

void mnuDrawMantraCostBadge(s32 x, s32 y, s32 depth, u8 *record, s32 amount, s32 drawArg) {
    char buttons[9] = {0, 'n', 's', 'o', 'q', 'p', 'r', 't', 'u'};
    char text[MNU_MANTRA_COST_TEXT_BUFFER_BYTES];
    s32 drawFlags = amount | MNU_MANTRA_COST_TEXT_FLAGS;

    mnuDrawMantraSprite(x, y, depth, amount, buttons[((MantraCostRecord *)record)->iconIndex], 0, drawArg);
    mnuDrawMantraSprite(x, y, depth, amount, MNU_MANTRA_COST_MARKER_CODE, 0, drawArg);
    memset(text, 0, sizeof(text));
    func_0035C860(text, mnuMantraNumberFormat, ((MantraCostRecord *)record)->cost);
    if (strlen(text) > 1) {
        frFontDrawTextVariantBAndMeasure(x + MNU_MANTRA_COST_MULTI_DIGIT_X_OFFSET, y + MNU_MANTRA_COST_TEXT_Y_OFFSET, depth, drawFlags, 0, text, 0, drawArg);
    } else {
        frFontDrawTextVariantBAndMeasure(x + MNU_MANTRA_COST_SINGLE_DIGIT_X_OFFSET, y + MNU_MANTRA_COST_TEXT_Y_OFFSET, depth, drawFlags, 0, text, 0, drawArg);
    }
}

/* Draw the record's cost marker at the renderer origin; x/y inputs are unused in DDS2. */
void mnuDrawMantraCostIcon(u32 unusedX, u32 unusedY, u32 depth, u32 recordAddress,
                   u32 amount, u32 drawArg) {
    char markers[9] = { '\0', '/', '4', '0', '2', '1', '3', '5', '6' };
    u16 index = ((MantraCostRecord *)recordAddress)->iconIndex;
    mnuDrawMantraSprite(0, 0, depth, amount, markers[index], 0, drawArg);
}

/* Draw the alternate cost marker, eight codes above the base marker. */
void mnuDrawMantraCostIconOffset(u32 unusedX, u32 unusedY, u32 depth, u32 recordAddress,
                   u32 amount, u32 drawArg) {
    char markers[9] = { '\0', '/', '4', '0', '2', '1', '3', '5', '6' };
    u16 index = ((MantraCostRecord *)recordAddress)->iconIndex;
    mnuDrawMantraSprite(0, 0, depth, amount, markers[index] + 8, 0, drawArg);
}

extern s32 func_0026F1F0(s32 x, s32 y, s32 depth, MantraDisplayNode *node, s32 index, s32 drawArg);
INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026F1F0);

MantraDisplayNode *mnuAllocateDisplayListNode(void) {
    MantraDisplayNode *node = (MantraDisplayNode *)sdfAllocSizeClassBlock(sizeof(MantraDisplayNode));

    memset(node, 0, sizeof(MantraDisplayNode));
    return node;
}

u32 mnuReleaseDisplayListNodeAndGetNext(MantraDisplayNode *node) {
    u32 next;

    next = (u32)node->next;
    sdfReleaseChipBlock(node);
    return next;
}

void mnuInitMantraListEntries(MantraListState *list, u32 *entries, s32 count, s32 index) {
    s32 n = 8;
    s32 i;

    memset(list, 0, sizeof(MantraListState));
    if (count < 8) {
        n = count;
    }
    for (i = 0; i < n; i++) {
        list->entries[i] = entries[i];
    }
    list->count = n;
    list->index = index;
    evtPrintDeveloperConsoleMessage("[MaxNum %d][CurrentIndex %d]\n", n, index);
}

/* Queue a display-list transition from the previous selection to the chosen one.
 * A repeat selection needs no node and returns zero. */
u32 mnuQueueMantraSelectionTransition(u32 state, s8 selection) {
    u32 item;
    u32 *entries;
    if (((MantraListState *)state)->index == selection) {
        return 0;
    }
    item = mnuAppendDisplayListNode(state);
    if (item != 0) {
        u32 *selected;
        u32 *previous;
        entries = ((MantraListState *)state)->entries;
        ((MantraDisplayNode *)item)->transitionKind = 2;
        selected = entries + selection;
        previous = entries + ((MantraListState *)state)->index;
        ((MantraListState *)state)->index = selection;
        ((MantraDisplayNode *)item)->fromValue = *previous;
        ((MantraDisplayNode *)item)->toValue = *selected;
    }
    return item;
}

/* Queue the next selection, wrapping to the first list entry at the end. */
u32 mnuQueueNextMantraSelection(u32 state) {
    u32 item = mnuAppendDisplayListNode(state);
    u32 *entries = ((MantraListState *)state)->entries;
    if (item != 0) {
        s16 index = ((MantraListState *)state)->index;
        s16 count = ((MantraListState *)state)->count;
        s32 next = index + 1;
        u32 *current;
        u32 *upcoming;
        ((MantraDisplayNode *)item)->transitionKind = 2;
        if (index >= count - 1) {
            next = 0;
        }
        current = entries + ((MantraListState *)state)->index;
        upcoming = entries + next;
        ((MantraListState *)state)->index = next;
        ((MantraDisplayNode *)item)->fromValue = *current;
        ((MantraDisplayNode *)item)->toValue = *upcoming;
    }
    return item;
}

/* Queue the previous selection, wrapping to the last list entry at the start. */
u32 mnuQueuePreviousMantraSelection(u32 state) {
    u32 item = mnuAppendDisplayListNode(state);
    u32 *entries = ((MantraListState *)state)->entries;
    if (item != 0) {
        s32 prev;
        u32 *current;
        u32 *upcoming;
        if (((MantraListState *)state)->index <= 0) {
            prev = ((MantraListState *)state)->count - 1;
        } else {
            prev = ((MantraListState *)state)->index - 1;
        }
        ((MantraDisplayNode *)item)->transitionKind = 1;
        current = entries + ((MantraListState *)state)->index;
        upcoming = entries + prev;
        ((MantraListState *)state)->index = prev;
        ((MantraDisplayNode *)item)->fromValue = *current;
        ((MantraDisplayNode *)item)->toValue = *upcoming;
    }
    return item;
}

u32 mnuAppendDisplayListNode(u32 state) {
    MantraDisplayNode *node = ((MantraListState *)state)->head;
    if (node == 0) {
        node = mnuAllocateDisplayListNode();
        ((MantraListState *)state)->head = node;
    } else {
        while (node->next != 0) {
            node = node->next;
        }
        node->next = mnuAllocateDisplayListNode();
        node = node->next;
    }
    return (u32)node;
}

void mnuReleaseDisplayListNodes(u32 state) {
    MantraDisplayNode *node = ((MantraListState *)state)->head;
    while (node != 0) {
        node = (MantraDisplayNode *)mnuReleaseDisplayListNodeAndGetNext(node);
    }
}

s32 func_0026F8A0(s32 x, s32 y, s32 depth, MantraListState *state, s32 amount, s32 drawArg) {
    s32 index = 0;
    u32 record;
    s32 offset;
    f32 fadeWeight = 0.0f;
    MantraDisplayNode *node;

    node = state->head;
    if (mnuDrawMantraSineFade(x, y, depth, state->unk0, amount, drawArg) != 0) {
        state->unk0 = 0;
    } else {
        state->unk0++;
    }

    if (node == NULL) {
        if (state->unk2C != 0) {
            state->unk2C--;
            fadeWeight = (f32)state->unk2C / 30.0f;
            fadeWeight = sdfSinPoly(fadeWeight * 6.2831852f + (-1.5707963f));
            fadeWeight = (fadeWeight + 1.0f) * 0.5f;
        }
        mnuDrawMantraCostIcon(x, y, depth, state->entries[state->index], amount, drawArg);
        record = state->entries[state->index];
        offset = (s32)((f32)amount * fadeWeight);
        mnuDrawMantraCostIconOffset(x, y, depth, record, offset, drawArg);
        record = state->entries[state->index];
        mnuDrawMantraCostBadge(x, y, depth, (u8 *)record, amount, drawArg);
        return 1;
    }
    do {
        if (func_0026F1F0(x, y, depth, node, index, drawArg) != 0) {
            node = (MantraDisplayNode *)mnuReleaseDisplayListNodeAndGetNext(node);
            state->head = node;
        } else {
            node = node->next;
        }
        index++;
    } while (node != NULL);
    return 0;
}

void func_0026FAA8(s32 state) {
    ((MantraListState *)state)->unk2C = 0x1e;
}

u32 func_0026FAB8(void) {
    return 0;
}

u32 func_0026FAC0(void) {
    return 0;
}

void func_0026FAC8(void) {
}

/* Allocate one header followed by fixed-size draw items; return the pool address word. */
u32 mnuCreateMantraDrawPool(u32 itemCount) {
    u32 poolBytes = itemCount * MNU_MANTRA_DRAW_ITEM_BYTES + MNU_MANTRA_DRAW_POOL_HEADER_BYTES;
    u32 allocationHandle = sdfAllocGeneralBlock(poolBytes);
    MantraDrawPool *pool = (MantraDrawPool *)sdfMemoryGetBlockAddress(allocationHandle);
    memset(pool, 0, poolBytes);
    pool->handle = allocationHandle;
    pool->count = itemCount;
    pool->items = (MantraDrawItem *)((u8 *)pool + MNU_MANTRA_DRAW_POOL_HEADER_BYTES);
    evtPrintDeveloperConsoleMessage("mtrDrawProcessCreate!! num[%d]\n", itemCount);
    return (u32)pool;
}

void mnuDestroyMantraDrawPool(u32 address) {
    MantraDrawPool *pool = (MantraDrawPool *)address;
    s32 count = pool->count;
    MantraDrawItem *item = pool->items;
    s32 i;
    for (i = 0; i < count; i++, item++) {
        if (item->flags & 1) {
            mnuMantraSetupSlot((u32)item);
        }
    }
    sdfReleaseResourceAllocation(pool->handle);
}

void mnuMantraSetupSlot(u32 item) {
    MantraDrawItem *entry = (MantraDrawItem *)item;
    if (!((entry->flags >> 11) & 1)) {
        if (entry->release != 0) {
            entry->release();
        }
        entry->flags |= 0x800;
    }
    entry->flags &= ~1;
}

s32 mnuFindFreeMantraDrawItem(s32 address) {
    MantraDrawPool *pool = (MantraDrawPool *)address;
    s32 count = pool->count;
    s32 i;
    MantraDrawItem *item = pool->items;
    for (i = 0; i < count; i++, item++) {
        if ((item->flags & 1) == 0) {
            return (s32)item;
        }
    }
    return 0;
}

/* Claim an inactive slot; null update/draw callbacks use the zero-return stub. */
u32 mnuRegisterMantraDrawItem(u32 pool, u32 kind, s32 (*update)(), void (*draw)(), u32 (*create)(), void (*release)(), s16 startDelay, s16 endDelay, u32 createArg) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindFreeMantraDrawItem(pool);

    memset(item, 0, MNU_MANTRA_DRAW_ITEM_BYTES);
    item->flags |= MNU_MANTRA_DRAW_ACTIVE_FLAG;
    item->kind = kind;
    item->createArg = createArg;
    item->endDelay = endDelay;
    item->startDelay = startDelay;
    item->create = create;
    if (update != 0) {
        item->update = update;
    } else {
        item->update = func_0026FAB8;
    }
    if (draw != 0) {
        item->draw = draw;
    } else {
        item->draw = func_0026FAB8;
    }
    item->release = release;
    if (startDelay > 0) {
        item->flags = (item->flags & MNU_MANTRA_DRAW_STATE_CLEAR_MASK) | (MNU_MANTRA_DRAW_STATE_START_DELAY << MNU_MANTRA_DRAW_STATE_SHIFT);
    } else if (create != 0) {
        item->flags = (item->flags & MNU_MANTRA_DRAW_STATE_CLEAR_MASK) | (MNU_MANTRA_DRAW_STATE_CREATE << MNU_MANTRA_DRAW_STATE_SHIFT);
    } else {
        item->flags = (item->flags & MNU_MANTRA_DRAW_STATE_CLEAR_MASK) | (MNU_MANTRA_DRAW_STATE_RUNNING << MNU_MANTRA_DRAW_STATE_SHIFT);
    }
    return (u32)item;
}

s32 mnuFindMantraDrawItemByKind(u32 address, u32 kind) {
    MantraDrawPool *pool = (MantraDrawPool *)address;
    s32 count = pool->count;
    s32 i;
    MantraDrawItem *item = pool->items;
    for (i = 0; i < count; i++, item++) {
        if ((item->flags & 0x7f9) == 0x21 && item->kind == kind) {
            return (s32)item;
        }
    }
    return 0;
}


/* Advance every active process. Completion stays latched for later items in
 * this call; do not reset it at the start of each iteration. */
void mnuUpdateMantraDrawPool(u8 *pool) {
    s32 itemIndex;
    s32 exitRequested = 0;
    s32 itemCount = ((MantraDrawPool *)pool)->count;
    MantraDrawItem *item = ((MantraDrawPool *)pool)->items;

    for (itemIndex = 0; itemIndex < itemCount; itemIndex++, item = (MantraDrawItem *)((u8 *)item + MNU_MANTRA_DRAW_ITEM_BYTES)) {
        if (!item->bits.active) {
            continue;
        }
        switch (item->bits.state) {
        case MNU_MANTRA_DRAW_STATE_START_DELAY:
            item->startDelay -= 1;
            if (item->startDelay == 0) {
                if (item->create != 0) {
                    item->bits.state = MNU_MANTRA_DRAW_STATE_CREATE;
                } else {
                    item->bits.state = MNU_MANTRA_DRAW_STATE_RUNNING;
                }
            }
            break;
        case MNU_MANTRA_DRAW_STATE_END_DELAY:
            item->endDelay -= 1;
            if (item->endDelay == 0) {
                if (item->release != 0) {
                    item->bits.state = MNU_MANTRA_DRAW_STATE_RELEASE;
                } else {
                    item->bits.state = MNU_MANTRA_DRAW_STATE_DEACTIVATE;
                }
            }
            break;
        case MNU_MANTRA_DRAW_STATE_CREATE:
            item->bits.state = MNU_MANTRA_DRAW_STATE_RUNNING;
            item->data = (void *)item->create(pool, item->createArg);
            break;
        case MNU_MANTRA_DRAW_STATE_RUNNING:
            if (item->update(pool, item) == 1) {
                exitRequested = 1;
            }
            item->draw(pool, item);
            if (exitRequested != 0) {
                if (item->endDelay > 0) {
                    item->bits.state = MNU_MANTRA_DRAW_STATE_END_DELAY;
                } else {
                    item->bits.state = MNU_MANTRA_DRAW_STATE_RELEASE;
                }
            }
            break;
        case MNU_MANTRA_DRAW_STATE_RELEASE:
            if (!item->bits.released) {
                item->release(item);
            }
            item->bits.released = 1;
            item->bits.active = 0;
            break;
        case MNU_MANTRA_DRAW_STATE_DEACTIVATE:
            item->bits.active = 0;
            break;
        case 7: /* Native no-action state; purpose is not established. */
            break;
        }
    }
}

u32 mnuRegisterMantraBackgroundDraw(u32 pool) {
    return mnuRegisterMantraDrawItem(pool, 0, mnuUpdateMantraBackgroundFade, func_00270848,
                         mnuInitMantraBackgroundDraw, mnuReleaseMantraBackgroundDraw, 0, 0, 0);
}

void mnuBeginMantraBackgroundDrawExit(u32 pool) {
    MantraDrawItem *item;

    item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 0);
    ((MantraBackgroundState *)item->data)->state = 3;
}

/* Queue a variant change and start its five-update transition delay. */
void mnuSetMantraBackgroundVariant(u32 pool, s8 variant) {
    MantraDrawItem *item;
    MantraBackgroundState *data;

    item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 0);
    data = (MantraBackgroundState *)item->data;
    data->transitionWord = (data->transitionWord & 0xffffff0f) | (((s32)variant & 0xfU) << 4);
    data->timing.transitionDelay = 5;
}

void mnuEnableMantraBackground(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 0);
    if (item != 0) {
        ((MantraBackgroundState *)item->data)->enabled = 1;
    }
}

void mnuDisableMantraBackground(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 0);
    if (item != 0) {
        ((MantraBackgroundState *)item->data)->enabled = 0;
    }
}

void mnuSetMantraBackgroundSelection(u32 pool, u32 value) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 0);
    if (item != 0) {
        ((MantraBackgroundState *)item->data)->selectedValue = value;
    }
}


u32 mnuInitMantraBackgroundDraw(void) {
    u32 data = sdfAllocSizeClassBlock(0x1c);
    memset((void *)data, 0, 0x1c);
    ((MantraBackgroundState *)data)->state = 1;
    ((MantraBackgroundState *)data)->transitionWord &= ~0xf;
    ((MantraBackgroundState *)data)->timing.transitionDelay = 0;
    ((MantraBackgroundState *)data)->burstPool = (MantraBurstPool *)mnuAllocateMantraBackgroundBurstPool();
    evtPrintDeveloperConsoleMessage("BG Draw Init\n");
    return data;
}

void mnuReleaseMantraBackgroundDraw(u32 obj) {
    s32 data = (s32)((MantraDrawItem *)obj)->data;
    mnuReleaseMantraBackgroundBurstPool((u32 *)((MantraBackgroundState *)data)->burstPool);
    sdfReleaseChipBlock(data);
    evtPrintDeveloperConsoleMessage("BG Draw Release\n");
}


/* Advance delayed variant changes and the fade; return one after final exit. */
s32 mnuUpdateMantraBackgroundFade(s32 unused, s32 item) {
    MantraBackgroundState *fade = (MantraBackgroundState *)((MantraDrawItem *)item)->data;

    fade->clock += 1;
    if ((s16)fade->clock >= 0x79) {
        fade->clock = 0;
    }
    if (fade->timing.transitionDelay > 0) {
        fade->timing.transitionDelay -= 1;
        if (fade->timing.transitionDelay == 0) {
            if (fade->variants.currentVariant != fade->variants.nextVariant) {
                fade->variants.currentVariant = fade->variants.nextVariant;
                fade->timing.transitionDelay = 5;
            }
        }
    }
    switch (fade->state) {
    case 1:
    case 6:
        fade->value = (f32)fade->timer / 22.0f;
        fade->timer += 1;
        if (fade->timer >= 22) {
            fade->state = 2;
            fade->value = 1.0f;
            fade->timer = 0;
        }
        break;
    case 2:
        fade->value = 1.0f;
        break;
    case 3:
    case 5:
        fade->value = 1.0f - (f32)fade->timer / 10.0f;
        fade->timer += 1;
        if (fade->timer >= 10) {
            fade->timer = 0;
            fade->value = 0.0f;
            if (fade->state == 5) {
                fade->state = 4;
            } else {
                return 1;
            }
        }
        break;
    case 4:
        fade->value = 0.0f;
        break;
    }
    return 0;
}


void mnuDrawMantraPulseFrame(s32 amount, s32 packet, f32 pulse) {
    sdfSubmitGsAlphaOneRegisterPacket(0x44, packet);
    sdfSubmitGsTestOneRegisterPacket(0x30000, packet);
    uiDrawUniformColorRect(0, 0, 0, 0x2000, 0xE00, 0, packet);
    sdfSubmitGsTestOneRegisterPacket(0x5100DL, packet);
    sdfSubmitGsTestOneRegisterPacket(0x3000DL, packet);
    uiDrawUniformColorRect(0, 0, 0xFFFFFF, 0x2000, 0xE00, 0, packet);
    uiDrawActiveSurfaceRegion(packet);
    mnuDrawMantraSprite(0, -15, 0, amount, 0x60, 0x20, packet);
    mnuDrawMantraSprite(0, -15, 0, amount, 0x61, 0x20, packet);
    mnuDrawMantraSprite(0, 0, 0, amount, 0x62, 0x20, packet);
    mnuDrawMantraSprite(0, 0, 0, amount, 0x63, 0x20, packet);
    sdfDispatchSurfaceWithPreparedTexturePacket(packet);
    sdfSubmitGsAlphaOneRegisterPacket(0x54, packet);
    mnuDrawMantraSprite(0, 0, 0, amount, 0x58, 0x60, packet);
    amount = amount * pulse * 0.5f;
    mnuDrawMantraSprite(0, 0, 0, amount, 0x5A, 0, packet);
    mnuDrawMantraRotatedSprite(0, 0x140, 0, amount, 0x5A, 0, packet, 180.0f);
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00270568);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00270848);

u32 mnuRegisterMantraBackgroundMaskDraw(u32 pool) {
    return mnuRegisterMantraDrawItem(pool, 4, mnuUpdateMantraBackgroundMaskFade, mnuDrawMantraBackgroundMaskPulse,
                         mnuInitMantraBackgroundMaskDraw, mnuReleaseMantraBackgroundMaskDraw, 0, 0, 0);
}

void mnuBeginMantraBackgroundMaskExit(u32 pool) {
    MantraDrawItem *item;

    item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 4);
    ((MantraPulseFade *)item->data)->state = 3;
}

void mnuBeginMantraBackgroundMaskFadeIn(u32 pool) {
    MantraDrawItem *item;

    item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 4);
    ((MantraPulseFade *)item->data)->state = 6;
}

void mnuBeginMantraBackgroundMaskFadeOut(u32 pool) {
    MantraDrawItem *item;

    item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 4);
    ((MantraPulseFade *)item->data)->state = 5;
}

void mnuKeepMantraBackgroundMaskVisible(u32 pool) {
    MantraDrawItem *item;

    item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 4);
    ((MantraPulseFade *)item->data)->state = 2;
}

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004251C8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425258);

u32 mnuInitMantraBackgroundMaskDraw(void) {
    u32 data = sdfAllocSizeClassBlock(0xc);
    memset((void *)data, 0, 0xc);
    ((MantraPulseFade *)data)->state = 1;
    evtPrintDeveloperConsoleMessage("BGMask Draw Init\n");
    return data;
}

void mnuReleaseMantraBackgroundMaskDraw(u32 obj) {
    sdfReleaseChipBlock(((MantraDrawItem *)obj)->data);
    evtPrintDeveloperConsoleMessage("BGMask Draw Release\n");
}

s32 mnuUpdateMantraBackgroundMaskFade(s32 unused, s32 item) {
    MantraPulseFade *fade = (MantraPulseFade *)((MantraDrawItem *)item)->data;

    fade->cycle += 1;
    if ((s16)fade->cycle >= 0x79) {
        fade->cycle = 0;
    }
    switch (fade->state) {
    case 1:
    case 6:
        fade->scale = (f32)fade->elapsed / 22.0f;
        fade->elapsed += 1;
        if (fade->elapsed >= 22) {
            fade->state = 2;
            fade->scale = 1.0f;
            fade->elapsed = 0;
        }
        break;
    case 2:
        fade->scale = 1.0f;
        break;
    case 3:
    case 5:
        fade->scale = 1.0f - (f32)fade->elapsed / 10.0f;
        fade->elapsed += 1;
        if (fade->elapsed >= 10) {
            fade->elapsed = 0;
            fade->scale = 0.0f;
            if (fade->state == 5) {
                fade->state = 4;
            } else {
                return 1;
            }
        }
        break;
    case 4:
        fade->scale = 0.0f;
        break;
    }
    return 0;
}

s32 mnuDrawMantraBackgroundMaskPulse(s32 unused, s32 item) {
    MantraPulseFade *data = (MantraPulseFade *)((MantraDrawItem *)item)->data;
    s32 amount = data->scale * 128.0f;
    f32 wave = sdfSinPoly((f32)(s16)data->cycle / 120.0f * (3.14159265f * 2.0f) + (-3.14159265f / 2.0f));

    mnuDrawMantraPulseFrame(amount, 0x53, (wave + 1.0f) * 0.5f);
    return 0;
}

u32 mnuCreateMantraFadeDrawItem(u32 list, u32 tag, u32 category) {
    MantraDrawItem *item = (MantraDrawItem *)mnuRegisterMantraDrawItem(list, 1, mnuUpdateMantraLimitLineFade, func_00271510,
                                                                    func_002712E0, mnuReleaseMantraFadeDrawData, 0, 0, tag);
    item->bits.category = category;
    return (u32)item;
}

void mnuBeginMantraLimitLineFadeOut(u32 pool) {
    s32 obj = mnuFindMantraDrawItemByKind(pool, 1);
    if (obj != 0) {
        s32 data = (s32)((MantraDrawItem *)obj)->data;
        if (((MantraFadeState *)data)->state == 4) {
            ((MantraFadeState *)data)->timer = 5;
        }
        ((MantraFadeState *)data)->state = 3;
    }
}

void mnuShowMantraLimitLine(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 1);
    if (item != 0) {
        ((MantraFadeState *)item->data)->state = 6;
        evtPrintDeveloperConsoleMessage("LimitLine Draw Show\n");
    }
}

void mnuHideMantraLimitLine(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 1);
    if (item != 0) {
        ((MantraFadeState *)item->data)->state = 5;
        evtPrintDeveloperConsoleMessage("LimitLine Draw Hide\n");
    }
}

void mnuSetMantraFadeState(s32 pool, u16 kind, u16 delay) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 1);
    MantraFadeState *fade;

    if (item != 0) {
        fade = item->data;
        fade->queuedState = kind;
        fade->delay = delay;
        if (delay == 0) {
            fade->state = kind;
            if (fade->state == 6 || fade->state == 1) {
                fade->timer = (1.0f - fade->value) * 20.0f;
            } else if (fade->state == 5 || fade->state == 3) {
                fade->timer = (1.0f - fade->value) * 5.0f;
            }
            fade->timer = 0;
        }
    }
}

void mnuSetMantraLimitLinePosition(s16 x, s16 y, u32 pool) {
    s32 obj = mnuFindMantraDrawItemByKind(pool, 1);
    if (obj != 0) {
        s32 data = (s32)((MantraDrawItem *)obj)->data;
        ((MantraFadeState *)data)->x = x;
        ((MantraFadeState *)data)->y = y;
    }
}

/* Merge pending flags after ten ticks: packed word 0x15 is armed | (10 << 1). */
void mnuArmMantraLimitLineFlags(u32 pool, u32 value) {
    s32 obj = mnuFindMantraDrawItemByKind(pool, 1);
    if (obj != 0) {
        s32 data = (s32)((MantraDrawItem *)obj)->data;
        ((MantraFadeState *)data)->pendingFlags = value;
        ((MantraFadeState *)data)->countdownWord = 0x15;
    }
}

void mnuQueueMantraLimitLineFlags(s32 pool, u32 flags) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 1);
    MantraFadeState *fade;

    if (item != 0) {
        fade = item->data;
        if (fade->delay == 0) {
            fade->timedFlags.armed = 0;
            fade->timedFlags.countdown = 0;
            fade->activeFlags = flags;
            fade->pendingFlags = 0;
            fade->queuedFlags = flags;
        } else {
            fade->queuedFlags = flags;
        }
    }
}

u32 func_002712E0(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 1);
    u8 *work = sdfAllocSizeClassBlock(0x24);

    memset(work, 0, 0x24);
    *(u16 *)(work + 2) = 0;
    *(u16 *)work = (item->flags >> 12) & 0xFF;
    *(u32 *)(work + 0x18) = 0;
    return (u32)work;
}

void mnuReleaseMantraFadeDrawData(u32 obj) {
    sdfReleaseChipBlock(((MantraDrawItem *)obj)->data);
}


s32 mnuUpdateMantraLimitLineFade(s32 unused, s32 item) {
    MantraFadeState *fade = (MantraFadeState *)((MantraDrawItem *)item)->data;

    fade->clock += 1;
    if (fade->clock >= 0x79) {
        fade->clock = 0;
    }
    if (fade->delay != 0) {
        fade->delay -= 1;
        if (fade->delay == 0) {
            fade->state = fade->queuedState;
            fade->activeFlags = fade->queuedFlags;
        }
    }
    if (fade->timedFlags.armed) {
        fade->timedFlags.countdown -= 1;
        if (fade->timedFlags.countdown == 0) {
            fade->timedFlags.armed = 0;
            fade->activeFlags |= fade->pendingFlags;
            fade->pendingFlags = 0;
        }
    }
    switch (fade->state) {
    case 1:
    case 6:
        fade->value = (f32)fade->timer / 20.0f;
        fade->timer += 1;
        if (fade->timer >= 20) {
            fade->state = 2;
            fade->value = 1.0f;
            fade->timer = 0;
        }
        break;
    case 2:
        fade->value = 1.0f;
        break;
    case 3:
    case 5:
        fade->value = 1.0f - (f32)fade->timer / 5.0f;
        fade->timer += 1;
        if (fade->timer >= 5) {
            fade->timer = 0;
            fade->value = 0.0f;
            if (fade->state == 5) {
                fade->state = 4;
            } else {
                return 1;
            }
        }
        break;
    case 4:
        fade->value = 0.0f;
        break;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00271510);

u32 mnuRegisterMantraTitleDraw(u32 pool) {
    return mnuRegisterMantraDrawItem(pool, 6, mnuUpdateMantraTitleBlinkFade, func_00272F08,
                         mnuInitMantraTitleDraw, mnuReleaseMantraTitleDraw, 0, 0, 0);
}

void mnuBeginMantraTitleDrawExit(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 6);
    *(u16 *)item->data = 3;
}

typedef struct MantraBlinkState {
    /* 0x00 */ u16 state;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ s16 timer;
    /* 0x06 */ s16 clock;
    /* 0x08 */ f32 value;
    /* 0x0C */ s16 delay;
    u16 variant; /* 0x0E: second title toggle */
} MantraBlinkState;

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425338);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425348);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425370);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425380);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004253A8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004253B8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425408);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425420);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425448);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425458);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425490);

void mnuHideMantraTitle(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 6);
    if (item != 0) {
        *(u16 *)item->data = 5;
        evtPrintDeveloperConsoleMessage("Title Draw Hide\n");
    }
}

void mnuShowMantraTitle(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 6);
    if (item != 0) {
        *(u16 *)item->data = 6;
        evtPrintDeveloperConsoleMessage("Title Draw Show\n");
    }
}

void mnuToggleMantraTitleBlink(u32 pool) {
    MantraBlinkState *blink = ((MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 6))->data;

    blink->delay = 10;
    blink->unk2 = blink->unk2 ^ 1;
}

void mnuToggleMantraTitleVariant(u32 pool) {
    MantraBlinkState *blink = ((MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 6))->data;

    blink->delay = 10;
    blink->variant = blink->variant ^ 1;
}

u32 mnuInitMantraTitleDraw(void) {
    u32 data = sdfAllocSizeClassBlock(0x10);
    memset((void *)data, 0, 0x10);
    ((MantraBlinkState *)data)->unk2 = 0;
    ((MantraBlinkState *)data)->timer = 0;
    *(u16 *)data = 1;
    ((MantraBlinkState *)data)->clock = 0;
    evtPrintDeveloperConsoleMessage("Title Draw Init\n");
    return data;
}

void mnuReleaseMantraTitleDraw(u32 obj) {
    sdfReleaseChipBlock(((MantraDrawItem *)obj)->data);
    evtPrintDeveloperConsoleMessage("Title Draw Release\n");
}


s32 mnuUpdateMantraTitleBlinkFade(s32 unused, s32 item) {
    MantraBlinkState *blink = (MantraBlinkState *)((MantraDrawItem *)item)->data;

    blink->clock += 1;
    if (blink->clock >= 0x25) {
        blink->clock = 0;
    }
    if (blink->delay > 0) {
        blink->delay -= 1;
    }
    switch (blink->state) {
    case 1:
    case 6:
        blink->value = (f32)blink->timer / 20.0f;
        blink->timer += 1;
        if (blink->timer >= 20) {
            blink->state = 2;
            blink->value = 1.0f;
            blink->timer = 0;
        }
        break;
    case 2:
        blink->value = 1.0f;
        break;
    case 3:
    case 5:
        blink->value = 1.0f - (f32)blink->timer / 10.0f;
        blink->timer += 1;
        if (blink->timer >= 10) {
            blink->timer = 0;
            blink->value = 0.0f;
            if (blink->state == 5) {
                blink->state = 4;
            } else {
                return 1;
            }
        }
        break;
    case 4:
        blink->value = 0.0f;
        break;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00272F08);

u32 mnuRegisterMantraInfoDraw(u32 pool) {
    return mnuRegisterMantraDrawItem(pool, 7, mnuUpdateMantraInfoPulseFade, func_00273828,
                         mnuInitMantraInfoDraw, mnuReleaseMantraInfoDraw, 10, 0, 0);
}

void mnuBeginMantraInfoDrawExit(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 7);
    *(u16 *)item->data = 3;
}

void mnuHideMantraInfo(u32 ctx) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(ctx, 7);
    if (item != 0) {
        s32 data = (s32)item->data;
        if (*(u16 *)data == 6) {
            *(u32 *)data = ((*(u32 *)data | 0x10000) & 0x1ffff) | 0xa0000;
        } else {
            *(u16 *)data = 5;
        }
        evtPrintDeveloperConsoleMessage("Info Draw Hide\n");
    }
}

void mnuShowMantraInfo(u32 ctx) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(ctx, 7);
    if (item != 0) {
        s32 data = (s32)item->data;
        if (*(u16 *)data == 5) {
            *(u32 *)data = ((*(u32 *)data | 0x10000) & 0x1ffff) | 0xa0000;
        } else {
            *(u16 *)data = 6;
        }
        evtPrintDeveloperConsoleMessage("Info Draw Show\n");
    }
}

typedef struct MantraPulseState {
    u32 state : 16;
    u16 reverse : 1;
    u16 countdown : 15;
    u16 timer;
    s16 clock;
    u16 x;             /* 0x08 */
    u16 y;             /* 0x0A */
    f32 value;
} MantraPulseState;

void mnuSetMantraInfoDrawPosition(u32 pool, s16 x, u16 y) {
    s32 obj = mnuFindMantraDrawItemByKind(pool, 7);
    if (obj != 0) {
        s32 data = (s32)((MantraDrawItem *)obj)->data;
        ((MantraPulseState *)data)->x = x;
        ((MantraPulseState *)data)->y = y;
    }
}

u32 mnuInitMantraInfoDraw(void) {
    u32 data = sdfAllocSizeClassBlock(0x10);
    memset((void *)data, 0, 0x10);
    *(u16 *)data = 1;
    evtPrintDeveloperConsoleMessage("Info Draw Init\n");
    return data;
}

void mnuReleaseMantraInfoDraw(u32 obj) {
    sdfReleaseChipBlock(((MantraDrawItem *)obj)->data);
    evtPrintDeveloperConsoleMessage("Info Draw Release\n");
}


s32 mnuUpdateMantraInfoPulseFade(s32 unused, s32 item) {
    MantraPulseState *pulse = (MantraPulseState *)((MantraDrawItem *)item)->data;

    pulse->clock += 1;
    if (pulse->clock >= 0x3D) {
        pulse->clock = 0;
    }
    if (pulse->countdown != 0) {
        pulse->countdown -= 1;
    }
    switch (pulse->state) {
    case 1:
    case 6:
        pulse->value = (f32)pulse->timer / 19.0f;
        pulse->timer += 1;
        if (pulse->timer >= 0x13) {
            if (pulse->reverse) {
                pulse->state = 4;
            } else {
                pulse->state = 2;
            }
            pulse->value = 1.0f;
            pulse->timer = 0;
            pulse->reverse = 0;
        }
        break;
    case 2:
        pulse->value = 1.0f;
        break;
    case 3:
    case 5:
        pulse->value = 1.0f - (f32)pulse->timer / 21.0f;
        pulse->timer += 1;
        if (pulse->timer >= 0x15) {
            pulse->timer = 0;
            pulse->value = 0.0f;
            if (pulse->state == 5) {
                if (pulse->reverse) {
                    pulse->state = 2;
                } else {
                    pulse->state = 4;
                }
                pulse->reverse = 0;
            } else {
                return 1;
            }
        }
        break;
    case 4:
        pulse->value = 0.0f;
        break;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00273828);

u32 mnuRegisterMantraGaugeDraw(u32 pool) {
    return mnuRegisterMantraDrawItem(pool, 8, mnuUpdateMantraGaugeFade, mnuDrawMantraGauge,
                         mnuInitMantraGaugeData, mnuReleaseMantraGaugeData, 0, 0, 0);
}

typedef struct MantraGaugeSlot {
    u8 active;
    u8 level;
} MantraGaugeSlot;

/* Slot 3 and the control bits occupy the low and high halves of the word at +8. */
typedef struct MantraGaugeState {
    /* 0x00 */ u16 state;
    /* 0x02 */ MantraGaugeSlot slots[4];
    u32 reverse : 1;
    u32 countdown : 8;
    u32 : 7;
    /* 0x0C */ u16 timer;
    /* 0x0E */ u16 pulseClock;
    /* 0x10 */ u32 pad10;
    /* 0x14 */ f32 value;
} MantraGaugeState;


void mnuBeginMantraScrollCursorExit(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 8);
    ((MantraGaugeState *)item->data)->state = 3;
}

void mnuHideMantraScrollCursor(u32 ctx) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(ctx, 8);
    if (item != 0) {
        MantraGaugeState *data = (MantraGaugeState *)item->data;
        u16 state = data->state;
        if (state == 6) {
            data->reverse = 1;
            data->countdown = 5;
        } else if (state != 4) {
            data->state = 5;
        }
        evtPrintDeveloperConsoleMessage("ScrollCursor Draw Hide\n");
    }
}

void mnuShowMantraScrollCursor(u32 ctx) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(ctx, 8);
    if (item != 0) {
        MantraGaugeState *data = (MantraGaugeState *)item->data;
        u16 state = data->state;
        if (state == 5) {
            data->reverse = 1;
            data->countdown = 5;
        } else if (state != 2) {
            data->state = 6;
        }
        evtPrintDeveloperConsoleMessage("ScrollCursor Draw Show\n");
    }
}

void mnuSetMantraScrollCursorSegmentFlags(u32 ctx, u16 flags) {
    s32 obj = mnuFindMantraDrawItemByKind(ctx, 8);
    if (obj != 0) {
        MantraGaugeSlot *entry = ((MantraGaugeState *)((MantraDrawItem *)obj)->data)->slots;
        s32 i;
        for (i = 0; i < 4; i++, entry++) {
            if ((flags >> i) & 1) {
                entry->active = 1;
            } else {
                entry->active = 0;
            }
        }
    }
}

u32 mnuInitMantraGaugeData(void) {
    u32 data = sdfAllocSizeClassBlock(0x18);
    memset((void *)data, 0, 0x18);
    ((MantraGaugeState *)data)->state = 4;
    evtPrintDeveloperConsoleMessage("ScrollCursor Draw Init\n");
    return data;
}

void mnuReleaseMantraGaugeData(u32 obj) {
    sdfReleaseChipBlock(((MantraDrawItem *)obj)->data);
    evtPrintDeveloperConsoleMessage("ScrollCursor Draw Release\n");
}


INCLUDE_ASM(const s32, "game/code_0026DBF8", mnuUpdateMantraGaugeFade);

s32 mnuDrawMantraGauge(s32 unused, s32 item) {
    s32 icons[4] = {0x3F, 0x40, 0x41, 0x42};
    s32 glowIcons[4] = {0x43, 0x44, 0x45, 0x46};
    MantraGaugeState *gauge = (MantraGaugeState *)((MantraDrawItem *)item)->data;
    s32 alpha = gauge->value * 128.0f;
    f32 wave;
    f32 level;
    s32 i;

    switch (gauge->state) {
    case 1:
    case 2:
    case 3:
    case 5:
    case 6:
        gauge->pulseClock += 1;
        if ((s16)gauge->pulseClock >= 0x79) {
            gauge->pulseClock = 0;
        }
        wave = (s16)gauge->pulseClock / 120.0f;
        wave = (sdfSinPoly(wave * (3.14159265f * 2.0f) + -3.14159265f) + 1.0f) * 0.5f;
        for (i = 0; i < 4; i++) {
            level = gauge->slots[i].level / 10.0f;
            mnuDrawMantraSprite(0, 0, 0, (s32)((f32)alpha * level), icons[i], 0, 0x53);
            mnuDrawMantraSprite(0, 0, 0, (s32)((f32)(s32)((f32)alpha * wave * level) * 0.7f), glowIcons[i], 0, 0x53);
        }
        break;
    }
    return 0;
}

/* Eight IDs followed by selected index and count in the menu's source list. */
typedef struct MantraPanelListInput {
    u32 entries[8];
    u32 selectedIndex;
    u32 count;
} MantraPanelListInput;

typedef struct MantraLampState {
    /* 0x00 */ u16 state;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ u16 timer;
    /* 0x06 */ u16 clock;
    /* 0x08 */ f32 value;
    /* 0x0C */ MantraListState list;
    /* 0x3C */ u32 bits;
} MantraLampState;

u32 mnuRegisterMantraUnitPanelDraw(u32 pool, u32 resource) {
    return mnuRegisterMantraDrawItem(pool, 9, mnuUpdateMantraUnitPanelFade, func_00274A70,
                         mnuInitMantraUnitPanelDraw, mnuReleaseMantraUnitPanelDraw, 10, 0, resource);
}

void mnuBeginMantraUnitPanelExit(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 9);
    ((MantraLampState *)item->data)->state = 3;
}

void mnuSetMantraUnitPanelValue(u32 pool, s16 value) {
    s32 obj = mnuFindMantraDrawItemByKind(pool, 9);
    if (obj != 0) {
        ((MantraLampState *)((MantraDrawItem *)obj)->data)->unk2 = value;
    }
}

void mnuHideMantraUnitPanel(u32 ctx) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(ctx, 9);
    if (item != 0) {
        s32 data = (s32)item->data;
        if (((MantraLampState *)data)->state == 6) {
            ((MantraLampState *)data)->bits =
                ((((MantraLampState *)data)->bits | 1) & 0xffff0001) | 0xa;
        } else {
            ((MantraLampState *)data)->state = 5;
        }
        evtPrintDeveloperConsoleMessage("UnitPanel Draw Hide\n");
    }
}

void mnuShowMantraUnitPanel(u32 ctx) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(ctx, 9);
    if (item != 0) {
        s32 data = (s32)item->data;
        if (((MantraLampState *)data)->state == 5) {
            ((MantraLampState *)data)->bits =
                ((((MantraLampState *)data)->bits | 1) & 0xffff0001) | 0xa;
        } else {
            ((MantraLampState *)data)->state = 6;
        }
        evtPrintDeveloperConsoleMessage("UnitPanel Draw Show\n");
    }
}


void mnuAdvanceMantraUnitPanelListState(u32 pool) {
    s32 obj = mnuFindMantraDrawItemByKind(pool, 9);
    mnuQueuePreviousMantraSelection((u32)&((MantraLampState *)((MantraDrawItem *)obj)->data)->list);
}

void mnuQueueNextUnitPanelSelection(u32 pool) {
    s32 obj = mnuFindMantraDrawItemByKind(pool, 9);
    mnuQueueNextMantraSelection((u32)&((MantraLampState *)((MantraDrawItem *)obj)->data)->list);
}

u32 mnuQueueUnitPanelSelection(u32 pool, s8 value) {
    s32 obj = mnuFindMantraDrawItemByKind(pool, 9);
    return mnuQueueMantraSelectionTransition((u32)&((MantraLampState *)((MantraDrawItem *)obj)->data)->list, value) != 0;
}

u32 func_002747F0(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 9);
    func_0026FAA8((s32)&((MantraLampState *)item->data)->list);
    return 0;
}

/* Build the unit-panel fade state and copy its initial selection list. */
u32 mnuInitMantraUnitPanelDraw(u32 ctx, MantraPanelListInput *resources) {
    u32 data = sdfAllocSizeClassBlock(0x40);
    memset((void *)data, 0, 0x40);
    ((MantraLampState *)data)->state = 1;
    mnuInitMantraListEntries(&((MantraLampState *)data)->list, resources->entries,
                  resources->count, resources->selectedIndex);
    evtPrintDeveloperConsoleMessage("UnitPanel Draw Init\n");
    return data;
}

void mnuReleaseMantraUnitPanelDraw(u32 obj) {
    s32 data = (s32)((MantraDrawItem *)obj)->data;
    mnuReleaseDisplayListNodes((u32)&((MantraLampState *)data)->list);
    sdfReleaseChipBlock(data);
    evtPrintDeveloperConsoleMessage("UnitPanel Draw Release\n");
}


s32 mnuUpdateMantraUnitPanelFade(s32 unused, s32 item) {
    MantraLampState *lamp = (MantraLampState *)((MantraDrawItem *)item)->data;
    u32 bits;
    u16 hold;

    lamp->clock += 1;
    if ((s16)lamp->clock >= 0x3D) {
        lamp->clock = 0;
    }
    if (lamp->bits & 0xFFFE) {
        hold = (lamp->bits >> 1) & 0x7FFF;
        hold -= 1;
        lamp->bits = (lamp->bits & 0xFFFF0001) | ((hold & 0x7FFF) << 1);
    }
    switch (lamp->state) {
    case 1:
    case 6:
        lamp->value = (f32)lamp->timer / 20.0f;
        lamp->timer += 1;
        if (lamp->timer >= 20) {
            bits = lamp->bits;
            lamp->state = (bits & 1) ? 4 : 2;
            lamp->value = 1.0f;
            lamp->timer = 0;
            lamp->bits = bits & ~1;
        }
        break;
    case 2:
        lamp->value = 1.0f;
        break;
    case 3:
    case 5:
        lamp->value = 1.0f - (f32)lamp->timer / 10.0f;
        lamp->timer += 1;
        if (lamp->timer >= 10) {
            lamp->timer = 0;
            lamp->value = 0.0f;
            if (lamp->state == 5) {
                bits = lamp->bits;
                lamp->state = (bits & 1) ? 2 : 4;
                lamp->bits = bits & ~1;
            } else {
                return 1;
            }
        }
        break;
    case 4:
        lamp->value = 0.0f;
        break;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00274A70);

u32 mnuRegisterMantraTypeOnePanelDraw(u32 pool) {
    return mnuRegisterMantraDrawItem(pool, 10, mnuUpdateMantraRecordPanelFade, func_00274FF8,
                         mnuCreateTypeOneRecord, mnuReleaseMantraRecordPanelData, 0, 0, 0);
}

void mnuBeginMantraRecordPanelExit(u32 pool) {
    MantraDrawItem *item;

    item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 10);
    ((MantraRecordPanelState *)item->data)->state = 3;
}

void mnuToggleMantraTypeOnePanelMode(u32 pool) {
    MantraDrawItem *item;
    MantraRecordPanelState *record;

    item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 10);
    record = (MantraRecordPanelState *)item->data;
    record->delay = 0xf;
    record->flags = record->flags ^ 1;
}

u32 mnuCreateTypeOneRecord(void) {
    MantraRecordPanelState *record = (MantraRecordPanelState *)sdfAllocSizeClassBlock(sizeof(MantraRecordPanelState));
    memset(record, 0, sizeof(MantraRecordPanelState));
    record->state = 1;
    record->flags = 0;
    record->unk10 = datGameState->header.currency;
    return (u32)record;
}

void mnuReleaseMantraRecordPanelData(u32 obj) {
    sdfReleaseChipBlock(((MantraDrawItem *)obj)->data);
}

s32 mnuUpdateMantraRecordPanelFade(s32 unused, MantraDrawItem *item) {
    MantraRecordPanelState *fade = (MantraRecordPanelState *)item->data;

    fade->cycle += 1;
    if ((s16)fade->cycle >= 0x3D) {
        fade->cycle = 0;
    }
    if (fade->delay > 0) {
        fade->delay -= 1;
    }
    switch (fade->state) {
    case 1:
    case 6:
        fade->scale = (f32)fade->elapsed / 20.0f;
        fade->elapsed += 1;
        if (fade->elapsed >= 20) {
            fade->state = 2;
            fade->scale = 1.0f;
            fade->elapsed = 0;
        }
        break;
    case 2:
        fade->scale = 1.0f;
        break;
    case 3:
    case 5:
        fade->scale = 1.0f - (f32)fade->elapsed / 10.0f;
        fade->elapsed += 1;
        if (fade->elapsed >= 10) {
            fade->elapsed = 0;
            fade->scale = 0.0f;
            if (fade->state == 5) {
                fade->state = 4;
            } else {
                return 1;
            }
        }
        break;
    case 4:
        fade->scale = 0.0f;
        break;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00274FF8);

void mnuDrawMantraCounterTweenB(u32 x, u32 y, u32 depth, u32 fade, MantraCountStateB *state, u32 drawArg) {
    char text[16];
    u32 flags = fade | 0xA09DC300;

    mnuDrawMantraSprite(x, y, depth, fade, 0x2E, 0, drawArg);
    if (datGameState->header.currency != state->shown) {
        s32 steps = 20;

        sndSetSequenceVolumePan(0x13, 0x7F, 0x3F);
        state->step++;
        func_0035C860(text, mnuMantraValueFormat, state->shown + (datGameState->header.currency - state->shown) * state->step / steps);
        if (state->step == steps) {
            state->shown = datGameState->header.currency;
            state->step = 0;
        }
    } else {
        func_0035C860(text, mnuMantraValueFormat, datGameState->header.currency);
    }
    frFontDrawTextVariantAAndMeasure(x + 0x193, y + 0x26, depth, flags, 0, text, 0, drawArg);
}

extern u32 mnuClaimMantraIconEntry(u32 *, u32);

MantraIconEntry *mnuSpawnMantraIcon(s32 x, s32 y, MantraIconPool *pool, u32 mode) {
    MantraIconEntry *entry = (MantraIconEntry *)mnuClaimMantraIconEntry((u32 *)pool, mode);

    if (entry != 0) {
        if (mode & 0x80) {
            if (mode & 1) {
                entry->state = 0xB;
            } else if (mode & 2) {
                entry->state = 0xA;
            }
        } else {
            if (mode & 1) {
                entry->state = 8;
            } else if (mode & 2) {
                entry->state = 7;
            } else if (mode & 4) {
                entry->state = 6;
            } else if (mode & 8) {
                entry->state = 5;
            } else {
                entry->state = 1;
            }
        }
        if (mode == 0x21 || !(mode & 0x8F)) {
            entry->x = x;
            entry->y = y;
        }
        entry->unk7 = 0;
        entry->timer = 0;
        if (pool->current != 0 && (mode & 0x10) && *(s32 *)&pool->current->x == *(s32 *)&entry->x) {
            entry->unk7 = 1;
        }
        if (mode & 0x20) {
            entry->shortLoop = 1;
        }
        if (mode & 0x40) {
            entry->variant = 1;
        } else {
            entry->variant = 0;
        }
        return entry;
    }
    return 0;
}

u32 mnuAllocateMantraIconPool(u32 count) {
    u32 size = count * 12 + 0x14;
    u32 handle = sdfAllocGeneralBlock(size);
    u32 block = sdfMemoryGetBlockAddress(handle);
    memset((void *)block, 0, size);
    *(u32 *)block = handle;
    ((MantraIconPool *)block)->count = count;
    ((MantraIconPool *)block)->entries = (MantraIconEntry *)(block + 0x14);
    return block;
}

void mnuReleaseMantraIconSprite(u32 *sprite) {
    sdfReleaseResourceAllocation(*sprite);
}

u32 mnuClaimMantraIconEntry(u32 *pool, u32 flags) {
    s32 i;
    u32 *item;
    u32 *tail;
    u32 header;

    if (flags & 0x20) {
        if (pool[3] != 0) {
            return pool[3];
        }
    }
    if (flags & 0xF) {
        if (pool[1] == 0) {
            return 0;
        }
        return pool[2];
    }
    item = (u32 *)pool[1];
    for (i = 0; i < (s32)pool[4]; i++, item += 3) {
        if ((item[0] & 1) == 0) {
            memset(item, 0, 0xC);
            header = item[0] | 1;
            item[0] = header;
            if (flags & 0x10) {
                tail = (u32 *)pool[2];
                if (tail != 0) {
                    if (((tail[0] >> 1) & 0xF) == 9) {
                        tail[0] = (tail[0] & 0xFFFFFFE1) | 8;
                    } else {
                        tail[0] = (tail[0] & 0xFFFFFFE1) | 6;
                    }
                    tail[2] = 0;
                }
                pool[2] = (u32)item;
            } else {
                pool[3] = (u32)item;
                item[0] = header | 0x40;
            }
            return (u32)item;
        }
    }
    return 0;
}

extern u32 func_00275CE8();
extern u32 func_00277F38();
s32 mnuDrawMantraFadeIcon(s32, s32, s32, s32, MantraIconPool *, MantraIconEntry *);
s32 mnuDrawMantraFadeIcon2(s32, s32, s32, s32, MantraIconPool *, MantraIconEntry *);

s32 mnuUpdateMantraIconList(u8 *list) {
    MantraIconEntry *entry;
    s32 i;

    entry = ((MantraIconPool *)list)->entries;
    if (entry == 0) {
        return 0;
    }
    for (i = 0; i < ((MantraIconPool *)list)->count; i++, entry++) {
        if (!entry->active) {
            continue;
        }
        switch (entry->state) {
        case 1:
        case 5:
            entry->timer += 1;
            if (entry->timer >= 11) {
                entry->timer = 0;
                if (entry->leaving) {
                    entry->state = 3;
                } else {
                    entry->state = 2;
                }
            }
            break;
        case 2:
            entry->timer += 1;
            if (entry->shortLoop) {
                if (entry->timer >= 26) {
                    entry->timer = 0;
                }
            } else if (entry->timer >= 41) {
                entry->timer = 0;
            }
            if (entry->leaving) {
                entry->timer = 0;
                entry->state = 3;
            }
            break;
        case 3:
        case 6:
            entry->timer += 1;
            if (entry->timer >= 6) {
                entry->timer = 0;
                if (entry->state == 6) {
                    entry->state = 9;
                } else {
                    entry->state = 4;
                }
            }
            break;
        case 4:
            entry->active = 0;
            break;
        case 7:
        case 10:
            entry->timer += 1;
            if (entry->timer >= 31) {
                entry->timer = 0;
                if (entry->state == 10) {
                    entry->state = 12;
                } else {
                    entry->state = 9;
                }
            }
            break;
        case 8:
        case 11:
            entry->timer += 1;
            if (entry->timer >= 31) {
                entry->timer = 0;
                entry->state = 2;
            }
            break;
        case 9:
        case 12:
            break;
        }
    }
    return 0;
}

s32 mnuDrawActiveMantraIcons(s32 x, s32 y, u32 depth, s32 amount, MantraIconPool *pool) {
    MantraIconEntry *icon = pool->entries;
    s32 i;

    if (icon == 0) {
        return 0;
    }
    for (i = 0; i < pool->count; i++, icon++) {
        if (icon->active) {
            if (icon->variant == 0) {
                if (icon->shortLoop == 0) {
                    func_00275CE8(x, y, depth, amount, pool, icon);
                } else {
                    func_00277F38(x, y, depth, amount, pool, icon);
                }
            } else {
                if (icon->shortLoop == 0) {
                    mnuDrawMantraFadeIcon(x, y, depth, amount, pool, icon);
                } else {
                    mnuDrawMantraFadeIcon2(x, y, depth, amount, pool, icon);
                }
            }
        }
    }
    return 0;
}

s32 mnuDrawMantraFadeIcon(s32 x, s32 y, s32 depth, s32 amount, MantraIconPool *pool, MantraIconEntry *icon) {
    s32 count;
    f32 ratio;

    switch (icon->state) {
    case 1:
    case 5:
        count = icon->timer;
        if (count < 5) {
            ratio = (f32)count / 5.0f;
        } else {
            ratio = 1.0f;
        }
        amount = (f32)amount * ratio;
        mnuDrawMantraSprite(x + icon->x, y + icon->y, depth, amount, 0x10B, 0, 0x53);
        break;
    case 2:
        mnuDrawMantraSprite(x + icon->x, y + icon->y, depth, amount, 0x10B, 0, 0x53);
        break;
    case 3:
    case 6:
        count = icon->timer;
        if (count < 5) {
            ratio = (f32)count / 5.0f;
        } else {
            ratio = 1.0f;
        }
        ratio = 1.0f - ratio;
        amount = (f32)amount * ratio;
        mnuDrawMantraSprite(x + icon->x, y + icon->y, depth, amount, 0x10B, 0, 0x53);
        break;
    case 4:
    case 7:
    case 8:
    case 9:
        break;
    }
    return 0;
}

s32 mnuDrawMantraFadeIcon2(s32 x, s32 y, s32 depth, s32 amount, MantraIconPool *pool, MantraIconEntry *icon) {
    s32 count;
    f32 ratio;

    switch (icon->state) {
    case 1:
    case 5:
        count = icon->timer;
        if (count < 5) {
            ratio = (f32)count / 5.0f;
        } else {
            ratio = 1.0f;
        }
        amount = (f32)amount * ratio;
        mnuDrawMantraSprite(x + icon->x, y + icon->y, depth, amount, 0x10C, 0, 0x53);
        break;
    case 2:
        mnuDrawMantraSprite(x + icon->x, y + icon->y, depth, amount, 0x10C, 0, 0x53);
        break;
    case 3:
    case 6:
        count = icon->timer;
        if (count < 5) {
            ratio = (f32)count / 5.0f;
        } else {
            ratio = 1.0f;
        }
        ratio = 1.0f - ratio;
        amount = (f32)amount * ratio;
        mnuDrawMantraSprite(x + icon->x, y + icon->y, depth, amount, 0x10C, 0, 0x53);
        break;
    case 4:
    case 7:
    case 8:
    case 9:
        break;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425828);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00275CE8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00277F38);

u32 mnuRegisterMantraIconListADraw(u32 pool, u32 resource) {
    return mnuRegisterMantraDrawItem(pool, 2, mnuUpdateMantraFadeA, mnuDrawMantraIconList,
                         mnuCreateMantraIconListA, mnuReleaseMantraFadeData, 0, 0, resource);
}

void mnuBeginMantraIconListFadeOut(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 2);
    if (item != 0) {
        ((MantraFadeData *)item->data)->state = 3;
    }
}

void mnuSetMantraIconListPosition(s16 x, s16 y, u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 2);
    if (item != 0) {
        MantraFadeData *fade = (MantraFadeData *)item->data;
        fade->x = x;
        fade->y = y;
    }
}

void mnuSpawnMantraIconAtPosition(u32 a, u32 b, u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 2);
    if (item != 0) {
        mnuSpawnMantraIcon(a, b, ((MantraFadeData *)item->data)->iconPool, 0x10);
    }
}

void func_00278EA8(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 2);
    if (item != 0) {
        mnuSpawnMantraIcon(0, 0, ((MantraFadeData *)item->data)->iconPool, 0x18);
    }
}

void func_00278EE0(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 2);
    if (item != 0) {
        mnuSpawnMantraIcon(0, 0, ((MantraFadeData *)item->data)->iconPool, 0x14);
    }
}

void mnuSpawnMantraIconAndSelectPanelEntry(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 2);
    if (item != 0) {
        mnuSpawnMantraIcon(0, 0, ((MantraFadeData *)item->data)->iconPool, 0x11);
        mnuStorePanelEntry(0x20003, 5);
    }
}

void func_00278F60(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 2);
    if (item != 0) {
        mnuSpawnMantraIcon(0, 0, ((MantraFadeData *)item->data)->iconPool, 0x91);
        mnuStorePanelEntry(0x20003, 5);
    }
}

void func_00278FA8(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 2);
    if (item != 0) {
        mnuSpawnMantraIcon(0, 0, ((MantraFadeData *)item->data)->iconPool, 0x92);
        mnuStorePanelEntry(0x20002, 0);
    }
}

void func_00278FF0(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 2);
    if (item != 0) {
        mnuSpawnMantraIcon(0, 0, ((MantraFadeData *)item->data)->iconPool, 0x12);
    }
}

void mnuSpawnMantraShortLoopIconAtPosition(u32 a, u32 b, u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 2);
    if (item != 0) {
        mnuSpawnMantraIcon(a, b, ((MantraFadeData *)item->data)->iconPool, 0x20);
    }
}

void func_00279080(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 2);
    if (item != 0) {
        mnuSpawnMantraIcon(0, 0, ((MantraFadeData *)item->data)->iconPool, 0x28);
    }
}

void func_002790B8(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 2);
    if (item != 0) {
        mnuSpawnMantraIcon(0, 0, ((MantraFadeData *)item->data)->iconPool, 0x24);
    }
}

void func_002790F0(u32 a, u32 b, u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 2);
    if (item != 0) {
        mnuSpawnMantraIcon(a, b, ((MantraFadeData *)item->data)->iconPool, 0x21);
    }
}

void func_00279148(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 2);
    if (item != 0) {
        mnuSpawnMantraIcon(0, 0, ((MantraFadeData *)item->data)->iconPool, 0x22);
    }
}

/* Seed the fade list with the current selection and its saved node position. */
u32 mnuCreateMantraIconListA(s32 unused, u8 *menu) {
    u32 data = sdfAllocSizeClassBlock(0x10);
    MantraMenuWork *slot;
    MantraNodePos *first;
    MantraNodePos *second;

    memset((void *)data, 0, 0x10);
    ((MantraFadeData *)data)->iconPool = (MantraIconPool *)mnuAllocateMantraIconPool(0xA);
    ((MantraFadeData *)data)->state = 1;
    slot = &((MantraMenu *)menu)->work;
    first = mnuGetMantraNodePositionRecord(scrGetSelectedScriptEntryId((DatPartyRecord *)mnuGetSelectedNodeValue(menu)));
    mnuSpawnMantraIcon(first->x / 10.0f * 40.0f, first->y / 10.0f * 39.0f, ((MantraFadeData *)data)->iconPool, 0x20);
    second = slot->selectedNode;
    mnuSpawnMantraIcon(second->x / 10.0f * 40.0f, second->y / 10.0f * 39.0f, ((MantraFadeData *)data)->iconPool, 0x10);
    return data;
}

void mnuReleaseMantraFadeData(s32 obj) {
    MantraFadeData *fade = (MantraFadeData *)((MantraDrawItem *)obj)->data;
    mnuReleaseMantraIconSprite((u32 *)fade->iconPool);
    sdfReleaseChipBlock(fade);
}

s32 mnuUpdateMantraFadeA(s32 unused, s32 item) {
    MantraFadeData *fade = (MantraFadeData *)((MantraDrawItem *)item)->data;

    mnuUpdateMantraIconList((u8 *)fade->iconPool);
    switch (fade->state) {
    case 1:
    case 6:
        fade->scale = (f32)fade->elapsed / 20.0f;
        fade->elapsed += 1;
        if (fade->elapsed >= 20) {
            fade->state = 2;
            fade->scale = 1.0f;
            fade->elapsed = 0;
        }
        break;
    case 2:
        fade->scale = 1.0f;
        break;
    case 3:
    case 5:
        fade->scale = 1.0f - (f32)fade->elapsed / 10.0f;
        fade->elapsed += 1;
        if (fade->elapsed >= 10) {
            fade->elapsed = 0;
            fade->scale = 0.0f;
            if (fade->state == 5) {
                fade->state = 4;
            } else {
                return 1;
            }
        }
        break;
    case 4:
        fade->scale = 0.0f;
        break;
    }
    return 0;
}

u32 mnuDrawMantraIconList(u32 unused, u32 obj) {
    u32 data = (u32)((MantraDrawItem *)obj)->data;
    s32 scale = (s32)(((MantraFadeData *)data)->scale * 128.0f);
    mnuDrawActiveMantraIcons(((MantraFadeData *)data)->x, ((MantraFadeData *)data)->y, 0,
                  scale, ((MantraFadeData *)data)->iconPool);
    return 0;
}

/* Seed the alternate-scale list from the same current and saved positions. */
u32 mnuCreateMantraIconListB(s32 unused, u8 *menu) {
    u32 data = sdfAllocSizeClassBlock(0x10);
    MantraMenuWork *slot;
    MantraNodePos *first;
    MantraNodePos *second;

    memset((void *)data, 0, 0x10);
    ((MantraFadeData *)data)->iconPool = (MantraIconPool *)mnuAllocateMantraIconPool(0xA);
    ((MantraFadeData *)data)->state = 1;
    slot = &((MantraMenu *)menu)->work;
    first = mnuGetMantraNodePositionRecord(scrGetSelectedScriptEntryId((DatPartyRecord *)mnuGetSelectedNodeValue(menu)));
    mnuSpawnMantraIcon(first->x * 20 / 10.0f, first->y * 20 / 10.0f, ((MantraFadeData *)data)->iconPool, 0x60);
    second = slot->selectedNode;
    mnuSpawnMantraIcon(second->x * 20 / 10.0f, second->y * 20 / 10.0f, ((MantraFadeData *)data)->iconPool, 0x50);
    return data;
}

u32 mnuRegisterMantraIconListBDraw(u32 pool, u32 resource) {
    return mnuRegisterMantraDrawItem(pool, 3, mnuUpdateMantraFadeA, mnuDrawMantraIconList,
                         mnuCreateMantraIconListB, mnuReleaseMantraFadeData, 0, 0, resource);
}


/* Use the saved node's panel-table index for the first icon. */
u32 mnuCreateMantraIconListC(s32 unused, u8 *menu) {
    u32 data = sdfAllocSizeClassBlock(0x10);
    MantraMenuWork *slot = &((MantraMenu *)menu)->work;
    MantraNodePos *first;
    MantraNodePos *second;

    memset((void *)data, 0, 0x10);
    ((MantraFadeData *)data)->iconPool = (MantraIconPool *)mnuAllocateMantraIconPool(0xA);
    ((MantraFadeData *)data)->state = 1;
    first = mnuGetMantraPanelPositionRecord(((MantraMenu *)menu)->work.selectedNode->panelIndex);
    mnuSpawnMantraIcon(first->x * 20 / 10.0f, first->y * 20 / 10.0f, ((MantraFadeData *)data)->iconPool, 0x60);
    second = slot->selectedNode;
    mnuSpawnMantraIcon(second->x * 20 / 10.0f, second->y * 20 / 10.0f, ((MantraFadeData *)data)->iconPool, 0x50);
    return data;
}

u32 mnuRegisterMantraIconListCDraw(u32 pool, u32 resource) {
    return mnuRegisterMantraDrawItem(pool, 3, mnuUpdateMantraFadeA, mnuDrawMantraIconList,
                         mnuCreateMantraIconListC, mnuReleaseMantraFadeData, 0, 0, resource);
}

void mnuBeginMantraIconListExit(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 3);
    if (item != 0) {
        ((MantraFadeData *)item->data)->state = 3;
    }
}

void mnuSetMantraIconFadePosition(s16 x, s16 y, u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 3);
    if (item != 0) {
        MantraFadeData *fade = (MantraFadeData *)item->data;
        fade->x = x;
        fade->y = y;
    }
}

void mnuSpawnMantraVariantIconAtPosition(u32 a, u32 b, u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 3);
    if (item != 0) {
        mnuSpawnMantraIcon(a, b, ((MantraFadeData *)item->data)->iconPool, 0x50);
    }
}

void func_002798A0(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 3);
    if (item != 0) {
        mnuSpawnMantraIcon(0, 0, ((MantraFadeData *)item->data)->iconPool, 0x58);
    }
}

void func_002798D8(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 3);
    if (item != 0) {
        mnuSpawnMantraIcon(0, 0, ((MantraFadeData *)item->data)->iconPool, 0x54);
    }
}

void mnuSpawnMantraShortLoopVariantIconAtPosition(u32 a, u32 b, u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 3);
    if (item != 0) {
        mnuSpawnMantraIcon(a, b, ((MantraFadeData *)item->data)->iconPool, 0x60);
    }
}

void func_00279968(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 3);
    if (item != 0) {
        mnuSpawnMantraIcon(0, 0, ((MantraFadeData *)item->data)->iconPool, 0x68);
    }
}

void func_002799A0(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 3);
    if (item != 0) {
        mnuSpawnMantraIcon(0, 0, ((MantraFadeData *)item->data)->iconPool, 0x64);
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002799D8);

void mnuReleaseMantraIconSpriteHandle(u32 *sprite) {
    if (sprite != 0) {
        sdfReleaseResourceAllocation(*sprite);
    }
}


typedef struct MantraPanelPool MantraPanelPool;
typedef s32 (*MantraPanelDraw)(s32, s32, u32, s32,
    MantraPanelPool *, MantraPanelAnimation *, u32);
typedef void (*MantraPanelInit)(MantraPanelPool *, MantraPanelAnimation *);
typedef void (*MantraPanelRelease)(MantraPanelAnimation *);

/* Native creator reserves 974 entries per callback table before the slot array. */
struct MantraPanelPool {
    u32 handle;
    MantraPanelAnimation *items;
    s32 count;
    MantraPanelDraw draw[974];
    MantraPanelInit init[974];
    MantraPanelRelease release[974];
    s32 unk2DB4;
};

MantraPanelAnimation *mnuFindFreePanelSlot(MantraPanelPool *, s8);

MantraPanelAnimation *mnuSpawnPanelSlotA(MantraPanelPool *pool, s32 id, s8 kind, s16 x, s16 y, u32 w) {
    MantraPanelAnimation *panel = mnuFindFreePanelSlot(pool, kind);
    MantraNodePos *pos;

    if (panel == 0) {
        return 0;
    }
    if ((panel->flags >> 8) & 1) {
        return 0;
    }
    panel->flags = ((panel->flags | 0x08000100) & 0xFF87FFFF) | 0x300000;
    panel->tag.kind = kind;
    panel->startDelay = x;
    panel->endDelay = y;
    panel->animationTicks = w;
    panel->flags = ((panel->flags & 0xFFFC03FF) | 0x400) & 0xEFFFFFFF;
    panel->transitionDelay = 0;
    panel->id = id;
    panel->frame = 0;
    pos = mnuGetMantraNodePositionRecord(id);
    panel->x = (s16)(pos->x / 10.0f * 40.0f);
    panel->y = (s16)(pos->y / 10.0f * 39.0f);
    return panel;
}

MantraPanelAnimation *mnuSpawnPanelSlotB(MantraPanelPool *pool, s32 id, s8 kind, s16 x, s16 y, u32 w) {
    MantraPanelAnimation *panel = mnuFindFreePanelSlot(pool, kind);
    MantraNodePos *pos;

    if (panel == 0) {
        return 0;
    }
    if ((panel->flags >> 8) & 1) {
        return 0;
    }
    panel->flags = ((panel->flags | 0x08000100) & 0xFF87FFFF) | 0x300000;
    panel->tag.kind = kind;
    panel->startDelay = x;
    panel->endDelay = y;
    panel->animationTicks = w;
    panel->flags = (panel->flags & 0xFFFC03FF) | 0x10000400;
    panel->transitionDelay = 0;
    panel->id = id;
    panel->frame = 0;
    pos = mnuGetMantraPanelPositionRecord(id);
    panel->x = (s16)(pos->x / 10.0f * 40.0f);
    panel->y = (s16)(pos->y / 10.0f * 39.0f);
    return panel;
}

void mnuOffsetPanelAndSetVisualParams(MantraPanelAnimation *panel, s32 dx, s32 dy, u32 a, u32 b, u32 c, u8 d, u8 e) {
    dx = (s16)dx + panel->x;
    dy = (s16)dy + panel->y;
    panel->x = dx;
    panel->y = dy;
    panel->visualParameters[0] = a;
    panel->visualParameters[1] = b;
    panel->visualParameters[2] = c;
    panel->byte1C = d;
    panel->byte1D = e;
}

void mnuResetPanelAnimationFlags(MantraPanelAnimation *panel) {
    panel->flags = (panel->flags & 0xfffc03ff) | 0x800;
}

void mnuTransitionActivePanelAnimations(MantraPanelPool *pool, s32 mode) {
    s32 i;
    u32 *item = (u32 *)pool->items;

    for (i = 0; i < pool->count; i++, item += 12) {
        if ((((MantraPanelAnimation *)item)->flags >> 8) & 1) {
            if (((((MantraPanelAnimation *)item)->flags >> 19) & 0xF) == 2) {
                mnuQueuePanelAnimationTransition((MantraPanelAnimation *)item, 5, 0);
            } else if (mode == 1) {
                mnuQueuePanelAnimationTransition((MantraPanelAnimation *)item, 7, 0);
            } else if (mode == 0) {
                mnuQueuePanelAnimationTransition((MantraPanelAnimation *)item, 9, 0);
            } else {
                mnuResetPanelAnimationFlags((MantraPanelAnimation *)item);
            }
        }
    }
}

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004258B8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004258F0);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425928);

s32 mnuAdvanceMantraPanelAnim(s32 unused, MantraPanelAnimation *panel) {
    s32 result;

    if (panel->transitionDelay > 0) {
        panel->transitionDelay -= 1;
        if (panel->transitionDelay == 0) {
            panel->frame = 0;
            panel->flags = (panel->flags & 0xFF87FFFF) | (((panel->flags >> 23) & 0xF) << 19);
        }
    }
    result = 0;
    switch ((panel->flags >> 19) & 0xF) {
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        panel->frame += 1;
        if (panel->frame >= 4) {
            panel->frame = 0;
            panel->flags = panel->flags & 0xFF87FFFF;
        }
        break;
    case 8:
        panel->frame += 1;
        if (panel->frame >= 10) {
            panel->frame = 0;
            panel->flags = panel->flags & 0xFF87FFFF;
        }
        break;
    case 7:
        panel->frame += 1;
        if (panel->frame >= 4) {
            result = 1;
        }
        break;
    case 9:
        panel->frame += 1;
        result = panel->frame > 9;
        break;
    }
    return result;
}

/* Update slot lifetimes, then draw the active animations at the caller's alpha. */
s32 func_0027A198(s16 x, s16 y, u32 flags, s32 alpha, MantraPanelPool *pool, u32 packet) {
    MantraPanelAnimation *panel;
    f32 opacity;
    s32 i;

    panel = pool->items;
    for (i = 0; i < pool->count; i++, panel++) {
        if ((panel->flags >> 8) & 1) {
            switch ((panel->flags >> 10) & 0xFF) {
            case 1:
                if (panel->startDelay > 0) {
                    panel->startDelay--;
                }
                if (panel->startDelay == 0) {
                    pool->init[panel->tag.kind](pool, panel);
                    panel->flags = ((panel->flags & 0xFFFC03FF) | 0x1800) & 0xF7FFFFFF;
                }
                break;
            case 2:
                if (panel->endDelay > 0) {
                    panel->endDelay--;
                } else if (panel->endDelay == 0) {
                    pool->release[panel->tag.kind](panel);
                    panel->flags &= ~0x100;
                }
                break;
            }
        }
    }
    opacity = alpha * 0.0078125f;
    sdfSubmitGsTestOneRegisterPacket(0x30000, packet);
    uiDrawUniformColorRect(0, 0, 0, 0x2000, 0xE00, 0, packet);
    panel = pool->items;
    for (i = 0; i < pool->count; i++, panel++) {
        if (((panel->flags >> 27) & 1) == 0 && ((panel->flags >> 8) & 1)) {
            if (((panel->flags >> 10) & 0xFF) != 5) {
                if (((panel->flags >> 10) & 0xFF) == 6) {
                    s16 drawX;
                    s16 drawY;

                    if (panel->animationTicks > 0) {
                        panel->animationTicks--;
                        if (panel->animationTicks == 0) {
                            panel->flags = (panel->flags & 0xFFFC03FF) | 0x800;
                        }
                    }
                    if (mnuAdvanceMantraPanelAnim((s32)pool, panel) != 0) {
                        panel->flags = (panel->flags & 0xFFFC03FF) | 0x800;
                    }
                    /* Stored positions wrap as u16; rendering applies signed screen offsets. */
                    drawX = panel->x;
                    drawY = panel->y;
                    if (pool->draw[panel->tag.kind](drawX + x, drawY + y, flags,
                        panel->visualParameters[1] * opacity, pool, panel, packet) != 0) {
                        panel->flags = (panel->flags & 0xFFFC03FF) | 0x800;
                    }
                }
            } else {
                panel->flags &= ~0x100;
            }
        }
    }
    return 0;
}

MantraPanelAnimation *mnuFindFreePanelSlot(MantraPanelPool *pool, s8 kind) {
    s32 start[14] = {0, 0xB0, 0x160, 0x210, 0x238, 0x260, 0x288, 0x2B0, 0x2C4, 0x2E2, 0x300, 0x30A, 0x314, 0x31E};
    s32 count[14] = {0xB0, 0xB0, 0xB0, 0x28, 0x28, 0x28, 0x28, 0x14, 0x1E, 0x1E, 0xA, 0xA, 0xA, 0xB0};
    MantraPanelAnimation *panel = pool->items + start[kind];
    s32 i;

    for (i = 0; i < count[kind]; i++, panel++) {
        if (((panel->flags >> 8) & 1) == 0) {
            return panel;
        }
    }
    return 0;
}

MantraPanelAnimation *mnuFindPanelSlotById(MantraPanelPool *pool, s32 id, s8 kind) {
    s32 start[14] = {0, 0xB0, 0x160, 0x210, 0x238, 0x260, 0x288, 0x2B0, 0x2C4, 0x2E2, 0x300, 0x30A, 0x314, 0x31E};
    s32 count[14] = {0xB0, 0xB0, 0xB0, 0x28, 0x28, 0x28, 0x28, 0x14, 0x1E, 0x1E, 0xA, 0xA, 0xA, 0xB0};
    MantraPanelAnimation *panel = pool->items + start[kind];
    s32 i;

    for (i = 0; i < count[kind]; i++, panel++) {
        if (((panel->flags >> 8) & 1) && panel->id == id) {
            return panel;
        }
    }
    return 0;
}

u32 mnuQueuePanelAnimationTransition(MantraPanelAnimation *panel, u32 bits, s16 length) {
    u32 flags = (panel->flags & 0xF87FFFFF) | ((bits & 0xF) << 23);
    panel->transitionDelay = length;
    panel->flags = flags;
    if (length == 0) {
        panel->frame = 0;
        panel->flags = (flags & 0xFF87FFFF) | ((bits & 0xF) << 19);
    }
    return 0;
}

void func_0027A7F0(void) {
}

void func_0027A7F8(void) {
}

extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);
extern void func_0027AFE0(s32 x, s32 y, s32 z, s32 amount, s32 unused, u8 *object, s32 packet);

/* Mantra panel C transition / appear draw: kinds 6..9 fade the panel in or out over its frame counter, kind 0
 * draws it steady, and kind 1 plays the 60-frame appear (panel fade-in, pulsing inner glow, wave flash and three
 * orbiting sparks) and reports completion. */
s32 func_0027A800(s32 x, s32 y, s32 z, s32 amount, s32 unused, u8 *object, s32 packet) {
    MantraPanelAnimation *panel = (MantraPanelAnimation *)object;
    f32 scale;
    f32 wave;
    f32 spark;
    f32 spin;
    f32 pulse;
    f32 angle;
    f32 phase;  /* retail keeps the spark start angle apart from spin ($f21) */
    f32 radius; /* retail loads 15.0f into $f24 before the loop's other constants */
    s32 alpha;
    s32 i;

    switch ((panel->flags >> 19) & 0xF) {
    case 6:
        scale = panel->frame * 0.25f;
        alpha = amount * scale;
        mnuDrawMantraSprite(x, y, z, alpha, 0x77, 0, packet);
        mnuDrawMantraSprite(x, y, z, alpha, 0x95, 0, packet);
        break;
    case 8:
        scale = panel->frame / 10.0f;
        alpha = amount * scale;
        mnuDrawMantraSprite(x, y, z, alpha, 0x77, 0, packet);
        mnuDrawMantraSprite(x, y, z, alpha, 0x95, 0, packet);
        break;
    case 7:
        scale = panel->frame * 0.25f;
        scale = 1.0f - scale;
        alpha = amount * scale;
        mnuDrawMantraSprite(x, y, z, alpha, 0x77, 0, packet);
        mnuDrawMantraSprite(x, y, z, alpha, 0x95, 0, packet);
        break;
    case 9:
        scale = panel->frame / 10.0f;
        scale = 1.0f - scale;
        alpha = amount * scale;
        mnuDrawMantraSprite(x, y, z, alpha, 0x77, 0, packet);
        mnuDrawMantraSprite(x, y, z, alpha, 0x95, 0, packet);
        break;
    case 0:
        mnuDrawMantraSprite(x, y, z, amount, 0x77, 0, packet);
        mnuDrawMantraSprite(x, y, z, amount, 0x95, 0, packet);
        break;
    case 1:
        panel->frame++;
        scale = 0.0f;
        if (panel->frame >= 30) {
            if (panel->frame < 40) {
                scale = (panel->frame - 30) / 10.0f;
            } else {
                scale = 1.0f;
            }
        }
        scale = 1.0f - scale;
        if (panel->frame < 40) {
            wave = panel->frame / 40.0f;
        } else if (panel->frame < 50) {
            wave = (50 - panel->frame) / 20.0f;
        } else {
            wave = 0.0f;
        }
        wave = sdfSinPoly(wave * 1.5707963f);
        if (panel->frame >= 10) {
            if (panel->frame < 40) {
                spark = (panel->frame - 10) / 30.0f;
            } else if (panel->frame < 50) {
                spark = (50 - panel->frame) / 10.0f;
            } else {
                spark = 0.0f;
            }
        } else {
            spark = 0.0f;
        }
        if (panel->frame < 60) {
            spin = panel->frame / 60.0f;
        } else {
            spin = 1.0f;
        }
        pulse = 0.0f;
        if (panel->frame >= 30) {
            pulse = (panel->frame - 30) / 30.0f;
        }
        pulse = (sdfSinPoly(pulse * 6.2831853f + -1.5707963f) + 1.0f) * 0.5f;
        alpha = (f32)amount * scale;
        mnuDrawMantraSprite(x, y, z, alpha, 0x77, 0, packet);
        mnuDrawMantraSprite(x, y, z, alpha, 0x95, 0, packet);
        alpha = (f32)amount * pulse;
        func_0027AFE0(x, y, z, alpha, unused, object, packet);
        mnuDrawMantraSprite(x, y, z, alpha, 0x99, 0, packet);
        mnuDrawMantraSprite(x, y, z, (f32)amount * wave * 0.7f, 0x99, 0, packet);
        sdfSubmitGsTestOneRegisterPacket(0x30000, packet);
        uiDrawUniformColorRect((x - 0x80) << 4, (y - 0x80) << 3, 0xFFFFFF, 0x1000, 0x800, 0, packet);
        sdfSubmitGsTestOneRegisterPacket(0x3000DL, packet);
        uiDrawActiveSurfaceRegion(packet);
        mnuDrawMantraSprite(x, y, 0, amount, 0x7C, 0x60, packet);
        sdfDispatchSurfaceWithPreparedTexturePacket(packet);
        radius = 15.0f;
        phase = spin * -6.2831853f * 0.25f;
        for (i = 0; i < 3; i++) {
            angle = phase + i * 2.0943951f;
            mnuDrawMantraSprite((f32)x + sdfEvaluateCosineViaSinePhaseShift(angle) * radius,
                                (f32)y + -sdfSinPoly(angle) * radius, 1, (f32)amount * 0.6f * spark, 0x7D, 0,
                                packet);
        }
        sdfSubmitGsTestOneRegisterPacket(0x30000, packet);
        uiDrawUniformColorRect((x - 0x80) << 4, (y - 0x80) << 3, 0, 0x1000, 0x800, 0, packet);
        sdfSubmitGsTestOneRegisterPacket(0x5100DL, packet);
        if (panel->frame >= 60) {
            return 1;
        }
        break;
    }
    return 0;
}

extern f32 effMiscRandUnitFloat(void *state);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378C0);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378C8);

void mnuResetMantraPulsePhase(s32 unused, u8 *object) {
    u8 table[4] = {0, 20, 40, 60};
    u8 index;

    object[0x20] = 0;
    object[0x21] = 0;
    index = effMiscRandUnitFloat(0) * 3.0f;
    object[0x22] = table[index];
}

void func_0027AFD8(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027AFE0);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425A98);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425AB8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425AC8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027B678);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378D8);

void mnuRandomizeMantraPulseIconPhase(s32 unused, u8 *object) {
    u8 table[4] = {0, 10, 20, 30};
    u8 index;

    index = effMiscRandUnitFloat(0) * 3.0f;
    object[0x20] = table[index];
}

void mnuDrawOscillatingMantraOverlay(void) {
}

s32 mnuDrawMantraPulseIcon(s32 x, s32 y, s32 z, s32 amount, s32 unused, u8 *object, s32 packet) {
    u8 table[4] = {0, 10, 20, 30};
    f32 t;

    object[0x20] += 1;
    if (object[0x20] > 0x82) {
        object[0x20] = table[(u8)(effMiscRandUnitFloat(0) * 3.0f)];
    }
    t = 0.0f;
    if (object[0x20] >= 0x32) {
        if (object[0x20] < 0x78) {
            t = (object[0x20] - 0x32) / 70.0f;
        }
    }
    t = (sdfSinPoly(t * (3.14159265f * 2.0f) + (-3.14159265f / 2.0f)) + 1.0f) * 0.5f;
    mnuDrawMantraSprite(x, y, z, amount, 0x77, 0, packet);
    mnuDrawMantraSprite(x, y, z, amount, 0x93, 0, packet);
    mnuDrawMantraSprite(x, y, z, (s32)(amount * (t * 0.25f)), 0x94, 0, packet);
    return 0;
}

s32 mnuDrawMantraPulseIconWithFadeState(s32 x, s32 y, s32 z, s32 amount, s32 unused, u8 *object, s32 packet) {
    f32 scale;

    switch ((((MantraPanelAnimation *)object)->flags >> 19) & 0xF) {
    case 6:
        scale = ((MantraPanelAnimation *)object)->frame * 0.25f;
        amount = amount * scale;
        mnuDrawMantraPulseIcon(x, y, z, amount, unused, object, packet);
        break;
    case 8:
        scale = ((MantraPanelAnimation *)object)->frame / 10.0f;
        amount = amount * scale;
        mnuDrawMantraPulseIcon(x, y, z, amount, unused, object, packet);
        break;
    case 7:
        scale = ((MantraPanelAnimation *)object)->frame * 0.25f;
        scale = 1.0f - scale;
        amount = amount * scale;
        mnuDrawMantraPulseIcon(x, y, z, amount, unused, object, packet);
        break;
    case 9:
        scale = ((MantraPanelAnimation *)object)->frame / 10.0f;
        scale = 1.0f - scale;
        amount = amount * scale;
        mnuDrawMantraPulseIcon(x, y, z, amount, unused, object, packet);
        break;
    case 0:
        mnuDrawMantraPulseIcon(x, y, z, amount, unused, object, packet);
        break;
    }
    return 0;
}

void btlInitPanelASprite(u32 unused, s32 view) {
    u32 spriteHandle;

    spriteHandle = mnuAllocateMantraSparkleEmitter(1);
    ((MantraPanelAnimation *)view)->spriteHandle = spriteHandle;
}

void btlReleasePanelASprite(u32 obj) {
    mnuFreeMantraSparkleEmitter(((MantraPanelAnimation *)obj)->spriteHandle);
}

s32 btlDrawPanelA(s32 x, s32 y, u32 z, u32 amount, u32 unused, u32 object, u32 packet) {
    mnuDrawMantraSprite(x, y, 0, amount, 0x77, 0, packet);
    mnuDrawMantraSprite(x, y, 0, amount, 0x89, 0, packet);
    sdfSubmitGsTestOneRegisterPacket(0x30000, packet);
    uiDrawUniformColorRect((x - 0x80) << 4, (y - 0x80) << 3, 0xffffff, 0x1000, 0x800, 0, packet);
    sdfSubmitGsTestOneRegisterPacket(0x3000DL, packet);
    uiDrawActiveSurfaceRegion(packet);
    mnuDrawMantraSprite(x, y, 0, 0x80, 0x91, 0x60, packet);
    sdfDispatchSurfaceWithPreparedTexturePacket(packet);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027C558);

void mnuInitMantraPanelSpriteView(u32 unused, s32 view) {
    u32 spriteHandle;

    spriteHandle = mnuAllocateMantraSparkleEmitter(2);
    ((MantraPanelAnimation *)view)->spriteHandle = spriteHandle;
    ((MantraPanelAnimation *)view)->stateA = 0;
    ((MantraPanelAnimation *)view)->stateB = 0;
}

void mnuReleaseMantraPanelSpriteView(u32 obj) {
    mnuFreeMantraSparkleEmitter(((MantraPanelAnimation *)obj)->spriteHandle);
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027CDD0);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425B68);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425B78);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425B88);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027D3D8);

void func_0027DE30(void) {
}

void func_0027DE38(void) {
}

extern u16 mnuGetPanelValueAt(MenuPanelObject *, s32);

s32 mnuDrawMantraNeighborMarkers(s32 x, s32 y, s32 z, s32 alpha, s32 menuAddress,
                  u8 *object, s32 packet) {
    s8 offsets[6][2] = {
        {49, 22}, {69, 32}, {69, 62},
        {49, 72}, {29, 62}, {29, 32}
    };
    MantraPanelAnimation *panel = (MantraPanelAnimation *)object;
    MenuPanelObject *panelObject = ((MantraMenu *)menuAddress)->work.panelObject;
    MantraNodePos **neighbor;
    s32 mask;
    s32 i;
    f32 weight;

    panel->stateA++;
    if (panel->stateA >= 131) {
        panel->stateA = 0;
    }
    if (panel->stateA < 30) {
        weight = 0.0f;
    } else if (panel->stateA < 90) {
        weight = (f32)(panel->stateA - 30) / 60.0f * 0.5f;
    } else {
        weight = (f32)(panel->stateA - 90) / 40.0f * 0.5f + 0.5f;
    }
    mask = 0;
    i = 0;
    weight = (sdfSinPoly(weight * -(3.14159265f * 2.0f)) + 1.0f) * 0.5f;
    mnuDrawMantraSprite(x, y, z, alpha, 0x77, 0, packet);
    mnuDrawMantraSprite(x, y, z, alpha, 0x93, 0, packet);
    neighbor = mnuGetMantraNodePositionRecord(panel->id)->neighbors;
    do {
        MantraNodePos *node = *neighbor++;
        u32 value = mnuGetPanelValueAt(panelObject, node->panelIndex);
        if ((value >> 8) & 1) {
            mask |= 3 << i;
        }
        i++;
    } while (i < 6);
    if (mask & 0x40) {
        mask |= 1;
    }
    for (i = 0; i < 6; i++) {
        if ((mask >> i) & 1) {
            mnuDrawMantraScaledCenteredSprite(
                x + offsets[i][0], y + offsets[i][1], 0,
                alpha * (weight * 0.1f + 0.3f), 0x92, 0, packet,
                weight * 0.3f + 1.0f);
        }
    }
    return 0;
}


s32 mnuDrawMantraPanelSpriteTransition(s32 x, s32 y, s32 z, s32 amount, s32 unused, u8 *object, s32 packet) {
    f32 scale;

    switch ((((MantraPanelAnimation *)object)->flags >> 19) & 0xF) {
    case 6:
        scale = ((MantraPanelAnimation *)object)->frame * 0.25f;
        amount = amount * scale;
        mnuDrawMantraNeighborMarkers(x, y, z, amount, unused, object, packet);
        break;
    case 8:
        scale = ((MantraPanelAnimation *)object)->frame / 10.0f;
        amount = amount * scale;
        mnuDrawMantraNeighborMarkers(x, y, z, amount, unused, object, packet);
        break;
    case 7:
        scale = ((MantraPanelAnimation *)object)->frame * 0.25f;
        scale = 1.0f - scale;
        amount = amount * scale;
        mnuDrawMantraNeighborMarkers(x, y, z, amount, unused, object, packet);
        break;
    case 9:
        scale = ((MantraPanelAnimation *)object)->frame / 10.0f;
        scale = 1.0f - scale;
        amount = amount * scale;
        mnuDrawMantraNeighborMarkers(x, y, z, amount, unused, object, packet);
        break;
    case 0:
        mnuDrawMantraNeighborMarkers(x, y, z, amount, unused, object, packet);
        break;
    case 1:
        mnuDrawMantraNeighborMarkers(x, y, z, amount, unused, object, packet);
        break;
    }
    return 0;
}

void btlInitPanelBSprites(u32 unused, s32 view) {
    u32 resource;

    resource = mnuAllocateMantraSparkleEmitter(0);
    ((MantraPanelAnimation *)view)->spriteHandle = resource;
    resource = mnuAllocateMantraPanelBurstPool();
    ((MantraPanelAnimation *)view)->burstPool = resource;
}

void btlReleasePanelBSprites(s32 obj) {
    mnuFreeMantraSparkleEmitter(((MantraPanelAnimation *)obj)->spriteHandle);
    mnuReleaseMantraPanelBurstPool((u32 *)((MantraPanelAnimation *)obj)->burstPool);
}

s32 btlDrawPanelB(s32 x, s32 y, u32 z, u32 amount, u32 unused, u32 object, u32 packet) {
    mnuDrawMantraSprite(x, y, 0, amount, 0x77, 0, packet);
    mnuDrawMantraSprite(x, y, 0, amount, 0x87, 0, packet);
    sdfSubmitGsTestOneRegisterPacket(0x30000, packet);
    uiDrawUniformColorRect((x - 0x80) << 4, (y - 0x80) << 3, 0xffffff, 0x1000, 0x800, 0, packet);
    sdfSubmitGsTestOneRegisterPacket(0x3000DL, packet);
    uiDrawActiveSurfaceRegion(packet);
    mnuDrawMantraSprite(x, y, 0, 0x80, 0x91, 0x60, packet);
    sdfDispatchSurfaceWithPreparedTexturePacket(packet);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027E360);

void mnuDrawMantraBackdropWithGsTest(void) {
}

void func_0027FB68(void) {
}

s32 btlDrawPanelC(s32 x, s32 y, u32 z, u32 amount, u32 unused, u32 object, u32 packet) {
    sdfSubmitGsTestOneRegisterPacket(0x30000, packet);
    uiDrawUniformColorRect((x - 0x80) << 4, (y - 0x80) << 3, 0, 0x1000, 0x800, 0, packet);
    sdfSubmitGsTestOneRegisterPacket(0x5100DL, packet);
    mnuDrawMantraSprite(x, y, 0, amount, 0x77, 0, packet);
    mnuDrawMantraSprite(x, y, 0, amount, 0x9A, 0, packet);
    sdfSubmitGsTestOneRegisterPacket(0x30000, packet);
    uiDrawUniformColorRect((x - 0x80) << 4, (y - 0x80) << 3, 0, 0x1000, 0x800, 0, packet);
    sdfSubmitGsTestOneRegisterPacket(0x5100DL, packet);
    return 0;
}

s32 mnuDrawMantraPanelBackdropTransition(s32 x, s32 y, s32 z, s32 amount, s32 unused, u8 *object, s32 packet) {
    f32 scale;

    switch ((((MantraPanelAnimation *)object)->flags >> 19) & 0xF) {
    case 6:
        scale = ((MantraPanelAnimation *)object)->frame * 0.25f;
        amount = amount * scale;
        btlDrawPanelC(x, y, z, amount, unused, (u32)object, packet);
        break;
    case 8:
        scale = ((MantraPanelAnimation *)object)->frame / 10.0f;
        amount = amount * scale;
        btlDrawPanelC(x, y, z, amount, unused, (u32)object, packet);
        break;
    case 7:
        scale = ((MantraPanelAnimation *)object)->frame * 0.25f;
        scale = 1.0f - scale;
        amount = amount * scale;
        btlDrawPanelC(x, y, z, amount, unused, (u32)object, packet);
        break;
    case 9:
        scale = ((MantraPanelAnimation *)object)->frame / 10.0f;
        scale = 1.0f - scale;
        amount = amount * scale;
        btlDrawPanelC(x, y, z, amount, unused, (u32)object, packet);
        break;
    case 0:
        btlDrawPanelC(x, y, z, amount, unused, (u32)object, packet);
        break;
    case 1:
        btlDrawPanelC(x, y, z, amount, unused, (u32)object, packet);
        break;
    }
    return 0;
}

void func_0027FDB8(void) {
}

void func_0027FDC0(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027FDC8);

void mnuResetMantraPanelAnimationStates(u32 unused, s32 view) {
    ((MantraPanelAnimation *)view)->stateA = 0;
    ((MantraPanelAnimation *)view)->stateB = 0;
    ((MantraPanelAnimation *)view)->stateC = 0;
}

void func_002803A0(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002803A8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425C98);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425CA8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425CB8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002805E0);

void func_002817B8(u32 unused, s32 view) {
    ((MantraPanelAnimation *)view)->stateA = 0;
}

void func_002817C0(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002817C8);

extern s32 func_002817C8(s32, s32, s32, s32, s32, u8 *, s32);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425CF8);

s32 mnuDrawFadedMantraSingleCyclePanel(s32 x, s32 y, s32 z, s32 amount, s32 unused, u8 *object, s32 packet) {
    f32 scale;

    switch ((((MantraPanelAnimation *)object)->flags >> 19) & 0xF) {
    case 6:
        scale = ((MantraPanelAnimation *)object)->frame * 0.25f;
        amount = amount * scale;
        func_002817C8(x, y, z, amount, unused, object, packet);
        break;
    case 8:
        scale = ((MantraPanelAnimation *)object)->frame / 10.0f;
        amount = amount * scale;
        func_002817C8(x, y, z, amount, unused, object, packet);
        break;
    case 7:
        scale = ((MantraPanelAnimation *)object)->frame * 0.25f;
        scale = 1.0f - scale;
        amount = amount * scale;
        func_002817C8(x, y, z, amount, unused, object, packet);
        break;
    case 9:
        scale = ((MantraPanelAnimation *)object)->frame / 10.0f;
        scale = 1.0f - scale;
        amount = amount * scale;
        func_002817C8(x, y, z, amount, unused, object, packet);
        break;
    case 0:
        func_002817C8(x, y, z, amount, unused, object, packet);
        break;
    }
    return 0;
}

void mnuInitMantraPanelAccentSprite(u32 unused, s32 view) {
    u32 spriteHandle;

    spriteHandle = mnuAllocateMantraSparkleEmitter(1);
    ((MantraPanelAnimation *)view)->spriteHandle = spriteHandle;
}

void mnuReleaseMantraPanelAccentSprite(u32 obj) {
    mnuFreeMantraSparkleEmitter(((MantraPanelAnimation *)obj)->spriteHandle);
}

u32 mnuDrawMantraPanelAccentSprites(u32 ctx, u32 x, u32 y, u32 direction, u32 unused,
                  u32 sprite, u32 animation) {
    mnuDrawMantraSprite(ctx, x, y, direction, 0x77, 0, animation);
    mnuDrawMantraSprite(ctx, x, y, direction, 0xf7, 0, animation);
    mnuDrawMantraSprite(ctx, x, y, direction, 0xf8, 0, animation);
    mnuDrawMantraSprite(ctx, x, y, direction, 0xf9, 0, animation);
    func_00284508(ctx, x, 0, direction, ((MantraPanelAnimation *)sprite)->spriteHandle, animation);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00281DC0);

void func_00282B50(u32 unused, s32 view) {
    ((MantraPanelAnimation *)view)->stateA = 0;
    ((MantraPanelAnimation *)view)->stateB = 0;
}

void func_00282B60(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00282B68);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425D68);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00283090);

void func_00283F90(u32 unused, s32 view) {
    ((MantraPanelAnimation *)view)->stateA = 0;
    ((MantraPanelAnimation *)view)->stateB = 0;
}

void func_00283FA0(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00283FA8);

extern s32 func_00283FA8(s32, s32, s32, s32, s32, u8 *, s32);

s32 mnuDrawFadedMantraDualCyclePanel(s32 x, s32 y, s32 z, s32 amount, s32 unused, u8 *object, s32 packet) {
    f32 scale;

    switch ((((MantraPanelAnimation *)object)->flags >> 19) & 0xF) {
    case 6:
        scale = ((MantraPanelAnimation *)object)->frame * 0.25f;
        amount = amount * scale;
        func_00283FA8(x, y, z, amount, unused, object, packet);
        break;
    case 8:
        scale = ((MantraPanelAnimation *)object)->frame / 10.0f;
        amount = amount * scale;
        func_00283FA8(x, y, z, amount, unused, object, packet);
        break;
    case 7:
        scale = ((MantraPanelAnimation *)object)->frame * 0.25f;
        scale = 1.0f - scale;
        amount = amount * scale;
        func_00283FA8(x, y, z, amount, unused, object, packet);
        break;
    case 9:
        scale = ((MantraPanelAnimation *)object)->frame / 10.0f;
        scale = 1.0f - scale;
        amount = amount * scale;
        func_00283FA8(x, y, z, amount, unused, object, packet);
        break;
    case 0:
        func_00283FA8(x, y, z, amount, unused, object, packet);
        break;
    }
    return 0;
}

typedef struct MantraSparkle {
    s16 age;
    s16 life;
    u32 flags;
    f32 vx;
    f32 vy;
} MantraSparkle;

typedef struct MantraSparkleEmitter {
    MantraSparkle sparkle[10];
    s16 duration;
    s16 kind;
    s32 count;
} MantraSparkleEmitter;

MantraSparkle *func_00284818(MantraSparkleEmitter *);

u32 mnuAllocateMantraSparkleEmitter(s16 kind) {
    MantraSparkleEmitter *emitter = (MantraSparkleEmitter *)sdfAllocSizeClassBlock(0xA8);
    MantraSparkle *spark;

    memset(emitter, 0, 0xA8);
    emitter->duration = effMiscRandUnitFloat(0) * 20.0f + 5.0f;
    emitter->kind = kind;
    spark = func_00284818(emitter);
    spark->age = spark->life * effMiscRandUnitFloat(0) + 0.0f;
    spark = func_00284818(emitter);
    spark->age = spark->life * effMiscRandUnitFloat(0) + 0.0f;
    spark = func_00284818(emitter);
    spark->age = spark->life * effMiscRandUnitFloat(0) + 0.0f;
    return (u32)emitter;
}


void mnuFreeMantraSparkleEmitter(u32 sprite) {
    if (sprite != 0) {
        sdfReleaseChipBlock(sprite);
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00284508);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00284818);

/* Both burst allocators create this 16-byte pool header followed by 100 slots. */
typedef struct MantraBurstSlot {
    f32 x;
    f32 y;
    union {
        u32 flags;
        struct {
            s16 unk8;
            s16 angleDegrees;
        } polar;
    } w;
    u16 age;
    s16 life;
} MantraBurstSlot;

struct MantraBurstPool {
    u32 allocation;
    MantraBurstSlot *slots;
    s32 count;
    u16 tick;
};

u32 mnuAllocateMantraPanelBurstPool(void) {
    u32 handle = sdfAllocGeneralBlock(0x650);
    u32 block = sdfMemoryGetBlockAddress(handle);
    memset((void *)block, 0, 0x650);
    ((MantraBurstPool *)block)->allocation = handle;
    ((MantraBurstPool *)block)->slots = (MantraBurstSlot *)(block + 0x10);
    ((MantraBurstPool *)block)->count = 0x64;
    return block;
}

void mnuReleaseMantraPanelBurstPool(u32 *obj) {
    if (*obj != 0) {
        sdfReleaseResourceAllocation(*obj);
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00284B70);


MantraBurstSlot *mnuSpawnBurstSlotSmall(MantraBurstPool *pool, s8 wide, s8 side) {
    s32 base = 0x7A;
    s32 i;
    MantraBurstSlot *slot;

    if (wide == 0) {
        base = 0;
    }
    slot = pool->slots;
    for (i = 0; i < pool->count; i++, slot++) {
        if ((slot->w.flags & 1) == 0) {
            memset(slot, 0, 0x10);
            slot->w.flags = ((slot->w.flags | 1) & 0xFFFFFFF9) | ((side & 3) << 1);
            if (wide) {
                slot->w.polar.angleDegrees = (s32)(effMiscRandUnitFloat(0) * 10.0f) * 36;
            } else {
                slot->w.polar.angleDegrees = (s32)(effMiscRandUnitFloat(0) * 20.0f) * 18;
            }
            slot->life = 14;
            if (side != 0) {
                slot->x = (base + 39.0f) * sdfEvaluateCosineViaSinePhaseShift(slot->w.polar.angleDegrees * 0.017453293f);
                slot->y = (base + 39.0f) * sdfSinPoly(slot->w.polar.angleDegrees * 0.017453293f);
            } else {
                slot->x = (base + 19.0f) * sdfEvaluateCosineViaSinePhaseShift(slot->w.polar.angleDegrees * 0.017453293f);
                slot->y = (base + 19.0f) * sdfSinPoly(slot->w.polar.angleDegrees * 0.017453293f);
            }
            return slot;
        }
    }
    return 0;
}

u32 mnuAllocateMantraBackgroundBurstPool(void) {
    u32 handle = sdfAllocGeneralBlock(0x650);
    u32 block = sdfMemoryGetBlockAddress(handle);
    memset((void *)block, 0, 0x650);
    ((MantraBurstPool *)block)->allocation = handle;
    ((MantraBurstPool *)block)->slots = (MantraBurstSlot *)(block + 0x10);
    ((MantraBurstPool *)block)->count = 0x64;
    return block;
}

void mnuReleaseMantraBackgroundBurstPool(u32 *obj) {
    if (*obj != 0) {
        sdfReleaseResourceAllocation(*obj);
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00285148);

MantraBurstSlot *mnuSpawnBurstSlot(MantraBurstPool *pool, s8 wide, s8 side) {
    s32 base = 0x7A;
    s32 i;
    MantraBurstSlot *slot;

    if (wide == 0) {
        base = 100;
    }
    slot = pool->slots;
    for (i = 0; i < pool->count; i++, slot++) {
        if ((slot->w.flags & 1) == 0) {
            memset(slot, 0, 0x10);
            slot->w.flags = ((slot->w.flags | 1) & 0xFFFFFFF9) | ((side & 3) << 1);
            if (wide) {
                slot->w.polar.angleDegrees = (s32)(effMiscRandUnitFloat(0) * 10.0f) * 36;
            } else {
                slot->w.polar.angleDegrees = effMiscRandUnitFloat(0) * 360.0f;
            }
            slot->life = 30;
            if (side != 0) {
                slot->x = (base + 50.0f) * sdfEvaluateCosineViaSinePhaseShift(slot->w.polar.angleDegrees * 0.017453293f);
                slot->y = (base + 25.0f) * sdfSinPoly(slot->w.polar.angleDegrees * 0.017453293f);
            } else {
                slot->x = (base + 100.0f) * sdfEvaluateCosineViaSinePhaseShift(slot->w.polar.angleDegrees * 0.017453293f);
                slot->y = (base + 30.0f) * sdfSinPoly(slot->w.polar.angleDegrees * 0.017453293f);
            }
            return slot;
        }
    }
    return 0;
}

u32 mnuRequestEffectResource(u32 context, u32 config) {
    MantraEffectResource *resource = (MantraEffectResource *)sdfAllocSizeClassBlock(sizeof(MantraEffectResource));
    memset(resource, 0, sizeof(MantraEffectResource));
    effRequestResourceByMode(context, config, 0, &resource->handle);
    return (u32)resource;
}

u8 mnuHasEffectResourceHandle(MantraEffectResource *resource) {
    return resource->handle != 0;
}

void mnuReleaseEffectResource(MantraEffectResource *resource) {
    effDestroyResourceSlotSet(resource->handle);
    sdfReleaseChipBlock(resource);
}

typedef struct MantraSpark {
    s16 x;
    s16 y;
    s16 timer;
    s16 timerMax;
    s16 z;
    s8 phase;
    u8 size;
} MantraSpark;

typedef struct MantraSparkState {
    s32 timer;
    s32 unk4;
    f32 unk8;
    MantraSpark particles[8];
} MantraSparkState;

typedef struct MantraSparkEffectState {
    MantraSparkState sparkle;
    u32 effectHandle;
} MantraSparkEffectState;

void mnuUpdateSparkle(MantraSpark *);
void mnuTickMantraSparkParticles(MantraSparkState *);
s32 func_00285AC0(MantraSparkEffectState *, u32, s32);

s32 func_00285788(MantraSparkEffectState *state, s32 unused, s32 arg2) {
    f32 fraction = (f32)state->sparkle.timer / (f32)state->sparkle.unk4;

    fraction = 1.0f - fraction;
    sdfSinPoly(fraction * 3.14159265f);
    mnuTickMantraSparkParticles(&state->sparkle);
    return func_00285AC0(state, state->effectHandle, arg2);
}

/* Reset the shared timer as needed, then advance all eight spark particles. */
void mnuTickMantraSparkParticles(MantraSparkState *state) {
    MantraSpark *particle;
    s32 value;
    s32 i;

    state->timer = state->timer - 1;
    if (state->timer < 0) {
        value = effMiscRandUnitFloat(0) * 60.0f + 60.0f;
        state->unk4 = value;
        state->timer = value;
        state->unk8 = effMiscRandUnitFloat(0) * 0.20000005f + 0.4f;
    }
    particle = state->particles;
    for (i = 7; i >= 0; i--) {
        mnuUpdateSparkle(particle);
        particle++;
    }
}

void mnuUpdateSparkle(MantraSpark *spark) {
    spark->timer -= 1;
    if (spark->timer < 0) {
        if (spark->phase != 0) {
            spark->phase = 0;
            spark->timer = spark->timerMax = effMiscRandUnitFloat(0) * 30.0f + 1.0f;
        } else if (effMiscRandUnitFloat(0) > 0.7f) {
            spark->timer = spark->timerMax = effMiscRandUnitFloat(0) * 30.0f + 0.0f;
        } else {
            spark->phase = 1;
            spark->timer = spark->timerMax = effMiscRandUnitFloat(0) * 60.0f + 45.0f;
            spark->z = (effMiscRandUnitFloat(0) * 2.0f - 1.0f) * 512.0f + 320.0f;
            spark->size = effMiscRandUnitFloat(0) * 10.0f;
            spark->x = effMiscRandUnitFloat(0) * 512.0f + -256.0f;
            spark->y = effMiscRandUnitFloat(0) * 448.0f + -128.0f;
        }
    }
}


/* Evaluate the active particle's remaining-life sine envelope; discard the result. */
void mnuEvaluateActiveSparkleSine(MantraSpark *object) {
    if (object->phase != 0) {
        sdfSinPoly((1.0f - (f32)object->timer / (f32)object->timerMax) * 3.14159265f);
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00285AC0);

/* 0x28-byte header for a variable-length effect slot pool. */
typedef struct MantraEffectPoolHeader {
    u32 allocation;       /* 0x00 */
    u32 slots;            /* 0x04 */
    u32 count;            /* 0x08 */
    u16 counterC;         /* 0x0C */
    u16 counterE;         /* 0x0E */
    u16 width;            /* 0x10 */
    u16 height;           /* 0x12 */
    u8 pad14[2];
    u16 interval;         /* 0x16 */
    u16 active;           /* 0x18 */
    u16 duration;         /* 0x1A */
    u16 first;            /* 0x1C */
    u16 second;           /* 0x1E */
    u16 delay;            /* 0x20 */
    u8 phase;             /* 0x22 */
    u8 pad23;
    void (*callback)(void); /* 0x24 */
} MantraEffectPoolHeader;

u32 mnuAllocateMantraEffectSlotPool(u32 count) {
    u32 size = count * 12 + 0x28;
    u32 handle = sdfAllocGeneralBlock(size);
    u32 block = sdfMemoryGetBlockAddress(handle);
    memset((void *)block, 0, size);
    ((MantraEffectPoolHeader *)block)->allocation = handle;
    ((MantraEffectPoolHeader *)block)->slots = block + 0x28;
    ((MantraEffectPoolHeader *)block)->count = count;
    ((MantraEffectPoolHeader *)block)->interval = 10;
    ((MantraEffectPoolHeader *)block)->active = 1;
    ((MantraEffectPoolHeader *)block)->duration = 60;
    ((MantraEffectPoolHeader *)block)->phase = 5;
    ((MantraEffectPoolHeader *)block)->delay = 5;
    ((MantraEffectPoolHeader *)block)->width = 0x200;
    ((MantraEffectPoolHeader *)block)->height = 0x1c0;
    ((MantraEffectPoolHeader *)block)->first = 13;
    ((MantraEffectPoolHeader *)block)->second = 9;
    ((MantraEffectPoolHeader *)block)->callback = mnuDrawCellScaledGrid;
    ((MantraEffectPoolHeader *)block)->counterC = 0;
    ((MantraEffectPoolHeader *)block)->counterE = 0;
    return block;
}

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378F0);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378F8);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_00437900);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_00437908);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_00437910);

