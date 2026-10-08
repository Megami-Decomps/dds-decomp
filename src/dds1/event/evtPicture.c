#include "common.h"
#include "sdf.h"

/* Picture task data: active flags and the attached texture. */
typedef struct {
    u32 flags;
    void *texture;
} Picture;

extern void *kwlnTaskGetUserValue(void);
extern void sdfTexReleaseReferenceViaHandler(struct SdfTex *texture);
extern void sdfReleaseChipBlock(void *);

extern u8 D_003686C8[];
extern u8 D_003686D8[];
extern u8 D_003686E8[];
extern SdfPoolNode D_003255A8;
extern s32 sdfAllocPacketAligned(s32 size);
extern void sdfInitPacketList(SdfListHead *packet);
extern void itfSendTablePacket(SdfListHead *packet, s32 index, s32 flag);
extern void itfQueueTextureBoundQuadPacket(void *, void *, void *, s32, SdfTex *, s32, SdfListHead *);

void evtSubmitPictureDrawPacket(void *texture) {
    SdfListHead *packet;

    packet = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(packet);
    itfSendTablePacket(packet, 0, 0);
    itfQueueTextureBoundQuadPacket(D_003686C8, D_003686D8, D_003686E8, 0xFFF, texture, 0, packet);
    D_003255A8.append((SdfListHead *)&D_003255A8, packet);
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
        sdfTexReleaseReferenceViaHandler((struct SdfTex *)picture->texture);
        picture->texture = 0;
    }
    sdfReleaseChipBlock(picture);
}
