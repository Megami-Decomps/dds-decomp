#include "common.h"

extern u64 func_00325BB0(u64, u32);

extern u64 func_00325AB8(u64, u32);

extern u64 func_00320AE8(u64, u64, u32 *);

extern u64 func_00325790(u64, u32);

extern u64 func_0031F0E8(void);

extern void (*D_004389C4)(void);

extern void func_00320C88(u32);

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_0031F0E8);

void func_0031F138(u32 node) {
    func_0035A880(*(u32 *)(node + 4));
    func_0035A880(node);
}


INCLUDE_ASM(const s32, "game/code_0031F0E8", func_0031F168);

void func_0031F1B8(u32 node) {
    func_0035A880(*(u32 *)node);
    func_0035A880(node);
}


void func_0031F1E8(u32 unused, u32 node) {
    func_0031F138(node);
}


void func_0031F208(u32 unused, u32 node) {
    func_0031F1B8(node);
}


u64 func_0031F228(s32 arg0) {
    u64 temp_v0;

    temp_v0 = func_0031F0E8();
    func_00320CE0(*(u32 *)(arg0 + 4), 0, temp_v0);
    return temp_v0;
}

u32 func_0031F270(s32 arg0) {
    return *(u32 *)(*(s32 *)(*(s32 *)(arg0 + 4) + 8) + 0x10);
}

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_0031F280);

void func_0031F300(u32 node) {
    func_00320C88(*(u32 *)(node + 4));
    func_00320C88(*(u32 *)(node + 8));
    func_00320C88(*(u32 *)(node + 0xc));
    func_0035A880(node);
}


INCLUDE_ASM(const s32, "game/code_0031F0E8", func_0031F340);

void func_0031F410(arg0, arg1)
    u32 arg0;
    u32 arg1;
{
    u32 temp_v0[4];
    temp_v0[0] = arg1;
    func_0031F340(arg0, temp_v0);
}

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_0031F430);

void func_0031F4D8(u32 context, const char *name) {
    u32 record = func_0031F168();
    strncpy((char *)*(u32 *)record, name, 0x40);
    func_00320CE0(*(u32 *)(context + 0xc), 0, record);
    *(u32 *)(record + 4) = *(u32 *)context;
    func_0031F410(context, -1, 4);
}


u32 func_0031F550(u32 context, const char *name) {
    u32 node = *(u32 *)(context + 4);
    while (node) {
        u32 data = *(u32 *)(node + 0x10);
        if (strncmp(*(char **)data, name, 0x40) == 0) {
            return data;
        }
        node = *(u32 *)(node + 8);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_0031F5C0);

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_0031F618);

u32 func_0031F6A0(u32 object) {
    u32 node = *(u32 *)(*(u32 *)(object + 0xc) + 4);
    u32 destination;
    if (node == 0) {
        return 0;
    }
    destination = func_0031F280(object);
    do {
        func_0031F410(destination, *(u32 *)(*(u32 *)(node + 0x10) + 4), 4);
        node = *(u32 *)(node + 8);
    } while (node != 0);
    return destination;
}

u32 func_0031F708(u32 *object, u32 extra) {
    u32 node = *(u32 *)(object[1] + 4);
    while (node) {
        u32 value = *(u32 *)(node + 0x10);
        func_0035A648(*(u32 *)(value + 4), *(u32 *)value, 1, extra);
        node = *(u32 *)(node + 8);
    }
    return object[0];
}

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_0031F778);

void func_0031F840(u8 *base, u32 adjustment, u32 *offsets, u32 size) {
    s32 count = size >> 2;
    while (count > 0) {
        u32 *slot = (u32 *)(base + *offsets++);
        *slot += adjustment;
        count--;
    }
}

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_0031F878);

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_0031FA60);

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_00320020);

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_003201A0);

u32 func_00320380(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_00320388);

u64 func_00320510(u64 arg0, u64 arg1) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    temp_v0 = func_00320AE8(arg0, arg1, temp_v2);
    temp_v1 = func_00325790(temp_v0, temp_v2[0]);
    func_0035A880(temp_v0);
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_00320560);

u64 func_003206E8(u64 arg0, u64 arg1) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    temp_v0 = func_00320AE8(arg0, arg1, temp_v2);
    temp_v1 = func_00325790(temp_v0, temp_v2[0]);
    func_0035A880(temp_v0);
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_00320738);

u64 func_003208C0(u64 arg0, u64 arg1) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    temp_v0 = func_00320AE8(arg0, arg1, temp_v2);
    temp_v1 = func_00325AB8(temp_v0, temp_v2[0]);
    func_0035A880(temp_v0);
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_00320910);

u64 func_00320A98(u64 arg0, u64 arg1) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    temp_v0 = func_00320AE8(arg0, arg1, temp_v2);
    temp_v1 = func_00325BB0(temp_v0, temp_v2[0]);
    func_0035A880(temp_v0);
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_00320AE8);

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_00320C28);

void func_00320C88(u32 node) {
    if (node != 0) {
        void (*callback)(s32, u32);
        func_00321018(node);
        callback = *(void (**)(s32, u32))(node + 0x14);
        callback(-1, *(u32 *)(node + 0xc));
        func_0035A880(node);
    }
}


void func_00320CD0(s32 arg0, s32 arg1) {
    if (arg1 != 0) {
        *(s32 *)(arg0 + 0x14) = (s32)arg1;
    }
}

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_00320CE0);

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_00320D80);

void func_00320EA8(s32 arg0, s32 arg1) {
    if (arg1 != 0) {
        *(s32 *)(arg0 + 0x10) = (s32)arg1;
    }
}

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_00320EB8);

u32 func_00320F68(u32 list, u32 node) {
    u32 remaining;
    if (node == 0) {
        return 0;
    }
    remaining = func_00320EB8(list, node);
    (*(void (**)(u32, u32))(list + 0x10))(*(u32 *)node, *(u32 *)(node + 0x10));
    func_0035A880(node);
    return remaining;
}
INCLUDE_SDATA(const s32, "game/code_0031F0E8", D_00438960);

INCLUDE_SDATA(const s32, "game/code_0031F0E8", D_00438968);

INCLUDE_SDATA(const s32, "game/code_0031F0E8", D_00438970);

INCLUDE_SDATA(const s32, "game/code_0031F0E8", D_00438978);

INCLUDE_SDATA(const s32, "game/code_0031F0E8", D_00438980);

INCLUDE_SDATA(const s32, "game/code_0031F0E8", D_00438988);

INCLUDE_SDATA(const s32, "game/code_0031F0E8", D_00438990);

INCLUDE_SDATA(const s32, "game/code_0031F0E8", D_00438998);

