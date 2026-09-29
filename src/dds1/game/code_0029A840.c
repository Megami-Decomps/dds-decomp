#include "common.h"
#include "ee_mmi.h"

#include "pcp_vu0.h"

extern void func_002BD640(u32, u32);

extern char D_003B39C8[]; /* "/tool/effect/ep/" */

extern char D_003B39E0[]; /* "/tool/effect/" */

extern char D_003B3B88[]; /* "/tool/effect/mat/" */

extern char D_003B3BA0[]; /* "/tool/effect/hlp/" */

extern u32 func_002BD1F8(u32);

extern u32 sdfResourceRetainAddress(u32);

extern u32 fileGetResourceHandle(void);

extern u32 func_002BD9C0(u32, u32);

extern u32 D_003BD11C;

extern u32 D_003BD120;

extern u32 D_003BD128;

extern u32 D_003BD158;

extern u32 D_003BD15C;

extern u32 D_003BD160;

extern u32 D_003BD124;

extern s32 D_003BD118;

extern s32 D_003BD10C;

extern s32 D_003BD110;

extern s32 D_003BD098;

extern s32 D_003BD09C;

extern u8 D_003BD955;

extern s32 D_003BD06C;

extern s32 D_003BD05C;

extern s32 func_001A17F0(void);

extern u32 func_002B3390(u32, u32, u32);

extern void billDispatchByKind(void *);

extern u32 func_002B0988();

extern u32 D_003BC998;

extern s32 D_003BC99C;

extern u32 D_003BC9A0;

extern u32 func_0029BF88(u32, u32);

extern u32 D_003BC988;

extern u32 D_003BC984;

extern u32 D_003BC994;

extern u32 func_002EB028(const char *, u32 *, s32);

extern u8 D_0038E000[];

extern u8 D_0038E570[];

extern u8 *D_0037EBF8[];

extern u32 D_003BD068;

extern u32 D_003BC980;

extern s32 D_003BC98C;

extern u32 D_003BC990;

extern u32 func_0029C230(u32);

extern u32 D_003BC968;

extern u32 D_003BC96C;

extern u32 func_00151E60(u32);

extern void *fileResolvePrimaryBuffer();

extern u32 *fileResolveSecondaryBuffer(void *);

extern u32 func_002B1F58(u16, void *, void *, u32);

extern u32 createEffectResourceInstance(u16, void *, void *, u32);

extern void func_002D2D00(u32);

extern u32 D_003BC950;

extern u32 D_003BC954;

extern s32 *D_003BC948;

extern s64 func_001A1438(void);

extern s64 btlIsCurrentActorFullyMarked(void);

extern u32 func_0029A958();

/* 4x4 float matrix with 128-bit row access for VU0/DMA transfers. */
typedef struct Matrix4 {
    union {
        float m[4][4];
        s128 rows[4];
    } u; // 0x00
} Matrix4; // 0x40

extern u32 D_003BD058;

extern u32 D_0038F2FC[];

/* Reference-counted object header (layout inferred from field accesses). */
typedef struct RefObj {
    u8 pad_0x00[0x14]; // 0x00
    s32 cnt14;         // 0x14
    s32 unk18;         // 0x18
    s32 cnt1C;         // 0x1C
} RefObj; // 0x20

extern s32 D_003BC970[2];

extern RefObj *D_003BC978[2];

/* Battle/display work object (layout inferred from field accesses). */
typedef struct BdWork {
    s32 x0;            // 0x00
    s32 x4;            // 0x04
    u8 pad_0x08[0x58]; // 0x08
    void *x60;         // 0x60
    s32 x64;           // 0x64
} BdWork; // 0x68

extern void func_002DB538(void *, float);

extern u32 D_003BC94C;

extern void func_002BD3D8(void *, s32, void *);

extern u32 D_003BC9B0[2];

extern u32 D_003BC9B8[2];

extern u8 D_003BC9C0[2];

extern u8 D_003BC9C8[2];

typedef struct FnTbl20 {
    void (*fn)(void *);
    u8 pad_0x04[0x10]; // 0x04
} FnTbl20; // 0x14

extern FnTbl20 D_0037E774[];

extern FnTbl20 D_0037E7EC[];

extern FnTbl20 D_0037E778[];

extern FnTbl20 D_0037E7F0[];

extern FnTbl20 D_0037E7F4[];

/* Function-pointer tables indexed by object fields (entry size inferred). */
typedef struct FnTbl28 {
    void (*fn)();
    u32 (*createResource)();
    u32 unk_08;
    u32 (*createActiveResource)(void *);
    u32 resourceSize;
    u8 pad_0x14[8]; // 0x14
} FnTbl28; // 0x1C

typedef struct FnTbl24 {
    void (*fn)();
    u8 pad_0x04[0x14]; // 0x04
} FnTbl24; // 0x18

typedef struct FnTbl24Create {
    void (*fn)();
    u32 (*createResource)();
    u8 pad_0x08[0x10]; // 0x08
} FnTbl24Create; // 0x18

extern FnTbl28 D_0037E8A0[];

extern FnTbl28 D_0037E8A8[];

extern FnTbl28 D_0037E8B0[];

extern FnTbl24Create D_0037EAD0[];

extern FnTbl24 D_0037EAD8[];

extern FnTbl24 D_0037EADC[];

extern FnTbl24 D_0037EC50[];

extern FnTbl24 D_0037EC5C[];

extern FnTbl24 D_0037EC58[];

extern FnTbl28 D_0037ED18[];

extern FnTbl28 D_0037ED10[];

extern FnTbl28 D_0037ED08[];

extern FnTbl28 D_0037ED90[];

extern FnTbl28 D_0037ED98[];

extern FnTbl28 D_0037EEF4[];

extern FnTbl24Create D_0037EE38[];

extern FnTbl24 D_0037EE40[];

extern FnTbl24 D_0037EE44[];

extern void func_0029B168(s32 *);

extern FnTbl28 D_0037E8B4[];

extern FnTbl24 D_0037EAE0[];

extern FnTbl24 D_0037EC60[];

extern FnTbl28 D_0037ED1C[];

extern FnTbl28 D_0037EDA0[];

extern FnTbl28 D_0037EDA4[];

extern FnTbl24 D_0037EE48[];

/* Shared work object for the func_002BA538 helpers (layout inferred). */
typedef struct BaObj {
    u8 pad_0x00[0x0C]; // 0x00
    u8 *x0C;           // 0x0C
} BaObj; // 0x10

extern s32 func_002BA538(void *, s32, void *, void *);

extern u8 D_0038F9D0[];

extern u8 D_0038FAD0[];

extern u8 D_0038F8E8[];

extern u8 D_0038FA20[];

extern u8 D_0038F938[];

extern void func_002B9050(void);

extern void effQueueResource(void *, void *);

extern u32 func_001FC280();

extern void func_001FC720(u32, u32, u32);

extern void func_001FC738(u32, void *);

extern u8 D_003BD088[];

extern u8 D_003BD090[];

extern u8 D_003DF8D0[];

extern u8 D_0038F2B8[];

extern u8 D_003DF910[];

extern u8 *D_003BD074;

extern void mdlStoreTertiaryVectorVU(void *);

extern void *func_0029BD90(void *);

extern void func_002A6440(s32);

extern void func_002AB290(s32);

extern s32 func_002B7388(s32, void *, u32);

extern u8 D_003B3968[];

extern u8 D_003B3958[];

extern u8 D_003BCA40[];

extern u8 D_003BD050[];

extern u8 D_003B38C8[];

extern u8 D_003B3938[];

extern u8 D_003B3928[];

extern u8 D_003B3918[];

extern u8 D_003B3908[];

extern u8 D_003B38F8[];

extern u8 D_003B38E8[];

extern u8 D_003B38D8[];

extern u8 D_003B3888[];

extern u8 D_003BD000[];

extern u8 D_0038DE70[];

extern u8 D_0038DF50[];

extern u8 D_0038E0F0[];

extern u8 D_0038E1A0[];

extern u8 D_0038E280[];

extern u8 D_0038E240[];

extern u8 D_0038E310[];

extern u8 D_0038E450[];

extern u8 D_0038E500[];

extern u8 D_0038E620[];

extern u8 D_0038E6F0[];

extern u8 D_0038E7C0[];

extern u8 D_0038E800[];

extern u8 D_0038E9A0[];

extern u8 D_003DF840[];

extern u8 D_003DFA30[];

extern u8 D_003BD108;

extern u8 D_003BD109;

extern u8 D_003BD10A;

extern s32 func_002B9320(s32, s32, s32);

extern u8 D_003DE148[];

/* Value holder with a float field and a u16 data pointer (layout inferred). */
typedef struct ValPtr44 {
    u8 pad_0x00[0x08]; // 0x00
    float f08;         // 0x08
    u8 pad_0x0C[0x38]; // 0x0C
    u16 *p44;          // 0x44
} ValPtr44; // 0x48

typedef struct ValPtr34 {
    u8 pad_0x00[0x08]; // 0x00
    float f08;         // 0x08
    u8 pad_0x0C[0x28]; // 0x0C
    u16 *p34;          // 0x34
} ValPtr34; // 0x38

extern s32 pollEffectFileRecord(char *, s32);

extern void dds3DispatchIndexedCallback(u16 *, float);

extern void mdlStorePrimaryVectorVU(void *);

extern void func_00217FB8(void *);

/* Grid-sized effect object: width/height pair with an alternate pair at 0xB8. */
typedef struct EffGrid {
    u8 pad_0x00[0x20]; // 0x00
    s32 width;         // 0x20
    s32 height;        // 0x24
    u8 pad_0x28[0x90]; // 0x28
    s32 altHeight;     // 0xB8
} EffGrid; // 0xBC

/* Effect kind descriptor tables (0x14-byte entries, layout inferred from field accesses). */
typedef struct EffKindDesc {
    u32 (*create)(void *);   // 0x00
    u8 pad_0x04[8];          // 0x04
    void (*apply)(); // 0x0C
    u32 size;                // 0x10
} EffKindDesc; // 0x14

/* Effect work created from a kind descriptor: 0x40-byte header followed by a copy of the source payload. */
typedef struct EffKindWork {
    u8 vec[0x10];   // 0x00
    s32 mode;       // 0x10
    u32 color;      // 0x14
    f32 scale;      // 0x18
    s32 kind;       // 0x1C
    u8 pad_0x20[4]; // 0x20
    u32 handle;     // 0x24
    void *payload;  // 0x28
    u32 sourceKind; // 0x2C
    u32 target;     // 0x30
    u8 pad_0x34[0xC]; // 0x34
} EffKindWork; // 0x40

extern EffKindDesc D_0037E770[];
extern EffKindDesc D_0037E7E8[];
extern u32 func_002D3288(void *);
extern u32 func_00151FC8(u32);

/* Set of texture handles released and cleared one by one (layout inferred). */
typedef struct TexHandleSet {
    u8 pad_0x00[0x1C]; // 0x00
    u32 count;         // 0x1C
    u8 pad_0x20[4];    // 0x20
    void **handles;    // 0x24
} TexHandleSet; // 0x28

extern void func_002BD5C8(s32);

/* VU0 model helpers consume vf10 directly, matching the original macro-mode setup. */
extern void func_002B0B70(u8 *, void *);

void effInitModelVUState(void *model) {
    __asm__ volatile(".set noreorder\n\tvmove.xyzw vf10, vf0\n\t.set reorder");
    mdlStorePrimaryVectorVU(model);
    __asm__ volatile(".set noreorder\n\tvmove.xyzw vf10, vf0\n\t.set reorder");
    func_00217FB8(model);
    __asm__ volatile(".set noreorder\n\tvaddw.xyz vf10, vf0, vf0w\n\tvmulx.w vf10, vf0, vf0x\n\t.set reorder");
    mdlStoreTertiaryVectorVU(model);
    mdlBroadcastMasked(model, 0x80808080);
    if (*(void **)(model + 0x1C) != NULL) {
        mdlAddEntryPlain(model, 0, 0);
        *(float *)(*(u8 **)(model + 0x1C) + 0x20) = 1.0f;
    }
    *(u32 *)model &= ~1;
}

extern u16 D_003BC944;

extern void mdlLoadViewerPackage(u32, u16, u32, u32, u32);

extern void *func_00217680(u32, u32);

extern void effInitModelVUState(void *);

void *func_0029A8D8(u32 first, u32 second) {
    void *model;
    mdlLoadViewerPackage(6, D_003BC944, 0x101, first, second);
    model = func_00217680(6, D_003BC944);
    effInitModelVUState(model);
    D_003BC944++;
    return model;
}

void func_0029A938(s32 arg0) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x80) = 0;
    func_002177D0();
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029A958);

void effDestroyModelOwner(void *p) {
    void *q = *(void **)((s32)p + 8);
    if (q != NULL) {
        func_002CFF98(q);
    }
    if (*(s32 *)((s32)p + 4) != 0) {
        func_0029A938(*(s32 *)((s32)p + 4));
    }
    func_002CFF98(p);
}

u32 *effDuplicateEffectHeader(u32 *source) {
    u32 *effect = (u32 *)func_0029A958(0);
    effect[0] = source[0];
    recreateEffectModelFromSource(effect, (u8 *)source);
    return effect;
}

void recreateEffectModelFromSource(u32 *work, u8 *source) {
    void *model;

    if (work[1] != 0) {
        func_0029A938(work[1]);
    }
    model = func_00217680(func_002183D0(*(u32 *)(source + 4)), func_002183E0(*(u32 *)(source + 4)));
    effInitModelVUState(model);
    __asm__ volatile (".set noreorder\nvaddw.xyz vf10, vf0, vf0w\nvmulx.w vf10, vf0, vf0x\n.set reorder");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %0\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        ".set reorder"
        : : "f"(*(f32 *)work) : "$2", "memory");
    mdlStoreTertiaryVectorVU(model);
    work[1] = (u32)model;
}

void func_0029AB28(s32 arg0) {
    func_002DB538(*(void **)(*(s32 *)(arg0 + 4) + 0x1c), 0.0f);
}

extern u8 D_00325828[];

extern s64 func_002B3090(void *, void *);

extern void func_00217878(void *, void *);

void func_0029AB48(u8 *work) {
    void *model;
    if (func_002B3090(*(void **)(work + 4), *(void **)(work + 8)) != 0) {
        model = *(void **)(work + 4);
        *(void **)(*(u8 **)((u8 *)model + 0x18) + 0x80) = *(void **)(work + 8);
    } else {
        model = *(void **)(work + 4);
    }
    func_00217878(model, D_00325828);
}

/* Pass a vector to the VU0 model helpers via vf10 (gcc cannot do this from plain C). */
void func_0029ABA0(s32 arg0, void *vec) {
    __asm__ volatile ("lqc2 $vf10, 0(%0)" :: "r" (vec));
    mdlStorePrimaryVectorVU(*(void **)(arg0 + 4));
}

void effApplyModelVecB(s32 arg0, void *vec) {
    __asm__ volatile ("lqc2 $vf10, 0(%0)" :: "r" (vec));
    func_00217FB8(*(void **)(arg0 + 4));
}

void effBroadcastModelMask(s32 arg0) {
    mdlBroadcastMasked(*(u32 *)(arg0 + 4));
}

void func_0029ABF8(s32 arg0, float scale) {
    float v[3];
    float t;

    t = *(float *)arg0 * scale;
    v[0] = v[1] = v[2] = t;
    __asm__ volatile ("lqc2 $vf10, 0(%0)" :: "r" (v));
    mdlStoreTertiaryVectorVU(*(void **)(arg0 + 4));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029AC30);

u32 func_0029AD68(void) {
    u32 temp_v0;
    s64 temp_v1;

    temp_v0 = func_0029A958();
    func_0029AC30(temp_v0);
    temp_v1 = func_001A1438();
    if ((temp_v1 != 0) && (temp_v1 = btlIsCurrentActorFullyMarked(), temp_v1 == 0)) {
        func_0029B108(temp_v0);
    }
    return temp_v0;
}

void func_0029ADC8(u8 *work) {
    u32 flags = *(u32 *)(work + 0x0C) | 2;
    *(u32 *)(work + 0x0C) = flags;
    if ((flags & 4) == 0) {
        effDestroyModelOwner(work);
    }
}

extern void recreateEffectModelFromSource(u32 *, u8 *);

u32 *func_0029AE08(u8 *work) {
    u32 *effect = (u32 *)func_0029A958(0);

    effect[0] = *(u32 *)work;
    recreateEffectModelFromSource(effect, work);
    func_0029AC30(effect);
    if (func_001A1438() != 0 && btlIsCurrentActorFullyMarked() == 0) {
        func_0029B108(effect);
    }
    return effect;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029AE88);

extern u32 D_003BC8F8;

void func_0029B0B0(u8 *work) {
    u32 previous = *(u32 *)(work + 0xC);
    u32 flags = previous | 1;
    *(u32 *)(work + 0xC) = flags;
    if ((flags & 4) == 0) {
        if ((D_003BC8F8 & 1) == 0) {
            func_0029AE88(work);
        }
    } else if ((D_003BC8F8 & 1) != 0) {
        *(u32 *)(work + 0xC) = previous | 0x31;
    }
}

extern void *func_002CFF68(u32);

typedef struct TrackedEffectObject {
    u8 unk_00[0xC];
    u32 flags;
} TrackedEffectObject;

void func_0029B108(void *object) {
    s32 *entry = func_002CFF68(12);
    s32 *head = D_003BC948;
    TrackedEffectObject *linkedObject;

    entry[0] = (s32)object;
    entry[1] = 0;
    if (head != NULL) {
        head[1] = (s32)entry;
        entry[2] = (s32)head;
    } else {
        entry[2] = 0;
    }
    linkedObject = (TrackedEffectObject *)entry[0];
    D_003BC948 = entry;
    linkedObject->flags |= 4;
}

extern char D_003B2AA0[];

extern void func_003003F0(const char *, void *);

void func_0029B168(s32 *entry) {
    if (entry[0] != 0) {
        func_003003F0(D_003B2AA0, (void *)entry[0]);
        effDestroyModelOwner((void *)entry[0]);
        entry[0] = 0;
    }
    if (entry[2] != 0) {
        *(s32 *)(entry[2] + 4) = entry[1];
    }
    if (entry[1] != 0) {
        *(s32 *)(entry[1] + 8) = entry[2];
    } else {
        D_003BC948 = (s32 *)entry[2];
    }
    func_002CFF98(entry);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029B1D8);

void mdlPropagateObjectFlag(void) {
    s32 *node = D_003BC948;
    s32 obj;
    s32 v;

    if (node != NULL) {
        do {
            obj = *node;
            v = *(s32 *)(obj + 0xC);
            if ((v & 2) != 0) {
                *(s32 *)(obj + 0xC) = v | 8;
            }
            node = *(s32 **)((s32)node + 8);
        } while (node != NULL);
    }
}

void mdlClearListedObjectFlag(void) {
    s32 temp_v0;
    s32 *piVar2;

    piVar2 = D_003BC948;
    while (piVar2 != (s32 *)0x0) {
        temp_v0 = *piVar2;
        piVar2 = (s32 *)piVar2[2];
        *(u32 *)(temp_v0 + 0xc) = *(u32 *)(temp_v0 + 0xc) & 0xffffffef;
    }
}

void mdlSetListedObjectFlag(void) {
    s32 temp_v0;
    s32 *piVar2;

    piVar2 = D_003BC948;
    while (piVar2 != (s32 *)0x0) {
        temp_v0 = *piVar2;
        piVar2 = (s32 *)piVar2[2];
        *(u32 *)(temp_v0 + 0xc) = *(u32 *)(temp_v0 + 0xc) | 0x10;
    }
}

void mdlMarkAndProcessObjectNodes(void) {
    s32 *node = D_003BC948;
    s32 obj;
    s32 *next;
    s32 v;

    if (node == NULL) {
        return;
    }
    do {
        obj = *node;
        next = *(s32 **)((s32)node + 8);
        v = *(s32 *)(obj + 0xC) | 0xA;
        *(s32 *)(obj + 0xC) = v;
        func_0029B168(node);
        node = next;
    } while (node != NULL);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029B368);

extern void func_002944D8(u32);

void func_0029B528(u8 *object) {
    u32 i;

    if (*(u32 *)(object + 0x4C) != 0) {
        func_0029A938(*(s32 *)(object + 0x4C));
    }
    if (*(u32 *)(object + 0x48) != 0) {
        for (i = 0; i < *(u32 *)object; i++) {
            func_002944D8((*(u32 **)(object + 0x44))[i]);
        }
        func_002D0918(*(u32 *)(object + 0x48));
    }
    func_002CFF98(object);
}

typedef struct {
    u8 bytes[0x38];
    u32 last;
} EffectModelHeaderCopy;

extern u8 *func_0029B368(void *);

extern void func_0029B678(u8 *, u8 *);

u8 *func_0029B5B8(u8 *source) {
    u8 *effect = func_0029B368(0);

    *(EffectModelHeaderCopy *)(effect + 8) = *(EffectModelHeaderCopy *)(source + 8);
    func_0029B678(effect, source);
    return effect;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029B678);

void func_0029B7E0(s32 arg0) {
    func_002DB538(*(void **)(*(s32 *)(arg0 + 0x4C) + 0x1C), 0.0f);
    *(u32 *)(arg0 + 4) = 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029B818);

/* Pass a vector to the VU0 model helpers via vf10 (gcc cannot do this from plain C). */
void func_0029BCF8(s32 arg0, void *vec) {
    __asm__ volatile ("lqc2 $vf10, 0(%0)" :: "r" (vec));
    mdlStorePrimaryVectorVU(*(void **)(arg0 + 0x4c));
}

void func_0029BD18(s32 arg0, void *vec) {
    __asm__ volatile ("lqc2 $vf10, 0(%0)" :: "r" (vec));
    func_00217FB8(*(void **)(arg0 + 0x4c));
}

void func_0029BD38(s32 arg0) {
    mdlBroadcastMasked(*(u32 *)(arg0 + 0x4c));
}

void effScaleModelVec(s32 arg0, float f) {
    u32 bits;

    __asm__ volatile (
        ".set noreorder               \n"
        "vaddw.xyz vf10, vf0, vf0w    \n"
        "vmulx.w vf10, vf0, vf0x      \n"
        "mfc1 %0, $f12                \n"
        "qmtc2.ni %0, $vf2            \n"
        "vmulx.xyzw vf10, vf10, $vf2x \n"
        ".set reorder"
        : "+r" (bits) : : "memory");
    mdlStoreTertiaryVectorVU(*(void **)(arg0 + 0x4C));
}

void func_0029BD80(void) {
    D_003BC954 = 0;
    D_003BC950 = 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029BD90);

u32 func_0029BF88(u32 arg0, u32 arg1) {
    void *p;

    p = func_0029BD90((void *)arg0);
    *(u32 *)((s32)p + 0x18) = arg1;
    return (u32)p;
}

extern u8 *D_003BC958;

extern void sdfTexReleaseReference(void *);

void effReleaseSharedReference(RefObj *obj) {
    D_003BC94C--;
    if (D_003BC94C == 0) {
        u8 *graphics = D_003BC958;
        *(u16 *)(graphics + 0xC) = 0x100;
        *(u16 *)(graphics + 0xE) = 0x100;
        D_003BC950 = -1;
        sdfTexReleaseReference(graphics);
    }
    obj->cnt14--;
    if (obj->cnt14 == 0) {
        func_002D0918(obj->cnt1C);
    }
}

RefObj *effRetainSharedReference(RefObj *obj) {
    obj->cnt14++;
    D_003BC94C++;
    return obj;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029C048);

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

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029C230);

void effReleaseReferenceHolder(u8 *holder) {
    u32 i;
    if (--*(u32 *)(holder + 0x1C) != 0) {
        return;
    }
    for (i = 0; i < *(u32 *)(holder + 4); i++) {
        effReleaseSharedReference(*(RefObj **)(*(u32 *)(holder + 0x14) + 4 * i));
    }
    func_002D0918(*(u32 *)(holder + 0x24));
}

RefObj *func_0029C408(RefObj *obj) {
    obj->cnt1C++;
    return obj;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029C420);

void func_0029C500(s32 arg0, u32 arg1, s32 arg2) {
    func_0029C048(arg1, *(u32 *)(*(s32 *)(arg2 + 0xc) * 4 + *(s32 *)(arg0 + 0x14)));
}

u32 func_0029C530(source)
    u32 source;
{
    u32 *buffer = (u32 *)func_002CFEB8(12);
    *buffer = 0;
    memcpy(buffer + 1, (const void *)source, 8);
    return (u32)buffer;
}

void func_0029C570(void) {
    u64 temp_v0;

    temp_v0 = fileResolvePrimaryBuffer();
    func_0029C530(temp_v0);
}

void func_0029C590(u32 arg0) {
    func_00105888();
    func_002CFF98(arg0);
}

void func_0029C5B8(s32 arg0) {
    func_0029C530(arg0 + 4);
}

void func_0029C5D0(u32 *arg0) {
    *arg0 = 0;
}

void func_0029C5D8(s32 *arg0) {
    s32 temp_v0;

    temp_v0 = *arg0;
    if (temp_v0 == 0) {
        kwlnFadeSetupFrames(arg0[1], arg0[2]);
        temp_v0 = *arg0;
    }
    *arg0 = temp_v0 + 1;
}

u8 *func_0029C620(source)
const u8 *source;
{
    u8 *effect = (u8 *)func_002CFEB8(0x58);
    memset(effect, 0, 0x58);
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" : : "r"(effect) : "memory");
    memcpy(effect + 0x18, source, 0x40);
    return effect;
}

void func_0029C6F0(void) {
    u64 temp_v0;

    temp_v0 = fileResolvePrimaryBuffer();
    func_0029C620(temp_v0);
}

void func_0029C710(void) {
    func_002CFF98();
}

void func_0029C728(s32 arg0) {
    func_0029C620(arg0 + 0x18);
}

void func_0029C740(s32 arg0) {
    *(u32 *)(arg0 + 0x10) = 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029C748);

void func_0029CDE0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0029CDF0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

void func_0029CDF8(s32 arg0) {
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

void func_0029CE50(void *arg0) {
    func_002CEAE8();
}

void func_0029CE68(void) {
    func_002CEC08();
}

void func_0029CE80(void) {
    func_002CEC28();
}

void func_0029CE98(s32 arg0) {
    func_0029CE50((void *)(arg0 + 0x14));
}

void func_0029CEB0(s32 *p) {
    func_0029CDF8((s32)p);
    *p = 0;
}

void func_0029CED8(s32 arg0) {
    func_002CEC40();
}

void func_0029CEF0(s32 arg0) {
    func_002CF248();
}

void func_0029CF08(s32 arg0) {
    func_0029CED8(arg0);
    func_0029CEF0(arg0);
}

void func_0029CF30(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 4) = arg1;
}

