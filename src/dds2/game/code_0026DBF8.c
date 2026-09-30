#include "common.h"
extern char D_00425118[];
extern char D_004378A8[]; /* "%d" */
extern char D_004378B0[]; /* "---" */
extern s32 func_0026E470(u16);
extern f32 sdfSinPoly(f32);
extern void func_00311DB0(s32, s32, s32, s32, s32, char *, s32, s32);

extern u32 func_00284AE0(void);

extern u32 func_002843C0(s16);

extern s32 mnuFindMantraDrawItemByKind(u32, u32);

extern s32 func_00328D68(u32);

void func_003297C8(u32 sprite);

void func_002844E8(u32 sprite);

void func_00284B48(u32 *obj);
u32 func_002850B8(void);
extern void func_00286270();
u32 mnuQueueNextMantraSelection(u32);
u32 func_0026F778(u32);
extern u32 mnuRegisterMantraDrawItem(u32, u32, void (*)(), void (*)(), u32 (*)(), void (*)(), s16, s16, u32);
extern s32 mnuUpdateMantraFadeA();
extern u32 mnuDrawMantraIconList(u32, u32);
typedef struct MantraNodePos {
    u8 pad00[4];
    s16 x;
    s16 y;
} MantraNodePos;
extern MantraNodePos *func_0026CF70(s16);
extern MantraNodePos *func_0026D098(s16);
extern s32 func_00314B78(s32);
extern u32 mnuGetSelectedNodeValue(u8 *);
extern u32 mnuAllocateMantraIconPool(u32);
extern u32 mnuCreateMantraIconListA();
extern u32 mnuCreateMantraIconListB();
extern u32 mnuCreateMantraIconListC();
extern void mnuReleaseMantraFadeData(s32);
s32 func_00271368();
void func_00271510();
u32 func_002712E0();
void mnuMantraSetupSlot(u32);
void func_00271348();
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
extern s32 func_00270210();
extern void func_00270848();
extern u32 mnuInitMantraBackgroundDraw();
extern void mnuReleaseMantraBackgroundDraw();
extern s32 func_00270DD8();
extern s32 mnuDrawMantraBackgroundMaskPulse();
extern u32 mnuInitMantraBackgroundMaskDraw();
extern void mnuReleaseMantraBackgroundMaskDraw();
extern s32 func_00272DA8();
extern void func_00272F08();
extern u32 mnuInitMantraTitleDraw();
extern void mnuReleaseMantraTitleDraw();
extern s32 func_00273668();
extern void func_00273828();
extern u32 mnuInitMantraInfoDraw();
extern void mnuReleaseMantraInfoDraw();
extern s32 func_002741D0();
extern s32 mnuDrawMantraGauge();
extern u32 mnuInitMantraGaugeData();
extern void mnuReleaseMantraGaugeData();
extern s32 func_00274EA8();
extern void func_00274FF8();
extern u32 mnuCreateTypeOneRecord(void);
extern void func_00274E88();
extern u32 D_00453D00[12];
extern u8 *D_00435DD0;

typedef struct MantraMenuValues {
    u8 pad00[0x3C];
    s32 panelValue; /* 0x3C: value animated by both numeric counters */
} MantraMenuValues;

typedef struct MantraPanelSpriteView {
    u8 pad00[0x1E];
    s16 animationStep; /* 0x1E: signed step of panel opacity/size transition */
    u8 stateA;         /* 0x20 */
    u8 stateB;         /* 0x21 */
    u8 stateC;         /* 0x22 */
    u8 pad23;
    u32 spriteHandle;  /* 0x24 */
    u32 burstPool;     /* 0x28: paired with spriteHandle in panel B */
} MantraPanelSpriteView;

typedef struct MenuRecord {
    u16 type;
    u16 flags;
    u8 unk_04[8];
    u32 modeC;         /* 0x0C: initialized to 15 for the type-one record */
    u32 unk_10;
    u8 unk_14[4];
} MenuRecord;

typedef struct MantraDisplayNode {
    u8 pad00[4];
    u32 fromValue;
    u32 toValue;
    u16 transitionKind;
    u8 pad0E[2];
    struct MantraDisplayNode *next;
} MantraDisplayNode;

typedef struct MantraDrawItem {
    u32 kind;
    u32 flags;
    u8 pad8[4];
    void (*release)(void);
    u8 pad10[8];
    s16 timer0;        /* 0x18 */
    s16 timer1;        /* 0x1A */
    u32 arg;           /* 0x1C */
    void *data;
} MantraDrawItem;

typedef struct MantraDrawPool {
    u32 handle;
    MantraDrawItem *items;
    s32 count;
} MantraDrawPool;

/* MantraDrawItem seen through its flag word: bits 12..19 hold the category. */
typedef struct MantraDrawItemBits {
    u32 kind;
    u32 low : 12;
    u32 category : 8;
    u32 high : 12;
} MantraDrawItemBits;

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

typedef struct MantraPanelFade {
    u16 state;       /* 0x00 */
    u16 pad02;
    u16 elapsed;     /* 0x04 */
    u16 cycle;       /* 0x06 */
    f32 scale;       /* 0x08 */
    s32 delay;       /* 0x0C */
} MantraPanelFade;

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
    /* 0x0C */ u32 flagsC;
    /* 0x10 */ u32 flags10;
    /* 0x14 */ u32 armed : 1;
    u32 countdown : 31;
    /* 0x18 */ s32 clock;
    /* 0x1C */ u16 queuedState;
    /* 0x1E */ u16 delay;
    /* 0x20 */ u32 queuedFlags;
} MantraFadeState;

