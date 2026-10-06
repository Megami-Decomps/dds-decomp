#include "common.h"

extern s8 D_0043643D;

#include "eff.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"
#include "fpu.h"

#define EFF_DISPATCH_RESULT_BYTES 8
#define EFF_SUBWORK_PREFIX_BYTES 0x40
#define EFF_DIRECTORY_PATH_BYTES 0x70
#define EFF_BUILTIN_NAME_COUNT 0x2F
#define EFF_DIRECTORY_FLAG_CLEAR 0x1000
#define EFF_RESOURCE_DESCRIPTOR_BYTES 0x44
#define EFF_MATRIX_BYTES 0x40
#define EFF_VECTOR_WORD_COUNT 4
#define EFF_COLOR_UNPACK_SCALE_BITS 0x3C000000
#define EFF_SLOT_BYTES 0x38
#define EFF_SLOT_HEADER_BYTES 0xC

extern u32 sdfTexAcquireResourceTexture(u32);

extern u64 sdfReadNamedResource(u64, u32 *, u64);

extern void *sdfAllocSizeClassBlock(s32 size);

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

extern s32 func_0036B420(s32 directory);

/* The SDK directory completion copies 0x140 bytes and one trailing word.
 * Names begin at 0x40; this transmitted record has ordinary word alignment. */
typedef struct EffDirEnt {
    u32 flags;      /* 0x00: bit 12 cleared */
    u8 pad04[0x3C]; /* 0x04 */
    char name[0x100]; /* 0x40: SDK directory-name buffer */
    void *privateData; /* 0x140 */
} EffDirEnt;
typedef char EffDirEnt_size_must_be_0x144[(sizeof(EffDirEnt) == 0x144) ? 1 : -1];

extern char *D_003B20D8[];

extern s32 func_0036B588(s32 directory, EffDirEnt *entry);

extern u8 sdfViewMatrix[];

extern u8 sdfProjectionMatrix[];

extern u8 D_0037F660[];
extern u8 sdfViewTargetVector[];
extern u8 sdfViewUpVector[];

extern void sdfPostmultiplyVuMatrixFromMemory(void *);

/* Allocate a result pair and store the selected callback's returned word.
   Type, allocation, and callback are trusted; allocation occurs before dispatch. */
EffResult *effAllocDispatch(s32 type, s32 handlerArg) {
    EffResult *result = sdfAllocSizeClassBlock(EFF_DISPATCH_RESULT_BYTES);
    s32 handlerResult = D_003B2060[type].handler(handlerArg);

    result->unk0 = type;
    result->unk4 = handlerResult;
    return result;
}

/* Invoke the primary type callback with the payload; no NULL or type bounds check. */
void effTypeDispatch(EffWork *work) {
    D_003B2064[work->type].handler(work->payload);
}

/* Invoke the release callback, then free the outer work; no callback NULL check. */
void effTypeDispatchFree(EffWork *work) {
    D_003B2068[work->type].handler(work->payload);
    sdfReleaseChipBlock(work);
}

/* Return the raw payload word used as the type callback's argument. */
u32 effGetHandlerArg(EffWork *work) {
    return (u32)work->payload;
}

/* Read the pointed word without validating the pointer or asserting a record kind. */
u32 func_001947F8(u32 *word) {
    return *word;
}

void func_00194800(void) {
}

u32 func_00194808(void) {
    return 1;
}

/* Invoke the optional A callback when present; work and type index remain unchecked. */
void effTypeDispatchGuardedA(EffWork *work) {
    void (*handler)(void *) = D_003B206C[work->type].handler;

    if (handler != NULL) {
        handler(work->payload);
    }
}

/* Invoke the optional B callback when present; work and type index remain unchecked. */
void effTypeDispatchGuardedB(EffWork *work) {
    void (*handler)(void *) = D_003B2074[work->type].handler;

    if (handler != NULL) {
        handler(work->payload);
    }
}

/* Invoke the optional C callback when present; work and type index remain unchecked. */
void effTypeDispatchGuardedC(EffWork *work) {
    void (*handler)(void *) = D_003B2070[work->type].handler;

    if (handler != NULL) {
        handler(work->payload);
    }
}

