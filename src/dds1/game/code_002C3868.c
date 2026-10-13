#include "common.h"
#include "snd_sequence_volume_pan.h"
#include "sdf_vector_latch.h"
#include "sdf_counter_channel.h"
#include "sdf_counter_runtime.h"
#include "dds3obj.h"
#include "sdf_textured_rect.h"
#include "sdf_chip.h"
#include "eff_transform.h"
#include "gs_packet.h"
#include "sdf.h"
#include "mnu_list.h"
#include "fld_map_resource.h"
#include "mdl_flag_api.h"

extern void sdfCounterTickCountdown(void);

extern void mnuTickMapTimers(void);

extern void sdfCounterTickPositionTransition(void);

extern s32 sdfUpdateSelectedCounterMapCameraTransition(void);

extern s32 sdfCounterGetDisplayValue(void);

typedef struct SdfCounterTimer {
    s32 value;         /* 0x00 */
    s16 countdown;     /* 0x04 */
    s16 mode;          /* 0x06 */
    s16 pad08[2];
    s16 mapTimerPrimary;   /* 0x0C */
    s16 mapTimerSecondary; /* 0x0E */
    s32 y;             /* 0x10 */
    s16 startX;        /* 0x14 */
    s16 startY;        /* 0x16 */
    s16 targetX;       /* 0x18 */
    s16 targetY;       /* 0x1A */
    s16 curX;          /* 0x1C */
    s16 curY;          /* 0x1E */
    s32 frames;        /* 0x20 */
} SdfCounterTimer;


typedef struct SdfCounterRuntime {
    u8 pad00[0xC];                  /* title-specific/unknown prefix */
    SdfCounterRuntimeState state;   /* 0x0C */
} SdfCounterRuntime;
typedef char SdfCounterRuntimeState_at_0x0C[
    ((u32)&((SdfCounterRuntime *)0)->state == 0x0C) ? 1 : -1
];



extern s32 sdfActiveCounterRuntime;

extern u32 sdfSelectedCounterIndex;

extern s8 D_003BD270;

extern s32 sdfCounterSelectionCount;

extern void sdfLatchBaseVectorsForSelection(void);

extern s32 fldLocalMapCameraObject;

typedef struct {
    f32 v[4];
} SdfCameraVector;

extern SdfCameraVector fldLocalMapFirstCameraVector;
extern SdfCameraVector fldLocalMapSecondCameraVector;
extern SdfCameraVector D_003900C0;
extern SdfCameraVector D_003900D0;
extern f32 D_00390010[][4];
extern f32 D_00390050[][4];
extern SdfCameraVector D_003900E0[];
extern SdfCameraVector D_00390180[];
extern u32 fldGetLmapStage(void);
extern s8 D_003BD271;
extern void sdfQuatSlerp(f32 *, f32 *, f32 *, f32);
extern void sdfQuaternionNormalize(f32 *);

extern void sdfCounterDestroyRuntime(SdfCounterRuntime *);

extern void evtSubmitPrimaryGsTest(s32, s32, s32, s32, s32, s32, s32, s32);

extern void evtSubmitPrimaryAlphaBlendMode(s32);


extern void evtSubmitDefaultDepthGradientRect(s32, s32, s32, s32, s32, s32, s32, s32);

extern s32 sdfCounterMeasureLabelWidth(u32);

extern void evtPrepareSizedDrawResource(s32 width, s32 height, s32 colors,
                                        const char *text);

void sdfInitInnerVectors(void) {
    effObjSetInnerPosition((EffWorldNode *)fldLocalMapCameraObject, (u128 *)&fldLocalMapFirstCameraVector);
    effObjSetInnerRotation((EffWorldNode *)fldLocalMapCameraObject, (u128 *)&fldLocalMapSecondCameraVector);
    ((EffWorldNode *)fldLocalMapCameraObject)->ops->update(fldLocalMapCameraObject);
}

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C38B0);

