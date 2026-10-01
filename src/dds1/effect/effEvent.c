#include "common.h"
#include "eff.h"
#include "pcp_vu0.h"



/* Parameter blocks have distinct 0x18, 0x24, 0x2C and 0x30 byte layouts. */
typedef struct Work18 {
    u8 data[0x18];
} Work18;

typedef struct Work24 {
    u8 data[0x20];
    u32 unk20;
} Work24;

typedef struct Work2C {
    u8 data[0x28];
    u32 unk28;
} Work2C;

typedef struct Work30 {
    u8 data[0x30];
} Work30;

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

typedef struct BDCommon24 {
    u8 data[0x20];
    u32 unk20;
} BDCommon24;

typedef struct BDWork24 {
    BDCommon24 common;
    u32 id;
} BDWork24;

extern Work30 D_00355880;
extern Work30 D_00355908;
extern Work18 D_00355930;
extern Work30 D_003559A0;
extern Work2C D_00355AF8;
extern Work2C D_00355C70;
extern Work30 D_00355E48;
extern Work18 D_00355F88;
extern Work24 D_00356088;
extern Work2C D_003561C8;
extern void *memcpy(void *, const void *, u32);

extern BDWork24 *D_003BD80C;

extern s8 D_003BB075;

extern s8 D_003BB074;

extern s8 D_003BB073;

extern BDWork2C *D_003BD810;

extern s8 D_003BB076;

extern BDWork2C *D_003BD804;

extern s8 D_003BB072;

extern BDWork2C *D_003BD808;

extern s8 D_003BB071;

extern s8 D_003BB070;
extern u32 effGetResourceFirstWord(s32 arg);
extern u8 D_003558D8[];
extern u8 D_003558A8[];
extern u8 D_00355948[];
extern u8 D_00355970[];
extern u8 D_00325748[];
extern void *sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(void *);
extern void sdfAppendPacket();
extern s32 sdfCreateFormattedSifCommand();

extern BDWork2C *effCloneBlurTemplate(void *arg);
extern BDWork2C *func_00186F90(void *arg);
extern BDWork24 *effCloneResourceTemplate(void *arg);
extern BDWork2C *effCloneBlurWorkWithSlots(void *arg);
extern void effDrawBlurRectangle(Work30 *arg);
extern void effDrawBlurPixelRectWithResource(BDWork2C *arg);
extern void func_00187098(BDWork2C *arg);
extern void func_00187598(BDWork2C *arg);
extern void func_00187988(Work30 *arg);
extern void func_00187C08(Work18 *arg);
extern void func_00188068(BDWork24 *arg);
extern s32 func_002D03F8(s32);
extern u8 *sdfResourceRetainAddress(s32);

/* Allocate contiguous slots followed by their count and allocation handle. */
SlotTab *effCreateSlotArray(u32 count) {
    s32 slotBytes = count * 0x60;
    s32 handle = func_002D03F8(slotBytes + 0xC);
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
    func_002D0918((u32)header->unk8);
}

INCLUDE_ASM(const s32, "effect/effEvent", func_0018E2C0);

INCLUDE_ASM(const s32, "effect/effEvent", func_0018E408);

INCLUDE_ASM(const s32, "effect/effEvent", func_0018E548);

void effInitSlotTail(SlotTab *table, s32 index) {
    Slot60 *slot = &table->slots[index];

    slot->unk5C = 0.05f;
    slot->unk54 = slot->unk58 = 0;
}

s32 effGetSlotAt(SlotTab *table, s32 index) {
    return (s32)&table->slots[index];
}

INCLUDE_ASM(const s32, "effect/effEvent", func_0018E678);

INCLUDE_ASM(const s32, "effect/effEvent", func_0018E740);

INCLUDE_ASM(const s32, "effect/effEvent", func_0018E810);

INCLUDE_ASM(const s32, "effect/effEvent", func_0018E938);

void effSubmitPositionedDrawPacket(s32 x, s32 y, s32 arg2, s32 arg3) {
    void *task = sdfAllocPacketAligned(0x20);
    u8 *scene;

    sdfInitPacketList(task);
    sdfAppendPacket(task, sdfCreateFormattedSifCommand((x << 4) + 0x7000, (y << 3) + 0x7900, 0xFF0000, arg2, arg3));
    scene = D_00325748;
    (*(void (**)(void *, void *))(scene + 0x10))(scene, task);
}

