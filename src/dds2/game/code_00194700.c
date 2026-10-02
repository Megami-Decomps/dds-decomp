#include "common.h"

extern s8 D_0043643D;

#include "eff.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"

extern u32 sdfTexAcquireResourceTexture(u32);

extern u64 sdfReadNamedResource(u64, u32 *, u64);

extern void *func_00328D68(s32 size);

extern EffHandler32 D_003B2060[];

extern EffHandler D_003B2064[];

extern EffHandler D_003B206C[];

extern EffHandler D_003B2074[];

extern EffHandler D_003B2070[];

extern u8 sdfPfsDebugMode;

extern u32 effDataDirectoryIndex;

extern char D_00436448[];

extern void func_0035C860();

extern s32 sceDopen(void *path);

extern void sdfReleaseChipBlock(void *allocation);

extern char D_00436450[];

extern EffHandler D_003B2068[];

extern void func_0036B420(void);

/* Directory entry filled by the effect data directory iterator. */
typedef struct EffDirEnt {
    u32 flags;      /* 0x00: bit 12 cleared */
    u8 pad04[0x3C]; /* 0x04 */
    char name[0x40]; /* 0x40 */
} EffDirEnt;

extern char *D_003B20D8[];

extern s32 func_0036B588(void);

extern u8 sdfViewMatrix[];

extern u8 sdfProjectionMatrix[];

extern u8 D_0037F660[];

extern void sdfPostmultiplyVuMatrixFromMemory(void *);

EffResult *effAllocDispatch(s32 kind, s32 input) {
    EffResult *result = func_00328D68(8);
    s32 value = D_003B2060[kind].handler(input);

    result->unk0 = kind;
    result->unk4 = value;
    return result;
}

void effTypeDispatch(EffWork *work) {
    D_003B2064[work->type].handler(work->payload);
}

void effTypeDispatchFree(EffWork *work) {
    D_003B2068[work->type].handler(work->payload);
    sdfReleaseChipBlock(work);
}

u32 effGetHandlerArg(EffWork *work) {
    return (u32)work->payload;
}

u32 func_001947F8(u32 *value) {
    return *value;
}

void func_00194800(void) {
}

u32 func_00194808(void) {
    return 1;
}

void effTypeDispatchGuardedA(EffWork *work) {
    void (*handler)(void *) = D_003B206C[work->type].handler;

    if (handler != NULL) {
        handler(work->payload);
    }
}

void effTypeDispatchGuardedB(EffWork *work) {
    void (*handler)(void *) = D_003B2074[work->type].handler;

    if (handler != NULL) {
        handler(work->payload);
    }
}

void effTypeDispatchGuardedC(EffWork *work) {
    void (*handler)(void *) = D_003B2070[work->type].handler;

    if (handler != NULL) {
        handler(work->payload);
    }
}

void effSetSubSlot(EffWork *work, s32 slot) {
    u8 slotByte = slot;
    u32 kind = work->type;

    switch (kind) {
    case 0:
        ((EffSub *)work->payload)->unk20 = slotByte;
        return;
    case 1:
        ((EffSub *)work->payload)->unk40 = slotByte;
        return;
    case 2:
        ((EffSub *)work->payload)->unk50 = slotByte;
        return;
    case 3:
        ((EffSub *)work->payload)->unk50 = slotByte;
        return;
    case 4:
        ((EffSub *)work->payload)->unk50 = slotByte;
        return;
    default:
        return;
    }
}

u32 effGetSubSlot(EffWork *work, s32 unused) {
    u32 kind = work->type;

    switch (kind) {
    case 0:
        return ((EffSub *)work->payload)->unk20;
    case 1:
        return ((EffSub *)work->payload)->unk40;
    case 2:
        return ((EffSub *)work->payload)->unk50;
    case 3:
        return ((EffSub *)work->payload)->unk50;
    case 4:
        return ((EffSub *)work->payload)->unk50;
    default:
        break;
    }
    return 0;
}

void effAllocSubWork(EffWork *work) {
    u32 kind = work->type;
    u32 handlerInput = 0;

    switch (kind) {
    case 0:
        handlerInput = work->payload;
        break;
    case 1:
        handlerInput = work->payload;
        break;
    case 2:
        handlerInput = work->payload + 0x40;
        break;
    case 3:
        handlerInput = work->payload + 0x40;
        break;
    case 4:
        handlerInput = work->payload + 0x40;
        break;
    default:
        break;
    }
    effAllocDispatch((EffWork *)kind, handlerInput);
}

void func_001949D8(void) {
}

void func_001949E0(void) {
    dds3AdminSubmitModeRequest(0, 0, 0, 0);
}

u32 func_00194A08(u32 value) {
    return value;
}

void func_00194A10(void) {
}

void func_00194A18(void) {
}

