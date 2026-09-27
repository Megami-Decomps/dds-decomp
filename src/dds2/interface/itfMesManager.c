#include "common.h"

void func_00344120(int *param_1, int param_2, u8 *param_3, int param_4);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3370);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A33C0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3410);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3458);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A34D0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3560);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3568);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3570);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A35F0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3658);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A36C0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3700);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3780);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A37D0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3838);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A38A0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A38B0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A38C0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A38D8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A39D0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3A18);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3C28);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3CC8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3DA8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3EE0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4008);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4090);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A40B0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4118);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A41A8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4218);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A42A8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4318);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A43A8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4418);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4488);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A44A8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A44D8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4508);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4538);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4558);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4578);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4598);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A45C0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4858);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4888);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A48B8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4940);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4988);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4A10);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4AD8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4B98);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4BB8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4BD8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4CB0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4CC8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4CE8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4CF8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A50C0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A50D8);

/* Persona 4 func_00278d50 @ 00278D50 (src/itfMesManager.c), recompiled unchanged */
void func_001A5180(u8 *arg0)
{
    u8 *base;
    u8 *fixups;
    s32 size;
    if (*(u8 *)(arg0 + 0x1C) == 0) {
        base = arg0 + 0x20;
        fixups = arg0 + *(s32 *)(arg0 + 0x10);
        size = *(s32 *)(arg0 + 0x14);
        func_00344120((int *)base, (int)base, fixups, size);
        *(u8 *)(arg0 + 0x1C) = 1;
    }
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A51C8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A51F8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A5210);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A5228);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A5260);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A5288);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A52B8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A5480);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A55B0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A5600);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A5640);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A5670);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A5760);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A5880);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A58B8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A58E8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A5918);

/* Persona 4 func_0027a340 @ 0027A340 (src/itfMesManager.c), recompiled unchanged */
void func_001A5950(u8 *arg0, int arg1)
{
    while (arg0 != ((void*)0)) {
        *(int *)(arg0 + 0x14) = arg1;
        arg0 = *(u8 **)(arg0 + 0x24);
    }
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A5988);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A5A28);

/* Persona 4 func_0027a4d0 @ 0027A4D0 (src/itfMesManager.c), recompiled unchanged */
void func_001A5A80(int param_1,u32 param_2)
{
  int iVar1;
  for (; param_1 != 0; param_1 = *(int *)(param_1 + 0x24)) {
    for (iVar1 = *(int *)(param_1 + 0x1c); iVar1 != 0; iVar1 = *(int *)(iVar1 + 0x28)) {
      *(u32 *)(iVar1 + 0x10) = *(u32 *)(iVar1 + 0x10) & 0xffffff00 | param_2;
    }
  }
  return;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A5AD0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A5B20);

/* Persona 4 func_0027a580 @ 0027A580 (src/itfMesManager.c), recompiled unchanged */
void func_001A5B70(int param_1)
{
  for (; param_1 != 0; param_1 = *(int *)(param_1 + 0x24)) {
    if (*(u8 *)(*(int *)(param_1 + 0x1c) + 0x16) == '\0') {
      func_0019D038(param_1);
    }
  }
  return;
}