u32 func_0029CF38(source)
    u32 source;
{
    u32 *buffer = (u32 *)func_002CFEB8(0x14);
    *buffer = 0;
    memcpy(buffer + 1, (const void *)source, 16);
    return (u32)buffer;
}

void func_0029CF88(void) {
    u64 temp_v0;

    temp_v0 = fileResolvePrimaryBuffer();
    func_0029CF38(temp_v0);
}

void func_0029CFA8(u32 arg0) {
    func_0010A158();
    func_002CFF98(arg0);
}

void func_0029CFD0(s32 arg0) {
    func_0029CF38(arg0 + 4);
}

void func_0029CFE8(u32 *arg0) {
    *arg0 = 0;
}

void func_0029CFF0(s32 *arg0) {
    s32 temp_v0;

    temp_v0 = *arg0;
    if (temp_v0 == 0) {
        evtSelStateCreate(arg0[1], (s16)arg0[2], arg0[3], arg0[4]);
        temp_v0 = *arg0;
    }
    *arg0 = temp_v0 + 1;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029D040);

void func_0029D1B8(s32 arg0) {
    func_00186C18(arg0 + 0xc0);
}

void func_0029D1D0(void) {
    func_00186CB8();
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029D1E8);

void effUpdateTarget(u8 *work, u32 target) {
    u32 previous = *(u32 *)(work + 0x30);
    if (previous != 0 && previous != target) {
        if (*(u32 *)(work + 0x2C) != 4) {
            func_002D2D00(previous);
        }
        *(u32 *)(work + 0x30) = target;
    }
    *(u32 *)(*(u32 *)(work + 0x24) + 0x2C) = target;
}

void func_0029D450(s32 arg0) {
    func_00186F90(arg0 + 0xc0);
}

void func_0029D468(void) {
    func_00187080();
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029D480);

void func_0029D650(u8 *work, u32 target) {
    u32 previous = *(u32 *)(work + 0x30);
    if (previous != 0 && previous != target) {
        if (*(u32 *)(work + 0x2C) != 4) {
            func_002D2D00(previous);
        }
        *(u32 *)(work + 0x30) = target;
    }
    *(u32 *)(*(u32 *)(work + 0x24) + 0x2C) = target;
}

void func_0029D6B0(s32 arg0) {
    func_00187460(arg0 + 0xc0);
}

void func_0029D6C8(void) {
    func_00187580();
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029D6E0);

void func_0029D8B0(u8 *work, u32 target) {
    u32 previous = *(u32 *)(work + 0x30);
    if (previous != 0 && previous != target) {
        if (*(u32 *)(work + 0x2C) != 4) {
            func_002D2D00(previous);
        }
        *(u32 *)(work + 0x30) = target;
    }
    *(u32 *)(*(u32 *)(work + 0x24) + 0x2C) = target;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029D910);

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029DA70);

void func_0029DB78(s32 arg0) {
    func_00187FC0(arg0 + 0xc0);
}

void func_0029DB90(void) {
    func_00188050();
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029DBA8);

void func_0029DD70(u8 *work, u32 target) {
    u32 previous = *(u32 *)(work + 0x30);
    if (previous != 0 && previous != target) {
        if (*(u32 *)(work + 0x2C) != 4) {
            func_002D2D00(previous);
        }
        *(u32 *)(work + 0x30) = target;
    }
    *(u32 *)(*(u32 *)(work + 0x24) + 0x24) = target;
}

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2AA0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2AC0);

EffKindWork *func_0029DDD0(u16 kind, u8 *source) {
    u32 headerSize = 0x40;
    u32 size = D_0037E770[kind].size;
    EffKindWork *work = func_002CFF68(size + headerSize);

    work->kind = kind;
    work->target = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" : : "r"(work) : "memory");
    work->payload = (u8 *)work + headerSize;
    memcpy(work->payload, source, size);
    switch (*(s32 *)(source + 0x28)) {
    case 1:
        work->mode = 0x44;
        break;
    case 2:
        work->mode = 0x48;
        break;
    case 3:
    case 4:
        work->mode = 0x42;
        break;
    case 5:
    case 6:
        work->mode = 6;
        break;
    }
    if (D_0037E770[kind].create != NULL) {
        work->handle = D_0037E770[kind].create(source);
    }
    return work;
}

EffKindWork *func_0029DF00(u8 *work) {
    void *source = fileResolvePrimaryBuffer(work);
    EffKindWork *effect = func_0029DDD0(*(u16 *)(work + 0xC), source);
    u32 *secondary = fileResolveSecondaryBuffer(work);

    if (secondary != NULL) {
        if (D_0037E770[effect->kind].apply != NULL) {
            u32 kind = *(u16 *)(work + 0x1C);
            effect->sourceKind = kind;
            switch (kind) {
            case 1:
                effect->target = func_002D3288(secondary);
                break;
            case 4:
                effect->target = func_00151FC8(secondary[0]);
                break;
            }
            D_0037E770[effect->kind].apply(effect, effect->target);
        }
    }
    return effect;
}

void effReleaseLinkedTarget(u8 *work) {
    void *handle = *(void **)(work + 0x24);
    if (handle != NULL) {
        D_0037E774[*(s32 *)(work + 0x1C)].fn(handle);
    }
    if (*(u32 *)(work + 0x30) != 0 && *(u32 *)(work + 0x2C) != 4) {
        func_002D2D00(*(u32 *)(work + 0x30));
    }
    func_002CFF98(work);
}

void func_0029E058(s32 arg0) {
    func_0029DDD0(*(u16 *)(arg0 + 0x1c), *(u32 *)(arg0 + 0x28));
}

void func_0029E078(s32 arg0) {
    *(u32 *)(arg0 + 0x20) = 0;
}

void func_0029E080(s32 arg0) {
    D_0037E778[*(s32 *)(arg0 + 0x1C)].fn((void *)arg0);
    *(s32 *)(arg0 + 0x20) = *(s32 *)(arg0 + 0x20) + 1;
}

void func_0029E0D0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0029E0E0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

void func_0029E0E8(Matrix4 *mat, float value) {
    mat->u.m[1][2] = value;
}

EffKindWork *func_0029E0F0(u16 kind, u8 *source) {
    u32 headerSize = 0x40;
    u32 size = D_0037E7E8[kind].size;
    EffKindWork *work = func_002CFF68(size + headerSize);

    work->kind = kind;
    work->target = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" : : "r"(work) : "memory");
    work->payload = (u8 *)work + headerSize;
    memcpy(work->payload, source, size);
    switch (*(s32 *)(source + 0x28)) {
    case 1:
        work->mode = 0x44;
        break;
    case 2:
        work->mode = 0x48;
        break;
    case 3:
    case 4:
        work->mode = 0x42;
        break;
    case 5:
    case 6:
        work->mode = 6;
        break;
    }
    if (D_0037E7E8[kind].create != NULL) {
        work->handle = D_0037E7E8[kind].create(source);
    }
    return work;
}

EffKindWork *func_0029E220(u8 *work) {
    void *source = fileResolvePrimaryBuffer(work);
    EffKindWork *effect = func_0029E0F0(*(u16 *)(work + 0xC), source);
    u32 *secondary = fileResolveSecondaryBuffer(work);

    if (secondary != NULL) {
        if (D_0037E7E8[effect->kind].apply != NULL) {
            u32 kind = *(u16 *)(work + 0x1C);
            effect->sourceKind = kind;
            switch (kind) {
            case 1:
                effect->target = func_002D3288(secondary);
                break;
            case 4:
                effect->target = func_00151FC8(secondary[0]);
                break;
            }
            D_0037E7E8[effect->kind].apply(effect, effect->target);
        }
    }
    return effect;
}

void func_0029E300(u8 *work) {
    void *handle = *(void **)(work + 0x24);
    if (handle != NULL) {
        D_0037E7EC[*(s32 *)(work + 0x1C)].fn(handle);
    }
    if (*(u32 *)(work + 0x30) != 0 && *(u32 *)(work + 0x2C) != 4) {
        func_002D2D00(*(u32 *)(work + 0x30));
    }
    func_002CFF98(work);
}

void func_0029E378(s32 arg0) {
    func_0029E0F0(*(u16 *)(arg0 + 0x1c), *(u32 *)(arg0 + 0x28));
}

void func_0029E398(s32 arg0) {
    *(u32 *)(arg0 + 0x20) = 0;
}

void func_0029E3A0(s32 arg0) {
    D_0037E7F0[*(s32 *)(arg0 + 0x1C)].fn((void *)arg0);
    *(s32 *)(arg0 + 0x20) = *(s32 *)(arg0 + 0x20) + 1;
}

void func_0029E3F0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0029E400(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

void func_0029E408(Matrix4 *mat, float value) {
    mat->u.m[1][2] = value;
}

u8 *effCreateBillboardWork(u8 *source) {
    u8 *work = (u8 *)func_002CFEB8(0x68);
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

void func_0029E4C0(u32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)((s32)arg0 + 100);
    if (temp_v0 != 0) {
        billDispatchByKind(temp_v0);
    }
    func_002CFF98(arg0);
}

u8 *effDuplicateBillState(const u8 *source) {
    u8 *effect = (u8 *)effCreateBillboardWork(NULL);
    memcpy(effect + 0x2C, source + 0x2C, 0x38);
    func_0029E5B0((s32)effect, (s32)source);
    return effect;
}

void func_0029E5B0(s32 arg0, s32 arg1) {
    u32 temp_v0;

    if (*(s32 *)(arg0 + 100) != 0) {
        billDispatchByKind(*(s32 *)(arg0 + 100));
    }
    temp_v0 = func_00151E60(*(u32 *)(arg1 + 100));
    *(u32 *)(arg0 + 100) = temp_v0;
}

void func_0029E600(s32 arg0) {
    func_001522E8(*(u32 *)(arg0 + 100), 0);
    *(u32 *)(arg0 + 0x28) = 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029E630);

void func_0029E728(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0029E738(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x10, src);
}

void func_0029E750(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_0029E758(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

void effClearBillFrames(u8 *work) {
    u8 *state = *(u8 **)(work + 0x30);
    u8 *config = *(u8 **)(work + 0x34);
    u8 *asset = *(u8 **)(state + 4);
    s32 remaining = *(s32 *)(config + 0x38);
    u8 *entry = *(u8 **)state;

    memset(*(void **)(asset + 0x1C), 0, *(u32 *)(asset + 8) * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            *(s32 *)entry = -1;
            entry += 0x18;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029E7D8);

void func_0029E868(s32 arg0) {
    releaseEffectResourceRefs(*(u32 *)(arg0 + 4));
    func_002D0918(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029E898);

extern u32 func_00296F58(u8 *, u8 *, u32, u32);

extern void func_002E7D98(void);

extern u8 D_0037E0E0[];

extern void func_002A3E10(u8 *, void *);

typedef struct BillCellDrawWork {
    u8 pad0[0x10];
    u8 transform[0x10];
    f32 scale;
    s32 baseColor;
    u32 frameLimit;
    u8 pad2C[4];
    u32 *instances;
    u8 *config;
} BillCellDrawWork;

void func_0029EEC8(BillCellDrawWork *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = *(u32 *)(config + 0x34);
    u32 *list = work->instances;
    u8 *out = (u8 *)list[1];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = func_00296F58(config, config + 0x24, limit, progress);
    unit = 0x3C000000;
    color1[0] = work->baseColor;
    EE_MMI_RGBA_UNPACK(color1, unit);
    __asm__ volatile (".set noreorder\n\tvmove.xyzw vf11, vf10\n\t.set reorder");
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    __asm__ volatile (".set noreorder\n\tvmul.xyzw vf10, vf10, vf11\n\t.set reorder");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %1\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vftoi0.xyzw vf10, vf10\n"
        "qmfc2.ni %0, vf10\n"
        "ppach %0, $0, %0\n"
        "ppacb %0, $0, %0\n"
        ".set reorder"
        : "=r"(packed) : "f"(128.0f) : "$2");
    blended[0] = packed;
    *(u32 *)(out + 4) = blended[0];
    *(u32 *)out = *(u32 *)(config + 0x28);
    *(u8 *)(out + 0x14) = *(u8 *)(config + 0x56);
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(work->transform));
    func_002E7D98();
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(D_0037E0E0));
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %0\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmulx.xyzw vf28, vf28, vf10x\n"
        "vmuly.xyzw vf29, vf29, vf10y\n"
        "vmulz.xyzw vf30, vf30, vf10z\n"
        "lqc2 vf10, 0(%1)\n"
        "vmove.w vf10, vf0\n"
        "vmove.xyzw vf31, vf10\n"
        "sqc2 vf28, 0(%2)\n"
        "sqc2 vf29, 0x10(%2)\n"
        "sqc2 vf30, 0x20(%2)\n"
        "sqc2 vf31, 0x30(%2)\n"
        ".set reorder"
        : : "f"(work->scale), "r"(work), "r"(mtx) : "$2", "memory");
    func_002A3E10(out, mtx);
}

void billResetCellIndices(u8 *work) {
    u8 *state = *(u8 **)(work + 0x30);
    u8 *config = *(u8 **)(work + 0x34);
    u8 *asset = *(u8 **)(state + 4);
    s32 remaining = *(s32 *)(config + 0x38);
    u8 *entry = *(u8 **)state;

    memset(*(void **)(asset + 0x1C), 0, *(u32 *)(asset + 8) * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            *(s32 *)entry = -1;
            entry += 0x1C;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029F0A8);

void func_0029F138(s32 arg0) {
    releaseEffectResourceRefs(*(u32 *)(arg0 + 4));
    func_002D0918(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029F168);

void func_0029F8A0(BillCellDrawWork *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = *(u32 *)(config + 0x34);
    u32 *list = work->instances;
    u8 *out = (u8 *)list[1];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = func_00296F58(config, config + 0x24, limit, progress);
    unit = 0x3C000000;
    color1[0] = work->baseColor;
    EE_MMI_RGBA_UNPACK(color1, unit);
    __asm__ volatile (".set noreorder\n\tvmove.xyzw vf11, vf10\n\t.set reorder");
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    __asm__ volatile (".set noreorder\n\tvmul.xyzw vf10, vf10, vf11\n\t.set reorder");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %1\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vftoi0.xyzw vf10, vf10\n"
        "qmfc2.ni %0, vf10\n"
        "ppach %0, $0, %0\n"
        "ppacb %0, $0, %0\n"
        ".set reorder"
        : "=r"(packed) : "f"(128.0f) : "$2");
    blended[0] = packed;
    *(u32 *)(out + 4) = blended[0];
    *(u32 *)out = *(u32 *)(config + 0x28);
    *(u8 *)(out + 0x14) = *(u8 *)(config + 0x56);
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(work->transform));
    func_002E7D98();
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(D_0037E0E0));
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %0\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmulx.xyzw vf28, vf28, vf10x\n"
        "vmuly.xyzw vf29, vf29, vf10y\n"
        "vmulz.xyzw vf30, vf30, vf10z\n"
        "lqc2 vf10, 0(%1)\n"
        "vmove.w vf10, vf0\n"
        "vmove.xyzw vf31, vf10\n"
        "sqc2 vf28, 0(%2)\n"
        "sqc2 vf29, 0x10(%2)\n"
        "sqc2 vf30, 0x20(%2)\n"
        "sqc2 vf31, 0x30(%2)\n"
        ".set reorder"
        : : "f"(work->scale), "r"(work), "r"(mtx) : "$2", "memory");
    func_002A3E10(out, mtx);
}

void billResetParticleIndices(u8 *work) {
    u8 *state = *(u8 **)(work + 0x30);
    u8 *config = *(u8 **)(work + 0x34);
    u8 *asset = *(u8 **)(state + 4);
    s32 remaining = *(s32 *)(config + 0x38);
    u8 *entry = *(u8 **)state;

    memset(*(void **)(asset + 0x1C), 0, *(u32 *)(asset + 8) * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            *(s32 *)entry = -1;
            entry += 0x2C;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029FA80);

void func_0029FB18(s32 arg0) {
    releaseEffectResourceRefs(*(u32 *)(arg0 + 4));
    func_002D0918(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_0029FB48);

void func_002A0260(BillCellDrawWork *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = *(u32 *)(config + 0x34);
    u32 *list = work->instances;
    u8 *out = (u8 *)list[1];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = func_00296F58(config, config + 0x24, limit, progress);
    unit = 0x3C000000;
    color1[0] = work->baseColor;
    EE_MMI_RGBA_UNPACK(color1, unit);
    __asm__ volatile (".set noreorder\n\tvmove.xyzw vf11, vf10\n\t.set reorder");
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    __asm__ volatile (".set noreorder\n\tvmul.xyzw vf10, vf10, vf11\n\t.set reorder");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %1\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vftoi0.xyzw vf10, vf10\n"
        "qmfc2.ni %0, vf10\n"
        "ppach %0, $0, %0\n"
        "ppacb %0, $0, %0\n"
        ".set reorder"
        : "=r"(packed) : "f"(128.0f) : "$2");
    blended[0] = packed;
    *(u32 *)(out + 4) = blended[0];
    *(u32 *)out = *(u32 *)(config + 0x28);
    *(u8 *)(out + 0x14) = *(u8 *)(config + 0x56);
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(work->transform));
    func_002E7D98();
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(D_0037E0E0));
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %0\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmulx.xyzw vf28, vf28, vf10x\n"
        "vmuly.xyzw vf29, vf29, vf10y\n"
        "vmulz.xyzw vf30, vf30, vf10z\n"
        "lqc2 vf10, 0(%1)\n"
        "vmove.w vf10, vf0\n"
        "vmove.xyzw vf31, vf10\n"
        "sqc2 vf28, 0(%2)\n"
        "sqc2 vf29, 0x10(%2)\n"
        "sqc2 vf30, 0x20(%2)\n"
        "sqc2 vf31, 0x30(%2)\n"
        ".set reorder"
        : : "f"(work->scale), "r"(work), "r"(mtx) : "$2", "memory");
    func_002A3E10(out, mtx);
}

void effClearAnimatedFrames(u8 *work) {
    u8 *state = *(u8 **)(work + 0x30);
    u8 *config = *(u8 **)(work + 0x34);
    u8 *asset = *(u8 **)(state + 4);
    s32 remaining = *(s32 *)(config + 0x38);
    u8 *entry = *(u8 **)state;

    memset(*(void **)(asset + 0x1C), 0, *(u32 *)(asset + 8) * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            *(s32 *)entry = -1;
            entry += 0x18;
        }
    }
}

extern void *func_002D03F8(u32);

u8 *func_002A0440(u8 *config) {
    u32 headerSize = 0x10;
    u8 *base = func_002D03F8(*(u32 *)(config + 0x38) * 0x18 + headerSize);
    u8 *node = (u8 *)sdfResourceRetainAddress((u32)base);
    u8 *entries = node + headerSize;

    *(u8 **)(node + 8) = base;
    *(u8 **)node = entries;
    if (*(u32 *)(config + 0x70) == 0) {
        *(u32 *)(config + 0x70) = 1;
    }
    return node;
}

void effInitializeAlternatingTransformRows(u8 *node, u8 *config) {
    u32 count = *(u32 *)(config + 0x38);
    u32 index;
    f32 *transform;

    if (count == 0) {
        return;
    }
    transform = *(f32 **)(*(u8 **)(node + 4) + 0x20);
    index = 0;
    for (; index < count; index++, transform += 8) {
        if (index & 1) {
            transform[0] = 0.0f;
            transform[1] = 0.0f;
            transform[2] = 0.5f;
            transform[3] = 0.0f;
            transform[4] = 0.0f;
            transform[5] = 1.0f;
            transform[6] = 0.5f;
        } else {
            transform[0] = 0.5f;
            transform[1] = 0.0f;
            transform[2] = 1.0f;
            transform[3] = 0.0f;
            transform[4] = 0.5f;
            transform[5] = 1.0f;
            transform[6] = 1.0f;
        }
        transform[7] = 1.0f;
    }
}

extern u32 func_002A3BD8(u32, u32, u32);

u8 *billCreateAnimatedTransform(u8 *config, u32 resource) {
    u8 *node = func_002A0440(config);
    *(u32 *)(node + 4) = func_002A3BD8(*(u32 *)(config + 0x38), 3, resource);
    effInitializeAlternatingTransformRows(node, config);
    return node;
}

extern u8 *func_002A0DC0(u8 *);

extern u8 *func_002A2C40(u8 *);

extern u32 func_002A3D68(u32);

extern void billInitializeEmitterRows(u8 *, u8 *);

extern void billInitializeQuadRows(u8 *, u8 *);

u8 *billCloneAnimatedTransform(u8 *work) {
    u8 *config = *(u8 **)(work + 0x34);
    u8 *state = *(u8 **)(work + 0x30);
    u8 *node = func_002A0440(config);
    *(u32 *)(node + 4) = func_002A3D68(*(u32 *)(state + 4));
    effInitializeAlternatingTransformRows(node, config);
    return node;
}

void func_002A0608(s32 arg0) {
    releaseEffectResourceRefs(*(u32 *)(arg0 + 4));
    func_002D0918(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A0638);

void func_002A0BE0(BillCellDrawWork *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = *(u32 *)(config + 0x34);
    u32 *list = work->instances;
    u8 *out = (u8 *)list[1];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = func_00296F58(config, config + 0x24, limit, progress);
    unit = 0x3C000000;
    color1[0] = work->baseColor;
    EE_MMI_RGBA_UNPACK(color1, unit);
    __asm__ volatile (".set noreorder\n\tvmove.xyzw vf11, vf10\n\t.set reorder");
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    __asm__ volatile (".set noreorder\n\tvmul.xyzw vf10, vf10, vf11\n\t.set reorder");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %1\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vftoi0.xyzw vf10, vf10\n"
        "qmfc2.ni %0, vf10\n"
        "ppach %0, $0, %0\n"
        "ppacb %0, $0, %0\n"
        ".set reorder"
        : "=r"(packed) : "f"(128.0f) : "$2");
    blended[0] = packed;
    *(u32 *)(out + 4) = blended[0];
    *(u32 *)out = *(u32 *)(config + 0x28);
    *(u8 *)(out + 0x14) = *(u8 *)(config + 0x56);
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(work->transform));
    func_002E7D98();
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(D_0037E0E0));
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %0\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmulx.xyzw vf28, vf28, vf10x\n"
        "vmuly.xyzw vf29, vf29, vf10y\n"
        "vmulz.xyzw vf30, vf30, vf10z\n"
        "lqc2 vf10, 0(%1)\n"
        "vmove.w vf10, vf0\n"
        "vmove.xyzw vf31, vf10\n"
        "sqc2 vf28, 0(%2)\n"
        "sqc2 vf29, 0x10(%2)\n"
        "sqc2 vf30, 0x20(%2)\n"
        "sqc2 vf31, 0x30(%2)\n"
        ".set reorder"
        : : "f"(work->scale), "r"(work), "r"(mtx) : "$2", "memory");
    func_002A3E10(out, mtx);
}

void billResetEmitterIndices(u8 *work) {
    u8 *state = *(u8 **)(work + 0x30);
    u8 *config = *(u8 **)(work + 0x34);
    u8 *asset = *(u8 **)(state + 4);
    s32 remaining = *(s32 *)(config + 0x38);
    u8 *entry = *(u8 **)state;

    memset(*(void **)(asset + 0x1C), 0, *(u32 *)(asset + 8) * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            *(s32 *)entry = -1;
            entry += 0x18;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A0DC0);

void billInitializeEmitterRows(u8 *node, u8 *config) {
    u32 count = *(u32 *)(config + 0x38);
    u32 index;
    f32 *transform;

    if (count == 0) {
        return;
    }
    transform = *(f32 **)(*(u8 **)(node + 4) + 0x20);
    index = 0;
    for (; index < count; index++, transform += 8) {
        if (index & 1) {
            transform[0] = 0.0f;
            transform[1] = 0.0f;
            transform[2] = 0.5f;
            transform[3] = 0.0f;
            transform[4] = 0.0f;
            transform[5] = 1.0f;
            transform[6] = 0.5f;
        } else {
            transform[0] = 0.5f;
            transform[1] = 0.0f;
            transform[2] = 1.0f;
            transform[3] = 0.0f;
            transform[4] = 0.5f;
            transform[5] = 1.0f;
            transform[6] = 1.0f;
        }
        transform[7] = 1.0f;
    }
}

u8 *billCreateEmitterTransform(u8 *config, u32 resource) {
    u8 *node = func_002A0DC0(config);
    *(u32 *)(node + 4) = func_002A3BD8(*(u32 *)(config + 0x38), 4, resource);
    billInitializeEmitterRows(node, config);
    return node;
}

u8 *billCloneEmitterTransform(u8 *work) {
    u8 *config = *(u8 **)(work + 0x34);
    u8 *state = *(u8 **)(work + 0x30);
    u8 *node = func_002A0DC0(config);
    *(u32 *)(node + 4) = func_002A3D68(*(u32 *)(state + 4));
    billInitializeEmitterRows(node, config);
    return node;
}

void func_002A0F70(s32 arg0) {
    releaseEffectResourceRefs(*(u32 *)(arg0 + 4));
    func_002D0918(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A0FA0);

void func_002A1588(u8 *work) {
    u8 *config = *(u8 **)(work + 0x34);
    u32 limit = *(u32 *)(work + 0x28);
    u32 progress = *(u32 *)(config + 0x34);
    u32 *list = *(u32 **)(work + 0x30);
    u8 *out = (u8 *)list[1];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = func_00296F58(config, config + 0x24, limit, progress);
    unit = 0x3C000000;
    color1[0] = *(s32 *)(work + 0x24);
    EE_MMI_RGBA_UNPACK(color1, unit);
    __asm__ volatile (".set noreorder\n\tvmove.xyzw vf11, vf10\n\t.set reorder");
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    __asm__ volatile (".set noreorder\n\tvmul.xyzw vf10, vf10, vf11\n\t.set reorder");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %1\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vftoi0.xyzw vf10, vf10\n"
        "qmfc2.ni %0, vf10\n"
        "ppach %0, $0, %0\n"
        "ppacb %0, $0, %0\n"
        ".set reorder"
        : "=r"(packed) : "f"(128.0f) : "$2");
    blended[0] = packed;
    *(u32 *)(out + 4) = blended[0];
    *(u32 *)out = *(u32 *)(config + 0x28);
    *(u8 *)(out + 0x14) = *(u8 *)(config + 0x56);
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(work + 0x10));
    func_002E7D98();
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(D_0037E0E0));
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %0\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmulx.xyzw vf28, vf28, vf10x\n"
        "vmuly.xyzw vf29, vf29, vf10y\n"
        "vmulz.xyzw vf30, vf30, vf10z\n"
        "lqc2 vf10, 0(%1)\n"
        "vmove.w vf10, vf0\n"
        "vmove.xyzw vf31, vf10\n"
        "sqc2 vf28, 0(%2)\n"
        "sqc2 vf29, 0x10(%2)\n"
        "sqc2 vf30, 0x20(%2)\n"
        "sqc2 vf31, 0x30(%2)\n"
        ".set reorder"
        : : "f"(*(f32 *)(work + 0x20)), "r"(work), "r"(mtx) : "$2", "memory");
    func_002A3E10(out, mtx);
}

