#ifndef SDF_PAC_DECODER_H
#define SDF_PAC_DECODER_H

#include "common.h"

/* Incremental decoder access view shared by both providers. The backing
 * decoder block is allocated as 0x20 bytes; only this 0x14-byte prefix is
 * described here, and its trailing allocation bytes remain unknown. */
typedef struct SdfPacDecoder {
    u8 state;             /* 0x00 */
    u8 command;           /* 0x01 */
    u16 pendingBytes;     /* 0x02 */
    u8 *output;           /* 0x04 */
    u8 *input;            /* 0x08 */
    s32 inputBytes;       /* 0x0C */
    u8 backrefLow;        /* 0x10 */
    u8 pad11[3];          /* 0x11: opaque */
} SdfPacDecoder;

s32 sdfPacDecodeInputChunk(SdfPacDecoder *decoder, u8 *input, s32 inputBytes);

typedef char SdfPacDecoder_size_must_be_0x14[
    (sizeof(SdfPacDecoder) == 0x14) ? 1 : -1];
typedef char SdfPacDecoder_pendingBytes_must_be_at_2[
    ((u32)&((SdfPacDecoder *)0)->pendingBytes == 0x02) ? 1 : -1];
typedef char SdfPacDecoder_output_must_be_at_4[
    ((u32)&((SdfPacDecoder *)0)->output == 0x04) ? 1 : -1];
typedef char SdfPacDecoder_input_must_be_at_8[
    ((u32)&((SdfPacDecoder *)0)->input == 0x08) ? 1 : -1];
typedef char SdfPacDecoder_inputBytes_must_be_at_C[
    ((u32)&((SdfPacDecoder *)0)->inputBytes == 0x0C) ? 1 : -1];
typedef char SdfPacDecoder_backrefLow_must_be_at_10[
    ((u32)&((SdfPacDecoder *)0)->backrefLow == 0x10) ? 1 : -1];

#endif /* SDF_PAC_DECODER_H */