typedef struct MantraBackgroundState {
    u16 state;            /* 0x00 */
    u16 pad02;
    u32 transitionWord;   /* 0x04: low nibble selects frame */
    u8 pad08[8];
    u32 enabled;          /* 0x10 */
    u32 selectedValue;    /* 0x14 */
    u32 iconPool;         /* 0x18 */
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
    u32 unk0C;      /* 0x0C */
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
extern u32 func_00101958(void);
extern u32 func_002C7FF0(const char *);
extern void *kwlnTaskCreate(const char *, s32, s32, s32, s32 (*)(void),
                            void (*)(), void *);
s32 mnuLoadMantraSpriteTask(void);
extern void func_0026E788(u32, u32, u32, u32, u32, u32, u32);
void func_00284508(u32, u32, u32, u32, u32, u32);
extern char D_004250D8[];
extern s32 func_002748D0();
extern void func_00274A70();
extern u32 mnuInitMantraUnitPanelDraw();
extern void mnuReleaseMantraUnitPanelDraw();
void mnuStorePanelEntry(u32, u32);
void effDestroyResourceSlotSet(u32);
extern u32 func_003292A8(u32);
extern u32 sdfMemoryGetBlockAddress(u32);

s32 func_0026DBF8(void) {
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

extern char D_004378A0[]; /* "%7d" */

/* Interpolate the panel's numeric value over 20 updates, playing a sound on each changed step. */
void mnuDrawAnimatedMantraValue(u32 x, u32 y, u32 depth, u32 fade, MantraCountState *state, u32 drawArg) {
    char text[16];
    u32 flags = fade | 0xA09DC300;

    func_0026E788(x, y, depth, fade, 0x2E, 0, drawArg);
    if (((MantraMenuValues *)D_00435DD0)->panelValue != state->shown) {
        s32 steps = 20;

        sndSetSequenceVolumePan(0x13, 0x7F, 0x3F);
        state->step++;
        func_0035C860(text, D_004378A0, state->shown + (((MantraMenuValues *)D_00435DD0)->panelValue - state->shown) * state->step / steps);
        if (state->step == steps) {
            state->shown = ((MantraMenuValues *)D_00435DD0)->panelValue;
            state->step = 0;
        }
    } else {
        func_0035C860(text, D_004378A0, ((MantraMenuValues *)D_00435DD0)->panelValue);
    }
    func_00311D00(x + 0x193, y + 0x26, depth, flags, 0, text, 0, drawArg);
}

/* Draw a label for a nonzero entry, otherwise use the empty-entry panel art. */
void mnuDrawOptionalMantraLabel(u32 x, u32 y, u32 depth, u32 fade, u16 entryId, u32 drawArg) {
    u8 buffer[0x20];
    u32 flags = (fade & 0xFF) | 0xA09DC300;

    func_0026E788(x, y, depth, fade, 0x4C, 0, drawArg);
    if (entryId != 0) {
        memset(buffer, 0, 0x20);
        func_00314500(entryId, 1, buffer);
        func_00311C50(x + 0x27, y + 0x146, depth, flags, 4, buffer, 0x101, drawArg);
        func_0026E788(x, y, depth, fade, 0x50, 0, drawArg);
    } else {
        func_0026E788(x, y, depth, fade, 0x54, 0, drawArg);
    }
}

extern u8 *func_003164C0(void);

/* Render a panel icon when it has an ID, or the corresponding empty art. */
void mnuDrawMantraLabelA(u32 x, u32 y, u32 depth, s32 fade, u32 iconId, u32 drawArg) {
    u8 *handle = func_003164C0();
    u32 flags = (u32)(fade * 0.6f) | 0xA09D7D00;

    func_0026E788(x, y, depth, fade, 0x4D, 0, drawArg);
    if (iconId != 0) {
        func_00311E60(x + 0x2A, y + 0x160, depth, flags, 0, iconId & 0xFFFF, handle, 0, 0, drawArg);
    } else {
        func_0026E788(x, y, depth, fade, 0x55, 0, drawArg);
    }
}

extern u32 func_003151D0(u16);

/* Print the entry's count and draw one repeated marker per count unit. */
void mnuDrawMantraDigitRow(u32 x, u32 y, u32 depth, u32 fade, u32 entryId, u32 drawArg) {
    char text[0x20];
    u32 flags = fade | 0xA09D7D00;
    u32 count;
    u32 i;

    func_0026E788(x, y, depth, fade, 0x4E, 0, drawArg);
    if (entryId != 0) {
        count = func_003151D0(entryId);
        if (count != 0) {
            func_0035C860(text, D_004378A8, count);
            func_00311DB0(x + 0x82, y + 0x17C, depth, flags, 4, text, 0, drawArg);
            for (i = 0; i < count; i++) {
                func_0026E788(x, y, depth, fade, 0x51, 0, drawArg);
                x += 0xF;
            }
        }
    } else {
        func_0026E788(x, y, depth, fade, 0x56, 0, drawArg);
    }
}

void mnuDrawMantraCostCounter(u32 x, u32 y, u32 depth, s32 fade, u32 entryId, u32 showCost, u32 drawArg) {
    char text[0x20];
    u32 flags = fade | 0xA09DC300;

    func_0026E788(x, y, depth, fade, 0x4F, 0, drawArg);
    if (entryId != 0) {
        func_0026E788(x, y, depth, fade, 0x53, 0, drawArg);
        if (showCost != 0) {
            func_0035C860(text, D_004378A8, func_0026E470(entryId));
            func_00311DB0(x + 0x9A, y + 0x190, depth, flags, 4, text, 0, drawArg);
        } else {
            func_0035C860(text, D_004378B0);
            func_00311C50(x + 0xA2, y + 0x18C, depth, (u32)(fade * 0.5f) | 0xA09D7D00, 0, text, 0x80000000, drawArg);
        }
    } else {
        func_0026E788(x, y, depth, fade, 0x57, 0, drawArg);
    }
}

extern MantraSourceEntry *func_003162D8(u16);

/* Pick the value of the first active slot, preferring slot 0. */
s32 func_0026E470(u16 index) {
    s32 result = 0;
    MnuSourceEntrySlots *entry = (MnuSourceEntrySlots *)func_003162D8(index);
    s32 slot = 0;

    if (entry->flags & 0x20) {
        slot = 0;
    } else if (entry->slot[0].unk0C & 0x20) {
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
            D_00453D00[i] = values[i];
        }
    }
}

void mnuReleaseFirstMantraSpriteSlots(void) {
    s32 i;
    u32 *slot = D_00453D00;
    for (i = 1; i >= 0; i--, slot++) {
        if (*slot != 0) {
            effDestroyResourceSlotSet(*slot);
        }
        *slot = 0;
    }
}

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378A0);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378A8);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378B0);

void mnuReleaseMiddleMantraSpriteSlots(void) {
    s32 slots[2] = {2, 3};
    u32 i;

    for (i = 0; i < 2; i++) {
        if (D_00453D00[slots[i]] != 0) {
            effDestroyResourceSlotSet(D_00453D00[slots[i]]);
        }
        D_00453D00[slots[i]] = 0;
    }
}

void mnuStartMantraSpriteLoad(void) {
    if (D_00453D00[4] == 0) {
        u32 resource = func_002C7FF0("/facility/spr/mantra/sprite_a.lb");
        kwlnTaskCreate(D_004250D8, 0x402, 1, 1, mnuLoadMantraSpriteTask, 0,
                       (void *)resource);
    }
}

s32 mnuHasMantraSpriteTaskFinished(void) {
    if (D_00453D00[4] != 0) {
        return 1;
    }
    return func_00101740(D_004250D8) == 0;
}

void mnuReleaseMantraSpriteSlots(void) {
    s32 i;
    u32 *slot = D_00453D00;
    for (i = 11; i >= 0; i--, slot++) {
        if (*slot != 0) {
            effDestroyResourceSlotSet(*slot);
        }
        *slot = 0;
    }
}

s32 mnuLoadMantraSpriteTask(void) {
    MantraFileRequest *request = (MantraFileRequest *)func_00101958();
    s32 result = fileRequestIsReady(request);
    if (result != 0) {
        MantraFileEntry *entry = request->entries;
        s32 i;
        for (i = 4; entry != 0; entry = entry->next, i++) {
            if (entry->kind == 1) {
                D_00453D00[i] = func_00305148(entry->handle, 0);
                func_003297C8((u32)entry->handle);
            }
        }
        func_002C7CE8(request);
        result = -1;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026E788);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026E998);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026EBA8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026EDB0);

s32 mnuDrawMantraSineFade(u32 x, u32 y, u32 depth, s32 frame, s32 amount, u32 drawArg) {
    f32 wave = (f32)frame / 60.0f;
    s32 shown;

    wave = sdfSinPoly(wave * 3.14159265f);
    func_0026E788(x, y, depth, amount, 0x25, 0, drawArg);
    shown = (f32)amount * (wave * 0.5f + 0.5f);
    func_0026E788(0, 0, depth, shown, 0x26, 0, drawArg);
    func_0026E788(0, 0, depth, shown, 0x28, 0, drawArg);
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
INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004250D8);

void mnuDrawMantraCostBadge(s32 x, s32 y, s32 depth, u8 *record, s32 fade, s32 drawArg) {
    char buttons[9] = {0, 'n', 's', 'o', 'q', 'p', 'r', 't', 'u'};
    char text[8];
    s32 drawFlags = fade | 0xA09DC300;

    func_0026E788(x, y, depth, fade, buttons[((MantraCostRecord *)record)->iconIndex], 0, drawArg);
    func_0026E788(x, y, depth, fade, 0x76, 0, drawArg);
    memset(text, 0, sizeof(text));
    func_0035C860(text, D_004378A8, ((MantraCostRecord *)record)->cost);
    if (strlen(text) > 1) {
        func_00311DB0(x + 0x1E0, y + 0x173, depth, drawFlags, 0, text, 0, drawArg);
    } else {
        func_00311DB0(x + 0x1E4, y + 0x173, depth, drawFlags, 0, text, 0, drawArg);
    }
}

void mnuDrawMantraCostIcon(u32 unused1, u32 unused2, u32 third, u32 record,
                   u32 position, u32 packet) {
    char markers[9] = { '\0', '/', '4', '0', '2', '1', '3', '5', '6' };
    u16 index = ((MantraCostRecord *)record)->iconIndex;
    func_0026E788(0, 0, third, position, markers[index], 0, packet);
}

void mnuDrawMantraCostIconOffset(u32 unused1, u32 unused2, u32 third, u32 record,
                   u32 position, u32 packet) {
    char markers[9] = { '\0', '/', '4', '0', '2', '1', '3', '5', '6' };
    u16 index = ((MantraCostRecord *)record)->iconIndex;
    func_0026E788(0, 0, third, position, markers[index] + 8, 0, packet);
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026F1F0);

MantraDisplayNode *mnuAllocateDisplayListNode(void) {
    MantraDisplayNode *node = (MantraDisplayNode *)func_00328D68(sizeof(MantraDisplayNode));

    memset(node, 0, sizeof(MantraDisplayNode));
    return node;
}

u32 mnuReleaseDisplayListNodeAndGetNext(MantraDisplayNode *node) {
    u32 next;

    next = (u32)node->next;
    // Required to match: the node remains in $a0 for the allocator's release call.
    func_00328E48();
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
    func_0010AE38("[MaxNum %d][CurrentIndex %d]\n", n, index);
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

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026F778);

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

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026F8A0);

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

