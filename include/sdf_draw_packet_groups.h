#ifndef SDF_DRAW_PACKET_GROUPS_H
#define SDF_DRAW_PACKET_GROUPS_H

#include "sdf.h"
#include "sdf_gs_blend.h"
#include "sdf_dma_tag.h"

typedef struct SdfDrawPacketGroup {
    SdfListHead list;
    union {
        SdfGsBlendPacket blend;
        SdfDmaTagHeader dma;
    } packet;
} SdfDrawPacketGroup;

typedef struct SdfDrawPacketGroups {
    SdfDrawPacketGroup groups[4];
    SdfListHead syncList;
    u64 syncPacketWords[2];
} SdfDrawPacketGroups;

void sdfSubmitDrawPacketGroups(SdfPoolNode **owners, u8 *memory);

typedef char SdfDrawPacketGroup_size_must_be_0x60[
    (sizeof(SdfDrawPacketGroup) == 0x60) ? 1 : -1];
typedef char SdfDrawPacketGroup_packet_at_0x20[
    ((u32)&((SdfDrawPacketGroup *)0)->packet == 0x20) ? 1 : -1];
typedef char SdfDrawPacketGroups_syncList_at_0x180[
    ((u32)&((SdfDrawPacketGroups *)0)->syncList == 0x180) ? 1 : -1];
typedef char SdfDrawPacketGroups_syncPacketWords_at_0x1A0[
    ((u32)&((SdfDrawPacketGroups *)0)->syncPacketWords == 0x1A0) ? 1 : -1];
typedef char SdfDrawPacketGroups_size_must_be_0x1B0[
    (sizeof(SdfDrawPacketGroups) == 0x1B0) ? 1 : -1];

#endif
