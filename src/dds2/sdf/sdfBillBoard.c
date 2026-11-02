#include "common.h"
#include "ps2asm.h"
#include "billboard.h"
#include "sdf_lighting.h"

#define SDF_DMA_QWORD_SHIFT 4

#define SDF_QWORD_ALIGNMENT_MASK 0xF

#define SDF_VIF_ITOP_COMPACT_VERTEX 0x04000002

#define SDF_VIF_ITOP_WIDE_VERTEX 0x04000008

#define SDF_VIF_MSCAL_VERTICES 0x14000008

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 aSize);

s32 sdfBillBoardGetPacketSize(s32 aNum);

extern void *memcpy(void *, const void *, unsigned int);

void sdfBillBoardMakeTMPacket(u8 *aPacket, const f32 *aTM);

void sdfBillBoardMake1(u8 *aPacket, const f32 *aCenter, u32 aColor, const void *aUVList,
                                   const f32 *aXYOffsetList);

u32 *sdfBillBoardMakeN(const u128 *aCenter, const void *aColor, const void *aUVList, const void *aXYOffsetList, s32 aNum, void *(*aAllocator)(s32));

void sdfMultiBillBoardMake1(u8 *aPacket, const f32 *aCenter, u32 aColor0, u32 aColor1,
                   const void *aUVList0, const void *aUVList1, const f32 *aXYOffsetList);

u32 sdfMultiBillBoardGetPacketSize(s32 aNum);

u32 *sdfMultiBillBoardMakeN(u128 *aCenter, void *aColor, void *aUVList, void *aXYOffsetList, s32 aNum, void *(*aAllocator)(s32));

u32 sdfBillBoardVcMakeN(const u128 *aCenter, const void *aColor,
                  const void *aUVList, const void *aXYOffsetList,
                  s32 aNum, void *(*aAllocator)(s32));

void sdfMultiBillBoardVcMake1(u8 *aPacket, const f32 *aCenter, const void *aColor, const void *aUVList0,
                   const void *aUVList1, const void *aXYOffsetList);

u32 sdfMultiBillBoardVcGetPacketSize(s32 aNum);

u32 sdfMultiBillBoardVcMakeN(u128 *aCenter, void *aColor, void *aUVList, void *aXYOffsetList, s32 aNum, void *(*aAllocator)(s32));

void sdfBillBoardMakeTMPacket(u8 *aPacket, const f32 *aTM) {
    sdfBillBoardTMPacket_t *p = (sdfBillBoardTMPacket_t *)aPacket;

    p->dma_qwc = 2;
    p->vif_unpack = 0x6403C000;
    p->tm[0] = aTM[0];
    p->tm[1] = aTM[1];
    p->tm[2] = aTM[4];
    p->tm[3] = aTM[5];
    p->tm[4] = aTM[12];
    p->tm[5] = aTM[13];
    p->vif_itop = 0x04000000;
    p->vif_mscal = SDF_VIF_MSCAL_VERTICES;
    memset(p->pad, 0, sizeof(p->pad));
}

void sdfBillBoardMake1(u8 *aPacket, const f32 *aCenter, u32 aColor, const void *aUVList,
                                   const f32 *aXYOffsetList) {
    sdfBillBoard1Packet_t *p = (sdfBillBoard1Packet_t *)aPacket;

    p->dma_qwc = 7;
    p->vif_unpack1 = 0x6C01C000;
    p->count = 1;
    p->giftag_h = 0xA0000000;
    p->giftag_reg_l = 0x43434310;
    p->giftag_reg_h = 0x43;
    p->vif_unpack2 = 0x6001C001;
    p->prim = 0x155;
    p->vif_unpack3 = 0x6801C002;
    p->center[0] = aCenter[0];
    p->center[1] = aCenter[1];
    p->center[2] = aCenter[2];
    p->vif_unpack4 = 0x6E01C003;
    p->color = aColor;
    p->vif_unpack5 = 0x6504C004;
    EE_LOAD_UNALIGNED_WORD(p->uv[0], (const u8 *)aUVList);
    EE_LOAD_UNALIGNED_WORD(p->uv[1], (const u8 *)aUVList + 4);
    EE_LOAD_UNALIGNED_WORD(p->uv[2], (const u8 *)aUVList + 8);
    EE_LOAD_UNALIGNED_WORD(p->uv[3], (const u8 *)aUVList + 12);
    p->vif_unpack6 = 0x6404C008;
    p->xyOffset[0] = aXYOffsetList[0];
    p->xyOffset[1] = aXYOffsetList[1];
    p->xyOffset[2] = aXYOffsetList[2];
    p->xyOffset[3] = aXYOffsetList[3];
    p->xyOffset[4] = aXYOffsetList[4];
    p->xyOffset[5] = aXYOffsetList[5];
    p->xyOffset[6] = aXYOffsetList[6];
    p->xyOffset[7] = aXYOffsetList[7];
    p->vif_itop = SDF_VIF_ITOP_COMPACT_VERTEX;
    p->vif_mscal = SDF_VIF_MSCAL_VERTICES;
    p->pad = 0;
}