void effClearStripFrames(u8 *work) {
    u8 *state = *(u8 **)(work + 0x30);
    u8 *config = *(u8 **)(work + 0x34);
    u8 *asset = *(u8 **)(state + 4);
    s32 remaining = *(s32 *)(config + 0x38);
    u8 *entry = *(u8 **)state;

    memset(*(void **)(asset + 0x1C), 0, *(u32 *)(asset + 8) * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            *(s32 *)entry = -1;
            entry += 0x28;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A1768);

void billInitializeStripRows(u8 *node, u8 *config) {
    u32 count = *(u32 *)(config + 0x38);
    u32 index;
    f32 *transform;

    if (count == 0) {
        return;
    }
    transform = *(f32 **)(*(u8 **)(node + 4) + 0x20);
    index = 0;
    for (; index < count; index++, transform += 8) {
        if (index & 1) {
            transform[0] = 0.0f;
            transform[1] = 0.0f;
            transform[2] = 0.5f;
            transform[3] = 0.0f;
            transform[4] = 0.0f;
            transform[5] = 1.0f;
            transform[6] = 0.5f;
        } else {
            transform[0] = 0.5f;
            transform[1] = 0.0f;
            transform[2] = 1.0f;
            transform[3] = 0.0f;
            transform[4] = 0.5f;
            transform[5] = 1.0f;
            transform[6] = 1.0f;
        }
        transform[7] = 1.0f;
    }
}

extern u8 *func_002A1768(u8 *);

u8 *billCreateStripTransform(u8 *config, u32 resource) {
    u8 *node = func_002A1768(config);

    *(u32 *)(node + 4) = func_002A3BD8(*(u32 *)(config + 0x38), 3, resource);
    billInitializeStripRows(node, config);
    return node;
}

u8 *billCloneStripTransform(u8 *work) {
    u8 *config = *(u8 **)(work + 0x34);
    u8 *state = *(u8 **)(work + 0x30);
    u8 *node = func_002A1768(config);
    *(u32 *)(node + 4) = func_002A3D68(*(u32 *)(state + 4));
    billInitializeStripRows(node, config);
    return node;
}

void func_002A1918(s32 arg0) {
    releaseEffectResourceRefs(*(u32 *)(arg0 + 4));
    func_002D0918(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A1948);

void func_002A2008(u8 *work) {
    u8 *config = *(u8 **)(work + 0x34);
    u32 limit = *(u32 *)(work + 0x28);
    u32 progress = *(u32 *)(config + 0x34);
    u32 *list = *(u32 **)(work + 0x30);
    u8 *out = (u8 *)list[1];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = func_00296F58(config, config + 0x24, limit, progress);
    unit = 0x3C000000;
    color1[0] = *(s32 *)(work + 0x24);
    __asm__ volatile (
        ".set noreorder\n"
        "lw $2, 0(%1)\n"
        "pextlb $2, $0, $2\n"
        "pextlh $2, $0, $2\n"
        "qmtc2.ni $2, vf10\n"
        "vitof0.xyzw vf10, vf10\n"
        "qmtc2.ni %0, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmove.xyzw vf11, vf10\n"
        ".set reorder"
        : : "r"(unit), "r"(color1) : "$2", "memory");
    color2[0] = second;
    __asm__ volatile (
        ".set noreorder\n"
        "lw $2, 0(%1)\n"
        "pextlb $2, $0, $2\n"
        "pextlh $2, $0, $2\n"
        "qmtc2.ni $2, vf10\n"
        "vitof0.xyzw vf10, vf10\n"
        "qmtc2.ni %0, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmul.xyzw vf10, vf10, vf11\n"
        ".set reorder"
        : : "r"(unit), "r"(color2) : "$2", "memory");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %1\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vftoi0.xyzw vf10, vf10\n"
        "qmfc2.ni %0, vf10\n"
        "ppach %0, $0, %0\n"
        "ppacb %0, $0, %0\n"
        ".set reorder"
        : "=r"(packed) : "f"(128.0f) : "$2");
    blended[0] = packed;
    *(u32 *)(out + 4) = blended[0];
    *(u32 *)out = *(u32 *)(config + 0x28);
    *(u8 *)(out + 0x14) = *(u8 *)(config + 0x56);
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(work + 0x10));
    func_002E7D98();
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(D_0037E0E0));
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %0\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmulx.xyzw vf28, vf28, vf10x\n"
        "vmuly.xyzw vf29, vf29, vf10y\n"
        "vmulz.xyzw vf30, vf30, vf10z\n"
        "lqc2 vf10, 0(%1)\n"
        "vmove.w vf10, vf0\n"
        "vmove.xyzw vf31, vf10\n"
        "sqc2 vf28, 0(%2)\n"
        "sqc2 vf29, 0x10(%2)\n"
        "sqc2 vf30, 0x20(%2)\n"
        "sqc2 vf31, 0x30(%2)\n"
        ".set reorder"
        : : "f"(*(f32 *)(work + 0x20)), "r"(work), "r"(mtx) : "$2", "memory");
    func_002A3E10(out, mtx);
}

void billResetTrailIndices(u8 *work) {
    u8 *state = *(u8 **)(work + 0x30);
    u8 *config = *(u8 **)(work + 0x34);
    u8 *asset = *(u8 **)(state + 4);
    s32 remaining = *(s32 *)(config + 0x38);
    u8 *entry = *(u8 **)state;

    memset(*(void **)(asset + 0x1C), 0, *(u32 *)(asset + 8) * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            *(s32 *)entry = -1;
            entry += 0x2C;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A21E8);

void func_002A2288(s32 arg0) {
    releaseEffectResourceRefs(*(u32 *)(arg0 + 4));
    func_002D0918(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A22B8);

void func_002A2A60(u8 *work) {
    u8 *config = *(u8 **)(work + 0x34);
    u32 limit = *(u32 *)(work + 0x28);
    u32 progress = *(u32 *)(config + 0x34);
    u32 *list = *(u32 **)(work + 0x30);
    u8 *out = (u8 *)list[1];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = func_00296F58(config, config + 0x24, limit, progress);
    unit = 0x3C000000;
    color1[0] = *(s32 *)(work + 0x24);
    __asm__ volatile (
        ".set noreorder\n"
        "lw $2, 0(%1)\n"
        "pextlb $2, $0, $2\n"
        "pextlh $2, $0, $2\n"
        "qmtc2.ni $2, vf10\n"
        "vitof0.xyzw vf10, vf10\n"
        "qmtc2.ni %0, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmove.xyzw vf11, vf10\n"
        ".set reorder"
        : : "r"(unit), "r"(color1) : "$2", "memory");
    color2[0] = second;
    __asm__ volatile (
        ".set noreorder\n"
        "lw $2, 0(%1)\n"
        "pextlb $2, $0, $2\n"
        "pextlh $2, $0, $2\n"
        "qmtc2.ni $2, vf10\n"
        "vitof0.xyzw vf10, vf10\n"
        "qmtc2.ni %0, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmul.xyzw vf10, vf10, vf11\n"
        ".set reorder"
        : : "r"(unit), "r"(color2) : "$2", "memory");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %1\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vftoi0.xyzw vf10, vf10\n"
        "qmfc2.ni %0, vf10\n"
        "ppach %0, $0, %0\n"
        "ppacb %0, $0, %0\n"
        ".set reorder"
        : "=r"(packed) : "f"(128.0f) : "$2");
    blended[0] = packed;
    *(u32 *)(out + 4) = blended[0];
    *(u32 *)out = *(u32 *)(config + 0x28);
    *(u8 *)(out + 0x14) = *(u8 *)(config + 0x56);
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(work + 0x10));
    func_002E7D98();
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(D_0037E0E0));
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %0\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmulx.xyzw vf28, vf28, vf10x\n"
        "vmuly.xyzw vf29, vf29, vf10y\n"
        "vmulz.xyzw vf30, vf30, vf10z\n"
        "lqc2 vf10, 0(%1)\n"
        "vmove.w vf10, vf0\n"
        "vmove.xyzw vf31, vf10\n"
        "sqc2 vf28, 0(%2)\n"
        "sqc2 vf29, 0x10(%2)\n"
        "sqc2 vf30, 0x20(%2)\n"
        "sqc2 vf31, 0x30(%2)\n"
        ".set reorder"
        : : "f"(*(f32 *)(work + 0x20)), "r"(work), "r"(mtx) : "$2", "memory");
    func_002A3E10(out, mtx);
}

void billResetQuadIndices(u8 *work) {
    u8 *state = *(u8 **)(work + 0x30);
    u8 *config = *(u8 **)(work + 0x34);
    u8 *asset = *(u8 **)(state + 4);
    s32 remaining = *(s32 *)(config + 0x38);
    u8 *entry = *(u8 **)state;

    memset(*(void **)(asset + 0x1C), 0, *(u32 *)(asset + 8) * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            *(s32 *)entry = -1;
            entry += 0x20;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A2C40);

void billInitializeQuadRows(u8 *node, u8 *config) {
    u32 count = *(u32 *)(config + 0x38);
    u32 index;
    f32 *transform;

    if (count == 0) {
        return;
    }
    transform = *(f32 **)(*(u8 **)(node + 4) + 0x20);
    index = 0;
    for (; index < count; index++, transform += 8) {
        if (index & 1) {
            transform[0] = 0.0f;
            transform[1] = 0.0f;
            transform[2] = 0.5f;
            transform[3] = 0.0f;
            transform[4] = 0.0f;
            transform[5] = 1.0f;
            transform[6] = 0.5f;
        } else {
            transform[0] = 0.5f;
            transform[1] = 0.0f;
            transform[2] = 1.0f;
            transform[3] = 0.0f;
            transform[4] = 0.5f;
            transform[5] = 1.0f;
            transform[6] = 1.0f;
        }
        transform[7] = 1.0f;
    }
}

u8 *billCreateQuadTransform(u8 *config, u32 resource) {
    u8 *node = func_002A2C40(config);
    *(u32 *)(node + 4) = func_002A3BD8(*(u32 *)(config + 0x38), 4, resource);
    billInitializeQuadRows(node, config);
    return node;
}

u8 *billCloneQuadTransform(u8 *work) {
    u8 *config = *(u8 **)(work + 0x34);
    u8 *state = *(u8 **)(work + 0x30);
    u8 *node = func_002A2C40(config);
    *(u32 *)(node + 4) = func_002A3D68(*(u32 *)(state + 4));
    billInitializeQuadRows(node, config);
    return node;
}

void func_002A2DE8(s32 arg0) {
    releaseEffectResourceRefs(*(u32 *)(arg0 + 4));
    func_002D0918(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A2E18);

void func_002A34D8(u8 *work) {
    u8 *config = *(u8 **)(work + 0x34);
    u32 limit = *(u32 *)(work + 0x28);
    u32 progress = *(u32 *)(config + 0x34);
    u32 *list = *(u32 **)(work + 0x30);
    u8 *out = (u8 *)list[1];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = func_00296F58(config, config + 0x24, limit, progress);
    unit = 0x3C000000;
    color1[0] = *(s32 *)(work + 0x24);
    __asm__ volatile (
        ".set noreorder\n"
        "lw $2, 0(%1)\n"
        "pextlb $2, $0, $2\n"
        "pextlh $2, $0, $2\n"
        "qmtc2.ni $2, vf10\n"
        "vitof0.xyzw vf10, vf10\n"
        "qmtc2.ni %0, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmove.xyzw vf11, vf10\n"
        ".set reorder"
        : : "r"(unit), "r"(color1) : "$2", "memory");
    color2[0] = second;
    __asm__ volatile (
        ".set noreorder\n"
        "lw $2, 0(%1)\n"
        "pextlb $2, $0, $2\n"
        "pextlh $2, $0, $2\n"
        "qmtc2.ni $2, vf10\n"
        "vitof0.xyzw vf10, vf10\n"
        "qmtc2.ni %0, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmul.xyzw vf10, vf10, vf11\n"
        ".set reorder"
        : : "r"(unit), "r"(color2) : "$2", "memory");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %1\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vftoi0.xyzw vf10, vf10\n"
        "qmfc2.ni %0, vf10\n"
        "ppach %0, $0, %0\n"
        "ppacb %0, $0, %0\n"
        ".set reorder"
        : "=r"(packed) : "f"(128.0f) : "$2");
    blended[0] = packed;
    *(u32 *)(out + 4) = blended[0];
    *(u32 *)out = *(u32 *)(config + 0x28);
    *(u8 *)(out + 0x14) = *(u8 *)(config + 0x56);
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(work + 0x10));
    func_002E7D98();
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(D_0037E0E0));
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %0\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmulx.xyzw vf28, vf28, vf10x\n"
        "vmuly.xyzw vf29, vf29, vf10y\n"
        "vmulz.xyzw vf30, vf30, vf10z\n"
        "lqc2 vf10, 0(%1)\n"
        "vmove.w vf10, vf0\n"
        "vmove.xyzw vf31, vf10\n"
        "sqc2 vf28, 0(%2)\n"
        "sqc2 vf29, 0x10(%2)\n"
        "sqc2 vf30, 0x20(%2)\n"
        "sqc2 vf31, 0x30(%2)\n"
        ".set reorder"
        : : "f"(*(f32 *)(work + 0x20)), "r"(work), "r"(mtx) : "$2", "memory");
    func_002A3E10(out, mtx);
}

typedef struct EffectResourceSizeEntry {
    u32 resourceSize;
    u8 pad_04[0x18];
} EffectResourceSizeEntry;

extern EffectResourceSizeEntry D_0037E8B8[];

u8 *func_002A3640(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = D_0037E8B8[kind].resourceSize;
    u8 *effect = func_002CFEB8(size + headerSize);
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

u8 *func_002A36F8(u16 kind, void *source, u32 option) {
    u8 *effect = func_002A3640(kind, source);
    *(u32 *)(effect + 0x30) = D_0037E8A0[kind].createResource(source, option);
    D_0037E8A0[kind].fn(effect);
    return effect;
}

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
    return func_002A36F8(*(u16 *)(work + 0xC), source, (u32)secondary);
}

void effDispatchDestroyOp(u8 *work) {
    D_0037E8A8[*(s32 *)(work + 0x2c)].fn(*(void **)(work + 0x30));
    func_002CFF98(work);
}

u8 *createActiveEffectResource(u8 *work) {
    u8 *effect;
    if (D_0037E8A0[*(s32 *)(work + 0x2C)].createActiveResource == NULL) {
        effect = func_002A36F8(*(u16 *)(work + 0x2C), *(void **)(work + 0x34), 0);
    } else {
        effect = func_002A3640(*(u16 *)(work + 0x2C), *(void **)(work + 0x34));
        *(void **)(effect + 0x30) = (void *)D_0037E8A0[*(s32 *)(work + 0x2C)].createActiveResource(work);
        D_0037E8A0[*(s32 *)(work + 0x2C)].fn(effect);
    }
    return effect;
}

void func_002A3910(u8 *work) {
    D_0037E8A0[*(s32 *)(work + 0x2c)].fn();
    *(u32 *)(work + 0x28) = 0;
}

void func_002A3958(s32 arg0) {
    D_0037E8B0[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
    *(s32 *)(arg0 + 0x28) = *(s32 *)(arg0 + 0x28) + 1;
}

void func_002A39A8(s32 arg0) {
    D_0037E8B4[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
}

void func_002A39E0(u32 arg0) {
    func_002A3958(arg0);
    func_002A39A8(arg0);
}

void func_002A3A08(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002A3A18(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x10, src);
}

void func_002A3A30(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002A3A38(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2B10);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A3A40);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A3BD8);

void releaseEffectResourceRefs(u8 *work) {
    if (*(u32 *)(work + 0x20) != 0) {
        RefObj *ref = *(RefObj **)(work + 0x18);
        if (ref == NULL) {
            switch (*(u16 *)(work + 0xC)) {
            case 3:
                if (--D_003BC970[0] == 0) {
                    effReleaseSharedReference(D_003BC978[0]);
                    D_003BC978[0] = NULL;
                }
                break;
            case 4:
                if (--D_003BC970[1] == 0) {
                    effReleaseSharedReference(D_003BC978[1]);
                    D_003BC978[1] = NULL;
                }
                break;
            }
        } else {
            effReleaseSharedReference(ref);
        }
    }
    sdfQueueAssetRelease(*(u32 *)(work + 0x28));
    func_002D0918(*(u32 *)(work + 0x2C));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A3D68);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A3E10);

extern u32 D_003BC960[2];

void effLoadFlashTextures(void) {
    D_003BC960[0] = func_002EB028("/effect/flash00.tmx", &D_003BC968, 0);
    D_003BC960[1] = func_002EB028("/effect/flash01.tmx", &D_003BC968 + 1, 0);
}

u32 func_002A4378(s32 arg0) {
    return (&D_003BC968)[arg0];
}

void func_002A4390(s32 arg0) {
    *(u32 *)(**(s32 **)(arg0 + 0x30) + 4) = 0;
}

u32 *func_002A43A0(u8 *work) {
    u32 *handle = func_002CFEB8(4);
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
    ring = func_002A5540(kind);
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

void func_002A4448(u32 arg0) {
    func_002A5610(*(u32 *)arg0);
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A4478);

extern void func_002A5640(u8 *, void *);

void func_002A46E0(BillCellDrawWork *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = *(u32 *)(config + 0x34);
    u32 *list = work->instances;
    u8 *out = (u8 *)list[0];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = func_00296F58(config, config + 0x24, limit, progress);
    unit = 0x3C000000;
    color1[0] = work->baseColor;
    EE_MMI_RGBA_UNPACK(color1, unit);
    __asm__ volatile (".set noreorder\n\tvmove.xyzw vf11, vf10\n\t.set reorder");
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    __asm__ volatile (".set noreorder\n\tvmul.xyzw vf10, vf10, vf11\n\t.set reorder");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %1\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vftoi0.xyzw vf10, vf10\n"
        "qmfc2.ni %0, vf10\n"
        "ppach %0, $0, %0\n"
        "ppacb %0, $0, %0\n"
        ".set reorder"
        : "=r"(packed) : "f"(128.0f) : "$2");
    blended[0] = packed;
    *(u32 *)(out + 4) = blended[0];
    if ((packed & 0xFF000000) != 0) {
        *(u32 *)out = *(u32 *)(config + 0x28);
        *(u8 *)(out + 0xC) = *(u8 *)(config + 0x3C);
        __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(work->transform));
        func_002E7D98();
        __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(D_0037E0E0));
        __asm__ volatile (
            ".set noreorder\n"
            "mfc1 $2, %0\n"
            "qmtc2.ni $2, vf2\n"
            "vmulx.xyzw vf10, vf10, vf2x\n"
            "vmulx.xyzw vf28, vf28, vf10x\n"
            "vmuly.xyzw vf29, vf29, vf10y\n"
            "vmulz.xyzw vf30, vf30, vf10z\n"
            "lqc2 vf10, 0(%1)\n"
            "vmove.w vf10, vf0\n"
            "vmove.xyzw vf31, vf10\n"
            "sqc2 vf28, 0(%2)\n"
            "sqc2 vf29, 0x10(%2)\n"
            "sqc2 vf30, 0x20(%2)\n"
            "sqc2 vf31, 0x30(%2)\n"
            ".set reorder"
            : : "f"(work->scale), "r"(work), "r"(mtx) : "$2", "memory");
        func_002A5640(out, mtx);
    }
}

void func_002A4850(s32 arg0) {
    u32 temp_v0;

    temp_v0 = *(u32 *)(*(s32 *)(arg0 + 0x30) + 4);
    *(u32 *)(*(s32 *)(*(s32 *)(arg0 + 0x30) + 8) + 4) = 0;
    func_002A5410(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A4878);

void func_002A4AB8(s32 arg0) {
    releaseEffectResourceRefs(*(u32 *)(arg0 + 8));
    func_002A53A8(*(u32 *)(arg0 + 4));
    func_002D0918(*(u32 *)(arg0 + 0xc));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A4AF0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A4C40);

void func_002A4DE8(s32 arg0) {
    *(u32 *)(**(s32 **)(arg0 + 0x30) + 4) = 0;
}

extern u8 *func_002A5540(u32);

u32 *createEffectRingHandle(u8 *work) {
    u32 *handle = func_002CFEB8(4);
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
    ring = func_002A5540(kind);
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

void func_002A4EA0(u32 arg0) {
    func_002A5610(*(u32 *)arg0);
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A4ED0);

void func_002A5118(BillCellDrawWork *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = *(u32 *)(config + 0x34);
    u32 *list = work->instances;
    u8 *out = (u8 *)list[0];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = func_00296F58(config, config + 0x24, limit, progress);
    unit = 0x3C000000;
    color1[0] = work->baseColor;
    EE_MMI_RGBA_UNPACK(color1, unit);
    __asm__ volatile (".set noreorder\n\tvmove.xyzw vf11, vf10\n\t.set reorder");
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    __asm__ volatile (".set noreorder\n\tvmul.xyzw vf10, vf10, vf11\n\t.set reorder");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %1\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vftoi0.xyzw vf10, vf10\n"
        "qmfc2.ni %0, vf10\n"
        "ppach %0, $0, %0\n"
        "ppacb %0, $0, %0\n"
        ".set reorder"
        : "=r"(packed) : "f"(128.0f) : "$2");
    blended[0] = packed;
    *(u32 *)(out + 4) = blended[0];
    if ((packed & 0xFF000000) != 0) {
        *(u32 *)out = *(u32 *)(config + 0x28);
        *(u8 *)(out + 0xC) = *(u8 *)(config + 0x3C);
        __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(work->transform));
        func_002E7D98();
        __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(D_0037E0E0));
        __asm__ volatile (
            ".set noreorder\n"
            "mfc1 $2, %0\n"
            "qmtc2.ni $2, vf2\n"
            "vmulx.xyzw vf10, vf10, vf2x\n"
            "vmulx.xyzw vf28, vf28, vf10x\n"
            "vmuly.xyzw vf29, vf29, vf10y\n"
            "vmulz.xyzw vf30, vf30, vf10z\n"
            "lqc2 vf10, 0(%1)\n"
            "vmove.w vf10, vf0\n"
            "vmove.xyzw vf31, vf10\n"
            "sqc2 vf28, 0(%2)\n"
            "sqc2 vf29, 0x10(%2)\n"
            "sqc2 vf30, 0x20(%2)\n"
            "sqc2 vf31, 0x30(%2)\n"
            ".set reorder"
            : : "f"(work->scale), "r"(work), "r"(mtx) : "$2", "memory");
        func_002A5640(out, mtx);
    }
}

typedef struct EffectResourceSizeEntry24 {
    u32 resourceSize;
    u8 pad_04[0x14];
} EffectResourceSizeEntry24;

extern EffectResourceSizeEntry24 D_0037EAE4[];

u8 *func_002A5288(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = D_0037EAE4[kind].resourceSize;
    u8 *effect = func_002CFEB8(size + headerSize);
    *(u8 **)(effect + 0x34) = effect + headerSize;
    *(u32 *)(effect + 0x24) = 0x80808080;
    *(float *)(effect + 0x20) = 1.0f;
    *(u32 *)(effect + 0x28) = 0;
    *(u32 *)(effect + 0x2C) = kind;
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" : : "r"(effect));
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" : : "r"(effect + 0x10));
    memcpy(*(void **)(effect + 0x34), source, size);
    *(u32 *)(effect + 0x30) = D_0037EAD0[kind].createResource(source);
    D_0037EAD0[kind].fn(effect);
    return effect;
}

void func_002A5378(s32 arg0) {
    void *temp_v0;

    temp_v0 = fileResolvePrimaryBuffer();
    func_002A5288(*(u16 *)(arg0 + 0xc), temp_v0);
}

void func_002A53A8(u8 *work) {
    D_0037EAD8[*(s32 *)(work + 0x2c)].fn(*(void **)(work + 0x30));
    func_002CFF98(work);
}

void func_002A53F0(s32 arg0) {
    func_002A5288(*(u16 *)(arg0 + 0x2c), *(u32 *)(arg0 + 0x34));
}

void func_002A5410(u8 *work) {
    D_0037EAD0[*(s32 *)(work + 0x2c)].fn();
    *(u32 *)(work + 0x28) = 0;
}

