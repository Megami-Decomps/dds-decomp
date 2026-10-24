#include "sdk_ps2.h"
#include "eff_resource_browser.h"
#include "sdf_sif_command.h"
#include "common.h"
#include "sdf_vu0.h"
#include "sdf_vu_matrix.h"
#include "sdf_chip.h"
#include "kwln_sprite.h"
#include "sdf_packet_list.h"
#include "sdf_column_packet_api.h"
#include "eff_event_setup.h"
#include "sdf_resource.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"
#include "fpu.h"
#include "eff.h"
#include "eff_magatuhi_api.h"
#include "eff_math.h"
#include "sdf_texture_file.h"
#include "sdf_projection.h"
#include "eff_dirent.h"

#include "sdf.h"
#define EFF_SUBWORK_PREFIX_BYTES 0x40
#define EFF_DIRECTORY_PATH_BYTES 0x70
#define EFF_BUILTIN_NAME_COUNT 0x2F
#define EFF_DIRECTORY_FLAG_CLEAR 0x1000
#define EFF_MATRIX_BYTES 0x40
#define EFF_VECTOR_WORD_COUNT 4
#define EFF_COLOR_UNPACK_SCALE_BITS 0x3C000000
#define EFF_SLOT_BYTES 0x38
#define EFF_SLOT_HEADER_BYTES 0xC

extern s8 dds3PadBitTable[0x20];
extern s8 D_0039862B[];
extern SdfPoolNode sdfGrOtTable[];
#include "sdf_packet_alloc.h"
#include "sdf_bordered_rectangle_packet.h"
extern EffHandler D_00355734[];
extern EffHandler D_00355738[];
extern EffHandler D_0035573C[];
extern EffHandler D_00355740[];
extern EffHandler D_00355744[];
extern u8 sdfHddValid;
extern s8 D_003BB04D;
extern u32 effDataDirectoryIndex;
extern char D_003BB058[];
extern char D_003BB060[];
extern char *D_003557A8[];
extern void dds3AdminiCallSequence(s32 requestedMode, const void *requestData,
                                      u32 dataBytes, s32 markHistory);
extern u8 *sdf2DGetPrimPtr(SdfDrawPacket *packet);


extern EffCreateHandler D_00355730[];

extern u8 sdfViewMatrix[];

extern u8 sdfProjectionMatrix[];

extern u8 D_00324660[];
extern u8 sdfViewTargetVector[];
extern u8 aViewDown[];



/* Allocate the dispatch owner before calling the selected payload constructor. */
EffWork *dds3MagatuhiParCreate(s32 type, void *params) {
    EffWork *work = sdfChipAllocDirect(sizeof(*work));
    void *payload = D_00355730[type].handler(params);

    work->type = type;
    work->payload = payload;
    return work;
}

/* Invoke the primary type callback with the payload; no NULL or type bounds check. */
void dds3MagatuhiParDraw(EffWork *work) {
    D_00355734[work->type].handler(work->payload);
}

/* Invoke the release callback, then free the outer work; no callback NULL check. */
void dds3MagatuhiParDelete(EffWork *work) {
    D_00355738[work->type].handler(work->payload);
    sdfChipFreeDirect(work);
}

/* Return the retained payload passed to this type's callbacks. */
void *dds3MagatuhiGetHandle(EffWork *work) {
    return work->payload;
}

/* Read the type field from the dispatch owner; the pointer remains unchecked. */
u32 dds3MagatuhiGetType(EffWork *work) {
    return work->type;
}

/* Native node type-three restart slot: no instance state is changed. */
void effDispatchNodeRestartNoOp(void) {
}

/* Native node type-three condition slot: accept with the constant result one. */
u32 dds3MagatuhiParIsActive(void) {
    return 1;
}

/* Node set-vector slot: invoke optional subtype callback A; preserve the one-formal surface. */
void dds3MagatuhiParSetPosition(EffWork *work) {
    void (*handler)(void *) = D_0035573C[work->type].handler;

    if (handler != NULL) {
        handler(work->payload);
    }
}

