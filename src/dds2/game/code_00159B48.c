#include "common.h"
#include "pcp_vu0.h"
extern void func_00333288(void *, u32);
extern void func_00333270(void *, u32);
extern void func_003332A0(void *, u32);
#include "ee_mmi.h"

extern void *sdfCreateAssetWithDrawEntries();
extern void func_003332D0(void *, f32);
extern void func_001594C8();
extern s32 D_00451F20[];
extern void func_00341348();
#include "eff.h"

typedef struct EffResourceOwner {
    u8 pad00[0x30];
    u16 kind;          /* 0x30 */
    u8 pad32[6];
    s32 unk38;
    u8 pad3C[4];
    s32 unk40;
    s32 unk44;
    u8 pad48[0xAC];
    s32 billboard;     /* 0xF4 */
    s32 buffer;        /* 0xF8 */
} EffResourceOwner;

extern void effParReleaseNodeResource(s32);
extern void parReleaseCellSystem(s32);
extern void func_001900B8(s32);

extern void effDestroyResources(EffResourceOwner *owner);

/* Particle-style effect object and its per-record buffer entry. */
typedef struct EffParticle {
    f32 x;              /* 0x00 */
    f32 y;              /* 0x04 */
    f32 z;              /* 0x08 */
    u8 pad0C[4];
    f32 speed;          /* 0x10 */
    u8 pad14[0x10];
    u32 unk24;          /* 0x24 */
    u8 pad28[0x24];
    u32 color;          /* 0x4C */
    u8 pad50[8];
    u32 alpha;          /* 0x58 */
    u8 pad5C[0x30];
    f32 lastSpeed;      /* 0x8C */
    u8 pad90[4];
    f32 speedJitter;    /* 0x94 */
    f32 angleJitter;    /* 0x98 */
    u8 pad9C[0x54];
    u32 unkF0;          /* 0xF0 */
    u8 padF4[4];
    EffectBufferTail *buffer; /* 0xF8 */
} EffParticle;

typedef struct EffParticleRecord {
    f32 x;              /* 0x00 */
    f32 y;              /* 0x04 */
    f32 z;              /* 0x08 */
    u8 pad0C[0x14];
    s32 unk20;
    u32 unk24;
    f32 speed;
    f32 angle;
    u8 pad30[0x10];
} EffParticleRecord;

extern u32 effParModulateColors(u32, u32);

typedef struct EffTemplatePacketList {
    u8 pad00[0x10];
    f32 x; /* 0x10 */
    f32 y; /* 0x14 */
    f32 z; /* 0x18 */
    u8 pad1C[4];
    u32 packetCount; /* 0x20 */
    u32 packetTag; /* 0x24: cleared when cloning a template */
    u8 pad28[8];
    u16 kind; /* 0x30: resource type */
    u8 pad32[2];
    u16 subrecordCount; /* 0x34: subrecords per packet */
    u8 pad36[0x6A];
    s32 templateSize; /* 0xA0: prefix copied before appending tail bytes */
    u8 padA4[0x54];
    EffectBufferTail *buffer; /* 0xF8 */
    u8 padFC[0x58];
    s32 decayStep; /* 0x154 */
    f32 recordScale; /* 0x158 */
    union {
        f32 tailValues[4]; /* 0x15C-0x168: variant-specific scaled values */
        u32 tailWords[4];
    };
    s32 recordList; /* 0x16C: start of the three-word packet records */
    s32 recordsPerPacket; /* 0x170 */
    s32 listAllocation; /* 0x174 */
    void *auxiliaryData; /* 0x178: optional 16 bytes per packet */
    s32 auxiliaryAllocation; /* 0x17C */
} EffTemplatePacketList;

typedef struct EffInstance {
    u8 localMatrix[0x40]; /* 0x00 */
    u8 transform[0x40]; /* 0x40 */
    BillObj *billboard; /* 0x80 */
    void *renderState; /* 0x84 */
} EffInstance;

typedef struct EffScaledRecord {
    u8 pad00[0x20];
    u32 flags; /* 0x20 */
    u8 pad24[0x10];
    f32 scale; /* 0x34 */
    u8 pad38[8];
} EffScaledRecord; /* 0x40 */

typedef struct EffBillFrame {
    u8 pad00[0x0C];
    u32 period; /* 0x0C: animation frame modulus */
    u32 flags; /* 0x10 */
} EffBillFrame;

typedef struct EffBillEntry {
    u8 pad00[4];
    u32 frame; /* 0x04 */
    s32 mode; /* 0x08 */
    EffBillFrame *data; /* 0x0C */
    u8 pad10[4];
} EffBillEntry; /* 0x14 */

extern s32 D_00451EE0[];

s32 sdfAllocPacketAligned(s32 size);

void sdfInitPacketList(s32 packet);

void func_0015AA30(s32 packet, s32 source);

void func_0015AD18(s32 packet, s32 source);

void func_0015B330(s32 effect);

void effSetTemplateTagPeriod(EffTemplatePacketList *effect);

extern void *memset(void *s, s32 c, u32 n);

extern void *memcpy(void *dest, const void *src, u32 n);

extern void *func_00328D68(s32 size);

extern EffectConfig D_003AA884[];

s32 billCreateIndexed(s32 kind, s32 index);

s32 func_003292A8(s32 size);

EffectBufferRecord *sdfResourceRetainAddress(s32 allocation);

void effInitExpandRingPacketSchedule(EffTemplatePacketList *effect);

/* Resource-table entry holds a reference-counted resource at +0x30. */
typedef struct EffResourceRef {
    u8 pad00[0x30];
    s32 *resource;
} EffResourceRef;

void effRetainResource(s32 index) {
    s32 *effect = (s32 *)billCreateIndexed(D_003AA884[index].unk00, 0);
    s32 *resource = ((EffResourceRef *)D_00451EE0[index])->resource;
    s32 references = resource[2];

    effect[12] = (s32)resource;
    resource[2] = references + 1;
}

u32 func_00159BB0(void) {
    return 0xf;
}

s32 effGetResourceFirstWord(s32 index) {
    return *(s32 *)((EffResourceRef *)D_00451EE0[index])->resource;
}

void effCopyVector(dst, src)
void *dst;
void *src;
{
    PCP_COPY_VECTOR(dst, src);
}

void billSetLengthExtent(BillObj *effect, float scale) {
    effect->unk20 = scale;
}

void billSetChildScaleComponents(BillObj *effect, float x, float y) {
    effect->unk10 = x;
    effect->unk14 = y;
}

void billSetChildParameter(BillObj *effect, u32 value) {
    effect->unk24 = value;
}

void effCopyPosition(BillObj *effect, const void *position) {
    if (effect->unk2C == 0) {
        memcpy((void *)((s32)effect->unk30 + 0xc), position, 16);
    }
}

void billSetBillboardMode(BillObj *effect, s32 mode) {
    s32 count;
    s32 remaining;
    s32 entry;
    mode = (s16)mode;
    switch (effect->unk2C) {
    case 0:
    case 3:
        effect->unk2E = mode;
        break;
    case 1:
        count = effect->entryCount;
        if (count > 0) {
            remaining = count;
            /* Required to match: induction points to each entry's frame slot at +0x0C. */
            entry = (s32)effect->unk60 + 0xc;
            do {
                EffBillFrame *node = (EffBillFrame *)*(s32 *)entry;
                u32 flags = node->flags & ~6;
                node->flags = flags;
                if (mode == 2) {
                    node->flags = flags | 2;
                } else if (mode == 3) {
                    node->flags = flags | 4;
                }
                entry += 0x14;
            } while (--remaining != 0);
        }
        break;
    }
}

typedef struct EffSlot {
    u8 pad00[4];
    union {
        s16 value;   /* 0x04 */
        u16 variant; /* Same halfword read unsigned */
    };
} EffSlot;

typedef struct EffSlotList {
    u8 pad00[0x10];
    s32 count;        /* 0x10 */
    u8 pad14[4];
    EffSlot **slots;  /* 0x18 */
} EffSlotList;

void billSetAllChildVariants(BillObj *effect, s16 value) {
    switch (effect->unk2C) {
    case 0:
        ((EffSlot *)effect->unk30)->value = value;
        break;
    case 1: {
        EffSlotList *list = effect->unk30;
        s32 count = list->count;
        EffSlot **slots = list->slots;
        EffSlot **slot;

        if (count > 0) {
            slot = slots;
            do {
                (*slot)->value = value;
                slot++;
            } while (--count != 0);
        }
        break;
    }
    }
}

s32 billGetChildValue(BillObj *effect) {
    if (effect->unk2C == 0) {
        return *(s32 *)effect->unk30;
    }
    return 0;
}

u16 billGetKind(BillObj *effect) {
    return effect->unk2C;
}

void billSetVariantValue(BillObj *effect, s32 value) {
    s32 v = value & 0xffff;

    switch (effect->unk2C) {
    case 0:
        ((EffSlot *)effect->unk30)->value = v;
        break;
    case 1:
        effect->unk3C = v;
        break;
    }
}

u16 billGetVariantValue(BillObj *effect) {
    switch (effect->unk2C) {
    case 0:
        return ((EffSlot *)effect->unk30)->variant;
    case 1:
        return effect->unk3C;
    default:
        return 0;
    }
}

/* Replace the selected list entry only when its index changes. */
void billSetKind1Entry(BillObj *effect, u32 value) {
    if (effect->unk2C == 1 && effect->unk58 != value) {
        func_001594C8(effect, value);
    }
}

