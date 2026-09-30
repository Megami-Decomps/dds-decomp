#include "common.h"
#include "pcp_vu0.h"

typedef struct {
    f32 startVec[4];
    u8 pad10[0x54];
    s32 total;       /* 0x64 */
    s32 fadeIn;      /* 0x68 */
    s32 fadeOut;     /* 0x6C */
    u32 resource[2]; /* 0x70 */
    s32 frame;       /* 0x78 */
    u32 value7C;
} EffectPair;

extern u32 effBlendColor(u32 colorA, u32 colorB, f32 t);
extern void func_00167A78(u32 handle);
extern void func_001D63E8(u32 unit, s32 arg1);

void effFreePairedResources(EffectPair *pair) {
    func_00167350(pair->resource[1]);
    func_00167350(pair->resource[0]);
    func_002CFF98(pair);
}

void func_00185E50(EffectPair *work) {
    s32 total = work->total;
    s32 frame = work->frame;
    s32 fadeIn = work->fadeIn;
    s32 fadeOut = work->fadeOut;
    u32 obj = func_00161858();
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
                if (*(u16 *)(obj + 0x124) == 0x109) {
                    func_001D63E8(obj, i + 0x14);
                } else {
                    func_001D63E8(obj, 0xB);
                }
                VU0_STORE_VF(vf10, vec);
                record = (u8 *)func_001673C0(work->resource[i]);
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
                func_001673C8(work->resource[i], color);
                func_00167A78(work->resource[i]);
                i++;
            } while (i < 2);
        }
        work->frame++;
    }
}

void func_00186030(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00186040(EffectPair *pair, u32 value) {
    pair->value7C = value;
}

INCLUDE_ASM(const s32, "game/code_00185E18", func_00186048);

INCLUDE_ASM(const s32, "game/code_00185E18", func_00186398);

INCLUDE_ASM(const s32, "game/code_00185E18", func_00186498);

INCLUDE_ASM(const s32, "game/code_00185E18", func_00186718);

INCLUDE_ASM(const s32, "game/code_00185E18", func_001869F0);

INCLUDE_ASM(const s32, "game/code_00185E18", func_00186A98);

INCLUDE_ASM(const s32, "game/code_00185E18", func_00186B20);

INCLUDE_ASM(const s32, "game/code_00185E18", func_00186BC8);

INCLUDE_ASM(const s32, "game/code_00185E18", func_00186C18);

void func_00186CB8(void) {
    func_002CFF98();
}

INCLUDE_ASM(const s32, "game/code_00185E18", func_00186CD0);

INCLUDE_ASM(const s32, "game/code_00185E18", func_00186D48);
