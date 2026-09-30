#include "mnu.h"





























typedef struct PartyEntryCopy {
    u16 flags;
    u16 pad02;
    u32 word[0x68];
} PartyEntryCopy; /* 0x1A4 bytes */

typedef struct PartyMenuData {
    u8 pad00[0x840];
    PartyEntryCopy current[5];
    s32 activeCount; /* 0x1074 */
    PartyEntryCopy backup[5];
    s32 selection; /* 0x18AC */
} PartyMenuData;

/* Staff/skill menu context: resource handles and current panel work. */
typedef struct CampMenuContext {
    u8 pad00[0x64];
    s32 resource;             /* 0x64 */
    u8 pad68[4];
    s32 displayVariant;       /* 0x6C: passed with display to menu drawing */
    u8 pad70[4];
    s32 option;               /* 0x74 */
    s32 actor;                /* 0x78 */
    s32 staffVariant;         /* 0x7C: passed with display to menu drawing */
    s32 staffParam;           /* 0x80 */
    u8 pad84[0x5C];
    s32 variant;              /* 0xE0 */
    u8 padE4[0x40];
    s32 panel;                /* 0x124 */
    u8 pad128[4];
    s32 panelList;            /* 0x12C */
    u8 pad130[8];
    s32 display;              /* 0x138 */
    u8 pad13C[0x69C];
    s32 selectionList;        /* 0x7D8 */
    u8 pad7DC[0x10];
    s32 activePanel;          /* 0x7EC: active party panel */
    s32 finalPanelSlot;       /* 0x7F0: last displayed slot */
    u8 pad7F4[0x104];
    s32 sceneGroup;           /* 0x8F8 */
    s32 sprite;               /* 0x8FC */
    s32 effect;               /* 0x900 */
    u8 pad904[8];
    s32 menu;                 /* 0x90C */
    u8 pad910[0x10];
    s32 extraResource;        /* 0x920 */
} CampMenuContext;

typedef struct StaffMenuWork {
    s32 handle;               /* 0x00 */
    u8 pad04[0xC];
    s32 staffMode;            /* 0x10 */
    s32 staffImage;           /* 0x14 */
    u8 pad18[8];
    s32 resourceList;         /* 0x20 */
    s32 selectedList;         /* 0x24 */
    s32 activeMark;           /* 0x28 */
    u8 pad2C[4];
    u32 selectionFlags;       /* 0x30 */
    u32 selectionId;          /* 0x34 */
} StaffMenuWork;

typedef struct MenuSelectionNode {
    s32 index;                /* 0x00 */
    u8 pad04[0x44];
    u32 flags;                /* 0x48 */
    u8 pad4C[0xC];
    struct MenuSelectionNode *next; /* 0x58 */
    u8 pad5C[4];
    u32 sortKey;              /* 0x60 */
} MenuSelectionNode;

typedef struct MenuSelectionList {
    u8 pad00[0x10];
    MenuSelectionNode *first; /* 0x10 */
    u8 pad14[8];
    s32 *selectedSlot;        /* 0x1C: current menu selection */
} MenuSelectionList;

typedef struct MenuSelectionState {
    u8 pad00[0x14];
    MenuSelectionList *list;  /* 0x14 */
} MenuSelectionState;

typedef struct MenuInputInfo {
    u8 pad00[0x48];
    u32 flags;           /* 0x48: selectable state */
    u8 pad4C[0x14];
    s32 target;          /* 0x60: current selection target */
} MenuInputInfo;

typedef struct MenuInputFlags {
    u32 bits;            /* 0x00: clear bit 3 each frame */
    u8 pad04[0x18];
    MenuInputInfo *info; /* 0x1C */
} MenuInputFlags;

typedef struct MenuInputNode {
    u8 pad00[0x14];
    MenuInputFlags *flags; /* 0x14 */
} MenuInputNode;

extern void mnuForwardDupArg(s32, s32, s32, s32, s32);
extern void mnuSeekListNode(s32, s32);

typedef struct PartyMenuHead {
    u8 pad00[8];
    MenuSelectionState *selection; /* 0x08: linked party selector */
} PartyMenuHead;

typedef struct PartyPanelSlot {
    u32 flags;
    u8 pad04[0x130]; /* next slot at +0x134 */
} PartyPanelSlot;

