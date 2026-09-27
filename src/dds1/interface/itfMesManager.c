#include "common.h"

/* One 0x14-byte slot per message window; the first field points at its state. */
typedef struct ItfMesSlot {
    void *mes;
    u8 unk4[0x10];
} ItfMesSlot;

extern ItfMesSlot D_003D6ECC[];

extern u32 func_0019D1F0(u32);

extern u32 D_003BB1E8;

void func_002EB278(int *param_1, int param_2, u8 *param_3, int param_4);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B340);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B390);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B3E0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B428);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B4A0);

u32 func_0019B530(void) {
    return 1;
}

u32 func_0019B538(void) {
    return 1;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B540);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B5C0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B628);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B690);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B6D0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B750);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B7A0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B808);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B870);

void func_0019B880(u32 arg0) {
    D_003BB1E8 = D_003BB1E8 | arg0;
}

void func_0019B890(u32 arg0) {
    D_003BB1E8 = D_003BB1E8 & ~arg0;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B8A8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B9A0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B9E8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019BBF8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019BC98);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019BD78);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019BEB0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019BFD8);

void func_0019C060(s32 arg0, u32 arg1) {
    *(u32 *)((s32)D_003D6ECC[arg0].mes + 0x4c) = arg1;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C080);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C0E8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C178);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C1E8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C278);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C2E8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C378);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C3E8);

u32 func_0019C458(s32 arg0) {
    return *(u32 *)D_003D6ECC[arg0].mes;
}

void func_0019C478(s32 arg0, u32 arg1) {
    u32 *puVar1;

    puVar1 = (u32 *)D_003D6ECC[arg0].mes;
    *puVar1 = (u32)(u16)*puVar1 | (arg1 & 0xffff0000);
}

void func_0019C4A8(s32 arg0, u32 arg1) {
    u32 *puVar1;

    puVar1 = (u32 *)D_003D6ECC[arg0].mes;
    *puVar1 = *puVar1 | (arg1 & 0xffff0000);
}

void func_0019C4D8(s32 arg0, u32 arg1) {
    u32 *puVar1;

    puVar1 = (u32 *)D_003D6ECC[arg0].mes;
    *puVar1 = *puVar1 & (~arg1 | 0xffff);
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C508);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C528);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C548);

u32 func_0019C568(s32 arg0, s32 arg1) {
    return *(u32 *)(arg1 * 4 + (s32)D_003D6ECC[arg0].mes + 0xd0);
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C590);

void func_0019C838(s32 arg0, u32 arg1, u32 arg2) {
    func_0019D460((u32)D_003D6ECC[arg0].mes, arg1, arg2, 0);
}

void func_0019C868(s32 arg0) {
    func_0019D460((u32)D_003D6ECC[arg0].mes);
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C898);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C920);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C968);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C9F0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019CAB8);

void func_0019CB78(s32 arg0, u8 arg1) {
    *(u8 *)((s32)D_003D6ECC[arg0].mes + 0x39) = arg1;
}

void func_0019CB98(s32 arg0, u32 arg1) {
    *(u32 *)((s32)D_003D6ECC[arg0].mes + 0x1dc) = arg1;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019CBB8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019CC90);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019CCA8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019CCC8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019CCD8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D0A0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D0B8);

/* Persona 4 func_00278d50 @ 00278D50 (src/itfMesManager.c), recompiled unchanged */
void func_0019D160(u8 *arg0)
{
    u8 *base;
    u8 *fixups;
    s32 size;
    if (*(u8 *)(arg0 + 0x1C) == 0) {
        base = arg0 + 0x20;
        fixups = arg0 + *(s32 *)(arg0 + 0x10);
        size = *(s32 *)(arg0 + 0x14);
        func_002EB278((int *)base, (int)base, fixups, size);
        *(u8 *)(arg0 + 0x1C) = 1;
    }
}

u32 func_0019D1A8(s32 arg0) {
    u32 temp_v0;

    temp_v0 = 0;
    if ((*(s32 *)(arg0 + 8) == 0x3047534d) || (*(s32 *)(arg0 + 8) == 0x3147534d)) {
        temp_v0 = 1;
    }
    return temp_v0;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D1D8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D1F0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D208);

u32 func_0019D240(s32 arg0) {
    return *(u32 *)(*(s32 *)((s32)D_003D6ECC[arg0].mes + 4) + 0x18);
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D268);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D298);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D460);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D580);

u32 func_0019D5D0(s32 arg0) {
    s32 *piVar1;

    piVar1 = (s32 *)func_0019D1F0(*(u32 *)(arg0 + 4));
    return *(u32 *)((u32)*(u16 *)(arg0 + 0x20) * 4 + *piVar1);
}

s32 func_0019D610(s32 arg0, u32 arg1) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v0 = 0;
    while (0 < arg0) {
        temp_v1 = arg1 & 1;
        arg1 = arg1 >> 1;
        arg0 = arg0 - 1;
        if (temp_v1 == 0) {
            temp_v0 = temp_v0 + 1;
        }
    }
    return temp_v0;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D640);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D730);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D850);

void func_0019D888(u8 *arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x20);
    *(u8 *)(arg1 + 0x15) = *arg0 >> 1;
    *(u8 *)(arg1 + 0x12) = *(u8 *)(temp_v0 + 0x15);
    *(u8 *)(arg1 + 0x13) = *(u8 *)(temp_v0 + 0x14);
    *(u8 *)(arg1 + 0x14) = *(u8 *)(temp_v0 + 0x16);
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D8B8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D8E8);

/* Persona 4 func_0027a340 @ 0027A340 (src/itfMesManager.c), recompiled unchanged */
void func_0019D920(u8 *arg0, int arg1)
{
    while (arg0 != ((void*)0)) {
        *(int *)(arg0 + 0x14) = arg1;
        arg0 = *(u8 **)(arg0 + 0x24);
    }
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D958);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D9F8);

/* Persona 4 func_0027a4d0 @ 0027A4D0 (src/itfMesManager.c), recompiled unchanged */
void func_0019DA50(int param_1,u32 param_2)
{
  int iVar1;
  for (; param_1 != 0; param_1 = *(int *)(param_1 + 0x24)) {
    for (iVar1 = *(int *)(param_1 + 0x1c); iVar1 != 0; iVar1 = *(int *)(iVar1 + 0x28)) {
      *(u32 *)(iVar1 + 0x10) = *(u32 *)(iVar1 + 0x10) & 0xffffff00 | param_2;
    }
  }
  return;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019DAA0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019DAF0);

/* Persona 4 func_0027a580 @ 0027A580 (src/itfMesManager.c), recompiled unchanged */
void func_0019DB40(int param_1)
{
  for (; param_1 != 0; param_1 = *(int *)(param_1 + 0x24)) {
    if (*(u8 *)(*(int *)(param_1 + 0x1c) + 0x16) == '\0') {
      func_00195388(param_1);
    }
  }
  return;
}
