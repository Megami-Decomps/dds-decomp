#include "common.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"

extern void *sdfCreateAssetWithDrawEntries();
extern void func_002DA420(void *, f32);
extern void func_002DA3D8(void *, u32);
extern void func_002DA3C0(void *, u32);
extern void func_002DA3F0(void *, u32);
#include "eff.h"

typedef struct EffTemplatePacketList {
    u8 pad00[0x10];
    f32 x; /* 0x10 */
    f32 y; /* 0x14 */
    f32 z; /* 0x18 */
    u8 pad1C[4];
    u32 packetCount;
    s32 packetTag; /* 0x24: cleared when cloning a prefix */
    u8 pad28[8];
    u16 kind; /* 0x30: resource type */
    u8 pad32[2];
    u16 subrecordCount; /* 0x34: subrecords per packet */
    u8 pad36[0x6A];
    s32 templateSize; /* 0xA0: prefix copied before the appended tail */
    u8 padA4[0x54];
    EffectBufferTail *buffer;
    u8 padFC[0x58];
    s32 decayStep; /* 0x154: subtraction from each later packet tag */
    f32 recordScale;
    union {
        f32 tailValues[4];
        u32 tailWords[4];
    };
    s32 recordList; /* 0x16C: start of the three-word packet records */
    s32 recordsPerPacket; /* 0x170 */
    s32 listAllocation; /* 0x174 */
    void *auxiliaryData; /* 0x178: optional 16 bytes per packet */
    s32 auxiliaryAllocation; /* 0x17C */
} EffTemplatePacketList;

/* Particle record stored at the beginning of the effect's resource buffer. */
typedef struct EffParticleRecord {
    f32 x;
    f32 y;
    f32 z;
    u8 pad0C[0x14];
    s32 frame;   /* 0x20: starts at -1; read as a countdown and incremented */
    u32 color;   /* 0x24: packed RGBA built from the effect's colour and alpha */
    f32 speed;
    f32 angle;
    u8 pad30[0x10];
} EffParticleRecord;

extern u8 D_0034DF38[];
extern f32 effMiscRandUnitFloat(void *);

extern EffectConfig D_0034DF54[];

s32 billCreateIndexed(s32 kind, s32 index);

extern s32 D_003D6438[];

extern s32 effEmitterDelayRandomState[];

s32 sdfAllocPacketAligned(s32 size);

void sdfInitPacketList(s32 packet);

s32 func_00151398(s32 arg0, s32 arg1);

void func_00152E40(s32 packet, s32 source);

void func_00153128(s32 packet, s32 source);

void func_00153740(s32 effect);

void effSetTemplateTagPeriod(EffTemplatePacketList *effect);

extern void *memset(void *s, s32 c, u32 n);

extern void *memcpy(void *dest, const void *src, u32 n);

extern void *func_002CFEB8(s32 size);

s32 func_002D03F8(s32 size);

EffectBufferRecord *sdfResourceRetainAddress(s32 allocation);

void effInitExpandRingPacketSchedule(EffTemplatePacketList *effect);

typedef struct EffEmitterD EffEmitterD;

void effEmitterLookAtRingSpawn(EffEmitterD *effect, u32 index);

typedef struct EffResourceOwner {
    u8 pad00[0x30];
    s32 *resource; /* 0x30: reference-counted effect resource */
} EffResourceOwner;

/* Resource cleanup view; unlike the retain view above, +0x30 is a kind. */
typedef struct EffResourceSet {
    u8 pad00[0x30];
    u16 kind;
    u8 pad32[6];
    s32 unk38;
    u8 pad3C[4];
    s32 unk40;
    s32 unk44;
    u8 pad48[0xAC];
    s32 billboard;
    s32 buffer;
} EffResourceSet;

/* Attach the indexed effect resource to a new billboard and increment its reference count. */
void effRetainResource(s32 index) {
    s32 *effect = (s32 *)billCreateIndexed(D_0034DF54[index].unk00, 0);
    s32 *resource = ((EffResourceOwner *)D_003D6438[index])->resource;
    s32 references = resource[2];

    effect[12] = (s32)resource;
    resource[2] = references + 1;
}

u32 func_00151FC0(void) {
    return 0xf;
}

s32 effGetResourceFirstWord(s32 index) {
    return *(s32 *)((EffResourceOwner *)D_003D6438[index])->resource;
}

void effCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

