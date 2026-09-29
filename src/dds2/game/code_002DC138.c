#include "common.h"
#include "pcp_vu0.h"

typedef struct EffOp28 {
    void (*run)();
    u32 unk[6];
} EffOp28;

typedef struct EffOp24 {
    void (*run)();
    u32 unk[5];
} EffOp24;
extern void func_003343E8(s32, f32);
extern u32 D_00437E98[2];
extern u32 D_00437EA0[2];
extern u8 D_00437EA8[2];
extern u8 D_00437EB0[2];
extern u32 D_00400018[];
extern u32 D_0045C2F0[];
extern u8 D_004386E0[];
extern u8 D_004386E8[];

extern u32 func_00304938(u32);

extern u32 sdfResourceRetainAddress();

extern u64 fileGetResourceHandle(void);

extern u32 func_00305148();

extern s32 func_002DC1D0(u32, u32);

extern u32 func_00331940(u32, u32);

extern u32 func_00328D68(u32);

extern s32 D_00437E74;

extern u32 D_00437E78;

extern u32 D_00437E50;

extern u32 D_00437E08;

extern void *fileResolvePrimaryBuffer();

extern s64 func_001AA308(void);

extern s64 btlIsCurrentActorFullyMarked(void);

extern u64 func_002DC2D8();

typedef struct EffectObjectFlag {
    u8 pad00[0xC];
    u32 flags;
} EffectObjectFlag;

typedef struct EffectObjectNode {
    EffectObjectFlag *object;
    u32 unk04;
    struct EffectObjectNode *next;
} EffectObjectNode;

extern EffectObjectNode *D_00437E30;

extern u32 D_00437E38;

extern u32 D_00437E3C;

extern u32 func_00159A50(u32);

extern u32 func_002DDF48(u32);

extern u32 D_00437E70;

extern u32 D_00437E80;

extern s32 D_00437E84;

extern u32 D_00437E88;

extern u32 func_002DDCA0(u32, u32);

extern s32 *func_002F69F0(u32, u32, u32);

extern s32 func_001AA6F8(void);

extern s32 D_004386C4;

extern s32 D_004386B4;

extern u8 D_00439075;

extern s32 D_004386F0;

extern s32 D_004386F4;

extern s32 D_0043875C;

extern s32 D_00438760;

extern s32 D_00438768;

extern u32 D_0043876C;

extern u32 D_00438770;

extern u32 D_00438778;

extern u32 D_004387A8;

extern u32 D_004387AC;

extern u32 D_004387B0;

extern u32 D_00438774;

extern void mdlStoreTertiaryVectorVU(void *);

extern void func_002DCAE8(EffectObjectNode *);

extern void *func_002DDAA8(void *);

/* Reference-counted object header (layout inferred from field accesses). */
typedef struct RefObj {
    u8 pad_0x00[0x14]; // 0x00
    s32 refCount;      // 0x14 incremented with the global reference count
    s32 unk18;         // 0x18
    s32 cnt1C;         // 0x1C
} RefObj; // 0x20

typedef struct EffModelOwner {
    u8 pad00[4];
    s32 model;
    void *ownedBuffer;
} EffModelOwner;

extern u32 D_00437E34;

/* 4x4 float matrix with 128-bit row access for VU0/DMA transfers. */
typedef struct Matrix4 {
    union {
        float m[4][4];
        s128 rows[4];
    } u; // 0x00
} Matrix4; // 0x40

/* Function-pointer tables indexed by object fields (entry size inferred). */
typedef struct FnTbl28 {
    void (*fn)();
    u8 pad_0x04[0x18]; // 0x04
} FnTbl28; // 0x1C

extern FnTbl28 D_003E9964[];

typedef struct FnTbl24 {
    void (*fn)();
    u8 pad_0x04[0x14]; // 0x04
} FnTbl24; // 0x18

extern FnTbl24 D_003E9B90[];

extern u8 *D_003E9CA8[];

extern u32 D_00437E68;

extern FnTbl24 D_003E9D10[];

extern FnTbl28 D_003E9DEC[];

extern u32 D_00437E6C;

extern u32 func_00343ED0(const char *, u32 *, s32);

extern FnTbl28 D_003E9E74[];

extern u64 func_002F3D88();

extern u32 D_00437E7C;

extern FnTbl24 D_003E9F18[];

extern u32 D_004386B0;

extern u32 D_003FFA84[];

extern u8 D_0045C110[];

extern u8 D_00438758;

extern u8 D_00438759;

extern u8 D_0043875A;

extern u32 D_004386C0;

extern FnTbl28 D_003E9950[];

extern FnTbl24 D_003E9B80[];

extern FnTbl28 D_003E9DD8[];

extern FnTbl28 D_003E9E60[];

extern FnTbl24 D_003E9F08[];

extern u8 D_0045C1A0[];

extern u8 D_003FFA40[];

extern u8 D_0045C1E0[];

extern u8 *D_004386CC;

extern u8 D_0045C300[];

extern u8 D_00400150[];

extern u8 D_00400250[];

extern FnTbl24 D_003E9D00[];

extern FnTbl28 D_003EA028[];

extern FnTbl28 D_003EA02C[];

extern void *func_00232198(void *arg0, void *arg1);

extern void *func_00232EE8(void *arg);

extern void *func_00232EF8(void *arg);

extern FnTbl28 D_003E9E70[];

extern FnTbl24 D_003E9F14[];

/* VU0 model helpers consume vf10 directly, as in the DDS1 counterpart. */
extern void *func_003292A8(u32);

extern u8 *func_002E75C8(u32);

extern u32 func_002F2950(u8 *);

extern void func_002040A8(u32);

extern u32 func_002E5C50(u32, u32, u32);

extern u32 effRetainResource(u32);

typedef struct EffectStripNode {
    u32 percent;
    u32 color;
    f32 opacity;
    u8 pad_0C[0x20];
    u32 transform;
    u32 resource;
    u32 active;
    u16 count;
} EffectStripNode;

typedef struct EffectNodeHeader {
    u8 *entries;
    u32 unk_04;
    u8 *allocation;
} EffectNodeHeader;

extern u32 func_002F1740(u32, u32, u32);

extern u8 *func_002F15F8(u32, u32);

/* Battle/display work object (layout inferred from field accesses). */
typedef struct BdWork {
    s32 x0;            // 0x00
    s32 x4;            // 0x04
    u8 pad_0x08[0x58]; // 0x08
    void *x60;         // 0x60
    s32 x64;           // 0x64
} BdWork; // 0x68

typedef struct EffectMaterialSlot {
    u32 value;
    u8 unk_04[0x10];
} EffectMaterialSlot;

extern u128 *D_0037F770[];

extern u128 D_00458460[];

extern u128 D_004584A0[];

extern u128 D_0037F780[];

extern u8 D_00437E94;

typedef struct EffectResourceSizeEntry {
    u32 resourceSize;
    u8 pad_04[0x18];
} EffectResourceSizeEntry;

extern EffectResourceSizeEntry D_003E9968[];

extern u8 *func_002E56A8(u16, void *);

extern EffectResourceSizeEntry D_003E9E78[];

extern u8 *effAllocateBlockWithModel(u16, void *);

extern u32 func_002F5358(u16, void *, void *, u32);

void effInitModelVUState(void *model) {
    __asm__ volatile(".set noreorder\n\tvmove.xyzw vf10, vf0\n\t.set reorder");
    mdlStorePrimaryVectorVU(model);
    __asm__ volatile(".set noreorder\n\tvmove.xyzw vf10, vf0\n\t.set reorder");
    func_00232AD0(model);
    __asm__ volatile(".set noreorder\n\tvaddw.xyz vf10, vf0, vf0w\n\tvmulx.w vf10, vf0, vf0x\n\t.set reorder");
    mdlStoreTertiaryVectorVU(model);
    mdlBroadcastMasked(model, 0x80808080);
    if (*(void **)(model + 0x1C) != NULL) {
        mdlAddEntryPlain(model, 0, 0);
        *(float *)(*(u8 **)(model + 0x1C) + 0x20) = 1.0f;
    }
    *(u32 *)model &= ~1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC1D0);

void func_002DC260(s32 arg0) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x80) = 0;
    func_002322E8();
}

void *func_002DC280(void *arg0) {
    void *a;
    void *b;
    void *work;

    a = func_00232EE8(arg0);
    b = func_00232EF8(arg0);
    work = func_00232198(a, b);
    effInitModelVUState(work);
    return work;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC2D8);

void effDestroyModelOwner(EffModelOwner *owner) {
    void *buffer = owner->ownedBuffer;
    if (buffer != NULL) {
        func_00328E48(buffer);
    }
    if (owner->model != 0) {
        func_002DC260(owner->model);
    }
    func_00328E48(owner);
}

INCLUDE_ASM(const s32, "game/code_002DC138", effDuplicateEffectHeader);

INCLUDE_ASM(const s32, "game/code_002DC138", effRecreateModelFromSource);

void func_002DC4A8(EffModelOwner *owner) {
    func_003343E8(*(s32 *)(owner->model + 0x1C), 0.0f);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC4C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC520);

void effApplyModelVecB(EffModelOwner *owner, u8 *vec) {
    __asm__ volatile("lqc2 $vf10, 0(%0)" :: "r"(vec) : "memory");
    func_00232AD0((void *)owner->model);
}

void effBroadcastModelMask(EffModelOwner *owner) {
    mdlBroadcastMasked(owner->model);
}

void func_002DC578(s32 arg0, float scale) {
    float v[3];
    float t;

    t = *(float *)arg0 * scale;
    v[0] = v[1] = v[2] = t;
    __asm__ volatile ("lqc2 $vf10, 0(%0)" :: "r" (v));
    mdlStoreTertiaryVectorVU(*(void **)(arg0 + 4));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC5B0);

u64 func_002DC6E8(void) {
    u64 temp_v0;
    s64 temp_v1;

    temp_v0 = func_002DC2D8();
    func_002DC5B0(temp_v0);
    temp_v1 = func_001AA308();
    if ((temp_v1 != 0) && (temp_v1 = btlIsCurrentActorFullyMarked(), temp_v1 == 0)) {
        func_002DCA88(temp_v0);
    }
    return temp_v0;
}

void func_002DC748(u32 *p) {
    p[3] |= 2;
    if (!(p[3] & 4)) {
        effDestroyModelOwner(p);
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC788);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC808);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DCA30);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DCA88);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DCAE8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DCB58);

void mdlPropagateObjectFlag(void) {
    EffectObjectNode *node = D_00437E30;
    EffectObjectFlag *object;
    s32 flags;

    if (node != NULL) {
        do {
            object = node->object;
            flags = object->flags;
            if ((flags & 2) != 0) {
                object->flags = flags | 8;
            }
            node = node->next;
        } while (node != NULL);
    }
}

void mdlClearListedObjectFlag(void) {
    EffectObjectFlag *object;
    EffectObjectNode *node;

    node = D_00437E30;
    while (node != NULL) {
        object = node->object;
        node = node->next;
        object->flags = object->flags & 0xffffffef;
    }
}

void mdlSetListedObjectFlag(void) {
    EffectObjectFlag *object;
    EffectObjectNode *node;

    node = D_00437E30;
    while (node != NULL) {
        object = node->object;
        node = node->next;
        object->flags = object->flags | 0x10;
    }
}

void mdlMarkAndProcessObjectNodes(void) {
    EffectObjectNode *node = D_00437E30;
    EffectObjectFlag *object;
    EffectObjectNode *next;
    s32 flags;

    if (node == NULL) {
        return;
    }
    do {
        object = node->object;
        next = node->next;
        flags = object->flags | 0xA;
        object->flags = flags;
        func_002DCAE8(node);
        node = next;
    } while (node != NULL);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DCCE8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DD038);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DD0C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DD258);

extern void func_002D46A0(s32);

extern void func_003343E8(s32, f32);

void func_002DD3C0(s32 *work) {
    if (work[0xBC / 4] != 0) {
        u32 count = work[0];
        u32 i = 0;
        s32 *entries = (s32 *)work[0xB8 / 4];
        if (count != 0) {
            do {
                func_002D46A0(*entries);
                entries++;
                i++;
            } while (i < count);
        }
    }
    func_003343E8(*(s32 *)(work[0xC0 / 4] + 0x1C), 0.0f);
    work[1] = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DD448);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DDA10);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DDA30);

void func_002DDA50(s32 arg0) {
    mdlBroadcastMasked(*(u32 *)(arg0 + 0xc0));
}

void effScaleModelVec(u8 *work, float scale) {
    u32 bits;
    __asm__ volatile(
        ".set noreorder\n\t"
        "vaddw.xyz vf10, vf0, vf0w\n\t"
        "vmulx.w vf10, vf0, vf0x\n\t"
        ".set reorder" ::: "memory");
    __asm__ volatile(
        ".set noreorder\n\t"
        "mfc1 %0, %1\n\t"
        "qmtc2.ni %0, $vf2\n\t"
        "vmulx.xyzw vf10, vf10, $vf2x\n\t"
        ".set reorder"
        : "=&r"(bits) : "f"(scale) : "memory");
    mdlStoreTertiaryVectorVU(*(void **)(work + 0xC0));
}

void func_002DDA98(void) {
    D_00437E3C = 0;
    D_00437E38 = 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DDAA8);

u32 func_002DDCA0(u32 source, u32 value) {
    RefObj *copy;

    copy = (RefObj *)func_002DDAA8((void *)source);
    copy->unk18 = value;
    return (u32)copy;
}

INCLUDE_ASM(const s32, "game/code_002DC138", effReleaseSharedReference);

RefObj *effRetainSharedReference(RefObj *obj) {
    obj->refCount++;
    D_00437E34++;
    return obj;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DDD60);

s32 effCountExpandedEntries(void *work) {
    u32 count = *(u32 *)((u8 *)work + 4);
    u32 i = 0;
    s32 total = 0;

    if (count != 0) {
        u8 *entries = *(u8 **)((u8 *)work + 0x10);
        do {
            s32 value = *(s32 *)entries;
            entries += 0x10;
            i++;
            total++;
            total += value;
        } while (i < count);
    }
    return total;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DDF48);

INCLUDE_ASM(const s32, "game/code_002DC138", effReleaseReferenceHolder);

RefObj *func_002DE120(RefObj *obj) {
    obj->cnt1C++;
    return obj;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DE138);

void func_002DE218(s32 arg0, u32 arg1, s32 arg2) {
    func_002DDD60(arg1, *(u32 *)(*(s32 *)(arg2 + 0xc) * 4 + *(s32 *)(arg0 + 0x14)));
}

u32 *func_002DE248(src)
    void *src;
{
    u32 *p = (u32 *)func_00328D68(0xc);
    p[0] = 0;
    memcpy(p + 1, src, 8);
    return p;
}

void func_002DE288(void) {
    u64 temp_v0;

    temp_v0 = fileResolvePrimaryBuffer();
    func_002DE248(temp_v0);
}

void func_002DE2A8(u32 arg0) {
    func_001057A8();
    func_00328E48(arg0);
}

void func_002DE2D0(s32 arg0) {
    func_002DE248(arg0 + 4);
}

void func_002DE2E8(u32 *arg0) {
    *arg0 = 0;
}

void func_002DE2F0(s32 *arg0) {
    s32 temp_v0;

    temp_v0 = *arg0;
    if (temp_v0 == 0) {
        kwlnFadeSetupFrames(arg0[1], arg0[2]);
        temp_v0 = *arg0;
    }
    *arg0 = temp_v0 + 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DE338);

void func_002DE408(void) {
    u64 temp_v0;

    temp_v0 = fileResolvePrimaryBuffer();
    func_002DE338(temp_v0);
}

void func_002DE428(void) {
    func_00328E48();
}

void func_002DE440(s32 arg0) {
    func_002DE338(arg0 + 0x18);
}

void func_002DE458(s32 arg0) {
    *(u32 *)(arg0 + 0x10) = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DE460);

void func_002DEB10(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002DEB20(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

void func_002DEB28(s32 arg0) {
    u32 temp_v0;
    u32 *puVar2;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *(u32 *)(arg0 + 0x4c);
    puVar2 = *(u32 **)(arg0 + 8);
    if (temp_v0 != 0) {
        do {
            temp_v1 = temp_v1 + 1;
            *puVar2 = 0xffffffff;
            puVar2 = puVar2 + 2;
        } while (temp_v1 < temp_v0);
    }
    memset(*(u32 *)(arg0 + 0x10), 0, temp_v0 << 3);
}

void func_002DEB80() {
    func_00316528();
}

void func_002DEB98(void) {
    func_00316648();
}

void func_002DEBB0(void) {
    func_00316668();
}

void func_002DEBC8(s32 p) {
    func_002DEB80(p + 0x14);
}

void func_002DEBE0(s32 *p) {
    func_002DEB28((s32)p);
    *p = 0;
}

void func_002DEC08() {
    if ((D_00437E08 & 2) == 0) {
        func_00316680();
        return;
    }
}

void func_002DEC38() {
    func_00316C88();
}

void func_002DEC50(s32 p) {
    func_002DEC08(p);
    func_002DEC38(p);
}

void func_002DEC78(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 4) = arg1;
}

u32 *func_002DEC80(src)
    void *src;
{
    u32 *p = (u32 *)func_00328D68(0x14);
    p[0] = 0;
    memcpy(p + 1, src, 16);
    return p;
}

void func_002DECD0(void) {
    u64 temp_v0;

    temp_v0 = fileResolvePrimaryBuffer();
    func_002DEC80(temp_v0);
}

void func_002DECF0(u32 arg0) {
    func_0010A298();
    func_00328E48(arg0);
}

void func_002DED18(s32 arg0) {
    func_002DEC80(arg0 + 4);
}

void func_002DED30(u32 *arg0) {
    *arg0 = 0;
}

void func_002DED38(s32 *arg0) {
    s32 temp_v0;

    temp_v0 = *arg0;
    if (temp_v0 == 0) {
        evtSelStateCreate(arg0[1], (s16)arg0[2], arg0[3], arg0[4]);
        temp_v0 = *arg0;
    }
    *arg0 = temp_v0 + 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DED88);

void func_002DEF00(s32 arg0) {
    func_0018E850(arg0 + 0xc0);
}

void func_002DEF18(void) {
    func_0018E8F0();
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DEF30);

void func_002DF138(s32 arg0, u32 arg1) {
    *(u32 *)(*(s32 *)(arg0 + 0x24) + 0x2c) = arg1;
}

void func_002DF148(s32 arg0) {
    func_0018EBC8(arg0 + 0xc0);
}

void func_002DF160(void) {
    func_0018ECB8();
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DF178);

void func_002DF348(s32 arg0, u32 arg1) {
    *(u32 *)(*(s32 *)(arg0 + 0x24) + 0x2c) = arg1;
}

void func_002DF358(s32 arg0) {
    func_0018F098(arg0 + 0xc0);
}

void func_002DF370(void) {
    func_0018F1B8();
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DF388);

void func_002DF558(s32 arg0, u32 arg1) {
    *(u32 *)(*(s32 *)(arg0 + 0x24) + 0x2c) = arg1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DF568);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DF6C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DF978);

