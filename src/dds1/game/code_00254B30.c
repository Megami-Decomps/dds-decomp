#include "common.h"

#define MNU_DISPLAY_TEXT_RGB 0xA09DC300
#define MNU_DISPLAY_DIM_TEXT_RGB 0xA09D7D00
#define MNU_DISPLAY_ALPHA_MASK 0xFF
#define MNU_DISPLAY_DIM_ALPHA_SCALE 0.6f
#define MNU_DISPLAY_TEXT_BUFFER_BYTES 0x20
#define MNU_SELECTION_ROW_HEIGHT 26
#define MNU_DISPLAY_FIXED_X_SHIFT 4
#define MNU_DISPLAY_FIXED_Y_SHIFT 3
#define MNU_DISPLAY_RECORD_ID_MASK 0xFFFF
#define MNU_DISPLAY_TEXT_FLAGS 0x80000000
#define MNU_SELECTED_ROW_SPRITE 12
#define MNU_UNSELECTED_ROW_SPRITE 14

extern void dspStartEntry(s32 signal);

extern void func_0024E260(s32, s32, s32, s32, s32, s32);

extern s32 sdfAllocSizeClassBlock(u32);


extern void mnuCallInitWide(s32, s32, s32, s32, s32);

typedef struct {
    u8 pad0[4];
    u16 unitId; /* 0x4: index into the unit-name and unit-sprite tables */
    u8 pad06[0xE];
    u16 level; /* 0x14: party level, displayed with the two-column level format */
} DspEntry;

typedef struct {
    u8 pad0[0xC];
    u16 sceneId; /* 0xC: label lookup and displayed scene identifier */
    u8 pad0E[6];
    s32 state;
} DspScene;

typedef struct {
    DspEntry *entry;   /* 0x0: selected display entry */
    s32 mantraId; /* 0x4: index into the mantra-name table */
} DspSelection;

typedef struct {
    u8 pad0[0x1C];
    s32 *selectedIndex; /* 0x1C */
    s32 entryCount; /* 0x20 */
    u8 pad24[0xC];
    s32 *selectedValue; /* 0x30 */
} DspUnit;

typedef struct {
    u8 pad0[0xC];
    DspUnit *unit; /* 0xC */
} DspWindowContext;

typedef struct {
    u8 pad00[0x1C];
    void *selectedNode; /* 0x1C */
    u8 pad20[0xC];
    void (*drawCallback)();
    s32 *drawValues; /* 0x30 */
} DspMenuList;

typedef struct {
    DspEntry *entry;
    s32 profileId;
    s32 value;
    s32 cap;
} DspProfileProgress;

typedef struct {
    u8 pad00[0x70];
    DspProfileProgress *progress; /* 0x70 */
} DspMenuListNode;

typedef struct {
    s8 spriteIndices[7];
} DspEntrySpriteLookup;

extern DspEntrySpriteLookup D_003BC448[];
extern DspEntrySpriteLookup D_003BC480[];
extern char D_003BC450[];
extern void func_0024E310(s32, s32, s32, s32, s32, s32, s32);
extern void func_003014F0(char *, const char *, ...);
extern s32 itfDrawGlyphChainWithWidthQuery(s32, s32, s32, u32, u8, u32, s32, u32);

/* Draw the selection strip from the last row back to the first. */
void itfDspDrawSelectionStrip(s32 x, s32 y, s32 z, s32 alpha, DspWindowContext *windowContext, s32 drawContext) {
    DspUnit *displayUnit = windowContext->unit;
    s32 selectedIndex = *displayUnit->selectedIndex;
    s32 rowIndex;

    func_0024E260(x, y, z, alpha, 0x56, drawContext);
    rowIndex = displayUnit->entryCount - 1;
    if (rowIndex >= 0) {
        do {
            if (rowIndex == selectedIndex) {
                func_0024E310(x, y + rowIndex * MNU_SELECTION_ROW_HEIGHT, z, alpha, 0x57, MNU_SELECTED_ROW_SPRITE, drawContext);
            } else {
                func_0024E310(x, y + rowIndex * MNU_SELECTION_ROW_HEIGHT, z, alpha, 0x57, MNU_UNSELECTED_ROW_SPRITE, drawContext);
            }
            rowIndex--;
        } while (rowIndex >= 0);
    }
}

