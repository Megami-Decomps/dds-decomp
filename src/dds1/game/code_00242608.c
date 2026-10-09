#include "dat_command.h"
#include "itf_mes_window.h"
#include "fr_font.h"
#include "fr_font_context.h"
#include "kwln.h"
#include "sdf_packet_list.h"
#include "eff_resource_slots.h"
#include "evt_viewer.h"
#include "sdf_resource.h"
#include "mnu.h"
#include "mnu_list.h"
#include "mnu_shop.h"
#include "sdf.h"
#include "sdf_linked_packet.h"
#include "sdf_packet_builders.h"
#include "evt_unit.h"
#include "evt_event_pack.h"
#include "dat_state.h"
#include "fld.h"
#include "evt_solar.h"
#include "evt_task.h"
#include "kwln_task_lifecycle.h"
#include "sdf_chip.h"

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

#define CAMP_REGISTERED_ID_LIMIT 10
#define CAMP_TIMELINE_SCRIPT_REGISTER_BASE 200
#define CAMP_STATUS_BATCH_COUNT 2
#define CAMP_STATUS_INITIAL_PARAMETER 15
#define CAMP_SLOT_LAST_ROW_INDEX 4
#define CAMP_SLOT_LAST_ROW_OFFSET 0x410
#define CAMP_SLOT_ROW_STRIDE 0x104
#define CAMP_PARTY_SCAN_LAST 4
#define CAMP_PARTY_ACTIVE_FLAG 1


extern void evtLoadResourcePair(const char *, u8 *);
extern s32 evtCreateMessageWindowIfMissing(s32);
extern s32 func_00244848();
extern struct EffectSlotSet *D_003BC520;
extern void mnuUnpackNibbleFields();

extern u8 D_00368C40[];

extern s32 ptyCountBulletItem(s32);

extern s8 D_003BC39C;

extern KwlnTask *kwlnTaskFindByPriority(u32 prio);



extern char D_003BC3A0[]; /* "camp" */

extern char D_003AF418[]; /* "camp_draw" */

extern char D_003AF428[]; /* "camp_update" */

extern void evtFormatTaskName(s32 taskId, void *name);
extern void *memset(void *dst, s32 c, u32 n);
extern KwlnTask *kwlnTaskCreate(const char *name, u32 priority, s32 startDelay, s32 destroyDelay, TaskUpdate update, TaskDestroy destroy, u32 userValue);
extern f32 mnuShopSavedLastTransformVector[];
extern f32 mnuShopSavedMiddleTransformVector[];
extern f32 mnuShopSavedFirstTransformVector[];
extern s32 mnuShopRestoreMiddleVector;
extern s32 evtQueueValidatedBgmSoundCode(s32, s32);
extern s32 D_0036AA60[];

#define CAMP_TASK_PRIORITY 0x3EC


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

    while (task = kwlnTaskFindByPriority(CAMP_TASK_PRIORITY), task != NULL) {
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

extern void func_0022BFD8();

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
                func_0022BFD8(scene, track, key);
                key = track->children;
            }
        }
    }
}

