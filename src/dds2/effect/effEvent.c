#include "common.h"
#include "eff.h"
#include "pcp_vu0.h"


extern void *memcpy(void *, const void *, u32);

/* Each slot has a 0x60-byte stride; only its resettable tail is known. */
typedef struct Slot60 {
    u8 pad[0x54];
    s32 unk54;
    s32 unk58;
    f32 unk5C;
} Slot60;

typedef struct SlotTab {
    Slot60 *slots;
    u32 count;
    s32 handle;
} SlotTab;

typedef struct Work30 {
    u8 data[0x30];
} Work30;

extern Work30 D_003B21B0;

/* Per-channel work copies this 0x2C-byte prefix before its separate ID word.
 * The prefix's final word is copied with lw/sw rather than as part of the blob. */
typedef struct BDCommon2C {
    u8 data[0x28];
    u32 unk28;
} BDCommon2C;

typedef struct BDWork2C {
    BDCommon2C common;
    u32 id;
} BDWork2C;

extern BDWork2C *D_00438F10;

extern u32 effGetResourceFirstWord(s32 arg);

extern BDWork2C *D_00438F0C;

extern BDWork2C *D_00438F18;

extern Work30 D_003B2238;

/* Parameter blocks have distinct 0x18, 0x24, 0x2C and 0x30 byte layouts. */
typedef struct Work18 {
    u8 data[0x18];
} Work18;

extern Work18 D_003B2260;

typedef struct BDCommon24 {
    u8 data[0x20];
    u32 unk20;
} BDCommon24;

typedef struct BDWork24 {
    BDCommon24 common;
    u32 id;
} BDWork24;

extern BDWork24 *D_00438F14;

extern u8 D_003B2208[];

extern u8 D_003B21D8[];

extern u8 D_003B2278[];

extern u8 D_003B22A0[];

extern BDWork2C *effCloneBlurTemplate(void *arg);

extern BDWork2C *func_0018EBC8(void *arg);

extern BDWork24 *effCloneResourceTemplate(void *arg);

extern BDWork2C *effCloneBlurWorkWithSlots(void *arg);

extern Work30 D_003B22D0;

typedef struct Work2C {
    u8 data[0x28];
    u32 unk28;
} Work2C;

extern Work2C D_003B2428;

extern Work2C D_003B25A0;

extern Work30 D_003B2778;

extern Work18 D_003B28B8;

typedef struct Work24 {
    u8 data[0x20];
    u32 unk20;
} Work24;

extern Work24 D_003B29B8;

extern Work2C D_003B2AF8;

extern s8 D_00436465;

extern s8 D_00436464;

extern s8 D_00436463;

extern s8 D_00436466;

extern s8 D_00436462;

extern s8 D_00436461;

extern s8 D_00436460;

extern void effDrawBlurRectangle(Work30 *arg);

extern void effDrawBlurPixelRectWithResource(BDWork2C *arg);

extern void func_0018ECD0(BDWork2C *arg);

extern void func_0018F1D0(BDWork2C *arg);

extern void func_0018F5C0(Work30 *arg);

extern void func_0018F840(Work18 *arg);

extern void func_0018FCA0(BDWork24 *arg);

extern s32 func_003292A8(s32);
extern u8 *sdfResourceRetainAddress(s32);

/* Allocate contiguous slots followed by their count and allocation handle. */
SlotTab *effCreateSlotArray(u32 count) {
    s32 slotBytes = count * 0x60;
    s32 handle = func_003292A8(slotBytes + 0xC);
    Slot60 *slot = (Slot60 *)sdfResourceRetainAddress(handle);
    SlotTab *table = (SlotTab *)((u8 *)slot + slotBytes);
    u32 index = 0;
    table->handle = handle;
    table->slots = slot;
    table->count = count;
    if (count != 0) {
        do {
            index++;
            slot->unk54 = 0;
            slot->unk58 = 0;
            slot->unk5C = 0.05f;
            slot++;
        } while (index < count);
    }
    return table;
}

void effReleaseSlotArrayAllocation(EffArrHdr *header) {
    func_003297C8(header->unk8);
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00195EF8);

INCLUDE_ASM(const s32, "effect/effEvent", func_00196040);

INCLUDE_ASM(const s32, "effect/effEvent", func_00196180);

