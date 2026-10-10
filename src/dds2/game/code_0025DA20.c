#include "common.h"
#include "sdf_chip.h"
#include "dat_command.h"
#include "fr_font.h"
#include "fr_font_context.h"
#include "sdf_packet_list.h"
#include "eff_resource_slots.h"
#include "sdf_resource.h"
#include "evt_viewer.h"
#include "mnu.h"
#include "mnu_staff.h"
#include "mnu_shop.h"
#include "sdf.h"
#include "sdf_linked_packet.h"
#include "sdf_packet_builders.h"
#include "kwln.h"
#include "evt_world.h"
#include "evt_unit.h"
#include "evt_event_pack.h"
#include "dat_state.h"
#include "mnu_list.h"
#include "eff.h"
#include "eff_resource_records.h"
#include "fld.h"
#include "evt_task.h"
#include "kwln_task_lifecycle.h"

#define CAMP_TASK_NAME_BYTES 0x20
#define CAMP_TASK_DATA_BYTES 0x48
#define CAMP_TIMELINE_WRAP_FRAME 10
#define CAMP_TRACK_FIRST_OFFSET_TYPE 0x12
#define CAMP_CAMERA_COLOR_TRACK_TYPE 0x19
#define CAMP_KEY_LOW_BITS_MASK 0xFFF
#define CAMP_KEY_HIGH_BITS_SHIFT 12
#define CAMP_ENTRY_VALUE_FOLLOW_ALT 0
#define CAMP_ENTRY_VALUE_CLEAR 1
#define CAMP_ENTRY_NAME_CODE_BASE 2
#define CAMP_FONT_CONTEXT_WIDTH 0x960
#define CAMP_FONT_CONTEXT_HEIGHT 0x70
#define CAMP_DESCRIPTOR_WIDTH 0x200
#define CAMP_DESCRIPTOR_HEIGHT 0xE0

#define CAMP_OPTION_VALUE_MASK 3
#define CAMP_PRIMARY_OPTION_CLEAR_MASK 0xfffffffc
#define CAMP_SECONDARY_OPTION_CLEAR_MASK 0xfffffff3
#define CAMP_SECONDARY_OPTION_SHIFT 2
#define CAMP_SECONDARY_OPTION_MASK 0xc
#define CAMP_TRANSFORM_COMPONENT_COUNT 4
#define CAMP_TRANSFORM_MIDDLE_START 4
#define CAMP_TRANSFORM_LAST_START 8

#define CAMP_REGISTERED_ID_LIMIT 20
#define CAMP_TIMELINE_SCRIPT_REGISTER_BASE 200
#define CAMP_TIMELINE_SCRIPT_SLOT_COUNT 10
#define CAMP_STATUS_BATCH_COUNT 2
#define CAMP_STATUS_INITIAL_PARAMETER 15
#define CAMP_SLOT_LAST_ROW_INDEX 0x14
#define CAMP_PARTY_SCAN_LAST 4
#define CAMP_PARTY_ACTIVE_FLAG 1

extern void func_00101968(KwlnTask *, KwlnTask *);
extern s32 mnuPreparePopupAndDispatchSelection(KwlnTask *task);
extern s32 mnuAdvanceCampPopup(KwlnTask *task);
extern s32 mnuFinishCampPopup(KwlnTask *task);

extern void mnuInitializeShopStatusBatches(MenuTerminalContext *);
extern void func_002945B8(MenuTerminalContext *);
extern void mnuResetGradientFadeColor(MenuGradientFade *, s32);
extern void sndEnsureMidiBankResident(s32);
extern void sndStartTrackExtended(s32);
extern s32 func_00261198();
extern s32 func_002613C8();
extern s32 ptyCountBulletItem(s32);

extern s8 mnuCampListedItems[34];

extern void func_00246950();

typedef struct CampFlagRow {
    u32 messageSet;  /* 0x00: shop message resource number */
    s16 flag[8];     /* 0x04 */
    u8 value[9];     /* 0x14: [0] default, [i + 1] for flag[i] */
    u8 pad1D[3];
} CampFlagRow;

extern CampFlagRow D_003C9A40[];

extern s32 mdlFlagTest(u32);


extern u8 D_003CE658[];

extern SdfTex *kwlnHeldTextureReference;


extern s32 evtQueueValidatedBgmSoundCode(s32, s32);

extern s32 strcmp(const char *a, const char *b);

extern s32 D_003C99B8[];

extern void evtViewerCleanupMessageWindow();


extern KwlnTask *func_00101820(u32 priority);


extern s32 mnuRebuildCampSaleItemWindow(MenuTerminalContext *);


extern char D_00437838[]; /* "camp" */

extern char D_00424BC0[]; /* "camp_draw" */

extern char D_00424BD0[]; /* "camp_update" */

extern s8 mnuPanelTaskCompletionState;



extern void mnuShopReleaseWindowSprites();



extern void mnuShopReleaseWindowAndEffectResources();


extern s32 mnuShopReleaseSceneObjects(MenuTerminalContext *);

extern s32 dspCloseChannel(void);

extern void evtReleaseResourcePairHandle();


extern void mnuDrawAndStepGradientFade(MenuGradientFade *, s32);

extern void func_002C1B68(u32 *, u32);

extern s32 evtGetMessageWindowControlState(void);

extern u8 D_003CD8D0[];

extern u32 effMiscRand(void *);

extern u8 D_003CDA8C[];

extern u8 mnuCampCompactEntries[];

extern s32 evtGetMirroredSolarPhase(void);

extern u8 D_003CD8DD[];

extern void evtFormatTaskName(s32 taskId, void *name);


extern void *memset(void *dst, s32 c, u32 n);

extern KwlnTask *kwlnTaskCreate(const char *name, u32 priority, s32 startDelay, s32 destroyDelay, TaskUpdate update, TaskDestroy destroy, u32 userValue);


extern s32 func_002C54B0(s32);
extern s32 mnuIsBulletItemId(s32);
extern s32 func_002C5498(s32);

extern u8 D_003CDA88[];

extern s32 itfDrawBankTextWithLayoutFlags(s32, s32, u64, u64, u64, u64);

extern void frFontSetChildColors(struct FrFontGlyph *, u32);

extern s32 frFontDrawGlyphChain(s32, s32, u32);

extern f32 mnuShopSavedLastTransformVector[];
extern f32 mnuShopSavedMiddleTransformVector[];
extern f32 mnuShopSavedFirstTransformVector[];
extern s32 mnuShopRestoreMiddleVector;
extern s32 sdfAllocPacketAligned(s32 size);
extern void itfSendTablePacket(SdfListHead *packet, s32 table, s32 mode);
extern void itfQueueTextureBoundQuadPacket(void *, void *, void *, s32, SdfTex *, s32, SdfListHead *);
extern void evtSetDrawSurfaceIndex(u32);
extern void evtSubmitPrimaryAlphaBlendMode(s32);
extern void evtSubmitPrimaryGsTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern s32 D_003C9988[4];
extern s32 D_003C9998[4];
extern s32 D_003C99A8[4];
extern SdfPoolNode kwlnDrawSurfaces[];


extern ShopRankPriceRow D_003CBB70[];

/* Schedule the camp task only if no task currently owns this event ID. */
KwlnTask *mnuCampCreateTask(s32 taskId) {
    char taskName[CAMP_TASK_NAME_BYTES];
    EvtPackLoadState *taskData;
    KwlnTask *task;

    task = evtFindTaskById(taskId);
    if (task == 0) {
        evtFormatTaskName(taskId, taskName);
        taskData = sdfAllocSizeClassBlock(CAMP_TASK_DATA_BYTES);
        memset(taskData, 0, CAMP_TASK_DATA_BYTES);
        taskData->eventId = taskId;
        taskData->loaded = 0;
        task = kwlnTaskCreate(taskName, CAMP_TASK_PRIORITY, 1, 1, evtTickPackLoad, evtReleaseEventPackResources, (u32)taskData);
    }
    return task;
}

void mnuCampDestroyTaskById(s32 taskId) {
    KwlnTask *taskHandle;

    taskHandle = evtFindTaskById(taskId);
    if (taskHandle != 0) {
        kwlnTaskDestroyWithHierarchy(taskHandle, 0);
        return;
    }
}

/* Drain every camp task at the scheduler priority used during creation. */
void mnuCampDestroyAllTasks(void) {
    KwlnTask *task;

    while (task = func_00101820(CAMP_TASK_PRIORITY), task != NULL) {
        kwlnTaskDestroyWithHierarchy(task, 0);
    }
}


/* Timeline keys carry type-dependent payloads as well as their common links. */


/* The same track owns its key list, name/value state and next-track link. */




/* Event-viewer timeline data and the camp/shop state that owns those tracks. */


extern void fldApplyCameraColorKeyWords(EvtRuntime *scene, const EvtBlendKey *source);

/* Shift the selected track's frames; values exactly at the upper limit are kept. */
void mnuShopScrollList(EvtRuntime *scene, s32 delta) {
    EvtRuntimeGroup *track = scene->frameGroup;
    EvtRuntimeChild *key;
    s32 absoluteFrame;

    if (track == NULL) {
        return;
    }
    for (key = track->children; key != NULL; key = key->next) {
        absoluteFrame = key->frame + track->metadata.value + delta;
        if (absoluteFrame < track->metadata.value) {
            key->frame = 0;
        } else if (scene->headerThird < absoluteFrame) {
            key->frame = (u16)scene->headerThird - (u16)track->metadata.value - 1;
        } else {
            key->frame = key->frame + delta;
        }
    }
}

/* Shift qualifying keys and their supported payload offsets. Preserve the
 * incoming base/offset/ubase arguments when no key overwrites them: the native
 * cleanup call receives those values even after an empty track traversal. */
void mnuFxWorldScrollDelta(EvtRuntime *scene, s32 delta, s32 threshold, s32 base, s32 offset, s32 ubase) {
    EvtRuntimeGroup *track;
    EvtRuntimeChild *key;
    s32 absoluteFrame;

    if (scene->entryCount <= 0) {
        return;
    }
    if (scene->frameRange.word + delta < 0) {
        scene->frameRange.word = CAMP_TIMELINE_WRAP_FRAME;
    } else {
        scene->frameRange.word += delta;
    }
    if (scene->headerThird + delta < 0) {
        scene->headerThird = CAMP_TIMELINE_WRAP_FRAME;
    } else {
        scene->headerThird += delta;
    }
    if (scene->curFrame > scene->headerThird) {
        scene->curFrame = scene->headerThird;
    }
    track = scene->groups;
    while (track != NULL) {
        for (key = track->children; key != NULL; key = key->next) {
            offset = key->frame;
            base = track->metadata.value;
            ubase = (u16)track->metadata.value;
            absoluteFrame = offset + base;
            if (absoluteFrame < threshold) {
                continue;
            }
            absoluteFrame += delta;
            if (absoluteFrame < base) {
                key->frame = 0;
            } else if (scene->headerThird < absoluteFrame) {
                key->frame = (u16)scene->headerThird - ubase - 1;
            } else {
                key->frame = offset + delta;
            }
            /* Payload bounds test the updated offset plus delta a second time. */
            switch (track->type) {
            case CAMP_TRACK_FIRST_OFFSET_TYPE:
                if (key->p08.sh[0] != 0) {
                    key->p08.sh[0] += delta;
                    if (key->p08.sh[0] < 0) {
                        key->p08.sh[0] = 0;
                    }
                    if (scene->headerThird < key->p08.sh[0] + delta) {
                        key->p08.sh[0] = scene->headerThird - 1;
                    }
                }
                break;
            case 3:
            case 0x14:
            case 0x15:
            case 0x1A:
                if (key->p08.sh[1] != 0) {
                    key->p08.sh[1] += delta;
                    if (key->p08.sh[1] < 0) {
                        key->p08.sh[1] = 0;
                    }
                    if (scene->headerThird < key->p08.sh[1] + delta) {
                        key->p08.sh[1] = scene->headerThird - 1;
                    }
                }
                break;
            }
        }
        track = track->next;
    }
    scene->previousGlyphPosition -= 1;
    evtViewerCleanupMessageWindow(scene, delta, threshold, base, offset, ubase, track);
    evtViewerDispatchFlagMode(scene);
}

/* Process keys at or beyond threshold, restarting at the head after each call. */
void mnuFxWorldDropOutOfRange(EvtRuntime *scene, s32 threshold) {
    EvtRuntimeGroup *track;
    EvtRuntimeChild *key;

    if (scene->entryCount <= 0) {
        return;
    }
    for (track = scene->groups; track != NULL; track = track->next) {
        key = track->children;
        while (key != NULL) {
            if (key->frame + track->metadata.value < threshold) {
                key = key->next;
            } else {
                func_00246950(scene, track, key);
                key = track->children;
            }
        }
    }
}