/* Adjust the scalar used by an existing billboard instance. */
void billSetLengthExtent(BillObj *effect, float scale) {
    effect->lengthScale = scale;
}

/* Set the two child scale components stored at +0x10 and +0x14. */
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

/* The billboard entry points to frame data whose period wraps playback. */
typedef struct EffBillFrame {
    u8 pad00[0x0C];
    u32 period;
    u32 flags;
} EffBillFrame;

typedef struct EffBillEntry {
    u8 pad00[4];
    u32 frame;
    s32 mode;
    EffBillFrame *data;
    u8 pad10[4];
} EffBillEntry; /* 0x14 */

/* Each effect scene object owns a billboard and an asset reference. */
typedef struct EffUnitObject {
    u8 matrix[0x80];
    s32 billboard;  /* 0x80 */
    void *resource; /* 0x84 */
} EffUnitObject;

void billSetBillboardMode(BillObj *effect, s32 mode) {
    s32 count;
    s32 remaining;
    s32 entry;
    mode = (s16)mode;
    switch (effect->kind) {
    case 0:
    case 3:
        effect->unk2E = mode;
        break;
    case 1:
        count = effect->entryCount;
        if (count > 0) {
            remaining = count;
            entry = (s32)effect->unk60 + 0xc;
            do {
                s32 node = *(s32 *)entry;
                u32 flags = ((EffBillFrame *)node)->flags & ~6;
                ((EffBillFrame *)node)->flags = flags;
                if (mode == 2) {
                    ((EffBillFrame *)node)->flags = flags | 2;
                } else if (mode == 3) {
                    ((EffBillFrame *)node)->flags = flags | 4;
                }
                entry += 0x14;
            } while (--remaining != 0);
        }
        break;
    }
}

typedef struct BillEntryList {
    u8 pad00[0x10];
    s32 count;
    u8 pad14[4];
    s32 *entries;
} BillEntryList;

/* Kind-zero billboard payload stores its variant after a 32-bit value. */
typedef struct BillChildPayload {
    s32 value;
    union {
        s16 signedVariant;
        u16 variant;
    };
    u8 pad06[0x1E];
    f32 halfWidth;  /* 0x24 */
    f32 halfHeight; /* 0x28 */
} BillChildPayload;

typedef struct BillKindOneView {
    u8 pad00[0x2C];
    u16 kind;
    u8 pad2E[0x26];
    u32 flags; /* 0x54 */
} BillKindOneView;

/* Kind-one payload's +4 link leads to another +4 value word. */
typedef struct BillLinkedValue {
    s32 unk00;
    s32 value;
} BillLinkedValue;

typedef struct BillValueLink {
    s32 unk00;
    BillLinkedValue *target;
} BillValueLink;

void billSetAllChildVariants(BillObj *effect, s32 value) {
    s32 count;
    s32 *entries;
    s32 *entry;

    value = (s16)value;
    switch (effect->kind) {
    case 0:
        ((BillChildPayload *)effect->entryList)->signedVariant = value;
        break;
    case 1:
        count = ((BillEntryList *)effect->entryList)->count;
        entries = ((BillEntryList *)effect->entryList)->entries;

        if (count > 0) {
            entry = entries;
            do {
                ((BillChildPayload *)*entry)->signedVariant = value;
                entry++;
                count--;
            } while (count != 0);
        }
        break;
    }
}

s32 billGetChildValue(BillObj *effect) {
    if (effect->kind == 0) {
        return ((BillChildPayload *)effect->entryList)->value;
    }
    return 0;
}

u16 billGetKind(BillObj *effect) {
    return effect->kind;
}

void billSetVariantValue(BillObj *effect, s32 value) {
    s32 v = value & 0xffff;

    switch (effect->kind) {
    case 0:
        ((BillChildPayload *)effect->entryList)->signedVariant = v;
        break;
    case 1:
        effect->unk3C = v;
        break;
    }
}

u16 billGetVariantValue(BillObj *effect) {
    switch (effect->kind) {
    case 0:
        return ((BillChildPayload *)effect->entryList)->variant;
    case 1:
        return effect->unk3C;
    default:
        return 0;
    }
}

/* Replace the selected list entry only when its index changes. */
void billSetKind1Entry(BillObj *effect, u32 value) {
    if (effect->kind == 1 && effect->unk58 != value) {
        func_001518D8(effect, value);
    }
}

