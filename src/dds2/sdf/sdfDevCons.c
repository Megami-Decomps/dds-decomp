#include "sdk_ps2.h"
#include "sdf_gs_header.h"
#include "eeregs.h"
#include "common.h"
#include "sdf_vu0.h"
#include "sdf_vu_matrix.h"
#include "sdf_pad_entry.h"
#include "sdf_pad_index.h"
#include "sdf_pad_mode_api.h"
#include "sdf_pad_motor_api.h"
#include "sdf_cons_matrix_packet.h"
#include "sdf_dma_packet_header.h"
#include "kwln.h"
#include "sdf_asset_packets.h"
#include "sdf_asset_state.h"
#include "sdf_vu_lighting.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "sdf_packet_header.h"
#include "sdf_packet_append.h"
#include "sdf_resource.h"
#include "sdf_primitive_packet_api.h"
#include "sdf.h"
#include "sdf_draw.h"
#include "sdf_request.h"
#include "sdf_model.h"
#include "sdf_projection.h"
#include "pcp_vu0.h"
#include "sdf_vu_transform_api.h"
#include "ee_mmi.h"
#include "fpu.h"
#include "sdf_texture_file.h"
#include "sdf_column_packet_api.h"
#include "sdf_lookup_texture.h"
#include "sdf_compact_vertex_vif_packet.h"
#include "sdf_dev_console_node.h"
#include "btl.h"
#include "sdf_packet_alloc.h"
#include "sdf_dev_cons.h"
#include "sdf_dev_cons_stack_packets.h"
#include "sdf_console_packet.h"
#include "sdf_file_existence_api.h"
#include "sif_rpc_client_prefix.h"
#include "sdf_thread_workspace_api.h"
#include "sdf_alarm.h"
#include "sdf_semaphore_api.h"
#include "sdf_timer.h"
#include "sdf_sif_command.h"
#include "sdf_dev_event.h"
#include "sdf_dev_protocol.h"
#include "sdf_dev_state.h"
#include "sdf_dev_path_prefix.h"
#include "sdf_dev_worker.h"
#include "sdf_dev_start_load_api.h"
#include "sdf_disc_file_record.h"
#include "sdf_debug_api.h"

#define SDF_CONSOLE_CELL_BYTES 2

#define SDF_CONSOLE_NODE_BYTES 0x20

extern const struct SdfDmaTagHeader *sdf3DLightDefault;

extern u32 sdf3DFogDefault;

extern struct sdfTexHandle_t *sdfDebugFontHandle;

extern u32 D_00438AB4;

extern u8 D_00370B80[];

extern u32 sdfDevConsFontUvTable[];

extern sdfDevConsCB_tag *D_00438AB0;

extern sdfMemHandle_t *D_00438A40;

extern sdfTexHandle_t *D_00438A6C;

extern sdfTexHandle_t *D_00438A70;

extern u64 sdf3DRefMapParam;

extern sdfTexHandle_t *D_00438A80;

extern s8 D_00438A50;

extern f32 D_00438A54;

extern f32 D_00438A58;

extern f32 D_00438A5C;

extern f32 D_00438A60;

extern f32 sdfFaceDivSoften;

extern f32 sdfFaceDivBias;

extern u16 D_00438A90[4];

extern u8 sdfPadAnalogSticks[8];

extern u8 D_00438A88[4];

extern u8 D_00438A8C;

/* Acquire and cache the shared console texture on first use. */
void sdfDevConsInit(void) {
    if (D_00438AB4 == 0) {
        D_00438AB4 = 1;
        sdfDebugFontHandle = sdfTexSet((SdfTextureFileHeader *)(D_00370B80));
    }
}

/* Ensure initialization and return the cached console texture. */
struct sdfTexHandle_t *sdfDevConsGetFontHandle(void) {
    sdfDevConsInit();
    return sdfDebugFontHandle;
}

