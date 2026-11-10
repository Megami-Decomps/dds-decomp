#include "sdk_ps2.h"
#include "common.h"
#include "sdf_packet_list.h"
#include "sdf_packet_builders.h"
#include "sdf_texture_draw_packet.h"
#include "dds3_task_create.h"
#include "sdf_texture_lists.h"
#include "sdf_texture_registry.h"
#include "sdf_gs_blend.h"
#include "dds3_task_lifecycle.h"
#include "btl.h"
#include "sdf_packet_alloc.h"

extern u8 D_00435C60;
extern u32 dds3TextureViewerPageIndex;
extern void dds3DrawTextureListDiagnostic(sdfDrawTag_t *packetList, s32 x, s32 y);
extern u32 dds3TextureGetPageIndex(void);
extern s32 dds3TextureCountIncompleteResources(void);
extern void *sdfDevPrintf(s32, s32, s32, s32, const char *, ...);
extern u64 *dds3PrimFrameRect(s32, s32, s32, s32, s32, u32, u32);
extern s32 dds3DebugGraphUpdate(dds3ProcessID_t *task);
extern void dds3DebugGraphTaskDestroy(dds3ProcessID_t *task);
extern dds3ProcessID_t *dds3DebugGraphTask;
extern sdfTexHandle_t *dds3CurrentIncompleteResource;
extern s32 dds3TextureViewerHandlePad(void);
extern s8 dds3PadBitTable[32];
extern s32 dds3SelectIncompleteResourceAtPageIndex(s32);
extern void dds3DrawTextureViewerFrame(sdfGrOt_t *surface, sdfTexHandle_t *image);
extern sdfGrOt_t sdfDebugGrOt;
extern s8 D_0037F53B[];
extern void dds3TextureAttachTask(sdfGrOt_t *surface);
#define DDS3_PAD_REPEAT_BIT 2
#define DDS3_PREVIEW_MAX_TEXELS 0x100
#define DDS3_PREVIEW_WRAP_TEXELS 0x400
#define DDS3_DIAG_DEPTH 0x0FFFFF80

extern s16 D_00435BA8;
extern u16 sdfDrawFrameCounter;
extern u16 D_00435C50;
extern u16 D_00435C52;
extern u16 D_00435C54;
extern u16 D_00435C56;
extern char D_00435C58[];

s32 dds3DebugGraphUpdate(dds3ProcessID_t *task) {
    sdfDrawTag_t *packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfPrimTransPacket_t *blendPacket;
    s32 originX = 0x7100;
    /* Keep the four real conversion epochs in one local; see docs/idioms.md,
     * "Trace-justified pixel conversion reuse: dds3DebugGraphUpdate". */
    s32 pixels;

    sdfDrawTagInit(packetList);
    blendPacket = (sdfPrimTransPacket_t *)sdfGrAllocPacketBuff(0x40);
    sdfPrimSetOpaquePacket(blendPacket);
    sdfDrawTagLink(packetList, (u32)blendPacket);
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(0x70D0, 0x85A8,
                    0xFFFF7F, 0x1260, 0xF0, 0x20000000, 0x40806040));
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(originX,
                    0x85C0, 0xFFFF80, 0, D_00435C58, (u16)D_00435BA8));
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(originX,
                    0x8620, 0xFFFF80, 0, D_00435C58, sdfDrawFrameCounter));
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x8080,
                    0x85C0, 0xFFFF80, 6, D_00435C58, D_00435C50));
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x8080,
                    0x8620, 0xFFFF80, 6, D_00435C58, D_00435C52));
    pixels = (s32)((f32)(u16)D_00435BA8 / 224.0f * 200.0f);
    sdfPrimLinkRect(packetList, 0x8000C000, 0, 0x73A0, 0x85F0,
                    originX + ((pixels + 0x2A) << 4),
                    0x8600, 0xFF0000, NULL);
    pixels = (s32)((f32)sdfDrawFrameCounter / 224.0f * 200.0f);
    sdfPrimLinkRect(packetList, 0x80000080, 0, 0x73A0, 0x8650,
                    originX + ((pixels + 0x2A) << 4),
                    0x8660, 0xFF0000, NULL);
    pixels = (s32)((f32)D_00435C50 / 224.0f * 200.0f);
    sdfPrimLinkRect(packetList, 0x8000FFFF, 0,
                    originX + ((pixels + 0x2B) << 4),
                    0x85D0,
                    originX + 0x20 + ((pixels + 0x2B) << 4),
                    0x8610, 0xFF0000, NULL);
    pixels = (s32)((f32)D_00435C52 / 224.0f * 200.0f);
    sdfPrimLinkRect(packetList, 0x8000FFFF, 0,
                    originX + ((pixels + 0x2B) << 4),
                    0x8630,
                    originX + 0x20 + ((pixels + 0x2B) << 4),
                    0x8670, 0xFF0000, NULL);
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, packetList);
    if (D_00435C54 >= 0x47 || D_00435C50 < (u16)D_00435BA8) {
        D_00435C50 = D_00435BA8;
        D_00435C54 = 0;
    } else {
        D_00435C54++;
    }
    if (D_00435C56 >= 0x47 || D_00435C52 < sdfDrawFrameCounter) {
        D_00435C52 = sdfDrawFrameCounter;
        D_00435C56 = 0;
    } else {
        D_00435C56++;
    }
    return 0;
}

