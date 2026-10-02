#include "common.h"

INCLUDE_ASM(const s32, "game/code_002945C0", func_002945C0);

extern void func_00306CD0(s32, s32, s32, s32, s32, void *, s32, s32);

typedef struct BlendDispatchWork {
    u8 pad00[8];
    u32 mode;      /* 0x08: must be 2 */
    u8 pad0C[0x64];
    void *first;   /* 0x70 */
    void *second;  /* 0x74 */
} BlendDispatchWork;

void func_00294680(BlendDispatchWork *w, s32 a1, s32 a2) {
    void *p;

    if (w->mode != 2) {
        return;
    }
    func_00306CD0(0xE30, 0x610, 0, a1, 0, w->first, 1, a2);
    p = *(void **)((u8 *)w->second + 0x18);
    *(f32 *)((u8 *)p + 0xC4) = 90.0f;
    func_00306CD0(0x9F0, 0x610, 0, a1, 2, w->second, 1, a2);
}
