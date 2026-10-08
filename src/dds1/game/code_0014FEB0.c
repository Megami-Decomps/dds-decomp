#include "common.h"
#include "eff.h"
#include "eff_node_descriptor.h"

typedef struct EffectSource {
    u32 type;
    u32 field4;
    u32 argument;
} EffectSource;

typedef struct EffectHandler {
    u32 (*handler)(u32);
    u8 pad04[0x2C];
} EffectHandler;

extern EffectHandler D_0034DE40[];
extern void *sdfAllocSizeClassBlock(s32);
extern s32 D_003BD7F4;
extern BillObj *effBillResourceOwners[];
extern BillObj *billCreateFromResource(s32 kind, s32 resource);
extern void billDispatchByKind(BillObj *billboard);

typedef struct EffBillResourceInit {
    s32 resource;
    s16 kind;
    s16 pad06;
    f32 scale;
} EffBillResourceInit;

extern EffBillResourceInit effBillResourceInitTable[];

void *effCloneSourceWithTypeHandler(EffectSource *source) {
    EffectSource *copy = (EffectSource *)sdfAllocSizeClassBlock(0x10);
    u32 argument = source->argument;

    copy->type = source->type;
    copy->field4 = source->field4;
    copy->argument = D_0034DE40[source->type].handler(argument);
    return copy;
}

extern s32 func_003003F0(const char *format, ...);

/* Normalize legacy payloads before the effect manager constructs their nodes. */
void func_0014FF28(EffNodeDescriptor *descriptor) {
    u8 *payload = descriptor->payload;

    if (descriptor->version == 1.0f) {
        u32 type = descriptor->type;
        switch (type) {
        case 0:
            *(u32 *)(payload + 0xA0) = 0xAC;
            break;
        case 1:
            {
                u8 *source = payload + *(u32 *)(payload + 4);
                payload = source + 0x10;
                *(u32 *)(payload + 0xA0) = 0xAC;
            }
            break;
        case 2:
            *(u32 *)(payload + 0x18) = 0x20;
            break;
        }
    }

    if (descriptor->version <= 1.01f) {
        u32 type = descriptor->type;
        if (type == 1) {
            u8 *source = payload + *(u32 *)(payload + 4);
            payload = source + 0x10;
            *(f32 *)(payload + 0x8C) =
                *(f32 *)(payload + 0x8C) * 0.1f *
                    (f32)*(s32 *)(payload + 0x24) +
                *(f32 *)(payload + 0x10);
        }
        if (type < 2 && descriptor->arg == 0) {
            func_003003F0("effManager:par spiral old version!![%f]\n", descriptor->version);
        }
    }
}

void effInitializeBillResourceOwners(void) {
    u32 i;
    BillObj *billboard;
    BillChildPayload *payload;

    D_003BD7F4 = 0;
    i = 0;
    do {
        billboard = billCreateFromResource(effBillResourceInitTable[i].kind, effBillResourceInitTable[i].resource);
        effBillResourceOwners[i] = billboard;
        payload = (BillChildPayload *)billboard->entryList;
        payload->halfWidth *= effBillResourceInitTable[i].scale;
        payload->halfHeight *= effBillResourceInitTable[i].scale;
        i++;
    } while (i < 15);
}

void effBillDispatchAll(void) {
    u32 i;

    D_003BD7F4 = 0;
    for (i = 0; i < 15; i++) {
        billDispatchByKind(effBillResourceOwners[i]);
    }
}

INCLUDE_ASM(const s32, "game/code_0014FEB0", func_00150148);

/* Texture record: +0x00 is the handle the reference is dropped from, +0x08 the
 * reference count and +0x44 the allocation released when it reaches zero. */
typedef struct TexRecord {
    void *texture;     /* 0x00 */
    u32 flags;         /* 0x04 */
    s32 refCount;      /* 0x08 */
    u8 pad0C[0x38];
    void *allocation;  /* 0x44 */
} TexRecord;

/* Drop one reference; the last one releases the texture and its allocation. */
void effReleaseSharedTextureRecord(TexRecord *entry) {
    entry->refCount--;
    if (entry->refCount == 0) {
        sdfTexReleaseReferenceViaHandler(entry->texture);
        sdfReleaseResourceAllocation(entry->allocation);
    }
}

INCLUDE_RODATA(const s32, "game/code_0014FEB0", D_003A0AA8);

INCLUDE_RODATA(const s32, "game/code_0014FEB0", D_003A0AC0);

INCLUDE_RODATA(const s32, "game/code_0014FEB0", D_003A0AD8);

INCLUDE_RODATA(const s32, "game/code_0014FEB0", D_003A0AF0);

INCLUDE_RODATA(const s32, "game/code_0014FEB0", D_003A0B08);

INCLUDE_RODATA(const s32, "game/code_0014FEB0", D_003A0B20);

INCLUDE_RODATA(const s32, "game/code_0014FEB0", D_003A0B38);

INCLUDE_RODATA(const s32, "game/code_0014FEB0", D_003A0B50);

INCLUDE_RODATA(const s32, "game/code_0014FEB0", D_003A0B68);

INCLUDE_RODATA(const s32, "game/code_0014FEB0", D_003A0B80);

INCLUDE_RODATA(const s32, "game/code_0014FEB0", D_003A0B98);

INCLUDE_RODATA(const s32, "game/code_0014FEB0", D_003A0BB0);

INCLUDE_RODATA(const s32, "game/code_0014FEB0", D_003A0BC8);

INCLUDE_RODATA(const s32, "game/code_0014FEB0", D_003A0BE0);

INCLUDE_RODATA(const s32, "game/code_0014FEB0", D_003A0BF8);

