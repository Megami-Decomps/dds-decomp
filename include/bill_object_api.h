#ifndef BILL_OBJECT_API_H
#define BILL_OBJECT_API_H

#include "common.h"

struct billHandle_t;
struct BillData;
struct BillOut;
struct BillChildPayload;
struct BillSnapshot;
struct sdfTexHandle_t;
struct SdfTextureFileHeader;
struct BlurPacketQuad;

void dds3BillboardDelete(struct billHandle_t *obj);
struct sdfTexHandle_t *dds3BillboardGetGeneralTex(s32 index);
struct billHandle_t *effCreateBillboardSharingIndexedResource(s32 index);
struct BlurPacketQuad *effPkt2DGetPrimPtr(void *packet);

/* The selected kind interprets the opaque payload word. */
struct billHandle_t *dds3BillboardCreate(s32 kind, u32 data);
struct billHandle_t *dds3BillboardCopy(const struct billHandle_t *source);
void dds3Billboard_BasicInit(struct billHandle_t *billboard);
struct BillChildPayload *BillboardLoadTex(
    struct SdfTextureFileHeader *resource);
void dds3BillboardSetAnimColorAdvMode(struct billHandle_t *billboard);
void dds3BillboardSetRGBA(struct billHandle_t *billboard, u32 parameter);
void dds3BillboardSetDrawType(struct billHandle_t *billboard, s32 variantValue);
void dds3BillboardSetXYOffset(struct billHandle_t *billboard, f32 width, f32 height);
void dds3BillboardSetScale(struct billHandle_t *billboard, f32 x, f32 y);
/* Radian rotation of the billboard quad in its plane. */
void dds3BillboardSetRotate(struct billHandle_t *billboard, f32 angle);
void dds3BillboardSetMode(struct billHandle_t *billboard, s32 mode);
void dds3Billboard_P2ASetNumber(struct billHandle_t *billboard, s32 entryIndex);
void dds3BillboardSetAnimKey(struct billHandle_t *billboard, u32 frame);
void dds3BillboardSetAnimKey2(struct billHandle_t *billboard, u32 frame);
s32 dds3BillboardGetAnimFrame(const struct billHandle_t *billboard);
u16 dds3BillboardGetAnimState(const struct billHandle_t *billboard);
void billReleaseSharedEntryBlock(struct BillData *data);
void effReleaseSharedTextureRecord(struct BillChildPayload *child);
struct BillChildPayload *dds3Billboard_P2AAnimDraw(
    struct billHandle_t *billboard, struct BillOut *entry);
void billCopyCurrentRecordToSnapshot(
    struct billHandle_t *billboard, struct BillSnapshot *snapshot);

#endif