s32 sdfBillBoardGetPacketSize(s32 aNum) {
    return (aNum << 6) + 0x40;
}

/* vu0 routine: pack aligned positions into the VIF three-word stream. */
u32 *sdfBillBoardMakeN(const u128 *aCenter, const void *aColor, const void *aUVList, const void *aXYOffsetList, s32 aNum, void *(*aAllocator)(s32)) {
    s32 packetBytes;
    u32 *packet;
    u32 *cursor;
    u32 vuAddress;
    u32 unpackCode;
    s32 n;
    s32 vertexIndex;

    packetBytes = sdfBillBoardGetPacketSize(aNum);
    if (aAllocator == NULL) {
        cursor = (u32 *)sdfGrAllocPacketBuff(packetBytes);
    } else {
        cursor = aAllocator(packetBytes);
    }
    packet = cursor;
    packet[0] = (packetBytes >> SDF_DMA_QWORD_SHIFT) - 1;
    packet[1] = 0;
    packet[2] = 0x6C01C000;
    packet[3] = aNum;
    packet[4] = 0xA0000000;
    packet[5] = 0x43434310;
    packet[6] = 0x43;
    packet[7] = 0x6001C001;
    packet[8] = 0x155;
    packet[9] = (aNum << 16) | 0x6800C002;
    cursor = packet + 10;
    vertexIndex = 0;
    do {
        MMI_STORE_VEC3_VALUE(cursor, aCenter[vertexIndex]);
        cursor += 3;
        vertexIndex++;
    } while (vertexIndex != aNum);
    n = aNum;
    unpackCode = n << 16;
    vuAddress = aNum + 2;
    *cursor++ = unpackCode | vuAddress | 0x6E00C000;
    memcpy(cursor, aColor, n * 4);
    cursor += n;
    vuAddress += n;
    n = aNum * 4;
    unpackCode = n << 16;
    *cursor++ = unpackCode | vuAddress | 0x6500C000;
    vuAddress += n;
    unpackCode |= vuAddress;
    memcpy(cursor, aUVList, n * 4);
    cursor += n;
    n *= 8;
    unpackCode |= 0x6400C000;
    *cursor++ = unpackCode;
    memcpy(cursor, aXYOffsetList, n);
    cursor = (u32 *)((u8 *)cursor + n);
    cursor[0] = SDF_VIF_ITOP_COMPACT_VERTEX;
    cursor[1] = SDF_VIF_MSCAL_VERTICES;
    cursor += 2;
    while (((u32)cursor & SDF_QWORD_ALIGNMENT_MASK) != 0) {
        *cursor++ = 0;
    }
    return packet;
}

