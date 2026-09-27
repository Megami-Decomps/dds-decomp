#include "common.h"

extern s32 func_00101A70(void);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_00232F88);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_00232FB0);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_00232FE0);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_00233020);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_002330D8);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_002331A0);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_00233308);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_002335D8);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_00233748);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_002338F0);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_00233A10);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_00233B20);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_00233CC8);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_00233E48);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_00233FB8);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_00234120);

void func_00234288(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    **(u32 **)(temp_v0 + 8) = **(u32 **)(temp_v0 + 8) | arg1;
}

void func_002342C0(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    **(u32 **)(temp_v0 + 8) = **(u32 **)(temp_v0 + 8) & ~arg1;
}

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_002342F8);

void func_00234398(s32 arg0, u32 arg1, s32 arg2, s32 arg3) {
    s32 temp_v0;

    arg3 = arg3 - arg2;
    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0x18) + 4);
    if (arg3 < 0) {
        arg3 = 0;
    }
    if (temp_v0 != 0) {
        if ((s32)*(float *)(temp_v0 + 8) <= arg3) {
            arg3 = (s32)*(float *)(temp_v0 + 8);
        }
        *(float *)(temp_v0 + 0xc) = (float)arg3;
    }
}

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_002343D8);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_00234418);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_00234A30);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_00234B30);

INCLUDE_RODATA(const s32, "event/evtPolygonMovie", D_003ADC40);