/* Read the selected entry for list billboards; other kinds have none. */
s32 billGetKindOneEntry(BillObj *effect) {
    if (effect->unk2C == 1) {
        return effect->unk58;
    }
    return 0;
}

/* Kind-one payload's +4 link leads to another +4 value word. */
typedef struct BillLinkedValue {
    s32 unk00;
    s32 value;
} BillLinkedValue;

typedef struct BillValueLink {
    s32 unk00;
    BillLinkedValue *target;
} BillValueLink;

s32 billGetLinkedChildValue(s32 billboard) {
    if (((BillObj *)billboard)->unk2C == 1) {
        return ((BillValueLink *)((BillObj *)billboard)->unk30)->target->value;
    }
    return 0;
}

/* Start every entry's animation at the requested frame, with mode zero. */
void billSetEntryFrameMode0(BillObj *effect, u32 time) {
    if (effect->unk2C == 1) {
        s32 count = effect->entryCount;

        if (count > 0) {
            EffBillEntry *entry = (EffBillEntry *)effect->unk60;
            s32 remaining = count;

            do {
                EffBillFrame *frameData = entry->data;
                u32 frame = time % frameData->period;
                remaining -= 1;
                entry->mode = 0;
                entry->frame = frame;
                entry++;
            } while (remaining != 0);
        }
    }
}

/* Start every entry's animation at the requested frame, with mode one. */
void billSetEntryFrameMode1(BillObj *effect, u32 time) {
    if (effect->unk2C == 1) {
        s32 count = effect->entryCount;

        if (count > 0) {
            EffBillEntry *entry = (EffBillEntry *)effect->unk60;
            s32 remaining = count;

            do {
                EffBillFrame *frameData = entry->data;
                u32 frame = time % frameData->period;
                remaining -= 1;
                entry->mode = 1;
                entry->frame = frame;
                entry++;
            } while (remaining != 0);
        }
    }
}

/* Read the animation modulus of the first entry, if this is a list billboard. */
s32 billGetFirstEntryFramePeriod(BillObj *effect) {
    if (effect->unk2C == 1) {
        return ((EffBillEntry *)effect->unk60)->data->period;
    }
    return 0;
}

u16 billGetKindOneParameter(BillObj *effect) {
    if (effect->unk2C == 1) {
        return effect->unk50;
    }
    return 0;
}

/* Kind-one flag view and kind-zero dimensions, shared with the DDS1 twin. */
typedef struct BillKindOneView {
    u8 pad00[0x2C];
    u16 kind;
    u8 pad2E[0x26];
    u32 flags; /* 0x54 */
} BillKindOneView;

typedef struct BillChildPayload {
    s32 value;
    u8 pad04[0x20];
    f32 halfWidth;  /* 0x24 */
    f32 halfHeight; /* 0x28 */
} BillChildPayload;

s32 billGetKindOneFlags(s32 billboard) {
    if (((BillKindOneView *)billboard)->kind == 1) {
        return ((BillKindOneView *)billboard)->flags;
    }
    return 0;
}

void billMarkKindOneFlag(s32 billboard) {
    if (((BillKindOneView *)billboard)->kind == 1) {
        ((BillKindOneView *)billboard)->flags |= 0x1000000;
    }
}

void billSetChildHalfExtents(s32 billboard, float width, float height) {
    if (((BillKindOneView *)billboard)->kind == 0) {
        s32 tmp = (s32)((BillObj *)billboard)->unk30;
        ((BillChildPayload *)tmp)->halfWidth = width * 0.5f;
        ((BillChildPayload *)tmp)->halfHeight = height * 0.5f;
    }
}

extern u8 D_003846F0[];
extern u8 D_0037F610[];
extern u8 D_0037F650[];
extern u8 D_0037F660[];
extern u8 D_003AA9B0[];
extern void sdfPostmultiplyVuMatrixFromMemory(void *);
extern f32 sdfAtan2(f32, f32);

/* vu0 routine: computes the projected angle between two vectors */
f32 func_00159FF8(const void *position, const void *offset) {
    f32 delta[4];
    f32 projectedPosition[4];
    f32 projectedOffset[4];

    VU0_LOAD_MATRIX(D_003846F0);
    sdfPostmultiplyVuMatrixFromMemory(D_0037F610);
    VU0_LOAD_VF(vf10, position);
    VU0_MOVE_VF(vf12, vf10);
    VU0_TRANSFORM_POINT(vf10, vf10);
    VU0_PERSPECTIVE_DIVIDE_VF10();
    VU0_LOAD_VF(vf11, D_0037F650);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_0037F660);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, projectedPosition);

    VU0_LOAD_VF(vf10, offset);
    VU0_LOAD_VF(vf11, D_003AA9B0);
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf11, vf12);
    VU0_ADD(vf10, vf10, vf11);
    VU0_TRANSFORM_POINT(vf10, vf10);
    VU0_PERSPECTIVE_DIVIDE_VF10();
    VU0_LOAD_VF(vf11, D_0037F650);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_0037F660);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, projectedOffset);

    delta[0] = projectedPosition[0] - projectedOffset[0];
    delta[1] = projectedPosition[1] - projectedOffset[1];
    delta[2] = 0.0f;
    VU0_LOAD_VF(vf10, delta);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, delta);
    return sdfAtan2(delta[1], delta[0]);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015A150);

u8 *billCreateUnitObject(s32 index) {
    EffInstance *instance = func_00328D68(0x88);

    instance->billboard = (BillObj *)billCreateIndexed(1, index);
    instance->renderState = sdfCreateAssetWithDrawEntries();
    func_003332D0(instance->renderState, 1.0f);
    EE_MMI_UNIT_MATRIX(instance->transform);
    return (u8 *)instance;
}

u8 *billCloneUnitObject(EffInstance *source) {
    EffInstance *instance = func_00328D68(0x88);

    instance->billboard = (BillObj *)billCloneObjectRetainingSharedData((s32)source->billboard);
    instance->renderState = sdfCreateAssetWithDrawEntries();
    func_003332D0(instance->renderState, 1.0f);
    func_00333288(instance->renderState, 0x80808080);
    func_00333270(instance->renderState, 0x80808080);
    func_003332A0(instance->renderState, 0x80808080);
    EE_MMI_UNIT_MATRIX(instance->transform);
    EE_MMI_UNIT_MATRIX(instance->localMatrix);
    return (u8 *)instance;
}

void effDestroy(EffInstance *instance) {
    sdfQueueAssetRelease((u32)instance->renderState);
    billDispatchByKind(instance->billboard);
    sdfReleaseChipBlock(instance);
}

void effSetInstanceBillboardVector(EffInstance *instance) {
    effCopyVector((u32)instance->billboard);
}

void billSetChildScale2(EffInstance *instance, f32 scale) {
    billSetChildScaleComponents(instance->billboard, scale, scale);
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effCopyMatrixToNext(EffInstance *instance, void *src) {
    u8 *dst;

    VU0_LOAD_MATRIX(src);
    dst = instance->transform;
    VU0_STORE_MATRIX(dst);
}

void billSetChildValue(EffInstance *instance, u32 value) {
    billSetChildParameter(instance->billboard, value);
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effVuCopyMatrix(void *dst, void *src) {
    VU0_COPY_MATRIX(dst, src);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015A3F0);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015A4B0);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015AA30);

/* Effect owner installs a callback at +0x10 to accept a new packet list. */
typedef struct EffPacketSink {
    u8 pad00[0x10];
    void (*submit)(s32 owner, s32 packet);
} EffPacketSink;

void func_0015ACB0(s32 sink, s32 source) {
    s32 tmp = sdfAllocPacketAligned(0x20);

    sdfInitPacketList(tmp);
    func_0015AA30(tmp, source);
    ((EffPacketSink *)sink)->submit(sink, tmp);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015AD18);

void func_0015B208(s32 sink, s32 source) {
    s32 tmp = sdfAllocPacketAligned(0x20);

    sdfInitPacketList(tmp);
    func_0015AD18(tmp, source);
    ((EffPacketSink *)sink)->submit(sink, tmp);
}

void func_0015B270(void) {
    func_00341348(D_00451F20);
}

void func_0015B290(void) {
}

EffectBufferTail *effAllocateBuffer(s32 count) {
    s32 bytes = count * sizeof(EffectBufferRecord);
    s32 allocation = func_003292A8(bytes + sizeof(EffectBufferTail));
    EffectBufferRecord *record = sdfResourceRetainAddress(allocation);
    EffectBufferTail *tail = (EffectBufferTail *)((u8 *)record + bytes);

    tail->allocation = allocation;
    tail->records = record;
    if (count > 0) {
        s32 remaining = count;
        do {
            remaining--;
            record->unk20 = 0;
            record->unk24 = 0;
            record++;
        } while (remaining != 0);
    }
    return tail;
}

void effReleaseBufferAllocation(u32 *allocationSlot) {
    func_003297C8(*allocationSlot);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015B330);

void effDestroyResources(EffResourceOwner *owner) {
    switch (owner->kind) {
    case 1:
        effParReleaseNodeResource(owner->unk38);
        break;
    case 2:
        parReleaseCellSystem(owner->unk40);
        break;
    case 3:
        parReleaseCellSystem(owner->unk44);
        break;
    case 4:
        func_001900B8(owner->unk44);
        break;
    }
    billDispatchByKind(owner->billboard);
    effReleaseBufferAllocation(owner->buffer);
}

