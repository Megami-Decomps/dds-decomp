#include "common.h"

extern u64 battleFindGroupedEntity(void);

extern u8 D_00436F5D;

extern s32 mdlFlagTest(s32);

extern s32 D_00435DD0;

extern s32 func_001AA6F8(void);

extern s32 countBattleTasksByKind(u32);

extern u32 func_002290D0(void);

extern u32 func_00229158(void);

extern u32 func_00229198(void);

extern s8 D_00453068[];

extern s8 D_00453060[];

extern u8 D_003BF961[];

extern u16 D_003BF962[];

extern s32 createSemaphore(s32, s32, s32);

extern u32 D_00438F90;

extern s32 D_003C86F0[];

extern s32 D_003C8710[];

extern void func_0010D818();

extern s32 func_0010D650(u32);

extern void func_0011A118(u32, u32);

extern void func_0022D2F8(u32, u32);

extern u32 func_001E1468(u32);

extern u32 func_001E14F8(u32);

extern void func_0022BA08(void);

extern void func_0022A908(u32);

extern void func_0010C250(u32, u32);

extern u32 battleReleaseScriptResource(void);

extern s32 battleFindModelEntry();

typedef struct BattleEffectState {
    u32 actor, flags, value;
    u16 timer;
    u8 active, phase;
    u32 effect;
    f32 speed;
} BattleEffectState;

extern void *func_001E5DA8(void *, s32, s32);

extern void func_001E1580(void *);

extern void func_0035C860();

extern char D_0041B650[];

extern char D_00436D00[];

extern s32 func_0010BE90(s32, s32, s32);

extern char D_00436D08[];

extern s32 func_0025D008(s16, s32);

extern char D_0041B7E0[];

typedef struct BattleCombatant {
    u8 unk_00[0x110];
    u32 status;
    u8 unk_114[0x1a];
    u16 ailment;
    u8 unk_130[0x214];
    struct BattleCombatant *next;
} BattleCombatant;

extern s32 func_00231B80(s32, s32, s32);

extern s32 func_002C8168(void *);

