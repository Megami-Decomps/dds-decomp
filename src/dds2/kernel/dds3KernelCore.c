#include "common.h"

typedef struct KwlnTask KwlnTask;

extern KwlnTask* D_00435BE8;

typedef s32 (*TaskUpdate)(KwlnTask* task);

typedef void (*TaskDestroy)(KwlnTask* task);

struct KwlnTask {
    u8 unk00[0x18];
    s32 nameSum;
    u32 flags;
    u32 unk20;
    u32 unk24;
    u32 timer;
    s16 unk2C;
    s16 unk2E;
    TaskUpdate unk30;
    TaskDestroy unk34;
    u32 unk38;
    KwlnTask* listNext;
    KwlnTask* listPrev;
    KwlnTask* parent;
    KwlnTask* childList;
    KwlnTask* next;
};

extern void func_001005C8(KwlnTask* task);

extern void func_00100740(KwlnTask* task);

extern void func_00100980(KwlnTask* task);

extern void func_001009C8(void);

extern KwlnTask* D_00435BD0;

extern void func_00100D50(KwlnTask* task);

extern void func_00100DD8(KwlnTask* task);

extern void func_00100E50(void);

extern KwlnTask* D_00435BDC;

extern void* D_00435BD8;

extern void* D_00435BE4;

extern void* D_00435BF0;

extern void func_0035B6E0();

extern u8 D_00435BF8[];

extern u8 D_00411008[];

extern u8 D_00411038[];

extern u8 D_00411048[];

extern u8 D_00411078[];

extern void func_00100C28(void);

extern s32 func_001018B0(void* target);

void func_00100980(KwlnTask* task)
{
    func_001005C8(task);
    task->flags = (task->flags & ~0xF) | 2;
    func_00100740(task);
    task->unk24 = 0;
    task->timer = 0;
}

void func_001009C8(void)
{
    KwlnTask* node;
    KwlnTask* curr;

    node = D_00435BD0;
    while (node != 0) {
        if (node->unk2C > 0) {
            node->unk2C--;
        }
        curr = node;
        node = node->listNext;
        if (curr->unk2C == 0) {
            func_00100980(curr);
        }
    }
}

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100A28);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100C28);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100D50);

void func_00100DD8(KwlnTask* task)
{
    u32 state;

    state = task->flags & 0xF;
    if (state >= 3) {
        return;
    }
    if (state == 0) {
        return;
    }
    func_001005C8(task);
    task->flags = (task->flags & ~0xF) | 3;
    func_00100740(task);
    if (task->unk2E == 0) {
        func_00100D50(task);
    }
}

void func_00100E50(void)
{
    KwlnTask* node;
    KwlnTask* curr;

    node = D_00435BDC;
    while (node != 0) {
        if (node->unk2E > 0) {
            node->unk2E--;
        }
        curr = node;
        node = node->listNext;
        if (curr->unk2E == 0) {
            func_00100D50(curr);
        }
    }
}

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100EB0);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00100F48);

void* func_001010A8(u32 arg0)
{
    switch (arg0 & 0xF) {
    case 1:
        return D_00435BD8;
    case 2:
        return D_00435BF0;
    case 3:
        return D_00435BE4;
    default:
        return 0;
    }
}

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101100);

void func_00101198(KwlnTask* task, void* arg1)
{
    if (task == 0) {
        return;
    }
    do {
        task = task->listNext;
    } while (task != 0);
}

void func_001011D0(void)
{
    u8* tmp;

    func_00101198(D_00435BD0, D_00411008);
    tmp = D_00411038;
    func_0035B6E0(tmp, D_00435BD8);
    func_00101198(D_00435BE8, D_00411048);
    func_0035B6E0(tmp, D_00435BF0);
    func_00101198(D_00435BDC, D_00411078);
    func_0035B6E0(tmp, D_00435BE4);
    func_0035B6E0(D_00435BF8);
}

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101250);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101328);

s32 func_00101428(void)
{
    func_001009C8();
    func_00100C28();
    func_00100E50();
    return 1;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", kwlnTaskCreate);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", kwlnTaskDestroyWithHierarchyByName);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", kwlnTaskDestroyWithHierarchy);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101678);

void func_001016E0(KwlnTask* task, s32 arg1)
{
    u32 state;

    state = task->flags & 0xF;
    if (state == 0) {
        return;
    }
    if (state < 4) {
        task->unk2E = arg1;
    }
}

s32 func_00101700(KwlnTask* task)
{
    u32 state;

    if (func_001018B0(task) == 0) {
        return 0;
    }
    state = task->flags & 0xF;
    return (state < 4) ? state : 0;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101740);

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101820);

/* Persona 4 func_00452490 @ 00452490 (src/Kernel/sdkTask.c), recompiled unchanged */
s32 func_001018B0(void* target)
{
    s32 idx;
    void* node;
    node = 0;
    idx = 0;
    for (; idx < 3; idx++)
    {
        switch (idx)
        {
        case 0:
            node = D_00435BD0;
            break;
        case 1:
            node = D_00435BE8;
            break;
        case 2:
            node = D_00435BDC;
            break;
        }
        while (node != 0)
        {
            if (node == target)
            {
                return 1;
            }
            node = *(void**)((u8*)node + 0x3C);
        }
    }
    return 0;
}

u32 func_00101940(void* task)
{
    return *(u32*)((u8*)task + 0x24);
}

/* Persona 4 kwlnTaskGetTimer @ 00452540 (src/Kernel/sdkTask.c), recompiled unchanged */
u32 kwlnTaskGetTimer(void* task)
{
    return *(u32*)((u8*)task + 0x28);
}

void func_00101950(void* task, u32 value)
{
    *(u32*)((u8*)task + 0x38) = value;
}

u32 func_00101958(void* task)
{
    return *(u32*)((u8*)task + 0x38);
}

void func_00101960(void) {
}

INCLUDE_ASM(const s32, "kernel/dds3KernelCore", func_00101968);

INCLUDE_RODATA(const s32, "kernel/dds3KernelCore", D_00411008);

INCLUDE_RODATA(const s32, "kernel/dds3KernelCore", D_00411038);

INCLUDE_RODATA(const s32, "kernel/dds3KernelCore", D_00411048);

INCLUDE_RODATA(const s32, "kernel/dds3KernelCore", D_00411078);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_00435BF4);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_00435BF8);

INCLUDE_SDATA(const s32, "kernel/dds3KernelCore", D_00435C00);

