#include "common.h"
#include "mdl_motion_api.h"
#include "sdf_motion.h"
#include "itf_mes_window.h"
#include "sdf_resource.h"
#include "evt_viewer.h"
#include "kwln.h"
#include "sdf.h"
#include "sdf_draw.h"
#include "dds3obj.h"
#include "evt_world.h"
#include "pcp_vu0.h"
#include "evt_unit.h"
#include "eff_transform.h"
#include "eff_blur.h"
#include "eff_event_draw.h"
#include "dat_state.h"
#include "eff.h"
#include "eff_node.h"
#include "eff_object.h"
#include "mdl.h"
#include "evt_polygon_movie.h"
#include "kwln_task_lifecycle.h"
#include "file_request_api.h"
extern u16 D_004372B0;
extern u16 D_004372B2;
extern u8 D_00423050[];
extern s32 func_0035C860(char *buffer, const char *format, ...);
extern s32 evtViewerSaveTrackFiles(s32 mode, EvtRuntime *runtime);
extern void evtReloadEventViewer();
extern void *dds3GetWorldObject(void);
extern f32 dds3GetCameraFieldOfView(EffWorldNode *camera);
extern void sdfSetViewFieldOfView(f32);


extern void mdlAttachWorldObjectToSourceVector(s32, s32);

extern s32 evtViewerHasUpdateFlag(EvtRuntime *);
extern void func_002476B8(EvtRuntime *viewer);
extern void evtViewerApplySelectedEntry(EvtRuntime *viewer);
extern u8 D_004372C0[3];
extern u32 kwlnGetDrawBufferIndex(void);
extern void kwlnFadeSetColor(s32 red, s32 green, s32 blue, s32 alpha);

/* Effect-channel assignments driven by the viewer's timeline tracks. */

extern s32 evtViewerTestIndexedCondition(u32 encodedId);
extern void effInitCh71Id(void);
extern void effInitCh72Id(void);
extern void effInitCh75Id(void);
extern void effInitCh76Id(void);
extern void effSetCh71Id(SdfTex *texture);
extern void effSetCh72Id(SdfTex *sourceHandle);
extern void effSetCh75Id(SdfTex *texture);
extern void effSetCh76Id(SdfTex *sourceHandle);
extern void *mnuCampFindEntryByName(void *scene, const char *name);


void evtApplyViewerTimelineFrame(s32 time, EvtRuntime *viewer);

void evtViewerPushCommandHistory(s32 arg0, s32 arg1, s32 arg2, EvtRuntime *arg3);

/* Task user values are words; viewer callbacks decode the stored address. */

s32 evtViewerUpdateFrame(KwlnTask *task);

void func_00101968(s32 arg0, s32 arg1);

s32 evtCreateFrameVariableTask(void);

void *evtViewerScheduleFrameVariableTask(KwlnTask *task);

extern u32 mnuCampGetPrimaryOption(void *scene);
extern const char *D_003C91C0[];
extern const char *D_003C91C8[];
extern const char *D_003C91D0[];
extern const char *D_003C91E0[];
extern s32 D_004372CC;

extern u32 kwlnDrawControlFlags;

void fldInitializeCameraColorResource(void);

extern void func_0024DBB8(PolyMovieWork *work);
extern s32 mnuPollTitleStreamStateLocked(void);
extern void mnuMarkTitleStreamResetPending(void);
extern s32 func_0024D760(PolyMovieWork *ctx);

struct EvtRuntime;

struct EffNode;

typedef struct EvtViewerObjectData {
    u8 pad00[0xC];
    struct EffNode *parameterNode;
} EvtViewerObjectData;


/* Handles retained by the viewer and by its owning task context. */


/* Script-command parameter slots are interpreted according to track kind. */


u16 evtViewerPopHistory(EvtRuntime *viewer);

extern char evtViewerTaskName[]; /* "EventViewer" */


struct PolyMovieObject;

/* Each timeline parameter word's scalar format is selected by the track kind. */


/* Linked timeline track. Saved vectors and the attachment latch are used by
 * kind-1 world-object tracks; the vectors keep their original embedded layout. */

extern s8 D_003C953A[];
extern void evtReorderListNodes(EvtRuntimeGroup *track);

typedef struct CampDisplayDefaults CampDisplayDefaults;
extern void func_0025E460(EvtRuntimeChild *from, EvtRuntimeChild *to, CampDisplayDefaults *display, f32 ratio);
extern void mnuDrawCampScaledTexture(SdfTex *texture, CampDisplayDefaults *display);


/* Native five-word draw-vector parameters; the timeline swaps x and y. */
typedef struct EvtViewerDrawVector {
    f32 x, y, z, w;
    s32 mode;
} EvtViewerDrawVector;
extern EvtViewerDrawVector kwlnDrawVector;
extern u128 *D_0037F770[];
extern u128 kwlnDefaultColorVector[];
extern f32 D_00438A48;
extern f32 D_00438A4C;
extern EffBlurTemplateBody *effEventGetBlurTemplateSetupParams(void);
extern EffBlurScatterParams *effEventGetScatterBlurSetupParams(void);
extern EffBlurScaleParams *effEventGetScaleBlurSetupParams(void);
extern EffResourceRectParams *effEventGetResourceTemplateSetupParams(void);
extern void *memcpy(void *destination, const void *source, u32 size);


extern void effEnableTexturedBlur(void);
extern void effDisableTexturedBlur(void);
extern void effEnableTexturedSquare(void);
extern void effDisableTexturedSquare(void);
extern void effEnableFilterBlur(void);
extern void effDisableFilterBlur(void);
extern void effEnableStaggeredBlur(void);
extern void effDisableStaggeredBlur(void);
extern void effEnableFramebufferQuad(void);
extern void effDisableFramebufferQuad(void);
extern void effEnableColorRectangle(void);
extern void effDisableColorRectangle(void);
extern void func_0025EE00(EvtRuntime *);


extern s32 func_0035B6E0(const char *, ...);

/* Applies duration-scaled alpha to the selected effect for supported track
 * kinds. The key's duration is used directly, without validation here. */
void func_00247DE0(EvtRuntimeGroup *group, EvtRuntimeChild *key, s32 unused2, s32 unused3, EvtRuntime *viewer) {
    s32 elapsed;
    struct EffNode *node;
    s32 alpha;
    u32 color;

    switch (group->type) {
    case 3:
    case 20:
    case 21:
    case 26:
        break;
    default:
        return;
    }
    if (key->p08.sb[0] < 0 || key->p10.sb[0] == 0) {
        return;
    }
    elapsed = viewer->curFrame - key->frame;
    if (key->duration >= elapsed) {
        node = ((EvtViewerObjectData *)viewer->objects[key->p08.sb[0]]->data)->parameterNode;
        alpha = (s32)((128.0f / key->duration) * elapsed);
        func_0035B6E0("alpha=%d\n", alpha);
        color = ((u32)alpha << 24) | 0x808080;
        effSetNodeParameterValue(node, color);
    }
}

INCLUDE_ASM(const s32, "game/code_00247DE0", func_00247EE0);
INCLUDE_ASM(const s32, "game/code_00247DE0", func_00248000);


extern void evtEndUnitValueTransition(EvtUnit *, s32);

/* At time, selects each named unit's latest kind-9 transition key and starts,
 * ends or leaves its value transition according to the selected key flags. */
void func_00248B80(s32 time, EvtRuntime *viewer) {
    EffWorldNode *object;
    EvtRuntimeGroup *node;
    EvtRuntimeChild *key;
    EvtRuntimeChild *selected;
    EvtUnit *unit;
    s32 selectedValue;
    s32 selectedTime;

    if (dds3GetWorldObject() != NULL) {
        object = ((EvtWorldTable *)((EffWorldNode *)dds3GetWorldObject())->data)->slots[EVT_WORLD_SLOT_UNIT].head;
        while (object != NULL) {
            if (((u8 *)object->value) != NULL) {
                selected = NULL;
                selectedValue = 0;
                selectedTime = -1;
                node = viewer->groups;
                while (node != NULL) {
                    if (node->type == 9) {
                        key = node->children;
                        while (key != NULL) {
                            if (time >= key->frame + node->metadata.value && key->p08.sh[0] >= 0 &&
                                object == dds3FindIndexedObjectChainNodeByName(dds3GetWorldObject(),
                                    EVT_WORLD_SLOT_UNIT, viewer->entryName[key->p08.sh[0]])) {
                                if (selectedTime < key->frame + node->metadata.value) {
                                    selectedValue = (s32)node->info;
                                    selectedTime = key->frame + node->metadata.value;
                                    selected = key;
                                }
                            }
                            key = key->next;
                        }
                    }
                    node = node->next;
                }
                unit = evtUnitGetNestedValue(object);
                if (selected != NULL) {
                    if (selected->p08.sh[1] != 0) {
                        if (unit->currentTransitionValue != selectedValue || !(unit->flags & 0x40000)) {
                            evtSetUnitValueTransition(unit, (EffWorldNode *)selectedValue, selected->duration);
                        }
                    } else {
                        if (unit->flags & 0x40000) {
                            if (!(unit->flags & 0x100000)) {
                                evtEndUnitValueTransition(unit, selected->duration);
                            }
                        }
                    }
                } else {
                    if (unit->currentTransitionValue != 0) {
                        evtEndUnitValueTransition(unit, 0);
                    }
                }
            }
            object = object->next;
        }
    }
}

