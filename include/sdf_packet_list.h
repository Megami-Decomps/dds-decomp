#ifndef SDF_PACKET_LIST_H
#define SDF_PACKET_LIST_H

#include "common.h"

struct SdfListHead;
struct SdfDmaNode;
struct SdfPoolNode;

struct SdfListHead *sdfPrimAllocDrawTag(s32 (*allocator)(s32));
struct SdfListHead *dds3FontReqAllocDrawTag(void);
void sdfDrawTagInit(struct SdfListHead *list);

void sdfDrawTagLink(struct SdfListHead *list, u32 packetAddress);
void sdfAppendPacketRange(struct SdfListHead *list, u32 packetAddress,
                          u32 rangeTailAddress);
void sdfAppendReferencePacket(struct SdfListHead *list, u32 packetAddress);
void sdfDrawTagLinkCall(struct SdfListHead *list, u32 packetAddress);
void sdfDrawTagLinkRefGif(struct SdfListHead *list, u32 packetAddress);
void sdfGrLinkDispFrameTex(struct SdfListHead *list, u32 source,
                         struct SdfDmaNode *node);
void sdfGrLinkDrawFrameTex(struct SdfListHead *list, u32 source,
                           struct SdfDmaNode *node);
void sdfGrJoinDrawTag(struct SdfListHead *previous, struct SdfListHead *incoming);

void sdfGrOtNormalReg(struct SdfPoolNode *pool, struct SdfListHead *item);
void sdfGrOtFuncLinkToHead(struct SdfPoolNode *pool, struct SdfListHead *item);
s32 sdfGrOtNormalCallback(struct SdfPoolNode *pool, s32 mode, struct SdfListHead *item);
struct SdfListHead *sdfFlushPoolNodes(struct SdfPoolNode *node);

#endif /* SDF_PACKET_LIST_H */
