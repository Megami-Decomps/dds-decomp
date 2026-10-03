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
    u8 state;
    u8 stopRequested;
    u8 pad3;
    u8 pad4[0xC];
    DevState *deviceState;
    s32 totalBytes;
    s32 remainingBytes;
    MovSub *stream;
} MovObj;

s32 sdfDevQueueRead(DevState *state, void *data, s32 size);
s32 sdfDevQueueControlRequest(DevState *state);
s32 sdfDevQueueActiveOperation(DevState *state);
s32 sdfDevQueueReleaseState(DevState *state);
s32 func_0036DE70(void);
s32 EIntr(void);

void func_00345E18(MovObj *movie) {
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
        movie->state = 5;
        return;
    }

    movie->state = 4;
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

s32 func_00345EB0(DevState *deviceState, s32 operation, void *data, s32 bytesRead, MovObj *movie) {
    MovSub *stream;
    s32 restoreInterrupts;

    movie->deviceState = deviceState;
    stream = movie->stream;

    if (movie->stopRequested != 0 && (movie->state < 6 || movie->state > 7)) {
        movie->state = 7;
        sdfDevQueueActiveOperation(deviceState);
        return 0;
    }

    switch (movie->state) {
    case 0:
        if (operation == 2) {
            movie->state = 1;
            sdfDevQueueControlRequest(deviceState);
        }
        break;
    case 1:
        if (operation == 4) {
            movie->totalBytes = bytesRead;
            movie->remainingBytes = bytesRead;
            func_00345E18(movie);
        }
        break;
    case 4:
        if (operation == 5) {
            restoreInterrupts = func_0036DE70();
            stream->bufferedBytes += bytesRead;
            movie->remainingBytes -= bytesRead;
            if (restoreInterrupts != 0) {
                EIntr();
            }
            if (movie->remainingBytes == 0) {
                movie->state = 7;
                sdfDevQueueActiveOperation(deviceState);
            } else {
                func_00345E18(movie);
            }
        }
        break;
    case 7:
        if (operation == 7) {
            movie->deviceState = NULL;
            movie->state = 6;
            sdfDevQueueReleaseState(deviceState);
        }
        break;
    }

    return 0;
}

void sdfMovieProcessPendingData(MovObj *movie) {
    MovSub *stream;
    s32 remaining;

    stream = movie->stream;
    remaining = movie->remainingBytes;
    if (remaining == 0) {
        return;
    }
    if ((0x10000 - stream->unk5C) < 0x4000 || ((stream->unk6C - stream->unk68) + 0x10000) < 0x4000) {
        movie->state = 5;
        return;
    }
    movie->state = 4;
    sdfDevQueueRead(movie->deviceState, stream->pendingCursor, remaining <= 0x4000 ? remaining : 0x4000);
}

INCLUDE_ASM(const s32, "sdf/sdfMovie", func_003460D8);

INCLUDE_ASM(const s32, "sdf/sdfMovie", func_00346468);