/* Read the selected entry for list billboards; other kinds have none. */
s32 billGetKindOneEntry(BillObj *effect) {
    if (effect->kind == 1) {
        return effect->unk58;
    }
    return 0;
}

s32 billGetLinkedChildValue(s32 billboard) {
    if (((BillObj *)billboard)->kind == 1) {
        return ((BillValueLink *)((BillObj *)billboard)->entryList)->target->value;
    }
    return 0;
}

/* Start every entry's animation at the requested frame, with mode zero. */
void billSetEntryFrameMode0(BillObj *effect, u32 time) {
    if (effect->kind == 1) {
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
    if (effect->kind == 1) {
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
    if (effect->kind == 1) {
        return ((EffBillEntry *)effect->unk60)->data->period;
    }
    return 0;
}

/* Read the kind-one billboard's halfword at +0x50. */
u16 billGetKindOneParameter(BillObj *effect) {
    if (effect->kind == 1) {
        return effect->unk50;
    }
    return 0;
}

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
        s32 tmp = (s32)((BillObj *)billboard)->entryList;
        ((BillChildPayload *)tmp)->halfWidth = width * 0.5f;
        ((BillChildPayload *)tmp)->halfHeight = height * 0.5f;
    }
}

extern u8 sdfViewMatrix[];
extern u8 D_00324610[];
extern u8 D_00324650[];
extern u8 D_00324660[];
extern u8 D_0034E080[];
extern void sdfPostmultiplyVuMatrixFromMemory(void *);
extern f32 sdfAtan2(f32, f32);

/* vu0 routine: computes the projected angle between two vectors */
f32 effComputeProjectedOffsetAngle(const void *position, const void *offset) {
    f32 delta[4];
    f32 projectedPosition[4];
    f32 projectedOffset[4];

    VU0_LOAD_MATRIX(sdfViewMatrix);
    sdfPostmultiplyVuMatrixFromMemory(D_00324610);
    VU0_LOAD_VF(vf10, position);
    VU0_MOVE_VF(vf12, vf10);
    VU0_TRANSFORM_POINT(vf10, vf10);
    VU0_PERSPECTIVE_DIVIDE_VF10();
    VU0_LOAD_VF(vf11, D_00324650);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_00324660);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, projectedPosition);

    VU0_LOAD_VF(vf10, offset);
    VU0_LOAD_VF(vf11, D_0034E080);
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf11, vf12);
    VU0_ADD(vf10, vf10, vf11);
    VU0_TRANSFORM_POINT(vf10, vf10);
    VU0_PERSPECTIVE_DIVIDE_VF10();
    VU0_LOAD_VF(vf11, D_00324650);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_00324660);
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

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152560);

u8 *billCreateUnitObject(s32 index) {
    u8 *obj = func_002CFEB8(0x88);

    ((EffUnitObject *)obj)->billboard = billCreateIndexed(1, index);
    ((EffUnitObject *)obj)->resource = sdfCreateAssetWithDrawEntries();
    func_002DA420(((EffUnitObject *)obj)->resource, 1.0f);
    EE_MMI_UNIT_MATRIX(obj + 0x40);
    return obj;
}

u8 *billCloneUnitObject(u8 *src) {
    u8 *obj = func_002CFEB8(0x88);

    ((EffUnitObject *)obj)->billboard = billCloneObjectRetainingSharedData(((EffUnitObject *)src)->billboard);
    ((EffUnitObject *)obj)->resource = sdfCreateAssetWithDrawEntries();
    func_002DA420(((EffUnitObject *)obj)->resource, 1.0f);
    func_002DA3D8(((EffUnitObject *)obj)->resource, 0x80808080);
    func_002DA3C0(((EffUnitObject *)obj)->resource, 0x80808080);
    func_002DA3F0(((EffUnitObject *)obj)->resource, 0x80808080);
    EE_MMI_UNIT_MATRIX(obj + 0x40);
    EE_MMI_UNIT_MATRIX(obj);
    return obj;
}

void effDestroy(u32 instance) {
    sdfQueueAssetRelease((u32)((EffUnitObject *)instance)->resource);
    billDispatchByKind(((EffUnitObject *)instance)->billboard);
    sdfReleaseChipBlock(instance);
}

void effSetInstanceBillboardVector(s32 instance, s128 *position) {
    effCopyVector((s128 *)((EffUnitObject *)instance)->billboard, position);
}

