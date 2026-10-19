#ifndef SDF_MODEL_PACKET_CALLBACK_H
#define SDF_MODEL_PACKET_CALLBACK_H

#include "sdf_draw.h"

struct SdfPoolNode;

void sdfModelBuildVisibleNodePackets(struct SdfPoolNode **surfaces, SdfModel *model);
void sdfModelBuildNodeResourcePackets(struct SdfPoolNode **surfaces, SdfModel *model);

static inline void sdfBuildModelPacketsVia(
        void (*build)(struct SdfPoolNode **, SdfModel *),
        struct SdfPoolNode **surfaces, SdfModel *model) {
    build(surfaces, model);
}

#endif