/* Store the selected value in the display unit and open its scaled window. */
void itfDspInitSelectedWindow(s32 x, s32 y, s32 z, s32 drawValue, DspWindowContext *windowContext, s32 drawContext) {
    DspUnit *displayUnit = windowContext->unit;

    *displayUnit->selectedValue = drawValue;
    mnuCallInitWide(x << MNU_DISPLAY_FIXED_X_SHIFT, y << MNU_DISPLAY_FIXED_Y_SHIFT, z, (s32)displayUnit, drawContext);
}

/* Draw one party entry, highlighting the currently selected list node. */
void func_00254C68(s32 x, s32 y, s32 layer, DspMenuList *list, DspMenuListNode *node,
                   s32 drawContext) {
    char levelText[MNU_DISPLAY_TEXT_BUFFER_BYTES];
    DspEntrySpriteLookup spriteLookup = D_003BC448[0];
    DspProfileProgress *profileProgress = node->progress;
    s32 alpha = list->drawValues[0];
    u32 textColor;

    if (list->selectedNode == node) {
        func_0024E310(x >> MNU_DISPLAY_FIXED_X_SHIFT, y >> MNU_DISPLAY_FIXED_Y_SHIFT, layer, alpha, 0x58,
                      spriteLookup.spriteIndices[profileProgress->entry->unitId] + 1, drawContext);
        func_0024E310(x >> MNU_DISPLAY_FIXED_X_SHIFT, y >> MNU_DISPLAY_FIXED_Y_SHIFT, layer, alpha, 0x59, 0xB, drawContext);
        func_003014F0(levelText, D_003BC450, profileProgress->entry->level);
        textColor = alpha & MNU_DISPLAY_ALPHA_MASK;
        textColor |= MNU_DISPLAY_TEXT_RGB;
        itfDrawGlyphChainWithWidthQuery((x >> MNU_DISPLAY_FIXED_X_SHIFT) + 0xD5, (y >> MNU_DISPLAY_FIXED_Y_SHIFT) + 0x7D, layer,
                                       textColor,
                                       4, (u32)levelText, 0, drawContext);
    } else {
        func_0024E310(x >> MNU_DISPLAY_FIXED_X_SHIFT, y >> MNU_DISPLAY_FIXED_Y_SHIFT, layer, alpha, 0x58,
                      spriteLookup.spriteIndices[profileProgress->entry->unitId], drawContext);
        func_0024E310(x >> MNU_DISPLAY_FIXED_X_SHIFT, y >> MNU_DISPLAY_FIXED_Y_SHIFT, layer, alpha, 0x59, 0xA, drawContext);
        func_003014F0(levelText, D_003BC450, profileProgress->entry->level);
        textColor = alpha & MNU_DISPLAY_ALPHA_MASK;
        textColor |= MNU_DISPLAY_TEXT_RGB;
        itfDrawGlyphChainWithWidthQuery((x >> MNU_DISPLAY_FIXED_X_SHIFT) + 0xD5, (y >> MNU_DISPLAY_FIXED_Y_SHIFT) + 0x7D, layer,
                                       textColor,
                                       0, (u32)levelText, 0, drawContext);
    }
}

extern void *memset(void *, s32, u32);
extern void func_002CD0D8(u32, s32, void *);
extern void frFontDrawStyledGlyphChainAndMeasure(s32, s32, s32, u32, s32, void *, u32, s32);