void func_002429F0(EvtRuntime *world, s32 threshold, s32 delta) {
    EvtRuntimeGroup *track;
    EvtRuntimeChild *key;
    s32 end = 0;
    s32 i;
    s32 length;
    s32 offset;

    for (track = world->groups; track != NULL; track = track->next) {
        for (key = track->children; key != NULL; key = key->next) {
            for (i = 0; i < D_00368768[track->type].columns; i++) {
                switch (D_00368768[track->type].columnTypes[i]) {
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

void mnuInitializeCampPanelVisualDefaults(f32 *a, f32 *b, f32 *c, f32 *d, f32 *e) {
    a[0] = 0.7f;
    a[1] = 0.7f;
    a[2] = 0.7f;
    a[3] = 0.0f;
    b[0] = 0.65f;
    b[1] = 0.39f;
    b[2] = 0.65f;
    b[3] = 0.0f;
    c[0] = 0.2f;
    c[1] = 0.2f;
    c[2] = 0.2f;
    c[3] = 1.0f;
    d[0] = 7.0f;
    *e = 0.0f;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00242C30);

extern s32 strcmp(const char *a, const char *b);

extern s32 sdfAllocPacketAligned(s32 size);
extern void itfSendTablePacket(SdfListHead *packet, s32 table, s32 mode);
extern void itfQueueTextureBoundQuadPacket(void *, void *, void *, s32, SdfTex *, s32, SdfListHead *);
extern s32 D_00368BA8[4];
extern s32 D_00368BB8[4];
extern s32 D_00368BC8[4];
extern SdfPoolNode D_003255A8;

typedef struct CampDisplayDefaults {
    s32 x;
    s32 y;
    u8 color[4];
    f32 scaleY;
    f32 scaleX;
    s32 enabled;
    s32 variant;
} CampDisplayDefaults;

void mnuCampInitDisplayDefaults(CampDisplayDefaults *display) {
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
}

void mnuFindCampKeyTrackNeighbors(EvtRuntimeGroup *track, s32 value, EvtRuntimeChild **out1, EvtRuntimeChild **out2);
void func_00243048(EvtRuntimeChild *from, EvtRuntimeChild *to, CampDisplayDefaults *display, f32 ratio);

void func_00242E70(EvtRuntime *viewer, EvtRuntimeGroup *track, s32 value, CampDisplayDefaults *display) {
    EvtRuntimeChild *lo;
    EvtRuntimeChild *hi;
    s32 startFrame;
    s32 endFrame;
    f32 ratio;

    mnuFindCampKeyTrackNeighbors(track, value, &lo, &hi);
    if (lo != 0) {
        startFrame = lo->frame;
    } else {
        mnuCampInitDisplayDefaults(display);
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
    func_00243048(lo, hi, display, ratio);
}


void mnuInitializeCampListLayoutDefaults(EvtBlendKey *layout) {
    layout->w[0] = 0x96;
    layout->w[1] = 0x96;
    layout->x = 0x50;
    layout->w[2] = 0x96;
    layout->w[3] = 0x1E;
    layout->flagWord = 1;
    layout->y[0] = 7;
    layout->y[1] = 4;
    layout->y[2] = 0xA;
    layout->z[0] = 0x20;
    layout->z[1] = 0x10;
    layout->z[2] = 0x10;
}

void func_00242F78(EvtRuntime *viewer, EvtRuntimeGroup *track, EvtBlendKey *out) {
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
void func_00243048(EvtRuntimeChild *from, EvtRuntimeChild *to,
                   CampDisplayDefaults *display, f32 ratio) {
    if (from == NULL) {
        mnuCampInitDisplayDefaults(display);
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
        return;
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
}

extern s32 evtViewerTestIndexedCondition(u32);

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

/* Return 1 if a type-4 key's low 12-bit index yields 1 from the owner's item query. */
s32 campAnyPackedFlagSet(EvtRuntime *scene) {
    EvtRuntimeGroup *node;
    EvtRuntimeChild *child;
    s32 low;
    s32 high;

    for (node = scene->groups; node != NULL; node = node->next) {
        if (node->type == 4) {
            for (child = node->children; child != NULL; child = child->next) {
                mnuUnpackNibbleFields(child, &low, &high);
                if (itfMesGetWindowEntryItems(scene->windowContext->handle, low) == 1) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* Split the key's packed halfword into its low 12 bits and upper four bits. */
void mnuUnpackNibbleFields(EvtRuntimeChild *key, s32 *lowBitsOut, s32 *highBitsOut) {
    *lowBitsOut = key->p08.h[0] & CAMP_KEY_LOW_BITS_MASK;
    *highBitsOut = key->p08.h[0] >> CAMP_KEY_HIGH_BITS_SHIFT;
}

typedef struct CampNameLookup {
    u8 pad00[0x7C];
    char (*nameTable)[32]; /* 0x7C: fixed-width names indexed by nameIndex */
} CampNameLookup;

/* Return the scene's matching name index, or -1 when the track list has no match. */
s32 mnuCampFindMatchingEntryIndex(CampNameLookup *lookup, EvtRuntime *scene, s32 nameIndex) {
    EvtRuntimeGroup *track = scene->groups;
    while (track != NULL) {
        if (strcmp(scene->entryName[track->entryHeader],
                   lookup->nameTable[nameIndex]) == 0) {
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
    SdfListHead *packetList;

    D_00368BB8[2] = texture->width << 4;
    D_00368BB8[3] = texture->height << 4;
    halfWidth = (s32)(texture->width * display->scaleX) / 2;
    halfHeight = (s32)(texture->height * display->scaleY) / 2;
    D_00368BA8[0] = (display->x - halfWidth) << 4;
    D_00368BA8[1] = (display->y - halfHeight) << 3;
    D_00368BA8[2] = (display->x + halfWidth) << 4;
    D_00368BA8[3] = (display->y + halfHeight) << 3;
    D_00368BC8[0] = display->color[0];
    D_00368BC8[1] = display->color[1];
    D_00368BC8[2] = display->color[2];
    D_00368BC8[3] = display->color[3];
    variant = display->variant;
    if (display->color[3] != 0) {
        packetList = (SdfListHead *)sdfAllocPacketAligned(0x20);
        sdfInitPacketList(packetList);
        if (variant == 0) {
            itfSendTablePacket(packetList, 0, 0);
        } else if (variant == 1) {
            itfSendTablePacket(packetList, 1, 0);
        } else if (variant == 2) {
            itfSendTablePacket(packetList, 2, 0);
        }
        itfQueueTextureBoundQuadPacket(D_00368BA8, D_00368BB8, D_00368BC8,
                                      0xFF, texture, 0, packetList);
        D_003255A8.append(&D_003255A8, packetList);
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
void func_00243818(EvtRuntime *scene) {
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
INCLUDE_ASM(const s32, "game/code_00242608", fldApplyCameraColorKeyWords);

void func_00243A18(EvtRuntime *scene) {
    func_00243818(scene);
    if (scene->cameraColorActive == 1) {
        func_00134CD8();
        return;
    }
}

extern s32 D_00368BD8[];
void mnuCampInitFontResource(EvtRuntime *scene) {
    s32 fontHandle;
    scene->glyph = 0;
    fontHandle = (s32)(u32)func_001951C8((const char *)D_00368BD8, 0, 0, 0, 0);
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

extern SdfTex *kwlnHeldTextureReference;
extern s32 kwlnCreateHeldTextureBuffer(u16 width, u16 height, f32 value);
extern void func_00243BF0(EvtRuntime *scene);
extern void mnuShopSubmitDescriptor(EvtRuntime *scene);
extern void func_00243EC8(EvtRuntime *scene);

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
        func_00243BF0(scene);
        scene->menuState = scene->menuState + 1;
    case 4:
        mnuShopSubmitDescriptor(scene);
        scene->menuState = scene->menuState + 1;
    case 5:
        if (scene->shopFlag == 1) {
            func_00243EC8(scene);
        }
    case 0:
        break;
    default:
        scene->menuState = scene->menuState + 1;
        break;
    }
}


extern SdfPoolNode D_00325708;
extern u8 D_00325860[];

void func_00243BF0(EvtRuntime *scene) {
    s32 surface;
    SdfLinkedPacketList *context;
    s32 handle;

    if (scene->pendingResource == 0) {
        handle = (s32)sdfAllocGeneralBlockHigh(0x70000);
        scene->pendingResource = handle;
        scene->pendingWork = (s32)sdfResourceRetainAddress((struct SdfMemBlock *)(handle));
    }
    memset((void *)scene->pendingWork, 0x40, 0x70000);
    surface = (s32)(u32)sdfAllocatePacketList(0);
    context = (SdfLinkedPacketList *)sdfAllocPacketAligned(0x10);
    sdfClearLinkedPacketList(context);
    sdfCreatePatchableResourcePacket((SdfListHead *)surface, context, 0, 0, 0x200, 0xE0,
                                    scene->pendingWork, 0, 0, 0);
    sdfAppendPacketChainNode((SdfPacketChain *)D_00325860, context);
    D_00325708.append(&D_00325708, (SdfListHead *)surface);
}

void mnuShopSubmitDescriptor(EvtRuntime *scene) {
    s32 drawPacket;

    if (scene->pendingWork != 0) {
        drawPacket = (s32)(u32)sdfAllocatePacketList(0);
        sdfCreateDescriptorPacket((SdfListHead *)drawPacket,
                                  kwlnHeldTextureReference->primaryResource, 0, 0,
                                  CAMP_DESCRIPTOR_WIDTH, CAMP_DESCRIPTOR_HEIGHT,
                                  scene->pendingWork, 0);
        D_00325708.append(&D_00325708, (SdfListHead *)drawPacket);
    }
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00243D48);

void func_00243EC8(EvtRuntime *scene) {
    func_00243D48(scene->auxResource);
}

void func_00243EE0(void) {
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
void func_00243F48(EvtRuntime *scene) {
    EvtRuntimeGroup *track = scene->groups;
    s32 slotIndex;

    while (track != NULL) {
        if (track->type == 4) {
            break;
        }
        track = track->next;
    }
    if (track != NULL) {
        for (slotIndex = 0; slotIndex < CAMP_REGISTERED_ID_LIMIT; slotIndex++) {
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
void mnuShopRegisterSceneObject(EvtRuntime *scene, s32 identifier) {
    s32 registeredCount = scene->registeredCount;
    s32 idIndex = 0;
    if (registeredCount > 0) {
        s32 *idCursor = scene->registeredIds;
        do {
            if (*idCursor == identifier) {
                return;
            }
            idCursor++;
            idIndex++;
        } while (idIndex < registeredCount);
    }
    if (registeredCount < CAMP_REGISTERED_ID_LIMIT) {
        scene->registeredIds[registeredCount] = identifier;
        scene->registeredCount++;
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


typedef struct ShopBatchGraphics {
    u8 pad00[0x20];
    s32 *params; /* 0x20 */
} ShopBatchGraphics;

typedef struct ShopBatch {
    u8 pad00[8];
    ShopBatchGraphics *graphics; /* 0x08 */
} ShopBatch;

void mnuInitializeShopStatusBatches(ShopScene *scene) {
    struct EffMappedResource *batchHandle;
    ShopBatch *batchObject;
    ShopBatchGraphics *batchGraphics;
    s32 *batchParameters;
    s32 initialParameter = CAMP_STATUS_INITIAL_PARAMETER;
    scene->batchState = 0;
    batchHandle = effCreateStatusBatch(6);
    batchObject = (ShopBatch *)batchHandle;
    batchGraphics = batchObject->graphics;
    scene->batches[0] = batchHandle;
    batchParameters = batchGraphics->params;
    batchParameters[0] = initialParameter;
    batchParameters[1] = 0;
    batchParameters[2] = 0;
    batchParameters[3] = 0;
    batchParameters[4] = 0;
    batchHandle = effCreateStatusBatch(1);
    batchObject = (ShopBatch *)batchHandle;
    batchGraphics = batchObject->graphics;
    scene->batches[1] = batchHandle;
    batchParameters = batchGraphics->params;
    batchParameters[0] = initialParameter;
    batchParameters[1] = 0;
}

/* Destroy both batches and return the second destruction result. */
s32 mnuShopReleaseSceneObjects(ShopScene *scene) {
    struct EffMappedResource **batchCursor = scene->batches;
    s32 destroyResult;
    u32 batchIndex;
    for (batchIndex = 0; batchIndex < CAMP_STATUS_BATCH_COUNT; batchIndex++) {
        destroyResult = effDestroyPackedBatch(*batchCursor++);
    }
    return destroyResult;
}

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF3D0);

void mnuShopLoadSpriteAssets(ShopScene *scene) {
    struct EffectSlotSet **resource = &scene->spriteResource;
    *resource = effLoadIndexedResource("/facility/spr/shop/", D_0036AA60[0], 0);
}

INCLUDE_ASM(const s32, "game/code_00242608", mnuReleaseShopSceneSpriteResources);


s32 mnuShopHasPendingFlag(ShopScene *unused) {
    u8 *flags = datGameState->inventory.counts;
    DatItemSkillRecord *entry = datItemSkillRecords;
    s32 found = 0;
    s32 i;

    for (i = 0; i < 0xC0; flags++, i++) {
        if ((u32)(i - 0xA0) >= 0x20 && *flags != 0) {
            if ((entry->flags & 3) != 0) {
                found = 1;
                break;
            }
            if ((u32)(i - 0x60) < 0x20) {
                found = 1;
                break;
            }
        }
        entry++;
    }
    return found;
}



extern void func_0025E820();

MenuWindowContainer *mnuCreateShopListWindow(const void *unused, s32 count, ShopScene *settings) {
    MenuWindowContainer *window;
    MnuShopListContext *buffer;
    s32 i;

    if (settings->extraOption != 0) {
        count++;
    }
    window = mnuCreateWindowContainer(0, 0x260, 0x10, count, 0x15);
    mnuInitializeWindowEntryPlacement(0, window, 0, 8, 0xA);
    for (i = 0; i < count; i++) {
        mnuAppendWindowListNode(window, 0);
    }
    window->list->drawCallback = func_0025E820;
    buffer = sdfAllocSizeClassBlock(0x10);
    memset(buffer, 0, 0x10);
    window->list->context = buffer;
    buffer->extraOption = settings->extraOption;
    return window;
}


void func_002444D0(ShopScene *scene) {
    scene->sprite = mnuCreateShopListWindow(D_00368C40, 3, scene);
}

typedef struct CampFlagRow {
    s16 flag[8]; /* 0x00 */
    u8 value[9]; /* 0x10: [0] default, [i + 1] for flag[i] */
    u8 pad19;
} CampFlagRow;

extern CampFlagRow D_00368C50[];

s32 campFlagRowValue(s32 row) {
    s32 i;

    for (i = 7; i >= 0; i--) {
        if (D_00368C50[row].flag[i] > 0 && mdlFlagTest(D_00368C50[row].flag[i])) {
            return D_00368C50[row].value[i + 1];
        }
    }
    return D_00368C50[row].value[0];
}

extern u8 D_00369A88[];

/* Highest matching row wins; rows with nonpositive flags are not queried. */
s32 mnuCampFindActiveSlot(void) {
    s32 rowIndex;
    u8 *slotTable = D_00369A88;
    s16 *flagId = (s16 *)(slotTable + CAMP_SLOT_LAST_ROW_OFFSET);
    for (rowIndex = CAMP_SLOT_LAST_ROW_INDEX; rowIndex >= 0; rowIndex--, flagId = (s16 *)((u8 *)flagId - CAMP_SLOT_ROW_STRIDE)) {
        if (*flagId > 0 && mdlFlagTest(*flagId)) {
            return rowIndex;
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00244658);


extern void mnuDestroyWindowContainer(MenuWindowContainer *);

void mnuShopReleaseSprites(ShopScene *scene) {
    MenuWindowContainer **slot = &scene->sprite;
    u32 i;

    for (i = 0; i < 1; i++) {
        MenuWindowContainer *sprite = *slot;

        if (sprite->list->context != NULL) {
            sdfReleaseChipBlock(sprite->list->context);
            sprite = *slot;
            sprite->list->context = NULL;
        }
        mnuDestroyWindowContainer(sprite);
        slot++;
    }
    if (scene->window != 0) {
        mnuDestroyWindowContainer(scene->window);
    }
}

typedef struct ShopProgressPriceScale {
    f32 percent;
    u32 threshold;
} ShopProgressPriceScale;

extern ShopProgressPriceScale D_0036A234[];

s32 mnuCampGetProgressStage(void) {
    s32 result = 0;
    if (mdlFlagTest(0x970)) {
        result = 1;
    }
    if (mdlFlagTest(0x971)) {
        result = 2;
    }
    if (mdlFlagTest(0x972)) {
        result = 3;
    }
    if (mdlFlagTest(0x973)) {
        result = 4;
    }
    if (mdlFlagTest(0x974)) {
        result = 5;
    }
    return result;
}

s32 func_00244848(void) {
    u32 stage = mnuCampGetProgressStage();

    if (stage < 5) {
        if (datGameState->world.score >= D_0036A234[stage].threshold) {
            stage++;
        } else {
            stage = 0;
        }
    } else {
        stage = 0;
    }
    return stage;
}

/* Sum the active low bit across the five party entries. */
s32 mnuCountActivePartyEntries(void) {
    u16 entryFlags;
    DatPartyRecord *entryCursor;
    s32 entryCountdown;
    s32 enabledCount;

    enabledCount = 0;
    entryCountdown = CAMP_PARTY_SCAN_LAST;
    entryCursor = datGameState->party;
    do {
        entryFlags = entryCursor->flags;
        entryCursor++;
        entryCountdown--;
        enabledCount += entryFlags & CAMP_PARTY_ACTIVE_FLAG;
    } while (entryCountdown >= 0);
    return enabledCount;
}

ShopScene *mnuShopCreateScene(void) {
    struct SdfMemBlock *allocation;
    ShopScene *obj;

    allocation = sdfAllocGeneralBlock(0xB4);
    obj = (ShopScene *)sdfResourceRetainAddress(allocation);
    memset(obj, 0, 0xB4);
    obj->resourceHandle = allocation;
    mnuClearPanelTransitionState(&obj->transitionWork);
    mnuShopLoadSpriteAssets(obj);
    mnuInitializeShopStatusBatches(obj);
    evtLoadResourcePair("/facility/msg/shop/mes_data.bmd", obj->resourcePair);
    evtCreateMessageWindowIfMissing(obj->pairedHandle);
    D_003BC520 = obj->spriteResource;
    obj->count98 = func_00244848();
    obj->count8C = mnuCountActivePartyEntries();
    return obj;
}

extern void dspCloseChannel();
extern void evtReleaseResourcePairHandle();

void mnuShopDestroyScene(KwlnTask *arg) {
    ShopScene *scene = (ShopScene *)kwlnTaskGetUserValue(arg);

    if (scene != NULL) {
        mnuShopReleaseSprites(scene);
        mnuReleaseShopSceneSpriteResources(scene);
        mnuShopReleaseSceneObjects(scene);
        mnuDrainPanelTransitions(&scene->transitionWork, arg);
        dspCloseChannel();
        evtReleaseResourcePairHandle(scene->resourcePair);
        sdfReleaseResourceAllocation(scene->resourceHandle);
        D_003BC39C = 2;
    }
}

extern s32 mnuCampRunPanel0(KwlnTask *request);
extern s32 mnuCampRunPanel1(KwlnTask *request);
extern s32 mnuCampRunPanel2(KwlnTask *request);

/* Create the camp context and its three scheduler tasks (main, draw, update).
 * Optionally seed the initial selection from the caller. */

s32 mnuOpenShopSceneWithInitialSelection(s32 *initialSelection) {
    ShopScene *ctx = mnuShopCreateScene();
    s32 result;

    if (initialSelection != 0) {
        ctx->initialSelection = *initialSelection;
    }
    kwlnTaskCreate(D_003BC3A0, 0x402, 1, 1, mnuCampRunPanel0, 0, (u32)ctx);
    kwlnTaskCreate(D_003AF418, 0x2B12, 1, 1, mnuCampRunPanel1, 0, (u32)ctx);
    result = (s32)(u32)kwlnTaskCreate(D_003AF428, 0x520E, 1, 1, mnuCampRunPanel2, mnuShopDestroyScene, (u32)ctx);
    D_003BC39C = 1;
    return result;
}

void mnuCampDestroyPanelTasks(void) {
    kwlnTaskDestroyWithHierarchyByName(D_003BC3A0, 0);
    kwlnTaskDestroyWithHierarchyByName(D_003AF418, 0);
    kwlnTaskDestroyWithHierarchyByName(D_003AF428, 0);
}

s32 mnuPollTaskState(void) {
    s32 state = D_003BC39C;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        D_003BC39C = 0;
    }
    return 0;
}

extern u8 D_0036AB48[];

s32 mnuCampRunPanel0(KwlnTask *request) {
    s32 state = kwlnTaskGetUserValue(request);
    s32 *panel = (s32 *)(state + 0x54);
    mnuSetPopupEntry(panel, D_0036AB48);
    return menuRunPanel((void *)state, 0, request);
}


s32 mnuCampRunPanel1(KwlnTask *request) {
    s32 state = kwlnTaskGetUserValue(request);
    return menuRunPanel((void *)state, 1, request);
}

s32 mnuCampRunPanel2(KwlnTask *request) {
    s32 state = kwlnTaskGetUserValue(request);
    return menuRunPanel((void *)state, 2, request);
}

typedef struct ShopSourcePriceEntry {
    u16 itemId;
    u8 type;
    u8 flags;
    u16 pricePercent;
} ShopSourcePriceEntry;

typedef struct ShopSourcePriceRow {
    u16 pricePercent;
    ShopSourcePriceEntry entries[0x40];
} ShopSourcePriceRow;


extern ShopSourcePriceRow D_00368CF0[];

s32 func_00244C00(s32 index, s32 source, s32 halfPrice) {
    u32 rowIndex;
    u32 itemId;
    u32 rowPercent;
    u32 pricePercent;
    s32 itemPrice;
    s32 price;
    s32 progressStage;

    rowIndex = (u8)campFlagRowValue(source);
    itemId = D_00368CF0[rowIndex].entries[index].itemId;
    if (halfPrice == 0) {
        itemPrice = datItemSkillRecords[itemId].price;
        rowPercent = D_00368CF0[rowIndex].pricePercent;
        pricePercent = D_00368CF0[rowIndex].entries[index].pricePercent;
        if (pricePercent == 0) {
            pricePercent = rowPercent;
        }
        price = itemPrice * pricePercent / 100;
        progressStage = mnuCampGetProgressStage();
        if (progressStage != 0) {
            price = price * (s32)D_0036A234[progressStage].percent / 100;
        }
    } else {
        price = (u32)datItemSkillRecords[itemId].price >> 1;
    }
    return price;
}

s32 func_00244D10(s32 index, s32 halfPrice) {
    u32 rowOffset;
    u8 *row;
    u8 *entry;
    u32 itemId;
    u32 rowPercent;
    u32 pricePercent;
    s32 itemPrice;
    s32 price;
    s32 progressStage;

    rowOffset = (u8)mnuCampFindActiveSlot() * sizeof(ShopRankPriceRow);
    entry = D_00369A88 + index * sizeof(ShopRankPriceEntry) + rowOffset;
    row = D_00369A88 + rowOffset;
    itemId = *(u16 *)(entry + 4);
    if (halfPrice == 0) {
        itemPrice = datItemSkillRecords[itemId].price;
        rowPercent = *(u16 *)(row + 2);
        pricePercent = entry[7];
        if (pricePercent == 0) {
            pricePercent = rowPercent;
        }
        price = itemPrice * pricePercent / 100;
        progressStage = mnuCampGetProgressStage();
        if (progressStage != 0) {
            price = price * (s32)D_0036A234[progressStage].percent / 100;
        }
    } else {
        price = (u32)datItemSkillRecords[itemId].price >> 1;
    }
    return price;
}

extern const s32 D_003BC3A8[];

s32 mnuShopGetTransactionLimit(ShopScene *scene) {
    MenuWindowContainer *window = scene->window;
    CampWindowParams *parameters = &window->list->cursor->camp;
    s16 extraOption = scene->extraOption;
    s32 itemId = parameters->id;
    s32 price = parameters->price;
    s32 kind = parameters->mode;
    s32 operations[2];
    s32 operation;
    s32 limit = 0;
    s32 capacity;
    s32 available;
    s32 affordable;

    memcpy(operations, D_003BC3A8, sizeof(operations));
    if (extraOption != 0) {
        operation = scene->sprite->list->cursor->index + 1;
    } else {
        operation = operations[scene->sprite->list->cursor->index];
    }
    capacity = scene->count8C;
    switch (operation) {
    case 1:
    case 2: {
        DatGameState *globalState = datGameState;

        affordable = globalState->header.currency / price;
        limit = affordable;
        if (kind == 2) {
            available = capacity - ptyCountBulletItem(itemId);
        } else if (kind == 3) {
            available = 1 - globalState->inventory.counts[itemId];
        } else {
            available = 99 - globalState->inventory.counts[itemId];
        }
        if (available < 0) {
            available = 0;
        }
        if (available < affordable) {
            limit = available;
        }
        break;
    }
    case 3: {
        s32 quantity = datGameState->inventory.counts[itemId];

        if (quantity * price > 9999999) {
            limit = (s32)(9999999.0f / (f32)price);
        } else {
            limit = quantity;
        }
        break;
    }
    }
    return limit;
}

s32 func_00244FA0(ShopScene *context) {
    DatGameState *globalState = datGameState;
    MenuWindowContainer *itemObject = context->window;
    struct MenuList *record = itemObject->list;
    CampWindowParams *parameters = &record->cursor->camp;
    s32 itemId = parameters->id;
    s32 divisor = parameters->price;
    s32 kind = parameters->mode;
    s32 limit = globalState->header.currency / divisor;
    s32 quantity = context->count8C;
    s32 available;

    if (kind == 2) {
        available = quantity - ptyCountBulletItem(itemId);
    } else if (kind == 3) {
        available = 1 - globalState->inventory.counts[itemId];
    } else {
        available = 0x63 - globalState->inventory.counts[itemId];
    }
    if (available < 0) {
        available = 0;
    }
    if (limit == 0) {
        return -1;
    }
    if (available == 0) {
        return -2;
    }
    if (available < limit) {
        return available;
    }
    return limit;
}

void func_00245068(ShopScene *scene) {
    s32 index = 0;
    s32 capacity = scene->count8C;
    struct MenuListNode *node = scene->window->list->first;
    for (; index < scene->window->list->count; index++) {
        CampWindowParams *parameters = &node->camp;
        DatGameState *globalState = datGameState;
        s32 itemId = parameters->id;
        s32 divisor = parameters->price;
        s32 kind = parameters->mode;
        s32 affordable = globalState->header.currency / divisor;
        s32 available;
        s32 limit;
        if (kind == 2) {
            available = capacity - ptyCountBulletItem(itemId);
        } else if (kind == 3) {
            available = 1 - globalState->inventory.counts[itemId];
        } else {
            available = 99 - globalState->inventory.counts[itemId];
        }
        if (available < 0) {
            available = 0;
        }
        limit = available < affordable ? available : affordable;
        if (limit == 0) {
            node->flags48 = 1;
        }
        node = node->next;
        if (node == NULL) {
            break;
        }
    }
}


s32 mnuCampClampSceneCounter(s32 delta, ShopScene *scene) {
    s32 limit = mnuShopGetTransactionLimit(scene);
    s32 sum = scene->counter + delta;
    s32 current;
    scene->counter = sum;
    if (sum <= 0) {
        scene->counter = 1;
    }
    current = scene->counter;
    if (current >= limit) {
        scene->atLimit = 1;
        scene->counter = limit;
        current = limit;
    } else {
        scene->atLimit = 0;
    }
    return current;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00245208);

INCLUDE_ASM(const s32, "game/code_00242608", func_002453C8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00245628);

INCLUDE_ASM(const s32, "game/code_00242608", func_002457E8);

extern u32 D_0036A260[][16];
extern char (*D_003BAA84)[25];
extern void func_0025ECD0();

s32 func_00245A40(ShopScene *scene) {
    CampWindowParams *parameters;
    void *block;
    u32 value;
    s32 phase;
    s32 i;

    if (scene->window != NULL) {
        if (scene->window->list->context != NULL) {
            sdfReleaseChipBlock(scene->window->list->context);
            scene->window->list->context = NULL;
        }
        mnuDestroyWindowContainer(scene->window);
    }
    scene->window = mnuCreateWindowContainer(1, 0x260, 0x10, 8, 0x15);
    for (i = 0; i < 0xC0; i++) {
        scene->atLimit = 1;
        if ((u32)(i - 0xA0) >= 0x20 && datGameState->inventory.counts[i] != 0) {
            if ((datItemSkillRecords[i].flags & 3) != 0) {
                parameters = &mnuAppendWindowListNode(scene->window, D_003BAA84[i])->camp;
                value = (u32)datItemSkillRecords[i].price >> 1;
                parameters->id = i;
                parameters->value = value;
                parameters->price = value;
            } else if ((u32)(i - 0x60) < 0x20) {
                parameters = &mnuAppendWindowListNode(scene->window, D_003BAA84[i])->camp;
                phase = evtGetSolarPhase();
                value = D_0036A260[i - 0x60][phase];
                parameters->id = i;
                parameters->value = value;
                parameters->price = value;
            }
        }
    }
    scene->window->list->drawCallback = func_0025ECD0;
    block = sdfAllocSizeClassBlock(0x10);
    memset(block, 0, 0x10);
    scene->window->list->context = block;
    return scene->window->list->count;
}

extern s32 itfDrawBankTextWithLayoutFlags(s32, s32, s32, s32, s32, s32);

extern void frFontSetChildColors(struct FrFontGlyph *, u32);

extern void frFontDrawGlyphChain(s32, s32, s32);
void mnuQueueCampTextGlyphWithChildColor(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5) {
    s32 handle;

    if (a1 != 0) {
        handle = itfDrawBankTextWithLayoutFlags(0x970, 0xB58, 1, (u16)a0, a1, a4);
        frFontSetChildColors((struct FrFontGlyph *)(u32)handle, 0x80808040);
        frFontDrawGlyphChain(handle, 0, a5);
        frFontQueueGlyphForCurrentDrawBuffer((struct FrFontGlyph *)(u32)handle);
    }
}

#include "mnu_input.h"
#include "eff_resource_slots.h"
#include "common.h"
#include "sdf_resource.h"
#include "mnu_flag_snapshot.h"
#include "kwln.h"
#include "mnu.h"
#include "mnu_list.h"
#include "mnu_shop.h"
#include "dat_state.h"

typedef struct MenuResourceWork MenuResourceWork;

/* The dispatcher passes its last argument to entry callbacks as opaque data,
 * not as a function address. Modes select polling, primary and secondary actions. */
#define EVT_DISPATCH_OPERATION_POLL 0
#define EVT_DISPATCH_OPERATION_PRIMARY 1
#define EVT_DISPATCH_OPERATION_SECONDARY 2

#define MNU_SCENE_FLAG_PAIR_COUNT 4
#define MNU_PARTY_FLAG_PAIR_COUNT 16
#define MNU_FLAG_SNAPSHOT_BYTES 0x140

extern void func_0025DF68(s32, s32);

extern s32 func_00261760(ShopScene *);

extern void mnuStorePendingMenuCommandValue(struct MenuList *, u32);
extern void func_0025ECD0();
extern u8 D_0036AB64[];

extern void func_0025E108(ShopScene *, s32);

extern void mnuSetCommandPhase(ShopScene *, u32);
extern void func_00260570(struct MenuList *, u32);
extern void func_00260AB0(s32);
extern void func_0025F138();
extern void evtClearActiveFlag(s32);
extern s32 evtSetBoundedDisplayValue(s32, s32);
extern void func_002E96D8(s32);

extern void func_00260670(s32 context);

extern void func_0024DD78(void);
extern u8 D_0036AA68[];
extern u8 D_0036AA84[];
extern u8 D_0036AAA0[];
extern u8 D_0036AABC[];

extern s32 kwlnFadeIsActive(void);

extern s32 evtGetMessageWindowControlState(void);


extern void func_0025E308(s32, s32, s32, ShopScene *, s32, s32);
extern void mnuDrawIconTriple(s32, s32, s32, s32, s32, s32);
extern void mnuDrawIfActive(s32, s32, s32, MenuWindowContainer *, s32);
extern void func_0025F680(s32, s32, s32, ShopScene *, s32);
extern void func_0025FFC8(s32, s32, s32, ShopScene *, s32);
extern void func_00260100(ShopScene *, s32);
extern void func_00260208(s32, u32, s32, s32);
extern s32 D_003BC3D8[];

typedef struct {
    s32 values[2];
} MenuSelectionPair;

extern s32 mnuTickExtendedCommandPhase(ShopScene *);
extern void func_00260550(struct MenuList *, u32);
extern s32 mnuCampClampSceneCounter(s32, ShopScene *);
extern void func_0027C788(MenuWindowContainer *);

extern void mnuPlayInputSound(s32, s32, u32 *);
extern u8 D_0036AAF4[];
extern void mnuSelectLastListNode(struct MenuList *);


/* Wait until both the keyword fade and the dispatch callback are idle. */
s32 evtIsFadeDispatchIdle(void) {
    s32 fading = kwlnFadeIsActive();

    if (fading != 0) {
        return 0;
    }
    return evtGetMessageWindowControlState() == 0;
}

/* Install a dispatch state table and point the popup at the shared shop entry. */
#define MNU_INSTALL_STATE_TABLE(ctx, table, entry) \
    do { \
        (ctx)->stateTable = (s32)(table); \
        mnuSetPopupEntry(&(ctx)->dispatchState, (void *)(entry)); \
    } while (0)

void evtInstallStateTable(ShopScene *state) {
    if (state->menuMode == 2) {
        MNU_INSTALL_STATE_TABLE(state, D_0036AA68, D_0036AA68 + 0xC4);
    }
}

extern s32 func_00244658();
extern void func_002444D0(ShopScene *);
/* Prepare the active menu state and copy the selection into its window. */
s32 evtInitializeActiveMenuState(KwlnTask *task) {
    s32 stateAddress = kwlnTaskGetUserValue(task);
    MnuShopListContext *windowData;
    s32 pendingSelection;

    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(0, 2);
    if (((ShopScene *)stateAddress)->sprite == 0) {
        ((ShopScene *)stateAddress)->extraOption = func_00244658(stateAddress);
        func_002444D0((ShopScene *)stateAddress);
    }
    windowData = ((ShopScene *)stateAddress)->sprite->list->context;
    pendingSelection = mnuShopHasPendingFlag((ShopScene *)stateAddress);
    ((ShopScene *)stateAddress)->previousValue = datGameState->header.currency;
    windowData->pendingSelection = pendingSelection;
    ((ShopScene *)stateAddress)->pendingSelection = pendingSelection;
    return 1;
}

/* Advance command phase when the current menu state is phase one. */
s32 evtAdvancePhaseOne(KwlnTask *task) {
    ShopScene *state = (ShopScene *)kwlnTaskGetUserValue(task);

    if (state->action == 1) {
        mnuSetCommandPhase(state, 4);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF418);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF428);

INCLUDE_ASM(const s32, "game/code_00242608", func_00245DE0);

s32 evtPrimeDispatchStart(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);

    func_00260670(stateAddress);
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 evtSetupDispatchSync(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);

    func_0024DD78();
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

void evtInstallStateTableB(ShopScene *state) {
    if (state->menuMode == 1) {
        MNU_INSTALL_STATE_TABLE(state, D_0036AA84, D_0036AA84 + 0xA8);
    }
}

s32 func_00246160(KwlnTask *task) {
    ShopScene *state = (ShopScene *)kwlnTaskGetUserValue(task);

    if (state->action == 1) {
        func_002453C8(state);
    }
    return 1;
}

/* Map actions five and seven to their menu phases, then reset substate. */
s32 evtSelectStateAction(KwlnTask *task) {
    s32 stateAddress = kwlnTaskGetUserValue(task);
    s32 action = ((ShopScene *)stateAddress)->action;

    if (action == 5) {
        mnuSetCommandPhase((ShopScene *)stateAddress, 3);
    } else if (action == 7) {
        struct MenuList *linkedTask;

        mnuSetCommandPhase((ShopScene *)stateAddress, 9);
        linkedTask = ((ShopScene *)stateAddress)->window->list;
        linkedTask->drawCallback = func_0025F138;
        func_00260570(linkedTask, 10);
    }
    ((ShopScene *)stateAddress)->substate = 0;
    return 1;
}

/* Poll the dispatch slot, then process command phases and mapped input flags. */
s32 func_00246220(KwlnTask *task) {
    s32 dispatchResult;
    s32 inputFlags;
    s32 stateAddress;
    s32 *dispatchSlot;
    struct MenuList *linkedTask;
    struct MenuListNode *cursorNode;
    struct MenuList *callbackTask;
    s32 inputConsumed = 0;

    stateAddress = kwlnTaskGetUserValue(task);
    inputFlags = mnuMapPadMaskToFlags(0x33);
    dispatchSlot = &((ShopScene *)stateAddress)->dispatchState;
    linkedTask = ((ShopScene *)stateAddress)->window->list;
    cursorNode = linkedTask->cursor;
    dispatchResult = menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_POLL, task);
    if (dispatchResult == 0) {
        switch (mnuTickExtendedCommandPhase((ShopScene *)stateAddress)) {
        case -1:
            break;
        case 4:
            mnuSetCommandPhase((ShopScene *)stateAddress, 6);
            mnuStorePendingMenuCommandValue(linkedTask, 10);
            break;
        case 5:
            mnuSetPopupEntryFlagged(dispatchSlot, (void *)D_0036AA68);
            mnuStorePendingMenuCommandValue(((ShopScene *)stateAddress)->sprite->list, 10);
            break;
        case 7:
            mnuSetPopupEntryFlagged(dispatchSlot, (void *)D_0036AAF4);
            break;
        case 8:
            mnuSetCommandPhase((ShopScene *)stateAddress, 6);
            callbackTask = ((ShopScene *)stateAddress)->window->list;
            callbackTask->drawCallback = func_0025ECD0;
            mnuStorePendingMenuCommandValue(callbackTask, 0);
            ((ShopScene *)stateAddress)->substate = 10;
            break;
        case 6:
            evtInstallStateTableB((ShopScene *)stateAddress);
            /* Continue into the common input handling after installing the table. */
        default:
            if (((ShopScene *)stateAddress)->dispatchState == 0) {
                if (inputFlags & 1) {
                    if ((cursorNode->flags48 & 1) == 0) {
                        mnuSetCommandPhase((ShopScene *)stateAddress, 7);
                    } else {
                        if (func_00244FA0((ShopScene *)stateAddress) == -2) {
                            ((ShopScene *)stateAddress)->menuMode = 1;
                        }
                        inputConsumed = 1;
                    }
                } else if (inputFlags & 2) {
                    mnuSetCommandPhase((ShopScene *)stateAddress, 5);
                    func_00260550(linkedTask, 4);
                } else if ((inputFlags & 0x300000) == 0) {
                    func_0027C788(((ShopScene *)stateAddress)->window);
                } else if (inputFlags & 0x10) {
                    mnuRetreatWindowListSelection(((ShopScene *)stateAddress)->window);
                } else if (inputFlags & 0x20) {
                    mnuAdvanceWindowListSelection(((ShopScene *)stateAddress)->window);
                }
            }
            mnuPlayInputSound(0, inputConsumed ? 2 : inputFlags,
                              &((ShopScene *)stateAddress)->window->list->stateFlags);
            break;
        }
        return 0;
    }
    return dispatchResult;
}

s32 evtStageDispatchStart(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);

    func_00260AB0(stateAddress);
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 evtSetupDispatchSyncB(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);

    func_0024DD78();
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

void evtInstallStateTableC(ShopScene *state) {
    if (state->menuMode == 1) {
        MNU_INSTALL_STATE_TABLE(state, D_0036AAA0, D_0036AAA0 + 0x8C);
    }
}

s32 func_00246538(KwlnTask *task) {
    ShopScene *state = (ShopScene *)kwlnTaskGetUserValue(task);

    if (state->action == 1) {
        func_002457E8(state);
    }
    return 1;
}

s32 evtSelectStateActionB(KwlnTask *task) {
    s32 stateAddress = kwlnTaskGetUserValue(task);
    s32 action = ((ShopScene *)stateAddress)->action;

    if (action == 5) {
        mnuSetCommandPhase((ShopScene *)stateAddress, 3);
    } else if (action == 7) {
        struct MenuList *linkedTask;

        mnuSetCommandPhase((ShopScene *)stateAddress, 9);
        linkedTask = ((ShopScene *)stateAddress)->window->list;
        linkedTask->drawCallback = func_0025F138;
        func_00260570(linkedTask, 10);
    }
    ((ShopScene *)stateAddress)->substate = 0;
    return 1;
}


/* Poll the dispatch slot, then process command phases and mapped input flags. */
s32 func_002465F8(KwlnTask *task) {
    s32 dispatchResult;
    s32 inputFlags;
    s32 stateAddress;
    s32 *dispatchSlot;
    struct MenuList *linkedTask;
    struct MenuListNode *cursor;
    struct MenuList *callbackTask;
    s32 handled = 0;

    stateAddress = kwlnTaskGetUserValue(task);
    inputFlags = mnuMapPadMaskToFlags(0x33);
    dispatchSlot = &((ShopScene *)stateAddress)->dispatchState;
    linkedTask = ((ShopScene *)stateAddress)->window->list;
    cursor = linkedTask->cursor;
    dispatchResult = menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_POLL, task);
    if (dispatchResult == 0) {
        switch (mnuTickExtendedCommandPhase((ShopScene *)stateAddress)) {
        case -1:
            break;
        case 4:
            mnuSetCommandPhase((ShopScene *)stateAddress, 6);
            mnuStorePendingMenuCommandValue(linkedTask, 10);
            break;
        case 5:
            mnuSetPopupEntryFlagged(dispatchSlot, (void *)D_0036AA68);
            mnuStorePendingMenuCommandValue(((ShopScene *)stateAddress)->sprite->list, 10);
            break;
        case 7:
            mnuSetPopupEntryFlagged(dispatchSlot, (void *)D_0036AAF4);
            break;
        case 8:
            mnuSetCommandPhase((ShopScene *)stateAddress, 6);
            callbackTask = ((ShopScene *)stateAddress)->window->list;
            callbackTask->drawCallback = func_0025ECD0;
            mnuStorePendingMenuCommandValue(callbackTask, 0);
            ((ShopScene *)stateAddress)->substate = 10;
            break;
        case 6:
            evtInstallStateTableC((ShopScene *)stateAddress);
            /* Continue into the common input handling after installing the table. */
        default:
            if (((ShopScene *)stateAddress)->dispatchState == 0) {
                if (inputFlags & 1) {
                    if ((cursor->flags48 & 1) == 0) {
                        mnuSetCommandPhase((ShopScene *)stateAddress, 7);
                    } else {
                        if (func_00244FA0((ShopScene *)stateAddress) == -2) {
                            ((ShopScene *)stateAddress)->menuMode = 1;
                        }
                        handled = 1;
                    }
                } else if (inputFlags & 2) {
                    mnuSetCommandPhase((ShopScene *)stateAddress, 5);
                    func_00260550(linkedTask, 4);
                } else if ((inputFlags & 0x300000) == 0) {
                    func_0027C788(((ShopScene *)stateAddress)->window);
                } else if (inputFlags & 0x10) {
                    mnuRetreatWindowListSelection(((ShopScene *)stateAddress)->window);
                } else if (inputFlags & 0x20) {
                    mnuAdvanceWindowListSelection(((ShopScene *)stateAddress)->window);
                }
            }
            mnuPlayInputSound(0, handled ? 2 : inputFlags, &((ShopScene *)stateAddress)->window->list->stateFlags);
            break;
        }
        return 0;
    }
    return dispatchResult;
}

s32 evtStageDispatchStartB(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);

    func_00260AB0(stateAddress);
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 evtSetupDispatchSyncC(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);

    func_0024DD78();
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

void evtInstallStateTableD(ShopScene *state) {
    if (state->menuMode == 2) {
        MNU_INSTALL_STATE_TABLE(state, D_0036AABC, D_0036AABC + 0x70);
    }
}

s32 evtEnableStateFlag(KwlnTask *task) {
    s32 stateAddress = kwlnTaskGetUserValue(task);

    if ((((ShopScene *)stateAddress)->action == 1) && (func_00245A40((ShopScene *)stateAddress) == 0)) {
        ((ShopScene *)stateAddress)->menuMode = 2;
    }
    return 1;
}

s32 evtSelectStateActionC(KwlnTask *task) {
    s32 stateAddress = kwlnTaskGetUserValue(task);
    s32 action = ((ShopScene *)stateAddress)->action;

    if (action == 5) {
        mnuSetCommandPhase((ShopScene *)stateAddress, 3);
    } else if (action == 7) {
        struct MenuList *linkedTask;

        mnuSetCommandPhase((ShopScene *)stateAddress, 9);
        linkedTask = ((ShopScene *)stateAddress)->window->list;
        linkedTask->drawCallback = func_0025F138;
        func_00260570(linkedTask, 10);
    }
    ((ShopScene *)stateAddress)->substate = 0;
    return 1;
}

/* Poll the dispatch slot, then process command phases and mapped input flags. */
s32 func_002469F0(KwlnTask *task) {
    s32 dispatchResult;
    s32 inputFlags;
    s32 stateAddress;
    s32 *dispatchSlot;
    struct MenuList *linkedTask;
    struct MenuList *callbackTask;

    stateAddress = kwlnTaskGetUserValue(task);
    inputFlags = mnuMapPadMaskToFlags(0x33);
    dispatchSlot = &((ShopScene *)stateAddress)->dispatchState;
    linkedTask = ((ShopScene *)stateAddress)->window->list;
    dispatchResult = menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_POLL, task);
    if (dispatchResult == 0) {
        switch (mnuTickExtendedCommandPhase((ShopScene *)stateAddress)) {
        case -1:
            break;
        case 4:
            mnuSetCommandPhase((ShopScene *)stateAddress, 6);
            mnuStorePendingMenuCommandValue(linkedTask, 10);
            break;
        case 5:
            mnuSetPopupEntryFlagged(dispatchSlot, (void *)D_0036AA68);
            mnuStorePendingMenuCommandValue(((ShopScene *)stateAddress)->sprite->list, 10);
            break;
        case 7:
            mnuSetPopupEntryFlagged(dispatchSlot, (void *)D_0036AAF4);
            break;
        case 8:
            mnuSetCommandPhase((ShopScene *)stateAddress, 6);
            callbackTask = ((ShopScene *)stateAddress)->window->list;
            callbackTask->drawCallback = func_0025ECD0;
            mnuStorePendingMenuCommandValue(callbackTask, 0);
            ((ShopScene *)stateAddress)->substate = 10;
            break;
        case 6:
            evtInstallStateTableD((ShopScene *)stateAddress);
            /* Continue into the common input handling after installing the table. */
        default:
            if (((ShopScene *)stateAddress)->dispatchState == 0) {
                if (inputFlags & 1) {
                    mnuSetCommandPhase((ShopScene *)stateAddress, 7);
                } else if (inputFlags & 2) {
                    mnuSetCommandPhase((ShopScene *)stateAddress, 5);
                    func_00260550(linkedTask, 4);
                } else if ((inputFlags & 0x300000) == 0) {
                    func_0027C788(((ShopScene *)stateAddress)->window);
                } else if (inputFlags & 0x10) {
                    mnuRetreatWindowListSelection(((ShopScene *)stateAddress)->window);
                } else if (inputFlags & 0x20) {
                    mnuAdvanceWindowListSelection(((ShopScene *)stateAddress)->window);
                }
            }
            mnuPlayInputSound(0, inputFlags, &((ShopScene *)stateAddress)->window->list->stateFlags);
            break;
        }
        return 0;
    }
    return dispatchResult;
}

s32 evtStageDispatchStartC(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);

    func_00260AB0(stateAddress);
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 evtSetupDispatchSyncD(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);

    func_0024DD78();
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

u32 evtResetStateProgressTimer(KwlnTask *task) {
    s32 stateAddress;

    stateAddress = kwlnTaskGetUserValue(task);
    ((ShopScene *)stateAddress)->progressTicks = 0;
    mnuSelectLastListNode(((ShopScene *)stateAddress)->sprite->list);
    return 1;
}

/* Keep the dispatch query alive for 20 idle ticks before restarting its table. */
s32 evtQueryStateProgress(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    s32 dispatchResult = menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_POLL, callbackContext);

    if (dispatchResult == 0) {
        if ((((ShopScene *)stateAddress)->dispatchState == 0) && (evtGetMessageWindowControlState() == 0)) {
            s32 progressTicks = ((ShopScene *)stateAddress)->progressTicks;

            if ((f32)progressTicks < 20.0f) {
                ((ShopScene *)stateAddress)->progressTicks = progressTicks + 1;
            } else {
                mnuSetPopupEntry(&((ShopScene *)stateAddress)->dispatchState, D_0036AB64);
            }
        }
        dispatchResult = 0;
    }
    return dispatchResult;
}

s32 evtFetchDispatchStart(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);

    func_0025E108((ShopScene *)stateAddress, ((ShopScene *)stateAddress)->progressTicks);
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 evtSetupDispatchSyncE(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);

    func_0024DD78();
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

s32 evtApplyBaseRateProgressStep(KwlnTask *task) {
    ShopScene *state = (ShopScene *)kwlnTaskGetUserValue(task);

    state->counter = 1;
    mnuCampClampSceneCounter(-1, state);
    return 1;
}

s32 evtAdvanceStateStage(KwlnTask *task) {
    s32 stateAddress = kwlnTaskGetUserValue(task);

    if (((ShopScene *)stateAddress)->action == 0xA) {
        struct MenuList *linkedTask;

        ((ShopScene *)stateAddress)->substate = 0xA;
        mnuSetCommandPhase((ShopScene *)stateAddress, 6);
        linkedTask = ((ShopScene *)stateAddress)->window->list;
        linkedTask->drawCallback = func_0025ECD0;
        mnuStorePendingMenuCommandValue(linkedTask, 0);
    }
    return 1;
}

extern s32 mnuTickCommandWaitPhase(ShopScene *);
extern void func_00260590(struct MenuList *, u32);
extern void sndSetSequenceVolumePan(s32, s32, s32);
extern u8 D_0036AB10[];

s32 evtPollQuantitySelection(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    s32 *dispatchSlot = &((ShopScene *)stateAddress)->dispatchState;
    s32 previousQuantity = ((ShopScene *)stateAddress)->counter;
    s32 input = mnuMapPadMaskToFlags(0xF000F3);
    s32 result = menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_POLL, callbackContext);

    if (result != 0) {
        return result;
    }
    switch (mnuTickCommandWaitPhase((ShopScene *)stateAddress)) {
    case -1:
        break;
    case 9:
        mnuSetCommandPhase((ShopScene *)stateAddress, 11);
        break;
    case 10: {
        struct MenuList *primaryTask = ((ShopScene *)stateAddress)->sprite->list;
        mnuSetPopupEntryFlagged(dispatchSlot, (void *)(D_0036AA84 + primaryTask->cursor->index * 0x1C));
        break;
    }
    case 12:
        mnuSetPopupEntryFlagged(dispatchSlot, (void *)D_0036AB10);
        break;
    case 11:
    default:
        if (((ShopScene *)stateAddress)->dispatchState == 0) {
            if (input & 1) {
                mnuSetCommandPhase((ShopScene *)stateAddress, 12);
            } else if (input & 2) {
                mnuSetCommandPhase((ShopScene *)stateAddress, 10);
                func_00260590(((ShopScene *)stateAddress)->window->list, 10);
            }
            if (input & 0x300030) {
                if (input & 0x10) {
                    mnuCampClampSceneCounter(1, (ShopScene *)stateAddress);
                } else if (input & 0x20) {
                    mnuCampClampSceneCounter(-1, (ShopScene *)stateAddress);
                }
                if (previousQuantity == ((ShopScene *)stateAddress)->counter) {
                    input &= 3;
                }
            } else if (input & 0xC0) {
                if (input & 0x80) {
                    mnuCampClampSceneCounter(10, (ShopScene *)stateAddress);
                } else if (input & 0x40) {
                    mnuCampClampSceneCounter(-10, (ShopScene *)stateAddress);
                }
                if (previousQuantity == ((ShopScene *)stateAddress)->counter) {
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

s32 evtAlignDispatchStart(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);

    func_00261760((ShopScene *)stateAddress);
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 evtSetupDispatchSyncF(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);

    func_0024DD78();
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

extern void func_003014F0(char *, const char *, ...);
extern char D_003BC3B8[];
extern void evtCopyEntryStringToActiveWindow(s32 window, char *text);
extern void evtSetMessageWindowOptionWhenOpen(s32 option);
extern void evtStoreValueAndCaptureWindowPanelValue(s32 value);

/* Format the buy/sell price in the message window; sells over the currency cap use the overflow message. */
INCLUDE_SDATA(const s32, "game/code_00242608", mnuShopRestoreMiddleVector);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC388);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC390);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC398);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC39C);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC3A0);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC3A8);

s32 func_00247190(KwlnTask *task) {
    ShopScene *scene = (ShopScene *)kwlnTaskGetUserValue(task);
    char text[16];
    s32 options[2] = {1, 3};
    CampWindowParams *values = &scene->window->list->cursor->camp;
    s32 operation;
    s32 message;

    if (scene->extraOption != 0) {
        operation = scene->sprite->list->cursor->index + 1;
    } else {
        operation = options[scene->sprite->list->cursor->index];
    }
    message = 7;
    scene->previousValue = datGameState->header.currency;
    scene->elapsedFrames = 0;
    if (operation == 3) {
        message = (u32)(values->value * scene->counter + datGameState->header.currency) > 9999999 ? 0xC : 7;
    }
    func_003014F0(text, D_003BC3B8, values->value * scene->counter);
    evtCopyEntryStringToActiveWindow(3, text);
    dspSetActive(1);
    dspStartEntry(message);
    evtSetMessageWindowOptionWhenOpen(0);
    evtStoreValueAndCaptureWindowPanelValue(0xB);
    scene->window->list->drawCallback = func_0025F138;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002472D8);

void evtAccumulateStateScore(s32 stateAddress) {
    CampWindowParams *values = &((ShopScene *)stateAddress)->window->list->cursor->camp;

    if ((u32)(values->id - 0x60) < 0x20) {
        datGameState->world.score += values->value * ((ShopScene *)stateAddress)->counter;
    }
}

extern s32 datAddCurrencyClamped(s32);
extern u32 func_002CD800(u32);
extern void mdlFlagSet(s32);

/* Apply the selected buy/sell operation, then restore the row's displayed price. */
INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC3B8);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC3C0);

s32 func_00247420(KwlnTask *task) {
    ShopScene *scene;
    CampWindowParams *values;
    s32 itemId;
    s32 operation;

    scene = (ShopScene *)kwlnTaskGetUserValue(task);
    {
        s32 options[2] = {1, 3};
        values = &scene->window->list->cursor->camp;
        itemId = values->id;
        if (scene->extraOption != 0) {
            operation = scene->sprite->list->cursor->index + 1;
        } else {
            operation = options[scene->sprite->list->cursor->index];
        }
        switch (operation) {
        case 1:
        case 2:
            datAddCurrencyClamped(-values->value * scene->counter);
            if (values->mode == 1) {
                func_002CD800((u32)itemId & 0xFFFF);
            } else {
                datGameState->inventory.counts[itemId] += scene->counter;
            }
            break;
        case 3:
            datAddCurrencyClamped(values->value * scene->counter);
            datGameState->inventory.counts[itemId] -= scene->counter;
            if (itemId == 0x6C) {
                mdlFlagSet(0x97F);
            }
            break;
        }
        values->value = values->price;
        return 1;
    }
}


extern s32 D_003BC3D0[];
extern s8 evtGetCapturedWindowPanelValue(void);

s32 func_00247588(KwlnTask *task) {
    ShopScene *state = (ShopScene *)kwlnTaskGetUserValue(task);
    MenuSelectionPair options;
    s32 *dispatchSlot = &state->dispatchState;
    s32 result;
    s32 selectedOperation;

    memcpy(&options, D_003BC3D0, sizeof(options));
    result = func_00285670(&state->transitionWork, dispatchSlot, 0, task);

    if (result != 0) {
        return result;
    }
    if (*dispatchSlot == 0 && evtGetMessageWindowControlState() == 0) {
        if (state->extraOption != 0) {
            selectedOperation = state->sprite->list->cursor->index + 1;
        } else {
            selectedOperation = options.values[state->sprite->list->cursor->index];
        }
        if (evtGetCapturedWindowPanelValue() == 0) {
            func_00247420(task);
            switch (selectedOperation) {
            case 1:
            case 2:
                func_00245068(state);
                break;
            case 3:
                evtAccumulateStateScore((s32)state);
                break;
            }
        }
        /* Operation two is the additional option; the baseline pair is one/three. */
        switch (selectedOperation) {
        case 2:
            mnuSetPopupEntryFlagged(dispatchSlot, (void *)D_0036AAA0);
            break;
        case 1:
            mnuSetPopupEntryFlagged(dispatchSlot, (void *)D_0036AA84);
            break;
        case 3:
            mnuSetPopupEntryFlagged(dispatchSlot, (void *)D_0036AABC);
            break;
        }
    }
    return 0;
}


s32 func_00247728(KwlnTask *task) {
    ShopScene *state = (ShopScene *)kwlnTaskGetUserValue(task);
    s32 selectedIndex;
    MenuSelectionPair options = *(MenuSelectionPair *)D_003BC3D8;

    if (state->extraOption != 0) {
        selectedIndex = state->sprite->list->cursor->index;
    } else {
        selectedIndex = options.values[state->sprite->list->cursor->index];
    }

    func_0025E308(0, 0, 0, state, 0x100, 0x53);
    func_00260100(state, 0xA09DC380);
    mnuDrawIconTriple(0, 0, 0, 0, 0x100, 0x53);
    func_0025F680(0, 0, 0, state, 0x53);
    mnuDrawIfActive(0, 0, 0, state->window, 0x53);
    func_0025FFC8(0, 0, 0, state, 0x53);

    switch (selectedIndex) {
    case 0:
    case 1:
        func_00260208((s32)state, 0x100, 1, 0x53);
        break;
    case 2:
        func_00260208((s32)state, 0x100, 0, 0x53);
        break;
    }
    return func_00285670(&state->transitionWork, &state->dispatchState, 1, task);
}

s32 evtSetupDispatchSyncG(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);

    func_0024DD78();
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

extern void dspStartEntry();

s32 evtPlayDispatchModeCue(KwlnTask *task) {
    ShopScene *state = (ShopScene *)kwlnTaskGetUserValue(task);
    dspSetActive(1);
    switch (state->menuMode) {
    case 1:
        dspStartEntry(5);
        break;
    case 2:
        dspStartEntry(6);
        break;
    }
    return 1;
}


extern u8 D_0036AA68[];

extern void mnuSetCommandPhase(ShopScene *, u32);

s32 evtApplyDispatchModeState(KwlnTask *task) {
    ShopScene *state = (ShopScene *)kwlnTaskGetUserValue(task);
    switch (state->menuMode) {
    case 1:
        mnuSetCommandPhase(state, 6);
        break;
    case 2:
        if (state->stateTable != (s32)D_0036AA68) {
            mnuSetCommandPhase(state, 5);
        }
        break;
    }
    state->menuMode = 0;
    return 1;
}

/* After idle dispatch, resume the state table at its saved position. */
s32 evtSetPopupEntryWhenMessageWindowIdle(KwlnTask *callbackContext) {
    s32 stateAddress;
    s32 result;
    s32 *dispatchSlot;

    stateAddress = kwlnTaskGetUserValue(callbackContext);
    dispatchSlot = &((ShopScene *)stateAddress)->dispatchState;
    result = menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_POLL, callbackContext);
    if (result == 0) {
        if ((*dispatchSlot == 0) && (result = evtGetMessageWindowControlState(), result == 0)) {
            mnuSetPopupEntryFlagged(dispatchSlot, (void *)(u32)(((ShopScene *)stateAddress)->stateTable));
        }
        result = 0;
    }
    return result;
}

extern void mnuDrawStatusIconAndCompanion(s32, s32, s32, ShopScene *, s32, s32);
extern void func_0025E6B0(s32, s32, s32, ShopScene *, s32, s32);
extern void func_0025FD50(s32, s32, s32, ShopScene *, s32);
extern void mnuDrawListChildrenWithCountdown(s32, s32, s32, struct MenuList *, s32);
extern void mnuDrawIconFixedEntryWithBadge(s32, s32, s32, s32, s32, s32);

s32 func_00247A78(KwlnTask *callbackContext) {
    ShopScene *state = (ShopScene *)kwlnTaskGetUserValue(callbackContext);

    switch (state->menuMode) {
    case 1:
        func_0025E308(0, 0, 0, state, 0x100, 0x53);
        mnuDrawIconTriple(0, 0, 0, 0, 0x100, 0x53);
        func_0025E6B0(0, 0, 0, state, 0x100, 0x53);
        mnuDrawIfActive(0, 0, 0, state->window, 0x53);
        func_0025FD50(0, 0, 0, state, 0x53);
        func_00260100(state, 0xA09DC380);
        func_00260208((s32)state, 0x100, 3, 0x53);
        break;
    case 2:
        if (state->stateTable == (s32)D_0036AA68) {
            func_0025E308(0, 0, 0, state, 0x100, 0x53);
            mnuDrawStatusIconAndCompanion(0, 0, 0, state, 0x100, 0x53);
            mnuDrawListChildrenWithCountdown(0, 0, 0, state->sprite->list, 0x53);
            func_00260100(state, 0xA09DC380);
            func_00260208((s32)state, 0x100, 4, 0x53);
        } else {
            func_0025E308(0, 0, 0, state, 0x100, 0x53);
            mnuDrawIconTriple(0, 0, 0, 0, 0x100, 0x53);
            func_0025E6B0(0, 0, 0, state, 0x100, 0x53);
            mnuClearWindowPanelTransitionFlag(state->sprite);
            mnuDrawIconFixedEntryWithBadge(0, 0, 0, (s32)state, 0x100, 0x53);
            func_00260100(state, 0xA09DC380);
            func_00260208((s32)state, 0x100, 2, 0x53);
        }
        break;
    }
    return menuRunPanel(state, EVT_DISPATCH_OPERATION_PRIMARY, callbackContext);
}

s32 evtSetupDispatchSyncH(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);

    func_0024DD78();
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

u32 evtStartScriptFadeAndResetDisplayFlags(void) {
    evtCreateEventScriptProcess(0x323);
    kwlnFadeOutStart(0, 0, 0, 0xf);
    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(0, 0);
    evtSetBoundedDisplayValue(1, 1);
    return 1;
}

u32 func_00247D50(void) {
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF528);

INCLUDE_ASM(const s32, "game/code_00242608", func_00247D58);

s32 evtRefreshDispatchStart(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);
    s32 progressTicks = ((ShopScene *)stateAddress)->progressTicks;

    if (progressTicks != 0) {
        func_0025DF68(stateAddress, progressTicks);
    }
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 evtSetupDispatchSyncI(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);

    func_0024DD78();
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

u32 evtResetStateFlags(void) {
    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(0, 1);
    evtSetBoundedDisplayValue(1, 0);
    func_002E96D8(0x300000);
    return 1;
}

u32 evtStartFadeOut(void) {
    kwlnFadeOutStart(0, 0, 0, 0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002481A0);

/* Select the primary entry action with opaque callback data, not a callback address. */
s32 evtDispatchStart(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);

    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

/* Select the secondary entry action with the same opaque callback context. */
s32 evtDispatchSync(KwlnTask *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue(callbackContext);

    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

u32 func_002482B0(void) {
    return 1;
}

u32 func_002482B8(void) {
    return 1;
}

u32 func_002482C0(void) {
    return 0;
}

u32 func_002482C8(void) {
    return 0;
}

u32 func_002482D0(void) {
    return 0;
}

typedef struct FlagEntry {
    s32 firstFlag;
    s32 firstOn;
    s32 secondFlag;
    s32 secondOn;
} FlagEntry;

extern void mdlFlagSet(s32);

extern s32 mdlFlagTest(s32);

typedef struct FlagPair {
    s32 first;
    s32 second;
} FlagPair;

typedef struct FlagSource {
    s32 first;
    s32 second;
    s32 pad[2];
} FlagSource;

extern FlagSource mnuSceneFlagEventEntries[];
extern FlagPair mnuPartyFlagEventEntries[];

/* Snapshot four primary flag pairs and sixteen extra pairs for restoration. */
struct SdfMemBlock *mnuCreateFlagEntries(void) {
    struct SdfMemBlock *snapshotHandle = sdfAllocGeneralBlock(MNU_FLAG_SNAPSHOT_BYTES);
    FlagEntry *snapshot = (FlagEntry *)sdfResourceRetainAddress(snapshotHandle);
    u32 pairIndex;

    for (pairIndex = 0; pairIndex < MNU_SCENE_FLAG_PAIR_COUNT; pairIndex++) {
        snapshot[pairIndex].firstFlag = mnuSceneFlagEventEntries[pairIndex].first;
        snapshot[pairIndex].firstOn = mdlFlagTest(snapshot[pairIndex].firstFlag);
        snapshot[pairIndex].secondFlag = mnuSceneFlagEventEntries[pairIndex].second;
        snapshot[pairIndex].secondOn = mdlFlagTest(snapshot[pairIndex].secondFlag);
    }
    for (pairIndex = 0; pairIndex < MNU_PARTY_FLAG_PAIR_COUNT; pairIndex++) {
        snapshot[MNU_SCENE_FLAG_PAIR_COUNT + pairIndex].firstFlag = mnuPartyFlagEventEntries[pairIndex].first;
        snapshot[MNU_SCENE_FLAG_PAIR_COUNT + pairIndex].firstOn = mdlFlagTest(snapshot[MNU_SCENE_FLAG_PAIR_COUNT + pairIndex].firstFlag);
        snapshot[MNU_SCENE_FLAG_PAIR_COUNT + pairIndex].secondFlag = mnuPartyFlagEventEntries[pairIndex].second;
        snapshot[MNU_SCENE_FLAG_PAIR_COUNT + pairIndex].secondOn = mdlFlagTest(snapshot[MNU_SCENE_FLAG_PAIR_COUNT + pairIndex].secondFlag);
    }
    return snapshotHandle;
}

/* Restore only those flags that were enabled in the saved snapshot. */
void mnuApplyFlagEntries(struct SdfMemBlock *snapshotHandle) {
    FlagEntry *snapshot = (FlagEntry *)sdfResourceRetainAddress(snapshotHandle);
    u32 pairIndex;

    /* Scene pairs restore only their second flag; party pairs restore both. */
    for (pairIndex = 0; pairIndex < MNU_SCENE_FLAG_PAIR_COUNT; pairIndex++) {
        if (snapshot[pairIndex].secondOn != 0) {
            mdlFlagSet(snapshot[pairIndex].secondFlag);
        }
    }
    for (pairIndex = 0; pairIndex < MNU_PARTY_FLAG_PAIR_COUNT; pairIndex++) {
        if (snapshot[MNU_SCENE_FLAG_PAIR_COUNT + pairIndex].firstOn != 0) {
            mdlFlagSet(snapshot[MNU_SCENE_FLAG_PAIR_COUNT + pairIndex].firstFlag);
        }
        if (snapshot[MNU_SCENE_FLAG_PAIR_COUNT + pairIndex].secondOn != 0) {
            mdlFlagSet(snapshot[MNU_SCENE_FLAG_PAIR_COUNT + pairIndex].secondFlag);
        }
    }
}

extern char D_003AF590[];
extern s32 D_0036AC78[];

/* Keep the two loaded handles and their derived slot set in the menu work array. */
void mnuLoadResourceHandles(u32 *menuWork) {
    s32 resourceIndex;

    for (resourceIndex = 0; resourceIndex < 2; resourceIndex++) {
        struct EffectSlotSet *resourceHandle = effLoadIndexedResource(
            D_003AF590, (const char *)D_0036AC78[resourceIndex], 1);

        menuWork[0x19 + resourceIndex] = (u32)resourceHandle;
        effResolveAndReleaseResource(resourceHandle);
    }
    menuWork[0x1B] = (u32)effCreateResourceSlotSet((struct EffectSlotSet *)menuWork[0x19], 7, 1);
}

extern void mnuReleaseEffectResource(MenuResourceWork *);

/* Destroy the stored slot sets, then release and clear any optional effect handle. */
void mnuReleaseResourceHandles(u32 *menuWork) {
    s32 resourceIndex;

    for (resourceIndex = 0; resourceIndex < 2; resourceIndex++) {
        effDestroyResourceSlotSet((struct EffectSlotSet *)menuWork[0x19 + resourceIndex]);
    }
    effDestroyResourceSlotSet((struct EffectSlotSet *)menuWork[0x1B]);
    if (menuWork[0x56] != 0) {
        mnuReleaseEffectResource((MenuResourceWork *)menuWork[0x56]);
        menuWork[0x56] = 0;
    }
}

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF570);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF580);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF590);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC3D0);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC3D8);