void effInitSlotTail(SlotTab *table, s32 index) {
    Slot60 *slot = &table->slots[index];

    slot->unk5C = 0.05f;
    slot->unk54 = slot->unk58 = 0;
}

s32 effGetSlotAt(SlotTab *table, s32 index) {
    return (s32)&table->slots[index];
}

INCLUDE_ASM(const s32, "effect/effEvent", func_001962B0);

INCLUDE_ASM(const s32, "effect/effEvent", func_00196378);

INCLUDE_ASM(const s32, "effect/effEvent", func_00196448);

INCLUDE_ASM(const s32, "effect/effEvent", func_00196570);

typedef struct GsSurface {
    u8 pad00[0x10];
    void (*submit)(struct GsSurface *, void *);
} GsSurface;

extern GsSurface D_00380748;
extern void *sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(void *);
extern void *sdfCreateFormattedSifCommand(s32, s32, s32, s32, s32);
extern void sdfAppendPacket(void *, void *);

void effSubmitPositionedDrawPacket(s32 x, s32 y, s32 arg2, s32 arg3) {
    void *list = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    sdfAppendPacket(list, sdfCreateFormattedSifCommand(x * 0x10 + 0x7000, y * 8 + 0x7900, 0xFF0000, arg2, arg3));
    D_00380748.submit(&D_00380748, list);
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00196740);

INCLUDE_ASM(const s32, "effect/effEvent", func_001969B8);

INCLUDE_ASM(const s32, "effect/effEvent", func_00196B08);

extern void *func_0011F250(s32, s32, s32, s32, s32, s32, s32);

void effSubmitSizedDrawPacket(s32 x, s32 y, s32 w, s32 h, s32 arg4, s32 arg5) {
    void *list = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    sdfAppendPacket(list, func_0011F250(x * 0x10 + 0x7000, y * 8 + 0x7900, 0xFF0000, w * 0x10, h * 8, arg4, arg5));
    D_00380748.submit(&D_00380748, list);
}

void func_00196FD8(void) {
    D_00436460 = 1;
}

void func_00196FE8(void) {
    D_00436460 = 0;
}

void func_00196FF0(void *src) {
    memcpy(&D_003B21B0, src, 0x28);
}

Work30 *effGetCh70Params(void) {
    return &D_003B21B0;
}

void func_00197060(void) {
    D_00436461 = 1;
}

void func_00197070(void) {
    D_00436461 = 0;
}

void effCopyCh71Common(BDCommon2C *src) {
    D_00438F10->common = *src;
}

u32 effGetCh71Work(void) {
    return D_00438F10;
}

void effSetCh71Id(u32 id) {
    D_00438F10->id = id;
}

void effInitCh71Id(void) {
    D_00438F10->id = effGetResourceFirstWord(2);
}

void func_00197118(void) {
    D_00436462 = 1;
}

void func_00197128(void) {
    D_00436462 = 0;
}

void effCopyCh72Common(BDCommon2C *src) {
    D_00438F0C->common = *src;
}

u32 effGetCh72Work(void) {
    return D_00438F0C;
}

void effSetCh72Id(u32 id) {
    D_00438F0C->id = id;
}

void effInitCh72Id(void) {
    D_00438F0C->id = effGetResourceFirstWord(2);
}

void func_001971D0(void) {
    D_00436466 = 1;
}

void func_001971E0(void) {
    D_00436466 = 0;
}

void effCopyCh76Common(BDCommon2C *src) {
    D_00438F18->common = *src;
}

u32 effGetCh76Work(void) {
    return D_00438F18;
}

void effSetCh76Id(u32 id) {
    D_00438F18->id = id;
}

void effInitCh76Id(void) {
    D_00438F18->id = effGetResourceFirstWord(3);
}

void func_00197288(void) {
    D_00436463 = 1;
}

void func_00197298(void) {
    D_00436463 = 0;
}

void func_001972A0(void *src) {
    memcpy(&D_003B2238, src, 0x28);
}

Work30 *effGetCh73Params(void) {
    return &D_003B2238;
}

void func_00197310(void) {
    D_00436464 = 1;
}

void func_00197320(void) {
    D_00436464 = 0;
}

void func_00197328(Work18 *src) {
    D_003B2260 = *src;
}

