#include "common.h"





























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

extern void mnuForwardDupArg(s32, s32, s32, s32, s32);
extern void mnuSeekListNode(s32, s32);

extern u32 func_00285B20(u32);
extern void func_002858E8();
extern void func_002858F8(s32, void *);
extern void func_0027C788(s32);
extern void func_0027C770(s32);
extern void func_0027C758(s32);
extern void func_0027C658(s32);
extern void menuPlayInputSound(s32, u32, s32);
extern u8 D_0037CC74[];
extern u8 D_0037CC3C[];

extern s32 func_002877A8(void);

extern s32 func_00101A70();

extern s32 D_003BAA00;

extern s64 func_00285670(s32, s32 *, u64, u64);

void func_00274B80(u32 arg0) {
    mnuSetStaffDisplayMode(4, arg0);
}

void func_00274BA0(s32 context) {
}

s32 mnuIsFinalItemIndex(s32 index, s32 item) {
    if (index < (*(s32 *)(item + 0x20) - 1)) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00274BC0);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00274D48);

void func_00274EE0(s32 arg0) {
    func_0027C430(*(u32 *)(*(s32 *)(arg0 + 0x90c) + 8));
}

/* Snapshot five party entries; at most three active slots are displayed. */
void menuCopyPartyEntries(context)
    s32 context;
{
    PartyMenuData *menu = (PartyMenuData *)*(s32 *)(context + 0x90C);
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
    PartyMenuData *menu = (PartyMenuData *)*(s32 *)(context + 0x90C);
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
    initPartyPanelSlots(context + 0x7EC);
    menuUpdateHandleStates(panel);
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

extern void menuCopyPartyEntries();

void mnuClearPartySelectionAndActivateSlots(s32 context) {
    s32 menu = *(s32 *)(context + 0x90C);
    s32 i;
    s32 node;

    menuCopyPartyEntries();
    ((PartyMenuData *)menu)->selection = 0;
    memset(((PartyMenuData *)menu)->backup, 0, 0x834);
    *(s32 *)(context + 0x7EC) = 1;
    *(s32 *)(context + 0x7F0) = mnuCountActiveSlots() - 1;
    menuUpdateHandleStates(context + 0x15C);
    for (i = 0; i < 5; i++) {
        *(u32 *)(context + 0x1D8 + i * 0x134) |= 0x80;
    }
    for (node = *(s32 *)(*(s32 *)(*(s32 *)(menu + 8) + 0x14) + 0x10); node != 0; node = *(s32 *)(node + 0x58)) {
        *(u32 *)(node + 0x48) &= ~1;
    }
}

void mnuRefreshPartyPanelSlots(s32 context) {
    func_0027F0D8(context + 0x15c);
    initPartyPanelSlots(context + 0x7ec);
    menuUpdateHandleStates(context + 0x15c);
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_002755E0);

s32 mnuShopReleaseResources(void) {
    s32 context = func_00101A70();
    s32 menu = *(s32 *)(context + 0x90c);
    mnuRefreshPartyPanelSlots(context);
    func_00274EE0(context);
    func_00274BA0(context);
    func_002D0918(*(s32 *)menu);
    return 1;
}

void func_002758D8(s32 menu) {
    extern u8 D_0037CA58[];
    mnuRestorePartyEntriesAndRefresh();
    func_002858F8(menu + 0x54, (s32)D_0037CA58);
    func_0027E790(*(s32 *)(menu + 0x138), *(s32 *)(menu + 0x6c), 0, 1);
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

INCLUDE_ASM(const s32, "game/code_00274B80", func_002761C0);

static inline s64 menuRunPanel(s32 context, u64 mode, u64 arg) {
    return func_00285670(context + 8, (s32 *)(context + 0x54), mode, arg);
}

s64 func_00276250(s32 callback) {
    return menuRunPanel(func_00101A70(), 2, callback);
}

u8 func_00276288(void) {
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
    s32 menu = *(s32 *)(context + 0x90c);
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

INCLUDE_ASM(const s32, "game/code_00274B80", func_002764D8);

extern u8 D_0037C3A8[];
extern s32 func_002BD8F8(s32);
extern void mnuDrawStaffCampScreen();
extern void mnuCreateStaffImageSprite();
extern void func_002723B0();
extern void func_00272668();
extern void func_0027CDD0();

s64 func_002765E8(s32 arg0) {
    s32 context = func_00101A70();
    s32 menu = *(s32 *)(context + 0x90C);

    if (func_002BD8F8(*(s32 *)(context + 0x64)) != 0) {
        mnuDrawStaffCampScreen(0, arg0);
    } else {
        mnuDrawStaffCampScreen(1, arg0);
    }
    if (*(s32 *)(menu + 0x14) == 0) {
        mnuCreateStaffImageSprite(0x12);
    } else {
        mnuCreateStaffImageSprite(0x11);
    }
    func_0027CDD0(0x1C0, 0x3D0, 0, *(s32 *)(context + 0x124), 0x53);
    func_002723B0(0, *(s32 *)(context + 0x78));
    if (*(s32 *)(menu + 0x24) != 0) {
        s32 *slot = *(s32 **)(*(s32 *)(*(s32 *)(context + 0x124) + 0x14) + 0x1C);

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
s32 mnuStaffReleasePanelScene(void) {
    s32 context = func_00101A70();
    s32 menu = *(s32 *)(context + 0x90C);
    s32 entryList = context + 0x15C;

    func_00276720(entryList, 0, *(s32 *)(menu + 0x14), *(s32 *)(menu + 0x10));
    btlStopStage();
    mnuClearEntries(entryList);
    func_0027FA20(entryList);
    if (*(s32 *)(context + 0x8F8) != 0) {
        mnuDestroyPanelGroup(*(s32 *)(context + 0x8F8));
        *(s32 *)(context + 0x8F8) = 0;
    }
    if (*(s32 *)(context + 0x8FC) != 0) {
        func_002832F8(*(s32 *)(context + 0x8FC));
        *(s32 *)(context + 0x8FC) = 0;
    }
    if (*(s32 *)(context + 0x900) != 0) {
        func_00283820(*(s32 *)(context + 0x900));
        *(s32 *)(context + 0x900) = 0;
    }
    if (*(s32 *)(context + 0x920) != 0) {
        func_00285160(*(s32 *)(context + 0x920));
        *(s32 *)(context + 0x920) = 0;
    }
    mnuReleaseResourceList(*(s32 *)(menu + 0x20));
    effResolveAndReleaseResource(*(s32 *)(context + 0x64));
    func_0027E6B8(*(s32 *)(context + 0x138), *(s32 *)(context + 0x64), 0, 0);
    return 1;
}

void func_00276B10(s32 arg0) {
    *(u32 *)
      (*(s32 *)(**(s32 **)(*(s32 *)(arg0 + 0x7d8) + 0x1c) * 0x134 + arg0 + 0x2b4) + 0x3c) = 0x100
    ;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00276B38);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00276C28);

INCLUDE_ASM(const s32, "game/code_00274B80", func_00276DA0);

extern void func_002BF790(s32, s32, s32, s32, s32, s32, s32);

void func_00276E90(s32 flag, s32 obj) {
    s32 y;

    for (y = 0x360; y < 0xE40; y += 0x38) {
        func_002BF790(0xE80, y, 0, 1, *(s32 *)(obj + 0x20), 2, 0x53);
    }
    func_002BF790(0x10F0, 0x358, 0, 1, *(s32 *)(obj + 0x20), 4, 0x53);
    func_002BF790(0x1050, 0x500, 0, 1, *(s32 *)(obj + 0x20), 3, 0x53);
    if (flag == 0) {
        func_002BF790(-0x140, -0xA0, 0, 1, *(s32 *)(obj + 0x20), 7, 0x53);
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

INCLUDE_ASM(const s32, "game/code_00274B80", func_00277220);

extern void func_00283838(s32, s32, s32, s32, s32, s32, s32);
extern void func_00285440(s32, s32, s32, s32, s32);

void func_00277328(s32 obj, s32 unused1, s32 arg2, s32 arg3, s32 unused4, s32 arg5) {
    func_00283838(0, 0, 0, obj, *(s8 *)(obj + 0x55), arg2, arg5);
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
void mnuIdleVoiceTimer(MenuIdleVoiceState *voiceTimer, s32 panel) {
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

INCLUDE_ASM(const s32, "game/code_00274B80", func_002775D8);

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
    s32 temp_v0;

    temp_v0 = func_00101A70();
    temp_v0 = *(s32 *)(temp_v0 + 0x90c);
    func_0027C430(*(u32 *)(temp_v0 + 0x24));
    *(u32 *)(temp_v0 + 0x24) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00277CB8);

INCLUDE_ASM(const s32, "game/code_00274B80", ptySkillMenuRebuildAfterMutation);

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

s32 mnuIsSkillCodeInBitset(s32 arg0, u32 *arg1) {
    s32 temp_v0 = (arg0 < 0) ? arg0 + 0x1f : arg0;

    return (arg1[temp_v0 >> 5] & (1 << arg0)) != 0;
}

INCLUDE_ASM(const s32, "game/code_00274B80", ptySkillMenuInitPages);

INCLUDE_ASM(const s32, "game/code_00274B80", func_002786E8);

extern s32 func_002D03F8(s32);
extern s32 *sdfResourceRetainAddress(s32);
extern void func_0027E790();

s32 campMenuInit(void) {
    s32 context = func_00101A70();
    s32 handle = func_002D03F8(0x38);
    s32 *menu = sdfResourceRetainAddress(handle);

    *(s32 **)(context + 0x90C) = menu;
    memset(menu, 0, 0x38);
    *menu = handle;
    func_00277DD0(context);
    mnuForwardDupArg(*(s32 *)(context + 0x12C), *(s32 *)(context + 0x74), 0, 0, 0);
    switch (**(s32 **)(*(s32 *)(*(s32 *)(context + 0x124) + 0x14) + 0x1C)) {
    case 0:
        func_0027E790(*(s32 *)(context + 0x138), *(s32 *)(context + 0xE0), 0, 1);
        break;
    case 2:
        func_0027E790(*(s32 *)(context + 0x138), *(s32 *)(context + 0xE0), 0x35, 0x36);
        break;
    default:
        func_0027E790(*(s32 *)(context + 0x138), *(s32 *)(context + 0xE0), 0x33, 0x34);
        break;
    }
    mnuSeekListNode(0, *(s32 *)(*(s32 *)(context + 0x12C) + 0x14));
    return 1;
}

s32 func_00278868(s32 arg0) {
    s32 context = func_00101A70();
    s32 menu = *(s32 *)(context + 0x90C);

    if (*(s32 *)(menu + 0x24) != 0) {
        func_00277C80(arg0);
    }
    func_00277DF0(context);
    func_002D0918(*(u32 *)menu);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00274B80", ptySkillMenuShellUpdate);

s64 campMenuDrawSlotLabel(s32 param) {
    s32 context = func_00101A70();
    s32 *slot;

    func_00272778(param);
    slot = *(s32 **)(*(s32 *)(*(s32 *)(context + 0x124) + 0x14) + 0x1C);
    if (*slot == 0) {
        mnuCreateStaffImageSprite(1);
    } else {
        mnuCreateStaffImageSprite(0xA);
    }
    func_00272668(1, **(s32 **)(*(s32 *)(*(s32 *)(context + 0x124) + 0x14) + 0x1C), D_0037C3A8, context, 1, 0x53);
    if (**(s32 **)(*(s32 *)(*(s32 *)(context + 0x124) + 0x14) + 0x1C) == 0) {
        func_0027CDD0(0x1C0, 0x3D0, 0, *(s32 *)(context + 0x124), 0x53);
    } else {
        func_0027CDD0(0x1C0, 0x3D0, 0, *(s32 *)(context + 0x124), 0x53);
    }
    func_002723B0(0, *(s32 *)(context + 0x78));
    return menuRunPanel(context, 1, param);
}

s64 func_00278B90(s32 callback) {
    return menuRunPanel(func_00101A70(), 2, callback);
}

void func_00278BC8(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    *(u32 *)(*(s32 *)(temp_v0 + 0x90c) + 0x34) = 0xffffffff;
}

u32 func_00278BF0(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    return ~*(u32 *)(*(s32 *)(temp_v0 + 0x90c) + 0x34) >> 0x1f;
}

void func_00278C20(void) {
    s32 menu = *(s32 *)(func_00101A70() + 0x90C);
    s32 node = *(s32 *)(*(s32 *)(*(s32 *)(menu + 0x24) + 0x14) + 0x10);

    for (; node != 0; node = *(s32 *)(node + 0x58)) {
        if (*(s32 *)node == *(s32 *)(menu + 0x34)) {
            *(u32 *)(node + 0x48) |= 2;
        } else {
            *(u32 *)(node + 0x48) &= ~2;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00274B80", func_00278C90);

u32 func_00278D68(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    *(u32 *)(*(s32 *)(temp_v0 + 0x90c) + 0x30) = 0;
    return 1;
}

extern void ptyRecomputeMaxHpMp();
extern void scrClearSecondaryScriptFlag();

void func_00278D90(s32 obj, s32 id, s32 slot) {
    u16 code = id;

    if (ptyHasSkill(obj, code) == 0) {
        *(u16 *)(obj + slot * 2 + 0x22) = code;
        ptyRecomputeMaxHpMp(obj);
        scrClearSecondaryScriptFlag(obj, code);
    }
}

void func_00278E08(s32 arg0, s32 arg1) {
    *(u16 *)(arg1 * 2 + arg0 + 0x22) = 0;
    ptyRecomputeMaxHpMp();
}

void campMenuHandleInput(void) {
    s32 context = func_00101A70();
    s32 menu = *(s32 *)(context + 0x90C);
    u32 input = func_00285B20(0x33);
    s32 node = *(s32 *)(menu + 0x24);
    s32 flags = *(s32 *)(node + 0x14);

    *(u32 *)flags &= ~8;
    if (input & 1) {
        s32 info = *(s32 *)(flags + 0x1C);
        s32 target = *(s32 *)(info + 0x60);

        if (!(*(u32 *)(info + 0x48) & 1) && target != 0) {
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
        menuPlayInputSound(0, input, *(s32 *)(node + 0x14));
    }
}

INCLUDE_ASM(const s32, "game/code_00274B80", ptySkillMenuHandleSelection);

void func_00279130(s32 arg0, s32 arg1, s32 arg2) {
    u8 *temp_v0 = (u8 *)(arg0 + 2);
    s32 temp_v1 = arg1 * 2 + 32;
    s32 temp_v2 = arg2 * 2 + 32;
    u16 temp_v3 = *(u16 *)(temp_v0 + temp_v1);
    u16 temp_v4 = *(u16 *)(temp_v0 + temp_v2);

    *(u16 *)(temp_v0 + temp_v1) = temp_v4;
    *(u16 *)(temp_v0 + temp_v2) = temp_v3;
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

