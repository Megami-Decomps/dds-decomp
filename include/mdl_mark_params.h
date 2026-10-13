#ifndef MDL_MARK_PARAMS_H
#define MDL_MARK_PARAMS_H

#include "common.h"

/* Marker-record fields edited by the model viewer; the leading word stays opaque. */
typedef struct EffMarkParams {
    u8 opaque00[8];
    s32 mark0;
    s32 mark1;
    u16 start;
    u16 end;
    u16 interval;
    u8 face;
    u8 blend;
    u32 colors[4];
} EffMarkParams;

typedef char EffMarkParams_size_must_be_0x28[
    (sizeof(EffMarkParams) == 0x28) ? 1 : -1];
typedef char EffMarkParams_mark0_offset_must_be_0x08[
    ((u32)&((EffMarkParams *)0)->mark0 == 0x08) ? 1 : -1];
typedef char EffMarkParams_start_offset_must_be_0x10[
    ((u32)&((EffMarkParams *)0)->start == 0x10) ? 1 : -1];
typedef char EffMarkParams_face_offset_must_be_0x16[
    ((u32)&((EffMarkParams *)0)->face == 0x16) ? 1 : -1];
typedef char EffMarkParams_colors_offset_must_be_0x18[
    ((u32)&((EffMarkParams *)0)->colors == 0x18) ? 1 : -1];

#endif /* MDL_MARK_PARAMS_H */
