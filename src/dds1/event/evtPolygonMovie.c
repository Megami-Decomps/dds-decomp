#include "common.h"

/* Global event state behind func_00101A70; flag bits live at offset 0x8. */
typedef struct EvtGlobal {
    u8 pad[8];     /* 0x0 */
    u32 *flags;    /* 0x8 */
} EvtGlobal;

/* Polygon-movie clip: playback position is clamped to [0, duration]. */
typedef struct PolyMovieClip {
    u8 pad[8];       /* 0x0 */
    f32 duration;    /* 0x8 */
    f32 position;    /* 0xc */
} PolyMovieClip;

s32 func_00101A70(void);

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

void evtPolygonMovieSetFlagBits(s32 arg0, u32 bits)
{
    EvtGlobal *state;

    state = (EvtGlobal *)func_00101A70();
    *state->flags = *state->flags | bits;
}

void evtPolygonMovieClearFlagBits(s32 arg0, u32 bits)
{
    EvtGlobal *state;

    state = (EvtGlobal *)func_00101A70();
    *state->flags = *state->flags & ~bits;
}

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_002342F8);

void evtPolygonMovieClampTime(s32 arg0, s32 arg1, s32 start, s32 end)
{
    PolyMovieClip *clip;
    s32 frame;

    frame = end - start;
    if (frame < 0) {
        frame = 0;
    }
    clip = *(PolyMovieClip **)(*(s32 *)(arg0 + 0x18) + 4);
    if (clip != NULL) {
        if ((s32)clip->duration <= frame) {
            frame = (s32)clip->duration;
        }
        clip->position = (f32)frame;
    }
}

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_002343D8);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_00234418);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_00234A30);

INCLUDE_ASM(const s32, "event/evtPolygonMovie", func_00234B30);







INCLUDE_RODATA(const s32, "event/evtPolygonMovie", D_003ADC40);