void billSetChildScale2(s32 instance, float scale) {
    billSetChildScaleComponents(((EffUnitObject *)instance)->billboard, scale, scale);
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effCopyMatrixToNext(u8 *dst, void *src) {
    VU0_LOAD_MATRIX(src);
    dst += 0x40;
    VU0_STORE_MATRIX(dst);
}

void billSetChildValue(s32 instance, u32 value) {
    billSetChildParameter(((EffUnitObject *)instance)->billboard, value);
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effVuCopyMatrix(void *dst, void *src) {
    VU0_COPY_MATRIX(dst, src);
}

void func_00152800(EffUnitObject *instance, s32 *values) {
    BillObj *billboard = (BillObj *)instance->billboard;

    if (billboard->kind == 1) {
        u32 modeFlags = *(u32 *)((u8 *)billboard + 0x54);

        if (modeFlags & 0x40) {
            values[0] = 2;
            values[2] = func_00151398((s32)billboard, (s32)billboard->unk60);
            values[1] = func_00151398((s32)billboard, (s32)billboard->unk60 + 0x14);
        } else if (modeFlags & 0x80) {
            values[0] = 3;
            values[2] = func_00151398((s32)billboard, (s32)billboard->unk60);
            values[1] = func_00151398((s32)billboard, (s32)billboard->unk60 + 0x14);
        } else {
            values[0] = 0;
            values[1] = func_00151398((s32)billboard, (s32)billboard->unk60);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_001528C0);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152E40);

/* Effect owner installs a callback at +0x10 to accept a new packet list. */
typedef struct EffPacketSink {
    u8 pad00[0x10];
    void (*submit)(s32 owner, s32 packet);
} EffPacketSink;

void func_001530C0(s32 sink, s32 source) {
    s32 tmp = sdfAllocPacketAligned(0x20);

    sdfInitPacketList(tmp);
    func_00152E40(tmp, source);
    ((EffPacketSink *)sink)->submit(sink, tmp);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00153128);

void func_00153618(s32 sink, s32 source) {
    s32 tmp = sdfAllocPacketAligned(0x20);

    sdfInitPacketList(tmp);
    func_00153128(tmp, source);
    ((EffPacketSink *)sink)->submit(sink, tmp);
}

void func_00153680(void) {
    func_002E84A0(&effEmitterDelayRandomState);
}

void func_001536A0(void) {
}

EffectBufferTail *effAllocateBuffer(s32 count) {
    s32 bytes = count * sizeof(EffectBufferRecord);
    s32 allocation = func_002D03F8(bytes + sizeof(EffectBufferTail));
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
    func_002D0918(*allocationSlot);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00153740);

void effDestroyResources(effect)
    s32 effect;

{
    EffResourceSet *owner = (EffResourceSet *)effect;
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
        func_00188480(owner->unk44);
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

/* Scale the position, record scale, and this variant's two tail floats. */
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
    s32 obj = (s32)func_002CFEB8(0x180);
    s32 tailLen = 0x30;

    memset((void *)obj, 0, 0x180);
    memcpy((void *)obj, source, source->templateSize);
    memcpy((void *)(obj + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_00153740(obj);
    effSetTemplateTagPeriod(obj);
    return obj;
}

void effFreeRingTemplate(u32 effect) {
    effDestroyResources();
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

extern f32 sdfSinPoly(f32);
extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);

void parDispatchKindInit(void *work, s32 index);

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
    packet->f34 = effect->radius * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter));
    jitter = effect->speedRange;
    packet->vel[0] = effect->f168 * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter));
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
    packet->speed = effect->head.speed * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter));
    jitter = effect->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    parDispatchKindInit(&effect->head.sub, index);
}

extern void parUpdateSharedScaleAndDelta(void *sub);
extern u32 func_00159AB8(void *fade, u32 color, s32 age);
extern u32 effParModulateColors(u32 color, u32 mask);
void parDispatchKindUpdate(void *work, s32 index, u32 color, f32 speed);

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

            packet->color = func_00159AB8(effect->head.fade, packet->color, age);
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
    u32 i;

    i = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            i = i + 1;
            record = record + 1;
        } while (i < effect->packetCount);
    }
}

/* Scale template position, record scale, and this variant's two tail values. */
void effScaleDiscTemplate(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
}

s32 effCloneTemplate(EffTemplatePacketList *source) {
    s32 copy = (s32)func_002CFEB8(0x180);
    s32 tailLen = 0x30;

    memset((void *)copy, 0, 0x180);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_00153740(copy);
    effResetDiscPacketAges(copy);
    return copy;
}

