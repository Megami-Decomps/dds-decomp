#ifndef SDF_TEXTURE_DRAW_PACKET_H
#define SDF_TEXTURE_DRAW_PACKET_H

#include "sdf.h"

/* Variable-sized packet header initialized by sdfConsInitPacketHeader. */
typedef struct SdfDrawPacket {
    u16 quadwords;
    u8 pad02[6];
    u32 pad08;
    u32 command;
    u64 gifTag;
    u64 payloadHeader;
} SdfDrawPacket;

/* Fixed-size texture packet; constructor allocation and writes cover 0x50. */
typedef struct SdfTextureDrawPacket {
    SdfDrawPacket header;
    u64 textureWordA;
    u64 registerAddressA;
    u64 textureWordB;
    u64 registerAddressB;
    u64 textureWordC;
    u64 registerAddressC;
} SdfTextureDrawPacket;

typedef char SdfDrawPacket_size_must_be_0x20[
    (sizeof(SdfDrawPacket) == 0x20) ? 1 : -1];
typedef char SdfDrawPacket_quadwords_at_0[
    ((u32)&((SdfDrawPacket *)0)->quadwords == 0) ? 1 : -1];
typedef char SdfDrawPacket_pad08_at_8[
    ((u32)&((SdfDrawPacket *)0)->pad08 == 8) ? 1 : -1];
typedef char SdfDrawPacket_command_at_C[
    ((u32)&((SdfDrawPacket *)0)->command == 0xC) ? 1 : -1];
typedef char SdfDrawPacket_gifTag_at_10[
    ((u32)&((SdfDrawPacket *)0)->gifTag == 0x10) ? 1 : -1];
typedef char SdfDrawPacket_payloadHeader_at_18[
    ((u32)&((SdfDrawPacket *)0)->payloadHeader == 0x18) ? 1 : -1];
typedef char SdfTextureDrawPacket_size_must_be_0x50[
    (sizeof(SdfTextureDrawPacket) == 0x50) ? 1 : -1];
typedef char SdfTextureDrawPacket_header_at_0[
    ((u32)&((SdfTextureDrawPacket *)0)->header == 0) ? 1 : -1];
typedef char SdfTextureDrawPacket_textureWordA_at_20[
    ((u32)&((SdfTextureDrawPacket *)0)->textureWordA == 0x20) ? 1 : -1];
typedef char SdfTextureDrawPacket_registerAddressA_at_28[
    ((u32)&((SdfTextureDrawPacket *)0)->registerAddressA == 0x28) ? 1 : -1];
typedef char SdfTextureDrawPacket_textureWordB_at_30[
    ((u32)&((SdfTextureDrawPacket *)0)->textureWordB == 0x30) ? 1 : -1];
typedef char SdfTextureDrawPacket_registerAddressB_at_38[
    ((u32)&((SdfTextureDrawPacket *)0)->registerAddressB == 0x38) ? 1 : -1];
typedef char SdfTextureDrawPacket_textureWordC_at_40[
    ((u32)&((SdfTextureDrawPacket *)0)->textureWordC == 0x40) ? 1 : -1];
typedef char SdfTextureDrawPacket_registerAddressC_at_48[
    ((u32)&((SdfTextureDrawPacket *)0)->registerAddressC == 0x48) ? 1 : -1];

u32 sdfConsGetTextureDrawPacketSize(SdfTex *texture);
SdfTextureDrawPacket *sdfConsInitTextureDrawPacket(SdfTextureDrawPacket *drawPacket,
                                                   SdfTex *texture, s32 contextOffset);
s32 sdfConsCreateDrawPacket(SdfListHead *packetList, SdfTex *texture, s32 contextOffset);

#endif