void sdfInitializeMapCounterSelection(s32 index, s32 count) {
    fldLocalMapFirstCameraVector = D_003900E0[index];
    fldLocalMapSecondCameraVector = D_00390180[index];
    D_003900C0 = fldLocalMapFirstCameraVector;
    D_003900D0 = fldLocalMapSecondCameraVector;
    sdfCounterSelectionCount = count;
    sdfSelectedCounterIndex = index;
    D_003BD270 = 0;
    D_003BD271 = fldGetLmapStage();

    /* DDS1 refreshes the live pose components after the stage query. */
    fldLocalMapFirstCameraVector.v[0] = D_003900E0[index].v[0];
    fldLocalMapFirstCameraVector.v[1] = D_003900E0[index].v[1];
    fldLocalMapFirstCameraVector.v[2] = D_003900E0[index].v[2];
    fldLocalMapFirstCameraVector.v[3] = D_003900E0[index].v[3];
    fldLocalMapSecondCameraVector.v[0] = D_00390180[index].v[0];
    fldLocalMapSecondCameraVector.v[1] = D_00390180[index].v[1];
    fldLocalMapSecondCameraVector.v[2] = D_00390180[index].v[2];
    fldLocalMapSecondCameraVector.v[3] = D_00390180[index].v[3];
}


/* Latch the base vectors into the pending pair and flag the change. */
void sdfLatchBaseVectorsForSelection(void) {
    D_003BD270 = 1;
    D_003900C0 = fldLocalMapFirstCameraVector;
    D_003900D0 = fldLocalMapSecondCameraVector;
    sdfCommitPendingVectorAndMarkChanged();
}

/* Reset the current selection before installing the requested index. */
void sdfSetSelectedIndex(u32 index) {
    sdfLatchBaseVectorsForSelection();
    sdfSelectedCounterIndex = index;
}

void sdfCycleForward(void) {
    if ((s32)sdfSelectedCounterIndex < sdfCounterSelectionCount - 1) {
        sdfLatchBaseVectorsForSelection();
        sdfSelectedCounterIndex = sdfSelectedCounterIndex + 1;
    } else {
        sdfLatchBaseVectorsForSelection();
        sdfSelectedCounterIndex = 0;
    }
}

/* Cycle through the same bounded selection in the opposite direction. */
void sdfCycleBackward(void) {
    if (sdfSelectedCounterIndex != 0) {
        sdfLatchBaseVectorsForSelection();
        sdfSelectedCounterIndex = sdfSelectedCounterIndex - 1;
    } else {
        sdfLatchBaseVectorsForSelection();
        sdfSelectedCounterIndex = sdfCounterSelectionCount - 1;
    }
}

s8 sdfGetMapCameraTransitionFrame(void) {
    return D_003BD270;
}

void func_002C3D90(void) {
    sdfUpdateSelectedCounterMapCameraTransition();
}

