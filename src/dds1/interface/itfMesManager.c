#include "common.h"

void func_002EB278(int *param_1, int param_2, u8 *param_3, int param_4);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B340);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B390);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B3E0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B428);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B4A0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B530);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B538);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B540);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B5C0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B628);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B690);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B6D0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B750);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B7A0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B808);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B870);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B880);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B890);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B8A8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B9A0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B9E8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019BBF8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019BC98);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019BD78);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019BEB0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019BFD8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C060);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C080);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C0E8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C178);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C1E8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C278);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C2E8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C378);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C3E8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C458);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C478);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C4A8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C4D8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C508);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C528);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C548);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C568);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C590);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C838);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C868);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C898);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C920);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C968);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C9F0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019CAB8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019CB78);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019CB98);

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

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D1A8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D1D8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D1F0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D208);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D240);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D268);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D298);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D460);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D580);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D5D0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D610);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D640);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D730);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D850);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D888);

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