Work18 *effGetCh74Params(void) {
    return &D_003B2260;
}

void func_00197378(void) {
    D_00436465 = 1;
}

void func_00197388(void) {
    D_00436465 = 0;
}

void effCopyCh75Common(BDCommon24 *src) {
    D_00438F14->common = *src;
}

u32 effGetCh75Work(void) {
    return D_00438F14;
}

void effSetCh75Id(u32 id) {
    D_00438F14->id = id;
}

void effInitCh75Id(void) {
    D_00438F14->id = effGetResourceFirstWord(0);
}

void effInitWorks(void) {
    D_00438F10 = effCloneBlurTemplate(D_003B2208);
    D_00438F0C = func_0018EBC8(D_003B21D8);
    D_00438F14 = effCloneResourceTemplate(D_003B2278);
    D_00438F18 = effCloneBlurWorkWithSlots(D_003B22A0);
    *(s32 *)effGetCh76Work() = 4;
}

void effDispatchActive(void) {
    if (D_00436460) {
        effDrawBlurRectangle(&D_003B21B0);
    }
    if (D_00436461) {
        effDrawBlurPixelRectWithResource(D_00438F10);
    }
    if (D_00436462) {
        func_0018ECD0(D_00438F0C);
    }
    if (D_00436466) {
        func_0018F1D0(D_00438F18);
    }
    if (D_00436463) {
        func_0018F5C0(&D_003B2238);
    }
    if (D_00436464) {
        func_0018F840(&D_003B2260);
    }
    if (D_00436465) {
        func_0018FCA0(D_00438F14);
    }
}

typedef struct EffLoader {
    u8 pad00[0x1C];
    void *source;
    void *dest;
    u32 size;
    u8 pad28[4];
    s32 (*load)(void *);
    u8 pad30[8];
    s32 *result;
} EffLoader;

extern EffLoader D_003B23E8;
extern s8 D_004364AD;
extern s8 D_0040B7DB[];
extern void func_00194A08();
extern void func_00194A28();
extern void func_00194A30();
extern void func_00194A38();

s8 effUpdateCh72Params(void) {
    if (D_004364AD == 0) {
        if (D_003B23E8.load != 0) {
            *D_003B23E8.result = D_003B23E8.load(D_003B23E8.source);
        }
        if (D_003B23E8.dest != 0 && D_003B23E8.source != 0) {
            memcpy(D_003B23E8.dest, D_003B23E8.source, D_003B23E8.size);
        }
        D_004364AD = 1;
    }
    if ((u8)D_004364AD != 0) {
        func_00194A08(&D_003B23E8);
        func_00194A30();
        func_00194A28(&D_003B23E8);
        func_00194A38();
    }
    if (D_0040B7DB[0] < 0) {
        D_004364AD = 0;
    }
    return D_004364AD;
}

Work30 *effGetLoadDescA(void) {
    return &D_003B22D0;
}

void func_001975F8(void *src) {
    memcpy(&D_003B22D0, src, 0x28);
}

extern EffLoader D_003B2560;
extern s8 D_004364BD;

s8 func_00197658(void) {
    if (D_004364BD == 0) {
        if (D_003B2560.load != 0) {
            *D_003B2560.result = D_003B2560.load(&D_003B2428);
        }
        if (D_003B2560.dest != 0 && D_003B2560.source != 0) {
            memcpy(D_003B2560.dest, D_003B2560.source, D_003B2560.size);
        }
        D_004364BD = 1;
    }
    if ((u8)D_004364BD != 0) {
        func_00194A08(&D_003B2560);
        func_00194A30();
        func_00194A28(&D_003B2560);
        func_00194A38();
    }
    if (D_0040B7DB[0] < 0) {
        D_004364BD = 0;
    }
    return D_004364BD;
}

Work2C *effGetLoadDescB(void) {
    return &D_003B2428;
}

void func_00197720(Work2C *src) {
    D_003B2428 = *src;
}

extern EffLoader D_003B2738;
extern s8 D_004364EF;

