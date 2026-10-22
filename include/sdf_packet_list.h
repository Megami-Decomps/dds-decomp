#ifndef SDF_PACKET_LIST_H
#define SDF_PACKET_LIST_H

#include "common.h"

struct sdfDrawTag_t;
struct SdfDmaNode;
struct SdfPoolNode;

struct sdfDrawTag_t *sdfPrimAllocDrawTag(s32 (*allocator)(s32));
struct sdfDrawTag_t *dds3FontReqAllocDrawTag(void);
void sdfDrawTagInit(struct sdfDrawTag_t *list);

void sdfDrawTagLink(struct sdfDrawTag_t *list, u32 packetAddress);
void sdfAppendPacketRange(struct sdfDrawTag_t *list, u32 packetAddress,
                          u32 rangeTailAddress);
void sdfAppendReferencePacket(struct sdfDrawTag_t *list, u32 packetAddress);
void sdfDrawTagLinkCall(struct sdfDrawTag_t *list, u32 packetAddress);
void sdfDrawTagLinkRefGif(struct sdfDrawTag_t *list, u32 packetAddress);
void sdfGrLinkDispFrameTex(struct sdfDrawTag_t *list, u32 source,
                         struct SdfDmaNode *node);
void sdfGrLinkDrawFrameTex(struct sdfDrawTag_t *list, u32 source,
                           struct SdfDmaNode *node);
void sdfGrJoinDrawTag(struct sdfDrawTag_t *previous, struct sdfDrawTag_t *incoming);

void sdfGrOtNormalReg(struct SdfPoolNode *pool, struct sdfDrawTag_t *item);
void sdfGrOtFuncLinkToHead(struct SdfPoolNode *pool, struct sdfDrawTag_t *item);
s32 sdfGrOtNormalCallback(struct SdfPoolNode *pool, s32 mode, struct sdfDrawTag_t *item);
struct sdfDrawTag_t *sdfFlushPoolNodes(struct SdfPoolNode *node);

#endif /* SDF_PACKET_LIST_H */