/* Node set-matrix slot: invoke optional subtype callback B; preserve the one-formal surface. */
void dds3MagatuhiParSetRotateMatrix(EffWork *work) {
    void (*handler)(void *) = D_00355744[work->type].handler;

    if (handler != NULL) {
        handler(work->payload);
    }
}

/* Node parameter-word slot: invoke optional subtype callback C; preserve the one-formal surface. */
void dds3MagatuhiParSetRGBA(EffWork *work) {
    void (*handler)(void *) = D_00355740[work->type].handler;

    if (handler != NULL) {
        handler(work->payload);
    }
}

/* Store the low byte of slotValue at the type-specific subslot.
   Types two through four share the same location; other types are left unchanged. */
void dds3MagatuhiParSetByType(EffWork *work, s32 slotValue) {
    u8 slotByte = slotValue;
    u32 type = work->type;

    switch (type) {
    case 0:
        ((EffSub *)work->payload)->type0SubSlot = slotByte;
        return;
    case 1:
        ((EffSub *)work->payload)->type1SubSlot = slotByte;
        return;
    case 2:
        ((EffSub *)work->payload)->type2To4SubSlot = slotByte;
        return;
    case 3:
        ((EffSub *)work->payload)->type2To4SubSlot = slotByte;
        return;
    case 4:
        ((EffSub *)work->payload)->type2To4SubSlot = slotByte;
        return;
    default:
        return;
    }
}

/* Read the type-specific subslot, or zero for other types. */
u32 effGetSubSlot(EffWork *work, s32 unusedSlotValue) {
    u32 type = work->type;

    switch (type) {
    case 0:
        return ((EffSub *)work->payload)->type0SubSlot;
    case 1:
        return ((EffSub *)work->payload)->type1SubSlot;
    case 2:
        return ((EffSub *)work->payload)->type2To4SubSlot;
    case 3:
        return ((EffSub *)work->payload)->type2To4SubSlot;
    case 4:
        return ((EffSub *)work->payload)->type2To4SubSlot;
    default:
        break;
    }
    return 0;
}

/* Types zero/one dispatch from the payload base; two through four skip its prefix.
   Other types still dispatch with a zero argument; no type bounds check is added. */
EffWork *dds3MagatuhiParCopy(EffWork *work) {
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
    return dds3MagatuhiParCreate(type, handlerArg);
}

/* Native node type-three scale slot: no instance state is changed. */
void effDispatchNodeScaleNoOp(void) {
}

void InitMagatuhiEditorProcess(void) {
    dds3AdminiCallSequence(0, 0, 0, 0);
}

/* Return the setup-record address word unchanged; no record access or validation is made. */
u32 effReturnEventSetupWord(u32 value) {
    return value;
}

void func_0018CDD8(void) {
}

void func_0018CDE0(void) {
}

void func_0018CDE8(void) {
}

/* Setup paths supply a record address; this matching empty entry ignores it. */
void effEventSetupRecordNoOp(void) {
}

/* Empty stage immediately before the per-record entry in the paired setup paths. */
void effEventSetupBeforeRecordNoOp(void) {
}