s8 func_00197788(void) {
    if (D_004364EF == 0) {
        if (D_003B2738.load != 0) {
            *D_003B2738.result = D_003B2738.load(&D_003B25A0);
        }
        if (D_003B2738.dest != 0 && D_003B2738.source != 0) {
            memcpy(D_003B2738.dest, D_003B2738.source, D_003B2738.size);
        }
        D_004364EF = 1;
    }
    if ((u8)D_004364EF != 0) {
        func_00194A08(&D_003B2738);
        func_00194A30();
        func_00194A28(&D_003B2738);
        func_00194A38();
    }
    if (D_0040B7DB[0] < 0) {
        D_004364EF = 0;
    }
    return D_004364EF;
}

Work2C *effGetLoadDescC(void) {
    return &D_003B25A0;
}

void func_00197850(Work2C *src) {
    D_003B25A0 = *src;
}

extern EffLoader D_003B2878;
extern s8 D_00436504;

s8 func_001978B8(void) {
    if (D_00436504 == 0) {
        if (D_003B2878.load != 0) {
            *D_003B2878.result = D_003B2878.load(D_003B2878.source);
        }
        if (D_003B2878.dest != 0 && D_003B2878.source != 0) {
            memcpy(D_003B2878.dest, D_003B2878.source, D_003B2878.size);
        }
        D_00436504 = 1;
    }
    if ((u8)D_00436504 != 0) {
        func_00194A08(&D_003B2878);
        func_00194A30();
        func_00194A28(&D_003B2878);
        func_00194A38();
    }
    if (D_0040B7DB[0] < 0) {
        D_00436504 = 0;
    }
    return D_00436504;
}

Work30 *effGetLoadDescD(void) {
    return &D_003B2778;
}

void func_00197980(void *src) {
    memcpy(&D_003B2778, src, 0x28);
}

extern EffLoader D_003B2978;
extern s8 D_00436517;

s8 func_001979E0(void) {
    if (D_00436517 == 0) {
        if (D_003B2978.load != 0) {
            *D_003B2978.result = D_003B2978.load(D_003B2978.source);
        }
        if (D_003B2978.dest != 0 && D_003B2978.source != 0) {
            memcpy(D_003B2978.dest, D_003B2978.source, D_003B2978.size);
        }
        D_00436517 = 1;
    }
    if ((u8)D_00436517 != 0) {
        func_00194A08(&D_003B2978);
        func_00194A30();
        func_00194A28(&D_003B2978);
        func_00194A38();
    }
    if (D_0040B7DB[0] < 0) {
        D_00436517 = 0;
    }
    return D_00436517;
}

Work18 *effGetLoadDescE(void) {
    return &D_003B28B8;
}

void func_00197AA8(Work18 *src) {
    D_003B28B8 = *src;
}

extern EffLoader D_003B2AB8;
extern s8 D_0043651C;

s8 func_00197AE8(void) {
    if (D_0043651C == 0) {
        if (D_003B2AB8.load != 0) {
            *D_003B2AB8.result = D_003B2AB8.load(&D_003B29B8);
        }
        if (D_003B2AB8.dest != 0 && D_003B2AB8.source != 0) {
            memcpy(D_003B2AB8.dest, D_003B2AB8.source, D_003B2AB8.size);
        }
        D_0043651C = 1;
    }
    if ((u8)D_0043651C != 0) {
        func_00194A08(&D_003B2AB8);
        func_00194A30();
        func_00194A28(&D_003B2AB8);
        func_00194A38();
    }
    if (D_0040B7DB[0] < 0) {
        D_0043651C = 0;
    }
    return D_0043651C;
}

Work24 *effGetLoadDescF(void) {
    return &D_003B29B8;
}

void func_00197BB0(Work24 *src) {
    D_003B29B8 = *src;
}

extern EffLoader D_003B2C90;
extern s8 D_0043652F;

s8 effAdvancePendingChannelState(void) {
    if (D_0043652F == 0) {
        if (D_003B2C90.load != 0) {
            *D_003B2C90.result = D_003B2C90.load(&D_003B2AF8);
        }
        if (D_003B2C90.dest != 0 && D_003B2C90.source != 0) {
            memcpy(D_003B2C90.dest, D_003B2C90.source, D_003B2C90.size);
        }
        D_0043652F = 1;
    }
    if ((u8)D_0043652F != 0) {
        func_00194A08(&D_003B2C90);
        func_00194A30();
        func_00194A28(&D_003B2C90);
        func_00194A38();
    }
    if (D_0040B7DB[0] < 0) {
        D_0043652F = 0;
    }
    return D_0043652F;
}

