#include "common.h"

#define DDS3_EVENT_VECTOR_BACKUP_FLAG_SELECTOR 1

typedef struct {
    s32 handle;
} EventData;

typedef struct {
    u8 pad[0x18];
    EventData *eventData;
    void *node;
} EventObj;

void effObjFreeInner(void *arg);
void dds3DestroyObjectBase(s32 arg);
void sdfReleaseChipBlock(void *arg);
s32 effObjTestNodeFlags(void *arg, s32 arg1);
void effObjClearNodeFlags(void *arg, s32 arg1);
void effObjInnerVecBackup(void *arg);

/* Release the event-data handle and its allocation after the inner object. */
void dds3ReleaseEventData(EventObj *eventObject) {
    EventData *eventData;

    effObjFreeInner(eventObject);
    eventData = eventObject->eventData;
    dds3DestroyObjectBase(eventData->handle);
    sdfReleaseChipBlock(eventData);
}

/* Snapshot the node's vectors once its pending flag is observed. */
s32 dds3BackupEventNodeVectorsIfFlagged(EventObj *eventObject) {
    void *eventNode;

    eventNode = eventObject->node;
    if (effObjTestNodeFlags(eventNode, DDS3_EVENT_VECTOR_BACKUP_FLAG_SELECTOR) == 1) {
        effObjClearNodeFlags(eventNode, DDS3_EVENT_VECTOR_BACKUP_FLAG_SELECTOR);
        effObjInnerVecBackup(eventNode);
    }
    return 1;
}
