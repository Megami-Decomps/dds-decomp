#include "common.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"

typedef struct {
    void *unk0;
    void *unk4;
} Pair;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Triple;


typedef struct {
    f32 *unk0;
    f32 *unk4;
    f32 unk8;
} BlendArg;

typedef struct VTab {
    void (*invoke)(void);
} VTab;

typedef struct VObj {
    VTab *vtable;
} VObj;

typedef struct KeyOut {
    f32 *firstKey;
    f32 *secondKey;
    f32 weight;
} KeyOut;

typedef struct {
    Pair pair;
    s32 unk8;
    s32 unkC;
} TmpBuf;

typedef struct {
    s32 unk0;
    void *unk4;
} HasPtr4;

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

typedef struct {
    Pair pair;
    s32 unk8;
    s32 unkC;
} Dst360;

typedef struct {
    s32 i0;
    s32 i4;
    s32 i8;
    s32 iC;
    s32 i10;
    s32 i14;
    s32 i18;
    s32 i1C;
    s32 i20;
    s32 i24;
    s32 i28;
} SubI;

typedef struct {
    s32 u0;
    s32 u4;
    s32 u8;
    SubI *sub;
    s32 res;
} CmdI;

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
    u8 pad[0x14];
    u16 u14;
} SubU;

typedef struct {
    s32 u0;
    s32 u4;
    s32 u8;
    SubU *sub;
    s8 res;
} CmdB;
typedef struct {
    u16 u0;
    u16 u2;
    u16 n;
    u16 stride;
    u16 data[1];
} Keys;

typedef struct {
    u8 pad[0x2E];
    u16 unk2E;
    u8 unk30;
    u8 unk31;
    u8 unk32;
} Ctl7;

typedef struct {
    s32 u0;
    Ctl7 *ctl;
    Keys *keys;
} Arg7;

typedef struct {
    s32 u0;
    s32 u4;
    s32 u8;
    void *sub;
} HasSub;

typedef struct {
    s32 u0;
    s32 u4;
    s32 u8;
    SubU *sub;
} HasSubU;

typedef struct {
    s32 u0;
    s32 u4;
    s32 u8;
    void *sub;
    f32 f10;
} HasSubF;

typedef struct {
    s32 u0;
    s32 u4;
    s32 u8;
    void *sub;
    f32 f10[5];
} HasArr;

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

typedef struct {
    void *unk0;
    void *unk4;
    s32 u8;
    s32 uC;
    ArrObj *unk10;
} Obj308;

typedef struct {
    u8 pad[0x14];
    void *unk14;
} HasLink14;

typedef struct {
    u16 u0;
    u16 n;
    void **tbl;
    s32 data[1];
} KeysH;

typedef struct FuncTab {
    void *w0;
    void (*w4)(void *a0, void *a1);
    void (*w8)(void *a0, void *a1, f32 t);
    void (*wC)(void *a0);
    void (*w10)(void *a0, f32 t1, f32 t2);
} FuncTab;