typedef struct PartySkillSlots {
    u8 pad00[0x22];
    u16 code[8]; /* 0x22: indexed equipped skill codes */
} PartySkillSlots;

typedef struct MenuItemCount {
    u8 pad00[0x20];
    s32 count; /* 0x20 */
} MenuItemCount;

typedef struct MenuSpriteArguments {
    u8 pad00[0x55];
    s8 variant; /* 0x55: passed to sprite renderer */
} MenuSpriteArguments;

extern u32 func_00285B20(u32);
extern void func_0027BEB0();
extern void func_0027BE90();
extern void mnuClearListFlagsOneAndTwo();
extern void sndSetSequenceVolumePan();
extern void func_00276898();
extern void func_002858E8();
extern void func_002858F8(s32, void *);
extern void func_0027C788(s32);
extern void func_0027C770(s32);
extern void func_0027C758(s32);
extern void func_0027C658(s32);
extern void mnuPlayInputSound(s32, u32, s32);
extern u8 D_0037CC74[];
extern u8 D_0037CC3C[];
extern u8 D_0037CAB0[];
extern u8 D_0037CA78[];
extern void func_00280978();
extern void mnuClearListFlags();
extern s32 func_002D03F8(s32);
extern s32 *sdfResourceRetainAddress(s32);
extern void func_00287450();
extern void func_0027E790();

extern s32 func_002877A8(void);

extern s32 func_00101A70();

extern s32 D_003BAA00;

extern s32 D_003BAA7C;
extern s32 func_00197EC8();
extern s32 D_003BAA70;
extern void func_00283110();
extern void func_002833B0();

extern void mnuDestroyWindowContainer(u32);

extern s64 func_00285670(s32, s32 *, u64, u64);
extern u8 D_0037CA58[];

void func_00274B80(u32 arg0) {
    mnuSetStaffDisplayMode(4, arg0);
}

void func_00274BA0(s32 context) {
}

s32 mnuIsFinalItemIndex(s32 index, s32 item) {
    if (index < (((MenuItemCount *)item)->count - 1)) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00274BC0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00274D48);

void func_00274EE0(s32 arg0) {
    mnuDestroyWindowContainer((u32)((PartyMenuHead *)((CampMenuContext *)arg0)->menu)->selection);
}

/* Snapshot five party entries; at most three active slots are displayed. */
void mnuCopyPartyEntries(context)
    s32 context;
{
    PartyMenuData *menu = (PartyMenuData *)((CampMenuContext *)context)->menu;
    PartyEntryCopy *destination = menu->current;
    s32 i;
    s32 flagsOffset = 0xA60;
    s32 sourceOffset = 0;

    menu->activeCount = 0;
    for (i = 0; i < 5; i++) {
        *destination = *(PartyEntryCopy *)(sourceOffset + D_003BAA00 + 0xA60);
        if (((PartyEntryCopy *)(D_003BAA00 + flagsOffset))->flags & 1) {
            menu->activeCount = menu->activeCount + 1;
        }
        flagsOffset += 0x1A4;
        destination++;
        sourceOffset += 0x1A4;
    }
    if (menu->activeCount >= 4) {
        menu->activeCount = 3;
    }
    menu->selection = 0;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00275030);

void mnuRestorePartyEntriesAndRefresh(context)
    s32 context;
{
    PartyMenuData *menu = (PartyMenuData *)((CampMenuContext *)context)->menu;
    u16 *entry = (u16 *)menu->current;
    s32 i;
    s32 backupOffset;
    s32 panel;

    for (i = 0; i < 5; i++) {
        if (*entry & 1) {
            func_00275030(i, -3, 1, context);
        }
        entry += 0x1A4 / 2;
    }
    backupOffset = 0;
    for (i = 4; i >= 0; i--) {
        /* Required to match: offset-first arithmetic into menu->backup. */
        *(PartyEntryCopy *)(backupOffset + D_003BAA00 + 0xA60) = *(PartyEntryCopy *)(backupOffset + (s32)menu + 0x1078);
        backupOffset += 0x1A4;
    }
    panel = context + 0x15C;
    func_0027F0D8(panel);
    mnuInitPartyPanelSlots(context + 0x7EC);
    mnuUpdateHandleStates(panel);
    func_00280048(panel);
}

