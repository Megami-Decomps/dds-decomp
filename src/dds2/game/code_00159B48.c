#include "common.h"
#include "pcp_vu0.h"
extern void func_00333288(void *, u32);
extern void func_00333270(void *, u32);
extern void func_003332A0(void *, u32);
#include "ee_mmi.h"

extern void *sdfCreateAssetWithDrawEntries();
extern void func_003332D0(void *, f32);
extern void func_001594C8();
extern s32 effEmitterDelayRandomState[];
extern void effMiscSeedRandomFromClock();
#include "eff.h"

#define BILL_ENTRY_BYTES 0x14
#define BILL_FRAME_MODE_BITS 6
#define BILL_VARIANT_MASK 0xFFFF
#define EFF_INSTANCE_BYTES 0x88
#define EFF_MATRIX_BYTES 0x40
#define EFF_PACKET_LIST_BYTES 0x20
#define EFF_TEMPLATE_TAIL_OFFSET 0x150
#define EFF_PACKET_INITIAL_TAG 0xF0000001

typedef struct EffEmitterSub {
    u16 kind;
    u8 pad02[6];
    s32 nodeResource;       /* Kind 1: particle-node resource. */
    u32 unk0C;
    s32 primaryCellSystem;  /* Kind 2: cell system. */
    s32 secondaryResource;  /* Kind 3: cell system; kind 4: tracked model work. */
} EffEmitterSub;

/* Header shared by the effect emitters that spawn a ring or spray of packets:
 * an origin, a sub-effect, a fade descriptor, jitter ranges and the packet
 * buffer. The kind-specific parameters follow at +0x150. */
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
    s32 billboard;
    EffectBufferTail *buffer; /* 0xF8 */
    u8 padFC[0x46];
    u16 active;            /* 0x142 */
    u8 pad144[0xC];
} EffEmitterHead;


extern void effParReleaseNodeResource(s32);
extern void parReleaseCellSystem(s32);
extern void effTrackPolyDestroyModelWorkList(s32);

extern void effDestroyResources(EffEmitterHead *owner);

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
    s32 frame;   /* 0x20: starts at -1; zero requests reinitialization before increment */
    u32 color;   /* 0x24: packed RGBA built from the effect's colour and alpha */
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

extern s32 effBillResourceOwners[];

s32 sdfAllocPacketAligned(s32 size);

void sdfInitPacketList(s32 packet);

void func_0015AA30(s32 packet, s32 source);

void func_0015AD18(s32 packet, s32 source);

void func_0015B330(s32 effect);

void effSetTemplateTagPeriod(EffTemplatePacketList *effect);

extern void *memset(void *s, s32 c, u32 n);

extern void *memcpy(void *dest, const void *src, u32 n);

extern void *sdfAllocSizeClassBlock(s32 size);

extern EffectConfig D_003AA884[];

s32 billCreateIndexed(s32 kind, s32 index);

s32 sdfAllocGeneralBlock(s32 size);

EffectBufferRecord *sdfResourceRetainAddress(s32 allocation);

void effInitExpandRingPacketSchedule(EffTemplatePacketList *effect);

/* Create a billboard sharing the indexed entry's resource. Word two of the
 * resource stores the reference count; the BillObj entryList is not an emitter. */
void effRetainResource(s32 index) {
    BillObj *effect = (BillObj *)billCreateIndexed(D_003AA884[index].billboardKind, 0);
    s32 *resource = ((BillObj *)effBillResourceOwners[index])->entryList;
    s32 references = resource[2];

    effect->entryList = resource;
    resource[2] = references + 1;
}

u32 func_00159BB0(void) {
    return 0xf;
}

s32 effGetResourceFirstWord(s32 index) {
    return *(s32 *)((BillObj *)effBillResourceOwners[index])->entryList;
}

void effCopyVector(dst, src)
void *dst;
void *src;
{
    PCP_COPY_VECTOR(dst, src);
}

void billSetLengthExtent(BillObj *effect, float scale) {
    effect->lengthScale = scale;
}

void billSetChildScaleComponents(BillObj *effect, float x, float y) {
    effect->childScaleX = x;
    effect->childScaleY = y;
}

void billSetChildParameter(BillObj *effect, u32 value) {
    effect->childParam = value;
}

void effCopyPosition(BillObj *effect, const void *position) {
    if (effect->kind == 0) {
        memcpy((void *)((s32)effect->entryList + 0xc), position, 16);
    }
}