void effSetTemplateTagPeriod(EffTemplatePacketList *effect) {
    EffectBufferRecord *record;
    u32 i;
    u32 next;
    s32 tag;

    tag = 0xf0000001;
    i = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = tag;
            next = i + 1;
            record = record + 1;
            if (next % effect->tailWords[0] == 0) {
                tag = tag - effect->decayStep;
            }
            i = next;
        } while (i < effect->packetCount);
    }
}

void effScaleTemplateTail13(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[1] = effect->tailValues[1] * scale;
    effect->tailValues[3] = effect->tailValues[3] * scale;
}

s32 effCloneRingTemplate(EffTemplatePacketList *source) {
    s32 obj = (s32)func_00328D68(0x180);
    s32 tailLen = 0x30;

    memset((void *)obj, 0, 0x180);
    memcpy((void *)obj, source, source->templateSize);
    memcpy((void *)(obj + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_0015B330(obj);
    effSetTemplateTagPeriod(obj);
    return obj;
}

void effFreeRingTemplate(u32 effect) {
    effDestroyResources((EffResourceOwner *)effect);
    sdfReleaseChipBlock(effect);
}

/* Header shared by the effect emitters that spawn a ring or spray of packets:
 * an origin, a sub-effect, a fade descriptor, jitter ranges and the packet
 * buffer. The kind-specific parameters follow at +0x150. */
typedef struct EffEmitterSub {
    u16 kind;
    u8 pad02[0x16];
} EffEmitterSub;

typedef struct EffEmitterHead {
    f32 origin[4];         /* 0x00 */
    f32 speed;             /* 0x10: base packet speed */
    u8 pad14[0xC];
    s32 packetCount;       /* 0x20 */
    s32 frameCount;        /* 0x24 */
    u8 pad28[8];
    EffEmitterSub sub;     /* 0x30 */
    u8 fade[0x4C];         /* 0x48 */
    f32 speedJitter;       /* 0x94 */
    f32 spinJitter;        /* 0x98 */
    u8 pad9C[0x14];
    f32 matrix[16];        /* 0xB0 */
    u32 colorMask;         /* 0xF0 */
    u8 padF4[4];
    EffectBufferTail *buffer; /* 0xF8 */
    u8 padFC[0x46];
    u16 active;            /* 0x142 */
    u8 pad144[0xC];
} EffEmitterHead;

typedef struct EffPacket {
    f32 pos[4];   /* 0x00 */
    f32 vel[4];   /* 0x10 */
    s32 age;      /* 0x20 */
    u32 color;    /* 0x24 */
    f32 speed;    /* 0x28 */
    f32 spin;     /* 0x2C */
    f32 f30;
    f32 f34;
    f32 f38;
    f32 f3C;
} EffPacket;

extern f32 sdfSinPoly(f32);
extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);
extern f32 effMiscRandUnitFloat(void *);
extern u8 D_003AA868[];
extern u32 effMiscRand(void *state);
void parDispatchKindInit(void *work, s32 index);
void parDispatchKindUpdate(void *work, s32 index, u32 color, f32 speed);
extern void parUpdateSharedScaleAndDelta(void *sub);
extern u32 func_001616A8(void *fade, u32 color, s32 age);

typedef struct EffEmitterA {
    EffEmitterHead head;
    u8 flag150;
    u8 flag151;
    u8 pad152[6];
    f32 radius;      /* 0x158 */
    u32 period;      /* 0x15C */
    f32 f160;
    f32 f164;
    f32 f168;
    f32 f16C;
    f32 radiusRange; /* 0x170 */
    f32 speedRange;  /* 0x174 */
} EffEmitterA;

void effEmitterRingSpawn(EffEmitterA *effect, u32 index) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 rem;
    f32 phase;
    f32 jitter;

    packet += index;
    packet->color = 0;
    packet->age = 0;
    phase = 0.0f;
    rem = index % effect->period;
    if (rem != 0) {
        phase = (3.14159265f * 2.0f) / effect->period * rem;
    }
    packet->f30 = phase;
    jitter = effect->radiusRange;
    packet->f34 = effect->radius * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter));
    jitter = effect->speedRange;
    packet->vel[0] = effect->f168 * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter));
    packet->f38 = effect->f160;
    packet->pos[0] = sdfEvaluateCosineViaSinePhaseShift(phase) * effect->radius;
    packet->pos[1] = 0;
    packet->pos[2] = sdfSinPoly(phase) * effect->radius;
    VU0_LOAD_MATRIX(effect->head.matrix);
    VU0_LOAD_VF(vf10, packet->pos);
    VU0_APPLY_MATRIX(vf10, vf10);
    VU0_STORE_VF(vf10, packet->pos);
    packet->f3C = 0;
    packet->pos[0] += effect->head.origin[0];
    packet->pos[1] += effect->head.origin[1];
    packet->pos[2] += effect->head.origin[2];
    jitter = effect->head.speedJitter;
    packet->speed = effect->head.speed * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter));
    jitter = effect->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    parDispatchKindInit(&effect->head.sub, index);
}

/* vf12 keeps the packet's previous position and vf10 the new one for
 * parDispatchKindUpdate, which reads them as implicit arguments. */
void effEmitterRingUpdate(EffEmitterA *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 kind = effect->head.sub.kind;
    f32 origin[4];
    f32 offset[4];
    f32 decay;
    f32 spin;
    s32 cycles;
    s32 loop;
    s32 count;
    s32 frames;
    s32 i;

    parUpdateSharedScaleAndDelta(&effect->head.sub);
    VU0_LOAD_MATRIX(effect->head.matrix);
    spin = effect->f164 * (3.14159265f / 180.0f);
    decay = effect->f16C / 100.0f + 1.0f;
    cycles = 0;
    count = effect->head.packetCount;
    frames = effect->head.frameCount;
    loop = effect->flag151;
    PCP_COPY_VECTOR(origin, effect->head.origin);
    for (i = 0; i < count; i++, packet++) {
        s32 age = packet->age;

        if (age == 0xF0000001) {
            effEmitterRingSpawn(effect, i);
            age = packet->age;
        }
        if (age >= 0) {
            f32 theta;
            f32 radius;

            packet->color = func_001616A8(effect->head.fade, packet->color, age);
            packet->color = effParModulateColors(packet->color, effect->head.colorMask);
            if (kind != 0) {
                VU0_LOAD_VF(vf12, packet);
            }
            packet->f34 += (packet->vel[0] - effect->radius) / frames;
            packet->f30 += spin;
            if (packet->f34 < 0) {
                packet->f34 = 0;
            }
            packet->f3C += packet->f38;
            theta = packet->f30;
            radius = packet->f34;
            offset[0] = sdfEvaluateCosineViaSinePhaseShift(theta) * radius;
            offset[1] = packet->f3C;
            offset[2] = sdfSinPoly(theta) * radius;
            VU0_LOAD_VF(vf10, offset);
            VU0_APPLY_MATRIX(vf10, vf10);
            VU0_STORE_VF(vf10, offset);
            packet->pos[0] = origin[0] + offset[0];
            packet->pos[1] = origin[1] + offset[1];
            packet->pos[2] = origin[2] + offset[2];
            packet->f38 *= decay;
            if (kind != 0) {
                VU0_LOAD_VF(vf10, packet);
                parDispatchKindUpdate(&effect->head.sub, i, packet->color, packet->speed);
            }
        }
        age++;
        if (age >= frames) {
            if (loop) {
                age = 0xF0000001;
            } else {
                cycles++;
                parDispatchKindInit(&effect->head.sub, i);
                if (cycles >= count) {
                    effect->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

void effResetDiscPacketAges(EffTemplatePacketList *effect) {
    EffectBufferRecord *record;
    u32 index;

    index = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            index = index + 1;
            record = record + 1;
        } while (index < effect->packetCount);
    }
}

void effScaleDiscTemplate(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
}

s32 effCloneTemplate(EffTemplatePacketList *source) {
    s32 copy = (s32)func_00328D68(0x180);
    s32 tailLen = 0x30;

    memset((void *)copy, 0, 0x180);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_0015B330(copy);
    effResetDiscPacketAges(copy);
    return copy;
}

void effFreeDiscTemplate(u32 effect) {
    effDestroyResources((EffResourceOwner *)effect);
    sdfReleaseChipBlock(effect);
}

typedef struct EffEmitterB {
    EffEmitterHead head;
    u8 flag150;
    u8 pad151[3];
    u32 spread;      /* 0x154 */
    f32 radius;      /* 0x158 */
    f32 f15C;
    f32 f160;
    f32 f164;
    f32 f168;
    f32 f16C;
    f32 jitterA;     /* 0x170 */
    f32 jitterB;     /* 0x174 */
} EffEmitterB;

void effEmitterDiscSpawn(EffEmitterB *effect, u32 index) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    f32 tmp[4];
    f32 scale;
    f32 jitter;

    packet += index;
    scale = effect->radius * ((effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f);
    tmp[0] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    tmp[1] = 0;
    tmp[2] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, tmp);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, tmp);
    packet->pos[0] = tmp[0] * scale;
    packet->pos[1] = 0;
    packet->pos[2] = tmp[2] * scale;
    VU0_LOAD_MATRIX(effect->head.matrix);
    VU0_LOAD_VF(vf10, packet->pos);
    VU0_APPLY_MATRIX(vf10, vf10);
    VU0_STORE_VF(vf10, packet->pos);
    packet->pos[0] += effect->head.origin[0];
    packet->pos[1] += effect->head.origin[1];
    packet->pos[2] += effect->head.origin[2];
    tmp[0] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    tmp[1] = 0;
    tmp[2] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, tmp);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, packet->vel);
    packet->f30 = effect->f164 * (effMiscRandUnitFloat(D_003AA868) * effect->jitterB + (1.0f - effect->jitterB));
    packet->f34 = 0;
    packet->f38 = 0;
    packet->f3C = effect->f15C * (effMiscRandUnitFloat(D_003AA868) * effect->jitterA + (1.0f - effect->jitterA));
    packet->age = -(effMiscRand(D_00451F20) % (effect->spread + 1));
    packet->color = 0;
    jitter = effect->head.speedJitter;
    packet->speed = effect->head.speed * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter));
    jitter = effect->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    parDispatchKindInit(&effect->head.sub, index);
}

