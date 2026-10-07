#ifndef PAR_DRAW_H
#define PAR_DRAW_H

#include "eff.h"

#ifdef VERSION_DDS2
void func_00161A10(struct ParTable *table);
#else
void func_00159E20(struct ParTable *table);
#endif
void parPrependCellNode(struct ParSystem *system);
void effTrackPolyDrawModelWorkList(struct EffTrackPolyList *list);

/* Both particle render paths submit the resource selected by this shared kind. */
static __inline__ void parSubmitKindDrawing(ParKindState *kind) {
    switch (kind->kind) {
    case 1:
#ifdef VERSION_DDS2
        func_00161A10(kind->value.table);
#else
        func_00159E20(kind->value.table);
#endif
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