void effFreeDiscTemplate(u32 effect) {
    effDestroyResources();
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

extern u32 effMiscRand(void *state);

void effEmitterDiscSpawn(EffEmitterB *effect, u32 index) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    f32 tmp[4];
    f32 scale;
    f32 jitter;

    packet += index;
    scale = effect->radius * ((effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f);
    tmp[0] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    tmp[1] = 0;
    tmp[2] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
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
    tmp[0] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    tmp[1] = 0;
    tmp[2] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, tmp);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, packet->vel);
    packet->f30 = effect->f164 * (effMiscRandUnitFloat(D_0034DF38) * effect->jitterB + (1.0f - effect->jitterB));
    packet->f34 = 0;
    packet->f38 = 0;
    packet->f3C = effect->f15C * (effMiscRandUnitFloat(D_0034DF38) * effect->jitterA + (1.0f - effect->jitterA));
    packet->age = -(effMiscRand(effEmitterDelayRandomState) % (effect->spread + 1));
    packet->color = 0;
    jitter = effect->head.speedJitter;
    packet->speed = effect->head.speed * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter));
    jitter = effect->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
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

            packet->color = func_00159AB8(effect->head.fade, packet->color, age);
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
    u32 i;

    i = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            i = i + 1;
            record = record + 1;
        } while (i < effect->packetCount);
    }
}

/* Scale the position and three variant-specific tail values. */
void effScaleBallisticTemplate(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
    effect->tailValues[3] = effect->tailValues[3] * scale;
}

s32 effCloneBallisticTemplate(EffTemplatePacketList *source) {
    s32 copy = (s32)func_002CFEB8(0x180);
    s32 tailLen = 0x30;

    memset((void *)copy, 0, 0x180);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_00153740(copy);
    effResetBallisticPacketAges(copy);
    return copy;
}

void effFreeBallisticTemplate(u32 effect) {
    effDestroyResources();
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

void func_001547D8(EffEmitterC *effect, s32 index);
INCLUDE_ASM(const s32, "game/code_00151F58", func_001547D8);

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
            func_001547D8(effect, i);
            age = packet->age;
        }
        if (age >= 0) {
            packet->color = func_00159AB8(effect->head.fade, packet->color, age);
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

/* Scale position and the third variant-specific tail value. */
void effScaleLookAtRingTemplate(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
}

s32 effCloneLookAtRingTemplate(EffTemplatePacketList *source) {
    s32 copy = (s32)func_002CFEB8(0x170);
    s32 tailLen = 0x20;

    memset((void *)copy, 0, 0x170);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_00153740(copy);
    effInitLookAtRingPacketSchedule(copy);
    return copy;
}

void effFreeLookAtRingTemplate(u32 effect) {
    effDestroyResources();
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

extern u8 sdfViewEyeVector[];
extern u8 sdfViewTargetVector[];
extern u8 sdfViewUpVector[];
extern void sdfVuBuildLookAtBasis(void *, void *, void *);
extern void sdfInvertRigidVuTransform(void);

void effEmitterLookAtRingSpawn(EffEmitterD *effect, u32 index) {
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    u32 period = 0;
    f32 angle;
    f32 scale;
    f32 jitter;

    packet += index;
    sdfVuBuildLookAtBasis(sdfViewEyeVector, sdfViewTargetVector, sdfViewUpVector);
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
    packet->speed = effect->head.speed * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter));
    jitter = effect->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
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
    sdfVuBuildLookAtBasis(sdfViewEyeVector, sdfViewTargetVector, sdfViewUpVector);
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
            packet->color = func_00159AB8(effect->head.fade, packet->color, age);
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
    u32 i;

    i = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            i = i + 1;
            record = record + 1;
        } while (i < effect->packetCount);
    }
}

/* Scale position, record scale, and the second tail value. */
void effScaleBurstTemplate(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[1] = effect->tailValues[1] * scale;
}

s32 effCloneBurstTemplate(EffTemplatePacketList *source) {
    s32 copy = (s32)func_002CFEB8(0x170);
    s32 tailLen = 0x20;

    memset((void *)copy, 0, 0x170);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_00153740(copy);
    effResetBurstPacketAges(copy);
    return copy;
}

