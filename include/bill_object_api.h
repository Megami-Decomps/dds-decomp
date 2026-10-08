#ifndef BILL_OBJECT_API_H
#define BILL_OBJECT_API_H

#include "common.h"

struct BillObj;

struct BillObj *billCloneObjectRetainingSharedData(struct BillObj *source);
void billMarkKindOneFlag(struct BillObj *billboard);
void billSetChildHalfExtents(struct BillObj *billboard, f32 width, f32 height);

#endif
