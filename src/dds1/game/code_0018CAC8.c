#include "eff_resource_browser.h"
#include "sdf_sif_command.h"
#include "common.h"
#include "sdf_chip.h"
#include "kwln_sprite.h"
#include "sdf_packet_list.h"
#include "sdf_texture_draw_packet.h"
#include "eff_event_setup.h"
#include "sdf_resource.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"
#include "fpu.h"
#include "eff.h"
#include "eff_math.h"
#include "sdf_texture_file.h"

#define EFF_SUBWORK_PREFIX_BYTES 0x40
#define EFF_DIRECTORY_PATH_BYTES 0x70
#define EFF_BUILTIN_NAME_COUNT 0x2F
#define EFF_DIRECTORY_FLAG_CLEAR 0x1000
#define EFF_MATRIX_BYTES 0x40
#define EFF_VECTOR_WORD_COUNT 4
#define EFF_COLOR_UNPACK_SCALE_BITS 0x3C000000
#define EFF_SLOT_BYTES 0x38
#define EFF_SLOT_HEADER_BYTES 0xC

/* The SDK directory completion copies 0x140 bytes and one trailing word.
 * Names begin at 0x40; this transmitted record has ordinary word alignment. */
typedef struct EffDirEnt {
    u32 flags;      /* 0x00: bit 12 cleared */
    u8 pad04[0x3C]; /* 0x04 */
    char name[0x100]; /* 0x40: SDK directory-name buffer */
    void *privateData; /* 0x140 */
} EffDirEnt;
typedef char EffDirEnt_size_must_be_0x144[(sizeof(EffDirEnt) == 0x144) ? 1 : -1];

