#ifndef BILL_OBJECT_API_H
#define BILL_OBJECT_API_H

#include "common.h"

struct BillObj;
struct BillData;
struct BillOut;
struct BillChildPayload;
struct SdfTex;

struct SdfTex *effGetBillResourceTexture(s32 index);

/* The selected kind interprets the opaque payload word. */
struct BillObj *billCreateIndexed(s32 kind, u32 data);
struct BillObj *billCloneObjectRetainingSharedData(struct BillObj *source);
struct BillChildPayload *billCreateChildPayloadFromTextureResource(void *resource);
void billMarkKindOneFlag(struct BillObj *billboard);
void billSetChildHalfExtents(struct BillObj *billboard, f32 width, f32 height);
void billSetChildScaleComponents(struct BillObj *billboard, f32 x, f32 y);
void billSetAnimationEntry(struct BillObj *billboard, s32 entryIndex);
void billReleaseSharedEntryBlock(struct BillData *data);
void effReleaseSharedTextureRecord(struct BillChildPayload *child);
struct BillChildPayload *billStepAnimationEntryAndUpdateChild(
    struct BillObj *billboard, struct BillOut *entry);

#endif