/* Fetch the low-16-bit display record ID and draw its text with the supplied attributes and fixed RGB. */
void itfDspDrawIndexedRecord(s32 x, s32 y, s32 layer, u32 textAttributes, u32 packedEntryId, s32 drawContext) {
    u32 textColor = textAttributes | MNU_DISPLAY_TEXT_RGB;
    u32 recordId = packedEntryId & MNU_DISPLAY_RECORD_ID_MASK;
    u8 recordText[MNU_DISPLAY_TEXT_BUFFER_BYTES];

    memset(recordText, 0, MNU_DISPLAY_TEXT_BUFFER_BYTES);
    func_002CD0D8(recordId, 1, recordText);
    frFontDrawStyledGlyphChainAndMeasure(x + 0x35, y + 0x136, layer, textColor, 4, recordText, MNU_DISPLAY_TEXT_FLAGS, drawContext);
}

extern char D_003BC458[];
extern char D_003BC460[];
extern char D_003BC468[];
/* Draw the placeholder sprites and dim text. Keep both native formatting calls:
 * the second replaces the first call's buffer contents before drawing. */
void mnuDrawDisplayPlaceholder(s32 x, s32 y, s32 layer, s32 alpha, s32 drawContext) {
    u32 textColor = (u32)((f32)alpha * MNU_DISPLAY_DIM_ALPHA_SCALE) | MNU_DISPLAY_DIM_TEXT_RGB;
    char placeholderText[MNU_DISPLAY_TEXT_BUFFER_BYTES];

    func_0024E260(x, y, layer, alpha, 0x21, drawContext);
    func_0024E260(x, y, layer, alpha, 0x11, drawContext);
    func_003014F0(placeholderText, "%s %s %s", D_003BC458, D_003BC458, D_003BC458);
    func_003014F0(placeholderText, D_003BC460);
    frFontDrawStyledGlyphChainAndMeasure(x + 0xB3, y + 0x171, layer, textColor,
                                      0, placeholderText, MNU_DISPLAY_TEXT_FLAGS, drawContext);
}

/* Draw a formatted integer value with the same dim sprite/text treatment as the placeholder. */
void func_00255010(s32 x, s32 y, s32 layer, s32 alpha, s32 displayValue, s32 drawContext) {
    u32 textColor = (u32)((f32)alpha * MNU_DISPLAY_DIM_ALPHA_SCALE) | MNU_DISPLAY_DIM_TEXT_RGB;
    char valueText[MNU_DISPLAY_TEXT_BUFFER_BYTES];

    func_0024E260(x, y, layer, alpha, 0x21, drawContext);
    func_0024E260(x, y, layer, alpha, 0x11, drawContext);
    func_003014F0(valueText, D_003BC468, displayValue);
    itfDrawGlyphChainWithWidthQuery(x + 0xB3, y + 0x173, layer, textColor,
                                  0, (u32)valueText, MNU_DISPLAY_TEXT_FLAGS, drawContext);
}

typedef struct MnuSpritePlacement {
    s16 resourceIndex;
    s16 spriteIndex;
    s16 x;
    s16 y;
} MnuSpritePlacement;

extern MnuSpritePlacement D_0036B510[];
extern s32 D_0036C698[];
extern void func_002BF4E0(s32, s32, s32, u32, s32, s32, s32, s32);

void mnuDrawMantraPanelSprite(s32 x, s32 y, s32 z, s32 alpha) {
    s32 resource = D_0036C698[D_0036B510[41].resourceIndex];

    func_002BF4E0((x + D_0036B510[41].x) << 4,
                  (y + D_0036B510[41].y) << 3, z,
                  (u32)((f32)(alpha << 8) * 0.0078125f), 0,
                  resource,
                  D_0036B510[41].spriteIndex, 0x53);
}

INCLUDE_ASM(const s32, "game/code_00254B30", mnuDrawMantraCostCounter);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255368);

/* Encoded name-table rows; keep byte storage and the retail table strides. */
typedef struct DspUnitName {
    u8 encodedText[17];
} DspUnitName;