void func_0025DE08(EvtRuntime *world, s32 threshold, s32 delta) {
    EvtRuntimeGroup *track;
    EvtRuntimeChild *key;
    s32 end = 0;
    s32 i;
    s32 length;
    s32 offset;

    for (track = world->groups; track != NULL; track = track->next) {
        for (key = track->children; key != NULL; key = key->next) {
            for (i = 0; i < D_003C9538[track->type].columns; i++) {
                switch (D_003C9538[track->type].columnTypes[i]) {
                case 1:
                case 15:
                    length = key->duration;
                    if (length != 0) {
                        offset = key->frame;
                        if (offset < threshold) {
                            if (offset + length >= threshold) {
                                key->duration = length + delta;
                            }
                        }
                    }
                    break;
                case 9:
                    switch (track->type) {
                    case 0x12:
                        end = key->p08.sh[0];
                        break;
                    case 3:
                    case 0x14:
                    case 0x15:
                    case 0x1A:
                        end = key->p08.sh[1];
                        break;
                    }
                    if (key->frame < threshold && end >= threshold && end != 0) {
                        switch (track->type) {
                        case 0x12:
                            key->p08.sh[0] += delta;
                            if (key->p08.sh[0] < 0) {
                                key->p08.sh[0] = 0;
                            }
                            break;
                        case 3:
                        case 0x14:
                        case 0x15:
                        case 0x1A:
                            key->p08.sh[1] += delta;
                            if (key->p08.sh[1] < 0) {
                                key->p08.sh[1] = 0;
                            }
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }
}

/* Default three-vector slot contents; the trailing two scalars have unknown roles. */
void mnuInitializeCampPanelVisualDefaults(f32 *firstVector, f32 *secondVector, f32 *thirdVector, f32 *scalarA, f32 *scalarB) {
    firstVector[0] = 0.7f;
    firstVector[1] = 0.7f;
    firstVector[2] = 0.7f;
    firstVector[3] = 0.0f;
    secondVector[0] = 0.65f;
    secondVector[1] = 0.39f;
    secondVector[2] = 0.65f;
    secondVector[3] = 0.0f;
    thirdVector[0] = 0.2f;
    thirdVector[1] = 0.2f;
    thirdVector[2] = 0.2f;
    thirdVector[3] = 1.0f;
    *scalarA = 7.0f;
    *scalarB = 0.0f;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E048);

typedef struct CampDisplayDefaults {
    s32 x;
    s32 y;
    u8 color[4];
    f32 scaleY;
    f32 scaleX;
    s32 enabled;
    s32 variant;
    s32 unk1C;
} CampDisplayDefaults;

void mnuCampInitializeDisplayDefaults(CampDisplayDefaults *display) {
    display->x = 0x100;
    display->y = 0xE0;
    display->color[0] = 0x80;
    display->color[1] = 0x80;
    display->color[2] = 0x80;
    display->color[3] = 0x80;
    display->scaleX = 1.0f;
    display->scaleY = 1.0f;
    display->enabled = 1;
    display->variant = 0;
    display->unk1C = 0;
}

void mnuFindCampKeyTrackNeighbors(EvtRuntimeGroup *track, s32 value, EvtRuntimeChild **out1, EvtRuntimeChild **out2);
void func_0025E460(EvtRuntimeChild *from, EvtRuntimeChild *to, CampDisplayDefaults *display, f32 ratio);

/* Use defaults before the first key; otherwise blend the bounding keys. */
void func_0025E288(EvtRuntime *viewer, EvtRuntimeGroup *track, s32 value, CampDisplayDefaults *display) {
    EvtRuntimeChild *lo;
    EvtRuntimeChild *hi;
    s32 startFrame;
    s32 endFrame;
    f32 ratio;

    mnuFindCampKeyTrackNeighbors(track, value, &lo, &hi);
    if (lo != 0) {
        startFrame = lo->frame;
    } else {
        mnuCampInitializeDisplayDefaults(display);
        return;
    }
    if (hi != 0) {
        endFrame = hi->frame;
    } else {
        endFrame = lo->frame;
    }
    if (endFrame != startFrame) {
        ratio = (f32)(value - startFrame) / (f32)(endFrame - startFrame);
    } else {
        ratio = 0.0f;
    }
    func_0025E460(lo, hi, display, ratio);
}


void mnuInitializeCampListLayoutDefaults(EvtBlendKey *layout) {
    layout->w[0] = 150;
    layout->w[1] = 150;
    layout->x = 80;
    layout->w[2] = 150;
    layout->w[3] = 30;
    layout->flagWord = 1;
    layout->y[0] = 7;
    layout->y[1] = 4;
    layout->y[2] = 10;
    layout->z[0] = 32;
    layout->z[1] = 16;
    layout->z[2] = 16;
}

void func_0025E390(EvtRuntime *viewer, EvtRuntimeGroup *track, EvtBlendKey *out) {
    EvtRuntimeChild *lower;
    EvtRuntimeChild *upper;
    EvtCameraColorPayload fallback;
    EvtCameraColorPayload *lowerPayload;
    EvtCameraColorPayload *upperPayload;
    s32 currentFrame = viewer->curFrame;
    s32 lowerFrame;
    s32 upperFrame;
    s32 duration;
    f32 ratio;

    mnuFindCampKeyTrackNeighbors(track, currentFrame, &lower, &upper);
    if (lower != NULL) {
        lowerFrame = lower->frame;
        lowerPayload = lower->payload;
    } else {
        mnuInitializeCampListLayoutDefaults(out);
        return;
    }
    if (upper != NULL) {
        upperFrame = upper->frame;
        upperPayload = upper->payload;
        duration = upperFrame - lowerFrame;
        if (upperFrame != lowerFrame) {
            ratio = (f32)(currentFrame - lowerFrame) / (f32)duration;
        } else {
            ratio = 0.0f;
        }
    } else {
        upperPayload = &fallback;
        ratio = 0.0f;
    }
    evtBlendParamsH(1, ratio, &lowerPayload->parameters, &upperPayload->parameters, out);
}

/* Blend the display fields; control bytes always come from the lower key. */
void func_0025E460(EvtRuntimeChild *from, EvtRuntimeChild *to,
                   CampDisplayDefaults *display, f32 ratio) {
    if (from == NULL) {
        mnuCampInitializeDisplayDefaults(display);
        return;
    }
    if (to == NULL || ratio == 0.0f || from->p08.sh[0] == 0) {
        display->x = from->p0C.sh[0];
        display->y = from->p0C.sh[1];
        display->color[0] = from->p10.b[0];
        display->color[1] = from->p10.b[1];
        display->color[2] = from->p10.b[2];
        display->color[3] = from->p10.b[3];
        display->scaleX = from->p14.f;
        display->scaleY = from->p18.f;
        display->enabled = from->p08.sb[0];
        display->variant = from->p08.sb[1];
        goto copyExtraMetadata;
    }
    display->x = from->p0C.sh[0] + (to->p0C.sh[0] - from->p0C.sh[0]) * ratio;
    display->y = from->p0C.sh[1] + (to->p0C.sh[1] - from->p0C.sh[1]) * ratio;
    display->color[0] = (u32)(from->p10.b[0] + (to->p10.b[0] - from->p10.b[0]) * ratio);
    display->color[1] = (u32)(from->p10.b[1] + (to->p10.b[1] - from->p10.b[1]) * ratio);
    display->color[2] = (u32)(from->p10.b[2] + (to->p10.b[2] - from->p10.b[2]) * ratio);
    display->color[3] = (u32)(from->p10.b[3] + (to->p10.b[3] - from->p10.b[3]) * ratio);
    display->scaleX = from->p14.f + (to->p14.f - from->p14.f) * ratio;
    display->scaleY = from->p18.f + (to->p18.f - from->p18.f) * ratio;
    display->enabled = from->p08.sb[0];
    display->variant = from->p08.sb[1];
copyExtraMetadata:
    display->unk1C = from->p1C.sb[0];
}

void mnuFindCampKeyTrackNeighbors(EvtRuntimeGroup *track, s32 value, EvtRuntimeChild **out1, EvtRuntimeChild **out2) {
    s32 base;

    *out1 = 0;
    *out2 = 0;
    if (track == 0) {
        return;
    }
    base = track->metadata.value;
    *out2 = track->children;
    while (*out2 != 0) {
        if (value < (*out2)->frame + base) {
            break;
        }
        *out2 = (*out2)->next;
    }
    if (*out2 != 0) {
        *out1 = (*out2)->prev;
    } else {
        *out1 = track->lastChild;
    }
    if (track->type == 2) {
        while (*out1 != 0 && evtViewerTestIndexedCondition((*out1)->p0C.sh[0]) != 1) {
            *out1 = (*out1)->prev;
        }
    }
}

u32 func_0025E7B0(void) {
    return 0;
}

/* Split the key's packed halfword into its low 12 bits and upper four bits. */
void mnuUnpackNibbleFields(EvtRuntimeChild *key, s32 *lowBitsOut, s32 *highBitsOut) {
    *lowBitsOut = key->p08.h[0] & CAMP_KEY_LOW_BITS_MASK;
    *highBitsOut = key->p08.h[0] >> CAMP_KEY_HIGH_BITS_SHIFT;
}

/* Return the scene's matching name index, or -1 when the track list has no match. */
s32 mnuCampFindMatchingEntryIndex(PolyMovieWork *lookup, EvtRuntime *scene, s32 nameIndex) {
    EvtRuntimeGroup *track = scene->groups;
    while (track != NULL) {
        if (strcmp(scene->entryName[track->entryHeader],
                   (char *)lookup->subEntry1Data + nameIndex * 32) == 0) {
            return track->entryHeader;
        }
        track = track->next;
    }
    return -1;
}

/* Return the first track with this fixed-width name, or NULL when absent. */
void *mnuCampFindEntryByName(EvtRuntime *scene, const char *name) {
    EvtRuntimeGroup *track = scene->groups;
    while (track != NULL) {
        if (strcmp(scene->entryName[track->entryHeader], name) == 0) {
            return track;
        }
        track = track->next;
    }
    return NULL;
}

/* Follow zero-coded alternatives, clear on code 1, otherwise resolve a name.
 * Keep the native signed code and separately retained halfword bits distinct. */
void campResolvePendingValue(EvtRuntime *scene, EvtRuntimeChild *cue) {
    EvtRuntimeChild *linkedKey;
    s32 entryCode;
    u16 entryCodeBits;

    if (cue == NULL) {
        return;
    }
    entryCode = cue->p10.sh[0];
    entryCodeBits = cue->p10.sh[0];
    if (entryCode == CAMP_ENTRY_VALUE_CLEAR) {
        scene->selectedEntry = 0;
        return;
    }
    if (entryCode == CAMP_ENTRY_VALUE_FOLLOW_ALT) {
        linkedKey = cue->prev;
        scene->selectedEntry = 0;
        for (; ; linkedKey = linkedKey->prev) {
            s32 linkedEntryCode;

            if (linkedKey == NULL) {
                return;
            }
            linkedEntryCode = linkedKey->p10.sh[0];
            if (linkedEntryCode != CAMP_ENTRY_VALUE_FOLLOW_ALT) {
                if (linkedEntryCode == CAMP_ENTRY_VALUE_CLEAR) {
                    scene->selectedEntry = 0;
                    return;
                }
                scene->selectedEntry = (s32)((EvtRuntimeGroup *)mnuCampFindEntryByName(scene, scene->entryName[linkedEntryCode - CAMP_ENTRY_NAME_CODE_BASE]))->texture;
                return;
            }
        }
    } else {
        scene->selectedEntry = (s32)((EvtRuntimeGroup *)mnuCampFindEntryByName(scene, scene->entryName[(s16)entryCodeBits - CAMP_ENTRY_NAME_CODE_BASE]))->texture;
    }
}

void mnuDrawCampScaledTexture(SdfTex *texture, CampDisplayDefaults *display) {
    s32 halfWidth;
    s32 halfHeight;
    s32 variant;
    SdfListHead *packet;
    s32 surfaceIndex = 0x53;
    SdfPoolNode *surface;

    D_003C9998[2] = texture->width << 4;
    D_003C9998[3] = texture->height << 4;
    halfWidth = (s32)(texture->width * display->scaleX) / 2;
    halfHeight = (s32)(texture->height * display->scaleY) / 2;
    D_003C9988[0] = (display->x - halfWidth) << 4;
    D_003C9988[1] = (display->y - halfHeight) << 3;
    D_003C9988[2] = (display->x + halfWidth) << 4;
    D_003C9988[3] = (display->y + halfHeight) << 3;
    D_003C99A8[0] = display->color[0];
    D_003C99A8[1] = display->color[1];
    D_003C99A8[2] = display->color[2];
    D_003C99A8[3] = display->color[3];
    variant = display->variant;
    if (display->color[3] != 0) {
        packet = (SdfListHead *)sdfAllocPacketAligned(0x20);
        sdfInitPacketList(packet);
        if (variant == 0) {
            itfSendTablePacket(packet, 0, 0);
        } else if (variant == 1) {
            itfSendTablePacket(packet, 1, 0);
        } else if (variant == 2) {
            itfSendTablePacket(packet, 2, 0);
        }
        if (display->unk1C != 0) {
            surfaceIndex = 0x3E;
            evtSetDrawSurfaceIndex(0x3E);
            evtSubmitPrimaryAlphaBlendMode(0);
            evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
            itfQueueTextureBoundQuadPacket(D_003C9988, D_003C9998, D_003C99A8,
                                          0xFF, texture, 0, packet);
        } else {
            itfQueueTextureBoundQuadPacket(D_003C9988, D_003C9998, D_003C99A8,
                                          0xFF, texture, 0, packet);
        }
        surface = &kwlnDrawSurfaces[surfaceIndex];
        surface->append(surface, packet);
    }
}

/* Clear each track's unknown word and the scene state; do not alter links or values. */
void fldResetCampSceneEntries(EvtRuntime *scene) {
    EvtRuntimeGroup *track;

    track = scene->groups;
    if (track != 0) {
        track->unk28 = 0;
        while (track = track->next, track != 0) {
            track->unk28 = 0;
        }
    }
    scene->colorEditorActive = 0;
}

/* Blend the first camera-color track at the clamped frame. Lookup includes
 * the track base, but interpolation retains the native unre-based key frames.
 * Without a future key, pass zero blend and the uninitialized fallback block. */
void func_0025EC00(EvtRuntime *scene) {
    EvtRuntimeGroup *track;
    EvtRuntimeChild *selectedKey;
    EvtRuntimeChild *futureKey;
    EvtBlendKey fallbackBlend;
    EvtBlendKey blendedParameters;
    EvtBlendKey *futureBlend;
    s32 frameValue;
    u16 selectedFrame;
    u16 futureFrame;
    f32 blendFraction;

    selectedKey = NULL;
    futureKey = NULL;
    frameValue = scene->curFrame;
    if (scene->colorEditorActive == 1) {
        return;
    }
    track = scene->groups;
    while (track != NULL) {
        if (track->type == CAMP_CAMERA_COLOR_TRACK_TYPE) {
            mnuFindCampKeyTrackNeighbors(track, frameValue, &selectedKey, &futureKey);
            break;
        }
        track = track->next;
    }
    if (selectedKey == NULL) {
        scene->cameraColorActive = 0;
        return;
    }

    if (futureKey == NULL) {
        blendFraction = 0.0f;
        futureBlend = &fallbackBlend;
    } else {
        selectedFrame = selectedKey->frame;
        futureFrame = futureKey->frame;
        if (futureFrame != selectedFrame) {
            blendFraction = (f32)(frameValue - selectedFrame) / (f32)(futureFrame - selectedFrame);
        } else {
            blendFraction = 0.0f;
        }
        futureBlend = &((EvtCameraColorPayload *)futureKey->payload)->parameters;
    }
    evtBlendParamsH(selectedKey->p0C.sh[0], blendFraction, &((EvtCameraColorPayload *)selectedKey->payload)->parameters, futureBlend, &blendedParameters);
    fldApplyCameraColorKeyWords(scene, &blendedParameters);
}


/* The genuine three-row typed candidate remains non-matching. */
INCLUDE_ASM(const s32, "game/code_0025DA20", fldApplyCameraColorKeyWords);

void func_0025EE00(EvtRuntime *scene) {
    func_0025EC00(scene);
    if (scene->cameraColorActive == 1) {
        func_00137888();
        return;
    }
}

void mnuCampInitFontResource(EvtRuntime *scene) {
    s32 fontHandle;
    scene->glyph = 0;
    fontHandle = (s32)(u32)frFontBuildGlyphChain((const char *)D_003C99B8, 0, 0, 0, 0);
    scene->glyph = fontHandle;
    frFontSetGlyphPosition((struct FrFontGlyph *)(u32)fontHandle,
        CAMP_FONT_CONTEXT_WIDTH, CAMP_FONT_CONTEXT_HEIGHT);
}

void mnuCampLinkFontGlyph(EvtRuntime *scene) {
    frFontQueueGlyphForCurrentDrawBuffer((struct FrFontGlyph *)(u32)scene->glyph);
    scene->glyph = 0;
}

/* Retail keeps only the divide-by-zero check (break 7) of a division whose result is never used. */
void mnuCampCheckClockDivisor(void) {
    SdfGeneralHeapStats heapStats;
    s32 quotient;

    sdfGetGeneralHeapStats(&heapStats);
    quotient = 1 / heapStats.totalBytes;
}

void mnuEnterCampSceneMenuState(EvtRuntime *scene) {
    if ((scene->menuState == 0) || (scene->menuState == 5)) {
        scene->menuState = 1;
    }
}

extern s32 kwlnCreateHeldTextureBuffer(u16 width, u16 height, f32 value);
extern void func_0025EFD8(EvtRuntime *scene);
extern void mnuShopSubmitDescriptor(u8 *work);
extern void func_0025F2B0(EvtRuntime *scene);

/* Stages 1, 3, 4 and 5 fall through; stage 0 stops. All other stages,
 * including 2, advance once through the default arm. */
void mnuAdvanceShopMenuState(EvtRuntime *scene) {
    switch (scene->menuState) {
    case 1:
        if (kwlnHeldTextureReference == 0) {
            kwlnCreateHeldTextureBuffer(CAMP_DESCRIPTOR_WIDTH, CAMP_DESCRIPTOR_HEIGHT, 100.75f);
        }
        scene->menuState = scene->menuState + 1;
    case 3:
        func_0025EFD8(scene);
        scene->menuState = scene->menuState + 1;
    case 4:
        mnuShopSubmitDescriptor((u8 *)scene);
        scene->menuState = scene->menuState + 1;
    case 5:
        if (scene->shopFlag == 1) {
            func_0025F2B0(scene);
        }
    case 0:
        break;
    default:
        scene->menuState = scene->menuState + 1;
        break;
    }
}


extern SdfPoolNode D_00380708;
extern u8 D_00380860[];

void func_0025EFD8(EvtRuntime *scene) {
    SdfListHead *surface;
    SdfLinkedPacketList *context;
    s32 handle;

    if (scene->pendingResource == 0) {
        handle = (s32)sdfAllocGeneralBlockHigh(0x70000);
        scene->pendingResource = handle;
        scene->pendingWork = (s32)sdfResourceRetainAddress((struct SdfMemBlock *)(handle));
    }
    memset((void *)scene->pendingWork, 0x40, 0x70000);
    surface = sdfAllocatePacketList(0);
    context = (SdfLinkedPacketList *)sdfAllocPacketAligned(0x10);
    sdfClearLinkedPacketList(context);
    sdfCreatePatchableResourcePacket(surface, context, 0, 0, 0x200, 0xE0,
                                    scene->pendingWork, 0, 0, 0);
    sdfAppendPacketChainNode((SdfPacketChain *)D_00380860, context);
    D_00380708.append(&D_00380708, surface);
}

void mnuShopSubmitDescriptor(u8 *scene) {
    SdfListHead *drawPacket;

    if (((EvtRuntime *)scene)->pendingWork != 0) {
        drawPacket = sdfAllocatePacketList(0);
        sdfCreateDescriptorPacket(drawPacket,
                                  kwlnHeldTextureReference->primaryResource, 0, 0,
                                  CAMP_DESCRIPTOR_WIDTH, CAMP_DESCRIPTOR_HEIGHT,
                                  ((EvtRuntime *)scene)->pendingWork, 0);
        D_00380708.append(&D_00380708, drawPacket);
    }
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F130);

void func_0025F2B0(EvtRuntime *scene) {
    func_0025F130(scene->auxResource);
}

void func_0025F2C8(void) {
}

void mnuCampSetPrimaryOption(EvtRuntime *scene, u32 option) {
    scene->optionFlags = (scene->optionFlags & CAMP_PRIMARY_OPTION_CLEAR_MASK) | (option & CAMP_OPTION_VALUE_MASK);
}

u32 mnuCampGetPrimaryOption(EvtRuntime *scene) {
    return scene->optionFlags & CAMP_OPTION_VALUE_MASK;
}

void mnuCampSetSecondaryOption(EvtRuntime *scene, u32 option) {
    scene->optionFlags = (scene->optionFlags & CAMP_SECONDARY_OPTION_CLEAR_MASK) | ((option & CAMP_OPTION_VALUE_MASK) << CAMP_SECONDARY_OPTION_SHIFT);
}

u32 mnuCampGetSecondaryOption(EvtRuntime *scene) {
    return (scene->optionFlags & CAMP_SECONDARY_OPTION_MASK) >> CAMP_SECONDARY_OPTION_SHIFT;
}

/* Clear script registers 200..209 whose type-4 key is not active at this frame. */
void func_0025F330(EvtRuntime *scene) {
    EvtRuntimeGroup *track = scene->groups;
    s32 slotIndex;

    while (track != NULL) {
        if (track->type == 4) {
            break;
        }
        track = track->next;
    }
    if (track != NULL) {
        for (slotIndex = 0; slotIndex < CAMP_TIMELINE_SCRIPT_SLOT_COUNT; slotIndex++) {
            EvtRuntimeChild *key = track->children;
            s32 found = 0;
            while (key != NULL) {
                s32 low;
                s32 high;
                mnuUnpackNibbleFields(key, &low, &high);
                if (key->frame > scene->curFrame) {
                    break;
                }
                if (high - 1 == slotIndex) {
                    found = 1;
                    break;
                }
                key = key->next;
            }
            if (!found) {
                if (datGameState->script.ints[CAMP_TIMELINE_SCRIPT_REGISTER_BASE + slotIndex] != -1) {
                    datGameState->script.ints[CAMP_TIMELINE_SCRIPT_REGISTER_BASE + slotIndex] = -1;
                }
            }
        }
    }
}

/* Save the first and last four-component vectors; leave the middle unsaved. */
void mnuShopSavePrimaryTransform(u8 *scene) {
    s32 componentIndex;
    f32 *transformComponents = ((EvtRuntime *)scene)->commandMatrix;
    for (componentIndex = 0; componentIndex < CAMP_TRANSFORM_COMPONENT_COUNT; componentIndex++) {
        mnuShopSavedLastTransformVector[componentIndex] = transformComponents[componentIndex + CAMP_TRANSFORM_LAST_START];
        mnuShopSavedFirstTransformVector[componentIndex] = transformComponents[componentIndex];
    }
    mnuShopRestoreMiddleVector = 0;
}

/* Save all three vectors and enable the middle-vector restore path. */
void mnuShopSaveFullTransform(u8 *scene) {
    s32 componentIndex;
    f32 *transformComponents = ((EvtRuntime *)scene)->commandMatrix;
    for (componentIndex = 0; componentIndex < CAMP_TRANSFORM_COMPONENT_COUNT; componentIndex++) {
        mnuShopSavedLastTransformVector[componentIndex] = transformComponents[componentIndex + CAMP_TRANSFORM_LAST_START];
        mnuShopSavedMiddleTransformVector[componentIndex] = transformComponents[componentIndex + CAMP_TRANSFORM_MIDDLE_START];
        mnuShopSavedFirstTransformVector[componentIndex] = transformComponents[componentIndex];
    }
    mnuShopRestoreMiddleVector = 1;
}

/* Keep the native restore-mode snapshot and last/middle/first write order. */
void mnuShopRestoreTransform(u8 *scene) {
    s32 componentIndex;
    f32 *transformComponents = ((EvtRuntime *)scene)->commandMatrix;
    s32 restoreMiddle = mnuShopRestoreMiddleVector;
    for (componentIndex = 0; componentIndex < CAMP_TRANSFORM_COMPONENT_COUNT; componentIndex++) {
        transformComponents[componentIndex + CAMP_TRANSFORM_LAST_START] = mnuShopSavedLastTransformVector[componentIndex];
        if (restoreMiddle != 0) {
            transformComponents[componentIndex + CAMP_TRANSFORM_MIDDLE_START] = mnuShopSavedMiddleTransformVector[componentIndex];
        }
        transformComponents[componentIndex] = mnuShopSavedFirstTransformVector[componentIndex];
    }
}

/* Append a distinct identifier only while the native registry has room. */
void fldRegisterCampSceneId(EvtRuntime *scene, s32 identifier) {
    s32 registeredCount = scene->registeredCount;
    s32 idIndex = 0;
    if (registeredCount > 0) {
        s32 *idCursor = scene->registeredIds;
        s32 registeredId = *idCursor;
        do {
            idCursor++;
            if (registeredId == identifier) {
                return;
            }
            idIndex++;
            if (idIndex >= registeredCount) {
                break;
            }
            registeredId = *idCursor;
        } while (1);
    }
    if (registeredCount < CAMP_REGISTERED_ID_LIMIT) {
        scene->registeredIds[registeredCount] = identifier;
        ++scene->registeredCount;
    }
}

/* Queue the scene's BGM ID with each registered variation, then clear the list.
 * The loop bound is read again after each queue call, not cached up front. */
void mnuReleaseCampSceneRegisteredIds(EvtRuntime *scene) {
    s32 processedCount = 0;
    if (scene->registeredCount > 0) {
        s32 *idCursor = scene->registeredIds;
        do {
            s32 identifier = *idCursor++;
            processedCount++;
            evtQueueValidatedBgmSoundCode(scene->windowContext->eventId, identifier);
        } while (processedCount < scene->registeredCount);
    }
    scene->registeredCount = 0;
}

void func_0025F640(f32 parameter, const s32 *x, const s32 *y, s32 *outX, s32 *outY) {
    f32 inverse = 1.0f - parameter;
    f32 parameterSquared = parameter * parameter;
    f32 inverseSquared = inverse * inverse;
    f32 inverseCubed = inverseSquared * inverse;
    f32 threeInverseSquared = inverseSquared * 3.0f;
    f32 threeInverseParameterSquared = (inverse * 3.0f) * parameterSquared;
    f32 xValue;
    f32 yValue;

    xValue = inverseCubed * (f32)x[0];
    xValue += (threeInverseSquared * parameter) * (f32)x[1];
    xValue += threeInverseParameterSquared * (f32)x[2];
    xValue += (parameterSquared * parameter) * (f32)x[3];
    yValue = inverseCubed * (f32)y[0];
    yValue += (threeInverseSquared * parameter) * (f32)y[1];
    yValue += threeInverseParameterSquared * (f32)y[2];
    yValue += (parameterSquared * parameter) * (f32)y[3];
    *outX = (s32)xValue;
    *outY = (s32)yValue;
}

extern void func_0025F640(f32 parameter, const s32 *x, const s32 *y, s32 *outX, s32 *outY);

s32 func_0025F708(s32 target, const s32 *x, const s32 *y) {
    s32 currentX;
    s32 resultY;
    s32 nextX;
    f32 parameter = 0.5f;
    f32 step = 0.25f;

    func_0025F640(parameter, x, y, &currentX, &resultY);
    for (;;) {
        if (currentX == target || step < 0.0009999999310821295f) {
            return resultY;
        }
        if (currentX < target) {
            parameter += step;
        }
        if (target < currentX) {
            parameter -= step;
        }
        step *= 0.5f;
        func_0025F640(parameter, x, y, &nextX, &resultY);
        currentX = nextX;
    }
}


void mnuInitializeShopStatusBatches(MenuTerminalContext *scene) {
    EffMappedResource *batchObject;
    EffMappedRecord *batchGraphics;
    s32 *batchParameters;
    s32 initialParameter = CAMP_STATUS_INITIAL_PARAMETER;
    scene->state = 0;
    batchObject = effCreateStatusBatch(6);
    batchGraphics = batchObject->records;
    scene->objects[0] = batchObject;
    batchParameters = (s32 *)batchGraphics->status;
    batchParameters[0] = initialParameter;
    batchParameters[1] = 0;
    batchParameters[2] = 0;
    batchParameters[3] = 0;
    batchParameters[4] = 0;
    batchObject = effCreateStatusBatch(1);
    batchGraphics = batchObject->records;
    scene->objects[1] = batchObject;
    batchParameters = (s32 *)batchGraphics->status;
    batchParameters[0] = initialParameter;
    batchParameters[1] = 0;
}

/* Destroy both batches and return the second destruction result. */
s32 mnuShopReleaseSceneObjects(MenuTerminalContext *scene) {
    EffMappedResource **batchCursor = scene->objects;
    s32 destroyResult;
    u32 batchIndex;
    for (batchIndex = 0; batchIndex < CAMP_STATUS_BATCH_COUNT; batchIndex++) {
        destroyResult = effDestroyPackedBatch(*batchCursor++);
    }
    return destroyResult;
}


extern const CampMapArguments D_00424A90;
extern const CampEffectRows D_00424AC0;
extern const char D_00424AE0[];
extern void mnuInitializeMapPacket(u32, u32 *, s32, MapPacket *);
extern void mnuCopyCampEffectRowData(const CampEffectRows *, MenuEffectResources *);
extern void mnuOrEntryFlags(u32, u32 *);

void func_0025F8B8(EffectSlotSet *object, MenuEffectResources *resources) {
    CampMapArguments mapArguments = D_00424A90;
    CampEffectRows rows = D_00424AC0;
    u32 dataAddress;
    struct SdfMemBlock *allocation;
    struct EffMappedResource *mappedResource;

    allocation = sdfReadNamedResource(D_00424AE0, &dataAddress, 0);
    mappedResource = effCreateMappedResource((const u8 *)dataAddress);
    sdfReleaseResourceAllocation(allocation);
    mnuInitializeMapPacket(2, mapArguments.values, 11, &resources->packet);
    mnuSetCampEffectResourceHandles(object, mappedResource, resources);
    mnuCopyCampEffectRowData(&rows, resources);
    mnuOrEntryFlags(7, &resources->packet.type);
}


void mnuShopDestroyNestedEffectBatch(MenuEffectResources *resources) {
    effDestroyPackedBatch(resources->animationHandle);
}

extern s32 mnuFirstPresentMainCharacterIndex(void);
extern const char *D_003CE470[4];
extern const char *D_003CE480[]; /* Two entries in external .data, not small data. */
extern const char *D_003CE488[4];

/* Load the shop resource sets and initialize its two scrolling panels. */
INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A00);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A10);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A20);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A30);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A40);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A50);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A60);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A70);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A80);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A90);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424AC0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424AE0);