void *func_002CFF68(s32 size);
void func_002CFF98(void *a0);
void *func_002CFEB8(s32 size);
ArrObj *sdfDevCreateBufferedRequest(u16 n, s32 e1, s32 e2);
s32 func_002DB1C8(void *a0, s32 a1, s32 a2);
s32 func_002D7D68(void *a0, s32 a1);
void func_002DA3C0(void *a0, s32 a1);
void func_002DA3D8(void *a0, s32 a1);
void func_002DA3F0(void *a0, s32 a1);
void func_002DA408(void *a0, s32 a1);
void func_002DA420(void *a0, f32 a1);
Blk *sdfEnsurePrimaryTextSubParam(void *a0);
void func_002DA548(void *a0, void *a1);
void func_002DA5B0(void *a0, s32 a1);
Blk *sdfEnsureSecondaryTextSubParam(void *a0);
void func_002DA6B0(void *a0, void *a1);
void sdfDestroyDevRequest(void *a0);
void sdfSetMotionPointerPair(Pair *a0, void *a1, void *a2);
void func_002DB3D0(Motion *a0, s32 a1, s32 a2, f32 t0, f32 t1);
f32 sdfInterpolateMotionKeys(KeyOut *a0);
void func_002DB7C8(void *a0, KeyOut *out, f32 t);
s32 func_002DB958(KeyOut *a0);
void func_002DBA80(void *tmp, void *src, void *tbl, s32 x);
void func_002DC360(Dst360 *a0, Src360 *a1, void *a2, s32 a3);
extern void effMiscQuaternionNlerpVU(f32 amount);
extern void effMiscQuaternionToMatrixVU(void);
extern s32 (*D_003981B8[])(void *a0, s32 a1);
extern s32 (*D_00398248[])(void *a0, s32 a1);
extern void *D_003981D0[];
extern void *D_003981E8[];
extern void *D_00398200[];
extern void *D_00398218[];
extern void *D_00398230[];
extern void *D_00398270[];
extern void *D_00398288[];
extern void *D_003982A0[];
extern void *D_003982B8[];
extern void *D_003982D0[];
extern void *D_003982E8[];
extern void *D_00398300[];
extern void *D_00398318[];
extern void *D_00398330[];
extern void *D_00398348[];

void sdfInvokeMotionObjectCallback(VObj *object) {
    object->vtable->invoke();
}

void sdfSetMotionPointerPair(Pair *a0, void *a1, void *a2) {
    a0->unk0 = a2;
    a0->unk4 = a1;
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DB230);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DB308);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DB3D0);
void func_002DB518(void *a0, s32 a1, s32 a2) {
    func_002DB3D0(a0, a1, a2, 0.0f, 0.0f);
}


INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DB538);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DB660);
/* State 6 parks motion processing, retaining the previous state to resume. */
void sdfMotionSuspend(Motion *motion) {
    u8 previous;

    previous = motion->state;
    if (previous != 6) {
        motion->previousState = previous;
        motion->state = 6;
    }
}

void sdfMotionResume(Motion *motion) {
    if (motion->state == 6) {
        motion->state = motion->previousState;
    }
}

void func_002DB7A8(void *a0) {
    func_002CFF98(a0);
}

void sdfSetMotionOutputValue(Triple *a0, s32 a1) {
    a0->unk8 = a1;
}
INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DB7C8);

f32 sdfInterpolateMotionKeys(KeyOut *a0) {
    f32 t;
    f32 a;

    t = a0->weight;
    a = *a0->firstKey;
    return (a + (*a0->secondKey * t)) - (a * t);
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DB900);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DB958);

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

void func_002DBA28(KeyOut *src, f32 *dst) {
    sdfMotionBlendFiveFloats(dst, src->firstKey, src->secondKey, src->weight);
}

s32 sdfDispatchMotionBySelector(void *a0, s32 a1) {
    return D_003981B8[(u16)a1](a0, a1);
}

void func_002DBA80(void *tmp, void *src, void *tbl, s32 x) {
    sdfSetMotionPointerPair(tmp, src, tbl);
    ((TmpBuf *)tmp)->unkC = func_002D7D68(((HasPtr4 *)src)->unk4, x);
}

void *func_002DBAD0(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x20);
    func_002DBA80(r, a0, D_003981D0, a2);
    return r;
}

/* vu0 routine: blend the two vec3 keys by the segment weight, store to sub+0x60 */
void func_002DBB30(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    EE_MMI_LOAD_VEC3(vf10, b.firstKey);
    EE_MMI_LOAD_VEC3(vf11, b.secondKey);
    __asm__ volatile(".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\tvsubx.w vf3, vf0, vf2x\n\tvmulax.xyzw ACC, vf11, vf2x\n\tvmaddw.xyzw vf10, vf10, vf3w\n\tvmove.w vf10, vf0\n\t.set reorder" : : "f"(b.weight));
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"((u8 *)a0->sub + 0x60));
}