void effFreeBurstTemplate(u32 effect) {
    effDestroyResources();
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
    packet->age = -(effMiscRand(effEmitterDelayRandomState) % (effect->spread + 1));
    speed = effect->speed;
    jitter = effect->jitterA;
    tmp[0] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    tmp[1] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    tmp[2] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, tmp);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, tmp);
    packet->vel[0] = speed * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter)) * tmp[0];
    packet->vel[1] = speed * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter)) * tmp[1];
    packet->vel[2] = speed * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter)) * tmp[2];
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
        tmp[0] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
        tmp[1] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
        tmp[2] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
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
    packet->f3C = (effect->f160 * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter)) - length) / effect->head.frameCount;
    jitter = effect->head.speedJitter;
    packet->speed = effect->head.speed * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter));
    jitter = effect->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
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
            packet->color = func_00159AB8(effect->head.fade, packet->color, age);
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
    u32 i;

    i = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            i = i + 1;
            record = record + 1;
        } while (i < effect->packetCount);
    }
}

/* Scale template position, record scale, and this variant's tail values. */
void effScaleSphereTemplate(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
}

s32 effCloneSphereTemplate(EffTemplatePacketList *source) {
    s32 copy = (s32)func_002CFEB8(0x180);
    s32 tailLen = 0x30;

    memset((void *)copy, 0, 0x180);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_00153740(copy);
    effResetSpherePacketAges(copy);
    return copy;
}

void effFreeSphereTemplate(u32 effect) {
    effDestroyResources();
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
    tmp[0] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    tmp[1] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    tmp[2] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, tmp);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, tmp);
    packet->pos[0] = radius * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter)) * tmp[0];
    packet->pos[1] = radius * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter)) * tmp[1];
    packet->pos[2] = radius * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter)) * tmp[2];
    VU0_LOAD_MATRIX(effect->head.matrix);
    VU0_LOAD_VF(vf10, packet->pos);
    VU0_APPLY_MATRIX(vf10, vf10);
    VU0_STORE_VF(vf10, packet->pos);
    packet->pos[0] += effect->head.origin[0];
    packet->pos[1] += effect->head.origin[1];
    packet->pos[2] += effect->head.origin[2];
    packet->vel[0] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    packet->vel[1] = 0;
    packet->vel[2] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, packet->vel);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, packet->vel);
    packet->f30 = effect->f164 * (effMiscRandUnitFloat(D_0034DF38) * effect->f174 + (1.0f - effect->f174));
    packet->f34 = 0;
    packet->f38 = 0;
    packet->f3C = effect->f15C * (effMiscRandUnitFloat(D_0034DF38) * effect->f170 + (1.0f - effect->f170));
    packet->age = ~(effMiscRand(effEmitterDelayRandomState) % (effect->spread + 1));
    packet->color = 0;
    jitter = effect->head.speedJitter;
    packet->speed = effect->head.speed * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter));
    jitter = effect->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
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

            packet->color = func_00159AB8(effect->head.fade, packet->color, age);
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

/* Scale the position and record scale of a short template. */
void effScaleExpandRingTemplate(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->recordScale = effect->recordScale * scale;
}

s32 billCloneTemplateSmall(EffTemplatePacketList *source) {
    s32 copy = (s32)func_002CFEB8(0x170);
    s32 tailLen = 0x20;

    memset((void *)copy, 0, 0x170);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_00153740(copy);
    effInitExpandRingPacketSchedule(copy);
    return copy;
}

void effFreeExpandRingTemplate(u32 effect) {
    effDestroyResources();
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
    radius = effect->radius * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter));
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
    packet->speed = effect->head.speed * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter));
    jitter = effect->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
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

            packet->color = func_00159AB8(effect->head.fade, packet->color, age);
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
    u32 i;

    i = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            i = i + 1;
            record = record + 1;
        } while (i < effect->packetCount);
    }
}

/* Scale template position and three variant-specific tail values. */
void effScaleConeTemplate(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
    effect->tailValues[3] = effect->tailValues[3] * scale;
}

s32 effCloneConeTemplate(EffTemplatePacketList *source) {
    s32 copy = (s32)func_002CFEB8(0x180);
    s32 tailLen = 0x30;

    memset((void *)copy, 0, 0x180);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_00153740(copy);
    effResetConePacketAges(copy);
    return copy;
}

