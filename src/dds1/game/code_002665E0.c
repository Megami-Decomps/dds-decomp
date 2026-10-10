#include "common.h"
#include "mnu_result.h"

extern u32 uiBlendColors(u32, u32, s32);


void func_002665E0(s32 a0, s32 a1, s32 a2, BrsSkillPackageWork *work, s32 a4, s32 a5, s32 index) {
    s32 remaining = 0x100 - work->fadeProgress;

    if (work->fadeAnimation[index].backgroundState == 0) {
        u32 opacity = uiBlendColors(0x80808080, 0x80808000, remaining) & 0xFF;

        work->fadeAnimation[index].backgroundOpacity = opacity;
        if (work->fadeAnimation[index].backgroundOpacity >= 0x80) {
            work->fadeAnimation[index].backgroundState = 1;
        }
    }
}
INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFBB0);

INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFBC0);

INCLUDE_ASM(const s32, "game/code_002665E0", func_00266668);

INCLUDE_ASM(const s32, "game/code_002665E0", func_00266908);

void func_00266B10(s32 a0, s32 a1, s32 a2, BrsSkillPackageWork *work, s32 a4, s32 a5, s32 index) {
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
INCLUDE_ASM(const s32, "game/code_002665E0", func_00266BC0);


extern void mnuSetTitleSequenceVolumePan(u32);
extern u32 ptyComputeTotalExp(DatPartyRecord *, s32);
extern s32 func_002687C0(BrsSkillPackageWork *, BrsProgressRow *, s32, s8);

void func_00266E28(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                   BrsProgressRow *entry, s32 context, s32 index) {
    s32 level = work->levelAnimation[index].level;
    s32 fade = 0x100 - work->fadeProgress;
    s32 nextLevel = level + 1;
    s32 currentExp;
    s32 finished;

    nextLevel = nextLevel < 2 ? 1 : (nextLevel > 99 ? 99 : nextLevel);
    switch (work->levelAnimation[index].state) {
    case 0:
        work->levelAnimation[index].alpha = uiBlendColors(0x80808080, 0x80808000, fade) & 0xFF;
        if (work->levelAnimation[index].alpha >= 0x80) {
            work->levelAnimation[index].state = 1;
        }
        break;
    case 1:
        work->levelAnimation[index].state++;
        if (work->levelAnimation[index].remaining > 0 && work->resultPhase == 0) {
            mnuSetTitleSequenceVolumePan(0x14);
            work->resultPhase = 1;
        }
        break;
    case 2:
    case 3:
    case 4:
        if (level < 99 && entry->unit->hp != 0 && (entry->unit->status & 0x4000) == 0) {
            if (work->levelAnimation[index].state == 3) {
                goto flash;
            }
            work->levelAnimation[index].applied = func_002687C0(work, entry, index, 0);
            currentExp = work->levelAnimation[index].currentProgress + work->levelAnimation[index].applied;
            work->levelAnimation[index].currentProgress = currentExp;
            if (currentExp >= (s32)ptyComputeTotalExp(entry->unit, nextLevel - entry->unit->level) && work->levelAnimation[index].state == 2) {
                work->levelAnimation[index].level++;
                work->levelAnimation[index].level = work->levelAnimation[index].level < 2 ? 1 : (work->levelAnimation[index].level > 99 ? 99 : work->levelAnimation[index].level);
                if (work->unkD4D != 0) {
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
                work->levelAnimation[index].iconState = 1;
                work->levelAnimation[index].completionState = 1;
                work->levelAnimation[index].unk2D = 1;
                work->levelAnimation[index].iconOpacity = 0x80;
                work->levelAnimation[index].iconFrame = 0;
                work->levelAnimation[index].alpha = 0;
                work->levelAnimation[index].state++;
                mnuSetTitleSequenceVolumePan(0x12);
            }
        } else {
            work->levelAnimation[index].remaining = 0;
        }
        if (work->levelAnimation[index].state == 3) {
flash:
            work->levelAnimation[index].iconFrame++;
            work->levelAnimation[index].iconFrame = work->levelAnimation[index].iconFrame <= 0 ? 0 : (work->levelAnimation[index].iconFrame > 15 ? 15 : work->levelAnimation[index].iconFrame);
            if (work->levelAnimation[index].iconFrame >= 15) {
                work->levelAnimation[index].state++;
            }
        } else if (work->levelAnimation[index].state == 4) {
            work->levelAnimation[index].iconOpacity -= 16;
            work->levelAnimation[index].iconOpacity = work->levelAnimation[index].iconOpacity <= 0 ? 0 : (work->levelAnimation[index].iconOpacity > 128 ? 128 : work->levelAnimation[index].iconOpacity);
            if (work->levelAnimation[index].iconOpacity <= 0) {
                work->levelAnimation[index].unk2D = 0;
            }
            work->levelAnimation[index].alpha += 8;
            work->levelAnimation[index].alpha = work->levelAnimation[index].alpha <= 0 ? 0 : (work->levelAnimation[index].alpha > 128 ? 128 : work->levelAnimation[index].alpha);
            if (work->levelAnimation[index].alpha >= 128) {
                work->levelAnimation[index].state = 2;
                if (work->levelAnimation[index].remaining > 0 && work->resultPhase == 0) {
                    mnuSetTitleSequenceVolumePan(0x14);
                    work->resultPhase = 1;
                }
            }
        }
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_002665E0", func_002673C8);

extern f32 sdfSinPoly(f32);

void func_00267850(s32 screenX, s32 screenY, s32 depth, BrsSkillPackageWork *work, BrsProgressRow *row, s32 context, s32 index) {
    s32 y;
    s32 bob;

    switch (work->levelAnimation[index].iconState) {
    case 0:
        work->levelAnimation[index].iconColor = 0;
        work->levelAnimation[index].iconPosition[0] = 0xA6;
        work->levelAnimation[index].iconPosition[1] = 0x22;
        work->levelAnimation[index].iconAngle = 0xB4;
        break;
    case 1:
        work->levelAnimation[index].iconAngle = (work->levelAnimation[index].iconAngle + 15) % 360;
        work->levelAnimation[index].iconColor =
            (u32)(0.0f + (sdfSinPoly((f32)((work->levelAnimation[index].iconAngle + 90) % 360) / 180.0f * 3.1415899f) + 1.0f) * 0.5f * 144.0f);
        bob = (s32)(0.0f + (sdfSinPoly((f32)((work->levelAnimation[index].iconAngle + 90) % 360) / 180.0f * 3.1415899f) + 1.0f) * 0.5f * 22.0f);
        work->levelAnimation[index].iconPosition[1] = 0x24 - bob;
        work->levelAnimation[index].iconColor = (work->levelAnimation[index].iconColor > 0) ? ((work->levelAnimation[index].iconColor > 0x80) ? 0x80 : work->levelAnimation[index].iconColor) : 0;
        y = work->levelAnimation[index].iconPosition[1];
        work->levelAnimation[index].iconPosition[1] = (y < 0xF) ? 0xE : ((y > 0x24) ? 0x24 : y);
        if (work->levelAnimation[index].iconColor >= 0x80) {
            work->levelAnimation[index].iconState += 2;
            work->levelAnimation[index].iconAngle = 0x78;
        }
        break;
    case 2:
        work->levelAnimation[index].iconState++;
        work->levelAnimation[index].iconAngle = 0x78;
        break;
    default:
        work->levelAnimation[index].iconColor -= 10;
        work->levelAnimation[index].iconColor = (work->levelAnimation[index].iconColor > 0) ? ((work->levelAnimation[index].iconColor > 0x80) ? 0x80 : work->levelAnimation[index].iconColor) : 0;
        y = work->levelAnimation[index].iconPosition[1] - 1;
        work->levelAnimation[index].iconPosition[1] = y;
        work->levelAnimation[index].iconPosition[1] = (y < 5) ? 4 : ((y > 0xE) ? 0xE : y);
        if (work->levelAnimation[index].iconColor <= 0) {
            work->levelAnimation[index].iconState = 0;
        }
        break;
    }
    switch (work->levelAnimation[index].completionState) {
    case 0:
        work->levelAnimation[index].auxiliaryColor = 0;
        work->levelAnimation[index].auxiliaryPosition[0] = 0x95;
        work->levelAnimation[index].auxiliaryPosition[1] = 0x15;
        break;
    case 1:
        work->levelAnimation[index].auxiliaryColor += 10;
        work->levelAnimation[index].auxiliaryColor = (work->levelAnimation[index].auxiliaryColor > 0) ? ((work->levelAnimation[index].auxiliaryColor > 0x80) ? 0x80 : work->levelAnimation[index].auxiliaryColor) : 0;
        if (work->levelAnimation[index].auxiliaryColor >= 0x80) {
            work->levelAnimation[index].completionState++;
            work->levelAnimation[index].completionColor = 0;
        }
        break;
    case 2:
        work->levelAnimation[index].completionColor++;
        work->levelAnimation[index].completionColor = (work->levelAnimation[index].completionColor > 0) ? ((work->levelAnimation[index].completionColor > 8) ? 8 : work->levelAnimation[index].completionColor) : 0;
        if (work->levelAnimation[index].completionColor >= 8) {
            work->levelAnimation[index].completionState++;
        }
        break;
    default:
        work->levelAnimation[index].auxiliaryColor -= 0x10;
        work->levelAnimation[index].auxiliaryColor = (work->levelAnimation[index].auxiliaryColor > 0) ? ((work->levelAnimation[index].auxiliaryColor > 0x80) ? 0x80 : work->levelAnimation[index].auxiliaryColor) : 0;
        if (work->levelAnimation[index].auxiliaryColor <= 0) {
            work->levelAnimation[index].completionState = 0;
        }
        break;
    }
}


INCLUDE_ASM(const s32, "game/code_002665E0", func_00267E20);

void func_00267FF0(s32 screenX, s32 screenY, s32 depth, BrsSkillPackageWork *work, BrsProgressRow *row, s32 context, s32 index) {
    s32 y;
    s32 bob;

    switch (work->profileAnimation[index].iconState) {
    case 0:
        work->profileAnimation[index].iconColor = 0;
        work->profileAnimation[index].iconPosition[0] = 0x12F;
        work->profileAnimation[index].iconPosition[1] = 0x22;
        work->profileAnimation[index].iconAngle = 0xB4;
        break;
    case 1:
        work->profileAnimation[index].iconAngle = (work->profileAnimation[index].iconAngle + 15) % 360;
        work->profileAnimation[index].iconColor =
            (u32)(0.0f + (sdfSinPoly((f32)((work->profileAnimation[index].iconAngle + 90) % 360) / 180.0f * 3.1415899f) + 1.0f) * 0.5f * 144.0f);
        bob = (s32)(0.0f + (sdfSinPoly((f32)((work->profileAnimation[index].iconAngle + 90) % 360) / 180.0f * 3.1415899f) + 1.0f) * 0.5f * 22.0f);
        work->profileAnimation[index].iconPosition[1] = 0x24 - bob;
        work->profileAnimation[index].iconColor = (work->profileAnimation[index].iconColor > 0) ? ((work->profileAnimation[index].iconColor > 0x80) ? 0x80 : work->profileAnimation[index].iconColor) : 0;
        y = work->profileAnimation[index].iconPosition[1];
        work->profileAnimation[index].iconPosition[1] = (y < 0xF) ? 0xE : ((y > 0x24) ? 0x24 : y);
        if (work->profileAnimation[index].iconColor >= 0x80) {
            work->profileAnimation[index].iconState += 2;
            work->profileAnimation[index].iconAngle = 0x78;
        }
        break;
    case 2:
        work->profileAnimation[index].iconState++;
        work->profileAnimation[index].iconAngle = 0x78;
        break;
    default:
        work->profileAnimation[index].iconColor -= 10;
        work->profileAnimation[index].iconColor = (work->profileAnimation[index].iconColor > 0) ? ((work->profileAnimation[index].iconColor > 0x80) ? 0x80 : work->profileAnimation[index].iconColor) : 0;
        y = work->profileAnimation[index].iconPosition[1] - 1;
        work->profileAnimation[index].iconPosition[1] = y;
        work->profileAnimation[index].iconPosition[1] = (y < 5) ? 4 : ((y > 0xE) ? 0xE : y);
        if (work->profileAnimation[index].iconColor <= 0) {
            work->profileAnimation[index].iconState = 0;
        }
        break;
    }
    switch (work->profileAnimation[index].completionState) {
    case 0:
        work->profileAnimation[index].auxiliaryColor = 0;
        work->profileAnimation[index].auxiliaryPosition[0] = 0x122;
        work->profileAnimation[index].auxiliaryPosition[1] = 0x15;
        break;
    case 1:
        work->profileAnimation[index].auxiliaryColor += 10;
        work->profileAnimation[index].auxiliaryColor = (work->profileAnimation[index].auxiliaryColor > 0)
            ? ((work->profileAnimation[index].auxiliaryColor > 0x80) ? 0x80 : work->profileAnimation[index].auxiliaryColor)
            : 0;
        if (work->profileAnimation[index].auxiliaryColor >= 0x80) {
            work->profileAnimation[index].completionState++;
            work->profileAnimation[index].completionColor = 0;
        }
        break;
    case 2:
        work->profileAnimation[index].completionColor += 10;
        work->profileAnimation[index].completionColor = (work->profileAnimation[index].completionColor > 0)
            ? ((work->profileAnimation[index].completionColor > 0xFF) ? 0xFF : work->profileAnimation[index].completionColor)
            : 0;
        if (work->profileAnimation[index].completionColor >= 0xFF) {
            work->profileAnimation[index].completionState++;
        }
        break;
    default:
        work->profileAnimation[index].completionColor -= 4;
        work->profileAnimation[index].completionColor = (work->profileAnimation[index].completionColor > 0x80)
            ? ((work->profileAnimation[index].completionColor > 0xFF) ? 0xFF : work->profileAnimation[index].completionColor)
            : 0x80;
        break;
    }
}


INCLUDE_ASM(const s32, "game/code_002665E0", func_00268590);

extern u32 ptyComputeTotalExp(DatPartyRecord *, s32);
extern s32 ptyGetCurrentProfileId(DatPartyRecord *);
extern u32 prfGetCapValue(u16);

s32 func_002687C0(BrsSkillPackageWork *work, BrsProgressRow *entry, s32 index, s8 mode) {
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
        if (work->levelAnimation[index].remaining > 0) {
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
            if (work->unkD4D != 0) {
                step = work->levelAnimation[index].remaining;
            }
            work->levelAnimation[index].remaining -= step;
            if (work->levelAnimation[index].remaining < 0) {
                step += work->levelAnimation[index].remaining;
                work->levelAnimation[index].remaining = 0;
            }
            work->levelAnimation[index].remaining = work->levelAnimation[index].remaining <= 0 ? 0 : (work->levelAnimation[index].remaining > 0x1000000 ? 0x1000000 : work->levelAnimation[index].remaining);
            result = step;
        }
    } else {
        range = prfGetCapValue(ptyGetCurrentProfileId(entry->unit));
        if (work->profileAnimation[index].remaining > 0) {
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
            if (work->unkD4D != 0) {
                step = work->profileAnimation[index].remaining;
            }
            work->profileAnimation[index].remaining -= step;
            if (work->profileAnimation[index].remaining < 0) {
                step += work->profileAnimation[index].remaining;
                work->profileAnimation[index].remaining = 0;
            }
            work->profileAnimation[index].remaining = work->profileAnimation[index].remaining <= 0 ? 0 : (work->profileAnimation[index].remaining > 0x1000000 ? 0x1000000 : work->profileAnimation[index].remaining);
            result = step;
        }
    }
    return result;
}

#include "common.h"
#include "mnu_result.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "sdf_resource.h"
#include "kwln.h"
#include "mnu_title_effect.h"
#include "file.h"
#include "file_request_api.h"
#include "sdf_thread.h"

/* The two title-audio states own the complete ten-word frame/decoder record. */
typedef struct TitleAudioStreamState {
    s32 frameCount;
    s32 frameIndex;
    s32 frameBytes;
    s32 repeatFrame;
    s32 control;
    u8 *compressedData;
    s16 *samples;
    u32 *decoder;
    struct SdfMemBlock *allocation;
    s32 loadState;
} TitleAudioStreamState;
typedef char TitleAudioStreamState_size[(sizeof(TitleAudioStreamState) == 0x28) ? 1 : -1];

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

extern u32 dds3AdvanceWorldCounter(void);

extern s32 dds3CreateCameraObject(s32 counter, f32 *position, f32 *rotation);

struct EffWorldNode;
extern void dds3SetWorldNodeValue(struct EffWorldNode *node, u32 value);

extern void effObjSetInnerFloat(s32 object, f32 value);

extern s32 dds3GetWorldSecondaryObject(void);


extern u8 D_003DC1C0[];

extern u8 D_003DC1D0[];

extern void *mnuTitleCameraObject;

extern char D_003AFD48[]; /* "---------- AT3 --------\n", followed by 8 zero bytes no C function emits */

extern TitleAudioStreamState mnuTitleSoundBufferState;

extern u32 D_003DA1C0[];


extern TitleAudioStreamState mnuTitleStreamStatus;

extern u32 mnuTitleStreamSemaphore;

extern u32 D_003BD8D4;

extern u32 D_003BC5BC;

extern s32 D_003BC5C8;

extern u8 D_003D9178[];

extern s32 mnuPollTitleStreamStateLocked(void);

extern u32 sdfSoundIsCommandBusy(void);

typedef struct { u64 v; } __attribute__((packed)) u64p;

extern u8 D_003BC590[];

extern u8 D_003BC598[];

extern char *strcat(char *, char *);


extern KwlnTask *mnuTitleSoundTask;
extern KwlnTask *kwlnTaskCreate();

extern char D_003771D8[];

/* libc sprintf returns the signed vfprintf character count. */
extern s32 func_003014F0(char *dst, const char *fmt, ...);

extern s32 func_003003F0(const char *fmt, ...);

extern void mnuCreateTitleEffectTask(void);

extern u8 D_003BC5A0[];

extern s32 scrReadIntParameter(s32);
extern u32 sdfSoundIsCommandBusy(void);

extern u32 D_003BC5B0[2];

extern u32 D_003BC5B8;





extern s32 sceSifInitIopHeap(void);

extern s32 sceSifAllocIopHeap(s32 size);

extern void Exit(s32 status);

extern s32 D_003BD8C0;

extern char D_003AFBA0[]; /* "result2_draw" */

typedef struct BrsResultCounter {
    u8 pad00[0x14];
    s8 phase;
    u8 pad15[0x1F];
    s32 remaining;
    u8 pad38[0x14];
    s8 unk4C;
    u8 pad4D[0x13];
    s8 unk60;
    u8 pad61[7];
} BrsResultCounter;

typedef struct BrsResultWork {
    u8 pad00[0xD40];
    s32 settledFrames;
    u8 padD44[0x19C];
    BrsResultCounter levelCounters[8];
    BrsResultCounter profileCounters[8];
} BrsResultWork;

extern DatProfileRecord *ptyGetCurrentProfileRecord(DatPartyRecord *);
extern s32 ptyGetCurrentProfileId(DatPartyRecord *);
extern u32 prfGetCapValue(u16);
extern u32 uiBlendColors(u32, u32, s32);
extern s32 func_002687C0(BrsSkillPackageWork *, BrsProgressRow *, s32, s8);
extern void mnuSetTitleSequenceVolumePan(u32);


void func_00268AB8(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                   BrsProgressRow *row, s32 context, s32 index) {
    s32 fade = 0x100 - work->fadeProgress;
    s32 profileId;
    s32 cap;
    s32 currentProgress;
    s32 increment;

    ptyGetCurrentProfileRecord(row->unit);
    profileId = ptyGetCurrentProfileId(row->unit);
    if (profileId != 0) {
        cap = prfGetCapValue((u16)profileId);
    } else {
        cap = 0;
        work->profileAnimation[index].remaining = 0;
    }
    currentProgress = work->profileAnimation[index].currentProgress;
    switch (work->profileAnimation[index].state) {
    case 0:
        work->profileAnimation[index].alpha =
            uiBlendColors(0x80808080, 0x80808000, fade) & 0xFF;
        if (work->profileAnimation[index].alpha >= 0x80) {
            work->profileAnimation[index].state = 1;
        }
        break;
    case 1:
        work->profileAnimation[index].state++;
        if (work->profileAnimation[index].remaining > 0 && work->resultPhase == 0) {
            mnuSetTitleSequenceVolumePan(0x14);
            work->resultPhase = 1;
        }
        break;
    case 2:
    case 3:
    case 4:
        if (work->profileAnimation[index].state == 2) {
            if (cap != 0) {
                if (currentProgress < cap) {
                    increment = func_002687C0(work, row, index, 1);
                    work->profileAnimation[index].applied = increment;
                    work->profileAnimation[index].currentProgress += increment;
                    if (work->profileAnimation[index].currentProgress >= cap) {
                        work->profileAnimation[index].iconState = 1;
                        work->profileAnimation[index].completionState = 1;
                        work->profileAnimation[index].unk2D = 1;
                        work->profileAnimation[index].iconOpacity = 0x80;
                        work->profileAnimation[index].iconFrame = 0;
                        work->profileAnimation[index].state++;
                        mnuSetTitleSequenceVolumePan(0x12);
                    }
                } else {
                    work->profileAnimation[index].remaining = 0;
                }
            }
        }
        break;
    }
}



INCLUDE_ASM(const s32, "game/code_002665E0", func_00268D40);

/* Complete after six successful polls of five counter pairs.
 * Early returns deliberately leave the accumulated settled count unchanged. */
s32 brsPollResultCounterCompletion(void) {
    KwlnTask *resultTask = kwlnTaskGetTaskByName(D_003AFBA0);
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

void func_00269400(void) {
}

void func_00269408(void) {
}

void func_00269410(void) {
}

void func_00269418(void) {
}

/* Copy the packed eight-byte prefix, then append the caller's filename. */
char *mnuBuildSoundResourcePath(char *pathBuffer, char *filename) {
    *(u64p *)pathBuffer = *(u64p *)D_003BC590;
    return strcat(pathBuffer, filename);
}

/* Use the voice prefix with the same packed, unbounded append convention. */
char *mnuBuildVoiceResourcePath(char *pathBuffer, char *filename) {
    *(u64p *)pathBuffer = *(u64p *)D_003BC598;
    return strcat(pathBuffer, filename);
}

extern s8 D_003BC58C;

extern char D_00377318[];

extern char D_00377338[];

extern s32 D_00370D10[];

extern s32 D_003BC5A8;

extern s16 D_003BD8B0;

extern s32 D_003BC5AC;

extern void func_002E8C30(char *, s32, char *, s32);

extern void sndEnsureMidiBankResident(s32);

extern void func_0026A248(void);

extern void mnuCreateTitleEffectTask(void);

void mnuInitializeTitleAudioAndEffects(void) {
    if (D_003BC58C != 0) {
        func_002E8E50();
        return;
    }
    D_003BC58C = 1;
    func_002E8C30(D_00377318, 4, D_00377338, 4);
    sndEnsureMidiBankResident(D_00370D10[0]);
    D_003BC5A8 = 4;
    D_003BD8B0 = -1;
    D_003BC5AC = 0;
    func_0026A248();
    mnuCreateTitleEffectTask();
}

u32 func_002694F8(void) {
    return 0x599;
}

u32 mnuIncrementTitleEffectFrameCounter(KwlnTask *task) {
    TitleEffectState *effectState;

    effectState = (TitleEffectState *)kwlnTaskGetUserValue(task);
    effectState->frameCounter = effectState->frameCounter + 1;
    return 0;
}

void mnuDestroyTitleEffectTask(KwlnTask *task) {
    sdfReleaseChipBlock((void *)kwlnTaskGetUserValue(task));
    mnuTitleSoundTask = 0;
}

/* Allocate the two-word effect state and attach it to the frame-counter task. */
void mnuCreateTitleEffectTask(void) {
    TitleEffectState *effectState = (TitleEffectState *)sdfAllocSizeClassBlock(MNU_TITLE_EFFECT_BYTES);
    KwlnTask *effectTask = kwlnTaskCreate(D_003BC5A0, 0x5214, 1, 1,
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

/* The native char*-typed voice argument is passed to numeric %04d formatting;
 * retain that signature rather than treating it as a filename string. */
INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFBF0);

INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFC30);

INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFC40);

INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFC50);

INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFC60);

INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFC70);

INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFC80);

void mnuPlayTitleVoiceFile(char *voiceArgument) {
    char voicePath[MNU_TITLE_VOICE_PATH_BYTES];
    TitleEffectState *effectState = (TitleEffectState *)kwlnTaskGetUserValue(mnuTitleSoundTask);

    if (sdfSoundIsCommandBusy() != 0) {
        func_003003F0("now playeng start...\n");
        sdfSoundStopNamedPlayback();
    }
    func_003014F0(voicePath, "%s%04d.ADB", D_003771D8 + MNU_TITLE_VOICE_PREFIX_BYTES * effectState->soundNameIndex, voiceArgument);
    func_003003F0("--------------- VOICE -> %s\n", voicePath);
    sdfSoundSendNamedCommand(voicePath, 0x64);
    effectState->frameCounter = 0;
}

void mnuStopTitleVoicePlayback(void) {
    sdfSoundStopNamedPlayback();
}

s32 mnuQueryTitleSoundBusy(void) {
    return sdfSoundIsCommandBusy();
}

void func_00269728(void) {
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

u32 func_002697B0(void) {
    func_002E8E50();
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

extern void scrSetIntegerReturnValue(s32 value);

u32 sndOpPushCommandBusyState(void) {
    u32 soundBusy;

    soundBusy = sdfSoundIsCommandBusy();
    scrSetIntegerReturnValue(soundBusy);
    return 1;
}

u32 func_00269868(void) {
    return 1;
}

s32 movCheckStartupSoundState(void) {
    if (mnuPollTitleStreamStateLocked() == 0) {
        func_0026A5F0(scrReadIntParameter(0));
        return 0;
    }
    return mnuPollTitleStreamStateLocked() != 1;
}

u32 func_002698C0(void) {
    mnuTitleStreamUpdateAndLogBgm();
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

    D_003BC5B8 = sdfMemoryGetBlockAddress(sdfAllocGeneralBlock(bufferBytes));
    for (halfwordIndex = 0; halfwordIndex < wordCount * SND_TRANSFER_WORD_HALFWORDS; halfwordIndex++) {
        ((u16 *)D_003BC5B8)[halfwordIndex] = 0;
    }
    sceSifInitIopHeap();
    D_003BD8C0 = sceSifAllocIopHeap(wordCount * SND_TRANSFER_PAIR_WORD_BYTES);
    if (D_003BD8C0 <= 0) {
        Exit(0);
    }
    D_003BC5B0[0] = D_003BD8C0;
    D_003BC5B0[1] = D_003BD8C0 + bufferBytes;
    sndCopyWordsToIopSynchronously(D_003BC5B8, D_003BC5B0[0], wordCount);
    sndCopyWordsToIopSynchronously(D_003BC5B8, D_003BC5B0[1], wordCount);
}

INCLUDE_ASM(const s32, "game/code_002665E0", func_00269A68);

void mnuInitTitleSoundRemoteRequest(u32 wordCount) {
    sceSdRemoteInit();
    sndInitializeStreamTransferBuffers(wordCount);
    D_003BC5BC = 0;
}

extern u64 D_003BD8B8;
extern s32 func_002F5990(s32, s32, ...);

s32 func_00269B80(u32 source, u32 words) {
    u64 status = func_002F5990(1, 0x8100, 0);

    if ((D_003BD8B8 & 0x1000000) == (status & 0x1000000)) {
        return 0;
    }
    sndCopyWordsToIopSynchronously(source, D_003BC5B0[D_003BC5BC & 1], words);
    D_003BD8B8 = status;
    D_003BC5BC ^= 1;
    return 1;
}

/* The argument is a word count, not a source address; both use the EE buffer. */
void sndUploadStreamToBothIopBuffers(u32 wordCount) {
    sndCopyWordsToIopSynchronously(D_003BC5B8, D_003BC5B0[0], wordCount);
    sndCopyWordsToIopSynchronously(D_003BC5B8, D_003BC5B0[1], wordCount);
    func_002F5990(1, 0x8010, 0xf80, 0);
    func_002F5990(1, 0x8010, 0x1080, 0);
    func_002F5990(1, 0x80e0, 0, 2, 0, 0);
}

/* Copy the second source, add the first, and clamp to [-32767, 32767].
 * The native lower limit intentionally excludes the s16 value -32768. */
void sndMixSampleBuffers(s16 *destinationSamples, TitleAudioStreamState *firstSource, TitleAudioStreamState *secondSource) {
    s16 *destinationCursor = destinationSamples;
    s16 *sourceCursor = secondSource->samples;
    s32 sampleIndex;

    for (sampleIndex = 0; sampleIndex < SND_MIX_SAMPLE_COUNT; sampleIndex++) {
        *destinationCursor++ = *sourceCursor++;
    }
    sourceCursor = firstSource->samples;
    destinationCursor -= SND_MIX_SAMPLE_COUNT;
    for (sampleIndex = 0; sampleIndex < SND_MIX_SAMPLE_COUNT; sampleIndex++) {
        s32 mixedSample = *destinationCursor + *sourceCursor++;

        if (mixedSample > SND_MIX_SAMPLE_LIMIT) {
            mixedSample = SND_MIX_SAMPLE_LIMIT;
        }
        if (mixedSample < -SND_MIX_SAMPLE_LIMIT) {
            mixedSample = -SND_MIX_SAMPLE_LIMIT;
        }
        *destinationCursor++ = mixedSample;
    }
}

INCLUDE_ASM(const s32, "game/code_002665E0", func_00269D18);

void mnuRunTitleStreamThread(void) {
    for (;;) {
        sdfSleepThreadCount(1);
        WaitSema(mnuTitleStreamSemaphore);
        func_00269D18();
        SignalSema(mnuTitleStreamSemaphore);
    }
}

extern u32 mnuTitleStreamSemaphore;

extern SdfThreadNode mnuTitleStreamThread __attribute__((section(".sbss")));

extern u8 mnuTitleStreamThreadStack[];

void mnuCreateTitleStreamThread(void) {
    mnuTitleStreamSemaphore = sdfCreateSemaphore(1, 0xff, 0);
    sdfStartTrackedThread(&mnuTitleStreamThread, mnuRunTitleStreamThread, mnuTitleStreamThreadStack,
                  0x1000, 0x45, 0);
    sdfThreadSleepSelf();
}

INCLUDE_ASM(const s32, "game/code_002665E0", func_0026A248);

typedef struct AtracInfo {
    u32 unk0;
    u32 unk4;
    u32 unk8;
} AtracInfo;

extern s32 WaitSema(u32);

extern s32 SignalSema(u32);

/* Read state words 0, 1 and 3; the frame-byte word 2 is not in this snapshot. */
void mnuReadTitleStreamStatusLocked(AtracInfo *statusSnapshot) {
    WaitSema(mnuTitleStreamSemaphore);
    statusSnapshot->unk0 = mnuTitleStreamStatus.frameCount;
    statusSnapshot->unk4 = mnuTitleStreamStatus.frameIndex;
    statusSnapshot->unk8 = mnuTitleStreamStatus.repeatFrame;
    SignalSema(mnuTitleStreamSemaphore);
}

extern u32 mnuTitleStreamSemaphore;


/* Restore the same condensed three-word status mapping under the lock. */
void mnuWriteTitleStreamStatusLocked(u32 *statusValues) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleStreamStatus.frameCount = statusValues[0];
    mnuTitleStreamStatus.frameIndex = statusValues[1];
    mnuTitleStreamStatus.repeatFrame = statusValues[2];
    SignalSema(mnuTitleStreamSemaphore);
}

/* Copy into the state's installed buffer, count complete compressed
 * frames using its frame-byte divisor, then release the loaded resource. */

void mnuLoadTitleStreamFrameData(char *filePath, TitleAudioStreamState *streamState) {
    void *fileData;
    s32 frameCount;
    struct SdfMemBlock *resourceHandle = sdfReadNamedResource(filePath, (u32 *)&fileData, 0);
    s32 fileBytes = sdfMemoryGetBlockSize(resourceHandle);
    memcpy(streamState->compressedData, fileData, fileBytes);
    frameCount = fileBytes / streamState->frameBytes;
    streamState->frameIndex = 0;
    streamState->frameCount = frameCount;
    sdfQueueGeneralAllocationRelease(resourceHandle);
}

void mnuStoreTaskResult(char *audioPath) {
    D_003BD8D4 = (u32)fileQueueDefaultCallbackRequest(audioPath);
    mnuTitleStreamStatus.loadState = 1;
}

extern u32 D_003D9168[];







extern void func_002F7628(u32 *);

/* Allocate in global status, but use the supplied state's copy destination
 * and frame counts. Keep those distinct accesses and the cleanup-before-copy
 * ordering; return 1 after a ready file is copied, otherwise its ready result. */
s32 mnuCompleteTitleStreamFileLoad(TitleAudioStreamState *destinationState) {
    s32 ready = fileIsRequestReadyInCurrentMode((struct FileRequest *)D_003BD8D4);

    if (ready != 0) {
        s32 resourceHandle = (s32)fileGetResourceHandle((struct FileRequest *)(u32)D_003BD8D4);
        u32 fileDataAddress = fileGetLoadedDataAddress((struct FileRequest *)(u32)D_003BD8D4);
        s32 fileBytes = (s32)fileGetResourceSize((struct FileRequest *)(u32)D_003BD8D4);
        struct SdfMemBlock *allocation;

        filePollEntryCleanup((struct FileRequest *)(u32)D_003BD8D4);
        allocation = sdfAllocGeneralBlockHigh(fileBytes);
        mnuTitleStreamStatus.compressedData = (u8 *)sdfMemoryGetBlockAddress(allocation);
        mnuTitleStreamStatus.allocation = allocation;
        memcpy(destinationState->compressedData, (void *)fileDataAddress, fileBytes);
        destinationState->frameCount = fileBytes / destinationState->frameBytes;
        destinationState->frameIndex = 0;
        sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)resourceHandle);
        func_002F7628(D_003D9168);
        mnuTitleStreamStatus.loadState = MNU_STREAM_LOAD_COPIED;
        ready = 1;
    }
    return ready;
}