void func_002A5458(s32 arg0) {
    D_0037EADC[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
    *(s32 *)(arg0 + 0x28) = *(s32 *)(arg0 + 0x28) + 1;
}

void func_002A54A8(s32 arg0) {
    D_0037EAE0[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
}

void func_002A54E0(u32 arg0) {
    func_002A5458(arg0);
    func_002A54A8(arg0);
}

void func_002A5508(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002A5518(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x10, src);
}

void func_002A5530(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002A5538(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A5540);

void func_002A5610(s32 arg0) {
    sdfQueueAssetRelease(*(u32 *)(arg0 + 0x18));
    func_002D0918(*(u32 *)(arg0 + 0x1c));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A5640);

typedef struct EffectSurfaceNode {
    u32 percent;
    u32 color;
    f32 opacity;
    u32 kind;
    u8 pad_10[0x1C];
    u32 index;
    u32 pad_30;
    void *resource;
    u8 pad_38[0x0C];
    u32 active;
    u16 count;
} EffectSurfaceNode;

EffectSurfaceNode *func_002A5A00(u32 percent) {
    EffectSurfaceNode *node = func_002CFF68(sizeof(EffectSurfaceNode));
    node->percent = percent;
    node->color = 0x80808080;
    node->opacity = 1.0f;
    node->index = 0;
    node->active = 0;
    node->count = 1;
    return node;
}

EffectSurfaceNode *func_002A5A58(EffGrid *grid) {
    u32 n;
    s32 w = grid->width;
    n = (w ? w : grid->height) * (w ? grid->height : grid->altHeight);
    return func_002A5A00(n > 100 ? 100 : n);
}

EffectSurfaceNode *func_002A5A90(u8 *work) {
    u8 *source = fileResolvePrimaryBuffer(work);
    EffGrid *params = (EffGrid *)(source + 0x1C);
    EffectSurfaceNode *node = func_002A5A58(params);

    func_002A5F80(node, *(u16 *)(work + 0xC), source);
    effReplaceResourceRef(node, *(u16 *)(work + 0xC), params);
    return node;
}

extern EffectSurfaceNode *func_002A5A90(u8 *);

extern void func_002A6298(EffectSurfaceNode *, void *);

extern void func_002A6390(s32, u32);

EffectSurfaceNode *effInitializeSurfaceForKind(u8 *work) {
    EffectSurfaceNode *node = func_002A5A90(work);
    u32 *secondary = fileResolveSecondaryBuffer(work);

    if (secondary == NULL) {
        return node;
    }
    switch (*(u16 *)(work + 0x1C)) {
    case 1:
        func_002A6198(node, (u32)secondary);
        break;
    case 2:
        func_002A6218(node, (u32)secondary);
        break;
    case 3:
        break;
    case 4:
        func_002A6120(node, *secondary);
        break;
    case 5:
        func_002A6298(node, secondary);
        break;
    case 6:
        break;
    case 7:
        func_002A6390((s32)node, (u32)secondary);
        break;
    }
    node->kind = *(u16 *)(work + 0x1C);
    return node;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A5BD8);


extern void func_002A5F80(EffectSurfaceNode *, u16, void *);

EffectSurfaceNode *func_002A5CF0(u8 *work) {
    EffGrid *params = *(EffGrid **)(*(u8 **)(work + 0x44) + 0x24);
    EffectSurfaceNode *node = func_002A5A58(params);
    func_002A5F80(node, *(u16 *)(*(u8 **)(work + 0x44)), work + 0x10);
    effReplaceResourceRef(node, *(u16 *)(*(u8 **)(work + 0x44)), params);
    return node;
}

extern void func_002A5DE0(EffectSurfaceNode *, u8 *);

EffectSurfaceNode *func_002A5D60(u8 *work) {
    EffGrid *params = *(EffGrid **)(*(u8 **)(work + 0x44) + 0x24);
    EffectSurfaceNode *node = func_002A5A58(params);
    func_002A5F80(node, *(u16 *)(*(u8 **)(work + 0x44)), work + 0x10);
    effReplaceResourceRef(node, *(u16 *)(*(u8 **)(work + 0x44)), params);
    func_002A5DE0(node, work);
    return node;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A5DE0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A5F80);

extern u32 func_0029A5E0(u16, u32, void *);

void effReplaceResourceRef(EffectSurfaceNode *node, u32 entryId, void *resource) {
    if (node->active != 0) {
        func_0029A730(node->active);
    }
    node->active = func_0029A5E0((u16)entryId, node->percent, resource);
}

extern u32 effRetainResource(u32);

void func_002A6120(EffectSurfaceNode *node, u32 resourceId) {
    u32 resource = node->resource;
    if (resource != 0) {
        billDispatchByKind((void *)resource);
    }
    resource = effRetainResource(resourceId);
    node->resource = (void *)resource;
    if (node->active != 0) {
        effBillSetMode(resource, *(s16 *)(*(u32 *)(node->active + 0x20) + 0x54));
    }
}

extern u32 billCreateIndexed(u32, u32);

void func_002A6198(EffectSurfaceNode *node, u32 resourceId) {
    u32 resource = node->resource;
    if (resource != 0) {
        billDispatchByKind((void *)resource);
    }
    resource = billCreateIndexed(0, resourceId);
    node->resource = (void *)resource;
    if (node->active != 0) {
        effBillSetMode(resource, *(s16 *)(*(u32 *)(node->active + 0x20) + 0x54));
    }
}

void func_002A6218(EffectSurfaceNode *node, u32 resourceId) {
    u32 resource = node->resource;
    if (resource != 0) {
        billDispatchByKind((void *)resource);
    }
    resource = billCreateIndexed(1, resourceId);
    node->resource = (void *)resource;
    func_001523B0(resource);
    if (node->active != 0) {
        effBillSetMode(node->resource, *(s16 *)(*(u32 *)(node->active + 0x20) + 0x54));
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A6298);

void func_002A6390(s32 arg0, u32 arg1) {
    u32 temp_v0;

    if (*(s32 *)(arg0 + 0x40) != 0) {
        effReleaseReferenceHolder(*(s32 *)(arg0 + 0x40));
    }
    temp_v0 = func_0029C230(arg1);
    *(u32 *)(arg0 + 0x40) = temp_v0;
}

void func_002A63E0(s32 arg0) {
    if (*(s32 *)(arg0 + 0x44) != 0) {
        fileClearRecordReferences(*(s32 *)(arg0 + 0x44));
        return;
    }
}

void func_002A6410(s32 arg0) {
    if (*(s32 *)(arg0 + 0x44) != 0) {
        fileAcquireRecord(*(s32 *)(arg0 + 0x44));
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A6440);

void func_002A70B0(s32 arg0) {
    func_002A6410(arg0);
    func_002A6440(arg0);
}

void func_002A70D8(s32 arg0) {
    menuRecordSetVector(*(u32 *)(arg0 + 0x44));
}

void func_002A70F0(s32 arg0) {
    func_0029A7F8(*(u32 *)(arg0 + 0x44));
}

void func_002A7108(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 4) = arg1;
}

void func_002A7110(ValPtr44 *p, float v) {
    p->f08 = v;
    dds3DispatchIndexedCallback(p->p44, v);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A7130);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A7288);

void func_002A74B0(s32 arg0) {
    sdfQueueAssetRelease(*(u32 *)(arg0 + 0x2c));
    func_002D0918(*(u32 *)(arg0 + 0x38));
}

void func_002A74E0(s32 arg0) {
    *(u32 *)(arg0 + 0x14) = 0;
    *(u32 *)(arg0 + 0x18) = 3;
}

void effCopyRingFrameVectors(u8 *work, u128 *destination, s32 frame) {
    s32 index = *(s32 *)(work + 0x18) - 3 * (*(s32 *)(work + 0x1C) * (frame - 1) + frame);
    u128 *source;
    s32 i;

    if (index < 3) {
        index += *(s32 *)(work + 0x10) - 3;
    }
    source = *(u128 **)(work + 0x24) + index;
    for (i = 0; i < 3; i++) {
        PCP_COPY_VECTOR(destination + i, source + i);
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A7568);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A7610);

s32 func_002A7890(const Matrix4 *matrix) {
    void *work = sdfAllocPacketAligned(0x20);
    D_003BC980 = (u32)work;
    sdfInitPacketList(work);
    __asm__ volatile(
        ".set noreorder\n\t"
        "lqc2 $vf28, 0(%0)\n\t"
        "lqc2 $vf29, 16(%0)\n\t"
        "lqc2 $vf30, 32(%0)\n\t"
        "lqc2 $vf31, 48(%0)\n\t"
        ".set reorder" :: "r"(matrix) : "memory");
    return sdfConsAppendVuPacket(D_003BC980, 0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A78E0);

void func_002A7B30(u32 index) {
    u8 *entry = D_0037EBF8[index];
    ((void (*)(u8 *, u32))*(void **)(entry + 0x10))(entry, D_003BC980);
    D_003BC980 = 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A7B68);

void effReleaseRenderResources(u8 *work) {
    if (*(void **)(work + 0xC8) != NULL) {
        billDispatchByKind(*(void **)(work + 0xC8));
    }
    if (*(void **)(work + 0xCC) != NULL) {
        effReleaseReferenceHolder(*(void **)(work + 0xCC));
    }
    if (*(void **)(work + 0xD0) != NULL) {
        sdfQueueAssetRelease(*(void **)(work + 0xD0));
    }
    func_002CFF98(work);
}

u8 *func_002A7E58(u8 *source) {
    u8 *effect = func_002A7B68(NULL);
    memcpy(effect + 0x30, source + 0x30, 0x98);
    func_002A7F78(effect, source);
    return effect;
}

extern void func_001523B0(u32);

extern void effBillSetMode(u32, s16);

void func_002A7F78(u8 *work, u8 *source) {
    u32 resource;

    if (*(u32 *)(source + 0xC8) != 0) {
        if (*(u32 *)(work + 0xC8) != 0) {
            billDispatchByKind(*(void **)(work + 0xC8));
        }
        resource = func_00151E60(*(u32 *)(source + 0xC8));
        *(u32 *)(work + 0xC8) = resource;
        func_001523B0(resource);
        effBillSetMode(*(u32 *)(work + 0xC8), *(s16 *)(work + 0x58));
        return;
    }
    if (*(RefObj **)(work + 0xCC) != NULL) {
        effReleaseReferenceHolder(*(RefObj **)(work + 0xCC));
    }
    *(RefObj **)(work + 0xCC) = func_0029C408(*(RefObj **)(source + 0xCC));
}

void func_002A8018(s32 arg0) {
    *(u32 *)(arg0 + 0x2c) = 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A8020);

void func_002A85B0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002A85C0(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x10, src);
}

void func_002A85D8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002A85E0(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

extern s8 D_00324550[];

extern u32 effMiscRand(void *);

void randomizeEffectParticleFields(u8 *work) {
    u32 index = 0;
    u32 count = *(u32 *)(*(u8 **)(work + 0x34) + 0x38);
    u8 *entry = *(u8 **)(*(u8 **)(work + 0x30));

    if (count != 0) {
        do {
            *(s32 *)(entry + 4) = -1 - (effMiscRand(D_00324550) & 3);
            entry += 0x10;
            index++;
        } while (index < count);
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A8670);

extern void func_002AACD0(s32);

void effFreeIndexedEntries(u8 *work) {
    u32 index = 0;
    u32 count = *(u32 *)(*(u8 **)(work + 0x34) + 0x38);
    u8 *pool = *(u8 **)(work + 0x30);
    u8 *entry = *(u8 **)pool;

    if (count != 0) {
        do {
            func_002AACD0(*(s32 *)entry);
            entry += 0x10;
            index++;
        } while (index < count);
    }
    func_002CFF98(pool);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A8A90);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A9160);

extern void effResetDispatchCounter(u8 *);

void func_002A9340(u8 *work) {
    u32 index = 0;
    u32 count = *(u32 *)(*(u8 **)(work + 0x34) + 0x38);
    u8 *entry = *(u8 **)(*(u8 **)(work + 0x30));

    if (count != 0) {
        do {
            effResetDispatchCounter(*(u8 **)entry);
            entry += 4;
            index++;
        } while (index < count);
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A93A0);

extern void func_002AAA48(u8 *);

void func_002A95C0(u8 *work) {
    u8 *config = *(u8 **)(work + 0x34);
    u8 *state = *(u8 **)(work + 0x30);
    u32 count = *(u32 *)(config + 0x38);
    u32 index = 0;
    u8 *entry = *(u8 **)state;

    if (count != 0) {
        do {
            func_002AAA48(*(u8 **)entry);
            entry += 4;
        } while (++index < count);
    }
    func_002CFF98(state);
}

extern void func_002AAAF8(s32);

void func_002A9630(u8 *work) {
    u32 index = 0;
    u32 count = *(u32 *)(*(u8 **)(work + 0x34) + 0x38);
    u8 *entry = *(u8 **)(*(u8 **)(work + 0x30));

    if (count != 0) {
        do {
            func_002AAAF8(*(s32 *)entry);
            entry += 4;
            index++;
        } while (index < count);
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A9690);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A9860);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A9978);

extern void func_002AACD0(s32);

void func_002A9D38(u8 *work) {
    u8 *config = *(u8 **)(work + 0x34);
    u8 *state = *(u8 **)(work + 0x30);
    u32 count = *(u32 *)(config + 0x38);
    u32 index = 0;
    u8 *entry = *(u8 **)state;

    if (count != 0) {
        do {
            func_002AACD0(*(s32 *)entry);
            entry += 0x30;
        } while (++index < count);
    }
    func_002D0918(*(u32 *)(state + 0xC));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002A9DA8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AA748);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AA928);

void func_002AAA18(s32 arg0) {
    u64 temp_v0;

    temp_v0 = fileResolvePrimaryBuffer();
    func_002AA928(*(u16 *)(arg0 + 0xc), temp_v0);
}

void func_002AAA48(u8 *work) {
    D_0037EC58[*(s32 *)(work + 0x2c)].fn();
    func_002CFF98(work);
}

void func_002AAA90(s32 arg0) {
    func_002AA928(*(u16 *)(arg0 + 0x2c), *(u32 *)(arg0 + 0x34));
}

void effResetDispatchCounter(u8 *work) {
    D_0037EC50[*(s32 *)(work + 0x2C)].fn(work);
    *(u32 *)(work + 0x28) = 0;
}

void func_002AAAF8(s32 arg0) {
    D_0037EC5C[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
    *(s32 *)(arg0 + 0x28) = *(s32 *)(arg0 + 0x28) + 1;
}

void func_002AAB48(s32 arg0) {
    D_0037EC60[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
}

void func_002AAB80(u32 arg0) {
    func_002AAAF8(arg0);
    func_002AAB48(arg0);
}

void func_002AABA8(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002AABB8(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x10, src);
}

void func_002AABD0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002AABD8(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AABE0);

void func_002AACD0(s32 arg0) {
    sdfQueueAssetRelease(*(u32 *)(arg0 + 0x18));
    func_002D0918(*(u32 *)(arg0 + 0x1c));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AAD00);

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

EffectStripNode *func_002AAF70(u32 percent) {
    EffectStripNode *node = func_002CFF68(sizeof(EffectStripNode));
    node->percent = percent;
    node->color = 0x80808080;
    node->opacity = 1.0f;
    node->active = 0;
    node->transform = func_002A3BD8(percent * 4, 2, 0);
    node->resource = effRetainResource(0);
    node->count = 1;
    return node;
}

EffectStripNode *func_002AAFF0(EffGrid *grid) {
    u32 n;
    s32 w = grid->width;
    n = (w ? w : grid->height) * (w ? grid->height : grid->altHeight);
    return func_002AAF70(n > 100 ? 100 : n);
}

EffectStripNode *func_002AB028(u8 *work) {
    u8 *source = fileResolvePrimaryBuffer(work);
    EffGrid *params = (EffGrid *)(source + 0x20);
    EffectStripNode *node = func_002AAFF0(params);

    memcpy(node->pad_0C, source, sizeof(node->pad_0C));
    effReplaceFileResourceRef(node, *(u16 *)(work + 0xC), params);
    return node;
}

void effReleaseModelResources(u8 *work) {
    void *resource = *(void **)(work + 0x30);
    if (resource != NULL) {
        billDispatchByKind(resource);
    }
    if (*(u32 *)(work + 0x2C) != 0) {
        releaseEffectResourceRefs(*(u32 *)(work + 0x2C));
    }
    if (*(u32 *)(work + 0x34) != 0) {
        func_0029A730(*(u32 *)(work + 0x34));
    }
    func_002CFF98(work);
}

extern void effReplaceFileResourceRef(EffectStripNode *, u32, void *);

EffectStripNode *func_002AB130(u8 *work) {
    EffGrid *params = *(EffGrid **)(*(u8 **)(work + 0x34) + 0x24);
    EffectStripNode *node = func_002AAFF0(params);
    memcpy(node->pad_0C, params, sizeof(node->pad_0C));
    effReplaceFileResourceRef(node, *(u16 *)(*(u8 **)(work + 0x34)), params);
    return node;
}

void effReplaceFileResourceRef(EffectStripNode *node, u32 entryId, void *resource) {
    if (node->active != 0) {
        func_0029A730(node->active);
    }
    node->active = func_0029A5E0((u16)entryId, node->percent, resource);
}

void func_002AB230(s32 arg0) {
    if (*(s32 *)(arg0 + 0x34) != 0) {
        fileClearRecordReferences(*(s32 *)(arg0 + 0x34));
        return;
    }
}

void func_002AB260(s32 arg0) {
    if (*(s32 *)(arg0 + 0x34) != 0) {
        fileAcquireRecord(*(s32 *)(arg0 + 0x34));
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AB290);

void func_002AB968(s32 arg0) {
    func_002AB260(arg0);
    func_002AB290(arg0);
}

void func_002AB990(s32 arg0) {
    menuRecordSetVector(*(u32 *)(arg0 + 0x34));
}

void func_002AB9A8(s32 arg0) {
    func_0029A7F8(*(u32 *)(arg0 + 0x34));
}

void func_002AB9C0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 4) = arg1;
}

void func_002AB9C8(ValPtr34 *p, float v) {
    p->f08 = v;
    dds3DispatchIndexedCallback(p->p34, v);
}

void effResetBillTable(u8 *work) {
    u32 index = 0;
    u8 *state = *(u8 **)(work + 0x30);
    u32 count = *(u32 *)(*(u8 **)(work + 0x34) + 0x38);
    s32 *entry = *(s32 **)state;
    s32 *flags = *(s32 **)(*(u8 **)(state + 4) + 0x1C);

    if (count != 0) {
        do {
            index++;
            *flags++ = 0;
            *entry = -1;
            entry = (s32 *)((u8 *)entry + 0x30);
        } while (index < count);
    }
}

typedef struct EffectNodeHeader {
    u8 *entries;
    u32 unk_04;
    u8 *allocation;
} EffectNodeHeader;

u8 *func_002ABA38(u8 *config) {
    u8 *base = func_002D03F8(*(u32 *)(config + 0x38) * 0x30 + 0xC);
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

/* Ring fade tables of the ring effects: `segments + 1` entries of colors
 * (4 words each) and radii (8 words each). The alpha fades in over the first
 * fadeIn fraction of the entries and out from the fadeOut fraction; the table
 * is then copied for every remaining layer. */
typedef struct EffectFadeTable {
    u8 pad00[0x24];
    f32 *radii;
    u32 *colors;
} EffectFadeTable;

typedef struct EffectFadeConfig {
    u8 pad00[0x38];
    u32 layers;
    u8 pad3C[0x3C];
    f32 fadeIn;
    f32 fadeOut;
    u8 pad80[0xC];
    s32 segments;
    f32 radius;
} EffectFadeConfig;

void func_002ABAA8(u8 *node, u8 *config) {
    EffectFadeConfig *cfg = (EffectFadeConfig *)config;
    EffectFadeTable *table;
    u32 *colors;
    f32 *radii;
    u32 *colorsStart;
    f32 *radiiStart;
    u32 layers;
    s32 segments;
    s32 fadeInEnd;
    s32 fadeOutStart;
    u32 entries;
    u32 words;
    f32 radius;
    f32 fadeIn;
    f32 fadeOut;
    f32 fade;
    u32 alpha;
    u32 color;
    u32 i;

    layers = cfg->layers;
    if (layers == 0) {
        return;
    }
    segments = cfg->segments;
    entries = segments + 1;
    words = entries * 4;
    table = *(EffectFadeTable **)(node + 4);
    colors = table->colors;
    radii = table->radii;
    colorsStart = colors;
    radiiStart = radii;
    fadeOut = cfg->fadeOut;
    fadeIn = cfg->fadeIn;
    fadeInEnd = (s32)(fadeIn * (f32)segments);
    fadeOutStart = (s32)(fadeOut * (f32)segments);
    radius = cfg->radius / 3.0f;
    for (i = 0; i < entries; i++) {
        if (i < fadeInEnd) {
            fade = (f32)i / (f32)fadeInEnd;
        } else {
            fade = 1.0f;
            if (fadeOutStart < i) {
                fade = (f32)(segments - i) / (f32)(segments - fadeOutStart);
            }
        }
        alpha = (u32)(fade * 128.0f);
        color = (alpha << 24) | 0x808080;
        colors[0] = 0x808080;
        colors[1] = color;
        colors[2] = color;
        colors[3] = 0x808080;
        radii[0] = 0.0f;
        radii[2] = radius;
        radii[4] = radius * 2.0f;
        radii[6] = radius * 3.0f;
        colors += 4;
        radii += 8;
    }
    for (i = 1; i < layers; i++) {
        memcpy(colors, colorsStart, words * 4);
        colors += words;
        memcpy(radii, radiiStart, words * 8);
        radii += words * 2;
    }
}

extern u32 func_002AE350(u32, u32, u32);

u8 *func_002ABCF0(u8 *config, u32 resource) {
    u8 *node = func_002ABA38(config);

    *(u32 *)(node + 4) = func_002AE350(*(u32 *)(config + 0x38), *(u32 *)(config + 0x8C), resource);
    func_002ABAA8(node, config);
    return node;
}

extern u32 func_002AE430(u32);

u8 *func_002ABD50(u8 *work) {
    u8 *config = *(u8 **)(work + 0x34);
    u8 *state = *(u8 **)(work + 0x30);
    u8 *node = func_002ABA38(config);
    *(u32 *)(node + 4) = func_002AE430(*(u32 *)(state + 4));
    func_002ABAA8(node, config);
    return node;
}

void func_002ABDB0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x30);
    func_002AE3C8(*(u32 *)(temp_v0 + 4));
    func_002D0918(*(u32 *)(temp_v0 + 8));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002ABDE0);

extern void func_002AE498(u8 *, void *);

void func_002AC4E0(u8 *work) {
    u8 *config = *(u8 **)(work + 0x34);
    u32 limit = *(u32 *)(work + 0x28);
    u32 progress = *(u32 *)(config + 0x34);
    u32 *list = *(u32 **)(work + 0x30);
    u8 *out = (u8 *)list[1];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = func_00296F58(config, config + 0x24, limit, progress);
    unit = 0x3C000000;
    color1[0] = *(s32 *)(work + 0x24);
    EE_MMI_RGBA_UNPACK(color1, unit);
    __asm__ volatile (".set noreorder\n\tvmove.xyzw vf11, vf10\n\t.set reorder");
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    __asm__ volatile (".set noreorder\n\tvmul.xyzw vf10, vf10, vf11\n\t.set reorder");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %1\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vftoi0.xyzw vf10, vf10\n"
        "qmfc2.ni %0, vf10\n"
        "ppach %0, $0, %0\n"
        "ppacb %0, $0, %0\n"
        ".set reorder"
        : "=r"(packed) : "f"(128.0f) : "$2");
    blended[0] = packed;
    *(u32 *)(out + 8) = blended[0];
    *(u32 *)(out + 4) = *(u32 *)(config + 0x28);
    *(u8 *)(out + 0x14) = *(u8 *)(config + 0xB9);
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(work + 0x10));
    func_002E7D98();
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(D_0037E0E0));
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %0\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmulx.xyzw vf28, vf28, vf10x\n"
        "vmuly.xyzw vf29, vf29, vf10y\n"
        "vmulz.xyzw vf30, vf30, vf10z\n"
        "lqc2 vf10, 0(%1)\n"
        "vmove.w vf10, vf0\n"
        "vmove.xyzw vf31, vf10\n"
        "sqc2 vf28, 0(%2)\n"
        "sqc2 vf29, 0x10(%2)\n"
        "sqc2 vf30, 0x20(%2)\n"
        "sqc2 vf31, 0x30(%2)\n"
        ".set reorder"
        : : "f"(*(f32 *)(work + 0x20)), "r"(work), "r"(mtx) : "$2", "memory");
    func_002AE498(out, mtx);
}

void func_002AC648(u8 *work) {
    u32 index = 0;
    u8 *state = *(u8 **)(work + 0x30);
    u32 count = *(u32 *)(*(u8 **)(work + 0x34) + 0x38);
    s32 *entry = *(s32 **)state;
    s32 *flags = *(s32 **)(*(u8 **)(state + 4) + 0x1C);

    if (count != 0) {
        do {
            index++;
            *flags++ = 0;
            *entry = -1;
            entry = (s32 *)((u8 *)entry + 0x30);
        } while (index < count);
    }
}

u8 *func_002AC698(u8 *config) {
    u8 *base = func_002D03F8(*(u32 *)(config + 0x38) * 0x30 + 0xC);
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

void func_002AC708(u8 *node, u8 *config) {
    EffectFadeConfig *cfg = (EffectFadeConfig *)config;
    EffectFadeTable *table;
    u32 *colors;
    f32 *radii;
    u32 *colorsStart;
    f32 *radiiStart;
    u32 layers;
    s32 segments;
    s32 fadeInEnd;
    s32 fadeOutStart;
    u32 entries;
    u32 words;
    f32 radius;
    f32 fadeIn;
    f32 fadeOut;
    f32 fade;
    u32 alpha;
    u32 color;
    u32 i;

    layers = cfg->layers;
    if (layers == 0) {
        return;
    }
    segments = cfg->segments;
    entries = segments + 1;
    words = entries * 4;
    table = *(EffectFadeTable **)(node + 4);
    colors = table->colors;
    radii = table->radii;
    colorsStart = colors;
    radiiStart = radii;
    fadeOut = cfg->fadeOut;
    fadeIn = cfg->fadeIn;
    fadeInEnd = (s32)(fadeIn * (f32)segments);
    fadeOutStart = (s32)(fadeOut * (f32)segments);
    radius = cfg->radius / 3.0f;
    for (i = 0; i < entries; i++) {
        if (i < fadeInEnd) {
            fade = (f32)i / (f32)fadeInEnd;
        } else {
            fade = 1.0f;
            if (fadeOutStart < i) {
                fade = (f32)(segments - i) / (f32)(segments - fadeOutStart);
            }
        }
        alpha = (u32)(fade * 128.0f);
        color = (alpha << 24) | 0x808080;
        colors[0] = 0x808080;
        colors[1] = color;
        colors[2] = color;
        colors[3] = 0x808080;
        radii[0] = 0.0f;
        radii[2] = radius;
        radii[4] = radius * 2.0f;
        radii[6] = radius * 3.0f;
        colors += 4;
        radii += 8;
    }
    for (i = 1; i < layers; i++) {
        memcpy(colors, colorsStart, words * 4);
        colors += words;
        memcpy(radii, radiiStart, words * 8);
        radii += words * 2;
    }
}

u8 *func_002AC950(u8 *config, u32 resource) {
    u8 *node = func_002AC698(config);

    *(u32 *)(node + 4) = func_002AE350(*(u32 *)(config + 0x38), *(u32 *)(config + 0x8C), resource);
    func_002AC708(node, config);
    return node;
}

u8 *func_002AC9B0(u8 *work) {
    u8 *config = *(u8 **)(work + 0x34);
    u8 *state = *(u8 **)(work + 0x30);
    u8 *node = func_002AC698(config);
    *(u32 *)(node + 4) = func_002AE430(*(u32 *)(state + 4));
    func_002AC708(node, config);
    return node;
}

void func_002ACA10(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x30);
    func_002AE3C8(*(u32 *)(temp_v0 + 4));
    func_002D0918(*(u32 *)(temp_v0 + 8));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002ACA40);

