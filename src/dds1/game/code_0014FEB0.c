#include "common.h"
#include "sdf_chip.h"
#include "bill_object_api.h"
#include "sdf_resource.h"
#include "eff.h"
#include "eff_node.h"
#include "eff_node_descriptor.h"

struct SdfTex;
extern void sdfTexReleaseReferenceViaHandler(struct SdfTex *texture);

extern s32 D_003BD7F4;
extern BillObj *effBillResourceOwners[];
extern BillObj *billCreateFromResource(s32 kind, const char *path);

typedef struct EffBillResourceInit {
    const char *resourcePath;
    s16 billboardKind;
    s16 pad06;
    f32 scale;
} EffBillResourceInit;

typedef char EffBillResourceInit_size_must_be_0x0C[
    (sizeof(EffBillResourceInit) == 0x0C) ? 1 : -1];

extern EffBillResourceInit effBillResourceInitTable[];

EffNode *effCloneSourceWithTypeHandler(EffNode *source) {
    EffNode *copy = (EffNode *)sdfAllocSizeClassBlock(0x10);
    u32 argument = (u32)source->instance;

    copy->type = source->type;
    copy->arg = source->arg;
    copy->instance = effNodeTypeOperations[source->type].cloneInstanceWord(argument);
    return copy;
}

extern s32 func_003003F0(const char *format, ...);

/* Normalize legacy payloads before the effect manager constructs their nodes. */
void effConvertLegacyNodeDescriptor(EffNodeDescriptor *descriptor) {
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
        billboard = billCreateFromResource(effBillResourceInitTable[i].billboardKind,
                                           effBillResourceInitTable[i].resourcePath);
        payload = billboard->child;
        effBillResourceOwners[i] = billboard;
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

INCLUDE_ASM(const s32, "game/code_0014FEB0", billCreateChildPayloadFromTextureResource);

/* Drop one reference; the last one releases the texture and its allocation. */
void effReleaseSharedTextureRecord(BillChildPayload *entry) {
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