typedef struct DspMantraName {
    u8 encodedText[19];
} DspMantraName;

extern DspUnitName *D_003BAA70;
extern DspMantraName *D_003BAA78;
extern s32 fldGetSceneMetadataNode();
extern void evtCopyEntryStringToActiveWindow(s32, void *);
extern s32 mnuGetMantraSourceValue(s32);
extern void evtSetMessageWindowOptionWhenOpen(s32);
extern void dspStartEntry(s32);
extern void evtStoreValueAndCaptureWindowPanelValue(s32);

/* Populate the unit and both mantra labels, plus the selected mantra's cost. */
void itfDspPopulatePrimaryLabels(void) {
    DspSelection *selection = (DspSelection *)mnuGetSelectedNodeValue();
    DspScene *scene = (DspScene *)fldGetSceneMetadataNode();
    char text[16];

    evtCopyEntryStringToActiveWindow(0, D_003BAA70[selection->entry->unitId].encodedText);
    evtCopyEntryStringToActiveWindow(1, D_003BAA78[selection->mantraId].encodedText);
    evtCopyEntryStringToActiveWindow(2, D_003BAA78[scene->sceneId].encodedText);
    func_003014F0(text, D_003BC468, mnuGetMantraSourceValue(scene->sceneId));
    evtCopyEntryStringToActiveWindow(3, text);
    evtSetMessageWindowOptionWhenOpen(0);
    dspStartEntry(0);
    evtStoreValueAndCaptureWindowPanelValue(8);
}

/* Populate the same menu labels, selecting the alternate display signal. */
void itfDspPopulateAlternateLabels(void) {
    DspSelection *selection = (DspSelection *)mnuGetSelectedNodeValue();
    DspScene *scene = (DspScene *)fldGetSceneMetadataNode();
    char text[16];

    evtCopyEntryStringToActiveWindow(0, D_003BAA70[selection->entry->unitId].encodedText);
    evtCopyEntryStringToActiveWindow(1, D_003BAA78[selection->mantraId].encodedText);
    evtCopyEntryStringToActiveWindow(2, D_003BAA78[scene->sceneId].encodedText);
    func_003014F0(text, D_003BC468, mnuGetMantraSourceValue(scene->sceneId));
    evtCopyEntryStringToActiveWindow(3, text);
    evtSetMessageWindowOptionWhenOpen(0);
    dspStartEntry(1);
    evtStoreValueAndCaptureWindowPanelValue(8);
}

/* Populate menu labels for the third display signal. */
void itfDspPopulateThirdLabels(void) {
    DspSelection *selection = (DspSelection *)mnuGetSelectedNodeValue();
    DspScene *scene = (DspScene *)fldGetSceneMetadataNode();
    char text[16];

    evtCopyEntryStringToActiveWindow(0, D_003BAA70[selection->entry->unitId].encodedText);
    evtCopyEntryStringToActiveWindow(1, D_003BAA78[selection->mantraId].encodedText);
    evtCopyEntryStringToActiveWindow(2, D_003BAA78[scene->sceneId].encodedText);
    func_003014F0(text, D_003BC468, mnuGetMantraSourceValue(scene->sceneId));
    evtCopyEntryStringToActiveWindow(3, text);
    dspStartEntry(2);
}

void itfDspSignalA(void) {
    dspStartEntry(3);
}

void itfDspSignalB(void) {
    dspStartEntry(4);
}

void itfDspSignalC(void) {
    dspStartEntry(5);
}

void itfDspSignalD(void) {
    dspStartEntry(6);
}

void itfDspSignalE(void) {
    dspStartEntry(7);
}

extern f32 effMiscRandUnitFloat(s32);



typedef struct DspParticle {
    s16 x;         /* 0x00 */
    s16 y;         /* 0x02 */
    s16 timer;     /* 0x04 */
    s16 timerMax;  /* 0x06 */
    s16 z;         /* 0x08 */
    s8 phase;      /* 0x0A */
    u8 size;       /* 0x0B */
} DspParticle;