void func_002AD150(BillCellDrawWork *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = *(u32 *)(config + 0x34);
    u32 *list = work->instances;
    u8 *out = (u8 *)list[1];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = func_00296F58(config, config + 0x24, limit, progress);
    unit = 0x3C000000;
    color1[0] = work->baseColor;
    EE_MMI_RGBA_UNPACK(color1, unit);
    __asm__ volatile (".set noreorder\n\tvmove.xyzw vf11, vf10\n\t.set reorder");
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    __asm__ volatile (".set noreorder\n\tvmul.xyzw vf10, vf10, vf11\n\t.set reorder");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %1\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vftoi0.xyzw vf10, vf10\n"
        "qmfc2.ni %0, vf10\n"
        "ppach %0, $0, %0\n"
        "ppacb %0, $0, %0\n"
        ".set reorder"
        : "=r"(packed) : "f"(128.0f) : "$2");
    blended[0] = packed;
    *(u32 *)(out + 8) = blended[0];
    *(u32 *)(out + 4) = *(u32 *)(config + 0x28);
    *(u8 *)(out + 0x14) = *(u8 *)(config + 0xB9);
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(work->transform));
    func_002E7D98();
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(D_0037E0E0));
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %0\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmulx.xyzw vf28, vf28, vf10x\n"
        "vmuly.xyzw vf29, vf29, vf10y\n"
        "vmulz.xyzw vf30, vf30, vf10z\n"
        "lqc2 vf10, 0(%1)\n"
        "vmove.w vf10, vf0\n"
        "vmove.xyzw vf31, vf10\n"
        "sqc2 vf28, 0(%2)\n"
        "sqc2 vf29, 0x10(%2)\n"
        "sqc2 vf30, 0x20(%2)\n"
        "sqc2 vf31, 0x30(%2)\n"
        ".set reorder"
        : : "f"(work->scale), "r"(work), "r"(mtx) : "$2", "memory");
    func_002AE498(out, mtx);
}

void func_002AD2B8(u8 *work) {
    u32 index = 0;
    u8 *state = *(u8 **)(work + 0x30);
    u32 count = *(u32 *)(*(u8 **)(work + 0x34) + 0x38);
    s32 *entry = *(s32 **)state;
    s32 *flags = *(s32 **)(*(u8 **)(state + 4) + 0x1C);

    if (count != 0) {
        do {
            index++;
            *flags++ = 0;
            *entry = -1;
            entry = (s32 *)((u8 *)entry + 0x2C);
        } while (index < count);
    }
}

u8 *func_002AD308(u8 *config) {
    u8 *base = func_002D03F8(*(u32 *)(config + 0x38) * 0x2C + 0xC);
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

void func_002AD380(u8 *node, u8 *config) {
    EffectFadeConfig *cfg = (EffectFadeConfig *)config;
    EffectFadeTable *table;
    u32 *colors;
    f32 *radii;
    u32 *colorsStart;
    f32 *radiiStart;
    u32 layers;
    s32 segments;
    s32 fadeInEnd;
    s32 fadeOutStart;
    u32 entries;
    u32 words;
    f32 radius;
    f32 fadeIn;
    f32 fadeOut;
    f32 fade;
    u32 alpha;
    u32 color;
    u32 i;

    layers = cfg->layers;
    if (layers == 0) {
        return;
    }
    segments = cfg->segments;
    entries = segments + 1;
    words = entries * 4;
    table = *(EffectFadeTable **)(node + 4);
    colors = table->colors;
    radii = table->radii;
    colorsStart = colors;
    radiiStart = radii;
    fadeOut = cfg->fadeOut;
    fadeIn = cfg->fadeIn;
    fadeInEnd = (s32)(fadeIn * (f32)segments);
    fadeOutStart = (s32)(fadeOut * (f32)segments);
    radius = cfg->radius / 3.0f;
    for (i = 0; i < entries; i++) {
        if (i < fadeInEnd) {
            fade = (f32)i / (f32)fadeInEnd;
        } else {
            fade = 1.0f;
            if (fadeOutStart < i) {
                fade = (f32)(segments - i) / (f32)(segments - fadeOutStart);
            }
        }
        alpha = (u32)(fade * 128.0f);
        color = (alpha << 24) | 0x808080;
        colors[0] = 0x808080;
        colors[1] = color;
        colors[2] = color;
        colors[3] = 0x808080;
        radii[0] = 0.0f;
        radii[2] = radius;
        radii[4] = radius * 2.0f;
        radii[6] = radius * 3.0f;
        colors += 4;
        radii += 8;
    }
    for (i = 1; i < layers; i++) {
        memcpy(colors, colorsStart, words * 4);
        colors += words;
        memcpy(radii, radiiStart, words * 8);
        radii += words * 2;
    }
}

u8 *func_002AD5C8(u8 *config, u32 resource) {
    u8 *node = func_002AD308(config);

    *(u32 *)(node + 4) = func_002AE350(*(u32 *)(config + 0x38), *(u32 *)(config + 0x8C), resource);
    func_002AD380(node, config);
    return node;
}

u8 *func_002AD628(u8 *work) {
    u8 *config = *(u8 **)(work + 0x34);
    u8 *state = *(u8 **)(work + 0x30);
    u8 *node = func_002AD308(config);

    *(u32 *)(node + 4) = func_002AE430(*(u32 *)(state + 4));
    func_002AD380(node, config);
    return node;
}

void func_002AD688(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x30);
    func_002AE3C8(*(u32 *)(temp_v0 + 4));
    func_002D0918(*(u32 *)(temp_v0 + 8));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AD6B8);

void func_002ADCE0(u8 *work) {
    u8 *config = *(u8 **)(work + 0x34);
    u32 limit = *(u32 *)(work + 0x28);
    u32 progress = *(u32 *)(config + 0x34);
    u32 *list = *(u32 **)(work + 0x30);
    u8 *out = (u8 *)list[1];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = func_00296F58(config, config + 0x24, limit, progress);
    unit = 0x3C000000;
    color1[0] = *(s32 *)(work + 0x24);
    EE_MMI_RGBA_UNPACK(color1, unit);
    __asm__ volatile (".set noreorder\n\tvmove.xyzw vf11, vf10\n\t.set reorder");
    color2[0] = second;
    __asm__ volatile (
        ".set noreorder\n"
        "lw $2, 0(%1)\n"
        "pextlb $2, $0, $2\n"
        "pextlh $2, $0, $2\n"
        "qmtc2.ni $2, vf10\n"
        "vitof0.xyzw vf10, vf10\n"
        "qmtc2.ni %0, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmul.xyzw vf10, vf10, vf11\n"
        ".set reorder"
        : : "r"(unit), "r"(color2) : "$2", "memory");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %1\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vftoi0.xyzw vf10, vf10\n"
        "qmfc2.ni %0, vf10\n"
        "ppach %0, $0, %0\n"
        "ppacb %0, $0, %0\n"
        ".set reorder"
        : "=r"(packed) : "f"(128.0f) : "$2");
    blended[0] = packed;
    *(u32 *)(out + 8) = blended[0];
    *(u32 *)(out + 4) = *(u32 *)(config + 0x28);
    *(u8 *)(out + 0x14) = *(u8 *)(config + 0xB9);
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(work + 0x10));
    func_002E7D98();
    __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(D_0037E0E0));
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %0\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmulx.xyzw vf28, vf28, vf10x\n"
        "vmuly.xyzw vf29, vf29, vf10y\n"
        "vmulz.xyzw vf30, vf30, vf10z\n"
        "lqc2 vf10, 0(%1)\n"
        "vmove.w vf10, vf0\n"
        "vmove.xyzw vf31, vf10\n"
        "sqc2 vf28, 0(%2)\n"
        "sqc2 vf29, 0x10(%2)\n"
        "sqc2 vf30, 0x20(%2)\n"
        "sqc2 vf31, 0x30(%2)\n"
        ".set reorder"
        : : "f"(*(f32 *)(work + 0x20)), "r"(work), "r"(mtx) : "$2", "memory");
    func_002AE498(out, mtx);
}

extern EffectResourceSizeEntry D_0037ED20[];

u8 *allocateEffectBlock(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = D_0037ED20[kind].resourceSize;
    u8 *effect = func_002CFEB8(size + headerSize);
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

extern u8 *allocateEffectBlock(u16, void *);

u8 *effCreateResourceInstanceB(u16 kind, void *source, u32 option) {
    u8 *effect = allocateEffectBlock(kind, source);
    *(u32 *)(effect + 0x30) = D_0037ED08[kind].createResource(source, option);
    D_0037ED08[kind].fn(effect);
    return effect;
}

u8 *effCreateFileResourceInstanceB(u8 *work) {
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
    return effCreateResourceInstanceB(*(u16 *)(work + 0xC), source, (u32)secondary);
}

void func_002AE000(u8 *work) {
    D_0037ED10[*(s32 *)(work + 0x2c)].fn();
    func_002CFF98(work);
}

u8 *effDuplicateActiveResourceB(u8 *work) {
    u8 *effect = allocateEffectBlock(*(u16 *)(work + 0x2C), *(void **)(work + 0x34));
    u32 active = D_0037ED08[*(s32 *)(work + 0x2C)].createActiveResource(work);
    s32 kind = *(s32 *)(work + 0x2C);
    *(u32 *)(effect + 0x30) = active;
    D_0037ED08[kind].fn(effect);
    return effect;
}

void func_002AE0D8(u8 *work) {
    D_0037ED08[*(s32 *)(work + 0x2c)].fn();
    *(u32 *)(work + 0x28) = 0;
}

void func_002AE120(s32 arg0) {
    D_0037ED18[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
    *(s32 *)(arg0 + 0x28) = *(s32 *)(arg0 + 0x28) + 1;
}

void func_002AE170(s32 arg0) {
    D_0037ED1C[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
}

void func_002AE1A8(u32 arg0) {
    func_002AE120(arg0);
    func_002AE170(arg0);
}

void func_002AE1D0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002AE1E0(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x10, src);
}

void func_002AE1F8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002AE200(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AE208);

extern u8 *func_002AE208(u32, u32);

u32 func_002AE350(u32 count, u32 repeat, u32 resource) {
    u8 *node = func_002AE208(count, repeat);

    if (resource == 0) {
        s32 references = D_003BC98C;
        *(u32 *)(node + 0x18) = 0;
        if (references == 0) {
            D_003BC990 = func_0029BF88(D_003BC988, 0x300);
            references = D_003BC98C;
        }
        references++;
        D_003BC98C = references;
    } else {
        *(void **)(node + 0x18) = func_0029BD90((void *)resource);
    }
    return (u32)node;
}

void func_002AE3C8(s32 arg0) {
    if (*(s32 *)(arg0 + 0x18) == 0) {
        D_003BC98C = D_003BC98C - 1;
        if (D_003BC98C == 0) {
            effReleaseSharedReference(D_003BC990);
            D_003BC990 = 0;
        }
    }
    else {
        effReleaseSharedReference(*(s32 *)(arg0 + 0x18));
    }
    sdfQueueAssetRelease(*(u32 *)(arg0 + 0x2c));
    func_002D0918(*(u32 *)(arg0 + 0x30));
}

u32 func_002AE430(u32 work) {
    u8 *node = func_002AE208(*(u32 *)work, *(u32 *)(work + 0x10));
    RefObj *texture = *(RefObj **)(work + 0x18);

    if (texture != NULL) {
        *(RefObj **)(node + 0x18) = effRetainSharedReference(texture);
    } else {
        D_003BC98C++;
        *(RefObj **)(node + 0x18) = NULL;
    }
    return (u32)node;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AE498);

void effLoadWindTexture(void) {
    D_003BC984 = func_002EB028("/effect/wind00.tmx", &D_003BC988, 0);
}

u32 func_002AE8D8(void) {
    return D_003BC988;
}

void func_002AE8E0(u8 *work) {
    u32 index = 0;
    u8 *state = *(u8 **)(work + 0x30);
    u32 count = *(u32 *)(*(u8 **)(work + 0x34) + 0x38);
    s32 *entry = *(s32 **)state;
    s32 *flags = *(s32 **)(*(u8 **)(state + 4) + 0x18);

    if (count != 0) {
        do {
            index++;
            *flags++ = 0;
            *entry = -1;
            entry = (s32 *)((u8 *)entry + 0x30);
        } while (index < count);
    }
}

u32 *effAllocateAnimationBuffer(u8 *work) {
    u32 age;
    u32 *buffer;
    void *allocation = func_002D03F8(*(u32 *)(work + 0x38) * 0x30 + 0xC);

    buffer = (u32 *)sdfResourceRetainAddress((u32)allocation);
    age = *(u32 *)(work + 0x8C);

    buffer[0] = (u32)(buffer + 3);
    buffer[2] = (u32)allocation;
    if (age < 3) {
        *(u32 *)(work + 0x8C) = 3;
    }
    return buffer;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AE9A0);

extern void func_002AE9A0(u32 *, u8 *);

extern u32 func_002B0AD8(u32, u32);

u32 *effPrepareTextureAnimation(u8 *work) {
    u32 *buffer = effAllocateAnimationBuffer(work);

    buffer[1] = func_002B0AD8(*(u32 *)(work + 0x38), *(u32 *)(work + 0x8C));
    func_002AE9A0(buffer, work);
    return buffer;
}

extern u32 func_002B0B40(u8 *);

u32 *effPrepareOwnedTextureAnimation(u8 *work) {
    u8 *anim = *(u8 **)(work + 0x34);
    u8 *owner = *(u8 **)(work + 0x30);
    u32 *buffer = effAllocateAnimationBuffer(anim);

    buffer[1] = func_002B0B40(*(u8 **)(owner + 4));
    func_002AE9A0(buffer, anim);
    return buffer;
}

void func_002AEC30(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x30);
    func_002B0B08(*(u32 *)(temp_v0 + 4));
    func_002D0918(*(u32 *)(temp_v0 + 8));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AEC60);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AF370);

extern float func_002E8398(void *);

void effInitializeAnimationPositions(u8 *work) {
    u8 *resource = *(u8 **)(work + 0x30);
    u8 *payload = *(u8 **)(resource + 8);
    float *positions = *(float **)resource;
    u32 count = *(u32 *)(payload + 8);
    u32 i = 0;

    fileClearRecordReferences((s32)payload);
    for (; i < count; i++) {
        positions[0] = func_002E8398(D_00324550);
        positions[1] = func_002E8398(D_00324550);
        positions += 2;
    }
}

u32 func_002AF560(EffGrid *grid) {
    u32 n;
    s32 w = grid->width;
    n = (w ? w : grid->height) * (w ? grid->height : grid->altHeight);
    return n > 200 ? 200 : n;
}

u32 *effCreateAnimationState(u32 unused, u32 count) {
    u32 allocation = (u32)func_002D03F8(count * 8 + 0x10);
    u32 *state = (u32 *)sdfResourceRetainAddress(allocation);

    state[3] = allocation;
    state[0] = (u32)(state + 4);
    state[2] = 0;
    state[1] = func_002B0918();
    return state;
}

u32 *effActivateAnimationState(s32 work) {
    s32 owner = *(s32 *)(work + 0x30);
    s32 resource = *(s32 *)(owner + 8);
    u32 *state = effCreateAnimationState(*(u32 *)(work + 0x34), *(u32 *)(resource + 8));

    resource = *(s32 *)(owner + 8);
    state[2] = func_0029A5E0(*(u16 *)resource, *(u32 *)(resource + 8),
                               *(void **)(resource + 0x24));
    return state;
}

extern void func_002B0958(s32);

void func_002AF640(u8 *work) {
    u8 *state = *(u8 **)(work + 0x30);

    func_002B0958(*(s32 *)(state + 4));
    if (*(u32 *)(state + 8) != 0) {
        func_0029A730(*(u32 *)(state + 8));
    }
    func_002D0918(*(u32 *)(state + 0xC));
}