/* Return the packed glyph-UV table indexed by character minus 0x20. */
u32 *sdfDevConsGetFontUvTable(void) {
    return sdfDevConsFontUvTable;
}

/* Append to the next-linked list and move its stored tail to this node. */
void sdfDevConsLinkToTop(sdfDevConsCB_tag *node) {
    sdfDevConsCB_tag *previousTail = D_00438AB0;

    node->next = NULL;
    node->previousNode = previousTail;
    if (previousTail != NULL) {
        previousTail->next = node;
    }
    D_00438AB0 = node;
}

/* Unlink this node, moving the stored tail when its next link is NULL. */
void sdfDevConsUnlink(node)
    sdfDevConsCB_tag *node;
{
    sdfDevConsCB_tag *nextNode = node->next;
    sdfDevConsCB_tag *previousNode = node->previousNode;
    if (nextNode != NULL) {
        nextNode->previousNode = previousNode;
    } else {
        D_00438AB0 = previousNode;
    }
    if (previousNode != NULL) {
        previousNode->next = nextNode;
    }
}

/* Unlink the console, release its cell-buffer allocation, then free the node. */
void sdfDevConsClose(sdfDevConsCB_tag *node) {
    sdfDevConsUnlink(node);
    sdfMemFreeDirect(node->bufferAllocation);
    sdfChipFreeDirect(node);
}

/* Reset both cursor coordinates and clear the two-byte character-cell grid. */
void sdfDevConsClearInside(sdfDevConsCB_tag *node) {
    node->cursorColumn = 0;
    node->cursorRow = 0;
    memset(node->cells, 0, node->columns * node->rows * SDF_CONSOLE_CELL_BYTES);
}

/* Reset a console through the existing clear operation. */
void sdfDevConsClear(sdfDevConsCB_tag *node) {
    sdfDevConsClearInside(node);
}

/* Allocate a two-byte character grid and link it for cleanup.
 * Allocation uses the supplied dimensions; clearing uses their stored s16
 * values. Dimensions and the two opaque halfword inputs are not validated. */
sdfDevConsCB_tag *sdfDevConsOpen(u32 screenOriginX, u32 screenOriginY, s32 columns, s32 rows) {
    sdfDevConsCB_tag *node;
    struct sdfMemHandle_t *bufferAllocation;

    sdfDevConsInit();
    node = sdfChipAllocDirect(SDF_CONSOLE_NODE_BYTES);
    node->screenOriginX = screenOriginX;
    node->screenOriginY = screenOriginY;
    node->columns = columns;
    node->rows = rows;
    node->tabWidth = 8;
    node->controlByte = 0;
    node->textAttribute = 0;
    bufferAllocation = sdfMemAllocDirect((columns * rows) * SDF_CONSOLE_CELL_BYTES);
    node->bufferAllocation = bufferAllocation;
    node->cells = (u8 *)sdfMemLock(bufferAllocation);
    sdfDevConsClearInside(node);
    sdfDevConsLinkToTop(node);
    return node;
}

