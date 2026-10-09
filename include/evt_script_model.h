#ifndef EVT_SCRIPT_MODEL_H
#define EVT_SCRIPT_MODEL_H

#include "common.h"

struct EffWorldNode;
struct MotionTable;

/* Kind-10 script/model work. The operation-table allocator clears 0x1C bytes;
 * the model and motion constructors establish the first four pointer fields. */
typedef struct EvtScriptModelWork {
    struct EffWorldNode *fallbackModelObject;
    void *itemList;
    struct MotionTable *motionTable;
    void *loadedModelResource;
    u8 opaqueTail10[0x0C];
} EvtScriptModelWork;

typedef char EvtScriptModelWork_size_must_be_0x1C[
    (sizeof(EvtScriptModelWork) == 0x1C) ? 1 : -1];

#endif /* EVT_SCRIPT_MODEL_H */