void mnuShopLoadSpriteAssets(MenuTerminalContext *scene) {
    s32 indices[3] = {77, 78, 76};
    DspScrollingStripState *firstPanel = &scene->panelWork[0];
    DspScrollingStripState *secondPanel = &scene->panelWork[1];

    scene->effectSlots[0] = effLoadIndexedResource(
        "/facility/spr/shop/", D_003CE470[0], 0);
    switch (scene->type) {
    case 0:
    case 2:
        scene->effectSlots[1] = effLoadIndexedResource(
            "/facility/spr/shop/", D_003CE470[1], 0);
        break;
    case 1:
    case 3:
        scene->effectSlots[1] = effLoadIndexedResource(
            "/facility/spr/shop/", D_003CE470[2], 0);
        func_0025F8B8(scene->effectSlots[1], &scene->campEffect.resources);
        break;
    }
    scene->effectSlots[2] = effLoadIndexedResource(
        "/facility/spr/shop/", D_003CE480[0], 0);
    scene->effectSlots[3] = effLoadIndexedResource(
        "/facility/spr/shop/", D_003CE488[mnuFirstPresentMainCharacterIndex()], 0);
    mnuInitScrollingStripState(firstPanel, 0, scene->effectSlots[0], 0x46, 0x43);
    func_0026BE28(firstPanel, 1, 0x10, 0x20);
    mnuInitScrollingStripState(secondPanel, 0, scene->effectSlots[0], 0x46, 0x43);
    func_0026BE28(secondPanel, 0, 0x10, 0x20);
    func_0026BEB0(secondPanel, 0x1150, 0xCB8, 0);
    scene->windowResource = mnuCreateWindowSpriteResources(
        0, 0, 0, scene->effectSlots[0], indices, 3);
}


extern void evtLoadResourcePair();
extern void evtCreateMessageWindowIfMissing();

void mnuShopLoadMessageResource(MenuTerminalContext *scene) {
    scene->type = D_003C9A40[scene->shopRow].messageSet;
    switch (scene->type) {
    case 0:
        evtLoadResourcePair("/facility/msg/shop/mes_01.bmd", scene->messageResources);
        break;
    case 1:
        evtLoadResourcePair("/facility/msg/shop/mes_02.bmd", scene->messageResources);
        break;
    case 2:
        evtLoadResourcePair("/facility/msg/shop/mes_03.bmd", scene->messageResources);
        break;
    case 3:
        evtLoadResourcePair("/facility/msg/shop/mes_04.bmd", scene->messageResources);
        break;
    }
    evtCreateMessageWindowIfMissing(scene->messageResources[1]);
}


void mnuShopReleaseWindowAndEffectResources(MenuTerminalContext *scene) {
    s32 i;
    struct EffectSlotSet **slot = scene->effectSlots;

    mnuReleaseWindowTextures(scene->windowResource);
    for (i = 1; i >= 0; i--) {
        effDestroyResourceSlotSet(*slot++);
    }
    effDestroyResourceSlotSet(scene->effectSlots[2]);
    effDestroyResourceSlotSet(scene->effectSlots[3]);
    switch (scene->type) {
    case 1:
    case 3:
        mnuShopDestroyNestedEffectBatch(&scene->campEffect.resources);
        break;
    case 0:
    case 2:
        break;
    default:
        return;
    }
}

extern void func_00306CD0(s32, s32, s32, u32, s32, EffectSlotSet *, s32, s32);
extern void mnuDrawCampIconBackdrop(MenuCampEffect *, s32);

/* The shop layouts use slot 1 as their background and slot 3 as the rotated portrait. */
void func_0025FD78(MenuTerminalContext *scene) {
    switch (scene->type) {
    case 0:
    case 2:
        func_00306CD0(0, 0, 0, 0x100, 0, scene->effectSlots[1], 0, 0x53);
        func_00306CD0(0xE30, 0x610, 0, 0x100, 0, scene->effectSlots[2], 0, 0x53);
        scene->effectSlots[3]->workEntries[0].geometry.angleDegrees = 90.0f;
        func_00306CD0(0x9F0, 0x610, 0, 0x100, 2, scene->effectSlots[3], 0, 0x53);
        return;
    case 1:
    case 3:
        mnuDrawCampIconBackdrop(&scene->campEffect, 0x53);
        break;
    }
}

/* Callers pass their menu context; the check reads only global stock. */
s32 mnuCampHasEligibleOwnedItems(void *context) {
    s32 result = 0;
    s32 i;

    for (i = 0; i < 0x100; i++) {
        if (mnuIsBulletItemId(i) != 0) {
            continue;
        }
        if (func_002C54B0(i) != 0) {
            continue;
        }
        if (datGameState->inventory.counts[i] == 0) {
            continue;
        }
        if ((datItemSkillRecords[i].flags & 3) != 0) {
            result = 1;
            break;
        }
        if (func_002C5498(i) != 0) {
            result = 1;
            break;
        }
    }
    return result;
}

extern void func_00295400(void);

MenuWindowContainer *mnuCreateEnabledCampEntryWindow(s32 count, s32 *enabled, MenuTerminalContext *settings) {
    MenuWindowContainer *window;
    MenuTerminalWindowState *storage;
    s32 i;

    window = mnuCreateWindowContainer(0, 0x260, 0x10, count, 0x16);
    mnuSetWindowEntryParameters(0, window, 0, 8, 0xA);
    for (i = 0; i < count; i++) {
        if (enabled[i] != 0) {
            mnuAppendWindowListNode(window, 0)->camp.value = i;
        }
    }
    window->list->drawCallback = func_00295400;
    storage = sdfAllocSizeClassBlock(0x14);
    memset(storage, 0, 0x14);
    window->list->context = storage;
    storage->unk0C = settings->unkA0;
    storage->unk0E = settings->unkA2;
    storage->unk10 = settings->unkA4;
    return window;
}

typedef struct CampEntryEnableSet {
    s32 enabled[7];
} CampEntryEnableSet;

extern const CampEntryEnableSet D_00424BA0;
extern s32 func_00260250(MenuTerminalContext *, s32);

void mnuBuildEnabledCampEntryWindow(MenuTerminalContext *scene) {
    CampEntryEnableSet options = D_00424BA0;
    scene->unkA0 = func_00260250(scene, 1);
    if (mdlFlagTest(0x901)) {
        scene->unkA2 = func_00260250(scene, 3);
    }
    switch (D_003C9A40[scene->shopRow].messageSet) {
    case 1:
    case 2:
    case 3:
        scene->unkA4 = 1;
        break;
    }
    if (scene->unkA0 != 0) {
        options.enabled[1] = 1;
    }
    if (scene->unkA2 != 0) {
        options.enabled[2] = 1;
    }
    if (datGameState->progressTotal != 0) {
        options.enabled[4] = 1;
    }
    if (scene->unkA4 != 0) {
        options.enabled[5] = 1;
    }
    scene->ownedWindows[0] = mnuCreateEnabledCampEntryWindow(7, options.enabled, scene);
}

s32 mnuCampResolveFlagRowValue(s32 row) {
    s32 i;

    for (i = 7; i >= 0; i--) {
        if (D_003C9A40[row].flag[i] > 0 && mdlFlagTest(D_003C9A40[row].flag[i])) {
            return D_003C9A40[row].value[i + 1];
        }
    }
    return D_003C9A40[row].value[0];
}