void effEmitterDiscUpdate(EffEmitterB *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 kind = effect->head.sub.kind;
    f32 tmp[4];
    f32 decay;
    f32 spin;
    f32 waveRate;
    s32 cycles;
    s32 frames;
    s32 count;
    s32 i;
    u32 loop;

    parUpdateSharedScaleAndDelta(&effect->head.sub);
    VU0_LOAD_MATRIX(effect->head.matrix);
    spin = effect->f160 * (3.14159265f / 180.0f);
    decay = effect->f16C / 100.0f + 1.0f;
    frames = effect->head.frameCount;
    count = effect->head.packetCount;
    loop = effect->flag150;
    waveRate = effect->f168;
    cycles = 0;
    for (i = 0; i < count; i++, packet++) {
        s32 age = packet->age;

        if (age == 0xF0000001) {
            effEmitterDiscSpawn(effect, i);
            age = packet->age;
        }
        if (age >= 0) {
            f32 lift;
            f32 old;
            f32 k;

            packet->color = func_001616A8(effect->head.fade, packet->color, age);
            packet->color = effParModulateColors(packet->color, effect->head.colorMask);
            if (kind != 0) {
                VU0_LOAD_VF(vf12, packet);
            }
            packet->f30 += waveRate;
            lift = packet->f3C;
            packet->f34 += spin;
            k = sdfSinPoly(packet->f34);
            old = packet->f38;
            packet->f38 = k;
            k = (k - old) * packet->f30;
            tmp[0] = packet->vel[0] * k;
            tmp[1] = lift;
            tmp[2] = packet->vel[2] * k;
            VU0_LOAD_VF(vf10, tmp);
            VU0_APPLY_MATRIX(vf10, vf10);
            VU0_STORE_VF(vf10, tmp);
            packet->pos[0] += tmp[0];
            packet->pos[1] += tmp[1];
            packet->pos[2] += tmp[2];
            packet->f3C = lift * decay;
            if (kind != 0) {
                VU0_LOAD_VF(vf10, packet);
                parDispatchKindUpdate(&effect->head.sub, i, packet->color, packet->speed);
            }
        }
        age++;
        if (age >= frames) {
            if (loop) {
                age = 0xF0000001;
            } else {
                parDispatchKindInit(&effect->head.sub, i);
                cycles++;
                if (cycles >= count) {
                    effect->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

void effResetBallisticPacketAges(EffTemplatePacketList *effect) {
    EffectBufferRecord *record;
    u32 index;

    index = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            index = index + 1;
            record = record + 1;
        } while (index < effect->packetCount);
    }
}

void effScaleBallisticTemplate(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
    effect->tailValues[3] = effect->tailValues[3] * scale;
}

s32 effCloneBallisticTemplate(EffTemplatePacketList *source) {
    s32 copy = (s32)func_00328D68(0x180);
    s32 tailLen = 0x30;

    memset((void *)copy, 0, 0x180);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_0015B330(copy);
    effResetBallisticPacketAges(copy);
    return copy;
}

void effFreeBallisticTemplate(u32 effect) {
    effDestroyResources((EffResourceOwner *)effect);
    sdfReleaseChipBlock(effect);
}

typedef struct EffEmitterC {
    EffEmitterHead head;
    u8 mode;         /* 0x150 */
    u8 loop;         /* 0x151 */
    u8 pad152[2];
    u32 spread;      /* 0x154 */
    u8 randomDir;    /* 0x158 */
    u8 pad159[3];
    f32 radius;      /* 0x15C */
    f32 cone;        /* 0x160 */
    f32 gravity;     /* 0x164 */
    f32 speed;       /* 0x168 */
    f32 decayPct;    /* 0x16C */
    f32 jitter;      /* 0x170 */
} EffEmitterC;

void func_0015C3C8(EffEmitterC *effect, s32 index);
INCLUDE_ASM(const s32, "game/code_00159B48", func_0015C3C8);

void effEmitterBallisticUpdate(EffEmitterC *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 kind = effect->head.sub.kind;
    u32 mode;
    f32 tmp[4];
    f32 decay;
    s32 cycles;
    s32 frames;
    s32 count;
    s32 loop;
    s32 i;

    parUpdateSharedScaleAndDelta(&effect->head.sub);
    mode = effect->mode;
    if (mode == 0) {
        VU0_LOAD_MATRIX(effect->head.matrix);
    }
    decay = effect->decayPct / 100.0f + 1.0f;
    cycles = 0;
    frames = effect->head.frameCount;
    count = effect->head.packetCount;
    loop = effect->loop;
    for (i = 0; i < count; i++, packet++) {
        s32 age = packet->age;

        if (age == 0xF0000001) {
            func_0015C3C8(effect, i);
            age = packet->age;
        }
        if (age >= 0) {
            packet->color = func_001616A8(effect->head.fade, packet->color, age);
            packet->color = effParModulateColors(packet->color, effect->head.colorMask);
            if (kind != 0) {
                VU0_LOAD_VF(vf12, packet);
            }
            packet->vel[1] = packet->vel[1] * decay + packet->f38;
            packet->vel[0] *= decay;
            packet->vel[2] *= decay;
            VU0_LOAD_VF(vf10, packet->vel);
            if (mode == 0) {
                VU0_APPLY_MATRIX(vf10, vf10);
            }
            VU0_STORE_VF(vf10, tmp);
            packet->pos[0] += tmp[0];
            packet->pos[1] += tmp[1];
            packet->pos[2] += tmp[2];
            if (kind != 0) {
                VU0_LOAD_VF(vf10, packet);
                parDispatchKindUpdate(&effect->head.sub, i, packet->color, packet->speed);
            }
        }
        age++;
        if (age >= frames) {
            if (loop) {
                age = 0xF0000001;
            } else {
                parDispatchKindInit(&effect->head.sub, i);
                cycles++;
                if (cycles >= count) {
                    effect->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

typedef struct EffEmitterD EffEmitterD;

extern void effEmitterLookAtRingSpawn(EffEmitterD *effect, u32 index);

void effInitLookAtRingPacketSchedule(EffTemplatePacketList *effect) {
    EffectBufferRecord *record;
    u32 i;
    u32 next;
    s32 tag;

    tag = 0;
    i = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            effEmitterLookAtRingSpawn((EffEmitterD *)effect, i);
            record->unk20 = tag;
            next = i + 1;
            record = record + 1;
            if (next % effect->tailWords[0] == 0) {
                tag = tag - effect->decayStep;
            }
            i = next;
        } while (i < effect->packetCount);
    }
}

void effScaleLookAtRingTemplate(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
}

s32 effCloneLookAtRingTemplate(EffTemplatePacketList *source) {
    s32 copy = (s32)func_00328D68(0x170);
    s32 tailLen = 0x20;

    memset((void *)copy, 0, 0x170);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_0015B330(copy);
    effInitLookAtRingPacketSchedule(copy);
    return copy;
}

void effFreeLookAtRingTemplate(u32 effect) {
    effDestroyResources((EffResourceOwner *)effect);
    sdfReleaseChipBlock(effect);
}

struct EffEmitterD {
    EffEmitterHead head;
    u8 mode;         /* 0x150 */
    u8 loop;         /* 0x151 */
    u8 pad152[10];
    u32 period;      /* 0x15C */
    f32 f160;
    f32 f164;
};

extern u8 D_0037F680[];
extern u8 D_0037F690[];
extern u8 D_0037F6A0[];
extern void sdfVuBuildLookAtBasis(void *, void *, void *);
extern void sdfInvertRigidVuTransform(void);

void effEmitterLookAtRingSpawn(EffEmitterD *effect, u32 index) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 period = 0;
    f32 angle;
    f32 scale;
    f32 jitter;

    packet += index;
    sdfVuBuildLookAtBasis(D_0037F680, D_0037F690, D_0037F6A0);
    sdfInvertRigidVuTransform();
    if (effect->mode == 0) {
        PCP_COPY_VECTOR(packet, effect->head.origin);
        period = effect->period;
        angle = (3.14159265f * 2.0f) / period * (index % period);
        packet->vel[0] = sdfEvaluateCosineViaSinePhaseShift(angle);
        packet->vel[1] = sdfSinPoly(angle);
        packet->vel[2] = 0;
        VU0_LOAD_VF(vf10, packet->vel);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF(vf10, packet->vel);
        packet->f34 = angle;
        packet->age = 0;
        packet->color = 0;
    } else {
        f32 frames = effect->head.frameCount;

        scale = effect->f164;
        angle = (3.14159265f * 2.0f) / period * (index % period);
        scale *= frames;
        angle += effect->f160 * (3.14159265f / 180.0f) * frames;
        packet->vel[0] = sdfEvaluateCosineViaSinePhaseShift(angle);
        packet->vel[1] = 0;
        packet->vel[2] = sdfSinPoly(angle);
        packet->f34 = angle;
        packet->pos[0] = packet->vel[0] * scale;
        packet->pos[1] = packet->vel[1] * scale;
        packet->pos[2] = packet->vel[2] * scale;
        packet->age = 0;
        packet->color = 0;
    }
    jitter = effect->head.speedJitter;
    packet->speed = effect->head.speed * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter));
    jitter = effect->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    parDispatchKindInit(&effect->head.sub, index);
}

void effEmitterLookAtRingUpdate(EffEmitterD *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 kind = effect->head.sub.kind;
    f32 spin;
    f32 scale;
    s32 cycles = 0;
    s32 loop;
    s32 count;
    s32 frames;
    s32 i;

    parUpdateSharedScaleAndDelta(&effect->head.sub);
    sdfVuBuildLookAtBasis(D_0037F680, D_0037F690, D_0037F6A0);
    sdfInvertRigidVuTransform();
    frames = effect->head.frameCount;
    spin = effect->f160 * (3.14159265f / 180.0f);
    count = effect->head.packetCount;
    loop = effect->loop;
    scale = effect->f164;
    for (i = 0; i < count; i++, packet++) {
        s32 age = packet->age;
        f32 angle;

        if (age >= 0) {
            packet->color = func_001616A8(effect->head.fade, packet->color, age);
            packet->color = effParModulateColors(packet->color, effect->head.colorMask);
            if (kind != 0) {
                VU0_LOAD_VF(vf12, packet);
            }
            packet->pos[0] += packet->vel[0] * scale;
            packet->pos[1] += packet->vel[1] * scale;
            packet->pos[2] += packet->vel[2] * scale;
            angle = packet->f34;
            packet->vel[0] = sdfEvaluateCosineViaSinePhaseShift(angle);
            packet->vel[1] = sdfSinPoly(angle);
            packet->vel[2] = 0;
            VU0_LOAD_VF(vf10, packet->vel);
            VU0_ROTATE_VEC(vf10, vf10);
            VU0_STORE_VF(vf10, packet->vel);
            packet->f34 += spin;
            if (kind != 0) {
                VU0_LOAD_VF(vf10, packet);
                parDispatchKindUpdate(&effect->head.sub, i, packet->color, packet->speed);
            }
        }
        age++;
        if (age >= frames) {
            if (loop) {
                effEmitterLookAtRingSpawn((EffEmitterD *)effect, i);
                age = packet->age;
            } else {
                parDispatchKindInit(&effect->head.sub, i);
                cycles++;
                if (cycles >= count) {
                    effect->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

void effResetBurstPacketAges(EffTemplatePacketList *effect) {
    EffectBufferRecord *record;
    u32 index;

    index = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            index = index + 1;
            record = record + 1;
        } while (index < effect->packetCount);
    }
}

void effScaleBurstTemplate(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[1] = effect->tailValues[1] * scale;
}

s32 effCloneBurstTemplate(EffTemplatePacketList *source) {
    s32 copy = (s32)func_00328D68(0x170);
    s32 tailLen = 0x20;

    memset((void *)copy, 0, 0x170);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_0015B330(copy);
    effResetBurstPacketAges(copy);
    return copy;
}

void effFreeBurstTemplate(u32 effect) {
    effDestroyResources((EffResourceOwner *)effect);
    sdfReleaseChipBlock(effect);
}

typedef struct EffEmitterE {
    EffEmitterHead head;
    u8 mode;         /* 0x150 */
    u8 loop;         /* 0x151 */
    u8 pad152[2];
    u32 spread;      /* 0x154 */
    f32 speed;       /* 0x158 */
    f32 spinRate;    /* 0x15C */
    f32 f160;
    f32 jitterA;     /* 0x164 */
    f32 jitterB;     /* 0x168 */
} EffEmitterE;

void effEmitterBurstSpawn(EffEmitterE *effect, u32 index) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    f32 tmp[4];
    f32 speed;
    f32 jitter;
    f32 length;

    packet += index;
    packet->color = 0;
    packet->age = -(effMiscRand(D_00451F20) % (effect->spread + 1));
    speed = effect->speed;
    jitter = effect->jitterA;
    tmp[0] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    tmp[1] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    tmp[2] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, tmp);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, tmp);
    packet->vel[0] = speed * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) * tmp[0];
    packet->vel[1] = speed * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) * tmp[1];
    packet->vel[2] = speed * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) * tmp[2];
    VU0_LOAD_VF(vf10, packet->vel);
    VU0_LENGTH_VF10(length);
    VU0_LOAD_MATRIX(effect->head.matrix);
    VU0_LOAD_VF(vf10, packet->vel);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF_UNCLOBBERED(vf10, packet->vel);
    packet->pos[0] = packet->vel[0] + effect->head.origin[0];
    packet->pos[1] = packet->vel[1] + effect->head.origin[1];
    packet->pos[2] = packet->vel[2] + effect->head.origin[2];
    if (effect->mode == 1) {
        tmp[0] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
        tmp[1] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
        tmp[2] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
        VU0_LOAD_VF(vf10, tmp);
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF(vf10, tmp);
        packet->f30 = tmp[0];
        packet->f34 = tmp[1];
        packet->f38 = tmp[2];
    } else {
        packet->f30 = 0;
        packet->f34 = -1.0f;
        packet->f38 = 0;
    }
    jitter = effect->jitterB;
    packet->f3C = (effect->f160 * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) - length) / effect->head.frameCount;
    jitter = effect->head.speedJitter;
    packet->speed = effect->head.speed * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter));
    jitter = effect->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    parDispatchKindInit(&effect->head.sub, index);
}