void effFreeConeTemplate(u32 effect) {
    effDestroyResources();
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
    packet->age = -(effMiscRand(effEmitterDelayRandomState) % (effect->spread + 1));
    packet->color = 0;
    jitter = effect->head.speedJitter;
    packet->speed = effect->head.speed * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter));
    jitter = effect->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
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
            packet->color = func_00159AB8(effect->head.fade, packet->color, age);
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

/* Same 0x40-byte per-packet scale view as the DDS2 effect twin. */
typedef struct EffScaledRecord {
    u8 pad00[0x20];
    u32 flags; /* 0x20 */
    u8 pad24[0x10];
    f32 scale; /* 0x34 */
    u8 pad38[8];
} EffScaledRecord;

void effApplyTemplateScaleToRecords(EffTemplatePacketList *effect) {
    s32 i = 0;
    EffectBufferRecord *record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            ((EffScaledRecord *)record)->scale = effect->recordScale;
            i++;
            record++;
        } while ((u32)i < effect->packetCount);
    }
}

/* Scale the position, record scale, and first two tail values. */
void effScalePacketRecordTemplate(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[1] = effect->tailValues[1] * scale;
}

s32 effClonePacketRecordScaleTemplate(EffTemplatePacketList *source) {
    s32 copy = (s32)func_002CFEB8(0x170);
    s32 tailLen = 0x20;

    memset((void *)copy, 0, 0x170);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_00153740(copy);
    effApplyTemplateScaleToRecords(copy);
    return copy;
}

void effFreePacketRecordScaleTemplate(u32 effect) {
    effDestroyResources();
    sdfReleaseChipBlock(effect);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00157280);

INCLUDE_ASM(const s32, "game/code_00151F58", func_001575D0);

void func_00157970(void) {
    effInitParticleRecord();
}

/* Apply a uniform scale to the particle template's position. */
void effScaleSingleParticleTemplate(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
}

/* Clone the prefix without tail bytes; the zero-length copy is retained for matching. */
s32 effCloneSingleParticleTemplate(EffTemplatePacketList *source) {
    s32 copy = (s32)func_002CFEB8(0x150);
    s32 tailLen = 0;

    memset((void *)copy, 0, 0x150);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    ((EffTemplatePacketList *)copy)->packetCount = 1;
    ((EffTemplatePacketList *)copy)->packetTag = 0;
    func_00153740(copy);
    return copy;
}

void effFreeSingleParticleTemplate(u32 effect) {
    effDestroyResources();
    sdfReleaseChipBlock(effect);
}

typedef struct EffParticle {
    f32 x;
    f32 y;
    f32 z;
    u8 pad0C[4];
    f32 speed;
    u8 pad14[0x10];
    u32 unk24; /* 0x24 */
    u8 pad28[0x24];
    u32 color; /* 0x4C */
    u8 pad50[8];
    u32 alpha; /* 0x58 */
    u8 pad5C[0x30];
    f32 lastSpeed; /* 0x8C */
    u8 pad90[4];
    f32 speedJitter; /* 0x94 */
    f32 angleJitter; /* 0x98 */
    u8 pad9C[0x54];
    u32 unkF0; /* 0xF0 */
    u8 padF4[4];
    EffectBufferTail *buffer; /* 0xF8 */
} EffParticle;



/* Initialize a particle record, applying random speed and angle jitter. */
void effInitParticleRecord(effect)
    EffParticle *effect;
{
    EffParticleRecord *particle = (EffParticleRecord *)effect->buffer->records;
    f32 jitter;
    u32 color;

    color = effect->color | (effect->alpha << 24);
    particle->x = effect->x;
    particle->frame = -1;
    particle->y = effect->y;
    particle->color = color;
    particle->z = effect->z;
    particle->speed = 1.0f;
    particle->angle = 0;
    effect->lastSpeed = effect->speed;
    jitter = effect->speedJitter;
    particle->speed = effect->speed * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter));
    jitter = effect->angleJitter;
    if (jitter != 0) {
        particle->angle = (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        particle->angle = 0;
    }
}

extern u32 effParModulateColors(u32, u32);

void effUpdateParticleRecord(EffParticle *effect) {
    EffParticleRecord *particle = (EffParticleRecord *)effect->buffer->records;
    s32 count = particle->frame;
    u32 color;

    if (count == 0) {
        effInitParticleRecord(effect);
        count = particle->frame;
    }
    particle->frame = count + 1;
    particle->x = effect->x;
    effect->unk24 = count + 2;
    particle->y = effect->y;
    color = effect->color | (effect->alpha << 24);
    particle->z = effect->z;
    particle->color = color;
    particle->color = effParModulateColors(color, effect->unkF0);
}

