#include "common.h"
#include "sdf_task_work.h"
#include "mnu_mantra_grid.h"
#include "mnu_profile_progress.h"
#include "mnu_scene_work.h"
#include "sdf_grid.h"

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
extern u32 ptyGetProfileRecordValue(struct DatPartyRecord *, u16);


extern u32 D_003E274C[];

INCLUDE_ASM(const s32, "game/code_00258258", func_00258258);

u32 func_00258508(s32 direction, MnuMantraGridEntry *scene, MantraNeighborState *states, MnuProfileProgress *target) {
    MantraNeighborRecord *record = &D_0036AE80[scene->sceneId];
    u32 flags = 0x100;
    if (prfGetCapValue(scene->sceneId) == ptyGetProfileRecordValue(target->partyRecord, scene->sceneId)) {
        flags = 0x101;
    }
    if (states[record->neighbors[direction]].status == 1) {
        flags |= 0x12;
    }
    if (states[record->neighbors[direction]].status == 2) {
        flags |= 0x10;
    }
    if (prfGetCapValue((u16)record->neighbors[direction]) ==
        ptyGetProfileRecordValue(target->partyRecord, (u16)record->neighbors[direction])) {
        flags |= 4;
    }
    if (record->edgeFlags[direction] & 1) {
        flags |= 8;
    }
    return flags;
}

INCLUDE_ASM(const s32, "game/code_00258258", func_00258620);

void func_0024E728(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);

INCLUDE_ASM(const s32, "game/code_00258258", func_00258A70);


void func_00258AF0(MnuGridFeedbackState *state, u32 mode) {
    state->mode = mode;
    state->frame = 0;
}

void sndSetSequenceVolumePan(s32 arg0, s32 arg1, s32 arg2);


/* Step the notification frame (wraps at 31); mode 1 plays the cue once at frame 0.
 * Its unused implicit-int result preserves the call's non-sibling epilogue. */
func_00258B00(MnuGridFeedbackState *voice) {
    s32 notify;

    notify = 0;
    switch (voice->mode) {
    case 0:
        voice->frame = voice->frame + 1;
        if (voice->frame >= 0x1F) {
            voice->frame = 0;
        }
        break;
    case 1:
        notify = voice->frame == 0;
        voice->frame = voice->frame + 1;
        if (voice->frame >= 0x1F) {
            voice->frame = 0;
            voice->mode = 0;
        }
        break;
    }
    if (notify == 1) {
        sndSetSequenceVolumePan(0x15, 0x7F, 0x3F);
    }
}

void func_0024E3C0(s32 x, s32 y, s32 z, s32 alpha, s32 flags,
                   s32 placementIndex, s32 context);
void func_0024E470(s32 x, s32 y, s32 z, s32 alpha, s32 flags,
                   s32 placementIndex, s32 context, f32 rotation);

void func_00258B90(s32 x, s32 y, s32 z, s32 alpha, MnuGridFeedbackState *state,
                   s32 context) {
    f32 progress;

    /* Both modes interpolate in radians and draw the ring pieces in degrees. */
    switch (state->mode) {
    case 0: {
        f32 rotation;
        f32 degrees;
        progress = (f32)state->frame / 30.0f;
        rotation = 6.2831853f;
        degrees = 57.29578f;
        func_0024E3C0(x, y, z, alpha, 0x20, 0x43, context);
        rotation = progress * rotation;
        func_0024E470(x, y, z, alpha, 0x20, 0x42, context, rotation * degrees);
        rotation += 3.14159265f;
        func_0024E470(x, y, z, alpha, 0x20, 0x42, context, rotation * degrees);
        return;
    }
    case 1: {
        f32 originalAlpha;
        f32 fade;
        f32 secondary;
        f32 degrees;
        s32 count;
        progress = (f32)state->frame / 30.0f;
        progress = 1.0f - progress;
        originalAlpha = (f32)alpha;
        func_0024E3C0(x, y, z, (s32)(originalAlpha * progress), 0x20, 0x41, context);
        count = state->frame;
        if (count < 10) {
            progress = (f32)count / 10.0f;
            fade = 1.0f;
        } else {
            progress = 1.0f;
            fade = (f32)(30 - count) / 20.0f;
        }
        secondary = 0.0f;
        if (count >= 7) {
            if (count >= 11) {
                secondary = (f32)(30 - count) / 19.0f;
            } else {
                secondary = (f32)(count - 6) * 0.25f;
            }
        }
        alpha = (s32)(originalAlpha * fade);
        degrees = 57.29578f;
        func_0024E470(x, y, z, alpha, 0x20, 0x3F, context, progress * -1.5707963f * degrees);
        func_0024E470(x, y, z, alpha, 0x20, 0x40, context, progress * 1.5707963f * degrees);
        func_0024E470(x, y, z, (s32)(originalAlpha * secondary), 0x20, 0x40, context,
                      1.5707963f * 57.29578f);
        progress = (f32)state->frame / 30.0f;
        func_0024E3C0(x, y, z, (s32)(originalAlpha * progress), 0x20, 0x43, context);
        break;
    }
    }
}





INCLUDE_ASM(const s32, "game/code_00258258", func_00258EB8);


void func_00258FD0(s32 x, s32 y, s32 z, s32 alpha, MnuMantraGridEntry *entry,
                   s32 context) {
    MnuMantraGridPulse *pulse = &entry->pulse;
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

extern TaskWork *mnuSceneResourceContext;





extern u32 mnuGetMantraDisplayFlags(MnuMantraGridEntry *scene, MnuProfileProgress *target);
extern void func_00258EB8(MnuMantraGridEntry *entry);

void func_002593E0(MnuProfileProgress *target, SdfGrid *grid, SdfGridCell *entry) {
    MenuSceneWork *scene;
    MnuMantraGridEntry *displayEntry;
    u32 flags;

    scene = (MenuSceneWork *)sdfGetTaskValueByKey(mnuSceneResourceContext, 1);
    displayEntry = (MnuMantraGridEntry *)(u32)entry->value;
    displayEntry->frame += 1;
    if ((f32)displayEntry->frame > 60.0f) {
        displayEntry->frame = 0;
    }
    flags = mnuGetMantraDisplayFlags(displayEntry, target);
    if ((flags & MNU_MANTRA_DISPLAY_FLAG_PROFILE_MATCH) != 0) {
        func_00258B00(&scene->gridFeedback);
        return;
    }
    if ((flags & MNU_MANTRA_DISPLAY_FLAG_AT_CAP) != 0) {
        func_00258EB8(displayEntry);
    }
}

INCLUDE_RODATA(const s32, "game/code_00258258", D_003AF9B8);

INCLUDE_SDATA(const s32, "game/code_00258258", D_003BC488);

INCLUDE_SDATA(const s32, "game/code_00258258", D_003BC490);

INCLUDE_SDATA(const s32, "game/code_00258258", D_003BC498);