void mnuUpdateSparkle(DspParticle *spark) {
    spark->timer = spark->timer - 1;
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



typedef struct {
    s32 countdown;
    s32 period;
    f32 strength;
    DspParticle particles[8];
} DspParticleState;

void mnuTickMantraSparkParticles(DspParticleState *state) {
    DspParticle *particle;
    s32 duration;
    s32 i;

    state->countdown = state->countdown - 1;
    if (state->countdown < 0) {
        duration = effMiscRandUnitFloat(0) * 60.0f + 60.0f;
        state->period = duration;
        state->countdown = duration;
        state->strength = effMiscRandUnitFloat(0) * 0.20000005f + 0.4f;
    }
    /* Advance every particle, including those waiting for their next phase. */
    particle = state->particles;
    for (i = 7; i >= 0; i--) {
        mnuUpdateSparkle(particle);
        particle++;
    }
}

extern f32 sdfSinPoly(f32);
extern void func_0024E1C8(s32, s32, s32, s32, s32, s32, s32, s32);

void mnuDrawMantraSparkParticle(const DspParticle *particle, s32 sprite, s32 surfaceIndex) {
    f32 fraction;
    s32 x;
    s32 y;
    s32 alpha;

    if (particle->phase == 0) {
        return;
    }
    fraction = 1.0f - (f32)particle->timer / particle->timerMax;
    x = (s32)(particle->x + fraction * particle->z);
    y = particle->y;
    alpha = (s32)(sdfSinPoly(fraction * 3.14159265f) * 25.6f);
    func_0024E1C8(x, y, 1, alpha, sprite, 0x2D, 0x20, surfaceIndex);
}

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255B78);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255D00);

INCLUDE_ASM(const s32, "game/code_00254B30", func_00255E08);

void itfDspDrawMarksA(s32 scale, s32 context) {
    func_0024E260(0, 0, 0, scale, 0x3A, context);
    func_0024E260(0, 0, 0, scale, 0x38, context);
    func_0024E260(0, 0, 0, scale, 0x39, context);
}

void itfDspDrawMarksB(s32 scale, s32 context) {
    func_0024E260(0, 0, 0, scale, 0xF, context);
    func_0024E260(0, 0, 0, scale, 0x10, context);
    func_0024E260(0, 0, 0, scale, 0x12, context);
}

typedef struct DspProfileSelection {
    u32 unit;
    s32 profileId;
} DspProfileSelection;

extern u32 prfGetCapValue(u16);
extern u32 ptyGetProfileRecordValue(u32, u16);
extern u32 func_00250758(u16);

u32 mnuGetMantraDisplayFlags(DspScene *entry, DspProfileSelection *target) {
    u32 flags = 0;
    u32 cap;

    if (entry->sceneId == target->profileId) {
        flags |= 1;
    }
    cap = prfGetCapValue(entry->sceneId);
    if (cap == ptyGetProfileRecordValue(target->unit, entry->sceneId)) {
        flags |= 2;
    }
    if (entry->state == 1) {
        flags |= 4;
    } else if (entry->state == 2) {
        flags |= 8;
    } else if ((func_00250758(entry->sceneId) & 2) != 0) {
        flags |= 0x20;
    } else {
        flags |= 0x10;
    }
    return flags;
}

void itfDspDrawStrip(s32 x, s32 y, s32 layer, s32 scale, s32 context) {
    func_0024E260(x, y, layer, scale, 4, context);
    func_0024E260(x, y, layer, scale, 5, context);
    func_0024E260(x, y, layer, scale, 0xA, context);
    func_0024E260(x, y, layer, scale, 0xB, context);
    func_0024E260(x, y, layer, scale, 0xC, context);
    func_0024E260(x, y, layer, scale, 0xD, context);
}

typedef struct DspUnitSpriteLookup {
    s8 spriteIndices[7];
} DspUnitSpriteLookup;