/* Install the fixed title track and queue its compressed frames under the lock. */
void func_0026A588(void) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleStreamStatus.decoder = D_003D9168;
    mnuTitleStreamStatus.samples = (s16 *)D_003DA1C0;
    mnuTitleStreamStatus.control = 2;
    mnuTitleStreamStatus.repeatFrame = 0;
    mnuTitleStreamStatus.frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
    D_003D9168[2] = 2;
    mnuStoreTaskResult("/soundat3/b_bgm2-2.at3");
    SignalSema(mnuTitleStreamSemaphore);
}

typedef struct MnuTitleStreamEntry {
    u8 format;
    u8 pad;
    s16 parameter;
    char filename[12];
} MnuTitleStreamEntry;

extern MnuTitleStreamEntry D_00377650[];

extern MnuTitleStreamEntry D_00377350[];

extern char D_003AFCF0[];


/* Load the named sound stream for the requested entry format under the lock. */
void func_0026A5F0(s32 soundEntryIndex) {
    char soundPath[MNU_TITLE_SOUND_PATH_BYTES];

    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleStreamStatus.decoder = D_003D9168;
    mnuTitleStreamStatus.samples = (s16 *)D_003DA1C0;
    mnuTitleStreamStatus.repeatFrame = D_00377350[soundEntryIndex].parameter;
    mnuTitleStreamStatus.control = 2;
    switch (D_00377350[soundEntryIndex].format) {
    case 1:
        D_003D9168[2] = D_00377350[soundEntryIndex].format;
        mnuTitleStreamStatus.frameBytes = MNU_SOUND_FRAME_BYTES_LARGE;
        break;
    case 2:
        D_003D9168[2] = D_00377350[soundEntryIndex].format;
        mnuTitleStreamStatus.frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
        break;
    case 3:
        D_003D9168[2] = D_00377350[soundEntryIndex].format;
        mnuTitleStreamStatus.frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
        break;
    case 4:
        D_003D9168[2] = D_00377350[soundEntryIndex].format;
        mnuTitleStreamStatus.frameBytes = MNU_SOUND_FRAME_BYTES_SMALL;
        break;
    }
    func_003014F0(soundPath, D_003AFCF0, D_00377350[soundEntryIndex].filename);
    mnuStoreTaskResult(soundPath);
    SignalSema(mnuTitleStreamSemaphore);
}