void dds3DebugGraphTaskDestroy(dds3ProcessID_t *task) {
}

/* Only mode 1 creates and mode 0 destroys the timing graph; other modes do nothing. */
void dds3DebugGraphSetEnabled(s8 enabled) {
    if (enabled == 1) {
        dds3DebugGraphTask = dds3StartProcess("DebugTimeGrph", 0x2710, 1, 1, dds3DebugGraphUpdate, dds3DebugGraphTaskDestroy, NULL);
    } else if (enabled == 0) {
        dds3KillProcess(dds3DebugGraphTask, 0);
    }
}

/* Scale the longer preview side to at most 256 texels, preserving division
 * before coordinate scaling; UV endpoints at 1024 texels are reduced by one. */
void dds3DrawImageOutline(sdfDrawTag_t *packetList, sdfTexHandle_t *image) {
    s32 width = image->width;
    s32 height = image->height;
    s32 drawWidth;
    s32 drawHeight;

    sdf2DLinkTexPacket(packetList, image, SDF_GS_CONTEXT_1);
    drawWidth = width * 0x10;
    drawHeight = height * 8;
    if (width < height) {
        if (height > DDS3_PREVIEW_MAX_TEXELS) {
            drawHeight = 0x800;
            drawWidth = (width << 8) / height * 0x10;
        }
    } else if (width > DDS3_PREVIEW_MAX_TEXELS) {
        drawWidth = 0x1000;
        drawHeight = (height << 8) / width * 8;
    }
    if (width == DDS3_PREVIEW_WRAP_TEXELS) {
        width--;
    }
    if (height == DDS3_PREVIEW_WRAP_TEXELS) {
        height--;
    }
    sdfPrimLinkSprite(packetList, 0x80808080, 0, 0x7180, 0x7A60, 0, 0, drawWidth + 0x7180, drawHeight + 0x7A60,
                                width * 0x10, height * 0x10, DDS3_DIAG_DEPTH, 0);
}

/* Append the zero-based viewer page and final page index; an empty list prints -1. */
void dds3TextureDrawPageCounter(sdfDrawTag_t *packetList) {
    char pageText[0x70];
    s32 pageIndex = dds3TextureGetPageIndex();
    s32 resourceCount = dds3TextureCountIncompleteResources();
    sprintf(pageText, "TEX VIEWER [%d/%d]", pageIndex, resourceCount - 1);
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x7180, 0x79C0, DDS3_DIAG_DEPTH, 0, pageText));
}