s32 sdfUpdateSelectedCounterMapCameraTransition(void) {
    f32 t;

    if (D_003BD271 == 0) {
        return 1;
    }
    if (D_003BD270 != 0) {
        t = (45 - D_003BD270) / 45.0f;
        t = 1.0f - t * t;
        if (t > 1.0f) {
            t = 1.0f;
        }
        fldLocalMapFirstCameraVector.v[0] = D_003900C0.v[0] * (1.0f - t) +
            D_003900E0[sdfSelectedCounterIndex].v[0] * t;
        fldLocalMapFirstCameraVector.v[1] = D_003900C0.v[1] * (1.0f - t) +
            D_003900E0[sdfSelectedCounterIndex].v[1] * t;
        fldLocalMapFirstCameraVector.v[2] = D_003900C0.v[2] * (1.0f - t) +
            D_003900E0[sdfSelectedCounterIndex].v[2] * t;
        fldLocalMapFirstCameraVector.v[3] = 1.0f;
        sdfQuatSlerp(fldLocalMapSecondCameraVector.v, D_003900D0.v,
            D_00390180[sdfSelectedCounterIndex].v, t);
        if ((s8)(D_003BD270 + 1) >= 45) {
            D_003BD270 = 0;
        } else {
            D_003BD270++;
        }
    } else {
        fldLocalMapFirstCameraVector.v[0] =
            D_003900E0[sdfSelectedCounterIndex].v[0];
        fldLocalMapFirstCameraVector.v[1] =
            D_003900E0[sdfSelectedCounterIndex].v[1];
        fldLocalMapFirstCameraVector.v[2] =
            D_003900E0[sdfSelectedCounterIndex].v[2];
        fldLocalMapFirstCameraVector.v[3] =
            D_003900E0[sdfSelectedCounterIndex].v[3];
        fldLocalMapSecondCameraVector.v[0] =
            D_00390180[sdfSelectedCounterIndex].v[0];
        fldLocalMapSecondCameraVector.v[1] =
            D_00390180[sdfSelectedCounterIndex].v[1];
        fldLocalMapSecondCameraVector.v[2] =
            D_00390180[sdfSelectedCounterIndex].v[2];
        fldLocalMapSecondCameraVector.v[3] =
            D_00390180[sdfSelectedCounterIndex].v[3];
    }
    return 1;
}

s32 sdfStepSelectedMapCameraTransition(void) {
    f32 t;
    s32 index = D_003BD271;

    if (D_003BD270 != 0) {
        t = (45 - D_003BD270) / 45.0f;
        t = 1.0f - t * t;
        if (t > 1.0f) t = 1.0f;
        fldLocalMapFirstCameraVector.v[0] = D_003900C0.v[0] * (1.0f - t) + D_00390010[index][0] * t;
        fldLocalMapFirstCameraVector.v[1] = D_003900C0.v[1] * (1.0f - t) + D_00390010[index][1] * t;
        fldLocalMapFirstCameraVector.v[2] = D_003900C0.v[2] * (1.0f - t) + D_00390010[index][2] * t;
        fldLocalMapFirstCameraVector.v[3] = 1.0f;
        sdfQuatSlerp(fldLocalMapSecondCameraVector.v, D_003900D0.v, D_00390050[index], t);
        if ((s8)(D_003BD270 + 1) >= 45) D_003BD270 = 0;
        else D_003BD270++;
    } else {
        fldLocalMapFirstCameraVector.v[0] = D_00390010[index][0];
        fldLocalMapFirstCameraVector.v[1] = D_00390010[index][1];
        fldLocalMapFirstCameraVector.v[2] = D_00390010[index][2];
        fldLocalMapFirstCameraVector.v[3] = D_00390010[index][3];
        sdfQuaternionNormalize(D_00390050[index]);
        fldLocalMapSecondCameraVector.v[0] = D_00390050[index][0];
        fldLocalMapSecondCameraVector.v[1] = D_00390050[index][1];
        fldLocalMapSecondCameraVector.v[2] = D_00390050[index][2];
        fldLocalMapSecondCameraVector.v[3] = D_00390050[index][3];
    }
    return 1;
}