INCLUDE_ASM(const s32, "effect/effEvent", func_0018EB08);

INCLUDE_ASM(const s32, "effect/effEvent", func_0018ED80);

INCLUDE_ASM(const s32, "effect/effEvent", func_0018EED0);

extern void *func_0011D3E8(s32, s32, s32, s32, s32, s32, s32);

void effSubmitSizedDrawPacket(s32 x, s32 y, s32 w, s32 h, s32 arg4, s32 arg5) {
    void *list = sdfAllocPacketAligned(0x20);
    u8 *scene;

    sdfInitPacketList(list);
    sdfAppendPacket(list, func_0011D3E8(x * 0x10 + 0x7000, y * 8 + 0x7900, 0xFF0000, w * 0x10, h * 8, arg4, arg5));
    scene = D_00325748;
    (*(void (**)(void *, void *))(scene + 0x10))(scene, list);
}

void func_0018F3A0(void) {
    D_003BB070 = 1;
}

void func_0018F3B0(void) {
    D_003BB070 = 0;
}

void func_0018F3B8(void *src) {
    memcpy(&D_00355880, src, 0x28);
}

Work30 *effGetCh70Params(void) {
    return &D_00355880;
}

void func_0018F428(void) {
    D_003BB071 = 1;
}

void func_0018F438(void) {
    D_003BB071 = 0;
}

void effCopyCh71Common(BDCommon2C *src) {
    D_003BD808->common = *src;
}

BDWork2C *effGetCh71Work(void) {
    return D_003BD808;
}

void effSetCh71Id(u32 id) {
    D_003BD808->id = id;
}

void effInitCh71Id(void) {
    D_003BD808->id = effGetResourceFirstWord(2);
}

void func_0018F4E0(void) {
    D_003BB072 = 1;
}

void func_0018F4F0(void) {
    D_003BB072 = 0;
}

void effCopyCh72Common(BDCommon2C *src) {
    D_003BD804->common = *src;
}

BDWork2C *effGetCh72Work(void) {
    return D_003BD804;
}

void effSetCh72Id(u32 id) {
    D_003BD804->id = id;
}

void effInitCh72Id(void) {
    D_003BD804->id = effGetResourceFirstWord(2);
}

void func_0018F598(void) {
    D_003BB076 = 1;
}

void func_0018F5A8(void) {
    D_003BB076 = 0;
}

void effCopyCh76Common(BDCommon2C *src) {
    D_003BD810->common = *src;
}

BDWork2C *effGetCh76Work(void) {
    return D_003BD810;
}

void effSetCh76Id(u32 id) {
    D_003BD810->id = id;
}

void effInitCh76Id(void) {
    D_003BD810->id = effGetResourceFirstWord(3);
}

void func_0018F650(void) {
    D_003BB073 = 1;
}

void func_0018F660(void) {
    D_003BB073 = 0;
}

void func_0018F668(void *src) {
    memcpy(&D_00355908, src, 0x28);
}

Work30 *effGetCh73Params(void) {
    return &D_00355908;
}

void func_0018F6D8(void) {
    D_003BB074 = 1;
}

void func_0018F6E8(void) {
    D_003BB074 = 0;
}

void func_0018F6F0(Work18 *src) {
    D_00355930 = *src;
}

Work18 *effGetCh74Params(void) {
    return &D_00355930;
}

void func_0018F740(void) {
    D_003BB075 = 1;
}

void func_0018F750(void) {
    D_003BB075 = 0;
}

void effCopyCh75Common(BDCommon24 *src) {
    D_003BD80C->common = *src;
}

BDWork24 *effGetCh75Work(void) {
    return D_003BD80C;
}

void effSetCh75Id(u32 id) {
    D_003BD80C->id = id;
}

void effInitCh75Id(void) {
    D_003BD80C->id = effGetResourceFirstWord(0);
}

void effInitWorks(void) {
    D_003BD808 = effCloneBlurTemplate(D_003558D8);
    D_003BD804 = func_00186F90(D_003558A8);
    D_003BD80C = effCloneResourceTemplate(D_00355948);
    D_003BD810 = effCloneBlurWorkWithSlots(D_00355970);
    *(s32 *)effGetCh76Work() = 4;
}