/* Narrow mode to s16; kinds 0/3 store it, while kind 1 replaces only frame bits 1..2. */
void billSetBillboardMode(BillObj *effect, s32 mode) {
    s32 entryCount;
    s32 remaining;
    s32 frameSlotAddress;
    mode = (s16)mode;
    switch (effect->kind) {
    case 0:
    case 3:
        effect->unk2E = mode;
        break;
    case 1:
        entryCount = effect->entryCount;
        if (entryCount > 0) {
            remaining = entryCount;
            /* Required to match: induction points to each entry's frame slot at +0x0C. */
            frameSlotAddress = (s32)effect->unk60 + 0xc;
            do {
                EffBillFrame *frameData = (EffBillFrame *)*(s32 *)frameSlotAddress;
                u32 frameFlags = frameData->flags & ~BILL_FRAME_MODE_BITS;
                frameData->flags = frameFlags;
                if (mode == 2) {
                    frameData->flags = frameFlags | 2;
                } else if (mode == 3) {
                    frameData->flags = frameFlags | 4;
                }
                frameSlotAddress += BILL_ENTRY_BYTES;
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

/* Store the signed variant in one child payload or every child of a list. */
void billSetAllChildVariants(BillObj *effect, s16 variant) {
    switch (effect->kind) {
    case 0:
        ((EffSlot *)effect->entryList)->value = variant;
        break;
    case 1: {
        EffSlotList *childList = effect->entryList;
        s32 remainingChildren = childList->count;
        EffSlot **childEntries = childList->slots;
        EffSlot **childCursor;

        if (remainingChildren > 0) {
            childCursor = childEntries;
            do {
                (*childCursor)->value = variant;
                childCursor++;
            } while (--remainingChildren != 0);
        }
        break;
    }
    }
}

s32 billGetChildValue(BillObj *effect) {
    if (effect->kind == 0) {
        return *(s32 *)effect->entryList;
    }
    return 0;
}

u16 billGetKind(BillObj *effect) {
    return effect->kind;
}

/* Store the low halfword through the kind-specific variant field. */
void billSetVariantValue(BillObj *effect, s32 value) {
    s32 variantValue = value & BILL_VARIANT_MASK;

    switch (effect->kind) {
    case 0:
        ((EffSlot *)effect->entryList)->value = variantValue;
        break;
    case 1:
        effect->unk3C = variantValue;
        break;
    }
}

u16 billGetVariantValue(BillObj *effect) {
    switch (effect->kind) {
    case 0:
        return ((EffSlot *)effect->entryList)->variant;
    case 1:
        return effect->unk3C;
    default:
        return 0;
    }
}

/* Replace the selected list entry only when its index changes. */
void billSetKind1Entry(BillObj *effect, u32 entryIndex) {
    if (effect->kind == 1 && effect->unk58 != entryIndex) {
        func_001594C8(effect, entryIndex);
    }
}

/* Read the selected entry for list billboards; other kinds have none. */
s32 billGetKindOneEntry(BillObj *effect) {
    if (effect->kind == 1) {
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
    if (((BillObj *)billboard)->kind == 1) {
        return ((BillValueLink *)((BillObj *)billboard)->entryList)->target->value;
    }
    return 0;
}

/* Start every entry's animation at the requested frame, with mode zero. */
void billSetEntryFrameMode0(BillObj *effect, u32 startFrame) {
    if (effect->kind == 1) {
        s32 entryCount = effect->entryCount;

        if (entryCount > 0) {
            EffBillEntry *entry = (EffBillEntry *)effect->unk60;
            s32 remaining = entryCount;

            do {
                EffBillFrame *frameData = entry->data;
                u32 wrappedFrame = startFrame % frameData->period;
                remaining -= 1;
                entry->mode = 0;
                entry->frame = wrappedFrame;
                entry++;
            } while (remaining != 0);
        }
    }
}

/* Start every entry's animation at the requested frame, with mode one. */
void billSetEntryFrameMode1(BillObj *effect, u32 startFrame) {
    if (effect->kind == 1) {
        s32 entryCount = effect->entryCount;

        if (entryCount > 0) {
            EffBillEntry *entry = (EffBillEntry *)effect->unk60;
            s32 remaining = entryCount;

            do {
                EffBillFrame *frameData = entry->data;
                u32 wrappedFrame = startFrame % frameData->period;
                remaining -= 1;
                entry->mode = 1;
                entry->frame = wrappedFrame;
                entry++;
            } while (remaining != 0);
        }
    }
}

/* Read the animation modulus of the first entry, if this is a list billboard. */
s32 billGetFirstEntryFramePeriod(BillObj *effect) {
    if (effect->kind == 1) {
        return ((EffBillEntry *)effect->unk60)->data->period;
    }
    return 0;
}

u16 billGetKindOneParameter(BillObj *effect) {
    if (effect->kind == 1) {
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

/* Kind 0 stores half the supplied width/height; other kinds remain unchanged. */
void billSetChildHalfExtents(s32 billboard, float width, float height) {
    if (((BillKindOneView *)billboard)->kind == 0) {
        s32 payloadAddress = (s32)((BillObj *)billboard)->entryList;
        ((BillChildPayload *)payloadAddress)->halfWidth = width * 0.5f;
        ((BillChildPayload *)payloadAddress)->halfHeight = height * 0.5f;
    }
}

extern u8 sdfViewMatrix[];
extern u8 sdfProjectionMatrix[];
extern u8 D_0037F650[];
extern u8 D_0037F660[];
extern u8 D_003AA9B0[];
extern void sdfPostmultiplyVuMatrixFromMemory(void *);
extern f32 sdfAtan2(f32, f32);

/* vu0 routine: project position and position + scaled offset; return the reverse XY heading. */
f32 effComputeProjectedOffsetAngle(const void *position, const void *offset) {
    f32 projectedDelta[4];
    f32 projectedPosition[4];
    f32 projectedEndpoint[4];

    VU0_LOAD_MATRIX(sdfViewMatrix);
    sdfPostmultiplyVuMatrixFromMemory(sdfProjectionMatrix);
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
    VU0_STORE_VF(vf10, projectedEndpoint);

    projectedDelta[0] = projectedPosition[0] - projectedEndpoint[0];
    projectedDelta[1] = projectedPosition[1] - projectedEndpoint[1];
    projectedDelta[2] = 0.0f;
    VU0_LOAD_VF(vf10, projectedDelta);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, projectedDelta);
    return sdfAtan2(projectedDelta[1], projectedDelta[0]);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015A150);

/* Create the indexed billboard and render asset; only the second matrix is initialized. */
u8 *billCreateUnitObject(s32 entryIndex) {
    EffInstance *instance = sdfAllocSizeClassBlock(EFF_INSTANCE_BYTES);

    instance->billboard = (BillObj *)billCreateIndexed(1, entryIndex);
    instance->renderState = sdfCreateAssetWithDrawEntries();
    func_003332D0(instance->renderState, 1.0f);
    EE_MMI_UNIT_MATRIX(instance->transform);
    return (u8 *)instance;
}

/* Retain a cloned billboard's shared data, create a fresh render asset, and reset both matrices. */
u8 *billCloneUnitObject(EffInstance *source) {
    EffInstance *instance = sdfAllocSizeClassBlock(EFF_INSTANCE_BYTES);

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

/* vu0 routine: copy the supplied 4x4 matrix to the instance's second matrix. */
void effCopyMatrixToNext(EffInstance *instance, void *matrix) {
    u8 *transformBytes;

    VU0_LOAD_MATRIX(matrix);
    transformBytes = instance->transform;
    VU0_STORE_MATRIX(transformBytes);
}

void billSetChildValue(EffInstance *instance, u32 value) {
    billSetChildParameter(instance->billboard, value);
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effVuCopyMatrix(void *dst, void *src) {
    VU0_COPY_MATRIX(dst, src);
}

/* Kind 1 writes mode plus one/two entry values; untouched output words retain their contents. */
void effReadBillboardModeValues(EffInstance *instance, s32 *modeValues) {
    BillObj *billboard = instance->billboard;

    if (billboard->kind == 1) {
        u32 modeFlags = billboard->modeFlags;

        if (modeFlags & 0x40) {
            modeValues[0] = 2;
            modeValues[2] = func_00158F88((s32)billboard, (s32)billboard->unk60);
            modeValues[1] = func_00158F88((s32)billboard, (s32)billboard->unk60 + BILL_ENTRY_BYTES);
        } else if (modeFlags & 0x80) {
            modeValues[0] = 3;
            modeValues[2] = func_00158F88((s32)billboard, (s32)billboard->unk60);
            modeValues[1] = func_00158F88((s32)billboard, (s32)billboard->unk60 + BILL_ENTRY_BYTES);
        } else {
            modeValues[0] = 0;
            modeValues[1] = func_00158F88((s32)billboard, (s32)billboard->unk60);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015A4B0);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015AA30);

/* Effect owner installs a callback at +0x10 to accept a new packet list. */
typedef struct EffPacketSink {
    u8 pad00[0x10];
    void (*submit)(s32 owner, s32 packet);
} EffPacketSink;

/* Allocate/init a packet list, generate its texture payload, then submit it to the sink. */
void effSubmitGeneratedTexturePacket(s32 sink, s32 source) {
    s32 packetAddress = sdfAllocPacketAligned(EFF_PACKET_LIST_BYTES);

    sdfInitPacketList(packetAddress);
    func_0015AA30(packetAddress, source);
    ((EffPacketSink *)sink)->submit(sink, packetAddress);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015AD18);

/* Allocate/init a packet list, generate its composite GS payload, then submit it to the sink. */
void effSubmitCompositeGsPacket(s32 sink, s32 source) {
    s32 packetAddress = sdfAllocPacketAligned(EFF_PACKET_LIST_BYTES);

    sdfInitPacketList(packetAddress);
    func_0015AD18(packetAddress, source);
    ((EffPacketSink *)sink)->submit(sink, packetAddress);
}

void func_0015B270(void) {
    effMiscSeedRandomFromClock(effEmitterDelayRandomState);
}

void func_0015B290(void) {
}

/* Allocate records followed by their owner tail; clear only two native state words per record. */
EffectBufferTail *effAllocateBuffer(s32 recordCount) {
    s32 recordBytes = recordCount * sizeof(EffectBufferRecord);
    s32 allocationHandle = sdfAllocGeneralBlock(recordBytes + sizeof(EffectBufferTail));
    EffectBufferRecord *recordCursor = sdfResourceRetainAddress(allocationHandle);
    EffectBufferTail *bufferTail = (EffectBufferTail *)((u8 *)recordCursor + recordBytes);

    bufferTail->allocation = allocationHandle;
    bufferTail->records = recordCursor;
    if (recordCount > 0) {
        s32 remaining = recordCount;
        do {
            remaining--;
            recordCursor->unk20 = 0;
            recordCursor->unk24 = 0;
            recordCursor++;
        } while (remaining != 0);
    }
    return bufferTail;
}

void effReleaseBufferAllocation(u32 *allocationSlot) {
    sdfReleaseResourceAllocation(*allocationSlot);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015B330);

/* Release the selected sub-effect resource, billboard and packet-buffer
 * allocation described by the common emitter prefix. */
void effDestroyResources(EffEmitterHead *owner) {
    switch (owner->sub.kind) {
    case 1:
        effParReleaseNodeResource(owner->sub.nodeResource);
        break;
    case 2:
        parReleaseCellSystem(owner->sub.primaryCellSystem);
        break;
    case 3:
        parReleaseCellSystem(owner->sub.secondaryResource);
        break;
    case 4:
        effTrackPolyDestroyModelWorkList(owner->sub.secondaryResource);
        break;
    }
    billDispatchByKind(owner->billboard);
    effReleaseBufferAllocation(owner->buffer);
}

/* Stagger native tags in period-sized groups; a nonempty buffer requires a nonzero period. */
void effSetTemplateTagPeriod(EffTemplatePacketList *effect) {
    EffectBufferRecord *packetCursor;
    u32 packetIndex;
    u32 nextPacketIndex;
    s32 packetTag;

    packetTag = EFF_PACKET_INITIAL_TAG;
    packetIndex = 0;
    packetCursor = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            packetCursor->unk20 = packetTag;
            nextPacketIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
            if (nextPacketIndex % effect->tailWords[0] == 0) {
                packetTag = packetTag - effect->decayStep;
            }
            packetIndex = nextPacketIndex;
        } while (packetIndex < effect->packetCount);
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

/* Clone the template prefix and appended tail, then allocate its packet records. */
s32 effCloneRingTemplate(EffTemplatePacketList *source) {
    s32 clone = (s32)sdfAllocSizeClassBlock(0x180);
    s32 tailBytes = 0x30;

    memset((void *)clone, 0, 0x180);
    memcpy((void *)clone, source, source->templateSize);
    memcpy((void *)(clone + EFF_TEMPLATE_TAIL_OFFSET), (u8 *)source + source->templateSize, tailBytes);
    func_0015B330(clone);
    effSetTemplateTagPeriod(clone);
    return clone;
}

void effFreeRingTemplate(u32 effect) {
    effDestroyResources((EffEmitterHead *)effect);
    sdfReleaseChipBlock(effect);
}


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

typedef struct EffRingEmitter {
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
} EffRingEmitter;

/* Seed one ring packet's phase, radius and vertical motion, then its subeffect. */
void effEmitterRingSpawn(EffRingEmitter *effect, u32 packetIndex) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 phaseIndex;
    f32 phase;
    f32 jitter;

    packet += packetIndex;
    packet->color = 0;
    packet->age = 0;
    phase = 0.0f;
    phaseIndex = packetIndex % effect->period;
    if (phaseIndex != 0) {
        phase = (3.14159265f * 2.0f) / effect->period * phaseIndex;
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
    parDispatchKindInit(&effect->head.sub, packetIndex);
}

/* Advance ring motion and ages; repeat by tagging packets for the next update.
 * completedCount counts expired nonrepeating packets in this update only. */
/* vf12 keeps the packet's previous position and vf10 the new one for
 * parDispatchKindUpdate, which reads them as implicit arguments. */
void effEmitterRingUpdate(EffRingEmitter *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 subeffectKind = effect->head.sub.kind;
    f32 origin[4];
    f32 offset[4];
    f32 verticalScale;
    f32 phaseStep;
    s32 completedCount;
    s32 repeatEnabled;
    s32 packetCount;
    s32 lifetimeFrames;
    s32 packetIndex;

    parUpdateSharedScaleAndDelta(&effect->head.sub);
    VU0_LOAD_MATRIX(effect->head.matrix);
    phaseStep = effect->f164 * (3.14159265f / 180.0f);
    verticalScale = effect->f16C / 100.0f + 1.0f;
    completedCount = 0;
    packetCount = effect->head.packetCount;
    lifetimeFrames = effect->head.frameCount;
    repeatEnabled = effect->flag151;
    PCP_COPY_VECTOR(origin, effect->head.origin);
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++) {
        s32 age = packet->age;

        if (age == EFF_PACKET_INITIAL_TAG) {
            effEmitterRingSpawn(effect, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            f32 theta;
            f32 radius;

            packet->color = func_001616A8(effect->head.fade, packet->color, age);
            packet->color = effParModulateColors(packet->color, effect->head.colorMask);
            if (subeffectKind != 0) {
                VU0_LOAD_VF(vf12, packet);
            }
            packet->f34 += (packet->vel[0] - effect->radius) / lifetimeFrames;
            packet->f30 += phaseStep;
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
            packet->f38 *= verticalScale;
            if (subeffectKind != 0) {
                VU0_LOAD_VF(vf10, packet);
                parDispatchKindUpdate(&effect->head.sub, packetIndex, packet->color, packet->speed);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                completedCount++;
                parDispatchKindInit(&effect->head.sub, packetIndex);
                if (completedCount >= packetCount) {
                    effect->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

/* Initialize only the native age/tag word of each packet record. */
void effResetDiscPacketAges(EffTemplatePacketList *effect) {
    EffectBufferRecord *packetCursor;
    u32 packetIndex;

    packetIndex = 0;
    packetCursor = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            packetCursor->unk20 = EFF_PACKET_INITIAL_TAG;
            packetIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
        } while (packetIndex < effect->packetCount);
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

/* Clone the disc prefix/tail, allocate packet records, and initialize their native age tags. */
s32 effCloneTemplate(EffTemplatePacketList *source) {
    s32 clone = (s32)sdfAllocSizeClassBlock(0x180);
    s32 tailBytes = 0x30;

    memset((void *)clone, 0, 0x180);
    memcpy((void *)clone, source, source->templateSize);
    memcpy((void *)(clone + EFF_TEMPLATE_TAIL_OFFSET), (u8 *)source + source->templateSize, tailBytes);
    func_0015B330(clone);
    effResetDiscPacketAges(clone);
    return clone;
}

void effFreeDiscTemplate(u32 effect) {
    effDestroyResources((EffEmitterHead *)effect);
    sdfReleaseChipBlock(effect);
}

typedef struct EffDiscEmitter {
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
} EffDiscEmitter;

/* Seed a random planar position and direction; radialDistance may be negative. */
void effEmitterDiscSpawn(EffDiscEmitter *effect, u32 packetIndex) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    f32 directionVector[4];
    f32 radialDistance;
    f32 jitter;

    packet += packetIndex;
    radialDistance = effect->radius * ((effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f);
    directionVector[0] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    directionVector[1] = 0;
    directionVector[2] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, directionVector);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, directionVector);
    packet->pos[0] = directionVector[0] * radialDistance;
    packet->pos[1] = 0;
    packet->pos[2] = directionVector[2] * radialDistance;
    VU0_LOAD_MATRIX(effect->head.matrix);
    VU0_LOAD_VF(vf10, packet->pos);
    VU0_APPLY_MATRIX(vf10, vf10);
    VU0_STORE_VF(vf10, packet->pos);
    packet->pos[0] += effect->head.origin[0];
    packet->pos[1] += effect->head.origin[1];
    packet->pos[2] += effect->head.origin[2];
    directionVector[0] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    directionVector[1] = 0;
    directionVector[2] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, directionVector);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, packet->vel);
    packet->f30 = effect->f164 * (effMiscRandUnitFloat(D_003AA868) * effect->jitterB + (1.0f - effect->jitterB));
    packet->f34 = 0;
    packet->f38 = 0;
    packet->f3C = effect->f15C * (effMiscRandUnitFloat(D_003AA868) * effect->jitterA + (1.0f - effect->jitterA));
    packet->age = -(effMiscRand(effEmitterDelayRandomState) % (effect->spread + 1));
    packet->color = 0;
    jitter = effect->head.speedJitter;
    packet->speed = effect->head.speed * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter));
    jitter = effect->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    parDispatchKindInit(&effect->head.sub, packetIndex);
}

/* Advance sinusoidal planar displacement and scaled vertical steps, then ages. */
void effEmitterDiscUpdate(EffDiscEmitter *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 subeffectKind = effect->head.sub.kind;
    f32 positionDelta[4];
    f32 verticalScale;
    f32 phaseStep;
    f32 waveRate;
    s32 completedCount;
    s32 lifetimeFrames;
    s32 packetCount;
    s32 packetIndex;
    u32 repeatEnabled;

    parUpdateSharedScaleAndDelta(&effect->head.sub);
    VU0_LOAD_MATRIX(effect->head.matrix);
    phaseStep = effect->f160 * (3.14159265f / 180.0f);
    verticalScale = effect->f16C / 100.0f + 1.0f;
    lifetimeFrames = effect->head.frameCount;
    packetCount = effect->head.packetCount;
    repeatEnabled = effect->flag150;
    waveRate = effect->f168;
    completedCount = 0;
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++) {
        s32 age = packet->age;

        if (age == EFF_PACKET_INITIAL_TAG) {
            effEmitterDiscSpawn(effect, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            f32 verticalStep;
            f32 previousWaveValue;
            f32 factor;

            packet->color = func_001616A8(effect->head.fade, packet->color, age);
            packet->color = effParModulateColors(packet->color, effect->head.colorMask);
            if (subeffectKind != 0) {
                VU0_LOAD_VF(vf12, packet);
            }
            packet->f30 += waveRate;
            verticalStep = packet->f3C;
            packet->f34 += phaseStep;
            factor = sdfSinPoly(packet->f34);
            previousWaveValue = packet->f38;
            packet->f38 = factor;
            factor = (factor - previousWaveValue) * packet->f30;
            positionDelta[0] = packet->vel[0] * factor;
            positionDelta[1] = verticalStep;
            positionDelta[2] = packet->vel[2] * factor;
            VU0_LOAD_VF(vf10, positionDelta);
            VU0_APPLY_MATRIX(vf10, vf10);
            VU0_STORE_VF(vf10, positionDelta);
            packet->pos[0] += positionDelta[0];
            packet->pos[1] += positionDelta[1];
            packet->pos[2] += positionDelta[2];
            packet->f3C = verticalStep * verticalScale;
            if (subeffectKind != 0) {
                VU0_LOAD_VF(vf10, packet);
                parDispatchKindUpdate(&effect->head.sub, packetIndex, packet->color, packet->speed);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                parDispatchKindInit(&effect->head.sub, packetIndex);
                completedCount++;
                if (completedCount >= packetCount) {
                    effect->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

/* Initialize only the native age/tag word of each packet record. */
void effResetBallisticPacketAges(EffTemplatePacketList *effect) {
    EffectBufferRecord *packetCursor;
    u32 packetIndex;

    packetIndex = 0;
    packetCursor = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            packetCursor->unk20 = EFF_PACKET_INITIAL_TAG;
            packetIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
        } while (packetIndex < effect->packetCount);
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

/* Clone the ballistic prefix/tail, allocate packet records, and initialize their native age tags. */
s32 effCloneBallisticTemplate(EffTemplatePacketList *source) {
    s32 clone = (s32)sdfAllocSizeClassBlock(0x180);
    s32 tailBytes = 0x30;

    memset((void *)clone, 0, 0x180);
    memcpy((void *)clone, source, source->templateSize);
    memcpy((void *)(clone + EFF_TEMPLATE_TAIL_OFFSET), (u8 *)source + source->templateSize, tailBytes);
    func_0015B330(clone);
    effResetBallisticPacketAges(clone);
    return clone;
}

void effFreeBallisticTemplate(u32 effect) {
    effDestroyResources((EffEmitterHead *)effect);
    sdfReleaseChipBlock(effect);
}

typedef struct EffBallisticEmitter {
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
} EffBallisticEmitter;

void func_0015C3C8(EffBallisticEmitter *effect, s32 index);
INCLUDE_ASM(const s32, "game/code_00159B48", func_0015C3C8);

/* Scale velocity, add its stored vertical acceleration, and advance position.
 * Only mode zero transforms the displacement through the emitter matrix. */
void effEmitterBallisticUpdate(EffBallisticEmitter *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 subeffectKind = effect->head.sub.kind;
    u32 mode;
    f32 positionDelta[4];
    f32 velocityScale;
    s32 completedCount;
    s32 lifetimeFrames;
    s32 packetCount;
    s32 repeatEnabled;
    s32 packetIndex;

    parUpdateSharedScaleAndDelta(&effect->head.sub);
    mode = effect->mode;
    if (mode == 0) {
        VU0_LOAD_MATRIX(effect->head.matrix);
    }
    velocityScale = effect->decayPct / 100.0f + 1.0f;
    completedCount = 0;
    lifetimeFrames = effect->head.frameCount;
    packetCount = effect->head.packetCount;
    repeatEnabled = effect->loop;
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++) {
        s32 age = packet->age;

        if (age == EFF_PACKET_INITIAL_TAG) {
            func_0015C3C8(effect, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            packet->color = func_001616A8(effect->head.fade, packet->color, age);
            packet->color = effParModulateColors(packet->color, effect->head.colorMask);
            if (subeffectKind != 0) {
                VU0_LOAD_VF(vf12, packet);
            }
            packet->vel[1] = packet->vel[1] * velocityScale + packet->f38;
            packet->vel[0] *= velocityScale;
            packet->vel[2] *= velocityScale;
            VU0_LOAD_VF(vf10, packet->vel);
            if (mode == 0) {
                VU0_APPLY_MATRIX(vf10, vf10);
            }
            VU0_STORE_VF(vf10, positionDelta);
            packet->pos[0] += positionDelta[0];
            packet->pos[1] += positionDelta[1];
            packet->pos[2] += positionDelta[2];
            if (subeffectKind != 0) {
                VU0_LOAD_VF(vf10, packet);
                parDispatchKindUpdate(&effect->head.sub, packetIndex, packet->color, packet->speed);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                parDispatchKindInit(&effect->head.sub, packetIndex);
                completedCount++;
                if (completedCount >= packetCount) {
                    effect->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

typedef struct EffLookAtRingEmitter EffLookAtRingEmitter;

extern void effEmitterLookAtRingSpawn(EffLookAtRingEmitter *effect, u32 index);

/* Spawn each packet, then overwrite its tag with a zero-based, period-grouped schedule. */
void effInitLookAtRingPacketSchedule(EffTemplatePacketList *effect) {
    EffectBufferRecord *packetCursor;
    u32 packetIndex;
    u32 nextPacketIndex;
    s32 packetTag;

    packetTag = 0;
    packetIndex = 0;
    packetCursor = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            effEmitterLookAtRingSpawn((EffLookAtRingEmitter *)effect, packetIndex);
            packetCursor->unk20 = packetTag;
            nextPacketIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
            if (nextPacketIndex % effect->tailWords[0] == 0) {
                packetTag = packetTag - effect->decayStep;
            }
            packetIndex = nextPacketIndex;
        } while (packetIndex < effect->packetCount);
    }
}

void effScaleLookAtRingTemplate(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
}

/* Clone the look-at ring prefix/tail and spawn its staggered packet schedule. */
s32 effCloneLookAtRingTemplate(EffTemplatePacketList *source) {
    s32 clone = (s32)sdfAllocSizeClassBlock(0x170);
    s32 tailBytes = 0x20;

    memset((void *)clone, 0, 0x170);
    memcpy((void *)clone, source, source->templateSize);
    memcpy((void *)(clone + EFF_TEMPLATE_TAIL_OFFSET), (u8 *)source + source->templateSize, tailBytes);
    func_0015B330(clone);
    effInitLookAtRingPacketSchedule(clone);
    return clone;
}

void effFreeLookAtRingTemplate(u32 effect) {
    effDestroyResources((EffEmitterHead *)effect);
    sdfReleaseChipBlock(effect);
}

struct EffLookAtRingEmitter {
    EffEmitterHead head;
    u8 mode;         /* 0x150 */
    u8 loop;         /* 0x151 */
    u8 pad152[10];
    u32 period;      /* 0x15C */
    f32 f160;
    f32 f164;
};

extern u8 sdfViewEyeVector[];
extern u8 sdfViewTargetVector[];
extern u8 sdfViewUpVector[];
extern void sdfVuBuildLookAtBasis(void *, void *, void *);
extern void sdfInvertRigidVuTransform(void);

/* Seed a view-oriented ring packet and initialize its subeffect. */
void effEmitterLookAtRingSpawn(EffLookAtRingEmitter *effect, u32 packetIndex) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 period = 0;
    f32 angle;
    f32 scale;
    f32 jitter;

    packet += packetIndex;
    sdfVuBuildLookAtBasis(sdfViewEyeVector, sdfViewTargetVector, sdfViewUpVector);
    sdfInvertRigidVuTransform();
    if (effect->mode == 0) {
        PCP_COPY_VECTOR(packet, effect->head.origin);
        period = effect->period;
        angle = (3.14159265f * 2.0f) / period * (packetIndex % period);
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
        f32 lifetimeFrames = effect->head.frameCount;

        scale = effect->f164;
        /* Native quirk: period was assigned only in mode zero and is still zero here. */
        angle = (3.14159265f * 2.0f) / period * (packetIndex % period);
        scale *= lifetimeFrames;
        angle += effect->f160 * (3.14159265f / 180.0f) * lifetimeFrames;
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
    parDispatchKindInit(&effect->head.sub, packetIndex);
}

/* Advance view-oriented motion and ages; repeated packets respawn immediately. */
void effEmitterLookAtRingUpdate(EffLookAtRingEmitter *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 subeffectKind = effect->head.sub.kind;
    f32 phaseStep;
    f32 stepScale;
    s32 completedCount = 0;
    s32 repeatEnabled;
    s32 packetCount;
    s32 lifetimeFrames;
    s32 packetIndex;

    parUpdateSharedScaleAndDelta(&effect->head.sub);
    sdfVuBuildLookAtBasis(sdfViewEyeVector, sdfViewTargetVector, sdfViewUpVector);
    sdfInvertRigidVuTransform();
    lifetimeFrames = effect->head.frameCount;
    phaseStep = effect->f160 * (3.14159265f / 180.0f);
    packetCount = effect->head.packetCount;
    repeatEnabled = effect->loop;
    stepScale = effect->f164;
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++) {
        s32 age = packet->age;
        f32 angle;

        if (age >= 0) {
            packet->color = func_001616A8(effect->head.fade, packet->color, age);
            packet->color = effParModulateColors(packet->color, effect->head.colorMask);
            if (subeffectKind != 0) {
                VU0_LOAD_VF(vf12, packet);
            }
            packet->pos[0] += packet->vel[0] * stepScale;
            packet->pos[1] += packet->vel[1] * stepScale;
            packet->pos[2] += packet->vel[2] * stepScale;
            angle = packet->f34;
            packet->vel[0] = sdfEvaluateCosineViaSinePhaseShift(angle);
            packet->vel[1] = sdfSinPoly(angle);
            packet->vel[2] = 0;
            VU0_LOAD_VF(vf10, packet->vel);
            VU0_ROTATE_VEC(vf10, vf10);
            VU0_STORE_VF(vf10, packet->vel);
            packet->f34 += phaseStep;
            if (subeffectKind != 0) {
                VU0_LOAD_VF(vf10, packet);
                parDispatchKindUpdate(&effect->head.sub, packetIndex, packet->color, packet->speed);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                effEmitterLookAtRingSpawn((EffLookAtRingEmitter *)effect, packetIndex);
                age = packet->age;
            } else {
                parDispatchKindInit(&effect->head.sub, packetIndex);
                completedCount++;
                if (completedCount >= packetCount) {
                    effect->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

/* Initialize only the native age/tag word of each packet record. */
void effResetBurstPacketAges(EffTemplatePacketList *effect) {
    EffectBufferRecord *packetCursor;
    u32 packetIndex;

    packetIndex = 0;
    packetCursor = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            packetCursor->unk20 = EFF_PACKET_INITIAL_TAG;
            packetIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
        } while (packetIndex < effect->packetCount);
    }
}

void effScaleBurstTemplate(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[1] = effect->tailValues[1] * scale;
}

/* Clone the burst prefix/tail, allocate packet records, and initialize their native age tags. */
s32 effCloneBurstTemplate(EffTemplatePacketList *source) {
    s32 clone = (s32)sdfAllocSizeClassBlock(0x170);
    s32 tailBytes = 0x20;

    memset((void *)clone, 0, 0x170);
    memcpy((void *)clone, source, source->templateSize);
    memcpy((void *)(clone + EFF_TEMPLATE_TAIL_OFFSET), (u8 *)source + source->templateSize, tailBytes);
    func_0015B330(clone);
    effResetBurstPacketAges(clone);
    return clone;
}

void effFreeBurstTemplate(u32 effect) {
    effDestroyResources((EffEmitterHead *)effect);
    sdfReleaseChipBlock(effect);
}

typedef struct EffBurstEmitter {
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
} EffBurstEmitter;

/* Seed burst velocity and an optional random axis; retain the native spawn delay. */
void effEmitterBurstSpawn(EffBurstEmitter *effect, u32 packetIndex) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    f32 directionVector[4];
    f32 speed;
    f32 jitter;
    f32 velocityLength;

    packet += packetIndex;
    packet->color = 0;
    packet->age = -(effMiscRand(effEmitterDelayRandomState) % (effect->spread + 1));
    speed = effect->speed;
    jitter = effect->jitterA;
    directionVector[0] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    directionVector[1] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    directionVector[2] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, directionVector);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, directionVector);
    packet->vel[0] = speed * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) * directionVector[0];
    packet->vel[1] = speed * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) * directionVector[1];
    packet->vel[2] = speed * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) * directionVector[2];
    VU0_LOAD_VF(vf10, packet->vel);
    VU0_LENGTH_VF10(velocityLength);
    VU0_LOAD_MATRIX(effect->head.matrix);
    VU0_LOAD_VF(vf10, packet->vel);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF_UNCLOBBERED(vf10, packet->vel);
    packet->pos[0] = packet->vel[0] + effect->head.origin[0];
    packet->pos[1] = packet->vel[1] + effect->head.origin[1];
    packet->pos[2] = packet->vel[2] + effect->head.origin[2];
    if (effect->mode == 1) {
        directionVector[0] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
        directionVector[1] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
        directionVector[2] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
        VU0_LOAD_VF(vf10, directionVector);
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF(vf10, directionVector);
        packet->f30 = directionVector[0];
        packet->f34 = directionVector[1];
        packet->f38 = directionVector[2];
    } else {
        packet->f30 = 0;
        packet->f34 = -1.0f;
        packet->f38 = 0;
    }
    jitter = effect->jitterB;
    packet->f3C = (effect->f160 * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) - velocityLength) / effect->head.frameCount;
    jitter = effect->head.speedJitter;
    packet->speed = effect->head.speed * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter));
    jitter = effect->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    parDispatchKindInit(&effect->head.sub, packetIndex);
}

extern void sdfBuildVuRotationFromAxisAngle(f32 angle, f32 *axis);

/* Adjust velocity length, rotate around the packet axis, and rebuild position. */
void effEmitterBurstUpdate(EffBurstEmitter *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 subeffectKind = effect->head.sub.kind;
    f32 origin[4];
    f32 rotationAxis[4];
    f32 scaleVector[4];
    f32 velocityLength;
    f32 rotationStep;
    s32 completedCount;
    s32 lifetimeFrames;
    s32 repeatEnabled;
    s32 packetCount;
    s32 packetIndex;

    parUpdateSharedScaleAndDelta(&effect->head.sub);
    VU0_LOAD_MATRIX_B(effect->head.matrix);
    packetCount = effect->head.packetCount;
    lifetimeFrames = effect->head.frameCount;
    completedCount = 0;
    rotationStep = effect->spinRate * (3.14159265f / 180.0f);
    repeatEnabled = effect->loop;
    PCP_COPY_VECTOR(origin, effect->head.origin);
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++) {
        s32 age = packet->age;

        if (age == EFF_PACKET_INITIAL_TAG) {
            effEmitterBurstSpawn(effect, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            packet->color = func_001616A8(effect->head.fade, packet->color, age);
            packet->color = effParModulateColors(packet->color, effect->head.colorMask);
            if (subeffectKind != 0) {
                VU0_LOAD_VF(vf12, packet);
            }
            VU0_LOAD_VF(vf10, packet->vel);
            VU0_LENGTH_VF10(velocityLength);
            velocityLength += packet->f3C;
            scaleVector[0] = velocityLength;
            scaleVector[1] = velocityLength;
            scaleVector[2] = velocityLength;
            VU0_NORMALIZE_VF10();
            VU0_LOAD_VF(vf11, scaleVector);
            VU0_MUL(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, packet->vel);
            VU0_MOVE_VF(vf28, vf24);
            VU0_MOVE_VF(vf29, vf25);
            VU0_MOVE_VF(vf30, vf26);
            VU0_MOVE_VF(vf31, vf27);
            rotationAxis[0] = packet->f30;
            rotationAxis[1] = packet->f34;
            rotationAxis[2] = packet->f38;
            rotationAxis[3] = 0;
            VU0_LOAD_VF(vf10, rotationAxis);
            VU0_ROTATE_VEC(vf10, vf10);
            VU0_STORE_VF_UNCLOBBERED(vf10, rotationAxis);
            sdfBuildVuRotationFromAxisAngle(rotationStep, rotationAxis);
            VU0_LOAD_VF(vf10, packet->vel);
            VU0_ROTATE_VEC(vf10, vf10);
            VU0_LOAD_VF(vf11, origin);
            VU0_STORE_VF(vf10, packet->vel);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, packet);
            if (subeffectKind != 0) {
                VU0_LOAD_VF(vf10, packet);
                parDispatchKindUpdate(&effect->head.sub, packetIndex, packet->color, packet->speed);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                parDispatchKindInit(&effect->head.sub, packetIndex);
                completedCount++;
                if (completedCount >= packetCount) {
                    effect->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

/* Initialize only the native age/tag word of each packet record. */
void effResetSpherePacketAges(EffTemplatePacketList *effect) {
    EffectBufferRecord *packetCursor;
    u32 packetIndex;

    packetIndex = 0;
    packetCursor = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            packetCursor->unk20 = EFF_PACKET_INITIAL_TAG;
            packetIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
        } while (packetIndex < effect->packetCount);
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

/* Clone the sphere prefix/tail, allocate packet records, and initialize their native age tags. */
s32 effCloneSphereTemplate(EffTemplatePacketList *source) {
    s32 clone = (s32)sdfAllocSizeClassBlock(0x180);
    s32 tailBytes = 0x30;

    memset((void *)clone, 0, 0x180);
    memcpy((void *)clone, source, source->templateSize);
    memcpy((void *)(clone + EFF_TEMPLATE_TAIL_OFFSET), (u8 *)source + source->templateSize, tailBytes);
    func_0015B330(clone);
    effResetSpherePacketAges(clone);
    return clone;
}

void effFreeSphereTemplate(u32 effect) {
    effDestroyResources((EffEmitterHead *)effect);
    sdfReleaseChipBlock(effect);
}

typedef struct EffSphereEmitter {
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
} EffSphereEmitter;

/* Seed a jittered spherical position and planar direction.
 * Native delay uses bitwise complement, not unary negation. */
void effEmitterSphereSpawn(EffSphereEmitter *effect, u32 packetIndex) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    f32 directionVector[4];
    f32 radius;
    f32 jitter;

    packet += packetIndex;
    radius = effect->radius;
    jitter = effect->f178;
    directionVector[0] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    directionVector[1] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    directionVector[2] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, directionVector);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, directionVector);
    packet->pos[0] = radius * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) * directionVector[0];
    packet->pos[1] = radius * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) * directionVector[1];
    packet->pos[2] = radius * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) * directionVector[2];
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
    packet->age = ~(effMiscRand(effEmitterDelayRandomState) % (effect->spread + 1));
    packet->color = 0;
    jitter = effect->head.speedJitter;
    packet->speed = effect->head.speed * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter));
    jitter = effect->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    parDispatchKindInit(&effect->head.sub, packetIndex);
}

/* Advance sinusoidal planar displacement and scaled vertical steps, then ages. */
void effEmitterSphereUpdate(EffSphereEmitter *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 subeffectKind = effect->head.sub.kind;
    f32 positionDelta[4];
    f32 verticalScale;
    f32 phaseStep;
    f32 waveRate;
    s32 completedCount;
    s32 lifetimeFrames;
    s32 packetCount;
    s32 packetIndex;
    u32 repeatEnabled;

    parUpdateSharedScaleAndDelta(&effect->head.sub);
    VU0_LOAD_MATRIX(effect->head.matrix);
    phaseStep = effect->f160 * (3.14159265f / 180.0f);
    verticalScale = effect->f16C / 100.0f + 1.0f;
    lifetimeFrames = effect->head.frameCount;
    packetCount = effect->head.packetCount;
    repeatEnabled = effect->flag150;
    waveRate = effect->f168;
    completedCount = 0;
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++) {
        s32 age = packet->age;

        if (age == EFF_PACKET_INITIAL_TAG) {
            effEmitterSphereSpawn(effect, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            f32 verticalStep;
            f32 previousWaveValue;
            f32 factor;

            packet->color = func_001616A8(effect->head.fade, packet->color, age);
            packet->color = effParModulateColors(packet->color, effect->head.colorMask);
            if (subeffectKind != 0) {
                VU0_LOAD_VF(vf12, packet);
            }
            packet->f30 += waveRate;
            verticalStep = packet->f3C;
            packet->f34 += phaseStep;
            factor = sdfSinPoly(packet->f34);
            previousWaveValue = packet->f38;
            packet->f38 = factor;
            factor = (factor - previousWaveValue) * packet->f30;
            positionDelta[0] = packet->vel[0] * factor;
            positionDelta[1] = verticalStep;
            positionDelta[2] = packet->vel[2] * factor;
            VU0_LOAD_VF(vf10, positionDelta);
            VU0_APPLY_MATRIX(vf10, vf10);
            VU0_STORE_VF(vf10, positionDelta);
            packet->pos[0] += positionDelta[0];
            packet->pos[1] += positionDelta[1];
            packet->pos[2] += positionDelta[2];
            packet->f3C = verticalStep * verticalScale;
            if (subeffectKind != 0) {
                VU0_LOAD_VF(vf10, packet);
                parDispatchKindUpdate(&effect->head.sub, packetIndex, packet->color, packet->speed);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                parDispatchKindInit(&effect->head.sub, packetIndex);
                completedCount++;
                if (completedCount >= packetCount) {
                    effect->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

/* Stagger native tags using this variant's second tail word as the group period. */
void effInitExpandRingPacketSchedule(EffTemplatePacketList *effect) {
    EffectBufferRecord *packetCursor;
    u32 packetIndex;
    u32 nextPacketIndex;
    s32 packetTag;

    packetTag = EFF_PACKET_INITIAL_TAG;
    packetIndex = 0;
    packetCursor = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            packetCursor->unk20 = packetTag;
            nextPacketIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
            if (nextPacketIndex % effect->tailWords[1] == 0) {
                packetTag = packetTag - effect->decayStep;
            }
            packetIndex = nextPacketIndex;
        } while (packetIndex < effect->packetCount);
    }
}

void effScaleExpandRingTemplate(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->recordScale = effect->recordScale * scale;
}

/* Clone the expanding-ring prefix/tail and initialize its period-grouped packet tags. */
s32 billCloneTemplateSmall(EffTemplatePacketList *source) {
    s32 clone = (s32)sdfAllocSizeClassBlock(0x170);
    s32 tailBytes = 0x20;

    memset((void *)clone, 0, 0x170);
    memcpy((void *)clone, source, source->templateSize);
    memcpy((void *)(clone + EFF_TEMPLATE_TAIL_OFFSET), (u8 *)source + source->templateSize, tailBytes);
    func_0015B330(clone);
    effInitExpandRingPacketSchedule(clone);
    return clone;
}

void effFreeExpandRingTemplate(u32 effect) {
    effDestroyResources((EffEmitterHead *)effect);
    sdfReleaseChipBlock(effect);
}

typedef struct EffExpandingRingEmitter {
    EffEmitterHead head;
    u8 pad150;
    u8 loop;         /* 0x151 */
    u8 pad152[6];
    f32 radius;      /* 0x158 */
    f32 jitter;      /* 0x15C */
    u32 period;      /* 0x160 */
    f32 spinRate;    /* 0x164 */
} EffExpandingRingEmitter;

/* Seed radius and ring direction, placing the packet at its rotated vertical offset. */
void effEmitterExpandRingSpawn(EffExpandingRingEmitter *effect, u32 packetIndex) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    f32 offsetVector[4];
    f32 jitter;
    f32 radius;
    f32 angle;

    packet += packetIndex;
    packet->age = -1;
    packet->color = 0;
    jitter = effect->jitter;
    radius = effect->radius * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter));
    packet->f30 = radius;
    angle = (3.14159265f * 2.0f) / effect->period * (packetIndex % effect->period);
    packet->vel[0] = sdfEvaluateCosineViaSinePhaseShift(angle);
    packet->vel[1] = 0;
    packet->vel[2] = sdfSinPoly(angle);
    VU0_LOAD_MATRIX(effect->head.matrix);
    offsetVector[1] = radius;
    offsetVector[0] = 0;
    offsetVector[2] = 0;
    VU0_LOAD_VF(vf10, offsetVector);
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
    parDispatchKindInit(&effect->head.sub, packetIndex);
}

/* Build age-phase ring positions and rotate the stored X/Z directions.
 * vector is reused for the position offset and old direction components. */
void effEmitterExpandRingUpdate(EffExpandingRingEmitter *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 subeffectKind = effect->head.sub.kind;
    f32 origin[4];
    f32 vector[4];
    f32 rotationStep;
    f32 rotationCos;
    f32 rotationSin;
    s32 completedCount;
    s32 repeatEnabled;
    s32 packetCount;
    s32 lifetimeFrames;
    s32 packetIndex;

    parUpdateSharedScaleAndDelta(&effect->head.sub);
    VU0_LOAD_MATRIX(effect->head.matrix);
    rotationStep = effect->spinRate * (3.14159265f / 180.0f);
    completedCount = 0;
    packetCount = effect->head.packetCount;
    lifetimeFrames = effect->head.frameCount;
    repeatEnabled = effect->loop;
    rotationCos = sdfEvaluateCosineViaSinePhaseShift(rotationStep);
    rotationSin = sdfSinPoly(rotationStep);
    PCP_COPY_VECTOR(origin, effect->head.origin);
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++) {
        s32 age = packet->age;

        if (age == EFF_PACKET_INITIAL_TAG) {
            effEmitterExpandRingSpawn(effect, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            f32 angle;
            f32 radius;
            f32 phaseCos;
            f32 phaseSin;

            packet->color = func_001616A8(effect->head.fade, packet->color, age);
            packet->color = effParModulateColors(packet->color, effect->head.colorMask);
            if (subeffectKind != 0) {
                VU0_LOAD_VF(vf12, packet);
            }
            angle = (f32)age / (f32)lifetimeFrames * 3.14159265f;
            radius = packet->f30;
            phaseCos = sdfEvaluateCosineViaSinePhaseShift(angle);
            phaseSin = sdfSinPoly(angle);
            vector[0] = packet->vel[0] * (radius * phaseSin);
            vector[1] = radius * phaseCos;
            vector[2] = packet->vel[2] * (radius * phaseSin);
            VU0_LOAD_VF(vf10, vector);
            VU0_APPLY_MATRIX(vf10, vf10);
            VU0_LOAD_VF(vf11, origin);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, packet);
            vector[0] = packet->vel[0];
            vector[2] = packet->vel[2];
            packet->vel[0] = rotationCos * vector[0] + rotationSin * vector[2];
            packet->vel[2] = rotationCos * vector[2] - rotationSin * vector[0];
            if (subeffectKind != 0) {
                VU0_LOAD_VF(vf10, packet);
                parDispatchKindUpdate(&effect->head.sub, packetIndex, packet->color, packet->speed);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                parDispatchKindInit(&effect->head.sub, packetIndex);
                completedCount++;
                if (completedCount >= packetCount) {
                    effect->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

/* Initialize only the native age/tag word of each packet record. */
void effResetConePacketAges(EffTemplatePacketList *effect) {
    EffectBufferRecord *packetCursor;
    u32 packetIndex;

    packetIndex = 0;
    packetCursor = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            packetCursor->unk20 = EFF_PACKET_INITIAL_TAG;
            packetIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
        } while (packetIndex < effect->packetCount);
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

/* Clone the cone prefix/tail, allocate packet records, and initialize their native age tags. */
s32 effCloneConeTemplate(EffTemplatePacketList *source) {
    s32 clone = (s32)sdfAllocSizeClassBlock(0x180);
    s32 tailBytes = 0x30;

    memset((void *)clone, 0, 0x180);
    memcpy((void *)clone, source, source->templateSize);
    memcpy((void *)(clone + EFF_TEMPLATE_TAIL_OFFSET), (u8 *)source + source->templateSize, tailBytes);
    func_0015B330(clone);
    effResetConePacketAges(clone);
    return clone;
}

void effFreeConeTemplate(u32 effect) {
    effDestroyResources((EffEmitterHead *)effect);
    sdfReleaseChipBlock(effect);
}

typedef struct EffConeEmitter {
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
} EffConeEmitter;

/* Seed mode-zero cone position/velocity; every mode initializes age and subeffect. */
void effEmitterConeSpawn(EffConeEmitter *effect, s32 packetIndex) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    f32 gravity;
    f32 sweepRadians;
    f32 angle;
    f32 radius;
    f32 cone;
    f32 speed;
    f32 angleCos;
    f32 angleSin;
    f32 jitter;

    packet += packetIndex;
    gravity = effect->gravity / 100.0f;
    sweepRadians = effect->degrees * (3.14159265f / 180.0f);
    radius = effect->radius;
    cone = effect->cone;
    speed = effect->speed;
    if (effect->mode == 0) {
        angle = sweepRadians / (u32)effect->head.packetCount * packetIndex;
        angleCos = sdfEvaluateCosineViaSinePhaseShift(angle);
        angleSin = sdfSinPoly(angle);
        packet->pos[0] = angleCos * radius;
        packet->pos[1] = 0;
        packet->pos[2] = angleSin * radius;
        VU0_LOAD_MATRIX(effect->head.matrix);
        VU0_LOAD_VF(vf10, packet->pos);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, packet->pos);
        packet->pos[0] += effect->head.origin[0];
        packet->pos[1] += effect->head.origin[1];
        packet->pos[2] += effect->head.origin[2];
        packet->vel[0] = angleCos * cone * speed;
        packet->vel[1] = -speed * (1.0f - cone);
        packet->vel[2] = angleSin * cone * speed;
        packet->f38 = gravity;
    }
    packet->age = -(effMiscRand(effEmitterDelayRandomState) % (effect->spread + 1));
    packet->color = 0;
    jitter = effect->head.speedJitter;
    packet->speed = effect->head.speed * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter));
    jitter = effect->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    parDispatchKindInit(&effect->head.sub, packetIndex);
}

/* Scale velocity, apply stored gravity, and transform each motion step before aging. */
void effEmitterConeUpdate(EffConeEmitter *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 subeffectKind = effect->head.sub.kind;
    f32 positionDelta[4];
    f32 velocityScale;
    s32 completedCount;
    s32 lifetimeFrames;
    s32 packetCount;
    s32 repeatEnabled;
    s32 packetIndex;

    parUpdateSharedScaleAndDelta(&effect->head.sub);
    VU0_LOAD_MATRIX(effect->head.matrix);
    velocityScale = effect->decayPct / 100.0f + 1.0f;
    completedCount = 0;
    lifetimeFrames = effect->head.frameCount;
    packetCount = effect->head.packetCount;
    repeatEnabled = effect->loop;
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++) {
        s32 age = packet->age;

        if (age == EFF_PACKET_INITIAL_TAG) {
            effEmitterConeSpawn(effect, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            packet->color = func_001616A8(effect->head.fade, packet->color, age);
            packet->color = effParModulateColors(packet->color, effect->head.colorMask);
            if (subeffectKind != 0) {
                VU0_LOAD_VF(vf12, packet);
            }
            packet->vel[0] *= velocityScale;
            packet->vel[1] = packet->vel[1] * velocityScale + packet->f38;
            packet->vel[2] *= velocityScale;
            VU0_LOAD_VF(vf10, packet->vel);
            VU0_APPLY_MATRIX(vf10, vf10);
            VU0_STORE_VF(vf10, positionDelta);
            packet->pos[0] += positionDelta[0];
            packet->pos[1] += positionDelta[1];
            packet->pos[2] += positionDelta[2];
            if (subeffectKind != 0) {
                VU0_LOAD_VF(vf10, packet);
                parDispatchKindUpdate(&effect->head.sub, packetIndex, packet->color, packet->speed);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                parDispatchKindInit(&effect->head.sub, packetIndex);
                completedCount++;
                if (completedCount >= packetCount) {
                    effect->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

/* Initialize each packet's native tag and its template-selected uniform scale. */
void effApplyTemplateScaleToRecords(EffTemplatePacketList *effect) {
    s32 packetIndex = 0;
    EffScaledRecord *packetCursor = (EffScaledRecord *)effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            packetCursor->flags = EFF_PACKET_INITIAL_TAG;
            packetCursor->scale = effect->recordScale;
            packetIndex++;
            packetCursor++;
        } while ((u32)packetIndex < effect->packetCount);
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

/* Clone the prefix/tail and initialize each packet record with the template's uniform scale. */
s32 effClonePacketRecordScaleTemplate(EffTemplatePacketList *source) {
    s32 clone = (s32)sdfAllocSizeClassBlock(0x170);
    s32 tailBytes = 0x20;

    memset((void *)clone, 0, 0x170);
    memcpy((void *)clone, source, source->templateSize);
    memcpy((void *)(clone + EFF_TEMPLATE_TAIL_OFFSET), (u8 *)source + source->templateSize, tailBytes);
    func_0015B330(clone);
    effApplyTemplateScaleToRecords(clone);
    return clone;
}

void effFreePacketRecordScaleTemplate(u32 effect) {
    effDestroyResources((EffEmitterHead *)effect);
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

/* Clone the prefix without tail bytes; the zero-length copy is retained for matching. */
void *effCloneSingleParticleTemplate(EffTemplatePacketList *source) {
    EffTemplatePacketList *clone = sdfAllocSizeClassBlock(0x150);
    s32 tailBytes = 0;

    memset(clone, 0, 0x150);
    memcpy(clone, source, source->templateSize);
    memcpy((u8 *)clone + EFF_TEMPLATE_TAIL_OFFSET, (u8 *)source + source->templateSize, tailBytes);
    clone->packetCount = 1;
    clone->packetTag = 0;
    func_0015B330((s32)clone);
    return clone;
}

void effFreeSingleParticleTemplate(u32 effect) {
    effDestroyResources((EffEmitterHead *)effect);
    sdfReleaseChipBlock(effect);
}

extern u8 D_003AA868[];
extern f32 effMiscRandUnitFloat(void *);

/* Initialize the first record at frame -1; speed draws RNG, angle draws it only for nonzero jitter. */
void effInitParticleRecord(effect)
    EffParticle *effect;
{
    EffParticleRecord *record = (EffParticleRecord *)effect->buffer->records;
    f32 jitter;
    u32 baseColor;

    baseColor = effect->color | (effect->alpha << 24);
    record->x = effect->x;
    record->frame = -1;
    record->y = effect->y;
    record->color = baseColor;
    record->z = effect->z;
    record->speed = 1.0f;
    record->angle = 0;
    effect->lastSpeed = effect->speed;
    jitter = effect->speedJitter;
    record->speed = effect->speed * (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter));
    jitter = effect->angleJitter;
    if (jitter != 0) {
        record->angle = (effMiscRandUnitFloat(D_003AA868) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        record->angle = 0;
    }
}

/* Frame zero reinitializes to -1 before incrementing; refresh position and modulated packed color. */
void effUpdateParticleRecord(EffParticle *effect) {
    EffParticleRecord *record = (EffParticleRecord *)effect->buffer->records;
    u32 baseColor;

    if (record->frame == 0) {
        effInitParticleRecord(effect);
    }
    record->frame = record->frame + 1;
    record->x = effect->x;
    record->y = effect->y;
    baseColor = effect->color | (effect->alpha << 24);
    effect->unk24 = record->frame + 1;
    record->color = baseColor;
    record->z = effect->z;
    record->color = effParModulateColors(baseColor, effect->unkF0);
}

/* Initialize only the native age/tag word of each packet record. */
void effResetOffsetGravityPacketAges(EffTemplatePacketList *effect) {
    EffectBufferRecord *packetCursor;
    u32 packetIndex;

    packetIndex = 0;
    packetCursor = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            packetCursor->unk20 = EFF_PACKET_INITIAL_TAG;
            packetIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
        } while (packetIndex < effect->packetCount);
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

/* Clone the offset-gravity prefix/tail, allocate records, and initialize their native age tags. */
s32 effCloneOffsetGravityTemplate(EffTemplatePacketList *source) {
    s32 clone = (s32)sdfAllocSizeClassBlock(0x190);
    s32 tailBytes = 0x40;

    memset((void *)clone, 0, 0x190);
    memcpy((void *)clone, source, source->templateSize);
    memcpy((void *)(clone + EFF_TEMPLATE_TAIL_OFFSET), (u8 *)source + source->templateSize, tailBytes);
    func_0015B330(clone);
    effResetOffsetGravityPacketAges(clone);
    return clone;
}

void effFreeOffsetGravityTemplate(u32 effect) {
    effDestroyResources((EffEmitterHead *)effect);
    sdfReleaseChipBlock(effect);
}

typedef struct EffOffsetGravityEmitter {
    EffEmitterHead head;
    u8 mode;         /* 0x150 */
    u8 loop;         /* 0x151 */
    u8 pad152[0x1A];
    f32 decayPct;    /* 0x16C */
} EffOffsetGravityEmitter;

void func_0015F918(EffOffsetGravityEmitter *effect, s32 index);
INCLUDE_ASM(const s32, "game/code_00159B48", func_0015F918);

/* Accumulate motion in the stored offset, then add the emitter origin.
 * Only mode zero transforms the per-frame displacement through the matrix. */
void effEmitterOffsetGravityUpdate(EffOffsetGravityEmitter *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 subeffectKind = effect->head.sub.kind;
    u32 mode;
    f32 positionDelta[4];
    f32 origin[4];
    f32 velocityScale;
    s32 completedCount;
    s32 lifetimeFrames;
    s32 packetCount;
    s32 repeatEnabled;
    s32 packetIndex;

    parUpdateSharedScaleAndDelta(&effect->head.sub);
    mode = effect->mode;
    if (mode == 0) {
        VU0_LOAD_MATRIX(effect->head.matrix);
    }
    velocityScale = effect->decayPct / 100.0f + 1.0f;
    completedCount = 0;
    packetCount = effect->head.packetCount;
    lifetimeFrames = effect->head.frameCount;
    repeatEnabled = effect->loop;
    PCP_COPY_VECTOR(origin, effect->head.origin);
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++) {
        s32 age = packet->age;

        if (age == EFF_PACKET_INITIAL_TAG) {
            func_0015F918(effect, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            packet->color = func_001616A8(effect->head.fade, packet->color, age);
            packet->color = effParModulateColors(packet->color, effect->head.colorMask);
            if (subeffectKind != 0) {
                VU0_LOAD_VF(vf12, packet);
            }
            packet->vel[1] = packet->vel[1] * velocityScale + packet->f3C;
            packet->vel[0] *= velocityScale;
            packet->vel[2] *= velocityScale;
            VU0_LOAD_VF(vf10, packet->vel);
            if (mode == 0) {
                VU0_APPLY_MATRIX(vf10, vf10);
            }
            VU0_STORE_VF(vf10, positionDelta);
            packet->f30 += positionDelta[0];
            packet->f34 += positionDelta[1];
            packet->f38 += positionDelta[2];
            VU0_LOAD_VF(vf10, &packet->f30);
            VU0_LOAD_VF(vf11, origin);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, packet);
            if (subeffectKind != 0) {
                VU0_LOAD_VF(vf10, packet);
                parDispatchKindUpdate(&effect->head.sub, packetIndex, packet->color, packet->speed);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                parDispatchKindInit(&effect->head.sub, packetIndex);
                completedCount++;
                if (completedCount >= packetCount) {
                    effect->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

/* Initialize only the native age/tag word of each packet record. */
void effResetDiscAuxPacketAges(EffTemplatePacketList *effect) {
    EffectBufferRecord *packetCursor;
    u32 packetIndex;

    packetIndex = 0;
    packetCursor = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            packetCursor->unk20 = EFF_PACKET_INITIAL_TAG;
            packetIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
        } while (packetIndex < effect->packetCount);
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

/* Clone the disc prefix/tail plus a separate 16-byte-per-packet auxiliary allocation. */
void *effCloneDiscAuxTemplate(EffTemplatePacketList *source) {
    EffTemplatePacketList *clone = sdfAllocSizeClassBlock(0x200);
    s32 auxAllocationHandle;
    s32 tailBytes = 0xB0;

    memset(clone, 0, 0x200);
    memcpy(clone, source, source->templateSize);
    memcpy((u8 *)clone + EFF_TEMPLATE_TAIL_OFFSET, (u8 *)source + source->templateSize, tailBytes);
    auxAllocationHandle = sdfAllocGeneralBlock(clone->packetCount * 0x10);
    clone->auxiliaryAllocation = auxAllocationHandle;
    clone->auxiliaryData = sdfResourceRetainAddress(auxAllocationHandle);
    func_0015B330((s32)clone);
    effResetDiscAuxPacketAges(clone);
    return clone;
}

void effFreeDiscAuxTemplate(u32 effect) {
    sdfReleaseResourceAllocation(((EffTemplatePacketList *)effect)->auxiliaryAllocation);
    effDestroyResources(effect);
    sdfReleaseChipBlock(effect);
}

typedef struct EffDiscAuxEmitter {
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
} EffDiscAuxEmitter;

/* Seed the auxiliary position and packet direction separately, then the subeffect. */
void effEmitterDiscAuxSpawn(EffDiscAuxEmitter *effect, u32 packetIndex) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    f32 (*auxiliaryCursor)[4] = effect->aux;
    f32 directionVector[4];
    f32 radialDistance;
    f32 jitter;

    packet += packetIndex;
    auxiliaryCursor += packetIndex;
    radialDistance = effect->radius * ((effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f);
    directionVector[0] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    directionVector[1] = 0;
    directionVector[2] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, directionVector);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, directionVector);
    (*auxiliaryCursor)[0] = directionVector[0] * radialDistance;
    (*auxiliaryCursor)[1] = 0;
    (*auxiliaryCursor)[2] = directionVector[2] * radialDistance;
    VU0_LOAD_MATRIX(effect->head.matrix);
    VU0_LOAD_VF(vf10, *auxiliaryCursor);
    VU0_APPLY_MATRIX(vf10, vf10);
    VU0_STORE_VF(vf10, *auxiliaryCursor);
    directionVector[0] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    directionVector[1] = 0;
    directionVector[2] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, directionVector);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, packet->vel);
    packet->f30 = effect->f164 * (effMiscRandUnitFloat(D_003AA868) * effect->f174 + (1.0f - effect->f174));
    packet->f34 = 0;
    packet->f38 = 0;
    packet->f3C = effect->f15C * (effMiscRandUnitFloat(D_003AA868) * effect->f170 + (1.0f - effect->f170));
    packet->age = -(effMiscRand(effEmitterDelayRandomState) % (effect->spread + 1));
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
    parDispatchKindInit(&effect->head.sub, packetIndex);
}

/* Advance auxiliary positions, then transform/store packet positions with origin.
 * Auxiliary accumulation remains separate from the transformed packet position. */
void effEmitterDiscAuxUpdate(EffDiscAuxEmitter *effect) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 subeffectKind = effect->head.sub.kind;
    f32 (*auxiliaryCursor)[4] = effect->aux;
    f32 positionVector[4];
    f32 origin[4];
    f32 verticalScale;
    f32 phaseStep;
    f32 waveRate;
    s32 completedCount;
    s32 lifetimeFrames;
    s32 packetCount;
    s32 packetIndex;
    u32 repeatEnabled;

    parUpdateSharedScaleAndDelta(&effect->head.sub);
    VU0_LOAD_MATRIX(effect->head.matrix);
    phaseStep = effect->f160 * (3.14159265f / 180.0f);
    verticalScale = effect->f16C / 100.0f + 1.0f;
    completedCount = 0;
    packetCount = effect->head.packetCount;
    lifetimeFrames = effect->head.frameCount;
    repeatEnabled = effect->flag150;
    waveRate = effect->f168;
    PCP_COPY_VECTOR(origin, effect->head.origin);
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++, auxiliaryCursor++) {
        s32 age = packet->age;

        if (age == EFF_PACKET_INITIAL_TAG) {
            effEmitterDiscAuxSpawn(effect, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            f32 verticalStep;
            f32 previousWaveValue;
            f32 factor;

            packet->color = func_001616A8(effect->head.fade, packet->color, age);
            packet->color = effParModulateColors(packet->color, effect->head.colorMask);
            if (subeffectKind != 0) {
                VU0_LOAD_VF(vf12, packet);
            }
            packet->f30 += waveRate;
            verticalStep = packet->f3C;
            packet->f34 += phaseStep;
            factor = sdfSinPoly(packet->f34);
            previousWaveValue = packet->f38;
            packet->f38 = factor;
            factor = (factor - previousWaveValue) * packet->f30;
            positionVector[0] = (*auxiliaryCursor)[0] + packet->vel[0] * factor;
            positionVector[1] = (*auxiliaryCursor)[1] + verticalStep;
            positionVector[2] = (*auxiliaryCursor)[2] + packet->vel[2] * factor;
            VU0_LOAD_VF(vf10, positionVector);
            VU0_STORE_VF(vf10, *auxiliaryCursor);
            VU0_APPLY_MATRIX(vf10, vf10);
            VU0_LOAD_VF(vf11, origin);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, packet);
            packet->f3C = verticalStep * verticalScale;
            if (subeffectKind != 0) {
                VU0_LOAD_VF(vf10, packet);
                parDispatchKindUpdate(&effect->head.sub, packetIndex, packet->color, packet->speed);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                parDispatchKindInit(&effect->head.sub, packetIndex);
                completedCount++;
                if (completedCount >= packetCount) {
                    effect->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

/* Initialize only the native age/tag word of each packet record. */
void effMarkAllTemplateBufferRecords(EffTemplatePacketList *effect) {
    EffectBufferRecord *packetCursor;
    u32 packetIndex;

    packetIndex = 0;
    packetCursor = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            packetCursor->unk20 = EFF_PACKET_INITIAL_TAG;
            packetIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
        } while (packetIndex < effect->packetCount);
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
    s32 copy = (s32)sdfAllocSizeClassBlock(0x190);
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
        ((EffTemplatePacketList *)copy)->listAllocation = sdfAllocGeneralBlock(listBytes + perRecord * count * 16);
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
