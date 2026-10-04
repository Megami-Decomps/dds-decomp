#include "common.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"
#include "fpu.h"
#include "eff.h"

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

/* Directory entry filled by the effect data directory iterator. */
typedef struct EffDirEnt {
    u32 flags;      /* 0x00: bit 12 cleared */
    u8 pad04[0x3C]; /* 0x04 */
    char name[0x40]; /* 0x40 */
} EffDirEnt;

extern EffHandler D_00355734[];
extern EffHandler D_00355738[];
extern EffHandler D_0035573C[];
extern EffHandler D_00355740[];
extern EffHandler D_00355744[];
extern u8 sdfPfsDebugMode;
extern s8 D_003BB04D;
extern u32 effDataDirectoryIndex;
extern char D_003BB058[];
extern char D_003BB060[];
extern char *D_003557A8[];
extern s32 func_00310320(void);
extern u32 sdfTexAcquireResourceTexture(u32);
extern u64 sdfReadNamedResource(u64, u32 *, u64);
extern void sdfReleaseChipBlock(void *arg0);
extern void sdfTexReleaseReferenceViaHandler(s32 arg0);
extern void sdfReleaseResourceAllocation(u64 arg0);
extern void dds3AdminSubmitModeRequest(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void func_003101B8(void);
extern void func_003014F0();
extern s32 sceDopen(void *arg0);
extern void *sdfAllocSizeClassBlock(s32 arg0);
extern void sdfInitPacketList(s32 arg0);
extern void sdfConsCreateDrawPacket(s32 arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
extern void *sdfConsMeasurePacketWithHeader(s32 arg0);
extern s32 sdfConsAllocateColumnPacket(s32 arg0);
extern void sdfAppendPacket(s32 arg0, s32 arg1);


extern EffHandler32 D_00355730[];

extern u8 sdfViewMatrix[];

extern u8 sdfProjectionMatrix[];

extern u8 D_00324660[];
extern u8 sdfViewTargetVector[];
extern u8 sdfViewUpVector[];

extern void sdfPostmultiplyVuMatrixFromMemory(void *);


/* Allocate a result pair and store the selected callback's returned word.
   Type, allocation, and callback are trusted; allocation occurs before dispatch. */
EffResult *effAllocDispatch(s32 type, s32 handlerArg) {
    EffResult *result = sdfAllocSizeClassBlock(EFF_DISPATCH_RESULT_BYTES);
    s32 handlerResult = D_00355730[type].handler(handlerArg);

    result->unk0 = type;
    result->unk4 = handlerResult;
    return result;
}

/* Invoke the primary type callback with the payload; no NULL or type bounds check. */
void effTypeDispatch(EffWork *work) {
    D_00355734[work->type].handler(work->payload);
}

/* Invoke the release callback, then free the outer work; no callback NULL check. */
void effTypeDispatchFree(EffWork *work) {
    D_00355738[work->type].handler(work->payload);
    sdfReleaseChipBlock(work);
}

/* Return the raw payload word used as the type callback's argument. */
u32 effGetHandlerArg(EffWork *work) {
    return (u32)work->payload;
}

/* Read the pointed word without validating the pointer or asserting a record kind. */
u32 func_0018CBC0(u32 *word) {
    return *word;
}

void func_0018CBC8(void) {
}

u32 func_0018CBD0(void) {
    return 1;
}

/* Invoke the optional A callback when present; work and type index remain unchecked. */
void effTypeDispatchGuardedA(EffWork *work) {
    void (*handler)(void *) = D_0035573C[work->type].handler;

    if (handler != NULL) {
        handler(work->payload);
    }
}

/* Invoke the optional B callback when present; work and type index remain unchecked. */
void effTypeDispatchGuardedB(EffWork *work) {
    void (*handler)(void *) = D_00355744[work->type].handler;

    if (handler != NULL) {
        handler(work->payload);
    }
}

/* Invoke the optional C callback when present; work and type index remain unchecked. */
void effTypeDispatchGuardedC(EffWork *work) {
    void (*handler)(void *) = D_00355740[work->type].handler;

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

/* Read the type-specific subslot, or zero for other types.
   Retain the unused argument and DDS1's native unused byte declaration. */
u32 effGetSubSlot(EffWork *work, s32 unusedSlotValue) {
    u8 unusedValue = unusedSlotValue;
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
    /* Required to match: the effect type is passed in the handler's pointer-shaped slot. */
    effAllocDispatch((EffWork *)type, handlerArg);
}

void func_0018CDA0(void) {
}

void func_0018CDA8(void) {
    dds3AdminSubmitModeRequest(0, 0, 0, 0);
}

/* Return the callback word unchanged; its payload semantics are not established here. */
u32 func_0018CDD0(u32 value) {
    return value;
}

void func_0018CDD8(void) {
}

void func_0018CDE0(void) {
}

void func_0018CDE8(void) {
}

void func_0018CDF0(void) {
}

void func_0018CDF8(void) {
}

void func_0018CE00(void) {
}

void func_0018CE08(void) {
}

void func_0018CE10(void) {
}

void func_0018CE18(void) {
}

void func_0018CE20(void) {
}

void func_0018CE28(void) {
}

/* Return the callback word unchanged, retaining this entry's unsigned C surface. */
u32 func_0018CE30(u32 value) {
    return value;
}

void func_0018CE38(void) {
}

u32 func_0018CE40(void) {
    return 0;
}

u32 func_0018CE48(void) {
    return 0;
}

void func_0018CE50(void) {
}

u32 func_0018CE58(void) {
    return 0;
}

void func_0018CE60(void) {
}

s32 func_0018CE68(void) {
    return D_003BB04D;
}

/* Debug filesystem mode opens the formatted pfs0 directory; otherwise reset built-in iteration.
   Return the native directory-open result, or zero for built-in mode. */
s32 effOpenDataDir(void *name) {
    u8 path[EFF_DIRECTORY_PATH_BYTES];

    if (sdfPfsDebugMode != 0) {
        func_003014F0(path, D_003BB058, name);
        return sceDopen(path);
    } else {
        effDataDirectoryIndex = 0;
        return 0;
    }
}


/* Run the native directory callback only in debug filesystem mode; its result is ignored. */
void effRunIfEnabled(void) {
    if (sdfPfsDebugMode != 0) {
        func_003101B8();
    }
}

/* Debug mode delegates to the native iterator. Built-in mode copies the next name,
   clears flag mask 0x1000, and returns name length; zero also marks table exhaustion. */
s32 effNextDataDirEntry(s32 unused, EffDirEnt *entry) {
    if (sdfPfsDebugMode != 0) {
        return func_00310320();
    }
    if ((u32)effDataDirectoryIndex >= EFF_BUILTIN_NAME_COUNT) {
        return 0;
    }
    strcpy(entry->name, D_003557A8[effDataDirectoryIndex]);
    entry->flags &= ~EFF_DIRECTORY_FLAG_CLEAR;
    effDataDirectoryIndex++;
    return strlen(entry->name);
}


INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018CF98);

/* Free linked nodes, then the root's separate payload and the root itself.
   Save each successor before freeing; node payloads are not separately released here. */
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
    s32 resourceCount;
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

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018D4B8);

/* Release any retained texture reference, clear its handle, and free the work block. */
void effFreeWork(EffWork *work) {
    if (work->textureHandle != 0) {
        sdfTexReleaseReferenceViaHandler(work->textureHandle);
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
    func_003014F0(destination, D_003BB060, message->prefixRecord[1], message->nameRecord + 1);
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

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018DA70);


void func_0018DB88(void) {
    dds3AdminSubmitModeRequest(0, 0, 0, 0);
}

void func_0018DBB0(void) {
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
    VU0_LOAD_VF_MEMORY(vf11, D_00324660);
    VU0_ADD(vf10, vf10, vf11);
}

/* Project a point and its camera-right offset, returning their rounded screen distance. */
s32 func_0018DC58(f32 scale) {
    volatile f32 scaleVector[4];
    f32 projectedEnd[4];
    f32 projectedStart[4];
    f32 original[4];
    f32 dx;
    f32 dy;

    VU0_STORE_VF_UNCLOBBERED(vf10, original);
    sdfProjectVuVectorToScreen();
    VU0_STORE_VF(vf10, projectedStart);

    scaleVector[0] = scale;
    scaleVector[2] = scale;
    scaleVector[1] = scale;
    VU0_LOAD_VF(vf10, original);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf11, sdfViewTargetVector);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, sdfViewUpVector);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_LOAD_VF(vf11, scaleVector);
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
    u8 *headerAddress = (u8 *)slot + count * EFF_SLOT_BYTES;

    ((EffArrHdr *)headerAddress)->allocation = allocation;
    ((EffArrHdr *)headerAddress)->slots = retainedAddress;
    ((EffArrHdr *)headerAddress)->unk4 = count;
    if (count != 0) {
        do {
            slotIndex++;
            slot->unk30 = 0;
            slot->unk34 = 0.05f;
            slot = (EffSlot38 *)((u8 *)slot + EFF_SLOT_BYTES);
        } while (slotIndex < count);
    }
    return headerAddress;
}

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0F88);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0F98);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0FA8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0FB8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0FC8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0FD8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0FE8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0FF8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1008);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1018);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1028);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1038);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1048);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1058);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1068);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1078);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1088);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1098);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A10A8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A10B8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A10C8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A10D8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A10E8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A10F8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1108);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1118);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1128);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1138);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1148);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1158);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1168);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1178);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1188);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1198);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A11A8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A11B8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A11C8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A11D8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A11E8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A11F8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1208);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1218);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1228);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1238);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1248);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1258);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1270);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1280);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1290);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A12A0);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A12B0);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A12C0);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A12D0);

INCLUDE_SDATA(const s32, "game/code_0018CAC8", D_003BB050);

INCLUDE_SDATA(const s32, "game/code_0018CAC8", D_003BB058);

INCLUDE_SDATA(const s32, "game/code_0018CAC8", D_003BB060);

