#ifndef SDF_PACKET_LIST_H
#define SDF_PACKET_LIST_H

#include "common.h"

struct sdfDrawTag_t;
struct sdfGrRefPacketHeader_t;
struct sdfGrOt_t;

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
                         struct sdfGrRefPacketHeader_t *node);
void sdfGrLinkDrawFrameTex(struct sdfDrawTag_t *list, u32 source,
                           struct sdfGrRefPacketHeader_t *node);
void sdfGrJoinDrawTag(struct sdfDrawTag_t *previous, struct sdfDrawTag_t *incoming);

void sdfGrOtNormalReg(struct sdfGrOt_t *pool, struct sdfDrawTag_t *item);
void sdfGrOtFuncLinkToHead(struct sdfGrOt_t *pool, struct sdfDrawTag_t *item);
s32 sdfGrOtNormalCallback(struct sdfGrOt_t *pool, s32 mode, struct sdfDrawTag_t *item);
struct sdfDrawTag_t *sdfFlushPoolNodes(struct sdfGrOt_t *node);

#endif /* SDF_PACKET_LIST_H */
