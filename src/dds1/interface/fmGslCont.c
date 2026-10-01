#include "common.h"

/* File group header: two handles to release plus an "in use" flag word. */
typedef struct {
    void *unk0;
    void *unk4;
    s32 pad8[5];
    s32 unk1C;
} FmGslWork;

extern FmGslWork frFontResourceList;
extern void func_002D0918(void *);

/* Release the group's handles once and clear its active-node flag. */
s32 fmGslReleaseActiveResourceBuffers(void) {
    if (frFontResourceList.unk1C == 0) {
        return 0;
    }
    func_002D0918(frFontResourceList.unk0);
    func_002D0918(frFontResourceList.unk4);
    frFontResourceList.unk1C = 0;
    return 1;
}

INCLUDE_ASM(const s32, "interface/fmGslCont", func_00193BB8);
