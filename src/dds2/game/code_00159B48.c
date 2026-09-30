#include "common.h"
#include "pcp_vu0.h"
extern void func_00333288(void *, u32);
extern void func_00333270(void *, u32);
extern void func_003332A0(void *, u32);
#include "ee_mmi.h"
extern void *func_003335E0();
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

extern void func_001618C8(s32);
extern void func_001634A8(s32);
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

extern u32 func_00161EE8(u32, u32);

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

void func_0015E1D0(EffTemplatePacketList *effect);

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

s32 func_00159BB8(s32 index) {
    return *(s32 *)((EffResourceRef *)D_00451EE0[index])->resource;
}

void effCopyVector(dst, src)
void *dst;
void *src;
{
    PCP_COPY_VECTOR(dst, src);
}

void func_00159BE8(BillObj *effect, float scale) {
    effect->unk20 = scale;
}

void func_00159BF0(BillObj *effect, float x, float y) {
    effect->unk10 = x;
    effect->unk14 = y;
}

void func_00159C00(BillObj *effect, u32 value) {
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

void func_00159CF0(BillObj *effect, s16 value) {
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

s32 func_00159D60(BillObj *effect) {
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

s32 func_00159E50(s32 billboard) {
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

u16 func_00159F60(BillObj *effect) {
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

s32 func_00159F80(s32 billboard) {
    if (((BillKindOneView *)billboard)->kind == 1) {
        return ((BillKindOneView *)billboard)->flags;
    }
    return 0;
}

void func_00159FA0(s32 billboard) {
    if (((BillKindOneView *)billboard)->kind == 1) {
        ((BillKindOneView *)billboard)->flags |= 0x1000000;
    }
}

void func_00159FC8(s32 billboard, float width, float height) {
    if (((BillKindOneView *)billboard)->kind == 0) {
        s32 tmp = (s32)((BillObj *)billboard)->unk30;
        ((BillChildPayload *)tmp)->halfWidth = width * 0.5f;
        ((BillChildPayload *)tmp)->halfHeight = height * 0.5f;
    }
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_00159FF8);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015A150);

u8 *billCreateUnitObject(s32 index) {
    EffInstance *instance = func_00328D68(0x88);

    instance->billboard = (BillObj *)billCreateIndexed(1, index);
    instance->renderState = func_003335E0();
    func_003332D0(instance->renderState, 1.0f);
    EE_MMI_UNIT_MATRIX(instance->transform);
    return (u8 *)instance;
}

u8 *billCloneUnitObject(EffInstance *source) {
    EffInstance *instance = func_00328D68(0x88);

    instance->billboard = (BillObj *)func_00159A50((s32)source->billboard);
    instance->renderState = func_003335E0();
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
    func_00328E48(instance);
}

void func_0015A348(EffInstance *instance) {
    effCopyVector((u32)instance->billboard);
}

void billSetChildScale2(EffInstance *instance, f32 scale) {
    func_00159BF0(instance->billboard, scale, scale);
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effCopyMatrixToNext(EffInstance *instance, void *src) {
    u8 *dst;

    VU0_LOAD_MATRIX(src);
    dst = instance->transform;
    VU0_STORE_MATRIX(dst);
}

void billSetChildValue(EffInstance *instance, u32 value) {
    func_00159C00(instance->billboard, value);
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

void func_0015B318(u32 *allocationSlot) {
    func_003297C8(*allocationSlot);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015B330);

void effDestroyResources(EffResourceOwner *owner) {
    switch (owner->kind) {
    case 1:
        func_001618C8(owner->unk38);
        break;
    case 2:
        func_001634A8(owner->unk40);
        break;
    case 3:
        func_001634A8(owner->unk44);
        break;
    case 4:
        func_001900B8(owner->unk44);
        break;
    }
    billDispatchByKind(owner->billboard);
    func_0015B318(owner->buffer);
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

s32 func_0015B680(EffTemplatePacketList *source) {
    s32 obj = (s32)func_00328D68(0x180);
    s32 tailLen = 0x30;

    memset((void *)obj, 0, 0x180);
    memcpy((void *)obj, source, source->templateSize);
    memcpy((void *)(obj + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_0015B330(obj);
    effSetTemplateTagPeriod(obj);
    return obj;
}

void func_0015B700(u32 effect) {
    effDestroyResources((EffResourceOwner *)effect);
    func_00328E48(effect);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015B728);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015B988);

void func_0015BC38(EffTemplatePacketList *effect) {
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

void func_0015BC80(float scale, EffTemplatePacketList *effect) {
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
    func_0015BC38(copy);
    return copy;
}

void func_0015BD50(u32 effect) {
    effDestroyResources((EffResourceOwner *)effect);
    func_00328E48(effect);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015BD78);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015C020);

void func_0015C288(EffTemplatePacketList *effect) {
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

void func_0015C2D0(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
    effect->tailValues[3] = effect->tailValues[3] * scale;
}

s32 func_0015C320(EffTemplatePacketList *source) {
    s32 copy = (s32)func_00328D68(0x180);
    s32 tailLen = 0x30;

    memset((void *)copy, 0, 0x180);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_0015B330(copy);
    func_0015C288(copy);
    return copy;
}

void func_0015C3A0(u32 effect) {
    effDestroyResources((EffResourceOwner *)effect);
    func_00328E48(effect);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015C3C8);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015CBC8);

extern void func_0015CF70(EffTemplatePacketList *effect, u32 index);

void func_0015CDF0(EffTemplatePacketList *effect) {
    EffectBufferRecord *record;
    u32 i;
    u32 next;
    s32 tag;

    tag = 0;
    i = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            func_0015CF70(effect, i);
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

void func_0015CE90(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
}

s32 func_0015CEC8(EffTemplatePacketList *source) {
    s32 copy = (s32)func_00328D68(0x170);
    s32 tailLen = 0x20;

    memset((void *)copy, 0, 0x170);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_0015B330(copy);
    func_0015CDF0(copy);
    return copy;
}

void func_0015CF48(u32 effect) {
    effDestroyResources((EffResourceOwner *)effect);
    func_00328E48(effect);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015CF70);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015D208);

void func_0015D420(EffTemplatePacketList *effect) {
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

void func_0015D468(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[1] = effect->tailValues[1] * scale;
}

s32 func_0015D4A8(EffTemplatePacketList *source) {
    s32 copy = (s32)func_00328D68(0x170);
    s32 tailLen = 0x20;

    memset((void *)copy, 0, 0x170);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_0015B330(copy);
    func_0015D420(copy);
    return copy;
}

void func_0015D528(u32 effect) {
    effDestroyResources((EffResourceOwner *)effect);
    func_00328E48(effect);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015D550);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015D8B0);

void func_0015DB20(EffTemplatePacketList *effect) {
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

void func_0015DB68(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
}

s32 func_0015DBB8(EffTemplatePacketList *source) {
    s32 copy = (s32)func_00328D68(0x180);
    s32 tailLen = 0x30;

    memset((void *)copy, 0, 0x180);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_0015B330(copy);
    func_0015DB20(copy);
    return copy;
}

void func_0015DC38(u32 effect) {
    effDestroyResources((EffResourceOwner *)effect);
    func_00328E48(effect);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015DC60);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015DF68);

void func_0015E1D0(EffTemplatePacketList *effect) {
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

void func_0015E240(float scale, EffTemplatePacketList *effect) {
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
    func_0015E1D0(obj);
    return obj;
}

void func_0015E2F8(u32 effect) {
    effDestroyResources((EffResourceOwner *)effect);
    func_00328E48(effect);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015E320);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015E510);

void func_0015E788(EffTemplatePacketList *effect) {
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

void func_0015E7D0(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
    effect->tailValues[3] = effect->tailValues[3] * scale;
}

s32 func_0015E820(EffTemplatePacketList *source) {
    s32 copy = (s32)func_00328D68(0x180);
    s32 tailLen = 0x30;

    memset((void *)copy, 0, 0x180);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_0015B330(copy);
    func_0015E788(copy);
    return copy;
}

void func_0015E8A0(u32 effect) {
    effDestroyResources((EffResourceOwner *)effect);
    func_00328E48(effect);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015E8C8);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015EB20);

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

void func_0015ED78(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[1] = effect->tailValues[1] * scale;
}

s32 func_0015EDC8(EffTemplatePacketList *source) {
    s32 copy = (s32)func_00328D68(0x170);
    s32 tailLen = 0x20;

    memset((void *)copy, 0, 0x170);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_0015B330(copy);
    effApplyTemplateScaleToRecords(copy);
    return copy;
}

void func_0015EE48(u32 effect) {
    effDestroyResources((EffResourceOwner *)effect);
    func_00328E48(effect);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015EE70);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015F1C0);

void func_0015F560(void) {
    effInitParticleRecord();
}

void func_0015F578(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
}

void *func_0015F5A0(EffTemplatePacketList *source) {
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

void func_0015F620(u32 effect) {
    effDestroyResources((EffResourceOwner *)effect);
    func_00328E48(effect);
}

extern u8 D_003AA868[];
extern f32 func_00341240(void *);

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
    particle->speed = effect->speed * (func_00341240(D_003AA868) * jitter + (1.0f - jitter));
    jitter = effect->angleJitter;
    if (jitter != 0) {
        particle->angle = (func_00341240(D_003AA868) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        particle->angle = 0;
    }
}

void func_0015F748(EffParticle *effect) {
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
    record->unk24 = func_00161EE8(color, effect->unkF0);
}

void func_0015F7D8(EffTemplatePacketList *effect) {
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

void func_0015F820(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
    effect->tailValues[3] = effect->tailValues[3] * scale;
}

s32 func_0015F870(EffTemplatePacketList *source) {
    s32 copy = (s32)func_00328D68(0x190);
    s32 tailLen = 0x40;

    memset((void *)copy, 0, 0x190);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_0015B330(copy);
    func_0015F7D8(copy);
    return copy;
}

void func_0015F8F0(u32 effect) {
    effDestroyResources((EffResourceOwner *)effect);
    func_00328E48(effect);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015F918);

INCLUDE_ASM(const s32, "game/code_00159B48", func_001600A8);

void func_00160308(EffTemplatePacketList *effect) {
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

void func_00160350(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
}

void *func_001603A0(EffTemplatePacketList *source) {
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
    func_00160308(copy);
    return copy;
}

void func_00160438(u32 effect) {
    func_003297C8(((EffTemplatePacketList *)effect)->auxiliaryAllocation);
    effDestroyResources(effect);
    func_00328E48(effect);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_00160470);

INCLUDE_ASM(const s32, "game/code_00159B48", func_00160700);

void func_00160978(EffTemplatePacketList *effect) {
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

void func_001609C0(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[1] = effect->tailValues[1] * scale;
}

/* Allocate one 12-byte descriptor per packet and its attached 16-byte records. */
s32 func_00160A00(EffTemplatePacketList *source) {
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
    func_00160978((EffTemplatePacketList *)copy);
    return copy;
}