/* vu0 routine: blend the two vec3 keys by the segment weight, then blend that with sub->vec by t2 into sub+0x60 */
void func_002DBBB0(HasSub *a0, f32 t1, f32 t2) {
    KeyOut b;

    func_002DB7C8(a0, &b, t1);
    EE_MMI_LOAD_VEC3(vf10, b.firstKey);
    EE_MMI_LOAD_VEC3(vf11, b.secondKey);
    __asm__ volatile(".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\tvsubx.w vf3, vf0, vf2x\n\tvmulax.xyzw ACC, vf11, vf2x\n\tvmaddw.xyzw vf10, vf10, vf3w\n\tvmove.xyzw vf11, vf10\n\t.set reorder" : : "f"(b.weight));
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"((u8 *)a0 + 0x10));
    __asm__ volatile(".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\tvsubx.w vf3, vf0, vf2x\n\tvmulax.xyzw ACC, vf11, vf2x\n\tvmaddw.xyzw vf10, vf10, vf3w\n\tvmove.w vf10, vf0\n\t.set reorder" : : "f"(t2));
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"((u8 *)a0->sub + 0x60));
}

void *func_002DBC60(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x20);
    func_002DBA80(r, a0, D_003981E8, a2);
    return r;
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DBCC0);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DBD80);

void *func_002DBE60(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x20);
    func_002DBA80(r, a0, D_00398200, a2);
    return r;
}

/* vu0 routine: blend the two vec3 keys by the segment weight, store to sub+0x70 */
void func_002DBEC0(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    EE_MMI_LOAD_VEC3(vf10, b.firstKey);
    EE_MMI_LOAD_VEC3(vf11, b.secondKey);
    __asm__ volatile(".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\tvsubx.w vf3, vf0, vf2x\n\tvmulax.xyzw ACC, vf11, vf2x\n\tvmaddw.xyzw vf10, vf10, vf3w\n\tvmove.w vf10, vf0\n\t.set reorder" : : "f"(b.weight));
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"((u8 *)a0->sub + 0x70));
}

/* vu0 routine: as func_002DBBB0, stored to sub+0x70 */
void func_002DBF40(HasSub *a0, f32 t1, f32 t2) {
    KeyOut b;

    func_002DB7C8(a0, &b, t1);
    EE_MMI_LOAD_VEC3(vf10, b.firstKey);
    EE_MMI_LOAD_VEC3(vf11, b.secondKey);
    __asm__ volatile(".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\tvsubx.w vf3, vf0, vf2x\n\tvmulax.xyzw ACC, vf11, vf2x\n\tvmaddw.xyzw vf10, vf10, vf3w\n\tvmove.xyzw vf11, vf10\n\t.set reorder" : : "f"(b.weight));
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"((u8 *)a0 + 0x10));
    __asm__ volatile(".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\tvsubx.w vf3, vf0, vf2x\n\tvmulax.xyzw ACC, vf11, vf2x\n\tvmaddw.xyzw vf10, vf10, vf3w\n\tvmove.w vf10, vf0\n\t.set reorder" : : "f"(t2));
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"((u8 *)a0->sub + 0x70));
}

void *func_002DBFF0(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x20);
    func_002DBA80(r, a0, D_00398218, a2);
    return r;
}

/* vu0 routine: nlerp the two quaternion keys by the segment weight, store quaternion and matrix rows */
void func_002DC050(HasSub *a0, f32 t) {
    KeyOut b;
    u8 *sub;
    u8 *matrix;
    f32 *key;

    func_002DB7C8(a0, &b, t);
    key = b.firstKey;
    EE_MMI_LOAD_S16X4_FIXED12(vf10, key);
    key = b.secondKey;
    EE_MMI_LOAD_S16X4_FIXED12(vf11, key);
    effMiscQuaternionNlerpVU(b.weight);
    sub = a0->sub;
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(sub + 0x50));
    matrix = sub + 0x80;
    effMiscQuaternionToMatrixVU();
    __asm__ volatile(".set noreorder\n\tsqc2 vf28, 0(%0)\n\t.set reorder" : : "r"(matrix));
    __asm__ volatile(".set noreorder\n\tsqc2 vf29, 0(%0)\n\t.set reorder" : : "r"(matrix + 0x10));
    __asm__ volatile(".set noreorder\n\tsqc2 vf30, 0(%0)\n\t.set reorder" : : "r"(matrix + 0x20));
}