/* Step the local-map camera transition toward fixed preset 3. */
s32 fldAdvanceLocalMapCameraTransition(void) {
    f32 t;
    s32 index = 3;

    if (D_003BD270 != 0) {
        t = (45 - D_003BD270) / 45.0f;
        t = 1.0f - t * t;
        if (t > 1.0f) t = 1.0f;
        fldLocalMapFirstCameraVector.v[0] = D_003900C0.v[0] * (1.0f - t) + D_00390010[index][0] * t;
        fldLocalMapFirstCameraVector.v[1] = D_003900C0.v[1] * (1.0f - t) + D_00390010[index][1] * t;
        fldLocalMapFirstCameraVector.v[2] = D_003900C0.v[2] * (1.0f - t) + D_00390010[index][2] * t;
        fldLocalMapFirstCameraVector.v[3] = 1.0f;
        sdfQuatSlerp(fldLocalMapSecondCameraVector.v, D_003900D0.v, D_00390050[index], t);
        if ((s8)(D_003BD270 + 1) >= 45) D_003BD270 = 0;
        else D_003BD270++;
    } else {
        fldLocalMapFirstCameraVector.v[0] = D_00390010[index][0];
        fldLocalMapFirstCameraVector.v[1] = D_00390010[index][1];
        fldLocalMapFirstCameraVector.v[2] = D_00390010[index][2];
        fldLocalMapFirstCameraVector.v[3] = D_00390010[index][3];
        sdfQuaternionNormalize(D_00390050[index]);
        fldLocalMapSecondCameraVector.v[0] = D_00390050[index][0];
        fldLocalMapSecondCameraVector.v[1] = D_00390050[index][1];
        fldLocalMapSecondCameraVector.v[2] = D_00390050[index][2];
        fldLocalMapSecondCameraVector.v[3] = D_00390050[index][3];
    }
    return 1;
}

extern s32 mnuCreateListState(s32, s32, s32);
extern void *memset(void *, s32, u32);
extern s32 mdlCollectFlagBitsIntoMask(void);
extern u8 *sdfResolveSceneCounterInfo(s32);
extern void sdfCounterSelectChannelByIndex(SdfCounterRuntime *, s32);
extern void sdfDrawCounterMapSelectionRow(s32, s32, s32, SdfCounterRuntime *, SdfCounterChannel *, s32);
extern u8 D_00390220[];