u32 mnuCreateMantraDrawPool(u32 count) {
    u32 size = count * 0x24 + 0xc;
    u32 handle = func_003292A8(size);
    MantraDrawPool *pool = (MantraDrawPool *)sdfMemoryGetBlockAddress(handle);
    memset(pool, 0, size);
    pool->handle = handle;
    pool->count = count;
    pool->items = (MantraDrawItem *)((u8 *)pool + 0xc);
    func_0010AE38("mtrDrawProcessCreate!! num[%d]\n", count);
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
    func_003297C8(pool->handle);
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

u32 mnuRegisterMantraDrawItem(u32 pool, u32 kind, void (*update)(), void (*draw)(), u32 (*poll)(), void (*release)(), s16 startDelay, s16 endDelay, u32 data) {
    u32 *item = (u32 *)mnuFindFreeMantraDrawItem(pool);

    memset(item, 0, 0x24);
    item[1] |= 1;
    item[0] = kind;
    item[7] = data;
    ((MantraDrawItem *)item)->timer1 = endDelay;
    ((MantraDrawItem *)item)->timer0 = startDelay;
    item[2] = (u32)poll;
    if (update != 0) {
        item[4] = (u32)update;
    } else {
        item[4] = (u32)func_0026FAB8;
    }
    if (draw != 0) {
        item[5] = (u32)draw;
    } else {
        item[5] = (u32)func_0026FAB8;
    }
    item[3] = (u32)release;
    if (startDelay > 0) {
        item[1] = (item[1] & 0xFFFFF807) | 8;
    } else if (poll != 0) {
        item[1] = (item[1] & 0xFFFFF807) | 0x18;
    } else {
        item[1] = (item[1] & 0xFFFFF807) | 0x20;
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

typedef struct DrawItem {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u32 active : 1;
    u32 unk_bits : 2;
    u32 state : 8;
    u32 started : 1;
    u32 unk_hi : 20;
    /* 0x08 */ s32 (*onStart)(struct DrawItem *, s32);
    /* 0x0C */ void (*onEnd)(struct DrawItem *);
    /* 0x10 */ s32 (*isReady)(void *, struct DrawItem *);
    /* 0x14 */ void (*update)(void *, struct DrawItem *);
    /* 0x18 */ s16 timer0;
    /* 0x1A */ s16 timer1;
    /* 0x1C */ s32 arg;
    /* 0x20 */ s32 result;
} DrawItem;

void mnuUpdateMantraDrawPool(u8 *pool) {
    s32 i;
    s32 ready = 0;
    s32 count = ((MantraDrawPool *)pool)->count;
    DrawItem *item = (DrawItem *)((MantraDrawPool *)pool)->items;

    for (i = 0; i < count; i++, item = (DrawItem *)((u8 *)item + 0x24)) {
        if (!item->active) {
            continue;
        }
        switch (item->state) {
        case 1:
            item->timer0 -= 1;
            if (item->timer0 == 0) {
                if (item->onStart != 0) {
                    item->state = 3;
                } else {
                    item->state = 4;
                }
            }
            break;
        case 2:
            item->timer1 -= 1;
            if (item->timer1 == 0) {
                if (item->onEnd != 0) {
                    item->state = 5;
                } else {
                    item->state = 6;
                }
            }
            break;
        case 3:
            item->state = 4;
            item->result = item->onStart(pool, item->arg);
            break;
        case 4:
            if (item->isReady(pool, item) == 1) {
                ready = 1;
            }
            item->update(pool, item);
            if (ready != 0) {
                if (item->timer1 > 0) {
                    item->state = 2;
                } else {
                    item->state = 5;
                }
            }
            break;
        case 5:
            if (!item->started) {
                item->onEnd(item);
            }
            item->started = 1;
            item->active = 0;
            break;
        case 6:
            item->active = 0;
            break;
        case 7:
            break;
        }
    }
}

u32 mnuRegisterMantraBackgroundDraw(u32 pool) {
    return mnuRegisterMantraDrawItem(pool, 0, func_00270210, func_00270848,
                         mnuInitMantraBackgroundDraw, mnuReleaseMantraBackgroundDraw, 0, 0, 0);
}

void func_00270050(u32 pool) {
    MantraDrawItem *item;

    item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 0);
    *(u16 *)item->data = 3;
}

void mnuSetMantraBackgroundVariant(u32 pool, s8 variant) {
    MantraDrawItem *item;
    u8 *data;

    item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 0);
    data = (u8 *)item->data;
    *(u32 *)(data + 4) = (*(u32 *)(data + 4) & 0xffffff0f) | (((s32)variant & 0xfU) << 4);
    *(u8 *)(data + 5) = 5;
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
    u32 data = func_00328D68(0x1c);
    memset((void *)data, 0, 0x1c);
    *(u16 *)data = 1;
    ((MantraBackgroundState *)data)->transitionWord &= ~0xf;
    *(u8 *)(data + 5) = 0;
    ((MantraBackgroundState *)data)->iconPool = func_002850B8();
    func_0010AE38("BG Draw Init\n");
    return data;
}

void mnuReleaseMantraBackgroundDraw(u32 obj) {
    s32 data = (s32)((MantraDrawItem *)obj)->data;
    func_00285120(*(u32 **)(data + 0x18));
    func_00328E48(data);
    func_0010AE38("BG Draw Release\n");
}

typedef struct MantraCursorFade {
    /* 0x00 */ u16 state;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ u32 cur : 4;
    u32 next : 4;
    u32 unk4_hi : 24;
    /* 0x08 */ u16 timer;
    /* 0x0A */ u16 clock;
    /* 0x0C */ f32 value;
} MantraCursorFade;

s32 func_00270210(s32 unused, s32 item) {
    MantraCursorFade *fade = (MantraCursorFade *)((MantraDrawItem *)item)->data;

    fade->clock += 1;
    if ((s16)fade->clock >= 0x79) {
        fade->clock = 0;
    }
    if (*(s8 *)((u8 *)fade + 5) > 0) {
        *(s8 *)((u8 *)fade + 5) -= 1;
        if (*(s8 *)((u8 *)fade + 5) == 0) {
            if (fade->cur != fade->next) {
                fade->cur = fade->next;
                *(s8 *)((u8 *)fade + 5) = 5;
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

extern void func_0026EDB0(s32, s32, s32, s32, s32, s32, s32, f32);

void mnuDrawMantraPulseFrame(s32 amount, s32 packet, f32 pulse) {
    func_00308478(0x44, packet);
    func_00308380(0x30000, packet);
    func_00308808(0, 0, 0, 0x2000, 0xE00, 0, packet);
    func_00308380(0x5100DL, packet);
    func_00308380(0x3000DL, packet);
    func_00308808(0, 0, 0xFFFFFF, 0x2000, 0xE00, 0, packet);
    func_00308DB0(packet);
    func_0026E788(0, -15, 0, amount, 0x60, 0x20, packet);
    func_0026E788(0, -15, 0, amount, 0x61, 0x20, packet);
    func_0026E788(0, 0, 0, amount, 0x62, 0x20, packet);
    func_0026E788(0, 0, 0, amount, 0x63, 0x20, packet);
    func_00308E60(packet);
    func_00308478(0x54, packet);
    func_0026E788(0, 0, 0, amount, 0x58, 0x60, packet);
    amount = amount * pulse * 0.5f;
    func_0026E788(0, 0, 0, amount, 0x5A, 0, packet);
    func_0026EDB0(0, 0x140, 0, amount, 0x5A, 0, packet, 180.0f);
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00270568);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00270848);

u32 mnuRegisterMantraBackgroundMaskDraw(u32 pool) {
    return mnuRegisterMantraDrawItem(pool, 4, func_00270DD8, mnuDrawMantraBackgroundMaskPulse,
                         mnuInitMantraBackgroundMaskDraw, mnuReleaseMantraBackgroundMaskDraw, 0, 0, 0);
}

void func_00270CC0(u32 pool) {
    MantraDrawItem *item;

    item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 4);
    *(u16 *)item->data = 3;
}

void func_00270CE8(u32 pool) {
    MantraDrawItem *item;

    item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 4);
    *(u16 *)item->data = 6;
}

void func_00270D10(u32 pool) {
    MantraDrawItem *item;

    item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 4);
    *(u16 *)item->data = 5;
}

void func_00270D38(u32 pool) {
    MantraDrawItem *item;

    item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 4);
    *(u16 *)item->data = 2;
}

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004251C8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425258);

u32 mnuInitMantraBackgroundMaskDraw(void) {
    u32 data = func_00328D68(0xc);
    memset((void *)data, 0, 0xc);
    *(u16 *)data = 1;
    func_0010AE38("BGMask Draw Init\n");
    return data;
}

void mnuReleaseMantraBackgroundMaskDraw(u32 obj) {
    func_00328E48(((MantraDrawItem *)obj)->data);
    func_0010AE38("BGMask Draw Release\n");
}

s32 func_00270DD8(s32 unused, s32 item) {
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
    u8 *data = (u8 *)((MantraDrawItem *)item)->data;
    s32 amount = ((MantraPulseFade *)data)->scale * 128.0f;
    f32 wave = sdfSinPoly((f32)*(s16 *)(data + 4) / 120.0f * (3.14159265f * 2.0f) + (-3.14159265f / 2.0f));

    mnuDrawMantraPulseFrame(amount, 0x53, (wave + 1.0f) * 0.5f);
    return 0;
}