s32 mnuCountActiveSlots(void) {
    s32 i;
    s32 active = 0;
    u16 *slotFlags = (u16 *)(D_003BAA00 + 0xa60);

    for (i = 4; i >= 0; i--) {
        active += *slotFlags & 1;
        slotFlags += 210;
    }
    return (active < 4) ? active : 3;
}

extern void mnuCopyPartyEntries();

void mnuClearPartySelectionAndActivateSlots(s32 context) {
    s32 menu = ((CampMenuContext *)context)->menu;
    s32 i;
    s32 node;

    mnuCopyPartyEntries();
    ((PartyMenuData *)menu)->selection = 0;
    memset(((PartyMenuData *)menu)->backup, 0, 0x834);
    ((CampMenuContext *)context)->activePanel = 1;
    ((CampMenuContext *)context)->finalPanelSlot = mnuCountActiveSlots() - 1;
    mnuUpdateHandleStates(context + 0x15C);
    for (i = 0; i < 5; i++) {
        ((PartyPanelSlot *)(context + 0x1D8))[i].flags |= 0x80;
    }
    for (node = (s32)((PartyMenuHead *)menu)->selection->list->first; node != 0; node = (s32)((MenuSelectionNode *)node)->next) {
        ((MenuSelectionNode *)node)->flags &= ~1;
    }
}

void mnuRefreshPartyPanelSlots(s32 context) {
    func_0027F0D8(context + 0x15c);
    mnuInitPartyPanelSlots(context + 0x7ec);
    mnuUpdateHandleStates(context + 0x15c);
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_002755E0);

s32 mnuShopReleaseResources(void) {
    s32 context = func_00101A70();
    s32 menu = ((CampMenuContext *)context)->menu;
    mnuRefreshPartyPanelSlots(context);
    func_00274EE0(context);
    func_00274BA0(context);
    func_002D0918(*(s32 *)menu);
    return 1;
}

void func_002758D8(s32 menu) {
    mnuRestorePartyEntriesAndRefresh();
    func_002858F8(menu + 0x54, (s32)D_0037CA58);
    func_0027E790(((CampMenuContext *)menu)->display, ((CampMenuContext *)menu)->displayVariant, 0, 1);
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00275920);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00275B40);

typedef struct StaffFadeState {
    u8 pad0[0x1BF8];
    s32 fadeA;
    s32 fadeB;
} StaffFadeState;

