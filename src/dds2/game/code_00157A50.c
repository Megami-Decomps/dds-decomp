#include "bill_object_api.h"
#include "common.h"
#include "sdf_chip.h"
#include "sdf_resource.h"
#include "eff.h"
#include "file_request_api.h"
#include "eff_node_descriptor.h"

struct SdfTex;
extern void sdfTexReleaseReferenceViaHandler(struct SdfTex *texture);

typedef struct EffectSource {
    u32 type;
    u32 field4;
    u32 argument;
} EffectSource;

typedef struct EffectHandler {
    u32 (*handler)(u32);
    u8 pad04[0x2C];
} EffectHandler;

extern EffectHandler D_003AA770[];

void *effCloneSourceWithTypeHandler(EffectSource *source) {
    EffectSource *copy = (EffectSource *)sdfAllocSizeClassBlock(0x10);
    u32 argument = source->argument;

    copy->type = source->type;
    copy->field4 = source->field4;
    copy->argument = D_003AA770[source->type].handler(argument);
    return copy;
}

extern void func_0035B6E0(const char *format, ...);

void func_00157AC8(EffNodeDescriptor *descriptor) {
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
            func_0035B6E0("effManager:par spiral old version!![%f]\n", descriptor->version);
        }
    }
}

typedef struct MemBlock MemBlock;

typedef struct EffBillResourceNode {
    struct EffBillResourceNode *next;
    u8 pad04[4];
    MemBlock *blockHandle;
} EffBillResourceNode;

typedef struct EffBillResourceArchive {
    u8 pad00[0x60];
    EffBillResourceNode *nodes;
} EffBillResourceArchive;

typedef struct EffBillResourceInit {
    const char *resourcePath;
    s16 billboardKind;
    s16 pad06;
    f32 scale;
} EffBillResourceInit;

extern s32 D_00438EFC;
extern BillObj *effBillResourceOwners[];
extern EffBillResourceInit D_003AA880[];
extern char D_004142B0[];
extern char D_004142C0[];
extern void func_002C7CE8(EffBillResourceArchive *archive);
extern void func_0035B6E0(const char *format, ...);

void effInitializeBillResourceOwners(void) {
    EffBillResourceArchive *archive;
    EffBillResourceNode *node;
    f32 *scale;
    u8 *configTable;
    f32 *scaleTable;
    BillObj **owner;
    BillObj *billboard;
    BillChildPayload *payload;
    u32 configOffset;

    D_00438EFC = 0;
    archive = (EffBillResourceArchive *)fileQueuePlainDispatchRequest(D_004142B0);
    func_002C81D0((struct FileRequest *)archive);
    node = archive->nodes;
    if (node != NULL) {
        configTable = (u8 *)D_003AA880;
        owner = effBillResourceOwners;
        scaleTable = (f32 *)((u8 *)D_003AA880 + 8);
        configOffset = 0;
        do {
            billboard = billCreateIndexed(((EffBillResourceInit *)(configOffset + (u32)configTable))->billboardKind,
                                          (u32)sdfMemoryGetBlockAddress((struct SdfMemBlock *)(u32)node->blockHandle));
            scale = (f32 *)(configOffset + (u32)scaleTable);
            configOffset += sizeof(EffBillResourceInit);
            payload = billboard->child;
            *owner++ = billboard;
            payload->halfWidth = payload->halfWidth * *scale;
            payload->halfHeight = payload->halfHeight * *scale;
            sdfReleaseResourceAllocation((struct SdfMemBlock *)(node->blockHandle));
            node = node->next;
        } while (node != NULL);
    }
    func_002C7CE8(archive);
    func_0035B6E0(D_004142C0, D_004142B0);
}

void effBillDispatchAll(void) {
    u32 i;

    D_00438EFC = 0;
    for (i = 0; i < 15; i++) {
        billDispatchByKind(effBillResourceOwners[i]);
    }
}

INCLUDE_ASM(const s32, "game/code_00157A50", billCreateChildPayloadFromTextureResource);

/* Drop one reference; the last one releases the texture and its allocation. */
void effReleaseSharedTextureRecord(BillChildPayload *entry) {
    entry->refCount--;
    if (entry->refCount == 0) {
        sdfTexReleaseReferenceViaHandler(entry->texture);
        sdfReleaseResourceAllocation(entry->allocation);
    }
}

INCLUDE_RODATA(const s32, "game/code_00157A50", D_00414148);

INCLUDE_RODATA(const s32, "game/code_00157A50", D_00414160);

INCLUDE_RODATA(const s32, "game/code_00157A50", D_00414178);

INCLUDE_RODATA(const s32, "game/code_00157A50", D_00414190);

INCLUDE_RODATA(const s32, "game/code_00157A50", D_004141A8);

INCLUDE_RODATA(const s32, "game/code_00157A50", D_004141C0);

INCLUDE_RODATA(const s32, "game/code_00157A50", D_004141D8);

INCLUDE_RODATA(const s32, "game/code_00157A50", D_004141F0);

INCLUDE_RODATA(const s32, "game/code_00157A50", D_00414208);

INCLUDE_RODATA(const s32, "game/code_00157A50", D_00414220);

INCLUDE_RODATA(const s32, "game/code_00157A50", D_00414238);

INCLUDE_RODATA(const s32, "game/code_00157A50", D_00414250);

INCLUDE_RODATA(const s32, "game/code_00157A50", D_00414268);

INCLUDE_RODATA(const s32, "game/code_00157A50", D_00414280);

INCLUDE_RODATA(const s32, "game/code_00157A50", D_00414298);

INCLUDE_RODATA(const s32, "game/code_00157A50", D_004142B0);

INCLUDE_RODATA(const s32, "game/code_00157A50", D_004142C0);