/* Scan the twenty-one flag IDs in descending slot order. */
/* Highest matching row wins; rows with nonpositive flags are not queried. */
s32 mnuCampFindActiveSlot(void) {
    s32 rowIndex;
    for (rowIndex = CAMP_SLOT_LAST_ROW_INDEX; rowIndex >= 0; rowIndex--) {
        if (D_003CBB70[rowIndex].unlockFlag > 0 && mdlFlagTest(D_003CBB70[rowIndex].unlockFlag)) {
            return rowIndex;
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260250);


void mnuShopReleaseWindowSprites(s32 keepExtra, MenuTerminalContext *scene) {
    MenuWindowContainer **slot = scene->ownedWindows;
    MenuWindowContainer *sprite;
    u32 i;

    for (i = 0; i < 1; i++) {
        sprite = *slot;
        if (sprite != NULL) {
            if (sprite->list->context != NULL) {
                sdfReleaseChipBlock(sprite->list->context);
                sprite = *slot;
                sprite->list->context = NULL;
            }
            mnuDestroyWindowContainer(sprite);
        }
        slot++;
    }
    if (keepExtra == 0) {
        if (scene->window != NULL) {
            if (scene->window->list->context != NULL) {
                sdfReleaseChipBlock(scene->window->list->context);
                scene->window->list->context = NULL;
            }
            mnuDestroyWindowContainer(scene->window);
        }
    }
}

u32 func_00260458(void) {
    return 0;
}

u32 func_00260460(void) {
    return 0;
}

/* Sum the active low bit across five entries using the native halfword stride. */
s32 mnuCountActivePartyEntries(void) {
    u16 entryFlags;
    DatPartyRecord *entryFlagsCursor;
    s32 entryCountdown;
    s32 enabledCount;

    enabledCount = 0;
    entryCountdown = CAMP_PARTY_SCAN_LAST;
    entryFlagsCursor = datGameState->party;
    do {
        entryFlags = entryFlagsCursor->flags;
        entryFlagsCursor++;
        entryCountdown = entryCountdown - 1;
        enabledCount = enabledCount + (entryFlags & CAMP_PARTY_ACTIVE_FLAG);
    } while (-1 < entryCountdown);
    return enabledCount;
}

typedef struct CampTier {
    u32 threshold;
    s32 value;
} CampTier;

extern CampTier D_003CE408[];

s32 mnuCampResolveProgressTierValue(void) {
    u32 i;

    for (i = 0; i < 3; i++) {
        if (i + 1 < 3) {
            if (D_003CE408[i].threshold > datGameState->savedCurrency) {
                break;
            }
        } else if (D_003CE408[i].threshold <= datGameState->savedCurrency) {
            break;
        }
    }
    return D_003CE408[i].value;
}

void mnuCampClearListedItemCounts(void) {
    u16 entryId;
    s8 *entry;
    u32 index;

    index = 0;
    entry = mnuCampListedItems;
    do {
        entryId = *(u16 *)entry;
        entry = (s8 *)((s32)entry + 8);
        index = index + 1;
        datGameState->inventory.counts[(u32)entryId] = 0;
    } while (index < 3);
}

MenuTerminalContext *mnuTerminalCreateContext(void) {
    struct SdfMemBlock *handle;
    MenuTerminalContext *obj;

    handle = sdfAllocGeneralBlock(0x38C);
    obj = (MenuTerminalContext *)sdfResourceRetainAddress(handle);
    memset(obj, 0, 0x38C);
    obj->resourceHandle = handle;
    mnuClearPanelTransitionState(&obj->transitionWork);
    mnuInitializeShopStatusBatches(obj);
    obj->options = func_00260460();
    obj->availableCount = mnuCountActivePartyEntries();
    obj->delayFrames = 0xF;
    func_002945B8(obj);
    mnuResetGradientFadeColor(&obj->gradientFade, 0x60);
    mnuCampClearListedItemCounts();
    sndEnsureMidiBankResident(0x300000);
    sndStartTrackExtended(0x300000);
    return obj;
}

void mnuTerminalReleaseContextAndResources(KwlnTask *arg) {
    MenuTerminalContext *scene = (MenuTerminalContext *)kwlnTaskGetUserValue(arg);

    if (scene != 0) {
        mnuShopReleaseWindowSprites(0, scene);
        mnuShopReleaseWindowAndEffectResources(scene);
        mnuShopReleaseSceneObjects(scene);
        mnuDrainPanelTransitions(&scene->transitionWork, arg);
        dspCloseChannel();
        evtReleaseResourcePairHandle(scene->messageResources);
        sdfReleaseResourceAllocation(scene->resourceHandle);
        mnuPanelTaskCompletionState = 2;
    }
}

s32 mnuTerminalSyncMessageWindowControl(KwlnTask *task) {
    MenuGradientFade *state = &((MenuTerminalContext *)kwlnTaskGetUserValue(task))->gradientFade;
    mnuDrawAndStepGradientFade(state, 0x53);
    if (evtGetMessageWindowControlState() != 0) {
        func_002C1B68(&state->active, 1);
    } else {
        func_002C1B68(&state->active, 0);
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424BA0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424BC0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424BD0);

void func_00260708(const s32 *stockGroup) {
    MenuTerminalContext *scene = mnuTerminalCreateContext();
    KwlnTask *drawTask;
    KwlnTask *fadeTask;

    if (stockGroup != NULL) {
        scene->shopRow = *stockGroup;
    }
    kwlnTaskCreate(D_00437838, 0x402, 1, 1,
        mnuPreparePopupAndDispatchSelection, NULL, (u32)scene);
    drawTask = kwlnTaskCreate(D_00424BC0, 0x2B12, 1, 1,
        mnuAdvanceCampPopup, NULL, (u32)scene);
    kwlnTaskCreate(D_00424BD0, 0x520E, 1, 1,
        mnuFinishCampPopup, mnuTerminalReleaseContextAndResources, (u32)scene);
    fadeTask = kwlnTaskCreate("shop_fade", 0x2B13, 1, 0,
        mnuTerminalSyncMessageWindowControl, NULL, (u32)scene);
    func_00101968(drawTask, fadeTask);
    mnuPanelTaskCompletionState = 1;
}

void mnuCampDestroyPanelTasks(void) {
    kwlnTaskDestroyWithHierarchyByName(D_00437838, 0);
    kwlnTaskDestroyWithHierarchyByName(D_00424BC0, 0);
    kwlnTaskDestroyWithHierarchyByName(D_00424BD0, 0);
}

s32 mnuCampConsumePanelTaskCompletion(void) {
    s32 state = mnuPanelTaskCompletionState;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        mnuPanelTaskCompletionState = 0;
    }
    return 0;
}

static inline s32 campSetHandler(MenuTerminalContext *context, s32 mode, void *callback) {
    return func_002C4038(&context->transitionWork, &context->popupState, mode, callback);
}

s32 mnuPreparePopupAndDispatchSelection(KwlnTask *callback) {
    MenuTerminalContext *context = (MenuTerminalContext *)kwlnTaskGetUserValue(callback);
    mnuSetPopupEntry(&context->popupState, D_003CE658);
    return campSetHandler(context, 0, (void *)callback);
}

s32 mnuAdvanceCampPopup(KwlnTask *callback) {
    return campSetHandler((MenuTerminalContext *)kwlnTaskGetUserValue(callback), 1, (void *)callback);
}

s32 mnuFinishCampPopup(KwlnTask *callback) {
    return campSetHandler((MenuTerminalContext *)kwlnTaskGetUserValue(callback), 2, (void *)callback);
}

typedef struct ShopSourcePriceEntry {
    u16 itemId;
    u8 type;
    u8 pricePercent;
} ShopSourcePriceEntry;

typedef struct ShopSourcePriceRow {
    u16 pricePercent;
    ShopSourcePriceEntry entries[0x60];
} ShopSourcePriceRow;


typedef struct ShopProgressPrice {
    f32 percent;
    u32 unk4;
} ShopProgressPrice;

extern ShopSourcePriceRow D_003C9BC0[];
extern ShopProgressPrice D_003CD0C4[];

s32 mnuCampGetSourceItemPrice(s32 index, s32 source, s32 halfPrice) {
    u32 rowIndex;
    u32 itemId;
    u32 rowPercent;
    u32 pricePercent;
    s32 itemPrice;
    s32 price;
    s32 progressStage;

    rowIndex = (u8)mnuCampResolveFlagRowValue(source);
    itemId = D_003C9BC0[rowIndex].entries[index].itemId;
    if (halfPrice == 0) {
        itemPrice = datItemSkillRecords[itemId].price;
        rowPercent = D_003C9BC0[rowIndex].pricePercent;
        pricePercent = D_003C9BC0[rowIndex].entries[index].pricePercent;
        if (pricePercent == 0) {
            pricePercent = rowPercent;
        }
        price = itemPrice * pricePercent / 100;
        progressStage = func_00260458();
        if (progressStage != 0) {
            price = price * (s32)D_003CD0C4[progressStage].percent / 100;
        }
    } else {
        price = (u32)datItemSkillRecords[itemId].price >> 1;
    }
    return price;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260A58);

s32 mnuCampFindListedItemIndex(s32 id) {
    u16 *entry = (u16 *)mnuCampListedItems;
    u32 index = 0;
    do {
        if (id == *entry) {
            return index;
        }
        entry += 4;
        index++;
    } while (index < 3);
    return -1;
}

typedef struct EffItemSlot44 {
    u32 id;
    u8 pad_04[0x40];
} EffItemSlot44;

extern EffItemSlot44 D_003CD8F0[];
extern void mdlFlagSet(s32);

s32 itmClaimFreeSlot(s32 row) {
    s32 first = row * 2;
    EffItemSlot44 *slot = &D_003CD8F0[first];
    s32 i = 0;

    do {
        u32 id = slot->id;

        slot++;
        if (id != 0) {
            if (mdlFlagTest(id) == 0) {
                mdlFlagSet(id);
                return i;
            }
        }
        i++;
    } while (i < 2);
    return -1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260C28);

s32 func_00260C38(s32 slot, s32 row) {
    return row * 2 + slot;
}

u32 mnuCampChooseSolarWeightedOption(s32 row) {
    s32 chance = effMiscRand(0) & 0xFF;
    u8 *item = D_003CD8DD + row * 8;
    s32 total = 0;
    u32 index = 0;
    do {
        if (evtGetMirroredSolarPhase() == 8) {
            total += item[-3];
        } else {
            total += item[0];
        }
        if (total >= chance) {
            return index;
        }
        index++;
        item++;
    } while (index < 3);
    return 0;
}

u8 mnuCampChooseWeightedTableValue(void) {
    s32 chance = effMiscRand(0) & 0xFF;
    s32 total = 0;
    u8 *item = D_003CD8D0;
    u32 index = 0;
    do {
        total += item[1];
        if (total >= chance) {
            return item[0];
        }
        index++;
        item += 2;
    } while (index < 3);
    return 1;
}

s32 mnuCampFindFreeWideEntryIndex(s32 row) {
    s32 *entry = (s32 *)(D_003CDA8C + row * 0xC0);
    s32 index = 0;
    do {
        s32 value = *entry;
        entry += 3;
        if (value == 0) {
            return index;
        }
        index++;
    } while (index < 16);
    return 15;
}

s32 mnuCampFindFreeCompactEntryIndex(s32 unused, s32 row) {
    s32 *entry = (s32 *)(mnuCampCompactEntries + row * 0x44);
    s32 index = 0;
    do {
        s32 value = *entry;
        entry += 2;
        if (value == 0) {
            return index;
        }
        index++;
    } while (index < 8);
    return 7;
}

/* A wide reward record carries its weight, kind, item ID and optional variant. */
typedef struct CampWideRewardEntry {
    u8 weight;
    u8 kind;
    u8 reserved02[2];
    s32 id;
    u16 variant;
    u8 reserved0A[2];
} CampWideRewardEntry;

typedef char CampWideRewardEntry_size_check[
    sizeof(CampWideRewardEntry) == 0xC ? 1 : -1];

extern s32 mtrMantraFindIndex(s32);

s32 func_00260DF0(s32 row) {
    s32 column = 0;
    s32 eligibleCount = 0;
    s32 count = mnuCampFindFreeWideEntryIndex(row);
    s32 cumulative = 0;
    s32 totalWeight = 0;
    s32 indices[count];
    s32 roll;

    if (count > 0) {
        s32 *output = indices;
        for (; column < count; column++) {
            CampWideRewardEntry *entry = (CampWideRewardEntry *)(
                D_003CDA88 + column * sizeof(CampWideRewardEntry) + row * 0xC0);
            s32 id = entry->id;
            s32 allowed = 1;

            if (entry->kind == 0) {
                if (mdlFlagTest(0x901) == 0) {
                    if (func_002C54B0(id) != 0) {
                        allowed = 0;
                    }
                }
                if (mdlFlagTest(0x990) == 0) {
                    if (mtrMantraFindIndex(id) != 0) {
                        allowed = 0;
                    }
                }
            }
            if (allowed != 0) {
                *output++ = column;
                eligibleCount++;
                totalWeight += entry->weight;
            }
        }
    }
    roll = effMiscRand(NULL) % (totalWeight + 1);
    for (column = 0; column < eligibleCount; column++) {
        s32 selected = indices[column];
        cumulative += D_003CDA88[selected * 0xC + row * 0xC0];
        if (cumulative >= roll) {
            return selected;
        }
    }
    if (eligibleCount == 0) {
        return 0;
    }
    return indices[eligibleCount - 1];
}

extern u8 D_003CDA89[];

u8 func_00260FE8(s32 row, s32 column) {
    return D_003CDA89[column * 0xC + row * 0xC0];
}

extern u8 D_003CD8F4[];

u8 func_00261018(s32 row, s32 column) {
    return D_003CD8F4[column * 8 + row * 0x44];
}

s32 mnuCampResolveOwnedItemVariant(s32 row, s32 column) {
    u8 *entry = D_003CDA88 + column * 0xC + row * 0xC0;
    s32 id = *(s32 *)(D_003CDA88 + column * 0xC + row * 0xC0 + 4);

    if (entry[1] == 0 && func_002C54B0(id) != 0 && datGameState->inventory.counts[id] != 0) {
        id = *(u16 *)(entry + 8);
    }
    return id;
}

s32 mnuCampGetCompactEntryId(s32 row, s32 column) {
    return *(s32 *)(mnuCampCompactEntries + row * 0x44 + column * 8);
}

s32 mnuCampCountRemainingUses(s32 mode, s32 id, MenuTerminalContext *record) {
    s32 value = record->availableCount;

    if (mode == 1) {
        value -= ptyCountBulletItem(id);
    } else if (mode == 3) {
        value = 1 - datGameState->inventory.counts[id];
    } else if (mode == 2) {
        value = 1 - datGameState->inventory.counts[id];
    } else {
        value = 99 - datGameState->inventory.counts[id];
    }
    if (mnuCampFindListedItemIndex(id) >= 0) {
        if (value >= 2) {
            value = datGameState->inventory.counts[id] == 0;
        }
    }
    return value < 0 ? 0 : value;
}

/* Largest number of uses the current camp row allows; zero for unsupported list types. */
s32 func_00261198(MenuTerminalContext *scene) {
    s32 count = 0;
    CampWindowParams *item = &scene->window->list->cursor->camp;
    s32 listType = scene->ownedWindows[0]->list->cursor->camp.value + 1;
    s32 id = item->id;
    s32 price = item->price;
    s32 mode = item->mode;
    s32 remaining;
    u8 owned;

    switch (listType) {
    case 1:
    case 2:
    case 3:
        count = datGameState->header.currency / price;
        remaining = mnuCampCountRemainingUses(mode, id, scene);
        if (remaining < count) {
            count = remaining;
        }
        break;
    case 4:
        owned = datGameState->inventory.counts[id];
        if (owned * price > 9999999) {
            count = 9999999.0f / (f32)price;
        } else {
            count = owned;
        }
        break;
    }
    return count;
}

s32 func_00261290(MenuTerminalContext *scene) {
    CampWindowParams *item = &scene->window->list->cursor->camp;
    s32 affordable;
    s32 remaining;

    affordable = datGameState->header.currency / item->price;
    remaining = mnuCampCountRemainingUses(item->mode, item->id, scene);
    if (affordable == 0) {
        return -1;
    }
    if (remaining == 0) {
        return -2;
    }
    if (affordable > remaining) {
        affordable = remaining;
    }
    return affordable;
}

/* Mark rows unavailable when neither currency nor inventory capacity permits a use. */
void mnuCampDisableUnavailableItemEntries(MenuTerminalContext *scene) {
    struct MenuListNode *node;
    CampWindowParams *item;
    s32 i;
    s32 count;
    s32 remaining;

    node = scene->window->list->first;
    for (i = 0; i < scene->window->list->count; i++) {
        item = &node->camp;
        /* Keep the unchecked division: retail traps when the row price is zero. */
        count = datGameState->header.currency / item->price;
        remaining = mnuCampCountRemainingUses(item->mode, item->id, scene);
        if (remaining < count) {
            count = remaining;
        }
        if (count == 0) {
            node->flags48 = 1;
        }
        node = node->next;
        if (node == NULL) {
            break;
        }
    }
}

typedef struct CampPriceTier {
    u8 limit;
    u8 percent;
} CampPriceTier;

extern CampPriceTier D_003CE3E8[];

typedef struct CampRateRow {
    u16 percent;
    u8 pad02[10];
} CampRateRow;

extern CampRateRow D_003CE150[];

s32 func_002613C8(s32 level, s32 price) {
    u32 i;

    if (level != 0) {
        for (i = 0; i < 8; i++) {
            if (D_003CE3E8[i].limit != 0 && D_003CE3E8[i].limit >= level) {
                price = price * D_003CE3E8[i].percent / 100;
                break;
            }
        }
    } else {
        price = price * D_003CE150[datGameState->progressSlot].percent / 100;
    }
    return price;
}

s32 mnuCampAdvanceCounter(s32 delta, MenuTerminalContext *scene) {
    s32 max = func_00261198(scene);
    CampWindowParams *slot = &scene->window->list->cursor->camp;
    s32 sum = scene->multiplier + delta;
    s32 cur;

    scene->multiplier = sum;
    if (sum <= 0) {
        scene->multiplier = 1;
    }
    cur = scene->multiplier;
    if (cur >= max) {
        scene->unkC7 = 1;
        scene->multiplier = max;
        cur = max;
    } else {
        scene->unkC7 = 0;
    }
    if (scene->ownedWindows[0]->list->cursor->camp.value == 3) {
        slot->value = func_002613C8(cur, slot->price);
        cur = scene->multiplier;
    }
    return cur;
}

extern u8 *D_00435E5C;
extern u8 D_003C9BC4[];
extern void mnuSelectFirstListNode();
extern void func_002B9808();
extern s32 func_002958B0();
extern s32 mnuCampGetSourceItemPrice(s32, s32, s32);

s32 func_00261538(MenuTerminalContext *scene) {
    s32 rowIndex;
    ShopRankPriceRow *row;
    MenuWindowContainer *window;
    u32 i;

    rowIndex = mnuCampFindActiveSlot();
    if (rowIndex == -1) {
        return 0;
    }
    row = &D_003CBB70[rowIndex];
    for (i = 0; i < 0x20; i++) {
        u32 id = row->entries[i].itemId;
        u32 mode = row->entries[i].mode;
        struct MenuListNode *node;
        CampWindowParams *params;
        s32 price;

        if (mnuIsBulletItemId(id) != 0 || id == 0) {
            continue;
        }
        node = mnuAppendWindowListNode(scene->window,
            D_00435E5C + id * 0x19);
        func_002B9808(scene->window);
        window = scene->window;
        mnuAdvanceListCursorDefault(window->list);
        params = &node->camp;
        price = func_00260A58(i, 0);
        params->id = id;
        params->value = price;
        params->price = price;
        params->mode = mode;
        if (func_00261198(scene) == 0) {
            node->flags48 = 1;
        }
    }
    mnuSelectFirstListNode(scene->window->list);
    return 0;
}

s32 func_00261850(MenuTerminalContext *scene, s32 filterMode);

s32 func_00261670(MenuTerminalContext *scene) {
    MenuTerminalWindowState *buffer;
    u8 *entry;
    u32 i;
    s32 row;

    if (scene->window != NULL) {
        if (scene->window->list->context != NULL) {
            sdfReleaseChipBlock(scene->window->list->context);
            scene->window->list->context = NULL;
        }
        mnuDestroyWindowContainer(scene->window);
    }
    row = mnuCampResolveFlagRowValue(scene->shopRow);
    scene->window = mnuCreateWindowContainer(1, 0x260, 0x10, 8, 0x16);
    /* Each packed stock row spans 0xC1 halfwords. */
    entry = D_003C9BC4 + (row << 8) + (((row << 6) + row) << 1);
    for (i = 0; i < 0x60; i++, entry += 4) {
        u32 id = *(u16 *)(entry - 2);
        u32 mode = entry[0];
        struct MenuListNode *node;
        CampWindowParams *params;
        s32 price;

        if (mnuIsBulletItemId(id) != 0 || func_002C54B0(id) != 0 || id == 0) {
            continue;
        }
        node = mnuAppendWindowListNode(scene->window,
            D_00435E5C + id * 0x19);
        func_002B9808(scene->window);
        mnuAdvanceListCursorDefault(scene->window->list);
        params = &node->camp;
        price = mnuCampGetSourceItemPrice(i, scene->shopRow, 0);
        params->value = price;
        params->id = id;
        params->price = price;
        params->mode = mode;
        if (func_00261198(scene) == 0) {
            node->flags48 = 1;
        }
    }
    func_00261538(scene);
    mnuSelectFirstListNode(scene->window->list);
    scene->window->list->drawCallback = func_002958B0;
    buffer = sdfAllocSizeClassBlock(0x14);
    memset(buffer, 0, 0x14);
    scene->window->list->context = buffer;
    return scene->window->list->count;
}

s32 func_00261850(MenuTerminalContext *scene, s32 filterMode) {
    s32 rowIndex;
    ShopRankPriceRow *row;
    u32 i;

    rowIndex = mnuCampFindActiveSlot();
    if (rowIndex == -1) {
        return 0;
    }
    row = &D_003CBB70[rowIndex];
    for (i = 0; i < 0x20; i++) {
        u32 id = row->entries[i].itemId;
        u32 mode = row->entries[i].mode;
        struct MenuListNode *node;
        CampWindowParams *params;
        s32 price;
        s32 include;

        if (filterMode == 1) {
            include = mnuIsBulletItemId(id);
        } else {
            include = func_002C54B0(id);
        }
        if (include == 0) {
            continue;
        }
        node = mnuAppendWindowListNode(scene->window,
            D_00435E5C + id * 0x19);
        func_002B9808(scene->window);
        mnuAdvanceListCursorDefault(scene->window->list);
        params = &node->camp;
        price = func_00260A58(i, 0);
        params->value = price;
        params->id = id;
        params->price = price;
        params->mode = mode;
        if (func_00261198(scene) == 0) {
            node->flags48 = 1;
        }
    }
    mnuSelectFirstListNode(scene->window->list);
    return 0;
}

s32 func_002619A8(MenuTerminalContext *scene, s32 filterMode) {
    MenuTerminalWindowState *buffer;
    struct MenuListNode *node;
    u8 *entry;
    u32 i;
    s32 row;

    if (scene->window != NULL) {
        if (scene->window->list->context != NULL) {
            sdfReleaseChipBlock(scene->window->list->context);
            scene->window->list->context = NULL;
        }
        mnuDestroyWindowContainer(scene->window);
    }
    row = mnuCampResolveFlagRowValue(scene->shopRow);
    scene->window = mnuCreateWindowContainer(1, 0x260, 0x10, 8, 0x16);
    /* Each packed stock row spans 0xC1 halfwords. */
    entry = D_003C9BC4 + (row << 8) + (((row << 6) + row) << 1);
    for (i = 0; i < 0x60; i++, entry += 4) {
        u32 id = *(u16 *)(entry - 2);
        u32 mode = entry[0];
        CampWindowParams *params;
        s32 price;
        s32 include;

        if (filterMode == 1) {
            include = mnuIsBulletItemId(id);
        } else {
            include = func_002C54B0(id);
        }
        if (include == 0) {
            continue;
        }
        node = mnuAppendWindowListNode(scene->window,
            D_00435E5C + id * 0x19);
        func_002B9808(scene->window);
        mnuAdvanceListCursorDefault(scene->window->list);
        params = &node->camp;
        price = mnuCampGetSourceItemPrice(i, scene->shopRow, 0);
        params->value = price;
        params->id = id;
        params->price = price;
        params->mode = mode;
        if (func_00261198(scene) == 0) {
            node->flags48 = 1;
        }
    }
    func_00261850(scene, filterMode);
    mnuSelectFirstListNode(scene->window->list);
    scene->window->list->drawCallback = func_002958B0;
    buffer = sdfAllocSizeClassBlock(0x14);
    memset(buffer, 0, 0x14);
    scene->window->list->context = buffer;
    return scene->window->list->count;
}

extern u32 D_003CD0D0[][16];

s32 mnuRebuildCampSaleItemWindow(MenuTerminalContext *scene) {
    CampWindowParams *params;
    void *block;
    u32 value;
    s32 price;
    s32 i;

    if (scene->window != NULL) {
        if (scene->window->list->context != NULL) {
            sdfReleaseChipBlock(scene->window->list->context);
            scene->window->list->context = NULL;
        }
        mnuDestroyWindowContainer(scene->window);
    }
    scene->window = mnuCreateWindowContainer(1, 0x260, 0x10, 8, 0x16);
    for (i = 0; i < 0x100; i++) {
        scene->unkC7 = 1;
        if (mnuIsBulletItemId(i) != 0 || func_002C54B0(i) != 0) {
            continue;
        }
        if (datGameState->inventory.counts[i] != 0) {
            if ((datItemSkillRecords[i].flags & 3) != 0) {
                params = &mnuAppendWindowListNode(scene->window, D_00435E5C + i * 0x19)->camp;
                value = datItemSkillRecords[i].price;
                value >>= 1;
                params->value = value;
                price = func_002613C8(0, value);
                params->id = i;
                params->value = price;
                params->price = price;
            } else if (func_002C5498(i) != 0) {
                params = &mnuAppendWindowListNode(scene->window, D_00435E5C + i * 0x19)->camp;
                value = D_003CD0D0[i - 0x60][evtGetSolarPhase()];
                params->value = value;
                price = func_002613C8(0, value);
                params->id = i;
                params->value = price;
                params->price = price;
            }
        }
    }
    scene->window->list->drawCallback = func_002958B0;
    block = sdfAllocSizeClassBlock(0x14);
    memset(block, 0, 0x14);
    scene->window->list->context = block;
    return scene->window->list->count;
}

void mnuQueueCampTextGlyphWithChildColor(s32 fontValue, s32 enabled, s32 unused2, s32 unused3, s32 fontArg, s32 flags) {
    s32 handle;

    if (enabled != 0) {
        handle = itfDrawBankTextWithLayoutFlags(0x970, 0xB58, 1, (u16)fontValue, enabled, fontArg);
        frFontSetChildColors((struct FrFontGlyph *)(u32)handle, 0x80808040);
        frFontDrawGlyphChain(handle, 0, flags);
        frFontQueueGlyphForCurrentDrawBuffer((struct FrFontGlyph *)(u32)handle);
    }
}

#include "mnu_input.h"
#include "common.h"
#include "mnu.h"
#include "dat_state.h"
#include "mnu_list.h"
#include "kwln.h"

/* The dispatcher passes its last argument to entry callbacks as opaque data,
 * not as a function address. Modes select polling, primary and secondary actions. */
#define EVT_DISPATCH_OPERATION_POLL 0
#define EVT_DISPATCH_OPERATION_PRIMARY 1
#define EVT_DISPATCH_OPERATION_SECONDARY 2

#define EVT_PROGRESS_SLOT_COUNT 8
#define EVT_PROGRESS_RECORD_WORDS 3
#define EVT_ALLOWED_ITEM_COUNT 3
#define EVT_PROGRESS_UNSIGNED_LIMIT 999999U
#define EVT_PROGRESS_FLAG_GATE_COUNT 1

extern s32 evtAdvanceSlotFlags(void);

extern s32 evtGetMessageWindowControlState(void);


extern s32 kwlnFadeIsActive(void);

extern s32 datAddCurrencyClamped(s32);
extern s32 mnuCampFindListedItemIndex(s32);
extern void mdlFlagClear(s32);
extern void func_00297320(s32);
extern void mnuDrawCampCommandTransition(s32);


extern s32 D_003CE148[];
extern u8 D_003CE4EC[];
extern void func_00297220(struct MenuList *, u32);
extern s32 mnuCampAdvanceCounter(s32, MenuTerminalContext *);


extern u8 D_003CE498[];
extern s32 D_00435E48;
extern char D_00437840[];
extern s32 D_003C9A20[];
extern void evtCopyEntryStringToActiveWindow(s32, const void *);
extern s32 func_0035C860(char *, const char *, ...);
extern void evtClearActiveFlag();
extern void evtSetBoundedDisplayValue();
extern void mnuBuildEnabledCampEntryWindow();
extern s32 mnuCampHasEligibleOwnedItems();
extern s32 D_003CE14C[];
extern u8 D_003CE620[];
extern u8 D_003CE400[];
extern s32 mnuTickExtendedCommandPhase(MenuTerminalContext *);
extern s32 mdlFlagTest();
extern s32 dspStartEntry(s32);


typedef struct EvtFlagGate {
    s8 threshold;
    u8 pad1;
    u16 cue;
    u32 flag;
} EvtFlagGate;


extern s32 func_002C5498();
extern u8 D_003CE604[];
extern u16 D_003CE3F8[];
extern void mnuDrawSpriteMenuFadeOut();
extern s32 mnuDrawCommandClosePhase(MenuTerminalContext *);

extern void mnuSetCommandPhase(MenuTerminalContext *, u32);

extern void func_0026C900(void);

extern u8 D_003CE4B4[];

extern void func_00297200(struct MenuList *, u32);

extern void mnuDrawFadingValueListRow();

extern u8 D_003CE4D0[];

extern u8 D_003CE508[];

extern s32 mnuRebuildCampSaleItemWindow(MenuTerminalContext *);

extern u8 D_003CE690[];

extern void mnuStorePendingMenuCommandValue(struct MenuList *, u32);

extern void func_002971E0(struct MenuList *, u32);
extern void func_002B9808(MenuWindowContainer *);

extern void mnuPlayInputSound(s32, s32, u32 *);
extern u8 D_003CE578[];


s32 evtIsFadeDispatchIdle(void) {
    s32 fadeActive = kwlnFadeIsActive();

    if (fadeActive != 0) {
        return 0;
    }
    return evtGetMessageWindowControlState() == 0;
}

/* Install a dispatch state table and point the popup at the shared terminal entry. */
#define MNU_INSTALL_STATE_TABLE(ctx, table, entry) \
    do { \
        (ctx)->stateTable = (s32)(table); \
        mnuSetPopupEntry(&(ctx)->popupState, (entry)); \
    } while (0)

void evtInstallStateTable(s32 stateAddress) {
    if (((MenuTerminalContext *)stateAddress)->dispatchMode == 2) {
        MNU_INSTALL_STATE_TABLE(((MenuTerminalContext *)stateAddress), D_003CE498, D_003CE498 + 0x118);
    }
}

/* Mirror the chosen slot into both the active scene record and dispatch state. */
s32 evtInitializeSelectedSlot(KwlnTask *task) {
    s32 stateAddress = kwlnTaskGetUserValue(task);
    MenuTerminalWindowState *windowState;
    s32 selectedSlot;
    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(0, 2);
    if (((MenuTerminalContext *)stateAddress)->ownedWindows[0] == 0) {
        mnuBuildEnabledCampEntryWindow(stateAddress);
    }
    windowState = ((MenuTerminalContext *)stateAddress)->ownedWindows[0]->list->context;
    selectedSlot = mnuCampHasEligibleOwnedItems(stateAddress);
    ((MenuTerminalContext *)stateAddress)->previousValue = datGameState->header.currency;
    windowState->selectedSlot = selectedSlot;
    ((MenuTerminalContext *)stateAddress)->selectedSlot = selectedSlot;
    return 1;
}

s32 evtAdvancePhaseOne(KwlnTask *task) {
    MenuTerminalContext *state = (MenuTerminalContext *)kwlnTaskGetUserValue(task);
    if (state->phase == 1) {
        mnuSetCommandPhase(state, 4);
    }
    return 1;
}

extern s32 func_00297250(MenuTerminalContext *);
extern u8 D_003CE55C[];

s32 func_00261F48(KwlnTask *task) {
    MenuTerminalContext *state;
    MenuWindowContainer *window;
    struct MenuList *list;
    s32 inputFlags;
    s32 phase;
    s32 dispatchResult;
    u32 selection;
    s32 initialAllowed = 1;

    state = (MenuTerminalContext *)kwlnTaskGetUserValue(task);
    inputFlags = mnuMapPadMaskToFlags(0x33);
    window = state->ownedWindows[0];
    list = window->list;
    dispatchResult = func_002C4038(&state->transitionWork, &state->popupState, 0,
                                   task);
    if (dispatchResult != 0) {
        return dispatchResult;
    }

    phase = func_00297250(state);
    switch (phase) {
    case -1:
        return 0;
    case 0:
        mnuSetCommandPhase(state, 3);
        mnuStorePendingMenuCommandValue(list, 10);
        return 0;
    case 1: {
        struct MenuListNode *node = state->ownedWindows[0]->list->cursor;
        mnuSetPopupEntryFlagged(&state->popupState,
                                (void *)(D_003CE4B4 + node->camp.value * 0x1C));
        return 0;
    }
    case 2:
        mnuSetPopupEntryFlagged(&state->popupState, D_003CE55C);
        return 0;
    case 3:
        evtInstallStateTable((s32)state);
        break;
    default:
        break;
    }

    if (state->popupState == 0) {
        if ((inputFlags & 1) != 0) {
            if ((s16)state->selectedSlot == 0) {
                struct MenuListNode *node = state->ownedWindows[0]->list->cursor;
                initialAllowed = node->camp.value != 3;
            }

            if (initialAllowed != 0) {
                struct MenuListNode *node = state->ownedWindows[0]->list->cursor;
                selection = node->camp.value + 1U;
                if (selection < 7) {
                    if (selection >= 5) {
                        mnuSetPopupEntryFlagged(
                            &state->popupState,
                            (void *)(D_003CE4B4 + node->camp.value * sizeof(MenuPopupEntry)));
                    } else {
                        mnuSetCommandPhase(state, 1);
                        func_002971E0(list, 4);
                    }
                } else {
                    mnuSetCommandPhase(state, 1);
                    func_002971E0(list, 4);
                }
            } else {
                state->dispatchMode = 2;
            }
        } else if ((inputFlags & 2) != 0) {
            mnuSetCommandPhase(state, 2);
            func_002971E0(list, 4);
        } else if ((inputFlags & 0x300000) == 0) {
            func_002B9808(state->ownedWindows[0]);
        } else if ((inputFlags & 0x10) != 0) {
            mnuRetreatWindowListSelection(state->ownedWindows[0]);
        } else if ((inputFlags & 0x20) != 0) {
            mnuAdvanceWindowListSelection(state->ownedWindows[0]);
        }
    }

    window = state->ownedWindows[0];
    mnuPlayInputSound(0, inputFlags, &window->list->stateFlags);
    return 0;
}


s32 func_00262190(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    func_0025FD78((MenuTerminalContext *)stateAddress);
    func_00297320(stateAddress);
    return evtMenuSetHandler((void *)stateAddress, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 evtSetupDispatchSync(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    func_0026C900();
    return evtMenuSetHandler((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

void evtInstallStateTableB(s32 stateAddress) {
    if (((MenuTerminalContext *)stateAddress)->dispatchMode == 1) {
        MNU_INSTALL_STATE_TABLE(((MenuTerminalContext *)stateAddress), D_003CE4B4, D_003CE4B4 + 0xfc);
    }
}

s32 func_00262270(KwlnTask *task) {
    s32 stateAddress = kwlnTaskGetUserValue(task);
    if (((MenuTerminalContext *)stateAddress)->phase == 1) {
        func_00261670(stateAddress);
    }
    return 1;
}

s32 evtSelectStateAction(KwlnTask *task) {
    s32 stateAddress = kwlnTaskGetUserValue(task);
    struct MenuList *linkedList;
    if (((MenuTerminalContext *)stateAddress)->phase == 5) {
        mnuSetCommandPhase((MenuTerminalContext *)stateAddress, 3);
    } else if (((MenuTerminalContext *)stateAddress)->phase == 7) {
        mnuSetCommandPhase((MenuTerminalContext *)stateAddress, 9);
        linkedList = ((MenuTerminalContext *)stateAddress)->window->list;
        linkedList->drawCallback = mnuDrawFadingValueListRow;
        func_00297200(linkedList, 0xa);
    }
    ((MenuTerminalContext *)stateAddress)->stateStep = 0;
    return 1;
}


s32 func_00262330(KwlnTask *task) {
    MenuTerminalContext *state;
    MenuWindowContainer *window;
    struct MenuList *list;
    struct MenuListNode *cursor;
    s32 inputFlags;
    s32 dispatchResult;
    s32 phase;
    s32 confirmAccepted = 0;

    state = (MenuTerminalContext *)kwlnTaskGetUserValue(task);
    inputFlags = mnuMapPadMaskToFlags(0xC33);
    window = state->window;
    list = window->list;
    cursor = list->cursor;
    dispatchResult = func_002C4038(&state->transitionWork, &state->popupState,
                                   EVT_DISPATCH_OPERATION_POLL, task);
    if (dispatchResult != 0) {
        return dispatchResult;
    }

    phase = mnuTickExtendedCommandPhase(state);
    switch (phase) {
    case 4:
        mnuSetCommandPhase(state, 6);
        mnuStorePendingMenuCommandValue(list, 10);
        return 0;
    case 5:
        mnuSetPopupEntryFlagged(&state->popupState, D_003CE498);
        mnuStorePendingMenuCommandValue(state->ownedWindows[0]->list, 10);
        return 0;
    case 7:
        mnuSetPopupEntryFlagged(&state->popupState, D_003CE578);
        return 0;
    case 8: {
        struct MenuList *phaseList;
        mnuSetCommandPhase(state, 6);
        phaseList = state->window->list;
        phaseList->drawCallback = func_002958B0;
        mnuStorePendingMenuCommandValue(phaseList, 0);
        state->stateStep = 10;
        return 0;
    }
    case 6:
        evtInstallStateTableB((s32)state);
        break;
    case -1:
        return 0;
    default:
        break;
    }

    if (state->popupState == 0) {
        if ((inputFlags & 1) != 0) {
            if ((cursor->flags48 & 1) == 0) {
                mnuSetCommandPhase(state, 7);
            } else {
                if (func_00261290(state) == -2) {
                    state->dispatchMode = 1;
                }
                confirmAccepted = 1;
            }
        } else if ((inputFlags & 2) != 0) {
            mnuSetCommandPhase(state, 5);
            func_002971E0(list, 4);
        } else if ((inputFlags & 0x300000) == 0) {
            func_002B9808(state->window);
        } else if ((inputFlags & 0x10) != 0) {
            mnuRetreatWindowListSelection(state->window);
        } else if ((inputFlags & 0x20) != 0) {
            mnuAdvanceWindowListSelection(state->window);
        }

        mnuHandleListPageJumpInput((s32)state->windowResource, state->window,
                                   (u32 *)&inputFlags);
    }

    if (confirmAccepted == 1) {
        inputFlags = 2;
    }
    mnuPlayInputSound(0, inputFlags, &state->window->list->stateFlags);
    return 0;
}

s32 func_00262598(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    func_0025FD78((MenuTerminalContext *)stateAddress);
    mnuDrawCampCommandTransition(stateAddress);
    return evtMenuSetHandler((void *)stateAddress, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 evtSetupDispatchSyncB(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    func_0026C900();
    return evtMenuSetHandler((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

void evtInstallStateTableC(s32 stateAddress) {
    if (((MenuTerminalContext *)stateAddress)->dispatchMode == 1) {
        MNU_INSTALL_STATE_TABLE(((MenuTerminalContext *)stateAddress), D_003CE4D0, D_003CE4D0 + 0xe0);
    }
}

s32 func_00262678(KwlnTask *task) {
    s32 stateAddress = kwlnTaskGetUserValue(task);
    if (((MenuTerminalContext *)stateAddress)->phase == 1) {
        func_002619A8(stateAddress, 1);
    }
    return 1;
}

s32 evtSelectStateActionB(KwlnTask *task) {
    s32 context = kwlnTaskGetUserValue(task);
    struct MenuList *linkedList;
    if (((MenuTerminalContext *)context)->phase == 5) {
        mnuSetCommandPhase((MenuTerminalContext *)context, 3);
    } else if (((MenuTerminalContext *)context)->phase == 7) {
        mnuSetCommandPhase((MenuTerminalContext *)context, 9);
        linkedList = ((MenuTerminalContext *)context)->window->list;
        linkedList->drawCallback = mnuDrawFadingValueListRow;
        func_00297200(linkedList, 0xa);
    }
    ((MenuTerminalContext *)context)->stateStep = 0;
    return 1;
}

s32 func_00262740(KwlnTask *task) {
    s32 dispatchResult;
    u32 inputFlags;
    MenuTerminalContext *state;
    s32 *dispatchSlot;
    struct MenuList *linkedList;
    struct MenuList *callbackList;
    struct MenuListNode *cursor;
    s32 handled = 0;

    state = (MenuTerminalContext *)kwlnTaskGetUserValue(task);
    inputFlags = mnuMapPadMaskToFlags(0xC33);
    dispatchSlot = &state->popupState;
    linkedList = state->window->list;
    cursor = linkedList->cursor;
    dispatchResult = func_002C4038(&state->transitionWork, dispatchSlot, 0, task);
    if (dispatchResult == 0) {
        switch (mnuTickExtendedCommandPhase(state)) {
        case -1:
            break;
        case 4:
            mnuSetCommandPhase(state, 6);
            mnuStorePendingMenuCommandValue(linkedList, 10);
            break;
        case 5:
            mnuSetPopupEntryFlagged(dispatchSlot, D_003CE498);
            mnuStorePendingMenuCommandValue(state->ownedWindows[0]->list, 10);
            break;
        case 7:
            mnuSetPopupEntryFlagged(dispatchSlot, D_003CE578);
            break;
        case 8:
            mnuSetCommandPhase(state, 6);
            callbackList = state->window->list;
            callbackList->drawCallback = func_002958B0;
            mnuStorePendingMenuCommandValue(callbackList, 0);
            state->stateStep = 10;
            break;
        case 6:
            evtInstallStateTableC((s32)state);
        default:
            if (state->popupState == 0) {
                if (inputFlags & 1) {
                    if (!(cursor->flags48 & 1)) {
                        mnuSetCommandPhase(state, 7);
                    } else {
                        if (func_00261290(state) == -2) {
                            state->dispatchMode = 1;
                        }
                        handled = 1;
                    }
                } else if (inputFlags & 2) {
                    mnuSetCommandPhase(state, 5);
                    func_002971E0(linkedList, 4);
                } else if ((inputFlags & 0x300000) == 0) {
                    func_002B9808(state->window);
                } else if (inputFlags & 0x10) {
                    mnuRetreatWindowListSelection(state->window);
                } else if (inputFlags & 0x20) {
                    mnuAdvanceWindowListSelection(state->window);
                }
                mnuHandleListPageJumpInput(state->windowResource, state->window,
                                          &inputFlags);
            }
            if (handled == 1) {
                inputFlags = 2;
            }
            mnuPlayInputSound(0, inputFlags, &state->window->list->stateFlags);
            break;
        }
        return 0;
    }
    return dispatchResult;
}

s32 func_002629A8(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    func_0025FD78((MenuTerminalContext *)stateAddress);
    mnuDrawCampCommandTransition(stateAddress);
    return evtMenuSetHandler((void *)stateAddress, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 func_00262A00(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    func_0026C900();
    return evtMenuSetHandler((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

void func_00262A48(s32 stateAddress) {
    if (((MenuTerminalContext *)stateAddress)->dispatchMode == 1) {
        MNU_INSTALL_STATE_TABLE(((MenuTerminalContext *)stateAddress), D_003CE4EC, D_003CE4EC + 0xc4);
    }
}

s32 func_00262A88(KwlnTask *task) {
    s32 stateAddress = kwlnTaskGetUserValue(task);
    if (((MenuTerminalContext *)stateAddress)->phase == 1) {
        func_002619A8(stateAddress, 3);
    }
    return 1;
}

s32 mnuResetCommandStepAndSelectPhase(KwlnTask *task) {
    s32 stateAddress = kwlnTaskGetUserValue(task);
    struct MenuList *linkedList;
    if (((MenuTerminalContext *)stateAddress)->phase == 5) {
        mnuSetCommandPhase((MenuTerminalContext *)stateAddress, 3);
    } else if (((MenuTerminalContext *)stateAddress)->phase == 7) {
        mnuSetCommandPhase((MenuTerminalContext *)stateAddress, 9);
        linkedList = ((MenuTerminalContext *)stateAddress)->window->list;
        linkedList->drawCallback = mnuDrawFadingValueListRow;
        func_00297200(linkedList, 0xa);
    }
    ((MenuTerminalContext *)stateAddress)->stateStep = 0;
    return 1;
}


s32 func_00262B50(KwlnTask *callbackContext) {
    MenuTerminalContext *state;
    struct MenuList *list;
    struct MenuListNode *selected;
    s32 *popup;
    u32 input;
    s32 result;
    s32 rejected = 0;

    state = (MenuTerminalContext *)kwlnTaskGetUserValue(callbackContext);
    input = mnuMapPadMaskToFlags(0xC33);
    list = state->window->list;
    selected = list->cursor;
    popup = &state->popupState;
    result = func_002C4038(&state->transitionWork, popup,
                          EVT_DISPATCH_OPERATION_POLL, (void *)callbackContext);
    if (result != 0) {
        return result;
    }
    switch (mnuTickExtendedCommandPhase(state)) {
    case -1:
        return 0;
    case 4:
        mnuSetCommandPhase(state, 6);
        mnuStorePendingMenuCommandValue(list, 10);
        return 0;
    case 5:
        mnuSetPopupEntryFlagged(popup, D_003CE498);
        mnuStorePendingMenuCommandValue(state->ownedWindows[0]->list, 10);
        return 0;
    case 7:
        mnuSetPopupEntryFlagged(popup, D_003CE578);
        return 0;
    case 8: {
        struct MenuList *phaseList;
        mnuSetCommandPhase(state, 6);
        phaseList = state->window->list;
        phaseList->drawCallback = func_002958B0;
        mnuStorePendingMenuCommandValue(phaseList, 0);
        state->stateStep = 10;
        return 0;
    }
    case 6:
        func_00262A48((s32)state);
        break;
    default:
        break;
    }
    if (state->popupState == 0) {
        if (input & 1) {
            if (!(selected->flags48 & 1)) {
                mnuSetCommandPhase(state, 7);
            } else {
                if (func_00261290(state) == -2) {
                    state->dispatchMode = 1;
                }
                rejected = 1;
            }
        } else if (input & 2) {
            mnuSetCommandPhase(state, 5);
            func_002971E0(list, 4);
        } else if (!(input & 0x300000)) {
            func_002B9808(state->window);
        } else if (input & 0x10) {
            mnuRetreatWindowListSelection(state->window);
        } else if (input & 0x20) {
            mnuAdvanceWindowListSelection(state->window);
        }
        mnuHandleListPageJumpInput((s32)state->windowResource, state->window, &input);
    }
    if (rejected == 1) {
        input = 2;
    }
    mnuPlayInputSound(0, input, &state->window->list->stateFlags);
    return 0;
}

s32 func_00262DB8(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    func_0025FD78((MenuTerminalContext *)stateAddress);
    mnuDrawCampCommandTransition(stateAddress);
    return evtMenuSetHandler((void *)stateAddress, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 func_00262E10(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    func_0026C900();
    return evtMenuSetHandler((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

void evtInstallStateTableD(MenuTerminalContext *state) {
    if (state->dispatchMode == 2) {
        MNU_INSTALL_STATE_TABLE(state, D_003CE508, D_003CE508 + 0xa8);
    }
}

s32 evtEnableStateFlag(KwlnTask *task) {
    s32 stateAddress = kwlnTaskGetUserValue(task);
    if (((MenuTerminalContext *)stateAddress)->phase == 1 && !mnuRebuildCampSaleItemWindow((MenuTerminalContext *)stateAddress)) {
        ((MenuTerminalContext *)stateAddress)->dispatchMode = 2;
    }
    return 1;
}

s32 evtEnterProgressCommandPhase(KwlnTask *task) {
    s32 stateAddress = kwlnTaskGetUserValue(task);
    struct MenuList *linkedList;
    if (((MenuTerminalContext *)stateAddress)->phase == 5) {
        mnuSetCommandPhase((MenuTerminalContext *)stateAddress, 3);
    } else if (((MenuTerminalContext *)stateAddress)->phase == 7) {
        mnuSetCommandPhase((MenuTerminalContext *)stateAddress, 9);
        linkedList = ((MenuTerminalContext *)stateAddress)->window->list;
        linkedList->drawCallback = mnuDrawFadingValueListRow;
        func_00297200(linkedList, 0xa);
    }
    ((MenuTerminalContext *)stateAddress)->stateStep = 0;
    return 1;
}

s32 func_00262F78(KwlnTask *task) {
    s32 dispatchResult;
    u32 inputFlags;
    MenuTerminalContext *state;
    s32 *dispatchSlot;
    struct MenuList *linkedList;
    struct MenuList *callbackList;

    state = (MenuTerminalContext *)kwlnTaskGetUserValue(task);
    inputFlags = mnuMapPadMaskToFlags(0xC33);
    dispatchSlot = &state->popupState;
    linkedList = state->window->list;
    dispatchResult = func_002C4038(&state->transitionWork, dispatchSlot, 0, task);
    if (dispatchResult == 0) {
        switch (mnuTickExtendedCommandPhase(state)) {
        case -1:
            break;
        case 4:
            mnuSetCommandPhase(state, 6);
            mnuStorePendingMenuCommandValue(linkedList, 10);
            break;
        case 5:
            mnuSetPopupEntryFlagged(dispatchSlot, D_003CE498);
            mnuStorePendingMenuCommandValue(state->ownedWindows[0]->list, 10);
            break;
        case 7:
            mnuSetPopupEntryFlagged(dispatchSlot, D_003CE578);
            break;
        case 8:
            mnuSetCommandPhase(state, 6);
            callbackList = state->window->list;
            callbackList->drawCallback = func_002958B0;
            mnuStorePendingMenuCommandValue(callbackList, 0);
            state->stateStep = 10;
            break;
        case 6:
            evtInstallStateTableD(state);
        default:
            if (state->popupState == 0) {
                if (inputFlags & 1) {
                    mnuSetCommandPhase(state, 7);
                } else if (inputFlags & 2) {
                    mnuSetCommandPhase(state, 5);
                    func_002971E0(linkedList, 4);
                } else if ((inputFlags & 0x300000) == 0) {
                    func_002B9808(state->window);
                } else if (inputFlags & 0x10) {
                    mnuRetreatWindowListSelection(state->window);
                } else if (inputFlags & 0x20) {
                    mnuAdvanceWindowListSelection(state->window);
                }
                mnuHandleListPageJumpInput(state->windowResource, state->window,
                                          &inputFlags);
            }
            mnuPlayInputSound(0, inputFlags, &state->window->list->stateFlags);
            break;
        }
        return 0;
    }
    return dispatchResult;
}

s32 func_00263180(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    func_0025FD78((MenuTerminalContext *)stateAddress);
    mnuDrawCampCommandTransition(stateAddress);
    return evtMenuSetHandler((void *)stateAddress, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 func_002631D8(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    func_0026C900();
    return evtMenuSetHandler((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

/* Return a slot's remaining threshold, or -1 for an unavailable slot. */
s32 evtGetRemainingSlotThreshold(u32 slotIndex) {
    s32 threshold;
    s32 remaining;
    if (slotIndex >= EVT_PROGRESS_SLOT_COUNT) {
        return -1;
    }
    threshold = D_003CE148[slotIndex * EVT_PROGRESS_RECORD_WORDS];
    if (threshold == 0) {
        return -1;
    }
    remaining = threshold - datGameState->progressTotal;
    if (remaining > 0) {
        return remaining;
    }
    return 0;
}

s32 evtShowResultText(void) {
    char formattedText[0x40];
    evtCopyEntryStringToActiveWindow(0, (const void *)(D_00435E48 + 0x11));
    func_0035C860(formattedText, D_00437840, datGameState->progressTotal);
    evtCopyEntryStringToActiveWindow(1, formattedText);
    evtCopyEntryStringToActiveWindow(2, (const void *)(D_003C9A20[datGameState->progressSlot]));
    func_0035C860(formattedText, D_00437840, evtGetRemainingSlotThreshold(datGameState->progressSlot + 1));
    evtCopyEntryStringToActiveWindow(3, formattedText);
    if (evtGetRemainingSlotThreshold(datGameState->progressSlot + 1) >= 0) {
        dspStartEntry(datGameState->progressSlot + 0x1a);
    } else {
        dspStartEntry(0x21);
    }
    return 1;
}

u32 func_00263378(void) {
    return 1;
}

s32 evtOpenProgressResultPopupWhenIdle(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    s32 *dispatchSlot = &((MenuTerminalContext *)stateAddress)->popupState;
    s32 dispatchResult = func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, dispatchSlot, EVT_DISPATCH_OPERATION_POLL, (void *)callbackContext);
    if (dispatchResult == 0) {
        if (evtGetMessageWindowControlState() == 0) {
            mnuSetPopupEntryFlagged(dispatchSlot, D_003CE498);
        }
        return 0;
    }
    return dispatchResult;
}

s32 func_002633F0(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    func_0025FD78((MenuTerminalContext *)stateAddress);
    func_00297320(stateAddress);
    return evtMenuSetHandler((void *)stateAddress, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 func_00263448(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    func_0026C900();
    return evtMenuSetHandler((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00263490);

u32 func_002635E0(void) {
    return 1;
}

s32 func_002635E8(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    s32 *dispatchSlot = &((MenuTerminalContext *)stateAddress)->popupState;
    s32 dispatchResult = func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, dispatchSlot, EVT_DISPATCH_OPERATION_POLL, (void *)callbackContext);
    if (dispatchResult == 0) {
        if (evtGetMessageWindowControlState() == 0) {
            mnuSetPopupEntryFlagged(dispatchSlot, D_003CE498);
        }
        return 0;
    }
    return dispatchResult;
}

s32 func_00263658(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    func_0025FD78((MenuTerminalContext *)stateAddress);
    func_00297320(stateAddress);
    return evtMenuSetHandler((void *)stateAddress, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 func_002636B0(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    func_0026C900();
    return evtMenuSetHandler((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

u32 evtResetStateProgressTimer(KwlnTask *task) {
    MenuTerminalContext *state;

    state = (MenuTerminalContext *)kwlnTaskGetUserValue(task);
    state->retryFrames = 0;
    mnuSelectLastListNode(state->ownedWindows[0]->list);
    return 1;
}

/* Delay the next state table until dispatch is idle and 20 progress ticks elapse. */
s32 evtQueryStateProgress(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    s32 *dispatchSlot = &((MenuTerminalContext *)stateAddress)->popupState;
    s32 dispatchResult = func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, dispatchSlot, EVT_DISPATCH_OPERATION_POLL, (void *)callbackContext);
    if (dispatchResult == 0) {
        if (*dispatchSlot == 0) {
            if (evtGetMessageWindowControlState() == 0) {
                s32 progressTicks = ((MenuTerminalContext *)stateAddress)->retryFrames;
                if ((f32)progressTicks < 20.0f) {
                    ((MenuTerminalContext *)stateAddress)->retryFrames = progressTicks + 1;
                } else {
                    mnuSetPopupEntry(dispatchSlot, D_003CE690);
                }
            }
        }
        return 0;
    }
    return dispatchResult;
}

s32 evtDispatchProgressCallback(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    func_0025FD78((MenuTerminalContext *)stateAddress);
    mnuDrawSpriteMenuFadeOut(stateAddress, ((MenuTerminalContext *)stateAddress)->retryFrames);
    return evtMenuSetHandler((void *)stateAddress, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 evtSetupDispatchSyncE(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    func_0026C900();
    return evtMenuSetHandler((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

s32 evtApplyBaseRateProgressStep(KwlnTask *task) {
    MenuTerminalContext *state = (MenuTerminalContext *)kwlnTaskGetUserValue(task);
    state->multiplier = 1;
    mnuCampAdvanceCounter(-1, state);
    return 1;
}

s32 evtAdvanceStateStage(KwlnTask *task) {
    s32 stateAddress = kwlnTaskGetUserValue(task);
    struct MenuList *linkedList;
    if (((MenuTerminalContext *)stateAddress)->phase == 0xa) {
        ((MenuTerminalContext *)stateAddress)->stateStep = 0xa;
        mnuSetCommandPhase((MenuTerminalContext *)stateAddress, 6);
        linkedList = ((MenuTerminalContext *)stateAddress)->window->list;
        linkedList->drawCallback = func_002958B0;
        mnuStorePendingMenuCommandValue(linkedList, 0);
    }
    return 1;
}

extern s32 mnuTickCommandWaitPhase(MenuTerminalContext *);
extern void sndSetSequenceVolumePan(s32, s32, s32);
extern u8 D_003CE594[];

/* Poll command phases, adjust the idle quantity selection, and play UI sounds. */
s32 evtPollQuantitySelection(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    s32 *dispatchSlot = &((MenuTerminalContext *)stateAddress)->popupState;
    s32 previousQuantity = ((MenuTerminalContext *)stateAddress)->multiplier;
    CampWindowParams *values =
        &((MenuTerminalContext *)stateAddress)->window->list->cursor->camp;
    s32 input = mnuMapPadMaskToFlags(0xF000F3);
    s32 result = func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, dispatchSlot,
                             EVT_DISPATCH_OPERATION_POLL, (void *)callbackContext);

    if (result != 0) {
        return result;
    }
    switch (mnuTickCommandWaitPhase((MenuTerminalContext *)stateAddress)) {
    case -1:
        break;
    case 9:
        mnuSetCommandPhase((MenuTerminalContext *)stateAddress, 11);
        break;
    case 10: {
        struct MenuList *primaryList =
            ((MenuTerminalContext *)stateAddress)->ownedWindows[0]->list;
        values->value = values->price;
        mnuSetPopupEntryFlagged(dispatchSlot, (D_003CE4B4 +
            primaryList->cursor->camp.value * 0x1C));
        break;
    }
    case 12:
        mnuSetPopupEntryFlagged(dispatchSlot, D_003CE594);
        break;
    case 11:
    default:
        if (((MenuTerminalContext *)stateAddress)->popupState == 0) {
            if (input & 1) {
                mnuSetCommandPhase((MenuTerminalContext *)stateAddress, 12);
            } else if (input & 2) {
                mnuSetCommandPhase((MenuTerminalContext *)stateAddress, 10);
                func_00297220(
                    ((MenuTerminalContext *)stateAddress)->window->list, 10);
            }
            if (input & 0x300030) {
                if (input & 0x10) {
                    mnuCampAdvanceCounter(1, (MenuTerminalContext *)stateAddress);
                } else if (input & 0x20) {
                    mnuCampAdvanceCounter(-1, (MenuTerminalContext *)stateAddress);
                }
                if (previousQuantity == ((MenuTerminalContext *)stateAddress)->multiplier) {
                    input &= 3;
                }
            } else if (input & 0xC0) {
                if (input & 0x80) {
                    mnuCampAdvanceCounter(10, (MenuTerminalContext *)stateAddress);
                } else if (input & 0x40) {
                    mnuCampAdvanceCounter(-10, (MenuTerminalContext *)stateAddress);
                }
                if (previousQuantity == ((MenuTerminalContext *)stateAddress)->multiplier) {
                    input &= 3;
                }
            }
        }
        if (input & 0xF0) {
            sndSetSequenceVolumePan(1, 0x7F, 0x3F);
        }
        if (input & 1) {
            sndSetSequenceVolumePan(8, 0x7F, 0x3F);
        }
        if (input & 2) {
            sndSetSequenceVolumePan(10, 0x7F, 0x3F);
        }
        break;
    }
    return 0;
}

s32 func_00263B98(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    func_0025FD78((MenuTerminalContext *)stateAddress);
    mnuDrawCommandClosePhase((MenuTerminalContext *)stateAddress);
    return evtMenuSetHandler((void *)stateAddress, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 func_00263BF0(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    func_0026C900();
    return evtMenuSetHandler((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

extern void dspSetActive(s32);
extern void evtSetMessageWindowOptionWhenOpen(s32);
extern s32 evtStoreValueAndCaptureWindowPanelValue(s32);
s32 func_00263C38(KwlnTask *task) {
    MenuTerminalContext *state = (MenuTerminalContext *)kwlnTaskGetUserValue(task);
    s32 entry = 7;
    CampWindowParams *item = &state->window->list->cursor->camp;
    s32 mode = state->ownedWindows[0]->list->cursor->camp.value + 1;
    char text[16];

    state->previousValue = datGameState->header.currency;
    state->elapsedFrames = 0;
    if (mode == 4) {
        if ((u32)(item->value * state->multiplier + datGameState->header.currency) > 9999999U) {
            entry = 12;
        }
    }
    func_0035C860(text, D_00437840, item->value * state->multiplier);
    evtCopyEntryStringToActiveWindow(3, text);
    dspSetActive(1);
    dspStartEntry(entry);
    evtSetMessageWindowOptionWhenOpen(0);
    evtStoreValueAndCaptureWindowPanelValue(11);
    state->window->list->drawCallback = mnuDrawFadingValueListRow;
    state->sceneReady = 0;
    return 1;
}


s32 evtUpdateSlotItemCompletionState(KwlnTask *task) {
    s32 stateAddress = kwlnTaskGetUserValue(task);
    s32 nextStage = ((MenuTerminalContext *)stateAddress)->ownedWindows[0]->list->cursor->camp.value + 1;
    s32 itemId = ((MenuTerminalContext *)stateAddress)->window->list->cursor->camp.id;
    if (nextStage == 4) {
        if (datGameState->inventory.counts[itemId] == 0) {
            mnuRemoveListCursorNode(((MenuTerminalContext *)stateAddress)->window->list);
        }
        if (((MenuTerminalContext *)stateAddress)->window->list->count == 0) {
            ((MenuTerminalContext *)stateAddress)->dispatchMode = 2;
        }
    }
    return 1;
}

s32 evtPollStageSelectionAndAdvance(KwlnTask *task) {
    s32 stateAddress = kwlnTaskGetUserValue(task);
    struct MenuList *linkedList;
    switch (mnuTickExtendedCommandPhase((MenuTerminalContext *)stateAddress)) {
    case 6:
        return 1;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        return 0;
    case 8:
        mnuSetCommandPhase((MenuTerminalContext *)stateAddress, 6);
        linkedList = ((MenuTerminalContext *)stateAddress)->window->list;
        linkedList->drawCallback = func_002958B0;
        mnuStorePendingMenuCommandValue(linkedList, 0);
        ((MenuTerminalContext *)stateAddress)->stateStep = 0xa;
        return 0;
    default:
        return 0;
    }
}

/* The stage-selection caller forwards its task to the user-value lookup. */
void evtMarkSceneFollowupReadyAndQueueAction(KwlnTask *task) {
    s32 stateAddress = kwlnTaskGetUserValue(task);
    mnuSetCommandPhase((MenuTerminalContext *)stateAddress, 8);
    func_00297220(((MenuTerminalContext *)stateAddress)->window->list, 10);
    ((MenuTerminalContext *)stateAddress)->sceneReady = 1;
}

void evtAccumulateEligibleStageMultiplierValue(MenuTerminalContext *state) {
    CampWindowParams *entryValues = &state->window->list->cursor->camp;
    if (func_002C5498(entryValues->id)) {
        datGameState->world.score += entryValues->value * state->multiplier;
    }
}

s32 evtIsAllowedId(s32 itemId) {
    u32 allowedItemIndex;
    for (allowedItemIndex = 0; allowedItemIndex < EVT_ALLOWED_ITEM_COUNT; allowedItemIndex++) {
        if (D_003CE3F8[allowedItemIndex] == itemId) {
            return 1;
        }
    }
    return 0;
}

/* Add progress, treating zero as one. The unsigned check also clamps negative totals. */
s32 func_00263F50(s32 progressDelta) {
    if (progressDelta == 0) {
        progressDelta = 1;
    }
    datGameState->progressTotal += progressDelta;
    if ((u32)datGameState->progressTotal > EVT_PROGRESS_UNSIGNED_LIMIT) {
        datGameState->progressTotal = 999999;
    }
    return datGameState->progressTotal;
}

/* Apply the selected shop transaction and preserve its progress baseline. */
s32 evtApplyShopQuantityTransaction(KwlnTask *task) {
    MenuTerminalContext *state;
    CampWindowParams *values;
    s32 itemId;
    s32 operation;
    s32 total;

    state = (MenuTerminalContext *)kwlnTaskGetUserValue(task);
    values = &state->window->list->cursor->camp;
    itemId = values->id;
    operation = state->ownedWindows[0]->list->cursor->camp.value + 1;
    total = values->value * state->multiplier;
    state->unkC8 = datGameState->progressTotal;
    switch (operation) {
    case 1:
    case 2:
    case 3:
        datAddCurrencyClamped(-total);
        if (mnuCampFindListedItemIndex(itemId) < 0) {
            datGameState->inventory.counts[itemId] += state->multiplier;
        }
        if (!evtIsAllowedId(itemId)) {
            func_00263F50(total / 100);
        }
        break;
    case 4:
        datAddCurrencyClamped(total);
        datGameState->inventory.counts[itemId] -= state->multiplier;
        if (itemId == 0x54) {
            mdlFlagClear(0xA20);
        }
        func_00263F50(total / 100);
        break;
    }
    values->value = values->price;
    return 1;
}

extern s8 evtGetCapturedWindowPanelValue(void);
extern u8 D_003CE5CC[];
extern u8 D_003CE5E8[];

/* Complete an idle shop confirmation and queue its followup state. */
s32 func_00264120(KwlnTask *task) {
    MenuTerminalContext *state = (MenuTerminalContext *)kwlnTaskGetUserValue(task);
    s32 *dispatchSlot = &state->popupState;
    s32 operation;
    s32 result = func_002C4038(&state->transitionWork, dispatchSlot,
                             EVT_DISPATCH_OPERATION_POLL, task);

    if (result != 0) {
        return result;
    }
    if (*dispatchSlot == 0 && evtGetMessageWindowControlState() == 0) {
        operation = state->ownedWindows[0]->list->cursor->camp.value + 1;
        if (evtGetCapturedWindowPanelValue() == 0) {
            evtApplyShopQuantityTransaction(task);
            switch (operation) {
            case 1:
            case 2:
            case 3:
                mnuCampDisableUnavailableItemEntries(state);
                break;
            case 4:
                evtAccumulateEligibleStageMultiplierValue(state);
                break;
            }
            state->unkCD = 1;
            mnuSetPopupEntryFlagged(dispatchSlot, D_003CE5CC);
        } else {
            state->unkCD = 0;
            mnuSetPopupEntryFlagged(dispatchSlot, D_003CE5E8);
        }
        evtMarkSceneFollowupReadyAndQueueAction(task);
    }
    return 0;
}


s32 evtDispatchSceneReadyFollowup(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    func_0025FD78((MenuTerminalContext *)stateAddress);
    if (((MenuTerminalContext *)stateAddress)->sceneReady == 1) {
        mnuDrawCampCommandTransition(stateAddress);
    } else {
        mnuDrawCommandClosePhase((MenuTerminalContext *)stateAddress);
    }
    return evtMenuSetHandler((void *)stateAddress, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 func_002642B8(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    func_0026C900();
    return evtMenuSetHandler((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

s32 evtPlayDispatchModeCue(KwlnTask *task) {
    s32 stateAddress = kwlnTaskGetUserValue(task);
    dspSetActive(1);
    switch (((MenuTerminalContext *)stateAddress)->dispatchMode) {
    case 1:
        dspStartEntry(5);
        break;
    case 2:
        dspStartEntry(6);
        break;
    }
    return 1;
}

s32 evtApplyDispatchModeState(KwlnTask *task) {
    s32 stateAddress = kwlnTaskGetUserValue(task);
    switch (((MenuTerminalContext *)stateAddress)->dispatchMode) {
    case 1:
        mnuSetCommandPhase((MenuTerminalContext *)stateAddress, 6);
        break;
    case 2:
        if (((MenuTerminalContext *)stateAddress)->stateTable != (s32)D_003CE498) {
            mnuSetCommandPhase((MenuTerminalContext *)stateAddress, 5);
        }
        break;
    }
    ((MenuTerminalContext *)stateAddress)->dispatchMode = 0;
    return 1;
}

s32 evtSetPopupEntryWhenMessageWindowIdle(KwlnTask *callbackContext) {
    s32 stateAddress;
    s32 result;
    s32 *dispatchSlot;

    stateAddress = kwlnTaskGetUserValue(callbackContext);
    dispatchSlot = &((MenuTerminalContext *)stateAddress)->popupState;
    result = func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, dispatchSlot, EVT_DISPATCH_OPERATION_POLL, callbackContext);
    if (result == 0) {
        if ((*dispatchSlot == 0) && (result = evtGetMessageWindowControlState(), result == 0)) {
            mnuSetPopupEntryFlagged(dispatchSlot, (void *)((MenuTerminalContext *)stateAddress)->stateTable);
        }
        result = 0;
    }
    return result;
}

extern void func_00294B40(s32, s32, s32, void *, s32, s32);
extern void func_00294D50(s32, s32, s32, void *, s32, s32);
extern void mnuDrawWindowBorderAndScrollBar(s32, s32, s32, void *, s32, s32);
extern void func_00295030(s32, s32, s32, void *, s32, s32, s32);
extern void mnuDrawIfActive(s32, s32, s32, void *, s32);
extern void func_002969D8(s32, s32, s32, void *, s32);
extern void mnuDrawListChildrenWithCountdown(s32, s32, s32, u8 *, s32);

extern void func_00296B48(s32, s32, s32, s32, s32, s32);
extern void func_00296D90(void *, s32);
extern void func_00296E98(s32, u32, s32, s32);

/* Draw the active dispatch mode before advancing its callback state. */
s32 func_00264480(KwlnTask *callbackContext) {
    MenuTerminalContext *state = (MenuTerminalContext *)kwlnTaskGetUserValue(callbackContext);

    func_0025FD78(state);
    switch (state->dispatchMode) {
    case 1:
        func_00294B40(0, 0, 0, state, 0x100, 0x53);
        mnuDrawWindowBorderAndScrollBar(0, 0, 0, state, 0x100, 0x53);
        func_00295030(0, 0, 0, state, 0x100, 0, 0x53);
        mnuDrawIfActive(0, 0, 0, state->window, 0x53);
        func_002969D8(0, 0, 0, state, 0x53);
        func_00296D90(state, 0xA09DC380);
        func_00296E98((s32)state, 0x100, 3, 0x53);
        break;
    case 2:
        if (state->stateTable == (s32)D_003CE498) {
            func_00294B40(0, 0, 0, state, 0x100, 0x53);
            func_00294D50(0, 0, 0, state, 0x100, 0x53);
            mnuDrawListChildrenWithCountdown(
                0, 0, 0, (u8 *)state->ownedWindows[0]->list, 0x53);
            func_00296D90(state, 0xA09DC380);
            func_00296E98((s32)state, 0x100, 4, 0x53);
        } else {
            func_00294B40(0, 0, 0, state, 0x100, 0x53);
            mnuDrawWindowBorderAndScrollBar(0, 0, 0, state, 0x100, 0x53);
            func_00295030(0, 0, 0, state, 0x100, 0, 0x53);
            mnuClearWindowPanelTransitionFlag(state->ownedWindows[0]);
            func_00296B48(0, 0, 0, (s32)state, 0x100, 0x53);
            func_00296D90(state, 0xA09DC380);
            func_00296E98((s32)state, 0x100, 2, 0x53);
        }
        break;
    }
    return evtMenuSetHandler(state, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 func_002646C8(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    func_0026C900();
    return evtMenuSetHandler((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

extern s32 func_00260C28(s32 row, s32 option);
extern u32 mnuCampChooseSolarWeightedOption(s32 row);
extern u8 mnuCampChooseWeightedTableValue(void);
extern s32 mnuCampFindFreeCompactEntryIndex(s32 unused, s32 row);
extern void sndSetSequenceVolumePan(s32 sequence, s32 volume, s32 pan);

s32 func_00264710(KwlnTask *task) {
    MenuTerminalContext *scene = (MenuTerminalContext *)kwlnTaskGetUserValue(task);
    struct MenuList *activeList = scene->window->list;
    struct MenuListNode *selectedNode = scene->ownedWindows[0]->list->cursor;
    s32 selection = selectedNode->sortKeyPrimary + 1;
    s32 listedIndex;
    s8 slot;

    if (activeList->count != 0) {
        listedIndex = mnuCampFindListedItemIndex(activeList->cursor->sortKeySecondary);
    } else {
        listedIndex = -1;
    }
    if (selection == 1 && listedIndex >= 0) {
        scene->rewardMode = itmClaimFreeSlot(listedIndex);
        slot = scene->rewardMode;
        scene->padCF = (u8)listedIndex;
        if (slot < 0) {
            scene->rewardRow = (s8)func_00260C28(
                listedIndex, mnuCampChooseSolarWeightedOption(listedIndex));
            scene->remainingRewards = mnuCampChooseWeightedTableValue();
        } else {
            scene->rewardRow = (s8)func_00260C38(slot, listedIndex);
            scene->remainingRewards = (s8)mnuCampFindFreeCompactEntryIndex(
                slot, listedIndex);
        }
        scene->rewardIndex = 0;
        scene->announceNextReward = 0;
        scene->grantPendingReward = 0;
        sndSetSequenceVolumePan(0x300001, 0x7F, 0x3F);
        dspStartEntry(0x2C);
    } else {
        scene->padCF = (u8)-1;
        scene->rewardRow = -1;
        scene->remainingRewards = 0;
        scene->announceNextReward = 0;
        scene->grantPendingReward = 0;
    }
    scene->rewardDelay = 0;
    return 1;
}

u32 func_00264848(void) {
    return 1;
}

extern void ptyAdjustItemQuantity(s32, s32);
extern s32 func_00260DF0(s32);
extern u8 func_00260FE8(s32, s32);
extern u8 func_00261018(s32, s32);
extern s32 mnuCampResolveOwnedItemVariant(s32, s32);
extern s32 mnuCampGetCompactEntryId(s32, s32);
extern s32 func_002C54B0(s32);
extern u8 D_003CE5E8[];
extern char D_00437848[];
extern char D_00437850[];

s32 evtAdvancePendingRewards(KwlnTask *callbackContext) {
    char text[64];
    MenuTerminalContext *state;
    s32 *dispatchSlot;
    s32 result;
    s32 column;
    s32 sequence;

    state = (MenuTerminalContext *)kwlnTaskGetUserValue(callbackContext);
    dispatchSlot = &state->popupState;
    result = func_002C4038(&state->transitionWork, dispatchSlot,
                         EVT_DISPATCH_OPERATION_POLL, (void *)callbackContext);
    if (result == 0) {
        if (*dispatchSlot == 0 &&
            (result = evtGetMessageWindowControlState()) == 0) {
            if (state->grantPendingReward != 0) {
                if (state->rewardKind == 0) {
                    ptyAdjustItemQuantity(state->rewardValue, 1);
                } else {
                    datAddCurrencyClamped(state->rewardValue);
                }
                state->grantPendingReward = 0;
            }
            if (state->announceNextReward != 0) {
                state->announceNextReward = 0;
                dspStartEntry(0x2F);
            } else if (state->remainingRewards == 0) {
                mnuSetPopupEntryFlagged(dispatchSlot, D_003CE5E8);
            } else if (state->rewardDelay <= 0) {
                if (state->rewardMode < 0) {
                    column = func_00260DF0(state->rewardRow);
                    state->rewardKind = func_00260FE8(state->rewardRow, column);
                    state->rewardValue = mnuCampResolveOwnedItemVariant(state->rewardRow, column);
                } else {
                    state->rewardKind = func_00261018(state->rewardRow, state->rewardIndex);
                    state->rewardValue = mnuCampGetCompactEntryId(state->rewardRow, state->rewardIndex);
                }
                if (state->rewardKind == 0 && func_002C54B0(state->rewardValue) != 0) {
                    state->rewardDelay = 60;
                    sequence = 0x300003;
                } else {
                    state->rewardDelay = 30;
                    sequence = 0x300002;
                }
                sndSetSequenceVolumePan(sequence, 0x7F, 0x3F);
            } else {
                state->rewardDelay--;
                if (state->rewardDelay > 0) {
                    return 0;
                }
                if (state->rewardKind == 0) {
                    evtCopyEntryStringToActiveWindow(0, D_00435E5C + state->rewardValue * 0x19);
                    evtCopyEntryStringToActiveWindow(1, D_00437848);
                    dspStartEntry(0x2D);
                } else {
                    func_0035C860(text, D_00437850, state->rewardValue);
                    evtCopyEntryStringToActiveWindow(0, text);
                    dspStartEntry(0x2E);
                }
                state->remainingRewards--;
                state->rewardIndex++;
                if (state->remainingRewards != 0) {
                    state->announceNextReward = 1;
                }
                state->grantPendingReward = 1;
            }
        }
        result = 0;
    }
    return result;
}

s32 func_00264AB8(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    func_0025FD78((MenuTerminalContext *)stateAddress);
    mnuDrawCampCommandTransition(stateAddress);
    return evtMenuSetHandler((void *)stateAddress, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 func_00264B10(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    func_0026C900();
    return evtMenuSetHandler((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

/* Mark each newly completed slot and advance the persistent slot index. */
s32 evtAdvanceSlotFlags(void) {
    s32 advancedSlots = 0;
    while (evtGetRemainingSlotThreshold(datGameState->progressSlot + advancedSlots + 1) == 0) {
        mdlFlagSet(D_003CE14C[(datGameState->progressSlot + advancedSlots) * EVT_PROGRESS_RECORD_WORDS + EVT_PROGRESS_RECORD_WORDS]);
        advancedSlots++;
    }
    datGameState->progressSlot += advancedSlots;
    return advancedSlots;
}

u32 evtUpdateSlotAdvanceCount(KwlnTask *task) {
    u8 advancedSlots;
    s32 stateAddress;

    stateAddress = kwlnTaskGetUserValue(task);
    advancedSlots = evtAdvanceSlotFlags();
    ((MenuTerminalContext *)stateAddress)->advancedSlots = advancedSlots;
    if ((((MenuTerminalContext *)stateAddress)->unkC8 == 0) && (((MenuTerminalContext *)stateAddress)->unkCD == '\x01')) {
        dspStartEntry(0x22);
    }
    return 1;
}

u32 func_00264C58(void) {
    return 1;
}

s32 evtOpenSlotAdvancePopupWhenIdle(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    s32 *dispatchSlot = &((MenuTerminalContext *)stateAddress)->popupState;
    s32 dispatchResult = func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, dispatchSlot, EVT_DISPATCH_OPERATION_POLL, (void *)callbackContext);
    if (dispatchResult == 0) {
        if (*dispatchSlot == 0) {
            if (evtGetMessageWindowControlState() == 0) {
                mnuSetPopupEntryFlagged(dispatchSlot, D_003CE604);
            }
        }
        return 0;
    }
    return dispatchResult;
}

s32 func_00264CE0(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    func_0025FD78((MenuTerminalContext *)stateAddress);
    mnuDrawCampCommandTransition(stateAddress);
    return evtMenuSetHandler((void *)stateAddress, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 func_00264D38(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    func_0026C900();
    return evtMenuSetHandler((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

/* Raise a gate's flag and play its cue only on the first threshold crossing. */
s32 evtTriggerProgressFlagGate(s32 unusedContext) {
    EvtFlagGate *gateEntry = (EvtFlagGate *)D_003CE400;
    u32 gateIndex;
    for (gateIndex = 0; gateIndex < EVT_PROGRESS_FLAG_GATE_COUNT; gateIndex++, gateEntry++) {
        if ((u32)(datGameState->progressSlot + 1) >= (u32)gateEntry->threshold) {
            u32 progressFlag = gateEntry->flag;
            if (mdlFlagTest(progressFlag) == 0) {
                mdlFlagSet(progressFlag);
                dspStartEntry(gateEntry->cue);
                return 1;
            }
        }
    }
    return 0;
}

s32 evtShowSlotText(KwlnTask *task) {
    char formattedText[0x40];
    if (((MenuTerminalContext *)kwlnTaskGetUserValue(task))->advancedSlots > 0) {
        evtCopyEntryStringToActiveWindow(0, (const void *)(D_00435E48 + 0x11));
        func_0035C860(formattedText, D_00437840, D_003CE148[datGameState->progressSlot * EVT_PROGRESS_RECORD_WORDS]);
        evtCopyEntryStringToActiveWindow(1, formattedText);
        evtCopyEntryStringToActiveWindow(2, (const void *)(D_003C9A20[datGameState->progressSlot]));
        if (evtGetRemainingSlotThreshold(datGameState->progressSlot + 1) >= 0) {
            dspStartEntry(0x24);
        } else {
            dspStartEntry(0x25);
        }
    }
    return 1;
}

u32 func_00264EF8(void) {
    return 1;
}

s32 evtTriggerProgressGateThenOpenPopup(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    s32 *dispatchSlot = &((MenuTerminalContext *)stateAddress)->popupState;
    s32 dispatchResult = func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, dispatchSlot, EVT_DISPATCH_OPERATION_POLL, (void *)callbackContext);
    if (dispatchResult == 0) {
        if (*dispatchSlot == 0) {
            if (evtGetMessageWindowControlState() == 0) {
                if (evtTriggerProgressFlagGate(stateAddress) == 0) {
                    mnuSetPopupEntryFlagged(dispatchSlot, D_003CE620);
                }
            }
        }
        return 0;
    }
    return dispatchResult;
}

s32 func_00264F98(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    func_0025FD78((MenuTerminalContext *)stateAddress);
    mnuDrawCampCommandTransition(stateAddress);
    return evtMenuSetHandler((void *)stateAddress, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 func_00264FF0(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    func_0026C900();
    return evtMenuSetHandler((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00265038);

s32 evtIsLastSlot(s32 slotIndex) {
    s32 activeSlots = 0;
    u32 entryIndex;
    for (entryIndex = 0; entryIndex < EVT_PROGRESS_SLOT_COUNT; entryIndex++) {
        if (D_003CE1A8[entryIndex].flag != 0) {
            activeSlots++;
        }
    }
    return slotIndex + 1 == activeSlots;
}

/* Set the model flag owned by the given slot. */
void mnuSetFlagForMenuEntry(s32 index) {
    s32 flag;

    flag = D_003CE1A8[index].flag;
    if (flag != 0) {
        mdlFlagSet(flag);
    }
}


#include "kwln.h"
#include "mnu.h"
#include "dat_state.h"
#include "mnu_list.h"

extern s32 mdlFlagTest(u32);


extern void func_0026C900(void);



extern s32 evtGetMessageWindowControlState(void);


extern char D_003CE63C[];
extern char D_00437850[];
extern s32 func_00265038();
extern void mnuShopReleaseWindowSprites(s32, MenuTerminalContext *);
extern void mnuBuildEnabledCampEntryWindow();
extern s32 mnuCampHasEligibleOwnedItems();
extern void mnuShopLoadMessageResource(MenuTerminalContext *);
extern s32 mnuFirstPresentMainCharacterIndex();
extern void evtCreateEventScriptProcess();
extern void kwlnFadeOutStart(s32, s32, s32, s32);
extern void evtClearActiveFlag();
extern void evtSetBoundedDisplayValue();
extern u32 D_003CE460[];
extern char D_00437840[];
extern void mdlFlagClear();
extern void func_0026C7F8();
extern s32 mnuCampResolveProgressTierValue();
extern void dspSetActive();
extern void evtCopyEntryStringToActiveWindow(s32, const void *);
extern s32 dspStartEntry(s32);
extern void ptyAdjustItemQuantity();
extern s32 func_0035C860(char *, const char *, ...);
extern s32 evtIsLastSlot(s32);
extern void evtSetMessageWindowOptionWhenOpen(s32);
extern s32 evtStoreValueAndCaptureWindowPanelValue(s32);
struct KwlnTask;


s32 evtMenuPopulateSelectedSlotLabels(struct KwlnTask *task) {
    MenuTerminalContext *context = (MenuTerminalContext *)kwlnTaskGetUserValue(task);
    s32 slotIndex = func_00265038();
    s32 i;
    MnuProgressReward *reward;
    char text[0x40];

    if (context->unkCD != 0) {
        if (slotIndex >= 0) {
            i = 0;
            reward = D_003CE1A8[slotIndex].rewards;
            do {
                s32 value = reward->value;
                if ((reward++)->kind == 0) {
                    evtCopyEntryStringToActiveWindow(i, D_00435E5C + value * 0x19);
                } else {
                    func_0035C860(text, D_00437850, value);
                    evtCopyEntryStringToActiveWindow(i, text);
                }
                i++;
            } while (i < 3);
            if (evtIsLastSlot(slotIndex) == 0) {
                dspStartEntry(0x26);
            } else {
                dspStartEntry(0x2A);
            }
            evtSetMessageWindowOptionWhenOpen(0);
            evtStoreValueAndCaptureWindowPanelValue(0x27);
        }
    }
    return 1;
}

u32 func_002652D8(void) {
    return 1;
}

/* Poll the event window; when it closes, install the default window if needed. */
s32 evtMenuPollWindow(KwlnTask *callback) {
    MenuTerminalContext *context = (MenuTerminalContext *)kwlnTaskGetUserValue(callback);
    s32 *window = &context->popupState;
    s32 state = func_002C4038(&context->transitionWork, window, 0, (void *)callback);
    if (state == 0) {
        if (*window == 0) {
            if (evtGetMessageWindowControlState() == 0) {
                mnuSetPopupEntryFlagged(window, D_003CE63C);
            }
        }
        return 0;
    }
    return state;
}

s32 func_00265360(KwlnTask *callback) {
    s32 context = kwlnTaskGetUserValue(callback);
    func_0025FD78((MenuTerminalContext *)context);
    mnuDrawCampCommandTransition((MenuTerminalContext *)context);
    return evtMenuSetHandler((void *)context, 1, (void *)callback);
}

s32 evtFinishPopupAfterMenuConfiguration(KwlnTask *callback) {
    s32 context = kwlnTaskGetUserValue(callback);
    func_0026C7F8(1, 0);
    return evtMenuSetHandler((void *)context, 2, (void *)callback);
}

/* Hand out the captured slot's reward: an item (named in the window) or a currency amount. */
s32 func_00265408(KwlnTask *task) {
    char text[0x40];
    MenuTerminalContext *context = (MenuTerminalContext *)kwlnTaskGetUserValue(task);
    s32 slotIndex = func_00265038();
    s32 choice;
    s32 rewardValue;

    context->rewardGranted = 0;
    if (context->unkCD != 0 && slotIndex >= 0) {
        choice = evtGetCapturedWindowPanelValue();
        rewardValue = D_003CE1A8[slotIndex].rewards[choice].value;
        if (D_003CE1A8[slotIndex].rewards[choice].kind == 0) {
            evtCopyEntryStringToActiveWindow(0, D_00435E5C + rewardValue * 0x19);
            ptyAdjustItemQuantity(rewardValue, 1);
        } else {
            func_0035C860(text, D_00437850, rewardValue);
            evtCopyEntryStringToActiveWindow(0, text);
            datAddCurrencyClamped(rewardValue);
        }
        context->rewardGranted = 1;
        dspStartEntry(0x28);
    }
    return 1;
}

/* Walk the list until its selected id is found, then persist the slot choice. */
s32 evtMenuPersistSelectedSlot(KwlnTask *task) {
    MenuTerminalContext *context = (MenuTerminalContext *)kwlnTaskGetUserValue(task);
    s32 selectedId = context->ownedWindows[0]->list->cursor->camp.value;
    struct MenuListNode *node;
    MenuTerminalWindowState *record;
    s32 slot;
    mnuShopReleaseWindowSprites(1, context);
    mnuBuildEnabledCampEntryWindow(context);
    for (node = context->ownedWindows[0]->list->first;
         node != 0 && node->camp.value != selectedId; node = node->next) {
        mnuAdvanceListCursorDefault(context->ownedWindows[0]->list);
    }
    record = context->ownedWindows[0]->list->context;
    slot = mnuCampHasEligibleOwnedItems(context);
    record->selectedSlot = slot;
    context->selectedSlot = slot;
    return 1;
}

extern void mnuSetFlagForMenuEntry(s32);
extern s32 func_00261670(MenuTerminalContext *);
extern s32 func_002619A8(MenuTerminalContext *, s32);
extern s32 mnuRebuildCampSaleItemWindow(MenuTerminalContext *);

s32 func_002655C0(KwlnTask *callback) {
    MenuTerminalContext *context = (MenuTerminalContext *)kwlnTaskGetUserValue(callback);
    s32 slotIndex = func_00265038();
    s32 *popup = &context->popupState;
    s32 dispatch = func_002C4038(&context->transitionWork, popup, 0, (void *)callback);
    s32 choice;

    if (dispatch != 0) {
        return dispatch;
    }
    if (*popup == 0 && evtGetMessageWindowControlState() == 0) {
        s32 rewardState = context->rewardGranted;
        if (rewardState == 1) {
            if (evtIsLastSlot(slotIndex) == 0) {
                dspStartEntry(0x29);
            } else {
                dspStartEntry(0x2B);
            }
            context->rewardGranted = 2;
            mnuSetFlagForMenuEntry(slotIndex);
            return 0;
        }

        choice = context->ownedWindows[0]->list->cursor->camp.value + 1;
        if (context->advancedSlots != 0) {
            switch (choice) {
            case 1:
                func_00261670(context);
                mnuSetPopupEntryFlagged(popup, D_003CE4B4);
                break;
            case 2:
                func_002619A8(context, 1);
                mnuSetPopupEntryFlagged(popup, D_003CE4D0);
                break;
            case 3:
                func_002619A8(context, 3);
                mnuSetPopupEntryFlagged(popup, D_003CE4EC);
                break;
            case 4:
                mnuRebuildCampSaleItemWindow(context);
                mnuSetPopupEntryFlagged(popup, D_003CE508);
                break;
            }
        } else {
            switch (choice) {
            case 2:
                mnuSetPopupEntryFlagged(popup, D_003CE4D0);
                break;
            case 3:
                mnuSetPopupEntryFlagged(popup, D_003CE4EC);
                break;
            case 1:
                mnuSetPopupEntryFlagged(popup, D_003CE4B4);
                break;
            case 4:
                if (rewardState == 2) {
                    mnuRebuildCampSaleItemWindow(context);
                }
                mnuSetPopupEntryFlagged(popup, D_003CE508);
                break;
            }
        }
    }
    return 0;
}

s32 func_002657F8(KwlnTask *callback) {
    s32 context = kwlnTaskGetUserValue(callback);
    func_0025FD78((MenuTerminalContext *)context);
    mnuDrawCampCommandTransition((MenuTerminalContext *)context);
    return evtMenuSetHandler((void *)context, 1, (void *)callback);
}

s32 func_00265850(KwlnTask *callback) {
    s32 context = kwlnTaskGetUserValue(callback);
    func_0026C900();
    return evtMenuSetHandler((void *)context, 2, (void *)callback);
}

/* Fade out according to the event mode, with a separate flag-dependent case 2. */
s32 evtStartFadeByState(KwlnTask *task) {
    MenuTerminalContext *context = (MenuTerminalContext *)kwlnTaskGetUserValue(task);
    mnuShopLoadMessageResource(context);
    switch (context->type) {
    case 2:
        if (mdlFlagTest(0x42a) == 0 && mnuFirstPresentMainCharacterIndex() == 0) {
            evtCreateEventScriptProcess(0x323);
        } else {
            kwlnFadeOutStart(0, 0, 0, 0xf);
        }
        break;
    case 0:
    case 1:
    case 3:
        kwlnFadeOutStart(0, 0, 0, 0xf);
        break;
    }
    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(0, 0);
    evtSetBoundedDisplayValue(1, 1);
    return 1;
}

u32 func_00265980(void) {
    return 1;
}

u32 evtMenuSetProgressFlag(MenuTerminalContext *context) {
    u32 changed;
    s64 flagSet;

    if (((context->type == 2) && (flagSet = mdlFlagTest(4), flagSet != 0)) &&
          (flagSet = mdlFlagTest(0x290), flagSet == 0)) {
        mdlFlagSet(0x290);
        changed = 1;
    }
    else {
        changed = 0;
    }
    return changed;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", mnuAwardCampProgressCurrency);

/* One-shot menu flag: set the object's flag the first time it is not yet set, returning 1 only then. */
s32 func_00265A60(MenuTerminalContext *object) {
    u32 flag = D_003CE460[object->type];
    if (flag == 0) {
        return 0;
    }
    if (mdlFlagTest(flag) == 0) {
        mdlFlagSet(flag);
        return 1;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424D08);

INCLUDE_SDATA(const s32, "game/code_0025DA20", mnuShopRestoreMiddleVector);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_004377F8);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437800);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437808);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437810);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437818);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437820);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437828);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437830);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437838);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437840);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437848);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437850);

