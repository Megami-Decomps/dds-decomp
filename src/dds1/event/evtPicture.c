#include "common.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "kwln.h"
#include "sdf.h"
#include "evt_picture.h"
extern u8 evtPictureQuadBounds[];
extern u8 evtPictureQuadTexcoords[];
extern u8 evtPictureQuadColor[];
extern SdfPoolNode D_003255A8;
#include "sdf_packet_alloc.h"
extern void itfMiscDrawAlphaPrim(sdfDrawTag_t *packet, s32 index, s32 flag);
extern void itfMiscDrawSpritePrim(void *vertexData, void *uvData, void *colorData, s32 tail, sdfTexHandle_t *texture, s32 flag, sdfDrawTag_t *command);

void evtSubmitPictureDrawPacket(struct sdfTexHandle_t *texture) {
    sdfDrawTag_t *packet;

    packet = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(packet);
    itfMiscDrawAlphaPrim(packet, 0, 0);
    itfMiscDrawSpritePrim(evtPictureQuadBounds, evtPictureQuadTexcoords, evtPictureQuadColor, 0xFFF, texture, 0, packet);
    D_003255A8.append(&D_003255A8, packet);
}

/* Run the picture's own update step while its active flag is set. */
s32 evtUpdatePictureWhenFlagged(dds3ProcessID_t *task) {
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
void evtPictureShutdown(dds3ProcessID_t *task) {
    EvtPictureWork *picture;

    picture = (EvtPictureWork *)dds3GetProcessData(task);
    if (picture->texture != 0) {
        sdfTexReleaseReferenceViaHandler(picture->texture);
        picture->texture = 0;
    }
    sdfChipFreeDirect(picture);
}
