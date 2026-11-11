#ifndef SDF_CORE_PACKET_CB_T_H
#define SDF_CORE_PACKET_CB_T_H

#include "common.h"
#include "sdfCorePacketPointBuff_tag.h"

struct sdfMatPacket_t;

/* Native inline-matrix view through +0xAC. The generator allocation is larger. */
typedef struct {
    f32 tmSave[4][4];      /* 0x00 */
    u16 faceCount;            /* 0x40 */
    s16 pointCount;            /* 0x42 */
    u32 attrBits;     /* 0x44 */
    u32 packetBits;         /* 0x48 */
    u32 orgAttrBits;             /* 0x4C */
    s16 drawFaceCount;         /* 0x50 */
    s16 backfaceCount; /* 0x52: rejected-triangle index count */
    struct sdfMatPacket_t *matPacketPtr;   /* 0x54 */
    u32 color;       /* 0x58: packed RGBA word */
    f32 fdSoften;           /* 0x5C */
    f32 fdBias;           /* 0x60 */
    u32 pointBuff;           /* 0x64 */
    sdfCorePacketPointBuff_tag *freePointBuff; /* 0x68: next free ring matPacketPtr */
    sdfCorePacketPointBuff_tag *extPointBuff; /* 0x6C: ring wrap matPacketPtr */
    u32 freePointCount;         /* 0x70 */
    u32 extPointCount;         /* 0x74 */
    sdfCorePacketPointBuff_tag *extPointTop; /* 0x78 */
    sdfCorePacketPointBuff_tag **extPointLink;  /* 0x7C: list tail link */
    u32 packetOpenPtr;       /* 0x80 */
    u32 packetTopPtr;            /* 0x84: aligned packet packetPtr + 0x30 */
    u32 *packetStartPtr;        /* 0x88 */
    u32 *packetPtr;           /* 0x8C */
    u8 *dataPtr;           /* 0x90 */
    u8 *facePtr;             /* 0x94 */
    u8 *vertexPtr;         /* 0x98 */
    u8 *normalPtr;           /* 0x9C */
    u8 *uvPtr;       /* 0xA0 */
    u8 *uv2Ptr; /* 0xA4 */
    u8 *vertColPtr;      /* 0xA8 */
    f32 backfaceSign;             /* 0xAC */
} sdfCorePacketCb_t;

#endif /* SDF_CORE_PACKET_CB_T_H */
