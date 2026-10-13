#ifndef SDF_CONS_MATRIX_PACKET_H
#define SDF_CONS_MATRIX_PACKET_H

#include "common.h"

/* Shared 0xD0 VIF packet: two stored matrices, three vector slots, and trailer. */
typedef struct ConsMatrixPacket {
    u16 quadwords;        /* 0x00 */
    u8 unknown02[6];      /* 0x02 */
    u32 reservedWord;     /* 0x08 */
    u32 command;          /* 0x0C */
    u8 matrixA[0x40];     /* 0x10 */
    u8 matrixB[0x40];     /* 0x50 */
    u8 vecA[0x10];        /* 0x90 */
    u8 vecB[0x10];        /* 0xA0 */
    u8 vecC[0x10];        /* 0xB0 */
    u32 stmodCommand;     /* 0xC0 */
    u32 mscalCommand;     /* 0xC4 */
    u32 unknownC8;        /* 0xC8 */
    u32 unknownCC;        /* 0xCC */
} ConsMatrixPacket;

typedef ConsMatrixPacket SdfVuBonePacket;

typedef char ConsMatrixPacket_size_must_be_0xD0[
    (sizeof(ConsMatrixPacket) == 0xD0) ? 1 : -1];
typedef char ConsMatrixPacket_quadwords_offset_must_be_0[
    ((u32)&((ConsMatrixPacket *)0)->quadwords == 0x00) ? 1 : -1];
typedef char ConsMatrixPacket_reservedWord_offset_must_be_8[
    ((u32)&((ConsMatrixPacket *)0)->reservedWord == 0x08) ? 1 : -1];
typedef char ConsMatrixPacket_command_offset_must_be_C[
    ((u32)&((ConsMatrixPacket *)0)->command == 0x0C) ? 1 : -1];
typedef char ConsMatrixPacket_matrixA_offset_must_be_10[
    ((u32)&((ConsMatrixPacket *)0)->matrixA == 0x10) ? 1 : -1];
typedef char ConsMatrixPacket_matrixB_offset_must_be_50[
    ((u32)&((ConsMatrixPacket *)0)->matrixB == 0x50) ? 1 : -1];
typedef char ConsMatrixPacket_vecA_offset_must_be_90[
    ((u32)&((ConsMatrixPacket *)0)->vecA == 0x90) ? 1 : -1];
typedef char ConsMatrixPacket_vecB_offset_must_be_A0[
    ((u32)&((ConsMatrixPacket *)0)->vecB == 0xA0) ? 1 : -1];
typedef char ConsMatrixPacket_vecC_offset_must_be_B0[
    ((u32)&((ConsMatrixPacket *)0)->vecC == 0xB0) ? 1 : -1];
typedef char ConsMatrixPacket_stmodCommand_offset_must_be_C0[
    ((u32)&((ConsMatrixPacket *)0)->stmodCommand == 0xC0) ? 1 : -1];
typedef char ConsMatrixPacket_mscalCommand_offset_must_be_C4[
    ((u32)&((ConsMatrixPacket *)0)->mscalCommand == 0xC4) ? 1 : -1];
typedef char ConsMatrixPacket_unknownC8_offset_must_be_C8[
    ((u32)&((ConsMatrixPacket *)0)->unknownC8 == 0xC8) ? 1 : -1];
typedef char ConsMatrixPacket_unknownCC_offset_must_be_CC[
    ((u32)&((ConsMatrixPacket *)0)->unknownCC == 0xCC) ? 1 : -1];

#endif /* SDF_CONS_MATRIX_PACKET_H */