/* Packed console glyph UVs for characters 0x20 through 0x7F. */
u32 sdfDevConsFontUvTable[96] __attribute__((aligned(8))) = {
    0x00000000, 0x000000C0, 0x00000180, 0x00000240, 0x00000300, 0x000003C0, 0x00000480, 0x00000540,
    0x00000600, 0x000006C0, 0x00C00000, 0x00C000C0, 0x00C00180, 0x00C00240, 0x00C00300, 0x00C003C0,
    0x00C00480, 0x00C00540, 0x00C00600, 0x00C006C0, 0x01800000, 0x018000C0, 0x01800180, 0x01800240,
    0x01800300, 0x018003C0, 0x01800480, 0x01800540, 0x01800600, 0x018006C0, 0x02400000, 0x024000C0,
    0x02400180, 0x02400240, 0x02400300, 0x024003C0, 0x02400480, 0x02400540, 0x02400600, 0x024006C0,
    0x03000000, 0x030000C0, 0x03000180, 0x03000240, 0x03000300, 0x030003C0, 0x03000480, 0x03000540,
    0x03000600, 0x030006C0, 0x03C00000, 0x03C000C0, 0x03C00180, 0x03C00240, 0x03C00300, 0x03C003C0,
    0x03C00480, 0x03C00540, 0x03C00600, 0x03C006C0, 0x04800000, 0x048000C0, 0x04800180, 0x04800240,
    0x04800300, 0x048003C0, 0x04800480, 0x04800540, 0x04800600, 0x048006C0, 0x05400000, 0x054000C0,
    0x05400180, 0x05400240, 0x05400300, 0x054003C0, 0x05400480, 0x05400540, 0x05400600, 0x054006C0,
    0x06000000, 0x060000C0, 0x06000180, 0x06000240, 0x06000300, 0x060003C0, 0x06000480, 0x06000540,
    0x06000600, 0x060006C0, 0x06C00000, 0x06C000C0, 0x06C00180, 0x06C00240, 0x06C00300, 0x06C003C0,
};

/* Mutable colors used by the device console and packet command setter. */
u32 sdfDevConsColorTable[16] = {
    0x80808080, 0x80800000, 0x80000080, 0x80800080,
    0x80008000, 0x80808000, 0x80008080, 0x80606060,
    0x80202020, 0x80400000, 0x80000040, 0x80400040,
    0x80004000, 0x80404000, 0x80004040, 0x00000000,
};

#define SDF_DEVCONS_CELL_BYTES 2

#define SDF_DEVCONS_TEXT_BUFFER_BYTES 0x200

#define SDF_DEVCONS_NONEMPTY_SHIFT 3

#define SDF_DEVCONS_GIF_TAG_BYTES 0x10

#define SDF_DMA_QWORD_SHIFT 4

#define SDF_DMA_CHCR_TTE 0x40

#define SDF_DMA_ADDRESS_MASK 0x0FFFFFFF

void sdfDevConsPutInside(sdfDevConsCB_tag *console, s32 character, s32 attribute);

/* Scroll only at the last row; otherwise advance it. Always reset the column. */
void sdfDevConsNewLine(sdfDevConsCB_tag *console) {
    if (console->cursorRow == console->rows - 1) {
        s32 rowBytes = console->columns * SDF_DEVCONS_CELL_BYTES;
        s32 copyBytes = rowBytes * console->cursorRow;
        if (console->cursorRow > 0) {
            memmove(console->cells, console->cells + rowBytes, copyBytes);
        }
        memset(console->cells + copyBytes, 0, rowBytes);
    } else {
        console->cursorRow = (u16)console->cursorRow + 1;
    }
    console->cursorColumn = 0;
}

/* Store one character/attribute cell, interpreting cursor controls and row wrap. */
void sdfDevConsPutInside(sdfDevConsCB_tag *console, s32 character, s32 attribute) {
    s32 tabCellsRemaining;
    s32 cursorColumnRemainder;
    s32 tabWidth;
    s32 offset;

    switch (character) {
    case 9:
        offset = (console->cursorRow * console->columns + console->cursorColumn) * SDF_DEVCONS_CELL_BYTES;
        tabWidth = console->tabWidth;
        cursorColumnRemainder = console->cursorColumn % tabWidth;
        tabCellsRemaining = cursorColumnRemainder ? cursorColumnRemainder : tabWidth;
        do {
            console->cells[offset] = 0;
            console->cells[offset + 1] = attribute;
            offset += SDF_DEVCONS_CELL_BYTES;
            console->cursorColumn = (u16)console->cursorColumn + 1;
            if (console->cursorColumn == console->columns) {
                sdfDevConsNewLine(console);
                break;
            }
        } while (--tabCellsRemaining != 0);
        break;
    case 10:
        sdfDevConsNewLine(console);
        return;
    case 13:
        console->cursorColumn = 0;
        break;
    default:
        if (character == 0x20) {
            character = 0;
        } else if ((u32)character - 0x20U >= 0x60U) {
            character = 0x7F;
        }
        offset = (console->cursorRow * console->columns + console->cursorColumn) * SDF_DEVCONS_CELL_BYTES;
        console->cells[offset] = character;
        console->cells[offset + 1] = attribute;
        console->cursorColumn = (u16)console->cursorColumn + 1;
        if (console->cursorColumn == console->columns) {
            sdfDevConsNewLine(console);
        }
        break;
    }
}

