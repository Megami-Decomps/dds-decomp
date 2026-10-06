#include "common.h"
#include "sdf.h"

/* Picture task data: active flags and the attached texture. */
typedef struct {
    u32 flags;
    void *texture;
} Picture;

extern void *kwlnTaskGetUserValue(void);
extern void sdfTexReleaseReferenceViaHandler(void *);
extern void sdfReleaseChipBlock(void *);

extern u8 D_003C9498[];
extern u8 D_003C94A8[];
extern u8 D_003C94B8[];
extern SdfPoolNode D_003805A8;
extern s32 sdfAllocPacketAligned(s32 size);
extern void sdfInitPacketList(SdfListHead *packet);
extern void itfSendTablePacket(SdfListHead *packet, s32 index, s32 flag);
extern void itfQueueTextureBoundQuadPacket(void *, void *, void *, s32, SdfTex *, s32, SdfListHead *);

void evtSubmitPictureDrawPacket(void *texture) {
    SdfListHead *packet;

    packet = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(packet);
    itfSendTablePacket(packet, 0, 0);
    itfQueueTextureBoundQuadPacket(D_003C9498, D_003C94A8, D_003C94B8, 0xFFF, texture, 0, packet);
    D_003805A8.append((SdfListHead *)&D_003805A8, packet);
}

/* Run the picture's own update step while its active flag is set. */
s32 evtUpdatePictureWhenFlagged(void) {
    Picture *picture;

    picture = kwlnTaskGetUserValue();
    if (picture->flags & 1) {
        if (picture->texture != 0) {
            evtSubmitPictureDrawPacket(picture->texture);
        }
        return 0;
    }
    return 0;
}

/* Drop the picture's texture, then free the task data. */
void evtPictureReleaseTaskTextureAndState(void) {
    Picture *picture;

    picture = kwlnTaskGetUserValue();
    if (picture->texture != 0) {
        sdfTexReleaseReferenceViaHandler(picture->texture);
        picture->texture = 0;
    }
    sdfReleaseChipBlock(picture);
}
