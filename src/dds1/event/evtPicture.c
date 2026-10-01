#include "common.h"

/* Picture task data: active flags and the attached texture. */
typedef struct {
    u32 flags;
    void *texture;
} Picture;

extern void *kwlnTaskGetUserValue(void);
extern void sdfTexReleaseReferenceViaHandler(void *);
extern void sdfReleaseChipBlock(void *);
typedef struct PicturePacketDevice {
    u8 pad00[0x10];
    void (*invoke)(void *, s32);
} PicturePacketDevice;

extern u8 D_003686C8[];
extern u8 D_003686D8[];
extern u8 D_003686E8[];
extern PicturePacketDevice D_003255A8;
extern s32 sdfAllocPacketAligned(s32 size);
extern void sdfInitPacketList(s32 packet);
extern void itfSendTablePacket(s32 packet, s32 index, s32 flag);
extern void func_00198C70(void *, void *, void *, s32, s32, s32, s32);

void func_00235150(void *texture) {
    s32 packet;

    packet = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(packet);
    itfSendTablePacket(packet, 0, 0);
    func_00198C70(D_003686C8, D_003686D8, D_003686E8, 0xFFF, (s32)texture, 0, packet);
    D_003255A8.invoke(&D_003255A8, packet);
}

/* Run the picture's own update step while its active flag is set. */
s32 evtUpdatePictureWhenFlagged(void) {
    Picture *picture;

    picture = kwlnTaskGetUserValue();
    if (picture->flags & 1) {
        if (picture->texture != 0) {
            func_00235150(picture->texture);
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