/* Forward a character and explicit attribute to the console renderer. */
void sdfDevConsPut(sdfDevConsCB_tag *console, s32 character, s32 attribute) {
    sdfDevConsPutInside(console, character, attribute);
}

/* Return the emitted character count. The formatter receives no buffer limit. */
s32 sdfDevConsPrintf(sdfDevConsCB_tag *console, const char *format, ...) {
    char textBuffer[SDF_DEVCONS_TEXT_BUFFER_BYTES];
    __builtin_va_list arguments;
    char *textCursor;
    s32 characterCount = 0;
    s32 character;
    __builtin_stdarg_start(arguments, format);
    vsprintf(textBuffer, format, arguments);
    character = textBuffer[0];
    if (character != 0) {
        do {
            characterCount++;
            sdfDevConsPutInside(console, character, console->textAttribute);
            textCursor = textBuffer + characterCount;
            character = *textCursor;
        } while (character != 0);
    }
    return characterCount;
}

/* Set the next cell coordinates without clamping them to the grid. */
void sdfDevConsLocate(sdfDevConsCB_tag *console, s16 column, s16 row) {
    console->cursorColumn = column;
    console->cursorRow = row;
}

/* Select the attribute passed to each subsequently printed character. */
void sdfDevConsAttribute(sdfDevConsCB_tag *console, u8 attribute) {
    console->textAttribute = attribute;
}

/* Read or write the console control byte at offset 0x14. */
u8 sdfDevConsGetMode(sdfDevConsCB_tag *console) {
    return console->controlByte;
}

/* Store the control byte unchanged; its individual bits are not decoded here. */
void sdfDevConsSetMode(sdfDevConsCB_tag *console, u8 controlByte) {
    console->controlByte = controlByte;
}

struct sdfTexHandle_t;

/* Close the ordered register-list tag in the console's uncached DMA buffer.
 * Five eight-byte register values form one sprite; pad an odd value count. */
void sdfDevConsTagClose(sdfDevConsDrawCB_t *buffers) {
    u8 *cursor = buffers->writeCursor;
    vu64 *tag = (vu64 *)buffers->currentTag;
    s32 registerCount = ((cursor - buffers->currentTag) >> 3) - 2;

    if (registerCount != 0) {
        if (registerCount & 1) {
            cursor += 8;
        }
        registerCount /= 5;
        buffers->currentTag = cursor;
        buffers->writeCursor = buffers->currentTag + SDF_DEVCONS_GIF_TAG_BYTES;
        tag[0] = (s64)registerCount | 0x5400000000008000ULL;
        tag[1] = 0x53531;
    }
}

/* Finalize the current GIF tag and submit completed packets, then rotate buffers.
 * Keep the eight-byte pending-data test distinct from the quadword DMA count.
 */
