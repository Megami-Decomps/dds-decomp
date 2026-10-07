#include "common.h"
#include "eff_transform.h"

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
u8 effObjTestNodeFlags(ObjectTransform *node, u32 flags);
void effObjClearNodeFlags(ObjectTransform *node, u32 flags);
void effObjInnerVecBackup(ObjectTransform *node);

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
    ObjectTransform *eventNode;

    eventNode = eventObject->node;
    if (effObjTestNodeFlags(eventNode, DDS3_EVENT_VECTOR_BACKUP_FLAG_SELECTOR) == 1) {
        effObjClearNodeFlags(eventNode, DDS3_EVENT_VECTOR_BACKUP_FLAG_SELECTOR);
        effObjInnerVecBackup(eventNode);
    }
    return 1;
}
