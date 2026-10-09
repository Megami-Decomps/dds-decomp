#ifndef EVT_PICTURE_H
#define EVT_PICTURE_H

#include "common.h"
#include "kwln.h"
#include "evt_world.h"

enum {
    EVT_PICTURE_FLAG_DRAW_ENABLED = 1
};

extern char evtPictureTaskName[];

void evtSetContextFlag(KwlnTask *task);
void evtClearContextFlag(KwlnTask *task);
EvtPictureWork *evtAllocateContext(void);
void evtSetConvertedContextValue(EvtPictureWork *context, const char *path);
void evtSubmitPictureDrawPacket(struct SdfTex *texture);
s32 evtUpdatePictureWhenFlagged(KwlnTask *task);
void evtPictureReleaseTaskTextureAndState(KwlnTask *task);
KwlnTask *evtCreateTask(s32 taskId, const char *path);
KwlnTask *evtCreateTaskWithValue(s32 taskId, struct SdfTex *texture);

#endif /* EVT_PICTURE_H */