void sdfDevConsDrawFlush(sdfDevConsDrawCB_t *packetBuffers) {
    s32 packetBytes;
    vu32 *channelRegisters;
    u32 nextBufferIndex;

    sdfDevConsTagClose(packetBuffers);
    packetBytes = packetBuffers->currentTag - packetBuffers->packetStart;
    if ((packetBytes >> SDF_DEVCONS_NONEMPTY_SHIFT) != 0) {
        channelRegisters = packetBuffers->dmaChannel;
        sceDmaSync((void *)channelRegisters, 0, 0);
        HW_WRITE(channelRegisters, HW_READ(channelRegisters) | SDF_DMA_CHCR_TTE); /* CHCR bit 6: tag-transfer enable */
        EE_SYNC();
        sceDmaSendN((void *)channelRegisters, (void *)((u32)packetBuffers->packetStart & SDF_DMA_ADDRESS_MASK), packetBytes >> SDF_DMA_QWORD_SHIFT);
        nextBufferIndex = packetBuffers->activeBufferIndex ^ 1;
        packetBuffers->activeBufferIndex = nextBufferIndex;
        packetBuffers->currentTag = (u8 *)packetBuffers->bufferAddresses[nextBufferIndex];
        packetBuffers->writeCursor = packetBuffers->currentTag + SDF_DEVCONS_GIF_TAG_BYTES;
        packetBuffers->packetStart = packetBuffers->currentTag;
        sceGsSyncPath(0, 0);
    }
}

/* Append the console background sprite, then reserve the next glyph tag.
 * Packed coordinates wrap as words before their zero-extended packet writes. */
void sdfDevConsDrawBG(sdfDevConsCB_tag *console, sdfDevConsDrawCB_t *packetBuffers) {
    vu64 *packet;
    u32 origin;
    u32 extent;

    sdfDevConsTagClose(packetBuffers);
    if (packetBuffers->bufferBytes < packetBuffers->currentTag - packetBuffers->packetStart + 0x80) {
        sdfDevConsDrawFlush(packetBuffers);
    }
    packet = (vu64 *)packetBuffers->currentTag;
    origin = (u16)(console->screenOriginX - 0x30) | ((u32)console->screenOriginY << 16);
    extent = (u16)(console->columns * 3 * 0x40 + 0x70) | ((u32)(console->rows * packetBuffers->rowStep) << 16);
    if (console->controlByte & 4) {
        origin += 0xFFD00000;
        extent += 0x700000;
    } else {
        origin += 0xFFE80000;
        extent += 0x380000;
    }
    packet[0] = 0x5400000000008001ULL;
    packet[1] = 0x5510;
    packet[2] = 0x146;
    packet[3] = 0x30000000;
    packet[4] = origin;
    packet[5] = origin + extent;
    packet[6] = 0x156;
    packetBuffers->currentTag = (u8 *)packet + 0x40;
    packetBuffers->writeCursor = packetBuffers->currentTag + SDF_DEVCONS_GIF_TAG_BYTES;
}

extern u32 sdfDevConsColorTable[];

/* Emit five ordered register values per glyph into the console DMA buffer.
 * The native do-loops have no zero-dimension guard. */
void sdfDevConsDraw(sdfDevConsCB_tag *console, sdfDevConsDrawCB_t *buffers) {
    u8 *cells;
    volatile SdfConsoleGlyph *writeCursor;
    u8 *packetStart;
    s32 row;
    s32 column;
    s32 y;
    s32 cellStep;
    u64 top;
    s32 character;
    s32 attribute;
    u64 glyph;
    u64 color;

    if ((u8)(console->controlByte & 1)) {
        return;
    }
    buffers->rowStep = (console->controlByte & 4) ? 0xC0 : 0x60;
    if (console->controlByte & 2) {
        sdfDevConsDrawBG(console, buffers);
    }
    cellStep = ((u32)buffers->rowStep << 16) | 0xC0;
    row = 0;
    writeCursor = (volatile SdfConsoleGlyph *)buffers->writeCursor;
    packetStart = buffers->packetStart;
    cells = console->cells;
    y = console->screenOriginY;
    do {
        top = (u32)console->screenOriginX | ((u32)y << 16);
        column = 0;
        do {
            character = cells[0];
            attribute = cells[1];
            cells += 2;
            if (character >= 0x20) {
                if (buffers->bufferBytes < (u8 *)writeCursor - packetStart + 0x50) {
                    buffers->writeCursor = (u8 *)writeCursor;
                    sdfDevConsDrawFlush(buffers);
                    writeCursor = (volatile SdfConsoleGlyph *)buffers->writeCursor;
                    packetStart = buffers->packetStart;
                }
                glyph = sdfDevConsFontUvTable[character - 0x20];
                color = sdfDevConsColorTable[attribute];
                writeCursor->color = color;
                writeCursor->firstUv = glyph;
                writeCursor->firstPosition = top;
                writeCursor->secondUv = glyph + 0xC000C0;
                writeCursor->secondPosition = top + cellStep;
                writeCursor++;
            }
            column++;
            top += 0xC0;
        } while (column != console->columns);
        row++;
        y += buffers->rowStep;
    } while (row != console->rows);
    buffers->writeCursor = (u8 *)writeCursor;
}

