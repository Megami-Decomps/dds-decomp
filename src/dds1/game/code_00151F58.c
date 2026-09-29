#include "common.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
extern void *func_002DA730();
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
    u8 pad24[0xC];
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
    s32 unk20;
    u32 unk24;
    f32 speed;
    f32 angle;
    u8 pad30[0x10];
} EffParticleRecord;

extern u8 D_0034DF38[];
extern f32 func_002E8398(void *);

extern EffectConfig D_0034DF54[];

s32 billCreateIndexed(s32 arg0, s32 arg1);

extern s32 D_003D6438[];

extern s32 D_003D6480[];

s32 sdfAllocPacketAligned(s32 arg0);

void sdfInitPacketList(s32 arg0);

s32 func_00151398(s32 arg0, s32 arg1);

void func_00152E40(s32 arg0, s32 arg1);

void func_00153128(s32 arg0, s32 arg1);

void func_00153740(s32 arg0);

void func_001539D0(EffTemplatePacketList *effect);

extern void *memset(void *s, s32 c, u32 n);

extern void *memcpy(void *dest, const void *src, u32 n);

extern void *func_002CFEB8(s32 size);

s32 func_002D03F8(s32 size);

EffectBufferRecord *sdfResourceRetainAddress(s32 allocation);

void func_001565E0(EffTemplatePacketList *effect);

void func_00155380(EffTemplatePacketList *effect, u32 index);

void effRetainResource(s32 index) {
    s32 *effect = (s32 *)billCreateIndexed(D_0034DF54[index].unk00, 0);
    s32 *resource = *(s32 **)(D_003D6438[index] + 0x30);
    s32 references = resource[2];

    effect[12] = (s32)resource;
    resource[2] = references + 1;
}

u32 func_00151FC0(void) {
    return 0xf;
}

s32 func_00151FC8(s32 arg0) {
    return *(s32 *)(*(s32 *)(D_003D6438[arg0] + 0x30));
}

void effCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00151FF8(BillObj *effect, float scale) {
    effect->unk20 = scale;
}

void func_00152000(BillObj *effect, float x, float y) {
    effect->unk10 = x;
    effect->unk14 = y;
}

void func_00152010(BillObj *effect, u32 value) {
    effect->unk24 = value;
}

