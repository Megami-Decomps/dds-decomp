#ifndef DDS3_BASIC_ARRAY_VISIT_H
#define DDS3_BASIC_ARRAY_VISIT_H

#include "eff_transform.h"

/* A zero callback result stops traversal; any nonzero result continues. */
typedef s32 Dds3BasicArrayVisitResult;

enum {
    DDS3_BASIC_ARRAY_VISIT_STOP = 0,
    DDS3_BASIC_ARRAY_VISIT_CONTINUE = 1
};

typedef Dds3BasicArrayVisitResult (*Dds3BasicArrayVisitCallback)(dds3Basic_t *node);

/* Visits in order; exact zero stops, while NULL callbacks/handles and empty arrays complete. */
Dds3BasicArrayVisitResult dds3ForEachBasicArray(
    dds3BasicArrayHandle_t *object, Dds3BasicArrayVisitCallback callback);

#endif /* DDS3_BASIC_ARRAY_VISIT_H */