extern void sdfBuildVuRotationFromAxisAngle(f32 angle, f32 *axis);

void effEmitterBurstUpdate(EffEmitterE *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 kind = effect->head.sub.kind;
    f32 origin[4];
    f32 axis[4];
    f32 growth[4];
    f32 length;
    f32 spin;
    s32 cycles;
    s32 frames;
    s32 loop;
    s32 count;
    s32 i;

    parUpdateSharedScaleAndDelta(&effect->head.sub);
    VU0_LOAD_MATRIX_B(effect->head.matrix);
    count = effect->head.packetCount;
    frames = effect->head.frameCount;
    cycles = 0;
    spin = effect->spinRate * (3.14159265f / 180.0f);
    loop = effect->loop;
    PCP_COPY_VECTOR(origin, effect->head.origin);
    for (i = 0; i < count; i++, packet++) {
        s32 age = packet->age;

        if (age == 0xF0000001) {
            effEmitterBurstSpawn(effect, i);
            age = packet->age;
        }
        if (age >= 0) {
            packet->color = func_001616A8(effect->head.fade, packet->color, age);
            packet->color = effParModulateColors(packet->color, effect->head.colorMask);
            if (kind != 0) {
                VU0_LOAD_VF(vf12, packet);
            }
            VU0_LOAD_VF(vf10, packet->vel);
            VU0_LENGTH_VF10(length);
            length += packet->f3C;
            growth[0] = length;
            growth[1] = length;
            growth[2] = length;
            VU0_NORMALIZE_VF10();
            VU0_LOAD_VF(vf11, growth);
            VU0_MUL(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, packet->vel);
            VU0_MOVE_VF(vf28, vf24);
            VU0_MOVE_VF(vf29, vf25);
            VU0_MOVE_VF(vf30, vf26);
            VU0_MOVE_VF(vf31, vf27);
            axis[0] = packet->f30;
            axis[1] = packet->f34;
            axis[2] = packet->f38;
            axis[3] = 0;
            VU0_LOAD_VF(vf10, axis);
            VU0_ROTATE_VEC(vf10, vf10);
            VU0_STORE_VF_UNCLOBBERED(vf10, axis);
            sdfBuildVuRotationFromAxisAngle(spin, axis);
            VU0_LOAD_VF(vf10, packet->vel);
            VU0_ROTATE_VEC(vf10, vf10);
            VU0_LOAD_VF(vf11, origin);
            VU0_STORE_VF(vf10, packet->vel);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, packet);
            if (kind != 0) {
                VU0_LOAD_VF(vf10, packet);
                parDispatchKindUpdate(&effect->head.sub, i, packet->color, packet->speed);
            }
        }
        age++;
        if (age >= frames) {
            if (loop) {
                age = 0xF0000001;
            } else {
                parDispatchKindInit(&effect->head.sub, i);
                cycles++;
                if (cycles >= count) {
                    effect->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

void effResetSpherePacketAges(EffTemplatePacketList *effect) {
    EffectBufferRecord *record;
    u32 index;

    index = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            index = index + 1;
            record = record + 1;
        } while (index < effect->packetCount);
    }
}

void effScaleSphereTemplate(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
}

s32 effCloneSphereTemplate(EffTemplatePacketList *source) {
    s32 copy = (s32)func_00328D68(0x180);
    s32 tailLen = 0x30;

    memset((void *)copy, 0, 0x180);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_0015B330(copy);
    effResetSpherePacketAges(copy);
    return copy;
}

void effFreeSphereTemplate(u32 effect) {
    effDestroyResources((EffResourceOwner *)effect);
    sdfReleaseChipBlock(effect);
}

typedef struct EffEmitterF {
    EffEmitterHead head;
    u8 flag150;
    u8 pad151[3];
    u32 spread;      /* 0x154 */
    f32 radius;      /* 0x158 */
    f32 f15C;
    f32 f160;
    f32 f164;
    f32 f168;
    f32 f16C;
    f32 f170;
    f32 f174;
    f32 f178;
} EffEmitterF;

void effEmitterSphereSpawn(EffEmitterF *effect, u32 index) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    f32 tmp[4];
    f32 radius;
    f32 jitter;

    packet += index;
    radius = effect->radius;
    jitter = effect->f178;
    tmp[0] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    tmp[1] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    tmp[2] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, tmp);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, tmp);
    packet->pos[0] = radius * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) * tmp[0];
    packet->pos[1] = radius * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) * tmp[1];
    packet->pos[2] = radius * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) * tmp[2];
    VU0_LOAD_MATRIX(effect->head.matrix);
    VU0_LOAD_VF(vf10, packet->pos);
    VU0_APPLY_MATRIX(vf10, vf10);
    VU0_STORE_VF(vf10, packet->pos);
    packet->pos[0] += effect->head.origin[0];
    packet->pos[1] += effect->head.origin[1];
    packet->pos[2] += effect->head.origin[2];
    packet->vel[0] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    packet->vel[1] = 0;
    packet->vel[2] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, packet->vel);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, packet->vel);
    packet->f30 = effect->f164 * (effMiscRandUnitFloat(D_003AA868) * effect->f174 + (1.0f - effect->f174));
    packet->f34 = 0;
    packet->f38 = 0;
    packet->f3C = effect->f15C * (effMiscRandUnitFloat(D_003AA868) * effect->f170 + (1.0f - effect->f170));
    packet->age = ~(effMiscRand(D_00451F20) % (effect->spread + 1));
    packet->color = 0;
    jitter = effect->head.speedJitter;
    packet->speed = effect->head.speed * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter));
    jitter = effect->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    parDispatchKindInit(&effect->head.sub, index);
}

