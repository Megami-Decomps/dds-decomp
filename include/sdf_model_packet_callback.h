#ifndef SDF_MODEL_PACKET_CALLBACK_H
#define SDF_MODEL_PACKET_CALLBACK_H

#include "sdf_draw.h"

struct sdfGrOt_t;

void sdfModelTempPacketLinkOt(struct sdfGrOt_t **surfaces, sdfModelRootNode_t *model);
void sdfModelLinkOt(struct sdfGrOt_t **surfaces, sdfModelRootNode_t *model);

static inline void sdfBuildModelPacketsVia(
        void (*build)(struct sdfGrOt_t **, sdfModelRootNode_t *),
        struct sdfGrOt_t **surfaces, sdfModelRootNode_t *model) {
    build(surfaces, model);
}

#endif
