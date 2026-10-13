#include "common.h"
#include "snd_sequence_volume_pan.h"
#include "sdf_trig.h"
#include "mnu_sound_resource_path_api.h"
#include "mnu_voice_resource_path_api.h"
#include "ui_color.h"
#include "mnu_result.h"
#include "brs_result_counter.h"

typedef struct { u32 word[6]; } BurstSprite;

typedef struct { BurstSprite sprite[8]; } BurstTable;

extern BurstTable D_00428420;



struct EffectSlotSet;
extern void effDrawSpriteWithCornerColors(s32, s32, s32, u32 *, s32, struct EffectSlotSet *, s32, s32);
extern void *memcpy(void *, const void *, u32);
extern u32 D_004285E0[4];



/* Fade a result row's background in; the row is ready once it is fully opaque. */
void brsUpdateResultBackgroundFade(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                   BrsProgressRow *unused, s32 context, s32 index) {
    s32 fade = 0x100 - work->fadeProgress;

    if (work->fadeAnimation[index].backgroundState == 0) {
        work->fadeAnimation[index].backgroundOpacity = uiBlendColors(0x80808080, 0x80808000, fade) & 0xFF;
        if (work->fadeAnimation[index].backgroundOpacity >= 0x80) {
            work->fadeAnimation[index].backgroundState = 1;
        }
    }
}

INCLUDE_RODATA(const s32, "game/code_0029DF18", D_00428560);

INCLUDE_RODATA(const s32, "game/code_0029DF18", D_00428570);

INCLUDE_ASM(const s32, "game/code_0029DF18", brsDrawProgressRowEntries);

INCLUDE_ASM(const s32, "game/code_0029DF18", brsDrawProgressRowUnitInfo);