/* Store the low byte of slotValue at the type-specific subslot.
   Types two through four share the same location; other types are left unchanged. */
void effSetSubSlot(EffWork *work, s32 slotValue) {
    u8 slotByte = slotValue;
    u32 type = work->type;

    switch (type) {
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

/* Read the type-specific subslot, or zero for other types; retain the unused argument. */
u32 effGetSubSlot(EffWork *work, s32 unusedSlotValue) {
    u32 type = work->type;

    switch (type) {
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

/* Types zero/one dispatch from the payload base; two through four skip its prefix.
   Other types still dispatch with a zero argument; no type bounds check is added. */
void effAllocSubWork(EffWork *work) {
    u32 type = work->type;
    u32 handlerArg = 0;

    switch (type) {
    case 0:
        handlerArg = work->payload;
        break;
    case 1:
        handlerArg = work->payload;
        break;
    case 2:
        handlerArg = work->payload + EFF_SUBWORK_PREFIX_BYTES;
        break;
    case 3:
        handlerArg = work->payload + EFF_SUBWORK_PREFIX_BYTES;
        break;
    case 4:
        handlerArg = work->payload + EFF_SUBWORK_PREFIX_BYTES;
        break;
    default:
        break;
    }
    effAllocDispatch((EffWork *)type, handlerArg);
}

void func_001949D8(void) {
}

void func_001949E0(void) {
    dds3AdminSubmitModeRequest(0, 0, 0, 0);
}

/* Return the callback word unchanged; its payload semantics are not established here. */
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

/* Return the callback word unchanged, retaining this entry's signed C surface. */
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

/* Debug filesystem mode opens the formatted pfs0 directory; otherwise reset built-in iteration.
   Return the native directory-open result, or zero for built-in mode. */
s32 effOpenDataDir(void *name) {
    u8 path[EFF_DIRECTORY_PATH_BYTES];

    if (sdfPfsDebugMode != 0) {
        func_0035C860(path, D_00436448, name);
        return sceDopen(path);
    } else {
        effDataDirectoryIndex = 0;
        return 0;
    }
}

/* Run the native directory callback only in debug filesystem mode; its result is ignored. */
void effRunIfEnabled(s32 directory) {
    if (sdfPfsDebugMode != 0) {
        func_0036B420(directory);
    }
}

/* Debug mode delegates to the native iterator. Built-in mode copies the next name,
   clears flag mask 0x1000, and returns name length; zero also marks table exhaustion. */
s32 effNextDataDirEntry(s32 directory, EffDirEnt *entry) {
    if (sdfPfsDebugMode != 0) {
        return func_0036B588(directory, entry);
    }
    if ((u32)effDataDirectoryIndex >= EFF_BUILTIN_NAME_COUNT) {
        return 0;
    }
    strcpy(entry->name, D_003B20D8[effDataDirectoryIndex]);
    entry->flags &= ~EFF_DIRECTORY_FLAG_CLEAR;
    effDataDirectoryIndex++;
    return strlen(entry->name);
}

/* Directory list roots and filename nodes have distinct allocation extents. */
typedef struct EffResourceListNode {
    u32 type;
    char name[0x30];
    struct EffResourceListNode *previous;
    struct EffResourceListNode *next;
} EffResourceListNode;

typedef struct EffResourceList {
    s32 resourceCount;
    char *directoryPath;
    EffResourceListNode *head;
} EffResourceList;
typedef char EffResourceList_size_must_be_12[(sizeof(EffResourceList) == 12) ? 1 : -1];
typedef char EffResourceListNode_size_must_be_60[(sizeof(EffResourceListNode) == 60) ? 1 : -1];

EffResourceList *func_00194BD0(char *path, s32 flags) {
    EffDirEnt entry;
    s32 directory;
    EffResourceList *list;
    EffResourceListNode *node;
    EffResourceListNode *tail;
    s32 length;
    s32 type;
    directory = effOpenDataDir(path);
    list = sdfAllocSizeClassBlock(sizeof(*list));
    if (sdfPfsDebugMode != 0) {
        list->directoryPath = sdfAllocSizeClassBlock(strlen(path) + 1);
        strcpy(list->directoryPath, path);
    } else {
        list->directoryPath = sdfAllocSizeClassBlock(strlen(path) + 1);
        strcpy(list->directoryPath, path);
    }
    tail = NULL;
    list->head = NULL;
    list->resourceCount = 0;
    while ((length = effNextDataDirEntry(directory, &entry)) > 0) {
        if ((entry.flags & 0xF000) == 0x1000) continue;
        if (length < 5) continue;
        if (length >= 0x2F) continue;
        if ((flags & 0x10 ? entry.name[length - 3] : entry.name[length - 4]) != '.') continue;
        type = 0;
        if ((flags & 1) &&
            ((entry.name[length-3]=='t' && entry.name[length-2]=='m' && entry.name[length-1]=='x') ||
             (entry.name[length-3]=='T' && entry.name[length-2]=='M' && entry.name[length-1]=='X'))) type = 1;
        else if ((flags & 2) &&
            ((entry.name[length-3]=='p' && entry.name[length-2]=='2' && entry.name[length-1]=='a') ||
             (entry.name[length-3]=='P' && entry.name[length-2]=='2' && entry.name[length-1]=='A'))) type = 2;
        else if ((flags & 4) &&
            ((entry.name[length-3]=='d' && entry.name[length-2]=='3' && entry.name[length-1]=='p') ||
             (entry.name[length-3]=='D' && entry.name[length-2]=='3' && entry.name[length-1]=='P'))) type = 4;
        else if ((flags & 8) &&
            ((entry.name[length-3]=='b' && entry.name[length-2]=='e' && entry.name[length-1]=='d') ||
             (entry.name[length-3]=='B' && entry.name[length-2]=='E' && entry.name[length-1]=='D'))) type = 8;
        else if ((flags & 0x10) &&
            ((entry.name[length-2]=='p' && entry.name[length-1]=='b') ||
             (entry.name[length-2]=='P' && entry.name[length-1]=='B'))) type = 0x10;
        else if ((flags & 0x20) &&
            ((entry.name[length-3]=='p' && entry.name[length-2]=='c' && entry.name[length-1]=='f') ||
             (entry.name[length-3]=='P' && entry.name[length-2]=='C' && entry.name[length-1]=='F'))) type = 0x20;
        if (type == 0) continue;
        node = sdfAllocSizeClassBlock(sizeof(*node));
        node->type = type;
        strcpy(node->name, entry.name);
        if (tail != NULL) tail->next = node;
        else list->head = node;
        node->next = NULL;
        node->previous = tail;
        list->resourceCount++;
        tail = node;
    }
    effRunIfEnabled(directory);
    return list;
}


/* Free filename nodes, the copied directory path, then the list root.
   Each filename is inline; save the next link before releasing its node. */
void effFreeWorkList(EffResourceList *root) {
    EffResourceListNode *node = root->head;

    if (node != NULL) {
        do {
            EffResourceListNode *next = node->next;
            sdfReleaseChipBlock(node);
            node = next;
        } while (node != NULL);
    }
    sdfReleaseChipBlock(root->directoryPath);
    sdfReleaseChipBlock(root);
}



typedef struct EffResourceDescriptor {
    u32 word00;
    u32 word04;
    u32 word08;
    s32 resourceCount;
    u32 word10[5];
    u32 word24;
    u32 word28;
    u32 word2C;
    EffResourceListNode *word30;
    EffResourceListNode *word34;
    u32 word38;
    u32 word3C;
    EffResourceList *resourceList; /* 0x40: source list pointer copied into descriptor */
} EffResourceDescriptor;

/* Copy the source count, duplicate its head pointer, and store the source-list pointer.
   Other descriptor words retain their native defaults; no retain operation occurs here. */
EffResourceDescriptor *effCreateResourceListDescriptor(EffResourceList *list) {
    EffResourceDescriptor *resource = sdfAllocSizeClassBlock(EFF_RESOURCE_DESCRIPTOR_BYTES);

    resource->word00 = 0;
    resource->word04 = 0xC8;
    resource->word08 = 0;
    resource->resourceCount = list->resourceCount;
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

/* Release any retained texture reference, clear its handle, and free the work block. */
void effFreeWork(EffWork *work) {
    s32 textureHandle;

    textureHandle = work->textureHandle;
    if (textureHandle != 0) {
        sdfTexReleaseReferenceViaHandler(textureHandle);
        work->textureHandle = 0;
    }
    sdfReleaseChipBlock(work);
}

/* Store two opaque message-header words; neither meaning is established by this setter. */
void effSetMsgHeader(EffMsg *message, s32 first, s32 second) {
    message->unk0 = first;
    message->unk4 = second;
}

/* Return the opaque work parameter word unchanged. */
u32 effGetWorkParam(EffWork *work) {
    return work->unk14;
}

/* Return the raw list-head word, without traversing or retaining the list. */
u32 effGetWorkLink(EffWork *work) {
    return work->listHead;
}

/* Format the prefix/name records through the native template and return the name record's first word.
   Buffer capacity and record pointers remain unchecked. */
u32 effFormatMsgNames(EffMsg *message, void *destination) {
    func_0035C860(destination, D_00436450, message->prefixRecord[1], message->nameRecord + 1);
    return *message->nameRecord;
}

/* Store the first opaque work word; leave its field unnamed until a reader establishes meaning. */
void effSetWorkFirst(EffWork *work, u32 value) {
    work->unk20 = value;
}

/* Store the second opaque work word without interpreting it. */
void effSetWorkSecond(EffWork *work, u32 value) {
    work->unk24 = value;
}

/* Store the message's two opaque pair words without narrowing or interpretation. */
void effSetMsgPair(EffMsg *message, u32 first, u32 second) {
    message->unk28 = first;
    message->unk2C = second;
}

/* Release the previous texture reference before loading/acquiring its replacement.
   Release the temporary loaded resource afterward; native failure results are unchecked. */
void effSetWorkTextureResource(EffWork *work, u64 textureResource) {
    u32 textureHandle;
    u64 loadedResource;
    u32 resourceWords[4];

    if (work->textureHandle != 0) {
        sdfTexReleaseReferenceViaHandler(work->textureHandle);
        work->textureHandle = 0;
    }
    loadedResource = sdfReadNamedResource(textureResource, resourceWords, 0);
    textureHandle = sdfTexAcquireResourceTexture(resourceWords[0]);
    work->textureHandle = textureHandle;
    sdfReleaseResourceAllocation(loadedResource);
}

INCLUDE_ASM(const s32, "game/code_00194700", func_001956A8);


void func_001957C0(void) {
    dds3AdminSubmitModeRequest(0, 0, 0, 0);
}

void func_001957E8(void) {
    dds3AdminSubmitModeRequest(0, 0, 0, 0);
}

/* vu0 routine: transform the point in vf10 by view/projection, divide by w, then scale/bias.
   No clipping or zero-w check is performed; the result remains in vf10. */
void sdfProjectVuVectorToScreen(void) {
    u8 *projectionData;
    VU0_LOAD_MATRIX(sdfViewMatrix);
    projectionData = sdfProjectionMatrix;
    sdfPostmultiplyVuMatrixFromMemory(projectionData);
    VU0_TRANSFORM_POINT(vf10, vf10);
    VU0_PERSPECTIVE_DIVIDE_VF10();
    projectionData += EFF_MATRIX_BYTES;
    VU0_LOAD_VF_MEMORY(vf11, projectionData);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF_MEMORY(vf11, D_0037F660);
    VU0_ADD(vf10, vf10, vf11);
}

/* vu0 routine: project a point and its camera-right offset, returning their rounded screen distance. */
s32 effMeasureCameraRightScreenOffsetVU(f32 scale) {
    f32 scaleVector[4];
    f32 projectedEnd[4];
    f32 projectedStart[4];
    f32 original[4];
    f32 dx;
    f32 dy;

    VU0_STORE_VF_UNCLOBBERED(vf10, original);
    sdfProjectVuVectorToScreen();
    VU0_STORE_VF(vf10, projectedStart);

    scaleVector[0] = scaleVector[1] = scaleVector[2] = scale;
    VU0_LOAD_VF(vf10, original);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf11, sdfViewTargetVector);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, sdfViewUpVector);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_LOAD_VF_MEMORY(vf11, scaleVector);
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf11, vf12);
    VU0_ADD(vf10, vf10, vf11);

    sdfProjectVuVectorToScreen();
    VU0_STORE_VF(vf10, projectedEnd);
    dx = projectedEnd[0] - projectedStart[0];
    dy = projectedEnd[1] - projectedStart[1];
    VU0_LOAD_VF(vf10, projectedStart);
    return (s32)fsqrtf(dx * dx + dy * dy);
}

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

/* vu0 routine: multiply RGBA channels normalized by 128, then pack at the same scale.
   Native integer conversion/byte packing remains unclamped; quadword storage is unchanged. */
u32 effMultiplyPackedColors(u32 colorA, u32 colorB) {
    s32 colorAWords[EFF_VECTOR_WORD_COUNT];
    s32 colorBWords[EFF_VECTOR_WORD_COUNT];
    s32 packedWords[EFF_VECTOR_WORD_COUNT];
    u32 packed;
    u32 unpackScaleBits = EFF_COLOR_UNPACK_SCALE_BITS;
    colorAWords[0] = colorA;
    EE_MMI_RGBA_UNPACK(colorAWords, unpackScaleBits);
    VU0_MOVE_VF(vf11, vf10);
    colorBWords[0] = colorB;
    EE_MMI_RGBA_UNPACK(colorBWords, unpackScaleBits);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK(packed);
    packedWords[0] = packed;
    return packedWords[0];
}

/* vu0 routine: distance from point to the line through origin along a unit direction.
   Direction is not normalized here; dot/length use xyz despite full quadword loads. */
f32 effPointToLineDistance(f32 *direction, f32 *origin, f32 *point) {
    f32 projectedOffset[EFF_VECTOR_WORD_COUNT];
    f32 pointOffset[EFF_VECTOR_WORD_COUNT];
    f32 projectionAmount;
    f32 distance;

    VU0_LOAD_VF(vf10, point);
    VU0_LOAD_VF(vf11, origin);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, pointOffset);
    VU0_LOAD_VF(vf11, direction);
    VU0_DOT_XYZ(projectionAmount, vf10, vf11);
    /* The fourth projection component remains unwritten: only xyz contributes to distance. */
    projectedOffset[0] = direction[0] * projectionAmount;
    projectedOffset[1] = direction[1] * projectionAmount;
    projectedOffset[2] = direction[2] * projectionAmount;
    VU0_LOAD_VF(vf10, pointOffset);
    VU0_LOAD_VF(vf11, projectedOffset);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(distance);
    return distance;
}

/* Allocate/retain slot storage and return its header after the slots, not the slot base.
   Only the two native slot defaults are initialized; count/allocation validity is unchecked. */
void *effAllocSlotArray(s32 count) {
    void *allocation = sdfAllocGeneralBlock(count * EFF_SLOT_BYTES + EFF_SLOT_HEADER_BYTES);
    void *retainedAddress = sdfResourceRetainAddress(allocation);
    u32 slotIndex = 0;
    EffSlot38 *slot = retainedAddress;
    u8 *headerAddress = (u8 *)(slot + count);

    ((EffArrHdr *)headerAddress)->allocation = allocation;
    ((EffArrHdr *)headerAddress)->slots = retainedAddress;
    ((EffArrHdr *)headerAddress)->unk4 = count;
    if (count != 0) {
        do {
            slotIndex++;
            slot->unk30 = 0;
            slot->unk34 = 0.05f;
            slot++;
        } while (slotIndex < count);
    }
    return headerAddress;
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

