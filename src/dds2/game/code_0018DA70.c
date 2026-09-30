#include "common.h"
#include "pcp_vu0.h"

typedef struct PairedEffectResources {
    f32 startVec[4];
    u8 pad10[0x54];
    s32 total;       /* 0x64 */
    s32 fadeIn;      /* 0x68 */
    s32 fadeOut;     /* 0x6C */
    u32 resource[2]; /* 0x70 */
    s32 frame;       /* 0x78 */
    u32 value7C;
} PairedEffectResources;

extern u32 effBlendColor(u32 colorA, u32 colorB, f32 t);
extern void func_0016F6D0(u32 handle);
extern void func_001E31F0(u32 unit, s32 arg1);

void effFreePairedResources(PairedEffectResources *resources) {
    func_0016EFA8(resources->resource[1]);
    func_0016EFA8(resources->resource[0]);
    func_00328E48(resources);
}

void func_0018DAA8(PairedEffectResources *work) {
    s32 total = work->total;
    s32 frame = work->frame;
    s32 fadeIn = work->fadeIn;
    s32 fadeOut = work->fadeOut;
    u32 obj = func_00169438();
    s32 remain;
    f32 t;
    u32 color;
    u32 i;
    u8 *record;
    f32 vec[4];
    if (obj != 0) {
        if (frame < total) {
            remain = total - frame;
            if (frame < fadeIn && fadeIn != 0) {
                t = (f32)frame / (f32)fadeIn;
            } else if (remain <= fadeOut && fadeOut != 0) {
                t = (f32)remain / (f32)fadeOut;
            } else {
                t = 1.0f;
            }
            color = effBlendColor(work->value7C & 0xFFFFFF, work->value7C, t);
            i = 0;
            do {
                func_001E31F0(obj, 0xB);
                VU0_STORE_VF(vf10, vec);
                record = (u8 *)func_0016F018(work->resource[i]);
                VU0_LOAD_VF(vf10, vec);
                VU0_MOVE_VF(vf12, vf10);
                VU0_STORE_VF(vf10, record);
                VU0_LOAD_VF(vf11, work);
                VU0_LERP_VF10(0.33f);
                VU0_STORE_VF(vf10, record + 0x10);
                VU0_MOVE_VF(vf10, vf12);
                VU0_LERP_VF10(0.66f);
                VU0_STORE_VF(vf10, record + 0x20);
                VU0_STORE_VF(vf11, record + 0x30);
                func_0016F020(work->resource[i], color);
                func_0016F6D0(work->resource[i]);
                i++;
            } while (i < 2);
        }
        work->frame++;
    }
}

INCLUDE_ASM(const s32, "game/code_0018DA70", func_0018DC68);

void func_0018DC78(PairedEffectResources *resources, u32 value) {
    resources->value7C = value;
}

INCLUDE_ASM(const s32, "game/code_0018DA70", func_0018DC80);

INCLUDE_ASM(const s32, "game/code_0018DA70", func_0018DFD0);

INCLUDE_ASM(const s32, "game/code_0018DA70", func_0018E0D0);

INCLUDE_ASM(const s32, "game/code_0018DA70", func_0018E350);

INCLUDE_ASM(const s32, "game/code_0018DA70", func_0018E628);

INCLUDE_ASM(const s32, "game/code_0018DA70", func_0018E6D0);

INCLUDE_ASM(const s32, "game/code_0018DA70", func_0018E758);

INCLUDE_ASM(const s32, "game/code_0018DA70", func_0018E800);

INCLUDE_ASM(const s32, "game/code_0018DA70", func_0018E850);