extern f32 evtMovieInterpolateFloatIfEnabled(s32 enable, f32 t, f32 a, f32 b);
extern s32 evtMovieInterpolateIntIfEnabled(s32 enable, f32 t, s32 a, s32 b);
extern void evtPolygonMovieSetObjectMode(struct PolyMovieObject *obj, u32 mode, s32 setFlags, s32 clearFlags);

/* At time, applies the track's latest enabled mode keys, then interpolates the
 * unit's byte and float parameters from the active kind-3/4 key to the next
 * enabled key of the same kind. */
void evtStepPolygonMovieConditionalKeys(EvtRuntimeGroup *track, s32 time) {
    u32 objectMode = 0;
    u32 unitMode = 2;
    s32 pass;
    s32 setFlags = 0;
    s32 clearFlags = 0;
    EvtUnit *unit = NULL;
    EvtRuntimeChild *key;

    key = track->children;
    if (key != NULL) {
        do {
            if (time < key->frame) {
                break;
            }
            switch (key->p08.sb[0]) {
            case 0:
            case 1:
                if (evtViewerTestIndexedCondition(key->p10.sh[0]) == 1) {
                    objectMode = key->p08.sb[0];
                }
                break;
            case 2:
            case 3:
            case 4:
                if (evtViewerTestIndexedCondition(key->p10.sh[0]) == 1) {
                    unitMode = key->p08.sb[0];
                    unit = evtUnitGetNestedValue(track->info);
                }
                break;
            }
            key = key->next;
        } while (key != NULL);
    }

    for (pass = 0; pass < 1; pass++) {
        EvtRuntimeChild *current = NULL;
        EvtRuntimeChild *next;
        s8 kind;
        f32 t;
        s32 startFrame;
        s32 endFrame;

        for (key = track->children; key != NULL; key = key->next) {
            if ((key->p08.sb[0] == 3 || key->p08.sb[0] == 4) &&
                evtViewerTestIndexedCondition(key->p10.sh[0]) == 1) {
                if (key->frame <= time) {
                    current = key;
                } else {
                    break;
                }
            }
        }
        if (current == NULL) {
            break;
        }
        kind = current->p08.sb[0];
        next = NULL;
        for (key = track->children; key != NULL; key = key->next) {
            if (key->p08.sb[0] == kind && evtViewerTestIndexedCondition(key->p10.sh[0]) == 1 &&
                time < key->frame) {
                next = key;
                break;
            }
        }
        if (next == NULL) {
            unit->unkD3 = current->p0C.b[1];
            unit->unkD4 = current->p14.f;
        } else {
            startFrame = current->frame;
            endFrame = next->frame;
            if (endFrame != startFrame) {
                t = (f32)(time - startFrame) / (f32)(endFrame - startFrame);
            } else {
                t = 0.0f;
            }
            if (next->p0C.sb[0] == 0 || next->p0C.sb[0] == 2) {
                t = 0.0f;
            }
            unit->unkD3 = evtMovieInterpolateIntIfEnabled(0, t, current->p0C.b[1], next->p0C.b[1]);
            if (next->p0C.b[0] < 2) {
                t = 0.0f;
            }
            unit->unkD4 = evtMovieInterpolateFloatIfEnabled(0, t, current->p14.f, next->p14.f);
        }
        if (current->p0C.sb[0] == 1 || current->p0C.sb[0] == 3) {
            setFlags |= 0x4000;
        } else {
            clearFlags |= 0x4000;
        }
        switch (current->p0C.sb[0]) {
        case 2:
        case 3:
            setFlags |= 0x8000;
            break;
        default:
            clearFlags |= 0x8000;
            break;
        }
    }
    evtPolygonMovieSetObjectMode((struct PolyMovieObject *)track->info, objectMode, 0, 0);
    evtPolygonMovieSetObjectMode((struct PolyMovieObject *)track->info, unitMode, setFlags, clearFlags);
}

extern void evtViewerClampMovieTimes(s32, EvtRuntime *);
extern void evtViewerSyncWorldGroups(u32, EvtRuntime *);
extern void evtViewerApplyGlyphLodChannel(s32, EvtRuntime *);
extern void func_00249C40(s32, EvtRuntime *);
extern void evtViewerActivateWindowForGlyphEntry(s32, EvtRuntime *);
extern void evtViewerCountFlaggedUpdates(EvtRuntime *);
extern void func_0024A158(s32, EvtRuntime *);
extern s32 evtViewerUpdateTimedAction(EvtRuntime *);
extern void func_0024A400(EvtRuntime *);
extern s32 evtViewerTestIndexedCondition(u32);
extern void func_00247858(EvtRuntimeGroup *, EvtRuntimeChild *, s32, s32, EvtRuntime *);
extern void func_00247EE0(EvtRuntimeGroup *, EvtRuntimeChild *, s32, s32, EvtRuntime *);
extern void func_00248000(EvtRuntimeGroup *, EvtRuntimeChild *, EvtRuntimeChild *, s32, s32 *, EvtRuntime *, s32);
extern void evtResetUnitVectorSlots();
extern void kwlnCancelConfiguredFadeFrames(void);
extern u8 kwlnDrawOverlayEnabled;
extern EvtRuntimeGroup *D_004372B8;

void evtApplyViewerTimelineFrame(s32 time, EvtRuntime *viewer) {
    EvtRuntimeGroup *group;
    EvtRuntimeChild *child;
    EvtRuntimeChild *other;
    EvtRuntimeChild *selected;
    s32 difference;
    s32 bestDistance;
    s32 conditionIndex;
    s16 duration;
    s16 offset;

    viewer->blurRectangleEnabled = 0;
    viewer->texturedBlurEnabled = 0;
    viewer->flags &= ~2;
    viewer->colorRectangleEnabled = 0;
    viewer->flags &= ~4;
    conditionIndex = 0;
    viewer->texturedSquareEnabled = 0;
    viewer->filterBlurEnabled = 0;
    viewer->staggeredBlurEnabled = 0;
    viewer->framebufferQuadEnabled = 0;
    evtResetUnitVectorSlots();
    evtViewerClampMovieTimes(time, viewer);
    evtViewerSyncWorldGroups(time, viewer);
    evtViewerApplyGlyphLodChannel(time, viewer);
    func_00249C40(time, viewer);
    evtViewerActivateWindowForGlyphEntry(time, viewer);
    evtViewerCountFlaggedUpdates(viewer);
    func_0024A158(time, viewer);
    evtViewerUpdateTimedAction(viewer);
    func_0024A400(viewer);

    for (group = viewer->groups; group != NULL; group = group->next) {
        if (group->type == 1) {
            evtStepPolygonMovieConditionalKeys(group, time);
            continue;
        }
        for (child = group->children; child != NULL; child = child->next) {
            switch (group->type) {
            case 3:
            case 20:
            case 21:
            case 26:
                duration = child->p08.sh[1];
                if (duration == 0) {
                    difference = viewer->headerThird - child->frame;
                } else {
                    difference = duration - child->frame;
                    if (difference < 0) {
                        difference = 0;
                    }
                }
                break;
            case 18:
                duration = child->p08.sh[0];
                if (duration == 0) {
                    difference = viewer->headerThird - child->frame;
                } else {
                    difference = duration - child->frame;
                    if (difference < 0) {
                        difference = 0;
                    }
                }
                break;
            case 9:
                other = child->next;
                while (other != NULL && other->p08.sh[0] != child->p08.sh[0]) {
                    other = other->next;
                }
                if (other != NULL) {
                    difference = other->frame - child->frame;
                } else {
                    difference = viewer->headerThird - child->frame;
                }
                break;
            default:
                difference = child->duration;
                break;
            }

            if ((difference == 0 && time == child->frame) ||
                (difference > 0 && time >= child->frame &&
                 time < child->frame + difference)) {
                func_00247858(group, child, time, difference, viewer);
                func_00247DE0(group, child, time, difference, viewer);
            } else {
                func_00247EE0(group, child, time, difference, viewer);
            }
        }
    }

    bestDistance = 10000;
    D_004372B8 = NULL;
    for (group = viewer->groups; group != NULL; group = group->next) {
        offset = group->metadata.value;
        child = group->children;
        for (; child != NULL; child = child->next) {
            if (group->type == 2) {
                if (evtViewerTestIndexedCondition(child->p0C.sh[0]) == 0) {
                    continue;
                }
            } else {
                if (group->type == 16 || group->type == 17 || group->type == 12) {
                    if (evtViewerTestIndexedCondition(child->p14.sh[0]) == 0) {
                        continue;
                    }
                }
            }
            if (time < child->frame + offset) {
                break;
            }
        }

        selected = child != NULL ? child->prev : group->lastChild;
        if (group->type == 2 || group->type == 16 ||
            group->type == 17 || group->type == 12) {
            switch (group->type) {
            case 2:
                conditionIndex = 1;
                break;
            case 12:
            case 16:
            case 17:
                conditionIndex = 3;
                break;
            }
            while (selected != NULL) {
                s16 condition;

                condition = (s16)selected->body.words[conditionIndex];
                if (evtViewerTestIndexedCondition(condition) == 1) {
                    break;
                }
                selected = selected->prev;
            }
        }
        func_00248000(group, child, selected, time, &bestDistance, viewer, offset);
    }

    if (D_004372B8 != NULL) {
        viewer->fallbackEntry = (s32)D_004372B8->info;
        evtViewerApplySelectedEntry(viewer);
    }
    func_00248B80(time, viewer);
    if (viewer->flags & 1) {
        if (!(viewer->flags & 2)) {
            kwlnCancelConfiguredFadeFrames();
        }
        if (!(viewer->flags & 4)) {
            kwlnDrawOverlayEnabled = 0;
        }
    }
}