/* Two-stage fade: B rises first when opening, A falls first when closing. */
void mnuUpdateStaffFade(s32 opening, StaffFadeState *state) {
    if (opening == 0) {
        if (state->fadeA > 0) {
            state->fadeA -= 0x10;
        }
        if (state->fadeA < 0) {
            state->fadeA = 0;
        }
        if (state->fadeA < 0x50) {
            if (state->fadeB > 0) {
                state->fadeB -= 0x10;
            }
            if (state->fadeB < 0) {
                state->fadeB = 0;
            }
        }
    } else {
        if (state->fadeB < 0x100) {
            state->fadeB += 0x10;
        }
        if (state->fadeB > 0x100) {
            state->fadeB = 0x100;
        }
        if (state->fadeB > 0xB0) {
            if (state->fadeA < 0x100) {
                state->fadeA += 0x10;
            }
            if (state->fadeA > 0x100) {
                state->fadeA = 0x100;
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00276018);

s64 func_002761C0(s32 callback) {
    s32 context = func_00101A70();
    PartyMenuHead *menu = (PartyMenuHead *)((CampMenuContext *)context)->menu;

    func_00272778(callback);
    mnuCreateStaffImageSprite(0x13);
    func_0027CDD0(0x1C0, 0x3D0, 0, menu->selection, 0x53);
    func_00276018(context);
    func_002723B0(0, ((CampMenuContext *)context)->actor);
    return menuRunPanel(context, 1, callback);
}

s64 func_00276250(s32 callback) {
    return menuRunPanel(func_00101A70(), 2, callback);
}

u8 mnuIsStateNotOne(void) {
    s64 result;

    result = func_002877A8();
    return result != 1;
}

void func_002762B0(u32 arg0) {
    mnuSetStaffDisplayMode(3, arg0);
}

void func_002762D0() {
}

void func_002762D8(s32 *menu) {
    s32 i;
    for (i = 0; i < 2; i++) {
        effResolveAndReleaseResource(menu[7 + i]);
    }
}

void func_00276320(s32 *menu) {
    s32 i;
    for (i = 0; i < 2; i++) {
        func_002BD870(menu[7 + i]);
    }
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00276368);

s32 func_00276428(void) {
    s32 context = func_00101A70();
    s32 menu = ((CampMenuContext *)context)->menu;
    mnuResetWorkFloats();
    func_002762D0(context);
    func_002D0918(*(s32 *)menu);
    return 1;
}

void func_00276478(u32 arg0) {
    func_00276368(arg0, 1);
}

void func_00276490(void) {
    func_00276428();
}

void func_002764A8(u32 arg0) {
    func_00276368(arg0, 0);
}

void func_002764C0(void) {
    func_00276428();
}

s64 func_002764D8(s32 callback) {
    s32 context = func_00101A70();
    u8 *menu = (u8 *)((CampMenuContext *)context)->menu;
    s32 *popup = (s32 *)(context + 0x54);
    u32 buttons = func_00285B20(3);
    s64 state;
    s32 window;
    state = func_00285670(context + 8, popup, 0, callback);
    if (state != 0) {
        return state;
    }
    if (*popup == 0) {
        window = context + 0x15C;
        func_00280978(4, window);
        if (buttons & 1) {
            *(s32 *)(menu + 0x18) = **(s32 **)(((CampMenuContext *)context)->selectionList + 0x1C);
            func_002858E8(popup, D_0037CAB0);
            *(s32 *)(menu + 0x24) = 1;
        }
        if (buttons & 2) {
            func_002858F8((s32)popup, D_0037CA78);
            func_0027E790(((CampMenuContext *)context)->display, ((CampMenuContext *)context)->displayVariant, 0, 1);
            mnuClearListFlags(0, window);
        }
        mnuPlayInputSound(0, buttons, 0);
    }
    return 0;
}

extern u8 D_0037C3A8[];
extern s32 func_002BD8F8(s32);
extern void mnuDrawStaffCampScreen();
extern void mnuCreateStaffImageSprite();
extern void func_002723B0();
extern void func_00272668();
extern void func_0027CDD0();

s64 func_002765E8(s32 arg0) {
    s32 context = func_00101A70();
    CampMenuContext *work = (CampMenuContext *)context;
    StaffMenuWork *menu = (StaffMenuWork *)work->menu;

    if (func_002BD8F8(work->resource) != 0) {
        mnuDrawStaffCampScreen(0, arg0);
    } else {
        mnuDrawStaffCampScreen(1, arg0);
    }
    if (menu->staffImage == 0) {
        mnuCreateStaffImageSprite(0x12);
    } else {
        mnuCreateStaffImageSprite(0x11);
    }
    func_0027CDD0(0x1C0, 0x3D0, 0, work->panel, 0x53);
    func_002723B0(0, work->actor);
    if (menu->selectedList != 0) {
        s32 *slot = ((MenuSelectionState *)work->panel)->list->selectedSlot;

        func_00272668(1, *slot, D_0037C3A8, context, 1, 0x53, slot);
    }
    return menuRunPanel(context, 1, arg0);
}

s64 func_002766E8(s32 callback) {
    return menuRunPanel(func_00101A70(), 2, callback);
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00276720);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00276898);

extern void func_00276720();
extern void btlStopStage();
extern void mnuClearEntries();
extern void func_0027FA20();
extern void mnuDestroyPanelGroup();
extern void func_002832F8();
extern void func_00283820();
extern void func_00285160();
extern void mnuReleaseResourceList();
extern void func_0027E6B8();

/* Tear down the staff panel and all four optional scene-side resources. */
s32 mnuStaffReleasePanelScene(s32 unused) {
    s32 context = func_00101A70();
    StaffMenuWork *menu = (StaffMenuWork *)((CampMenuContext *)context)->menu;
    s32 entryList = context + 0x15C;
    CampMenuContext *work = (CampMenuContext *)context;

    func_00276720(entryList, 0, menu->staffImage, menu->staffMode);
    btlStopStage();
    mnuClearEntries(entryList);
    func_0027FA20(entryList);
    if (work->sceneGroup != 0) {
        mnuDestroyPanelGroup(work->sceneGroup);
        work->sceneGroup = 0;
    }
    if (work->sprite != 0) {
        func_002832F8(work->sprite);
        work->sprite = 0;
    }
    if (work->effect != 0) {
        func_00283820(work->effect);
        work->effect = 0;
    }
    if (work->extraResource != 0) {
        func_00285160(work->extraResource);
        work->extraResource = 0;
    }
    mnuReleaseResourceList(menu->resourceList);
    effResolveAndReleaseResource(work->resource);
    func_0027E6B8(work->display, work->resource, 0, 0);
    return 1;
}

void func_00276B10(s32 arg0) {
    *(u32 *)
      (*(s32 *)(**(s32 **)(*(s32 *)(arg0 + 0x7d8) + 0x1c) * 0x134 + arg0 + 0x2b4) + 0x3c) = 0x100
    ;
}

s32 func_00276B38(s32 arg0) {
    s32 context = func_00101A70();
    StaffMenuWork *menu = (StaffMenuWork *)((CampMenuContext *)context)->menu;
    s32 changed = 0;
    u32 input = func_00285B20(0x300);

    if (input & 0x100) {
        mnuStaffReleasePanelScene(arg0);
        func_0027BEB0(*(s32 *)(context + 0x7D8));
        changed = 1;
    }
    if (input & 0x200 && changed == 0) {
        mnuStaffReleasePanelScene(arg0);
        func_0027BE90(*(s32 *)(context + 0x7D8));
        changed = 1;
    }
    mnuClearListFlagsOneAndTwo(*(s32 *)(context + 0x7D8));
    if (changed != 0) {
        func_00276898(arg0);
        sndSetSequenceVolumePan(4, 0x7F, 0x3F);
        menu->activeMark = 0;
        if (menu->staffMode == 1) {
            func_00276B10(context);
        }
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00276C28);

void func_00276DA0(s32 x, s32 context) {
    s32 slot = D_003BAA00 + **(s32 **)(*(s32 *)(context + 0x67C) + 0x1c) * 0x1a4 + 0xa60;
    s32 i;
    s32 y;
    s32 handle;

    func_00197220(0x13);
    x = x * 8;
    y = x + 0xbc0;
    for (i = 0; i < 3; i++, y += 0xa8) {
        handle = func_00197EC8(0x190, y, 0, 0xa09dc359, D_003BAA7C + *(u16 *)(slot + 4) * 45, i);
        if (handle != 0) {
            func_001958A0(handle, 1, 0x53);
            func_00194920(handle);
        }
    }
    func_00197220(-1);
}

extern void func_002BF790(s32, s32, s32, s32, s32, s32, s32);

void func_00276E90(s32 flag, s32 obj) {
    s32 y;

    for (y = 0x360; y < 0xE40; y += 0x38) {
        func_002BF790(0xE80, y, 0, 1, ((StaffMenuWork *)obj)->resourceList, 2, 0x53);
    }
    func_002BF790(0x10F0, 0x358, 0, 1, ((StaffMenuWork *)obj)->resourceList, 4, 0x53);
    func_002BF790(0x1050, 0x500, 0, 1, ((StaffMenuWork *)obj)->resourceList, 3, 0x53);
    if (flag == 0) {
        func_002BF790(-0x140, -0xA0, 0, 1, ((StaffMenuWork *)obj)->resourceList, 7, 0x53);
    }
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00276F70);

extern void func_00195520(s32);
extern s32 func_00195160(s32, s32, s32, s32, s32);
extern void func_00195450(s32, s32, s32);
extern void func_00195460(s32, s32);
extern void frFontSetChildColors(s32, u32);
extern void func_00195530(s32);
extern void func_001958A0(s32, s32, s32);
extern void func_00194920(s32);

void mnuDrawTextSprite(s32 x, s32 y, s32 scale, s32 color, s32 textId, s32 param) {
    s32 item;
    s32 top = y - 0x10;

    func_00195520(1);
    item = func_00195160(textId, 0, 0, 0, 0);
    func_00195450(item, x, top);
    func_00195460(item, scale * 0x10);
    frFontSetChildColors(item, color);
    func_00195530(1);
    func_001958A0(item, 1, param);
    func_00194920(item);
}

void func_00277220(u8 *entry, s32 id, s32 packedGroup, s32 group, s32 obj, s32 spriteFlags) {
    func_00283110(0xeb0, 0x518, 0, entry, packedGroup, spriteFlags);
    func_002833B0(0, 0, 0, entry, group, spriteFlags);
    func_002BF790(0xb0, 0xa68, 0, 1, *(s32 *)(obj + 0x1c), 0x37, spriteFlags);
    mnuDrawTextSprite(0x220, 0xa20, 0, 0xa09dc380, D_003BAA70 + *(u16 *)(entry + 4) * 17 + 0x110, spriteFlags);
    func_002BF790(0x120, 0xad0, 0, 1, *(s32 *)(obj + 0x14), 0x25, spriteFlags);
    func_00276DA0(-0x16, id);
}

extern void func_00283838(s32, s32, s32, s32, s32, s32, s32);
extern void func_00285440(s32, s32, s32, s32, s32);

void func_00277328(s32 obj, s32 unused1, s32 arg2, s32 arg3, s32 unused4, s32 arg5) {
    func_00283838(0, 0, 0, obj, ((MenuSpriteArguments *)obj)->variant, arg2, arg5);
    func_00285440(0x1200, 0x730, 0, arg3, arg5);
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00277390);

extern u32 effMiscRand(s32);
extern s32 func_00287FE0();
extern u32 evtStageTestCountFlags(s32);
extern void evtStageTestQueueMotion(s32, u32);

typedef struct MenuIdleVoiceState {
    u8 pad00[0x28];
    s32 idleTicks;
    s32 voiceState; /* -1 permits a new random voice */
} MenuIdleVoiceState;

#define MENU_IDLE_VOICE_TICKS 0x12D

/* Pick another idle voice after MENU_IDLE_VOICE_TICKS eligible ticks. */
void mnuIdleVoiceTimer(MenuIdleVoiceState *voiceTimer) {
    u32 voiceCount;

    if (voiceTimer->voiceState == -1 && func_002877A8() != 1) {
        if (func_00287FE0() == 0) {
            voiceTimer->idleTicks = voiceTimer->idleTicks + 1;
        }
        if (voiceTimer->idleTicks >= MENU_IDLE_VOICE_TICKS) {
            voiceCount = evtStageTestCountFlags(0);
            evtStageTestQueueMotion(0, effMiscRand(0) % voiceCount);
            voiceTimer->idleTicks = 0;
        }
    }
}

s64 func_002775D8(s32 callback) {
    s32 context = func_00101A70();
    StaffMenuWork *menu = (StaffMenuWork *)((CampMenuContext *)context)->menu;

    if (menu->staffMode == 0) {
        mnuIdleVoiceTimer((MenuIdleVoiceState *)menu);
    }
    return menuRunPanel(context, 2, callback);
}

u32 func_00277638(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00277640);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00277848);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2208);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2260);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2270);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2280);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2290);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B22A0);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B22B0);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B22C0);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B22D0);

