#ifndef BILL_OBJECT_API_H
#define BILL_OBJECT_API_H

#include "common.h"

struct BillObj;
struct BillData;
struct BillOut;
struct BillChildPayload;
struct BillSnapshot;
struct SdfTex;
struct SdfTextureFileHeader;
struct BlurPacketQuad;

void dds3BillboardDelete(struct BillObj *obj);
struct SdfTex *dds3BillboardGetGeneralTex(s32 index);
struct BillObj *effCreateBillboardSharingIndexedResource(s32 index);
struct BlurPacketQuad *effPkt2DGetPrimPtr(void *packet);

/* The selected kind interprets the opaque payload word. */
struct BillObj *dds3BillboardCreate(s32 kind, u32 data);
struct BillObj *dds3BillboardCopy(const struct BillObj *source);
void dds3Billboard_BasicInit(struct BillObj *billboard);
struct BillChildPayload *BillboardLoadTex(
    struct SdfTextureFileHeader *resource);
void dds3BillboardSetAnimColorAdvMode(struct BillObj *billboard);
void dds3BillboardSetRGBA(struct BillObj *billboard, u32 parameter);
void dds3BillboardSetDrawType(struct BillObj *billboard, s32 variantValue);
void dds3BillboardSetXYOffset(struct BillObj *billboard, f32 width, f32 height);
void dds3BillboardSetScale(struct BillObj *billboard, f32 x, f32 y);
/* Radian rotation of the billboard quad in its plane. */
void dds3BillboardSetRotate(struct BillObj *billboard, f32 angle);
void dds3BillboardSetMode(struct BillObj *billboard, s32 mode);
void dds3Billboard_P2ASetNumber(struct BillObj *billboard, s32 entryIndex);
void dds3BillboardSetAnimKey(struct BillObj *billboard, u32 frame);
void dds3BillboardSetAnimKey2(struct BillObj *billboard, u32 frame);
s32 dds3BillboardGetAnimFrame(const struct BillObj *billboard);
u16 dds3BillboardGetAnimState(const struct BillObj *billboard);
void billReleaseSharedEntryBlock(struct BillData *data);
void effReleaseSharedTextureRecord(struct BillChildPayload *child);
struct BillChildPayload *dds3Billboard_P2AAnimDraw(
    struct BillObj *billboard, struct BillOut *entry);
void billCopyCurrentRecordToSnapshot(
    struct BillObj *billboard, struct BillSnapshot *snapshot);

#endif
