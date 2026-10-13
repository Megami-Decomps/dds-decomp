#ifndef EFF_CHANNEL_LAYOUT_H
#define EFF_CHANNEL_LAYOUT_H

#include "eff_channel.h"
#include "eff_param.h"

struct SdfMemBlock;

/* effCopyVertRows writes XYZ at a four-float stride, leaves +0x40..+0x57
 * untouched, then copies four scalars at +0x58. */
typedef struct EffFloatRows {
    f32 primary[16];
    u8 opaque40[0x18];
    f32 secondary[4];
} EffFloatRows;

/* Allocated interpolation cursor; effCreateChannel requests exactly 0x18 bytes. */
typedef struct EffChan {
    struct SdfMemBlock *allocation;
    u32 recordCount;
    EffFloatRows *rows;
    u32 cursorIndex;
    f32 cursorPosition;
    f32 cursorStep;
} EffChan;

/* XYZ interpolation result; the cubic channel sampler also writes W. */
typedef struct EffVert {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} EffVert;

/* Emitter view: the first eight bytes are not used by this consumer. */
typedef struct EffEmit {
    u8 opaque00[8];
    EffChanWork *primitive;
} EffEmit;

/* Parameter template owner; the four-byte prefix is not read by channel work. */
typedef struct EffChanSourceOwner {
    u8 opaque00[4];
    EffParamWork *param;
} EffChanSourceOwner;

/* Source header followed by the owner used to duplicate each record parameter. */
typedef struct EffChanSource {
    EffChanHead head;
    EffChanSourceOwner *owner;
} EffChanSource;

typedef char EffFloatRows_secondary_offset_must_be_0x58[
    ((u32)&((EffFloatRows *)0)->secondary == 0x58) ? 1 : -1];
typedef char EffChan_size_must_be_0x18[(sizeof(EffChan) == 0x18) ? 1 : -1];
typedef char EffChan_recordCount_offset_must_be_4[
    ((u32)&((EffChan *)0)->recordCount == 4) ? 1 : -1];
typedef char EffChan_rows_offset_must_be_8[
    ((u32)&((EffChan *)0)->rows == 8) ? 1 : -1];
typedef char EffChan_cursorIndex_offset_must_be_0xC[
    ((u32)&((EffChan *)0)->cursorIndex == 0xC) ? 1 : -1];
typedef char EffChan_cursorPosition_offset_must_be_0x10[
    ((u32)&((EffChan *)0)->cursorPosition == 0x10) ? 1 : -1];
typedef char EffChan_cursorStep_offset_must_be_0x14[
    ((u32)&((EffChan *)0)->cursorStep == 0x14) ? 1 : -1];
typedef char EffVert_size_must_be_0x10[(sizeof(EffVert) == 0x10) ? 1 : -1];
typedef char EffEmit_primitive_offset_must_be_8[
    ((u32)&((EffEmit *)0)->primitive == 8) ? 1 : -1];
typedef char EffChanSourceOwner_param_offset_must_be_4[
    ((u32)&((EffChanSourceOwner *)0)->param == 4) ? 1 : -1];
typedef char EffChanSource_owner_offset_must_be_0x168[
    ((u32)&((EffChanSource *)0)->owner == 0x168) ? 1 : -1];

#endif