u32 mnuCreateMantraFadeDrawItem(u32 list, u32 tag, u32 category) {
    MantraDrawItemBits *item = (MantraDrawItemBits *)mnuRegisterMantraDrawItem(list, 1, func_00271368, func_00271510,
                                                                    func_002712E0, func_00271348, 0, 0, tag);
    item->category = category;
    return (u32)item;
}

void func_00271020(u32 pool) {
    s32 obj = mnuFindMantraDrawItemByKind(pool, 1);
    if (obj != 0) {
        s32 data = (s32)((MantraDrawItem *)obj)->data;
        if (*(u16 *)data == 4) {
            ((MantraFadeState *)data)->timer = 5;
        }
        *(u16 *)data = 3;
    }
}

void mnuShowMantraLimitLine(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 1);
    if (item != 0) {
        *(u16 *)item->data = 6;
        func_0010AE38("LimitLine Draw Show\n");
    }
}

void mnuHideMantraLimitLine(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 1);
    if (item != 0) {
        *(u16 *)item->data = 5;
        func_0010AE38("LimitLine Draw Hide\n");
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

void func_002711F8(s16 x, s16 y, u32 pool) {
    s32 obj = mnuFindMantraDrawItemByKind(pool, 1);
    if (obj != 0) {
        s32 data = (s32)((MantraDrawItem *)obj)->data;
        ((MantraFadeState *)data)->x = x;
        ((MantraFadeState *)data)->y = y;
    }
}

void func_00271250(u32 pool, u32 value) {
    s32 obj = mnuFindMantraDrawItemByKind(pool, 1);
    if (obj != 0) {
        s32 data = (s32)((MantraDrawItem *)obj)->data;
        ((MantraFadeState *)data)->flags10 = value;
        *(u32 *)(data + 0x14) = 0x15;
    }
}

void func_00271290(s32 pool, u32 flags) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 1);
    MantraFadeState *fade;

    if (item != 0) {
        fade = item->data;
        if (fade->delay == 0) {
            fade->armed = 0;
            fade->countdown = 0;
            fade->flagsC = flags;
            fade->flags10 = 0;
            fade->queuedFlags = flags;
        } else {
            fade->queuedFlags = flags;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002712E0);

void func_00271348(u32 obj) {
    func_00328E48(((MantraDrawItem *)obj)->data);
}


s32 func_00271368(s32 unused, s32 item) {
    MantraFadeState *fade = (MantraFadeState *)((MantraDrawItem *)item)->data;

    fade->clock += 1;
    if (fade->clock >= 0x79) {
        fade->clock = 0;
    }
    if (fade->delay != 0) {
        fade->delay -= 1;
        if (fade->delay == 0) {
            fade->state = fade->queuedState;
            fade->flagsC = fade->queuedFlags;
        }
    }
    if (fade->armed) {
        fade->countdown -= 1;
        if (fade->countdown == 0) {
            fade->armed = 0;
            fade->flagsC |= fade->flags10;
            fade->flags10 = 0;
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
    return mnuRegisterMantraDrawItem(pool, 6, func_00272DA8, func_00272F08,
                         mnuInitMantraTitleDraw, mnuReleaseMantraTitleDraw, 0, 0, 0);
}

void func_00272C18(u32 pool) {
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
        func_0010AE38("Title Draw Hide\n");
    }
}

void mnuShowMantraTitle(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 6);
    if (item != 0) {
        *(u16 *)item->data = 6;
        func_0010AE38("Title Draw Show\n");
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
    u32 data = func_00328D68(0x10);
    memset((void *)data, 0, 0x10);
    ((MantraBlinkState *)data)->unk2 = 0;
    ((MantraBlinkState *)data)->timer = 0;
    *(u16 *)data = 1;
    ((MantraBlinkState *)data)->clock = 0;
    func_0010AE38("Title Draw Init\n");
    return data;
}

void mnuReleaseMantraTitleDraw(u32 obj) {
    func_00328E48(((MantraDrawItem *)obj)->data);
    func_0010AE38("Title Draw Release\n");
}


s32 func_00272DA8(s32 unused, s32 item) {
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
    return mnuRegisterMantraDrawItem(pool, 7, func_00273668, func_00273828,
                         mnuInitMantraInfoDraw, mnuReleaseMantraInfoDraw, 10, 0, 0);
}

void func_00273498(u32 pool) {
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
        func_0010AE38("Info Draw Hide\n");
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
        func_0010AE38("Info Draw Show\n");
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

void func_002735A0(u32 pool, s16 x, u16 y) {
    s32 obj = mnuFindMantraDrawItemByKind(pool, 7);
    if (obj != 0) {
        s32 data = (s32)((MantraDrawItem *)obj)->data;
        ((MantraPulseState *)data)->x = x;
        ((MantraPulseState *)data)->y = y;
    }
}

u32 mnuInitMantraInfoDraw(void) {
    u32 data = func_00328D68(0x10);
    memset((void *)data, 0, 0x10);
    *(u16 *)data = 1;
    func_0010AE38("Info Draw Init\n");
    return data;
}

void mnuReleaseMantraInfoDraw(u32 obj) {
    func_00328E48(((MantraDrawItem *)obj)->data);
    func_0010AE38("Info Draw Release\n");
}


s32 func_00273668(s32 unused, s32 item) {
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
    return mnuRegisterMantraDrawItem(pool, 8, func_002741D0, mnuDrawMantraGauge,
                         mnuInitMantraGaugeData, mnuReleaseMantraGaugeData, 0, 0, 0);
}

void func_00273FD0(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 8);
    *(u16 *)item->data = 3;
}

void mnuHideMantraScrollCursor(u32 ctx) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(ctx, 8);
    if (item != 0) {
        s32 data = (s32)item->data;
        u16 state = *(u16 *)data;
        if (state == 6) {
            *(u32 *)(data + 8) =
                ((*(u32 *)(data + 8) | 0x10000) & 0xfe01ffff) | 0xa0000;
        } else if (state != 4) {
            *(u16 *)data = 5;
        }
        func_0010AE38("ScrollCursor Draw Hide\n");
    }
}

void mnuShowMantraScrollCursor(u32 ctx) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(ctx, 8);
    if (item != 0) {
        s32 data = (s32)item->data;
        u16 state = *(u16 *)data;
        if (state == 5) {
            *(u32 *)(data + 8) =
                ((*(u32 *)(data + 8) | 0x10000) & 0xfe01ffff) | 0xa0000;
        } else if (state != 2) {
            *(u16 *)data = 6;
        }
        func_0010AE38("ScrollCursor Draw Show\n");
    }
}

void func_002740E8(u32 ctx, u16 flags) {
    s32 obj = mnuFindMantraDrawItemByKind(ctx, 8);
    if (obj != 0) {
        u8 *entry = (u8 *)((u32)((MantraDrawItem *)obj)->data + 2);
        s32 i;
        for (i = 0; i < 4; i++, entry += 2) {
            if ((flags >> i) & 1) {
                *entry = 1;
            } else {
                *entry = 0;
            }
        }
    }
}

u32 mnuInitMantraGaugeData(void) {
    u32 data = func_00328D68(0x18);
    memset((void *)data, 0, 0x18);
    *(u16 *)data = 4;
    func_0010AE38("ScrollCursor Draw Init\n");
    return data;
}

void mnuReleaseMantraGaugeData(u32 obj) {
    func_00328E48(((MantraDrawItem *)obj)->data);
    func_0010AE38("ScrollCursor Draw Release\n");
}

typedef struct MantraGaugeSlot {
    u8 active;
    u8 level;
} MantraGaugeSlot;

typedef struct MantraGaugeState {
    /* 0x00 */ u16 state;
    /* 0x02 */ MantraGaugeSlot slots[4]; /* slot 3 shares its bytes with bits */
    /* 0x0A */ u16 pad0A;
    /* 0x0C */ u16 timer;
    /* 0x0E */ u16 clockE;
    /* 0x10 */ u32 pad10;
    /* 0x14 */ f32 value;
} MantraGaugeState;

/* Overlays slot 3 of MantraGaugeState: bit 16 = reverse, bits 17..24 = countdown. */
#define GAUGE_BITS(g) (*(u32 *)((u8 *)(g) + 8))