void effEmitterSphereUpdate(EffEmitterF *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 kind = effect->head.sub.kind;
    f32 tmp[4];
    f32 decay;
    f32 spin;
    f32 waveRate;
    s32 cycles;
    s32 frames;
    s32 count;
    s32 i;
    u32 loop;

    parUpdateSharedScaleAndDelta(&effect->head.sub);
    VU0_LOAD_MATRIX(effect->head.matrix);
    spin = effect->f160 * (3.14159265f / 180.0f);
    decay = effect->f16C / 100.0f + 1.0f;
    frames = effect->head.frameCount;
    count = effect->head.packetCount;
    loop = effect->flag150;
    waveRate = effect->f168;
    cycles = 0;
    for (i = 0; i < count; i++, packet++) {
        s32 age = packet->age;

        if (age == 0xF0000001) {
            effEmitterSphereSpawn(effect, i);
            age = packet->age;
        }
        if (age >= 0) {
            f32 lift;
            f32 old;
            f32 k;

            packet->color = func_001616A8(effect->head.fade, packet->color, age);
            packet->color = effParModulateColors(packet->color, effect->head.colorMask);
            if (kind != 0) {
                VU0_LOAD_VF(vf12, packet);
            }
            packet->f30 += waveRate;
            lift = packet->f3C;
            packet->f34 += spin;
            k = sdfSinPoly(packet->f34);
            old = packet->f38;
            packet->f38 = k;
            k = (k - old) * packet->f30;
            tmp[0] = packet->vel[0] * k;
            tmp[1] = lift;
            tmp[2] = packet->vel[2] * k;
            VU0_LOAD_VF(vf10, tmp);
            VU0_APPLY_MATRIX(vf10, vf10);
            VU0_STORE_VF(vf10, tmp);
            packet->pos[0] += tmp[0];
            packet->pos[1] += tmp[1];
            packet->pos[2] += tmp[2];
            packet->f3C = lift * decay;
            if (kind != 0) {
                VU0_LOAD_VF(vf10, packet);
                parDispatchKindUpdate(&effect->head.sub, i, packet->color, packet->speed);
            }
        }
        age++;
        if (age >= frames) {
            if (loop) {
                age = 0xF0000001;
            } else {
                parDispatchKindInit(&effect->head.sub, i);
                cycles++;
                if (cycles >= count) {
                    effect->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

void effInitExpandRingPacketSchedule(EffTemplatePacketList *effect) {
    EffectBufferRecord *record;
    u32 i;
    u32 next;
    s32 tag;

    tag = 0xf0000001;
    i = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = tag;
            next = i + 1;
            record = record + 1;
            if (next % effect->tailWords[1] == 0) {
                tag = tag - effect->decayStep;
            }
            i = next;
        } while (i < effect->packetCount);
    }
}

void effScaleExpandRingTemplate(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->recordScale = effect->recordScale * scale;
}

s32 billCloneTemplateSmall(EffTemplatePacketList *source) {
    s32 obj = (s32)func_00328D68(0x170);
    s32 tailLen = 0x20;

    memset((void *)obj, 0, 0x170);
    memcpy((void *)obj, source, source->templateSize);
    memcpy((void *)(obj + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_0015B330(obj);
    effInitExpandRingPacketSchedule(obj);
    return obj;
}

void effFreeExpandRingTemplate(u32 effect) {
    effDestroyResources((EffResourceOwner *)effect);
    sdfReleaseChipBlock(effect);
}

typedef struct EffEmitterG {
    EffEmitterHead head;
    u8 pad150;
    u8 loop;         /* 0x151 */
    u8 pad152[6];
    f32 radius;      /* 0x158 */
    f32 jitter;      /* 0x15C */
    u32 period;      /* 0x160 */
    f32 spinRate;    /* 0x164 */
} EffEmitterG;

void effEmitterExpandRingSpawn(EffEmitterG *effect, u32 index) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    f32 tmp[4];
    f32 jitter;
    f32 radius;
    f32 angle;

    packet += index;
    packet->age = -1;
    packet->color = 0;
    jitter = effect->jitter;
    radius = effect->radius * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter));
    packet->f30 = radius;
    angle = (3.14159265f * 2.0f) / effect->period * (index % effect->period);
    packet->vel[0] = sdfEvaluateCosineViaSinePhaseShift(angle);
    packet->vel[1] = 0;
    packet->vel[2] = sdfSinPoly(angle);
    VU0_LOAD_MATRIX(effect->head.matrix);
    tmp[1] = radius;
    tmp[0] = 0;
    tmp[2] = 0;
    VU0_LOAD_VF(vf10, tmp);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_LOAD_VF(vf11, effect->head.origin);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, packet);
    jitter = effect->head.speedJitter;
    packet->speed = effect->head.speed * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter));
    jitter = effect->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    parDispatchKindInit(&effect->head.sub, index);
}

void effEmitterExpandRingUpdate(EffEmitterG *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 kind = effect->head.sub.kind;
    f32 origin[4];
    f32 tmp[4];
    f32 spin;
    f32 cosv;
    f32 sinv;
    s32 cycles;
    s32 loop;
    s32 count;
    s32 frames;
    s32 i;

    parUpdateSharedScaleAndDelta(&effect->head.sub);
    VU0_LOAD_MATRIX(effect->head.matrix);
    spin = effect->spinRate * (3.14159265f / 180.0f);
    cycles = 0;
    count = effect->head.packetCount;
    frames = effect->head.frameCount;
    loop = effect->loop;
    cosv = sdfEvaluateCosineViaSinePhaseShift(spin);
    sinv = sdfSinPoly(spin);
    PCP_COPY_VECTOR(origin, effect->head.origin);
    for (i = 0; i < count; i++, packet++) {
        s32 age = packet->age;

        if (age == 0xF0000001) {
            effEmitterExpandRingSpawn(effect, i);
            age = packet->age;
        }
        if (age >= 0) {
            f32 angle;
            f32 radius;
            f32 c;
            f32 s;

            packet->color = func_001616A8(effect->head.fade, packet->color, age);
            packet->color = effParModulateColors(packet->color, effect->head.colorMask);
            if (kind != 0) {
                VU0_LOAD_VF(vf12, packet);
            }
            angle = (f32)age / (f32)frames * 3.14159265f;
            radius = packet->f30;
            c = sdfEvaluateCosineViaSinePhaseShift(angle);
            s = sdfSinPoly(angle);
            tmp[0] = packet->vel[0] * (radius * s);
            tmp[1] = radius * c;
            tmp[2] = packet->vel[2] * (radius * s);
            VU0_LOAD_VF(vf10, tmp);
            VU0_APPLY_MATRIX(vf10, vf10);
            VU0_LOAD_VF(vf11, origin);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, packet);
            tmp[0] = packet->vel[0];
            tmp[2] = packet->vel[2];
            packet->vel[0] = cosv * tmp[0] + sinv * tmp[2];
            packet->vel[2] = cosv * tmp[2] - sinv * tmp[0];
            if (kind != 0) {
                VU0_LOAD_VF(vf10, packet);
                parDispatchKindUpdate(&effect->head.sub, i, packet->color, packet->speed);
            }
        }
        age++;
        if (age >= frames) {
            if (loop) {
                age = 0xF0000001;
            } else {
                parDispatchKindInit(&effect->head.sub, i);
                cycles++;
                if (cycles >= count) {
                    effect->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

void effResetConePacketAges(EffTemplatePacketList *effect) {
    EffectBufferRecord *record;
    u32 index;

    index = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            index = index + 1;
            record = record + 1;
        } while (index < effect->packetCount);
    }
}

void effScaleConeTemplate(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
    effect->tailValues[3] = effect->tailValues[3] * scale;
}

s32 effCloneConeTemplate(EffTemplatePacketList *source) {
    s32 copy = (s32)func_00328D68(0x180);
    s32 tailLen = 0x30;

    memset((void *)copy, 0, 0x180);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_0015B330(copy);
    effResetConePacketAges(copy);
    return copy;
}

void effFreeConeTemplate(u32 effect) {
    effDestroyResources((EffResourceOwner *)effect);
    sdfReleaseChipBlock(effect);
}

typedef struct EffEmitterH {
    EffEmitterHead head;
    u8 mode;         /* 0x150 */
    u8 loop;         /* 0x151 */
    u8 pad152[2];
    u32 spread;      /* 0x154 */
    u8 pad158[4];
    f32 radius;      /* 0x15C */
    f32 cone;        /* 0x160 */
    f32 gravity;     /* 0x164 */
    f32 speed;       /* 0x168 */
    f32 decayPct;    /* 0x16C */
    u8 pad170[4];
    f32 degrees;     /* 0x174 */
} EffEmitterH;

void effEmitterConeSpawn(EffEmitterH *effect, s32 index) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    f32 gravity;
    f32 sweep;
    f32 angle;
    f32 radius;
    f32 cone;
    f32 speed;
    f32 cosv;
    f32 sinv;
    f32 jitter;

    packet += index;
    gravity = effect->gravity / 100.0f;
    sweep = effect->degrees * (3.14159265f / 180.0f);
    radius = effect->radius;
    cone = effect->cone;
    speed = effect->speed;
    if (effect->mode == 0) {
        angle = sweep / (u32)effect->head.packetCount * index;
        cosv = sdfEvaluateCosineViaSinePhaseShift(angle);
        sinv = sdfSinPoly(angle);
        packet->pos[0] = cosv * radius;
        packet->pos[1] = 0;
        packet->pos[2] = sinv * radius;
        VU0_LOAD_MATRIX(effect->head.matrix);
        VU0_LOAD_VF(vf10, packet->pos);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, packet->pos);
        packet->pos[0] += effect->head.origin[0];
        packet->pos[1] += effect->head.origin[1];
        packet->pos[2] += effect->head.origin[2];
        packet->vel[0] = cosv * cone * speed;
        packet->vel[1] = -speed * (1.0f - cone);
        packet->vel[2] = sinv * cone * speed;
        packet->f38 = gravity;
    }
    packet->age = -(effMiscRand(D_00451F20) % (effect->spread + 1));
    packet->color = 0;
    jitter = effect->head.speedJitter;
    packet->speed = effect->head.speed * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter));
    jitter = effect->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    parDispatchKindInit(&effect->head.sub, index);
}

