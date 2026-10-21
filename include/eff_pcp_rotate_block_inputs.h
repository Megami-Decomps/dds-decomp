#ifndef EFF_PCP_ROTATE_BLOCK_INPUTS_H
#define EFF_PCP_ROTATE_BLOCK_INPUTS_H

#include "common.h"

/* Stack input shared by the rotate parameter-table constructor and its child
 * builder. Slot 3 is reserved for a head pointer that the builder replaces;
 * the remaining slots contain five group, three midpoint, five tail, and one
 * final borrowed parameter block. */
typedef struct EffPCPRotateBlockInputs {
    void *head[3];
    void *selectedHead; /* +0x0C: replaced with a head before the child read. */
    void *group[5];
    void *mid[3];
    void *tail[5];
    void *last;
} EffPCPRotateBlockInputs;

typedef char EffPCPRotateBlockInputs_size_must_be_0x48[
    (sizeof(EffPCPRotateBlockInputs) == 0x48) ? 1 : -1];
typedef char EffPCPRotateBlockInputs_group_at_0x10[
    ((u32)&((EffPCPRotateBlockInputs *)0)->group == 0x10) ? 1 : -1];
typedef char EffPCPRotateBlockInputs_mid_at_0x24[
    ((u32)&((EffPCPRotateBlockInputs *)0)->mid == 0x24) ? 1 : -1];
typedef char EffPCPRotateBlockInputs_tail_at_0x30[
    ((u32)&((EffPCPRotateBlockInputs *)0)->tail == 0x30) ? 1 : -1];
typedef char EffPCPRotateBlockInputs_last_at_0x44[
    ((u32)&((EffPCPRotateBlockInputs *)0)->last == 0x44) ? 1 : -1];

#endif