s32 sdfCreateMaskedCounterChannels(s32 mask, s32 index) {
    SdfCounterDisplay *display;
    SdfCounterChannel *channel;
    s32 completedMask;
    s32 count;
    s32 i;

    count = 0;
    sdfActiveCounterRuntime = mnuCreateListState(0, 6, 0x16);
    ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.timer = sdfAllocSizeClassBlock(0x24);
    memset(((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.timer, 0, 0x24);
    ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.draw = sdfDrawCounterMapSelectionRow;
    completedMask = mdlCollectFlagBitsIntoMask();
    for (i = 0; i != 10; i++) {
        if ((mask >> i) & 1) {
            channel = (SdfCounterChannel *)mnuListAppendNode(
                (struct MenuList *)sdfActiveCounterRuntime, NULL);
            display = sdfAllocSizeClassBlock(0x10);
            memset(display, 0, 0x10);
            display->value = i + 1;
            display->word = (u32 *)(D_00390220 + i * 0x18);
            display->info = sdfResolveSceneCounterInfo(i + 1);
            if ((completedMask >> i) & 1) {
                display->flag = 1;
            }
            channel->display = display;
            count++;
        }
    }
    if (count < 6) {
        ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.base = count;
    }
    if (count == 3) {
        ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.posX = 0x148;
        ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.timer->y = 0x8F;
    } else if (count == 4) {
        ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.posX = 0x100;
        ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.timer->y = 0x84;
    } else if (count == 5) {
        ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.posX = 0xD8;
        ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.timer->y = 0x7F;
    } else {
        ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.posX = 0xB0;
        ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.timer->y = 0x7E;
    }
    sdfCounterSelectChannelByIndex(sdfActiveCounterRuntime, index);
    return 1;
}

extern u8 D_00390310[][52];

u8 *sdfResolveSceneCounterInfo(s32 scene) {
    switch (scene) {
    case 3:
        if (mdlFlagTest(7)) {
            scene = 15;
        }
        break;
    case 4:
        if (mdlFlagTest(6)) {
            scene = 11;
        }
        break;
    case 5:
        if (mdlFlagTest(0x31)) {
            scene = 12;
        }
        break;
    case 6:
        if (mdlFlagTest(0x1F)) {
            scene = 13;
        }
        break;
    case 7:
        if (mdlFlagTest(0x1E)) {
            scene = 16;
        }
        break;
    case 9:
        if (mdlFlagTest(0x16)) {
            scene = 14;
        }
        break;
    }
    return D_00390310[scene - 1];
}

void sdfCounterDestroyRuntime(SdfCounterRuntime *rt) {
    SdfCounterChannel *channel;

    if (rt != NULL) {
        for (channel = rt->state.first; channel != NULL; channel = channel->next) {
            sdfReleaseChipBlock(channel->display);
            channel->display = NULL;
        }
        sdfReleaseChipBlock(rt->state.timer);
        rt->state.timer = NULL;
        mnuDestroyListState((struct MenuList *)rt);
    }
}

void sdfDestroyActiveCounterRuntime(void) {
    sdfCounterDestroyRuntime(sdfActiveCounterRuntime);
}

void sdfCounterTickCountdownAndMapTimers(void) {
    sdfCounterTickCountdown();
    mnuTickMapTimers();
    sdfCounterTickPositionTransition();
}

extern s8 D_00324510[];
extern s32 D_003BD240;
extern s8 D_003BD280;
extern void mnuClearListFlagsOneAndTwo(SdfCounterRuntime *);
extern void sdfCounterSetMode(s32);
extern void sdfCounterStartTimerPositionTransition(s32, s32);
extern s32 sdfCounterGetSelectionBoundaryFlags(void);
extern void mnuSetMapTimerFlags(s32);

/* Step the map selection, restore its starting channel, or confirm/cancel. */
s32 sdfHandleMapCounterSelectionInput(void) {
    s32 index;
    s32 previousIndex;

    if (D_00324510[0x26] < 0 || (D_00324510[0x26] & 2)) {
        previousIndex = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.channel->index;
        if (mnuRetreatListCursorDefault((struct MenuList *)sdfActiveCounterRuntime) != NULL) {
            index = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.channel->index;
            sdfCounterSetMode(previousIndex);
            sdfCounterStartTimerPositionTransition(0,
                index * (((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.posX >> 3));
            sndSetSequenceVolumePan(0, 127, 63);
        }
    }
    if (D_00324510[0x27] < 0 || (D_00324510[0x27] & 2)) {
        previousIndex = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.channel->index;
        if (mnuAdvanceListCursorDefault((struct MenuList *)sdfActiveCounterRuntime) != NULL) {
            index = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.channel->index;
            sdfCounterSetMode(previousIndex);
            sndSetSequenceVolumePan(0, 127, 63);
            sdfCounterStartTimerPositionTransition(0,
                index * (((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.posX >> 3));
        }
    }
    if (D_00324510[0x23] < 0) {
        sdfCounterSetMode(((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.channel->index);
        index = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.channel->index;
        if (D_003BD240 != index) {
            sdfCounterSelectChannelByIndex((SdfCounterRuntime *)sdfActiveCounterRuntime,
                D_003BD240);
        }
        sdfSetSelectedIndex(D_003BD240);
        sndSetSequenceVolumePan(10, 127, 63);
    }
    if (D_00324510[0x21] < 0) {
        sndSetSequenceVolumePan(8, 127, 63);
        sdfSetSelectedIndex(((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.channel->index);
        return -2;
    }
    mnuSetMapTimerFlags(sdfCounterGetSelectionBoundaryFlags());
    if (D_00324510[0x26] == 0 && D_00324510[0x27] == 0) {
        mnuClearListFlagsOneAndTwo((SdfCounterRuntime *)sdfActiveCounterRuntime);
    }
    if (D_003BD280 < 11) {
        return 0;
    }
    return 1;
}

extern void sdfCounterDecrease(void);
extern void sdfCounterIncrease(void);
extern void sdfCounterDrawFadingLabelPanel(s32, s32);
extern void mnuDrawMapCounterTimerHints(void);
extern void sdfUpdateCounterSelectionFade(void);
extern void mnuCallInitWide(s32, s32, s32, s32, s32);

s32 sdfAdjustCounterValueAndRedraw(s32 decrease) {
    /* Untouched retail 32-byte slot: frame 0x70 rather than 0x50.
     * DDS2 sdfAdjustCounterValueAndRedraw has the same 32-byte frame gap. */
    s32 drawScratch[8];
    f32 progress = (f32)((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.timer->value / 10.0f;
    s32 x = progress * -32.0f;
    u32 color;

    if (decrease) {
        sdfCounterDecrease();
    } else {
        sdfCounterIncrease();
    }
    evtSetDrawSurfaceIndex(0x53);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    color = ((u32)((1.0f - progress) * 128.0f) << 24) | 0x808080U;
    kwlnDrawTexturedColorQuad(x, 0x77, 0xAA, 0xA3, 9, 0x151, 0xAA, 0xA3,
                  color, color, color, color, fldLocalMapTextureResource.texture);
    evtSetDrawSurfaceIndex(0x54);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    sdfCounterDrawFadingLabelPanel(0x100, 0x180);
    mnuDrawMapCounterTimerHints();
    mnuCallInitWide(x << 4, ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.timer->y << 3,
                    0, sdfActiveCounterRuntime, 0x54);
    sdfUpdateCounterSelectionFade();
    return 1;
}

u32 sdfCounterGetDisplayWordPointer(void) {
    return (u32)((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.channel->display->word;
}

s32 sdfCounterGetDisplayValue(void) {
    return ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.channel->display->value;
}

float sdfCounterGetScaledValue(void) {
    SdfCounterTimer *timer;

    timer = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.timer;
    return (float)timer->value / 10.0f;
}

/* Draw the counter's three label plates and submit the frame gradient. */
void sdfCounterDrawLabelPlateRow(s32 x, s32 y, s32 arg2, SdfCounterRuntime *rt, s32 arg4) {
    SdfCounterChannel *channelCursor;
    f32 fade;
    s32 plateIndex;
    s32 drawX;
    s32 drawY;

    fade = (f32)rt->state.timer->value / 10.0f;
    channelCursor = rt->state.channel;
    if (channelCursor->prev != NULL) {
        channelCursor = channelCursor->prev;
    } else {
        channelCursor = rt->state.last;
    }
    drawY = y - 11;
    drawX = x - 240;
    for (plateIndex = 2; plateIndex != -1; plateIndex--) {
        rt->state.draw(drawX, drawY, arg2, rt, channelCursor, arg4);
        drawX += 160;
        if (channelCursor->next != NULL) {
            channelCursor = channelCursor->next;
        } else {
            channelCursor = rt->state.first;
        }
    }
    evtSubmitDefaultDepthGradientRect(0, 384, 512, 32,
        PACK(0x60, 0x60, 0x60, (u32)(fade * 64.0f)),
        PACK(0x10, 0x10, 0x10, (u32)(fade * 16.0f)),
        PACK(0x30, 0x30, 0x30, (u32)(fade * 64.0f)),
        PACK(0x40, 0x40, 0x40, (u32)(fade * 64.0f)));
}

INCLUDE_ASM(const s32, "game/code_002C3868", sdfDrawCounterMapSelectionRow);

/* Draw one counter channel's label plate at (x, y): a shaded frame whose alpha follows the timer fraction, then the channel's text centred in it. */
void sdfCounterDrawChannelPlate(s32 x, s32 y, s32 unused, SdfCounterRuntime *rt, SdfCounterChannel *channel, s32 drawContext) {
    f32 fade = (f32)rt->state.timer->value / 10.0f;
    s32 base = 0;
    s32 width;

    if (channel->index == rt->state.channel->index) {
        base = 0x40;
    }
    evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    evtSubmitDefaultDepthGradientRect(x + 1, y + 1, 0x80, 0x16,
                  PACK(base + 0x40, base + 0x40, base + 0x40, (u32)(fade * 32.0f)),
                  PACK(base, base, base, (u32)((f32)(base + 0x10) * fade)),
                  PACK(base + 0x30, base + 0x30, base + 0x30, (u32)(fade * 64.0f)),
                  PACK(base + 0x60, base + 0x60, base + 0x60, (u32)((f32)(base + 0x70) * fade)));
    width = sdfCounterMeasureLabelWidth((u32)channel->display->word);
    evtPrepareSizedDrawResource(x + (0x80 - width) / 2 + 1, y + 1,
                                PACK((u32)(fade * 128.0f), (u32)(fade * 128.0f), (u32)((f32)(base + 0x80) * fade), (u32)(fade * 128.0f)),
                                (const char *)channel->display->word);
}

/* The display counter saturates at ten rather than wrapping. */
void sdfCounterIncrease(void) {
    s32 currentValue;

    currentValue = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.timer->value;
    if (currentValue < 10) {
        ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.timer->value = currentValue + 1;
    }
}

void sdfCounterDecrease(void) {
    s32 currentValue;

    currentValue = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.timer->value;
    if (currentValue != 0) {
        ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.timer->value = currentValue - 1;
    }
}

void sdfCounterSetMode(s32 mode) {
    SdfCounterTimer *timer;

    timer = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.timer;
    timer->mode = mode;
    timer->countdown = 8;
}

void sdfCounterTickCountdown(void) {
    SdfCounterTimer *timer;

    timer = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.timer;
    if (0 < timer->countdown) {
        timer->countdown = timer->countdown - 1;
    }
}

/* Enabling a stopped timer starts it at one; disabling clears it. */
void mnuSetMapTimerFlags(s32 flags) {
    SdfCounterTimer *timer = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.timer;
    if ((flags & 1) != 0) {
        if (timer->mapTimerPrimary == 0) {
            timer->mapTimerPrimary = 1;
        }
    } else {
        timer->mapTimerPrimary = 0;
    }
    if ((flags & 2) != 0) {
        if (timer->mapTimerSecondary == 0) {
            timer->mapTimerSecondary = 1;
        }
    } else {
        timer->mapTimerSecondary = 0;
    }
}

/* Active timers repeat every 60 ticks rather than stopping at zero. */
void mnuTickMapTimers(void) {
    SdfCounterTimer *timer = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.timer;
    if (timer->mapTimerPrimary > 0) {
        timer->mapTimerPrimary--;
        if (timer->mapTimerPrimary == 0) {
            timer->mapTimerPrimary = 60;
        }
    }
    if (timer->mapTimerSecondary > 0) {
        timer->mapTimerSecondary--;
        if (timer->mapTimerSecondary == 0) {
            timer->mapTimerSecondary = 60;
        }
    }
}

void mnuDrawMapCounterTimerHints(void) {
    SdfCounterTimer *timer = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.timer;
    f32 fade = (f32)timer->value / 10.0f;
    s32 displacement = (s32)(fade * -32.0f);
    f32 value;
    s32 tick;

    fade = 1.0f - fade;
    tick = timer->mapTimerPrimary;
    if (tick > 0) {
        tick = abs(tick);
        value = 60.0f;
        value = (f32)tick / value;
        if (value < 0.5f) {
            value = 1.0f - value;
        }
    } else {
        value = 0.0f;
    }
    value *= fade;
    kwlnDrawTexturedColorQuad(displacement + 11, 122, 17, 14, 204, 339, 17, 14,
                  ((u32)(value * 128.0f) << 24) | 0x808080,
                  ((u32)(value * 128.0f) << 24) | 0x808080,
                  ((u32)(value * 128.0f) << 24) | 0x808080,
                  ((u32)(value * 128.0f) << 24) | 0x808080,
                  fldLocalMapTextureResource.texture);
    tick = timer->mapTimerSecondary;
    if (tick > 0) {
        tick = abs(tick);
        value = 60.0f;
        value = (f32)tick / value;
        if (value < 0.5f) {
            value = 1.0f - value;
        }
    } else {
        value = 0.0f;
    }
    value *= fade;
    kwlnDrawTexturedColorQuad(displacement + 11, 257, 17, 14, 224, 339, 17, 14,
                  ((u32)(value * 128.0f) << 24) | 0x808080,
                  ((u32)(value * 128.0f) << 24) | 0x808080,
                  ((u32)(value * 128.0f) << 24) | 0x808080,
                  ((u32)(value * 128.0f) << 24) | 0x808080,
                  fldLocalMapTextureResource.texture);
}


s32 sdfCounterGetSelectionBoundaryFlags(void) {
    SdfCounterRuntime *rt = (SdfCounterRuntime *)sdfActiveCounterRuntime;
    s32 count;
    s32 done;
    if (rt->state.active == 0) {
        return 0;
    }
    count = rt->state.selected->index;
    done = count != 0;
    return ((count + rt->state.base - 1) ^ rt->state.last->index) != 0 ? (done | 2) : done;
}

void sdfCounterStartTimerPositionTransition(s32 x, s32 y) {
    SdfCounterTimer *timer = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.timer;

    timer->startX = timer->curX;
    timer->startY = timer->curY;
    timer->targetX = x;
    timer->targetY = y;
    timer->frames = 3;
}

/* Advance the timer's position transition one frame (3-frame lerp start -> target). */
void sdfCounterTickPositionTransition(void) {
    SdfCounterTimer *timer = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.timer;
    s32 frames = timer->frames;
    f32 t;

    if (frames != 0) {
        frames = --timer->frames;
    }
    t = (f32)frames / 3.0f;
    if (frames != 0) {
        timer->curX = timer->startX * t + timer->targetX * (1.0f - t);
        timer->curY = timer->startY * t + timer->targetY * (1.0f - t);
    } else {
        timer->curX = timer->targetX;
        timer->curY = timer->targetY;
    }
}

void sdfUpdateCounterSelectionFade(void) {
}

/* Draw the selected timer icon; the first callback argument is unused.
 * Fade controls both alpha and a scale shrinking from 2.5 to 0.5. */
void sdfCounterDrawSelectedTimerFade(s32 unused, s32 x, f32 fade) {
    SdfCounterTimer *timer;
    s32 offset;
    f32 grow;
    f32 shrink;

    grow = fade * 0.5f + (1.0f - fade) * 2.5f;
    offset = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.selected->index * (((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.posX >> 3);
    timer = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->state.timer;
    evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    evtSubmitPrimaryAlphaBlendMode(1);
    shrink = (1.0f - grow) * 8.5f;
    kwlnDrawTexturedColorQuad((s32)(shrink + 13.0f), (s32)((f32)(x - offset + timer->y + 7) + shrink), (s32)(grow * 17.0f), (s32)(grow * 17.0f),
                  0xB6, 0x151, 0x11, 0x11,
                  ((u32)(fade * 64.0f) << 24) | 0x808080,
                  ((u32)(fade * 64.0f) << 24) | 0x808080,
                  ((u32)(fade * 64.0f) << 24) | 0x808080,
                  ((u32)(fade * 64.0f) << 24) | 0x808080,
                  fldLocalMapTextureResource.texture);
    evtSubmitPrimaryAlphaBlendMode(0);
}

INCLUDE_SDATA(const s32, "game/code_002C3868", sdfSelectedCounterIndex);

INCLUDE_SDATA(const s32, "game/code_002C3868", sdfCounterSelectionCount);

INCLUDE_SDATA(const s32, "game/code_002C3868", D_003BD270);

INCLUDE_SDATA(const s32, "game/code_002C3868", D_003BD271);

INCLUDE_SDATA(const s32, "game/code_002C3868", sdfActiveCounterRuntime);