void sdfMultiBillBoardMake1(u8 *aPacket, const f32 *aCenter, u32 aColor0, u32 aColor1,
                   const void *aUVList0, const void *aUVList1, const f32 *aXYOffsetList) {
    sdfMultiBillBoard1Packet_t *p = (sdfMultiBillBoard1Packet_t *)aPacket;

    p->dma_qwc = 8;
    p->vif_unpack1 = 0x6C01C000;
    p->count = 1;
    p->giftag_h = 0xA0000000;
    p->giftag_reg_l = 0x43434310;
    p->giftag_reg_h = 0x43;
    p->vif_unpack2 = 0x6001C001;
    p->prim = 0x155;
    p->vif_unpack3 = 0x6801C002;
    p->center[0] = aCenter[0];
    p->center[1] = aCenter[1];
    p->center[2] = aCenter[2];
    p->vif_unpack4 = 0x6E02C003;
    p->color[0] = aColor0;
    p->color[1] = aColor1;
    p->vif_unpack5 = 0x6D04C005;
    EE_LOAD_UNALIGNED_WORD(p->uv[0], (const u8 *)aUVList0);
    EE_LOAD_UNALIGNED_WORD(p->uv[1], (const u8 *)aUVList1);
    EE_LOAD_UNALIGNED_WORD(p->uv[2], (const u8 *)aUVList0 + 4);
    EE_LOAD_UNALIGNED_WORD(p->uv[3], (const u8 *)aUVList1 + 4);
    EE_LOAD_UNALIGNED_WORD(p->uv[4], (const u8 *)aUVList0 + 8);
    EE_LOAD_UNALIGNED_WORD(p->uv[5], (const u8 *)aUVList1 + 8);
    EE_LOAD_UNALIGNED_WORD(p->uv[6], (const u8 *)aUVList0 + 12);
    EE_LOAD_UNALIGNED_WORD(p->uv[7], (const u8 *)aUVList1 + 12);
    p->vif_unpack6 = 0x6404C009;
    p->xyOffset[0] = aXYOffsetList[0];
    p->xyOffset[1] = aXYOffsetList[1];
    p->xyOffset[2] = aXYOffsetList[2];
    p->xyOffset[3] = aXYOffsetList[3];
    p->xyOffset[4] = aXYOffsetList[4];
    p->xyOffset[5] = aXYOffsetList[5];
    p->xyOffset[6] = aXYOffsetList[6];
    p->xyOffset[7] = aXYOffsetList[7];
    p->vif_itop = 0x04000004;
    p->vif_mscal = SDF_VIF_MSCAL_VERTICES;
}

u32 sdfMultiBillBoardGetPacketSize(s32 aNum) {
    return (aNum * 0x54 + 0x4bU) & 0xfffffff0;
}

u32 *sdfMultiBillBoardMakeN(u128 *aCenter, void *aColor, void *aUVList, void *aXYOffsetList, s32 aNum, void *(*aAllocator)(s32)) {
    s32 bytes;
    u32 *packet;
    u32 *cursor;
    u32 index;
    u32 code;
    s32 n;
    s32 i;

    bytes = sdfMultiBillBoardGetPacketSize(aNum);
    if (aAllocator == NULL) {
        cursor = (u32 *)sdfGrAllocPacketBuff(bytes);
    } else {
        cursor = aAllocator(bytes);
    }
    packet = cursor;
    packet[0] = (bytes >> 4) - 1;
    packet[1] = 0;
    packet[2] = 0x6C01C000;
    packet[3] = aNum;
    packet[4] = 0xA0000000;
    packet[5] = 0x43434310;
    packet[6] = 0x43;
    packet[7] = 0x6001C001;
    packet[8] = 0x155;
    packet[9] = (aNum << 16) | 0x6800C002;
    cursor = packet + 10;
    i = 0;
    do {
        MMI_STORE_VEC3_VALUE(cursor, aCenter[i]);
        cursor += 3;
        i++;
    } while (i != aNum);
    n = aNum * 2;
    index = aNum + 2;
    *cursor++ = (n << 16) | index | 0x6E00C000;
    memcpy(cursor, aColor, n * 4);
    cursor += n;
    index += n;
    n = aNum * 4;
    code = n << 16;
    *cursor++ = code | index | 0x6D00C000;
    memcpy(cursor, aUVList, n * 8);
    cursor += n * 2;
    index += n;
    code |= index;
    *cursor++ = code | 0x6400C000;
    memcpy(cursor, aXYOffsetList, n * 8);
    cursor += n * 2;
    cursor[0] = 0x04000004;
    cursor[1] = 0x14000008;
    cursor += 2;
    while (((u32)cursor & 0xF) != 0) {
        *cursor++ = 0;
    }
    return packet;
}

