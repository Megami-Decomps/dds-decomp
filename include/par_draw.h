#ifndef PAR_DRAW_H
#define PAR_DRAW_H

#include "eff_track_poly_list.h"

void parDrawHistorySlots(struct ParTable *table);
void parPrependCellNode(struct ParSystem *system);

/* Both particle render paths submit the resource selected by this shared kind. */
static __inline__ void parSubmitKindDrawing(ParKindState *kind) {
    switch (kind->kind) {
    case 1:
        parDrawHistorySlots(kind->value.table);
        break;
    case 2:
        parPrependCellNode(kind->primaryDrawSystem);
        break;
    case 3:
        parPrependCellNode(kind->secondaryDraw.system);
        break;
    case 4:
        effTrackPolyDrawModelWorkList(kind->secondaryDraw.modelList);
        break;
    }
}

#endif