extern void func_0032F108(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

void func_00227288(void) {
    func_00226F58();
}

INCLUDE_ASM(const s32, "game/code_00227288", func_002272A0);

INCLUDE_ASM(const s32, "game/code_00227288", func_002274D0);

INCLUDE_ASM(const s32, "game/code_00227288", func_00227528);

INCLUDE_ASM(const s32, "game/code_00227288", func_00227660);

INCLUDE_ASM(const s32, "game/code_00227288", func_00227748);

INCLUDE_ASM(const s32, "game/code_00227288", func_002277D8);

INCLUDE_ASM(const s32, "game/code_00227288", func_00227820);

INCLUDE_ASM(const s32, "game/code_00227288", func_002279F0);

INCLUDE_ASM(const s32, "game/code_00227288", func_00227C70);

INCLUDE_ASM(const s32, "game/code_00227288", func_00227CC8);

INCLUDE_ASM(const s32, "game/code_00227288", func_00227DA8);

s32 func_00228320(u32 actor) {
    BattleEffectState *state = *(BattleEffectState **)(func_001AA6F8() + 0x718);
    u32 active = state->actor;
    if (active != 0) {
        return active == actor;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00227288", func_00228360);

s32 battleHasDifferentActiveTarget(u32 target) {
    if (func_00229110() == 0) {
        return 1;
    }
    return func_00229198() != target;
}

INCLUDE_ASM(const s32, "game/code_00227288", func_00228458);

INCLUDE_ASM(const s32, "game/code_00227288", func_00228598);

INCLUDE_ASM(const s32, "game/code_00227288", func_002286D8);

INCLUDE_ASM(const s32, "game/code_00227288", func_00228A30);

INCLUDE_ASM(const s32, "game/code_00227288", func_00228B08);

INCLUDE_ASM(const s32, "game/code_00227288", func_00228D68);

INCLUDE_ASM(const s32, "game/code_00227288", func_00228F20);

INCLUDE_ASM(const s32, "game/code_00227288", func_00228F48);

u32 func_002290D0(void) {
    s32 battle = func_001AA6F8();
    u32 battleId = *(u32 *)(battle + 0x2a0);
    BattleEffectState *state;
    if (battleId != 0x312) {
        return 0;
    }
    state = *(BattleEffectState **)(battle + 0x718);
    if (state != NULL) {
        return state->active;
    }
    return 0;
}

s32 func_00229110(void) {
    s32 battle = func_001AA6F8();
    u32 battleId = *(u32 *)(battle + 0x2a0);
    BattleEffectState *state;
    if (battleId != 0x312) {
        return 0;
    }
    state = *(BattleEffectState **)(battle + 0x718);
    if (state == NULL) {
        return 0;
    }
    return state->actor != 0;
}

u32 func_00229158(void) {
    s32 battle = func_001AA6F8();
    u32 battleId = *(u32 *)(battle + 0x2a0);
    BattleEffectState *state;
    if (battleId != 0x312) {
        return 0;
    }
    state = *(BattleEffectState **)(battle + 0x718);
    if (state != NULL) {
        return state->value;
    }
    return 0;
}

u32 func_00229198(void) {
    BattleEffectState *state = *(BattleEffectState **)(func_001AA6F8() + 0x718);
    return state->actor;
}

INCLUDE_ASM(const s32, "game/code_00227288", func_002291C0);

void func_00229248(void) {
    u8 *puVar1;
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    puVar1 = *(u8 **)(temp_v0 + 0x718);
    puVar1[1] = 1;
    *puVar1 = 0;
}

u32 func_00229278(void) {
    return 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_00227288", func_00229280);

void func_002292D8(void) {
    u16 temp_v0;
    u16 *puVar2;
    u32 temp_v1;
    u32 temp_v2;
    u32 temp_v3;
    u8 temp_v4;

    temp_v4 = 0;
    temp_v1 = 0;
    temp_v3 = 0;
    puVar2 = (u16 *)(D_00435DD0 + 0xa60);
    temp_v2 = 0;
    do {
        temp_v0 = *puVar2;
        if ((temp_v0 & 1) != 0) {
            if (puVar2[2] == 2) {
                if ((temp_v0 & 2) != 0) {
                    return;
                }
                temp_v4 = 1;
            }
            temp_v3 = temp_v3 + 1;
            if ((temp_v0 & 2) != 0) {
                temp_v1 = temp_v1 + 1;
            }
        }
        temp_v2 = temp_v2 + 1;
        puVar2 = puVar2 + 0xe2;
    } while (temp_v2 < 5);
    if (((temp_v3 < 4) && (temp_v1 < 3)) && (temp_v4)) {
        func_0011AEE0(2);
        return;
    }
}

s32 func_00229378(void) {
    u32 node = *(u32 *)(func_001AA6F8() + 0x24c);
    while (node != 0) {
        if (*(u32 *)(node + 0x110) & 1) {
            *(u32 *)(node + 0x114) &= ~0x100000;
        }
        node = *(u32 *)(node + 0x364);
    }
    return -1;
}

void func_002293D8(u32 unit) {
    u32 flags = *(u32 *)(unit + 0x110);
    if ((flags & 0x200) && *(u16 *)(unit + 0x124) == 2) {
        *(u32 *)(unit + 0x110) = flags | 0x1000;
        *(u32 *)(unit + 0x114) |= 0x100000;
        *(u16 *)(unit + 0x120) |= 0x1000;
    }
}

void func_00229420(u32 unit) {
    if ((*(u32 *)(unit + 0x110) & 0x400) &&
        *(u16 *)(unit + 0x124) == 0x144 &&
        mdlFlagTest(0x841)) {
        *(u16 *)(unit + 0x126) = 1;
    }
}

u32 func_00229470(void) {
    return 6;
}

void func_00229478(void) {
    func_0011AEE0(7);
}

void func_00229490(void) {
    func_0011AEE0(1);
    func_0011AEE0(4);
    func_0011AEE0(5);
}

void func_002294B8(void) {
    func_0011AEE0(8);
}

INCLUDE_ASM(const s32, "game/code_00227288", func_002294D0);

INCLUDE_ASM(const s32, "game/code_00227288", func_002295D8);

INCLUDE_ASM(const s32, "game/code_00227288", func_00229690);

INCLUDE_ASM(const s32, "game/code_00227288", func_00229728);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022A7D0);

INCLUDE_ASM(const s32, "game/code_00227288", battleReleaseBossData);

INCLUDE_ASM(const s32, "game/code_00227288", battleFindScriptResource);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022A908);

INCLUDE_ASM(const s32, "game/code_00227288", battleReleaseScriptResource);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022A9D0);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022AA38);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022AAC8);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022AAE8);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022AB60);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022ABF0);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022AC10);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022AF90);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022B108);