void func_002DFA80(s32 arg0) {
    func_0018FBF8(arg0 + 0xc0);
}

void func_002DFA98(void) {
    func_0018FC88();
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DFAB0);

void func_002DFC78(s32 arg0, u32 arg1) {
    *(u32 *)(*(s32 *)(arg0 + 0x24) + 0x24) = arg1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DFC88);

typedef struct DispatchInit20 {
    u8 reserved[12];
    void (*initialize)(s32, s32);
    u8 tail[4];
} DispatchInit20;

extern DispatchInit20 D_003E9810[];

extern s32 func_002DFC88(u16, s32);

extern s32 fileResolveSecondaryBuffer(void *);

extern s32 *func_002E0378(s32 *, u16);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DFDB8);

typedef struct Dispatch20 {
    void (*callback)();
    u8 reserved[16];
} Dispatch20;

extern Dispatch20 D_003E9814[];

extern Dispatch20 D_003E9818[];

extern void func_002E0400(s32 *);

void func_002DFE70(s32 *work) {
    s32 object = work[0x24 / 4];
    if (object != 0) {
        D_003E9814[work[0x1C / 4]].callback(object);
    }
    if (work[0x2C / 4] != 0) {
        func_002E0400((s32 *)work[0x2C / 4]);
    }
    func_00328E48(work);
}

s32 func_002DFED8(s32 *source) {
    s32 *copy = (s32 *)func_002DFC88(*(u16 *)((u8 *)source + 0x1C), source[0x28 / 4]);
    if (source[0x2C / 4] != 0 && D_003E9810[copy[0x1C / 4]].initialize != NULL) {
        s32 *child = (s32 *)func_002E0460(source[0x2C / 4]);
        s32 parameter = child[2];
        copy[0x2C / 4] = (s32)child;
        D_003E9810[copy[0x1C / 4]].initialize((s32)copy, parameter);
    }
    return (s32)copy;
}

void func_002DFF78(s32 arg0) {
    *(u32 *)(arg0 + 0x20) = 0;
}

void func_002DFF80(s32 *work) {
    D_003E9818[work[0x1C / 4]].callback();
    if ((D_00437E08 & 2) == 0) {
        work[0x20 / 4]++;
    }
}

void func_002DFFE0(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002DFFF0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

void func_002DFFF8(Matrix4 *mat, float value) {
    mat->u.m[1][2] = value;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0000);

extern DispatchInit20 D_003E98A0[];

extern s32 func_002E0000(u16, s32);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0130);

extern Dispatch20 D_003E98A4[];

void func_002E01E8(s32 *work) {
    s32 object = work[0x24 / 4];
    if (object != 0) {
        D_003E98A4[work[0x1C / 4]].callback(object);
    }
    if (work[0x2C / 4] != 0) {
        func_002E0400((s32 *)work[0x2C / 4]);
    }
    func_00328E48(work);
}

s32 func_002E0250(s32 *source) {
    s32 *copy = (s32 *)func_002E0000(*(u16 *)((u8 *)source + 0x1C), source[0x28 / 4]);
    if (source[0x2C / 4] != 0 && D_003E98A0[copy[0x1C / 4]].initialize != NULL) {
        s32 *child = (s32 *)func_002E0460(source[0x2C / 4]);
        s32 parameter = child[2];
        copy[0x2C / 4] = (s32)child;
        D_003E98A0[copy[0x1C / 4]].initialize((s32)copy, parameter);
    }
    return (s32)copy;
}

void func_002E02F0(s32 arg0) {
    *(u32 *)(arg0 + 0x20) = 0;
}

extern Dispatch20 D_003E98A8[];

void func_002E02F8(s32 *work) {
    D_003E98A8[work[0x1C / 4]].callback();
    if ((D_00437E08 & 2) == 0) {
        work[0x20 / 4]++;
    }
}

void func_002E0358(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002E0368(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

void func_002E0370(s32 object, f32 value) {
    *(f32 *)(object + 0x18) = value;
}

extern s32 *func_00328E18(s32);

extern s32 func_0032C138(s32 *);

extern s32 func_00159BB8(s32);

s32 *func_002E0378(s32 *source, u16 kind) {
    s32 *object = func_00328E18(0xC);
    object[0] = kind;
    object[1] = 1;
    switch (kind) {
    case 1:
        object[2] = func_0032C138(source);
        break;
    case 4:
        object[2] = func_00159BB8(*source);
        break;
    }
    return object;
}

extern void func_0032BBB0(s32);

void func_002E0400(s32 *object) {
    object[1]--;
    if (object[1] == 0) {
        if (object[0] != 4) {
            func_0032BBB0(object[2]);
        }
        func_00328E48(object);
    }
}

u32 func_002E0460(u32 arg0) {
    *(s32 *)((s32)arg0 + 4) = *(s32 *)((s32)arg0 + 4) + 1;
    return arg0;
}

u8 *effCreateBillboardWork(u8 *source) {
    u8 *work = (u8 *)func_00328D68(0x68);
    memset(work, 0, 0x68);
    *(s32 *)(work + 0x28) = 0;
    *(u32 *)(work + 0x24) = 0x80808080;
    *(float *)(work + 0x20) = 1.0f;
    __asm__ volatile("sqc2 $vf0, 0(%0)" :: "r"(work) : "memory");
    __asm__ volatile("sqc2 $vf0, 0(%0)" :: "r"(work + 0x10) : "memory");
    if (source == NULL) {
        return work;
    }
    memcpy(work + 0x2C, fileResolvePrimaryBuffer(source),
           *(u32 *)(source + 0x14));
    *(s32 *)(work + 0x64) =
        billCreateIndexed(1, fileResolveSecondaryBuffer(source));
    return work;
}

void func_002E0528(u32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)((s32)arg0 + 100);
    if (temp_v0 != 0) {
        billDispatchByKind(temp_v0);
    }
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", effDuplicateBillState);

void func_002E0618(s32 arg0, s32 arg1) {
    u32 temp_v0;

    if (*(s32 *)(arg0 + 100) != 0) {
        billDispatchByKind(*(s32 *)(arg0 + 100));
    }
    temp_v0 = func_00159A50(*(u32 *)(arg1 + 100));
    *(u32 *)(arg0 + 100) = temp_v0;
}

void func_002E0668(s32 arg0) {
    func_00159ED8(*(u32 *)(arg0 + 100), 0);
    *(u32 *)(arg0 + 0x28) = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0698);

void func_002E0790(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002E07A0(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst + 1, src);
}

void func_002E07B8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002E07C0(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

void effClearBillFrames(u8 *owner) {
    u8 *source = *(u8 **)(owner + 0x30);
    u8 *descriptor = *(u8 **)(source + 4);
    u8 *entry = *(u8 **)source;
    s32 count = *(s32 *)(*(u8 **)(owner + 0x34) + 0x38);

    memset(*(void **)(descriptor + 0x1C), 0, *(u32 *)(descriptor + 8) * 0x10);
    if (count > 0) {
        s32 remaining = count;
        do {
            --remaining;
            *(s32 *)entry = -1;
            entry += 0x18;
        } while (remaining);
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0840);

void func_002E08D0(s32 arg0) {
    effReleaseResourceRefs(*(u32 *)(arg0 + 4));
    func_003297C8(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0900);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0F30);

void billResetCellIndices(u8 *owner) {
    u8 *source = *(u8 **)(owner + 0x30);
    u8 *descriptor = *(u8 **)(source + 4);
    u8 *entry = *(u8 **)source;
    s32 count = *(s32 *)(*(u8 **)(owner + 0x34) + 0x38);

    memset(*(void **)(descriptor + 0x1C), 0, *(u32 *)(descriptor + 8) * 0x10);
    if (count > 0) {
        s32 remaining = count;
        do {
            --remaining;
            *(s32 *)entry = -1;
            entry += 0x1C;
        } while (remaining);
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E1110);

void func_002E11A0(s32 arg0) {
    effReleaseResourceRefs(*(u32 *)(arg0 + 4));
    func_003297C8(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E11D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E1908);

void billResetParticleIndices(u8 *owner) {
    u8 *source = *(u8 **)(owner + 0x30);
    u8 *descriptor = *(u8 **)(source + 4);
    u8 *entry = *(u8 **)source;
    s32 count = *(s32 *)(*(u8 **)(owner + 0x34) + 0x38);

    memset(*(void **)(descriptor + 0x1C), 0, *(u32 *)(descriptor + 8) * 0x10);
    if (count > 0) {
        s32 remaining = count;
        do {
            --remaining;
            *(s32 *)entry = -1;
            entry += 0x2C;
        } while (remaining);
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E1AE8);

void func_002E1B80(s32 arg0) {
    effReleaseResourceRefs(*(u32 *)(arg0 + 4));
    func_003297C8(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E1BB0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E22C8);

void effClearAnimatedFrames(u8 *owner) {
    u8 *source = *(u8 **)(owner + 0x30);
    u8 *descriptor = *(u8 **)(source + 4);
    u8 *entry = *(u8 **)source;
    s32 count = *(s32 *)(*(u8 **)(owner + 0x34) + 0x38);

    memset(*(void **)(descriptor + 0x1C), 0, *(u32 *)(descriptor + 8) * 0x10);
    if (count > 0) {
        s32 remaining = count;
        do {
            --remaining;
            *(s32 *)entry = -1;
            entry += 0x18;
        } while (remaining);
    }
}

u8 *func_002E24A8(u8 *config) {
    u32 headerSize = 0x10;
    u8 *base = func_003292A8(*(u32 *)(config + 0x38) * 0x18 + headerSize);
    u8 *node = (u8 *)sdfResourceRetainAddress((u32)base);
    u8 *entries = node + headerSize;

    *(u8 **)(node + 8) = base;
    *(u8 **)node = entries;
    if (*(u32 *)(config + 0x70) == 0) {
        *(u32 *)(config + 0x70) = 1;
    }
    return node;
}

void effInitializeAlternatingTransformRows(u8 *work, u8 *descriptor) {
    u32 count = *(u32 *)(descriptor + 0x38);
    if (count != 0) {
        f32 *row = *(f32 **)(*(u8 **)(work + 4) + 0x20);
        u32 index;

        for (index = 0; index < count; index++, row += 8) {
            if (index & 1) {
                row[0] = 0.0f;
                row[1] = 0.0f;
                row[2] = 0.5f;
                row[3] = 0.0f;
                row[4] = 0.0f;
                row[5] = 1.0f;
                row[6] = 0.5f;
            } else {
                row[0] = 0.5f;
                row[1] = 0.0f;
                row[2] = 1.0f;
                row[3] = 0.0f;
                row[4] = 0.5f;
                row[5] = 1.0f;
                row[6] = 1.0f;
            }
            row[7] = 1.0f;
        }
    }
}

extern u8 *func_002E24A8();
u8 *billCreateAnimatedTransform(u8 *descriptor, s32 handle) {
    u8 *work = func_002E24A8(descriptor);

    *(s32 *)(work + 4) = func_002E5C50(*(u32 *)(descriptor + 0x38), 3, handle);
    effInitializeAlternatingTransformRows(work, descriptor);
    return work;
}

u8 *billCloneAnimatedTransform(u8 *owner) {
    u8 *descriptor = *(u8 **)(owner + 0x34);
    u8 *source = *(u8 **)(owner + 0x30);
    u8 *work = func_002E24A8(descriptor);

    *(s32 *)(work + 4) = func_002E5DE0(*(u32 *)(source + 4));
    effInitializeAlternatingTransformRows(work, descriptor);
    return work;
}

void func_002E2670(s32 arg0) {
    effReleaseResourceRefs(*(u32 *)(arg0 + 4));
    func_003297C8(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E26A0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E2C48);

void billResetEmitterIndices(u8 *owner) {
    u8 *source = *(u8 **)(owner + 0x30);
    u8 *descriptor = *(u8 **)(source + 4);
    u8 *entry = *(u8 **)source;
    s32 count = *(s32 *)(*(u8 **)(owner + 0x34) + 0x38);

    memset(*(void **)(descriptor + 0x1C), 0, *(u32 *)(descriptor + 8) * 0x10);
    if (count > 0) {
        s32 remaining = count;
        do {
            --remaining;
            *(s32 *)entry = -1;
            entry += 0x18;
        } while (remaining);
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E2E28);

void billInitializeEmitterRows(u8 *work, u8 *descriptor) {
    u32 count = *(u32 *)(descriptor + 0x38);
    if (count != 0) {
        f32 *row = *(f32 **)(*(u8 **)(work + 4) + 0x20);
        u32 index;

        for (index = 0; index < count; index++, row += 8) {
            if (index & 1) {
                row[0] = 0.0f;
                row[1] = 0.0f;
                row[2] = 0.5f;
                row[3] = 0.0f;
                row[4] = 0.0f;
                row[5] = 1.0f;
                row[6] = 0.5f;
            } else {
                row[0] = 0.5f;
                row[1] = 0.0f;
                row[2] = 1.0f;
                row[3] = 0.0f;
                row[4] = 0.5f;
                row[5] = 1.0f;
                row[6] = 1.0f;
            }
            row[7] = 1.0f;
        }
    }
}

extern u8 *func_002E2E28();
u8 *billCreateEmitterTransform(u8 *descriptor, s32 handle) {
    u8 *work = func_002E2E28(descriptor);

    *(s32 *)(work + 4) = func_002E5C50(*(u32 *)(descriptor + 0x38), 4, handle);
    billInitializeEmitterRows(work, descriptor);
    return work;
}

u8 *billCloneEmitterTransform(u8 *owner) {
    u8 *descriptor = *(u8 **)(owner + 0x34);
    u8 *source = *(u8 **)(owner + 0x30);
    u8 *work = func_002E2E28(descriptor);

    *(s32 *)(work + 4) = func_002E5DE0(*(u32 *)(source + 4));
    billInitializeEmitterRows(work, descriptor);
    return work;
}

void func_002E2FD8(s32 arg0) {
    effReleaseResourceRefs(*(u32 *)(arg0 + 4));
    func_003297C8(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E3008);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E35F0);

void effClearStripFrames(u8 *owner) {
    u8 *source = *(u8 **)(owner + 0x30);
    u8 *descriptor = *(u8 **)(source + 4);
    u8 *entry = *(u8 **)source;
    s32 count = *(s32 *)(*(u8 **)(owner + 0x34) + 0x38);

    memset(*(void **)(descriptor + 0x1C), 0, *(u32 *)(descriptor + 8) * 0x10);
    if (count > 0) {
        s32 remaining = count;
        do {
            --remaining;
            *(s32 *)entry = -1;
            entry += 0x28;
        } while (remaining);
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E37D0);

void billInitializeStripRows(u8 *work, u8 *descriptor) {
    u32 count = *(u32 *)(descriptor + 0x38);
    if (count != 0) {
        f32 *row = *(f32 **)(*(u8 **)(work + 4) + 0x20);
        u32 index;

        for (index = 0; index < count; index++, row += 8) {
            if (index & 1) {
                row[0] = 0.0f;
                row[1] = 0.0f;
                row[2] = 0.5f;
                row[3] = 0.0f;
                row[4] = 0.0f;
                row[5] = 1.0f;
                row[6] = 0.5f;
            } else {
                row[0] = 0.5f;
                row[1] = 0.0f;
                row[2] = 1.0f;
                row[3] = 0.0f;
                row[4] = 0.5f;
                row[5] = 1.0f;
                row[6] = 1.0f;
            }
            row[7] = 1.0f;
        }
    }
}

extern u8 *func_002E37D0();
u8 *billCreateStripTransform(u8 *descriptor, s32 handle) {
    u8 *work = func_002E37D0(descriptor);

    *(s32 *)(work + 4) = func_002E5C50(*(u32 *)(descriptor + 0x38), 3, handle);
    billInitializeStripRows(work, descriptor);
    return work;
}

u8 *billCloneStripTransform(u8 *owner) {
    u8 *descriptor = *(u8 **)(owner + 0x34);
    u8 *source = *(u8 **)(owner + 0x30);
    u8 *work = func_002E37D0(descriptor);

    *(s32 *)(work + 4) = func_002E5DE0(*(u32 *)(source + 4));
    billInitializeStripRows(work, descriptor);
    return work;
}

void func_002E3980(s32 arg0) {
    effReleaseResourceRefs(*(u32 *)(arg0 + 4));
    func_003297C8(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E39B0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E4070);

void billResetTrailIndices(u8 *owner) {
    u8 *source = *(u8 **)(owner + 0x30);
    u8 *descriptor = *(u8 **)(source + 4);
    u8 *entry = *(u8 **)source;
    s32 count = *(s32 *)(*(u8 **)(owner + 0x34) + 0x38);

    memset(*(void **)(descriptor + 0x1C), 0, *(u32 *)(descriptor + 8) * 0x10);
    if (count > 0) {
        s32 remaining = count;
        do {
            --remaining;
            *(s32 *)entry = -1;
            entry += 0x2C;
        } while (remaining);
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E4250);

void func_002E42F0(s32 arg0) {
    effReleaseResourceRefs(*(u32 *)(arg0 + 4));
    func_003297C8(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E4320);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E4AC8);

void billResetQuadIndices(u8 *owner) {
    u8 *source = *(u8 **)(owner + 0x30);
    u8 *descriptor = *(u8 **)(source + 4);
    u8 *entry = *(u8 **)source;
    s32 count = *(s32 *)(*(u8 **)(owner + 0x34) + 0x38);

    memset(*(void **)(descriptor + 0x1C), 0, *(u32 *)(descriptor + 8) * 0x10);
    if (count > 0) {
        s32 remaining = count;
        do {
            --remaining;
            *(s32 *)entry = -1;
            entry += 0x20;
        } while (remaining);
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E4CA8);

void billInitializeQuadRows(u8 *work, u8 *descriptor) {
    u32 count = *(u32 *)(descriptor + 0x38);
    if (count != 0) {
        f32 *row = *(f32 **)(*(u8 **)(work + 4) + 0x20);
        u32 index;

        for (index = 0; index < count; index++, row += 8) {
            if (index & 1) {
                row[0] = 0.0f;
                row[1] = 0.0f;
                row[2] = 0.5f;
                row[3] = 0.0f;
                row[4] = 0.0f;
                row[5] = 1.0f;
                row[6] = 0.5f;
            } else {
                row[0] = 0.5f;
                row[1] = 0.0f;
                row[2] = 1.0f;
                row[3] = 0.0f;
                row[4] = 0.5f;
                row[5] = 1.0f;
                row[6] = 1.0f;
            }
            row[7] = 1.0f;
        }
    }
}

extern u8 *func_002E4CA8();
u8 *billCreateQuadTransform(u8 *descriptor, s32 handle) {
    u8 *work = func_002E4CA8(descriptor);

    *(s32 *)(work + 4) = func_002E5C50(*(u32 *)(descriptor + 0x38), 4, handle);
    billInitializeQuadRows(work, descriptor);
    return work;
}

u8 *billCloneQuadTransform(u8 *owner) {
    u8 *descriptor = *(u8 **)(owner + 0x34);
    u8 *source = *(u8 **)(owner + 0x30);
    u8 *work = func_002E4CA8(descriptor);

    *(s32 *)(work + 4) = func_002E5DE0(*(u32 *)(source + 4));
    billInitializeQuadRows(work, descriptor);
    return work;
}

void func_002E4E50(s32 arg0) {
    effReleaseResourceRefs(*(u32 *)(arg0 + 4));
    func_003297C8(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E4E80);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E5540);

u8 *func_002E56A8(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = D_003E9968[kind].resourceSize;
    u8 *effect = func_00328D68(size + headerSize);
    *(u8 **)(effect + 0x34) = effect + headerSize;
    *(u32 *)(effect + 0x24) = 0x80808080;
    *(float *)(effect + 0x20) = 1.0f;
    *(u32 *)(effect + 0x2C) = kind;
    *(u32 *)(effect + 0x28) = 0;
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" : : "r"(effect) : "memory");
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" : : "r"(effect + 0x10) : "memory");
    memcpy(*(void **)(effect + 0x34), source, size);
    return effect;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E5760);

u8 *effCreateFileResourceInstance(u8 *work) {
    u32 *secondary = fileResolveSecondaryBuffer(work);
    void *source;
    switch (*(u16 *)(work + 0x1C)) {
    case 1:
        break;
    case 4:
        secondary = NULL;
        break;
    }
    source = fileResolvePrimaryBuffer(work);
    return func_002E5760(*(u16 *)(work + 0xC), source, (u32)secondary);
}

extern EffOp28 D_003E9958[];

void effDispatchDestroyOp(u32 *obj) {

    D_003E9958[obj[0x2C / 4]].run(obj[0x30 / 4]);
    func_00328E48(obj);
}

INCLUDE_ASM(const s32, "game/code_002DC138", effCreateActiveResource);

void func_002E5978(u8 *work) {
    D_003E9950[*(s32 *)(work + 0x2c)].fn();
    *(u32 *)(work + 0x28) = 0;
}

extern FnTbl28 D_003E9960[];

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E59C0);

void func_002E5A20(s32 arg0) {
    D_003E9964[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
}

void func_002E5A58(u32 arg0) {
    func_002E59C0();
    func_002E5A20(arg0);
}

void func_002E5A80(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002E5A90(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst + 1, src);
}

void func_002E5AA8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002E5AB0(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BC10);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E5AB8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E5C50);

INCLUDE_ASM(const s32, "game/code_002DC138", effReleaseResourceRefs);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E5DE0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E5E88);

INCLUDE_ASM(const s32, "game/code_002DC138", effLoadFlashTextures);

u32 func_002E63F0(s32 arg0) {
    return (&D_00437E50)[arg0];
}

void func_002E6408(s32 arg0) {
    *(u32 *)(**(s32 **)(arg0 + 0x30) + 4) = 0;
}

u32 *func_002E6418(u8 *work) {
    u32 *handle = func_00328D68(4);
    u32 kind = *(u32 *)(work + 0x38);
    u8 *ring;
    u32 *entry;
    u32 groups;
    u32 i;
    u32 first;
    u32 second;
    u32 third;

    if (kind < 3) {
        *(u32 *)(work + 0x38) = 3;
        kind = 3;
    }
    ring = func_002E75C8(kind);
    first = *(u32 *)(work + 0x44);
    groups = *(s32 *)(ring + 8) / 4;
    *handle = (u32)ring;
    entry = *(u32 **)(ring + 0x14);
    second = *(u32 *)(work + 0x48);
    third = *(u32 *)(work + 0x4C);
    for (i = 0; i < groups; i++) {
        entry[0] = first;
        entry[1] = second;
        entry[2] = second;
        entry[3] = third;
        entry += 4;
    }
    return handle;
}

void func_002E64C0(u32 arg0) {
    func_002E7698(*(u32 *)arg0);
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E64F0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E6758);

void func_002E68C8(s32 arg0) {
    u32 temp_v0;

    temp_v0 = *(u32 *)(*(s32 *)(arg0 + 0x30) + 4);
    *(u32 *)(*(s32 *)(*(s32 *)(arg0 + 0x30) + 8) + 4) = 0;
    func_002E7488(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E68F0);

void func_002E6B30(s32 arg0) {
    effReleaseResourceRefs(*(u32 *)(arg0 + 8));
    func_002E7420(*(u32 *)(arg0 + 4));
    func_003297C8(*(u32 *)(arg0 + 0xc));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E6B68);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E6CB8);

void func_002E6E60(s32 arg0) {
    *(u32 *)(**(s32 **)(arg0 + 0x30) + 4) = 0;
}

u32 *effCreateRingHandle(u8 *work) {
    u32 *handle = func_00328D68(4);
    u32 kind = *(u32 *)(work + 0x38);
    u8 *ring;
    u32 *entry;
    u32 groups;
    u32 i;
    u32 first;
    u32 second;
    u32 third;

    if (kind < 3) {
        *(u32 *)(work + 0x38) = 3;
        kind = 3;
    }
    ring = func_002E75C8(kind);
    first = *(u32 *)(work + 0x44);
    groups = *(s32 *)(ring + 8) / 4;
    *handle = (u32)ring;
    entry = *(u32 **)(ring + 0x14);
    second = *(u32 *)(work + 0x48);
    third = *(u32 *)(work + 0x4C);
    for (i = 0; i < groups; i++) {
        entry[0] = first;
        entry[1] = second;
        entry[2] = second;
        entry[3] = third;
        entry += 4;
    }
    return handle;
}

void func_002E6F18(u32 arg0) {
    func_002E7698(*(u32 *)arg0);
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E6F48);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7190);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7300);

void func_002E73F0(s32 arg0) {
    u64 temp_v0;

    temp_v0 = fileResolvePrimaryBuffer();
    func_002E7300(*(u16 *)(arg0 + 0xc), temp_v0);
}

extern EffOp24 D_003E9B88[];

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7420);

void func_002E7468(s32 arg0) {
    func_002E7300(*(u16 *)(arg0 + 0x2c), *(u32 *)(arg0 + 0x34));
}

void func_002E7488(u8 *work) {
    D_003E9B80[*(s32 *)(work + 0x2c)].fn();
    *(u32 *)(work + 0x28) = 0;
}

extern FnTbl24 D_003E9B8C[];

void func_002E74D0(work)
s32 *work;
{
    if ((D_00437E08 & 2) == 0) {
        D_003E9B8C[work[0x2C / 4]].fn();
        work[0x28 / 4]++;
    }
}

void func_002E7530(s32 arg0) {
    D_003E9B90[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
}

void func_002E7568(u32 arg0) {
    func_002E74D0();
    func_002E7530(arg0);
}

void func_002E7590(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002E75A0(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst + 1, src);
}

void func_002E75B8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002E75C0(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E75C8);

void func_002E7698(s32 arg0) {
    sdfQueueAssetRelease(*(u32 *)(arg0 + 0x18));
    func_003297C8(*(u32 *)(arg0 + 0x1c));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E76C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7A88);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7AE0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7B18);

extern s32 func_002E7AE0(s32);

extern void func_002E81B0(s32, s32, s32);

extern void effReplaceResourceRef(s32, s32, s32);

s32 func_002E7B90(u16 kind, s32 source) {
    s32 next = source + 0x1C;
    s32 object = func_002E7AE0(next);
    func_002E81B0(object, kind, source);
    effReplaceResourceRef(object, kind, next);
    return object;
}

extern void func_002E83C8(s32 *, s32 *);

extern void func_002E8448(s32 *, s32 *);

extern void func_002E8350(s32 *, s32);

extern void func_002E84C8(s32 *, s32 *);

extern void func_002E85C0(s32 *, s32 *);

extern void func_002E86B8(s32, u32);

void func_002E7C00(s32 *object, s32 kind, s32 *settings) {
    u16 type = kind;
    switch (type) {
    case 1:
        func_002E83C8(object, settings);
        break;
    case 2:
        func_002E8448(object, settings);
        break;
    case 4:
        func_002E8350(object, *settings);
        break;
    case 5:
        func_002E84C8(object, settings);
        break;
    case 6:
        func_002E85C0(object, settings);
        break;
    case 7:
        func_002E86B8((s32)object, (u32)settings);
        break;
    }
    object[3] = type;
}

extern s32 func_002E7B18(void);

s32 func_002E7CB8(s32 *source) {
    s32 *object = (s32 *)func_002E7B18();
    s32 *data = (s32 *)fileResolveSecondaryBuffer(source);
    if (data != NULL) {
        func_002E7C00(object, *(u16 *)((u8 *)source + 0x1C), data);
    }
    return (s32)object;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7D08);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7E70);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7EE0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7F60);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E81B0);

void effReplaceResourceRef(s32 obj, s32 id, s32 arg) {
    u32 *p = (u32 *)obj;
    if (p[0x4C / 4] != 0) {
        func_002DC028(p[0x4C / 4]);
    }
    p[0x4C / 4] = func_002DBED8(id & 0xffff, p[0], arg);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E8350);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E83C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E8448);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E84C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E85C0);

void func_002E86B8(s32 arg0, u32 arg1) {
    u32 temp_v0;

    if (*(s32 *)(arg0 + 0x48) != 0) {
        effReleaseReferenceHolder(*(s32 *)(arg0 + 0x48));
    }
    temp_v0 = func_002DDF48(arg1);
    *(u32 *)(arg0 + 0x48) = temp_v0;
}

void func_002E8708(s32 arg0) {
    if (*(s32 *)(arg0 + 0x4c) != 0) {
        fileClearRecordReferences(*(s32 *)(arg0 + 0x4c));
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E8738);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E8770);

void func_002E9668(u32 arg0) {
    func_002E8738();
    func_002E8770(arg0);
}

void func_002E9690(s32 arg0) {
    menuRecordSetVector(*(u32 *)(arg0 + 0x4c));
}

void func_002E96A8(s32 arg0) {
    func_002DC0F0(*(u32 *)(arg0 + 0x4c));
}

void func_002E96C0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 4) = arg1;
}

void func_002E96C8(u8 *p, f32 value) {
    *(f32 *)(p + 8) = value;
    dds3DispatchIndexedCallback(*(s32 *)(p + 0x4C));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E96E8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E9840);

void func_002E9A68(s32 arg0) {
    sdfQueueAssetRelease(*(u32 *)(arg0 + 0x3c));
    func_003297C8(*(u32 *)(arg0 + 0x48));
}

void func_002E9A98(s32 arg0) {
    *(u32 *)(arg0 + 0x24) = 0;
    *(u32 *)(arg0 + 0x28) = 3;
}

INCLUDE_ASM(const s32, "game/code_002DC138", effCopyRingFrameVectors);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E9B20);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E9BC8);