void effEmitterConeUpdate(EffEmitterH *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 kind = effect->head.sub.kind;
    f32 tmp[4];
    f32 decay;
    s32 cycles;
    s32 frames;
    s32 count;
    s32 loop;
    s32 i;

    parUpdateSharedScaleAndDelta(&effect->head.sub);
    VU0_LOAD_MATRIX(effect->head.matrix);
    decay = effect->decayPct / 100.0f + 1.0f;
    cycles = 0;
    frames = effect->head.frameCount;
    count = effect->head.packetCount;
    loop = effect->loop;
    for (i = 0; i < count; i++, packet++) {
        s32 age = packet->age;

        if (age == 0xF0000001) {
            effEmitterConeSpawn(effect, i);
            age = packet->age;
        }
        if (age >= 0) {
            packet->color = func_001616A8(effect->head.fade, packet->color, age);
            packet->color = effParModulateColors(packet->color, effect->head.colorMask);
            if (kind != 0) {
                VU0_LOAD_VF(vf12, packet);
            }
            packet->vel[0] *= decay;
            packet->vel[1] = packet->vel[1] * decay + packet->f38;
            packet->vel[2] *= decay;
            VU0_LOAD_VF(vf10, packet->vel);
            VU0_APPLY_MATRIX(vf10, vf10);
            VU0_STORE_VF(vf10, tmp);
            packet->pos[0] += tmp[0];
            packet->pos[1] += tmp[1];
            packet->pos[2] += tmp[2];
            if (kind != 0) {
                VU0_LOAD_VF(vf10, packet);
                parDispatchKindUpdate(&effect->head.sub, i, packet->color, packet->speed);
            }
        }
        age++;
        if (age >= frames) {
            if (loop) {
                age = 0xF0000001;
            } else {
                parDispatchKindInit(&effect->head.sub, i);
                cycles++;
                if (cycles >= count) {
                    effect->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

void effApplyTemplateScaleToRecords(EffTemplatePacketList *effect) {
    s32 i = 0;
    EffScaledRecord *record = (EffScaledRecord *)effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->flags = 0xf0000001;
            record->scale = effect->recordScale;
            i++;
            record++;
        } while ((u32)i < effect->packetCount);
    }
}

void effScalePacketRecordTemplate(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[1] = effect->tailValues[1] * scale;
}

s32 effClonePacketRecordScaleTemplate(EffTemplatePacketList *source) {
    s32 copy = (s32)func_00328D68(0x170);
    s32 tailLen = 0x20;

    memset((void *)copy, 0, 0x170);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_0015B330(copy);
    effApplyTemplateScaleToRecords(copy);
    return copy;
}

void effFreePacketRecordScaleTemplate(u32 effect) {
    effDestroyResources((EffResourceOwner *)effect);
    sdfReleaseChipBlock(effect);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015EE70);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015F1C0);

void func_0015F560(void) {
    effInitParticleRecord();
}

void effScaleSingleParticleTemplate(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
}

void *effCloneSingleParticleTemplate(EffTemplatePacketList *source) {
    EffTemplatePacketList *copy = func_00328D68(0x150);
    s32 tailLen = 0;

    memset(copy, 0, 0x150);
    memcpy(copy, source, source->templateSize);
    memcpy((u8 *)copy + 0x150, (u8 *)source + source->templateSize, tailLen);
    copy->packetCount = 1;
    copy->packetTag = 0;
    func_0015B330((s32)copy);
    return copy;
}

void effFreeSingleParticleTemplate(u32 effect) {
    effDestroyResources((EffResourceOwner *)effect);
    sdfReleaseChipBlock(effect);
}

extern u8 D_003AA868[];
extern f32 effMiscRandUnitFloat(void *);

/* Initialize a particle record, applying random speed and angle jitter. */
void effInitParticleRecord(effect)
    EffParticle *effect;
{
    EffParticleRecord *particle = (EffParticleRecord *)effect->buffer->records;
    f32 jitter;
    u32 color;

    color = effect->color | (effect->alpha << 24);
    particle->x = effect->x;
    particle->unk20 = -1;
    particle->y = effect->y;
    particle->unk24 = color;
    particle->z = effect->z;
    particle->speed = 1.0f;
    particle->angle = 0;
    effect->lastSpeed = effect->speed;
    jitter = effect->speedJitter;
    particle->speed = effect->speed * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter));
    jitter = effect->angleJitter;
    if (jitter != 0) {
        particle->angle = (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        particle->angle = 0;
    }
}

void effUpdateParticleRecord(EffParticle *effect) {
    EffParticleRecord *record = (EffParticleRecord *)effect->buffer->records;
    u32 color;

    if (record->unk20 == 0) {
        effInitParticleRecord(effect);
    }
    record->unk20 = record->unk20 + 1;
    record->x = effect->x;
    record->y = effect->y;
    color = effect->color | (effect->alpha << 24);
    effect->unk24 = record->unk20 + 1;
    record->unk24 = color;
    record->z = effect->z;
    record->unk24 = effParModulateColors(color, effect->unkF0);
}

void effResetOffsetGravityPacketAges(EffTemplatePacketList *effect) {
    EffectBufferRecord *record;
    u32 index;

    index = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            index = index + 1;
            record = record + 1;
        } while (index < effect->packetCount);
    }
}

void effScaleOffsetGravityTemplate(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
    effect->tailValues[3] = effect->tailValues[3] * scale;
}

s32 effCloneOffsetGravityTemplate(EffTemplatePacketList *source) {
    s32 copy = (s32)func_00328D68(0x190);
    s32 tailLen = 0x40;

    memset((void *)copy, 0, 0x190);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_0015B330(copy);
    effResetOffsetGravityPacketAges(copy);
    return copy;
}

void effFreeOffsetGravityTemplate(u32 effect) {
    effDestroyResources((EffResourceOwner *)effect);
    sdfReleaseChipBlock(effect);
}

typedef struct EffEmitterJ {
    EffEmitterHead head;
    u8 mode;         /* 0x150 */
    u8 loop;         /* 0x151 */
    u8 pad152[0x1A];
    f32 decayPct;    /* 0x16C */
} EffEmitterJ;

void func_0015F918(EffEmitterJ *effect, s32 index);
INCLUDE_ASM(const s32, "game/code_00159B48", func_0015F918);

