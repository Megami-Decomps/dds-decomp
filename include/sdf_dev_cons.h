#ifndef SDF_DEV_CONS_H
#define SDF_DEV_CONS_H

#include "common.h"

typedef struct DevConsState {
    u8 pad0[4]; /* 0x00 */
    struct DevConsState *unk4; /* 0x04 */
    u16 unk8; /* 0x08 */
    u16 unkA; /* 0x0A */
    s16 columns; /* 0x0C */
    s16 rows; /* 0x0E */
    s16 cursorColumn; /* 0x10 */
    s16 cursorRow; /* 0x12 */
    u8 controlByte; /* 0x14 */
    u8 pad15;
    u8 textAttribute; /* 0x16 */
    u8 tabWidth; /* 0x17: used to advance tabs to the next column stop */
    u8 pad18[4]; /* 0x18 */
    u8 *cells; /* 0x1C: two bytes per character cell */
} DevConsState;

typedef struct ConsBuf {
    u8 *writeCursor; /* 0x00: next glyph-data write */
    u8 *packetStart; /* 0x04: beginning of the DMA range */
    u8 *currentTag; /* 0x08: reserved, not-yet-finalized GIF tag */
    void *dmaChannel; /* 0x0C */
    u8 *bufferAddresses[2]; /* 0x10: start of each alternating packet buffer */
    s32 bufferBytes; /* 0x18: byte capacity of each buffer */
    u16 activeBufferIndex; /* 0x1C */
    u16 rowStep; /* 0x1E: added when the renderer advances to the next row */
} ConsBuf;

/* Update the console state stored at the corresponding field offsets. */
void sdfDevConsSetControlByte(DevConsState *console, u8 controlByte);
void sdfDevConsSetTextAttribute(DevConsState *console, u8 attribute);

typedef char DevConsState_size_must_be_0x20[(sizeof(DevConsState) == 0x20) ? 1 : -1];
typedef char DevConsState_cells_at_0x1C[((u32)&((DevConsState *)0)->cells == 0x1C) ? 1 : -1];
typedef char ConsBuf_size_must_be_0x20[(sizeof(ConsBuf) == 0x20) ? 1 : -1];
typedef char ConsBuf_bufferAddresses_at_0x10[((u32)&((ConsBuf *)0)->bufferAddresses == 0x10) ? 1 : -1];
typedef char ConsBuf_activeBufferIndex_at_0x1C[((u32)&((ConsBuf *)0)->activeBufferIndex == 0x1C) ? 1 : -1];

#endif