void effDispatchActive(void) {
    if (D_003BB070) {
        effDrawBlurRectangle(&D_00355880);
    }
    if (D_003BB071) {
        effDrawBlurPixelRectWithResource(D_003BD808);
    }
    if (D_003BB072) {
        func_00187098(D_003BD804);
    }
    if (D_003BB076) {
        func_00187598(D_003BD810);
    }
    if (D_003BB073) {
        func_00187988(&D_00355908);
    }
    if (D_003BB074) {
        func_00187C08(&D_00355930);
    }
    if (D_003BB075) {
        func_00188068(D_003BD80C);
    }
}

typedef struct ChState {
    u8 pad00[0x1C];
    void *src;
    void *dst;
    u32 size;
    u8 pad28[4];
    s32 (*getter)(void *);
    u8 pad30[8];
    s32 *result;
} ChState;

extern ChState D_00355AB8;
extern s8 D_003BB0BD;
extern s8 D_0039862B[];
extern void func_0018CDD0(void *);
extern void func_0018CDF8(void);
extern void func_0018CDF0(void *);
extern void func_0018CE00(void);

s32 effUpdateCh72Params(void) {
    u8 ready = D_003BB0BD;

    if (D_003BB0BD == 0) {
        ChState *state = &D_00355AB8;

        if (state->getter != NULL) {
            *state->result = state->getter(state->src);
        }
        if (state->dst != NULL) {
            if (state->src != NULL) {
                memcpy(state->dst, state->src, state->size);
            }
        }
        ready = 1;
        D_003BB0BD = ready;
    }
    if (ready != 0) {
        func_0018CDD0(&D_00355AB8);
        func_0018CDF8();
        func_0018CDF0(&D_00355AB8);
        func_0018CE00();
    }
    if (D_0039862B[0] < 0) {
        D_003BB0BD = 0;
    }
    return D_003BB0BD;
}

Work30 *effGetLoadDescA(void) {
    return &D_003559A0;
}

void func_0018F9C0(void *src) {
    memcpy(&D_003559A0, src, 0x28);
}

extern ChState D_00355C30;
extern s8 D_003BB0CD;

s32 func_0018FA20(void) {
    u8 ready = D_003BB0CD;

    if (D_003BB0CD == 0) {
        ChState *state = &D_00355C30;

        if (state->getter != NULL) {
            *state->result = state->getter(&D_00355AF8);
        }
        if (state->dst != NULL) {
            if (state->src != NULL) {
                memcpy(state->dst, state->src, state->size);
            }
        }
        ready = 1;
        D_003BB0CD = ready;
    }
    if (ready != 0) {
        func_0018CDD0(&D_00355C30);
        func_0018CDF8();
        func_0018CDF0(&D_00355C30);
        func_0018CE00();
    }
    if (D_0039862B[0] < 0) {
        D_003BB0CD = 0;
    }
    return D_003BB0CD;
}

Work2C *effGetLoadDescB(void) {
    return &D_00355AF8;
}

void func_0018FAE8(Work2C *src) {
    D_00355AF8 = *src;
}

extern ChState D_00355E08;
extern s8 D_003BB0FF;

s32 func_0018FB50(void) {
    u8 ready = D_003BB0FF;

    if (D_003BB0FF == 0) {
        ChState *state = &D_00355E08;

        if (state->getter != NULL) {
            *state->result = state->getter(&D_00355C70);
        }
        if (state->dst != NULL) {
            if (state->src != NULL) {
                memcpy(state->dst, state->src, state->size);
            }
        }
        ready = 1;
        D_003BB0FF = ready;
    }
    if (ready != 0) {
        func_0018CDD0(&D_00355E08);
        func_0018CDF8();
        func_0018CDF0(&D_00355E08);
        func_0018CE00();
    }
    if (D_0039862B[0] < 0) {
        D_003BB0FF = 0;
    }
    return D_003BB0FF;
}

Work2C *effGetLoadDescC(void) {
    return &D_00355C70;
}

void func_0018FC18(Work2C *src) {
    D_00355C70 = *src;
}

extern ChState D_00355F48;
extern s8 D_003BB114;