extern DspUnitSpriteLookup D_003BC478[];
extern DspUnitSpriteLookup D_003BC470[];
extern char D_003BC450[];
extern s32 frFontQueueFlaggedGlyphAndMeasure(s32, s32, s32, u32, u8, u32, s32, u32);

/* Draw the unit-specific display sprite and its two-column numeric level. */
void func_002561A0(s32 x, s32 y, s32 layer, DspEntry *entry, s32 scale, s32 context) {
    DspUnitSpriteLookup lookup = D_003BC470[0];
    char text[0x10];
    u32 textColor = scale | 0xA09DC300;

    func_0024E260(x, y, layer, scale, lookup.spriteIndices[entry->unitId], context);
    func_003014F0(text, D_003BC450, entry->level);
    frFontQueueFlaggedGlyphAndMeasure(x + 0x1BB, y + 0x173, layer, textColor,
                                     4, (u32)text, 0, context);
}

/* Select the unit's display sprite; retain the complete signed-byte table copy. */
void mnuDrawDisplayEntrySpriteFromLookup(s32 x, s32 y, s32 layer, DspEntry *entry, s32 scale, s32 context) {
    DspUnitSpriteLookup lookup = D_003BC478[0];

    func_0024E260(x, y, layer, scale, lookup.spriteIndices[entry->unitId], context);
}

/* Draw both layers of a display entry and queue its formatted value. */
void func_002562E8(s32 x, s32 y, s32 layer, DspEntry *entry, s32 scale, s32 context) {
    DspEntrySpriteLookup lookup = D_003BC480[0];
    char text[0x10];
    u32 tag = scale | 0xA09DC300;

    func_0024E260(x, y, layer, scale, lookup.spriteIndices[entry->unitId], context);
    func_0024E260(x, y, layer, scale, lookup.spriteIndices[entry->unitId] + 0x14, context);
    func_003014F0(text, D_003BC450, entry->level);
    frFontQueueFlaggedGlyphAndMeasure(x + 0x1BB, y + 0x173, layer, tag,
                                     4, (u32)text, 0, context);
}

extern f32 sdfSinPoly(f32);

