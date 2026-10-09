#ifndef MDL_OBJECT_STREAM_H
#define MDL_OBJECT_STREAM_H

#include "sdf.h"

/* These four bytes are consumed by the same sound-format initializer. */
typedef struct SoundFormat {
    u8 hasAudio;
    u8 stereo;
    u8 loopMode;
    u8 playbackCadenceStep;
} SoundFormat;
typedef SoundFormat SdfStreamParams;

typedef char SdfStreamParams_size_must_be_4[(sizeof(SdfStreamParams) == 4) ? 1 : -1];

/* The model-object allocation ends with its complete embedded stream node. */
typedef struct MdlObj {
    s32 sourceAddress; /* 0x00: frame data passed to sdfStreamOpen */
    SdfMemBlock *backingAllocation; /* 0x04: released by mdlObjDestroy */
    u8 inUse; /* 0x08: claimed by a model resource item */
    u8 initialized; /* 0x09: stream-node initialization guard */
    u8 pad0A[6];
    s32 sourceSize; /* 0x10 */
    u8 pad14[0x0C];
    SdfStreamFrameNode soundNode; /* 0x20: complete 0x8C embedded node */
} MdlObj;

typedef char MdlObj_size_must_be_0xAC[(sizeof(MdlObj) == 0xAC) ? 1 : -1];
typedef char MdlObj_backingAllocation_offset_must_be_4[
    ((u32)&((MdlObj *)0)->backingAllocation == 4) ? 1 : -1];
typedef char MdlObj_inUse_offset_must_be_8[
    ((u32)&((MdlObj *)0)->inUse == 8) ? 1 : -1];
typedef char MdlObj_initialized_offset_must_be_9[
    ((u32)&((MdlObj *)0)->initialized == 9) ? 1 : -1];
typedef char MdlObj_sourceSize_offset_must_be_0x10[
    ((u32)&((MdlObj *)0)->sourceSize == 0x10) ? 1 : -1];
typedef char MdlObj_soundNode_offset_must_be_0x20[
    ((u32)&((MdlObj *)0)->soundNode == 0x20) ? 1 : -1];

#endif /* MDL_OBJECT_STREAM_H */
