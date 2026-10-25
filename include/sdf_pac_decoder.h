#ifndef SDF_PAC_DECODER_H
#define SDF_PAC_DECODER_H

#include "common.h"

/* The stream phase is stored as a byte; zero marks the end marker. */
enum SdfPacDecoderState {
    SDF_DECODE_COMPLETE = 0,
    SDF_DECODE_READ_COMMAND = 1,
    SDF_DECODE_READ_COUNT_LOW = 2,
    SDF_DECODE_READ_COUNT_HIGH = 3,
    SDF_DECODE_DISPATCH = 4,
    SDF_DECODE_COPY_LITERAL = 5,
    SDF_DECODE_FILL_BYTE = 6,
    SDF_DECODE_COPY_SHORT_BACKREF = 7,
    SDF_DECODE_READ_BACKREF_LOW = 8,
    SDF_DECODE_READ_BACKREF_HIGH = 9,
    SDF_DECODE_EXPAND_BYTES = 10,
};

/* sdfDecomp reports whether the stream end marker was consumed, independently
 * of how many bytes from the current input span were consumed. */
typedef s32 SdfPacDecoderResult;
enum SdfPacDecoderResultValue {
    SDF_PAC_DECODE_INCOMPLETE = 0,
    SDF_PAC_DECODE_COMPLETE = 1
};

/* Incremental decoder access view shared by both providers. The backing
 * decoder block is allocated as 0x20 bytes; only this 0x14-byte prefix is
 * described here, and its trailing allocation bytes remain unknown. */
typedef struct SdfPacDecoder {
    u8 state;             /* 0x00: SdfPacDecoderState */
    u8 command;           /* 0x01 */
    u16 pendingBytes;     /* 0x02 */
    u8 *output;           /* 0x04 */
    const u8 *input;      /* 0x08 */
    s32 remainingInputBytes; /* 0x0C: bytes left in the current input span */
    u8 backrefLow;        /* 0x10 */
    u8 pad11[3];          /* 0x11: opaque */
} SdfPacDecoder;

SdfPacDecoderResult sdfDecomp(SdfPacDecoder *decoder, const u8 *input, s32 inputBytes);
void sdfDecompInit(SdfPacDecoder *decoder, u8 *output);

typedef char SdfPacDecoder_size_must_be_0x14[
    (sizeof(SdfPacDecoder) == 0x14) ? 1 : -1];
typedef char SdfPacDecoder_pendingBytes_must_be_at_2[
    ((u32)&((SdfPacDecoder *)0)->pendingBytes == 0x02) ? 1 : -1];
typedef char SdfPacDecoder_output_must_be_at_4[
    ((u32)&((SdfPacDecoder *)0)->output == 0x04) ? 1 : -1];
typedef char SdfPacDecoder_input_must_be_at_8[
    ((u32)&((SdfPacDecoder *)0)->input == 0x08) ? 1 : -1];
typedef char SdfPacDecoder_remainingInputBytes_must_be_at_C[
    ((u32)&((SdfPacDecoder *)0)->remainingInputBytes == 0x0C) ? 1 : -1];
typedef char SdfPacDecoder_backrefLow_must_be_at_10[
    ((u32)&((SdfPacDecoder *)0)->backrefLow == 0x10) ? 1 : -1];

#endif /* SDF_PAC_DECODER_H */