extern u8 sdfPadButtonStates[0x20];
extern s8 D_0039862B[];
extern SdfPoolNode kwlnDrawSurfaces[];
extern s32 sdfAllocPacketAligned(s32);
extern void *func_0011D3E8(s32, s32, s32, s32, s32, u32, u32);
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
extern s32 func_00310320(s32 directory, EffDirEnt *entry);
extern void sdfTexReleaseReferenceViaHandler(SdfTex *texture);
extern void dds3AdminSubmitModeRequest(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern s32 func_003101B8(s32 directory);
extern s32 func_003014F0(char *destination, const char *format, ...);
extern s32 sceDopen(void *arg0);
extern s32 sdfConsMeasurePacketWithHeader(s32 arg0);
extern void *sdfConsAllocateColumnPacket(s32 arg0);


extern EffCreateHandler D_00355730[];

extern u8 sdfViewMatrix[];

extern u8 sdfProjectionMatrix[];

extern u8 D_00324660[];
extern u8 sdfViewTargetVector[];
extern u8 sdfViewUpVector[];

extern void sdfPostmultiplyVuMatrixFromMemory(void *);


/* Allocate the dispatch owner before calling the selected payload constructor. */
EffWork *effAllocDispatch(s32 type, void *params) {
    EffWork *work = sdfAllocSizeClassBlock(sizeof(*work));
    void *payload = D_00355730[type].handler(params);

    work->type = type;
    work->payload = payload;
    return work;
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

/* Return the retained payload passed to this type's callbacks. */
void *effGetHandlerArg(EffWork *work) {
    return work->payload;
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
    void *handlerArg = NULL;

    switch (type) {
    case 0:
        handlerArg = work->payload;
        break;
    case 1:
        handlerArg = work->payload;
        break;
    case 2:
        handlerArg = (u8 *)work->payload + EFF_SUBWORK_PREFIX_BYTES;
        break;
    case 3:
        handlerArg = (u8 *)work->payload + EFF_SUBWORK_PREFIX_BYTES;
        break;
    case 4:
        handlerArg = (u8 *)work->payload + EFF_SUBWORK_PREFIX_BYTES;
        break;
    default:
        break;
    }
    effAllocDispatch(type, handlerArg);
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
void effRunIfEnabled(s32 directory) {
    if (sdfPfsDebugMode != 0) {
        func_003101B8(directory);
    }
}

/* Debug mode delegates to the native iterator. Built-in mode copies the next name,
   clears flag mask 0x1000, and returns name length; zero also marks table exhaustion. */
s32 effNextDataDirEntry(s32 directory, EffDirEnt *entry) {
    if (sdfPfsDebugMode != 0) {
        return func_00310320(directory, entry);
    }
    if ((u32)effDataDirectoryIndex >= EFF_BUILTIN_NAME_COUNT) {
        return 0;
    }
    strcpy(entry->name, D_003557A8[effDataDirectoryIndex]);
    entry->flags &= ~EFF_DIRECTORY_FLAG_CLEAR;
    effDataDirectoryIndex++;
    return strlen(entry->name);
}


EffResourceList *func_0018CF98(char *path, s32 flags) {
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




/* Copy the source count, duplicate its head pointer, and store the source-list pointer.
   Other descriptor words retain their native defaults; no retain operation occurs here. */
EffResourceDescriptor *effCreateResourceListDescriptor(EffResourceList *list) {
    EffResourceDescriptor *resource = sdfAllocSizeClassBlock(sizeof(*resource));

    resource->x = 0;
    resource->y = 0xC8;
    resource->result = 0;
    resource->resourceCount = list->resourceCount;
    resource->word10 = 0;
    resource->selectedIndex = 0;
    resource->visibleIndex = 0;
    resource->previewActive = 0;
    resource->repeatDelay = 0;
    resource->drawSurface = 0x53;
    resource->borderColor = 0x40806020;
    resource->fillColor = 0x30000000;
    resource->firstVisibleEntry = list->head;
    resource->selectedEntry = list->head;
    resource->cachedEntry = 0;
    resource->textureHandle = 0;
    resource->resourceList = list;
    return resource;
}


/* Navigate the resource list, update its held preview, and submit fifteen rows. */
s32 func_0018D4B8(EffResourceDescriptor *descriptor) {
    EffResourceListNode *visibleEntry;
    SdfListHead *packetList;
    SdfPoolNode *surface;
    s32 drawnRows;
    s32 contentRow;

    if (descriptor->repeatDelay != 0) {
        descriptor->repeatDelay--;
    } else if (descriptor->firstVisibleEntry == NULL) {
        if (D_0039862B[0] < 0) {
            descriptor->result = 2;
        }
    } else if (descriptor->result == 0) {
        EffResourceListNode *navigationEntry;
        EffResourceListNode *selectedEntry;
        s32 navigationStep;
        if ((sdfPadButtonStates[6] & 2) != 0) {
            if (descriptor->selectedEntry->previous != NULL) {
                descriptor->selectedEntry = descriptor->selectedEntry->previous;
                descriptor->selectedIndex--;
                if (descriptor->visibleIndex != 0) {
                    descriptor->visibleIndex--;
                }
                if (descriptor->selectedEntry == descriptor->firstVisibleEntry) {
                    if (descriptor->selectedEntry->previous != NULL) {
                        descriptor->firstVisibleEntry = descriptor->selectedEntry->previous;
                        descriptor->visibleIndex = 1;
                    }
                }
            }
        } else if ((sdfPadButtonStates[7] & 2) != 0) {
            if (descriptor->selectedEntry->next != NULL) {
                descriptor->selectedEntry = descriptor->selectedEntry->next;
                descriptor->selectedIndex++;
                descriptor->visibleIndex++;
                if (descriptor->visibleIndex == 0xE) {
                    if (descriptor->firstVisibleEntry->next != NULL) {
                        descriptor->firstVisibleEntry = descriptor->firstVisibleEntry->next;
                        descriptor->visibleIndex = 0xD;
                    }
                }
            }
        } else if ((sdfPadButtonStates[9] & 2) != 0) {
            selectedEntry = descriptor->selectedEntry;
            for (navigationStep = 0; navigationStep < 0xF; navigationStep++) {
                navigationEntry = selectedEntry->previous;
                if (navigationEntry != NULL) {
                    descriptor->selectedEntry = navigationEntry;
                    descriptor->selectedIndex--;
                    if (descriptor->visibleIndex != 0) {
                        descriptor->visibleIndex--;
                    }
                    selectedEntry = descriptor->selectedEntry;
                    if (selectedEntry == descriptor->firstVisibleEntry) {
                        EffResourceListNode *previous = selectedEntry->previous;
                        if (previous != NULL) {
                            descriptor->firstVisibleEntry = previous;
                            descriptor->visibleIndex = 1;
                        }
                    }
                }
            }
        } else if ((sdfPadButtonStates[11] & 2) != 0) {
            for (navigationStep = 0; navigationStep < 0xF; navigationStep++) {
                navigationEntry = descriptor->selectedEntry->next;
                if (navigationEntry != NULL) {
                    descriptor->selectedEntry = navigationEntry;
                    descriptor->selectedIndex++;
                    descriptor->visibleIndex++;
                    if (descriptor->visibleIndex == 0xE) {
                        EffResourceListNode *next = descriptor->firstVisibleEntry->next;
                        if (next != NULL) {
                            descriptor->firstVisibleEntry = next;
                            descriptor->visibleIndex = 0xD;
                        }
                    }
                }
            }
        } else if ((s8)sdfPadButtonStates[3] < 0) {
            descriptor->result = 2;
        } else if ((s8)sdfPadButtonStates[1] < 0) {
            descriptor->result = 1;
        } else {
            EffResourceListNode *cachedEntry = descriptor->cachedEntry;
            selectedEntry = descriptor->selectedEntry;
            if (cachedEntry != selectedEntry) {
                s32 category = selectedEntry->type;
                switch (category) {
                case 1: {
                    char resourceName[0x70];
                    func_003014F0(resourceName, D_003BB060,
                        descriptor->resourceList->directoryPath, selectedEntry->name);
                    effSetWorkTextureResource(descriptor, resourceName);
                    descriptor->previewActive = category;
                    selectedEntry = descriptor->selectedEntry;
                    break;
                }
                case 2:
                    descriptor->previewActive = 0;
                    break;
                case 4:
                    descriptor->previewActive = 0;
                    break;
                default:
                    descriptor->previewActive = 0;
                    break;
                }
                descriptor->cachedEntry = selectedEntry;
            }
            if (descriptor->previewActive != 0) {
                func_0018DA70(descriptor);
            }
        }
    }

    packetList = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(packetList);
    sdfAppendPacket(packetList, (u32)func_0011D3E8(
        0x8000 - (descriptor->x << 4), 0x8000 - (descriptor->y << 3),
        0xFEFFFF, 0xC00, 0x620, descriptor->fillColor, descriptor->borderColor));
    if (descriptor->previewActive != 0) {
        sdfAppendPacket(packetList, (u32)func_0011D3E8(
            0x8000 - (descriptor->x << 4),
            0x8000 - ((descriptor->y - 0xC4) << 3),
            0xFEFFFF, 0x800, 0x400, 0, 0x40806020));
    }
    contentRow = descriptor->y - 2;
    sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(
        0x8000 - ((descriptor->x - 2) << 4),
        0x8000 - (contentRow << 3), 0xFF0000, 5,
        descriptor->resourceList->directoryPath));
    contentRow -= 0xC;
    visibleEntry = descriptor->firstVisibleEntry;
    drawnRows = 0;
    while (visibleEntry != NULL) {
        s32 selectionFlags = visibleEntry == descriptor->selectedEntry ? 4 : 0;
        sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(
            0x8000 - ((descriptor->x - 2) << 4),
            0x8000 - (contentRow << 3), 0xFF0000,
            selectionFlags, visibleEntry->name));
        drawnRows++;
        contentRow -= 0xC;
        if (drawnRows >= 0xF) {
            break;
        }
        visibleEntry = visibleEntry->next;
    }
    surface = &kwlnDrawSurfaces[descriptor->drawSurface];
    surface->append((SdfListHead *)surface, packetList);
    return descriptor->result;
}


/* Release any retained texture reference, clear its handle, and free the work block. */
void effFreeWork(EffResourceDescriptor *work) {
    SdfTex *texture = work->textureHandle;

    if (texture != NULL) {
        sdfTexReleaseReferenceViaHandler(texture);
        work->textureHandle = NULL;
    }
    sdfReleaseChipBlock(work);
}

/* Set the browser screen coordinates. */
void effSetMsgHeader(EffResourceDescriptor *message, s32 first, s32 second) {
    message->x = first;
    message->y = second;
}


/* Return the selected resource index as its native word. */
u32 effGetWorkParam(EffResourceDescriptor *work) {
    return work->selectedIndex;
}

/* Return the browser selection result as its native word. */
u32 effGetWorkLink(EffResourceDescriptor *work) {
    return work->result;
}

/* Format the prefix/name records through the native template and return the name record's first word.
   Buffer capacity and record pointers remain unchecked. */
u32 effFormatMsgNames(EffResourceDescriptor *message, void *destination) {
    func_003014F0(destination, D_003BB060, message->resourceList->directoryPath, message->selectedEntry->name);
    return message->selectedEntry->type;
}

/* Set the navigation repeat delay. */
void effSetWorkFirst(EffResourceDescriptor *work, u32 value) {
    work->repeatDelay = value;
}

/* Select the draw surface for browser packets. */
void effSetWorkSecond(EffResourceDescriptor *work, u32 value) {
    work->drawSurface = value;
}

/* Set the browser border and fill colors. */
void effSetMsgPair(EffResourceDescriptor *message, u32 first, u32 second) {
    message->borderColor = first;
    message->fillColor = second;
}

/* Release the previous texture reference before loading/acquiring its replacement.
   Release the temporary loaded resource afterward; native failure results are unchecked. */
void effSetWorkTextureResource(EffResourceDescriptor *work, const char *textureResource) {
    SdfTex *texture;
    SdfMemBlock *loadedResource;
    u32 resourceWords[4];

    if (work->textureHandle != NULL) {
        sdfTexReleaseReferenceViaHandler(work->textureHandle);
        work->textureHandle = NULL;
    }
    loadedResource = sdfReadNamedResource(textureResource, resourceWords, 0);
    texture = sdfTexAcquireResourceTexture((SdfTextureFileHeader *)(resourceWords[0]));
    work->textureHandle = texture;
    sdfReleaseResourceAllocation(loadedResource);
}

extern SdfPoolNode D_003255A8;


/* Queue the overlay's texture as a quad at its screen position. */
void func_0018DA70(EffResourceDescriptor *overlay) {
    SdfListHead *list;
    void *sprite;
    KwlnSpriteVertex *vertex;
    SdfTex *texture;

    if (overlay->textureHandle != 0) {
        list = (SdfListHead *)sdfAllocPacketAligned(0x20);
        sdfInitPacketList(list);
        sprite = sdfConsAllocateColumnPacket(1);
        vertex = (KwlnSpriteVertex *)sdfConsMeasurePacketWithHeader((s32)sprite);
        vertex->a = 0x80;
        vertex->b = 0x80;
        vertex->g = 0x80;
        vertex->r = 0x80;
        texture = overlay->textureHandle;
        vertex->corner[1].u = texture->width << 4;
        vertex->corner[1].v = texture->height << 4;
        vertex->corner[0].u = 0;
        vertex->corner[0].v = 0;
        vertex->corner[1].flag = 0;
        vertex->corner[0].flag = 0;
        vertex->corner[0].x = 0x8000 - (overlay->x << 4);
        vertex->corner[0].y = 0x8000 - ((overlay->y - 0xC4) << 3);
        vertex->corner[0].mask = 0xFF0000;
        vertex->corner[1].x = vertex->corner[0].x + 0x800;
        vertex->corner[1].y = vertex->corner[0].y + 0x400;
        vertex->corner[1].mask = 0xFF0000;
        sdfConsCreateDrawPacket(list, texture, 0);
        sdfAppendPacket(list, (u32)sprite);
        D_003255A8.append((SdfListHead *)&D_003255A8, list);
    }
}


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
EffArrHdr *effAllocSlotArray(s32 count) {
    struct SdfMemBlock *allocation = sdfAllocGeneralBlock(count * EFF_SLOT_BYTES + EFF_SLOT_HEADER_BYTES);
    void *retainedAddress = (void *)sdfResourceRetainAddress(allocation);
    u32 slotIndex = 0;
    EffCubicBezierSlot *slot = retainedAddress;
    u8 *headerAddress = (u8 *)slot + count * EFF_SLOT_BYTES;

    ((EffArrHdr *)headerAddress)->allocation = allocation;
    ((EffArrHdr *)headerAddress)->slots = retainedAddress;
    ((EffArrHdr *)headerAddress)->unk4 = count;
    if (count != 0) {
        do {
            slotIndex++;
            slot->t = 0;
            slot->step = 0.05f;
            slot = (EffCubicBezierSlot *)((u8 *)slot + EFF_SLOT_BYTES);
        } while (slotIndex < count);
    }
    return (EffArrHdr *)headerAddress;
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