extern const char *D_0037F560[];

INCLUDE_RODATA(const s32, "game/code_00104180", D_004111F8);

void dds3DrawTextureResourceDetails(sdfDrawTag_t *packetList, sdfTexHandle_t *texture) {
    char text[0x70];
    s32 formatIndex = 0;
    s32 formatMode = 0;

    sprintf(text, "W:%4d H:%4d", texture->width, texture->height);
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x7180, 0x82A0, 0x0FFFFF80, 0, text));
    switch (texture->pixelFormat) {
    case 0: formatIndex = 0; formatMode = 6; break;
    case 1: formatIndex = 1; formatMode = 6; break;
    case 2: formatIndex = 2; formatMode = 6; break;
    case 10: formatIndex = 3; formatMode = 6; break;
    case 19: formatIndex = 4; formatMode = 4; break;
    case 20: formatIndex = 5; formatMode = 5; break;
    case 27: formatIndex = 6; formatMode = 4; break;
    case 36: formatIndex = 7; formatMode = 5; break;
    case 44: formatIndex = 8; formatMode = 5; break;
    default: break;
    }
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x7180, 0x8300, 0x0FFFFF80,
                                                                formatMode, D_0037F560[formatIndex]));
    sprintf(text, "ID[%2d,%2d]", texture->texId, texture->clutId);
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x77C0, 0x8300, 0x0FFFFF80, 0, text));
}

#define DDS3_DIAG_PACKET_LIST_BYTES 0x20
#define DDS3_VIEWER_FAST_PAGE_STEP 10
#define DDS3_MAP_ROW_UNITS 0x1000
#define DDS3_MAP_ROW_SHIFT 12
#define DDS3_MAP_ROW_MASK 0xFFF
#define DDS3_MAP_COLUMN_SHIFT 6
#define DDS3_MAP_COLUMN_UNITS 64
#define DDS3_MAP_COLUMN_COUNT 64
#define DDS3_MAP_ROW_SPAN 0x400
#define DDS3_MAP_BORDER_WIDTH 0x440
#define DDS3_MAP_CELL_X_STEP 16
#define DDS3_MAP_ROW_Y_STEP 8
#define DDS3_MAP_CELL_X_SHIFT 4
#define DDS3_MAP_GREEN 0x80008000
#define DDS3_MAP_YELLOW 0x80008080
#define DDS3_MAP_RED 0x80000080
#define DDS3_MAP_BLUE 0x80800000
#define DDS3_MAP_CYAN 0x80808000

/* Count texture entries without a reference clone source. */
s32 dds3TextureCountIncompleteResources(void) {
    sdfTexHandle_t *texture = sdfTexHandleLink;
    sdfTexHandle_t *previousTexture;
    s32 incompleteCount = 0;
    while (texture != NULL) {
        while (texture->ref != NULL && texture->ref->origin != NULL) {
            previousTexture = texture->next;
            if (previousTexture == NULL) {
                return incompleteCount;
            }
            texture = previousTexture;
        }
        incompleteCount++;
        texture = texture->next;
    }
    return incompleteCount;
}

u32 dds3TextureGetPageIndex(void) {
    return dds3TextureViewerPageIndex;
}

/* DDS2 first skips clone-source entries at the head, even for requested index zero.
 * This assumes a nonempty list; initial exhaustion returns 0 before selection writes. */
