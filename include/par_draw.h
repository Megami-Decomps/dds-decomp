#ifndef PAR_DRAW_H
#define PAR_DRAW_H

#include "eff_track_poly_list.h"
#include "par_kind_api.h"

void dds3Particle_BlurDraw(struct dds3Particle_BlurBuff_t *table);
void dds3TrackPolyDraw(struct ParSystem *system);

/* Both particle render paths submit the resource selected by this shared kind. */
static __inline__ void parSubmitKindDrawing(ParKindState *kind) {
    switch (kind->kind) {
    case PAR_KIND_STATE_HISTORY:
        dds3Particle_BlurDraw(kind->payload.history.table);
        break;
    case PAR_KIND_STATE_BILLBOARD_CROSS_STRIP:
        dds3TrackPolyDraw(kind->payload.crossStrip.drawSystem);
        break;
    case PAR_KIND_STATE_BILLBOARD_CROSS_TRIANGLE:
        dds3TrackPolyDraw(kind->payload.crossTriangle.drawSystem);
        break;
    case PAR_KIND_STATE_TRACK_POLYGON_CROSS_AXES:
        Eff_ModelTrackPolyListDrawOnly(kind->payload.trackPolygon.modelList);
        break;
    }
}

#endif