/* Initialize both uncached packet buffers and complete the GS setup packet.
 * Reserve the first glyph tag only after the setup values have been written. */
void sdfDevConsDrawInit(sdfDevConsDrawCB_t *packetBuffers, u8 *storageBase, s32 storageCapacityBytes) {
    u32 ringAddress;
    u8 *ring;
    vu64 *packet;
    u64 textureState;
    s32 halfBufferBytes = storageCapacityBytes >> 1;

    ringAddress = ((u32)storageBase & SDF_DMA_ADDRESS_MASK) | 0x30000000;
    ring = (u8 *)ringAddress;
    packetBuffers->bufferBytes = halfBufferBytes;
    packetBuffers->bufferAddresses[0] = ring;
    packetBuffers->bufferAddresses[1] = ring + halfBufferBytes;
    packetBuffers->packetStart = ring;
    packetBuffers->activeBufferIndex = 0;
    textureState = sdfTexGetTex0Reg(sdfDebugFontHandle);
    packet = (vu64 *)ring;
    packet[0] = 0x10AB400000008005ULL;
    packet[1] = 0xE;
    packet[2] = 0x44;
    packet[3] = 0x42;
    packet[4] = 0x3000D;
    packet[5] = 0x47;
    packet[6] = 0;
    packet[7] = 8;
    packet[8] = 1;
    packet[9] = 0x14;
    packet[10] = textureState;
    packet[11] = 6;
    packetBuffers->currentTag = ring + 0x60;
    packetBuffers->writeCursor = packetBuffers->currentTag + SDF_DEVCONS_GIF_TAG_BYTES;
    packetBuffers->dmaChannel = sceDmaGetChan(2);
}

void sdfDevConsDrawInit(sdfDevConsDrawCB_t *packetBuffers, u8 *storageBase, s32 storageCapacityBytes);

void sdfDevConsDraw(sdfDevConsCB_tag *console, sdfDevConsDrawCB_t *buffers);

/* Render every console in the global chain into a stack packet buffer and submit it. */
void sdfDevConsDrawAll(void) {
    DevConsStackPackets storage;
    sdfDevConsDrawCB_t packetBuffers;
    sdfDevConsCB_tag *console;

    if (D_00438AB0 != NULL) {
        sdfDevConsDrawInit(&packetBuffers, storage.data, 0x1000);
        for (console = D_00438AB0; console != NULL; console = console->previousNode) {
            sdfDevConsDraw(console, &packetBuffers);
        }
        sdfDevConsDrawFlush(&packetBuffers);
    }
}

/* SifCommand is the complete 0x10-byte input value. Console rendering uses
 * source/end as x/y storage, argument as depth bits, and command as RGBA.
 * Its RPC source/end interpretation remains unchanged. */
extern void *sdfDevVprintf(SifCommand *input, const char *format, void *args);

extern void sdfDevConsInit(void);

extern u32 sdfDevConsFontUvTable[96];