/* Empty stage immediately after the per-record entry in the paired setup paths. */
void effEventSetupAfterRecordNoOp(void) {
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
u32 effReturnCallbackValue(u32 value) {
    return value;
}

void effFieldTaskStartupHook(void) {
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
s32 dds3EffectDirOpen(const char *name) {
    u8 path[EFF_DIRECTORY_PATH_BYTES];

    if (sdfHddValid != 0) {
        sprintf(path, D_003BB058, name);
        return sceDopen(path);
    } else {
        effDataDirectoryIndex = 0;
        return 0;
    }
}


/* Run the native directory callback only in debug filesystem mode; its result is ignored. */
void dds3EffectDirClose(s32 directory) {
    if (sdfHddValid != 0) {
        sceDclose(directory);
    }
}


/* Debug mode delegates to the native iterator. Built-in mode copies the next name,
   clears flag mask 0x1000, and returns name length; zero also marks table exhaustion. */
s32 dds3EffectDirRead(s32 directory, EffDirEnt *entry) {
    if (sdfHddValid != 0) {
        return sceDread(directory, entry);
    }
    if ((u32)effDataDirectoryIndex >= EFF_BUILTIN_NAME_COUNT) {
        return 0;
    }
    strcpy(entry->name, D_003557A8[effDataDirectoryIndex]);
    entry->flags &= ~EFF_DIRECTORY_FLAG_CLEAR;
    effDataDirectoryIndex++;
    return strlen(entry->name);
}


/* Enumerate enabled resource extensions into an owned doubly linked filename list. */
EffResourceList *dds3EffectFileListCreate(const char *path, s32 flags) {
    EffDirEnt entry;
    s32 directory;
    EffResourceList *list;
    EffResourceListNode *node;
    EffResourceListNode *tail;
    s32 length;
    EffResourceKind kind;
    directory = dds3EffectDirOpen(path);
    list = sdfChipAllocDirect(sizeof(*list));
    if (sdfHddValid != 0) {
        list->directoryPath = sdfChipAllocDirect(strlen(path) + 1);
        strcpy(list->directoryPath, path);
    } else {
        list->directoryPath = sdfChipAllocDirect(strlen(path) + 1);
        strcpy(list->directoryPath, path);
    }
    tail = NULL;
    list->head = NULL;
    list->resourceCount = 0;
    while ((length = dds3EffectDirRead(directory, &entry)) > 0) {
        if ((entry.flags & 0xF000) == 0x1000) continue;
        if (length < 5) continue;
        if (length >= 0x2F) continue;
        if ((flags & 0x10 ? entry.name[length - 3] : entry.name[length - 4]) != '.') continue;
        kind = EFF_RESOURCE_KIND_NONE;
        if ((flags & 1) &&
            ((entry.name[length-3]=='t' && entry.name[length-2]=='m' && entry.name[length-1]=='x') ||
             (entry.name[length-3]=='T' && entry.name[length-2]=='M' && entry.name[length-1]=='X'))) kind = EFF_RESOURCE_KIND_TMX;
        else if ((flags & 2) &&
            ((entry.name[length-3]=='p' && entry.name[length-2]=='2' && entry.name[length-1]=='a') ||
             (entry.name[length-3]=='P' && entry.name[length-2]=='2' && entry.name[length-1]=='A'))) kind = EFF_RESOURCE_KIND_P2A;
        else if ((flags & 4) &&
            ((entry.name[length-3]=='d' && entry.name[length-2]=='3' && entry.name[length-1]=='p') ||
             (entry.name[length-3]=='D' && entry.name[length-2]=='3' && entry.name[length-1]=='P'))) kind = EFF_RESOURCE_KIND_D3P;
        else if ((flags & 8) &&
            ((entry.name[length-3]=='b' && entry.name[length-2]=='e' && entry.name[length-1]=='d') ||
             (entry.name[length-3]=='B' && entry.name[length-2]=='E' && entry.name[length-1]=='D'))) kind = EFF_RESOURCE_KIND_BED;
        else if ((flags & 0x10) &&
            ((entry.name[length-2]=='p' && entry.name[length-1]=='b') ||
             (entry.name[length-2]=='P' && entry.name[length-1]=='B'))) kind = EFF_RESOURCE_KIND_PB;
        else if ((flags & 0x20) &&
            ((entry.name[length-3]=='p' && entry.name[length-2]=='c' && entry.name[length-1]=='f') ||
             (entry.name[length-3]=='P' && entry.name[length-2]=='C' && entry.name[length-1]=='F'))) kind = EFF_RESOURCE_KIND_PCF;
        if (kind == EFF_RESOURCE_KIND_NONE) continue;
        node = sdfChipAllocDirect(sizeof(*node));
        node->kind = kind;
        strcpy(node->name, entry.name);
        if (tail != NULL) tail->next = node;
        else list->head = node;
        node->next = NULL;
        node->previous = tail;
        list->resourceCount++;
        tail = node;
    }
    dds3EffectDirClose(directory);
    return list;
}

/* Free filename nodes, the copied directory path, then the list root.
   Each filename is inline; save the next link before releasing its node. */
void dds3EffectFileListDelete(EffResourceList *root) {
    EffResourceListNode *node = root->head;

    if (node != NULL) {
        do {
            EffResourceListNode *next = node->next;
            sdfChipFreeDirect(node);
            node = next;
        } while (node != NULL);
    }
    sdfChipFreeDirect(root->directoryPath);
    sdfChipFreeDirect(root);
}




/* Copy the source count, duplicate its head pointer, and store the source-list pointer.
   Other descriptor words retain their native defaults; no retain operation occurs here. */
EffResourceDescriptor *dds3EffectFileListWindowCreate(EffResourceList *list) {
    EffResourceDescriptor *resource = sdfChipAllocDirect(sizeof(*resource));

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
/* Retail 0x0018D774 reloads selectedEntry after the case-1 texture update. */
s32 dds3EffectFileListWindowDraw(EffResourceDescriptor *descriptor) {
    EffResourceListNode *visibleEntry;
    sdfDrawTag_t *packetList;
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
        if ((dds3PadBitTable[6] & 2) != 0) {
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
        } else if ((dds3PadBitTable[7] & 2) != 0) {
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
        } else if ((dds3PadBitTable[9] & 2) != 0) {
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
        } else if ((dds3PadBitTable[11] & 2) != 0) {
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
        } else if (dds3PadBitTable[3] < 0) {
            descriptor->result = 2;
        } else if (dds3PadBitTable[1] < 0) {
            descriptor->result = 1;
        } else {
            EffResourceListNode *cachedEntry = descriptor->cachedEntry;
            selectedEntry = descriptor->selectedEntry;
            if (cachedEntry != selectedEntry) {
                s32 category = selectedEntry->kind;
                switch (category) {
                case 1: {
                    char resourceName[0x70];
                    sprintf(resourceName, D_003BB060,
                        descriptor->resourceList->directoryPath, selectedEntry->name);
                    dds3EffectListSetTexture(descriptor, resourceName);
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
                dds3EffectFileListWindowDrawSprite(descriptor);
            }
        }
    }

    packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(packetList);
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(
        0x8000 - (descriptor->x << 4), 0x8000 - (descriptor->y << 3),
        0xFEFFFF, 0xC00, 0x620, descriptor->fillColor, descriptor->borderColor));
    if (descriptor->previewActive != 0) {
        sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(
            0x8000 - (descriptor->x << 4),
            0x8000 - ((descriptor->y - 0xC4) << 3),
            0xFEFFFF, 0x800, 0x400, 0, 0x40806020));
    }
    contentRow = descriptor->y - 2;
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(
        0x8000 - ((descriptor->x - 2) << 4),
        0x8000 - (contentRow << 3), 0xFF0000, 5,
        descriptor->resourceList->directoryPath));
    contentRow -= 0xC;
    visibleEntry = descriptor->firstVisibleEntry;
    drawnRows = 0;
    while (visibleEntry != NULL) {
        s32 selectionFlags = visibleEntry == descriptor->selectedEntry ? 4 : 0;
        sdfDrawTagLink(packetList, (u32)sdfDevPrintf(
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
    surface = &sdfGrOtTable[descriptor->drawSurface];
    surface->append(surface, packetList);
    return descriptor->result;
}


/* Release any retained texture reference, clear its handle, and free the work block. */
void dds3EffectFileListWindowDelete(EffResourceDescriptor *work) {
    sdfTexHandle_t *texture = work->textureHandle;

    if (texture != NULL) {
        sdfTexReleaseReferenceViaHandler(texture);
        work->textureHandle = NULL;
    }
    sdfChipFreeDirect(work);
}

/* Set the browser screen coordinates. */
void dds3EffectFileListWindowSetPos(EffResourceDescriptor *message, s32 x, s32 y) {
    message->x = x;
    message->y = y;
}


/* Return the selected resource index as its native word. */
u32 effGetWorkParam(EffResourceDescriptor *work) {
    return work->selectedIndex;
}

/* Return the browser selection result as its native word. */
u32 dds3EffectFileListWindowGetState(EffResourceDescriptor *work) {
    return work->result;
}

/* Format the prefix/name records through the native template and return the name record's first word.
   Buffer capacity and record pointers remain unchecked. */
u32 dds3EffectFileListWindowGetFilename(EffResourceDescriptor *message, char *destination) {
    sprintf(destination, D_003BB060, message->resourceList->directoryPath, message->selectedEntry->name);
    return message->selectedEntry->kind;
}

/* Set the navigation repeat delay. */
void dds3EffectFileListWindowSetOt(EffResourceDescriptor *work, u32 value) {
    work->repeatDelay = value;
}

/* Select the draw surface for browser packets. */
void dds3EffectFileListWindowSetPadWait(EffResourceDescriptor *work, u32 value) {
    work->drawSurface = value;
}

/* Set the browser border and fill colors. */
void dds3EffectFileListWindowSetWindowColor(EffResourceDescriptor *message, u32 borderColor, u32 fillColor) {
    message->borderColor = borderColor;
    message->fillColor = fillColor;
}

/* Release the previous texture reference before loading/acquiring its replacement.
   Release the temporary loaded resource afterward; native failure results are unchecked. */
void dds3EffectListSetTexture(EffResourceDescriptor *work, const char *textureResource) {
    sdfTexHandle_t *texture;
    sdfMemHandle_t *loadedResource;
    u32 resourceWords[4];

    if (work->textureHandle != NULL) {
        sdfTexReleaseReferenceViaHandler(work->textureHandle);
        work->textureHandle = NULL;
    }
    loadedResource = sdfDevLoadFileEx(textureResource, resourceWords, 0);
    texture = sdfTexSet((SdfTextureFileHeader *)(resourceWords[0]));
    work->textureHandle = texture;
    sdfMemFreeDirect(loadedResource);
}

extern SdfPoolNode D_003255A8;


/* Queue the overlay's texture as a quad at its screen position. */
void dds3EffectFileListWindowDrawSprite(EffResourceDescriptor *overlay) {
    sdfDrawTag_t *list;
    void *sprite;
    KwlnSpriteVertex *vertex;
    sdfTexHandle_t *texture;

    if (overlay->textureHandle != 0) {
        list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(list);
        sprite = sdf2DAllocSprite(1);
        vertex = (KwlnSpriteVertex *)sdf2DGetPrimPtr(sprite);
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
        sdf2DLinkTexPacket(list, texture, SDF_GS_CONTEXT_1);
        sdfDrawTagLink(list, (u32)sprite);
        D_003255A8.append(&D_003255A8, list);
    }
}


void InitParticleEditorProcess(void) {
    dds3AdminiCallSequence(0, 0, 0, 0);
}

void InitSpriteViewerProcess(void) {
    dds3AdminiCallSequence(0, 0, 0, 0);
}

/* vu0 routine: transform the point in vf10 by view/projection, divide by w, then scale/bias.
   No clipping or zero-w check is performed; the result remains in vf10. */
void effScreenOffset(void) {
    u8 *projectionData;
    VU0_LOAD_MATRIX(sdfViewMatrix);
    projectionData = sdfProjectionMatrix;
    sdfPostmultiplyVuMatrixFromMemory(projectionData);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    projectionData += EFF_MATRIX_BYTES;
    SDF_VU0_LOADVECTOR_B_BARRIER(projectionData);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B_BARRIER(D_00324660);
    SDF_VU0_ADDVECTOR();
}

/* vu0 routine: project a point and its camera-right offset, returning their rounded screen distance. */
s32 effPCPBlurScreenOffset(f32 scale) {
    f32 scaleVector[4];
    f32 projectedEnd[4];
    f32 projectedStart[4];
    f32 original[4];
    f32 dx;
    f32 dy;

    SDF_VU0_STOREVECTOR_NOMEM(original);
    effScreenOffset();
    SDF_VU0_STOREVECTOR(projectedStart);

    scaleVector[0] = scaleVector[1] = scaleVector[2] = scale;
    SDF_VU0_LOADVECTOR(original);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR_B(sdfViewTargetVector);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(aViewDown);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B_BARRIER(scaleVector);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_C_TO_B();
    SDF_VU0_ADDVECTOR();

    effScreenOffset();
    SDF_VU0_STOREVECTOR(projectedEnd);
    dx = projectedEnd[0] - projectedStart[0];
    dy = projectedEnd[1] - projectedStart[1];
    SDF_VU0_LOADVECTOR(projectedStart);
    return (s32)fsqrtf(dx * dx + dy * dy);
}

/* vu0 routine: blend two RGBA8888 colours by t (lerp in float, packed back to RGBA8888) */
u32 effColorBlend(u32 colorA, u32 colorB, f32 t) {
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
    SDF_VU0_COPYVECTOR_A_TO_B();
    color2[0] = colorA;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_SCALAR_OP_CLOBBER(1.0f - t, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALAR_OP_R3_CLOBBER(t, "vmulx.xyzw vf11, vf11, vf2x");
    SDF_VU0_ADDVECTOR();
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    
    return blended[0];
}

/* vu0 routine: multiply RGBA channels normalized by 128, then pack at the same scale.
   Native integer conversion/byte packing remains unclamped; quadword storage is unchanged. */
u32 effMagaCylinderCalcColor(u32 colorA, u32 colorB) {
    s32 colorAWords[EFF_VECTOR_WORD_COUNT];
    s32 colorBWords[EFF_VECTOR_WORD_COUNT];
    s32 packedWords[EFF_VECTOR_WORD_COUNT];
    u32 packed;
    u32 unpackScaleBits = EFF_COLOR_UNPACK_SCALE_BITS;
    colorAWords[0] = colorA;
    EE_MMI_RGBA_UNPACK(colorAWords, unpackScaleBits);
    SDF_VU0_COPYVECTOR_A_TO_B();
    colorBWords[0] = colorB;
    EE_MMI_RGBA_UNPACK(colorBWords, unpackScaleBits);
    SDF_VU0_MULVECTOR();
    EE_MMI_RGBA_PACK(packed);
    packedWords[0] = packed;
    return packedWords[0];
}

/* vu0 routine: distance from point to the line through origin along a unit direction.
   Direction is not normalized here; dot/length use xyz despite full quadword loads. */
f32 effLinePointLength(f32 *direction, f32 *origin, f32 *point) {
    f32 projectedOffset[EFF_VECTOR_WORD_COUNT];
    f32 pointOffset[EFF_VECTOR_WORD_COUNT];
    f32 projectionAmount;
    f32 distance;

    SDF_VU0_LOADVECTOR(point);
    SDF_VU0_LOADVECTOR_B(origin);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(pointOffset);
    SDF_VU0_LOADVECTOR_B(direction);
    SDF_VU0_INNERPRODUCT(projectionAmount);
    /* The fourth projection component remains unwritten: only xyz contributes to distance. */
    projectedOffset[0] = direction[0] * projectionAmount;
    projectedOffset[1] = direction[1] * projectionAmount;
    projectedOffset[2] = direction[2] * projectionAmount;
    SDF_VU0_LOADVECTOR(pointOffset);
    SDF_VU0_LOADVECTOR_B(projectedOffset);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(distance);
    return distance;
}

/* Allocate/retain slot storage and return its header after the slots, not the slot base.
   Only the two native slot defaults are initialized; count/allocation validity is unchecked. */
EffArrHdr *dds3Bezier4PCreate(s32 count) {
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(count * EFF_SLOT_BYTES + EFF_SLOT_HEADER_BYTES);
    void *retainedAddress = (void *)sdfMemLock(allocation);
    u32 slotIndex = 0;
    EffCubicBezierSlot *slot = retainedAddress;
    u8 *headerAddress = (u8 *)slot + count * EFF_SLOT_BYTES;

    ((EffArrHdr *)headerAddress)->allocation = allocation;
    ((EffArrHdr *)headerAddress)->slots = retainedAddress;
    ((EffArrHdr *)headerAddress)->slotCount = count;
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
