#include "common.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"

typedef struct VTab {
    void (*invoke)(void);
} VTab;

typedef struct VObj {
    VTab *vtable;
} VObj;

typedef struct {
    void *unk0;
    void *unk4;
} Pair;

void sdfSetMotionPointerPair(Pair *a0, void *a1, void *a2);

typedef struct {
    u8 pad[0x30];
    u8 mode;
    u8 previousMode;
} MotionState;

typedef struct SdfMotionTrack {
    u8 pad00[0x10];
    u32 value10;
    union {
        u32 word;
        u16 flags;
    } value14;
    u8 pad18[8];
    u32 value20;
    u8 pad24[4];
    u32 value28;
} SdfMotionTrack;

typedef struct SdfMotionBinding {
    u8 pad00[0x0C];
    SdfMotionTrack *track;
    union {
        u32 word;
        u8 lowByte;
    } current;
} SdfMotionBinding;

typedef struct KeyOut {
    f32 *firstKey;
    f32 *secondKey;
    f32 weight;
} KeyOut;

f32 sdfInterpolateMotionKeys(KeyOut *a0);
extern void effMiscQuaternionNlerpVU(f32 amount);
extern void effMiscQuaternionToMatrixVU(void);

extern s32 (*D_0040B368[])(void *a0, s32 a1);

extern s32 (*D_0040B3F8[])(void *a0, s32 a1);

typedef struct {
    s32 u0;
    s32 u4;
    s32 u8;
    s32 *arr;
} ArrHolder;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    ArrHolder *unkC;
} MidPtr;

typedef struct {
    s32 unk0;
    MidPtr *unk4;
} Src360;

typedef struct SdfMotionTarget {
    u8 pad00[0x18];
    u32 value;
} SdfMotionTarget;

typedef struct SdfMotionOutput {
    Pair pair;
    u32 value;
    SdfMotionTarget *target;
    u32 sampledValue;
} SdfMotionOutput;

void *func_00328D68(s32 size);

void func_00335210(SdfMotionOutput *output, Src360 *source, void *dispatch, s32 options);

extern void *D_0040B420[];

extern void *D_0040B438[];

extern void *D_0040B450[];

extern void *D_0040B468[];

extern void *D_0040B480[];

extern void *D_0040B498[];

extern void *D_0040B4B0[];

extern void *D_0040B4C8[];

extern void *D_0040B4E0[];

extern void *D_0040B4F8[];

typedef struct {
    s32 u0;
    s16 n4;
    s16 pad6;
    s32 u8;
    void **arrC;
} ArrObj;

typedef struct {
    void *unk0;
    void *unk4;
    void *unk8;
    s32 unkC;
    ArrObj *unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    s32 unk24;
    s32 unk28;
    s16 unk2C;
    u16 unk2E;
    u8 state;
    u8 previousState;
    u8 unk32;
    u8 pad33;
} Motion;

void func_00334280(Motion *a0, s32 a1, s32 a2, f32 t0, f32 t1);

typedef struct {
    Pair pair;
    s32 unk8;
    s32 unkC;
} TmpBuf;

typedef struct {
    s32 unk0;
    void *unk4;
} HasPtr4;

s32 func_00330C18(void *a0, s32 a1);

void func_00334930(void *tmp, void *src, void *tbl, s32 x);

extern void *D_0040B380[];

extern void *D_0040B398[];

extern void *D_0040B3B0[];

extern void *D_0040B3C8[];

extern void *D_0040B3E0[];

typedef struct {
    s32 w0;
    s32 w4;
    s32 w8;
    s32 wC;
    s32 w10;
    s32 w14;
    s32 w18;
    f32 f1C;
} SubF;

typedef struct {
    s32 u0;
    s32 u4;
    s32 u8;
    SubF *sub;
    f32 res;
} CmdF;

typedef struct {
    s64 a;
    s64 b;
} __attribute__((packed)) P16;

typedef struct {
    P16 p;
    s32 c;
} Blk;

typedef struct {
    s32 u0;
    s32 u4;
    s32 u8;
    void *sub;
    Blk blk;
} DstBlk;

Blk *sdfEnsurePrimaryTextSubParam(void *a0);

Blk *sdfEnsureSecondaryTextSubParam(void *a0);

void sdfInvokeMotionObjectCallback(VObj *object) {
    object->vtable->invoke();
}