/* Use the cached console font texture for this draw packet. */
extern struct sdfTexHandle_t *sdfDebugFontHandle;

void *sdfDevVprintf(SifCommand *input, const char *format, void *arguments) {
    char text[0x200];
    s32 formattedCount;
    u8 *start;
    u8 *cursor;
    SdfConsolePacketHeader *header;
    u8 *textCursor;
    s32 character;
    s32 glyphCount;
    u32 color;
    u64 position;
    s32 quadwordCount;

    sdfDevConsInit();
    formattedCount = vsprintf(text, format, arguments);
    start = (u8 *)sdfGrOpenPacketBuff();
    if (formattedCount == 0) {
        *(u128 *)start = 0;
        cursor = start + 0x10;
    } else {
        header = (SdfConsolePacketHeader *)start;
        cursor = start + sizeof(*header);
        glyphCount = 0;
        /* This dual-use command carries x, y, depth and RGBA in its four words. */
        position = (u32)((u16)input->source | ((u32)(u16)input->end << 16)) |
                   ((u64)(u32)input->argument << 32);
        color = input->command;
        textCursor = (u8 *)text;
        while ((character = *textCursor++) != 0) {
            if (character >= 0x20) {
                SdfConsoleGlyph *glyph = (SdfConsoleGlyph *)cursor;
                u32 uv = sdfDevConsFontUvTable[character - 0x20];

                glyph->color = color;
                glyph->firstUv = uv;
                glyph->firstPosition = position;
                glyph->secondUv = (u32)(uv + 0x00C000C0);
                glyph->secondPosition = position + 0x006000C0;
                cursor += sizeof(*glyph);
                glyphCount++;
            }
            /* Control bytes consume the same horizontal advance as glyphs. */
            position += 0xC0;
        }
        if (glyphCount & 1) {
            *(u64 *)cursor = 0;
            cursor += sizeof(u64);
        }
        quadwordCount = ((s32)(cursor - start) >> 4) - 1;
        input->source = (u16)position;
        header->quadwordCount = quadwordCount;
        header->vifNop = 0;
        header->vifDirect = 0x50000000 | quadwordCount;
        header->setupTag = 0x10AB400000000003ULL;
        header->setupRegisters = 0xEEE;
        header->textureClamp = 0;
        header->textureClampRegister = 8;
        header->textureFilter = 1;
        header->textureFilterRegister = 0x14;
        header->textureState = sdfTexGetTex0Reg(sdfDebugFontHandle);
        header->textureStateRegister = 6;
        header->glyphTag = 0x5400000000008000ULL | glyphCount;
        header->glyphRegisters = 0x53531;
    }
    sdfGrClosePacketBuff((s32)cursor);
    return start;
}

void *sdfDevPrintfEx(SifCommand *packet, const char *fmt, ...) {
    __builtin_va_list args;

    __builtin_stdarg_start(args, fmt);
    return sdfDevVprintf(packet, fmt, args);
}

void *sdfDevPrintf(s32 source, s32 end, s32 argument, s32 index, const char *fmt, ...) {
    SifCommand packet;
    __builtin_va_list args;

    sdfDevSetPrintCB(&packet, source, end, argument, index);
    __builtin_stdarg_start(args, fmt);
    return sdfDevVprintf(&packet, fmt, args);
}

void sdfPktSetCmd(SifCommand *packet, s32 index) {
    packet->command = sdfDevConsColorTable[index];
}

void sdfDevSetPrintCB(SifCommand *packet, s32 source, s32 end, s32 argument, s32 index) {
    packet->source = source;
    packet->end = end;
    packet->argument = argument;
    sdfPktSetCmd(packet, index);
}
INCLUDE_SDATA(const s32, "sdf/sdfDevCons", D_00438AB0);

INCLUDE_SDATA(const s32, "sdf/sdfDevCons", D_00438AB4);

