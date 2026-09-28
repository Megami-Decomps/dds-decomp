#include "common.h"

extern s32 func_00101958(void);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_0024DD20);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_0024DD48);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_0024DD78);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_0024DDB8);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_0024DE70);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_0024DF38);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_0024E0A0);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_0024E370);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_0024E4E0);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_0024E688);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_0024E7A8);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_0024E8B8);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_0024EA60);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_0024EBE0);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_0024ED50);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_0024EEB8);

void evtPolygonMovieSetFlagBits(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    **(u32 **)(temp_v0 + 8) = **(u32 **)(temp_v0 + 8) | arg1;
}

void evtPolygonMovieClearFlagBits(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    **(u32 **)(temp_v0 + 8) = **(u32 **)(temp_v0 + 8) & ~arg1;
}

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_0024F090);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_0024F130);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_0024F178);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_0024F1B8);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_0024F7D0);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_0024F8D0);







INCLUDE_RODATA(const s32, "event/evtPolygonMovie", D_004231F0);