/* Poll a pending file without acquiring the semaphore in this entry point. */
u32 mnuUpdateTitleTransition(void) {
    if (mnuTitleStreamStatus.loadState == MNU_STREAM_LOAD_PENDING) {
        mnuCompleteTitleStreamFileLoad(&mnuTitleStreamStatus);
    }
    return mnuTitleStreamStatus.loadState;
}

/* Poll under the semaphore, but retain the native load-state read after unlock. */
s32 mnuPollTitleStreamStateLocked(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuTitleStreamStatus.loadState == MNU_STREAM_LOAD_PENDING) {
        mnuCompleteTitleStreamFileLoad(&mnuTitleStreamStatus);
    }
    SignalSema(mnuTitleStreamSemaphore);
    return mnuTitleStreamStatus.loadState;
}

extern void fileWaitIdle(void);

/* Wait/copy a pending file while locked, then prepare a commit for controls
 * other than 1. DDS1 also prints its retail BGM banner before unlocking. */
INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFCF0);

void mnuTitleStreamUpdateAndLogBgm(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuUpdateTitleTransition() == MNU_STREAM_LOAD_PENDING) {
        fileWaitIdle();
        mnuCompleteTitleStreamFileLoad(&mnuTitleStreamStatus);
    }
    if (mnuTitleStreamStatus.control != 1) {
        mnuTitleStreamStatus.control = 0;
        mnuTitleStreamStatus.loadState = MNU_STREAM_COMMIT_READY;
        *(u32 *)D_003D9178 = 0;
    }
    func_003003F0("----------- AT3 BGM Play ------------\n");
    SignalSema(mnuTitleStreamSemaphore);
}