INCLUDE_ASM(const s32, "game/code_00227288", battleReleaseEventData);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022B288);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022B348);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022B398);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022B460);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022B510);

u8 func_0022B5E0(void) {
    s64 temp_v0;

    temp_v0 = countBattleTasksByKind(0x1a);
    return temp_v0 == 0;
}

INCLUDE_ASM(const s32, "game/code_00227288", func_0022B600);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022B6E8);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022B760);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022B7A0);

u32 func_0022B8E0(void) {
    u32 index = func_0010D650(0);
    u32 value = 0;
    if (index < 100) {
        value = D_003BF961[index * 4];
    }
    func_0010D818(value);
    return 1;
}

u32 func_0022B928(void) {
    s64 temp_v0;
    s32 temp_v1;
    u32 temp_v2;
    s32 temp_v3;

    temp_v2 = 0;
    temp_v3 = 0;
    do {
        temp_v1 = 0x8ff - temp_v2;
        temp_v2 = temp_v2 + 1;
        temp_v0 = mdlFlagTest(temp_v1);
        if (temp_v0 != 0) {
            temp_v3 = temp_v3 + 1;
        }
    } while (temp_v2 < 100);
    func_0010D818(temp_v3);
    return 1;
}

u32 func_0022B988(void) {
    u32 index = func_0010D650(0);
    u32 value = 1;
    if (index < 100) {
        value = D_003BF962[index * 2];
    }
    func_0010D818(value);
    return 1;
}

