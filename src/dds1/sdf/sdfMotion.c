#include "common.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"

typedef struct {
    void *dispatch; /* 0x00: binding callback table */
    void *source;   /* 0x04: source supplied during binding */
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
    f32 capturedValue; /* 0x10: target value saved before blending */
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
    f32 capturedValue; /* 0x10: target value saved before blending */
} HasSubF;

typedef struct {
    s32 u0;
    s32 u4;
    s32 u8;
    void *sub;
    f32 capturedValues[5]; /* 0x10: target scalars saved before blending */
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
    Blk capturedParams; /* 0x10: saved primary/secondary text scalars */
} DstBlk;
typedef struct {
    s32 u0;
    s16 objectCount; /* 0x04: number of allocated binding objects */
    s16 pad6;
    s32 u8;
    void **objects; /* 0x0C: binding objects, each beginning with a callback table */
} ArrObj;

/* Native motion constructor/tick layout. A blend lead starts the frame clock
 * below zero; blend callbacks normalize nonnegative elapsed frames by duration. */
typedef struct {
    void *next;        /* 0x00: owner's intrusive motion list */
    void *owner;       /* 0x04: owner whose list head is at +0x14 */
    void *motionTable; /* 0x08: motion entries and binding-command data */
    s32 unkC;
    ArrObj *request;        /* 0x10 */
    f32 blendDurationFrames; /* 0x14 */
    f32 blendStartFrame;    /* 0x18: negative lead when a blend is requested */
    f32 currentFrame;       /* 0x1C: advanced by frameStep on each tick */
    f32 frameStep;          /* 0x20: initialized to 1.0 */
    s32 unk24;
    s32 unk28;        /* 0x28: model code treats this as s16 searchId/slotIndex */
    s16 motionIndex;  /* 0x2C: selects an entry in motionTable */
    u16 frameCount;   /* 0x2E: selected entry's duration */
    u8 state;
    u8 previousState;
    u8 loopEnabled;   /* 0x32: wraps the frame clock instead of finishing */
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

void *sdfAllocAndClearQuadwords(s32 size);
void sdfReleaseChipBlock(void *a0);
void *func_002CFEB8(s32 size);
ArrObj *sdfDevCreateBufferedRequest(u16 n, s32 e1, s32 e2);
s32 sdfDispatchAssetCommandWord(void *a0, s32 a1, s32 a2);
s32 sdfModelFindDrawNode(void *a0, s32 a1);
void func_002DA3C0(void *a0, s32 a1);
void func_002DA3D8(void *a0, s32 a1);
void func_002DA3F0(void *a0, s32 a1);
void func_002DA408(void *a0, s32 a1);
void func_002DA420(void *a0, f32 a1);
Blk *sdfEnsurePrimaryTextSubParam(void *a0);
void sdfCopyPrimaryTextScalars(void *a0, void *a1);
void func_002DA5B0(void *a0, s32 a1);
Blk *sdfEnsureSecondaryTextSubParam(void *a0);
void sdfCopySecondaryTextScalars(void *a0, void *a1);
void sdfDestroyDevRequest(void *a0);
void sdfSetMotionPointerPair(Pair *binding, void *source, void *dispatch);
void func_002DB3D0(Motion *a0, s32 a1, s32 a2, f32 t0, f32 t1);
f32 sdfInterpolateMotionKeys(KeyOut *a0);
void func_002DB7C8(void *a0, KeyOut *out, f32 t);
s32 func_002DB958(KeyOut *a0);
void sdfMotionBindDrawNode(void *tmp, void *src, void *tbl, s32 x);
void sdfMotionBindIndexedTrack(Dst360 *a0, Src360 *a1, void *a2, s32 a3);
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

/* Install the binding's dispatch table and source without touching its payload. */
void sdfSetMotionPointerPair(Pair *binding, void *source, void *dispatch) {
    binding->dispatch = dispatch;
    binding->source = source;
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DB230);

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
    s32 motionTable; /* 0x08: motion-table address */
    s32 uC;
    ArrObj *request;
} MotionNode;

/* Unlinks the node from its owner's list, notifies each request callback, then frees the request and the node. */
void sdfDestroyMotion(MotionNode *node)
{
    Link *prev;
    Link *cur;
    ArrObj *request;
    void **objects;
    s32 objectCount;
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
    objectCount = request->objectCount;
    objects = request->objects;
    for (i = 0; i < objectCount; i++) {
        sdfInvokeMotionObjectCallback(objects[i]);
    }
    sdfDestroyDevRequest(node->request);
    sdfReleaseChipBlock(node);
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DB3D0);
/* Select a motion with no lead-in and no blend duration. */
void sdfMotionInitializeAtZeroTime(void *a0, s32 a1, s32 a2) {
    func_002DB3D0(a0, a1, a2, 0.0f, 0.0f);
}


INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DB538);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DB660);
/* State 6 parks motion processing, retaining the previous state to resume. */
void sdfMotionSuspend(Motion *motion) {
    u8 previousState;

    previousState = motion->state;
    if (previousState != 6) {
        motion->previousState = previousState;
        motion->state = 6;
    }
}

/* Resume only a suspended motion, restoring the state saved by suspend. */
void sdfMotionResume(Motion *motion) {
    if (motion->state == 6) {
        motion->state = motion->previousState;
    }
}

void func_002DB7A8(void *a0) {
    sdfReleaseChipBlock(a0);
}

void sdfSetMotionOutputValue(Triple *a0, s32 a1) {
    a0->unk8 = a1;
}
INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DB7C8);

/* Blend two sampled scalar keys; preserve this expression order for matching. */
f32 sdfInterpolateMotionKeys(KeyOut *a0) {
    f32 t;
    f32 a;

    t = a0->weight;
    a = *a0->firstKey;
    return (a + (*a0->secondKey * t)) - (a * t);
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DB900);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DB958);

/* Interpolate exactly five scalar channels, retaining the retail arithmetic order. */
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

void sdfMotionBlendFiveKeyValues(KeyOut *src, f32 *dst) {
    sdfMotionBlendFiveFloats(dst, src->firstKey, src->secondKey, src->weight);
}

s32 sdfDispatchMotionBySelector(void *a0, s32 a1) {
    return D_003981B8[(u16)a1](a0, a1);
}

void sdfMotionBindDrawNode(void *tmp, void *src, void *tbl, s32 x) {
    sdfSetMotionPointerPair(tmp, src, tbl);
    ((TmpBuf *)tmp)->unkC = sdfModelFindDrawNode(((HasPtr4 *)src)->unk4, x);
}

void *sdfMotionCreateDrawVectorBinding(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x20);
    sdfMotionBindDrawNode(r, a0, D_003981D0, a2);
    return r;
}

/* vu0 routine: blend the two vec3 keys by the segment weight, store to sub+0x60 */
void sdfMotionBlendDrawVector(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    EE_MMI_LOAD_VEC3(vf10, b.firstKey);
    EE_MMI_LOAD_VEC3(vf11, b.secondKey);
        VU0_LERP_VF10_W(b.weight);
        VU0_STORE_VF_UNCLOBBERED(vf10, (u8 *)a0->sub + 0x60);
}

/* vu0 routine: blend the two vec3 keys by the segment weight, then blend that with sub->vec by t2 into sub+0x60 */
void sdfMotionBlendDrawVectorWithCurrent(HasSub *a0, f32 t1, f32 t2) {
    KeyOut b;

    func_002DB7C8(a0, &b, t1);
    EE_MMI_LOAD_VEC3(vf10, b.firstKey);
    EE_MMI_LOAD_VEC3(vf11, b.secondKey);
        VU0_LERP_VF10_COPY(b.weight);
        VU0_LOAD_VF(vf10, (u8 *)a0 + 0x10);
        VU0_LERP_VF10_W(t2);
        VU0_STORE_VF_UNCLOBBERED(vf10, (u8 *)a0->sub + 0x60);
}

void *func_002DBC60(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x20);
    sdfMotionBindDrawNode(r, a0, D_003981E8, a2);
    return r;
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DBCC0);

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DBD80);

void *sdfMotionCreateScaleVectorBinding(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x20);
    sdfMotionBindDrawNode(r, a0, D_00398200, a2);
    return r;
}

/* vu0 routine: blend the two vec3 keys by the segment weight, store to sub+0x70 */
void sdfMotionBlendScaleVector(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    EE_MMI_LOAD_VEC3(vf10, b.firstKey);
    EE_MMI_LOAD_VEC3(vf11, b.secondKey);
        VU0_LERP_VF10_W(b.weight);
    VU0_STORE_VF(vf10, (u8 *)a0->sub + 0x70);
}

/* vu0 routine: as sdfMotionBlendDrawVectorWithCurrent, stored to sub+0x70 */
void sdfMotionBlendScaleVectorWithCurrent(HasSub *a0, f32 t1, f32 t2) {
    KeyOut b;

    func_002DB7C8(a0, &b, t1);
    EE_MMI_LOAD_VEC3(vf10, b.firstKey);
    EE_MMI_LOAD_VEC3(vf11, b.secondKey);
        VU0_LERP_VF10_COPY(b.weight);
        VU0_LOAD_VF(vf10, (u8 *)a0 + 0x10);
        VU0_LERP_VF10_W(t2);
    VU0_STORE_VF(vf10, (u8 *)a0->sub + 0x70);
}

