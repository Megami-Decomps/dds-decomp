#include "common.h"

typedef struct MantraPulseState {
    u16 timer;
    s16 duration;
    u32 mode;
} MantraPulseState;

typedef struct DspScene {
    s32 frame;
    s32 value;
    s32 cap;
    u16 sceneId;
    u16 param7b6;
    u32 param7b5;
    s32 state;
    u8 rawSkillList[0x34];
    MantraPulseState pulse;
    s8 profileFlag;
    u8 pad55[3];
} DspScene;

typedef struct DspProfileSelection {
    u32 unit;
    s32 profileId;
} DspProfileSelection;

typedef struct MantraNeighborRecord {
    s32 kind;
    u8 pad04[4];
    s8 neighbors[4];
    u8 edgeFlags[4];
} MantraNeighborRecord;

typedef struct MantraNeighborState {
    s32 unk00;
    s32 unk04;
    s32 status;
} MantraNeighborState;

extern MantraNeighborRecord D_0036AE80[89];
extern u32 prfGetCapValue(u16);
extern u32 ptyGetProfileRecordValue(u32, u16);


extern u32 D_003E274C[];

INCLUDE_ASM(const s32, "game/code_00258258", func_00258258);

u32 func_00258508(s32 direction, DspScene *scene, MantraNeighborState *states, DspProfileSelection *target) {
    MantraNeighborRecord *record = &D_0036AE80[scene->sceneId];
    u32 flags = 0x100;
    if (prfGetCapValue(scene->sceneId) == ptyGetProfileRecordValue(target->unit, scene->sceneId)) {
        flags = 0x101;
    }
    if (states[record->neighbors[direction]].status == 1) {
        flags |= 0x12;
    }
    if (states[record->neighbors[direction]].status == 2) {
        flags |= 0x10;
    }
    if (prfGetCapValue((u16)record->neighbors[direction]) ==
        ptyGetProfileRecordValue(target->unit, (u16)record->neighbors[direction])) {
        flags |= 4;
    }
    if (record->edgeFlags[direction] & 1) {
        flags |= 8;
    }
    return flags;
}

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


/* Step the voice's notification counter (wraps at 31); mode 1 plays the cue once at count 0 and then
 * drops back to mode 0. Implicit int: retail keeps jal + epilogue for the trailing void call. */
func_00258B00(SoundVoice *voice) {
    s32 notify;

    notify = 0;
    switch (voice->mode) {
    case 0:
        voice->count = voice->count + 1;
        if (voice->count >= 0x1F) {
            voice->count = 0;
        }
        break;
    case 1:
        notify = voice->count == 0;
        voice->count = voice->count + 1;
        if (voice->count >= 0x1F) {
            voice->count = 0;
            voice->mode = 0;
        }
        break;
    }
    if (notify == 1) {
        sndSetSequenceVolumePan(0x15, 0x7F, 0x3F);
    }
}

INCLUDE_ASM(const s32, "game/code_00258258", func_00258B90);




INCLUDE_ASM(const s32, "game/code_00258258", func_00258EB8);

void func_0024E3C0(s32 x, s32 y, s32 z, s32 alpha, s32 flags,
                   s32 placementIndex, s32 context);
void func_0024E470(s32 x, s32 y, s32 z, s32 alpha, s32 flags,
                   s32 placementIndex, s32 context, f32 rotation);

void func_00258FD0(s32 x, s32 y, s32 z, s32 alpha, DspScene *entry,
                   s32 context) {
    MantraPulseState *pulse = &entry->pulse;
    u32 mode = pulse->mode;
    f32 progress;

    if ((u32)mode >= 6) {
        return;
    }

    /* The first two ring pieces interpolate in radians, then draw in degrees. */
    switch (mode) {
    case 0:
    case 1:
        func_0024E3C0(x, y, z, alpha, 0x20, 0x44, context);
        func_0024E3C0(x, y, z, alpha, 0x20, 0x45, context);
        func_0024E3C0(x, y, z, alpha, 0x20, 0x46, context);
        return;

    case 2: {
        progress = (f32)(s16)pulse->timer / (f32)pulse->duration;

        func_0024E470(x, y, z, alpha, 0x20, 0x44, context,
                      progress * -1.0210175f * 57.29578f);
        func_0024E470(x, y, z, alpha, 0x20, 0x45, context,
                      progress * 0.31415924f * 57.29578f);
        func_0024E3C0(x, y, z, alpha, 0x20, 0x46, context);
        return;
    }

    case 3:
        func_0024E470(x, y, z, alpha, 0x20, 0x44, context,
                      -1.0210175f * 57.29578f);
        func_0024E470(x, y, z, alpha, 0x20, 0x45, context,
                      0.31415924f * 57.29578f);
        func_0024E3C0(x, y, z, alpha, 0x20, 0x46, context);
        return;

    case 4: {
        progress = (f32)(s16)pulse->timer / (f32)pulse->duration;
        progress = 1.0f - progress;

        func_0024E470(x, y, z, alpha, 0x20, 0x44, context,
                      progress * -1.0210175f * 57.29578f);
        func_0024E470(x, y, z, alpha, 0x20, 0x45, context,
                      progress * 0.31415924f * 57.29578f);
        func_0024E3C0(x, y, z, alpha, 0x20, 0x46, context);
        return;
    }

    case 5: {
        s16 timer = (s16)pulse->timer;

        if (timer < 50) {
            progress = (f32)timer / 50.0f;
        } else {
            progress = (f32)(60 - timer) / 10.0f;
        }
        func_0024E470(x, y, z, alpha, 0x20, 0x44, context,
                      progress * -1.0210175f * 57.29578f);
        func_0024E470(x, y, z, alpha, 0x20, 0x45, context,
                      progress * 0.31415924f * 57.29578f);
        func_0024E3C0(x, y, z, alpha, 0x20, 0x46, context);
        break;
    }
    }
}

extern s32 mnuSceneResourceContext;
typedef struct MantraPulseGrid MantraPulseGrid;

typedef struct MantraPulseEntry {
    s32 unk_0;
    struct DspScene *scene;
} MantraPulseEntry;





extern void *func_002CB3B8(s32 arg0, s32 arg1);
extern u32 mnuGetMantraDisplayFlags(DspScene *scene, DspProfileSelection *target);
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

