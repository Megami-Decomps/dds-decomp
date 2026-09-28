#include "common.h"

extern u32 D_00438FC8;

extern u32 *D_00437960;

extern u64 func_00279DC8(u32, u64, u64, u64, u64, u64);

extern u64 func_0027A628(u32, u64, u64);

extern s32 func_002890A8(s32);

extern void func_0026D168(s32, s32, s32);

extern s32 func_0026D0B0(s32, s32);


INCLUDE_RODATA(const s32, "game/code_0028FD30", D_00427560);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_0028FD30);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_0028FEF0);

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_004275B0);

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_004275E8);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00290240);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00290328);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00290410);

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_00427668);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_002906E0);

u32 func_00290A70(s32 arg0) {
    return *(u32 *)(arg0 + 0x7a0);
}

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00290A78);

s32 func_00290B28(s32 object) {
    s32 index = func_002890A8(object);
    s32 *slot = (s32 *)(index * 4 + object + 0x7ac);
    s32 source = *(s32 *)(*(s32 *)(*(s32 *)(object + 4) + 0x1c) + 0x70);
    if (*slot != 0) {
        func_0026D168(*slot, source, 0);
    } else {
        *slot = func_0026D0B0(source, 0);
    }
    return 1;
}

u16 func_00290B98(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(*(s32 *)(**(s32 **)(*(s32 *)(arg0 + 4) + 0x1c) * 4 + arg0 + 0x7ac) + 8);
    if (*(s32 *)(arg0 + 0x7a4) != 0) {
        return *(u16 *)(*(s16 *)(*(s32 *)(arg0 + 0x7a4) + 2) * 2 + temp_v0);
    }
    return *(u16 *)(*(s16 *)(*(s32 *)(arg0 + 0x7a0) + 2) * 2 + temp_v0);
}

u16 func_00290BF0(s32 arg0, s32 arg1) {
    return *(u16 *)
                    (((arg1 << 0x10) >> 0xf) +
                    *(s32 *)(*(s32 *)(**(s32 **)(*(s32 *)(arg0 + 4) + 0x1c) * 4 + arg0 + 0x7ac) + 8));
}

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00290C20);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00290E48);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00291038);

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_004276C0);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00291118);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_002911D0);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00291288);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00291338);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00291400);

void func_00291460(u8 *object, s8 selection) {
    u8 *state = object + 0x240;
    if (func_002747B0(*(s32 *)(object + 0xc00), selection) != 0) {
        u32 flags;
        u32 option;
        func_00289128(object, selection);
        func_00279F90(*(s32 *)(state + 0x9ac), 0);
        flags = *(u32 *)(state + 0x9b0);
        func_00291590(object, 5, 1, selection, (flags >> 28) & 1, 0);
        option = (selection & 0xf) << 24;
        *(u32 *)(state + 0x9b0) = (*(u32 *)(state + 0x9b0) & 0xf0ffffff) | option;
    }
}

void func_00291510(u8 *object, s8 selection) {
    u8 *state = object + 0x240;
    if (func_002747B0(*(s32 *)(object + 0xc00), selection) != 0) {
        func_00289128(object, selection);
        *(u32 *)(state + 0x9b0) = (*(u32 *)(state + 0x9b0) & 0xf0ffffff)
            | ((selection & 0xf) << 24);
    }
}

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00291590);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_002917C0);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00291A20);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00291C68);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00291DD0);

void func_00292458(u8 *object) {
    object += 0x240;
    *(u32 *)(object + 0x9b0) &= 0xff0000ff;
}

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_00427720);

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_00427740);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00292478);

void func_00292998(s32 object) {
    u32 resource;
    s32 record;
    u64 effectHandle;

    resource = *(u32 *)(object + 0xbec);
    func_0026D098(0);
    record = func_00291400(0, 8);
    effectHandle = func_0027A628(resource, 8, 0);
    func_0027A798(effectHandle, 7, 0);
    effectHandle = func_00279DC8(resource, 8, 1, 0, 0, 0);
    func_00279F30(effectHandle, 0, 0, 0, 0x80, 0x53, 0, 0);
    func_0027A798(effectHandle, 8, 0);
    *(u16 *)(record + 2) = (*(u16 *)(record + 2) & 0xfff0) | 1;
}

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00292A60);

s32 func_00292B90(s32 object) {
    return func_002917C0(object, 0, 8);
}


INCLUDE_ASM(const s32, "game/code_0028FD30", func_00292BB0);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00292C58);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00292CF0);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00292EA8);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00292FF0);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00293148);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_002932B0);

void func_00293360(u8 *object) {
    s32 node = *(s32 *)(*(s32 *)(object + 4) + 0x10);
    s32 count = 0;
    u8 *state = object + 0x240;
    if (node != 0) {
        u32 *slot = (u32 *)(object + 0x7f4);
        do {
            u32 value = *(u32 *)(node + 0x70);
            count++;
            node = *(s32 *)(node + 0x58);
            *slot++ = value;
        } while (node != 0);
    }
    *(s32 *)(state + 0x5d8) = count;
    *(s32 *)(state + 0x5d4) = *(s8 *)(object + 0x820);
}

INCLUDE_ASM(const s32, "game/code_0028FD30", func_002933A8);

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_004277A0);

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_00427858);

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_00427888);

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_004278C0);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_002933F0);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00293DB0);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00293FD0);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00294060);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00294420);

void func_00294488(void) {
    if (D_00437960 != (u32 *)0x0) {
        func_003297C8(*D_00437960);
    }
    D_00437960 = (u32 *)0x0;
}

INCLUDE_ASM(const s32, "game/code_0028FD30", func_002944B0);

s32 *func_00294538(void) {
    s32 count = D_00437960[2];
    s32 *entry = (s32 *)D_00437960[1];
    s32 index;
    for (index = 0; index < count; index++, entry += 2) {
        if (entry[0] == 0) {
            return entry;
        }
    }
    return 0;
}

void func_00294580(s32 first, s32 second) {
    s32 *entry = func_00294538();
    entry[0] = first;
    entry[1] = second;
}

void func_002945B8(u32 arg0) {
    D_00438FC8 = arg0;
}
INCLUDE_SDATA(const s32, "game/code_0028FD30", D_00437940);

INCLUDE_SDATA(const s32, "game/code_0028FD30", D_00437948);

INCLUDE_SDATA(const s32, "game/code_0028FD30", D_00437950);

INCLUDE_SDATA(const s32, "game/code_0028FD30", D_00437958);

INCLUDE_SDATA(const s32, "game/code_0028FD30", D_00437960);