INCLUDE_ASM(const s32, "game/code_00274B80", ptySkillMenuBuildEquippedSlots);

u32 func_00277C80() {
    s32 context = func_00101A70();
    StaffMenuWork *menu = (StaffMenuWork *)((CampMenuContext *)context)->menu;
    mnuDestroyWindowContainer(menu->selectedList);
    menu->selectedList = 0;
    return 1;
}

void func_00277CB8(void) {
    StaffMenuWork *menu = (StaffMenuWork *)((CampMenuContext *)func_00101A70())->menu;
    MenuSelectionState *state = (MenuSelectionState *)menu->selectedList;
    MenuSelectionNode *node = state->list->first;

    while (node != NULL) {
        u32 key = node->sortKey;
        if (!(node->flags & 1) && key != 0) {
            break;
        }
        node = node->next;
    }
    if (node != NULL) {
        mnuSeekListNode(node->index, (s32)((MenuInputNode *)state)->flags);
    }
}

u32 ptySkillMenuRebuildAfterMutation(s32 arg0, s32 arg1) {
    StaffMenuWork *menu = (StaffMenuWork *)((CampMenuContext *)func_00101A70(arg1))->menu;
    s32 selected = *((MenuSelectionState *)menu->selectedList)->list->selectedSlot;
    s32 state;

    func_00277C80(arg1);
    ptySkillMenuBuildEquippedSlots(arg0, arg1);
    mnuSeekListNode(selected, (s32)((MenuSelectionState *)menu->selectedList)->list);
    state = menu->selectedList;
    *(s32 *)(state + 0x44) = 0x200;
    *(s32 *)(state + 0x48) = 0x100;
    return state;
}