/* Returns the address word of the nearest kind-2 key at/before the current
 * frame, or zero. Equal-distance ties retain the first key visited. */
s32 evtViewFindGlyphAtOrBefore(EvtRuntime *viewer) {
    EvtRuntimeChild *result = NULL;
    s32 best = 99999;
    EvtRuntimeGroup *node = viewer->groups;

    while (node != NULL) {
        if (node->type == 2) {
            EvtRuntimeChild *glyph = node->children;

            if (glyph != NULL) {
                do {
                    s32 frame = glyph->frame;

                    if (frame <= viewer->curFrame) {
                        s32 distance = viewer->curFrame - frame;

                        if (distance < best) {
                            best = distance;
                            result = glyph;
                        }
                    }
                    glyph = glyph->next;
                } while (glyph != NULL);
            }
        }
        node = node->next;
    }
    return (s32)result;
}

extern void evtSetMovieClipPositionClampedToDuration(s32 object, s32 arg1, s32 start, s32 end, s32 extra);


/* Clamps each movie object's playback interval using its linked track's first
 * key frame (or track offset), endTime, and the current key's extra parameter. */
void evtViewerClampMovieTimes(s32 endTime, EvtRuntime *viewer) {
    s32 table;
    s32 slots;
    s32 object;
    EvtRuntimeGroup *node;
    s32 time;
    s32 extra;
    EvtRuntimeChild *glyph;

    if (dds3GetWorldObject() != 0) {
        table = (s32)((EvtWorldTable *)((EffWorldNode *)dds3GetWorldObject())->data);
        if (table != 0) {
            slots = (s32)((EvtWorldTable *)table)->slots;
            if (slots != 0) {
                object = (s32)((EvtWorldSlot *)slots)[EVT_WORLD_SLOT_MOVIE].head;
                if (object != 0) {
                    do {
                        node = viewer->groups;
                        while (node != NULL) {
                            if (node->info != NULL && (void *)object == dds3GetSlot(node->info, 1)) {
                                if (node->type == 2) {
                                    time = 0;
                                    if (node->childCount != 0) {
                                        time = node->children->frame;
                                    }
                                    glyph = (EvtRuntimeChild *)evtViewFindGlyphAtOrBefore(viewer);
                                    extra = 0;
                                    if (glyph != NULL) {
                                        extra = glyph->p0C.sh[1];
                                    }
                                } else {
                                    time = node->metadata.value;
                                    extra = 0;
                                }
                                evtSetMovieClipPositionClampedToDuration(object, 0, time, endTime, extra);
                                break;
                            }
                            node = node->next;
                        }
                        object = (s32)((EffWorldNode *)object)->next;
                    } while (object != 0);
                }
            }
        }
    }
}

extern Motion *mdlFindNodeById(MdlCtx *ctx, s32 id);
extern EvtRuntimeChild *evtViewerFindLatestMatchingMotionKey(EvtRuntimeGroup *, s32, s32);

void evtViewerSyncModelMotionChannels(s32 frame, EffWorldNode *object, EvtRuntimeGroup *track,
                   EvtRuntime *viewer, s32 mode) {
    EffectObjectData *data = object->data;
    MdlCtx *model = (MdlCtx *)data->modelHolder->resourceHandle;
    s32 channel;

    if (model->first == NULL) {
        return;
    }
    for (channel = 0; channel < 4; channel++) {
        Motion *motion = mdlFindNodeById(model, channel);
        EvtRuntimeChild *key;
        s32 sampleFrame;

        if (motion == NULL) {
            continue;
        }
        key = evtViewerFindLatestMatchingMotionKey(track, frame, channel);
        if (key == NULL) {
            if (mode == 0 && track->motionKeyCache[channel] == key) {
                continue;
            }
            motion = mdlFindNodeById(model, channel);
            sdfMotionInitialize(motion, 0, 0, 0.0f, 0.0f);
            if (mdlGetNodeFrameCount(model, channel) != 0) {
                if ((s32)mdlGetNodeFrameCount(model, channel) - 1 < frame) {
                    sampleFrame = (s32)mdlGetNodeFrameCount(model, channel) - 1;
                } else {
                    sampleFrame = frame;
                }
            } else {
                sampleFrame = 0;
            }
            if (channel == 0) {
                motion = mdlFindNodeById(model, 0);
                sdfMotionSampleAtFrame(motion, (f32)sampleFrame);
            } else {
                motion = mdlFindNodeById(model, channel);
                sdfMotionSampleAtFrame(motion, 0.0f);
            }
            if ((viewer->flags & 1) != 0) {
                motion = mdlFindNodeById(model, channel);
                sdfMotionSuspend(motion);
            } else {
                motion = mdlFindNodeById(model, channel);
                sdfMotionResume(motion);
            }
            track->motionKeyCache[channel] = NULL;
            if (channel != 0) {
                motion = mdlFindNodeById(model, channel);
                sdfMotionSuspend(motion);
            }
            continue;
        }

        {
            s32 loopEnabled = key->p0C.sb[2] == 0;
            s32 duration = key->p18.sh[0];
            s32 relativeFrame;
            s32 blendLead;
            s32 count;

            if (loopEnabled == 0) {
                s32 keyFrame = key->frame;
                count = (s32)mdlGetNodeFrameCount(model, channel);
                relativeFrame = frame - keyFrame + duration;
                if (count - 1 < relativeFrame) {
                    mode = 1;
                }
            }
            blendLead = 0;
            if (duration > 0) {
                mode = 1;
                if (key->p0C.sb[3] > 0) {
                    blendLead = -duration;
                }
            }
            if (mode == 0 && track->motionKeyCache[channel] == key) {
                continue;
            }
            motion = mdlFindNodeById(model, channel);
            sdfMotionInitialize(motion, key->p0C.sb[1], loopEnabled,
                                (f32)blendLead, (f32)key->p0C.sb[3]);
            if (mode == 1) {
                if (loopEnabled == mode) {
                    count = (s32)mdlGetNodeFrameCount(model, channel);
                    if (count != 0) {
                        count = (s32)mdlGetNodeFrameCount(model, channel);
                        sampleFrame = ((s32)frame - key->frame + duration) % count;
                    } else {
                        sampleFrame = 0;
                    }
                } else {
                    s32 capturedFrame = key->frame;
                    count = (s32)mdlGetNodeFrameCount(model, channel);
                    relativeFrame = frame - capturedFrame + duration;
                    if (count - 1 < relativeFrame) {
                        count = (s32)mdlGetNodeFrameCount(model, channel);
                        sampleFrame = count - 1;
                    } else {
                        sampleFrame = (s32)frame - key->frame + duration;
                    }
                }
                motion = mdlFindNodeById(model, channel);
                sdfMotionSampleAtFrame(motion, (f32)sampleFrame);
            }
            if ((viewer->flags & 1) != 0) {
                motion = mdlFindNodeById(model, channel);
                sdfMotionSuspend(motion);
            } else {
                motion = mdlFindNodeById(model, channel);
                sdfMotionResume(motion);
            }
            track->motionKeyCache[channel] = key;
        }
    }
}

/* Updates world units at position using the first track whose owner word
 * matches that unit's object-chain node. */
void evtViewerSyncWorldGroups(u32 position, EvtRuntime *viewer) {
    u8 *list;
    EvtRuntimeGroup *node;
    EvtRuntimeGroup *found;

    if (dds3GetWorldObject() != 0) {
        list = (u8 *)((EvtWorldTable *)((EffWorldNode *)dds3GetWorldObject())->data)->slots[EVT_WORLD_SLOT_UNIT].head;
        while (list != 0) {
            found = 0;
            for (node = viewer->groups; node != 0; node = node->next) {
                if (node->info == (EffWorldNode *)list) {
                    found = node;
                    break;
                }
            }
            if (found != 0) {
                evtViewerSyncModelMotionChannels(position, list, node, viewer, 0);
            }
            list = (u8 *)((EffWorldNode *)list)->next;
        }
    }
}

extern u32 sdfGetLodChunkValue(SdfModel *model);

/* Applies the latest kind-6 key at/before position to a kind-1 track's LOD
 * byte, provided the requested signed-byte level is supported by its chunk. */
void evtViewerApplyGlyphLodChannel(s32 position, EvtRuntime *viewer) {
    EvtRuntimeGroup *node = viewer->groups;
    EvtRuntimeChild *glyph;
    EvtRuntimeChild *best;
    s32 bestFrame;
    SdfModel *lod;
    s8 level;

    while (node != NULL) {
        if (node->type == 1) {
            glyph = node->children;
            bestFrame = -1;
            best = NULL;
            if (glyph != NULL) {
                do {
                    if (position >= glyph->frame && bestFrame < glyph->frame && glyph->p08.sb[0] == 6) {
                        bestFrame = glyph->frame;
                        best = glyph;
                    }
                    glyph = glyph->next;
                } while (glyph != NULL);
            }
            lod = ((MdlCtx *)((EffectObjectData *)node->info->data)->modelHolder->resourceHandle)->inner;
            if (best == NULL) {
                lod->lodIndex = 0;
            } else {
                level = best->p0C.sb[0];
                if ((s32)sdfGetLodChunkValue(lod) >= level) {
                    lod->lodIndex = best->p0C.sb[0];
                }
            }
        }
        node = node->next;
    }
}