void mnuMarkTitleStreamResetPending(void) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleStreamStatus.frameIndex = 0;
    mnuTitleStreamStatus.control = 2;
    SignalSema(mnuTitleStreamSemaphore);
}

/* Commit the ready load only for control 1, and select the native value 6. */
void mnuAdvanceTitleStateUnderSemaphore(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuTitleStreamStatus.control == 1 && mnuTitleStreamStatus.loadState == MNU_STREAM_COMMIT_READY) {
        mnuTitleStreamStatus.loadState = MNU_STREAM_COMMIT_COMPLETE;
        *(u32 *)D_003D9178 = 0;
        D_003BC5C8 = 6;
    }
    SignalSema(mnuTitleStreamSemaphore);
}


/* Save the pre-transition load-state read before marking the commit complete. */
void mnuCommitTitleStreamReadyState(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuTitleStreamStatus.control == 1 && mnuTitleStreamStatus.loadState == MNU_STREAM_COMMIT_READY) {
        D_003BC5C8 = mnuTitleStreamStatus.loadState;
        mnuTitleStreamStatus.loadState = MNU_STREAM_COMMIT_COMPLETE;
        *(u32 *)D_003D9178 = 0;
    }
    SignalSema(mnuTitleStreamSemaphore);
}

/* Reset load/data slots only when an allocation is present; otherwise do nothing. */
void mnuResetTitleStream(void) {
    if (mnuTitleStreamStatus.allocation != 0) {
        sdfQueueGeneralAllocationRelease(mnuTitleStreamStatus.allocation);
        mnuTitleStreamStatus.loadState = MNU_STREAM_LOAD_IDLE;
        mnuTitleStreamStatus.allocation = 0;
        mnuTitleStreamStatus.compressedData = 0;
        mnuTitleStreamStatus.samples = (s16 *)D_003DA1C0;
    }
}

