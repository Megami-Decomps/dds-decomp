#ifndef DDS3_OBJECT_BASE_RESOURCE_VIEW_H
#define DDS3_OBJECT_BASE_RESOURCE_VIEW_H

#include "dds3obj.h"

struct dds3ModelHandle_t;

/* resourceState selects the pointee type: DDS3_OBJECT_RESOURCE_MODEL_CONTEXT
 * is a model context and DDS3_OBJECT_RESOURCE_DEV_MOTION is an SDF root.
 * Borrowed views expire when the base releases or rebuilds its owned resource. */
static inline struct dds3ModelHandle_t *dds3ObjectBaseBorrowModelContext(
    dds3ObjectBaseControlWorkData_t *owner) {
    return (struct dds3ModelHandle_t *)owner->resourceHandle;
}

static inline struct sdfModelRootNode_t *dds3ObjectBaseBorrowDeviceMotionRoot(
    dds3ObjectBaseControlWorkData_t *owner) {
    return (struct sdfModelRootNode_t *)owner->resourceHandle;
}

/* Store an owner-held resource; the caller sets its matching resourceState. */
static inline void dds3ObjectBaseSetModelContext(
    dds3ObjectBaseControlWorkData_t *owner, struct dds3ModelHandle_t *context) {
    owner->resourceHandle = context;
}

static inline void dds3ObjectBaseSetDeviceMotionRoot(
    dds3ObjectBaseControlWorkData_t *owner, struct sdfModelRootNode_t *root) {
    owner->resourceHandle = root;
}

#endif /* DDS3_OBJECT_BASE_RESOURCE_VIEW_H */