void effSynchronizeFileTransform(u8 *work) {
    u8 *state = *(u8 **)(work + 0x30);
    u32 handle = *(u32 *)(state + 8);

    if (handle != 0) {
        menuRecordSetVector(handle, work);
        func_0029A7F8(*(u32 *)(state + 8), work + 0x10);
        dds3DispatchIndexedCallback((u16 *)*(u32 *)(state + 8), *(float *)(work + 0x20));
        fileAcquireRecord(*(u32 *)(state + 8));
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AF6F8);

extern u32 func_002AF560(EffGrid *);

u32 *func_002AFD10(u8 *work) {
    EffGrid *mapping = (EffGrid *)(work + 0x3C);
    u32 count = func_002AF560(mapping);
    u32 *state = effCreateAnimationState((u32)work, count);

    state[2] = func_0029A5E0(1, count, mapping);
    return state;
}

u32 *func_002AFD78(u8 *work) {
    EffGrid *mapping = (EffGrid *)(work + 0x3C);
    u32 count = func_002AF560(mapping);
    u32 *state = effCreateAnimationState((u32)work, count);

    state[2] = func_0029A5E0(3, count, mapping);
    return state;
}

void func_002AFDE0(s32 arg0) {
    *(u32 *)(*(s32 *)(*(s32 *)(arg0 + 0x30) + 4) + 8) = 0;
}

u32 *effAllocateQuantizedBuffer(u8 *work) {
    void *allocation = func_002D03F8(0xC);
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

INCLUDE_ASM(const s32, "game/code_0029A840", func_002AFE68);

extern void func_002AFE68(u32 *, u8 *);

u32 *effPrepareQuantizedTexture(u8 *work) {
    u32 *buffer = effAllocateQuantizedBuffer(work);

    buffer[1] = func_002B0AD8(buffer[0], *(u32 *)(work + 0x74));
    func_002AFE68(buffer, work);
    return buffer;
}

u32 *effPrepareOwnedQuantizedTexture(u8 *work) {
    u8 *anim = *(u8 **)(work + 0x34);
    u8 *owner = *(u8 **)(work + 0x30);
    u32 *buffer = effAllocateQuantizedBuffer(anim);

    buffer[1] = func_002B0B40(*(u8 **)(owner + 4));
    func_002AFE68(buffer, anim);
    return buffer;
}

void func_002B0360(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x30);
    func_002B0B08(*(u32 *)(temp_v0 + 4));
    func_002D0918(*(u32 *)(temp_v0 + 8));
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

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B0408);

extern EffectResourceSizeEntry D_0037EDA8[];

u8 *allocateEffectBlockWithModel(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = D_0037EDA8[kind].resourceSize;
    u8 *effect = func_002CFEB8(size + headerSize);
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

extern u8 *allocateEffectBlockWithModel(u16, void *);

u8 *effCreateResourceInstanceC(u16 kind, void *source) {
    u8 *effect = allocateEffectBlockWithModel(kind, source);
    *(u32 *)(effect + 0x30) = D_0037ED90[kind].createResource(source);
    D_0037ED90[kind].fn(effect);
    return effect;
}

void func_002B06E0(s32 arg0) {
    void *source;

    source = fileResolvePrimaryBuffer();
    effCreateResourceInstanceC(*(u16 *)(arg0 + 0xc), source);
}

void effDispatchCleanupOp(u8 *work) {
    D_0037ED98[*(s32 *)(work + 0x2c)].fn();
    func_002CFF98(work);
}

u8 *effRecreateActiveByClass(u8 *work) {
    u8 *effect = allocateEffectBlockWithModel(*(u16 *)(work + 0x2C), *(u8 **)(work + 0x34));
    u32 resource = D_0037ED90[*(s32 *)(work + 0x2C)].createActiveResource(work);
    s32 kind = *(s32 *)(work + 0x2C);
    *(u32 *)(effect + 0x30) = resource;
    D_0037ED90[kind].fn(effect);
    return effect;
}

void func_002B07E8(u8 *work) {
    D_0037ED90[*(s32 *)(work + 0x2c)].fn();
    *(u32 *)(work + 0x28) = 0;
}

void effAdvanceDispatchCounter(u8 *work) {
    D_0037EDA0[*(s32 *)(work + 0x2C)].fn(work);
    (*(u32 *)(work + 0x28))++;
}

void func_002B0880(s32 arg0) {
    D_0037EDA4[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
}

void func_002B08B8(u32 arg0) {
    effAdvanceDispatchCounter(arg0);
    func_002B0880(arg0);
}

void func_002B08E0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002B08F0(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x10, src);
}

void func_002B0908(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002B0910(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

u32 func_002B0918(void) {
    if (D_003BC99C == 0) {
        D_003BC9A0 = func_0029BF88(D_003BC998, 0x200);
    }
    D_003BC99C = D_003BC99C + 1;
    return D_003BC9A0;
}

void func_002B0958(s32 arg0) {
    D_003BC99C = D_003BC99C - 1;
    if (D_003BC99C == 0) {
        effReleaseSharedReference(D_003BC9A0);
        D_003BC9A0 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B0988);

u32 func_002B0AD8(u32 count, u32 age) {
    u32 texture = func_002B0988(count, age);
    func_002B0918();
    return texture;
}

void func_002B0B08(s32 arg0) {
    func_002B0958(D_003BC9A0);
    sdfQueueAssetRelease(*(s32 *)(arg0 + 0x2C));
    func_002D0918(*(s32 *)(arg0 + 0x30));
}

u32 func_002B0B40(u8 *work) {
    u32 texture = func_002B0988(*(u32 *)work, *(u32 *)(work + 0x10));
    D_003BC99C++;
    return texture;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B0B70);

void effLoadScalyTexture(void) {
    D_003BC994 = func_002EB028("/effect/scaly00.tmx", &D_003BC998, 0);
}

u32 func_002B11A0(void) {
    return D_003BC998;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B11A8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B12D8);

extern void func_002B2410(s32);

void effReleaseParticleList(u8 *list) {
    u32 i;
    u8 *entry = *(u8 **)list;

    for (i = 0; i < *(u32 *)(list + 4); i++, entry += 0x10) {
        func_002B2410(*(s32 *)(entry + 4));
        if (*(u32 *)(entry + 8) != 0) {
            releaseEffectResourceRefs(*(u32 *)(entry + 8));
        }
    }
    func_002D0918(*(u32 *)(list + 0xC));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B1560);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B1D68);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B1F58);

u32 func_002B2088(u8 *work) {
    void *first = fileResolvePrimaryBuffer();
    void *second = fileResolveSecondaryBuffer(work);
    return func_002B1F58(*(u16 *)(work + 0xC), first, second, *(u32 *)(work + 0x24));
}

typedef struct EffModelResource {
    u8 pad0[0x28];
    s32 updateCount;
    s32 kind;
    void *model;
    u32 attributes;
    u32 childResource;
    void *source;
} EffModelResource;

typedef struct EffModelCreateRequest {
    u8 pad0[0x2C];
    u16 kind;
    u8 pad2E[2];
    u32 assetId;
    u32 attributes;
    u8 pad38[4];
    void *source;
} EffModelCreateRequest;

void func_002B20D0(EffModelResource *effect) {
    D_0037EE40[effect->kind].fn((void *)effect->childResource);
    func_0029A938((s32)effect->model);
    func_002CFF98(effect);
}

EffModelResource *createEffectModelResource(EffModelCreateRequest *work) {
    EffModelResource *effect = (EffModelResource *)func_002B1F58(work->kind, work->source, 0, 0);
    u32 x = func_002183D0(work->assetId);
    u32 y = func_002183E0(work->assetId);
    void *model = func_00217680(x, y);

    effect->model = model;
    effInitModelVUState(model);
    effect->attributes = work->attributes;
    effect->childResource = D_0037EE38[effect->kind].createResource(effect->source, effect->model);
    D_0037EE38[effect->kind].fn(effect);
    return effect;
}

void func_002B21F0(EffModelResource *effect) {
    D_0037EE38[effect->kind].fn();
    effect->updateCount = 0;
}

void func_002B2238(EffModelResource *effect) {
    D_0037EE44[effect->kind].fn(effect);
    effect->updateCount = effect->updateCount + 1;
}

void func_002B2288(EffModelResource *effect) {
    D_0037EE48[effect->kind].fn(effect);
}

void func_002B22C0(EffModelResource *effect) {
    func_002B2238(effect);
    func_002B2288(effect);
}

void func_002B22E8(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002B22F8(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x10, src);
}

void func_002B2310(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002B2318(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B2320);

void func_002B2410(s32 arg0) {
    sdfQueueAssetRelease(*(u32 *)(arg0 + 0x18));
    func_002D0918(*(u32 *)(arg0 + 0x1c));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B2440);

typedef struct EffectVectorRequest {
    u8 kind;
    u8 count;
    u8 size;
    u8 pad_03;
    u32 unk04;
} EffectVectorRequest;

extern u32 func_00161858(void);

extern u32 func_00161860(void);

extern u32 func_00161868(void);

extern u32 func_00161870(void);

extern void func_00161AA0(u32, void *, void *);

void getEffectWorldVector(u32 which) {
    EffectVectorRequest request;
    u128 result;
    u32 handle = func_00161858();

    request.kind = 0xB;
    request.count = 1;
    request.size = 8;
    request.unk04 = 0;
    switch (which) {
    case 0:
        break;
    case 1:
        handle = func_00161858();
        request.kind = 0;
        break;
    case 2:
        handle = func_00161860();
        request.kind = 0;
        break;
    case 3:
        request.kind = 1;
        break;
    case 4:
        request.kind = 2;
        break;
    case 5:
        request.kind = 3;
        break;
    case 6:
        handle = func_00161868();
        request.kind = 6;
        break;
    case 7:
        handle = func_00161870();
        request.kind = 7;
        break;
    }
    if (request.kind != 0xB) {
        u128 *vec = &result;

        func_00161AA0(handle, &request, vec);
        __asm__ volatile (".set noreorder\nlqc2 vf10, 0(%0)\n.set reorder" : : "r"(vec) : "memory");
    } else {
        __asm__ volatile (".set noreorder\nvmove.xyzw vf10, vf0\n.set reorder");
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B27B8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B2938);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B2A48);

void func_002B2E98(void) {
    u8 *state = (u8 *)func_001A17F0();
    u8 *effect;

    if ((*(u32 *)(state + 0x1F4) & 0x6000000) == 0) {
        return;
    }
    effect = *(u8 **)(state + 0x228);
    while (effect != NULL) {
        if (*(u32 *)(effect + 0x110) & 2) {
            u8 *work = *(u8 **)(effect + 0x320);
            if (work != NULL) {
                *(u32 *)(work + 0x60) = *(u32 *)(effect + 0x54);
                func_00221E08(work, 0, *(u32 *)(effect + 0x54));
            }
        }
        effect = *(u8 **)(effect + 0x344);
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B2F20);

extern void mdlLoadPrimaryVectorVU(void *);

extern u128 D_003DCBD0[];

extern s8 D_003BC9AC;

extern u128 D_003DCBE0[];

extern u128 D_003DCBA0[];

extern u8 D_0037EE80[];

extern void func_002E1938(void *, void *, void *);

s64 func_002B3090(void *vector, void *target) {
    s64 result = func_001A1438();

    if (result != 0) {
        if (D_003BC9AC == 0) {
            return 0;
        }
    mdlLoadPrimaryVectorVU(vector);
    __asm__ volatile(".set noreorder\n\tlqc2 $vf11, 0(%0)\n\t.set reorder" : : "r"(D_003DCBE0));
    __asm__ volatile(".set noreorder\n\tvsub.xyzw $vf10, $vf10, $vf11\n\t.set reorder");
    __asm__ volatile(
        ".set noreorder\n\t"
        "vmulx.w $vf10, $vf10, $vf0x\n\t"
        "vmul.xyz $vf2, $vf10, $vf10\n\t"
        "vmulax.w ACC, $vf0, $vf2x\n\t"
        "vmadday.w ACC, $vf0, $vf2y\n\t"
        "vmaddz.w $vf2, $vf0, $vf2z\n\t"
        "vrsqrt Q, $vf0w, $vf2w\n\t"
        "vwaitq\n\t"
        "vmulq.xyz $vf10, $vf10, Q\n\t"
        ".set reorder");
    __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(D_003DCBA0) : "memory");
    func_002E1938(target, D_0037EE80, D_003DCBD0);
    return 1;
    }
    return result;
}

extern u128 *D_00324770[];

extern u128 D_003DCB90[];

extern u128 D_00324780[];

void effResetDefaultColorTables(void) {
    u128 *dst = D_003DCB90;
    u128 *src = D_00324770[0];
    PCP_COPY_VECTOR(dst, src);
    dst++;
    src++;
    PCP_COPY_VECTOR(dst, src);
    PCP_COPY_VECTOR(D_003DCBD0, D_00324780);
    D_003BC9AC = 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B3178);

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

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B3390);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B3420);

extern void func_00127898(u32, u32);

extern void func_002B3420(u32);

u32 effCreateInitializedObject(u32 type, u32 parameter, u32 index) {
    u32 *effect = (u32 *)func_002B3390(type, parameter, index);
    u32 child = *effect;
    func_00127898(child, child + 8);
    func_002B3420((u32)effect);
    return (u32)effect;
}

u32 func_002B35C8(s32 arg0) {
    u32 temp_v0;

    temp_v0 = func_002B3390(*(u32 *)(arg0 + 0x38), **(u32 **)(arg0 + 0x30),
                                                (*(u32 **)(arg0 + 0x30))[1]);
    func_002B3420(temp_v0);
    return temp_v0;
}

void effReleaseTargetSlots(u8 *work) {
    u32 *effects = (u32 *)(work + 0x10);
    u32 *targets = (u32 *)(work + 0x24);
    u32 i;

    for (i = 0; i < 5; i++) {
        if (*targets != 0) {
            dds3FreePathObject(*targets);
        }
        targets++;
        if (*effects != 0) {
            func_00110928(*effects);
        }
        effects++;
    }
    func_002D0918(*(u32 *)(work + 0x38));
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B3698);

extern s32 func_002B27B8(void *, u32);

extern s32 mdlGetNodeRefHalf(u32, s32);

extern void func_001D5DF8(void *, s32, u32, f32);

extern void func_001F4078(void *, s32);

void applyEffectOverlaySpecs(u8 *work) {
    u8 *objects[16];
    u16 *spec;
    u32 count;
    u32 i;

    if (*(s32 *)(work + 0x28) > 0) {
        return;
    }
    spec = *(u16 **)(work + 0x38);
    count = func_002B27B8(objects, *(u8 *)(spec + 2));
    for (i = 0; i < count; i++) {
        u32 flags = *(u32 *)(objects[i] + 0x110);
        if (flags & 2) {
            if ((flags & 0x20) == 0) {
                if ((*(u16 *)(objects[i] + 0x310) & 0x10) == 0) {
                    if (mdlGetNodeRefHalf(*(u32 *)(*(u8 **)(objects[i] + 0x320) + 0x8C), 0) > spec[0]) {
                        func_001D5DF8(objects[i], spec[0], spec[1] | 0x100, 1.0f);
                        if (spec[3] == 0) {
                            func_001F4078(objects[i], spec[0]);
                        }
                    }
                }
            }
        }
    }
}

extern void func_001F3460(u32);

void effReportResourceStatus(u8 *work) {
    u32 status;

    if (*(s32 *)(work + 0x28) > 0) {
        return;
    }
    status = **(u32 **)(work + 0x38);
    switch (status) {
    case 0:
        func_001F3460(0x1000A);
        break;
    case 1:
        func_001F3460(0x1000B);
        break;
    }
}

void func_002B3C20(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    if ((*(u32 *)(temp_v0 + 500) & 0x6000000) != 0) {
        func_001EFD58(temp_v0 + 0x50, temp_v0 + 0x60, 0);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B3C68);

void effResetSlots(void) {
    D_003BC9B0[0] = 0;
    D_003BC9B8[0] = 0;
    D_003BC9C0[0] = 0;
    D_003BC9C8[0] = 0;
    D_003BC9B0[1] = 0;
    D_003BC9B8[1] = 0;
    D_003BC9C0[1] = 0;
    D_003BC9C8[1] = 0;
    func_00104130();
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B3EC8);

void effSetDormantSlot(u32 index, u8 color, u32 value) {
    if (index < 2) {
        D_003BC9B0[index] = value;
        D_003BC9C0[index] = color;
        D_003BC9C8[index] = 0;
        D_003BC9B8[index] = value;
    }
}

void effSetActiveSlot(u32 index, u8 color, u32 value) {
    if (index < 2) {
        D_003BC9B0[index] = value;
        D_003BC9C0[index] = color;
        D_003BC9C8[index] = 1;
        D_003BC9B8[index] = value;
    }
}

void func_002B40E0(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    if ((*(u32 *)(temp_v0 + 500) & 0x6000000) != 0) {
        effResetSlots();
        return;
    }
}

void func_002B4118(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;
    u32 *puVar3;
    u8 *puVar4;
    u32 temp_v2;

    temp_v2 = 0;
    func_001A17F0();
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

void func_002B41D8(void) {
    func_001EF9D8(0xc);
}

void func_002B41F0(u8 *arg0) {
    u32 *puVar3;
    u32 temp_v0;
    u32 temp_v1;

    func_001A17F0();
    temp_v0 = *(u32 *)(arg0 + 0x28);
    puVar3 = *(u32 **)(arg0 + 0x38);
    temp_v1 = puVar3[0];
    if (temp_v1 < puVar3[3]) {
        return;
    }
    if (temp_v0 == 0) {
        func_001EF990(puVar3[1], *(u16 *)(puVar3 + 2));
        temp_v1 = puVar3[0];
    }
    if (temp_v1 != 0 && temp_v0 == temp_v1 - puVar3[3]) {
        func_001EF9D8(*(u16 *)(puVar3 + 3));
    }
}

extern f32 D_003BC9A8;

extern void func_001DC2A0(u8 *, f32);

/* Eases D_003BC9A8 (a rotation in radians) along the span's [start, end] degrees. */
void func_002B4270(u8 *work) {
    u8 *state = (u8 *)func_001A17F0();
    f32 *span = *(f32 **)(work + 0x38);
    u32 count = *(u32 *)span;
    f32 value;

    if (count != 0 && count >= *(u32 *)(work + 0x28)) {
        f32 t = (f32)*(s32 *)(work + 0x28) / (f32)count;
        value = ((span[2] - span[1]) * t + span[1]) * 0.017453293f;
        func_001DC2A0(state + 0x70, value);
        D_003BC9A8 = value;
    }
}

extern EffectResourceSizeEntry D_0037EEF8[];

u8 *effAllocateResourcePayload(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = D_0037EEF8[kind].resourceSize;
    u8 *effect = func_002CFEB8(size + headerSize);
    *(u8 **)(effect + 0x38) = effect + headerSize;
    *(u32 *)(effect + 0x24) = 0x80808080;
    *(float *)(effect + 0x20) = 1.0f;
    *(u32 *)(effect + 0x2C) = kind;
    *(u32 *)(effect + 0x28) = 0;
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" : : "r"(effect) : "memory");
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" : : "r"(effect + 0x10) : "memory");
    memcpy(*(void **)(effect + 0x38), source, size);
    return effect;
}

extern FnTbl28 D_0037EEE0[];

u32 createEffectResourceInstance(u16 kind, void *source, void *secondary, u32 param) {
    u8 *effect = effAllocateResourcePayload(kind, source);

    if (func_001A1438() != 0) {
        if (D_0037EEE0[kind].createResource != NULL) {
            *(u32 *)(effect + 0x30) = D_0037EEE0[kind].createResource(source, secondary, param);
        }
        if (D_0037EEE0[kind].fn != NULL) {
            D_0037EEE0[kind].fn(effect);
        }
    }
    return (u32)effect;
}

u32 func_002B4498(u8 *work) {
    void *first = fileResolvePrimaryBuffer();
    void *second = fileResolveSecondaryBuffer(work);
    return createEffectResourceInstance(*(u16 *)(work + 0xC), first, second, *(u32 *)(work + 0x24));
}

extern FnTbl28 D_0037EEE8[];

void effDestroyResourceInstance(u8 *work) {
    if (func_001A1438() != 0) {
        void (*callback)(void *) = D_0037EEE8[*(s32 *)(work + 0x2C)].fn;
        if (callback != NULL) {
            callback(*(void **)(work + 0x30));
        }
    }
    func_002CFF98(work);
}

u8 *effDuplicateActiveResource(u8 *source) {
    u8 *effect;
    u32 kind = *(u32 *)(source + 0x2C);

    if (D_0037EEE0[kind].createActiveResource == NULL) {
        effect = (u8 *)createEffectResourceInstance(*(u16 *)(source + 0x2C), *(u8 **)(source + 0x38), 0, 0);
    } else {
        u32 resource;
        u32 activeKind;
        effect = effAllocateResourcePayload(*(u16 *)(source + 0x2C), *(u8 **)(source + 0x38));
        resource = D_0037EEE0[*(s32 *)(source + 0x2C)].createActiveResource(source);
        activeKind = *(u32 *)(source + 0x2C);
        *(u32 *)(effect + 0x30) = resource;
        if (D_0037EEE0[activeKind].fn != NULL) {
            D_0037EEE0[activeKind].fn(effect);
        }
    }
    return effect;
}

void effClearCallbackFrame(u8 *work) {
    if (func_001A1438() != 0) {
        void (*callback)(void *) = D_0037EEE0[*(s32 *)(work + 0x2C)].fn;
        if (callback != NULL) {
            callback(work);
        }
        *(u32 *)(work + 0x28) = 0;
    }
}

extern FnTbl28 D_0037EEF0[];

void effDispatchIndexedCallback(work)
    u8 *work;
{
    if (func_001A1438() != 0) {
        void (*callback)(void *) = D_0037EEF0[*(s32 *)(work + 0x2C)].fn;
        if (callback != NULL) {
            callback(work);
        }
        *(s32 *)(work + 0x28) += 1;
    }
}

void effDispatchEnabledCallback(u8 *work) {
    if (func_001A1438() != 0) {
        void (*callback)(void *) = D_0037EEF4[*(s32 *)(work + 0x2C)].fn;
        if (callback != NULL) {
            callback(work);
        }
    }
}

void func_002B4738(u32 arg0) {
    effDispatchIndexedCallback();
    effDispatchEnabledCallback(arg0);
}

void func_002B4760(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_002B4770(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x10, src);
}

void func_002B4788(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002B4790(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B4798);

void effReleaseParticleResources(u8 *work) {
    void *particle = *(void **)(work + 0xA4);
    if (particle != NULL) {
        billDispatchByKind(particle);
    }
    if (*(void **)(work + 0xA8) != NULL) {
        effReleaseReferenceHolder(*(void **)(work + 0xA8));
    }
    func_002CFF98(work);
}

u8 *func_002B49E8(u8 *source) {
    u8 *effect = func_002B4798(NULL);
    memcpy(effect + 0xC, source + 0xC, 0x98);
    effReplaceSharedResource(effect, source);
    return effect;
}

void effReplaceSharedResource(u8 *work, u8 *source) {
    u32 resource;
    if (*(u32 *)(source + 0xA4) != 0) {
        if (*(u32 *)(work + 0xA4) != 0) {
            billDispatchByKind(*(void **)(work + 0xA4));
        }
        resource = func_00151E60(*(u32 *)(source + 0xA4));
        *(u32 *)(work + 0xA4) = resource;
        func_001523B0(resource);
        return;
    }
    if (*(RefObj **)(work + 0xA8) != NULL) {
        effReleaseReferenceHolder(*(RefObj **)(work + 0xA8));
    }
    *(RefObj **)(work + 0xA8) = func_0029C408(*(RefObj **)(source + 0xA8));
}

void func_002B4B98(s32 arg0) {
    *(u32 *)(arg0 + 8) = 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B4BA0);

void func_002B5128(u32 *arg0, u32 arg1) {
    *arg0 = arg1;
}

void btlInitializeEffectWork(void) {
    D_003BD074 = D_0038F2B8;
    D_003DF8D0[0] = 0;
    D_003BD058 = 0;
    D_003BD955 = 0;
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" :: "r"(D_003DF910) : "memory");
}

s32 btlUpdateEffectWork(void) {
    if (D_003BD074 != NULL) {
        if ((func_002B59A8(0) & 1) == 0) {
            func_002B51B0();
            return 0;
        }
    }
    return 1;
}

void func_002B51A8(void) {
}

void func_002B51B0(void) {
    effResetFileResources();
    func_002BC510();
}

void setBattleEffectOffset(void *src) {
    PCP_COPY_VECTOR(D_003DF910, src);
}

extern s32 D_003BD060;

extern void func_00295D08(u8 *, void *);

void func_002B51E8(u8 *effect) {
    u128 direction;
    u32 flags = *(u32 *)(effect + 0x68);

    if ((flags & 0x18) != 0) {
        func_00295D08(effect, &direction);
        __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(&direction));
        flags = *(u32 *)(effect + 0x68);
    } else {
        __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(effect + 0x40));
    }
    if ((flags & 0x80) != 0) {
        __asm__ volatile(
            ".set noreorder\n\t"
            "mfc1 $2, %0\n\t"
            "qmtc2.ni $2, $vf2\n\t"
            "vmulx.xyzw $vf10, $vf10, $vf2x\n\t"
            ".set reorder"
            : : "f"(*(f32 *)((u8 *)D_003BD060 + 0x74)) : "$2");
    }
    __asm__ volatile(".set noreorder\n\tlqc2 $vf11, 0(%0)\n\t.set reorder" : : "r"(D_003DF910));
    __asm__ volatile(".set noreorder\n\tvadd.xyzw $vf10, $vf10, $vf11\n\t.set reorder");
    __asm__ volatile(".set noreorder\n\tlqc2 $vf11, 0(%0)\n\t.set reorder" : : "r"(D_003BD060));
    __asm__ volatile(".set noreorder\n\tvadd.xyzw $vf10, $vf10, $vf11\n\t.set reorder");
    if ((flags & 4) != 0) {
        __asm__ volatile(
            ".set noreorder\n\t"
            "mfc1 $2, %0\n\t"
            "qmtc2.ni $2, $vf2\n\t"
            "vaddx.y $vf10, $vf0, $vf2x\n\t"
            ".set reorder"
            : : "f"(-5.0f) : "$2");
    }
}

extern s32 D_003BD060;

extern void func_00295E00(void *, void *);

extern void effMiscQuatMultiplyVU(void);

void effMultiplyQuatWithFlag(u8 *effect) {
    if ((*(u32 *)(effect + 0x68) & 0x60) != 0) {
        u128 quaternion;
        func_00295E00(effect, &quaternion);
        __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"((u8 *)D_003BD060 + 0x50));
        __asm__ volatile(".set noreorder\n\tlqc2 $vf11, 0(%0)\n\t.set reorder" : : "r"(&quaternion));
        effMiscQuatMultiplyVU();
    } else {
        __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"((u8 *)D_003BD060 + 0x50));
        __asm__ volatile(".set noreorder\n\tlqc2 $vf11, 0(%0)\n\t.set reorder" : : "r"(effect + 0x50));
        effMiscQuatMultiplyVU();
    }
}

typedef struct EffBattleCamera {
    u8 pad_00[0x60];
    f32 scale;
    u32 mode;
} EffBattleCamera;

extern u8 D_003DF9A0[];

extern void func_002B51E8(u8 *);

extern void func_002936E0(void *, void *);

extern void func_00293720(void *, void *);

extern void func_00293760(void *, f32);

extern void func_002937A0(void *, u32);

void func_002B5300(work)
    void *work;
{
    u128 rotation[2];

    func_002B51E8(D_003DF9A0);
    __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(&rotation[0]));
    func_002936E0(work, &rotation[0]);
    effMultiplyQuatWithFlag(D_003DF9A0);
    __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(&rotation[1]));
    func_00293720(work, &rotation[1]);
    func_00293760(work, ((EffBattleCamera *)D_003DF9A0)->scale * *(f32 *)((u8 *)D_003BD060 + 0x74));
    func_002937A0(work, ((EffBattleCamera *)D_003DF9A0)->mode);
}

extern void func_002937E0(u32, void *, u32, u16);

u32 func_002B5390(u8 *request) {
    void *output = *(void **)(request + 0xC);
    u32 job;

    if (output != NULL) {
        if (*(u16 *)(request + 4) == 6) {
            memcpy(output, D_003DE148, 0x3C);
            output = *(void **)(request + 0xC);
        }
        func_002937E0(D_003BD068, output, *(u32 *)(request + 0x10), *(u16 *)(request + 8));
    }
    job = func_00293450(D_003BD068);
    func_002B5300(job);
    return job;
}

extern u32 func_00293D90(u32);

extern u32 fileCreateJob(u16);

extern void func_002937E0(u32, void *, u32, u16);

extern void func_00293A00(u32, u32, u32);

extern void func_00293960(u32, void *, u32, u32);

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
    u32 relatedResource;
} EffFileJobRequest;

u32 effLoadFileJobPayload(EffFileJobRequest *request, u32 existingJob) {
    u32 job;
    if (existingJob != 0) {
        void *source;
        job = func_00293D90(existingJob);
        source = fileResolvePrimaryBuffer(job);
        memcpy(request->output, source, request->size);
    } else {
        job = fileCreateJob(request->fileKind);
        if (request->output != NULL) {
            func_002937E0(job, request->output, request->size, request->transferMode);
        }
        if (request->relatedResource != 0) {
            func_00293A00(job, request->relatedResource, request->resourceMode);
        } else {
            u32 value = 0;
            func_00293960(job, &value, 4, 4);
        }
    }
    func_002BC510();
    return job;
}

void func_002B5548(u32 arg0) {
    if (D_003BD05C == 0) {
        func_002B5300();
        func_002936A8(arg0);
        return;
    }
}

void func_002B5590(u32 arg0) {
    if (D_003BD05C != 0) {
        func_002944D8(D_003BD05C);
        D_003BD05C = 0;
    }
    if (D_003BD06C != 0) {
        fileJobDestroy(D_003BD06C);
        D_003BD06C = 0;
    }
    fileJobDestroy(arg0);
}

extern char D_003BD080[];

extern s32 func_003014F0(char *, const char *, ...);

extern u8 D_003BD078[];

u32 effPollPrimaryFile(void) {
    u8 record[0x70];
    char path[0x70];
    u32 state;
    u32 result = 0x400001;

    effUpdateResourceQueue((u32 *)D_003B39C8, D_003BD078, record);
    state = *(u32 *)(record + 0x64);
    if (state == 2) {
        result = 0x400000;
    } else if (state == 1) {
        if (D_003BD068 != 0) {
            func_003014F0(path, D_003BD080, D_003B39C8, record);
            fileWriteToPfs(D_003BD068, path);
            result = 0x400002;
        }
    }
    return result;
}

void func_002B5698(void) {
    func_002B9050();
    effQueueResource(D_003BD088, D_003DF8D0);
}

extern s32 D_003BD060;

u32 effPollNamedFile(void) {
    u8 record[0x70];
    char path[0x70];
    u32 state;
    u32 result = 0x400001;

    effUpdateResourceQueue((u32 *)D_003B39E0, D_003BD088, record);
    state = *(u32 *)(record + 0x64);
    if (state == 2) {
        result = 0x400000;
    } else if (state == 1) {
        if (D_003BD060 != 0) {
            strcpy((char *)D_003DF8D0, (char *)record + 0x32);
            func_003014F0(path, D_003BD080, D_003B39E0, record);
            func_00295018(D_003BD060, path);
            result = 0x400002;
        }
    }
    return result;
}

void func_002B5788(void) {
    func_002B9050();
    effQueueResource(D_003BD090, D_003DF8D0);
}

u32 effPollAttachedFile(void) {
    u8 record[0x70];
    char path[0x70];
    u32 state;
    u32 result = 0x400001;

    effUpdateResourceQueue((u32 *)D_003B39E0, D_003BD090, record);
    state = *(u32 *)(record + 0x64);
    if (state == 2) {
        result = 0x400000;
    } else if (state == 1) {
        if (D_003BD060 != 0) {
            strcpy((char *)D_003DF8D0, (char *)record + 0x32);
            func_003014F0(path, D_003BD080, D_003B39E0, record);
            func_002954F0(D_003BD060, path);
            result = 0x400002;
        }
    }
    return result;
}

typedef struct EffectAssetLink {
    u8 *asset;
    u32 object;
    u8 pad_08[0x14];
} EffectAssetLink;

extern EffectAssetLink *D_0038E9D8[22];

u8 *effFindAssetData(u8 *work) {
    u8 *requested = *(u8 **)(work + 0x90);
    u16 type = *(u16 *)(requested + 4);
    u16 id = *(u16 *)(requested + 0xC);
    u16 i;
    for (i = 0; i < 22; i++) {
        EffectAssetLink *links = D_0038E9D8[i];
        if (links != NULL) {
            EffectAssetLink *current = links;
            if (current->asset != NULL) {
                do {
                    u8 *asset = current->asset;
                    if (*(u16 *)(asset + 4) == type && *(u16 *)(asset + 8) == id) {
                        return asset;
                    }
                    current++;
                } while (current->asset != NULL);
            }
        }
    }
    return NULL;
}

u32 effFindAssetObject(u8 *work) {
    u8 *requested = *(u8 **)(work + 0x90);
    u16 type = *(u16 *)(requested + 4);
    u16 id = *(u16 *)(requested + 0xC);
    u16 i;
    for (i = 0; i < 22; i++) {
        EffectAssetLink *links = D_0038E9D8[i];
        if (links != NULL) {
            EffectAssetLink *current = links;
            if (current->asset != NULL) {
                do {
                    u8 *asset = current->asset;
                    if (*(u16 *)(asset + 4) == type && *(u16 *)(asset + 8) == id) {
                        return current->object;
                    }
                    current++;
                } while (current->asset != NULL);
            }
        }
    }
    return 0;
}

u32 func_002B5990(void) {
    return D_0038F2FC[0] + D_003BD058;
}

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2D20);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2D30);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2D40);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2D50);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2D60);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2D70);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2D80);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2D90);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2DA0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2DB0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2DC0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2DD0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2DE0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2DF0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2E00);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2E10);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2E20);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2E30);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2E40);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2E50);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2E60);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2E70);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2E80);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2E90);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2EB0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2ED0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2EF0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2F10);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2F30);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2F50);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2F70);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2F90);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2FB0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2FD0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B2FF0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3010);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3030);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3050);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3070);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3090);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B30B0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B30D0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B30F0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3110);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3130);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3150);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3170);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3190);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B31B0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B31D8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B31F8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3218);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3238);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3258);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3278);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3298);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B32B8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B32D8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B32F8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3318);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3338);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3358);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3378);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3398);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B33B8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B33D8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B33F8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3418);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3438);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3458);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3478);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3498);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B34B8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B34D8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B34F8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3518);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3538);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3558);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3578);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3598);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B35B8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B35D8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B35F8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3618);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3638);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3658);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3678);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3698);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B36B8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B36D8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B36F8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3718);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3738);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3758);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3778);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3798);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B37B8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B37C8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B37D8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B37E8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B37F8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3808);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3818);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3828);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3838);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3848);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3858);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3868);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3878);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3888);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3898);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B38A8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B38B8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B38C8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B38D8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B38E8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B38F8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3908);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3918);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3928);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3938);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3948);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3958);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3968);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3978);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3988);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3998);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B39A8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B39B8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B39C8);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B39E0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B39F0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B59A8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B65C0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B6778);

extern u32 D_0038F2F0[];

extern u32 D_0038EA6C[];

extern s32 fileQueueCreate(void);

u32 effReinitializeFileQueue(void) {
    if (D_003BD06C != 0) {
        func_002944D8(D_003BD05C);
        D_003BD05C = 0;
    }
    if (D_003BD06C != 0) {
        fileJobDestroy(D_003BD06C);
        D_003BD06C = 0;
    }
    if (D_003BD060 != 0) {
        func_002944D8(D_003BD060);
    }
    D_003BD060 = fileQueueCreate();
    D_0038EA6C[0] = 0;
    D_003DF8D0[0] = 0;
    D_003BD058 = 0;
    D_0038F2F0[3] = 0;
    D_003BD09C = (s32)D_0038F2F0;
    return 0;
}

extern u32 D_0038EB3C[];

extern void *fileQueueGetAt(s32, s32);

extern void func_00294E50(s32, void *);

u32 effResetFileQueueState(void) {
    func_00294E50(D_003BD060, fileQueueGetAt(D_003BD060, func_002B5990()));
    D_0038EB3C[0] = 0;
    D_003BD058 = 0;
    D_0038F2F0[3] = 0;
    D_003BD09C = (s32)D_0038F2F0;
    return 0;
}

extern u32 D_0038EAD4[];

extern void *fileJobDuplicateAfter(s32, void *);

extern void fileJobCopyHeader(void *, void *);

u32 effFinalizeQueuedFile(void) {
    s32 index = func_002B5990();
    void *record = fileQueueGetAt(D_003BD060, index);
    void *copy = fileJobDuplicateAfter(D_003BD060, record);
    D_0038EAD4[0] = 0;
    fileJobCopyHeader(copy, record);
    D_003BD09C = (s32)D_0038F2F0;
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B6C00);

u32 func_002B6FD8(void) {
    D_003BD955 = 1;
    func_002B7000(0);
    return 0;
}

u32 func_002B7000(void) {
    if (*(s32 *)(D_003BD098 + 0x34) != 0) {
        D_003BD09C = *(s32 *)(D_003BD098 + 0x34);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B7020);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B71D0);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B7388);