s32 dds3SelectIncompleteResourceAtPageIndex(s32 requestedIndex) {
    sdfTexHandle_t *texture = sdfTexHandleLink;
    sdfTexHandle_t *previousTexture;
    s32 visitedIndex = 0;

    while (texture->ref != NULL && texture->ref->origin != NULL) {
        previousTexture = texture->next;
        if (previousTexture == NULL) {
            return 0;
        }
        texture = previousTexture;
    }
    if (requestedIndex < 0) {
        requestedIndex = 0;
    }
    while (texture != NULL && visitedIndex != requestedIndex) {
        dds3CurrentIncompleteResource = texture;
        while (texture->ref != NULL && texture->ref->origin != NULL) {
            previousTexture = texture->next;
            if (previousTexture == NULL) {
                return visitedIndex;
            }
            texture = previousTexture;
        }
        previousTexture = texture->next;
        if (previousTexture == NULL) {
            return visitedIndex;
        }
        visitedIndex++;
        texture = previousTexture;
    }
    dds3CurrentIncompleteResource = texture;
    return visitedIndex;
}

/* Exit input returns 0 immediately; otherwise apply one prioritized page step,
 * select its reached index, then optionally toggle the viewer mode and return 1. */
s32 dds3TextureViewerHandlePad(void) {
    if (dds3PadBitTable[0x13] < 0) {
        return 0;
    }
    if (dds3PadBitTable[0x14] & DDS3_PAD_REPEAT_BIT) {
        dds3TextureViewerPageIndex -= 1;
    } else if (dds3PadBitTable[0x15] & DDS3_PAD_REPEAT_BIT) {
        dds3TextureViewerPageIndex += 1;
    } else if ((dds3PadBitTable[0x18] & DDS3_PAD_REPEAT_BIT) || (dds3PadBitTable[0x19] & DDS3_PAD_REPEAT_BIT)) {
        dds3TextureViewerPageIndex -= DDS3_VIEWER_FAST_PAGE_STEP;
    } else if ((dds3PadBitTable[0x1A] & DDS3_PAD_REPEAT_BIT) || (dds3PadBitTable[0x1B] & DDS3_PAD_REPEAT_BIT)) {
        dds3TextureViewerPageIndex += DDS3_VIEWER_FAST_PAGE_STEP;
    }
    dds3TextureViewerPageIndex = dds3SelectIncompleteResourceAtPageIndex(dds3TextureViewerPageIndex);
    if (dds3PadBitTable[0x12] < 0) {
        D_00435C60 ^= 1;
    }
    return 1;
}

extern s32 D_00435CB0;

void dds3DrawTextureAllocationMapBlock(void *packetList, s32 x, s32 y, sdfGsMemSlice_t *block, u8 mode);

void dds3DrawTextureViewerFrame(sdfGrOt_t *surface, sdfTexHandle_t *image) {
    sdfDrawTag_t *packetList;
    s32 counter;
    s32 mode;

    if (image == NULL) {
        return;
    }
    packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(packetList);
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(0x7160, 0x79B0, 0x0FFFFF7F, 0x1040, 0x80, 0x80000000, 0x80806020));
    dds3TextureDrawPageCounter(packetList);
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(0x7160, 0x7A50, 0x0FFFFF7F, 0x1040, 0x820, 0x80000000, 0x80806020));
    dds3DrawImageOutline(packetList, image);
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(0x7160, 0x8290, 0x0FFFFF7F, 0x1040, 0xE0, 0x80000000, 0x80806020));
    dds3DrawTextureResourceDetails(packetList, image);

    if (D_00435C60 != 0) {
        dds3DrawTextureListDiagnostic(packetList, 0x8240, 0x7A60);
        counter = D_00435CB0;
        mode = (counter >> 4) & 1;
        if (image->pixelGsHandle != NULL) {
            dds3DrawTextureAllocationMapBlock(packetList, 0x8240, 0x7A60, image->pixelGsHandle, mode);
        }
        counter = D_00435CB0;
        /* Boolean highlight selection; see docs/idioms.md, "Trace-justified boolean extraction: texture viewer highlight". */
        mode = (counter & 0x10) ? 2 : 0;
        if (image->clutGsHandle != NULL) {
            dds3DrawTextureAllocationMapBlock(packetList, 0x8240, 0x7A60, image->clutGsHandle, mode);
        }
        D_00435CB0++;
    }
    surface->regFunc(surface, packetList);
}

