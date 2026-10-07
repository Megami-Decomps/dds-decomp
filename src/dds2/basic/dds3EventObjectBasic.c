#include "common.h"
#include "eff_event.h"

#define DDS3_EVENT_VECTOR_BACKUP_FLAG_SELECTOR 1

void effObjFreeInner(EffWorldNode *object);
void dds3DestroyObjectBase(ObjBase *resource);
void sdfReleaseChipBlock(void *allocation);
u8 effObjTestNodeFlags(ObjectTransform *node, u32 flags);

void effObjClearNodeFlags(ObjectTransform *node, u32 flags);

void effObjInnerVecBackup(ObjectTransform *node);

/* Release the event-data handle and its allocation after the inner object. */
void dds3ReleaseEventData(EffWorldNode *eventObject) {
    DdsSlotResourceBlock *eventData;

    effObjFreeInner(eventObject);
    eventData = eventObject->data;
    dds3DestroyObjectBase(eventData->resourceState);
    sdfReleaseChipBlock(eventData);
}

/* Snapshot the node's vectors once its pending flag is observed. */
s32 dds3BackupEventNodeVectorsIfFlagged(EffWorldNode *eventObject) {
    ObjectTransform *eventNode;

    eventNode = eventObject->inner;
    if (effObjTestNodeFlags(eventNode, DDS3_EVENT_VECTOR_BACKUP_FLAG_SELECTOR) == 1) {
        effObjClearNodeFlags(eventNode, DDS3_EVENT_VECTOR_BACKUP_FLAG_SELECTOR);
        effObjInnerVecBackup(eventNode);
    }
    return 1;
}