/* vu0 routine: nlerp the two quaternion keys by the segment weight, nlerp that toward the quaternion at +0x10 by t2, store quaternion and matrix rows */
void func_002DC0E8(HasSub *a0, f32 t1, f32 t2) {
    KeyOut b;
    u8 *sub;
    u8 *matrix;
    f32 *key;

    func_002DB7C8(a0, &b, t1);
    key = b.firstKey;
    EE_MMI_LOAD_S16X4_FIXED12(vf10, key);
    key = b.secondKey;
    EE_MMI_LOAD_S16X4_FIXED12(vf11, key);
    effMiscQuaternionNlerpVU(b.weight);
    sub = a0->sub;
    VU0_MOVE_VF(vf11, vf10);
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"((u8 *)a0 + 0x10));
    effMiscQuaternionNlerpVU(t2);
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(sub + 0x50));
    matrix = sub + 0x80;
    effMiscQuaternionToMatrixVU();
    __asm__ volatile(".set noreorder\n\tsqc2 vf28, 0(%0)\n\t.set reorder" : : "r"(matrix));
    __asm__ volatile(".set noreorder\n\tsqc2 vf29, 0(%0)\n\t.set reorder" : : "r"(matrix + 0x10));
    __asm__ volatile(".set noreorder\n\tsqc2 vf30, 0(%0)\n\t.set reorder" : : "r"(matrix + 0x20));
}

void *func_002DC1A0(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x14);
    func_002DBA80(r, a0, D_00398230, a2);
    return r;
}

void func_002DC200(HasSubU *a0, f32 t) {
    KeyOut b;
    SubU *s;

    func_002DB7C8(a0, &b, t);
    s = a0->sub;
    if (*(u8 *)b.firstKey == 0) {
        s->u14 = s->u14 | 0x10;
    } else {
        s->u14 = s->u14 & 0xFFEF;
    }
}

void func_002DC258(HasSubU *a0, f32 t) {
    KeyOut b;
    SubU *s;

    func_002DB7C8(a0, &b, t);
    s = a0->sub;
    if (*(u8 *)b.firstKey == 0) {
        s->u14 = s->u14 | 0x10;
    } else {
        s->u14 = s->u14 & 0xFFEF;
    }
}

void func_002DC2B0(CmdB *a0) {
    a0->res = (s8)(((a0->sub->u14 >> 4) ^ 1) & 1);
}

void func_002DC2D0(void *work) {
    PCP_COPY_VECTOR((u8 *)work + 0x10, *(u8 **)((u8 *)work + 0xC) + 0x60);
}

void func_002DC2F0(void *work) {
    PCP_COPY_VECTOR((u8 *)work + 0x10, *(u8 **)((u8 *)work + 0xC) + 0x50);
}

void func_002DC310(void *work) {
    PCP_COPY_VECTOR((u8 *)work + 0x10, *(u8 **)((u8 *)work + 0xC) + 0x70);
}

s32 sdfDispatchMotionHandler(void *a0, s32 a1) {
    return D_00398248[(u16)a1](a0, a1);
}

void func_002DC360(Dst360 *a0, Src360 *a1, void *a2, s32 a3) {
    sdfSetMotionPointerPair(&a0->pair, a1, a2);
    a0->unkC = a1->unk4->unkC->arr[a3];
}

void *func_002DC3B8(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x14);
    func_002DC360(r, a0, D_00398270, a2);
    return r;
}

void func_002DC418(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    func_002DA3D8(a0->sub, func_002DB958(&b));
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DC458);

void sdfCopyTrackStateToBinding(CmdI *a0) {
    a0->res = a0->sub->i14;
}

void *func_002DC518(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x14);
    func_002DC360(r, a0, D_00398288, a2);
    return r;
}

void func_002DC578(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    func_002DA3C0(a0->sub, func_002DB958(&b));
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DC5B8);

void func_002DC668(CmdI *a0) {
    a0->res = a0->sub->i10;
}

