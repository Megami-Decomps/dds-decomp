#include "common.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "kwln.h"
#include "sdf.h"
#include "evt_picture.h"
extern u8 evtPictureQuadBounds[];
extern u8 evtPictureQuadTexcoords[];
extern u8 evtPictureQuadColor[];
extern SdfPoolNode D_003805A8;
#include "sdf_packet_alloc.h"
extern void itfSendTablePacket(SdfListHead *packet, s32 index, s32 flag);
extern void itfQueueTextureBoundQuadPacket(void *vertexData, void *uvData, void *colorData, s32 tail, SdfTex *texture, s32 flag, SdfListHead *command);

void evtSubmitPictureDrawPacket(struct SdfTex *texture) {
    SdfListHead *packet;

    packet = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(packet);
    itfSendTablePacket(packet, 0, 0);
    itfQueueTextureBoundQuadPacket(evtPictureQuadBounds, evtPictureQuadTexcoords, evtPictureQuadColor, 0xFFF, texture, 0, packet);
    D_003805A8.append(&D_003805A8, packet);
}

/* Run the picture's own update step while its active flag is set. */
s32 evtUpdatePictureWhenFlagged(KwlnTask *task) {
    EvtPictureWork *picture;

    picture = (EvtPictureWork *)dds3GetProcessData(task);
    if (picture->drawEnabled & EVT_PICTURE_FLAG_DRAW_ENABLED) {
        if (picture->texture != 0) {
            evtSubmitPictureDrawPacket(picture->texture);
        }
        return 0;
    }
    return 0;
}

/* Drop the picture's texture, then free the task data. */
void evtPictureReleaseTaskTextureAndState(KwlnTask *task) {
    EvtPictureWork *picture;

    picture = (EvtPictureWork *)dds3GetProcessData(task);
    if (picture->texture != 0) {
        sdfTexReleaseReferenceViaHandler(picture->texture);
        picture->texture = 0;
    }
    sdfChipFreeDirect(picture);
}
