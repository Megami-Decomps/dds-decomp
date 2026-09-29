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
    f32 unk10;          /* 0x10 */
    u8 pad14[0x10];
    u32 unk24;          /* 0x24 */
    u8 pad28[0x24];
    u32 unk4C;          /* 0x4C */
    u8 pad50[8];
    u32 unk58;          /* 0x58 */
    u8 pad5C[0x30];
    f32 unk8C;          /* 0x8C */
    u8 pad90[4];
    f32 unk94;          /* 0x94 */
    f32 unk98;          /* 0x98 */
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
    f32 unk28;
    f32 unk2C;
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
    u8 pad24[0x7C];
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

s32 sdfAllocPacketAligned(s32 arg0);

void sdfResetPacketList(s32 arg0);

void func_0015AA30(s32 arg0, s32 arg1);

void func_0015AD18(s32 arg0, s32 arg1);

void func_0015B330(s32 arg0);

void func_0015B5C0(EffTemplatePacketList *effect);

extern void *memset(void *s, s32 c, u32 n);

extern void *memcpy(void *dest, const void *src, u32 n);

extern void *func_00328D68(s32 size);

extern EffectConfig D_003AA884[];

s32 billCreateIndexed(s32 arg0, s32 arg1);

s32 func_003292A8(s32 size);

EffectBufferRecord *sdfResourceRetainAddress(s32 allocation);

void func_0015E1D0(EffTemplatePacketList *effect);

void effRetainResource(s32 index) {
    s32 *effect = (s32 *)billCreateIndexed(D_003AA884[index].unk00, 0);
    s32 *resource = *(s32 **)(D_00451EE0[index] + 0x30);
    s32 references = resource[2];

    effect[12] = (s32)resource;
    resource[2] = references + 1;
}

u32 func_00159BB0(void) {
    return 0xf;
}

s32 func_00159BB8(s32 arg0) {
    return *(s32 *)(*(s32 *)(D_00451EE0[arg0] + 0x30));
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
    s16 value; /* 0x04 */
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
        *(s16 *)((u8 *)effect->unk30 + 4) = v;
        break;
    case 1:
        effect->unk3C = v;
        break;
    }
}

u16 billGetVariantValue(BillObj *effect) {
    switch (effect->unk2C) {
    case 0:
        return *(u16 *)((u8 *)effect->unk30 + 4);
    case 1:
        return effect->unk3C;
    default:
        return 0;
    }
}

void billSetKind1Entry(BillObj *effect, u32 value) {
    if (effect->unk2C == 1 && effect->unk58 != value) {
        func_001594C8(effect, value);
    }
}

s32 billGetKindOneEntry(BillObj *effect) {
    if (effect->unk2C == 1) {
        return effect->unk58;
    }
    return 0;
}

s32 func_00159E50(s32 arg0) {
    if (*(u16 *)(arg0 + 0x2c) == 1) {
        return *(s32 *)(*(s32 *)(*(s32 *)(arg0 + 0x30) + 4) + 4);
    }
    return 0;
}