Work2C *effGetLoadDescG(void) {
    return &D_003B2AF8;
}

void func_00197CD0(Work2C *src) {
    D_003B2AF8 = *src;
}

u32 func_00197D38() {
    return func_001682B0();
}

void func_00197D50() {
    sndReleaseAllVoices();
}
INCLUDE_ASM(const s32, "effect/effEvent", func_00197D68);



extern s32 D_00436530;

extern u32 D_00436534;

extern u32 D_00436538;

typedef struct {
    u8 bytes[0x30];
} __attribute__((packed)) FileRecordHeader;

/* Event work shared by resource setup, teardown and state updates. */
typedef struct EffEventWork {
    u8 pad00[4];
    u32 owner;              /* 0x04 */
    u8 initBlock[0x28];    /* 0x08: file-record header source */
    u32 state;              /* 0x30 */
    u32 effect;             /* 0x34 */
    u8 pad38[0x48];
    u8 flag;                /* 0x80 */
    u8 pad81[3];
    u32 resource;           /* 0x84 */
} EffEventWork;

extern struct EffEventWork *func_00197D68();
extern void *func_00328D68(s32 size);

extern u8 D_003B2D20[];

/* Release the attached effect before freeing the event work. */
void effEventReleaseNode(EffEventWork *work) {
    func_001686F0(work->effect);
    sdfReleaseChipBlock(work);
}

void effEventCopyFileRecordHeader(FileRecordHeader *destination, const FileRecordHeader *source) {
    *destination = *source;
}

void effEventCopyBillParticle(const FileRecordHeader *source, FileRecordHeader *destination) {
    *destination = *source;
}

void func_00197F40(EffEventWork *work) {
    func_00169168(work->effect);
}

void effEventSetState(EffEventWork *work, u32 state) {
    work->state = state;
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00197F60);
/* Init block of the event holder (0x30 bytes, copied to the event's owner record). */
typedef struct {
    f32 pos[3];           /* 0x00 */
    f32 unk0C;            /* 0x0C */
    u32 unk10;            /* 0x10 */
    u32 unk14;            /* 0x14 */
    u32 unk18;            /* 0x18 */
    f32 scale;            /* 0x1C */
    f32 rangeNear;        /* 0x20 */
    f32 rangeFar;         /* 0x24 */
    f32 param;            /* 0x28 */
    u32 color;            /* 0x2C */
} __attribute__((packed)) EffEventInit; /* 0x30 */


/* 0x3C-byte event holder: a handle, the event it owns, an init block copied to the event. */
typedef struct EffEventLight {
    u32 handle;           /* 0x00 */
    struct EffEventWork *owner; /* 0x04 */
    EffEventInit init;    /* 0x08 */
    u8 active;            /* 0x38 */
} EffEventLight; /* 0x3C */

EffEventLight *effEventLightCreate(u32 arg, f32 param) {
    EffEventLight *work = func_00328D68(sizeof(EffEventLight));

    work->init.scale = 1.0f;
    work->init.rangeNear = 50.0f;
    work->init.rangeFar = 180.0f;
    work->init.pos[1] = -90.0f;
    work->init.param = param;
    work->init.color = 0x80808080;
    work->init.unk10 = 0;
    work->init.unk14 = 0;
    work->init.unk18 = 0;
    work->init.pos[0] = 0;
    work->init.pos[2] = 0;
    work->init.unk0C = 0;
    work->handle = func_00197D38(arg);
    work->owner = func_00197D68(work->handle, 0, &work->init);
    work->active = 1;
    return work;
}

void func_00198340(EffEventLight *work) {
    effEventReleaseNode(work->owner);
    if (work->active != 0) {
        func_00197D50(work->handle);
    }
    sdfReleaseChipBlock(work);
}

EffEventLight *effEventLightClone(EffEventLight *src) {
    EffEventInit *block = &src->init;
    EffEventLight *work = func_00328D68(sizeof(EffEventLight));

    work->owner = func_00197D68(src->handle, 0, block);
    work->handle = src->handle;
    work->init = *block;
    work->active = 0;
    return work;
}