/* At an exact kind-7 key frame, attaches the indexed world object. Index -1
 * restores the saved vectors; losing the active key restores them once too. */
void func_00249C40(s32 position, EvtRuntime *viewer) {
    EvtRuntimeGroup *node = viewer->groups;

    while (node != NULL) {
        if (node->type == 1) {
            EvtRuntimeChild *glyph = node->children;
            s32 bestFrame = -1;
            EvtRuntimeChild *best = NULL;

            if (glyph != NULL) {
                do {
                    if (glyph->p08.sb[0] == 7 && position >= glyph->frame &&
                        bestFrame < glyph->frame) {
                        bestFrame = glyph->frame;
                        best = glyph;
                    }
                    glyph = glyph->next;
                } while (glyph != NULL);
            }
            if (best == NULL) {
                if (node->objectAttached == 1) {
                    effObjSetInnerPosition(node->info,
                                           (u128 *)node->savedPosition);
                    effObjSetInnerRotation(node->info,
                                            (u128 *)node->savedRotation);
                    effObjFetchInnerPosition(node->info);
                    VU0_STORE_VF(vf10, &node->info->inner->smoothedPosition);
                    node->objectAttached = 0;
                }
            } else if (best->frame == position) {
                s16 channel = best->p0C.sh[0];

                if (channel == -1) {
                    effObjSetInnerPosition(node->info,
                                           (u128 *)node->savedPosition);
                    effObjSetInnerRotation(node->info,
                                            (u128 *)node->savedRotation);
                    effObjFetchInnerPosition(node->info);
                    VU0_STORE_VF(vf10, &node->info->inner->smoothedPosition);
                    node->objectAttached = 0;
                } else {
                    EffWorldNode *object = dds3FindObjectChainNodeByName(
                        dds3GetWorldObject(), viewer->entryName[channel]);

                    mdlAttachWorldObjectToSourceVector(
                        node->info->key, object->key);
                    node->objectAttached = 1;
                }
            }
        }
        node = node->next;
    }
}

extern void mnuUnpackNibbleFields(EvtRuntimeChild *, s32 *, s32 *);
void evtViewerMarkWindowActive(EvtRuntime *);

/* Activates the message window thirty frames before a kind-4 key when its
 * unpacked entry is ready. Only the first kind-4 track is considered. */
void evtViewerActivateWindowForGlyphEntry(s32 position, EvtRuntime *viewer) {
    EvtRuntimeGroup *group;
    EvtRuntimeChild *glyph;
    s32 entry;
    s32 mode;

    if (viewer->windowContext == 0) {
        return;
    }
    if (viewer->windowContext->handle == -1) {
        return;
    }
    for (group = viewer->groups; group != NULL; group = group->next) {
        if (group->type == 4) {
            for (glyph = group->children; glyph != NULL; glyph = glyph->next) {
                if (glyph->frame - 30 == position) {
                    mnuUnpackNibbleFields(glyph, &entry, &mode);
                    if (itfMesGetWindowEntryItems(
                            viewer->windowContext->handle, entry) == 1) {
                        evtViewerMarkWindowActive(viewer);
                        break;
                    }
                }
            }
            break;
        }
    }
}

void evtViewerCountFlaggedUpdates(EvtRuntime *viewer) {
    s64 active;

    active = evtViewerHasUpdateFlag(viewer);
    if (active != 0) {
        viewer->updateCount = viewer->updateCount + 1;
    }
}

extern s32 func_0025E7B0(void *scene);
extern s32 scrCommandIsProcessControlFlagClear(void);
extern u32 mnuCampGetSecondaryOption(void *scene);
extern void mnuReleaseCampSceneRegisteredIds(EvtRuntime *viewer);
extern s32 mnuQueryTitleSoundBusy(void);
extern void mnuStopTitleVoicePlayback(void);
extern void sdfSoundSetChannelCount(u32 channels);
extern s32 fldTitleIsActive(void);
extern void func_0014E668(s32 active);
void evtViewerCleanupMessageWindow(EvtRuntime *viewer);

void func_00249EE8(EvtRuntime *viewer) {
    if (func_0025E7B0(viewer) == 1 || scrCommandIsProcessControlFlagClear() == 0) {
        if (viewer->glyphTickCount == 0) {
            viewer->glyphTickCount = 1;
        }
        return;
    }
    if (viewer->curFrame < viewer->frameRange.word - 30) {
        if (viewer->headerFirst + 20 >= viewer->curFrame) {
            return;
        }
    } else {
        return;
    }

    viewer->flags |= 0x10;
    if (mnuCampGetSecondaryOption(viewer) == 2) {
        viewer->updateCount = 0x17;
    } else {
        viewer->updateCount = 0;
    }
    mnuReleaseCampSceneRegisteredIds(viewer);
    if (mnuQueryTitleSoundBusy() == 1) {
        mnuStopTitleVoicePlayback();
    } else if (viewer->timedActive == 1) {
        sdfSoundSetChannelCount(10);
    }
    if (fldTitleIsActive() == 1) {
        func_0014E668(0);
    }
    evtViewerCleanupMessageWindow(viewer);
}

s32 evtViewerHasUpdateFlag(EvtRuntime *viewer) {
    s32 flags = viewer->flags;
    return (flags & 0x10) > 0;
}


/* Fades the viewer's background colour over its update countdown. */
void func_0024A020(EvtRuntime *viewer) {
    s32 fade = 0;

    kwlnGetDrawBufferIndex();
    if (viewer->updateCount != 0x19) {
        fade = (s32)(128.0f - ((f32)viewer->unk24A0 +
                               ((128.0f - (f32)viewer->unk24A0) / 25.0f) * (f32)viewer->updateCount));
    }
    if (fade < 0) {
        fade = 0;
    }
    if (fade < 0x80 && evtViewerHasUpdateFlag(viewer) != 0) {
        s32 option = (s32)mnuCampGetSecondaryOption(viewer);

        switch (option) {
        case 0:
            D_004372C0[0] = D_004372C0[1] = D_004372C0[2] = 0;
            break;
        case 1:
            D_004372C0[0] = D_004372C0[1] = D_004372C0[2] = 0xFF;
            break;
        case 2:
            return;
        }
        kwlnFadeSetColor(D_004372C0[0], D_004372C0[1], D_004372C0[2], 0x80 - fade);
    }
}

/* Applies the effect-channel assignments driven by the viewer's tracks. */
void func_0024A158(s32 frame, EvtRuntime *viewer) {
    EvtRuntimeGroup *track = viewer->groups;

    while (track != NULL) {
        if ((u32)(track->type - 0xE) < 2 || track->type == 0x17 || track->type == 0x11) {
            EvtRuntimeChild *key = track->children;
            SdfTex *value = 0;

            while (key != NULL) {
                if (track->type != 0x11 || evtViewerTestIndexedCondition(key->p14.sh[0]) != 0) {
                    if (frame < key->frame + track->metadata.value) {
                        break;
                    }
                    if (key->p10.sh[0] != 0) {
                        value = 0;
                        if (key->p10.sh[0] != 1) {
                            value = ((EvtRuntimeGroup *)mnuCampFindEntryByName(
                                         viewer, (char *)viewer->entryName[key->p10.sh[0] - 2]))->texture;
                        }
                    }
                }
                key = key->next;
            }

            switch (track->type) {
            case 0xE:
                if (viewer->ch71 != value) {
                    viewer->ch71 = value;
                    if (value == 0) {
                        effInitCh71Id();
                    } else {
                        effSetCh71Id(value);
                    }
                }
                break;
            case 0xF:
                if (viewer->ch72 != value) {
                    viewer->ch72 = value;
                    if (value == 0) {
                        effInitCh72Id();
                    } else {
                        effSetCh72Id(value);
                    }
                }
                break;
            case 0x17:
                if (viewer->ch76 != value) {
                    viewer->ch76 = value;
                    if (value == 0) {
                        effInitCh76Id();
                    } else {
                        effSetCh76Id(value);
                    }
                }
                break;
            case 0x11:
                if (viewer->ch75 != value) {
                    viewer->ch75 = value;
                    if (value == 0) {
                        effInitCh75Id();
                    } else {
                        effSetCh75Id(value);
                    }
                }
                break;
            }
        }
        track = track->next;
    }
}

