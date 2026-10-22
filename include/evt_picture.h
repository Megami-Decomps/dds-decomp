#ifndef EVT_PICTURE_H
#define EVT_PICTURE_H

#include "common.h"
#include "kwln.h"
#include "evt_world.h"

enum {
    EVT_PICTURE_FLAG_DRAW_ENABLED = 1
};

extern char evtPictureTaskName[];

void evtPictureDrawOn(dds3ProcessID_t *task);
void evtPictureDrawOff(dds3ProcessID_t *task);
EvtPictureWork *evtAllocateContext(void);
void LoadTexture(EvtPictureWork *context, const char *path);
void evtSubmitPictureDrawPacket(struct sdfTexHandle_t *texture);
s32 evtUpdatePictureWhenFlagged(dds3ProcessID_t *task);
void evtPictureShutdown(dds3ProcessID_t *task);
dds3ProcessID_t *evtStartPictureProcess2(s32 taskId, const char *path);
dds3ProcessID_t *evtStartPictureProcess(s32 taskId, struct sdfTexHandle_t *texture);

#endif /* EVT_PICTURE_H */
