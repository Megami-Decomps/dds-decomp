#ifndef SDF_DEV_CONSOLE_NODE_H
#define SDF_DEV_CONSOLE_NODE_H

#include "common.h"

typedef struct ConsNode {
    struct ConsNode *next;
    struct ConsNode *prev;
    u16 opaque08[2];
    s16 columns;
    s16 rows;
    u16 cursorColumn;
    u16 cursorRow;
    u8 controlByte;
    u8 opaque15;
    u8 textAttribute;
    u8 opaque17;
    u32 bufferHandle;
    u8 *cells;
} ConsNode;

typedef char ConsNodeSizeCheck[(sizeof(ConsNode) == 0x20) ? 1 : -1];
typedef char ConsNodeDimensionsOffsetCheck[
    (((unsigned int)&((ConsNode *)0)->columns) == 0x0C) ? 1 : -1];
typedef char ConsNodeCellsOffsetCheck[
    (((unsigned int)&((ConsNode *)0)->cells) == 0x1C) ? 1 : -1];

ConsNode *sdfDevConsNodeCreate(u32 first, u32 second, s32 columns, s32 rows);
void sdfDevConsNodeDestroy(ConsNode *node);

#endif /* SDF_DEV_CONSOLE_NODE_H */