s32 func_0018FC80(void) {
    u8 ready = D_003BB114;

    if (D_003BB114 == 0) {
        ChState *state = &D_00355F48;

        if (state->getter != NULL) {
            *state->result = state->getter(state->src);
        }
        if (state->dst != NULL) {
            if (state->src != NULL) {
                memcpy(state->dst, state->src, state->size);
            }
        }
        ready = 1;
        D_003BB114 = ready;
    }
    if (ready != 0) {
        func_0018CDD0(&D_00355F48);
        func_0018CDF8();
        func_0018CDF0(&D_00355F48);
        func_0018CE00();
    }
    if (D_0039862B[0] < 0) {
        D_003BB114 = 0;
    }
    return D_003BB114;
}

Work30 *effGetLoadDescD(void) {
    return &D_00355E48;
}

void func_0018FD48(void *src) {
    memcpy(&D_00355E48, src, 0x28);
}

extern ChState D_00356048;
extern s8 D_003BB127;

s32 func_0018FDA8(void) {
    u8 ready = D_003BB127;

    if (D_003BB127 == 0) {
        ChState *state = &D_00356048;

        if (state->getter != NULL) {
            *state->result = state->getter(state->src);
        }
        if (state->dst != NULL) {
            if (state->src != NULL) {
                memcpy(state->dst, state->src, state->size);
            }
        }
        ready = 1;
        D_003BB127 = ready;
    }
    if (ready != 0) {
        func_0018CDD0(&D_00356048);
        func_0018CDF8();
        func_0018CDF0(&D_00356048);
        func_0018CE00();
    }
    if (D_0039862B[0] < 0) {
        D_003BB127 = 0;
    }
    return D_003BB127;
}

Work18 *effGetLoadDescE(void) {
    return &D_00355F88;
}

void func_0018FE70(Work18 *src) {
    D_00355F88 = *src;
}

extern ChState D_00356188;
extern s8 D_003BB12C;

s32 func_0018FEB0(void) {
    u8 ready = D_003BB12C;

    if (D_003BB12C == 0) {
        ChState *state = &D_00356188;

        if (state->getter != NULL) {
            *state->result = state->getter(&D_00356088);
        }
        if (state->dst != NULL) {
            if (state->src != NULL) {
                memcpy(state->dst, state->src, state->size);
            }
        }
        ready = 1;
        D_003BB12C = ready;
    }
    if (ready != 0) {
        func_0018CDD0(&D_00356188);
        func_0018CDF8();
        func_0018CDF0(&D_00356188);
        func_0018CE00();
    }
    if (D_0039862B[0] < 0) {
        D_003BB12C = 0;
    }
    return D_003BB12C;
}

Work24 *effGetLoadDescF(void) {
    return &D_00356088;
}

void func_0018FF78(Work24 *src) {
    D_00356088 = *src;
}

extern ChState D_00356360;
extern s8 D_003BB13F;

s32 effAdvancePendingChannelState(void) {
    u8 ready = D_003BB13F;

    if (D_003BB13F == 0) {
        ChState *state = &D_00356360;

        if (state->getter != NULL) {
            *state->result = state->getter(&D_003561C8);
        }
        if (state->dst != NULL) {
            if (state->src != NULL) {
                memcpy(state->dst, state->src, state->size);
            }
        }
        ready = 1;
        D_003BB13F = ready;
    }
    if (ready != 0) {
        func_0018CDD0(&D_00356360);
        func_0018CDF8();
        func_0018CDF0(&D_00356360);
        func_0018CE00();
    }
    if (D_0039862B[0] < 0) {
        D_003BB13F = 0;
    }
    return D_003BB13F;
}

Work2C *effGetLoadDescG(void) {
    return &D_003561C8;
}

void func_00190098(Work2C *src) {
    D_003561C8 = *src;
}

u32 func_00190100() {
    return func_001606C0();
}

void func_00190118() {
    sndReleaseAllVoices();
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00190130);

extern u8 D_003563F0[];

extern void func_00190118();
extern void *func_002CFEB8(s32 size);

extern s32 D_003BB140;

extern u32 D_003BB144;

extern u32 D_003BB148;

/* Event work shared by resource setup, teardown and state updates. */
typedef struct {
    u8   pad_0x00[0x04]; /* 0x00 */
    void *owner;         /* 0x04: file-record header destination */
    u8   initBlock[0x28];/* 0x08: file-record header source */
    u32  state;          /* 0x30 */
    void *effect;        /* 0x34 */
    u8   pad_0x38[0x48]; /* 0x38 */
    u8   flag;           /* 0x80 */
    u8   pad_0x81[0x03]; /* 0x81 */
    void *resource;      /* 0x84: released on teardown */
} EffEventWork; /* 0x88 */