s32 func_002741D0(s32 unused, s32 item) {
    MantraGaugeState *gauge = (MantraGaugeState *)((MantraDrawItem *)item)->data;
    s32 i;

    if (GAUGE_BITS(gauge) & 0x1FE0000) {
        GAUGE_BITS(gauge) = (GAUGE_BITS(gauge) & ~0x1FE0000) | ((((GAUGE_BITS(gauge) >> 17) - 1) & 0xFF) << 17);
    }
    for (i = 0; i < 4; i++) {
        if (gauge->slots[i].active != 0) {
            if (gauge->slots[i].level < 10) {
                gauge->slots[i].level += 1;
            }
        } else if (gauge->slots[i].level != 0) {
            gauge->slots[i].level -= 1;
        }
    }
    switch (gauge->state) {
    case 1:
    case 6:
        gauge->value = (f32)gauge->timer / 10.0f;
        gauge->timer += 1;
        if (gauge->timer >= 10) {
            if (GAUGE_BITS(gauge) & 0x10000) {
                gauge->state = 4;
            } else {
                gauge->state = 2;
            }
            gauge->timer = 0;
            gauge->value = 1.0f;
            GAUGE_BITS(gauge) &= ~0x10000;
        }
        break;
    case 2:
        gauge->value = 1.0f;
        break;
    case 3:
    case 5:
        gauge->value = 1.0f - (f32)gauge->timer / 10.0f;
        gauge->timer += 1;
        if (gauge->timer >= 10) {
            gauge->timer = 0;
            gauge->value = 0.0f;
            if (gauge->state == 5) {
                if (GAUGE_BITS(gauge) & 0x10000) {
                    gauge->state = 2;
                } else {
                    gauge->state = 4;
                }
                GAUGE_BITS(gauge) &= ~0x10000;
            } else {
                return 1;
            }
        }
        break;
    case 4:
        gauge->value = 0.0f;
        break;
    }
    return 0;
}

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
        gauge->clockE += 1;
        if ((s16)gauge->clockE >= 0x79) {
            gauge->clockE = 0;
        }
        wave = (s16)gauge->clockE / 120.0f;
        wave = (sdfSinPoly(wave * (3.14159265f * 2.0f) + -3.14159265f) + 1.0f) * 0.5f;
        for (i = 0; i < 4; i++) {
            level = gauge->slots[i].level / 10.0f;
            func_0026E788(0, 0, 0, (s32)((f32)alpha * level), icons[i], 0, 0x53);
            func_0026E788(0, 0, 0, (s32)((f32)(s32)((f32)alpha * wave * level) * 0.7f), glowIcons[i], 0, 0x53);
        }
        break;
    }
    return 0;
}

typedef struct MantraLampState {
    /* 0x00 */ u16 state;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ u16 timer;
    /* 0x06 */ u16 clock;
    /* 0x08 */ f32 value;
    /* 0x0C */ u8 unkC[0x30];
    /* 0x3C */ u32 bits;
} MantraLampState;

u32 mnuRegisterMantraUnitPanelDraw(u32 pool, u32 resource) {
    return mnuRegisterMantraDrawItem(pool, 9, func_002748D0, func_00274A70,
                         mnuInitMantraUnitPanelDraw, mnuReleaseMantraUnitPanelDraw, 10, 0, resource);
}

void func_00274628(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 9);
    *(u16 *)item->data = 3;
}

void func_00274650(u32 pool, s16 value) {
    s32 obj = mnuFindMantraDrawItemByKind(pool, 9);
    if (obj != 0) {
        *(s16 *)((s32)((MantraDrawItem *)obj)->data + 2) = value;
    }
}

void mnuHideMantraUnitPanel(u32 ctx) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(ctx, 9);
    if (item != 0) {
        s32 data = (s32)item->data;
        if (*(u16 *)data == 6) {
            ((MantraLampState *)data)->bits =
                ((((MantraLampState *)data)->bits | 1) & 0xffff0001) | 0xa;
        } else {
            *(u16 *)data = 5;
        }
        func_0010AE38("UnitPanel Draw Hide\n");
    }
}

void mnuShowMantraUnitPanel(u32 ctx) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(ctx, 9);
    if (item != 0) {
        s32 data = (s32)item->data;
        if (*(u16 *)data == 5) {
            ((MantraLampState *)data)->bits =
                ((((MantraLampState *)data)->bits | 1) & 0xffff0001) | 0xa;
        } else {
            *(u16 *)data = 6;
        }
        func_0010AE38("UnitPanel Draw Show\n");
    }
}


void func_00274760(u32 pool) {
    s32 obj = mnuFindMantraDrawItemByKind(pool, 9);
    func_0026F778((u32)((MantraDrawItem *)obj)->data + 0xc);
}

void mnuQueueNextUnitPanelSelection(u32 pool) {
    s32 obj = mnuFindMantraDrawItemByKind(pool, 9);
    mnuQueueNextMantraSelection((u32)((MantraDrawItem *)obj)->data + 0xc);
}

u32 mnuQueueUnitPanelSelection(u32 pool, s8 value) {
    s32 obj = mnuFindMantraDrawItemByKind(pool, 9);
    return mnuQueueMantraSelectionTransition((u32)((MantraDrawItem *)obj)->data + 0xc, value) != 0;
}

u32 func_002747F0(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 9);
    func_0026FAA8((s32)item->data + 0xc);
    return 0;
}

u32 mnuInitMantraUnitPanelDraw(u32 ctx, u32 resources) {
    u32 data = func_00328D68(0x40);
    memset((void *)data, 0, 0x40);
    *(u16 *)data = 1;
    mnuInitMantraListEntries(data + 0xc, resources,
                  *(u32 *)(resources + 0x24), *(u32 *)(resources + 0x20));
    func_0010AE38("UnitPanel Draw Init\n");
    return data;
}

void mnuReleaseMantraUnitPanelDraw(u32 obj) {
    s32 data = (s32)((MantraDrawItem *)obj)->data;
    mnuReleaseDisplayListNodes(data + 0xc);
    func_00328E48(data);
    func_0010AE38("UnitPanel Draw Release\n");
}


s32 func_002748D0(s32 unused, s32 item) {
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
    return mnuRegisterMantraDrawItem(pool, 10, func_00274EA8, func_00274FF8,
                         mnuCreateTypeOneRecord, func_00274E88, 0, 0, 0);
}

void func_00274DD0(u32 pool) {
    MantraDrawItem *item;

    item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 10);
    *(u16 *)item->data = 3;
}

void mnuToggleMantraTypeOnePanelMode(u32 pool) {
    MantraDrawItem *item;
    MenuRecord *record;

    item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 10);
    record = (MenuRecord *)item->data;
    record->modeC = 0xf;
    record->flags = record->flags ^ 1;
}

u32 mnuCreateTypeOneRecord(void) {
    MenuRecord *record = (MenuRecord *)func_00328D68(sizeof(MenuRecord));
    memset(record, 0, sizeof(MenuRecord));
    record->type = 1;
    record->flags = 0;
    record->unk_10 = ((MantraMenuValues *)D_00435DD0)->panelValue;
    return (u32)record;
}

void func_00274E88(u32 obj) {
    func_00328E48(((MantraDrawItem *)obj)->data);
}

s32 func_00274EA8(s32 unused, s32 item) {
    MantraPanelFade *fade = (MantraPanelFade *)((MantraDrawItem *)item)->data;

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

    func_0026E788(x, y, depth, fade, 0x2E, 0, drawArg);
    if (((MantraMenuValues *)D_00435DD0)->panelValue != state->shown) {
        s32 steps = 20;

        sndSetSequenceVolumePan(0x13, 0x7F, 0x3F);
        state->step++;
        func_0035C860(text, D_004378A0, state->shown + (((MantraMenuValues *)D_00435DD0)->panelValue - state->shown) * state->step / steps);
        if (state->step == steps) {
            state->shown = ((MantraMenuValues *)D_00435DD0)->panelValue;
            state->step = 0;
        }
    } else {
        func_0035C860(text, D_004378A0, ((MantraMenuValues *)D_00435DD0)->panelValue);
    }
    func_00311D00(x + 0x193, y + 0x26, depth, flags, 0, text, 0, drawArg);
}

extern u32 func_002755B8(u32 *, u32);

MantraIconEntry *mnuSpawnMantraIcon(s32 x, s32 y, MantraIconPool *pool, u32 mode) {
    MantraIconEntry *entry = (MantraIconEntry *)func_002755B8((u32 *)pool, mode);

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
    u32 handle = func_003292A8(size);
    u32 block = sdfMemoryGetBlockAddress(handle);
    memset((void *)block, 0, size);
    *(u32 *)block = handle;
    ((MantraIconPool *)block)->count = count;
    ((MantraIconPool *)block)->entries = (MantraIconEntry *)(block + 0x14);
    return block;
}

void mnuReleaseMantraIconSprite(u32 *sprite) {
    func_003297C8(*sprite);
}