s32 func_002B7718(void) {
    return func_002B7388((s32)D_003B3968, D_0038DE70, 8);
}

s32 func_002B7740(void) {
    return func_002B7388((s32)D_003B3958, D_0038DF50, 6);
}

s32 effCreatePolyTrackTask(void) {
    return func_002B7388((s32)"POLY TRACK S", D_0038E000, 6);
}

s32 func_002B7790(void) {
    return func_002B7388((s32)D_003BCA40, D_0038E0F0, 6);
}

s32 func_002B77B8(void) {
    return func_002B7388((s32)D_003BD050, D_0038E1A0, 3);
}

s32 func_002B77E0(void) {
    return func_002B7388((s32)D_003B38C8, D_0038E280, 5);
}

s32 func_002B7808(void) {
    return func_002B7388((s32)D_003B3938, D_0038E240, 2);
}

s32 func_002B7830(void) {
    return func_002B7388((s32)D_003B3928, D_0038E310, 0xB);
}

s32 func_002B7858(void) {
    return func_002B7388((s32)D_003B3918, D_0038E450, 6);
}

s32 func_002B7880(void) {
    return func_002B7388((s32)D_003B3908, D_0038E500, 4);
}

s32 effCreatePolyTwinkleTask(void) {
    return func_002B7388((s32)"POLY TWINKLE", D_0038E570, 6);
}

s32 func_002B78D0(void) {
    return func_002B7388((s32)D_003B38F8, D_0038E620, 7);
}

s32 func_002B78F8(void) {
    return func_002B7388((s32)D_003B38E8, D_0038E6F0, 5);
}

s32 func_002B7920(void) {
    return func_002B7388((s32)D_003B38D8, D_0038E7C0, 2);
}

s32 func_002B7948(void) {
    return func_002B7388((s32)D_003B3888, D_0038E800, 0xA);
}

s32 func_002B7970(void) {
    return func_002B7388((s32)D_003BD000, D_0038E9A0, 2);
}

extern void func_002B8EA8(char *, u32, void *);

extern u8 *fileAppendJobFromEntry(s32, void *);

u32 pollEpEffectFileJob(void) {
    u8 record[0x110];
    u32 state;
    u32 result = 0x400001;

    func_002B8EA8(D_003B39C8, 0x20, record);
    state = *(u32 *)(record + 0x100);
    if (state == 2) {
        result = 0x400000;
    } else if (state == 1) {
        if (D_003BD060 != 0) {
            u8 *job = fileAppendJobFromEntry(D_003BD060, record);
            u8 *asset = effFindAssetData(job);
            strcpy((char *)job + 0x9C, *(char **)asset);
        }
        result = 0x400002;
    }
    return result;
}

extern s32 D_003BD070;

typedef struct EffectBlock128 {
    u32 word[32];
} EffectBlock128;

extern EffectBlock128 D_003DF920;

extern s32 func_002959E8(void *);

u32 pollNamedEffectFileJob(void) {
    u8 record[0x110];
    u32 state;
    u32 result = 0x400001;

    func_002B8EA8(D_003B39E0, 0x10, record);
    state = *(u32 *)(record + 0x100);
    if (state == 2) {
        result = 0x400000;
    } else if (state == 1) {
        if (D_003BD060 != 0) {
            func_002944D8(D_003BD060);
        }
        strcpy((char *)D_003DF8D0, (char *)record + 0xC8);
        D_003BD060 = func_002959E8(record);
        D_003DF920 = *(EffectBlock128 *)D_003BD060;
        D_003BD058 = 0;
        D_0038F2FC[0] = 0;
        result = 0x400002;
    }
    return result;
}

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3A90);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3AA8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B7B78);

extern u32 D_003DFA20[];

s32 effResetStaticState(void) {
    D_003DFA20[0] = 0;
    D_003DFA20[1] = 0;
    D_003DFA20[2] = 0;
    D_003DFA20[3] = 0;
    D_003DF840[0x88] = 8;
    D_003DF840[0x89] = 0;
    D_003DF840[0x8A] = 0;
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B7E60);

void func_002B80E0(void) {
    D_003BD108 = D_003DF840[0x88];
    D_003BD109 = D_003DF840[0x89];
    D_003BD10A = D_003DF840[0x8A];
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B8108);

s32 func_002B8618(void) {
    D_003DF840[0x88] = 8;
    D_003DF840[0x89] = 0;
    D_003DF840[0x8a] = 0;
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" :: "r"(D_003DFA30) : "memory");
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B8648);

u32 effLoadFileSlotAndPoll(void) {
    u8 *file = fileQueueGetAt(D_003BD060, func_002B5990());
    u32 result;

    memcpy(D_003DF840, file, 0x90);
    result = func_002B7E60(D_003DF840, 1);
    memcpy(file, D_003DF840, 0x90);
    if (result & 1) {
        result |= 0x800000;
    }
    return result;
}

typedef struct EffectStateSnapshot {
    s128 vectors[8];
} EffectStateSnapshot;

extern s32 func_002B7E60(EffectStateSnapshot *, s32);

s32 effRunWithStateBackup(void) {
    EffectStateSnapshot snapshot = *(EffectStateSnapshot *)&D_003DF920;
    s128 *backup = &snapshot.vectors[4];
    s32 result;

    PCP_COPY_VECTOR(backup, &D_003DF920);
    result = func_002B7E60(&snapshot, 0);
    PCP_COPY_VECTOR(&D_003DF920, backup);
    if (result & 1) {
        result |= 0x800000;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B8BF0);

extern u8 D_003BD954;

void effResetFileResources(void) {
    D_0038F2FC[0] = 0;
    D_003BD954 = 0;
    D_003BD058 = 0;
    if (D_003BD060 != 0) {
        func_002944D8(D_003BD060);
        D_003BD060 = 0;
    }
    if (D_003BD05C != 0) {
        func_002944D8(D_003BD05C);
        D_003BD05C = 0;
    }
    if (D_003BD06C != 0) {
        fileJobDestroy(D_003BD06C);
        D_003BD06C = 0;
    }
}

void func_002B8E60(void) {
    if (D_003BD110 != 0) {
        func_001FBEE8(D_003BD110);
        D_003BD110 = 0;
    }
    if (D_003BD10C != 0) {
        func_001FB870(D_003BD10C);
        D_003BD10C = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B8EA8);

extern u32 D_003BD114;

extern u8 D_003BCF30[];

extern void func_001FC7D0(u32, s32);

void effInitializeResourceQueue(void) {
    void *record;

    if (D_003BD114 != 0) {
        func_001FC2E8(D_003BD114);
    }
    D_003BD114 = func_001FC280(D_003BCF30);
    func_001FC720(D_003BD114, 0xC2, 0xC8);
    func_001FC7D0(D_003BD114, 9);
    record = fileQueueGetAt(D_003BD060, func_002B5990());
    func_001FC738(D_003BD114, (u8 *)record + 0x9C);
}

extern void func_001FC300(u32);

extern void func_001FC7A8(u32, void *);

extern u32 func_001FC730(u32);

u32 effPollResourceQueue(void) {
    u32 state;

    func_001FC300(D_003BD114);
    func_001FC7A8(D_003BD114, (u8 *)fileQueueGetAt(D_003BD060, func_002B5990()) + 0x9C);
    state = func_001FC730(D_003BD114);
    if ((u32)(state - 1) < 2) {
        func_001FC2E8(D_003BD114);
        D_003BD114 = 0;
        return 0;
    }
    return 0x200001;
}

void func_002B9050(void) {
    if (D_003BD118 != 0) {
        func_001FC2E8(D_003BD118);
        D_003BD118 = 0;
    }
}

void effQueueResource(void *unused, void *effect) {
    if (D_003BD118 == 0) {
        D_003BD118 = func_001FC280();
        func_001FC720(D_003BD118, 0xC2, 0xC8);
    }
    func_001FC738(D_003BD118, effect);
}

extern void func_001FC778(u32, void *);

extern u32 func_001FC7D8(u32, u32 *);

void effUpdateResourceQueue(u32 *result, void *queueData, u8 *record) {
    u32 state;

    if (D_003BD118 == 0) {
        D_003BD118 = func_001FC280(queueData);
        func_001FC720(D_003BD118, 0xC2, 0xC8);
        return;
    }
    func_001FC300(D_003BD118);
    func_001FC778(D_003BD118, record);
    func_001FC7A8(D_003BD118, record + 0x32);
    state = func_001FC730(D_003BD118);
    *(u32 *)(record + 0x64) = state;
    if (state == 1) {
        if (func_001FC7D8(D_003BD118, result) != 0) {
            func_001FC2E8(D_003BD118);
            D_003BD118 = 0;
        } else {
            *(u32 *)(record + 0x64) = 0;
        }
    }
}

void func_002B9188(void) {
    if (D_003BD110 != 0) {
        func_001FBEE8(D_003BD110);
        D_003BD110 = 0;
    }
    if (D_003BD10C != 0) {
        func_001FB870(D_003BD10C);
        D_003BD10C = 0;
    }
}

extern s32 func_001FB3C8(s32, s32);

extern s32 func_00151FC0(void);

extern void btlAppendEntry(s32, char *, s32, s32, s32);

extern s32 func_001FB9A8(s32);

extern void func_001FBF30(s32, s32, s32);

extern void func_001FBA38(s32);

extern s32 func_001FBF48(s32);

extern s32 func_001FBF50(s32, void *);

extern s32 func_001FC078(s32);

extern s32 func_003014F0(char *, const char *, ...);

typedef struct EffectPollRecord {
    u8 pad_00[0xC8];
    s32 type;   // 0xC8
    s32 state;  // 0xCC
    s32 value;  // 0xD0
} EffectPollRecord;

void pollEffectResourceBank(s32 flags, void *out) {
    EffectPollRecord *record = out;

    if (D_003BD10C == 0) {
        D_003BD10C = func_001FB3C8(0, flags);
        if (flags & 8) {
            u32 i = 0;
            u32 count = func_00151FC0();
            if (count != 0) {
                do {
                    char name[0x70];
                    func_003014F0(name, "GENERAL %d", i);
                    btlAppendEntry(D_003BD10C, name, 8, i, 0);
                    i++;
                } while (i < count);
            }
        }
        D_003BD110 = func_001FB9A8(D_003BD10C);
        func_001FBF30(D_003BD110, 0xBA, 0x1C);
    } else {
        func_001FBA38(D_003BD110);
        record->state = func_001FBF48(D_003BD110);
        record->type = func_001FBF50(D_003BD110, record);
        record->value = func_001FC078(D_003BD110);
        if (record->state == 1) {
            func_001FBEE8(D_003BD110);
            D_003BD110 = 0;
            func_001FB870(D_003BD10C);
            D_003BD10C = 0;
        }
    }
}

typedef struct EffectMapping {
    u8 pad_00[0x0C];
    u32 field0C;
    u32 field10;
    u8 *table;
    u32 count;
} EffectMapping;

extern EffectMapping D_0038F898;

void resetEffectMappingFlags(void) {
    D_003BD11C = 0;
    D_003BD128 = 0;
    D_003BD120 = 0;
    D_003BD124 = 1;
    D_0038F898.field0C = 1;
    D_0038F898.field10 |= 0x10;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002B9320);

extern u8 D_0038F7D8[];

extern u8 D_0038F718[];

extern u8 D_0038F658[];

s32 func_002BA058(s32 arg0) {
    s32 result;
    s32 object = *(s32 *)(arg0 + 0xC);

    D_0038F898.table = D_0038F7D8;
    D_0038F898.count = 8;
    result = func_002B9320(object + 0x2c, object + 0x50, *(s32 *)(object + 0xb8));
    D_0038F898.table = D_0038F658;
    D_0038F898.count = 8;
    return result;
}

s32 func_002BA0C0(s32 arg0) {
    s32 result;
    s32 object = *(s32 *)(arg0 + 0xC);

    D_0038F898.table = D_0038F7D8;
    D_0038F898.count = 8;
    result = func_002B9320(object + 0x48, object + 0x6c, *(s32 *)(object + 0xd4));
    D_0038F898.table = D_0038F658;
    D_0038F898.count = 8;
    return result;
}

void func_002BA128(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_002B9320(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0xb8));
}

s32 func_002BA148(void) {
    return func_002B9320((s32)D_003DE148, (s32)D_003DE148 + 0x24, *(s32 *)(D_003DE148 + 0x34));
}

s32 func_002BA170(s32 arg0) {
    s32 result;
    s32 object = *(s32 *)(arg0 + 0xC);

    D_0038F898.table = D_0038F7D8;
    D_0038F898.count = 8;
    result = func_002B9320(object, object + 0x24, *(s32 *)(object + 0x8c));
    D_0038F898.table = D_0038F658;
    D_0038F898.count = 8;
    return result;
}

void func_002BA1D0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_002B9320(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

void func_002BA1F0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_002B9320(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x38));
}