void mnuResetTitleStreamLocked(void) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuResetTitleStream();
    SignalSema(mnuTitleStreamSemaphore);
}

extern u32 D_003DA1A8[];

/* Install the default medium-frame buffer and stream under the shared lock. */
void mnuInitializeTitleSoundBuffer(void) {
    TitleAudioStreamState *streamState = &mnuTitleSoundBufferState;
    u32 *decoder = D_003DA1A8;
    struct SdfMemBlock *allocation;
    s32 bufferAddress;

    WaitSema(mnuTitleStreamSemaphore);
    streamState->decoder = decoder;
    allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_MEDIUM);
    bufferAddress = sdfMemoryGetBlockAddress(allocation);
    streamState->allocation = allocation;
    streamState->control = 2;
    streamState->decoder[2] = 2;
    streamState->compressedData = (u8 *)bufferAddress;
    streamState->samples = (s16 *)D_003DA1C0;
    streamState->frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
    mnuLoadTitleStreamFrameData("/soundat3/se01-2.at3", streamState);
    func_002F7628(decoder);
    SignalSema(mnuTitleStreamSemaphore);
}

extern char D_003AFCF0[];

/* Each format reserves 600 compressed frames before loading its named stream. */
void func_0026AA28(s32 soundEntryIndex) {
    struct SdfMemBlock *allocation = NULL;
    s32 bufferAddress;
    char soundPath[MNU_TITLE_SOUND_PATH_BYTES];

    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleSoundBufferState.samples = (s16 *)D_003DA1C0;
    mnuTitleSoundBufferState.repeatFrame = D_00377650[soundEntryIndex].parameter;
    mnuTitleSoundBufferState.decoder = D_003DA1A8;
    mnuTitleSoundBufferState.control = 2;
    switch (D_00377650[soundEntryIndex].format) {
    case 1:
        D_003DA1A8[2] = 1;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_LARGE;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_LARGE);
        break;
    case 2:
        D_003DA1A8[2] = 2;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_MEDIUM);
        break;
    case 3:
        D_003DA1A8[2] = 3;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_MEDIUM);
        break;
    case 4:
        D_003DA1A8[2] = 4;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_SMALL;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_SMALL);
        break;
    }
    bufferAddress = sdfMemoryGetBlockAddress(allocation);
    mnuTitleSoundBufferState.allocation = allocation;
    mnuTitleSoundBufferState.compressedData = (u8 *)bufferAddress;
    func_003014F0(soundPath, D_003AFCF0, D_00377650[soundEntryIndex].filename);
    mnuLoadTitleStreamFrameData(soundPath, &mnuTitleSoundBufferState);
    func_002F7628(D_003DA1A8);
    SignalSema(mnuTitleStreamSemaphore);
}