u32 func_002755B8(u32 *pool, u32 flags) {
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

s32 func_002758B8(s32 x, s32 y, u32 depth, s32 amount, MantraIconPool *pool) {
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
        func_0026E788(x + icon->x, y + icon->y, depth, amount, 0x10B, 0, 0x53);
        break;
    case 2:
        func_0026E788(x + icon->x, y + icon->y, depth, amount, 0x10B, 0, 0x53);
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
        func_0026E788(x + icon->x, y + icon->y, depth, amount, 0x10B, 0, 0x53);
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
        func_0026E788(x + icon->x, y + icon->y, depth, amount, 0x10C, 0, 0x53);
        break;
    case 2:
        func_0026E788(x + icon->x, y + icon->y, depth, amount, 0x10C, 0, 0x53);
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
        func_0026E788(x + icon->x, y + icon->y, depth, amount, 0x10C, 0, 0x53);
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

void func_00278DC8(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 2);
    if (item != 0) {
        ((MantraFadeData *)item->data)->state = 3;
    }
}

void func_00278DF8(s16 x, s16 y, u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 2);
    if (item != 0) {
        MantraFadeData *fade = (MantraFadeData *)item->data;
        fade->x = x;
        fade->y = y;
    }
}

void func_00278E50(u32 a, u32 b, u32 pool) {
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

void func_00278F18(u32 pool) {
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

void func_00279028(u32 a, u32 b, u32 pool) {
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

u32 mnuCreateMantraIconListA(s32 unused, u8 *menu) {
    u32 data = func_00328D68(0x10);
    u8 *slot;
    MantraNodePos *first;
    MantraNodePos *second;

    memset((void *)data, 0, 0x10);
    *(u32 *)data = mnuAllocateMantraIconPool(0xA);
    *(u16 *)(data + 4) = 1;
    slot = menu + 0x240;
    first = func_0026CF70(func_00314B78(mnuGetSelectedNodeValue(menu)));
    mnuSpawnMantraIcon(first->x / 10.0f * 40.0f, first->y / 10.0f * 39.0f, *(MantraIconPool **)data, 0x20);
    second = *(MantraNodePos **)(slot + 0x560);
    mnuSpawnMantraIcon(second->x / 10.0f * 40.0f, second->y / 10.0f * 39.0f, *(MantraIconPool **)data, 0x10);
    return data;
}

void mnuReleaseMantraFadeData(s32 obj) {
    MantraFadeData *fade = (MantraFadeData *)((MantraDrawItem *)obj)->data;
    mnuReleaseMantraIconSprite((u32 *)fade->iconPool);
    func_00328E48(fade);
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
    func_002758B8(((MantraFadeData *)data)->x, ((MantraFadeData *)data)->y, 0,
                  scale, ((MantraFadeData *)data)->iconPool);
    return 0;
}

u32 mnuCreateMantraIconListB(s32 unused, u8 *menu) {
    u32 data = func_00328D68(0x10);
    u8 *slot;
    MantraNodePos *first;
    MantraNodePos *second;

    memset((void *)data, 0, 0x10);
    *(u32 *)data = mnuAllocateMantraIconPool(0xA);
    *(u16 *)(data + 4) = 1;
    slot = menu + 0x240;
    first = func_0026CF70(func_00314B78(mnuGetSelectedNodeValue(menu)));
    mnuSpawnMantraIcon(first->x * 20 / 10.0f, first->y * 20 / 10.0f, *(MantraIconPool **)data, 0x60);
    second = *(MantraNodePos **)(slot + 0x560);
    mnuSpawnMantraIcon(second->x * 20 / 10.0f, second->y * 20 / 10.0f, *(MantraIconPool **)data, 0x50);
    return data;
}

u32 mnuRegisterMantraIconListBDraw(u32 pool, u32 resource) {
    return mnuRegisterMantraDrawItem(pool, 3, mnuUpdateMantraFadeA, mnuDrawMantraIconList,
                         mnuCreateMantraIconListB, mnuReleaseMantraFadeData, 0, 0, resource);
}

typedef struct MantraNodeRef {
    u8 pad00[2];
    s16 id;
} MantraNodeRef;

u32 mnuCreateMantraIconListC(s32 unused, u8 *menu) {
    u32 data = func_00328D68(0x10);
    u8 *slot = menu + 0x240;
    MantraNodePos *first;
    MantraNodePos *second;

    memset((void *)data, 0, 0x10);
    *(u32 *)data = mnuAllocateMantraIconPool(0xA);
    *(u16 *)(data + 4) = 1;
    first = func_0026D098((*(MantraNodeRef **)(menu + 0x7A0))->id);
    mnuSpawnMantraIcon(first->x * 20 / 10.0f, first->y * 20 / 10.0f, *(MantraIconPool **)data, 0x60);
    second = *(MantraNodePos **)(slot + 0x560);
    mnuSpawnMantraIcon(second->x * 20 / 10.0f, second->y * 20 / 10.0f, *(MantraIconPool **)data, 0x50);
    return data;
}

u32 mnuRegisterMantraIconListCDraw(u32 pool, u32 resource) {
    return mnuRegisterMantraDrawItem(pool, 3, mnuUpdateMantraFadeA, mnuDrawMantraIconList,
                         mnuCreateMantraIconListC, mnuReleaseMantraFadeData, 0, 0, resource);
}

void func_002797C0(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 3);
    if (item != 0) {
        ((MantraFadeData *)item->data)->state = 3;
    }
}

void func_002797F0(s16 x, s16 y, u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 3);
    if (item != 0) {
        MantraFadeData *fade = (MantraFadeData *)item->data;
        fade->x = x;
        fade->y = y;
    }
}

void func_00279848(u32 a, u32 b, u32 pool) {
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

void func_00279910(u32 a, u32 b, u32 pool) {
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

void func_00279C38(u32 *sprite) {
    if (sprite != 0) {
        func_003297C8(*sprite);
    }
}

typedef struct MantraPanelAnimation {
    u32 flags;
    u16 unk4;
    u16 unk6;
    u32 unk8;
    u16 x;
    u16 y;
    u32 visualParameters[3];
    u8 byte1C;
    u8 byte1D;
    u16 frame;
    u8 pad20[0xC];
    s16 id;
    s16 transitionDelay;
} MantraPanelAnimation;

typedef struct MantraPanelPool {
    u32 handle;
    MantraPanelAnimation *items;
    s32 count;
} MantraPanelPool;

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
    *(s8 *)panel = kind;
    panel->unk4 = x;
    panel->unk6 = y;
    panel->unk8 = w;
    panel->flags = ((panel->flags & 0xFFFC03FF) | 0x400) & 0xEFFFFFFF;
    panel->transitionDelay = 0;
    panel->id = id;
    panel->frame = 0;
    pos = func_0026CF70(id);
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
    *(s8 *)panel = kind;
    panel->unk4 = x;
    panel->unk6 = y;
    panel->unk8 = w;
    panel->flags = (panel->flags & 0xFFFC03FF) | 0x10000400;
    panel->transitionDelay = 0;
    panel->id = id;
    panel->frame = 0;
    pos = func_0026D098(id);
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
        if ((s16)panel->frame >= 4) {
            panel->frame = 0;
            panel->flags = panel->flags & 0xFF87FFFF;
        }
        break;
    case 8:
        panel->frame += 1;
        if ((s16)panel->frame >= 10) {
            panel->frame = 0;
            panel->flags = panel->flags & 0xFF87FFFF;
        }
        break;
    case 7:
        panel->frame += 1;
        if ((s16)panel->frame >= 4) {
            result = 1;
        }
        break;
    case 9:
        panel->frame += 1;
        result = (s16)panel->frame > 9;
        break;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027A198);

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

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027A800);

extern f32 func_00341240(void *state);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378C0);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378C8);

