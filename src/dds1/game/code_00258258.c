#include "common.h"

extern u32 D_003E274C[];

INCLUDE_ASM(const s32, "game/code_00258258", func_00258258);

INCLUDE_ASM(const s32, "game/code_00258258", func_00258508);

INCLUDE_ASM(const s32, "game/code_00258258", func_00258620);

typedef struct {
    s32 count;
    s32 mode;
} SoundVoice;

void func_0024E728(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);

INCLUDE_ASM(const s32, "game/code_00258258", func_00258A70);


void func_00258AF0(u32 *state, u32 value) {
    state[1] = value;
    *state = 0;
}

void sndSetSequenceVolumePan(s32 arg0, s32 arg1, s32 arg2);


INCLUDE_ASM(const s32, "game/code_00258258", func_00258B00);
INCLUDE_ASM(const s32, "game/code_00258258", func_00258B90);




INCLUDE_ASM(const s32, "game/code_00258258", func_00258EB8);

INCLUDE_ASM(const s32, "game/code_00258258", func_00258FD0);

extern s32 mnuSceneResourceContext;
typedef struct MantraPulseGrid MantraPulseGrid;

typedef struct MantraPulseEntry {
    s32 unk_0;
    struct DspScene *scene;
} MantraPulseEntry;

typedef struct DspScene {
    s32 frame;
    u8 pad04[8];
    u16 sceneId;
    u8 pad0E[6];
    s32 state;
} DspScene;

typedef struct DspProfileSelection {
    u32 unit;
    s32 profileId;
} DspProfileSelection;

extern void *func_002CB3B8(s32 arg0, s32 arg1);
extern u32 mnuGetMantraDisplayFlags(DspScene *scene, DspProfileSelection *target);
extern void func_00258B00(void *sceneState);
extern void func_00258EB8(DspScene *entry);

void func_002593E0(DspProfileSelection *target, MantraPulseGrid *grid, MantraPulseEntry *entry) {
    void *scene;
    DspScene *displayEntry;
    u32 flags;

    scene = func_002CB3B8(mnuSceneResourceContext, 1);
    displayEntry = entry->scene;
    displayEntry->frame += 1;
    if ((f32)displayEntry->frame > 60.0f) {
        displayEntry->frame = 0;
    }
    flags = mnuGetMantraDisplayFlags(displayEntry, target);
    if ((flags & 1) != 0) {
        func_00258B00((u8 *)scene + 0x488);
        return;
    }
    if ((flags & 2) != 0) {
        func_00258EB8(displayEntry);
    }
}

INCLUDE_RODATA(const s32, "game/code_00258258", D_003AF9B8);

INCLUDE_SDATA(const s32, "game/code_00258258", D_003BC488);

INCLUDE_SDATA(const s32, "game/code_00258258", D_003BC490);

INCLUDE_SDATA(const s32, "game/code_00258258", D_003BC498);