void func_00159E78(BillObj *effect, u32 time) {
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

void func_00159ED8(BillObj *effect, u32 time) {
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

s32 func_00159F80(s32 arg0) {
    if (*(u16 *)(arg0 + 0x2c) == 1) {
        return *(s32 *)(arg0 + 0x54);
    }
    return 0;
}

void func_00159FA0(s32 arg0) {
    if (*(u16 *)(arg0 + 0x2c) == 1) {
        *(s32 *)(arg0 + 0x54) |= 0x1000000;
    }
}

void func_00159FC8(s32 arg0, float arg1, float arg2) {
    if (*(u16 *)(arg0 + 0x2c) == 0) {
        s32 tmp = *(s32 *)(arg0 + 0x30);
        *(float *)(tmp + 0x24) = arg1 * 0.5f;
        *(float *)(tmp + 0x28) = arg2 * 0.5f;
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
void func_0015A3C8(void *dst, void *src) {
    VU0_COPY_MATRIX(dst, src);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015A3F0);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015A4B0);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015AA30);

void func_0015ACB0(s32 arg0, s32 arg1) {
    s32 tmp = sdfAllocPacketAligned(0x20);

    sdfResetPacketList(tmp);
    func_0015AA30(tmp, arg1);
    ((void (*)(s32, s32))*(s32 *)(arg0 + 0x10))(arg0, tmp);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015AD18);

void func_0015B208(s32 arg0, s32 arg1) {
    s32 tmp = sdfAllocPacketAligned(0x20);

    sdfResetPacketList(tmp);
    func_0015AD18(tmp, arg1);
    ((void (*)(s32, s32))*(s32 *)(arg0 + 0x10))(arg0, tmp);
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

void func_0015B318(u32 *arg0) {
    func_003297C8(*arg0);
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

void func_0015B5C0(EffTemplatePacketList *effect) {
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

void func_0015B630(float scale, EffTemplatePacketList *effect) {
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
    func_0015B5C0(obj);
    return obj;
}

void func_0015B700(u32 arg0) {
    effDestroyResources((EffResourceOwner *)arg0);
    func_00328E48(arg0);
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

void func_0015BD50(u32 arg0) {
    effDestroyResources((EffResourceOwner *)arg0);
    func_00328E48(arg0);
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

void func_0015C3A0(u32 arg0) {
    effDestroyResources((EffResourceOwner *)arg0);
    func_00328E48(arg0);
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

void func_0015CF48(u32 arg0) {
    effDestroyResources((EffResourceOwner *)arg0);
    func_00328E48(arg0);
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

void func_0015D528(u32 arg0) {
    effDestroyResources((EffResourceOwner *)arg0);
    func_00328E48(arg0);
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

void func_0015DC38(u32 arg0) {
    effDestroyResources((EffResourceOwner *)arg0);
    func_00328E48(arg0);
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

void func_0015E2F8(u32 arg0) {
    effDestroyResources((EffResourceOwner *)arg0);
    func_00328E48(arg0);
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

void func_0015E8A0(u32 arg0) {
    effDestroyResources((EffResourceOwner *)arg0);
    func_00328E48(arg0);
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

void func_0015EE48(u32 arg0) {
    effDestroyResources((EffResourceOwner *)arg0);
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015EE70);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015F1C0);

void func_0015F560(void) {
    func_0015F648();
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
    *(u32 *)((u8 *)copy + 0x24) = 0;
    func_0015B330((s32)copy);
    return copy;
}

void func_0015F620(u32 arg0) {
    effDestroyResources((EffResourceOwner *)arg0);
    func_00328E48(arg0);
}

extern u8 D_003AA868[];
extern f32 func_00341240(void *);

void func_0015F648(effect)
    EffParticle *effect;
{
    EffParticleRecord *particle = (EffParticleRecord *)effect->buffer->records;
    f32 jitter;
    u32 color;

    color = effect->unk4C | (effect->unk58 << 24);
    particle->x = effect->x;
    particle->unk20 = -1;
    particle->y = effect->y;
    particle->unk24 = color;
    particle->z = effect->z;
    particle->unk28 = 1.0f;
    particle->unk2C = 0;
    effect->unk8C = effect->unk10;
    jitter = effect->unk94;
    particle->unk28 = effect->unk10 * (func_00341240(D_003AA868) * jitter + (1.0f - jitter));
    jitter = effect->unk98;
    if (jitter != 0) {
        particle->unk2C = (func_00341240(D_003AA868) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        particle->unk2C = 0;
    }
}

void func_0015F748(EffParticle *effect) {
    EffParticleRecord *record = (EffParticleRecord *)effect->buffer->records;
    u32 color;

    if (record->unk20 == 0) {
        func_0015F648(effect);
    }
    record->unk20 = record->unk20 + 1;
    record->x = effect->x;
    record->y = effect->y;
    color = effect->unk4C | (effect->unk58 << 24);
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

void func_0015F8F0(u32 arg0) {
    effDestroyResources((EffResourceOwner *)arg0);
    func_00328E48(arg0);
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
    *(s32 *)((u8 *)copy + 0x17C) = allocation;
    *(void **)((u8 *)copy + 0x178) = sdfResourceRetainAddress(allocation);
    func_0015B330((s32)copy);
    func_00160308(copy);
    return copy;
}

void func_00160438(u32 arg0) {
    func_003297C8(*(u32 *)((s32)arg0 + 0x17c));
    effDestroyResources(arg0);
    func_00328E48(arg0);
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

s32 func_00160A00(s32 source) {
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
    memcpy((void *)copy, (void *)source, *(s32 *)(source + 0xa0));
    memcpy((void *)(copy + 0x150), (void *)(source + *(s32 *)(source + 0xa0)), tailLen);
    func_0015B330(copy);
    perRecord = 0;
    if (*(u16 *)(copy + 0x30) != 0) {
        count = *(s32 *)(copy + 0x20);
        switch (*(u16 *)(copy + 0x30)) {
        case 1:
            perRecord = *(u16 *)(copy + 0x34);
            break;
        case 2:
            perRecord = *(u16 *)(copy + 0x34);
            break;
        case 3:
            perRecord = *(u16 *)(copy + 0x34);
            break;
        case 4:
            perRecord = *(u16 *)(copy + 0x34);
            break;
        }
        listBytes = count * 12;
        *(s32 *)(copy + 0x170) = perRecord;
        *(s32 *)(copy + 0x174) = func_003292A8(listBytes + perRecord * count * 16);
        addr = (s32 *)sdfResourceRetainAddress(*(s32 *)(copy + 0x174));
        i = 0;
        base = (s32)addr;
        *(s32 *)(copy + 0x16C) = base;
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
        *(s32 *)(copy + 0x174) = 0;
    }
    func_00160978((EffTemplatePacketList *)copy);
    return copy;
}