s32 func_002E9E48(const Matrix4 *matrix) {
    void *work = sdfAllocPacketAligned(0x20);
    D_00437E68 = (u32)work;
    sdfResetPacketList(work);
    __asm__ volatile(
        ".set noreorder\n\t"
        "lqc2 $vf28, 0(%0)\n\t"
        "lqc2 $vf29, 16(%0)\n\t"
        "lqc2 $vf30, 32(%0)\n\t"
        "lqc2 $vf31, 48(%0)\n\t"
        ".set reorder" :: "r"(matrix) : "memory");
    return sdfConsAppendVuPacket(D_00437E68, 0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E9E98);

void func_002EA0E8(u32 index) {
    u8 *entry = D_003E9CA8[index];
    ((void (*)(u8 *, u32))*(void **)(entry + 0x10))(entry, D_00437E68);
    D_00437E68 = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EA120);

INCLUDE_ASM(const s32, "game/code_002DC138", effReleaseRenderResources);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EA410);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EA530);

void func_002EA5D0(s32 arg0) {
    *(u32 *)(arg0 + 0x2c) = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EA5D8);

void func_002EAB78(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002EAB88(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst + 1, src);
}

void func_002EABA0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002EABA8(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_002DC138", effRandomizeParticleFields);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EAC38);

INCLUDE_ASM(const s32, "game/code_002DC138", effFreeIndexedEntries);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EB058);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EB728);

extern void effResetDispatchCounter(u8 *);

void func_002EB908(s32 *work) {
    u32 count = *(u32 *)(work[0x34 / 4] + 0x38);
    s32 *entry = *(s32 **)work[0x30 / 4];
    u32 i;
    for (i = 0; i < count; i++) {
        effResetDispatchCounter((u8 *)*entry++);
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EB968);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EBB88);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EBBF8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EBC58);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EBE28);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EBF40);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EC300);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EC370);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002ECD10);

extern s32 D_0037F550[4];

extern s32 effMiscRand(s32 *);

void func_002ECEF0(s32 *work) {
    u32 count = *(u32 *)(work[0x34 / 4] + 0x38);
    s32 *entry = *(s32 **)work[0x30 / 4];
    u32 i;
    for (i = 0; i < count; i++, entry += 3) {
        entry[1] = -1 - (effMiscRand(D_0037F550) & 3);
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002ECF78);

extern void func_002EE0A8(s32);

void func_002ED360(s32 *work) {
    u32 count = *(u32 *)(work[0x34 / 4] + 0x38);
    s32 *entries = (s32 *)work[0x30 / 4];
    s32 *entry = (s32 *)*entries;
    u32 i;
    for (i = 0; i < count; i++, entry += 3) {
        func_002EE0A8(entry[0]);
    }
    func_00328E48(entries);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002ED3D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EDB10);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EDCF0);

void func_002EDDE0(s32 arg0) {
    u64 temp_v0;

    temp_v0 = fileResolvePrimaryBuffer();
    func_002EDCF0(*(u16 *)(arg0 + 0xc), temp_v0);
}

extern EffOp24 D_003E9D08[];

void func_002EDE10(s32 arg0) {
    u32 *obj = (u32 *)arg0;
    D_003E9D08[obj[0x2C / 4]].run();
    func_00328E48(obj);
}

extern u32 func_002EDCF0(u32, u32);

u32 func_002EDE58(s32 arg0) {
    return func_002EDCF0(*(u16 *)(arg0 + 0x2c), *(u32 *)(arg0 + 0x34));
}

void effResetDispatchCounter(u8 *work) {
    D_003E9D00[*(s32 *)(work + 0x2C)].fn(work);
    *(u32 *)(work + 0x28) = 0;
}

extern FnTbl24 D_003E9D0C[];

void func_002EDEC0(work)
s32 *work;
{
    if ((D_00437E08 & 2) == 0) {
        D_003E9D0C[work[0x2C / 4]].fn();
        work[0x28 / 4]++;
    }
}

void func_002EDF20(s32 arg0) {
    D_003E9D10[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
}

void func_002EDF58(u32 arg0) {
    func_002EDEC0();
    func_002EDF20(arg0);
}

void func_002EDF80(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002EDF90(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst + 1, src);
}

void func_002EDFA8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002EDFB0(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EDFB8);

void func_002EE0A8(s32 arg0) {
    sdfQueueAssetRelease(*(u32 *)(arg0 + 0x18));
    func_003297C8(*(u32 *)(arg0 + 0x1c));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EE0D8);

EffectStripNode *func_002EE348(u32 percent) {
    EffectStripNode *node = func_00328E18(sizeof(EffectStripNode));
    node->percent = percent;
    node->color = 0x80808080;
    node->opacity = 1.0f;
    node->active = 0;
    node->transform = func_002E5C50(percent * 4, 2, 0);
    node->resource = effRetainResource(0);
    node->count = 1;
    return node;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EE3C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EE400);

void effReleaseModelResources(u32 *p) {
    if (p[0x30 / 4] != 0) {
        billDispatchByKind(p[0x30 / 4]);
    }
    if (p[0x2C / 4] != 0) {
        effReleaseResourceRefs(p[0x2C / 4]);
    }
    if (p[0x34 / 4] != 0) {
        func_002DC028(p[0x34 / 4]);
    }
    func_00328E48(p);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EE508);

void effReplaceFileResourceRef(s32 obj, s32 id, s32 arg) {
    u32 *p = (u32 *)obj;
    if (p[0x34 / 4] != 0) {
        func_002DC028(p[0x34 / 4]);
    }
    p[0x34 / 4] = func_002DBED8(id & 0xffff, p[0], arg);
}

void func_002EE608(s32 arg0) {
    if (*(s32 *)(arg0 + 0x34) != 0) {
        fileClearRecordReferences(*(s32 *)(arg0 + 0x34));
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EE638);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EE670);

void func_002EED48(u32 arg0) {
    func_002EE638();
    func_002EE670(arg0);
}

void func_002EED70(s32 arg0) {
    menuRecordSetVector(*(u32 *)(arg0 + 0x34));
}

void func_002EED88(s32 arg0) {
    func_002DC0F0(*(u32 *)(arg0 + 0x34));
}

void func_002EEDA0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 4) = arg1;
}

void func_002EEDA8(u8 *p, f32 value) {
    *(f32 *)(p + 8) = value;
    dds3DispatchIndexedCallback(*(s32 *)(p + 0x34));
}

void effResetBillTable(u8 *p) {
    u8 *a = *(u8 **)(p + 0x30);
    u8 *b = *(u8 **)(p + 0x34);
    u32 n = *(u32 *)(b + 0x38);
    u32 *counts = *(u32 **)(*(u8 **)(a + 4) + 0x1C);
    u8 *rec = *(u8 **)a;
    u32 i;
    for (i = 0; i < n; i++) {
        *counts++ = 0;
        *(s32 *)rec = -1;
        rec += 0x30;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EEE18);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EEE88);

u32 *func_002EF0D0(u8 *p, u32 a1) {
    u32 *buf = (u32 *)func_002EEE18(p);
    buf[1] = func_002F1740(*(u32 *)(p + 0x38), *(u32 *)(p + 0x8C), a1);
    func_002EEE88(buf, p);
    return buf;
}

u32 *func_002EF130(u8 *p) {
    u8 *dst = *(u8 **)(p + 0x34);
    u32 *src = *(u32 **)(p + 0x30);
    u32 *buf = (u32 *)func_002EEE18(dst);
    buf[1] = func_002F1820(src[1]);
    func_002EEE88(buf, dst);
    return buf;
}