void *func_002DC678(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x14);
    func_002DC360(r, a0, D_003982A0, a2);
    return r;
}

void func_002DC6D8(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    func_002DA3F0(a0->sub, func_002DB958(&b));
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DC718);

void func_002DC7C8(CmdI *a0) {
    a0->res = a0->sub->i20;
}

void *func_002DC7D8(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x14);
    func_002DC360(r, a0, D_003982B8, a2);
    return r;
}

void func_002DC838(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    func_002DA408(a0->sub, func_002DB958(&b));
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DC878);

void func_002DC928(CmdI *a0) {
    a0->res = a0->sub->i28;
}

void *func_002DC938(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x14);
    func_002DC360(r, a0, D_003982D0, a2);
    return r;
}

void func_002DC998(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    func_002DA420(a0->sub, sdfInterpolateMotionKeys(&b));
}

void func_002DC9D8(HasSubF *a0, f32 t1, f32 t2) {
    KeyOut b;

    func_002DB7C8(a0, &b, t1);
    func_002DA420(a0->sub, (a0->f10 + sdfInterpolateMotionKeys(&b) * t2) - (a0->f10 * t2));
}

void func_002DCA30(CmdF *a0) {
    a0->res = a0->sub->f1C;
}

void *func_002DCA40(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x24);
    func_002DC360(r, a0, D_003982E8, a2);
    return r;
}

void func_002DCAA0(HasSub *a0, f32 t) {
    KeyOut b0;
    f32 b1[5];

    func_002DB7C8(a0, &b0, t);
    func_002DBA28(&b0, b1);
    func_002DA548(a0->sub, b1);
}

void func_002DCAF0(HasArr *a0, f32 t1, f32 t2) {
    KeyOut b0;
    f32 b1[5];
    f32 b2[5];

    func_002DB7C8(a0, &b0, t1);
    func_002DBA28(&b0, b2);
    sdfMotionBlendFiveFloats(b1, a0->f10, b2, t2);
    func_002DA548(a0->sub, b1);
}

void func_002DCB68(DstBlk *a0) {
    Blk *p;

    p = sdfEnsurePrimaryTextSubParam(a0->sub);
    a0->blk = *p;
}

void *func_002DCBB8(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x24);
    func_002DC360(r, a0, D_00398300, a2);
    return r;
}

void func_002DCC18(HasSub *a0, f32 t) {
    KeyOut b0;
    f32 b1[5];

    func_002DB7C8(a0, &b0, t);
    func_002DBA28(&b0, b1);
    func_002DA6B0(a0->sub, b1);
}

void func_002DCC68(HasArr *a0, f32 t1, f32 t2) {
    KeyOut b0;
    f32 b1[5];
    f32 b2[5];

    func_002DB7C8(a0, &b0, t1);
    func_002DBA28(&b0, b1);
    sdfMotionBlendFiveFloats(b2, a0->f10, b1, t2);
    func_002DA6B0(a0->sub, b2);
}

void func_002DCCE0(DstBlk *a0) {
    Blk *p;

    p = sdfEnsureSecondaryTextSubParam(a0->sub);
    a0->blk = *p;
}

void *func_002DCD30(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x14);
    func_002DC360(r, a0, D_00398318, a2);
    return r;
}

void func_002DCD90(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    func_002DA5B0(a0->sub, func_002DB958(&b));
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DCDD0);

void sdfCopyMotionTargetValue(CmdI *a0) {
    a0->res = a0->sub->i18;
}

void *func_002DCE90(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x24);
    func_002DC360(r, a0, D_00398330, a2);
    return r;
}

void func_002DCEF0(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    func_002DA548(a0->sub, b.firstKey);
}

void func_002DCF28(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    func_002DA548(a0->sub, b.firstKey);
}

void func_002DCF60(void) {
}

void *func_002DCF68(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x24);
    func_002DC360(r, a0, D_00398348, a2);
    return r;
}

void func_002DCFC8(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    func_002DA6B0(a0->sub, b.firstKey);
}

void func_002DD000(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    func_002DA6B0(a0->sub, b.firstKey);
}
