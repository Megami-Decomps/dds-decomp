#ifndef BTL_CAMERA_PARAMETER_RECORD_H
#define BTL_CAMERA_PARAMETER_RECORD_H

#include "common.h"

/* Script camera-track rows are read at the native 0x80-byte bank stride. */
typedef struct BtlCameraParameterRecord {
    u8 flags00; /* 0x00: case-5 pose handling tests bits 0 and 2 */
    u8 pad01[0xF];
    f32 value[4]; /* 0x10: values forwarded to the active track helper */
    u8 flags20; /* 0x20 */
    u8 pad21[0xF];
    f32 fadeInDuration; /* 0x30 */
    u8 pad34[0xC];
    f32 fadeOutDuration; /* 0x40 */
    u8 pad44[0x18];
    f32 curveScale5C; /* 0x5C */
    f32 curveScale60; /* 0x60 */
    u8 pad64[0xC];
    s8 stop; /* 0x70: expired track records stop when nonzero */
    u8 pad71[0xF];
} BtlCameraParameterRecord;

typedef char BtlCameraParameterRecord_size[(sizeof(BtlCameraParameterRecord) == 0x80) ? 1 : -1];
typedef char BtlCameraParameterRecord_flags00_offset[((unsigned long)&((BtlCameraParameterRecord *)0)->flags00 == 0x00) ? 1 : -1];
typedef char BtlCameraParameterRecord_value_offset[((unsigned long)&((BtlCameraParameterRecord *)0)->value == 0x10) ? 1 : -1];
typedef char BtlCameraParameterRecord_flags20_offset[((unsigned long)&((BtlCameraParameterRecord *)0)->flags20 == 0x20) ? 1 : -1];
typedef char BtlCameraParameterRecord_fadeInDuration_offset[((unsigned long)&((BtlCameraParameterRecord *)0)->fadeInDuration == 0x30) ? 1 : -1];
typedef char BtlCameraParameterRecord_fadeOutDuration_offset[((unsigned long)&((BtlCameraParameterRecord *)0)->fadeOutDuration == 0x40) ? 1 : -1];
typedef char BtlCameraParameterRecord_curveScale5C_offset[((unsigned long)&((BtlCameraParameterRecord *)0)->curveScale5C == 0x5C) ? 1 : -1];
typedef char BtlCameraParameterRecord_curveScale60_offset[((unsigned long)&((BtlCameraParameterRecord *)0)->curveScale60 == 0x60) ? 1 : -1];
typedef char BtlCameraParameterRecord_stop_offset[((unsigned long)&((BtlCameraParameterRecord *)0)->stop == 0x70) ? 1 : -1];

#endif