typedef struct {
    u8 bytes[0x30];
} __attribute__((packed)) FileRecordHeader;

/* Release the attached effect before freeing the event work. */
void effEventReleaseNode(EffEventWork *work) {
    func_00160B00(work->effect);
    sdfReleaseChipBlock(work);
}

void effEventCopyFileRecordHeader(FileRecordHeader *destination, const FileRecordHeader *source) {
    *destination = *source;
}

void effEventCopyBillParticle(const FileRecordHeader *source, FileRecordHeader *destination) {
    *destination = *source;
}

void func_00190308(EffEventWork *work) {
    func_00161588(work->effect);
}

void effEventSetState(EffEventWork *work, u32 value) {
    work->state = value;
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00190328);

struct EffEventWork;

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

extern struct EffEventWork *func_00190130();

EffEventLight *effEventLightCreate(u32 arg, f32 param) {
    EffEventLight *work = func_002CFEB8(sizeof(EffEventLight));

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
    work->handle = func_00190100(arg);
    work->owner = func_00190130(work->handle, 0, &work->init);
    work->active = 1;
    return work;
}

void func_00190708(EffEventLight *work) {
    effEventReleaseNode(work->owner);
    if (work->active != 0) {
        func_00190118(work->handle);
    }
    sdfReleaseChipBlock(work);
}

EffEventLight *effEventLightClone(EffEventLight *src) {
    EffEventInit *block = &src->init;
    EffEventLight *work = func_002CFEB8(sizeof(EffEventLight));

    work->owner = func_00190130(src->handle, 0, block);
    work->handle = src->handle;
    work->init = *block;
    work->active = 0;
    return work;
}

void func_00190810(EffEventWork *work) {
    func_00190328(work->owner);
}

void effEventLightSetPosition(EffEventLight *work, f32 *vec) {
    work->init.pos[0] = vec[0];
    work->init.pos[1] = vec[1] - work->init.rangeFar * 0.5f;
    work->init.pos[2] = vec[2];
    work->init.unk0C = 0;
    effEventCopyFileRecordHeader((FileRecordHeader *)work->owner, (FileRecordHeader *)&work->init);
}