void *sdfMotionCreateQuaternionBinding(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x20);
    sdfMotionBindDrawNode(r, a0, D_00398218, a2);
    return r;
}

/* vu0 routine: nlerp the two quaternion keys by the segment weight, store quaternion and matrix rows */
void sdfMotionBlendQuaternionToMatrix(HasSub *a0, f32 t) {
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
        VU0_STORE_VF_UNCLOBBERED(vf10, sub + 0x50);
    matrix = sub + 0x80;
    effMiscQuaternionToMatrixVU();
        VU0_STORE_VF_UNCLOBBERED(vf28, matrix);
        VU0_STORE_VF_UNCLOBBERED(vf29, matrix + 0x10);
        VU0_STORE_VF_UNCLOBBERED(vf30, matrix + 0x20);
}

/* vu0 routine: nlerp the two quaternion keys by the segment weight, nlerp that toward the quaternion at +0x10 by t2, store quaternion and matrix rows */
void sdfMotionBlendKeyQuaternionWithBase(HasSub *a0, f32 t1, f32 t2) {
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
        VU0_LOAD_VF(vf10, (u8 *)a0 + 0x10);
    effMiscQuaternionNlerpVU(t2);
        VU0_STORE_VF_UNCLOBBERED(vf10, sub + 0x50);
    matrix = sub + 0x80;
    effMiscQuaternionToMatrixVU();
        VU0_STORE_VF_UNCLOBBERED(vf28, matrix);
        VU0_STORE_VF_UNCLOBBERED(vf29, matrix + 0x10);
        VU0_STORE_VF_UNCLOBBERED(vf30, matrix + 0x20);
}

void *sdfMotionCreateKeyFlagBinding(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x14);
    sdfMotionBindDrawNode(r, a0, D_00398230, a2);
    return r;
}

void sdfMotionUpdateKeyFlag(HasSubU *a0, f32 t) {
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

void sdfMotionReadKeyFlag(CmdB *a0) {
    a0->res = (s8)(((a0->sub->u14 >> 4) ^ 1) & 1);
}

void sdfMotionCaptureDrawVector(void *work) {
    PCP_COPY_VECTOR((u8 *)work + 0x10, *(u8 **)((u8 *)work + 0xC) + 0x60);
}

void sdfMotionCaptureQuaternion(void *work) {
    PCP_COPY_VECTOR((u8 *)work + 0x10, *(u8 **)((u8 *)work + 0xC) + 0x50);
}

void sdfMotionCaptureScaleVector(void *work) {
    PCP_COPY_VECTOR((u8 *)work + 0x10, *(u8 **)((u8 *)work + 0xC) + 0x70);
}

s32 sdfDispatchMotionHandler(void *a0, s32 a1) {
    return D_00398248[(u16)a1](a0, a1);
}

void sdfMotionBindIndexedTrack(Dst360 *a0, Src360 *a1, void *a2, s32 a3) {
    sdfSetMotionPointerPair(&a0->pair, a1, a2);
    a0->unkC = a1->unk4->unkC->arr[a3];
}

void *func_002DC3B8(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x14);
    sdfMotionBindIndexedTrack(r, a0, D_00398270, a2);
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
    sdfMotionBindIndexedTrack(r, a0, D_00398288, a2);
    return r;
}

void func_002DC578(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    func_002DA3C0(a0->sub, func_002DB958(&b));
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DC5B8);

void sdfMotionReadBoundTrackInteger(CmdI *a0) {
    a0->res = a0->sub->i10;
}

void *func_002DC678(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x14);
    sdfMotionBindIndexedTrack(r, a0, D_003982A0, a2);
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
    sdfMotionBindIndexedTrack(r, a0, D_003982B8, a2);
    return r;
}

void func_002DC838(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    func_002DA408(a0->sub, func_002DB958(&b));
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DC878);

void sdfMotionCopyTrackValueToBinding(CmdI *a0) {
    a0->res = a0->sub->i28;
}

void *sdfMotionCreateFloatBinding(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x14);
    sdfMotionBindIndexedTrack(r, a0, D_003982D0, a2);
    return r;
}

void sdfMotionApplyInterpolatedFloat(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    func_002DA420(a0->sub, sdfInterpolateMotionKeys(&b));
}