extern void mnuCheckMovieDecoderStatus(void);
extern void mnuStopMovieDrawTask(void);
/* Advance or stop the timed viewer action according to the current position. */
s32 evtViewerUpdateTimedAction(EvtRuntime *viewer) {
    if (viewer->timedActive == 1) {
        if (viewer->curFrame < viewer->timedStart) {
            mnuStopMovieDrawTask();
            viewer->timedActive = 0;
            viewer->timedStart = 0;
            viewer->timedEnd = 0;
        } else if (viewer->timedEnd < 0) {
            mnuCheckMovieDecoderStatus();
        } else if (viewer->curFrame >= viewer->timedEnd) {
            mnuStopMovieDrawTask();
            viewer->timedActive = 0;
            viewer->timedStart = 0;
            viewer->timedEnd = 0;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00247DE0", func_0024A400);

void func_0024A5F8(EvtRuntime *viewer) {
}

void evtViewerAdvanceGlyphTick(EvtRuntime *viewer) {
    s32 nextTick;

    if ((viewer->curFrame < viewer->frameRange.word - 3) && (0 < viewer->glyphTickCount))
    {
        frFontDrawGlyphInDefaultMode(viewer->glyph);
        nextTick = viewer->glyphTickCount + 1;
        viewer->glyphTickCount = nextTick;
        if (0x1d < nextTick) {
            viewer->glyphTickCount = 0;
        }
    }
}

void func_0024A668(EvtRuntime *viewer) {
}

/* Returns the latest eligible kind-5 motion key at/before frame for channel,
 * or NULL. Its condition must pass; equal-frame ties retain the first key. */
EvtRuntimeChild *evtViewerFindLatestMatchingMotionKey(EvtRuntimeGroup *group, s32 frame, s32 channel) {
    s32 bestFrame = -1;
    EvtRuntimeChild *best = NULL;
    EvtRuntimeChild *key = group->children;

    if (key != NULL) {
        do {
            if (frame >= key->frame && bestFrame < key->frame && key->p08.sb[0] == 5 &&
                key->p0C.sb[0] == channel && evtViewerTestIndexedCondition(key->p10.sh[0]) == 1) {
                bestFrame = key->frame;
                best = key;
            }
            key = key->next;
        } while (key != NULL);
    }
    return best;
}

typedef struct EvtViewerPlaybackData {
    ObjBase *object;
    void *counter;
} EvtViewerPlaybackData;
extern void sdfFreezeFloatCounter(void *counter);
extern void sdfUnfreezeFloatCounter(void *counter);
extern s32 evtPolygonMovieScaleByProgress(void *movie, s32 mode, s32 start, s32 end);

/* Apply the viewer playback mode to unit, motion and movie-object tracks. */
void func_0024A738(s32 mode, u32 frame, EvtRuntime *viewer) {
    EvtWorldTable *table;
    EffWorldNode *object;
    EvtRuntimeGroup *track;
    EvtRuntimeGroup *found;
    EvtRuntimeChild *key;
    Motion *motion;
    void *counter;
    void *movie;
    s32 useTrackTime;

    if (dds3GetWorldObject() == NULL) {
        return;
    }
    table = ((EvtWorldTable *)((EffWorldNode *)dds3GetWorldObject())->data);
    object = table->slots[5].head;
    while (object != NULL) {
        track = viewer->groups;
        found = NULL;
        while (track != NULL) {
            if (track->info == object) {
                found = track;
                break;
            }
            track = track->next;
        }
        if (found != NULL) {
            evtViewerSyncModelMotionChannels(frame, object, track, viewer, 1);
        }
        object = object->next;
    }
    object = table->slots[6].head;
    while (object != NULL) {
        motion = ((EvtViewerPlaybackData *)object->data)->object->motion;
        if (motion != NULL) {
            if (mode == 0) {
                sdfMotionSampleAtFrame(motion, (f32)frame);
                sdfMotionSuspend(motion);
            } else {
                sdfMotionResume(motion);
            }
        }
        object = object->next;
    }
    object = table->slots[3].head;
    while (object != NULL) {
        counter = ((EvtViewerPlaybackData *)object->data)->counter;
        if (counter != NULL) {
            if (mode == 0) {
                sdfFreezeFloatCounter(counter);
            } else {
                sdfUnfreezeFloatCounter(counter);
            }
        }
        object = object->next;
    }
    track = viewer->groups;
    while (track != NULL) {
        switch (track->type) {
        case 3: case 18: case 20: case 21: case 26:
            key = track->children;
            useTrackTime = 0;
            while (key != NULL) {
                switch (track->type) {
                case 3: case 26:
                    useTrackTime = track->metadata.extra1;
                    /* These track kinds use the same indexed movie lookup. */
                case 20: case 21:
                    if (key->p08.sb[0] < 0) {
                        break;
                    }
                    object = viewer->objects[key->p08.sb[0]];
                    goto updateMovie;
                case 18:
                    object = key->payload;
                    useTrackTime = track->metadata.extra1;
updateMovie:
                    if (object != NULL) {
                        movie = dds3GetObjectOwnedHandle(object)->slots[1];
                        if (movie != NULL) {
                            if (useTrackTime == 0) {
                                evtPolygonMovieScaleByProgress(movie, mode, key->frame, frame);
                            } else {
                                evtPolygonMovieScaleByProgress(movie, mode, 0, frame);
                            }
                        }
                    }
                    break;
                }
                key = key->next;
            }
            break;
        }
        track = track->next;
    }
}


/* Dispatch one of two viewer modes based on its lowest flag bit. */
void evtViewerDispatchFlagMode(EvtRuntime *viewer) {
    if ((viewer->flags & 1) != 0) {
        func_0024A738(0, viewer->curFrame, viewer);
        return;
    }
    func_0024A738(1, viewer->curFrame, viewer);
}

/* Returns the address word of the nearest kind-2 key strictly after the current
 * frame, or zero. Equal-distance ties retain the first key visited. */
s32 evtViewFindNextGlyph(EvtRuntime *viewer) {
    EvtRuntimeChild *result = NULL;
    s32 best = 99999;
    EvtRuntimeGroup *node = viewer->groups;

    while (node != NULL) {
        if (node->type == 2) {
            EvtRuntimeChild *glyph = node->children;

            if (glyph != NULL) {
                do {
                    s32 frame = glyph->frame;

                    if (viewer->curFrame < frame) {
                        s32 distance = frame - viewer->curFrame;

                        if (distance < best) {
                            best = distance;
                            result = glyph;
                        }
                    }
                    glyph = glyph->next;
                } while (glyph != NULL);
            }
        }
        node = node->next;
    }
    return (s32)result;
}

/* Returns the address word of the nearest kind-2 key strictly before the current
 * frame, or zero. Equal-distance ties retain the first key visited. */
s32 evtViewFindPrevGlyph(EvtRuntime *viewer) {
    EvtRuntimeChild *result = NULL;
    s32 best = 99999;
    EvtRuntimeGroup *node = viewer->groups;

    while (node != NULL) {
        if (node->type == 2) {
            EvtRuntimeChild *glyph = node->children;

            if (glyph != NULL) {
                do {
                    s32 frame = glyph->frame;

                    if (frame < viewer->curFrame) {
                        s32 distance = viewer->curFrame - frame;

                        if (distance < best) {
                            best = distance;
                            result = glyph;
                        }
                    }
                    glyph = glyph->next;
                } while (glyph != NULL);
            }
        }
        node = node->next;
    }
    return (s32)result;
}

void evtViewerPushCommandHistory(s32 mode, s32 first, s32 second, EvtRuntime *viewer) {
    s32 count = viewer->historyCount + 1;

    viewer->actionMode = mode;
    viewer->historyCount = count;
    viewer->history[count].id = mode;
    viewer->history[count].a = first;
    viewer->history[count].b = second;
    if (mode > 0) {
        if (mode >= 3) {
            if (mode == 3) {
                viewer->charCol = 0;
                viewer->charRow = 0;
                viewer->nameStorage[0] = 0;
                viewer->nameStorage[0xC] = 0;
            }
        }
    }
}

u16 evtViewerPopHistory(EvtRuntime *viewer) {
    u16 id;
    s32 index;

    index = viewer->historyCount - 1;
    if (viewer->historyCount == 0) {
        viewer->actionMode = 0;
        return 0;
    }
    viewer->historyCount = index;
    id = viewer->history[index].id;
    viewer->actionMode = (u32)id;
    return id;
}

extern s16 itfPanelGetPairFirst(s32);
extern void itfPanelSetPairFirst(s32,s16);
extern void itfMesCleanupWindow(s32,s32);
extern void itfMesResetWindow(s32);
void evtViewerMarkWindowInactive(EvtRuntime *);
void func_0024AD48(EvtRuntime *);

void evtUpdateViewerMessageWindow(EvtRuntime *viewer) {
    if (viewer->windowContext == 0) {
        return;
    }
    if (viewer->windowContext->handle == -1) {
        return;
    }
    if (itfMesGetWindowFlags(viewer->windowContext->handle) & 4) {
        if ((viewer->flags & 1) == 0) {
            itfMesCleanupWindow(viewer->windowContext->handle, 1);
            itfMesFinishWindowAndClearStatus(viewer->windowContext->handle);
        }
    }
    if (itfPanelGetPairFirst(viewer->windowContext->handle) < 0) {
        itfPanelSetPairFirst(viewer->windowContext->handle, 0);
        datGameState->script.ints[viewer->unk23C6] = itfMesGetWindowClearBitCount(viewer->windowContext->handle);
        itfMesResetWindow(viewer->windowContext->handle);
        evtViewerMarkWindowInactive(viewer);
        viewer->flags ^= 1;
        evtViewerDispatchFlagMode(viewer);
    }
    func_0024AD48(viewer);
}


void evtViewerCleanupMessageWindow(EvtRuntime *viewer) {
    PolyMovieWork *v0;
    s32 v1;

    v0 = viewer->windowContext;
    if (v0 == 0) {
        return;
    }
    v1 = v0->handle;
    if (v1 == -1) {
        return;
    }
    itfMesCleanupWindow(v1, 1);
    v0 = viewer->windowContext;
    itfMesFinishWindowAndClearStatus(v0->handle);
    v0 = viewer->windowContext;
    itfPanelSetPairFirst(v0->handle, 0);
    v0 = viewer->windowContext;
    itfMesResetWindow(v0->handle);
    viewer->windowActive = 0;
    viewer->windowShadeFade = 0;
}

void evtViewerMarkWindowActive(EvtRuntime *viewer) {
    viewer->windowActive = 1;
}

void evtViewerMarkWindowInactive(EvtRuntime *viewer) {
    viewer->windowActive = 0;
}

INCLUDE_ASM(const s32, "game/code_00247DE0", func_0024AD48);

s32 evtViewerTestIndexedCondition(u32 encodedId) {
    u32 idx;
    u32 lo;

    idx = (encodedId << 16) >> 28;
    lo = encodedId & 0xfff;
    if (idx == 0) {
        return 1;
    }
    return (datGameState->script.ints[199 + idx] ^ lo) == 0;
}

u32 func_0024B078(void) {
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_00247DE0", D_004227A0);

INCLUDE_RODATA(const s32, "game/code_00247DE0", D_004227B0);

INCLUDE_RODATA(const s32, "game/code_00247DE0", D_004227C0);

INCLUDE_RODATA(const s32, "game/code_00247DE0", D_004227D0);

s32 func_0024B080(s32 arg0, s32 arg1, EvtRuntime *viewer) {
    switch ((u32)viewer->inputA) {
    case 0:
    case 1:
    case 2:
        viewer->charCol = 0;
        viewer->charRow = 0;
        evtViewerPushCommandHistory(9, 0xE4, 0x3C, viewer);
        break;
    case 5:
        viewer->value = viewer->headerThird - 1;
        viewer->valueMin = viewer->frameRange.word;
        viewer->valueMax = 10000;
        evtViewerPushCommandHistory(7, 0xB4, 0x78, viewer);
        break;
    case 3:
    case 4:
        viewer->cursor = 0;
        viewer->itemCount = 2;
        viewer->title = "FRAME SET.OK? ";
        viewer->itemNames = D_003C91C0;
        evtViewerPushCommandHistory(2, 0xE4, 0x3C, viewer);
        break;
    case 6:
        viewer->commandStart = 1;
        viewer->flags |= 1;
        evtViewerDispatchFlagMode(viewer);
        viewer->actionMode = 0;
        viewer->historyCount = 0;
        viewer->commandResetId = 0;
        D_004372CC = 0;
        return 1;
    case 7:
        viewer->cursor = mnuCampGetPrimaryOption(viewer);
        viewer->itemCount = 2;
        viewer->title = "START FRAME SELECT";
        viewer->itemNames = D_003C91E0;
        evtViewerPushCommandHistory(2, 0xE4, 0x3C, viewer);
        break;
    case 8:
        viewer->value = 0;
        viewer->valueMin = -5000;
        viewer->valueMax = 5000;
        evtViewerPushCommandHistory(7, 0xB4, 0x78, viewer);
        break;
    case 9:
        viewer->cursor = mnuCampGetPrimaryOption(viewer);
        viewer->itemCount = 2;
        viewer->title = "SET BISTAMODE";
        viewer->itemNames = D_003C91C8;
        evtViewerPushCommandHistory(2, 0xE4, 0x3C, viewer);
        break;
    case 10:
        viewer->cursor = mnuCampGetSecondaryOption(viewer);
        viewer->itemCount = 3;
        viewer->title = "SET SKIPMODE";
        viewer->itemNames = D_003C91D0;
        evtViewerPushCommandHistory(2, 0xE4, 0x3C, viewer);
        break;
    }
    return 0;
}


INCLUDE_ASM(const s32, "game/code_00247DE0", func_0024B268);

u32 func_0024B678(u32 unused0, u32 unused1, EvtRuntime *viewer) {
    evtViewerPushCommandHistory(5, 0x90, 0x48, viewer);
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_00247DE0", D_004229A0);

INCLUDE_RODATA(const s32, "game/code_00247DE0", D_004229B0);

INCLUDE_RODATA(const s32, "game/code_00247DE0", D_004229C0);

INCLUDE_RODATA(const s32, "game/code_00247DE0", D_004229D0);

INCLUDE_RODATA(const s32, "game/code_00247DE0", D_004229E0);

INCLUDE_RODATA(const s32, "game/code_00247DE0", D_004229F0);

INCLUDE_RODATA(const s32, "game/code_00247DE0", D_00422A00);

INCLUDE_RODATA(const s32, "game/code_00247DE0", D_00422A10);

INCLUDE_RODATA(const s32, "game/code_00247DE0", D_00422A20);

INCLUDE_RODATA(const s32, "game/code_00247DE0", D_00422A30);

INCLUDE_RODATA(const s32, "game/code_00247DE0", D_00422A40);

INCLUDE_RODATA(const s32, "game/code_00247DE0", D_00422A50);

INCLUDE_RODATA(const s32, "game/code_00247DE0", D_00422A60);

INCLUDE_RODATA(const s32, "game/code_00247DE0", D_00422A70);

INCLUDE_RODATA(const s32, "game/code_00247DE0", D_00422A80);

INCLUDE_RODATA(const s32, "game/code_00247DE0", D_00422A90);

INCLUDE_RODATA(const s32, "game/code_00247DE0", D_00422AA0);

INCLUDE_RODATA(const s32, "game/code_00247DE0", D_00422AB0);

INCLUDE_RODATA(const s32, "game/code_00247DE0", D_00422AC0);

INCLUDE_ASM(const s32, "game/code_00247DE0", func_0024B6A8);

/* Store the edited timing or selector halfword in the pending timeline key. */
s32 evtViewerStoreKeyTimingOrSelector(s32 unused0, s32 unused1, EvtRuntime *viewer) {
    EvtRuntimeChild *key = evtEventViewerGetPendingNode(viewer);
    EvtRuntimeGroup *track;
    s32 kind;
    s8 category;

    if (key != NULL) {
        track = viewer->frameGroup;
        kind = track->type;
        category = D_003C953A[viewer->frameColumn + kind * 10];
        switch (category) {
        case 0:
            key->frame = viewer->value - track->metadata.value;
            evtReorderListNodes(track);
            evtApplyViewerTimelineFrame(viewer->curFrame, viewer);
            break;
        case 9:
            switch (kind) {
            case 3:
            case 20:
            case 21:
            case 26:
                key->p08.sh[1] = viewer->value;
                break;
            case 18:
                key->p08.sh[0] = viewer->value;
                break;
            }
            break;
        case 15:
            key->duration = viewer->value;
            break;
        }
        evtViewerPopHistory(viewer);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00247DE0", func_0024C650);

/* Store the command value as a halfword and clear its extra halfword when tagged. */
s32 evtViewCmdSetValue(s32 unused0, s32 unused1, EvtRuntime *viewer) {
    s32 value = viewer->value;
    EvtRuntimeChild *entry = evtEventViewerGetPendingNode(viewer);

    if (entry == NULL) {
        return 0;
    }
    entry->p08.h[0] = value;
    if (((u32)(value << 16) >> 28) != 0) {
        entry->p08.h[1] = 0;
    }
    evtApplyViewerTimelineFrame(viewer->curFrame, viewer);
    evtViewerPopHistory(viewer);
    return 0;
}

/* Store the command value in the selected halfword of a viewer entry. */
u32 evtViewerStoreCommandInSelectedField(u32 unused0, u32 unused1, EvtRuntime *viewer) {
    EvtRuntimeChild *entry;
    u32 value;
    s32 slot;
    value = viewer->value;
    entry = evtEventViewerGetPendingNode(viewer);
    if (entry != 0) {
        slot = viewer->frameGroup->type - 1;
        if ((u32)slot < 0x13u) {
            switch (slot) {
            case 0:
                entry->p10.h[0] = value;
                break;
            case 11:
            case 15:
            case 16:
                entry->p14.h[0] = value;
                break;
            case 3:
                entry->p08.h[1] = value;
                break;
            case 4:
                entry->p08.h[1] = value;
                break;
            case 1:
                entry->p0C.h[0] = value;
                break;
            case 18:
                entry->p0C.h[0] = value;
                break;
            }
        }
        evtApplyViewerTimelineFrame(viewer->curFrame, viewer);
        evtViewerPopHistory(viewer);
        return 0;
    }
}

u32 func_0024C9D0(void) {
    return 0;
}

/* Copy the current command word into the selected script entry. */
u32 evtViewerStoreCommandInEntryWord(u32 unused0, u32 unused1, EvtRuntime *viewer) {
    EvtRuntimeChild *entry;

    entry = evtEventViewerGetPendingNode(viewer);
    if (entry != 0) {
        entry->p0C.i = viewer->value;
        evtApplyViewerTimelineFrame(viewer->curFrame, viewer);
        evtViewerPopHistory(viewer);
        return 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00247DE0", func_0024CA28);

u32 kwlnBattleCopyMatrix(u32 unused0, u32 unused1, EvtRuntime *viewer) {
    EvtRuntimeChild *key = evtEventViewerGetPendingNode(viewer);
    if (key != NULL) {
        f32 *dst = key->payload;
        f32 *src = viewer->commandMatrix + 8;
        s32 index = 3;
        do {
            index--;
            dst[0] = src[-8];
            dst[4] = src[-4];
            dst[8] = src[0];
            src++;
            dst++;
        } while (index >= 0);
        evtApplyViewerTimelineFrame(viewer->curFrame, viewer);
        evtViewerPopHistory(viewer);
        return 0;
    }
    return (u32)key;
}

/* Transfer a selected two-component viewer position to the command entry. */
s32 evtViewCmdSetPosition(s32 unused0, s32 unused1, EvtRuntime *viewer) {
    EvtRuntimeChild *entry = evtEventViewerGetPendingNode(viewer);

    if (entry == NULL) {
        return 0;
    }
    if (viewer->frameGroup == 0) {
        return 0;
    }
    entry->p08.f = viewer->floatEditX;
    entry->p0C.f = viewer->floatEditY;
    evtApplyViewerTimelineFrame(viewer->curFrame, viewer);
    evtViewerPopHistory(viewer);
    return 0;
}

u32 evtViewCmdCancelSelection(u32 unused0, u32 unused1, u32 viewerAddr) {
    evtViewerPopHistory((EvtRuntime *)viewerAddr);
    return 0;
}

s32 evtViewCmdResolveSlot(s32 operation, void *argument, EvtRuntime *viewer) {
    s32 count = 0;
    EvtRuntimeChild *key;
    EvtRuntimeGroup *group;
    s32 selection;

    key = evtEventViewerGetPendingNode(viewer);
    selection = viewer->groupFirst + viewer->groupCursor;
    if (selection == 0) {
        key->p10.h[0] = 0;
    } else if (selection == 1) {
        key->p10.h[0] = 1;
    } else {
        s32 ordinal = selection - 2;

        group = viewer->groups;
        while (group != NULL) {
            if (group->type == 0x18) {
                if (count == ordinal) {
                    u16 entry = group->entryHeader;
                    key->p10.h[0] = entry + 2;
                    break;
                }
                count++;
            }
            group = group->next;
        }
    }
    evtApplyViewerTimelineFrame(viewer->curFrame, viewer);
    evtViewerPopHistory(viewer);
    return 0;
}


/* Copy the selected slot descriptor and numeric value into the script entry. */
s32 evtViewCmdSetSlot(s32 unused0, s32 unused1, EvtRuntime *viewer) {
    EvtRuntimeChild *entry = evtEventViewerGetPendingNode(viewer);

    entry->p0C.b[0] = viewer->shadowMode;
    entry->p0C.b[1] = viewer->shadowAlpha;
    entry->p14.f = viewer->shadowY;
    evtApplyViewerTimelineFrame(viewer->curFrame, viewer);
    evtViewerPopHistory(viewer);
    return 0;
}

/* Apply one of three viewer selection modes to the selected slots. */
s32 evtViewCmdSelectMode(s32 unused0, s32 unused1, EvtRuntime *viewer) {
    s32 handled = 0;
    s32 mode = viewer->inputA;

    if (mode < 3) {
        if (mode >= 0) {
            func_0035C860(viewer->eventName, D_00423050, D_004372B0, D_004372B2);
            mode = viewer->inputA;
            if (mode == 0) {
                evtViewerSaveTrackFiles(0, viewer);
                evtViewerSaveTrackFiles(1, viewer);
            } else if (mode == 1) {
                evtReloadEventViewer(0, viewer);
            } else if (mode == 2) {
                evtReloadEventViewer(1, viewer);
            }
            handled = 1;
        }
    }
    return handled ? -1 : 0;
}

INCLUDE_ASM(const s32, "game/code_00247DE0", func_0024CE18);

u32 evtViewerClearPendingNodeAndPushHistory(u32 unused0, u32 unused1, EvtRuntime *viewer) {
    if (evtEventViewerGetPendingNode(viewer) != 0) {
        viewer->groupFirst = 0;
        viewer->groupCursor = 0;
        evtViewerPushCommandHistory(0xa, 0x9c, 0x54, viewer);
        return 0;
    }
}

/* Restore default effect parameters for the selected timeline key. */
INCLUDE_RODATA(const s32, "game/code_00247DE0", D_00423050);

s32 func_0024D148(s32 unused0, s32 unused1, EvtRuntime *viewer) {
    EvtRuntimeChild *entry = evtEventViewerGetPendingNode(viewer);
    u128 *destination;
    u128 *source;
    EvtViewerDrawPayload *draw;

    switch (viewer->frameGroup->type) {
    case 10:
        switch (viewer->commandCategory) {
        case 13:
            destination = entry->payload;
            source = D_0037F770[0];
            PCP_COPY_VECTOR(destination, source);
            PCP_COPY_VECTOR(destination + 1, source + 1);
            PCP_COPY_VECTOR(destination + 2, kwlnDefaultColorVector);
            break;
        case 14:
            entry->p08.f = D_00438A48;
            entry->p0C.f = D_00438A4C;
            break;
        }
        break;
    case 11:
        draw = entry->payload;
        draw->x = kwlnDrawVector.y;
        draw->mode = kwlnDrawVector.mode;
        draw->y = kwlnDrawVector.x;
        draw->z = kwlnDrawVector.z;
        draw->w = kwlnDrawVector.w;
        break;
    case 13:
        memcpy(entry->payload, effGetLoadDescA(), 0x28);
        break;
    case 14:
        memcpy(entry->payload, effEventGetBlurTemplateSetupParams(), 0x2C);
        break;
    case 15:
        memcpy(entry->payload, effEventGetScatterBlurSetupParams(), 0x2C);
        break;
    case 23:
        memcpy(entry->payload, effEventGetScaleBlurSetupParams(), 0x2C);
        break;
    case 27:
        memcpy(entry->payload, effGetLoadDescD(), 0x28);
        break;
    case 16:
        memcpy(entry->payload, effEventGetSolidRectangleSetupParams(), 0x18);
        break;
    case 17:
        memcpy(entry->payload, effEventGetResourceTemplateSetupParams(), 0x24);
        break;
    }
    evtApplyViewerTimelineFrame(viewer->curFrame, viewer);
    evtViewerPopHistory(viewer);
    return 0;
}


/* Hand control back to the frame update whether or not the task still owns a viewer. */
s32 evtViewerPickNextHandler(KwlnTask *task) {
    if (kwlnTaskGetUserValue(task) != 0) {
        return (s32)evtViewerUpdateFrame;
    }
    return (s32)evtViewerUpdateFrame;
}

extern s16 D_004372B4;
extern s8 D_0037F510[];
extern void itfMesStartEntry(s32, s32, s32);
extern void evtPrintDeveloperConsoleMessage(const char *, ...);
extern void evtUpdateViewerMessageWindow(EvtRuntime *);
extern void mnuAdvanceShopMenuState();
extern u8 func_002A8028(void);
extern s32 evtViewerPickNextHandler(KwlnTask *);

/* Advance the viewer timeline, deferred voice and task-update handoff. */
s32 evtViewerUpdateFrame(KwlnTask *task) {
    EvtRuntime *viewer = (EvtRuntime *)kwlnTaskGetUserValue(task);
    s32 flags;

    D_004372B4 = 0;
    if (viewer->unk2238 == 1) {
        func_0024A5F8(viewer);
    }
    evtViewerAdvanceGlyphTick(viewer);
    func_002476B8(viewer);
    flags = viewer->flags;
    if (!(flags & 8)) {
        if (D_0037F510[0x22] < 0) {
            viewer->flags = flags | 1;
            evtViewerDispatchFlagMode(viewer);
            viewer->actionMode = 0;
            viewer->historyCount = 0;
            viewer->commandResetId = 0;
            evtViewerPushCommandHistory(1, 0x24, 0x18, viewer);
            return (s32)evtViewerPickNextHandler;
        }
        func_0024A668(viewer);
    } else {
        if (viewer->curFrame == viewer->headerFirst) {
            viewer->flags = flags & ~1;
            evtViewerDispatchFlagMode(viewer);
        }
        if (D_0037F510[0x22] >= 0 && D_0037F510[0x2C] < 0 &&
            evtViewerHasUpdateFlag(viewer) == 0) {
            func_00249EE8(viewer);
        }
    }
    if (viewer->voicePending == 1 && evtViewerHasUpdateFlag(viewer) == 0 &&
        mnuQueryTitleSoundBusy() == 0) {
        itfMesStartEntry(viewer->windowContext->handle,
            viewer->voiceMessage, 0);
        viewer->voicePending = 0;
        evtPrintDeveloperConsoleMessage("[conflict voice play      ] mesno= %d\n",
            viewer->voiceMessage);
    }
    if (viewer->curFrame != viewer->previousGlyphPosition) {
        evtApplyViewerTimelineFrame(viewer->curFrame, viewer);
    }
    viewer->previousGlyphPosition = viewer->curFrame;
    if (viewer->flags & 8) {
        evtViewerApplySelectedEntry(viewer);
    }
    evtUpdateViewerMessageWindow(viewer);
    func_0024A020(viewer);
    mnuAdvanceShopMenuState(viewer);
    if (viewer->flags & 8) {
        if (viewer->curFrame >= viewer->frameRange.word) {
            return -1;
        }
        if (evtViewerHasUpdateFlag(viewer) == 1 && viewer->updateCount >= 25) {
            return -1;
        }
    }
    if (!(viewer->flags & 1)) {
        if (viewer->curFrame >= viewer->frameRange.word) {
            if (!(viewer->flags & 8)) {
                viewer->flags ^= 1;
                evtViewerDispatchFlagMode(viewer);
            }
        } else if (viewer->timedActive == 1) {
            if (func_002A8028() == 2) {
                viewer->curFrame++;
            }
        } else {
            viewer->curFrame++;
        }
    }
    return 0;
}

/* Update the active viewer, then switch to its frame-variable task. */
void *evtViewerScheduleFrameVariableTask(KwlnTask *task) {
    void *viewer;

    viewer = (void *)kwlnTaskGetUserValue(task);
    evtApplyViewerTimelineFrame(((EvtRuntime *)viewer)->curFrame, viewer);
    func_00101968(task, evtCreateFrameVariableTask());
    kwlnDrawControlFlags |= 0x2000000;
    return (void *)evtViewerUpdateFrame;
}

/* Initialize the active viewer and schedule its next update callback. */
void *evtViewerInitializeUpdateSequence(KwlnTask *task) {
    struct EvtRuntime *viewer;

    viewer = (struct EvtRuntime *)kwlnTaskGetUserValue(task);
    fldInitializeCameraColorResource();
    evtEventViewerReset(viewer);
    kwlnDrawControlFlags |= 0x2000000;
    return (void *)evtViewerScheduleFrameVariableTask;
}

s32 func_0024D760(PolyMovieWork *ctx) {
    s32 id = ctx->eventId;

    if (id == 0x28B || id == 0x28E) {
        return 1;
    }
    return 0;
}

/* Advance the viewer update: tick the timed action or hand over to the next task. */
void *evtViewerAdvanceUpdate(KwlnTask *task) {
    EvtRuntime *viewer = (EvtRuntime *)kwlnTaskGetUserValue(task);
    PolyMovieWork *window;
    s32 windowFlags;

    func_0024DBB8(viewer->windowContext);
    window = viewer->windowContext;
    windowFlags = window->flags;
    if ((windowFlags & 8) == 0) {
        kwlnDrawControlFlags |= 0x2000000;
        return 0;
    } else {
        if ((windowFlags & 1) != 0) {
            kwlnDrawControlFlags |= 0x2000000;
            return 0;
        }
        if (func_0024D760(window) == 0) {
            if ((u32)(mnuPollTitleStreamStateLocked() - 3) < 2) {
                if (viewer->titleStreamWaitFrames == 0x78) {
                    mnuMarkTitleStreamResetPending();
                }
                viewer->titleStreamWaitFrames++;
                kwlnDrawControlFlags |= 0x2000000;
                return 0;
            }
        }
        func_0025A280(viewer->windowContext, viewer);
        kwlnDrawControlFlags |= 0x2000000;
        return (void *)evtViewerScheduleFrameVariableTask;
    }
}

s32 evtViewerStartUpdate(KwlnTask *task) {
    EvtRuntime *viewer = (EvtRuntime *)kwlnTaskGetUserValue(task);
    PolyMovieWork *context;
    u16 eventId;
    u16 sceneId;

    fldInitializeCameraColorResource();
    context = viewer->windowContext;
    eventId = context->eventId;
    sceneId = context->sceneId;
    D_004372B0 = eventId;
    D_004372B2 = sceneId;
    func_0035C860(viewer->eventName, D_00423050, D_004372B0, D_004372B2);
    viewer->flags = 1;
    evtViewerDispatchFlagMode(viewer);
    viewer->unk2238 = 0;
    viewer->flags |= 8;
    kwlnDrawControlFlags |= 0x2000000;
    return (s32)(u32)evtViewerAdvanceUpdate;
}

u8 func_0024D908(PolyMovieWork *task) {
    return task->eventId == 0x263;
}

extern f32 D_0037F590[];
extern void evtResetUnitVectorSlots();
extern void mnuCampLinkFontGlyph();
extern void func_0014E668();
extern void kwlnCancelConfiguredFadeFrames();
extern s32 sdfCheckPendingWorkWithInterrupts();
extern void evtDestroySecondaryWorldNode();
extern void kwlnTextureReleaseHeldReference();
extern void fldReleaseCameraColorEffect();
extern void kwlnFadeSetMode();
extern void mnuReleaseCampSceneRegisteredIds(EvtRuntime *viewer);
void evtViewerCleanupMessageWindow(EvtRuntime *viewer);

void evtViewerRelease(viewer)
    EvtRuntime *viewer;
{
    if (func_0024D908(viewer->windowContext) == 0) {
        mnuReleaseCampSceneRegisteredIds(viewer);
    }
    evtResetUnitVectorSlots();
    evtViewerCleanupMessageWindow(viewer);
    mnuCampLinkFontGlyph(viewer);
    func_0014E668(0);
    kwlnCancelConfiguredFadeFrames();
    D_0037F590[0] = D_0037F590[1] = D_0037F590[2] = D_0037F590[3] = 0.0f;
    if (viewer->timedActive == 1) {
        if (viewer->timedEnd != -2 || evtViewerHasUpdateFlag(viewer) == 1) {
            mnuStopMovieDrawTask();
        }
        viewer->timedActive = 0;
    }
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    evtDestroySecondaryWorldNode();
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    if (viewer->pendingResource != 0) {
        sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)viewer->pendingResource);
        viewer->pendingResource = 0;
        viewer->pendingWork = 0;
    }
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    kwlnTextureReleaseHeldReference();
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    evtEventViewerReleaseGroups(viewer);
    evtEventViewerShutdown(viewer);
    sdfReleaseResourceAllocation(viewer->resourceHandle);
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    fldReleaseCameraColorEffect();
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    kwlnFadeSetMode(0);
    kwlnDrawControlFlags |= 0x2000000;
}

void func_0024DAA0(KwlnTask *task) {
    EvtRuntime *viewer;

    viewer = (EvtRuntime *)kwlnTaskGetUserValue(task);
    evtViewerRelease(viewer);
}

void func_0024DAC0(KwlnTask *task) {
    EvtRuntime *viewer;

    viewer = (EvtRuntime *)kwlnTaskGetUserValue(task);
    evtViewerRelease(viewer);
}

extern void mnuCampInitFontResource(EvtRuntime *viewer);

void func_0024DAE0(EvtRuntime *viewer) {
    mnuCampInitFontResource(viewer);
}

extern u32 D_00435CBC;
extern void *memset(void *dst, s32 value, u32 size);
extern void *kwlnTaskCreate(const char *name, s32 id, s32 arg2, s32 arg3, void *update, void *destroy, void *data);
void evtViewerCreateTaskWithSky(void) {
    EvtRuntime *viewer;
    SdfMemBlock *viewerHandle;
    void *viewerTask;

    D_00435CBC = 0x80000000;
    viewerHandle = sdfAllocGeneralBlock(0x24BC);
    viewer = (EvtRuntime *)sdfResourceRetainAddress(viewerHandle);
    memset(viewer, 0, 0x24BC);
    viewer->resourceHandle = viewerHandle;
    viewerTask = kwlnTaskCreate(evtViewerTaskName, 0x3EB, 1, 1, evtViewerInitializeUpdateSequence, func_0024DAA0, viewer);
    func_00101968((s32)viewerTask, evtCreateSkyTask());
    func_0024DAE0(viewer);
}

void evtEventViewerDestroyTask(void) {
    kwlnTaskDestroyWithHierarchyByName(evtViewerTaskName, 1);
}


void func_0024DBB8(PolyMovieWork *assets) {

    if (assets->flags & 8) {
        return;
    }
    if (assets->flags & 2) {
        if (assets->mainResource.request == 0) {
            return;
        }
        if (fileIsRequestReadyInCurrentMode((struct FileRequest *)assets->mainResource.request) == 0) {
            return;
        }
        assets->mainResource.handle = (SdfMemBlock *)fileGetResourceHandle((struct FileRequest *)assets->mainResource.request);
        assets->mainResource.address = (PmdHeader *)sdfResourceRetainAddress(assets->mainResource.handle);
        filePollEntryCleanup(assets->mainResource.request);
        assets->mainResource.request = 0;
        assets->flags &= ~2;
    } else if (assets->flags & 4) {
        if (assets->secondaryResource.request == 0) {
            return;
        }
        if (fileIsRequestReadyInCurrentMode((struct FileRequest *)assets->secondaryResource.request) == 0) {
            return;
        }
        assets->secondaryResource.handle = (SdfMemBlock *)fileGetResourceHandle((struct FileRequest *)assets->secondaryResource.request);
        assets->secondaryResource.address = (PmdHeader *)sdfResourceRetainAddress(assets->secondaryResource.handle);
        filePollEntryCleanup(assets->secondaryResource.request);
        assets->secondaryResource.request = 0;
        assets->flags &= ~4;
    } else if (assets->flags & 0x10) {
        if (assets->tertiaryResource.request == 0) {
            return;
        }
        if (fileIsRequestReadyInCurrentMode((struct FileRequest *)assets->tertiaryResource.request) == 0) {
            return;
        }
        assets->tertiaryResource.handle = (SdfMemBlock *)fileGetResourceHandle((struct FileRequest *)assets->tertiaryResource.request);
        assets->tertiaryResource.address = (PmdHeader *)sdfResourceRetainAddress(assets->tertiaryResource.handle);
        filePollEntryCleanup(assets->tertiaryResource.request);
        assets->tertiaryResource.request = 0;
        assets->flags &= ~0x10;
    } else if (assets->mainResource.address != NULL && assets->secondaryResource.address != NULL) {
        evtPolygonMovieInitWork(assets, assets->mainResource.address, assets->secondaryResource.address, assets->tertiaryResource.address);
        assets->flags |= 8;
    }
}

INCLUDE_RODATA(const s32, "game/code_00247DE0", evtViewerTaskName);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_004372B0);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_004372B2);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_004372B4);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_004372B8);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_004372C0);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_004372C8);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_004372CC);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_004372D0);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_004372D8);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_004372E0);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_004372E8);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_004372F0);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_004372F8);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_00437300);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_00437308);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_00437310);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_00437318);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_00437320);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_00437328);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_00437330);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_00437338);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_00437340);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_00437348);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_00437350);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_00437358);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_00437360);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_00437368);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_00437370);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_00437378);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_00437380);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_00437388);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_00437390);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_00437398);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_004373A0);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_004373A8);

INCLUDE_SDATA(const s32, "game/code_00247DE0", D_004373B0);