/* Install an in-memory ATRAC stream and configure the decoder for its frame
 * format while holding the shared sound-buffer semaphore. The native code
 * has no default-format guard or buffer-capacity check. */
void func_0026ABA8(void *compressedData, s32 dataBytes, s32 format) {
    struct SdfMemBlock *allocation = NULL;
    s32 bufferAddress;
    s32 frameCount;

    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleSoundBufferState.samples = (s16 *)D_003DA1C0;
    mnuTitleSoundBufferState.decoder = D_003DA1A8;
    mnuTitleSoundBufferState.control = 2;
    mnuTitleSoundBufferState.repeatFrame = -1;
    switch (format) {
    case 1:
        D_003DA1A8[2] = format;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_LARGE;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_LARGE);
        break;
    case 2:
        D_003DA1A8[2] = format;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_MEDIUM);
        break;
    case 3:
        D_003DA1A8[2] = format;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_MEDIUM);
        break;
    case 4:
        D_003DA1A8[2] = format;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_SMALL;
        allocation = sdfAllocGeneralBlock(MNU_SOUND_BUFFER_BYTES_SMALL);
        break;
    }
    bufferAddress = sdfMemoryGetBlockAddress(allocation);
    mnuTitleSoundBufferState.allocation = allocation;
    mnuTitleSoundBufferState.compressedData = (u8 *)bufferAddress;
    memcpy((void *)bufferAddress, compressedData, dataBytes);
    frameCount = dataBytes / mnuTitleSoundBufferState.frameBytes;
    mnuTitleSoundBufferState.frameIndex = 0;
    mnuTitleSoundBufferState.frameCount = frameCount;
    func_002F7628(D_003DA1A8);
    SignalSema(mnuTitleStreamSemaphore);
}

