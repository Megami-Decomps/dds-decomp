#ifndef SDF_GS_PACKET_H
#define SDF_GS_PACKET_H

#include "common.h"

/* Register selectors carried by GIF A+D writes in the two GS contexts. */
enum {
    SDF_GS_TEX0_1 = 0x06,
    SDF_GS_TEX0_2 = 0x07,
    SDF_GS_CLAMP_1 = 0x08,
    SDF_GS_CLAMP_2 = 0x09,
    SDF_GS_TEX1_1 = 0x14,
    SDF_GS_TEX1_2 = 0x15,
    SDF_GS_ALPHA_1 = 0x42,
    SDF_GS_ALPHA_2 = 0x43,
    SDF_GS_TEST_1 = 0x47,
    SDF_GS_TEST_2 = 0x48,
    SDF_GIF_REGISTER_AD = 0x0E
};

/* Appending the packet patches the DMA tag's link; the VIF and GIF words
 * retain the renderer's original commands and register-list encoding. */
typedef struct SdfGsPacketHeader {
    u64 dmaTag;
    u64 vifCommands;
    u64 gifTag;
    u64 gifRegisters;
} SdfGsPacketHeader;

typedef struct SdfGsRegisterWrite {
    u64 value;
    u64 registerId;
} SdfGsRegisterWrite;

/* Fixed packets allocated by the paired composite-quad renderers. */
typedef struct SdfGsBlendPacket {
    SdfGsPacketHeader header;
    SdfGsRegisterWrite test;
    SdfGsRegisterWrite alpha;
} SdfGsBlendPacket;

typedef struct SdfGsTexturePacket {
    SdfGsPacketHeader header;
    SdfGsRegisterWrite sampling;
    SdfGsRegisterWrite texture;
    SdfGsRegisterWrite clamp;
} SdfGsTexturePacket;

typedef char SdfGsPacketHeader_size_must_be_0x20[
    (sizeof(SdfGsPacketHeader) == 0x20) ? 1 : -1];
typedef char SdfGsRegisterWrite_size_must_be_0x10[
    (sizeof(SdfGsRegisterWrite) == 0x10) ? 1 : -1];
typedef char SdfGsBlendPacket_size_must_be_0x40[
    (sizeof(SdfGsBlendPacket) == 0x40) ? 1 : -1];
typedef char SdfGsTexturePacket_size_must_be_0x50[
    (sizeof(SdfGsTexturePacket) == 0x50) ? 1 : -1];
typedef char SdfGsBlendPacket_test_at_0x20[
    ((u32)&((SdfGsBlendPacket *)0)->test == 0x20) ? 1 : -1];
typedef char SdfGsBlendPacket_alpha_at_0x30[
    ((u32)&((SdfGsBlendPacket *)0)->alpha == 0x30) ? 1 : -1];
typedef char SdfGsTexturePacket_sampling_at_0x20[
    ((u32)&((SdfGsTexturePacket *)0)->sampling == 0x20) ? 1 : -1];
typedef char SdfGsTexturePacket_texture_at_0x30[
    ((u32)&((SdfGsTexturePacket *)0)->texture == 0x30) ? 1 : -1];
typedef char SdfGsTexturePacket_clamp_at_0x40[
    ((u32)&((SdfGsTexturePacket *)0)->clamp == 0x40) ? 1 : -1];

#endif /* SDF_GS_PACKET_H */
