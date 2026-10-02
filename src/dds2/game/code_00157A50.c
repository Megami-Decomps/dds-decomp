#include "common.h"
#include "eff.h"

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
extern void *func_00328D68(s32);

void *effCloneSourceWithTypeHandler(EffectSource *source) {
    EffectSource *copy = (EffectSource *)func_00328D68(0x10);
    u32 argument = source->argument;

    copy->type = source->type;
    copy->field4 = source->field4;
    copy->argument = D_003AA770[source->type].handler(argument);
    return copy;
}

INCLUDE_ASM(const s32, "game/code_00157A50", func_00157AC8);

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
extern EffBillResourceArchive *func_002C7FF0(const char *path);
extern void func_002C81D0(EffBillResourceArchive *archive);
extern void func_002C7CE8(EffBillResourceArchive *archive);
extern u32 sdfMemoryGetBlockAddress(MemBlock *block);
extern void sdfReleaseResourceAllocation(MemBlock *block);
extern BillObj *billCreateIndexed(s32 kind, u32 data);
extern void func_0035B6E0(const char *format, const char *value);

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
    archive = func_002C7FF0(D_004142B0);
    func_002C81D0(archive);
    node = archive->nodes;
    if (node != NULL) {
        configTable = (u8 *)D_003AA880;
        owner = effBillResourceOwners;
        scaleTable = (f32 *)((u8 *)D_003AA880 + 8);
        configOffset = 0;
        do {
            billboard = billCreateIndexed(((EffBillResourceInit *)(configOffset + (u32)configTable))->billboardKind,
                                          sdfMemoryGetBlockAddress(node->blockHandle));
            scale = (f32 *)(configOffset + (u32)scaleTable);
            configOffset += sizeof(EffBillResourceInit);
            *owner++ = billboard;
            payload = (BillChildPayload *)billboard->entryList;
            payload->halfWidth = payload->halfWidth * *scale;
            payload->halfHeight = payload->halfHeight * *scale;
            sdfReleaseResourceAllocation(node->blockHandle);
            node = node->next;
        } while (node != NULL);
    }
    func_002C7CE8(archive);
    func_0035B6E0(D_004142C0, D_004142B0);
}

extern void billDispatchByKind(BillObj *billboard);

void effBillDispatchAll(void) {
    u32 i;

    D_00438EFC = 0;
    for (i = 0; i < 15; i++) {
        billDispatchByKind(effBillResourceOwners[i]);
    }
}

INCLUDE_ASM(const s32, "game/code_00157A50", func_00157D38);

/* Texture record: +0x00 is the handle the reference is dropped from, +0x08 the
 * reference count and +0x44 the allocation released when it reaches zero. */
typedef struct TexRecord {
    void *texture;     /* 0x00 */
    u32 flags;         /* 0x04 */
    s32 refCount;      /* 0x08 */
    u8 pad0C[0x38];
    MemBlock *allocation;  /* 0x44 */
} TexRecord;

/* Drop one reference; the last one releases the texture and its allocation. */
void effReleaseSharedTextureRecord(TexRecord *entry) {
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

