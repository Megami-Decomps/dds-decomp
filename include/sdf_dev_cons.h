#ifndef SDF_DEV_CONS_H
#define SDF_DEV_CONS_H

#include "common.h"

struct sdfMemHandle_t;

typedef struct sdfDevConsCB_tag {
    struct sdfDevConsCB_tag *next; /* 0x00: forward console-list link. */
    struct sdfDevConsCB_tag *previousNode; /* 0x04: linked-list predecessor; the global head is the tail. */
    u16 screenOriginX; /* 0x08: GS 12.4 X coordinate used for the console sprite/grid. */
    u16 screenOriginY; /* 0x0A: GS 12.4 Y coordinate used for the console sprite/grid. */
    s16 columns; /* 0x0C */
    s16 rows; /* 0x0E */
    s16 cursorColumn; /* 0x10 */
    s16 cursorRow; /* 0x12 */
    u8 controlByte; /* 0x14 */
    u8 pad15;
    u8 textAttribute; /* 0x16 */
    u8 tabWidth; /* 0x17: used to advance tabs to the next column stop */
    struct sdfMemHandle_t *bufferAllocation; /* 0x18: owns the cell buffer. */
    u8 *cells; /* 0x1C: two bytes per character cell */
} sdfDevConsCB_tag;

typedef struct sdfDevConsDrawCB_t {
    u8 *writeCursor; /* 0x00: next glyph-data write */
    u8 *packetStart; /* 0x04: beginning of the DMA range */
    u8 *currentTag; /* 0x08: reserved, not-yet-finalized GIF tag */
    u32 *dmaChannel; /* 0x0C: DMA channel registers; CHCR is the first word. */
    u8 *bufferAddresses[2]; /* 0x10: start of each alternating packet buffer */
    s32 bufferBytes; /* 0x18: byte capacity of each buffer */
    u16 activeBufferIndex; /* 0x1C */
    u16 rowStep; /* 0x1E: added when the renderer advances to the next row */
} sdfDevConsDrawCB_t;

/* Update the console state stored at the corresponding field offsets. */
void sdfDevConsSetMode(sdfDevConsCB_tag *console, u8 controlByte);
void sdfDevConsAttribute(sdfDevConsCB_tag *console, u8 attribute);
s32 sdfDevConsPrintf(sdfDevConsCB_tag *console, const char *format, ...);

typedef char DevConsState_size_must_be_0x20[(sizeof(sdfDevConsCB_tag) == 0x20) ? 1 : -1];
typedef char DevConsState_previousNode_at_0x04[
    ((u32)&((sdfDevConsCB_tag *)0)->previousNode == 0x04) ? 1 : -1];
typedef char DevConsState_screenOriginX_at_0x08[
    ((u32)&((sdfDevConsCB_tag *)0)->screenOriginX == 0x08) ? 1 : -1];
typedef char DevConsState_screenOriginY_at_0x0A[
    ((u32)&((sdfDevConsCB_tag *)0)->screenOriginY == 0x0A) ? 1 : -1];
typedef char DevConsState_cells_at_0x1C[((u32)&((sdfDevConsCB_tag *)0)->cells == 0x1C) ? 1 : -1];
typedef char ConsBuf_size_must_be_0x20[(sizeof(sdfDevConsDrawCB_t) == 0x20) ? 1 : -1];
typedef char ConsBuf_dmaChannel_at_0x0C[((u32)&((sdfDevConsDrawCB_t *)0)->dmaChannel == 0x0C) ? 1 : -1];
typedef char ConsBuf_bufferAddresses_at_0x10[((u32)&((sdfDevConsDrawCB_t *)0)->bufferAddresses == 0x10) ? 1 : -1];
typedef char ConsBuf_activeBufferIndex_at_0x1C[((u32)&((sdfDevConsDrawCB_t *)0)->activeBufferIndex == 0x1C) ? 1 : -1];

#endif