void sdfSetMotionPointerPair(Pair *a0, void *a1, void *a2) {
    a0->unk0 = a2;
    a0->unk4 = a1;
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_003340E0);

void sdfDestroyDevRequest(void *a0);
void func_00328E48(void *a0);

typedef struct Link {
    struct Link *next;
} Link;

typedef struct LinkOwner {
    u8 pad[0x14];
    Link head;
} LinkOwner;

typedef struct MotionNode {
    Link link;
    LinkOwner *owner;
    s32 u8;
    s32 uC;
    ArrObj *request;
} MotionNode;

/* Unlinks the node from its owner's list, notifies each request callback, then frees the request and the node. */
void func_003341B8(MotionNode *node)
{
    Link *prev;
    Link *cur;
    ArrObj *request;
    void **cb;
    s32 n;
    s32 i;

    if (node == NULL) {
        return;
    }
    if (node->owner != NULL) {
        prev = &node->owner->head;
        while ((cur = prev->next) != NULL) {
            if (cur == &node->link) {
                prev->next = node->link.next;
                break;
            }
            prev = cur;
        }
    }
    request = node->request;
    n = request->n4;
    cb = request->arrC;
    for (i = 0; i < n; i++) {
        sdfInvokeMotionObjectCallback(cb[i]);
    }
    sdfDestroyDevRequest(node->request);
    func_00328E48(node);
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00334280);

void func_003343C8(void *a0, s32 a1, s32 a2) {
    func_00334280(a0, a1, a2, 0.0f, 0.0f);
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_003343E8);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00334510);

/* State 6 parks motion processing, retaining the previous state to resume. */
void sdfMotionSuspend(MotionState *state) {
    u8 mode;

    mode = state->mode;
    if (mode != 6) {
        state->previousMode = mode;
        state->mode = 6;
    }
}

void sdfMotionResume(MotionState *state) {
    if (state->mode == 6) {
        state->mode = state->previousMode;
    }
}

void func_00334658(void *a0) {
    func_00328E48(a0);
}

void sdfSetMotionOutputValue(SdfMotionOutput *output, u32 value) {
    output->value = value;
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00334678);

/* Blend two sampled scalar keys; preserve this expression order for matching. */
f32 sdfInterpolateMotionKeys(KeyOut *output) {
    f32 first;
    f32 weight;

    weight = output->weight;
    first = *output->firstKey;
    return (first + (*output->secondKey * weight)) - (first * weight);
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_003347B0);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00334808);

void sdfMotionBlendFiveFloats(f32 *dst, f32 *src1, f32 *src2, f32 weight) {
    s32 i;
    f32 inverseWeight;

    i = 0;
    inverseWeight = 1.0f - weight;
    do {
        dst[i] = src1[i] * inverseWeight + src2[i] * weight;
        i++;
    } while (i != 5);
}

void func_003348D8(KeyOut *src, f32 *dst) {
    sdfMotionBlendFiveFloats(dst, src->firstKey, src->secondKey, src->weight);
}

s32 sdfDispatchMotionBySelector(void *object, s32 selector) {
    return D_0040B368[(u16)selector](object, selector);
}

void func_00334930(void *tmp, void *src, void *tbl, s32 x) {
    sdfSetMotionPointerPair(tmp, src, tbl);
    ((TmpBuf *)tmp)->unkC = func_00330C18(((HasPtr4 *)src)->unk4, x);
}

void *func_00334980(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_00328D68(0x20);
    func_00334930(r, a0, D_0040B380, a2);
    return r;
}

/* vu0 routine: blend the two vec3 keys by the segment weight, store to sub+0x60 */
void func_003349E0(u8 *motion) {
    KeyOut b;

    func_00334678(motion, &b);
    EE_MMI_LOAD_VEC3(vf10, b.firstKey);
    EE_MMI_LOAD_VEC3(vf11, b.secondKey);
    __asm__ volatile(".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\tvsubx.w vf3, vf0, vf2x\n\tvmulax.xyzw ACC, vf11, vf2x\n\tvmaddw.xyzw vf10, vf10, vf3w\n\tvmove.w vf10, vf0\n\t.set reorder" : : "f"(b.weight));
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(*(u8 **)(motion + 0xC) + 0x60));
}

