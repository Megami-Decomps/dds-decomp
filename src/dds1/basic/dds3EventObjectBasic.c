#include "common.h"

typedef struct {
    s32 handle;
} EventData;

typedef struct {
    u8 pad[0x18];
    EventData *eventData;
    void *node;
} EventObj;

void effObjFreeInner(void *arg);
void func_00111840(s32 arg);
void func_002CFF98(void *arg);
s32 effObjTestNodeFlags(void *arg, s32 arg1);
void effObjClearNodeFlags(void *arg, s32 arg1);
void effObjInnerVecBackup(void *arg);

/* Release the event-data handle and its allocation after the inner object. */
void dds3ReleaseEventData(EventObj *event) {
    EventData *data;

    effObjFreeInner(event);
    data = event->eventData;
    func_00111840(data->handle);
    func_002CFF98(data);
}

/* Snapshot the node's vectors once its pending flag is observed. */
s32 func_00116188(EventObj *event) {
    void *node;

    node = event->node;
    if (effObjTestNodeFlags(node, 1) == 1) {
        effObjClearNodeFlags(node, 1);
        effObjInnerVecBackup(node);
    }
    return 1;
}