void func_00194A20(void) {
}

void func_00194A28(void) {
}

void func_00194A30(void) {
}

void func_00194A38(void) {
}

void func_00194A40(void) {
}

void func_00194A48(void) {
}

void func_00194A50(void) {
}

void func_00194A58(void) {
}

void func_00194A60(void) {
}

s32 effReturnCallbackValue(s32 value) {
    return value;
}

void func_00194A70(void) {
}

u32 func_00194A78(void) {
    return 0;
}

u32 func_00194A80(void) {
    return 0;
}

void func_00194A88(void) {
}

s32 func_00194A90(void) {
    return 0;
}

void func_00194A98(void) {
}

s8 func_00194AA0(void) {
    return D_0043643D;
}

/* An enabled disc directory uses sceDopen; otherwise iteration uses
 * the built-in name table and starts over at entry zero. */
s32 effOpenDataDir(void *name) {
    u8 path[0x70];

    if (sdfPfsDebugMode != 0) {
        func_0035C860(path, D_00436448, name);
        return sceDopen(path);
    } else {
        effDataDirectoryIndex = 0;
        return 0;
    }
}

void effRunIfEnabled(void) {
    if (sdfPfsDebugMode != 0) {
        func_0036B420();
    }
}

/* In built-in mode the name table supplies entries instead of the
 * directory iterator; clearing 0x1000 marks a synthesized entry. */
s32 effNextDataDirEntry(s32 unused, EffDirEnt *entry) {
    if (sdfPfsDebugMode != 0) {
        return func_0036B588();
    }
    if ((u32)effDataDirectoryIndex >= 0x2F) {
        return 0;
    }
    strcpy(entry->name, D_003B20D8[effDataDirectoryIndex]);
    entry->flags &= ~0x1000;
    effDataDirectoryIndex++;
    return strlen(entry->name);
}

INCLUDE_ASM(const s32, "game/code_00194700", func_00194BD0);

void effFreeWorkList(EffWork *root) {
    EffWork *node = (EffWork *)root->listHead;

    if (node != NULL) {
        do {
            EffWork *next = node->next;
            sdfReleaseChipBlock(node);
            node = next;
        } while (node != NULL);
    }
    sdfReleaseChipBlock(root->payload);
    sdfReleaseChipBlock(root);
}

typedef struct EffResourceList {
    s32 count;
    s32 unk04;
    void *head;
} EffResourceList;

typedef struct EffResourceDescriptor {
    u32 word00;
    u32 word04;
    u32 word08;
    s32 resourceCount;
    u32 word10[5];
    u32 word24;
    u32 word28;
    u32 word2C;
    void *word30;
    void *word34;
    u32 word38;
    u32 word3C;
    EffResourceList *resourceList; /* 0x40: source list retained by descriptor */
} EffResourceDescriptor;

/* Build a resource descriptor with the list's count and head. */
EffResourceDescriptor *effCreateResourceListDescriptor(EffResourceList *list) {
    EffResourceDescriptor *resource = func_00328D68(0x44);

    resource->word00 = 0;
    resource->word04 = 0xC8;
    resource->word08 = 0;
    resource->resourceCount = list->count;
    resource->word10[0] = 0;
    resource->word10[1] = 0;
    resource->word10[2] = 0;
    resource->word10[3] = 0;
    resource->word10[4] = 0;
    resource->word24 = 0x53;
    resource->word28 = 0x40806020;
    resource->word2C = 0x30000000;
    resource->word30 = list->head;
    resource->word34 = list->head;
    resource->word38 = 0;
    resource->word3C = 0;
    resource->resourceList = list;
    return resource;
}

INCLUDE_ASM(const s32, "game/code_00194700", func_001950F0);

void effFreeWork(EffWork *work) {
    s32 textureHandle;

    textureHandle = work->textureHandle;
    if (textureHandle != 0) {
        sdfTexReleaseReferenceViaHandler(textureHandle);
        work->textureHandle = 0;
    }
    sdfReleaseChipBlock(work);
}

void effSetMsgHeader(EffMsg *message, s32 first, s32 second) {
    message->unk0 = first;
    message->unk4 = second;
}

u32 effGetWorkParam(EffWork *work) {
    return work->unk14;
}

u32 effGetWorkLink(EffWork *work) {
    return work->listHead;
}

u32 effFormatMsgNames(EffMsg *message, void *destination) {
    func_0035C860(destination, D_00436450, message->prefixRecord[1], message->nameRecord + 1);
    return *message->nameRecord;
}

void effSetWorkFirst(EffWork *work, u32 value) {
    work->unk20 = value;
}

void effSetWorkSecond(EffWork *work, u32 value) {
    work->unk24 = value;
}

void effSetMsgPair(EffMsg *message, u32 first, u32 second) {
    message->unk28 = first;
    message->unk2C = second;
}