void func_00198448(EffEventWork *work) {
    func_00197F60(work->owner);
}

void effEventLightSetPosition(EffEventLight *work, f32 *vec) {
    work->init.pos[0] = vec[0];
    work->init.pos[1] = vec[1] - work->init.rangeFar * 0.5f;
    work->init.pos[2] = vec[2];
    work->init.unk0C = 0;
    effEventCopyFileRecordHeader((FileRecordHeader *)work->owner, (FileRecordHeader *)&work->init);
}

/* Attach the effect and copy the initial file-record header to its owner. */
void effEventBindEffect(EffEventWork *work, u32 effect) {
    work->effect = effect;
    effEventCopyFileRecordHeader(work->owner, work->initBlock);
}

typedef struct EffAimSource {
    f32 pos[4];           /* 0x00 */
    f32 rot[4];           /* 0x10 */
    f32 range;            /* 0x20 */
    f32 height;           /* 0x24 */
} EffAimSource;

typedef struct EffAimParams {
    u8 pad0;
    u8 mode;              /* 0x01 */
    u8 sub;               /* 0x02 */
    u8 pad3;
    s32 range;            /* 0x04 */
} EffAimParams;

extern f32 D_003B2CD0[];
extern f32 D_003B2CE8[];
extern u8 D_0037F690[];
extern u8 D_0037F6A0[];
extern void sdfVuBuildLookAtBasis(void *, void *, void *);
extern void sdfInvertRigidVuTransform(void);
extern void effMiscQuaternionToMatrixVU(void);
extern void func_00336818(f32 angle);
extern void sdfComposeVuMatrixFromRegisters(void);

/* vu0 routine: vf10 = the aimed offset point computed from the source and parameters */
void func_001984D8(EffAimSource *src, EffAimParams *param) {
    f32 out[4];
    f32 dir[4];
    f32 pos[4];
    f32 radius;
    f32 half;
    f32 y;
    s32 range = param->range;
    u8 sub = param->sub;
    u8 mode = param->mode;

    if (range == 0) {
        radius = src->range;
    } else {
        radius = (f32)range;
    }
    half = src->height * 0.5f;
    PCP_COPY_VECTOR(pos, src->pos);
    if (mode == 5) {
        if (sub == 8 || sub == 10) {
            y = -1.0f;
            if (range != 0) {
                y = -radius;
            }
        } else {
            y = -1.0f;
        }
    } else {
        y = pos[1] - D_003B2CD0[mode] * half;
        if (sub == 8 || sub == 10) {
            if (range != 0) {
                y -= radius;
            }
        }
    }
    if (sub == 9 || mode == 4) {
        dir[2] = half < radius ? -radius : -half;
        dir[0] = dir[1] = 0.0f;
        pos[1] = y;
        sdfVuBuildLookAtBasis(pos, D_0037F690, D_0037F6A0);
        sdfInvertRigidVuTransform();
        VU0_LOAD_VF(vf10, dir);
        VU0_CLEAR_W(vf10);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_LOAD_VF(vf11, pos);
        VU0_ADD(vf10, vf10, vf11);
        return;
    }
    if (sub == 8 || sub == 10) {
        out[0] = pos[0];
        out[1] = y;
        out[2] = pos[2];
    } else {
        dir[0] = 0.0f;
        dir[2] = 1.0f;
        dir[1] = 0.0f;
        VU0_LOAD_VF(vf10, src->rot);
        effMiscQuaternionToMatrixVU();
        func_00336818(D_003B2CE8[sub]);
        sdfComposeVuMatrixFromRegisters();
        VU0_LOAD_VF(vf10, dir);
        VU0_CLEAR_W(vf10);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, dir);
        out[0] = pos[0] + radius * dir[0];
        out[1] = y + radius * dir[1];
        out[2] = pos[2] + radius * dir[2];
    }
    VU0_LOAD_VF(vf10, out);
}