void sdfBillBoardVcMake1(u8 *aPacket, const f32 *aCenter, const u32 *aColor, const void *aUVList,
                   const f32 *aXYOffsetList) {
    sdfBillBoardVc1Packet_t *p = (sdfBillBoardVc1Packet_t *)aPacket;

    p->dma_qwc = 8;
    p->vif_unpack1 = 0x6C01C000;
    p->count = 1;
    p->giftag_h = 0xD0000000;
    p->giftag_reg_l = 0x34134130;
    p->giftag_reg_h = 0x41341;
    p->vif_unpack2 = 0x6001C001;
    p->prim = 0x155;
    p->vif_unpack3 = 0x6801C002;
    p->center[0] = aCenter[0];
    p->center[1] = aCenter[1];
    p->center[2] = aCenter[2];
    p->vif_unpack4 = 0x6E04C003;
    p->color[0] = aColor[0];
    p->color[1] = aColor[1];
    p->color[2] = aColor[2];
    p->color[3] = aColor[3];
    p->vif_unpack5 = 0x6504C007;
    EE_LOAD_UNALIGNED_WORD(p->uv[0], (const u8 *)aUVList);
    EE_LOAD_UNALIGNED_WORD(p->uv[1], (const u8 *)aUVList + 4);
    EE_LOAD_UNALIGNED_WORD(p->uv[2], (const u8 *)aUVList + 8);
    EE_LOAD_UNALIGNED_WORD(p->uv[3], (const u8 *)aUVList + 12);
    p->vif_unpack6 = 0x6404C00B;
    p->xyOffset[0] = aXYOffsetList[0];
    p->xyOffset[1] = aXYOffsetList[1];
    p->xyOffset[2] = aXYOffsetList[2];
    p->xyOffset[3] = aXYOffsetList[3];
    p->xyOffset[4] = aXYOffsetList[4];
    p->xyOffset[5] = aXYOffsetList[5];
    p->xyOffset[6] = aXYOffsetList[6];
    p->xyOffset[7] = aXYOffsetList[7];
    p->vif_itop = 0x04000006;
    p->vif_mscal = SDF_VIF_MSCAL_VERTICES;
    p->pad[0] = 0;
    p->pad[1] = 0;
}

u32 sdfBillBoardVcGetPacketSize(s32 aNum) {
    return (aNum * 0x4c + 0x4bU) & 0xfffffff0;
}

u32 sdfBillBoardVcMakeN(const u128 *aCenter, const void *aColor,
                  const void *aUVList, const void *aXYOffsetList,
                  s32 aNum, void *(*aAllocator)(s32)) {
    s32 bytes;
    u32 *packet;
    u32 *cursor;
    u32 index;
    u32 code;
    s32 n;
    s32 i;

    bytes = sdfBillBoardGetPacketSize(aNum);
    if (aAllocator == NULL) {
        cursor = (u32 *)sdfGrAllocPacketBuff(bytes);
    } else {
        cursor = aAllocator(bytes);
    }
    packet = cursor;
    packet[0] = (bytes >> 4) - 1;
    packet[1] = 0;
    packet[2] = 0x6C01C000;
    packet[3] = aNum;
    packet[4] = 0xD0000000;
    packet[5] = 0x34134130;
    packet[6] = 0x41341;
    packet[7] = 0x6001C001;
    packet[8] = 0x155;
    packet[9] = (aNum << 16) | 0x6800C002;

    cursor = packet + 10;
    i = 0;
    do {
        MMI_STORE_VEC3_VALUE(cursor, aCenter[i]);
        cursor += 3;
        i++;
    } while (i != aNum);

    code = aNum << 18;
    index = aNum + 2;
    n = aNum * 4;
    *cursor++ = code | index | 0x6E00C000;
    memcpy(cursor, aColor, n * 4);
    cursor += n;
    index += n;
    *cursor++ = code | index | 0x6500C000;
    memcpy(cursor, aUVList, n * 4);
    cursor += n;
    index += n;
    n = aNum * 8;
    *cursor++ = code | index | 0x6400C000;
    memcpy(cursor, aXYOffsetList, n * 4);
    cursor += n;
    cursor[0] = 0x04000006;
    cursor[1] = 0x14000008;
    cursor += 2;
    while (((u32)cursor & 0xF) != 0) {
        *cursor++ = 0;
    }
    return (u32)packet;
}