/* These return codes are not load-state codes: 0 means no allocation,
 * 2 means control 2, and 3 covers the remaining allocated states. */
s32 mnuGetSoundBufferStateLocked(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuTitleSoundBufferState.allocation == 0) {
        SignalSema(mnuTitleStreamSemaphore);
        return 0;
    }
    if (mnuTitleSoundBufferState.control == 2) {
        SignalSema(mnuTitleStreamSemaphore);
        return 2;
    }
    SignalSema(mnuTitleStreamSemaphore);
    return 3;
}

void mnuPrintTitleDebugBanner(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuTitleSoundBufferState.control != 1) {
        mnuTitleSoundBufferState.control = 0;
    }
    func_003003F0(D_003AFD48);
    SignalSema(mnuTitleStreamSemaphore);
}

void mnuResetSoundBuffer(void) {
    mnuTitleSoundBufferState.frameIndex = 0;
    mnuTitleSoundBufferState.control = 2;
    mnuReleaseSoundBuffer();
}

/* Release the allocation handle, not the data address, then clear its slot. */
void mnuReleaseSoundBuffer(void) {
    TitleAudioStreamState *streamState = &mnuTitleSoundBufferState;
    struct SdfMemBlock *allocationHandle = streamState->allocation;

    if (allocationHandle == 0) {
        return;
    }
    sdfReleaseResourceAllocation(allocationHandle);
    streamState->allocation = 0;
}

void mnuResetSoundBufferLocked(void) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuResetSoundBuffer();
    SignalSema(mnuTitleStreamSemaphore);
}

void mnuReleaseSoundBufferLocked(void) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuReleaseSoundBuffer();
    SignalSema(mnuTitleStreamSemaphore);
}

void func_0026AEB0(void) {
    func_0026BFC8();
}

INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFD48);

void mnuCreateTitleCameraWorldEntry(void) {
    mnuTitleCameraObject = dds3CreateCameraObject(dds3AdvanceWorldCounter(), (f32 *)D_003DC1C0, (f32 *)D_003DC1D0);
    dds3SetWorldNodeValue(mnuTitleCameraObject, (u32)"title_camera");
    effObjSetInnerFloat(mnuTitleCameraObject, 2.0f);
    dds3SetWorldCameraObject(dds3GetWorldSecondaryObject(), mnuTitleCameraObject);
}
INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC570);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC578);

INCLUDE_SDATA(const s32, "game/code_002665E0", mnuTitleSoundTask);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC58C);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC590);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC598);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC5A0);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC5A8);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC5AC);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC5B0);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC5B8);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC5BC);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC5C0);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC5C4);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC5C8);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC5CC);