s32 mnuDrawPulsingDisplaySprites(s32 frame, s32 scale, s32 context) {
    f32 wave = (f32)frame / 60.0f;
    f32 baseScale = (f32)scale;
    s32 drawScale;

    wave = sdfSinPoly(wave * 3.1415926f);

    func_0024E260(0, 0, 0, scale, 0x1F, context);
    drawScale = (s32)(baseScale * (wave * 0.5f + 0.5f));
    func_0024E260(0, 0, 0, drawScale, 0x16, context);
    func_0024E260(0, 0, 0, drawScale, 0x17, context);
    drawScale = (s32)(baseScale * 0.5f);
    func_0024E260(0, 0, 0, drawScale, 0x18, context);
    func_0024E260(0, 0, 0, drawScale, 0x19, context);
    if (frame < 60) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00254B30", func_00256540);

typedef struct DspListNode {
    u8 pad00[0x10];
    struct DspListNode *next; /* 0x10 */
} DspListNode;

typedef struct {
    s32 frameCount;        /* 0x00 */
    u32 pad04;            /* 0x04 */
    DspListNode *first;   /* 0x08 */
} DspListHead;

DspListNode *mnuAllocateDisplayListNode(void) {
    DspListNode *node = (DspListNode *)sdfAllocSizeClassBlock(0x14);

    memset(node, 0, 0x14);
    return node;
}

DspListNode *mnuAppendDisplayListNode(DspListHead *head) {
    DspListNode *node = head->first;
    if (node == NULL) {
        node = mnuAllocateDisplayListNode();
        head->first = node;
    } else {
        while (node->next != NULL) {
            node = node->next;
        }
        node->next = mnuAllocateDisplayListNode();
        node = node->next;
    }
    return node;
}

DspListNode *mnuReleaseDisplayListNodeAndGetNext(DspListNode *node) {
    DspListNode *next;

    next = node->next;
    sdfReleaseChipBlock();
    return next;
}

void mnuReleaseDisplayListNodes(DspListHead *head) {
    DspListNode *node = head->first;

    while (node != NULL) {
        node = mnuReleaseDisplayListNodeAndGetNext(node);
    }
}

extern s32 func_00256540(DspListNode *, s32, s32);

/* Advance the display list's timer and release completed nodes in order. */
s32 itfAdvanceDisplayList(DspListHead *head, s32 scale, s32 context) {
    s32 *counter = &head->frameCount;
    DspListNode *node = head->first;
    s32 index = 0;

    if (mnuDrawPulsingDisplaySprites(*counter, scale, context) != 0) {
        *counter = 0;
    } else {
        *counter = *counter + 1;
    }
    if (node == NULL) {
        return 1;
    }
    do {
        s32 completed = func_00256540(node, index, context);

        index++;
        if (completed != 0) {
            node = mnuReleaseDisplayListNodeAndGetNext(node);
            head->first = node;
        } else {
            node = node->next;
        }
    } while (node != NULL);
    return 0;
}

void itfDspDrawScaledA(s32 unused, s32 scale, s32 context) {
    func_0024E260(0, 0, 0, scale, 0x16, context);
    func_0024E260(0, 0, 0, scale * 0.5f, 0x18, context);
}

void itfDspDrawScaledB(s32 unused, s32 scale, s32 context) {
    func_0024E260(0, 0, 0, scale, 0x17, context);
    func_0024E260(0, 0, 0, scale * 0.5f, 0x19, context);
}

void itfDspDrawPair(s32 x, s32 y, s32 layer, s32 scale, s32 context) {
    func_0024E260(x, y, layer, scale, 0x14, context);
    func_0024E260(x, y, layer, scale, 0x15, context);
}

void func_00256E88(void) {
}

INCLUDE_ASM(const s32, "game/code_00254B30", func_00256E90);

extern s32 mnuGetSelectedNodeValue(void);

typedef struct {
    u8 pad0[4];
    DspScene *entry; /* 0x4: entry tested by mnuGetMantraDisplayFlags */
} DspEntryLink;

typedef struct {
    u8 pad0[8];
    DspEntryLink *link; /* 0x8 */
} DspEntryContainer;

typedef struct {
    u8 pad0[0x484];
    DspEntryContainer *entries; /* 0x484 */
} DspDisplayObject;

void mnuChooseDisplaySpriteKindFromEntryFlags(DspDisplayObject *obj, s32 scale, s32 context) {
    DspProfileSelection *target = (DspProfileSelection *)mnuGetSelectedNodeValue();
    DspScene *entry = obj->entries->link->entry;
    u32 flags;
    s32 kind;

    if (entry != NULL) {
        flags = mnuGetMantraDisplayFlags(entry, target);
        if (flags & 1) {
            kind = 9;
        } else if (flags & 2) {
            kind = 8;
        } else if (flags & 4) {
            kind = 6;
        } else {
            kind = 7;
        }
        func_0024E260(0, 0, 1, scale, kind, context);
    }
}

INCLUDE_SDATA(const s32, "game/code_00254B30", D_003BC448);

INCLUDE_SDATA(const s32, "game/code_00254B30", D_003BC450);

INCLUDE_SDATA(const s32, "game/code_00254B30", D_003BC458);

INCLUDE_SDATA(const s32, "game/code_00254B30", D_003BC460);

INCLUDE_SDATA(const s32, "game/code_00254B30", D_003BC468);

INCLUDE_SDATA(const s32, "game/code_00254B30", D_003BC470);

INCLUDE_SDATA(const s32, "game/code_00254B30", D_003BC478);

INCLUDE_SDATA(const s32, "game/code_00254B30", D_003BC480);