void sdfMultiBillBoardVcMake1(u8 *aPacket, const f32 *aCenter, const void *aColor, const void *aUVList0,
                   const void *aUVList1, const void *aXYOffsetList) {
    sdfMultiBillBoardVc1Packet_t *p = (sdfMultiBillBoardVc1Packet_t *)aPacket;

    p->dma_qwc = 10;
    p->vif_unpack1 = 0x6C01C000;
    p->count = 1;
    p->giftag_h = 0xD0000000;
    p->giftag_reg_l = 0x34134130;
    p->giftag_reg_h = 0x41341;
    p->vif_unpack2 = 0x6001C001;
    p->prim = 0x15D;
    p->vif_unpack3 = 0x6801C002;
    p->center[0] = aCenter[0];
    p->center[1] = aCenter[1];
    p->center[2] = aCenter[2];
    p->vif_unpack4 = 0x6E08C003;
    memcpy(aPacket + 56, aColor, 32);
    p->vif_unpack5 = 0x6D04C00B;
    EE_LOAD_UNALIGNED_WORD(p->uv[0], (const u8 *)aUVList0);
    EE_LOAD_UNALIGNED_WORD(p->uv[1], (const u8 *)aUVList1);
    EE_LOAD_UNALIGNED_WORD(p->uv[2], (const u8 *)aUVList0 + 4);
    EE_LOAD_UNALIGNED_WORD(p->uv[3], (const u8 *)aUVList1 + 4);
    EE_LOAD_UNALIGNED_WORD(p->uv[4], (const u8 *)aUVList0 + 8);
    EE_LOAD_UNALIGNED_WORD(p->uv[5], (const u8 *)aUVList1 + 8);
    EE_LOAD_UNALIGNED_WORD(p->uv[6], (const u8 *)aUVList0 + 12);
    EE_LOAD_UNALIGNED_WORD(p->uv[7], (const u8 *)aUVList1 + 12);
    p->vif_unpack6 = 0x6404C00F;
    memcpy(aPacket + 128, aXYOffsetList, 32);
    p->vif_itop = 0x04000008;
    p->vif_mscal = SDF_VIF_MSCAL_VERTICES;
    p->pad[0] = 0;
    p->pad[1] = 0;
}

u32 sdfMultiBillBoardVcGetPacketSize(s32 aNum) {
    return (aNum * 0x6c + 0x4bU) & 0xfffffff0;
}

/* vu0 routine: pack aligned positions into the VIF three-word stream. */
u32 sdfMultiBillBoardVcMakeN(u128 *aCenter, void *aColor, void *aUVList, void *aXYOffsetList, s32 aNum, void *(*aAllocator)(s32)) {
    s32 packetBytes;
    u32 *packet;
    u32 *cursor;
    u32 vuAddress;
    u32 unpackCode;
    s32 n;
    s32 vertexIndex;

    packetBytes = sdfMultiBillBoardVcGetPacketSize(aNum);
    if (aAllocator == NULL) {
        cursor = (u32 *)sdfGrAllocPacketBuff(packetBytes);
    } else {
        cursor = aAllocator(packetBytes);
    }
    packet = cursor;
    packet[0] = (packetBytes >> SDF_DMA_QWORD_SHIFT) - 1;
    packet[1] = 0;
    packet[2] = 0x6C01C000;
    packet[3] = aNum;
    packet[4] = 0xD0000000;
    packet[5] = 0x34134130;
    packet[6] = 0x41341;
    packet[7] = 0x6001C001;
    packet[8] = 0x15D;
    packet[9] = (aNum << 16) | 0x6800C002;
    cursor = packet + 10;
    vertexIndex = 0;
    do {
        MMI_STORE_VEC3_VALUE(cursor, aCenter[vertexIndex]);
        cursor += 3;
        vertexIndex++;
    } while (vertexIndex != aNum);
    n = aNum * 8;
    vuAddress = aNum + 2;
    *cursor++ = (n << 16) | vuAddress | 0x6E00C000;
    memcpy(cursor, aColor, n * 4);
    cursor += n;
    vuAddress += n;
    n = aNum * 4;
    unpackCode = n << 16;
    *cursor++ = unpackCode | vuAddress | 0x6D00C000;
    memcpy(cursor, aUVList, n * 8);
    cursor += n * 2;
    vuAddress += n;
    unpackCode |= vuAddress;
    *cursor++ = unpackCode | 0x6400C000;
    memcpy(cursor, aXYOffsetList, n * 8);
    cursor += n * 2;
    cursor[0] = SDF_VIF_ITOP_WIDE_VERTEX;
    cursor[1] = SDF_VIF_MSCAL_VERTICES;
    cursor += 2;
    while (((u32)cursor & SDF_QWORD_ALIGNMENT_MASK) != 0) {
        *cursor++ = 0;
    }
    return (u32)packet;
}