void func_002EF190(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x30);
    func_002F17B8(*(u32 *)(temp_v0 + 4));
    func_003297C8(*(u32 *)(temp_v0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EF1C0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EF8C0);

void func_002EFA28(u8 *p) {
    u8 *a = *(u8 **)(p + 0x30);
    u8 *b = *(u8 **)(p + 0x34);
    u32 n = *(u32 *)(b + 0x38);
    u32 *counts = *(u32 **)(*(u8 **)(a + 4) + 0x1C);
    u8 *rec = *(u8 **)a;
    u32 i;
    for (i = 0; i < n; i++) {
        *counts++ = 0;
        *(s32 *)rec = -1;
        rec += 0x30;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EFA78);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EFAE8);

u32 *func_002EFD30(u8 *p, u32 a1) {
    u32 *buf = (u32 *)func_002EFA78(p);
    buf[1] = func_002F1740(*(u32 *)(p + 0x38), *(u32 *)(p + 0x8C), a1);
    func_002EFAE8(buf, p);
    return buf;
}

u32 *func_002EFD90(u8 *p) {
    u8 *dst = *(u8 **)(p + 0x34);
    u32 *src = *(u32 **)(p + 0x30);
    u32 *buf = (u32 *)func_002EFA78(dst);
    buf[1] = func_002F1820(src[1]);
    func_002EFAE8(buf, dst);
    return buf;
}

void func_002EFDF0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x30);
    func_002F17B8(*(u32 *)(temp_v0 + 4));
    func_003297C8(*(u32 *)(temp_v0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EFE20);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F0530);

void func_002F0698(u8 *p) {
    u8 *a = *(u8 **)(p + 0x30);
    u8 *b = *(u8 **)(p + 0x34);
    u32 n = *(u32 *)(b + 0x38);
    u32 *counts = *(u32 **)(*(u8 **)(a + 4) + 0x1C);
    u8 *rec = *(u8 **)a;
    u32 i;
    for (i = 0; i < n; i++) {
        *counts++ = 0;
        *(s32 *)rec = -1;
        rec += 0x2C;
    }
}

u8 *func_002F06E8(u8 *config) {
    u8 *base = func_003292A8(*(u32 *)(config + 0x38) * 0x2C + 0xC);
    EffectNodeHeader *node = (EffectNodeHeader *)sdfResourceRetainAddress((u32)base);
    u32 count = *(u32 *)(config + 0x8C);
    u8 *entries = (u8 *)node + 0xC;

    node->entries = entries;
    node->allocation = base;
    if (count < 3) {
        *(u32 *)(config + 0x8C) = 3;
    }
    return (u8 *)node;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F0760);

u32 *func_002F09A8(u8 *p, u32 a1) {
    u32 *buf = (u32 *)func_002F06E8(p);
    buf[1] = func_002F1740(*(u32 *)(p + 0x38), *(u32 *)(p + 0x8C), a1);
    func_002F0760(buf, p);
    return buf;
}

u32 *func_002F0A08(u8 *p) {
    u8 *dst = *(u8 **)(p + 0x34);
    u32 *src = *(u32 **)(p + 0x30);
    u32 *buf = (u32 *)func_002F06E8(dst);
    buf[1] = func_002F1820(src[1]);
    func_002F0760(buf, dst);
    return buf;
}

void func_002F0A68(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x30);
    func_002F17B8(*(u32 *)(temp_v0 + 4));
    func_003297C8(*(u32 *)(temp_v0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F0A98);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F10C0);

INCLUDE_ASM(const s32, "game/code_002DC138", effAllocateBlock);

INCLUDE_ASM(const s32, "game/code_002DC138", effCreateResourceInstanceB);

INCLUDE_ASM(const s32, "game/code_002DC138", effCreateFileResourceInstanceB);

extern EffOp28 D_003E9DE0[];

void func_002F13E0(u32 *obj) {
    D_003E9DE0[obj[0x2C / 4]].run();
    func_00328E48(obj);
}

INCLUDE_ASM(const s32, "game/code_002DC138", effDuplicateActiveResourceB);

void func_002F14B8(u8 *work) {
    D_003E9DD8[*(s32 *)(work + 0x2c)].fn();
    *(u32 *)(work + 0x28) = 0;
}

extern FnTbl28 D_003E9DE8[];

void func_002F1500(work)
s32 *work;
{
    if ((D_00437E08 & 2) == 0) {
        D_003E9DE8[work[0x2C / 4]].fn();
        work[0x28 / 4]++;
    }
}

void func_002F1560(s32 arg0) {
    D_003E9DEC[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
}

void func_002F1598(u32 arg0) {
    func_002F1500();
    func_002F1560(arg0);
}

void func_002F15C0(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002F15D0(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst + 1, src);
}

void func_002F15E8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002F15F0(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F15F8);

u32 func_002F1740(u32 count, u32 repeat, u32 resource) {
    u8 *node = func_002F15F8(count, repeat);

    if (resource == 0) {
        s32 references = D_00437E74;
        *(u32 *)(node + 0x18) = 0;
        if (references == 0) {
            D_00437E78 = func_002DDCA0(D_00437E70, 0x300);
            references = D_00437E74;
        }
        references++;
        D_00437E74 = references;
    } else {
        *(void **)(node + 0x18) = func_002DDAA8((void *)resource);
    }
    return (u32)node;
}

void func_002F17B8(s32 arg0) {
    if (*(s32 *)(arg0 + 0x18) == 0) {
        D_00437E74 = D_00437E74 - 1;
        if (D_00437E74 == 0) {
            effReleaseSharedReference(D_00437E78);
            D_00437E78 = 0;
        }
    }
    else {
        effReleaseSharedReference(*(s32 *)(arg0 + 0x18));
    }
    sdfQueueAssetRelease(*(u32 *)(arg0 + 0x2c));
    func_003297C8(*(u32 *)(arg0 + 0x30));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F1820);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F1888);

void effLoadWindTexture(void) {
    D_00437E6C = func_00343ED0("/effect/wind00.tmx", &D_00437E70, 0);
}

u32 func_002F1CC8(void) {
    return D_00437E70;
}

void func_002F1CD0(u8 *p) {
    u8 *a = *(u8 **)(p + 0x30);
    u8 *b = *(u8 **)(p + 0x34);
    u32 n = *(u32 *)(b + 0x38);
    u32 *counts = *(u32 **)(*(u8 **)(a + 4) + 0x18);
    u8 *rec = *(u8 **)a;
    u32 i;
    for (i = 0; i < n; i++) {
        *counts++ = 0;
        *(s32 *)rec = -1;
        rec += 0x30;
    }
}

u8 *effAllocateAnimationBuffer(u8 *config) {
    u8 *base = func_003292A8(*(u32 *)(config + 0x38) * 0x30 + 0xC);
    EffectNodeHeader *node = (EffectNodeHeader *)sdfResourceRetainAddress((u32)base);
    u32 count = *(u32 *)(config + 0x8C);
    u8 *entries = (u8 *)node + 0xC;

    node->entries = entries;
    node->allocation = base;
    if (count < 3) {
        *(u32 *)(config + 0x8C) = 3;
    }
    return (u8 *)node;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F1D90);

u32 *effPrepareTextureAnimation(u8 *src) {
    u32 *buf = (u32 *)effAllocateAnimationBuffer(src);
    buf[1] = func_002F3ED8(*(u32 *)(src + 0x38), *(u32 *)(src + 0x8C));
    func_002F1D90(buf, src);
    return buf;
}

u32 *effPrepareOwnedTextureAnimation(u8 *p) {
    u8 *dst = *(u8 **)(p + 0x34);
    u32 *src = *(u32 **)(p + 0x30);
    u32 *buf = (u32 *)effAllocateAnimationBuffer(dst);
    buf[1] = func_002F3F40(src[1]);
    func_002F1D90(buf, dst);
    return buf;
}

void func_002F2020(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x30);
    func_002F3F08(*(u32 *)(temp_v0 + 4));
    func_003297C8(*(u32 *)(temp_v0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F2050);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F2760);

INCLUDE_ASM(const s32, "game/code_002DC138", effInitializeAnimationPositions);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F2950);

u32 *effCreateAnimationState(u32 unused, u32 count) {
    u32 allocation = (u32)func_003292A8(count * 8 + 0x10);
    u32 *state = (u32 *)sdfResourceRetainAddress(allocation);

    state[3] = allocation;
    state[0] = (u32)(state + 4);
    state[2] = 0;
    state[1] = func_002F3D18();
    return state;
}

u32 *effActivateAnimationState(s32 work) {
    s32 owner = *(s32 *)(work + 0x30);
    s32 resource = *(s32 *)(owner + 8);
    u32 *state = effCreateAnimationState(*(u32 *)(work + 0x34), *(u32 *)(resource + 8));

    resource = *(s32 *)(owner + 8);
    state[2] = func_002DBED8(*(u16 *)resource, *(u32 *)(resource + 8),
                               *(void **)(resource + 0x24));
    return state;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F2A30);

INCLUDE_ASM(const s32, "game/code_002DC138", effSynchronizeFileTransform);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F2AE8);

u32 *func_002F3100(u8 *work) {
    u8 *mapping = work + 0x3C;
    u32 count = func_002F2950(mapping);
    u32 *state = effCreateAnimationState((u32)work, count);

    state[2] = func_002DBED8(1, count, mapping);
    return state;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3168);

void func_002F31D0(s32 arg0) {
    *(u32 *)(*(s32 *)(*(s32 *)(arg0 + 0x30) + 4) + 8) = 0;
}

u32 *effAllocateQuantizedBuffer(u8 *work) {
    void *allocation = func_003292A8(0xC);
    u32 *buffer = (u32 *)sdfResourceRetainAddress((u32)allocation);
    u32 count = *(u32 *)(work + 0x74);

    buffer[2] = (u32)allocation;
    if (count < 4) {
        *(u32 *)(work + 0x74) = 4;
        count = 4;
    }
    buffer[0] = count >> 2;
    if ((*(u32 *)(work + 0x74) & 3) != 0) {
        buffer[0] = (count >> 2) + 1;
    }
    return buffer;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3258);

u32 *effPrepareQuantizedTexture(u8 *src) {
    u32 *buf = (u32 *)effAllocateQuantizedBuffer(src);
    buf[1] = func_002F3ED8(buf[0], *(u32 *)(src + 0x74));
    func_002F3258(buf, src);
    return buf;
}

u32 *effPrepareOwnedQuantizedTexture(u8 *p) {
    u8 *dst = *(u8 **)(p + 0x34);
    u32 *src = *(u32 **)(p + 0x30);
    u32 *buf = (u32 *)effAllocateQuantizedBuffer(dst);
    buf[1] = func_002F3F40(src[1]);
    func_002F3258(buf, dst);
    return buf;
}

void func_002F3750(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x30);
    func_002F3F08(*(u32 *)(temp_v0 + 4));
    func_003297C8(*(u32 *)(temp_v0 + 8));
}

