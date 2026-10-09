#ifndef EVT_SCRIPT_MODEL_H
#define EVT_SCRIPT_MODEL_H

#include "common.h"

struct EffWorldNode;
struct MotionTable;
struct SdfItemListRef;

/* Kind-10 script/model work. The operation-table allocator clears 0x1C bytes;
 * the model and motion constructors establish the first four pointer fields. */
typedef struct EvtScriptModelWork {
    struct EffWorldNode *fallbackModelObject;
    struct SdfItemListRef *itemList;
    struct MotionTable *motionTable;
    void *loadedModelResource;
    u8 opaqueTail10[0x0C];
} EvtScriptModelWork;

typedef char EvtScriptModelWork_size_must_be_0x1C[
    (sizeof(EvtScriptModelWork) == 0x1C) ? 1 : -1];

struct EffWorldNode *evtCreateScriptObject(s32 key, struct SdfItemListRef *itemList,
    struct MotionTable *motionTable, struct EffWorldNode *fallbackModelObject,
    const char *name);
struct EffWorldNode *evtCreateScriptObjectWithResource(s32 key,
    struct SdfItemListRef *itemList, struct MotionTable *motionTable,
    void *loadedModelResource, const char *name);

#endif /* EVT_SCRIPT_MODEL_H */