/* vu0 routine: blend the two vec3 keys by the segment weight, then blend that with the vec3 at +0x10 by t2 into sub+0x60 */
void func_00334A60(u8 *motion, f32 t1, f32 t2) {
    KeyOut b;

    func_00334678(motion, &b);
    EE_MMI_LOAD_VEC3(vf10, b.firstKey);
    EE_MMI_LOAD_VEC3(vf11, b.secondKey);
    __asm__ volatile(".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\tvsubx.w vf3, vf0, vf2x\n\tvmulax.xyzw ACC, vf11, vf2x\n\tvmaddw.xyzw vf10, vf10, vf3w\n\tvmove.xyzw vf11, vf10\n\t.set reorder" : : "f"(b.weight));
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(motion + 0x10));
    __asm__ volatile(".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\tvsubx.w vf3, vf0, vf2x\n\tvmulax.xyzw ACC, vf11, vf2x\n\tvmaddw.xyzw vf10, vf10, vf3w\n\tvmove.w vf10, vf0\n\t.set reorder" : : "f"(t2));
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(*(u8 **)(motion + 0xC) + 0x60));
}

void *func_00334B10(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_00328D68(0x20);
    func_00334930(r, a0, D_0040B398, a2);
    return r;
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00334B70);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00334C30);

void *func_00334D10(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_00328D68(0x20);
    func_00334930(r, a0, D_0040B3B0, a2);
    return r;
}

/* vu0 routine: blend the two vec3 keys by the segment weight, store to sub+0x70 */
void func_00334D70(u8 *motion) {
    KeyOut b;

    func_00334678(motion, &b);
    EE_MMI_LOAD_VEC3(vf10, b.firstKey);
    EE_MMI_LOAD_VEC3(vf11, b.secondKey);
    __asm__ volatile(".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\tvsubx.w vf3, vf0, vf2x\n\tvmulax.xyzw ACC, vf11, vf2x\n\tvmaddw.xyzw vf10, vf10, vf3w\n\tvmove.w vf10, vf0\n\t.set reorder" : : "f"(b.weight));
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(*(u8 **)(motion + 0xC) + 0x70));
}

/* vu0 routine: as func_00334A60, stored to sub+0x70 */
void func_00334DF0(u8 *motion, f32 t1, f32 t2) {
    KeyOut b;

    func_00334678(motion, &b);
    EE_MMI_LOAD_VEC3(vf10, b.firstKey);
    EE_MMI_LOAD_VEC3(vf11, b.secondKey);
    __asm__ volatile(".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\tvsubx.w vf3, vf0, vf2x\n\tvmulax.xyzw ACC, vf11, vf2x\n\tvmaddw.xyzw vf10, vf10, vf3w\n\tvmove.xyzw vf11, vf10\n\t.set reorder" : : "f"(b.weight));
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(motion + 0x10));
    __asm__ volatile(".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\tvsubx.w vf3, vf0, vf2x\n\tvmulax.xyzw ACC, vf11, vf2x\n\tvmaddw.xyzw vf10, vf10, vf3w\n\tvmove.w vf10, vf0\n\t.set reorder" : : "f"(t2));
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(*(u8 **)(motion + 0xC) + 0x70));
}

void *func_00334EA0(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_00328D68(0x20);
    func_00334930(r, a0, D_0040B3C8, a2);
    return r;
}

/* vu0 routine: nlerp the two quaternion keys by the segment weight, store quaternion and matrix rows */
void func_00334F00(u8 *motion) {
    KeyOut b;
    u8 *sub;
    u8 *matrix;
    f32 *key;

    func_00334678(motion, &b);
    key = b.firstKey;
    EE_MMI_LOAD_S16X4_FIXED12(vf10, key);
    key = b.secondKey;
    EE_MMI_LOAD_S16X4_FIXED12(vf11, key);
    effMiscQuaternionNlerpVU(b.weight);
    sub = *(u8 **)(motion + 0xC);
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(sub + 0x50));
    matrix = sub + 0x80;
    effMiscQuaternionToMatrixVU();
    __asm__ volatile(".set noreorder\n\tsqc2 vf28, 0(%0)\n\t.set reorder" : : "r"(matrix));
    __asm__ volatile(".set noreorder\n\tsqc2 vf29, 0(%0)\n\t.set reorder" : : "r"(matrix + 0x10));
    __asm__ volatile(".set noreorder\n\tsqc2 vf30, 0(%0)\n\t.set reorder" : : "r"(matrix + 0x20));
}

