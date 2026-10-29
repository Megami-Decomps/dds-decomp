#ifndef FLD_SCENE_SELECTION_INDEX_H
#define FLD_SCENE_SELECTION_INDEX_H

#include "common.h"

/* The stored selector indexes the scene resource table; -1 means there is no
 * current interaction hit. Consumers treat any negative selector as absent. */
typedef s32 FldSceneSelectionIndex;
enum {
    FLD_SCENE_SELECTION_INDEX_NONE = -1,
};

FldSceneSelectionIndex fldGetCurrentSceneSelectionId(void);

#endif /* FLD_SCENE_SELECTION_INDEX_H */