void effEmitterOffsetGravityUpdate(EffEmitterJ *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 kind = effect->head.sub.kind;
    u32 mode;
    f32 tmp[4];
    f32 origin[4];
    f32 decay;
    s32 cycles;
    s32 frames;
    s32 count;
    s32 loop;
    s32 i;

    parUpdateSharedScaleAndDelta(&effect->head.sub);
    mode = effect->mode;
    if (mode == 0) {
        VU0_LOAD_MATRIX(effect->head.matrix);
    }
    decay = effect->decayPct / 100.0f + 1.0f;
    cycles = 0;
    count = effect->head.packetCount;
    frames = effect->head.frameCount;
    loop = effect->loop;
    PCP_COPY_VECTOR(origin, effect->head.origin);
    for (i = 0; i < count; i++, packet++) {
        s32 age = packet->age;

        if (age == 0xF0000001) {
            func_0015F918(effect, i);
            age = packet->age;
        }
        if (age >= 0) {
            packet->color = func_001616A8(effect->head.fade, packet->color, age);
            packet->color = effParModulateColors(packet->color, effect->head.colorMask);
            if (kind != 0) {
                VU0_LOAD_VF(vf12, packet);
            }
            packet->vel[1] = packet->vel[1] * decay + packet->f3C;
            packet->vel[0] *= decay;
            packet->vel[2] *= decay;
            VU0_LOAD_VF(vf10, packet->vel);
            if (mode == 0) {
                VU0_APPLY_MATRIX(vf10, vf10);
            }
            VU0_STORE_VF(vf10, tmp);
            packet->f30 += tmp[0];
            packet->f34 += tmp[1];
            packet->f38 += tmp[2];
            VU0_LOAD_VF(vf10, &packet->f30);
            VU0_LOAD_VF(vf11, origin);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, packet);
            if (kind != 0) {
                VU0_LOAD_VF(vf10, packet);
                parDispatchKindUpdate(&effect->head.sub, i, packet->color, packet->speed);
            }
        }
        age++;
        if (age >= frames) {
            if (loop) {
                age = 0xF0000001;
            } else {
                parDispatchKindInit(&effect->head.sub, i);
                cycles++;
                if (cycles >= count) {
                    effect->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

void effResetDiscAuxPacketAges(EffTemplatePacketList *effect) {
    EffectBufferRecord *record;
    u32 index;

    index = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            index = index + 1;
            record = record + 1;
        } while (index < effect->packetCount);
    }
}

void effScaleDiscAuxTemplate(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
}

void *effCloneDiscAuxTemplate(EffTemplatePacketList *source) {
    EffTemplatePacketList *copy = func_00328D68(0x200);
    s32 allocation;
    s32 tailLen = 0xB0;

    memset(copy, 0, 0x200);
    memcpy(copy, source, source->templateSize);
    memcpy((u8 *)copy + 0x150, (u8 *)source + source->templateSize, tailLen);
    allocation = func_003292A8(copy->packetCount * 0x10);
    copy->auxiliaryAllocation = allocation;
    copy->auxiliaryData = sdfResourceRetainAddress(allocation);
    func_0015B330((s32)copy);
    effResetDiscAuxPacketAges(copy);
    return copy;
}

void effFreeDiscAuxTemplate(u32 effect) {
    func_003297C8(((EffTemplatePacketList *)effect)->auxiliaryAllocation);
    effDestroyResources(effect);
    sdfReleaseChipBlock(effect);
}

typedef struct EffEmitterK {
    EffEmitterHead head;
    u8 flag150;
    u8 flag151;
    u8 pad152[2];
    u32 spread;      /* 0x154 */
    f32 radius;      /* 0x158 */
    f32 f15C;
    f32 f160;
    f32 f164;
    f32 f168;
    f32 f16C;
    f32 f170;
    f32 f174;
    f32 (*aux)[4];   /* 0x178: 16 bytes per packet */
} EffEmitterK;

void effEmitterDiscAuxSpawn(EffEmitterK *effect, u32 index) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    f32 (*aux)[4] = effect->aux;
    f32 tmp[4];
    f32 radius;
    f32 jitter;

    packet += index;
    aux += index;
    radius = effect->radius * ((effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f);
    tmp[0] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    tmp[1] = 0;
    tmp[2] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, tmp);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, tmp);
    (*aux)[0] = tmp[0] * radius;
    (*aux)[1] = 0;
    (*aux)[2] = tmp[2] * radius;
    VU0_LOAD_MATRIX(effect->head.matrix);
    VU0_LOAD_VF(vf10, *aux);
    VU0_APPLY_MATRIX(vf10, vf10);
    VU0_STORE_VF(vf10, *aux);
    tmp[0] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    tmp[1] = 0;
    tmp[2] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, tmp);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, packet->vel);
    packet->f30 = effect->f164 * (effMiscRandUnitFloat(D_003AA868) * effect->f174 + (1.0f - effect->f174));
    packet->f34 = 0;
    packet->f38 = 0;
    packet->f3C = effect->f15C * (effMiscRandUnitFloat(D_003AA868) * effect->f170 + (1.0f - effect->f170));
    packet->age = -(effMiscRand(D_00451F20) % (effect->spread + 1));
    packet->color = 0;
    PCP_COPY_VECTOR(packet, effect->head.origin);
    jitter = effect->head.speedJitter;
    packet->speed = effect->head.speed * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter));
    jitter = effect->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    parDispatchKindInit(&effect->head.sub, index);
}

void effEmitterDiscAuxUpdate(EffEmitterK *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 kind = effect->head.sub.kind;
    f32 (*aux)[4] = effect->aux;
    f32 tmp[4];
    f32 origin[4];
    f32 decay;
    f32 spin;
    f32 waveRate;
    s32 cycles;
    s32 frames;
    s32 count;
    s32 i;
    u32 loop;

    parUpdateSharedScaleAndDelta(&effect->head.sub);
    VU0_LOAD_MATRIX(effect->head.matrix);
    spin = effect->f160 * (3.14159265f / 180.0f);
    decay = effect->f16C / 100.0f + 1.0f;
    cycles = 0;
    count = effect->head.packetCount;
    frames = effect->head.frameCount;
    loop = effect->flag150;
    waveRate = effect->f168;
    PCP_COPY_VECTOR(origin, effect->head.origin);
    for (i = 0; i < count; i++, packet++, aux++) {
        s32 age = packet->age;

        if (age == 0xF0000001) {
            effEmitterDiscAuxSpawn(effect, i);
            age = packet->age;
        }
        if (age >= 0) {
            f32 lift;
            f32 old;
            f32 k;

            packet->color = func_001616A8(effect->head.fade, packet->color, age);
            packet->color = effParModulateColors(packet->color, effect->head.colorMask);
            if (kind != 0) {
                VU0_LOAD_VF(vf12, packet);
            }
            packet->f30 += waveRate;
            lift = packet->f3C;
            packet->f34 += spin;
            k = sdfSinPoly(packet->f34);
            old = packet->f38;
            packet->f38 = k;
            k = (k - old) * packet->f30;
            tmp[0] = (*aux)[0] + packet->vel[0] * k;
            tmp[1] = (*aux)[1] + lift;
            tmp[2] = (*aux)[2] + packet->vel[2] * k;
            VU0_LOAD_VF(vf10, tmp);
            VU0_STORE_VF(vf10, *aux);
            VU0_APPLY_MATRIX(vf10, vf10);
            VU0_LOAD_VF(vf11, origin);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, packet);
            packet->f3C = lift * decay;
            if (kind != 0) {
                VU0_LOAD_VF(vf10, packet);
                parDispatchKindUpdate(&effect->head.sub, i, packet->color, packet->speed);
            }
        }
        age++;
        if (age >= frames) {
            if (loop) {
                age = 0xF0000001;
            } else {
                parDispatchKindInit(&effect->head.sub, i);
                cycles++;
                if (cycles >= count) {
                    effect->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

void effMarkAllTemplateBufferRecords(EffTemplatePacketList *effect) {
    EffectBufferRecord *record;
    u32 index;

    index = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            index = index + 1;
            record = record + 1;
        } while (index < effect->packetCount);
    }
}

void effScaleTemplatePacketPositions(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[1] = effect->tailValues[1] * scale;
}

/* Allocate one 12-byte descriptor per packet and its attached 16-byte records. */
s32 effCloneTemplateWithPacketDescriptors(EffTemplatePacketList *source) {
    s32 copy = (s32)func_00328D68(0x190);
    s32 tailLen = 0x40;
    s32 count;
    s32 perRecord;
    s32 listBytes;
    s32 *node;
    s32 *addr;
    s32 step;
    s32 base;
    s32 i;

    memset((void *)copy, 0, 0x190);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_0015B330(copy);
    perRecord = 0;
    if (((EffTemplatePacketList *)copy)->kind != 0) {
        count = ((EffTemplatePacketList *)copy)->packetCount;
        /* All four supported resource kinds use the same subrecord count. */
        switch (((EffTemplatePacketList *)copy)->kind) {
        case 1:
            perRecord = ((EffTemplatePacketList *)copy)->subrecordCount;
            break;
        case 2:
            perRecord = ((EffTemplatePacketList *)copy)->subrecordCount;
            break;
        case 3:
            perRecord = ((EffTemplatePacketList *)copy)->subrecordCount;
            break;
        case 4:
            perRecord = ((EffTemplatePacketList *)copy)->subrecordCount;
            break;
        }
        listBytes = count * 12;
        ((EffTemplatePacketList *)copy)->recordsPerPacket = perRecord;
        ((EffTemplatePacketList *)copy)->listAllocation = func_003292A8(listBytes + perRecord * count * 16);
        addr = (s32 *)sdfResourceRetainAddress(((EffTemplatePacketList *)copy)->listAllocation);
        i = 0;
        base = (s32)addr;
        ((EffTemplatePacketList *)copy)->recordList = base;
        base = base + listBytes;
        if (count > 0) {
            step = perRecord * 16;
            node = addr;
            do {
                i++;
                node[2] = base;
                node[0] = 0;
                node[1] = 0;
                node += 3;
                base += step;
            } while (i < count);
        }
    } else {
        ((EffTemplatePacketList *)copy)->listAllocation = 0;
    }
    effMarkAllTemplateBufferRecords((EffTemplatePacketList *)copy);
    return copy;
}