void func_00277DD0(u32 arg0) {
    mnuSetStaffDisplayMode(1, arg0);
}

void func_00277DF0(s32 context) {
}

INCLUDE_ASM(const s32, "game/code_00274B80", ptySkillMenuDrawEntry);

INCLUDE_ASM(const s32, "game/code_00274B80", ptySkillMenuRefreshEntries);

typedef struct SkillInfo {
    u8 pad0[0x20];
    u32 count;
    u16 codes[14];
} SkillInfo;

extern s32 func_002CFEB8(s32);
extern void prfBuildRawSkillList(u32, SkillInfo *);

s32 mnuBuildSkillCodeBitset(void) {
    s32 id = 0;
    u32 *bits = (u32 *)func_002CFEB8(0x50);
    SkillInfo info;

    memset(bits, 0, 0x50);
    for (id = 0; id < 0x60; id++) {
        u32 i;

        prfBuildRawSkillList(id & 0xFFFF, &info);
        for (i = 0; i < info.count; i++) {
            u16 code = info.codes[i];

            if (code != 0) {
                bits[code >> 5] |= 1 << code;
            }
        }
    }
    return (s32)bits;
}

void func_002782E0(void) {
    func_002CFF98();
}

s32 mnuIsSkillCodeInBitset(s32 code, u32 *bits) {
    s32 roundedCode = (code < 0) ? code + 0x1f : code;

    return (bits[roundedCode >> 5] & (1 << code)) != 0;
}