void effOffsetNodeRowsVU(u8 *work) {
    s32 *list = *(s32 **)(work + 0x30);
    u8 *config = *(u8 **)(work + 0x34);
    s32 rows = *(s32 *)(config + 0x74) + 1;
    s32 count = list[0];
    u8 *node = *(u8 **)&list[1];
    u8 *entry = *(u8 **)(node + 0x24);
    s32 i;
    s32 j;
    s32 k;
    f32 *v;

    for (i = 0; i < count; i++) {
        for (j = 0; j < rows; j++) {
            v = (f32 *)entry + 1;
            for (k = 0; k < 4; k++) {
                *v += *(f32 *)(config + 0x88);
                v += 2;
            }
            entry += 0x20;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F37F8);

u8 *effAllocateBlockWithModel(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = D_003E9E78[kind].resourceSize;
    u8 *effect = func_00328D68(size + headerSize);
    *(u8 **)(effect + 0x34) = effect + headerSize;
    *(u32 *)(effect + 0x24) = 0x80808080;
    *(float *)(effect + 0x20) = 1.0f;
    *(u32 *)(effect + 0x2C) = kind;
    *(u32 *)(effect + 0x28) = 0;
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" : : "r"(effect) : "memory");
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" : : "r"(effect + 0x10) : "memory");
    memcpy(*(void **)(effect + 0x34), source, size);
    return effect;
}

INCLUDE_ASM(const s32, "game/code_002DC138", effCreateResourceInstanceC);

void func_002F3AD0(s32 arg0) {
    u64 temp_v0;

    temp_v0 = fileResolvePrimaryBuffer();
    effCreateResourceInstanceC(*(u16 *)(arg0 + 0xc), temp_v0);
}

extern EffOp28 D_003E9E68[];

INCLUDE_ASM(const s32, "game/code_002DC138", effDispatchCleanupOp);

INCLUDE_ASM(const s32, "game/code_002DC138", effRecreateActiveByClass);

void func_002F3BD8(u8 *work) {
    D_003E9E60[*(s32 *)(work + 0x2c)].fn();
    *(u32 *)(work + 0x28) = 0;
}

void func_002F3C20(work)
s32 *work;
{
    if ((D_00437E08 & 2) == 0) {
        D_003E9E70[work[0x2C / 4]].fn();
        work[0x28 / 4]++;
    }
}

void func_002F3C80(s32 arg0) {
    D_003E9E74[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
}

void func_002F3CB8(u32 arg0) {
    func_002F3C20();
    func_002F3C80(arg0);
}

void func_002F3CE0(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002F3CF0(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst + 1, src);
}

void func_002F3D08(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002F3D10(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

u32 func_002F3D18(void) {
    if (D_00437E84 == 0) {
        D_00437E88 = func_002DDCA0(D_00437E80, 0x200);
    }
    D_00437E84 = D_00437E84 + 1;
    return D_00437E88;
}

void func_002F3D58(void) {
    D_00437E84 = D_00437E84 - 1;
    if (D_00437E84 == 0) {
        effReleaseSharedReference(D_00437E88);
        D_00437E88 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3D88);

u64 func_002F3ED8(void) {
    u64 temp_v0;

    temp_v0 = func_002F3D88();
    func_002F3D18();
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3F08);

void func_002F3F40(u8 *work) {
    func_002F3D88(*(u32 *)work, *(u32 *)(work + 0x10));
    D_00437E84++;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3F70);

void effLoadScalyTexture(void) {
    D_00437E7C = func_00343ED0("/effect/scaly00.tmx", &D_00437E80, 0);
}

u32 func_002F45A0(void) {
    return D_00437E80;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F45A8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F46D8);

INCLUDE_ASM(const s32, "game/code_002DC138", effReleaseParticleList);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F4960);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5168);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5358);

u32 func_002F5488(u8 *work) {
    void *first = fileResolvePrimaryBuffer();
    void *second = fileResolveSecondaryBuffer(work);
    return func_002F5358(*(u16 *)(work + 0xC), first, second, *(u32 *)(work + 0x24));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F54D0);

INCLUDE_ASM(const s32, "game/code_002DC138", effCreateModelResource);

void func_002F55F0(u8 *work) {
    D_003E9F08[*(s32 *)(work + 0x2c)].fn();
    *(u32 *)(work + 0x28) = 0;
}

void func_002F5638(work)
s32 *work;
{
    if ((D_00437E08 & 2) == 0) {
        D_003E9F14[work[0x2C / 4]].fn();
        work[0x28 / 4]++;
    }
}

void func_002F5698(s32 arg0) {
    D_003E9F18[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
}

void func_002F56D0(u32 arg0) {
    func_002F5638();
    func_002F5698(arg0);
}

void func_002F56F8(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002F5708(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst + 1, src);
}

void func_002F5720(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002F5728(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5730);

void func_002F5820(s32 arg0) {
    sdfQueueAssetRelease(*(u32 *)(arg0 + 0x18));
    func_003297C8(*(u32 *)(arg0 + 0x1c));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5850);

INCLUDE_ASM(const s32, "game/code_002DC138", effGetWorldVector);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5BC8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5D70);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5EF0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F6000);

extern void func_0023C978(s32, s32, s32);

void func_002F6450(void) {
    s32 owner = func_001AA6F8();
    s32 *entry;

    if ((*(u32 *)(owner + 0x218) & 0x6000000) != 0x6000000) {
        return;
    }
    entry = *(s32 **)(owner + 0x24C);
    while (entry != NULL) {
        if (entry[0x110 / 4] & 2) {
            s32 child = entry[0x340 / 4];
            if (child != 0) {
                *(s32 *)(child + 0x60) = entry[0x54 / 4];
                func_0023C978(child, 0, entry[0x54 / 4]);
            }
        }
        entry = (s32 *)entry[0x364 / 4];
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F64D8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F66F0);

void effResetDefaultColorTables(void) {
    u128 *dst = D_00458460;
    u128 *src = D_0037F770[0];
    PCP_COPY_VECTOR(dst, src);
    dst++;
    src++;
    PCP_COPY_VECTOR(dst, src);
    PCP_COPY_VECTOR(D_004584A0, D_0037F780);
    D_00437E94 = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F67D8);

void effResetObjectSlots(u8 *work) {
    u32 *objects = (u32 *)(*(u32 *)(work + 0x30) + 0x24);
    u32 i;
    for (i = 0; i < 5; i++) {
        u32 object = objects[i];
        if (object != 0) {
            *(u32 *)object = 0;
            *(u32 *)(object + 0x0C) = 0;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F69F0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F6A80);

extern void func_00129E50(s32, s32);

extern void func_002F6A80(s32 *);

s32 *func_002F6BE8(u32 owner, u32 unused, u32 source, u32 kind) {
    s32 *work = func_002F69F0(owner, source, kind);
    s32 object = *work;
    func_00129E50(object, object + 8);
    func_002F6A80(work);
    return work;
}

s32 *func_002F6C30(s32 arg0) {
    s32 *work;

    work = func_002F69F0(*(u32 *)(arg0 + 0x38), **(u32 **)(arg0 + 0x30),
                                                (*(u32 **)(arg0 + 0x30))[1]);
    func_002F6A80(work);
    return work;
}

extern void dds3FreePathObject();
extern void func_00110B50();

void effReleaseTargetSlots(u32 *obj) {
    u32 *tails = obj + 0x10 / 4;
    u32 *heads = obj + 0x24 / 4;
    u32 i;
    for (i = 0; i < 5; i++) {
        if (*heads != 0) {
            dds3FreePathObject(*heads);
        }
        heads++;
        if (*tails != 0) {
            func_00110B50(*tails);
        }
        tails++;
    }
    func_003297C8(obj[0x38 / 4]);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F6D00);

typedef struct EffActor {
    u8 unk0[0x110];
    u32 flags;
    u8 unk114[0x21C];
    u16 animFlags;
    u8 unk332[0xE];
    struct EffModelRef *model;
} EffActor;

typedef struct EffModelRef {
    u8 unk0[0x8C];
    s32 nodes;
} EffModelRef;

typedef struct EffAnimInfo {
    u16 id;
    u16 flags;
    u8 unk4;
    u8 unk5;
    u16 loop;
} EffAnimInfo;

typedef struct EffAnimOwner {
    u8 unk0[0x28];
    s32 hold;
    u8 unk2C[0xC];
    EffAnimInfo *info;
} EffAnimOwner;

extern u32 func_002F5D70(EffActor **, u8);
extern s32 mdlGetNodeRefHalf();
extern void func_001E2C00(EffActor *, u16, s32, f32);
extern void func_00204D08();

void func_002F7128(EffAnimOwner *owner) {
    EffActor *actor[16];
    EffAnimInfo *info;
    u32 count;
    u32 i;
    if (owner->hold <= 0) {
        info = owner->info;
        count = func_002F5D70(actor, info->unk4);
        for (i = 0; i < count; i++) {
            if (actor[i]->flags & 2) {
                if (!(actor[i]->flags & 0x20) && !(actor[i]->animFlags & 0x10)) {
                    if (actor[i]->animFlags & 0x40) {
                        switch (info->id) {
                        case 0:
                        case 2:
                        case 0xa:
                            continue;
                        }
                    }
                    if (mdlGetNodeRefHalf(actor[i]->model->nodes, 0) > info->id) {
                        func_001E2C00(actor[i], info->id, info->flags | 0x100, 1.0f);
                        if (info->loop == 0) {
                            func_00204D08(actor[i], info->id);
                        }
                    }
                }
            }
        }
    }
}

void effReportResourceStatus(u8 *work) {
    u32 status;

    if (*(s32 *)(work + 0x28) > 0) {
        return;
    }
    status = **(u32 **)(work + 0x38);
    switch (status) {
    case 0:
        func_002040A8(0x1000A);
        break;
    case 1:
        func_002040A8(0x1000B);
        break;
    }
}

void func_002F72D8(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    if ((*(u32 *)(temp_v0 + 0x218) & 0x6000000) == 0x6000000) {
        func_00200930(temp_v0 + 0x50, temp_v0 + 0x60, 0);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7320);

void effResetSlots(void) {
    D_00437E98[0] = 0;
    D_00437EA0[0] = 0;
    D_00437EA8[0] = 0;
    D_00437EB0[0] = 0;
    D_00437E98[1] = 0;
    D_00437EA0[1] = 0;
    D_00437EA8[1] = 0;
    D_00437EB0[1] = 0;
    func_00104020();
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7580);

void effSetDormantSlot(u32 slot, u8 value, u32 data) {
    if (slot < 2) {
        D_00437E98[slot] = data;
        D_00437EA0[slot] = data;
        D_00437EA8[slot] = value;
        D_00437EB0[slot] = 0;
    }
}

void effSetActiveSlot(u32 slot, u8 value, u32 data) {
    if (slot < 2) {
        D_00437E98[slot] = data;
        D_00437EA0[slot] = data;
        D_00437EA8[slot] = value;
        D_00437EB0[slot] = 1;
    }
}

void func_002F7798(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    if ((*(u32 *)(temp_v0 + 0x218) & 0x6000000) == 0x6000000) {
        effResetSlots();
        return;
    }
}

void func_002F77D0(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;
    u32 *puVar3;
    u8 *puVar4;
    u32 temp_v2;

    temp_v2 = 0;
    func_001AA6F8();
    temp_v0 = *(s32 *)(arg0 + 0x28);
    puVar4 = (u8 *)(*(s32 *)(arg0 + 0x38) + 0x18);
    puVar3 = (u32 *)(*(s32 *)(arg0 + 0x38) + 0x10);
    do {
        temp_v1 = puVar3[-4];
        if (temp_v1 < *puVar3) {
            return;
        }
        if (temp_v0 == 0) {
            effSetActiveSlot(temp_v2, *puVar4, puVar3[-2]);
            temp_v1 = puVar3[-4];
        }
        if ((temp_v1 != 0) && (temp_v0 == temp_v1 - *puVar3)) {
            effSetDormantSlot(temp_v2, *puVar4, *puVar3);
        }
        temp_v2 = temp_v2 + 1;
        puVar4 = puVar4 + 1;
        puVar3 = puVar3 + 1;
    } while (temp_v2 < 2);
}

void func_002F7890(void) {
    func_002005B0(0xc);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F78A8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7928);

u32 *func_002F79E0(u32 owner) {
    u32 *work = (u32 *)func_00328D68(4);
    *work = 0;
    return work;
}

u32 *func_002F7A00(u32 owner) {
    u32 *work = func_002F79E0(owner);
    *work = func_002EDCF0(4, owner);
    return work;
}

u32 *func_002F7A48(u8 *request) {
    u32 *source = *(u32 **)(request + 0x30);
    u32 *work = func_002F79E0(*(u32 *)(request + 0x38));
    *work = func_002EDE58(*source);
    return work;
}

void func_002F7A90(u32 arg0) {
    if (*(s32 *)arg0 != 0) {
        func_002EDE10(*(s32 *)arg0);
    }
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7AC8);

void func_002F7C40(s32 arg0) {
    func_002EDF20(**(u32 **)(arg0 + 0x30));
}

u32 *func_002F7C60(u32 owner) {
    u32 *work = (u32 *)func_00328D68(4);
    *work = 0;
    return work;
}

u32 *func_002F7C80(s32 source, u16 kind, s32 *settings) {
    u32 *work = func_002F7C60(source);
    *work = func_002E7B90(7, source);
    func_002E7C00((s32 *)*work, kind, settings);
    return work;
}

extern u32 func_002E7EE0(s32);

u32 *func_002F7CF0(u8 *request) {
    s32 *source = *(s32 **)(request + 0x30);
    u32 *work = func_002F7C60(*(u32 *)(request + 0x38));
    *work = func_002E7EE0(*source);
    return work;
}

void func_002F7D38(u32 arg0) {
    if (*(s32 *)arg0 != 0) {
        func_002E7D08(*(s32 *)arg0);
    }
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7D70);

void func_002F7E88(s32 arg0) {
    func_002E8770(**(u32 **)(arg0 + 0x30));
}

u32 *func_002F7EA8(u32 owner) {
    u32 *work = (u32 *)func_00328D68(4);
    *work = 0;
    return work;
}

extern void mdlAddEntryPlain(s32, u32, u32);

extern void mdlAddEntryFlagged(s32, u32, u32);

u32 *func_002F7EC8(u32 *owner, u32 kind, u32 source, u32 settings) {
    u32 *work = func_002F7EA8((u32)owner);
    s32 object = func_002DC1D0(source, settings);
    s32 active = *(s32 *)(object + 0x1C);
    *work = object;
    if (active != 0) {
        if (*owner != 0) {
            mdlAddEntryPlain(object, 0, 0);
        } else {
            mdlAddEntryFlagged(object, 0, 0);
        }
    }
    return work;
}

u32 *func_002F7F58(u8 *request) {
    u32 *owner = *(u32 **)(request + 0x38);
    void **source = *(void ***)(request + 0x30);
    u32 *work = func_002F7EA8((u32)owner);
    void *a = func_00232EE8(*source);
    void *b = func_00232EF8(*source);
    void *object = func_00232198(a, b);
    *work = (u32)object;
    effInitModelVUState(object);
    if (*(s32 *)(*work + 0x1C) != 0) {
        if (*owner != 0) {
            mdlAddEntryPlain(*work, 0, 0);
        } else {
            mdlAddEntryFlagged(*work, 0, 0);
        }
    }
    return work;
}

void func_002F8008(u32 arg0) {
    if (*(s32 *)arg0 != 0) {
        func_002DC260(*(s32 *)arg0);
    }
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F8040);

s32 *func_002F81A8(s32 *owner) {
    s32 *work = (s32 *)func_00328D68(8);
    work[0] = 0;
    work[1] = 0;
    return work;
}

s32 *func_002F81D0(s32 *owner, u32 kind, u32 source, u32 settings) {
    s32 *work = func_002F81A8(owner);
    s32 object;
    s32 active;

    work[0] = func_002EDCF0(4, (u32)owner);
    object = func_002DC1D0(source, settings);
    active = *(s32 *)(object + 0x1C);
    work[1] = object;
    if (active != 0) {
        if (owner[0x34 / 4] != 0) {
            mdlAddEntryPlain(object, 0, 0);
        } else {
            mdlAddEntryFlagged(object, 0, 0);
        }
    }
    return work;
}

s32 *func_002F8270(u8 *request) {
    s32 *owner = *(s32 **)(request + 0x38);
    u32 *source = *(u32 **)(request + 0x30);
    s32 *work = func_002F81A8(owner);
    void *a;
    void *b;
    void *object;
    s32 material;
    u32 modelSource;

    material = func_002EDE58(source[0]);
    modelSource = source[1];
    work[0] = material;
    a = func_00232EE8((void *)modelSource);
    b = func_00232EF8((void *)source[1]);
    object = func_00232198(a, b);
    work[1] = (s32)object;
    effInitModelVUState(object);
    if (*(s32 *)(work[1] + 0x1C) != 0) {
        if (owner[0x34 / 4] != 0) {
            mdlAddEntryPlain(work[1], 0, 0);
        } else {
            mdlAddEntryFlagged(work[1], 0, 0);
        }
    }
    return work;
}

extern void func_002EDE10(s32);

void func_002F8328(s32 *work) {
    if (work[1] != 0) {
        func_002DC260(work[1]);
    }
    if (work[0] != 0) {
        func_002EDE10(work[0]);
    }
    func_00328E48(work);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F8378);

extern u8 D_00380828[];
extern void func_00232390(void *, const void *);
extern void mdlStorePrimaryVectorVU(void *);
extern void func_00232AD0(void *);

/* The model helpers read vf10, following the SDK's VU0 macro-mode convention. */
void effApplyModelTransform(u8 *work) {
    u8 *modelContext = *(u8 **)(work + 0x30);
    u8 *animation = *(u8 **)(work + 0x38);
    u32 bits;
    float scale;
    __asm__ volatile("lqc2 $vf10, 0(%0)" :: "r"(work) : "memory");
    mdlStorePrimaryVectorVU(*(void **)(modelContext + 4));
    __asm__ volatile("lqc2 $vf10, 0(%0)" :: "r"(work + 0x10) : "memory");
    func_00232AD0(*(void **)(modelContext + 4));
    __asm__ volatile(
        ".set noreorder\n\t"
        "vaddw.xyz vf10, vf0, vf0w\n\t"
        "vmulx.w vf10, vf0, vf0x\n\t"
        ".set reorder" ::: "memory");
    scale = *(float *)(work + 0x20);
    __asm__ volatile(
        ".set noreorder\n\t"
        "mfc1 %0, %1\n\t"
        "qmtc2.ni %0, $vf2\n\t"
        "vmulx.xyzw vf10, vf10, $vf2x\n\t"
        ".set reorder"
        : "=&r"(bits) : "f"(scale) : "memory");
    mdlStoreTertiaryVectorVU(*(void **)(modelContext + 4));
    *(float *)(*(u8 **)(*(u8 **)(modelContext + 4) + 0x1C) + 0x20) =
        *(float *)(animation + 0x94);
    func_00232390(*(void **)(modelContext + 4), D_00380828);
    func_002EDF20(*(s32 *)modelContext);
}

extern s32 *func_003335E0(void);

s32 *func_002F8590() {
    s32 *work = func_00328E18(0xC);
    s32 *position;
    work[2] = 0;
    position = func_003335E0();
    work[1] = (s32)position;
    *(f32 *)((u8 *)position + 0x1C) = 1.0f;
    return work;
}

void func_002F85D8(void) {
    func_002F8590();
}

void func_002F85F0(s32 arg0) {
    func_002F8590(*(u32 *)(arg0 + 0x38));
}

void func_002F8608(u32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)((s32)arg0 + 4);
    if (temp_v0 != 0) {
        sdfQueueAssetRelease(temp_v0);
    }
    func_00328E48(arg0);
}

void func_002F8640(void) {
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F8648);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F8C38);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F8CF0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F8FB8);

extern s32 *func_002F8FB8(s32 *);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F9048);

extern void func_00159FA0(s32);

extern void billSetBillboardMode(s32, s16);

s32 *func_002F9118(s32 *request) {
    s32 *source = (s32 *)request[0x30 / 4];
    s32 *context = (s32 *)request[0x38 / 4];
    s32 *resource = func_002F8FB8(context);
    resource[0] = func_00159A50(*source);
    func_00159FA0(resource[0]);
    billSetBillboardMode(resource[0], *(s16 *)((u8 *)context + 0x4C));
    return resource;
}

void func_002F9180(s32 *work) {
    if (work[0] != 0) {
        billDispatchByKind(work[0]);
    }
    if (work[1] != 0) {
        sdfQueueAssetRelease(work[1]);
    }
    func_00328E48(work);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F91D0);

INCLUDE_ASM(const s32, "game/code_002DC138", effAllocateResourcePayload);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F9608);

extern void func_002F9608(u16, u64, u16, s32, s32);

void func_002F96D0(s32 *source) {
    u64 primary = fileResolvePrimaryBuffer();
    s32 secondary = fileResolveSecondaryBuffer(source);
    func_002F9608(*(u16 *)((u8 *)source + 0xC), primary,
                  *(u16 *)((u8 *)source + 0x1C), secondary, source[0x24 / 4]);
}

extern FnTbl28 D_003EA020[];

void effDestroyResourceInstance(u32 *obj) {
    if (func_001AA308()) {
        if (D_003EA020[obj[0x2C / 4]].fn != NULL) {
            D_003EA020[obj[0x2C / 4]].fn(obj[0x30 / 4]);
        }
    }
    func_00328E48(obj);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F9780);

extern FnTbl28 D_003EA018[];
void effClearCallbackFrame(u32 *obj) {
    if (func_001AA308()) {
        if (D_003EA018[obj[0x2C / 4]].fn != NULL) {
            D_003EA018[obj[0x2C / 4]].fn(obj);
        }
        obj[0x28 / 4] = 0;
    }
}

void effAdvanceCallbackFrame(work)
u8 *work;
{
    if (func_001AA308() != 0 && (D_00437E08 & 2) == 0) {
        s32 kind = *(s32 *)(work + 0x2C);
        FnTbl28 *entry = &D_003EA028[kind];
        void (*callback)(void *) = entry->fn;
        if (callback != NULL) {
            callback(work);
        }
        (*(s32 *)(work + 0x28))++;
    }
}

void effDispatchEnabledCallback(u8 *work) {
    if (func_001AA308() != 0) {
        void (*callback)(void *) = D_003EA02C[*(s32 *)(work + 0x2C)].fn;
        if (callback != NULL) {
            callback(work);
        }
    }
}

void func_002F9988(u32 arg0) {
    effAdvanceCallbackFrame();
    effDispatchEnabledCallback(arg0);
}

void func_002F99B0(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002F99C0(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst + 1, src);
}

void func_002F99D8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002F99E0(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F99E8);

void effReleaseParticleResources(u32 *p) {
    if (p[0xA4 / 4] != 0) {
        billDispatchByKind(p[0xA4 / 4]);
    }
    if (p[0xA8 / 4] != 0) {
        effReleaseReferenceHolder(p[0xA8 / 4]);
    }
    func_00328E48(p);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F9C38);

INCLUDE_ASM(const s32, "game/code_002DC138", effReplaceSharedResource);

void func_002F9DE8(s32 arg0) {
    *(u32 *)(arg0 + 8) = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F9DF0);

void func_002FA388(u32 *arg0, u32 arg1) {
    *arg0 = arg1;
}

extern f32 func_00341240(void *);

/* Normalize the seed vector with the original VU0 macro-mode operations. */
void effInitializeParticleDirection(u8 *work, float *entry) {
    float vector[4];
    if (work[0x10] == 0) {
        vector[0] = *(float *)(work + 0x14);
        vector[1] = *(float *)(work + 0x18);
        vector[2] = *(float *)(work + 0x1C);
    } else {
        vector[0] = (func_00341240(D_0037F550) - 0.5f) * 2.0f;
        vector[1] = (func_00341240(D_0037F550) - 0.5f) * 2.0f;
        vector[2] = (func_00341240(D_0037F550) - 0.5f) * 2.0f;
    }
    __asm__ volatile(
        ".set noreorder\n\t"
        "lqc2 $vf10, %0\n\t"
        "vmul.xyz $vf2, $vf10, $vf10\n\t"
        "vmulax.w ACC, $vf0, $vf2x\n\t"
        "vmadday.w ACC, $vf0, $vf2y\n\t"
        "vmaddz.w $vf2, $vf0, $vf2z\n\t"
        "vrsqrt Q, $vf0w, $vf2w\n\t"
        "vwaitq\n\t"
        "vmulq.xyz $vf10, $vf10, Q\n\t"
        "sqc2 $vf10, %0\n\t"
        ".set reorder"
        : "+m"(vector) :: "memory");
    entry[0] = vector[0];
    entry[2] = vector[2];
    entry[1] = vector[1];
    entry[5] =
        *(float *)(work + 0x24) *
        (func_00341240(D_0037F550) * *(float *)(work + 0x28) +
         (1.0f - *(float *)(work + 0x28)));
    entry[4] = func_00341240(D_0037F550) * 6.2831853f;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FA4D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FA550);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FA5B8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FA5F0);

extern void fileQueueDestroy(s32);

void effDestroyOwnedResources(s32 *work) {
    u32 i;
    if (work[0x60 / 4] != 0) {
        func_002DC260(work[0x60 / 4]);
    }
    if (work[0x64 / 4] != 0) {
        sdfReleaseDevSlot(work[0x64 / 4], 1, 1);
    }
    if (work[0x6C / 4] != 0) {
        u32 count = ((s32 *)work[0x70 / 4])[2];
        for (i = 0; i < count; i++) {
            fileQueueDestroy(((s32 *)work[0x68 / 4])[i]);
        }
        func_003297C8(work[0x6C / 4]);
    }
    if (work[0x70 / 4] != 0) {
        func_002DC028(work[0x70 / 4]);
    }
    if (work[0x78 / 4] != 0) {
        func_003297C8(work[0x78 / 4]);
    }
    func_00328E48(work);
}

extern s32 *func_002FA5B8(s32 *);
extern void effRebuildResourceEntries(u8 *, u32, s32 *);

s32 *effCloneOwnedState(u8 *owner) {
    s32 *source = *(s32 **)(*(u8 **)(owner + 0x70) + 0x24);
    s32 *work = func_002FA5B8(source);
    memcpy((u8 *)work + 0x10, owner + 0x10, 0x50);
    effRebuildResourceEntries((u8 *)work, *(u16 *)(*(u8 **)(owner + 0x70)), source);
    func_002FA978(work, owner);
    return work;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FA978);

extern s32 func_002DBED8(u16, s32, s32);
extern void func_002DC028(s32);

void effRebuildResourceEntries(u8 *work, u32 kind, s32 *config) {
    s32 previous = *(s32 *)(work + 0x70);
    s32 allocation;
    u32 count;
    u32 i;
    u8 *entries;
    if (previous != 0) {
        func_002DC028(previous);
    }
    *(s32 *)(work + 0x70) = func_002DBED8(kind, *(s32 *)work, (s32)config);
    allocation = *(s32 *)(work + 0x78);
    if (allocation != 0) {
        func_003297C8(allocation);
    }
    count = *(u32 *)(*(s32 *)(work + 0x70) + 8);
    *(s32 *)(work + 0x78) = func_003292A8(count * 0x18);
    *(u8 **)(work + 0x74) = (u8 *)sdfResourceRetainAddress(*(s32 *)(work + 0x78));
    entries = *(u8 **)(work + 0x74);
    for (i = 0; i < count; i++, entries += 0x18) {
        effInitializeParticleDirection(work, (float *)entries);
    }
}

void func_002FABA8(s32 arg0, u32 arg1, u32 arg2) {
    s32 temp_v0;
    s32 temp_v1;
    u32 temp_v2;

    if (*(s32 *)(arg0 + 0x60) != 0) {
        func_002DC260(*(s32 *)(arg0 + 0x60));
        *(u32 *)(arg0 + 0x60) = 0;
    }
    if (*(s32 *)(arg0 + 100) != 0) {
        sdfReleaseDevSlot(*(s32 *)(arg0 + 100), 1, 1);
        *(u32 *)(arg0 + 100) = 0;
    }
    temp_v1 = func_002DC1D0(arg1, arg2);
    temp_v0 = *(s32 *)(temp_v1 + 0xc);
    *(s32 *)(arg0 + 0x60) = temp_v1;
    temp_v2 = func_00331940(*(u32 *)(temp_v0 + 0x14), *(u32 *)(temp_v0 + 0x18));
    *(u32 *)(arg0 + 100) = temp_v2;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FAC38);

void func_002FAD30(s32 arg0) {
    if (*(s32 *)(arg0 + 0x70) != 0) {
        fileClearRecordReferences(*(s32 *)(arg0 + 0x70));
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FAD60);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FAD98);

void func_002FB400(u32 arg0) {
    func_002FAD60();
    func_002FAD98(arg0);
}

void func_002FB428(s32 arg0) {
    menuRecordSetVector(*(u32 *)(arg0 + 0x70));
}

void func_002FB440(s32 arg0) {
    func_002DC0F0(*(u32 *)(arg0 + 0x70));
}

void func_002FB458(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 4) = arg1;
}