/* Fade a result row's portrait in; the row is ready once the opacity reaches 0x60. */
void brsUpdateResultPortraitFade(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                   BrsProgressRow *unused, s32 context, s32 index) {
    s32 remaining = 0x100 - work->fadeProgress;

    if (work->fadeAnimation[index].portraitReady == 0) {
        u32 opacity = uiBlendColors(0x80808060, 0x80808000, remaining) & 0xFF;

        work->fadeAnimation[index].portraitOpacity = opacity;
        work->fadeAnimation[index].portraitPosition[0] = 0;
        work->fadeAnimation[index].portraitPosition[1] = 0;
        if (work->fadeAnimation[index].portraitOpacity >= 0x60) {
            work->fadeAnimation[index].portraitReady = 1;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0029DF18", brsDrawProgressRowOverlays);

extern void mnuSetTitleSequenceVolumePan(u32);
extern u32 ptyComputeTotalExp(DatPartyRecord *, s32);
extern s32 brsCalcProgressAnimationStep(BrsSkillPackageWork *, BrsProgressRow *, s32, s8);

void brsUpdateLevelProgressAnimation(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                   BrsProgressRow *entry, s32 context, s32 index) {
    s32 level = work->levelAnimation[index].level;
    s32 fade = 0x100 - work->fadeProgress;
    s32 nextLevel = level + 1;
    s32 currentExp;
    s32 finished;

    nextLevel = nextLevel < 2 ? 1 : (nextLevel > 99 ? 99 : nextLevel);
    switch (work->levelAnimation[index].drawPhase) {
    case 0:
        work->levelAnimation[index].alpha = uiBlendColors(0x80808080, 0x80808000, fade) & 0xFF;
        if (work->levelAnimation[index].alpha >= 0x80) {
            work->levelAnimation[index].drawPhase = 1;
        }
        break;
    case 1:
        work->levelAnimation[index].drawPhase++;
        if (work->levelAnimation[index].applied > 0 && work->resultPhase == 0) {
            mnuSetTitleSequenceVolumePan(0x14);
            work->resultPhase = 1;
        }
        break;
    case 2:
    case 3:
    case 4:
        if (level < 99 && entry->unit->hp != 0 && (entry->unit->status & 0x4000) == 0) {
            if (work->levelAnimation[index].drawPhase == 3) {
                goto flash;
            }
            work->levelAnimation[index].appliedStep = brsCalcProgressAnimationStep(work, entry, index, 0);
            currentExp = work->levelAnimation[index].remaining + work->levelAnimation[index].appliedStep;
            work->levelAnimation[index].remaining = currentExp;
            if (currentExp >= (s32)ptyComputeTotalExp(entry->unit, nextLevel - entry->unit->level) && work->levelAnimation[index].drawPhase == 2) {
                work->levelAnimation[index].level++;
                work->levelAnimation[index].level = work->levelAnimation[index].level < 2 ? 1 : (work->levelAnimation[index].level > 99 ? 99 : work->levelAnimation[index].level);
                if (work->unkAEB9 != 0) {
                    finished = 0;
                    while (work->levelAnimation[index].level < 99) {
                        nextLevel = work->levelAnimation[index].level + 1;
                        nextLevel = nextLevel < 2 ? 1 : (nextLevel > 99 ? 99 : nextLevel);
                        if (currentExp >= (s32)ptyComputeTotalExp(entry->unit, nextLevel - entry->unit->level)) {
                            work->levelAnimation[index].level++;
                            work->levelAnimation[index].level = work->levelAnimation[index].level < 2 ? 1 : (work->levelAnimation[index].level > 99 ? 99 : work->levelAnimation[index].level);
                        } else {
                            finished = 1;
                        }
                        if (work->levelAnimation[index].level >= 99) {
                            finished = 1;
                        }
                        if (finished != 0) {
                            break;
                        }
                    }
                }
                work->levelAnimation[index].progressIconEnabled = 1;
                work->levelAnimation[index].iconState = 1;
                work->levelAnimation[index].unk2D = 1;
                work->levelAnimation[index].flashOpacity = 0x80;
                work->levelAnimation[index].flashFrame = 0;
                work->levelAnimation[index].alpha = 0;
                work->levelAnimation[index].drawPhase++;
                mnuSetTitleSequenceVolumePan(0x12);
            }
        } else {
            work->levelAnimation[index].applied = 0;
        }
        if (work->levelAnimation[index].drawPhase == 3) {
flash:
            work->levelAnimation[index].flashFrame++;
            work->levelAnimation[index].flashFrame = work->levelAnimation[index].flashFrame <= 0 ? 0 : (work->levelAnimation[index].flashFrame > 15 ? 15 : work->levelAnimation[index].flashFrame);
            if (work->levelAnimation[index].flashFrame >= 15) {
                work->levelAnimation[index].drawPhase++;
            }
        } else if (work->levelAnimation[index].drawPhase == 4) {
            work->levelAnimation[index].flashOpacity -= 16;
            work->levelAnimation[index].flashOpacity = work->levelAnimation[index].flashOpacity <= 0 ? 0 : (work->levelAnimation[index].flashOpacity > 128 ? 128 : work->levelAnimation[index].flashOpacity);
            if (work->levelAnimation[index].flashOpacity <= 0) {
                work->levelAnimation[index].unk2D = 0;
            }
            work->levelAnimation[index].alpha += 8;
            work->levelAnimation[index].alpha = work->levelAnimation[index].alpha <= 0 ? 0 : (work->levelAnimation[index].alpha > 128 ? 128 : work->levelAnimation[index].alpha);
            if (work->levelAnimation[index].alpha >= 128) {
                work->levelAnimation[index].drawPhase = 2;
                if (work->levelAnimation[index].applied > 0 && work->resultPhase == 0) {
                    mnuSetTitleSequenceVolumePan(0x14);
                    work->resultPhase = 1;
                }
            }
        }
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_0029DF18", brsDrawLevelProgressDetails);


void brsUpdateLevelProgressIndicator(s32 screenX, s32 screenY, s32 depth, BrsSkillPackageWork *work, BrsProgressRow *row, s32 context, s32 index) {
    s32 y;
    s32 bob;

    switch (work->levelAnimation[index].progressIconEnabled) {
    case 0:
        work->levelAnimation[index].progressIconOpacity = 0;
        work->levelAnimation[index].progressIconPosition[0] = 0xA6;
        work->levelAnimation[index].progressIconPosition[1] = 0x22;
        work->levelAnimation[index].progressIconAngle = 0xB4;
        break;
    case 1:
        work->levelAnimation[index].progressIconAngle = (work->levelAnimation[index].progressIconAngle + 15) % 360;
        work->levelAnimation[index].progressIconOpacity =
            (u32)(0.0f + (sdfSinPoly((f32)((work->levelAnimation[index].progressIconAngle + 90) % 360) / 180.0f * 3.1415899f) + 1.0f) * 0.5f * 144.0f);
        bob = (s32)(0.0f + (sdfSinPoly((f32)((work->levelAnimation[index].progressIconAngle + 90) % 360) / 180.0f * 3.1415899f) + 1.0f) * 0.5f * 22.0f);
        work->levelAnimation[index].progressIconPosition[1] = 0x24 - bob;
        work->levelAnimation[index].progressIconOpacity = (work->levelAnimation[index].progressIconOpacity > 0) ? ((work->levelAnimation[index].progressIconOpacity > 0x80) ? 0x80 : work->levelAnimation[index].progressIconOpacity) : 0;
        y = work->levelAnimation[index].progressIconPosition[1];
        work->levelAnimation[index].progressIconPosition[1] = (y < 0xF) ? 0xE : ((y > 0x24) ? 0x24 : y);
        if (work->levelAnimation[index].progressIconOpacity >= 0x80) {
            work->levelAnimation[index].progressIconEnabled += 2;
            work->levelAnimation[index].progressIconAngle = 0x78;
        }
        break;
    case 2:
        work->levelAnimation[index].progressIconEnabled++;
        work->levelAnimation[index].progressIconAngle = 0x78;
        break;
    default:
        work->levelAnimation[index].progressIconOpacity -= 10;
        work->levelAnimation[index].progressIconOpacity = (work->levelAnimation[index].progressIconOpacity > 0) ? ((work->levelAnimation[index].progressIconOpacity > 0x80) ? 0x80 : work->levelAnimation[index].progressIconOpacity) : 0;
        y = work->levelAnimation[index].progressIconPosition[1] - 1;
        work->levelAnimation[index].progressIconPosition[1] = y;
        work->levelAnimation[index].progressIconPosition[1] = (y < 5) ? 4 : ((y > 0xE) ? 0xE : y);
        if (work->levelAnimation[index].progressIconOpacity <= 0) {
            work->levelAnimation[index].progressIconEnabled = 0;
        }
        break;
    }
    switch (work->levelAnimation[index].iconState) {
    case 0:
        work->levelAnimation[index].unk54 = 0;
        work->levelAnimation[index].unk58[0] = 0x95;
        work->levelAnimation[index].unk58[1] = 0x15;
        break;
    case 1:
        work->levelAnimation[index].unk54 += 10;
        work->levelAnimation[index].unk54 = (work->levelAnimation[index].unk54 > 0) ? ((work->levelAnimation[index].unk54 > 0x80) ? 0x80 : work->levelAnimation[index].unk54) : 0;
        if (work->levelAnimation[index].unk54 >= 0x80) {
            work->levelAnimation[index].iconState++;
            work->levelAnimation[index].iconOpacity = 0;
        }
        break;
    case 2:
        work->levelAnimation[index].iconOpacity++;
        work->levelAnimation[index].iconOpacity = (work->levelAnimation[index].iconOpacity > 0) ? ((work->levelAnimation[index].iconOpacity > 8) ? 8 : work->levelAnimation[index].iconOpacity) : 0;
        if (work->levelAnimation[index].iconOpacity >= 8) {
            work->levelAnimation[index].iconState++;
        }
        break;
    default:
        work->levelAnimation[index].unk54 -= 0x10;
        work->levelAnimation[index].unk54 = (work->levelAnimation[index].unk54 > 0) ? ((work->levelAnimation[index].unk54 > 0x80) ? 0x80 : work->levelAnimation[index].unk54) : 0;
        if (work->levelAnimation[index].unk54 <= 0) {
            work->levelAnimation[index].iconState = 0;
        }
        break;
    }
}

extern u32 D_004285D0[4];

void brsDrawLevelProgressIndicator(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                   BrsProgressRow *row, s32 depth, s32 index) {
    u32 color[4];
    s32 packed;

    memcpy(color, D_004285D0, sizeof(color));
    if (work->teardownResource != NULL) {
        if (work->levelAnimation[index].progressIconEnabled != 0) {
            packed = work->levelAnimation[index].progressIconOpacity | 0x80808000;
            color[0] = packed;
            color[1] = packed;
            color[2] = packed;
            color[3] = packed;
            effDrawSpriteWithCornerColors(x + (work->levelAnimation[index].progressIconPosition[0] << 4),
                         y + (work->levelAnimation[index].progressIconPosition[1] << 3) + 0x40,
                         z, color, 0, work->teardownResource, 0x12, depth);
        }
        if (work->levelAnimation[index].iconState != 0) {
            packed = work->levelAnimation[index].unk54 | 0x80808000;
            color[0] = packed;
            color[1] = packed;
            color[2] = packed;
            color[3] = packed;
        }
    }
}

void brsUpdateProfileProgressIndicator(s32 screenX, s32 screenY, s32 depth, BrsSkillPackageWork *work, BrsProgressRow *row, s32 context, s32 index) {
    s32 y;
    s32 bob;

    switch (work->profileAnimation[index].progressIconEnabled) {
    case 0:
        work->profileAnimation[index].progressIconOpacity = 0;
        work->profileAnimation[index].progressIconPosition[0] = 0x12F;
        work->profileAnimation[index].progressIconPosition[1] = 0x22;
        work->profileAnimation[index].progressIconAngle = 0xB4;
        break;
    case 1:
        work->profileAnimation[index].progressIconAngle = (work->profileAnimation[index].progressIconAngle + 15) % 360;
        work->profileAnimation[index].progressIconOpacity =
            (u32)(0.0f + (sdfSinPoly((f32)((work->profileAnimation[index].progressIconAngle + 90) % 360) / 180.0f * 3.1415899f) + 1.0f) * 0.5f * 144.0f);
        bob = (s32)(0.0f + (sdfSinPoly((f32)((work->profileAnimation[index].progressIconAngle + 90) % 360) / 180.0f * 3.1415899f) + 1.0f) * 0.5f * 22.0f);
        work->profileAnimation[index].progressIconPosition[1] = 0x24 - bob;
        work->profileAnimation[index].progressIconOpacity = (work->profileAnimation[index].progressIconOpacity > 0) ? ((work->profileAnimation[index].progressIconOpacity > 0x80) ? 0x80 : work->profileAnimation[index].progressIconOpacity) : 0;
        y = work->profileAnimation[index].progressIconPosition[1];
        work->profileAnimation[index].progressIconPosition[1] = (y < 0xF) ? 0xE : ((y > 0x24) ? 0x24 : y);
        if (work->profileAnimation[index].progressIconOpacity >= 0x80) {
            work->profileAnimation[index].progressIconEnabled += 2;
            work->profileAnimation[index].progressIconAngle = 0x78;
        }
        break;
    case 2:
        work->profileAnimation[index].progressIconEnabled++;
        work->profileAnimation[index].progressIconAngle = 0x78;
        break;
    default:
        work->profileAnimation[index].progressIconOpacity -= 10;
        work->profileAnimation[index].progressIconOpacity = (work->profileAnimation[index].progressIconOpacity > 0) ? ((work->profileAnimation[index].progressIconOpacity > 0x80) ? 0x80 : work->profileAnimation[index].progressIconOpacity) : 0;
        y = work->profileAnimation[index].progressIconPosition[1] - 1;
        work->profileAnimation[index].progressIconPosition[1] = y;
        work->profileAnimation[index].progressIconPosition[1] = (y < 5) ? 4 : ((y > 0xE) ? 0xE : y);
        if (work->profileAnimation[index].progressIconOpacity <= 0) {
            work->profileAnimation[index].progressIconEnabled = 0;
        }
        break;
    }
    switch (work->profileAnimation[index].iconState) {
    case 0:
        work->profileAnimation[index].unk54 = 0;
        work->profileAnimation[index].unk58[0] = 0x122;
        work->profileAnimation[index].unk58[1] = 0x15;
        break;
    case 1:
        work->profileAnimation[index].unk54 = 0x80;
        work->profileAnimation[index].iconState++;
        work->profileAnimation[index].iconOpacity = 0;
        break;
    case 2:
        work->profileAnimation[index].iconOpacity = 0xFF;
        work->profileAnimation[index].iconState++;
        break;
    default:
        work->profileAnimation[index].iconOpacity -= 8;
        work->profileAnimation[index].iconOpacity = (work->profileAnimation[index].iconOpacity > 0)
            ? ((work->profileAnimation[index].iconOpacity > 0xFF) ? 0xFF : work->profileAnimation[index].iconOpacity)
            : 0;
        break;
    }
}

void brsDrawProfileProgressIndicator(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                   BrsProgressRow *unused, s32 depth, s32 index) {
    u32 color[4];
    s32 packed;

    memcpy(color, D_004285E0, sizeof(color));

    if (work->teardownResource != NULL) {
        if (work->profileAnimation[index].iconState >= 2) {
            packed = work->profileAnimation[index].iconOpacity | 0x80808000;
            color[0] = packed;
            color[1] = packed;
            color[2] = packed;
            color[3] = packed;
            effDrawSpriteWithCornerColors(x + 0x1180, y + 0x130, z, color, 0,
                         work->teardownResource, 0x16, depth);
            effDrawSpriteWithCornerColors(x + 0x17E0, y + 0x130, z, color, 0,
                         work->teardownResource, 0x17, depth);
        }
    }
}

extern u32 ptyComputeTotalExp(DatPartyRecord *, s32);
extern s32 ptyGetCurrentProfileId(DatPartyRecord *);
extern u32 ptyGetProfileRecordCap(u16);

s32 brsCalcProgressAnimationStep(BrsSkillPackageWork *work, BrsProgressRow *entry, s32 index, s8 mode) {
    s32 result = 0;
    s32 levelDelta = work->levelAnimation[index].level - entry->unit->level;
    s32 nextLevelExp = ptyComputeTotalExp(entry->unit, levelDelta + 1);
    s32 range = nextLevelExp - ptyComputeTotalExp(entry->unit, levelDelta);
    s32 step;
    s32 denominator;

    if (range == 0) {
        range = ptyComputeTotalExp(entry->unit, levelDelta + 1);
    }
    if (mode == 0) {
        if (work->levelAnimation[index].applied > 0) {
            if (work->levelAnimation[index].skipRamp == 0) {
                work->levelAnimation[index].frames++;
                work->levelAnimation[index].frames = work->levelAnimation[index].frames <= 0 ? 0 : (work->levelAnimation[index].frames > 120 ? 120 : work->levelAnimation[index].frames);
                denominator = 150 - work->levelAnimation[index].frames;
                step = 1;
                if (range >= denominator) {
                    step = range / denominator;
                }
            } else {
                step = 10000;
            }
            if (work->unkAEB9 != 0) {
                step = work->levelAnimation[index].applied;
            }
            work->levelAnimation[index].applied -= step;
            if (work->levelAnimation[index].applied < 0) {
                step += work->levelAnimation[index].applied;
                work->levelAnimation[index].applied = 0;
            }
            work->levelAnimation[index].applied = work->levelAnimation[index].applied <= 0 ? 0 : (work->levelAnimation[index].applied > 0x1000000 ? 0x1000000 : work->levelAnimation[index].applied);
            result = step;
        }
    } else {
        range = ptyGetProfileRecordCap(ptyGetCurrentProfileId(entry->unit));
        if (work->profileAnimation[index].applied > 0) {
            if (work->profileAnimation[index].skipRamp == 0) {
                work->profileAnimation[index].frames++;
                work->profileAnimation[index].frames = work->profileAnimation[index].frames <= 0 ? 0 : (work->profileAnimation[index].frames > 120 ? 120 : work->profileAnimation[index].frames);
                denominator = 150 - work->profileAnimation[index].frames;
                step = 1;
                if (range >= denominator) {
                    step = range / denominator;
                }
            } else {
                step = 10000;
            }
            if (work->unkAEB9 != 0) {
                step = work->profileAnimation[index].applied;
            }
            work->profileAnimation[index].applied -= step;
            if (work->profileAnimation[index].applied < 0) {
                step += work->profileAnimation[index].applied;
                work->profileAnimation[index].applied = 0;
            }
            work->profileAnimation[index].applied = work->profileAnimation[index].applied <= 0 ? 0 : (work->profileAnimation[index].applied > 0x1000000 ? 0x1000000 : work->profileAnimation[index].applied);
            result = step;
        }
    }
    return result;
}

#include "common.h"
#include "mnu_result.h"
#include "sdf_chip.h"
#include "sdf_resource.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "mnu_title_effect.h"
#include "mnu.h"
#include "file.h"
#include "kwln_task_lifecycle.h"
#include "file_request_api.h"
#include "sdf_thread.h"

#include "mnu_title_stream.h"

#define BRS_RESULT_COUNTER_PAIR_COUNT 5
#define BRS_RESULT_SETTLED_POLL_LIMIT 6
#define BRS_RESULT_SETTLED_POLL_CLAMP 7

#define SND_TRACK_DEFAULT_VOLUME 0x7f
#define SND_TRACK_DEFAULT_PAN 0x3f
#define SND_TRANSFER_WORD_BYTES 4
#define SND_TRANSFER_WORD_HALFWORDS 2
#define SND_TRANSFER_PAIR_WORD_BYTES 8
#define SND_MIX_SAMPLE_COUNT 0x800
#define SND_MIX_SAMPLE_LIMIT 0x7FFF

#define MNU_TITLE_EFFECT_BYTES 8
#define MNU_TITLE_VOICE_PREFIX_BYTES 5
#define MNU_TITLE_VOICE_PATH_BYTES 16
#define MNU_TITLE_SOUND_PATH_BYTES 32

/* Each compressed-frame size has a corresponding 600-frame allocation. */
#define MNU_SOUND_FRAME_BYTES_LARGE 0x180
#define MNU_SOUND_FRAME_BYTES_MEDIUM 0xC0
#define MNU_SOUND_FRAME_BYTES_SMALL 0x60
#define MNU_SOUND_BUFFER_BYTES_LARGE 0x38400
#define MNU_SOUND_BUFFER_BYTES_MEDIUM 0x1C200
#define MNU_SOUND_BUFFER_BYTES_SMALL 0xE100


#define MNU_STREAM_LOAD_IDLE 0
#define MNU_STREAM_LOAD_PENDING 1
#define MNU_STREAM_LOAD_COPIED 2
#define MNU_STREAM_COMMIT_READY 3
#define MNU_STREAM_COMMIT_COMPLETE 4

extern u32 mnuTitleStreamSemaphore;

extern s32 scrReadIntParameter(s32);

extern u32 sdfSoundIsCommandBusy(void);

extern s32 mnuPollTitleStreamStateLocked(void);

extern u32 D_00437A2C;

extern KwlnTask *mnuTitleSoundTask;

extern TitleAudioStreamState mnuTitleStreamStatus;

extern u32 D_00438FEC;

extern TitleAudioStreamState mnuTitleSoundBufferState;

extern char D_00428680[]; /* "titleProc" */

typedef struct { u64 v; } __attribute__((packed)) u64p;

extern u8 D_00437A00[];

extern char *strcat(char *, char *);

extern u8 D_00437A08[];

extern s32 WaitSema(u32);

extern s32 SignalSema(u32);

extern s32 sceSifInitIopHeap(void);
extern s32 sceSifAllocIopHeap(s32 size);
extern void Exit(s32 status);
extern s32 D_00438FD8;

/* libc sprintf returns the signed vfprintf character count. */
extern s32 func_0035C860(char *dst, const char *fmt, ...);
extern u32 D_00437A20[2];

extern u32 D_00437A28;

extern char D_00428550[]; /* "result2_draw" */

typedef struct BrsResultCounter {
    u8 pad00[0x14];
    s8 phase;
    u8 pad15[7];
    s32 alpha;
    u8 pad20[0x10];
    s32 shown;
    s32 remaining;
    s32 increment;
    u8 pad3C[5];
    s8 unk41;
    u8 pad42[2];
    s32 unk44;
    s32 unk48;
    s8 unk4C;
    u8 pad4D[0x13];
    s8 unk60;
    u8 pad61[7];
} BrsResultCounter;

typedef char BrsResultCounter_shared_view_size[
    (sizeof(BrsResultCounter) == sizeof(BrsResultCounterView)) ? 1 : -1];
typedef char BrsResultCounter_shared_phase_offset[
    ((u32)&((BrsResultCounter *)0)->phase == 0x14) ? 1 : -1];
typedef char BrsResultCounter_shared_remaining_offset[
    ((u32)&((BrsResultCounter *)0)->remaining == 0x34) ? 1 : -1];

typedef struct BrsResultWork {
    u8 pad00[0xAEAC];
    s32 settledFrames;
    u8 padAEB0[8];
    s8 soundStarted;
    u8 padAEB9[0x193];
    BrsResultCounter levelCounters[8];
    BrsResultCounter profileCounters[8];
    u8 padB6CC[0x14];
    s32 fadeProgress;
    u8 padB6E4[0x20];
} BrsResultWork;

extern DatProfileRecord *ptyGetCurrentProfileRecord(DatPartyRecord *);
extern s32 ptyGetCurrentProfileId(DatPartyRecord *);
extern u32 ptyGetProfileRecordCap(u16);
extern s32 brsCalcProgressAnimationStep(BrsSkillPackageWork *, BrsProgressRow *, s32, s8);
void mnuSetTitleSequenceVolumePan(u32);

void brsUpdateProfileProgressAnimation(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                   BrsProgressRow *entry, s32 context, s32 index) {
    s32 fade = 0x100 - work->fadeProgress;
    s32 profileId;
    s32 cap;
    s32 progress;

    ptyGetCurrentProfileRecord(entry->unit);
    profileId = ptyGetCurrentProfileId(entry->unit);
    if (profileId != 0) {
        cap = ptyGetProfileRecordCap(profileId);
    } else {
        cap = 0;
        work->profileAnimation[index].applied = 0;
    }
    progress = work->profileAnimation[index].remaining;
    switch (work->profileAnimation[index].drawPhase) {
    case 0:
        work->profileAnimation[index].alpha = uiBlendColors(0x80808080, 0x80808000, fade) & 0xFF;
        if (work->profileAnimation[index].alpha >= 0x80) {
            work->profileAnimation[index].drawPhase = 1;
        }
        break;
    case 1:
        work->profileAnimation[index].drawPhase++;
        if (work->profileAnimation[index].applied > 0 && work->resultPhase == 0) {
            mnuSetTitleSequenceVolumePan(0x14);
            work->resultPhase = 1;
        }
        break;
    case 2:
    case 3:
    case 4:
        if (work->profileAnimation[index].drawPhase == 2) {
            if (cap != 0) {
                if (progress < cap) {
                    work->profileAnimation[index].appliedStep = brsCalcProgressAnimationStep(work, entry, index, 1);
                    work->profileAnimation[index].remaining += work->profileAnimation[index].appliedStep;
                    if (work->profileAnimation[index].remaining >= cap) {
                        work->profileAnimation[index].progressIconEnabled = 1;
                        work->profileAnimation[index].iconState = 1;
                        work->profileAnimation[index].unk2D = 1;
                        work->profileAnimation[index].flashOpacity = 0x80;
                        work->profileAnimation[index].flashFrame = 0;
                        work->profileAnimation[index].alpha = 0;
                        work->profileAnimation[index].drawPhase++;
                        mnuSetTitleSequenceVolumePan(0x12);
                    }
                } else {
                    work->profileAnimation[index].applied = 0;
                }
            }
        } else {
            if (work->profileAnimation[index].alpha < 0x80) {
                work->profileAnimation[index].alpha += 16;
            }
            if (work->profileAnimation[index].alpha > 0x80) {
                work->profileAnimation[index].alpha = 0x80;
            }
        }
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_0029DF18", brsDrawProfileProgressDetails);

/* Complete after six successful polls of five counter pairs.
 * Early returns deliberately leave the accumulated settled count unchanged. */
s32 brsPollResultCounterCompletion(void) {
    KwlnTask *resultTask = kwlnTaskGetTaskByName(D_00428550);
    BrsResultWork *resultWork;
    s32 counterIndex;

    if (resultTask == NULL) {
        return 0;
    }
    resultWork = (BrsResultWork *)kwlnTaskGetUserValue(resultTask);
    for (counterIndex = 0; counterIndex < BRS_RESULT_COUNTER_PAIR_COUNT; counterIndex++) {
        if (resultWork->levelCounters[counterIndex].phase >= 2) {
            if (resultWork->levelCounters[counterIndex].remaining != 0 ||
                resultWork->levelCounters[counterIndex].unk4C != 0 ||
                resultWork->levelCounters[counterIndex].unk60 != 0) {
                return 0;
            }
        } else if (resultWork->levelCounters[counterIndex].remaining != 0) {
            return 0;
        }
        if (resultWork->profileCounters[counterIndex].phase >= 2) {
            if (resultWork->profileCounters[counterIndex].phase == 2) {
                if (resultWork->profileCounters[counterIndex].remaining != 0) {
                    return 0;
                }
            }
        }
    }
    resultWork->settledFrames++;
    resultWork->settledFrames = resultWork->settledFrames <= 0 ? 0 :
        resultWork->settledFrames >= BRS_RESULT_SETTLED_POLL_CLAMP ? BRS_RESULT_SETTLED_POLL_LIMIT : resultWork->settledFrames;
    if (resultWork->settledFrames < BRS_RESULT_SETTLED_POLL_LIMIT) {
        return 0;
    }
    return 1;
}

void mnuSetTitleSequenceVolumePan(u32 sequenceId) {
    sndSetSequenceVolumePan(sequenceId, SND_TRACK_DEFAULT_VOLUME, SND_TRACK_DEFAULT_PAN);
}

void func_002A1020(void) {
}

void func_002A1028(void) {
}

void func_002A1030(void) {
}

void func_002A1038(void) {
}

/* Copy the packed eight-byte prefix, then append the caller's filename. */
char *mnuBuildSoundResourcePath(char *pathBuffer, char *filename) {
    *(u64p *)pathBuffer = *(u64p *)D_00437A00;
    return strcat(pathBuffer, filename);
}

/* Use the voice prefix with the same packed, unbounded append convention. */
char *mnuBuildVoiceResourcePath(char *pathBuffer, char *filename) {
    *(u64p *)pathBuffer = *(u64p *)D_00437A08;
    return strcat(pathBuffer, filename);
}

extern s8 D_004379FC;

extern char D_003E0B30[];

extern char D_003E0B50[];

extern s32 D_003D9D60[];

extern s32 D_00437A18;

extern s16 D_00438FCC;

extern s32 D_00437A1C;

extern void func_00341AD8(char *, s32, char *, s32);

extern void sndEnsureMidiBankResident(s32);

extern void mnuInitializeTitleAudioStreams(void);

extern void mnuCreateTitleEffectTask(void);

void mnuInitializeTitleAudioAndEffects(void) {
    if (D_004379FC != 0) {
        sndQueueNoPayloadCommand40();
        return;
    }
    D_004379FC = 1;
    func_00341AD8(D_003E0B30, 4, D_003E0B50, 4);
    sndEnsureMidiBankResident(D_003D9D60[0]);
    D_00437A18 = 4;
    D_00438FCC = -1;
    D_00437A1C = 0;
    mnuInitializeTitleAudioStreams();
    mnuCreateTitleEffectTask();
}

u32 func_002A1118(void) {
    return 0x608;
}

s32 mnuIncrementTitleEffectFrameCounter(KwlnTask *task) {
    TitleEffectState *effectState;

    effectState = (TitleEffectState *)kwlnTaskGetUserValue(task);
    effectState->frameCounter = effectState->frameCounter + 1;
    return 0;
}

void mnuDestroyTitleEffectTask(KwlnTask *task) {
    sdfReleaseChipBlock((void *)kwlnTaskGetUserValue(task));
    mnuTitleSoundTask = 0;
}

extern u8 D_00437A10[];

/* Allocate the two-word effect state and attach it to the frame-counter task. */
void mnuCreateTitleEffectTask(void) {
    TitleEffectState *effectState = (TitleEffectState *)sdfAllocSizeClassBlock(MNU_TITLE_EFFECT_BYTES);
    KwlnTask *effectTask = kwlnTaskCreate(D_00437A10, 0x5214, 1, 1,
                              mnuIncrementTitleEffectFrameCounter, mnuDestroyTitleEffectTask, 0);
    mnuTitleSoundTask = effectTask;
    kwlnTaskSetUserValue(effectTask, (u32)effectState);
    effectState->soundNameIndex = 0;
    effectState->frameCounter = 0;
}

void mnuResetTitleEffectState(s32 command) {
    TitleEffectState *effectState = (TitleEffectState *)kwlnTaskGetUserValue(mnuTitleSoundTask);
    if (sdfSoundIsCommandBusy() != 0) {
        sdfSoundStopNamedPlayback();
    }
    sdfSoundSendNamedCommand(command, 0x7f);
    effectState->frameCounter = 0;
}

void mnuSetTitleVoicePrefixIndex(s32 prefixIndex) {
    ((TitleEffectState *)kwlnTaskGetUserValue(mnuTitleSoundTask))->soundNameIndex = prefixIndex;
}

extern char D_00428610[];

extern char D_00428628[];

extern char D_003E09F0[];

/* The native char*-typed voice argument is passed to numeric %04d formatting;
 * retain that signature rather than treating it as a filename string. */
void mnuPlayTitleVoiceFile(char *voiceArgument) {
    char voicePath[MNU_TITLE_VOICE_PATH_BYTES];
    TitleEffectState *effectState = (TitleEffectState *)kwlnTaskGetUserValue(mnuTitleSoundTask);
    if (sdfSoundIsCommandBusy() != 0) {
        sdfDebugPrintf(D_00428610);
        sdfSoundStopNamedPlayback();
    }
    func_0035C860(voicePath, D_00428628, D_003E09F0 + MNU_TITLE_VOICE_PREFIX_BYTES * effectState->soundNameIndex, voiceArgument);
    sdfSoundSendNamedCommand(voicePath, 0x64);
    effectState->frameCounter = 0;
}

void mnuStopTitleVoicePlayback(void) {
    sdfSoundStopNamedPlayback();
}

s32 mnuQueryTitleSoundBusy(void) {
    return sdfSoundIsCommandBusy();
}

void func_002A1338(void) {
}

s32 mnuGetTitleEffectFrameCounter(void) {
    return ((TitleEffectState *)kwlnTaskGetUserValue(mnuTitleSoundTask))->frameCounter;
}

u32 sndOpStartTrackFromScript(void) {
    s32 sequence;

    sequence = scrReadIntParameter(0);
    sndStartTrackDefault(sequence);
    return 1;
}

u32 sndOpSetTrackDefaultVolumePan(void) {
    s32 sequence;

    sequence = scrReadIntParameter(0);
    sndSetSequenceVolumePan(sequence, SND_TRACK_DEFAULT_VOLUME, SND_TRACK_DEFAULT_PAN);
    return 1;
}

u32 sndOpSendTitleAudioCommand(void) {
    sndQueueNoPayloadCommand40();
    return 1;
}

s32 mnuInitializeTitleEffects(void) {
    s32 commandName;
    commandName = scrReadStringParameter(0);
    if (sdfSoundIsCommandBusy() != 0) {
        sdfSoundStopNamedPlayback();
    }
    sdfSoundSendNamedCommand(commandName, 0x7f);
    return 1;
}

u32 sndOpStopNamedPlayback(void) {
    sdfSoundStopNamedPlayback();
    return 1;
}

u32 sndOpPushCommandBusyState(void) {
    u32 soundBusy;

    soundBusy = sdfSoundIsCommandBusy();
    scrSetIntegerReturnValue(soundBusy);
    return 1;
}

u32 func_002A1478(void) {
    return 1;
}

s32 movCheckStartupSoundState(void) {
    if (mnuPollTitleStreamStateLocked() == 0) {
        mnuQueueTitleSoundStreamEntry(scrReadIntParameter(0));
        return 0;
    }
    return mnuPollTitleStreamStateLocked() != 1;
}

u32 sndOpResetTitleStreamAfterFileIdle(void) {
    mnuResetTitleStreamAfterFileIdle();
    return 1;
}

u32 mnuResetTitlePlaybackCommands(void) {
    mnuMarkTitleStreamResetPending();
    mnuResetTitleStreamLocked();
    return 1;
}

u32 mnuAdvanceTitleStreamFromScript(void) {
    mnuAdvanceTitleStateUnderSemaphore();
    return 1;
}

u8 mnuIsTitleStreamIdle(void) {
    s64 loadState;

    loadState = mnuPollTitleStreamStateLocked();
    return loadState == MNU_STREAM_LOAD_IDLE;
}

/* Flush EE cache, submit one word-sized transfer to IOP, and busy-wait for DMA.
 * Return the native request ID; there is no failure retry or timeout here. */
s32 sndCopyWordsToIopSynchronously(u32 sourceAddress, u32 iopAddress, u32 wordCount) {
    struct {
        u32 source;
        u32 destination;
        u32 size;
        u32 attributes;
    } transfer;
    s32 dmaRequest;
    transfer.source = sourceAddress;
    transfer.destination = iopAddress;
    transfer.size = wordCount * SND_TRANSFER_WORD_BYTES;
    transfer.attributes = 0;
    FlushCache(0);
    dmaRequest = sceSifSetDma(&transfer, 1);
    while (sceSifDmaStat(dmaRequest) >= 0) {
    }
    return dmaRequest;
}

/* Zero one EE staging buffer, then seed both halves of the IOP allocation. */
void sndInitializeStreamTransferBuffers(s32 wordCount) {
    s32 bufferBytes = wordCount * SND_TRANSFER_WORD_BYTES;
    s32 halfwordIndex;

    D_00437A28 = sdfMemoryGetBlockAddress(sdfAllocGeneralBlock(bufferBytes));
    for (halfwordIndex = 0; halfwordIndex < wordCount * SND_TRANSFER_WORD_HALFWORDS; halfwordIndex++) {
        ((u16 *)D_00437A28)[halfwordIndex] = 0;
    }
    sceSifInitIopHeap();
    D_00438FD8 = sceSifAllocIopHeap(wordCount * SND_TRANSFER_PAIR_WORD_BYTES);
    if (D_00438FD8 <= 0) {
        Exit(0);
    }
    D_00437A20[0] = D_00438FD8;
    D_00437A20[1] = D_00438FD8 + bufferBytes;
    sndCopyWordsToIopSynchronously(D_00437A28, D_00437A20[0], wordCount);
    sndCopyWordsToIopSynchronously(D_00437A28, D_00437A20[1], wordCount);
}

INCLUDE_ASM(const s32, "game/code_0029DF18", mnuConfigureTitleAudioStream);

void mnuInitTitleSoundRemoteRequest(u32 wordCount) {
    sceSdRemoteInit();
    sndInitializeStreamTransferBuffers(wordCount);
    D_00437A2C = 0;
}

extern s32 func_0034E820(s32, s32, ...);
extern u64 D_00438FD0;

s32 mnuCopySoundStreamOnStatusChange(u32 source, u32 words) {
    u64 status = func_0034E820(1, 0x8100, 0);

    if ((D_00438FD0 & 0x1000000) == (status & 0x1000000)) {
        return 0;
    }
    sndCopyWordsToIopSynchronously(source, D_00437A20[D_00437A2C & 1], words);
    D_00438FD0 = status;
    D_00437A2C ^= 1;
    return 1;
}