void effCopyPosition(BillObj *effect, const void *position) {
    if (effect->unk2C == 0) {
        memcpy((void *)((s32)effect->unk30 + 0xc), position, 16);
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

void effBillSetMode(BillObj *effect, s32 mode) {
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
            entry = (s32)effect->unk60 + 0xc;
            do {
                s32 node = *(s32 *)entry;
                u32 flags = *(u32 *)(node + 0x10) & ~6;
                *(u32 *)(node + 0x10) = flags;
                if (mode == 2) {
                    *(u32 *)(node + 0x10) = flags | 2;
                } else if (mode == 3) {
                    *(u32 *)(node + 0x10) = flags | 4;
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

void func_00152100(BillObj *effect, s32 value) {
    s32 count;
    s32 *entries;
    s32 *entry;

    value = (s16)value;
    switch (effect->unk2C) {
    case 0:
        *(s16 *)((u8 *)effect->unk30 + 4) = value;
        break;
    case 1:
        count = ((BillEntryList *)effect->unk30)->count;
        entries = ((BillEntryList *)effect->unk30)->entries;
        if (count > 0) {
            entry = entries;
            do {
                *(s16 *)(*entry + 4) = value;
                entry++;
                count--;
            } while (count != 0);
        }
        break;
    }
}

s32 func_00152170(BillObj *effect) {
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

/* Replace the selected list entry only when its index changes. */
void billSetKind1Entry(BillObj *effect, u32 value) {
    if (effect->unk2C == 1 && effect->unk58 != value) {
        func_001518D8(effect, value);
    }
}

/* Read the selected entry for list billboards; other kinds have none. */
s32 billGetKindOneEntry(BillObj *effect) {
    if (effect->unk2C == 1) {
        return effect->unk58;
    }
    return 0;
}

s32 func_00152260(s32 arg0) {
    if (*(u16 *)(arg0 + 0x2c) == 1) {
        return *(s32 *)(*(s32 *)(*(s32 *)(arg0 + 0x30) + 4) + 4);
    }
    return 0;
}

/* Start every entry's animation at the requested frame, with mode zero. */
void func_00152288(BillObj *effect, u32 time) {
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
void func_001522E8(BillObj *effect, u32 time) {
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

/* Read the kind-one billboard's halfword at +0x50. */
u16 func_00152370(BillObj *effect) {
    if (effect->unk2C == 1) {
        return effect->unk50;
    }
    return 0;
}

s32 func_00152390(s32 arg0) {
    if (*(u16 *)(arg0 + 0x2c) == 1) {
        return *(s32 *)(arg0 + 0x54);
    }
    return 0;
}

void func_001523B0(s32 arg0) {
    if (*(u16 *)(arg0 + 0x2c) == 1) {
        *(s32 *)(arg0 + 0x54) |= 0x1000000;
    }
}

void func_001523D8(s32 arg0, float arg1, float arg2) {
    if (*(u16 *)(arg0 + 0x2c) == 0) {
        s32 tmp = *(s32 *)(arg0 + 0x30);
        *(float *)(tmp + 0x24) = arg1 * 0.5f;
        *(float *)(tmp + 0x28) = arg2 * 0.5f;
    }
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152408);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152560);

u8 *billCreateUnitObject(s32 arg0) {
    u8 *obj = func_002CFEB8(0x88);

    *(s32 *)(obj + 0x80) = billCreateIndexed(1, arg0);
    *(void **)(obj + 0x84) = func_002DA730();
    func_002DA420(*(void **)(obj + 0x84), 1.0f);
    EE_MMI_UNIT_MATRIX(obj + 0x40);
    return obj;
}

u8 *billCloneUnitObject(u8 *src) {
    u8 *obj = func_002CFEB8(0x88);

    *(s32 *)(obj + 0x80) = func_00151E60(*(s32 *)(src + 0x80));
    *(void **)(obj + 0x84) = func_002DA730();
    func_002DA420(*(void **)(obj + 0x84), 1.0f);
    func_002DA3D8(*(void **)(obj + 0x84), 0x80808080);
    func_002DA3C0(*(void **)(obj + 0x84), 0x80808080);
    func_002DA3F0(*(void **)(obj + 0x84), 0x80808080);
    EE_MMI_UNIT_MATRIX(obj + 0x40);
    EE_MMI_UNIT_MATRIX(obj);
    return obj;
}

void effDestroy(u32 arg0) {
    sdfQueueAssetRelease(*(u32 *)(arg0 + 0x84));
    billDispatchByKind(*(u32 *)(arg0 + 0x80));
    func_002CFF98(arg0);
}

void func_00152758(s32 arg0, s128 *arg1) {
    effCopyVector(*(s128 **)(arg0 + 0x80), arg1);
}

void billSetChildScale2(s32 arg0, float arg1) {
    func_00152000(*(s32 *)(arg0 + 0x80), arg1, arg1);
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effCopyMatrixToNext(u8 *dst, void *src) {
    VU0_LOAD_MATRIX(src);
    dst += 0x40;
    VU0_STORE_MATRIX(dst);
}

void billSetChildValue(s32 arg0, u32 arg1) {
    func_00152010(*(s32 *)(arg0 + 0x80), arg1);
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void func_001527D8(void *dst, void *src) {
    VU0_COPY_MATRIX(dst, src);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152800);

INCLUDE_ASM(const s32, "game/code_00151F58", func_001528C0);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152E40);

void func_001530C0(s32 arg0, s32 arg1) {
    s32 tmp = sdfAllocPacketAligned(0x20);

    sdfInitPacketList(tmp);
    func_00152E40(tmp, arg1);
    ((void (*)(s32, s32))*(s32 *)(arg0 + 0x10))(arg0, tmp);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00153128);

void func_00153618(s32 arg0, s32 arg1) {
    s32 tmp = sdfAllocPacketAligned(0x20);

    sdfInitPacketList(tmp);
    func_00153128(tmp, arg1);
    ((void (*)(s32, s32))*(s32 *)(arg0 + 0x10))(arg0, tmp);
}

void func_00153680(void) {
    func_002E84A0(&D_003D6480);
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

void func_00153728(u32 *arg0) {
    func_002D0918(*arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00153740);

void effDestroyResources(effect)
    s32 effect;

{
    switch (*(u16 *)(effect + 0x30)) {
    case 1:
        func_00159CD8(*(s32 *)(effect + 0x38));
        break;
    case 2:
        func_0015B8B8(*(s32 *)(effect + 0x40));
        break;
    case 3:
        func_0015B8B8(*(s32 *)(effect + 0x44));
        break;
    case 4:
        func_00188480(*(s32 *)(effect + 0x44));
        break;
    }
    billDispatchByKind(*(s32 *)(effect + 0xf4));
    func_00153728(*(s32 *)(effect + 0xf8));
}

void func_001539D0(EffTemplatePacketList *effect) {
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
void func_00153A40(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[1] = effect->tailValues[1] * scale;
    effect->tailValues[3] = effect->tailValues[3] * scale;
}

/* Clone the template prefix and appended tail, then allocate its packet records. */
s32 func_00153A90(EffTemplatePacketList *source) {
    s32 obj = (s32)func_002CFEB8(0x180);
    s32 tailLen = 0x30;

    memset((void *)obj, 0, 0x180);
    memcpy((void *)obj, source, source->templateSize);
    memcpy((void *)(obj + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_00153740(obj);
    func_001539D0(obj);
    return obj;
}

void func_00153B10(u32 arg0) {
    effDestroyResources();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00153B38);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00153D98);

void func_00154048(EffTemplatePacketList *effect) {
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
void func_00154090(float scale, EffTemplatePacketList *effect) {
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
    func_00154048(copy);
    return copy;
}

void func_00154160(u32 arg0) {
    effDestroyResources();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00154188);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00154430);

void func_00154698(EffTemplatePacketList *effect) {
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
void func_001546E0(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
    effect->tailValues[3] = effect->tailValues[3] * scale;
}

s32 func_00154730(EffTemplatePacketList *source) {
    s32 copy = (s32)func_002CFEB8(0x180);
    s32 tailLen = 0x30;

    memset((void *)copy, 0, 0x180);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_00153740(copy);
    func_00154698(copy);
    return copy;
}

void func_001547B0(u32 arg0) {
    effDestroyResources();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_001547D8);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00154FD8);

void func_00155200(EffTemplatePacketList *effect) {
    EffectBufferRecord *record;
    u32 i;
    u32 next;
    s32 tag;

    tag = 0;
    i = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            func_00155380(effect, i);
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
void func_001552A0(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
}

s32 func_001552D8(EffTemplatePacketList *source) {
    s32 copy = (s32)func_002CFEB8(0x170);
    s32 tailLen = 0x20;

    memset((void *)copy, 0, 0x170);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_00153740(copy);
    func_00155200(copy);
    return copy;
}

void func_00155358(u32 arg0) {
    effDestroyResources();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00155380);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00155618);

void func_00155830(EffTemplatePacketList *effect) {
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
void func_00155878(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[1] = effect->tailValues[1] * scale;
}

s32 func_001558B8(EffTemplatePacketList *source) {
    s32 copy = (s32)func_002CFEB8(0x170);
    s32 tailLen = 0x20;

    memset((void *)copy, 0, 0x170);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_00153740(copy);
    func_00155830(copy);
    return copy;
}

void func_00155938(u32 arg0) {
    effDestroyResources();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00155960);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00155CC0);

void func_00155F30(EffTemplatePacketList *effect) {
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
void func_00155F78(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
}

s32 func_00155FC8(EffTemplatePacketList *source) {
    s32 copy = (s32)func_002CFEB8(0x180);
    s32 tailLen = 0x30;

    memset((void *)copy, 0, 0x180);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_00153740(copy);
    func_00155F30(copy);
    return copy;
}

void func_00156048(u32 arg0) {
    effDestroyResources();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00156070);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00156378);

void func_001565E0(EffTemplatePacketList *effect) {
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
void func_00156650(float scale, EffTemplatePacketList *effect) {
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
    func_001565E0(copy);
    return copy;
}

void func_00156708(u32 arg0) {
    effDestroyResources();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00156730);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00156920);

void func_00156B98(EffTemplatePacketList *effect) {
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
void func_00156BE0(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
    effect->tailValues[3] = effect->tailValues[3] * scale;
}

s32 func_00156C30(EffTemplatePacketList *source) {
    s32 copy = (s32)func_002CFEB8(0x180);
    s32 tailLen = 0x30;

    memset((void *)copy, 0, 0x180);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_00153740(copy);
    func_00156B98(copy);
    return copy;
}

void func_00156CB0(u32 arg0) {
    effDestroyResources();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00156CD8);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00156F30);

void effApplyTemplateScaleToRecords(EffTemplatePacketList *effect) {
    s32 i = 0;
    EffectBufferRecord *record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            *(f32 *)((u8 *)record + 0x34) = effect->recordScale;
            i++;
            record++;
        } while ((u32)i < effect->packetCount);
    }
}

/* Scale the position, record scale, and first two tail values. */
void func_00157188(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[1] = effect->tailValues[1] * scale;
}

s32 func_001571D8(EffTemplatePacketList *source) {
    s32 copy = (s32)func_002CFEB8(0x170);
    s32 tailLen = 0x20;

    memset((void *)copy, 0, 0x170);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_00153740(copy);
    effApplyTemplateScaleToRecords(copy);
    return copy;
}

void func_00157258(u32 arg0) {
    effDestroyResources();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00157280);

INCLUDE_ASM(const s32, "game/code_00151F58", func_001575D0);

void func_00157970(void) {
    func_00157A58();
}

/* Apply a uniform scale to the particle template's position. */
void func_00157988(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
}

/* Clone the prefix without tail bytes; the zero-length copy is retained for matching. */
s32 func_001579B0(EffTemplatePacketList *source) {
    s32 copy = (s32)func_002CFEB8(0x150);
    s32 tailLen = 0;

    memset((void *)copy, 0, 0x150);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    ((EffTemplatePacketList *)copy)->packetCount = 1;
    *(s32 *)(copy + 0x24) = 0;
    func_00153740(copy);
    return copy;
}

void func_00157A30(u32 arg0) {
    effDestroyResources();
    func_002CFF98(arg0);
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
void func_00157A58(effect)
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
    particle->speed = effect->speed * (func_002E8398(D_0034DF38) * jitter + (1.0f - jitter));
    jitter = effect->angleJitter;
    if (jitter != 0) {
        particle->angle = (func_002E8398(D_0034DF38) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        particle->angle = 0;
    }
}

extern u32 func_0015A2F8(u32, u32);

void func_00157B58(EffParticle *effect) {
    EffParticleRecord *particle = (EffParticleRecord *)effect->buffer->records;
    s32 count = particle->unk20;
    u32 color;

    if (count == 0) {
        func_00157A58(effect);
        count = particle->unk20;
    }
    particle->unk20 = count + 1;
    particle->x = effect->x;
    effect->unk24 = count + 2;
    particle->y = effect->y;
    color = effect->color | (effect->alpha << 24);
    particle->z = effect->z;
    particle->unk24 = color;
    particle->unk24 = func_0015A2F8(color, effect->unkF0);
}

void func_00157BE8(EffTemplatePacketList *effect) {
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
void func_00157C30(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
    effect->tailValues[3] = effect->tailValues[3] * scale;
}

s32 func_00157C80(EffTemplatePacketList *source) {
    s32 copy = (s32)func_002CFEB8(0x190);
    s32 tailLen = 0x40;

    memset((void *)copy, 0, 0x190);
    memcpy((void *)copy, source, source->templateSize);
    memcpy((void *)(copy + 0x150), (u8 *)source + source->templateSize, tailLen);
    func_00153740(copy);
    func_00157BE8(copy);
    return copy;
}

void func_00157D00(u32 arg0) {
    effDestroyResources();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00157D28);

INCLUDE_ASM(const s32, "game/code_00151F58", func_001584B8);

void func_00158718(EffTemplatePacketList *effect) {
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
void func_00158760(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[0] = effect->tailValues[0] * scale;
    effect->tailValues[2] = effect->tailValues[2] * scale;
}

s32 func_001587B0(EffTemplatePacketList *source) {
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
    func_00158718(copy);
    return copy;
}

void func_00158848(u32 arg0) {
    func_002D0918(((EffTemplatePacketList *)arg0)->auxiliaryAllocation);
    effDestroyResources(arg0);
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00158880);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00158B10);

void func_00158D88(EffTemplatePacketList *effect) {
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
void func_00158DD0(float scale, EffTemplatePacketList *effect) {
    effect->x = effect->x * scale;
    effect->y = effect->y * scale;
    effect->z = effect->z * scale;
    effect->recordScale = effect->recordScale * scale;
    effect->tailValues[1] = effect->tailValues[1] * scale;
}

/* Allocate one 12-byte descriptor per packet and its attached 16-byte records. */
s32 func_00158E10(EffTemplatePacketList *source) {
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
    func_00158D88(copy);
    return copy;
}