void sdfMotionBlendInterpolatedFloat(HasSubF *a0, f32 t1, f32 t2) {
    KeyOut b;

    func_002DB7C8(a0, &b, t1);
    func_002DA420(a0->sub, (a0->capturedValue + sdfInterpolateMotionKeys(&b) * t2) - (a0->capturedValue * t2));
}

/* Capture the bound float as the base value for a later blend. */
void sdfMotionReadBoundFloat(CmdF *a0) {
    a0->capturedValue = a0->sub->f1C;
}

void *sdfMotionCreateTextBlendBinding(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x24);
    sdfMotionBindIndexedTrack(r, a0, D_003982E8, a2);
    return r;
}

void sdfMotionApplyFiveFloatKeys(HasSub *a0, f32 t) {
    KeyOut b0;
    f32 b1[5];

    func_002DB7C8(a0, &b0, t);
    sdfMotionBlendFiveKeyValues(&b0, b1);
    sdfCopyPrimaryTextScalars(a0->sub, b1);
}

void sdfMotionBlendFiveFloatKeys(HasArr *a0, f32 t1, f32 t2) {
    KeyOut b0;
    f32 b1[5];
    f32 b2[5];

    func_002DB7C8(a0, &b0, t1);
    sdfMotionBlendFiveKeyValues(&b0, b2);
    sdfMotionBlendFiveFloats(b1, a0->capturedValues, b2, t2);
    sdfCopyPrimaryTextScalars(a0->sub, b1);
}

/* Save the current primary text scalars for the blend callback. */
void sdfMotionCapturePrimaryTextParams(DstBlk *a0) {
    Blk *p;

    p = sdfEnsurePrimaryTextSubParam(a0->sub);
    a0->capturedParams = *p;
}

void *sdfMotionCreateSecondaryTextBinding(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x24);
    sdfMotionBindIndexedTrack(r, a0, D_00398300, a2);
    return r;
}

void sdfMotionApplySecondaryTextKeys(HasSub *a0, f32 t) {
    KeyOut b0;
    f32 b1[5];

    func_002DB7C8(a0, &b0, t);
    sdfMotionBlendFiveKeyValues(&b0, b1);
    sdfCopySecondaryTextScalars(a0->sub, b1);
}

void sdfMotionBlendSecondaryTextKeys(HasArr *a0, f32 t1, f32 t2) {
    KeyOut b0;
    f32 b1[5];
    f32 b2[5];

    func_002DB7C8(a0, &b0, t1);
    sdfMotionBlendFiveKeyValues(&b0, b1);
    sdfMotionBlendFiveFloats(b2, a0->capturedValues, b1, t2);
    sdfCopySecondaryTextScalars(a0->sub, b2);
}

/* Save the current secondary text scalars for the blend callback. */
void sdfMotionCaptureSecondaryTextParams(DstBlk *a0) {
    Blk *p;

    p = sdfEnsureSecondaryTextSubParam(a0->sub);
    a0->capturedParams = *p;
}

void *func_002DCD30(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x14);
    sdfMotionBindIndexedTrack(r, a0, D_00398318, a2);
    return r;
}

void sdfMotionApplyInterpolatedKey(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    func_002DA5B0(a0->sub, func_002DB958(&b));
}

INCLUDE_ASM(const s32, "sdf/sdfMotion", func_002DCDD0);

void sdfCopyMotionTargetValue(CmdI *a0) {
    a0->res = a0->sub->i18;
}

void *sdfMotionCreateDirectTextKeyBinding(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x24);
    sdfMotionBindIndexedTrack(r, a0, D_00398330, a2);
    return r;
}

void sdfMotionApplySelectedTextKey(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    sdfCopyPrimaryTextScalars(a0->sub, b.firstKey);
}

void sdfMotionApplySampleToTarget(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    sdfCopyPrimaryTextScalars(a0->sub, b.firstKey);
}

void func_002DCF60(void) {
}

void *func_002DCF68(void *a0, s32 a1, s32 a2) {
    void *r;

    r = func_002CFEB8(0x24);
    sdfMotionBindIndexedTrack(r, a0, D_00398348, a2);
    return r;
}

void sdfMotionApplySampledSecondaryTextValue(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    sdfCopySecondaryTextScalars(a0->sub, b.firstKey);
}

void sdfMotionSampleTextScalarsAtTime(HasSub *a0, f32 t) {
    KeyOut b;

    func_002DB7C8(a0, &b, t);
    sdfCopySecondaryTextScalars(a0->sub, b.firstKey);
}
