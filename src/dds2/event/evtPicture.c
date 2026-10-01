#include "common.h"

/* Picture task data: an owner link and the attached texture handle. */
typedef struct {
    u32 unk0;
    void *unk4;
} Picture;

extern void *kwlnTaskGetUserValue(void);
extern void sdfTexReleaseReferenceViaHandler(void *);
extern void sdfReleaseChipBlock(void *);

extern void func_0024FEF0(void *);

INCLUDE_ASM(const s32, "event/evtPicture", func_0024FEF0);

/* Run the picture's own update step while its active flag is set. */
s32 evtUpdatePictureWhenFlagged(void) {
    Picture *picture;

    picture = kwlnTaskGetUserValue();
    if (picture->unk0 & 1) {
        if (picture->unk4 != 0) {
            func_0024FEF0(picture->unk4);
        }
        return 0;
    }
    return 0;
}

/* Drop the picture's texture, then free the task data. */
void evtPictureReleaseTaskTextureAndState(void) {
    Picture *picture;

    picture = kwlnTaskGetUserValue();
    if (picture->unk4 != 0) {
        sdfTexReleaseReferenceViaHandler(picture->unk4);
        picture->unk4 = 0;
    }
    sdfReleaseChipBlock(picture);
}