/* With a nonempty list, invoke its renderer even when input reports exit.
 * The surface owns the packet submission callback; return input status. */
s32 dds3SwapActiveResource(sdfGrOt_t *surface) {
    s32 inputStatus;

    if (sdfTexHandleLink == 0) {
        return 0;
    }
    inputStatus = dds3TextureViewerHandlePad();
    dds3DrawTextureViewerFrame(surface, dds3CurrentIncompleteResource);
    return inputStatus;
}

/* Use the default surface: empty list returns 0, exit input -1, drawing 0. */
s32 dds3LoadDefaultResource(void) {
    if (sdfTexHandleLink == 0) {
        return 0;
    }
    if (dds3TextureViewerHandlePad() == 0) {
        return -1;
    }
    dds3DrawTextureViewerFrame(&sdfDebugGrOt, dds3CurrentIncompleteResource);
    return 0;
}

/* Initialize viewer state and return its default renderer for the first eligible
 * node. Exhausting a nonempty list returns NULL after the initial state writes. */
s32 (*dds3TextureFindIncompleteResource(void))(void) {
    sdfTexHandle_t *texture = sdfTexHandleLink;
    if (texture == NULL) {
        return NULL;
    }
    dds3CurrentIncompleteResource = texture;
    D_00435C60 = 1;
    dds3TextureViewerPageIndex = 0;
    while (texture->ref != NULL && texture->ref->origin != NULL) {
        texture = dds3CurrentIncompleteResource->next;
        if (texture == NULL) {
            return NULL;
        }
        dds3CurrentIncompleteResource = texture;
    }
    return dds3LoadDefaultResource;
}

/* Draw one allocation-map block in address units, choosing color by mode.
 * Only the initial partial row forces a nonzero remainder to occupy one cell;
 * the final partial row retains its truncated cell count. */
void dds3DrawTextureAllocationMapBlock(void *packetList, s32 x, s32 y, sdfGsMemSlice_t *block, u8 mode) {
    s32 color;
    u32 remainingUnits;
    s32 columnIndex;
    s32 rowUnits;
    s32 leftX;
    s32 topY;
    s32 cellCount;

    if (mode == 0) {
        switch ((s32)block->type) {
        case 1:
            color = DDS3_MAP_GREEN;
            break;
        case 2:
            color = DDS3_MAP_YELLOW;
            break;
        case 3:
            color = DDS3_MAP_RED;
            break;
        default:
            color = 0;
            break;
        }
    } else {
        color = mode == 1 ? DDS3_MAP_BLUE : DDS3_MAP_CYAN;
    }
    remainingUnits = block->size;
    topY = y + (block->addr >> DDS3_MAP_ROW_SHIFT) * DDS3_MAP_ROW_Y_STEP;
    columnIndex = (block->addr & DDS3_MAP_ROW_MASK) >> DDS3_MAP_COLUMN_SHIFT;
    leftX = x + columnIndex * DDS3_MAP_CELL_X_STEP;
    if (columnIndex > 0) {
        rowUnits = (DDS3_MAP_COLUMN_COUNT - columnIndex) * DDS3_MAP_COLUMN_UNITS;
        if (remainingUnits >= rowUnits) {
            sdfPrimLinkLine(packetList, color, 0, leftX, topY,
                                         leftX + (DDS3_MAP_COLUMN_COUNT - columnIndex) * DDS3_MAP_CELL_X_STEP, topY + DDS3_MAP_ROW_Y_STEP,
                                         DDS3_DIAG_DEPTH, NULL);
            remainingUnits -= rowUnits;
        } else {
            cellCount = remainingUnits >> DDS3_MAP_COLUMN_SHIFT;
            if (cellCount == 0) {
                cellCount = remainingUnits != 0;
            }
            sdfPrimLinkLine(packetList, color, 0, leftX, topY,
                                         leftX + cellCount * DDS3_MAP_CELL_X_STEP, topY + DDS3_MAP_ROW_Y_STEP,
                                         DDS3_DIAG_DEPTH, NULL);
            remainingUnits = 0;
        }
        topY += DDS3_MAP_ROW_Y_STEP;
    }
    while (remainingUnits >= DDS3_MAP_ROW_UNITS) {
        sdfPrimLinkLine(packetList, color, 0, x, topY, x + DDS3_MAP_ROW_SPAN, topY + DDS3_MAP_ROW_Y_STEP,
                                     DDS3_DIAG_DEPTH, NULL);
        remainingUnits -= DDS3_MAP_ROW_UNITS;
        topY += DDS3_MAP_ROW_Y_STEP;
    }
    if (remainingUnits != 0) {
        sdfPrimLinkLine(packetList, color, 0, x, topY,
                                     x + ((remainingUnits >> DDS3_MAP_COLUMN_SHIFT) << DDS3_MAP_CELL_X_SHIFT), topY + DDS3_MAP_ROW_Y_STEP,
                                     DDS3_DIAG_DEPTH, NULL);
    }
}

