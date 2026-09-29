#include "common.h"

/* Global event state behind func_00101958; flag bits live at offset 0x8. */
typedef struct EvtGlobal {
    u8 pad[8];
    u32 *flags;
} EvtGlobal;

extern EvtGlobal *func_00101958(void);

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

/* Apply a caller-provided mask to the event state's flag word. */
void evtPolygonMovieSetFlagBits(u32 unused, u32 bits) {
    EvtGlobal *state;

    state = func_00101958();
    *state->flags = *state->flags | bits;
}

void evtPolygonMovieClearFlagBits(u32 unused, u32 bits) {
    EvtGlobal *state;

    state = func_00101958();
    *state->flags = *state->flags & ~bits;
}

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_0024F090);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_0024F130);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_0024F178);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_0024F1B8);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_0024F7D0);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_0024F8D0);

INCLUDE_RODATA(const s32, "event/evtPolygonMovie", D_004231F0);