void effResetOffsetGravityPacketAges(EffTemplatePacketList *effect) {
    EffectBufferRecord *record;
    u32 i;

    i = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            i = i + 1;
            record = record + 1;
        } while (i < effect->packetCount);
    }
}

/* Scale the position and three variant-specific tail values. */
void effScaleOffsetGravityTemplate(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
    effect->tailValues[3] = effect->tailValues[3] * scale;
}

s32 effCloneOffsetGravityTemplate(EffTemplatePacketList *source) {
    s32 copy = (s32)func_002CFEB8(0x190);
    s32 tailLen = 0x40;

    memset((void *)copy, 0, 0x190);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_00153740(copy);
    effResetOffsetGravityPacketAges(copy);
    return copy;
}

void effFreeOffsetGravityTemplate(u32 effect) {
    effDestroyResources();
    sdfReleaseChipBlock(effect);
}

typedef struct EffEmitterJ {
    EffEmitterHead head;
    u8 mode;         /* 0x150 */
    u8 loop;         /* 0x151 */
    u8 pad152[0x1A];
    f32 decayPct;    /* 0x16C */
} EffEmitterJ;

void func_00157D28(EffEmitterJ *effect, s32 index);
INCLUDE_ASM(const s32, "game/code_00151F58", func_00157D28);

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
            func_00157D28(effect, i);
            age = packet->age;
        }
        if (age >= 0) {
            packet->color = func_00159AB8(effect->head.fade, packet->color, age);
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
    u32 i;

    i = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            i = i + 1;
            record = record + 1;
        } while (i < effect->packetCount);
    }
}

/* Scale position, record scale, and two tail values. */
void effScaleDiscAuxTemplate(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
}

s32 effCloneDiscAuxTemplate(EffTemplatePacketList *source) {
    s32 copy = (s32)func_002CFEB8(0x200);
    s32 tailLen = 0xB0;

    memset((void *)copy, 0, 0x200);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    ((EffTemplatePacketList *)copy)->auxiliaryAllocation =
        func_002D03F8(((EffTemplatePacketList *)copy)->packetCount << 4);
    ((EffTemplatePacketList *)copy)->auxiliaryData =
        sdfResourceRetainAddress(((EffTemplatePacketList *)copy)->auxiliaryAllocation);
    func_00153740(copy);
    effResetDiscAuxPacketAges(copy);
    return copy;
}

void effFreeDiscAuxTemplate(u32 effect) {
    func_002D0918(((EffTemplatePacketList *)effect)->auxiliaryAllocation);
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
    radius = effect->radius * ((effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f);
    tmp[0] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    tmp[1] = 0;
    tmp[2] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
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
    tmp[0] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    tmp[1] = 0;
    tmp[2] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, tmp);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, packet->vel);
    packet->f30 = effect->f164 * (effMiscRandUnitFloat(D_0034DF38) * effect->f174 + (1.0f - effect->f174));
    packet->f34 = 0;
    packet->f38 = 0;
    packet->f3C = effect->f15C * (effMiscRandUnitFloat(D_0034DF38) * effect->f170 + (1.0f - effect->f170));
    packet->age = -(effMiscRand(effEmitterDelayRandomState) % (effect->spread + 1));
    packet->color = 0;
    PCP_COPY_VECTOR(packet, effect->head.origin);
    jitter = effect->head.speedJitter;
    packet->speed = effect->head.speed * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter));
    jitter = effect->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
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

            packet->color = func_00159AB8(effect->head.fade, packet->color, age);
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
    u32 i;

    i = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            i = i + 1;
            record = record + 1;
        } while (i < effect->packetCount);
    }
}

/* Scale template position, record scale, and the second tail value. */
void effScaleTemplatePacketPositions(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[1] = effect->tailValues[1] * scale;
}

/* Allocate one 12-byte descriptor per packet and its attached 16-byte records. */
s32 effCloneTemplateWithPacketDescriptors(EffTemplatePacketList *source) {
    s32 copy = (s32)func_002CFEB8(0x190);
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
    func_00153740(copy);
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
        ((EffTemplatePacketList *)copy)->listAllocation = func_002D03F8(listBytes + perRecord * count * 16);
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
    effMarkAllTemplateBufferRecords(copy);
    return copy;
}