/* vu0 routine: nlerp the two quaternion keys by the segment weight, nlerp that toward the quaternion at +0x10 by t2, store quaternion and matrix rows */
void func_00334F98(u8 *motion, f32 t1, f32 t2) {
    KeyOut b;
    u8 *sub;
    u8 *matrix;
    f32 *key;

    func_00334678(motion, &b);
    key = b.firstKey;
    EE_MMI_LOAD_S16X4_FIXED12(vf10, key);
    key = b.secondKey;
    EE_MMI_LOAD_S16X4_FIXED12(vf11, key);
    effMiscQuaternionNlerpVU(b.weight);
    sub = *(u8 **)(motion + 0xC);
    VU0_MOVE_VF(vf11, vf10);
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(motion + 0x10));
    effMiscQuaternionNlerpVU(t2);
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(sub + 0x50));
    matrix = sub + 0x80;
    effMiscQuaternionToMatrixVU();
    __asm__ volatile(".set noreorder\n\tsqc2 vf28, 0(%0)\n\t.set reorder" : : "r"(matrix));
    __asm__ volatile(".set noreorder\n\tsqc2 vf29, 0(%0)\n\t.set reorder" : : "r"(matrix + 0x10));
    __asm__ volatile(".set noreorder\n\tsqc2 vf30, 0(%0)\n\t.set reorder" : : "r"(matrix + 0x20));
}

void *func_00335050(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_00328D68(0x14);
    func_00334930(r, a0, D_0040B3E0, a2);
    return r;
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_003350B0);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335108);

void func_00335160(SdfMotionBinding *binding) {
    binding->current.lowByte = ((u8)(binding->track->value14.flags >> 4) ^ 1) & 1;
}

void func_00335180(void *work) {
    PCP_COPY_VECTOR((u8 *)work + 0x10, *(u8 **)((u8 *)work + 0xC) + 0x60);
}

void func_003351A0(void *work) {
    PCP_COPY_VECTOR((u8 *)work + 0x10, *(u8 **)((u8 *)work + 0xC) + 0x50);
}

void func_003351C0(void *work) {
    PCP_COPY_VECTOR((u8 *)work + 0x10, *(u8 **)((u8 *)work + 0xC) + 0x70);
}

s32 sdfDispatchMotionHandler(void *a0, s32 a1) {
    return D_0040B3F8[(u16)a1](a0, a1);
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335210);

void *func_00335268(void *source, s32 unused, s32 options) {
    void *motion;

    motion = func_00328D68(0x14);
    func_00335210(motion, source, D_0040B420, options);
    return motion;
}

extern void func_00333288();

void func_003352C8(u8 *motion) {
    u8 buffer[16];
    func_00334678(motion, buffer);
    func_00333288(*(s32 *)(motion + 0xC), func_00334808(buffer));
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335308);

void sdfCopyTrackStateToBinding(SdfMotionBinding *binding) {
    binding->current.word = binding->track->value14.word;
}

void *func_003353C8(void *source, s32 unused, s32 options) {
    void *motion;

    motion = func_00328D68(0x14);
    func_00335210(motion, source, D_0040B438, options);
    return motion;
}

extern void func_00333270();

void func_00335428(u8 *motion) {
    u8 buffer[16];
    func_00334678(motion, buffer);
    func_00333270(*(s32 *)(motion + 0xC), func_00334808(buffer));
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335468);

void func_00335518(SdfMotionBinding *binding) {
    binding->current.word = binding->track->value10;
}

void *func_00335528(void *source, s32 unused, s32 options) {
    void *motion;

    motion = func_00328D68(0x14);
    func_00335210(motion, source, D_0040B450, options);
    return motion;
}

extern void func_003332A0();

void func_00335588(u8 *motion) {
    u8 buffer[16];
    func_00334678(motion, buffer);
    func_003332A0(*(s32 *)(motion + 0xC), func_00334808(buffer));
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_003355C8);

void func_00335678(SdfMotionBinding *binding) {
    binding->current.word = binding->track->value20;
}

void *func_00335688(void *source, s32 unused, s32 options) {
    void *motion;

    motion = func_00328D68(0x14);
    func_00335210(motion, source, D_0040B468, options);
    return motion;
}