/* 0x7C-byte parameter block; the counters at 0x18/0x2C/0x30 are clamped to at least 1 on create/clone. */
typedef struct {
    u8 pad00[0x10];
    u32 count;            /* 0x10 particle count */
    u8 pad14[4];
    s32 unk18;            /* 0x18 modulus of the first delay */
    s32 unk1C;            /* 0x1C modulus of the second delay */
    f32 range;            /* 0x20 spawn radius */
    u8 pad24[8];
    s32 unk2C;
    s32 unk30;
    u8 pad34[4];
    f32 f38;              /* 0x38 */
    f32 blend3C;          /* 0x3C */
    f32 f40;              /* 0x40 */
    f32 f44;              /* 0x44 */
    f32 f48;              /* 0x48 */
    f32 f4C;              /* 0x4C */
    f32 f50;              /* 0x50 */
    f32 blend54;          /* 0x54 */
    f32 f58;              /* 0x58 */
    f32 blend5C;          /* 0x5C */
    u8 pad60[8];
    f32 f68;              /* 0x68 */
    u8 pad6C[4];
    f32 blend70;          /* 0x70 */
    u8 pad74[8];
} EffEventBlock7C;

typedef struct {
    f32 pos[3];           /* 0x00 */
    u8 pad0C[4];
    f32 dir[3];           /* 0x10 */
    u8 pad1C[4];
    s32 delayA;           /* 0x20 */
    s32 delayB;           /* 0x24 */
    f32 f28;              /* 0x28 */
    f32 f2C;              /* 0x2C */
    f32 f30;              /* 0x30 */
    f32 f34;              /* 0x34 */
    f32 f38;              /* 0x38 */
    f32 f3C;              /* 0x3C */
    f32 f40;              /* 0x40 */
    f32 f44;              /* 0x44 */
    f32 f48;              /* 0x48 */
    f32 f4C;              /* 0x4C */
    f32 f50;              /* 0x50 */
    f32 f54;              /* 0x54 */
    f32 f58;              /* 0x58 */
    u8 pad5C[4];
} EffEventBillParticle; /* 0x60 */

typedef struct {
    EffEventBlock7C head;
    EffEventBillParticle *particles; /* 0x7C */
    u8 flag;              /* 0x80 */
    u8 pad81[3];
    u32 handle;           /* 0x84 */
} EffEventBillSet; /* 0x88 */

extern void *billCreateFromResource(s32 kind, const char *path);

INCLUDE_RODATA(const s32, "effect/effEvent", D_00414A00);

EffEventBillSet *effEventBillSetCreate(EffEventBlock7C *src) {
    u32 count = src->count;
    u32 size = count * sizeof(EffEventBillParticle);
    u32 handle = func_003292A8(size + sizeof(EffEventBillSet));
    EffEventBillParticle *particle = (EffEventBillParticle *)sdfResourceRetainAddress(handle);
    EffEventBillSet *work = (EffEventBillSet *)((u8 *)particle + size);
    u32 i;

    work->head = *src;
    work->handle = handle;
    work->particles = particle;
    work->flag = 0;
    if (work->head.unk2C <= 0) {
        work->head.unk2C = 1;
    }
    if (work->head.unk30 <= 0) {
        work->head.unk30 = 1;
    }
    if (work->head.unk18 <= 0) {
        work->head.unk18 = 1;
    }
    D_00436530 = D_00436530 + 1;
    if (D_00436530 == 1) {
        D_00436534 = (u32)billCreateFromResource(0, "/efftool/bill/dbball01.tmx");
        D_00436538 = (u32)billCreateFromResource(0, "/efftool/bill/dbball02.tmx");
    }
    count = work->head.count;
    for (i = 0; i < count; i++) {
        particle->delayA = 0;
        particle++;
    }
    return work;
}

void effEventReleaseSharedResources(EffEventWork *work) {
    D_00436530 = D_00436530 - 1;
    if (D_00436530 == 0) {
        billDispatchByKind(D_00436534);
        billDispatchByKind(D_00436538);
    }
    func_003297C8(work->resource);
}

extern u32 effMiscRand(void *state);
extern f32 effMiscRandUnitFloat(void *state);
extern u8 D_003AA868[];

