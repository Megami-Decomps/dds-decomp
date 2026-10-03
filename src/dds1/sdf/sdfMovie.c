#include "common.h"

typedef struct DevState DevState;

typedef struct MovSub {
    u8 pad0[0x4];
    u8 *bufferStart;
    u8 pad8[0x4];
    u8 *writeCursor;
    s32 bufferedBytes;
    u8 pad14[0x3C];
    void *pendingCursor;
    u8 pad54[0x8];
    s32 unk5C;
    u8 pad60[0x8];
    s32 unk68;
    s32 unk6C;
} MovSub;

typedef struct MovObj {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    u8 pad4[0xC];
    DevState *deviceState;
    u8 pad14[0x4];
    s32 remainingBytes;
    MovSub *stream;
} MovObj;

s32 sdfDevQueueRead(DevState *state, void *data, s32 size);

void func_002ECF70(MovObj *movie) {
    MovSub *stream = movie->stream;
    s32 remaining = movie->remainingBytes;
    s32 readSize;
    u8 *bufferPosition;
    u8 *streamPosition;
    u8 *ringStart;
    u8 *ringEnd;
    DevState *deviceState;

    if (remaining == 0) {
        return;
    }
    if (0x20000 - stream->bufferedBytes < 0x4000) {
        movie->unk1 = 5;
        return;
    }

    movie->unk1 = 4;
    readSize = 0x4000;
    if (remaining <= 0x4000) {
        readSize = remaining;
    }

    bufferPosition = stream->writeCursor;
    ringStart = stream->bufferStart;
    ringEnd = ringStart + 0x20000;
    deviceState = movie->deviceState;
    streamPosition = bufferPosition + readSize;
    if (streamPosition >= ringEnd) {
        streamPosition = ringStart;
    }
    stream->writeCursor = streamPosition;
    sdfDevQueueRead(deviceState, bufferPosition, readSize);
}

INCLUDE_ASM(const s32, "sdf/sdfMovie", func_002ED008);

void sdfMovieProcessPendingData(MovObj *movie) {
    MovSub *stream;
    s32 remaining;

    stream = movie->stream;
    remaining = movie->remainingBytes;
    if (remaining == 0) {
        return;
    }
    if ((0x10000 - stream->unk5C) < 0x4000 || ((stream->unk6C - stream->unk68) + 0x10000) < 0x4000) {
        movie->unk1 = 5;
        return;
    }
    movie->unk1 = 4;
    sdfDevQueueRead(movie->deviceState, stream->pendingCursor, remaining <= 0x4000 ? remaining : 0x4000);
}

INCLUDE_ASM(const s32, "sdf/sdfMovie", func_002ED230);

INCLUDE_ASM(const s32, "sdf/sdfMovie", func_002ED5C0);