void effSetWorkTextureResource(EffWork *work, u64 resource) {
    u32 textureHandle;
    u64 allocation;
    u32 resourceData[4];

    if (work->textureHandle != 0) {
        sdfTexReleaseReferenceViaHandler(work->textureHandle);
        work->textureHandle = 0;
    }
    allocation = sdfReadNamedResource(resource, resourceData, 0);
    textureHandle = sdfTexAcquireResourceTexture(resourceData[0]);
    work->textureHandle = textureHandle;
    sdfReleaseResourceAllocation(allocation);
}

INCLUDE_ASM(const s32, "game/code_00194700", func_001956A8);

void func_001957C0(void) {
    dds3AdminSubmitModeRequest(0, 0, 0, 0);
}

void func_001957E8(void) {
    dds3AdminSubmitModeRequest(0, 0, 0, 0);
}

void sdfProjectVuVectorToScreen(void) {
    u8 *matrix;
    VU0_LOAD_MATRIX(sdfViewMatrix);
    matrix = sdfProjectionMatrix;
    sdfPostmultiplyVuMatrixFromMemory(matrix);
    VU0_TRANSFORM_POINT(vf10, vf10);
    VU0_PERSPECTIVE_DIVIDE_VF10();
    matrix += 0x40;
    VU0_LOAD_VF_MEMORY(vf11, matrix);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF_MEMORY(vf11, D_0037F660);
    VU0_ADD(vf10, vf10, vf11);
}

INCLUDE_ASM(const s32, "game/code_00194700", func_00195890);

/* vu0 routine: blend two RGBA8888 colours by t (lerp in float, packed back to RGBA8888) */
u32 effBlendColor(u32 colorA, u32 colorB, f32 t) {
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;

    if (t >= 1.0f) {
        return colorB;
    }
    unit = 0x3C000000;
    color1[0] = colorB;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = colorA;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_SCALAR_OP_CLOBBER(1.0f - t, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALAR_OP_R3_CLOBBER(t, "vmulx.xyzw vf11, vf11, vf2x");
    VU0_ADD(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;

    return packed;
}

/* vu0 routine: modulate two RGBA8888 colours, (a/128 * b/128) * 128 per channel */
u32 effMultiplyPackedColors(u32 colorA, u32 colorB) {
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit = 0x3C000000;
    color1[0] = colorA;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = colorB;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    return blended[0];
}

/* vu0 routine: distance from `point` to the line through `origin` along the unit vector `direction`. */
f32 effPointToLineDistance(f32 *direction, f32 *origin, f32 *point) {
    f32 projection[4];
    f32 offset[4];
    f32 along;
    f32 distance;

    VU0_LOAD_VF(vf10, point);
    VU0_LOAD_VF(vf11, origin);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, offset);
    VU0_LOAD_VF(vf11, direction);
    VU0_DOT_XYZ(along, vf10, vf11);
    projection[0] = direction[0] * along;
    projection[1] = direction[1] * along;
    projection[2] = direction[2] * along;
    VU0_LOAD_VF(vf10, offset);
    VU0_LOAD_VF(vf11, projection);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(distance);
    return distance;
}

void *effAllocSlotArray(s32 count) {
    void *allocation = sdfAllocGeneralBlock(count * 0x38 + 0xC);
    void *slotBase = sdfResourceRetainAddress(allocation);
    u32 index = 0;
    EffSlot38 *slot = slotBase;
    u8 *end = (u8 *)(slot + count);

    ((EffArrHdr *)end)->allocation = allocation;
    ((EffArrHdr *)end)->slots = slotBase;
    ((EffArrHdr *)end)->unk4 = count;
    if (count != 0) {
        do {
            index++;
            slot->unk30 = 0;
            slot->unk34 = 0.05f;
            slot++;
        } while (index < count);
    }
    return end;
}

INCLUDE_RODATA(const s32, "game/code_00194700", D_004146A8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004146B8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004146C8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004146D8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004146E8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004146F8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414708);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414718);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414728);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414738);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414748);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414758);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414768);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414778);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414788);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414798);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004147A8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004147B8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004147C8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004147D8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004147E8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004147F8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414808);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414818);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414828);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414838);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414848);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414858);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414868);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414878);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414888);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414898);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004148A8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004148B8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004148C8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004148D8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004148E8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004148F8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414908);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414918);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414928);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414938);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414948);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414958);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414968);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414978);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414990);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004149A0);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004149B0);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004149C0);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004149D0);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004149E0);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004149F0);

INCLUDE_SDATA(const s32, "game/code_00194700", D_00436440);

INCLUDE_SDATA(const s32, "game/code_00194700", D_00436448);

INCLUDE_SDATA(const s32, "game/code_00194700", D_00436450);

