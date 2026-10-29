#include "common.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "kwln.h"
#include "sdf.h"
#include "evt_picture.h"
#include "itf_panel_draw.h"
extern u8 evtPictureQuadBounds[];
extern u8 evtPictureQuadTexcoords[];
extern u8 evtPictureQuadColor[];
extern sdfGrOt_t D_003805A8;
#include "sdf_packet_alloc.h"
extern void itfMiscDrawAlphaPrim(sdfDrawTag_t *packet, s32 index, s32 flag);

void evtSubmitPictureDrawPacket(const struct sdfTexHandle_t *borrowedTexture) {
    sdfDrawTag_t *packet;

    packet = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(packet);
    itfMiscDrawAlphaPrim(packet, 0, 0);
    itfMiscDrawSpritePrim((DrawVertex *)evtPictureQuadBounds, (DrawColorRec *)evtPictureQuadTexcoords, (DrawColorRec *)evtPictureQuadColor, 0xFFF, borrowedTexture, 0, packet);
    D_003805A8.append(&D_003805A8, packet);
}

/* This recurring task keeps its callback installed while drawing or idle. */
KwlnTaskUpdateResult evtUpdatePictureWhenFlagged(dds3ProcessID_t *task) {
    EvtPictureWork *picture;

    picture = (EvtPictureWork *)dds3GetProcessData(task);
    if (picture->drawEnabled & EVT_PICTURE_FLAG_DRAW_ENABLED) {
        if (picture->texture != 0) {
            evtSubmitPictureDrawPacket(picture->texture);
        }
        return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
    }
    return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
}

/* Drop the picture's texture, then free the task data. */
void evtPictureShutdown(dds3ProcessID_t *task) {
    EvtPictureWork *picture;

    picture = (EvtPictureWork *)dds3GetProcessData(task);
    if (picture->texture != 0) {
        sdfTexReleaseReferenceViaHandler(picture->texture);
        picture->texture = 0;
    }
    sdfChipFreeDirect(picture);
}