void func_0027AF40(s32 unused, u8 *object) {
    u8 table[4] = {0, 20, 40, 60};
    u8 index;

    object[0x20] = 0;
    object[0x21] = 0;
    index = func_00341240(0) * 3.0f;
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

void func_0027C078(s32 unused, u8 *object) {
    u8 table[4] = {0, 10, 20, 30};
    u8 index;

    index = func_00341240(0) * 3.0f;
    object[0x20] = table[index];
}

void func_0027C108(void) {
}

s32 mnuDrawMantraPulseIcon(s32 x, s32 y, s32 z, s32 amount, s32 unused, u8 *object, s32 packet) {
    u8 table[4] = {0, 10, 20, 30};
    f32 t;

    object[0x20] += 1;
    if (object[0x20] > 0x82) {
        object[0x20] = table[(u8)(func_00341240(0) * 3.0f)];
    }
    t = 0.0f;
    if (object[0x20] >= 0x32) {
        if (object[0x20] < 0x78) {
            t = (object[0x20] - 0x32) / 70.0f;
        }
    }
    t = (sdfSinPoly(t * (3.14159265f * 2.0f) + (-3.14159265f / 2.0f)) + 1.0f) * 0.5f;
    func_0026E788(x, y, z, amount, 0x77, 0, packet);
    func_0026E788(x, y, z, amount, 0x93, 0, packet);
    func_0026E788(x, y, z, (s32)(amount * (t * 0.25f)), 0x94, 0, packet);
    return 0;
}

s32 func_0027C2F0(s32 x, s32 y, s32 z, s32 amount, s32 unused, u8 *object, s32 packet) {
    f32 scale;

    switch ((*(u32 *)object >> 19) & 0xF) {
    case 6:
        scale = ((MantraPanelSpriteView *)object)->animationStep * 0.25f;
        amount = amount * scale;
        mnuDrawMantraPulseIcon(x, y, z, amount, unused, object, packet);
        break;
    case 8:
        scale = ((MantraPanelSpriteView *)object)->animationStep / 10.0f;
        amount = amount * scale;
        mnuDrawMantraPulseIcon(x, y, z, amount, unused, object, packet);
        break;
    case 7:
        scale = ((MantraPanelSpriteView *)object)->animationStep * 0.25f;
        scale = 1.0f - scale;
        amount = amount * scale;
        mnuDrawMantraPulseIcon(x, y, z, amount, unused, object, packet);
        break;
    case 9:
        scale = ((MantraPanelSpriteView *)object)->animationStep / 10.0f;
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

    spriteHandle = func_002843C0(1);
    ((MantraPanelSpriteView *)view)->spriteHandle = spriteHandle;
}

void btlReleasePanelASprite(u32 obj) {
    func_002844E8(((MantraPanelSpriteView *)obj)->spriteHandle);
}

s32 btlDrawPanelA(s32 x, s32 y, u32 a2, u32 a3, u32 a4, u32 a5, u32 packet) {
    func_0026E788(x, y, 0, a3, 0x77, 0, packet);
    func_0026E788(x, y, 0, a3, 0x89, 0, packet);
    func_00308380(0x30000, packet);
    func_00308808((x - 0x80) << 4, (y - 0x80) << 3, 0xffffff, 0x1000, 0x800, 0, packet);
    func_00308380(0x3000DL, packet);
    func_00308DB0(packet);
    func_0026E788(x, y, 0, 0x80, 0x91, 0x60, packet);
    func_00308E60(packet);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027C558);

void func_0027CD78(u32 unused, s32 view) {
    u32 spriteHandle;

    spriteHandle = func_002843C0(2);
    ((MantraPanelSpriteView *)view)->spriteHandle = spriteHandle;
    ((MantraPanelSpriteView *)view)->stateA = 0;
    ((MantraPanelSpriteView *)view)->stateB = 0;
}

void func_0027CDB0(u32 obj) {
    func_002844E8(((MantraPanelSpriteView *)obj)->spriteHandle);
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

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027DE40);

extern s32 func_0027DE40(s32, s32, s32, s32, s32, u8 *, s32);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425BC8);

s32 func_0027E0E0(s32 x, s32 y, s32 z, s32 amount, s32 unused, u8 *object, s32 packet) {
    f32 scale;

    switch ((*(u32 *)object >> 19) & 0xF) {
    case 6:
        scale = ((MantraPanelSpriteView *)object)->animationStep * 0.25f;
        amount = amount * scale;
        func_0027DE40(x, y, z, amount, unused, object, packet);
        break;
    case 8:
        scale = ((MantraPanelSpriteView *)object)->animationStep / 10.0f;
        amount = amount * scale;
        func_0027DE40(x, y, z, amount, unused, object, packet);
        break;
    case 7:
        scale = ((MantraPanelSpriteView *)object)->animationStep * 0.25f;
        scale = 1.0f - scale;
        amount = amount * scale;
        func_0027DE40(x, y, z, amount, unused, object, packet);
        break;
    case 9:
        scale = ((MantraPanelSpriteView *)object)->animationStep / 10.0f;
        scale = 1.0f - scale;
        amount = amount * scale;
        func_0027DE40(x, y, z, amount, unused, object, packet);
        break;
    case 0:
        func_0027DE40(x, y, z, amount, unused, object, packet);
        break;
    case 1:
        func_0027DE40(x, y, z, amount, unused, object, packet);
        break;
    }
    return 0;
}

void btlInitPanelBSprites(u32 unused, s32 view) {
    u32 resource;

    resource = func_002843C0(0);
    ((MantraPanelSpriteView *)view)->spriteHandle = resource;
    resource = func_00284AE0();
    ((MantraPanelSpriteView *)view)->burstPool = resource;
}

void btlReleasePanelBSprites(s32 obj) {
    func_002844E8(((MantraPanelSpriteView *)obj)->spriteHandle);
    func_00284B48((u32 *)((MantraPanelSpriteView *)obj)->burstPool);
}

s32 btlDrawPanelB(s32 x, s32 y, u32 a2, u32 a3, u32 a4, u32 a5, u32 packet) {
    func_0026E788(x, y, 0, a3, 0x77, 0, packet);
    func_0026E788(x, y, 0, a3, 0x87, 0, packet);
    func_00308380(0x30000, packet);
    func_00308808((x - 0x80) << 4, (y - 0x80) << 3, 0xffffff, 0x1000, 0x800, 0, packet);
    func_00308380(0x3000DL, packet);
    func_00308DB0(packet);
    func_0026E788(x, y, 0, 0x80, 0x91, 0x60, packet);
    func_00308E60(packet);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027E360);

void func_0027FB60(void) {
}

void func_0027FB68(void) {
}

s32 btlDrawPanelC(s32 x, s32 y, u32 a2, u32 a3, u32 a4, u32 a5, u32 packet) {
    func_00308380(0x30000, packet);
    func_00308808((x - 0x80) << 4, (y - 0x80) << 3, 0, 0x1000, 0x800, 0, packet);
    func_00308380(0x5100DL, packet);
    func_0026E788(x, y, 0, a3, 0x77, 0, packet);
    func_0026E788(x, y, 0, a3, 0x9A, 0, packet);
    func_00308380(0x30000, packet);
    func_00308808((x - 0x80) << 4, (y - 0x80) << 3, 0, 0x1000, 0x800, 0, packet);
    func_00308380(0x5100DL, packet);
    return 0;
}

s32 func_0027FC90(s32 x, s32 y, s32 z, s32 amount, s32 unused, u8 *object, s32 packet) {
    f32 scale;

    switch ((*(u32 *)object >> 19) & 0xF) {
    case 6:
        scale = ((MantraPanelSpriteView *)object)->animationStep * 0.25f;
        amount = amount * scale;
        btlDrawPanelC(x, y, z, amount, unused, (u32)object, packet);
        break;
    case 8:
        scale = ((MantraPanelSpriteView *)object)->animationStep / 10.0f;
        amount = amount * scale;
        btlDrawPanelC(x, y, z, amount, unused, (u32)object, packet);
        break;
    case 7:
        scale = ((MantraPanelSpriteView *)object)->animationStep * 0.25f;
        scale = 1.0f - scale;
        amount = amount * scale;
        btlDrawPanelC(x, y, z, amount, unused, (u32)object, packet);
        break;
    case 9:
        scale = ((MantraPanelSpriteView *)object)->animationStep / 10.0f;
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

void func_00280390(u32 unused, s32 view) {
    ((MantraPanelSpriteView *)view)->stateA = 0;
    ((MantraPanelSpriteView *)view)->stateB = 0;
    ((MantraPanelSpriteView *)view)->stateC = 0;
}

void func_002803A0(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002803A8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425C98);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425CA8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425CB8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002805E0);

void func_002817B8(u32 unused, s32 view) {
    ((MantraPanelSpriteView *)view)->stateA = 0;
}

void func_002817C0(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002817C8);

extern s32 func_002817C8(s32, s32, s32, s32, s32, u8 *, s32);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425CF8);

s32 func_00281B60(s32 x, s32 y, s32 z, s32 amount, s32 unused, u8 *object, s32 packet) {
    f32 scale;

    switch ((*(u32 *)object >> 19) & 0xF) {
    case 6:
        scale = ((MantraPanelSpriteView *)object)->animationStep * 0.25f;
        amount = amount * scale;
        func_002817C8(x, y, z, amount, unused, object, packet);
        break;
    case 8:
        scale = ((MantraPanelSpriteView *)object)->animationStep / 10.0f;
        amount = amount * scale;
        func_002817C8(x, y, z, amount, unused, object, packet);
        break;
    case 7:
        scale = ((MantraPanelSpriteView *)object)->animationStep * 0.25f;
        scale = 1.0f - scale;
        amount = amount * scale;
        func_002817C8(x, y, z, amount, unused, object, packet);
        break;
    case 9:
        scale = ((MantraPanelSpriteView *)object)->animationStep / 10.0f;
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

void func_00281C88(u32 unused, s32 view) {
    u32 spriteHandle;

    spriteHandle = func_002843C0(1);
    ((MantraPanelSpriteView *)view)->spriteHandle = spriteHandle;
}

void func_00281CB8(u32 obj) {
    func_002844E8(((MantraPanelSpriteView *)obj)->spriteHandle);
}

u32 func_00281CD8(u32 ctx, u32 x, u32 y, u32 direction, u32 unused,
                  u32 sprite, u32 animation) {
    func_0026E788(ctx, x, y, direction, 0x77, 0, animation);
    func_0026E788(ctx, x, y, direction, 0xf7, 0, animation);
    func_0026E788(ctx, x, y, direction, 0xf8, 0, animation);
    func_0026E788(ctx, x, y, direction, 0xf9, 0, animation);
    func_00284508(ctx, x, 0, direction, ((MantraPanelSpriteView *)sprite)->spriteHandle, animation);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00281DC0);

void func_00282B50(u32 unused, s32 view) {
    ((MantraPanelSpriteView *)view)->stateA = 0;
    ((MantraPanelSpriteView *)view)->stateB = 0;
}

void func_00282B60(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00282B68);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425D68);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00283090);

void func_00283F90(u32 unused, s32 view) {
    ((MantraPanelSpriteView *)view)->stateA = 0;
    ((MantraPanelSpriteView *)view)->stateB = 0;
}

void func_00283FA0(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00283FA8);

extern s32 func_00283FA8(s32, s32, s32, s32, s32, u8 *, s32);

s32 func_00284298(s32 x, s32 y, s32 z, s32 amount, s32 unused, u8 *object, s32 packet) {
    f32 scale;

    switch ((*(u32 *)object >> 19) & 0xF) {
    case 6:
        scale = ((MantraPanelSpriteView *)object)->animationStep * 0.25f;
        amount = amount * scale;
        func_00283FA8(x, y, z, amount, unused, object, packet);
        break;
    case 8:
        scale = ((MantraPanelSpriteView *)object)->animationStep / 10.0f;
        amount = amount * scale;
        func_00283FA8(x, y, z, amount, unused, object, packet);
        break;
    case 7:
        scale = ((MantraPanelSpriteView *)object)->animationStep * 0.25f;
        scale = 1.0f - scale;
        amount = amount * scale;
        func_00283FA8(x, y, z, amount, unused, object, packet);
        break;
    case 9:
        scale = ((MantraPanelSpriteView *)object)->animationStep / 10.0f;
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

u32 func_002843C0(s16 kind) {
    MantraSparkleEmitter *emitter = (MantraSparkleEmitter *)func_00328D68(0xA8);
    MantraSparkle *spark;

    memset(emitter, 0, 0xA8);
    emitter->duration = func_00341240(0) * 20.0f + 5.0f;
    emitter->kind = kind;
    spark = func_00284818(emitter);
    spark->age = spark->life * func_00341240(0) + 0.0f;
    spark = func_00284818(emitter);
    spark->age = spark->life * func_00341240(0) + 0.0f;
    spark = func_00284818(emitter);
    spark->age = spark->life * func_00341240(0) + 0.0f;
    return (u32)emitter;
}


void func_002844E8(u32 sprite) {
    if (sprite != 0) {
        func_00328E48(sprite);
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00284508);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00284818);

/* The burst allocation starts with a 16-byte header, then 100 slots. */
typedef struct MantraBurstHeader {
    u32 allocation;
    u32 slots;            /* 0x04 */
    u32 count;            /* 0x08 */
    u32 reserved;         /* 0x0C */
} MantraBurstHeader;

u32 func_00284AE0(void) {
    u32 handle = func_003292A8(0x650);
    u32 block = sdfMemoryGetBlockAddress(handle);
    memset((void *)block, 0, 0x650);
    *(u32 *)block = handle;
    ((MantraBurstHeader *)block)->slots = block + 0x10;
    ((MantraBurstHeader *)block)->count = 0x64;
    return block;
}

void func_00284B48(u32 *obj) {
    if (*obj != 0) {
        func_003297C8(*obj);
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00284B70);

typedef struct MantraBurstSlot {
    f32 x;
    f32 y;
    union {
        u32 flags;
        s16 half[2];
    } w;
    u16 age;
    s16 life;
} MantraBurstSlot;

typedef struct MantraBurstPool {
    u32 unk0;
    MantraBurstSlot *slots;
    s32 count;
    u16 tick;
} MantraBurstPool;

extern f32 func_003407A0(f32);

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
                slot->w.half[1] = (s32)(func_00341240(0) * 10.0f) * 36;
            } else {
                slot->w.half[1] = (s32)(func_00341240(0) * 20.0f) * 18;
            }
            slot->life = 14;
            if (side != 0) {
                slot->x = (base + 39.0f) * func_003407A0(slot->w.half[1] * 0.017453293f);
                slot->y = (base + 39.0f) * sdfSinPoly(slot->w.half[1] * 0.017453293f);
            } else {
                slot->x = (base + 19.0f) * func_003407A0(slot->w.half[1] * 0.017453293f);
                slot->y = (base + 19.0f) * sdfSinPoly(slot->w.half[1] * 0.017453293f);
            }
            return slot;
        }
    }
    return 0;
}