void func_002BA210(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_002B9320(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

s32 func_002BA230(s32 arg0) {
    s32 result;
    s32 object = *(s32 *)(arg0 + 0xC);

    D_0038F898.table = D_0038F718;
    D_0038F898.count = 8;
    result = func_002B9320(object, object + 0x24, *(s32 *)(object + 0x34));
    D_0038F898.table = D_0038F658;
    D_0038F898.count = 8;
    return result;
}

void func_002BA290(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_002B9320(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

void func_002BA2B0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_002B9320(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

s32 func_002BA2D0(s32 arg0) {
    s32 result;
    s32 object = *(s32 *)(arg0 + 0xC);

    D_0038F898.table = D_0038F7D8;
    D_0038F898.count = 8;
    result = func_002B9320(object + 0x4c, object + 0x70, *(s32 *)(object + 0xd8));
    D_0038F898.table = D_0038F658;
    D_0038F898.count = 8;
    return result;
}

void func_002BA338(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_002B9320(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

void func_002BA358(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_002B9320(temp_v0 + 0x3c, temp_v0 + 0x60, *(u32 *)(temp_v0 + 0x80));
}

void func_002BA380(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_002B9320(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

void func_002BA3A0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_002B9320(temp_v0 + 0x3c, temp_v0 + 0x60, *(u32 *)(temp_v0 + 0x80));
}

s32 func_002BA3C8(s32 arg0) {
    s32 result;
    s32 object = *(s32 *)(arg0 + 0xC);

    D_0038F898.table = D_0038F7D8;
    D_0038F898.count = 8;
    result = func_002B9320(object + 0x68, object + 0x8c, *(s32 *)(object + 0xf4));
    D_0038F898.table = D_0038F658;
    D_0038F898.count = 8;
    return result;
}

void func_002BA430(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_002B9320(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x70));
}

void func_002BA450(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_002B9320(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

void func_002BA470(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_002B9320(temp_v0 + 0x50, temp_v0 + 0x74, *(u32 *)(temp_v0 + 0x84));
}

s32 func_002BA498(s32 arg0) {
    s32 result;
    s32 object = *(s32 *)(arg0 + 0xC);

    D_0038F898.table = D_0038F718;
    D_0038F898.count = 8;
    result = func_002B9320(object, object + 0x24, *(s32 *)(object + 0x8c));
    D_0038F898.table = D_0038F658;
    D_0038F898.count = 8;
    return result;
}

void btlResetEffectWork(void) {
    u8 *first = D_0038F9D0;
    u8 *second = D_0038FAD0;

    *(u32 *)(first + 0x10) |= 0x10;
    *(u32 *)(first + 0x0c) = 0;
    *(u32 *)(second + 0x10) |= 0x10;
    *(u32 *)(second + 0x0c) = 0;
    D_003BD158 = 0;
    D_003BD15C = 0;
    D_003BD160 = 0;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BA538);

s32 func_002BAE18(BaObj *p) {
    return func_002BA538(p->x0C + 0x60, *(s32 *)(p->x0C + 0xB8), D_0038F9D0, D_0038F8E8);
}

s32 func_002BAE48(BaObj *p) {
    return func_002BA538(p->x0C + 0x8C, *(s32 *)(p->x0C + 0xB8), D_0038FAD0, D_0038FA20);
}

s32 func_002BAE78(BaObj *p) {
    return func_002BA538(p->x0C + 0x7C, *(s32 *)(p->x0C + 0xD4), D_0038F9D0, D_0038F8E8);
}

s32 func_002BAEA8(BaObj *p) {
    return func_002BA538(p->x0C + 0xA8, *(s32 *)(p->x0C + 0xD4), D_0038FAD0, D_0038FA20);
}

s32 func_002BAED8(BaObj *p) {
    return func_002BA538(p->x0C + 0x34, *(s32 *)(p->x0C + 0xB8), D_0038F9D0, D_0038F8E8);
}

s32 func_002BAF08(BaObj *p) {
    return func_002BA538(p->x0C + 0x60, *(s32 *)(p->x0C + 0xB8), D_0038F9D0, D_0038F8E8);
}

s32 func_002BAF38(BaObj *p) {
    return func_002BA538(p->x0C + 0x8C, *(s32 *)(p->x0C + 0xB8), D_0038F9D0, D_0038F8E8);
}

s32 func_002BAF68(BaObj *p) {
    return func_002BA538(p->x0C + 0x34, *(s32 *)(p->x0C + 0x8C), D_0038F9D0, D_0038F8E8);
}

s32 func_002BAF98(BaObj *p) {
    return func_002BA538(p->x0C + 0x60, *(s32 *)(p->x0C + 0x8C), D_0038F9D0, D_0038F8E8);
}

s32 func_002BAFC8(BaObj *p) {
    return func_002BA538(p->x0C + 0x80, *(s32 *)(p->x0C + 0xD8), D_0038F9D0, D_0038F8E8);
}

s32 func_002BAFF8(BaObj *p) {
    return func_002BA538(p->x0C + 0xAC, *(s32 *)(p->x0C + 0xD8), D_0038FAD0, D_0038FA20);
}

s32 func_002BB028(BaObj *p) {
    return func_002BA538(p->x0C + 0x90, *(s32 *)(p->x0C + 0x80), D_0038F9D0, D_0038F938);
}

s32 func_002BB058(BaObj *p) {
    return func_002BA538(p->x0C + 0x9C, *(s32 *)(p->x0C + 0xF4), D_0038F9D0, D_0038F8E8);
}

s32 func_002BB088(BaObj *p) {
    return func_002BA538(p->x0C + 0xC8, *(s32 *)(p->x0C + 0xF4), D_0038FAD0, D_0038FA20);
}

s32 func_002BB0B8(BaObj *p) {
    return func_002BA538(p->x0C, *(s32 *)(p->x0C + 0xF4), D_0038F9D0, D_0038F938);
}

s32 func_002BB0E8(BaObj *p) {
    return func_002BA538(p->x0C + 0x34, *(s32 *)(p->x0C + 0x70), D_0038F9D0, D_0038F938);
}

s32 func_002BB118(BaObj *p) {
    return func_002BA538(p->x0C + 0x34, *(s32 *)(p->x0C + 0x8C), D_0038F9D0, D_0038F8E8);
}

s32 func_002BB148(BaObj *p) {
    return func_002BA538(p->x0C + 0x60, *(s32 *)(p->x0C + 0x8C), D_0038F9D0, D_0038F8E8);
}

void func_002BB178(void) {
    D_003BD124 = 1;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BB188);

void func_002BB6B0(s32 arg0) {
    func_002BB188(*(s32 *)(arg0 + 0xc) + 0x3c, *(u32 *)(*(s32 *)(arg0 + 0xc) + 0x4c));
}

void func_002BB6D0(s32 arg0) {
    func_002BB188(*(s32 *)(arg0 + 0xc) + 0x60, *(s32 *)(*(s32 *)(arg0 + 0xc) + 0x3c) + 1);
}

void func_002BB6F8(s32 arg0) {
    func_002BB188(*(s32 *)(arg0 + 0xc) + 0x70, *(s32 *)(*(s32 *)(arg0 + 0xc) + 0x8c) + 1);
}

void func_002BB720(s32 arg0) {
    func_002BB188(*(s32 *)(arg0 + 0xc) + 0x70, *(s32 *)(*(s32 *)(arg0 + 0xc) + 0x8c) + 1);
}

void func_002BB748(s32 arg0) {
    func_002BB188(*(s32 *)(arg0 + 0xc) + 0x60, *(s32 *)(*(s32 *)(arg0 + 0xc) + 0x74) + 1);
}

extern void func_00294DA0(s32, s32);

extern void func_00294C30(s32, s32, void *);

extern void pollEffectResourceBank(s32, void *);

s32 pollEffectFileQueueRecord(s32 arg0) {
    u8 record[0xE0];
    s32 kind;
    s32 result = 0x600001;

    pollEffectResourceBank(arg0, record);
    kind = *(s32 *)(record + 0xCC);
    if (kind == 2) {
        result = 0x400000;
    } else if (kind == 1) {
        if (*(s32 *)(record + 0xC8) != 8) {
            void *entry = fileQueueGetAt(D_003BD060, -*(s32 *)(record + 0xD0));
            if (D_003BD070 != (s32)entry) {
                func_00294DA0(D_003BD060, D_003BD070);
                func_00294C30(D_003BD060, D_003BD070, entry);
            }
        } else {
            func_00294DA0(D_003BD060, D_003BD070);
            func_00293960(D_003BD068, record + 0xD0, 4, 4);
        }
        result = 0x400002;
    }
    return result;
}

void func_002BB838(void) {
    pollEffectFileQueueRecord(0x4b);
}

void func_002BB850(void) {
    pollEffectFileQueueRecord(0x4b);
}

void func_002BB868(void) {
    pollEffectFileQueueRecord(0xb);
}

extern s32 func_002BC538(s32);

s32 pollEffectFileRecord(char *path, s32 arg1) {
    u8 record[0x110];
    s32 kind;
    s32 result = 0x600001;

    func_002B8EA8(path, arg1, record);
    kind = *(s32 *)(record + 0x100);
    if (kind == 2) {
        result = 0x400000;
    } else if (kind == 1) {
        s32 type;
        func_00294DA0(D_003BD060, D_003BD070);
        type = *(s32 *)(record + 0xFC);
        if (type != 8) {
            func_00293A00(D_003BD068, (u32)record, func_002BC538(type));
        } else {
            func_00293960(D_003BD068, record + 0x104, 4, 4);
        }
        result = 0x400002;
    }
    return result;
}

s32 func_002BB930(void) {
    return pollEffectFileRecord(D_003B3B88, 0x43);
}

s32 func_002BB950(void) {
    return pollEffectFileRecord(D_003B3B88, 0x43);
}

s32 func_002BB970(void) {
    return pollEffectFileRecord(D_003B39C8, 0x20);
}

s32 func_002BB990(void) {
    return pollEffectFileRecord(D_003B39E0, 0x10);
}

s32 func_002BB9B0(void) {
    return pollEffectFileRecord(D_003B39C8, 0x20);
}

s32 func_002BB9D0(void) {
    return pollEffectFileRecord(D_003B3B88, 1);
}

s32 func_002BB9F0(void) {
    return pollEffectFileRecord(D_003B3B88, 1);
}

s32 func_002BBA10(void) {
    return pollEffectFileRecord(D_003B3B88, 1);
}

u32 func_002BBA30(void) {
    u32 zero = 0;
    func_00293960(D_003BD068, &zero, 4, 4);
    return 0x400002;
}

s32 func_002BBA68(void) {
    return pollEffectFileRecord(D_003B3BA0, 4);
}

typedef struct EffectFileHeader {
    u8 unk_00[8];
    u16 mode;
    u8 unk_0A[2];
    u32 start;
    u32 length;
} EffectFileHeader;

extern u32 D_003BD064;

extern void func_002BC510(void);

extern EffectFileHeader D_0038DC08;

extern EffectFileHeader D_00383F18;

extern EffectFileHeader D_0038C758;

u32 fileLoadEffectSlotA(void) {
    u8 fileInfo[0x110];
    u8 *job;
    u8 *entry;
    u8 *resource;
    void *fileData;
    s32 status;
    u32 result;

    func_002B8EA8(D_003B3B88, 4, fileInfo);
    status = *(s32 *)(fileInfo + 0x100);
    result = 0x600001;
    if (status == 2) {
        result = 0x400000;
    } else if (status == 1) {
        job = (u8 *)fileCreateJob(3);
        func_002937E0(job, D_00383F18.start, D_00383F18.length,
                      D_00383F18.mode);
        func_00293A00(job, fileInfo, func_002BC538(*(u32 *)(fileInfo + 0xFC)));
        entry = (u8 *)fileAppendJob(D_003BD060, job);
        D_003BD070 = (s32)entry;
        memcpy(D_003DF9A0, entry, 0x80);
        D_003BD068 = *(u32 *)(entry + 0x90);
        resource = (u8 *)effFindAssetData(entry);
        strcpy((char *)(entry + 0x9C), *(char **)resource);
        fileData = fileResolvePrimaryBuffer(D_003BD068);
        memcpy(*(void **)(resource + 0xC), fileData,
               *(u32 *)(resource + 0x10));
        D_003BD064 = func_002B5390(resource);
        D_003BD09C = effFindAssetObject(entry);
        *(u8 **)(D_003BD09C + 0x34) = (u8 *)D_0038F2F0;
        func_002BC510();
        if (D_003BD06C != 0) {
            fileJobDestroy(D_003BD06C);
            D_003BD06C = 0;
        }
        result = 0x800002;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BBC70);

u32 fileLoadEffectSlotB(void) {
    u8 fileInfo[0x110];
    u8 *job;
    u8 *entry;
    u8 *resource;
    void *fileData;
    s32 status;
    u32 result;

    func_002B8EA8(D_003B3B88, 4, fileInfo);
    status = *(s32 *)(fileInfo + 0x100);
    result = 0x600001;
    if (status == 2) {
        result = 0x400000;
    } else if (status == 1) {
        job = (u8 *)fileCreateJob(18);
        func_002937E0(job, D_0038C758.start, D_0038C758.length,
                      D_0038C758.mode);
        func_00293A00(job, fileInfo, func_002BC538(*(u32 *)(fileInfo + 0xFC)));
        entry = (u8 *)fileAppendJob(D_003BD060, job);
        D_003BD070 = (s32)entry;
        memcpy(D_003DF9A0, entry, 0x80);
        D_003BD068 = *(u32 *)(entry + 0x90);
        resource = (u8 *)effFindAssetData(entry);
        strcpy((char *)(entry + 0x9C), *(char **)resource);
        fileData = fileResolvePrimaryBuffer(D_003BD068);
        memcpy(*(void **)(resource + 0xC), fileData,
               *(u32 *)(resource + 0x10));
        D_003BD064 = func_002B5390(resource);
        D_003BD09C = effFindAssetObject(entry);
        *(u8 **)(D_003BD09C + 0x34) = (u8 *)D_0038F2F0;
        func_002BC510();
        if (D_003BD06C != 0) {
            fileJobDestroy(D_003BD06C);
            D_003BD06C = 0;
        }
        result = 0x800002;
    }
    return result;
}

extern EffectFileHeader D_0038D470;

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3B18);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3B28);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3B38);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3B48);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3B58);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3B68);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3B78);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3B88);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3BA0);

u32 effLoadFileSlotF2(void) {
    u8 fileInfo[0x110];
    u8 *job;
    u8 *entry;
    u8 *resource;
    void *fileData;
    s32 status;
    u32 result;

    func_002B8EA8("/tool/effect/f2/", 0x80, fileInfo);
    status = *(s32 *)(fileInfo + 0x100);
    result = 0x600001;
    if (status == 2) {
        result = 0x400000;
    } else if (status == 1) {
        job = (u8 *)fileCreateJob(0x14);
        func_002937E0(job, D_0038D470.start, D_0038D470.length,
                      D_0038D470.mode);
        func_00293A00(job, fileInfo, func_002BC538(*(u32 *)(fileInfo + 0xFC)));
        entry = (u8 *)fileAppendJob(D_003BD060, job);
        D_003BD070 = (s32)entry;
        memcpy(D_003DF9A0, entry, 0x80);
        D_003BD068 = *(u32 *)(entry + 0x90);
        resource = (u8 *)effFindAssetData(entry);
        strcpy((char *)(entry + 0x9C), *(char **)resource);
        fileData = fileResolvePrimaryBuffer(D_003BD068);
        memcpy(*(void **)(resource + 0xC), fileData,
               *(u32 *)(resource + 0x10));
        D_003BD064 = func_002B5390(resource);
        D_003BD09C = effFindAssetObject(entry);
        *(u8 **)(D_003BD09C + 0x34) = (u8 *)D_0038F2F0;
        func_002BC510();
        if (D_003BD06C != 0) {
            fileJobDestroy(D_003BD06C);
            D_003BD06C = 0;
        }
        result = 0x800002;
    }
    return result;
}

u32 effLoadMaterialFile(void) {
    u8 fileInfo[0x110];
    u8 *job;
    u8 *entry;
    u8 *resource;
    void *fileData;
    s32 status;
    u32 result;

    func_002B8EA8(D_003B3B88, 2, fileInfo);
    status = *(s32 *)(fileInfo + 0x100);
    result = 0x600001;
    if (status == 2) {
        result = 0x400000;
    } else if (status == 1) {
        job = (u8 *)fileCreateJob(0x16);
        func_002937E0(job, D_0038DC08.start, D_0038DC08.length,
                      D_0038DC08.mode);
        func_00293A00(job, fileInfo, func_002BC538(*(u32 *)(fileInfo + 0xFC)));
        entry = (u8 *)fileAppendJob(D_003BD060, job);
        D_003BD070 = (s32)entry;
        memcpy(D_003DF9A0, entry, 0x80);
        D_003BD068 = *(u32 *)(entry + 0x90);
        resource = (u8 *)effFindAssetData(entry);
        strcpy((char *)(entry + 0x9C), *(char **)resource);
        fileData = fileResolvePrimaryBuffer(D_003BD068);
        memcpy(*(void **)(resource + 0xC), fileData,
               *(u32 *)(resource + 0x10));
        D_003BD064 = func_002B5390(resource);
        D_003BD09C = effFindAssetObject(entry);
        *(u8 **)(D_003BD09C + 0x34) = (u8 *)D_0038F2F0;
        func_002BC510();
        if (D_003BD06C != 0) {
            fileJobDestroy(D_003BD06C);
            D_003BD06C = 0;
        }
        result = 0x800002;
    }
    return result;
}

void func_002BC510(void) {
    D_003BD11C = 0;
    D_003BD124 = 1;
    D_003BD120 = 0;
    D_003BD128 = 0;
    D_003BD158 = 0;
    D_003BD15C = 0;
    D_003BD160 = 0;
}

s32 func_002BC538(s32 flags) {
    switch (flags) {
        case 1:   return 1;
        case 2:   return 2;
        case 4:   return 3;
        case 8:   return 4;
        case 16:  return 6;
        case 32:  return 5;
        case 64:  return 7;
        case 128: return 8;
        default:  return 0;
    }
}

extern void *func_002CFEB8(u32);

void *func_002BC5C0(void *owner) {
    u32 *data = func_002CFEB8(0x14);
    memset(data, 0, 0x14);
    data[0] = (u32)owner;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;
    return data;
}

void func_002BC618(void) {
    func_002CFF98();
}

u32 func_002BC630(u32 *arg0) {
    return *arg0;
}

typedef struct EffectListNode {
    u32 unknown;
    struct EffectListNode *next;
    u32 unk_08;
    u32 unk_0C;
    u32 unk_10;
    u32 unk_14;
} EffectListNode;

typedef struct EffectList {
    void *owner;
    s32 count;
    EffectListNode *head;
    EffectListNode *tail;
    u32 unk_10;
} EffectList;

s32 effAppendListEntry(EffectList *list, u32 field08, u32 field0C, u32 field10, u32 field14) {
    EffectListNode *node = func_002CFEB8(sizeof(EffectListNode));
    memset(node, 0, sizeof(EffectListNode));
    node->next = NULL;
    node->unk_10 = field10;
    node->unk_08 = field08;
    node->unk_0C = field0C;
    node->unk_14 = field14;
    if (list->tail == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
    return ++list->count;
}

extern void func_002CFF98(void *);

s32 effRemoveListEntry(EffectList *list) {
    EffectListNode *node = list->head;
    EffectListNode *next = node->next;
    func_002CFF98(node);
    list->head = next;
    if (--list->count == 0) {
        list->tail = NULL;
    }
    return list->count;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BC748);

extern char D_003BD198[];

u32 effLoadIndexedResource(const char *base, const char *name, u32 retainResource) {
    char path[0x80];
    u32 handle;
    u32 resource;
    u32 result;

    func_003014F0(path, D_003BD198, base, name);
    resource = func_002EB028(path, &handle, 0);
    result = func_002BD9C0(resource, retainResource);
    if (retainResource == 0) {
        func_002D0918(resource);
    }
    return result;
}

void func_002BC978(u64 arg0, u32 *arg1) {
    u32 temp_v0;
    u32 temp_v1;

    temp_v0 = fileGetResourceHandle();
    temp_v1 = func_002BD9C0(temp_v0, 0);
    *arg1 = temp_v1;
    func_002D0918(temp_v0);
    func_002887A0(arg0);
}

void func_002BC9D0(u64 arg0, u32 *arg1) {
    u32 temp_v0;
    u32 temp_v1;

    temp_v0 = fileGetResourceHandle();
    temp_v1 = func_002BD9C0(temp_v0, 1);
    *arg1 = temp_v1;
    func_002887A0(arg0);
}

extern s32 func_003014F0(char *, const char *, ...);

extern void func_00288AD0(const char *, u32, void (*)(u64, u32 *), u32 *);

void effRequestResourceByMode(const char *base, const char *name, u32 mode, u32 *out) {
    char path[0x80];

    func_003014F0(path, D_003BD198, base, name);
    *out = 0;
    if (mode == 1) {
        func_00288AD0(path, 0, func_002BC9D0, out);
    } else {
        func_00288AD0(path, 0, func_002BC978, out);
    }
}

u32 effLoadMappedResource(const char *base, const char *name) {
    char path[0x80];
    u32 handle;
    u32 value;
    u32 resource;

    func_003014F0(path, D_003BD198, base, name);
    resource = func_002EB028(path, &handle, 0);
    value = func_002BD1F8(handle);
    func_002D0918(resource);
    return value;
}

void func_002BCB18(u64 arg0, u32 *arg1) {
    u32 temp_v0;
    u32 temp_v1;
    u32 temp_v2;

    temp_v0 = fileGetResourceHandle();
    temp_v1 = sdfResourceRetainAddress(temp_v0);
    temp_v2 = func_002BD1F8(temp_v1);
    *arg1 = temp_v2;
    func_002D0918(temp_v0);
    func_002887A0(arg0);
}

void effRequestMappedResource(const char *base, const char *name, u32 *out) {
    char path[0x80];

    func_003014F0(path, D_003BD198, base, name);
    *out = 0;
    func_00288AD0(path, 0, func_002BCB18, out);
}

void *func_002BCBD0(void *owner) {
    u32 *data = func_002CFEB8(0x44);
    memset(data, 0, 0x44);
    data[0] = (u32)owner;
    return data;
}

typedef struct EffectRecord {
    void *owner;
    s32 slot;
    struct EffectRecord *prev;
    struct EffectRecord *next;
} EffectRecord;

typedef struct EffectOwnerRecord {
    void *owner;
    EffectRecord *entries[16];
} EffectOwnerRecord;

/* Per-slot effect data (0x80 bytes each); only the bucket index is known. */
typedef struct EffectSlot {
    u8 pad_0x00[0x28]; // 0x00
    s32 bucket;        // 0x28
    u8 pad_0x2C[0x54]; // 0x2C
} EffectSlot; // 0x80

typedef struct EffectSlotOwner {
    u8 pad_0x00[0x10];  // 0x00
    EffectSlot *slots;  // 0x10
} EffectSlotOwner;

void func_002BCC20(void *owner, EffectOwnerRecord *list, EffectSlotOwner *work, s32 slot) {
    EffectSlot *entry = &work->slots[slot];
    s32 bucket = entry->bucket;
    EffectRecord *record;

    if (bucket >= 16) {
        bucket = 15;
    }
    record = func_002CFEB8(sizeof(*record));
    record->prev = 0;
    record->owner = owner;
    record->slot = slot;
    record->next = list->entries[bucket];
    list->entries[bucket] = record;
}

s32 func_002BCCA8(EffectOwnerRecord *list, EffectSlotOwner *work, s32 slot) {
    EffectSlot *slotData = &work->slots[slot];
    s32 bucket = slotData->bucket;
    EffectRecord *record = list->entries[bucket];

    while (record != 0) {
        if (record->slot == slot) {
            if (record->prev != 0) {
                record->prev->next = record->next;
            }
            if (record->next != 0) {
                record->next->prev = record->prev;
            }
            if (record == list->entries[bucket]) {
                list->entries[bucket] = record->next;
            }
            func_002CFF98(record);
            return 1;
        }
        record = record->next;
    }
    return 0;
}

void effReleaseRecordBuckets(list)
EffectOwnerRecord *list;
{
    EffectRecord **entry = list->entries;
    s32 remaining = 15;

    do {
        EffectRecord *record = *entry;
        while (record != 0) {
            if (record->prev != 0) {
                record->prev->next = record->next;
            }
            if (record->next != 0) {
                record->next->prev = record->prev;
            }
            if (record == *entry) {
                *entry = record->next;
            }
            func_002CFF98(record);
            record = record->next;
        }
        ++entry;
    } while (--remaining >= 0);
}

extern void func_002BF790(u32, u32, u32, u32, u32, s32, s32);

extern void func_002BF970(void *, s32);

u32 effDispatchRecordBuckets(u32 active, EffectOwnerRecord *list, s32 option) {
    EffectRecord **entry = list->entries;
    s32 remaining = 15;

    do {
        EffectRecord *record = *entry;
        while (record != 0) {
            func_002BF790(0, 0, 0, 0, (u32)list->owner, record->slot, option);
            if (active != 0) {
                func_002BF970(list->owner, record->slot);
            }
            record = record->next;
        }
        ++entry;
    } while (--remaining >= 0);
    return 1;
}

u32 func_002BCEA8(u32 arg0) {
    effReleaseRecordBuckets();
    func_002CFF98(arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BCED8);

typedef struct EffectRecordGroup {
    u32 unk_00;
    u32 unk_04;
    u32 count;
    u8 *records;
} EffectRecordGroup;

extern EffectRecordGroup D_0038FD88[];

extern u32 func_002BCED8(u32 *, void *, void *, void *);

u32 effSumRecordStatuses(u32 *payload) {
    EffectRecordGroup *group = &D_0038FD88[payload[5]];
    u32 total = 0;
    u32 i;

    for (i = 0; i < group->count; i++) {
        total += func_002BCED8((u32 *)(group->records + i * 0x18 + 4), 0, 0, 0);
    }
    return total;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BD028);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BD1F8);

u32 *func_002BD258(u32 kind) {
    u32 *header = func_002CFEB8(0xC);
    u32 allocation;
    u32 data;
    u32 size;
    void *scratch;

    header[0] = 1;
    allocation = (u32)func_002D03F8(0x24);
    header[1] = allocation;
    data = sdfResourceRetainAddress(allocation);
    header[2] = data;
    memset((void *)data, 0, 0x24);
    {
        u32 *payload = (u32 *)header[2];
        payload[5] = kind;
        size = effSumRecordStatuses(payload);
    }
    scratch = func_002CFEB8(size);
    ((u32 *)header[2])[8] = (u32)scratch;
    memset(scratch, 0, size);
    ((u32 *)header[2])[6] = size;
    return header;
}

typedef struct PackedEffectRecord {
    u8 unk_00[0x20];
    void *storage;
} PackedEffectRecord;

typedef struct PackedEffectBatch {
    s32 count;
    u32 job;
    PackedEffectRecord *records;
} PackedEffectBatch;

u32 effDestroyPackedBatch(PackedEffectBatch *batch) {
    s32 i;
    for (i = 0; i < batch->count; i++) {
        func_002CFF98(batch->records[i].storage);
    }
    func_002D0918(batch->job);
    func_002CFF98(batch);
    return 1;
}

u32 func_002BD378(s32 arg0) {
    func_002D0918(*(u32 *)(arg0 + 0x14));
    return 1;
}

s32 func_002BD398(s32 arg0, s32 arg1) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = arg1 * 0xa0 + *(s32 *)(arg0 + 0x18);
    temp_v0 = *(s32 *)(temp_v1 + 0x9c);
    if (temp_v0 != 0) {
        temp_v1 = temp_v0;
    }
    return temp_v1;
}

void func_002BD3B8(BdWork *p) {
    if (p->x0 & 1) {
        p->x4 = 0;
    } else {
        p->x4 = 0x10000;
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BD3D8);

void func_002BD5A0(s32 arg0, s32 arg1) {
    func_002BD3D8(arg0, arg1, *(s32 *)(arg0 + 0x18) + arg1 * 0xa0);
}

void func_002BD5C8(s32 work) {
    u32 index;
    for (index = 0; index < *(u32 *)(work + 8); index++) {
        func_002BD640(work, index);
    }
}

void func_002BD620(void *a0, s32 a1, BdWork *p) {
    p->x60 = a0;
    p->x64 = a1;
    func_002BD3D8(a0, a1, p);
}

void func_002BD640(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)((s32)arg0 + 0x18) + (s32)arg1 * 0xa0;
    memset(temp_v0, 0, 0xa0);
    func_002BD620(arg0, arg1, temp_v0);
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BD6A8);

extern u32 func_002BD6A8(u32 *, u32, s32, s32);

void effResolveAndReleaseResource(u32 *handle) {
    if (*handle != 0) {
        u32 data = sdfResourceRetainAddress(*handle);
        func_002BD6A8(handle, data, 0, -1);
        func_002D0A60(*handle);
    }
}

void effResolveAndReleaseSelectedResource(u32 *handle, s32 slot) {
    if (*handle != 0) {
        u32 data = sdfResourceRetainAddress(*handle);
        func_002BD6A8(handle, data, 0, slot);
        func_002D0A60(*handle);
    }
}

void func_002BD870(TexHandleSet *set) {
    u32 i;

    for (i = 0; i < set->count; i++) {
        if (set->handles[i] != 0) {
            sdfTexReleaseReference(set->handles[i]);
            set->handles[i] = 0;
        }
    }
    func_002BD5C8((s32)set);
}

u8 func_002BD8F8(s32 arg0) {
    return **(s32 **)(arg0 + 0x24) != 0;
}

u32 *effCreatePayload(u32 count) {
    u32 size = count * 0x6c;
    u32 *header = func_002CFEB8(0xC);
    u32 allocation = (u32)func_002D03F8(size);
    u32 data;

    header[1] = count;
    header[0] = allocation;
    data = sdfResourceRetainAddress(allocation);
    header[2] = data;
    memset((void *)data, 0, size);
    return header;
}

u32 func_002BD988(u32 arg0) {
    func_002D0918(*(u32 *)arg0);
    func_002CFF98(arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BD9C0);

extern void func_002BD640(u32, u32);

u32 *effCreateResourceSlotSet(u32 *source, u32 slot, u32 count) {
    u32 *effect = (u32 *)func_002CFEB8(0x30);
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
    effect[3] = (u32)func_002D03F8(count * 0x80);
    effect[4] = sdfResourceRetainAddress(effect[3]);
    effect[5] = (u32)func_002D03F8(effect[2] * 0xA0);
    effect[6] = sdfResourceRetainAddress(effect[5]);
    if (effect[2] != 0) {
        do {
            memcpy((void *)(effect[4] + index * 0x80),
                   (void *)(source[4] + slot * 0x80), 0x80);
            func_002BD640((u32)effect, index);
            index++;
        } while (index < effect[2]);
    }
    return effect;
}

u32 func_002BDD60(u32 arg0) {
    s32 *piVar1;

    piVar1 = (s32 *)arg0;
    if (*piVar1 != 0) {
        func_002D0918(*piVar1);
    }
    if (piVar1[1] == 0) {
        func_002BD870(arg0);
        func_002D0918(piVar1[8]);
    }
    func_002D0918(piVar1[3]);
    func_002BD378(arg0);
    func_002CFF98(arg0);
    return 1;
}

u32 func_002BDDD0(u32 *arg0, u32 arg1, u32 arg2) {
    *arg0 = arg2;
    arg0[4] = arg1;
    if ((arg2 & 2) != 0) {
        func_002BD3B8((BdWork *)arg0);
    }
    arg0[2] = arg0[2] + 1;
    return 1;
}

u32 func_002BDE18(u32 arg0, s32 arg1, s32 arg2, u32 arg3) {
    func_002BDDD0(arg0, *(s32 *)(arg1 + 8) + arg2 * 0x24, arg3);
    return 1;
}

u32 func_002BDE50(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = 0;
    return 1;
}

u32 func_002BDE60(s32 work, s32 index, BdWork *effect) {
    if (effect->x4 > 0x10000) {
        s32 flags = effect->x0;
        effect->x4 = 0x10000;
        if (flags & 4) {
            if (flags & 8) {
                effect->x0 = flags & ~1;
            } else {
                func_002BD5A0(work, index);
            }
            return 0;
        }
    }
    return 1;
}

u32 func_002BDEC8(s32 work, s32 index, BdWork *effect) {
    if (effect->x4 < 0) {
        s32 flags = effect->x0;
        effect->x4 = 0;
        if (flags & 4) {
            if (flags & 8) {
                effect->x0 = flags | 1;
            } else {
                func_002BD5A0(work, index);
            }
            return 0;
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BDF28);

u32 func_002BE0C0(s32 work, s32 index, void *value) {
    s32 offset = index * 0xA0;
    if (*(void **)(offset + *(s32 *)(work + 0x18) + 0x9C) == 0) {
        func_002BD620((void *)work, index, (BdWork *)value);
    }
    *(void **)(offset + *(s32 *)(work + 0x18) + 0x9C) = value;
    return 1;
}

typedef struct EffectMaterialSlot {
    u32 value;
    u8 unk_04[0x10];
} EffectMaterialSlot;

u32 effSetMaterialSlots(s32 work, s32 index, u32 value, BdWork *asset) {
    s32 offset = index * 0xA0;
    u32 i;
    EffectMaterialSlot *slots;
    if (*(void **)(offset + *(s32 *)(work + 0x18) + 0x9C) == NULL) {
        func_002BD620((void *)work, index, asset);
    }
    *(BdWork **)(offset + *(s32 *)(work + 0x18) + 0x9C) = asset;
    slots = (EffectMaterialSlot *)((u8 *)asset + 0x30);
    for (i = 0; i < 2; i++) {
        slots[i].value = value;
    }
    return 1;
}

u32 func_002BE1C8(s32 arg0, s32 arg1) {
    *(u32 *)(arg1 * 0xa0 + *(s32 *)(arg0 + 0x18) + 0x9c) = 0;
    return 1;
}

extern u32 func_002BDF28(s32, s32, BdWork *);

u32 func_002BE1E8(s32 work, s32 index, u32 value, u32 flags) {
    s32 effect = *(s32 *)(work + 0x18) + index * 0xA0;
    func_002BDDD0((u32 *)(effect + 0x28), value, flags);
    func_002BDF28(work, index, (BdWork *)effect);
    return 1;
}

u32 func_002BE258(s32 work, s32 index, s32 data, s32 item, u32 flags) {
    s32 effect = *(s32 *)(work + 0x18) + index * 0xA0;
    func_002BDDD0((u32 *)(effect + 0x28), *(u32 *)(data + 8) + item * 0x24, flags);
    func_002BDF28(work, index, (BdWork *)effect);
    return 1;
}

u32 func_002BE2D8(s32 work, s32 index, s32 data, s32 item,
                  u32 flags, u32 color, u32 option) {
    s32 effect = *(s32 *)(work + 0x18) + index * 0xA0;
    func_002BDDD0((u32 *)(effect + 0x28), *(u32 *)(data + 8) + item * 0x24, option);
    func_002BDF28(work, index, (BdWork *)effect);
    *(u32 *)(effect + 0x30) = flags;
    *(u32 *)(effect + 0x34) = color;
    return 1;
}

u32 effConfigureWithDefaultSetting(u32 effect, u32 slot, u32 kind, u32 value, u32 flags, u32 color) {
    func_002BE2D8(effect, slot, kind, value, flags, 0, color);
    return 1;
}

u32 func_002BE3A0(u8 *table, u32 first, u32 arg) {
    u32 i = 0;
    u32 index;

    do {
        func_002BDE50((first + i) * 0xA0 + *(u32 *)(table + 0x18) + 0x28, arg);
        i++;
        index = first + i;
    } while (index < *(u32 *)(table + 8) && (*(u32 *)(index * 0x80 + *(u32 *)(table + 0x10) + 0x18) & 0x20));
    return 1;
}

void func_002BE448(u32 mode, u32 arg) {
    switch (mode) {
    case 0:
        func_002C0A48(0x44, arg);
        return;
    case 1:
        func_002C0A48(0x48, arg);
        return;
    case 2:
        func_002C0A48(0x42, arg);
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BE4B8);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BE728);

INCLUDE_ASM(const s32, "game/code_0029A840", func_002BE8A8);

void func_002BED28(u32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4, u32 arg5, u32 arg6, u32 arg7) {
    func_002BE448(arg6, arg7);
    func_002C0F88(arg0, arg1, arg2, arg3, arg4, arg5, arg7);
    func_002C0A48(0x44, arg7);
}

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3BD0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3BE0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3BF0);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3C00);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3C10);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3C20);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3C30);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3C40);

INCLUDE_RODATA(const s32, "game/code_0029A840", D_003B3C50);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC944);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC948);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC94C);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC950);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC954);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC958);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC960);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC968);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC96C);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC970);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC974);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC978);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC97C);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC980);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC984);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC988);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC98C);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC990);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC994);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC998);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC99C);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC9A0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC9A8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC9AC);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC9B0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC9B8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC9C0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC9C8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC9D0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC9D8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC9E0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC9E8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC9F0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BC9F8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCA00);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCA08);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCA10);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCA18);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCA20);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCA28);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCA30);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCA38);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCA40);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCA48);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCA50);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCA58);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCA60);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCA68);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCA70);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCA78);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCA80);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCA88);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCA90);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCA98);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCAA0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCAA8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCAB0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCAB8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCAC0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCAC8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCAD0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCAD8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCAE0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCAE8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCAF0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCAF8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCB00);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCB08);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCB10);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCB18);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCB20);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCB28);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCB30);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCB38);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCB40);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCB48);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCB50);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCB58);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCB60);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCB68);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCB70);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCB78);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCB80);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCB88);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCB90);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCB98);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCBA0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCBA8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCBB0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCBB8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCBC0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCBC8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCBD0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCBD8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCBE0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCBE8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCBF0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCBF8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCC00);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCC08);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCC10);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCC18);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCC20);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCC28);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCC30);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCC38);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCC40);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCC48);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCC50);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCC58);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCC60);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCC68);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCC70);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCC78);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCC80);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCC88);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCC90);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCC98);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCCA0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCCA8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCCB0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCCB8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCCC0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCCC8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCCD0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCCD8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCCE0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCCE8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCCF0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCCF8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCD00);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCD08);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCD10);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCD18);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCD20);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCD28);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCD30);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCD38);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCD40);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCD48);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCD50);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCD58);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCD60);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCD68);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCD70);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCD78);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCD80);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCD88);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCD90);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCD98);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCDA0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCDA8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCDB0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCDB8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCDC0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCDC8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCDD0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCDD8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCDE0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCDE8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCDF0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCDF8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCE00);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCE08);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCE10);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCE18);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCE20);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCE28);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCE30);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCE38);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCE40);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCE48);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCE50);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCE58);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCE60);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCE68);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCE70);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCE78);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCE80);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCE88);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCE90);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCE98);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCEA0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCEA8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCEB0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCEB8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCEC0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCEC8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCED0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCED8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCEE0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCEE8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCEF0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCEF8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCF00);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCF08);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCF10);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCF18);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCF20);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCF28);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCF30);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCF38);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCF40);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCF48);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCF50);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCF58);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCF60);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCF68);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCF70);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCF78);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCF80);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCF88);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCF90);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCF98);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCFA0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCFA8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCFB0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCFB8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCFC0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCFC8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCFD0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCFD8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCFE0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCFE8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCFF0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BCFF8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD000);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD008);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD010);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD018);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD020);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD028);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD030);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD038);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD040);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD048);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD050);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD058);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD05C);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD060);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD064);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD068);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD06C);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD070);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD074);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD078);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD080);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD088);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD090);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD098);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD09C);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD0A0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD0A8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD0B0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD0B8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD0C0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD0C8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD0D0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD0D8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD0E0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD0E8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD0F0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD0F8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD100);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD108);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD109);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD10A);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD10B);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD10C);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD110);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD114);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD118);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD11C);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD120);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD124);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD128);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD12C);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD130);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD138);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD140);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD148);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD150);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD158);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD15C);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD160);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD164);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD168);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD16C);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD170);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD178);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD180);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD188);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD190);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD198);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD1A0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD1A8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD1B0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD1B8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD1C0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD1C8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD1D0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD1D8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD1E0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD1E8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD1F0);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD1F8);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD200);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD208);

INCLUDE_SDATA(const s32, "game/code_0029A840", D_003BD210);