/* Attach the effect and copy the initial file-record header to its owner. */
void effEventBindEffect(EffEventWork *work, void *value) {
    work->effect = value;
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

extern f32 D_003563A0[];
extern f32 D_003563B8[];
extern u8 D_00324690[];
extern u8 D_003246A0[];
extern void sdfVuBuildLookAtBasis(void *, void *, void *);
extern void sdfInvertRigidVuTransform(void);
extern void effMiscQuaternionToMatrixVU(void);
extern void func_002DD968(f32 angle);
extern void sdfComposeVuMatrixFromRegisters(void);

/* vu0 routine: vf10 = the aimed offset point computed from the source and parameters */
void func_001908A0(EffAimSource *src, EffAimParams *param) {
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
        y = pos[1] - D_003563A0[mode] * half;
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
        sdfVuBuildLookAtBasis(pos, D_00324690, D_003246A0);
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
        func_002DD968(D_003563B8[sub]);
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

INCLUDE_RODATA(const s32, "effect/effEvent", D_003A12E0);

EffEventBillSet *effEventBillSetCreate(EffEventBlock7C *src) {
    u32 count = src->count;
    u32 size = count * sizeof(EffEventBillParticle);
    u32 handle = func_002D03F8(size + sizeof(EffEventBillSet));
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
    D_003BB140 = D_003BB140 + 1;
    if (D_003BB140 == 1) {
        D_003BB144 = (u32)billCreateFromResource(0, "/efftool/bill/dbball01.tmx");
        D_003BB148 = (u32)billCreateFromResource(0, "/efftool/bill/dbball02.tmx");
    }
    count = work->head.count;
    for (i = 0; i < count; i++) {
        particle->delayA = 0;
        particle++;
    }
    return work;
}

void effEventReleaseSharedResources(EffEventWork *work) {
    D_003BB140 = D_003BB140 - 1;
    if (D_003BB140 == 0) {
        billDispatchByKind(D_003BB144);
        billDispatchByKind(D_003BB148);
    }
    func_002D0918(work->resource);
}

extern u32 effMiscRand(void *state);
extern f32 effMiscRandUnitFloat(void *state);
extern u8 D_0034DF38[];

/* Randomize one billboard particle: delays, spin rates, radius and two unit direction vectors. */
void effEventRandomizeBillboardParticle(EffEventBillSet *work, s32 index) {
    EffEventBillParticle *p = &work->particles[index];
    f32 dir[4];
    f32 scale;
    f32 range;
    s32 periodA = work->head.unk18;
    s32 periodB = work->head.unk1C;

    p->delayA = -(effMiscRand(D_0034DF38) % periodA);
    p->delayB = -(effMiscRand(D_0034DF38) % periodB);
    scale = effMiscRandUnitFloat(D_0034DF38) * work->head.blend3C + (1.0f - work->head.blend3C);
    p->f28 = work->head.f38 * scale;
    p->f2C = effMiscRandUnitFloat(D_0034DF38) * (3.14159265f * 2.0f);
    p->f38 = effMiscRandUnitFloat(D_0034DF38) * (3.14159265f * 2.0f);
    p->f30 = work->head.f48 * (effMiscRandUnitFloat(D_0034DF38) * 0.5f + 0.5f);
    p->f3C = work->head.f48 * (effMiscRandUnitFloat(D_0034DF38) * 0.5f + 0.5f);
    p->f34 = work->head.f40 * (effMiscRandUnitFloat(D_0034DF38) * 0.3f + 0.7f) * scale;
    p->f40 = work->head.f44 * (effMiscRandUnitFloat(D_0034DF38) * 0.3f + 0.7f) * scale;
    p->f44 = effMiscRandUnitFloat(D_0034DF38) * (3.14159265f * 2.0f);
    if (effMiscRand(D_0034DF38) & 1) {
        p->f48 = work->head.f4C * (effMiscRandUnitFloat(D_0034DF38) * 0.3f + 0.7f);
    } else {
        p->f48 = -(work->head.f4C * (effMiscRandUnitFloat(D_0034DF38) * 0.3f + 0.7f));
    }
    range = work->head.range;
    dir[0] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    dir[1] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    dir[2] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, dir);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, dir);
    p->pos[0] = range * effMiscRandUnitFloat(D_0034DF38) * dir[0];
    p->pos[1] = range * effMiscRandUnitFloat(D_0034DF38) * dir[1];
    p->pos[2] = range * effMiscRandUnitFloat(D_0034DF38) * dir[2];
    p->dir[0] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    p->dir[1] = 0;
    p->dir[2] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, p->dir);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, p->dir);
    p->f4C = work->head.f68 * (effMiscRandUnitFloat(D_0034DF38) * work->head.blend70 + (1.0f - work->head.blend70));
    p->f50 = 0;
    p->f54 = work->head.f50 * (effMiscRandUnitFloat(D_0034DF38) * work->head.blend54 + (1.0f - work->head.blend54));
    p->f58 = work->head.f58 * (effMiscRandUnitFloat(D_0034DF38) * work->head.blend5C + (1.0f - work->head.blend5C));
}

INCLUDE_ASM(const s32, "effect/effEvent", func_001910C8);

INCLUDE_ASM(const s32, "effect/effEvent", func_001914E0);

void effEventCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effEventSetWorkFlag(EffEventWork *work, u8 value) {
    work->flag = value;
}

void func_00192030(const EffEventBlock7C *source, EffEventBlock7C *destination) {
    *destination = *source;
}

void func_00192110(EffEventBlock7C *destination, const EffEventBlock7C *source) {
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
    effEventBillSetCreate(D_003563F0);
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00192250);

extern void *effParamTableGetBlock(void *data, s32 index);
extern u32 effParamTableGetWord2(void *data, s32 index);
extern void func_00192250(void *src, u16 kind, void *params);

void effEventParticleSetCreateFromTable(void *data) {
    void *block0 = effParamTableGetBlock(data, 0);
    void *block1 = effParamTableGetBlock(data, 1);

    func_00192250(block0, effParamTableGetWord2(data, 1), block1);
}

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB068);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB06C);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB070);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB071);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB072);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB073);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB074);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB075);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB076);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB078);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB080);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB088);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB090);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB098);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0A0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0A8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0B0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0B8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0C0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0C8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0D0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0D8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0E0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0E8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0F0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0F8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB100);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB108);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB110);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB114);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB118);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB120);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB128);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB12C);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB130);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB138);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB140);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB144);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB148);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB14C);