/* Draw the allocation-map border, then walk blocks through their prev links. */
void dds3DrawTextureListDiagnostic(sdfDrawTag_t *packetList, s32 x, s32 y) {
    sdfGsMemSlice_t *block = sdfGsMemGetLinkHead();

    if (block != NULL) {
        sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(x - 0x20, y - 0x10,
                                          0x0FFFFF7F, DDS3_MAP_BORDER_WIDTH, 0x820,
                                          0x80000000, 0x80806020));
        do {
            dds3DrawTextureAllocationMapBlock(packetList, x, y, block, 0);
            block = block->next;
        } while (block != NULL);
    }
}

/* Build an allocation-map packet list and submit it through the surface callback. */
void dds3TextureAttachTask(sdfGrOt_t *surface) {
    sdfDrawTag_t *packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(DDS3_DIAG_PACKET_LIST_BYTES);
    sdfDrawTagInit(packetList);
    dds3DrawTextureListDiagnostic(packetList, 0x7180, 0x79C0);
    surface->regFunc(surface, packetList);
}

/* Submit the default surface's map only when the control array's first byte is zero. */
s32 dds3EnsureDefaultResource(void) {
    if (D_0037F53B[0] != 0) {
        return 0;
    }
    dds3TextureAttachTask(&sdfDebugGrOt);
    return 0;
}

INCLUDE_SDATA(const s32, "game/code_00104180", D_00435C50);

INCLUDE_SDATA(const s32, "game/code_00104180", D_00435C52);

INCLUDE_SDATA(const s32, "game/code_00104180", D_00435C54);

INCLUDE_SDATA(const s32, "game/code_00104180", D_00435C56);

INCLUDE_SDATA(const s32, "game/code_00104180", D_00435C58);

INCLUDE_SDATA(const s32, "game/code_00104180", D_00435C60);

INCLUDE_SDATA(const s32, "game/code_00104180", D_00435C68);

INCLUDE_SDATA(const s32, "game/code_00104180", D_00435C70);

INCLUDE_SDATA(const s32, "game/code_00104180", D_00435C78);

INCLUDE_SDATA(const s32, "game/code_00104180", D_00435C80);

INCLUDE_SDATA(const s32, "game/code_00104180", D_00435C88);

INCLUDE_SDATA(const s32, "game/code_00104180", D_00435C90);

INCLUDE_SDATA(const s32, "game/code_00104180", D_00435C98);

INCLUDE_SDATA(const s32, "game/code_00104180", D_00435CA0);

INCLUDE_SDATA(const s32, "game/code_00104180", dds3CurrentIncompleteResource);

INCLUDE_SDATA(const s32, "game/code_00104180", dds3TextureViewerPageIndex);

INCLUDE_SDATA(const s32, "game/code_00104180", D_00435CB0);

