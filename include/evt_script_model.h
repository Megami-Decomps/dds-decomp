#ifndef EVT_SCRIPT_MODEL_H
#define EVT_SCRIPT_MODEL_H

#include "common.h"

struct dds3Basic_t;
struct MotionTable;
struct sdfModelDef_t;

/* Kind-10 script/model work. The operation-table allocator clears 0x1C bytes;
 * the model and motion constructors establish the first four pointer fields. */
typedef struct EvtScriptModelWork {
    struct dds3Basic_t *fallbackModelObject;
    struct sdfModelDef_t *itemList;
    struct MotionTable *motionTable;
    void *loadedModelResource;
    u8 opaqueTail10[0x0C];
} EvtScriptModelWork;

typedef char EvtScriptModelWork_size_must_be_0x1C[
    (sizeof(EvtScriptModelWork) == 0x1C) ? 1 : -1];

struct dds3Basic_t *dds3AddModelBasic(s32 key, struct sdfModelDef_t *itemList,
    struct MotionTable *motionTable, struct dds3Basic_t *fallbackModelObject,
    const char *name);
struct dds3Basic_t *dds3AddModelBasic2(s32 key,
    struct sdfModelDef_t *itemList, struct MotionTable *motionTable,
    void *loadedModelResource, const char *name);

#endif /* EVT_SCRIPT_MODEL_H */