u32 func_0022B9D0(void) {
    s32 index = func_0010D650(0);
    if (index < 0x100) {
        func_0011A118(index, 1);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00227288", func_0022BA08);

u32 func_0022BAA8(u32 object, u32 group) {
    u32 task = func_001E1468(12);
    u32 data;
    *(u8 *)(task + 0) = 1;
    *(u16 *)(task + 0x20) = 0x68;
    *(void (**)(void))(task + 0x4c) = func_0022BA08;
    *(u8 *)(task + 0x10) = 0;
    data = func_001E14F8(task);
    *(u32 *)(data + 0) = object;
    *(u32 *)(data + 4) = group;
    *(u32 *)(data + 8) = 0;
    return task;
}

u32 func_0022BB28(u32 record) {
    if (*(u32 *)(record + 8) == 0) {
        u32 battle = func_001AA6F8();
        u32 object;
        func_0022A908(*(u32 *)(record + 4));
        object = *(u32 *)(battle + 0x2c8);
        if (object != 0) {
            func_0010C250(object, *(u32 *)record);
        }
    }
    if (battleReleaseScriptResource()) {
        return 1;
    }
    ++*(u32 *)(record + 8);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00227288", battleCreateActionTask);

INCLUDE_ASM(const s32, "game/code_00227288", battleHasRestrictedUnit);

s32 battleListHasMarkedFlag(u8 **entries, s32 count) {
    s32 i;
    for (i = 0; i < count; i++) {
        if ((*(u16 *)(entries[i] + 0xe) & 0x7fff) == 0x4000) {
            return 1;
        }
    }
    return 0;
}

s32 battleListCountersWithinLimits(u8 **entries, s32 count) {
    s32 i;
    for (i = 0; i < count; i++) {
        if (*(u16 *)(entries[i] + 6) < *(u16 *)(entries[i] + 8)) {
            return 0;
        }
    }
    return 1;
}

s32 battleListSecondaryCountersWithinLimits(u8 **entries, s32 count) {
    s32 i;
    for (i = 0; i < count; i++) {
        if (*(u16 *)(entries[i] + 0xa) < *(u16 *)(entries[i] + 0xc)) {
            return 0;
        }
    }
    return 1;
}

s32 battleListHasMatchingFlag(u8 **entries, s32 count, u32 flags) {
    s32 i;
    for (i = 0; i < count; i++) {
        if ((*(u16 *)(entries[i] + 0xe) & 0x7fff) & flags) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00227288", func_0022BDC0);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022BEB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041B800);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022C040);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022C1B0);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022C308);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022C518);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022C600);

INCLUDE_ASM(const s32, "game/code_00227288", battleCreateModelEntry);

INCLUDE_ASM(const s32, "game/code_00227288", battleReleaseModelEntry);

INCLUDE_ASM(const s32, "game/code_00227288", battleReleaseAllModelEntries);

INCLUDE_ASM(const s32, "game/code_00227288", battleFormatModelResourcePath);

s32 func_0022C948(u8 *task) {
    s32 result;
    if (*(s8 *)(task + 0xc) != 0) {
        return 1;
    }
    if (func_00231B80(*(s32 *)task, *(s32 *)(task + 4), 0) == 0 ||
        func_00231B80(*(s32 *)task, *(s32 *)(task + 4), 0) == -1) {
        return 0;
    }
    if (*(void **)(task + 0x10) == 0) {
        return 1;
    }
    result = func_002C8168(*(void **)(task + 0x10));
    return (s8)result;
}

INCLUDE_ASM(const s32, "game/code_00227288", battleFindModelEntry);

void func_0022CA40(void) {
}

void func_0022CA48(void) {
    battleReleaseAllModelEntries();
}

INCLUDE_ASM(const s32, "game/code_00227288", func_0022CA60);

void func_0022CB68(void) {
    s64 temp_v0;

    temp_v0 = battleFindModelEntry();
    if (temp_v0 != 0) {
        battleReleaseModelEntry(temp_v0);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_00227288", func_0022CBA0);

s32 battleGetEntryState(s32 kind, s32 value) {
    u8 *entry = (u8 *)battleFindModelEntry(kind, value);
    if (entry != 0) {
        return *(s8 *)(entry + 0xc);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00227288", func_0022CD30);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022CD60);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022CE30);

void func_0022CF58(s32 packet, s32 first, s32 second, s32 color) {
    func_0032F108(packet, second, first, 0x7000, 0x7900, 0x9000, 0x7900, 0x7000,
                  0x8700, 0x9000, 0x8700, color, 0);
}

INCLUDE_ASM(const s32, "game/code_00227288", func_0022CFB0);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022D040);

INCLUDE_ASM(const s32, "game/code_00227288", battleReleaseOwnedData);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022D2F8);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022DD18);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022DD70);

void func_0022DDC8(void) {
}

INCLUDE_ASM(const s32, "game/code_00227288", func_0022DDD0);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022DE38);

void func_0022DEB8(void) {
    func_00105538();
    func_001054E0();
}

INCLUDE_ASM(const s32, "game/code_00227288", battleInitializeGraphicsRuntime);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022DF98);

INCLUDE_ASM(const s32, "game/code_00227288", battleInitializeOverlayGraphics);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022E0E0);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022E338);

INCLUDE_ASM(const s32, "game/code_00227288", battleResetAsyncState);

INCLUDE_ASM(const s32, "game/code_00227288", battleActivateRuntime);

INCLUDE_ASM(const s32, "game/code_00227288", battleResetRuntimeState);

s32 func_0022E450(void) {
    return D_00453068[0];
}

s32 func_0022E460(void) {
    u16 state;

    if (D_00453060[8] == 0) {
        return 1;
    }
    state = *(u16 *)(D_00453060 + 4);
    if (state == 0) {
        return 1;
    }
    return state == 2;
}

s32 func_0022E490(void) {
    u16 state;

    if (D_00453060[8] == 0) {
        return 1;
    }
    state = *(u16 *)(D_00453060 + 4);
    if (state == 0) {
        return 1;
    }
    return state == 4;
}

void func_0022E4C0(void) {
    if (D_00453060[8] != 0) {
        D_00453060[7] = 1;
    }
}

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041B9D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041B9E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041B9F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BA00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BA10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BA20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BA30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BA40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BA50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BA60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BA70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BA80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BA90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BAA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BAB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BAC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BAD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BAE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BAF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BB00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BB10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BB20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BB38);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BB50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BB68);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BB80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BB98);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BBB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BBC8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BBE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BBF8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BC10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BC20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BC30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BC40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BC50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BC60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BC70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BC80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BC90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BCA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BCB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BCC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BCD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BCE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BCF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BD00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BD10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BD20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BD30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BD40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BD50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BD60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BD70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BDC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BDD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BDE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BDF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BE00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BE10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BE20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BE30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BE40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BE50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BE60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BE70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BE80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BE90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BEC8);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022E4E0);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022EBC0);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022ED38);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022ED90);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022EE88);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022EF18);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022F068);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022F180);

INCLUDE_ASM(const s32, "game/code_00227288", func_002303D0);

