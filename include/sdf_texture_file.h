#ifndef SDF_TEXTURE_FILE_H
#define SDF_TEXTURE_FILE_H

#include "common.h"

/* Serialized texture-resource header; payload begins after this 0x40-byte prefix. */
typedef struct SdfTextureFileHeader {
    u8 unk00;
    u8 flags;          /* 0x01: high nibble contributes to the variable payload offset. */
    u8 pad02[0xE];     /* 0x02 */
    u8 unk10;          /* 0x10 */
    u8 unk11;          /* 0x11 */
    s16 width;         /* 0x12 */
    s16 height;        /* 0x14 */
    u8 pixelFormat;    /* 0x16 */
    u8 clutFormat;     /* 0x17 */
    u16 lodParameters; /* 0x18 */
    u8 unk1A;          /* 0x1A */
    u8 clampMode;      /* 0x1B */
    s32 resourceKey;   /* 0x1C */
    s32 unk20;         /* 0x20 */
    u8 pad24[0x1C];    /* 0x24 */
} SdfTextureFileHeader;

typedef char SdfTextureFileHeader_size_must_be_0x40[
    (sizeof(SdfTextureFileHeader) == 0x40) ? 1 : -1];
#define SDF_TEXTURE_FILE_HEADER_OFFSET(field) \
    ((u32)&(((SdfTextureFileHeader *)0)->field))
typedef char SdfTextureFileHeader_flags_offset_must_be_0x01[
    (SDF_TEXTURE_FILE_HEADER_OFFSET(flags) == 0x01) ? 1 : -1];
typedef char SdfTextureFileHeader_unk10_offset_must_be_0x10[
    (SDF_TEXTURE_FILE_HEADER_OFFSET(unk10) == 0x10) ? 1 : -1];
typedef char SdfTextureFileHeader_unk11_offset_must_be_0x11[
    (SDF_TEXTURE_FILE_HEADER_OFFSET(unk11) == 0x11) ? 1 : -1];
typedef char SdfTextureFileHeader_width_offset_must_be_0x12[
    (SDF_TEXTURE_FILE_HEADER_OFFSET(width) == 0x12) ? 1 : -1];
typedef char SdfTextureFileHeader_height_offset_must_be_0x14[
    (SDF_TEXTURE_FILE_HEADER_OFFSET(height) == 0x14) ? 1 : -1];
typedef char SdfTextureFileHeader_pixel_format_offset_must_be_0x16[
    (SDF_TEXTURE_FILE_HEADER_OFFSET(pixelFormat) == 0x16) ? 1 : -1];
typedef char SdfTextureFileHeader_clut_format_offset_must_be_0x17[
    (SDF_TEXTURE_FILE_HEADER_OFFSET(clutFormat) == 0x17) ? 1 : -1];
typedef char SdfTextureFileHeader_lod_parameters_offset_must_be_0x18[
    (SDF_TEXTURE_FILE_HEADER_OFFSET(lodParameters) == 0x18) ? 1 : -1];
typedef char SdfTextureFileHeader_unk1A_offset_must_be_0x1A[
    (SDF_TEXTURE_FILE_HEADER_OFFSET(unk1A) == 0x1A) ? 1 : -1];
typedef char SdfTextureFileHeader_clamp_mode_offset_must_be_0x1B[
    (SDF_TEXTURE_FILE_HEADER_OFFSET(clampMode) == 0x1B) ? 1 : -1];
typedef char SdfTextureFileHeader_resource_key_offset_must_be_0x1C[
    (SDF_TEXTURE_FILE_HEADER_OFFSET(resourceKey) == 0x1C) ? 1 : -1];
typedef char SdfTextureFileHeader_unk20_offset_must_be_0x20[
    (SDF_TEXTURE_FILE_HEADER_OFFSET(unk20) == 0x20) ? 1 : -1];
#undef SDF_TEXTURE_FILE_HEADER_OFFSET

#endif /* SDF_TEXTURE_FILE_H */