extern void dds3DispatchIndexedCallback(s32);

void func_002FB460(s32 *work, f32 value) {
    *(f32 *)((u8 *)work + 8) = value;
    dds3DispatchIndexedCallback(work[0x70 / 4]);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FB480);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FB5C0);

void effReleaseSharedResourceReference(s32 *work) {
    s32 *resource = (s32 *)work[0x478 / 4];
    u32 references = *(u16 *)((u8 *)resource + 0x10) + 0xFFFF;
    *(u16 *)((u8 *)resource + 0x10) = references;
    if ((u16)references == 0) {
        s32 data = resource[2];
        if (data != 0) {
            func_00328E48(data);
        }
        resource = (s32 *)work[0x478 / 4];
        if (resource[0] != 0) {
            func_002DC260(resource[0]);
        }
        resource = (s32 *)work[0x478 / 4];
        if (resource[1] != 0) {
            sdfReleaseDevSlot(resource[1], 1, 1);
        }
        resource = (s32 *)work[0x478 / 4];
        if (resource[0x1C / 4] != 0) {
            func_003297C8(resource[0x1C / 4]);
        }
        func_00328E48(work[0x478 / 4]);
    }
    func_003297C8(work[0x47C / 4]);
}

typedef struct { u32 word[0x113]; } EffectBlob;
extern u8 *func_002FB5C0(s32);

s32 effCloneEffectRequest(u8 *src) {
    u8 *dst = func_002FB5C0(0);

    *(EffectBlob *)(dst + 0x2C) = *(EffectBlob *)(src + 0x2C);
    func_002FB948((s32)dst, (s32)src);
    return (s32)dst;
}

void func_002FB948(s32 arg0, s32 arg1) {
    *(u32 *)(arg0 + 0x478) = *(u32 *)(arg1 + 0x478);
    *(s16 *)(*(s32 *)(arg1 + 0x478) + 0x10) = *(s16 *)(*(s32 *)(arg1 + 0x478) + 0x10) + 1;
}

void func_002FB968(s32 *work) {
    s32 *context = *(s32 **)work[0x478 / 4];
    if (context != NULL) {
        func_003343E8(context[0x1C / 4], 0.0f);
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FB998);

void func_002FC0A8(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002FC0B8(s128 *dst, s128 *src) {
    PCP_COPY_VECTOR(dst + 1, src);
}

void func_002FC0D0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x20) = arg1;
}

void func_002FC0D8(s32 *work, f32 value) {
    *(f32 *)((u8 *)work + 0x24) = value;
}

void btlInitializeEffectWork(void) {
    D_004386CC = D_003FFA40;
    D_0045C1A0[0] = 0;
    D_004386B0 = 0;
    D_00439075 = 0;
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" :: "r"(D_0045C1E0) : "memory");
}

s32 btlUpdateEffectWork(void) {
    if (D_004386CC != NULL) {
        if ((func_002FCA58(0) & 1) == 0) {
            func_002FC160();
            return 0;
        }
    }
    return 1;
}

void func_002FC158(void) {
}

void func_002FC160(void) {
    effResetFileResources();
    func_00303C50();
}

void effSetBattleOffset(void *src) {
    PCP_COPY_VECTOR(D_0045C1E0, src);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC198);

INCLUDE_ASM(const s32, "game/code_002DC138", effMultiplyQuatWithFlag);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC2B0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC340);

typedef struct EffFileJobRequest {
    u8 pad0[4];
    u16 fileKind;
    u8 pad6[2];
    u16 transferMode;
    u8 padA[2];
    void *output;
    u32 size;
    u8 pad14[4];
    u16 resourceMode;
    u8 pad1A[2];
    s32 relatedResource;
} EffFileJobRequest;

s32 effLoadFileJobPayload(EffFileJobRequest *descriptor, s32 source) {
    s32 job;

    if (source != 0) {
        void *sourceBuffer;
        job = func_002D3DF8(source);
        sourceBuffer = fileResolvePrimaryBuffer((void *)job);
        memcpy(descriptor->output, sourceBuffer, descriptor->size);
    } else {
        job = fileCreateJob(descriptor->fileKind);
        if (descriptor->output != NULL) {
            func_002D3848(job, (s32)descriptor->output,
                          (s32)descriptor->size, descriptor->transferMode);
        }
        if (descriptor->relatedResource != 0) {
            func_002D3A68(job, descriptor->relatedResource,
                          descriptor->resourceMode);
        } else {
            s32 zero = 0;
            func_002D39C8(job, &zero, 4, 4);
        }
    }
    func_00303C50();
    return job;
}

void func_002FC5C8(u32 arg0) {
    if (D_004386B4 == 0) {
        func_002FC2B0();
        func_002D3710(arg0);
        return;
    }
}

void func_002FC610(u32 arg0) {
    if (D_004386B4 != 0) {
        fileQueueDestroy(D_004386B4);
        D_004386B4 = 0;
    }
    if (D_004386C4 != 0) {
        fileJobDestroy(D_004386C4);
        D_004386C4 = 0;
    }
    fileJobDestroy(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", effPollPrimaryFile);

void func_002FC718(void) {
    func_003002A8();
    effQueueResource(D_004386E0, D_0045C1A0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", effPollNamedFile);

void func_002FC808(void) {
    func_003002A8();
    effQueueResource(D_004386E8, D_0045C1A0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", effPollAttachedFile);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC8F8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC9B8);

u32 func_002FCA40(void) {
    return D_003FFA84[0] + D_004386B0;
}

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BEA0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BEB0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BEC0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BED0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BEE0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BEF0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF00);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF10);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF20);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF30);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF40);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF50);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF60);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF70);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF80);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF90);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BFA0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BFB0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BFC0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BFD0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BFE0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BFF0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C000);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C010);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C020);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C030);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C040);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C050);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C060);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C070);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C080);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C090);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C0A0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C0B0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C0C0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C0E0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C100);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C120);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C140);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C160);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C180);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C1A0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C1C0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C1E0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C200);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C220);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C240);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C260);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C280);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C2A0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C2C0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C2E0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C300);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C320);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C340);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C360);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C380);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C3A0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C3C0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C3E0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C400);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C420);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C440);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C460);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C480);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C4A0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C4C0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C4E0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C500);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C520);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C548);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C568);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C588);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C5A8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C5C8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C5E8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C608);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C628);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C648);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C668);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C688);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C6A8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C6C8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C6E8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C708);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C728);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C748);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C768);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C788);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C7A8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C7C8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C7E8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C808);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C828);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C848);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C868);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C888);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C8A8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C8C8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C8E8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C908);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C928);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C948);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C968);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C988);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C9A8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C9C8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C9E8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CA08);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CA28);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CA48);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CA68);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CA88);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CAB0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CAD8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CB00);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CB28);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CB48);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CB68);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CB88);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CBA8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CBC8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CBE8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CC08);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CC28);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CC48);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CC68);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CC88);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CCA8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CCC8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CCE8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD08);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD28);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD38);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD48);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD58);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD68);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD78);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD88);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD98);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CDA8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CDB8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CDC8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CDD8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CDE8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CDF8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE08);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE18);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE28);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE38);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE48);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE58);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE68);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE78);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE88);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE98);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CEA8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CEB8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CEC8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CED8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CEE8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CEF8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF08);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF18);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF28);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF38);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF48);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF58);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF70);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF80);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF90);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FCA58);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FD748);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FD900);

INCLUDE_ASM(const s32, "game/code_002DC138", effReinitializeFileQueue);

INCLUDE_ASM(const s32, "game/code_002DC138", effResetFileQueueState);

extern u32 D_004386B8;
extern u8 *fileQueueGetAt(u32, u32);
extern u8 D_003FFA78[];
extern u32 D_003FF22C[11];
extern void fileJobCopyHeader(s32 resource, s32 entry);

u32 effFinalizeQueuedFile(void) {
    s32 resource;
    s32 entry;
    entry = (s32)fileQueueGetAt(D_004386B8, func_002FCA40());
    resource = fileJobDuplicateAfter(D_004386B8, entry);
    D_003FF22C[0] = 0;
    fileJobCopyHeader(resource, entry);
    D_004386F4 = (s32)D_003FFA78;
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FDDD8);

u32 func_002FE208(void) {
    D_00439075 = 1;
    func_002FE230(0);
    return 0;
}

u32 func_002FE230(void) {
    if (*(s32 *)(D_004386F0 + 0x34) != 0) {
        D_004386F4 = *(s32 *)(D_004386F0 + 0x34);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FE250);

typedef struct EffectFileSelection {
    u8 reserved[0xC];
    s32 selected;
    u32 reserved_10;
    const void *table;
    s32 count;
    u8 tail[0x1C];
} EffectFileSelection;
extern EffectFileSelection D_003FF4B0;
extern u8 D_003FF390[], D_003FF3D8[], D_0045C270[], D_00439074;

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FE400);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FE5B8);

extern void func_002FE5B8(const char *, const void *, s32);
extern char D_0042CEF8[];
extern u8 D_003FE280[];

void func_002FE948(void) {
    func_002FE5B8(D_0042CEF8, D_003FE280, 0xF);
}

extern char D_0042CED8[];
extern u8 D_003FE430[];

void func_002FE970(void) {
    func_002FE5B8(D_0042CED8, D_003FE430, 7);
}

extern u8 D_003FE500[];

void effCreatePolyTrackTask(void) {
    func_002FE5B8("POLY TRACK S", D_003FE500, 7);
}

extern char D_00437F30[];
extern u8 D_003FE610[];

void func_002FE9C0(void) {
    func_002FE5B8(D_00437F30, D_003FE610, 7);
}

extern char D_004386A8[];
extern u8 D_003FE6E0[];

void func_002FE9E8(void) {
    func_002FE5B8(D_004386A8, D_003FE6E0, 3);
}

extern char D_0042CE48[];
extern u8 D_003FE7C0[];

void func_002FEA10(void) {
    func_002FE5B8(D_0042CE48, D_003FE7C0, 5);
}

extern char D_0042CEB8[];
extern u8 D_003FE780[];

void func_002FEA38(void) {
    func_002FE5B8(D_0042CEB8, D_003FE780, 2);
}

extern char D_0042CEA8[];
extern u8 D_003FE850[];

void func_002FEA60(void) {
    func_002FE5B8(D_0042CEA8, D_003FE850, 0xB);
}

extern char D_0042CE98[]; /* "POLY RING" */

extern u8 D_003FE990[];

void func_002FEA88(void) {
    func_002FE5B8(D_0042CE98, D_003FE990, 6);
}

extern char D_0042CE88[];

extern u8 D_003FEA40[];

void func_002FEAB0(void) {
    func_002FE5B8(D_0042CE88, D_003FEA40, 5);
}

extern u8 D_003FEAD0[];

void func_002FEAD8(void) {
    func_002FE5B8("POLY TWINKLE", D_003FEAD0, 6);
}

extern char D_0042CE78[];

extern u8 D_003FEB80[];

void func_002FEB00(void) {
    func_002FE5B8(D_0042CE78, D_003FEB80, 7);
}

extern char D_0042CE68[];

extern u8 D_003FEC50[];

void func_002FEB28(void) {
    func_002FE5B8(D_0042CE68, D_003FEC50, 5);
}

extern char D_0042CE58[];

extern u8 D_003FED20[];

void func_002FEB50(void) {
    func_002FE5B8(D_0042CE58, D_003FED20, 2);
}

extern char D_0042CDF8[];

extern u8 D_003FED60[];

void func_002FEB78(void) {
    func_002FE5B8(D_0042CDF8, D_003FED60, 0x12);
}

extern char D_00438640[]; /* "2D" */

extern u8 D_003FEFE0[];

void func_002FEBA0(void) {
    func_002FE5B8(D_00438640, D_003FEFE0, 2);
}

extern char D_0042CEE8[];

extern u8 D_003FF020[];

void func_002FEBC8(void) {
    func_002FE5B8(D_0042CEE8, D_003FF020, 7);
}

extern char D_0042CF58[];
extern char **func_002FC8F8();

s32 effPollPartResource(void) {
    u8 request[0x110];
    s32 state;
    s32 result = 0x400001;
    s32 entry;

    func_00300100(D_0042CF58, 0x20, request);
    state = *(s32 *)(request + 0x100);
    if (state == 2) {
        result = 0x400000;
    } else if (state == 1) {
        if (D_004386B8 != 0) {
            entry = fileAppendJobFromEntry(D_004386B8, request);
            strcpy((char *)(entry + 0x9C), *func_002FC8F8(entry));
        }
        result = 0x400002;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_002DC138", effPollNamedFileJob);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D030);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D048);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEDD0);

s32 effResetStaticState(void) {
    D_0045C2F0[0] = 0;
    D_0045C2F0[1] = 0;
    D_0045C2F0[2] = 0;
    D_0045C2F0[3] = 0;
    D_0045C110[0x88] = 8;
    D_0045C110[0x89] = 0;
    D_0045C110[0x8a] = 0;
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FF0B8);

void func_002FF338(void) {
    D_00438758 = D_0045C110[0x88];
    D_00438759 = D_0045C110[0x89];
    D_0043875A = D_0045C110[0x8A];
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FF360);

s32 func_002FF870(void) {
    D_0045C110[0x88] = 8;
    D_0045C110[0x89] = 0;
    D_0045C110[0x8a] = 0;
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" :: "r"(D_0045C300) : "memory");
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FF8A0);

INCLUDE_ASM(const s32, "game/code_002DC138", effLoadFileSlotAndPoll);

INCLUDE_ASM(const s32, "game/code_002DC138", effRunWithStateBackup);

extern u32 func_002FF8A0(u8 *, s32);

u32 func_002FFE48(void) {
    u8 *file = fileQueueGetAt(D_004386B8, func_002FCA40());
    u32 result;
    memcpy(D_0045C110, file, 0x90);
    result = func_002FF8A0(D_0045C110, 1);
    memcpy(file, D_0045C110, 0x90);
    if (result & 1) {
        result |= 0x800000;
    }
    return result;
}

extern u8 D_00439074;

void effResetFileResources(void) {
    D_003FFA84[0] = 0;
    D_00439074 = 0;
    D_004386B0 = 0;
    if (D_004386B8 != 0) {
        fileQueueDestroy(D_004386B8);
        D_004386B8 = 0;
    }
    if (D_004386B4 != 0) {
        fileQueueDestroy(D_004386B4);
        D_004386B4 = 0;
    }
    if (D_004386C4 != 0) {
        fileJobDestroy(D_004386C4);
        D_004386C4 = 0;
    }
}

void func_003000B8(void) {
    if (D_00438760 != 0) {
        func_0020DF68(D_00438760);
        D_00438760 = 0;
    }
    if (D_0043875C != 0) {
        func_0020D8F0(D_0043875C);
        D_0043875C = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00300100);

extern s32 D_00438764;

INCLUDE_ASM(const s32, "game/code_002DC138", effInitializeResourceQueue);

u32 effPollResourceQueue(void) {
    u32 result;
    func_0020E380(D_00438764);
    func_0020E828(D_00438764, fileQueueGetAt(D_004386B8, func_002FCA40()) + 0x9C);
    result = func_0020E7B0(D_00438764);
    if ((u32)(result - 1) < 2) {
        func_0020E368(D_00438764);
        D_00438764 = 0;
        return 0;
    }
    return 0x200001;
}

void func_003002A8(void) {
    if (D_00438768 != 0) {
        func_0020E368(D_00438768);
        D_00438768 = 0;
    }
}

void effQueueResource(s32 unused, s32 entry) {
    s32 queue = D_00438768;
    if (queue == 0) {
        queue = func_0020E300();
        D_00438768 = queue;
        func_0020E7A0(queue, 0xC2, 0xC8);
    }
    func_0020E7B8(D_00438768, entry);
}

INCLUDE_ASM(const s32, "game/code_002DC138", effUpdateResourceQueue);

void func_003003E0(void) {
    if (D_00438760 != 0) {
        func_0020DF68(D_00438760);
        D_00438760 = 0;
    }
    if (D_0043875C != 0) {
        func_0020D8F0(D_0043875C);
        D_0043875C = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", effPollResourceBank);

void effResetMappingFlags(void) {
    D_0043876C = 0;
    D_00438778 = 0;
    D_00438770 = 0;
    D_00438774 = 1;
    D_00400018[3] = 1;
    D_00400018[4] |= 0x10;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00300578);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003012B0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00301318);

void func_00301380(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0xb8));
}

extern u8 D_00459E30[] __attribute__((aligned(4)));

void func_003013A0(void) {
    func_00300578(D_00459E30, D_00459E30 + 0x24, *(u32 *)(D_00459E30 + 0x34));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003013C8);

void func_00301428(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

void func_00301448(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x38));
}

void func_00301468(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00301488);

void func_003014E8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

void func_00301508(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00301528);

void func_00301590(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

void func_003015B0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0 + 0x3c, temp_v0 + 0x60, *(u32 *)(temp_v0 + 0x80));
}

void func_003015D8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

void func_003015F8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0 + 0x3c, temp_v0 + 0x60, *(u32 *)(temp_v0 + 0x80));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00301620);

void func_00301688(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x70));
}

void func_003016A8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

