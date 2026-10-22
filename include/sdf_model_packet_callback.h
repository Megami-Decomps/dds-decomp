#ifndef SDF_MODEL_PACKET_CALLBACK_H
#define SDF_MODEL_PACKET_CALLBACK_H

#include "sdf_draw.h"

struct SdfPoolNode;

void sdfModelTempPacketLinkOt(struct SdfPoolNode **surfaces, sdfModelRootNode_t *model);
void sdfModelBuildNodeResourcePackets(struct SdfPoolNode **surfaces, sdfModelRootNode_t *model);

static inline void sdfBuildModelPacketsVia(
        void (*build)(struct SdfPoolNode **, sdfModelRootNode_t *),
        struct SdfPoolNode **surfaces, sdfModelRootNode_t *model) {
    build(surfaces, model);
}

#endif
