#ifndef BILL_STATE_PACKET_H
#define BILL_STATE_PACKET_H

#include "common.h"

/* 0x40-byte GS state packet, allocated and populated by both billboard managers. */
typedef struct BillStatePacket {
    u64 dmaTag;         /* 0x00 */
    u64 vifCommands;    /* 0x08 */
    u64 gifTag;         /* 0x10 */
    u64 gifRegisters;   /* 0x18 */
    u64 test;           /* 0x20 */
    u64 testRegister;   /* 0x28 */
    u64 alpha;          /* 0x30 */
    u64 alphaRegister;  /* 0x38 */
} BillStatePacket;

typedef char BillStatePacket_size_must_be_0x40[
    sizeof(BillStatePacket) == 0x40 ? 1 : -1];
typedef char BillStatePacket_test_at_0x20[
    ((u32)&((BillStatePacket *)0)->test == 0x20) ? 1 : -1];
typedef char BillStatePacket_alpha_at_0x30[
    ((u32)&((BillStatePacket *)0)->alpha == 0x30) ? 1 : -1];
typedef char BillStatePacket_alpha_register_at_0x38[
    ((u32)&((BillStatePacket *)0)->alphaRegister == 0x38) ? 1 : -1];

#endif