void func_003016C8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0 + 0x50, temp_v0 + 0x74, *(u32 *)(temp_v0 + 0x84));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003016F0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00301750);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003017B8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00301820);

void btlResetEffectWork(void) {
    u8 *first = D_00400150;
    u8 *second = D_00400250;

    *(u32 *)(first + 0x10) |= 0x10;
    *(u32 *)(first + 0x0c) = 0;
    *(u32 *)(second + 0x10) |= 0x10;
    *(u32 *)(second + 0x0c) = 0;
    D_004387A8 = 0;
    D_004387AC = 0;
    D_004387B0 = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003018C8);

extern u8 D_00400068[];
extern void func_003018C8(void *, s32, const void *, const void *);

void func_003021A8(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0x60, *(s32 *)(source + 0xB8), D_00400150, D_00400068);
}

extern u8 D_004001A0[];

void func_003021D8(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0x8C, *(s32 *)(source + 0xB8), D_00400250, D_004001A0);
}

void func_00302208(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0x7C, *(s32 *)(source + 0xD4), D_00400150, D_00400068);
}

extern u8 D_00400250[];


void func_00302238(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0xA8, *(s32 *)(source + 0xD4), D_00400250, D_004001A0);
}

void func_00302268(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0x34, *(s32 *)(source + 0xB8), D_00400150, D_00400068);
}

void func_00302298(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0x60, *(s32 *)(source + 0xB8), D_00400150, D_00400068);
}

void func_003022C8(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0x8C, *(s32 *)(source + 0xB8), D_00400150, D_00400068);
}

void func_003022F8(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0x34, *(s32 *)(source + 0x8C), D_00400150, D_00400068);
}

void func_00302328(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0x60, *(s32 *)(source + 0x8C), D_00400150, D_00400068);
}

void func_00302358(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0x80, *(s32 *)(source + 0xD8), D_00400150, D_00400068);
}

void func_00302388(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0xAC, *(s32 *)(source + 0xD8), D_00400250, D_004001A0);
}

extern u8 D_004000B8[];

void func_003023B8(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0x90, *(s32 *)(source + 0x80), D_00400150, D_004000B8);
}

void func_003023E8(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0x9C, *(s32 *)(source + 0xF4), D_00400150, D_00400068);
}

void func_00302418(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0xC8, *(s32 *)(source + 0xF4), D_00400250, D_004001A0);
}

void func_00302448(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source, *(s32 *)(source + 0xF4), D_00400150, D_004000B8);
}

void func_00302478(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0x34, *(s32 *)(source + 0x70), D_00400150, D_004000B8);
}

void func_003024A8(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0x34, *(s32 *)(source + 0x8C), D_00400150, D_00400068);
}

void func_003024D8(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0x60, *(s32 *)(source + 0x8C), D_00400150, D_00400068);
}

void func_00302508(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0x7C, *(s32 *)(source + 0xD4), D_00400150, D_00400068);
}

void func_00302538(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0xA8, *(s32 *)(source + 0xD4), D_00400250, D_004001A0);
}

void func_00302568(s32 *work) {
    u8 *source = (u8 *)work[3];
    func_003018C8(source + 0xB0, *(s32 *)(source + 0x108), D_00400150, D_00400068);
}

void func_00302598(void) {
    D_00438774 = 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003025A8);

void func_00302AD0(s32 arg0) {
    func_003025A8(*(s32 *)(arg0 + 0xc) + 0x3c, *(u32 *)(*(s32 *)(arg0 + 0xc) + 0x4c));
}

void func_00302AF0(s32 arg0) {
    func_003025A8(*(s32 *)(arg0 + 0xc) + 0x60, *(s32 *)(*(s32 *)(arg0 + 0xc) + 0x3c) + 1);
}

void func_00302B18(s32 arg0) {
    func_003025A8(*(s32 *)(arg0 + 0xc) + 0x70, *(s32 *)(*(s32 *)(arg0 + 0xc) + 0x8c) + 1);
}

void func_00302B40(s32 arg0) {
    func_003025A8(*(s32 *)(arg0 + 0xc) + 0x70, *(s32 *)(*(s32 *)(arg0 + 0xc) + 0x8c) + 1);
}

void func_00302B68(s32 arg0) {
    func_003025A8(*(s32 *)(arg0 + 0xc) + 0x60, *(s32 *)(*(s32 *)(arg0 + 0xc) + 0x74) + 1);
}

void func_00302B90(s32 arg0) {
    func_003025A8(*(u32 **)(arg0 + 0xc) + 4, **(u32 **)(arg0 + 0xc));
}

extern u32 D_004386C8;

s32 effPollGeneralResource(s32 mode) {
    u8 status[0xE0];
    u8 *entry;
    s32 result = 0x600001;

    effPollResourceBank(mode, status);
    if (*(s32 *)(status + 0xCC) == 2) {
        result = 0x400000;
    } else if (*(s32 *)(status + 0xCC) == 1) {
        if (*(s32 *)(status + 0xC8) != 8) {
            entry = fileQueueGetAt(D_004386B8, -*(s32 *)(status + 0xD0));
            if (D_004386C8 != (u32)entry) {
                func_002D4E60(D_004386B8, D_004386C8);
                func_002D4CF0(D_004386B8, D_004386C8, entry);
            }
        } else {
            func_002D4E60(D_004386B8, D_004386C8);
            func_002D39C8(D_004386C0, status + 0xD0, 4, 4);
        }
        result = 0x400002;
    }
    return result;
}

void func_00302C78(void) {
    effPollGeneralResource(0x4b);
}

void func_00302C90(void) {
    effPollGeneralResource(0x4b);
}

void func_00302CA8(void) {
    effPollGeneralResource(0xb);
}

s32 effPollNamedResource(const char *resourceName, s32 mode) {
    u8 status[0x110];
    s32 result = 0x600001;

    func_00300100(resourceName, mode, status);
    if (*(s32 *)(status + 0x100) == 2) {
        result = 0x400000;
    } else if (*(s32 *)(status + 0x100) == 1) {
        func_002D4E60(D_004386B8, D_004386C8);
        if (*(s32 *)(status + 0xFC) != 8) {
            func_002D3A68(D_004386C0, status,
                           func_00303C78(*(s32 *)(status + 0xFC)));
        } else {
            func_002D39C8(D_004386C0, status + 0x104, 4, 4);
        }
        result = 0x400002;
    }
    return result;
}
extern char D_0042D128[]; /* "/tool/effect/mat/" */
extern char D_0042CF70[]; /* "/tool/effect/" */
extern char D_0042D140[]; /* "/tool/effect/hlp/" */

void func_00302D70(void) {
    effPollNamedResource(D_0042D128, 0x43);
}

void func_00302D90(void) {
    effPollNamedResource(D_0042D128, 0x43);
}

void func_00302DB0(void) {
    effPollNamedResource(D_0042CF58, 0x20);
}

void func_00302DD0(void) {
    effPollNamedResource(D_0042CF70, 0x10);
}

void func_00302DF0(void) {
    effPollNamedResource(D_0042CF58, 0x20);
}

void func_00302E10(void) {
    effPollNamedResource(D_0042CF70, 0x10);
}

void func_00302E30(void) {
    effPollNamedResource(D_0042D128, 1);
}

void func_00302E50(void) {
    effPollNamedResource(D_0042D128, 1);
}

void func_00302E70(void) {
    effPollNamedResource(D_0042D128, 1);
}

u32 func_00302E90(void) {
    u32 zero = 0;
    func_002D39C8(D_004386C0, &zero, 4, 4);
    return 0x400002;
}

void func_00302EC8(void) {
    effPollNamedResource(D_0042D140, 4);
}

void func_00302EE8(void) {
    effPollNamedResource(D_0042D128, 4);
}

void func_00302F08(void) {
    effPollNamedResource(D_0042CF70, 0x10);
}

void func_00302F28(void) {
    effPollNamedResource(D_0042D140, 4);
}

INCLUDE_ASM(const s32, "game/code_002DC138", fileLoadEffectSlotA);

typedef struct EffectFileHeader {
    u8 unk_00[8];
    u16 mode;
    u8 unk_0A[2];
    u32 start;
    u32 length;
} EffectFileHeader;
extern EffectFileHeader D_003FB948;
extern u8 D_0045C270[];
extern u32 D_004386C8;
extern u32 D_004386BC;
extern void func_00303C50(void);

extern u8 D_003F0DA8[] __attribute__((aligned(4)));

typedef struct EffFileQueryInfo {
    u8 pad0[0xFC];
    u32 resourceMask;
    s32 status;
    u8 pad104[0xC];
} EffFileQueryInfo;

typedef struct EffFileResourceRecord {
    char *name;
    u8 pad4[4];
    u16 mode;
    u8 padA[2];
    u8 *buffer;
    u32 size;
    u32 allocationHandle;
} EffFileResourceRecord;

typedef struct EffFileJobEntry {
    u8 pad0[0x90];
    u32 fileHandle;
    u8 pad94[8];
    char filename[1];
} EffFileJobEntry;

u32 func_00303130(void) {
    EffFileQueryInfo fileInfo;
    u32 result;
    s32 status;
    u8 *job;
    EffFileJobEntry *entry;
    EffFileResourceRecord *resource;
    u8 *buffer;
    u32 command;
    u32 totalLength;
    u32 dataLength;
    u32 allocation;

    func_00300100(D_0042D140, 4, &fileInfo);
    status = fileInfo.status;
    result = 0x600001;
    if (status == 2) {
        result = 0x400000;
    } else if (status == 1) {
        u32 headerBytes = 0x80;
        u32 oldAllocation;
        u32 queuedFile;
        job = (u8 *)fileCreateJob(6);
        command = sdfDevCreateCommandState(&fileInfo);
        dataLength = func_0033EB30(command);
        totalLength = dataLength + headerBytes;
        allocation = func_003292A8(totalLength);
        buffer = (u8 *)sdfResourceRetainAddress(allocation);
        memset(buffer, 0, headerBytes);
        func_0033EB10(command, buffer + headerBytes, dataLength);
        func_0033EAE0(command);
        func_002D3848(job, buffer, totalLength, 1);
        entry = (EffFileJobEntry *)fileAppendJob(D_004386B8, job);
        D_004386C8 = (u32)entry;
        resource = (EffFileResourceRecord *)func_002FC8F8(entry);
        strcpy(entry->filename, resource->name);
        memcpy(D_0045C270, entry, 0x80);
        queuedFile = entry->fileHandle;
        oldAllocation = resource->allocationHandle;
        D_004386C0 = queuedFile;
        if (oldAllocation != 0) {
            func_003297C8(oldAllocation);
        }
        resource->allocationHandle = allocation;
        resource->buffer = buffer;
        resource->size = dataLength + headerBytes;
        resource->mode = 1;
        memcpy(D_00459E30, D_003F0DA8, 0x74);
        D_004386BC = func_002FC340(resource);
        D_004386F4 = func_002FC9B8(entry);
        *(u8 **)(D_004386F4 + 0x34) = D_003FFA78;
        func_00303C50();
        if (D_004386C4 != 0) {
            fileJobDestroy(D_004386C4);
            D_004386C4 = 0;
        }
        result = 0x800002;
    }
    return result;
}

extern EffectFileHeader D_003F9060;

u32 func_00303478(void) {
    EffFileQueryInfo fileInfo;
    u8 *job;
    EffFileJobEntry *entry;
    EffFileResourceRecord *resource;
    void *fileData;
    s32 status;
    u32 result;

    func_00300100(D_0042D128, 4, &fileInfo);
    status = fileInfo.status;
    result = 0x600001;
    if (status == 2) {
        result = 0x400000;
    } else if (status == 1) {
        job = (u8 *)fileCreateJob(0x12);
        func_002D3848(job, D_003F9060.start, D_003F9060.length,
                      D_003F9060.mode);
        func_002D3A68(job, &fileInfo, func_00303C78(fileInfo.resourceMask));
        entry = (EffFileJobEntry *)fileAppendJob(D_004386B8, job);
        D_004386C8 = (u32)entry;
        memcpy(D_0045C270, entry, 0x80);
        D_004386C0 = entry->fileHandle;
        resource = (EffFileResourceRecord *)func_002FC8F8(entry);
        strcpy(entry->filename, resource->name);
        fileData = fileResolvePrimaryBuffer(D_004386C0);
        memcpy(resource->buffer, fileData, resource->size);
        D_004386BC = func_002FC340(resource);
        D_004386F4 = func_002FC9B8(entry);
        *(u8 **)(D_004386F4 + 0x34) = D_003FFA78;
        func_00303C50();
        if (D_004386C4 != 0) {
            fileJobDestroy(D_004386C4);
            D_004386C4 = 0;
        }
        result = 0x800002;
    }
    return result;
}

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D0B8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D0C8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D0D8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D0E8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D0F8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D108);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D118);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D128);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D140);

u32 func_00303660(void) {
    EffFileQueryInfo fileInfo;
    u8 *job;
    EffFileJobEntry *entry;
    EffFileResourceRecord *resource;
    void *fileData;
    s32 status;
    u32 result;

    func_00300100("/tool/effect/f2/", 0x80, &fileInfo);
    status = fileInfo.status;
    result = 0x600001;
    if (status == 2) {
        result = 0x400000;
    } else if (status == 1) {
        job = (u8 *)fileCreateJob(0x14);
        func_002D3848(job, D_003FB948.start, D_003FB948.length,
                      D_003FB948.mode);
        func_002D3A68(job, &fileInfo, func_00303C78(fileInfo.resourceMask));
        entry = (EffFileJobEntry *)fileAppendJob(D_004386B8, job);
        D_004386C8 = (u32)entry;
        memcpy(D_0045C270, entry, 0x80);
        D_004386C0 = entry->fileHandle;
        resource = (EffFileResourceRecord *)func_002FC8F8(entry);
        strcpy(entry->filename, resource->name);
        fileData = fileResolvePrimaryBuffer(D_004386C0);
        memcpy(resource->buffer, fileData, resource->size);
        D_004386BC = func_002FC340(resource);
        D_004386F4 = func_002FC9B8(entry);
        *(u8 **)(D_004386F4 + 0x34) = D_003FFA78;
        func_00303C50();
        if (D_004386C4 != 0) {
            fileJobDestroy(D_004386C4);
            D_004386C4 = 0;
        }
        result = 0x800002;
    }
    return result;
}

extern EffectFileHeader D_003FD988;

u32 effLoadMaterialFile(void) {
    EffFileQueryInfo fileInfo;
    u8 *job;
    EffFileJobEntry *entry;
    EffFileResourceRecord *resource;
    void *fileData;
    s32 status;
    u32 result;

    func_00300100(D_0042D128, 2, &fileInfo);
    status = fileInfo.status;
    result = 0x600001;
    if (status == 2) {
        result = 0x400000;
    } else if (status == 1) {
        job = (u8 *)fileCreateJob(0x16);
        func_002D3848(job, D_003FD988.start, D_003FD988.length,
                      D_003FD988.mode);
        func_002D3A68(job, &fileInfo, func_00303C78(fileInfo.resourceMask));
        entry = (EffFileJobEntry *)fileAppendJob(D_004386B8, job);
        D_004386C8 = (u32)entry;
        memcpy(D_0045C270, entry, 0x80);
        D_004386C0 = entry->fileHandle;
        resource = (EffFileResourceRecord *)func_002FC8F8(entry);
        strcpy(entry->filename, resource->name);
        fileData = fileResolvePrimaryBuffer(D_004386C0);
        memcpy(resource->buffer, fileData, resource->size);
        D_004386BC = func_002FC340(resource);
        D_004386F4 = func_002FC9B8(entry);
        *(u8 **)(D_004386F4 + 0x34) = D_003FFA78;
        func_00303C50();
        if (D_004386C4 != 0) {
            fileJobDestroy(D_004386C4);
            D_004386C4 = 0;
        }
        result = 0x800002;
    }
    return result;
}

extern EffectFileHeader D_003FE040;

u32 func_00303A30(void) {
    EffFileQueryInfo fileInfo;
    u8 *job;
    EffFileJobEntry *entry;
    EffFileResourceRecord *resource;
    void *fileData;
    s32 status;
    u32 result;
    u32 index;
    f32 *coordinates;

    func_00300100(D_0042D128, 4, &fileInfo);
    status = fileInfo.status;
    result = 0x600001;
    if (status == 2) {
        result = 0x400000;
    } else if (status == 1) {
        job = (u8 *)fileCreateJob(0x19);
        coordinates = (f32 *)(D_003FE040.start + 0x20);
        for (index = 0; index < 0xFF; index++) {
            *coordinates++ = 1.0f;
        }
        func_002D3848(job, D_003FE040.start, D_003FE040.length,
                      D_003FE040.mode);
        func_002D3A68(job, &fileInfo, func_00303C78(fileInfo.resourceMask));
        entry = (EffFileJobEntry *)fileAppendJob(D_004386B8, job);
        D_004386C8 = (u32)entry;
        memcpy(D_0045C270, entry, 0x80);
        D_004386C0 = entry->fileHandle;
        resource = (EffFileResourceRecord *)func_002FC8F8(entry);
        strcpy(entry->filename, resource->name);
        fileData = fileResolvePrimaryBuffer(D_004386C0);
        memcpy(resource->buffer, fileData, resource->size);
        D_004386BC = func_002FC340(resource);
        D_004386F4 = func_002FC9B8(entry);
        *(u8 **)(D_004386F4 + 0x34) = D_003FFA78;
        func_00303C50();
        if (D_004386C4 != 0) {
            fileJobDestroy(D_004386C4);
            D_004386C4 = 0;
        }
        result = 0x800002;
    }
    return result;
}

void func_00303C50(void) {
    D_0043876C = 0;
    D_00438774 = 1;
    D_00438770 = 0;
    D_00438778 = 0;
    D_004387A8 = 0;
    D_004387AC = 0;
    D_004387B0 = 0;
}

s32 func_00303C78(s32 mask) {
    switch (mask) {
    case 1: return 1;
    case 2: return 2;
    case 4: return 3;
    case 8: return 4;
    case 0x20: return 5;
    case 0x10: return 6;
    case 0x40: return 7;
    case 0x80: return 8;
    default: return 0;
    }
}

void *func_00303D00(u32 value) {
    u8 *record = (u8 *)func_00328D68(0x14);
    memset(record, 0, 0x14);
    *(u32 *)record = value;
    *(u32 *)(record + 4) = 0;
    *(u32 *)(record + 8) = 0;
    *(u32 *)(record + 0xC) = 0;
    return record;
}

void func_00303D58(void) {
    func_00328E48();
}

u32 func_00303D70(u32 *arg0) {
    return *arg0;
}

typedef struct EffectListNode {
    u32 flags;
    struct EffectListNode *next;
    u32 value;
    u32 length;
    u32 kind;
    u32 reference;
} EffectListNode;

typedef struct EffectList {
    u32 flags;
    s32 count;
    EffectListNode *first;
    EffectListNode *last;
} EffectList;