u32 func_002850B8(void) {
    u32 handle = func_003292A8(0x650);
    u32 block = sdfMemoryGetBlockAddress(handle);
    memset((void *)block, 0, 0x650);
    *(u32 *)block = handle;
    ((MantraBurstHeader *)block)->slots = block + 0x10;
    ((MantraBurstHeader *)block)->count = 0x64;
    return block;
}

void func_00285120(u32 *obj) {
    if (*obj != 0) {
        func_003297C8(*obj);
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
                slot->w.half[1] = (s32)(func_00341240(0) * 10.0f) * 36;
            } else {
                slot->w.half[1] = func_00341240(0) * 360.0f;
            }
            slot->life = 30;
            if (side != 0) {
                slot->x = (base + 50.0f) * func_003407A0(slot->w.half[1] * 0.017453293f);
                slot->y = (base + 25.0f) * sdfSinPoly(slot->w.half[1] * 0.017453293f);
            } else {
                slot->x = (base + 100.0f) * func_003407A0(slot->w.half[1] * 0.017453293f);
                slot->y = (base + 30.0f) * sdfSinPoly(slot->w.half[1] * 0.017453293f);
            }
            return slot;
        }
    }
    return 0;
}

u32 mnuRequestEffectResource(u32 context, u32 config) {
    MantraEffectResource *resource = (MantraEffectResource *)func_00328D68(sizeof(MantraEffectResource));
    memset(resource, 0, sizeof(MantraEffectResource));
    effRequestResourceByMode(context, config, 0, &resource->handle);
    return (u32)resource;
}

u8 mnuHasEffectResourceHandle(MantraEffectResource *resource) {
    return resource->handle != 0;
}

void mnuReleaseEffectResource(MantraEffectResource *resource) {
    effDestroyResourceSlotSet(resource->handle);
    func_00328E48(resource);
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00285788);

typedef struct MantraSpark {
    s16 x;
    s16 y;
    s16 timer;
    s16 timerMax;
    s16 z;
    s8 phase;
    u8 size;
} MantraSpark;

void mnuUpdateSparkle(MantraSpark *);

void func_00285800(s32 *state) {
    MantraSpark *particle;
    s32 value;
    s32 i;

    state[0] = state[0] - 1;
    if (state[0] < 0) {
        value = func_00341240(0) * 60.0f + 60.0f;
        state[1] = value;
        state[0] = value;
        *(f32 *)(state + 2) = func_00341240(0) * 0.20000005f + 0.4f;
    }
    particle = (MantraSpark *)(state + 3);
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
            spark->timer = spark->timerMax = func_00341240(0) * 30.0f + 1.0f;
        } else if (func_00341240(0) > 0.7f) {
            spark->timer = spark->timerMax = func_00341240(0) * 30.0f + 0.0f;
        } else {
            spark->phase = 1;
            spark->timer = spark->timerMax = func_00341240(0) * 60.0f + 45.0f;
            spark->z = (func_00341240(0) * 2.0f - 1.0f) * 512.0f + 320.0f;
            spark->size = func_00341240(0) * 10.0f;
            spark->x = func_00341240(0) * 512.0f + -256.0f;
            spark->y = func_00341240(0) * 448.0f + -128.0f;
        }
    }
}


void func_00285A60(u8 *object) {
    if (*(s8 *)(object + 0xA) != 0) {
        sdfSinPoly((1.0f - (f32)*(s16 *)(object + 4) / (f32)*(s16 *)(object + 6)) * 3.14159265f);
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

u32 func_00285BD8(u32 count) {
    u32 size = count * 12 + 0x28;
    u32 handle = func_003292A8(size);
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
    ((MantraEffectPoolHeader *)block)->callback = func_00286270;
    ((MantraEffectPoolHeader *)block)->counterC = 0;
    ((MantraEffectPoolHeader *)block)->counterE = 0;
    return block;
}

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378F0);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378F8);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_00437900);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_00437908);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_00437910);

