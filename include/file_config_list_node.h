#ifndef FILE_CONFIG_LIST_NODE_H
#define FILE_CONFIG_LIST_NODE_H

#include "common.h"

typedef struct FileConfigCountdown {
    s32 ticks;
} FileConfigCountdown;

typedef struct FileConfigListNode {
    s32 index;                       /* 0x00 */
    u8 pad04[0x54];
    struct FileConfigListNode *next; /* 0x58 */
    u8 pad5C[0x14];
    FileConfigCountdown *resource;   /* 0x70 */
} FileConfigListNode;

typedef char FileConfigCountdownSizeCheck[
    sizeof(FileConfigCountdown) == 0x04 ? 1 : -1];
typedef char FileConfigListNodeSizeCheck[
    sizeof(FileConfigListNode) == 0x74 ? 1 : -1];
typedef char FileConfigListNodeNextOffsetCheck[
    (u32)&((FileConfigListNode *)0)->next == 0x58 ? 1 : -1];
typedef char FileConfigListNodeResourceOffsetCheck[
    (u32)&((FileConfigListNode *)0)->resource == 0x70 ? 1 : -1];

#endif