u32 effAppendListEntry(EffectList *list, u32 value, u32 length,
                          u32 kind, u32 reference) {
    EffectListNode *node = (EffectListNode *)func_00328D68(0x18);
    memset(node, 0, 0x18);
    node->next = 0;
    node->kind = kind;
    node->value = value;
    node->length = length;
    node->reference = reference;
    if (list->last == 0) {
        list->first = node;
        list->last = node;
    } else {
        list->last->next = node;
        list->last = node;
    }
    return ++list->count;
}

INCLUDE_ASM(const s32, "game/code_002DC138", effRemoveListEntry);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00303E88);

extern char D_004387E8[];

u32 effLoadIndexedResource(s32 category, s32 index, s32 preserve) {
    char path[0x80];
    u32 source;
    u32 handle;
    u32 resource;
    func_0035C860(path, D_004387E8, category, index);
    handle = func_00343ED0(path, &source, 0);
    resource = func_00305148(handle, preserve);
    if (preserve == 0) {
        func_003297C8(handle);
    }
    return resource;
}

void func_003040B8(u64 arg0, u32 *arg1) {
    u64 temp_v0;
    u32 temp_v1;

    temp_v0 = fileGetResourceHandle();
    temp_v1 = func_00305148(temp_v0, 0);
    *arg1 = temp_v1;
    func_003297C8(temp_v0);
    func_002C7D00(arg0);
}

void func_00304110(u64 arg0, u32 *arg1) {
    u64 temp_v0;
    u32 temp_v1;

    temp_v0 = fileGetResourceHandle();
    temp_v1 = func_00305148(temp_v0, 1);
    *arg1 = temp_v1;
    func_002C7D00(arg0);
}

void effRequestResourceByMode(s32 category, s32 index, s32 mode, u32 *result) {
    char path[0x80];
    func_0035C860(path, D_004387E8, category, index);
    *result = 0;
    if (mode == 1) {
        func_002C8040(path, 0, func_00304110, result);
    } else {
        func_002C8040(path, 0, func_003040B8, result);
    }
}

u32 effLoadMappedResource(s32 category, s32 index) {
    char path[0x80];
    u32 buffer;
    u32 handle;
    u32 resource;
    func_0035C860(path, D_004387E8, category, index);
    handle = func_00343ED0(path, &buffer, 0);
    resource = func_00304938(buffer);
    func_003297C8(handle);
    return resource;
}

void func_00304258(u64 arg0, u32 *arg1) {
    u64 temp_v0;
    u32 temp_v1;
    u32 temp_v2;

    temp_v0 = fileGetResourceHandle();
    temp_v1 = sdfResourceRetainAddress(temp_v0);
    temp_v2 = func_00304938(temp_v1);
    *arg1 = temp_v2;
    func_003297C8(temp_v0);
    func_002C7D00(arg0);
}

void effRequestMappedResource(s32 category, s32 index, u32 *result) {
    char path[0x80];
    func_0035C860(path, D_004387E8, category, index);
    *result = 0;
    func_002C8040(path, 0, func_00304258, result);
}

void *func_00304310(u32 value) {
    u8 *record = (u8 *)func_00328D68(0x44);
    memset(record, 0, 0x44);
    *(u32 *)record = value;
    return record;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304360);

typedef struct EffectRecordNode {
    u32 value;
    u32 index;
    struct EffectRecordNode *previous;
    struct EffectRecordNode *next;
} EffectRecordNode;

INCLUDE_ASM(const s32, "game/code_002DC138", func_003043E8);

void effReleaseRecordBuckets(u8 *buckets) {
    EffectRecordNode **head = (EffectRecordNode **)(buckets + 4);
    s32 index;
    for (index = 0xF; index >= 0; index--, head++) {
        EffectRecordNode *node = *head;
        while (node != 0) {
            if (node->previous != 0) {
                node->previous->next = node->next;
            }
            if (node->next != 0) {
                node->next->previous = node->previous;
            }
            if (node == *head) {
                *head = node->next;
            }
            func_00328E48(node);
            node = node->next;
        }
    }
}

s32 effDispatchRecordBuckets(s32 refresh, u8 *buckets, s32 context) {
    EffectRecordNode **head = (EffectRecordNode **)(buckets + 4);
    s32 index;
    for (index = 0xF; index >= 0; index--, head++) {
        EffectRecordNode *node = *head;
        while (node != 0) {
            func_00306F80(0, 0, 0, 0, *(u32 *)buckets, node->index, context);
            if (refresh != 0) {
                func_00307160(*(u32 *)buckets, node->index);
            }
            node = node->next;
        }
    }
    return 1;
}

u32 func_003045E8(u32 arg0) {
    effReleaseRecordBuckets((u8 *)arg0);
    func_00328E48(arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304618);

extern u8 D_00400508[];

INCLUDE_ASM(const s32, "game/code_002DC138", effSumRecordStatuses);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304768);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304938);

u32 *func_00304998(u32 category) {
    u32 *batch = (u32 *)func_00328D68(0xC);
    u32 allocation;
    u32 amount;
    u8 *statuses;

    batch[0] = 1;
    allocation = func_003292A8(0x24);
    batch[1] = allocation;
    batch[2] = sdfResourceRetainAddress(allocation);
    memset((void *)batch[2], 0, 0x24);
    {
        u8 *buffer = (u8 *)batch[2];
        *(u32 *)(buffer + 0x14) = category;
        amount = effSumRecordStatuses(buffer);
    }
    statuses = (u8 *)func_00328D68(amount);
    *(u8 **)(batch[2] + 0x20) = statuses;
    memset(statuses, 0, amount);
    *(u32 *)(batch[2] + 0x18) = amount;
    return batch;
}

INCLUDE_ASM(const s32, "game/code_002DC138", effDestroyPackedBatch);

u32 func_00304AB8(s32 arg0) {
    func_003297C8(*(u32 *)(arg0 + 0x14));
    return 1;
}

s32 func_00304AD8(s32 arg0, s32 arg1) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = arg1 * 0xa0 + *(s32 *)(arg0 + 0x18);
    temp_v0 = *(s32 *)(temp_v1 + 0x9c);
    if (temp_v0 != 0) {
        temp_v1 = temp_v0;
    }
    return temp_v1;
}

u32 func_00304AF8(u32 *state) {
    u32 active = state[0] & 1;
    if (active != 0) {
        state[1] = 0;
    } else {
        active = 0x10000;
        state[1] = active;
    }
    return active;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304B18);

void func_00304CE0(s32 arg0, s32 arg1) {
    func_00304B18(arg0, arg1, *(s32 *)(arg0 + 0x18) + arg1 * 0xa0);
}

extern void func_00304D80(u32, u32);

void func_00304D08(u8 *owner) {
    u32 index = 0;
    if (*(u32 *)(owner + 8) != 0) {
        do {
            func_00304D80((u32)owner, index++);
        } while (index < *(u32 *)(owner + 8));
    }
}

void func_00304D60(a, b, obj)
    u32 a;
    u32 b;
    u32 *obj;
{
    obj[0x60 / 4] = a;
    obj[0x64 / 4] = b;
    func_00304B18(a, b, obj);
}

void func_00304D80(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)((s32)arg0 + 0x18) + (s32)arg1 * 0xa0;
    memset(temp_v0, 0, 0xa0);
    func_00304D60(arg0, arg1, temp_v0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304DE8);

void effResolveAndReleaseResource(u32 *owner) {
    if (owner[0] != 0) {
        u32 resource = owner[0];
        u32 mapped = sdfResourceRetainAddress(resource);
        func_00304DE8(owner, mapped, 0, -1);
        func_00329910(owner[0]);
    }
}

void effResolveAndReleaseSelectedResource(u32 *owner, s32 mapping) {
    if (owner[0] != 0) {
        u32 resource = owner[0];
        u32 mapped = sdfResourceRetainAddress(resource);
        func_00304DE8(owner, mapped, 0, mapping);
        func_00329910(owner[0]);
    }
}

extern void sdfTexReleaseReference(s32, u32, u32);

void func_00304FB0(u8 *owner, s32 preserve) {
    u32 i = 0;
    u32 count = *(u32 *)(owner + 0x1C);
    u32 *resources;

    if (count != 0) {
        resources = *(u32 **)(owner + 0x24);
        do {
            if (resources[i] != 0) {
                u32 *current;
                sdfTexReleaseReference(resources[i], (u32)resources, count);
                current = *(u32 **)(owner + 0x24);
                count = *(u32 *)(owner + 0x1C);
                resources = current;
                current[i] = 0;
            }
            i++;
        } while (i < count);
    }
    if (!preserve) {
        func_00304D08(owner);
    }
}

void func_00305068(u32 arg0) {
    func_00304FB0(arg0, 0);
}

u8 func_00305080(s32 arg0) {
    return **(s32 **)(arg0 + 0x24) != 0;
}

u32 *effCreatePayload(u32 count) {
    u32 bytes = count * 0x6C;
    u32 *payload = (u32 *)func_00328D68(0xC);
    u32 allocation = func_003292A8(bytes);
    payload[1] = count;
    payload[0] = allocation;
    payload[2] = sdfResourceRetainAddress(allocation);
    memset((void *)payload[2], 0, bytes);
    return payload;
}

u32 func_00305110(u32 arg0) {
    func_003297C8(*(u32 *)arg0);
    func_00328E48(arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00305148);

u32 *effCreateResourceSlotSet(u32 *source, u32 slot, u32 count) {
    u32 *effect = (u32 *)func_00328D68(0x30);
    u32 index = 0;
    effect[1] = 1;
    {
        u32 mode = source[7];
        u32 size = source[9];
        effect[7] = mode;
        effect[9] = size;
    }
    effect[0] = 0;
    effect[8] = 0;
    effect[2] = count;
    effect[3] = func_003292A8(count * 0x80);
    effect[4] = sdfResourceRetainAddress(effect[3]);
    effect[5] = func_003292A8(effect[2] * 0xA0);
    effect[6] = sdfResourceRetainAddress(effect[5]);
    if (effect[2] != 0) {
        do {
            memcpy((void *)(effect[4] + index * 0x80),
                   (void *)(source[4] + slot * 0x80), 0x80);
            func_00304D80((u32)effect, index);
            index++;
        } while (index < effect[2]);
    }
    return effect;
}

u32 func_003054E8(u32 arg0) {
    s32 *piVar1;

    piVar1 = (s32 *)arg0;
    if (*piVar1 != 0) {
        func_003297C8(*piVar1);
    }
    if (piVar1[1] == 0) {
        func_00305068(arg0);
        func_003297C8(piVar1[8]);
    }
    func_003297C8(piVar1[3]);
    func_00304AB8(arg0);
    func_00328E48(arg0);
    return 1;
}

u32 func_00305558(u32 *arg0, u32 arg1, u32 arg2) {
    *arg0 = arg2;
    arg0[4] = arg1;
    if ((arg2 & 2) != 0) {
        func_00304AF8(arg0);
    }
    arg0[2] = arg0[2] + 1;
    return 1;
}

u32 func_003055A0(u32 arg0, s32 arg1, s32 arg2, u32 arg3) {
    func_00305558(arg0, *(s32 *)(arg1 + 8) + arg2 * 0x24, arg3);
    return 1;
}

u32 func_003055D8(s32 arg0) {
    *(u32 *)(arg0 + 0x10) = 0;
    return 1;
}

s32 func_003055E8(u32 effect, u32 slot, u32 *state) {
    if (0x10000 < (s32)state[1]) {
        u32 flags = state[0];
        state[1] = 0x10000;
        if (flags & 4) {
            if (flags & 8) {
                state[0] = flags & ~1;
            } else {
                func_00304CE0(effect, slot);
            }
            return 0;
        }
    }
    return 1;
}

s32 func_00305650(u32 effect, u32 slot, u32 *state) {
    if ((s32)state[1] < 0) {
        u32 flags = state[0];
        state[1] = 0;
        if (flags & 4) {
            if (flags & 8) {
                state[0] = flags | 1;
            } else {
                func_00304CE0(effect, slot);
            }
            return 0;
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003056B0);

s32 func_00305848(u8 *effect, u32 slot, u32 material) {
    u32 offset = slot * 0xA0;
    if (*(u32 *)(offset + *(u32 *)(effect + 0x18) + 0x9C) == 0) {
        func_00304D60(effect, slot);
    }
    *(u32 *)(offset + *(u32 *)(effect + 0x18) + 0x9C) = material;
    return 1;
}

u32 effSetMaterialSlots(s32 work, s32 index, u32 value, BdWork *asset) {
    s32 offset = index * 0xA0;
    u32 i;
    EffectMaterialSlot *slots;
    if (*(void **)(offset + *(s32 *)(work + 0x18) + 0x9C) == NULL) {
        func_00304D60((void *)work, index, asset);
    }
    *(BdWork **)(offset + *(s32 *)(work + 0x18) + 0x9C) = asset;
    slots = (EffectMaterialSlot *)((u8 *)asset + 0x30);
    for (i = 0; i < 2; i++) {
        slots[i].value = value;
    }
    return 1;
}

u32 func_00305950(s32 arg0, s32 arg1) {
    *(u32 *)(arg1 * 0xa0 + *(s32 *)(arg0 + 0x18) + 0x9c) = 0;
    return 1;
}

s32 func_00305970(u8 *effect, u32 slot, u32 resource, u32 flags) {
    u8 *entry = *(u8 **)(effect + 0x18) + slot * 0xA0;
    func_00305558((u32 *)(entry + 0x28), resource, flags);
    func_003056B0(effect, slot, entry);
    return 1;
}

s32 func_003059E0(u8 *effect, u32 slot, u8 *resources, u32 index, u32 flags) {
    u8 *entry = *(u8 **)(effect + 0x18) + slot * 0xA0;
    u32 resource = *(u32 *)(resources + 8) + index * 0x24;
    func_00305558((u32 *)(entry + 0x28), resource, flags);
    func_003056B0(effect, slot, entry);
    return 1;
}

s32 func_00305A60(u8 *effect, u32 slot, u8 *resources, u32 index,
                  u32 flags, u32 option, u32 color) {
    u8 *entry = *(u8 **)(effect + 0x18) + slot * 0xA0;
    u32 resource = *(u32 *)(resources + 8) + index * 0x24;
    func_00305558((u32 *)(entry + 0x28), resource, color);
    func_003056B0(effect, slot, entry);
    *(u32 *)(entry + 0x30) = flags;
    *(u32 *)(entry + 0x34) = option;
    return 1;
}

u32 effConfigureWithDefaultSetting(u32 effect, u32 slot, u32 kind, u32 value, u32 flags, u32 color) {
    func_00305A60(effect, slot, kind, value, flags, 0, color);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00305B28);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00305BD0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00305C40);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00305EB0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00306030);

extern void func_00306030(u32, u32, u32, u32, u32, u32, u32, u32,
                          u32, u32, u32, u32, u32);

void func_003064C0(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g, u32 h,
                   u32 x, u32 y, u32 width, u32 height) {
    func_00306030(a, b, c, d, e, f, g, h, x, y, 1, width, height);
}

void func_00306500(u32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4, u32 arg5, u32 arg6, u32 arg7) {
    func_00305BD0(arg6, arg7);
    func_003089B8(arg0, arg1, arg2, arg3, arg4, arg5, arg7);
    func_00308478(0x44, arg7);
}

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D170);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D180);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D190);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D1A0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D1B0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D1C0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D1D0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D1E0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D1F0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E2C);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E30);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E34);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E38);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E3C);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E40);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E48);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E50);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E54);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E58);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E5C);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E60);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E64);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E68);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E6C);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E70);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E74);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E78);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E7C);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E80);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E84);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E88);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E90);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E94);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E98);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EA0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EA8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EB0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EB4);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EB8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EC0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EC8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437ED0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437ED8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EE0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EE8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EF0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EF8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F00);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F08);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F10);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F18);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F20);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F28);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F30);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F38);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F40);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F48);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F50);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F58);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F60);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F68);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F70);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F78);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F80);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F88);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F90);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F98);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FA0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FA8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FB0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FB8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FC0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FC8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FD0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FD8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FE0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FE8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FF0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FF8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438000);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438008);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438010);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438018);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438020);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438028);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438030);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438038);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438040);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438048);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438050);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438058);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438060);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438068);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438070);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438078);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438080);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438088);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438090);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438098);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380A0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380A8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380B0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380B8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380C0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380C8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380D0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380D8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380E0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380E8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380F0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380F8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438100);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438108);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438110);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438118);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438120);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438128);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438130);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438138);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438140);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438148);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438150);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438158);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438160);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438168);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438170);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438178);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438180);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438188);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438190);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438198);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381A0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381A8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381B0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381B8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381C0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381C8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381D0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381D8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381E0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381E8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381F0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381F8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438200);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438208);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438210);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438218);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438220);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438228);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438230);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438238);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438240);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438248);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438250);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438258);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438260);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438268);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438270);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438278);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438280);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438288);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438290);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438298);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382A0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382A8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382B0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382B8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382C0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382C8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382D0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382D8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382E0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382E8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382F0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382F8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438300);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438308);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438310);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438318);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438320);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438328);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438330);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438338);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438340);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438348);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438350);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438358);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438360);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438368);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438370);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438378);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438380);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438388);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438390);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438398);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383A0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383A8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383B0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383B8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383C0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383C8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383D0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383D8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383E0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383E8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383F0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383F8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438400);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438408);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438410);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438418);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438420);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438428);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438430);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438438);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438440);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438448);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438450);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438458);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438460);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438468);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438470);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438478);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438480);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438488);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438490);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438498);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384A0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384A8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384B0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384B8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384C0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384C8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384D0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384D8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384E0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384E8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384F0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384F8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438500);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438508);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438510);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438518);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438520);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438528);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438530);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438538);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438540);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438548);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438550);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438558);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438560);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438568);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438570);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438578);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438580);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438588);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438590);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438598);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385A0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385A8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385B0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385B8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385C0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385C8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385D0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385D8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385E0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385E8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385F0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385F8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438600);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438608);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438610);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438618);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438620);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438628);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438630);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438638);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438640);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438644);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438648);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438650);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438658);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438660);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438668);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438670);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438678);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438680);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438688);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438690);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438698);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386A0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386A8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386B0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386B4);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386B8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386BC);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386C0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386C4);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386C8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386CC);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386D0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386D8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386E0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386E8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386F0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386F4);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386F8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438700);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438708);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438710);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438718);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438720);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438728);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438730);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438738);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438740);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438748);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438750);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438758);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438759);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_0043875A);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_0043875B);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_0043875C);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438760);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438764);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438768);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_0043876C);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438770);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438774);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438778);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_0043877C);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438780);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438788);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438790);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438798);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387A0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387A8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387AC);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387B0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387B4);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387B8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387BC);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387C0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387C8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387D0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387D8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387E0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387E8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387F0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387F8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438800);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438808);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438810);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438818);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438820);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438828);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438830);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438838);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438840);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438848);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438850);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438858);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438860);

