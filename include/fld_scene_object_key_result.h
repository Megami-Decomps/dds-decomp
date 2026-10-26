#ifndef FLD_SCENE_OBJECT_KEY_RESULT_H
#define FLD_SCENE_OBJECT_KEY_RESULT_H

#include "common.h"

typedef s32 FldSceneObjectKeyResult;

/* Name lookup returns the matching scene key or NOT_FOUND. */
#define FLD_SCENE_OBJECT_KEY_NOT_FOUND ((FldSceneObjectKeyResult)-1)

FldSceneObjectKeyResult fldCommand_SEARCH_ID(const char *name);

#endif
