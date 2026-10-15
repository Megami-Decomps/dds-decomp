#ifndef PAR_DRAW_H
#define PAR_DRAW_H

#include "eff_track_poly_list.h"
#include "par_kind_api.h"

void parDrawHistorySlots(struct ParTable *table);
void parPrependCellNode(struct ParSystem *system);

/* Both particle render paths submit the resource selected by this shared kind. */
static __inline__ void parSubmitKindDrawing(ParKindState *kind) {
    switch (kind->kind) {
    case PAR_KIND_STATE_HISTORY:
        parDrawHistorySlots(kind->payload.history.table);
        break;
    case PAR_KIND_STATE_BILLBOARD_CROSS_STRIP:
        parPrependCellNode(kind->payload.crossStrip.drawSystem);
        break;
    case PAR_KIND_STATE_BILLBOARD_CROSS_TRIANGLE:
        parPrependCellNode(kind->payload.crossTriangle.drawSystem);
        break;
    case PAR_KIND_STATE_TRACK_POLYGON_CROSS_AXES:
        effTrackPolyDrawModelWorkList(kind->payload.trackPolygon.modelList);
        break;
    }
}

#endif
