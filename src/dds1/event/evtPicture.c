#include "common.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "kwln.h"
#include "sdf.h"
#include "evt_picture.h"

extern void sdfTexReleaseReferenceViaHandler(struct SdfTex *texture);

extern u8 D_003686C8[];
extern u8 D_003686D8[];
extern u8 D_003686E8[];
extern SdfPoolNode D_003255A8;
extern s32 sdfAllocPacketAligned(s32 size);
extern void itfSendTablePacket(SdfListHead *packet, s32 index, s32 flag);
extern void itfQueueTextureBoundQuadPacket(void *, void *, void *, s32, SdfTex *, s32, SdfListHead *);

void evtSubmitPictureDrawPacket(struct SdfTex *texture) {
    SdfListHead *packet;

    packet = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(packet);
    itfSendTablePacket(packet, 0, 0);
    itfQueueTextureBoundQuadPacket(D_003686C8, D_003686D8, D_003686E8, 0xFFF, texture, 0, packet);
    D_003255A8.append((SdfListHead *)&D_003255A8, packet);
}

/* Run the picture's own update step while its active flag is set. */
s32 evtUpdatePictureWhenFlagged(KwlnTask *task) {
    EvtPictureWork *picture;

    picture = (EvtPictureWork *)kwlnTaskGetUserValue(task);
    if (picture->flags & EVT_PICTURE_FLAG_DRAW_ENABLED) {
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

    picture = (EvtPictureWork *)kwlnTaskGetUserValue(task);
    if (picture->texture != 0) {
        sdfTexReleaseReferenceViaHandler(picture->texture);
        picture->texture = 0;
    }
    sdfReleaseChipBlock(picture);
}