void func_00230960(void) {
    D_00436F5D = 0;
    dds3WorkClear();
}

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C118);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C128);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C138);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C148);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C158);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C168);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C178);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C188);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C198);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C1A8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C1B8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C1C8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C1D8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C1E8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C1F8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C208);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C218);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C228);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C238);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C248);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C258);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C268);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C278);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C288);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C298);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C2A8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C2B8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C2C8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C2D8);

INCLUDE_ASM(const s32, "game/code_00227288", func_00230978);

INCLUDE_ASM(const s32, "game/code_00227288", func_00230D90);

void func_002311C0(void) {
    s32 i;

    D_00438F90 = createSemaphore(1, 0x7f, 0);
    for (i = 0; i != 8; i++) {
        D_003C86F0[i] = 0;
        D_003C8710[i] = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00227288", battleFindGroupedEntity);

INCLUDE_ASM(const s32, "game/code_00227288", battleGroupContainsId);

INCLUDE_ASM(const s32, "game/code_00227288", func_002312A0);

void func_002312F8(s32 arg0, s32 arg1) {
    s32 *temp_v0;
    s32 *temp_v1;

    temp_v0 = &D_003C8710[arg0];
    temp_v1 = (s32 *)*temp_v0;
    if (temp_v1 == 0) {
        return;
    }
    do {
        if (*(temp_v1 + 1) == arg1) {
            *temp_v0 = *temp_v1;
            func_00328E48(temp_v1);
            break;
        } else {
            temp_v0 = temp_v1;
            temp_v1 = (s32 *)*temp_v1;
        }
    } while (temp_v1 != 0);
}

INCLUDE_ASM(const s32, "game/code_00227288", func_00231358);

INCLUDE_ASM(const s32, "game/code_00227288", func_00231470);

void func_00231588(void) {
    u64 temp_v0;

    temp_v0 = battleFindGroupedEntity();
    func_00231470(temp_v0);
}

void battleReleaseAllEntities(void) {
    u32 i = 0;
    s32 *head = D_003C86F0;
    do {
        s32 *node = (s32 *)*head;
        while (node != 0) {
            s32 *next = (s32 *)*node;
            func_00231470(node);
            node = next;
        }
        i++;
        head++;
    } while (i < 8);
}

INCLUDE_ASM(const s32, "game/code_00227288", func_00231618);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C300);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C310);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C320);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C330);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C340);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C350);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C360);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C370);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C380);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C390);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C3A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C3B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C3C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C3D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C3E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C3F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C400);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C410);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C420);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C430);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C440);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C450);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C460);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C470);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C480);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C490);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C4A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C4B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C4C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C4D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C4E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C4F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C500);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C510);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C520);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C530);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C540);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C550);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C560);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C570);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C580);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C590);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C5A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C5B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C5C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C5D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C5E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C5F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C600);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C610);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C620);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C630);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C640);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C650);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C660);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C670);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C680);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C690);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C6A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C6B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C6C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C6D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C6E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C6F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C700);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C710);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C720);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C730);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C740);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C750);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C760);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C770);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C780);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C790);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C7A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C7B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C7C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C7D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C7E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C7F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C800);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C810);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C820);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C830);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C840);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C850);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C860);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C870);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C880);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C890);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C8A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C8B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C8C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C8D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C8E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C8F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C900);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C910);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C920);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C930);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C940);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C950);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C960);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C970);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C980);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C990);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C9A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C9B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C9C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C9D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C9E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C9F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CA00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CA10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CA20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CA30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CA40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CA50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CA60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CA70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CA80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CA90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CAA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CAB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CAC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CAD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CAE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CAF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CB00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CB10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CB20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CB30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CB40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CB50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CB60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CB70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CB80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CB90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CBA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CBB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CBC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CBD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CBE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CBF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CC00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CC10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CC20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CC30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CC40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CC50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CC60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CC70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CC80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CC90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CCA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CCB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CCC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CCD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CCE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CCF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CD00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CD10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CD20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CD30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CD40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CD50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CD60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CD70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CD80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CD90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CDA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CDB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CDC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CDD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CDE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CDF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CE00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CE10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CE20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CE30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CE40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CE50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CE60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CE70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CE80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CE90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CEA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CEB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CEC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CED0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CEE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CEF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CF00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CF10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CF20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CF30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CF40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CF50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CF60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CF70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CF80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CF90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CFA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CFB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CFC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CFD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CFE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CFF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D000);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D010);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D020);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D030);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D040);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D050);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D060);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D070);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D080);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D090);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D0A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D0B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D0C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D0D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D0E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D0F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D100);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D110);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D120);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D130);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D140);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D150);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D160);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D170);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D180);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D190);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D1A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D1B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D1C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D1D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D1E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D1F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D200);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D210);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D220);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D230);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D240);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D250);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D260);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D270);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D280);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D290);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D2A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D2B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D2C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D2D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D2E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D2F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D300);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D310);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D320);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D330);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D340);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D350);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D360);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D370);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D380);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D390);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D3A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D3B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D3C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D3D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D3E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D3F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D400);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D410);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D420);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D430);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D440);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D450);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D460);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D470);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D480);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D490);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D4A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D4B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D4C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D4D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D4E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D4F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D500);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D510);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D520);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D530);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D540);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D550);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D560);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D570);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D580);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D590);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D5A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D5B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D5C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D5D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D5E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D5F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D600);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D610);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D620);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D630);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D640);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D650);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D660);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D670);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D680);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D690);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D6A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D6B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D6C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D6D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D6E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D6F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D700);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D710);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D720);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D730);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D740);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D750);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D760);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D770);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D780);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D798);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D7B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D7C8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D7E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D7F8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D810);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D828);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D840);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D858);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D870);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D888);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D8A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D8B8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D8D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D8E8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D900);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D918);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D930);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D948);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D960);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D978);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D990);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D9A8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D9C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D9D8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D9F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DA08);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DA20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DA38);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DA50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DA68);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DA80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DA90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DAA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DAB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DAC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DAD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DAE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DAF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DB00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DB10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DB20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DB30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DB40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DB50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DB60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DB70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DB80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DB90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DBA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DBB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DBC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DBD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DBE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DBF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DC00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DC10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DC20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DC30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DC40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DC50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DC60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DC70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DC80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DC90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DCA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DCB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DCC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DCD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DCE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DCF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DD00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DD10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DD20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DD30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DD40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DD50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DD60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DD70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DD80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DD90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DDA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DDB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DDC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DDD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DDE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DDF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DE00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DE10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DE20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DE30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DE40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DE50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DE60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DE70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DE80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DE90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DEA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DEB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DEC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DED0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DEE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DEF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DF00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DF10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DF20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DF30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DF40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DF50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DF60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DF70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DF80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DF90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DFA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DFB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DFC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DFD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DFE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DFF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E000);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E010);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E020);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E030);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E040);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E050);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E060);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E070);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E080);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E090);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E0A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E0B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E0C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E0D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E0E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E0F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E100);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E110);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E120);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E130);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E140);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E150);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E160);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E170);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E180);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E190);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E1A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E1B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E1C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E1D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E1E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E1F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E200);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E210);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E220);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E230);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E240);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E250);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E260);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E270);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E280);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E290);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E2A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E2B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E2C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E2D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E2E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E2F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E300);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E310);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E320);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E330);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E340);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E350);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E360);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E370);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E380);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E390);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E3A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E3B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E3C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E3D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E3E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E3F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E400);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E410);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E420);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E430);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E440);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E450);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E460);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E470);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E480);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E490);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E4A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E4B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E4C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E4D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E4E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E4F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E500);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E510);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E520);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E530);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E540);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E550);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E560);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E570);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E580);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E590);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E5A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E5B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E5C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E5D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E5E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E5F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E600);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E610);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E620);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E630);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E640);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E650);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E660);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E670);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E680);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E690);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E6A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E6B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E6C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E6D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E6E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E6F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E700);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E710);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E720);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E730);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E740);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E750);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E760);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E770);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E780);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E790);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E7A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E7B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E7C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E7D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E7E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E7F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E800);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E810);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E820);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E830);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E840);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E850);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E860);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E870);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E880);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E890);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E8A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E8B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E8C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E8D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E8E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E8F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E900);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E910);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E920);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E930);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E940);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E950);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E960);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E970);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E980);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E990);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E9A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E9B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E9C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E9D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E9E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E9F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EA00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EA10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EA20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EA30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EA40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EA50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EA60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EA70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EA80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EA90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EAA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EAB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EAC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EAD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EAE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EAF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EB00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EB10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EB20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EB30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EB40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EB50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EB60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EB70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EB80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EB90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EBA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EBB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EBC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EBD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EBE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EBF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EC00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EC10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EC20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EC30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EC40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EC50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EC60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EC70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EC80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EC90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ECA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ECB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ECC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ECD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ECE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ECF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ED00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ED10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ED20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ED30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ED40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ED50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ED60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ED70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ED80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ED90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EDA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EDB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EDC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EDD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EDE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EDF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EE00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EE10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EE20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EE30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EE40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EE50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EE60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EE70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EE80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EE90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EEA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EEB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EEC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EED0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EEE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EEF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EF00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EF10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EF20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EF30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EF40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EF50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EF60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EF70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EF80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EF90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EFA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EFB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EFC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EFD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EFE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EFF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F000);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F010);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F020);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F030);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F040);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F050);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F060);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F070);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F080);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F098);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F0B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F0C8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F0D8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F0F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F108);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F120);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F138);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F158);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F170);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F188);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F1A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F1B8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F1D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F1E8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F200);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F218);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F230);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F248);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F260);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F270);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F288);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F2A8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F2C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F2D8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F2F8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F310);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F328);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F340);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F358);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F370);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F388);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F3A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F3B8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F3D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F3E8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F400);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F418);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F428);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F440);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F458);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F470);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F488);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F4A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F4B8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F4D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F4E8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F4F8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F508);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F518);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F528);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F538);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F550);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F568);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F580);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F5A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F5C8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F5F8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F620);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F648);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F670);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F698);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F6C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F6E8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F710);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F738);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F760);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F788);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F7B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F7D8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F800);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F828);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F850);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F878);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F8A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F8C8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F8F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F918);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F940);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F968);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F990);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F9B8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F9E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FA08);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FA30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FA50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FA78);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FAA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FAC8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FAF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FB18);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FB40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FB68);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FB90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FBB8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FBE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FC08);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FC30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FC58);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FC80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FCA8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FCD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FCF8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FD20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FD48);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FD70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FD98);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FDC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FDE8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FE10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FE38);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FE60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FE88);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FEB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FED8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FF00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FF28);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FF50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FF78);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FFA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FFC8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FFE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420000);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420020);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420040);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420060);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420080);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004200A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004200C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004200E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420100);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420120);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420140);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420168);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420188);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004201A8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004201C8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004201E8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420208);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420228);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420248);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420268);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420288);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004202A8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004202C8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004202E8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420308);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420328);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420348);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420368);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420388);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004203A8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004203C8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004203E8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420408);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420428);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420448);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420468);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420488);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004204A8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004204C8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004204E8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420508);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420528);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420548);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420568);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420588);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004205A8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004205C8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004205E8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420608);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420628);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420648);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420668);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420688);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004206A8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004206C8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004206E8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420708);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420728);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420750);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420770);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420790);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004207B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004207D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004207F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420810);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420830);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420850);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420870);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420898);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004208C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004208E8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420910);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420930);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420958);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420978);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004209A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004209C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004209E8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420A10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420A38);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420A58);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420A80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420AA8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420AD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420AF8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420B18);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420B40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420B68);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420B90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420BB8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420BE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420C08);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420C30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420C58);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420C80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420CA8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420CD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420CF8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420D18);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420D38);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420D60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420D80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420DA8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420DD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420DF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420E10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420E30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420E50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420E78);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420E98);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420EB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420EC8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420EE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420EF8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420F10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420F20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420F30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420F40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420F50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420F60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420F70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420F80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420F90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420FA8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420FC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420FD8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436CF8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D00);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D08);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D10);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D18);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D20);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D28);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D30);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D38);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D40);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D48);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D50);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D58);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D60);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D68);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D70);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D78);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D80);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D88);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D8E);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D90);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D98);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DA0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DA8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DB0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DB8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DC0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DC8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DD0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DD8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DE0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DE8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DF0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DF8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E00);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E08);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E10);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E18);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E20);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E28);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E30);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E38);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E40);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E48);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E50);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E58);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E60);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E68);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E70);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E78);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E80);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E88);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E90);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E98);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EA0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EA8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EB0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EB8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EC0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EC8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436ED0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436ED8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EE0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EE8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EF0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EF8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F00);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F08);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F10);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F18);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F20);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F28);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F30);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F38);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F40);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F48);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F50);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F58);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F5D);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F5E);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F5F);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F60);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F61);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F62);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F64);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F68);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F6C);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F6E);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F70);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F74);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F78);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F7C);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F80);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F84);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F88);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F90);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F96);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F98);