extern void func_003332B8();

void func_003356E8(u8 *motion) {
    u8 buffer[16];
    func_00334678(motion, buffer);
    func_003332B8(*(s32 *)(motion + 0xC), func_00334808(buffer));
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335728);

void func_003357D8(SdfMotionBinding *binding) {
    binding->current.word = binding->track->value28;
}

void *func_003357E8(void *source, s32 unused, s32 options) {
    void *motion;

    motion = func_00328D68(0x14);
    func_00335210(motion, source, D_0040B480, options);
    return motion;
}

extern void func_003332D0(s32, f32);

void func_00335848(u8 *motion) {
    KeyOut sample;
    func_00334678(motion, &sample);
    func_003332D0(*(s32 *)(motion + 0xC), sdfInterpolateMotionKeys(&sample));
}

extern void func_003332D0(s32, f32);

void func_00335888(u8 *motion, f32 unused, f32 scale) {
    KeyOut sample;
    func_00334678(motion, &sample);
    func_003332D0(*(s32 *)(motion + 0xC),
                  *(f32 *)(motion + 0x10) + sdfInterpolateMotionKeys(&sample) * scale - *(f32 *)(motion + 0x10) * scale);
}

void func_003358E0(CmdF *a0) {
    a0->res = a0->sub->f1C;
}

void *func_003358F0(void *source, s32 unused, s32 options) {
    void *motion;

    motion = func_00328D68(0x24);
    func_00335210(motion, source, D_0040B498, options);
    return motion;
}

void func_00335950(SdfMotionOutput *output) {
    u8 keys[16];
    u8 interpolated[32];

    func_00334678(output, keys);
    func_003348D8(keys, interpolated);
    func_003333F8(output->target, interpolated);
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_003359A0);

void func_00335A18(DstBlk *a0) {
    Blk *p;

    p = sdfEnsurePrimaryTextSubParam(a0->sub);
    a0->blk = *p;
}

void *func_00335A68(void *source, s32 unused, s32 options) {
    void *motion;

    motion = func_00328D68(0x24);
    func_00335210(motion, source, D_0040B4B0, options);
    return motion;
}

void func_00335AC8(SdfMotionOutput *output) {
    u8 keys[16];
    u8 interpolated[32];

    func_00334678(output, keys);
    func_003348D8(keys, interpolated);
    func_00333560(output->target, interpolated);
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335B18);

void func_00335B90(DstBlk *a0) {
    Blk *p;

    p = sdfEnsureSecondaryTextSubParam(a0->sub);
    a0->blk = *p;
}

void *func_00335BE0(void *source, s32 unused, s32 options) {
    void *motion;

    motion = func_00328D68(0x14);
    func_00335210(motion, source, D_0040B4C8, options);
    return motion;
}

extern void func_00333460();

void func_00335C40(u8 *motion) {
    u8 buffer[16];
    func_00334678(motion, buffer);
    func_00333460(*(s32 *)(motion + 0xC), func_00334808(buffer));
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_00335C80);

void sdfCopyMotionTargetValue(SdfMotionOutput *output) {
    output->sampledValue = output->target->value;
}

void *func_00335D40(void *source, s32 unused, s32 options) {
    void *motion;

    motion = func_00328D68(0x24);
    func_00335210(motion, source, D_0040B4E0, options);
    return motion;
}

void func_00335DA0(SdfMotionOutput *output) {
    u32 sample[4];

    func_00334678(output, sample);
    func_003333F8(output->target, sample[0]);
}

void func_00335DD8(SdfMotionOutput *output) {
    u32 sample[4];

    func_00334678(output, sample);
    func_003333F8(output->target, sample[0]);
}

void func_00335E10(void) {
}

void *func_00335E18(void *source, s32 unused, s32 options) {
    void *motion;

    motion = func_00328D68(0x24);
    func_00335210(motion, source, D_0040B4F8, options);
    return motion;
}

void func_00335E78(SdfMotionOutput *output) {
    u32 sample[4];

    func_00334678(output, sample);
    func_00333560(output->target, sample[0]);
}

void func_00335EB0(SdfMotionOutput *output) {
    u32 sample[4];

    func_00334678(output, sample);
    func_00333560(output->target, sample[0]);
}