INCLUDE_ASM(const s32, "game/code_00274B80", ptySkillMenuInitPages);

void func_002786E8(s32 context) {
    u32 *menu = (u32 *)((CampMenuContext *)context)->menu;
    if (menu[2] != 0) {
        u32 i = 0;
        u32 *resource = menu + 4;
        mnuDestroyPanelState(menu[8]);
        func_0027B368(menu[3]);
        do {
            mnuDestroyWindowContainer(*resource++);
            i++;
        } while (i < 4);
        menu[2] = 0;
    }
}

s32 mnuCampMenuInit(void) {
    s32 context = func_00101A70();
    s32 handle = func_002D03F8(0x38);
    s32 *menu = sdfResourceRetainAddress(handle);
    CampMenuContext *work = (CampMenuContext *)context;

    work->menu = (s32)menu;
    memset(menu, 0, 0x38);
    *menu = handle;
    func_00277DD0(context);
    mnuForwardDupArg(work->panelList, work->option, 0, 0, 0);
    switch (*((MenuSelectionState *)work->panel)->list->selectedSlot) {
    case 0:
        func_0027E790(work->display, work->variant, 0, 1);
        break;
    case 2:
        func_0027E790(work->display, work->variant, 0x35, 0x36);
        break;
    default:
        func_0027E790(work->display, work->variant, 0x33, 0x34);
        break;
    }
    mnuSeekListNode(0, ((MenuSelectionState *)work->panelList)->list);
    return 1;
}