/* Randomize one billboard particle: delays, spin rates, radius and two unit direction vectors. */
void effEventRandomizeBillboardParticle(EffEventBillSet *work, s32 index) {
    EffEventBillParticle *p = &work->particles[index];
    f32 dir[4];
    f32 scale;
    f32 range;
    s32 periodA = work->head.unk18;
    s32 periodB = work->head.unk1C;

    p->delayA = -(effMiscRand(D_003AA868) % periodA);
    p->delayB = -(effMiscRand(D_003AA868) % periodB);
    scale = effMiscRandUnitFloat(D_003AA868) * work->head.blend3C + (1.0f - work->head.blend3C);
    p->f28 = work->head.f38 * scale;
    p->f2C = effMiscRandUnitFloat(D_003AA868) * (3.14159265f * 2.0f);
    p->f38 = effMiscRandUnitFloat(D_003AA868) * (3.14159265f * 2.0f);
    p->f30 = work->head.f48 * (effMiscRandUnitFloat(D_003AA868) * 0.5f + 0.5f);
    p->f3C = work->head.f48 * (effMiscRandUnitFloat(D_003AA868) * 0.5f + 0.5f);
    p->f34 = work->head.f40 * (effMiscRandUnitFloat(D_003AA868) * 0.3f + 0.7f) * scale;
    p->f40 = work->head.f44 * (effMiscRandUnitFloat(D_003AA868) * 0.3f + 0.7f) * scale;
    p->f44 = effMiscRandUnitFloat(D_003AA868) * (3.14159265f * 2.0f);
    if (effMiscRand(D_003AA868) & 1) {
        p->f48 = work->head.f4C * (effMiscRandUnitFloat(D_003AA868) * 0.3f + 0.7f);
    } else {
        p->f48 = -(work->head.f4C * (effMiscRandUnitFloat(D_003AA868) * 0.3f + 0.7f));
    }
    range = work->head.range;
    dir[0] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    dir[1] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    dir[2] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, dir);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, dir);
    p->pos[0] = range * effMiscRandUnitFloat(D_003AA868) * dir[0];
    p->pos[1] = range * effMiscRandUnitFloat(D_003AA868) * dir[1];
    p->pos[2] = range * effMiscRandUnitFloat(D_003AA868) * dir[2];
    p->dir[0] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    p->dir[1] = 0;
    p->dir[2] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, p->dir);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, p->dir);
    p->f4C = work->head.f68 * (effMiscRandUnitFloat(D_003AA868) * work->head.blend70 + (1.0f - work->head.blend70));
    p->f50 = 0;
    p->f54 = work->head.f50 * (effMiscRandUnitFloat(D_003AA868) * work->head.blend54 + (1.0f - work->head.blend54));
    p->f58 = work->head.f58 * (effMiscRandUnitFloat(D_003AA868) * work->head.blend5C + (1.0f - work->head.blend5C));
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00198D00);

INCLUDE_ASM(const s32, "effect/effEvent", func_00199118);

void effEventCopyParameterVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effEventSetWorkFlag(EffEventWork *work, u8 flag) {
    work->flag = flag;
}

void effEventCopyParameterBlock(const EffEventBlock7C *source, EffEventBlock7C *destination) {
    *destination = *source;
}

void effCopyEventBlockAndClampPositiveParameters(EffEventBlock7C *destination, const EffEventBlock7C *source) {
    *destination = *source;
    if (destination->unk2C <= 0) {
        destination->unk2C = 1;
    }
    if (destination->unk30 <= 0) {
        destination->unk30 = 1;
    }
    if (destination->unk18 <= 0) {
        destination->unk18 = 1;
    }
}

void effEventInstallBillParticleSet(void) {
    effEventBillSetCreate(D_003B2D20);
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00199E88);

extern void *effParamTableGetBlock(void *data, s32 index);
extern u32 effParamTableGetWord2(void *data, s32 index);
extern void func_00199E88(void *src, u16 kind, void *params);

void effEventParticleSetCreateFromTable(void *data) {
    void *block0 = effParamTableGetBlock(data, 0);
    void *block1 = effParamTableGetBlock(data, 1);

    func_00199E88(block0, effParamTableGetWord2(data, 1), block1);
}

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436458);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0043645C);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436460);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436461);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436462);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436463);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436464);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436465);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436466);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436468);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436470);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436478);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436480);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436488);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436490);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436498);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364A0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364A8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364B0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364B8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364C0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364C8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364D0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364D8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364E0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364E8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364F0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_004364F8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436500);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436504);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436508);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436510);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436518);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0043651C);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436520);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436528);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436530);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436534);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436538);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0043653C);