s32 func_00278868(s32 arg0) {
    s32 context = func_00101A70();
    StaffMenuWork *menu = (StaffMenuWork *)((CampMenuContext *)context)->menu;

    if (menu->selectedList != 0) {
        func_00277C80(arg0);
    }
    func_00277DF0(context);
    func_002D0918(menu->handle);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00274B80", ptySkillMenuShellUpdate);

s64 mnuCampMenuDrawSlotLabel(s32 param) {
    s32 context = func_00101A70();
    s32 *slot;

    func_00272778(param);
    slot = ((MenuSelectionState *)((CampMenuContext *)context)->panel)->list->selectedSlot;
    if (*slot == 0) {
        mnuCreateStaffImageSprite(1);
    } else {
        mnuCreateStaffImageSprite(0xA);
    }
    func_00272668(1, *((MenuSelectionState *)((CampMenuContext *)context)->panel)->list->selectedSlot, D_0037C3A8, context, 1, 0x53);
    if (*((MenuSelectionState *)((CampMenuContext *)context)->panel)->list->selectedSlot == 0) {
        func_0027CDD0(0x1C0, 0x3D0, 0, ((CampMenuContext *)context)->panel, 0x53);
    } else {
        func_0027CDD0(0x1C0, 0x3D0, 0, ((CampMenuContext *)context)->panel, 0x53);
    }
    func_002723B0(0, ((CampMenuContext *)context)->actor);
    return menuRunPanel(context, 1, param);
}

s64 func_00278B90(s32 callback) {
    return menuRunPanel(func_00101A70(), 2, callback);
}

void func_00278BC8(void) {
    s32 context = func_00101A70();
    ((StaffMenuWork *)((CampMenuContext *)context)->menu)->selectionId = 0xffffffff;
}

u32 func_00278BF0(void) {
    s32 context = func_00101A70();
    return ~((StaffMenuWork *)((CampMenuContext *)context)->menu)->selectionId >> 0x1f;
}

void func_00278C20(void) {
    StaffMenuWork *menu = (StaffMenuWork *)((CampMenuContext *)func_00101A70())->menu;
    MenuSelectionNode *node = ((MenuSelectionState *)menu->selectedList)->list->first;

    for (; node != 0; node = node->next) {
        if (node->index == menu->selectionId) {
            node->flags |= 2;
        } else {
            node->flags &= ~2;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00278C90);

u32 func_00278D68(void) {
    s32 context = func_00101A70();
    ((StaffMenuWork *)((CampMenuContext *)context)->menu)->selectionFlags = 0;
    return 1;
}

extern void ptyRecomputeMaxHpMp();
extern void scrClearSecondaryScriptFlag();

void mnuAddPartySkillIfMissing(s32 obj, s32 id, s32 slot) {
    u16 code = id;

    if (ptyHasSkill(obj, code) == 0) {
        ((PartySkillSlots *)obj)->code[slot] = code;
        ptyRecomputeMaxHpMp(obj);
        scrClearSecondaryScriptFlag(obj, code);
    }
}

void mnuClearPartySkillSlot(s32 arg0, s32 arg1) {
    ((PartySkillSlots *)arg0)->code[arg1] = 0;
    ptyRecomputeMaxHpMp();
}

void mnuCampMenuHandleInput(void) {
    s32 context = func_00101A70();
    s32 menu = ((CampMenuContext *)context)->menu;
    u32 input = func_00285B20(0x33);
    s32 node = ((StaffMenuWork *)menu)->selectedList;
    s32 flags = (s32)((MenuInputNode *)node)->flags;

    ((MenuInputFlags *)flags)->bits &= ~8;
    if (input & 1) {
        s32 info = (s32)((MenuInputFlags *)flags)->info;
        s32 target = ((MenuInputInfo *)info)->target;

        if (!(((MenuInputInfo *)info)->flags & 1) && target != 0) {
            func_002858E8(context + 0x54, D_0037CC74);
        } else {
            input = 0x8000;
        }
    }
    if (input & 2) {
        func_002858F8(context + 0x54, D_0037CC3C);
    }
    if (node != 0) {
        if (!(input & 0x300000)) {
            func_0027C788(node);
        }
        if (input & 0x10) {
            func_0027C770(node);
        }
        if (input & 0x20) {
            func_0027C758(node);
        }
        func_0027C658(node);
        mnuPlayInputSound(0, input, (s32)((MenuInputNode *)node)->flags);
    }
}

INCLUDE_ASM(const s32, "game/code_00274B80", ptySkillMenuHandleSelection);

void mnuSwapPartySkillSlots(s32 entry, s32 firstSlot, s32 secondSlot) {
    u8 *slotBase = (u8 *)(entry + 2);
    s32 firstOffset = firstSlot * 2 + 32;
    s32 secondOffset = secondSlot * 2 + 32;
    u16 firstCode = *(u16 *)(slotBase + firstOffset);
    u16 secondCode = *(u16 *)(slotBase + secondOffset);

    *(u16 *)(slotBase + firstOffset) = secondCode;
    *(u16 *)(slotBase + secondOffset) = firstCode;
}

INCLUDE_ASM(const s32, "game/code_00274B80", ptySkillMenuHandleSlotReorder);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B22F0);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2310);

INCLUDE_RODATA(const s32, "game/code_00274B80", D_003B2320);

INCLUDE_SDATA(const s32, "game/code_00274B80", D_003BC6E0);

INCLUDE_SDATA(const s32, "game/code_00274B80", D_003BC6E8);

INCLUDE_SDATA(const s32, "game/code_00274B80", D_003BC6F0);

INCLUDE_SDATA(const s32, "game/code_00274B80", D_003BC6F8);

INCLUDE_SDATA(const s32, "game/code_00274B80", D_003BC700);

INCLUDE_SDATA(const s32, "game/code_00274B80", D_003BC708);

INCLUDE_SDATA(const s32, "game/code_00274B80", D_003BC710);

