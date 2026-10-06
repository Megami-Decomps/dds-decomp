#include "common.h"
#include "kwln.h"
#include "sdf.h"
#include "sdf_draw.h"
#include "dds3obj.h"
#include "evt_world.h"
#include "pcp_vu0.h"
#include "evt_unit.h"
#include "eff_transform.h"
#include "dat_state.h"
extern u16 D_004372B0;
extern u16 D_004372B2;
extern u8 D_00423050[];
extern s32 func_0035C860(char *buffer, const char *format, ...);
extern void func_00259AE8();
extern void evtReloadEventViewer();
extern void *dds3GetWorldObject(void);
extern void dds3SetWorldCameraObject(void *, s32);
extern f32 dds3GetCameraFieldOfView(s32);
extern void func_001063A8(f32);

extern void effObjSetInnerFirstVec(EffTransformNode *, u128 *);
extern void effObjSetInnerSecondVec(EffTransformNode *, u128 *);
extern void effObjFetchInnerFirstVec(EffTransformNode *);
extern u32 *dds3FindObjectChainNodeByName(EvtWorldObject *, const u8 *);
extern void mdlAttachWorldObjectToSourceVector(s32, s32);

extern s32 evtViewerHasUpdateFlag(s32);
extern u8 D_004372C0[3];
extern u32 kwlnGetDrawBufferIndex(void);
extern void kwlnFadeSetColor(s32 red, s32 green, s32 blue, s32 alpha);

/* Effect-channel assignments driven by the viewer's timeline tracks. */
typedef struct EvtCampEntry {
    u8 pad00[0x24];
    s32 value; /* 0x24 */
} EvtCampEntry;

extern s32 evtViewerTestIndexedCondition(u32 encodedId);
extern void effInitCh71Id(void);
extern void effInitCh72Id(void);
extern void effInitCh75Id(void);
extern void effInitCh76Id(void);
extern void effSetCh71Id(u32 resourceWord);
extern void effSetCh72Id(u32 sourceHandle);
extern void effSetCh75Id(u32 resourceWord);
extern void effSetCh76Id(u32 sourceHandle);
extern void *mnuCampFindEntryByName(void *scene, const char *name);


s32 evtEventViewerGetPendingNode(s32 arg0);

void func_00249088(s32 arg0, void *arg1);

void evtViewerPushCommandHistory(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

/* Task user values are words; viewer callbacks decode the stored address. */
extern u32 kwlnTaskGetUserValue();

s32 evtViewerUpdateFrame(KwlnTask *task);

void func_00101968(s32 arg0, s32 arg1);

s32 evtCreateFrameVariableTask(void);

void *evtViewerScheduleFrameVariableTask(s32 arg0);

extern u32 mnuCampGetPrimaryOption(void *scene);
extern const char *D_003C91C0[];
extern const char *D_003C91C8[];
extern const char *D_003C91D0[];
extern const char *D_003C91E0[];
extern s32 D_004372CC;

extern u32 kwlnDrawControlFlags;

void fldInitializeCameraColorResource(void);

extern void func_0024DBB8(s32 arg0);
extern s32 mnuPollTitleStreamStateLocked(void);
extern void mnuMarkTitleStreamResetPending(void);
extern void func_0025A280(s32 arg0, void *arg1);
extern s32 func_0024D760(u8 *ctx);

struct EvtViewer;
void evtEventViewerReset(struct EvtViewer *viewer);

struct EffNode;

typedef struct EvtViewerObjectData {
    u8 pad00[0xC];
    struct EffNode *parameterNode;
} EvtViewerObjectData;

typedef struct EvtWorldLink {
    u8 pad00[8];
    u8 *name;
    u8 pad0C[0xC];
    void *data;
    u8 pad1C[4];
    struct EvtWorldLink *next;
} EvtWorldLink;

typedef struct EventViewerState {
    u32 resourceHandle; /* 0x00 */
    s32 flags;          /* 0x04: evtViewerHasUpdateFlag reads this as s32 */
    s32 windowContext; /* 0x08: owns the message-window handle at +0x104 */
    s32 frameCount; /* 0x0C */
    s32 glyphAdvanceStart; /* 0x10 */
    s32 glyphAdvanceLimit;
    s32 glyphAdvancePosition;
    s32 previousGlyphPosition; /* 0x1C */
    u8 pad20[4];
    u8 unitNames[256][32];
    s32 selectedEntry; /* 0x2024 */
    u8 pad2028[4];
    s32 fallbackEntry; /* 0x202C */
    u8 pad2030[4];
    struct EvtViewTrack *tracks; /* 0x2034 */
    u8 pad2038[4];
    EvtWorldLink *objects[127]; /* 0x203C */
    s32 unk2238;
    struct {
        u16 id;
        u16 a;
        u16 b;
        u16 pad6;
    } history[8];
    s32 historyCount;
    u32 currentId;
    s32 commandResetId; /* 0x2284 */
    u8 pad2288[4];
    void *unk228C;
    s32 blurRectangleEnabled; /* 0x2290 */
    s32 texturedBlurEnabled;  /* 0x2294 */
    s32 filterBlurEnabled;    /* 0x2298 */
    s32 colorRectangleEnabled; /* 0x229C */
    s32 texturedSquareEnabled; /* 0x22A0 */
    s32 staggeredBlurEnabled; /* 0x22A4 */
    s32 selectionMode; /* 0x22A8: command mode zero, one or two */
    s32 unk22AC;
    u8 pad22B0[4];
    s32 unk22B4;
    s32 unk22B8;
    s32 optionSelection; /* 0x22BC */
    s32 optionCount; /* 0x22C0 */
    const char *optionTitle; /* 0x22C4 */
    const char **optionNames; /* 0x22C8 */
    s32 commandResetA; /* 0x22CC: cleared on command mode three */
    s32 commandResetB; /* 0x22D0 */
    u8 commandResetC;  /* 0x22D4 */
    u8 pad22D5[0xB];
    u8 commandResetD;  /* 0x22E0 */
    u8 pad22E1[7];
    char eventName[0x14];
    s32 commandTableOffset; /* 0x22FC: byte offset into command descriptors */
    s32 unk2300;
    s32 unk2304;
    struct EvtViewTrack *sel; /* 0x2308: selected timeline track */
    s32 commandCategory; /* 0x230C: selected parameter category. */
    u32 commandValue; /* 0x2310: value of the active command */
    s32 commandMinimum; /* 0x2314 */
    s32 commandMaximum; /* 0x2318 */
    f32 unk231C;
    f32 unk2320;
    f32 unk2324;
    s32 unk2328;
    s32 unk232C;
    f32 commandMatrix[12];
    f32 savedCommandMatrix[12];
    s32 unk2390;
    s32 unk2394;
    s32 unk2398;
    u8 pad239C[4];
    s32 unk23A0;
    s32 unk23A4;
    f32 commandX; /* 0x23A8 */
    f32 commandY; /* 0x23AC */
    f32 unk23B0;
    f32 unk23B4;
    s32 unk23B8;
    u8 pad23BC[4];
    s32 updateCount;
    u8 pad23C4;
    u8 windowActive;
    u8 pad23C6[2];
    s32 unk23C8;
    u8 pad23CC[4];
    s32 ch71; /* 0x23D0 */
    s32 ch72; /* 0x23D4 */
    s32 ch76; /* 0x23D8 */
    s32 ch75; /* 0x23DC */
    u8 pad23E0[0x10];
    s32 glyphTickCount;
    s32 unk23F4;
    s32 framebufferQuadEnabled; /* 0x23F8 */
    u8 pad23FC[8];
    s32 unk2404;
    s32 unk2408;
    s32 unk240C;
    u32 glyph;
    s32 timedActive; /* 0x2414: gated time interval */
    s32 timedStart;  /* 0x2418 */
    s32 timedEnd;    /* 0x241C: negative is an open endpoint */
    u8 slotType;     /* 0x2420 */
    u8 slotFlag;     /* 0x2421 */
    u8 pad2422[2];
    f32 slotValue;   /* 0x2424 */
    s32 pendingWork; /* 0x2428: reset when pendingResource is released */
    s32 pendingResource; /* 0x242C */
    u8 pad2430[0x10];
    s32 titleStreamWaitFrames; /* 0x2440 */
    u8 pad2444[0x54];
    s32 voicePending; /* 0x2498 */
    s32 voiceMessage; /* 0x249C; work allocation is 0x24BC bytes */
    s32 unk24A0;
    u8 pad24A4[0xC];
    s32 commandStart; /* 0x24B0 */
    u8 pad24B4[8];
} EventViewerState;

/* Handles retained by the viewer and by its owning task context. */
typedef struct EvtViewerAssetSlot {
    void *request;
    s32 resource;
    u32 *address;
} EvtViewerAssetSlot;

typedef struct EvtWindowContext {
    s32 flags; /* 0x00 */
    EvtViewerAssetSlot first;
    u8 pad10[0x4C];
    EvtViewerAssetSlot second;
    EvtViewerAssetSlot third;
    u8 pad74[0x90];
    s32 windowHandle;
} EvtWindowContext;

typedef struct EvtTaskContext {
    u8 pad00[0x10C];
    s32 taskId;
} EvtTaskContext;


/* Script-command parameter slots are interpreted according to track kind. */
typedef union EvtViewParam {
    f32 f;
    s32 i;
    u32 u;
    u16 h[2];
    s16 sh[2];
    u8 b[4];
    s8 sb[4];
} EvtViewParam;


u16 evtViewerPopHistory(EventViewerState *viewer);

extern char evtViewerTaskName[]; /* "EventViewer" */

extern s32 kwlnTaskDestroyWithHierarchyByName(const char *name, s32 arg1);

struct PolyMovieObject;

/* Each timeline parameter word's scalar format is selected by the track kind. */
typedef struct EvtViewKey {
    u16 frame;
    u16 duration;
    s32 interpolationMode;
    EvtViewParam p08;
    EvtViewParam p0C;
    EvtViewParam p10;
    EvtViewParam p14;
    EvtViewParam p18;
    EvtViewParam p1C;
    u8 pad20[0xC];
    void *payload;
    struct EvtViewKey *next;
    struct EvtViewKey *previous;
} EvtViewKey;

/* Linked timeline track. Saved vectors and the attachment latch are used by
 * kind-1 world-object tracks; the vectors keep their original embedded layout. */
typedef struct EvtViewTrack {
    s32 kind;                 /* 0x00 */
    u8 pad04[0xC];
    union {
        EffTransformNode *transform;
        s32 transitionValue;
        u32 handle;
        struct PolyMovieObject *movie;
    } owner;                  /* 0x10: payload role is selected by kind */
    u8 pad14[8];
    s16 frameOffset; /* 0x1C: added to relative key frames. */
    s8 playbackTimeMode; /* 0x1E */
    u8 pad1F[5];
    s32 unk24;
    s32 keyMode;              /* 0x28 */
    f32 savedFirstVector[4]; /* 0x2C: restored when detaching a world object. */
    f32 savedSecondVector[4]; /* 0x3C */
    s32 objectAttached; /* 0x4C: world-object attachment needs restoring. */
    s32 hasKeys; /* 0x50 */
    EvtViewKey *keys; /* 0x54 */
    EvtViewKey *lastKey; /* 0x58 */
    u8 pad5C[0x20];
    struct EvtViewTrack *next; /* 0x7C */
    struct EvtViewTrack *previous; /* 0x80 */
} EvtViewTrack;
extern s8 D_003C953A[];
extern void evtReorderListNodes(EvtViewTrack *track);

extern void func_0025E460(u16 *from, u16 *to, u8 *out, f32 ratio);
typedef struct CampDisplayDefaults CampDisplayDefaults;
extern void mnuDrawCampScaledTexture(SdfTex *texture, CampDisplayDefaults *display);

/* Interpolates parameter keys at the viewer's current frame, accounting for
 * the track offset. DDS2 subtracts 35 from the second output word before applying it. */
void evtViewerApplyInterpolatedNodeKey(EventViewerState *viewer, EvtViewTrack *node, u16 *from, u16 *to) {
    u8 out[0x20];
    f32 ratio = 0.0f;

    if (from != NULL) {
        if (to != NULL) {
            s32 start = *from;
            f32 span = *to - start;
            f32 elapsed = viewer->glyphAdvancePosition - (start + node->frameOffset);

            if (span != 0.0f) {
                ratio = elapsed / span;
            }
        }
        func_0025E460(from, to, out, ratio);
        *(s32 *)(out + 4) -= 35;
        mnuDrawCampScaledTexture((SdfTex *)node->unk24, (CampDisplayDefaults *)out);
    }
}

/* Applies kind-24 parameter tracks using bracketing keys, or the pending key
 * when keyMode is 1. Traversal uses the shared timeline-key record. */
void evtViewerApplyParameterKeyTracks(EventViewerState *viewer) {
    EvtViewTrack *node = viewer->tracks;
    s32 position = viewer->glyphAdvancePosition;

    while (node != NULL) {
        if (node->kind == 24) {
            if (node->keyMode == 1) {
                u16 *key = (u16 *)evtEventViewerGetPendingNode((s32)viewer);
                evtViewerApplyInterpolatedNodeKey(viewer, node, key, NULL);
            } else {
                EvtViewKey *glyph = node->keys;
                EvtViewKey *from;

                while (glyph != NULL && position >= glyph->frame + node->frameOffset) {
                    glyph = glyph->next;
                }
                if (glyph != NULL) {
                    from = glyph->previous;
                } else {
                    from = node->lastKey;
                }
                evtViewerApplyInterpolatedNodeKey(viewer, node, (u16 *)from, (u16 *)glyph);
            }
        }
        node = node->next;
    }
}

typedef struct BlurSource {
    u8 color[4];
    s32 blendControl;
    f32 rotation;
    f32 scale;
    s32 centerX;
    s32 centerY;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
} BlurSource;

typedef struct EffScreenDrawParams {
    BlurSource source;
    u8 pad28[8];
} EffScreenDrawParams;

typedef struct EffSolidRectParams {
    u32 color;
    s32 blendControl;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
} EffSolidRectParams;

typedef struct EffBlurTemplateBody {
    s32 extent;
    BlurSource source;
} EffBlurTemplateBody;

typedef struct EffBlurScatterParams {
    s32 count;
    s32 delaySpread;
    f32 angleStep;
    u32 color;
    s32 unk10;
    f32 unk14;
    f32 unk18;
    s32 x;
    s32 y;
    s32 positionSpread;
    s32 size;
} EffBlurScatterParams;

typedef struct EffBlurScaleParams {
    s32 count;
    f32 phaseStep;
    f32 spacing;
    u32 color;
    s32 unk10;
    f32 unk14;
    f32 unk18;
    f32 angleStep;
    s32 x;
    s32 y;
    s32 size;
} EffBlurScaleParams;

typedef struct EffTemplateBody {
    u32 words[9];
} EffTemplateBody;
extern EffScreenDrawParams *effGetLoadDescA(void);

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
extern EffSolidRectParams *effEventGetSolidRectangleSetupParams(void);
extern EffTemplateBody *effEventGetResourceTemplateSetupParams(void);
extern EffScreenDrawParams *effGetLoadDescD(void);
extern void *memcpy(void *destination, const void *source, u32 size);


extern void effDrawBlurRectangle(EffScreenDrawParams *);
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
extern void func_0025EE00(EventViewerState *);

void func_002476B8(EventViewerState *viewer) {
    if (viewer->blurRectangleEnabled != 0) {
        effDrawBlurRectangle(effGetLoadDescA());
    }
    if (viewer->texturedBlurEnabled != 0) {
        effEnableTexturedBlur();
    } else {
        effDisableTexturedBlur();
    }
    if (viewer->texturedSquareEnabled != 0) {
        effEnableTexturedSquare();
    } else {
        effDisableTexturedSquare();
    }
    if (viewer->filterBlurEnabled != 0) {
        effEnableFilterBlur();
    } else {
        effDisableFilterBlur();
    }
    if (viewer->staggeredBlurEnabled != 0) {
        effEnableStaggeredBlur();
    } else {
        effDisableStaggeredBlur();
    }
    if (viewer->framebufferQuadEnabled != 0) {
        effEnableFramebufferQuad();
    } else {
        effDisableFramebufferQuad();
    }
    if (viewer->colorRectangleEnabled != 0) {
        effEnableColorRectangle();
    } else {
        effDisableColorRectangle();
    }
    func_0025EE00(viewer);
    evtViewerApplyParameterKeyTracks(viewer);
}

/* Select the active entry (or fallback) and sync world selection and camera. */
void evtViewerApplySelectedEntry(EventViewerState *viewer) {
    s32 unit;
    s32 first = viewer->selectedEntry;

    if (first != 0) {
        unit = first;
    } else {
        unit = viewer->fallbackEntry;
    }
    if (unit != 0) {
        dds3SetWorldCameraObject(dds3GetWorldObject(), unit);
        func_001063A8(dds3GetCameraFieldOfView(unit));
    }
}

INCLUDE_ASM(const s32, "game/code_00247518", func_00247858);

extern s32 func_0035B6E0(const char *, ...);
extern void effSetNodeParameterValue();

/* Applies duration-scaled alpha to the selected effect for supported track
 * kinds. The key's duration is used directly, without validation here. */
void func_00247DE0(EvtViewTrack *group, EvtViewKey *key, s32 unused2, s32 unused3, EventViewerState *viewer) {
    s32 elapsed;
    struct EffNode *node;
    s32 alpha;
    u32 color;

    switch (group->kind) {
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
    elapsed = viewer->glyphAdvancePosition - key->frame;
    if (key->duration >= elapsed) {
        node = ((EvtViewerObjectData *)viewer->objects[key->p08.sb[0]]->data)->parameterNode;
        alpha = (s32)((128.0f / key->duration) * elapsed);
        func_0035B6E0("alpha=%d\n", alpha);
        color = ((u32)alpha << 24) | 0x808080;
        effSetNodeParameterValue(node, color);
    }
}

INCLUDE_ASM(const s32, "game/code_00247518", func_00247EE0);
INCLUDE_ASM(const s32, "game/code_00247518", func_00248000);

extern EvtWorldLink *dds3FindIndexedObjectChainNodeByName(EvtWorldObject *, s32, const u8 *);
extern s32 evtUnitGetNestedValue(u8 *);
extern void evtSetUnitValueTransition(EvtUnit *, s32, s32);
extern void evtEndUnitValueTransition(EvtUnit *, s32);

/* At time, selects each named unit's latest kind-9 transition key and starts,
 * ends or leaves its value transition according to the selected key flags. */
void func_00248B80(s32 time, EventViewerState *viewer) {
    EvtWorldLink *object;
    EvtViewTrack *node;
    EvtViewKey *key;
    EvtViewKey *selected;
    EvtUnit *unit;
    s32 selectedValue;
    s32 selectedTime;

    if (dds3GetWorldObject() != NULL) {
        object = ((EvtWorldObject *)dds3GetWorldObject())->table->slots[EVT_WORLD_SLOT_UNIT].head;
        while (object != NULL) {
            if (object->name != NULL) {
                selected = NULL;
                selectedValue = 0;
                selectedTime = -1;
                node = viewer->tracks;
                while (node != NULL) {
                    if (node->kind == 9) {
                        key = node->keys;
                        while (key != NULL) {
                            if (time >= key->frame + node->frameOffset && key->p08.sh[0] >= 0 &&
                                object == dds3FindIndexedObjectChainNodeByName(dds3GetWorldObject(),
                                    EVT_WORLD_SLOT_UNIT, viewer->unitNames[key->p08.sh[0]])) {
                                if (selectedTime < key->frame + node->frameOffset) {
                                    selectedValue = node->owner.transitionValue;
                                    selectedTime = key->frame + node->frameOffset;
                                    selected = key;
                                }
                            }
                            key = key->next;
                        }
                    }
                    node = node->next;
                }
                unit = (EvtUnit *)evtUnitGetNestedValue((u8 *)object);
                if (selected != NULL) {
                    if (selected->p08.sh[1] != 0) {
                        if (unit->currentTransitionValue != selectedValue || !(unit->flags & 0x40000)) {
                            evtSetUnitValueTransition(unit, selectedValue, selected->duration);
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

INCLUDE_ASM(const s32, "game/code_00247518", func_00248D70);

INCLUDE_ASM(const s32, "game/code_00247518", func_00249088);

/* Returns the address word of the nearest kind-2 key at/before the current
 * frame, or zero. Equal-distance ties retain the first key visited. */
s32 evtViewFindGlyphAtOrBefore(EventViewerState *viewer) {
    EvtViewKey *result = NULL;
    s32 best = 99999;
    EvtViewTrack *node = viewer->tracks;

    while (node != NULL) {
        if (node->kind == 2) {
            EvtViewKey *glyph = node->keys;

            if (glyph != NULL) {
                do {
                    s32 frame = glyph->frame;

                    if (frame <= viewer->glyphAdvancePosition) {
                        s32 distance = viewer->glyphAdvancePosition - frame;

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

extern s32 dds3GetSlot(s32 owner, s32 kind);
extern void evtSetMovieClipPositionClampedToDuration(s32 object, s32 arg1, s32 start, s32 end, s32 extra);



/* Clamps each movie object's playback interval using its linked track's first
 * key frame (or track offset), endTime, and the current key's extra parameter. */
void evtViewerClampMovieTimes(s32 endTime, EventViewerState *viewer) {
    s32 table;
    s32 slots;
    s32 object;
    EvtViewTrack *node;
    s32 time;
    s32 extra;
    EvtViewKey *glyph;

    if (dds3GetWorldObject() != 0) {
        table = (s32)((EvtWorldObject *)dds3GetWorldObject())->table;
        if (table != 0) {
            slots = (s32)((EvtWorldTable *)table)->slots;
            if (slots != 0) {
                object = (s32)((EvtWorldSlot *)slots)[EVT_WORLD_SLOT_MOVIE].head;
                if (object != 0) {
                    do {
                        node = viewer->tracks;
                        while (node != NULL) {
                            if (node->owner.handle != 0 && object == dds3GetSlot(node->owner.handle, 1)) {
                                if (node->kind == 2) {
                                    time = 0;
                                    if (node->hasKeys != 0) {
                                        time = node->keys->frame;
                                    }
                                    glyph = (EvtViewKey *)evtViewFindGlyphAtOrBefore(viewer);
                                    extra = 0;
                                    if (glyph != NULL) {
                                        extra = glyph->p0C.sh[1];
                                    }
                                } else {
                                    time = node->frameOffset;
                                    extra = 0;
                                }
                                evtSetMovieClipPositionClampedToDuration(object, 0, time, endTime, extra);
                                break;
                            }
                            node = node->next;
                        }
                        object = (s32)((EvtWorldLink *)object)->next;
                    } while (object != 0);
                }
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00247518", func_002496B0);

/* Updates world units at position using the first track whose owner word
 * matches that unit's object-chain node. */
void evtViewerSyncWorldGroups(u32 position, EventViewerState *viewer) {
    u8 *list;
    EvtViewTrack *node;
    EvtViewTrack *found;

    if (dds3GetWorldObject() != 0) {
        list = (u8 *)((EvtWorldObject *)dds3GetWorldObject())->table->slots[EVT_WORLD_SLOT_UNIT].head;
        while (list != 0) {
            found = 0;
            for (node = viewer->tracks; node != 0; node = node->next) {
                if (node->owner.handle == (u32)list) {
                    found = node;
                    break;
                }
            }
            if (found != 0) {
                func_002496B0(position, list, node, viewer, 0);
            }
            list = (u8 *)((EvtWorldLink *)list)->next;
        }
    }
}

extern s32 sdfGetLodChunkValue();

/* Applies the latest kind-6 key at/before position to a kind-1 track's LOD
 * byte, provided the requested signed-byte level is supported by its chunk. */
void evtViewerApplyGlyphLodChannel(s32 position, EventViewerState *viewer) {
    EvtViewTrack *node = viewer->tracks;
    EvtViewKey *glyph;
    EvtViewKey *best;
    s32 bestFrame;
    u8 *lod;
    s8 level;

    while (node != NULL) {
        if (node->kind == 1) {
            glyph = node->keys;
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
            lod = *(u8 **)(*(s32 *)(*(s32 *)(node->owner.transform->ownerData + 0xC) + 0xC) + 0x18);
            if (best == NULL) {
                lod[0x98] = 0;
            } else {
                level = best->p0C.sb[0];
                if (sdfGetLodChunkValue(lod) >= level) {
                    lod[0x98] = best->p0C.sb[0];
                }
            }
        }
        node = node->next;
    }
}


/* At an exact kind-7 key frame, attaches the indexed world object. Index -1
 * restores the saved vectors; losing the active key restores them once too. */
void func_00249C40(s32 position, EventViewerState *viewer) {
    EvtViewTrack *node = viewer->tracks;

    while (node != NULL) {
        if (node->kind == 1) {
            EvtViewKey *glyph = node->keys;
            s32 bestFrame = -1;
            EvtViewKey *best = NULL;

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
                    effObjSetInnerFirstVec(node->owner.transform,
                                           (u128 *)node->savedFirstVector);
                    effObjSetInnerSecondVec(node->owner.transform,
                                            (u128 *)node->savedSecondVector);
                    effObjFetchInnerFirstVec(node->owner.transform);
                    VU0_STORE_VF(vf10, &node->owner.transform->inner->vec70);
                    node->objectAttached = 0;
                }
            } else if (best->frame == position) {
                s16 channel = best->p0C.sh[0];

                if (channel == -1) {
                    effObjSetInnerFirstVec(node->owner.transform,
                                           (u128 *)node->savedFirstVector);
                    effObjSetInnerSecondVec(node->owner.transform,
                                            (u128 *)node->savedSecondVector);
                    effObjFetchInnerFirstVec(node->owner.transform);
                    VU0_STORE_VF(vf10, &node->owner.transform->inner->vec70);
                    node->objectAttached = 0;
                } else {
                    u32 *object = dds3FindObjectChainNodeByName(
                        dds3GetWorldObject(), viewer->unitNames[channel]);

                    mdlAttachWorldObjectToSourceVector(
                        node->owner.transform->word4, object[1]);
                    node->objectAttached = 1;
                }
            }
        }
        node = node->next;
    }
}

typedef struct PackedPair PackedPair;
extern void mnuUnpackNibbleFields(PackedPair *, s32 *, s32 *);
extern u32 itfMesGetWindowEntryItems(s32, s32);
void evtViewerMarkWindowActive(EventViewerState *);

/* Activates the message window thirty frames before a kind-4 key when its
 * unpacked entry is ready. Only the first kind-4 track is considered. */
void evtViewerActivateWindowForGlyphEntry(s32 position, EventViewerState *viewer) {
    EvtViewTrack *group;
    EvtViewKey *glyph;
    s32 entry;
    s32 mode;

    if (viewer->windowContext == 0) {
        return;
    }
    if (((EvtWindowContext *)viewer->windowContext)->windowHandle == -1) {
        return;
    }
    for (group = viewer->tracks; group != NULL; group = group->next) {
        if (group->kind == 4) {
            for (glyph = group->keys; glyph != NULL; glyph = glyph->next) {
                if (glyph->frame - 30 == position) {
                    mnuUnpackNibbleFields((PackedPair *)glyph, &entry, &mode);
                    if (itfMesGetWindowEntryItems(
                            ((EvtWindowContext *)viewer->windowContext)->windowHandle, entry) == 1) {
                        evtViewerMarkWindowActive(viewer);
                        break;
                    }
                }
            }
            break;
        }
    }
}

void evtViewerCountFlaggedUpdates(EventViewerState *viewer) {
    s64 active;

    active = evtViewerHasUpdateFlag((s32)viewer);
    if (active != 0) {
        viewer->updateCount = viewer->updateCount + 1;
    }
}

extern s32 func_0025E7B0(void *scene);
extern s32 scrCommandIsProcessControlFlagClear(void);
extern u32 mnuCampGetSecondaryOption(void *scene);
extern void mnuReleaseCampSceneRegisteredIds();
extern s32 mnuQueryTitleSoundBusy(void);
extern void mnuStopTitleVoicePlayback(void);
extern void sdfSoundSetChannelCount(u32 channels);
extern s32 fldTitleIsActive(void);
extern void func_0014E668(s32 active);
void evtViewerCleanupMessageWindow(s32 viewerAddr);

void func_00249EE8(EventViewerState *viewer) {
    if (func_0025E7B0(viewer) == 1 || scrCommandIsProcessControlFlagClear() == 0) {
        if (viewer->glyphTickCount == 0) {
            viewer->glyphTickCount = 1;
        }
        return;
    }
    if (viewer->glyphAdvancePosition < viewer->glyphAdvanceLimit - 30) {
        if (viewer->glyphAdvanceStart + 20 >= viewer->glyphAdvancePosition) {
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
    evtViewerCleanupMessageWindow((s32)viewer);
}

s32 evtViewerHasUpdateFlag(s32 viewer) {
    return (*(s32 *)(viewer + 4) & 0x10) > 0;
}


/* Fades the viewer's background colour over its update countdown. */
void func_0024A020(EventViewerState *viewer) {
    s32 fade = 0;

    kwlnGetDrawBufferIndex();
    if (viewer->updateCount != 0x19) {
        fade = (s32)(128.0f - ((f32)viewer->unk24A0 +
                               ((128.0f - (f32)viewer->unk24A0) / 25.0f) * (f32)viewer->updateCount));
    }
    if (fade < 0) {
        fade = 0;
    }
    if (fade < 0x80 && evtViewerHasUpdateFlag((s32)viewer) != 0) {
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
void func_0024A158(s32 frame, EventViewerState *viewer) {
    EvtViewTrack *track = viewer->tracks;

    while (track != NULL) {
        if ((u32)(track->kind - 0xE) < 2 || track->kind == 0x17 || track->kind == 0x11) {
            EvtViewKey *key = track->keys;
            s32 value = 0;

            while (key != NULL) {
                if (track->kind != 0x11 || evtViewerTestIndexedCondition(key->p14.sh[0]) != 0) {
                    if (frame < key->frame + track->frameOffset) {
                        break;
                    }
                    if (key->p10.sh[0] != 0) {
                        value = 0;
                        if (key->p10.sh[0] != 1) {
                            value = ((EvtCampEntry *)mnuCampFindEntryByName(
                                         viewer, (char *)viewer->unitNames[key->p10.sh[0] - 2]))->value;
                        }
                    }
                }
                key = key->next;
            }

            switch (track->kind) {
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
s32 evtViewerUpdateTimedAction(EventViewerState *viewer) {
    if (viewer->timedActive == 1) {
        if (viewer->glyphAdvancePosition < viewer->timedStart) {
            mnuStopMovieDrawTask();
            viewer->timedActive = 0;
            viewer->timedStart = 0;
            viewer->timedEnd = 0;
        } else if (viewer->timedEnd < 0) {
            mnuCheckMovieDecoderStatus();
        } else if (viewer->glyphAdvancePosition >= viewer->timedEnd) {
            mnuStopMovieDrawTask();
            viewer->timedActive = 0;
            viewer->timedStart = 0;
            viewer->timedEnd = 0;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00247518", func_0024A400);

void func_0024A5F8(EventViewerState *viewer) {
}

void evtViewerAdvanceGlyphTick(EventViewerState *viewer) {
    s32 nextTick;

    if ((viewer->glyphAdvancePosition < viewer->glyphAdvanceLimit - 3) && (0 < viewer->glyphTickCount))
    {
        frFontDrawGlyphInDefaultMode(viewer->glyph);
        nextTick = viewer->glyphTickCount + 1;
        viewer->glyphTickCount = nextTick;
        if (0x1d < nextTick) {
            viewer->glyphTickCount = 0;
        }
    }
}

void func_0024A668(EventViewerState *viewer) {
}

/* Returns the latest eligible kind-5 key at/before position for channel, or
 * NULL. The condition must pass; equal-frame ties retain the first key visited. */
EvtViewKey *evtViewerFindLatestMatchingGlyph(EvtViewTrack *group, s32 position, s32 channel) {
    s32 bestFrame = -1;
    EvtViewKey *best = NULL;
    EvtViewKey *glyph = group->keys;

    if (glyph != NULL) {
        do {
            if (position >= glyph->frame && bestFrame < glyph->frame && glyph->p08.sb[0] == 5 &&
                glyph->p0C.sb[0] == channel && evtViewerTestIndexedCondition(glyph->p10.sh[0]) == 1) {
                bestFrame = glyph->frame;
                best = glyph;
            }
            glyph = glyph->next;
        } while (glyph != NULL);
    }
    return best;
}

typedef struct EvtViewerPlaybackData {
    ObjBase *object;
    void *counter;
} EvtViewerPlaybackData;
extern void sdfMotionSampleAtFrame(Motion *motion, f32 frame);
extern void sdfMotionSuspend(Motion *motion);
extern void sdfMotionResume(Motion *motion);
extern void sdfFreezeFloatCounter(void *counter);
extern void sdfUnfreezeFloatCounter(void *counter);
extern s32 evtPolygonMovieScaleByProgress(void *movie, s32 mode, s32 start, s32 end);

/* Apply the viewer playback mode to unit, motion and movie-object tracks. */
void func_0024A738(s32 mode, u32 frame, s32 viewerAddr) {
    EventViewerState *viewer = (EventViewerState *)viewerAddr;
    EvtWorldTable *table;
    EvtWorldLink *object;
    EvtViewTrack *track;
    EvtViewTrack *found;
    EvtViewKey *key;
    Motion *motion;
    void *counter;
    void *movie;
    s32 useTrackTime;

    if (dds3GetWorldObject() == NULL) {
        return;
    }
    table = ((EvtWorldObject *)dds3GetWorldObject())->table;
    object = table->slots[5].head;
    while (object != NULL) {
        track = viewer->tracks;
        found = NULL;
        while (track != NULL) {
            if (track->owner.handle == (u32)object) {
                found = track;
                break;
            }
            track = track->next;
        }
        if (found != NULL) {
            func_002496B0(frame, object, track, viewer, 1);
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
    track = viewer->tracks;
    while (track != NULL) {
        switch (track->kind) {
        case 3: case 18: case 20: case 21: case 26:
            key = track->keys;
            useTrackTime = 0;
            while (key != NULL) {
                switch (track->kind) {
                case 3: case 26:
                    useTrackTime = track->playbackTimeMode;
                    /* These track kinds use the same indexed movie lookup. */
                case 20: case 21:
                    if (key->p08.sb[0] < 0) {
                        break;
                    }
                    object = viewer->objects[key->p08.sb[0]];
                    goto updateMovie;
                case 18:
                    object = key->payload;
                    useTrackTime = track->playbackTimeMode;
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
void evtViewerDispatchFlagMode(u32 viewerAddr) {
    s32 viewer;

    viewer = (s32)viewerAddr;
    if ((((EventViewerState *)viewer)->flags & 1) != 0) {
        func_0024A738(0, ((EventViewerState *)viewer)->glyphAdvancePosition, viewerAddr);
        return;
    }
    func_0024A738(1, ((EventViewerState *)viewer)->glyphAdvancePosition, viewerAddr);
}

/* Returns the address word of the nearest kind-2 key strictly after the current
 * frame, or zero. Equal-distance ties retain the first key visited. */
s32 evtViewFindNextGlyph(EventViewerState *viewer) {
    EvtViewKey *result = NULL;
    s32 best = 99999;
    EvtViewTrack *node = viewer->tracks;

    while (node != NULL) {
        if (node->kind == 2) {
            EvtViewKey *glyph = node->keys;

            if (glyph != NULL) {
                do {
                    s32 frame = glyph->frame;

                    if (viewer->glyphAdvancePosition < frame) {
                        s32 distance = frame - viewer->glyphAdvancePosition;

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
s32 evtViewFindPrevGlyph(EventViewerState *viewer) {
    EvtViewKey *result = NULL;
    s32 best = 99999;
    EvtViewTrack *node = viewer->tracks;

    while (node != NULL) {
        if (node->kind == 2) {
            EvtViewKey *glyph = node->keys;

            if (glyph != NULL) {
                do {
                    s32 frame = glyph->frame;

                    if (frame < viewer->glyphAdvancePosition) {
                        s32 distance = viewer->glyphAdvancePosition - frame;

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

void evtViewerPushCommandHistory(s32 mode, s32 first, s32 second, s32 viewerAddr) {
    EventViewerState *viewer = (EventViewerState *)viewerAddr;
    s32 count = viewer->historyCount + 1;

    viewer->currentId = mode;
    viewer->historyCount = count;
    viewer->history[count].id = mode;
    viewer->history[count].a = first;
    viewer->history[count].b = second;
    if (mode > 0) {
        if (mode >= 3) {
            if (mode == 3) {
                viewer->commandResetA = 0;
                viewer->commandResetB = 0;
                viewer->commandResetC = 0;
                viewer->commandResetD = 0;
            }
        }
    }
}

u16 evtViewerPopHistory(EventViewerState *viewer) {
    u16 id;
    s32 index;

    index = viewer->historyCount - 1;
    if (viewer->historyCount == 0) {
        viewer->currentId = 0;
        return 0;
    }
    viewer->historyCount = index;
    id = viewer->history[index].id;
    viewer->currentId = (u32)id;
    return id;
}

INCLUDE_ASM(const s32, "game/code_00247518", func_0024ABD0);

void evtViewerCleanupMessageWindow(s32 viewerAddr) {
    s32 v0;
    s32 v1;

    v0 = ((EventViewerState *)viewerAddr)->windowContext;
    if (v0 == 0) {
        return;
    }
    v1 = ((EvtWindowContext *)v0)->windowHandle;
    if (v1 == -1) {
        return;
    }
    itfMesCleanupWindow(v1, 1);
    v0 = ((EventViewerState *)viewerAddr)->windowContext;
    itfMesFinishWindowAndClearStatus(((EvtWindowContext *)v0)->windowHandle);
    v0 = ((EventViewerState *)viewerAddr)->windowContext;
    itfPanelSetPairFirst(((EvtWindowContext *)v0)->windowHandle, 0);
    v0 = ((EventViewerState *)viewerAddr)->windowContext;
    itfMesResetWindow(((EvtWindowContext *)v0)->windowHandle);
    ((EventViewerState *)viewerAddr)->windowActive = 0;
    ((EventViewerState *)viewerAddr)->pad23C4 = 0;
}

void evtViewerMarkWindowActive(EventViewerState *viewer) {
    viewer->windowActive = 1;
}

void evtViewerMarkWindowInactive(EventViewerState *viewer) {
    viewer->windowActive = 0;
}

INCLUDE_ASM(const s32, "game/code_00247518", func_0024AD48);

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

INCLUDE_RODATA(const s32, "game/code_00247518", D_004227A0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_004227B0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_004227C0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_004227D0);

s32 func_0024B080(s32 arg0, s32 arg1, EventViewerState *viewer) {
    switch ((u32)viewer->selectionMode) {
    case 0:
    case 1:
    case 2:
        viewer->commandResetA = 0;
        viewer->commandResetB = 0;
        evtViewerPushCommandHistory(9, 0xE4, 0x3C, (s32)viewer);
        break;
    case 5:
        viewer->commandValue = viewer->frameCount - 1;
        viewer->commandMinimum = viewer->glyphAdvanceLimit;
        viewer->commandMaximum = 10000;
        evtViewerPushCommandHistory(7, 0xB4, 0x78, (s32)viewer);
        break;
    case 3:
    case 4:
        viewer->optionSelection = 0;
        viewer->optionCount = 2;
        viewer->optionTitle = "FRAME SET.OK? ";
        viewer->optionNames = D_003C91C0;
        evtViewerPushCommandHistory(2, 0xE4, 0x3C, (s32)viewer);
        break;
    case 6:
        viewer->commandStart = 1;
        viewer->flags |= 1;
        evtViewerDispatchFlagMode((u32)viewer);
        viewer->currentId = 0;
        viewer->historyCount = 0;
        viewer->commandResetId = 0;
        D_004372CC = 0;
        return 1;
    case 7:
        viewer->optionSelection = mnuCampGetPrimaryOption(viewer);
        viewer->optionCount = 2;
        viewer->optionTitle = "START FRAME SELECT";
        viewer->optionNames = D_003C91E0;
        evtViewerPushCommandHistory(2, 0xE4, 0x3C, (s32)viewer);
        break;
    case 8:
        viewer->commandValue = 0;
        viewer->commandMinimum = -5000;
        viewer->commandMaximum = 5000;
        evtViewerPushCommandHistory(7, 0xB4, 0x78, (s32)viewer);
        break;
    case 9:
        viewer->optionSelection = mnuCampGetPrimaryOption(viewer);
        viewer->optionCount = 2;
        viewer->optionTitle = "SET BISTAMODE";
        viewer->optionNames = D_003C91C8;
        evtViewerPushCommandHistory(2, 0xE4, 0x3C, (s32)viewer);
        break;
    case 10:
        viewer->optionSelection = mnuCampGetSecondaryOption(viewer);
        viewer->optionCount = 3;
        viewer->optionTitle = "SET SKIPMODE";
        viewer->optionNames = D_003C91D0;
        evtViewerPushCommandHistory(2, 0xE4, 0x3C, (s32)viewer);
        break;
    }
    return 0;
}


INCLUDE_ASM(const s32, "game/code_00247518", func_0024B268);

u32 func_0024B678(u32 unused0, u32 unused1, u32 viewerAddr) {
    evtViewerPushCommandHistory(5, 0x90, 0x48, viewerAddr);
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_00247518", D_004229A0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_004229B0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_004229C0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_004229D0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_004229E0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_004229F0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422A00);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422A10);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422A20);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422A30);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422A40);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422A50);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422A60);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422A70);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422A80);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422A90);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422AA0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422AB0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422AC0);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024B6A8);

/* Store the edited timing or selector halfword in the pending timeline key. */
s32 evtViewerStoreKeyTimingOrSelector(s32 unused0, s32 unused1, EventViewerState *viewer) {
    EvtViewKey *key = (EvtViewKey *)evtEventViewerGetPendingNode((s32)viewer);
    EvtViewTrack *track;
    s32 kind;
    s8 category;

    if (key != NULL) {
        track = viewer->sel;
        kind = track->kind;
        category = D_003C953A[viewer->commandTableOffset + kind * 10];
        switch (category) {
        case 0:
            key->frame = viewer->commandValue - track->frameOffset;
            evtReorderListNodes(track);
            func_00249088(viewer->glyphAdvancePosition, viewer);
            break;
        case 9:
            switch (kind) {
            case 3:
            case 20:
            case 21:
            case 26:
                key->p08.sh[1] = viewer->commandValue;
                break;
            case 18:
                key->p08.sh[0] = viewer->commandValue;
                break;
            }
            break;
        case 15:
            key->duration = viewer->commandValue;
            break;
        }
        evtViewerPopHistory(viewer);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00247518", func_0024C650);

/* Store the command value as a halfword and clear its extra halfword when tagged. */
s32 evtViewCmdSetValue(s32 unused0, s32 unused1, EventViewerState *viewer) {
    s32 value = viewer->commandValue;
    EvtViewKey *entry = (EvtViewKey *)evtEventViewerGetPendingNode((s32)viewer);

    if (entry == NULL) {
        return 0;
    }
    entry->p08.h[0] = value;
    if (((u32)(value << 16) >> 28) != 0) {
        entry->p08.h[1] = 0;
    }
    func_00249088(viewer->glyphAdvancePosition, viewer);
    evtViewerPopHistory(viewer);
    return 0;
}

/* Store the command value in the selected halfword of a viewer entry. */
u32 evtViewerStoreCommandInSelectedField(u32 unused0, u32 unused1, EventViewerState *viewer) {
    EvtViewKey *entry;
    u32 value;
    s32 slot;

    value = viewer->commandValue;
    entry = (EvtViewKey *)evtEventViewerGetPendingNode((s32)viewer);
    if (entry != 0) {
        slot = viewer->sel->kind - 1;
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
        func_00249088(viewer->glyphAdvancePosition, viewer);
        evtViewerPopHistory(viewer);
        return 0;
    }
}

u32 func_0024C9D0(void) {
    return 0;
}

/* Copy the current command word into the selected script entry. */
u32 evtViewerStoreCommandInEntryWord(u32 unused0, u32 unused1, EventViewerState *viewer) {
    EvtViewKey *entry;

    entry = (EvtViewKey *)evtEventViewerGetPendingNode((s32)viewer);
    if (entry != 0) {
        entry->p0C.i = viewer->commandValue;
        func_00249088(viewer->glyphAdvancePosition, viewer);
        evtViewerPopHistory(viewer);
        return 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00247518", func_0024CA28);

u32 kwlnBattleCopyMatrix(u32 unused0, u32 unused1, EventViewerState *viewer) {
    EvtViewKey *key = (EvtViewKey *)evtEventViewerGetPendingNode((s32)viewer);
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
        func_00249088(viewer->glyphAdvancePosition, viewer);
        evtViewerPopHistory(viewer);
        return 0;
    }
    return (u32)key;
}

/* Transfer a selected two-component viewer position to the command entry. */
s32 evtViewCmdSetPosition(s32 unused0, s32 unused1, EventViewerState *viewer) {
    EvtViewKey *entry = (EvtViewKey *)evtEventViewerGetPendingNode((s32)viewer);

    if (entry == NULL) {
        return 0;
    }
    if (viewer->sel == 0) {
        return 0;
    }
    entry->p08.f = viewer->commandX;
    entry->p0C.f = viewer->commandY;
    func_00249088(viewer->glyphAdvancePosition, viewer);
    evtViewerPopHistory(viewer);
    return 0;
}

u32 evtViewCmdCancelSelection(u32 unused0, u32 unused1, u32 viewerAddr) {
    evtViewerPopHistory((EventViewerState *)viewerAddr);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00247518", evtViewCmdResolveSlot);

/* Copy the selected slot descriptor and numeric value into the script entry. */
s32 evtViewCmdSetSlot(s32 unused0, s32 unused1, EventViewerState *viewer) {
    EvtViewKey *entry = (EvtViewKey *)evtEventViewerGetPendingNode((s32)viewer);

    entry->p0C.b[0] = viewer->slotType;
    entry->p0C.b[1] = viewer->slotFlag;
    entry->p14.f = viewer->slotValue;
    func_00249088(viewer->glyphAdvancePosition, viewer);
    evtViewerPopHistory(viewer);
    return 0;
}

/* Apply one of three viewer selection modes to the selected slots. */
s32 evtViewCmdSelectMode(s32 unused0, s32 unused1, EventViewerState *viewer) {
    s32 handled = 0;
    s32 mode = viewer->selectionMode;

    if (mode < 3) {
        if (mode >= 0) {
            func_0035C860(viewer->eventName, D_00423050, D_004372B0, D_004372B2);
            mode = viewer->selectionMode;
            if (mode == 0) {
                func_00259AE8(0, viewer);
                func_00259AE8(1, viewer);
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

INCLUDE_ASM(const s32, "game/code_00247518", func_0024CE18);

u32 evtViewerClearPendingNodeAndPushHistory(u32 unused0, u32 unused1, u32 viewerAddr) {
    if (evtEventViewerGetPendingNode(viewerAddr) != 0) {
        ((EventViewerState *)viewerAddr)->unk22AC = 0;
        ((EventViewerState *)viewerAddr)->unk22B4 = 0;
        evtViewerPushCommandHistory(0xa, 0x9c, 0x54, viewerAddr);
        return 0;
    }
}

INCLUDE_RODATA(const s32, "game/code_00247518", D_00423050);

/* Restore default effect parameters for the selected timeline key. */
s32 func_0024D148(s32 unused0, s32 unused1, EventViewerState *viewer) {
    EvtViewKey *entry = (EvtViewKey *)evtEventViewerGetPendingNode((s32)viewer);
    u128 *destination;
    u128 *source;
    EvtViewerDrawVector *draw;

    switch (viewer->sel->kind) {
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
    func_00249088(viewer->glyphAdvancePosition, viewer);
    evtViewerPopHistory(viewer);
    return 0;
}


INCLUDE_ASM(const s32, "game/code_00247518", evtViewerPickNextHandler);

extern s16 D_004372B4;
extern s8 D_0037F510[];
extern void itfMesStartEntry(s32, s32, s32);
extern void evtPrintDeveloperConsoleMessage(const char *, ...);
extern void func_0024ABD0(EventViewerState *);
extern void mnuAdvanceShopMenuState();
extern u8 func_002A8028(void);
extern s32 evtViewerPickNextHandler(KwlnTask *);

/* Advance the viewer timeline, deferred voice and task-update handoff. */
s32 evtViewerUpdateFrame(KwlnTask *task) {
    EventViewerState *viewer = (EventViewerState *)kwlnTaskGetUserValue(task);
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
            evtViewerDispatchFlagMode((u32)viewer);
            viewer->currentId = 0;
            viewer->historyCount = 0;
            viewer->commandResetId = 0;
            evtViewerPushCommandHistory(1, 0x24, 0x18, (s32)viewer);
            return (s32)evtViewerPickNextHandler;
        }
        func_0024A668(viewer);
    } else {
        if (viewer->glyphAdvancePosition == viewer->glyphAdvanceStart) {
            viewer->flags = flags & ~1;
            evtViewerDispatchFlagMode((u32)viewer);
        }
        if (D_0037F510[0x22] >= 0 && D_0037F510[0x2C] < 0 &&
            evtViewerHasUpdateFlag((s32)viewer) == 0) {
            func_00249EE8(viewer);
        }
    }
    if (viewer->voicePending == 1 && evtViewerHasUpdateFlag((s32)viewer) == 0 &&
        mnuQueryTitleSoundBusy() == 0) {
        itfMesStartEntry(((EvtWindowContext *)viewer->windowContext)->windowHandle,
            viewer->voiceMessage, 0);
        viewer->voicePending = 0;
        evtPrintDeveloperConsoleMessage("[conflict voice play      ] mesno= %d\n",
            viewer->voiceMessage);
    }
    if (viewer->glyphAdvancePosition != viewer->previousGlyphPosition) {
        func_00249088(viewer->glyphAdvancePosition, viewer);
    }
    viewer->previousGlyphPosition = viewer->glyphAdvancePosition;
    if (viewer->flags & 8) {
        evtViewerApplySelectedEntry(viewer);
    }
    func_0024ABD0(viewer);
    func_0024A020(viewer);
    mnuAdvanceShopMenuState(viewer);
    if (viewer->flags & 8) {
        if (viewer->glyphAdvancePosition >= viewer->glyphAdvanceLimit) {
            return -1;
        }
        if (evtViewerHasUpdateFlag((s32)viewer) == 1 && viewer->updateCount >= 25) {
            return -1;
        }
    }
    if (!(viewer->flags & 1)) {
        if (viewer->glyphAdvancePosition >= viewer->glyphAdvanceLimit) {
            if (!(viewer->flags & 8)) {
                viewer->flags ^= 1;
                evtViewerDispatchFlagMode((u32)viewer);
            }
        } else if (viewer->timedActive == 1) {
            if (func_002A8028() == 2) {
                viewer->glyphAdvancePosition++;
            }
        } else {
            viewer->glyphAdvancePosition++;
        }
    }
    return 0;
}

/* Update the active viewer, then switch to its frame-variable task. */
void *evtViewerScheduleFrameVariableTask(s32 task) {
    void *viewer;

    viewer = (void *)kwlnTaskGetUserValue();
    func_00249088(((EventViewerState *)viewer)->glyphAdvancePosition, viewer);
    func_00101968(task, evtCreateFrameVariableTask());
    kwlnDrawControlFlags |= 0x2000000;
    return (void *)evtViewerUpdateFrame;
}

/* Initialize the active viewer and schedule its next update callback. */
void *evtViewerInitializeUpdateSequence(void) {
    struct EvtViewer *viewer;

    viewer = (struct EvtViewer *)kwlnTaskGetUserValue();
    fldInitializeCameraColorResource();
    evtEventViewerReset(viewer);
    kwlnDrawControlFlags |= 0x2000000;
    return (void *)evtViewerScheduleFrameVariableTask;
}

s32 func_0024D760(u8 *ctx) {
    s32 id = ((EvtTaskContext *)ctx)->taskId;

    if (id == 0x28B || id == 0x28E) {
        return 1;
    }
    return 0;
}

/* Advance the viewer update: tick the timed action or hand over to the next task. */
void *evtViewerAdvanceUpdate(void) {
    EventViewerState *viewer = (EventViewerState *)kwlnTaskGetUserValue();
    EvtWindowContext *window;
    s32 windowFlags;

    func_0024DBB8(viewer->windowContext);
    window = (EvtWindowContext *)viewer->windowContext;
    windowFlags = window->flags;
    if ((windowFlags & 8) == 0) {
        kwlnDrawControlFlags |= 0x2000000;
        return 0;
    } else {
        if ((windowFlags & 1) != 0) {
            kwlnDrawControlFlags |= 0x2000000;
            return 0;
        }
        if (func_0024D760((u8 *)window) == 0) {
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

void *evtViewerStartUpdate(void) {
    EventViewerState *viewer = (EventViewerState *)kwlnTaskGetUserValue();
    u8 *context;
    u16 eventId;
    u16 sceneId;

    fldInitializeCameraColorResource();
    context = (u8 *)viewer->windowContext;
    eventId = *(u16 *)(context + 0x10C);
    sceneId = *(u16 *)(context + 0x110);
    D_004372B0 = eventId;
    D_004372B2 = sceneId;
    func_0035C860(viewer->eventName, D_00423050, D_004372B0, D_004372B2);
    viewer->flags = 1;
    evtViewerDispatchFlagMode((u32)viewer);
    viewer->unk2238 = 0;
    viewer->flags |= 8;
    kwlnDrawControlFlags |= 0x2000000;
    return evtViewerAdvanceUpdate;
}

u8 func_0024D908(s32 task) {
    return ((EvtTaskContext *)task)->taskId == 0x263;
}

extern f32 D_0037F590[];
extern void evtResetUnitVectorSlots();
extern void mnuCampLinkFontGlyph();
extern void func_0014E668();
extern void kwlnCancelConfiguredFadeFrames();
extern s32 sdfCheckPendingWorkWithInterrupts();
extern void evtDestroySecondaryWorldNode();
extern void sdfQueueNonzeroResourceId();
extern void kwlnTextureReleaseHeldReference();
extern void evtEventViewerReleaseGroups();
extern void evtEventViewerShutdown();
extern void sdfReleaseResourceAllocation();
extern void fldReleaseCameraColorEffect();
extern void kwlnFadeSetMode();
extern void mnuReleaseCampSceneRegisteredIds();
void evtViewerCleanupMessageWindow(s32 arg0);

void evtViewerRelease(viewer)
    EventViewerState *viewer;
{
    if (func_0024D908(viewer->windowContext) == 0) {
        mnuReleaseCampSceneRegisteredIds(viewer);
    }
    evtResetUnitVectorSlots();
    evtViewerCleanupMessageWindow((s32)viewer);
    mnuCampLinkFontGlyph(viewer);
    func_0014E668(0);
    kwlnCancelConfiguredFadeFrames();
    D_0037F590[0] = D_0037F590[1] = D_0037F590[2] = D_0037F590[3] = 0.0f;
    if (viewer->timedActive == 1) {
        if (viewer->timedEnd != -2 || evtViewerHasUpdateFlag((s32)viewer) == 1) {
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
        sdfQueueNonzeroResourceId(viewer->pendingResource);
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

void func_0024DAA0(void) {
    EventViewerState *viewer;

    viewer = (EventViewerState *)kwlnTaskGetUserValue();
    evtViewerRelease(viewer);
}

void func_0024DAC0(void) {
    EventViewerState *viewer;

    viewer = (EventViewerState *)kwlnTaskGetUserValue();
    evtViewerRelease(viewer);
}

void func_0024DAE0() {
    mnuCampInitFontResource();
}

extern u32 D_00435CBC;
extern s32 sdfAllocGeneralBlock(s32 size);
extern u32 *sdfResourceRetainAddress(s32 handle);
extern void *memset(void *dst, s32 value, u32 size);
extern void *kwlnTaskCreate(const char *name, s32 id, s32 arg2, s32 arg3, void *update, void *destroy, void *data);
void evtViewerCreateTaskWithSky(void) {
    u32 *viewer;
    s32 viewerHandle;
    void *viewerTask;

    D_00435CBC = 0x80000000;
    viewerHandle = sdfAllocGeneralBlock(0x24BC);
    viewer = sdfResourceRetainAddress(viewerHandle);
    memset(viewer, 0, 0x24BC);
    *viewer = viewerHandle;
    viewerTask = kwlnTaskCreate(evtViewerTaskName, 0x3EB, 1, 1, evtViewerInitializeUpdateSequence, func_0024DAA0, viewer);
    func_00101968((s32)viewerTask, evtCreateSkyTask());
    func_0024DAE0(viewer);
}

void evtEventViewerDestroyTask(void) {
    kwlnTaskDestroyWithHierarchyByName(evtViewerTaskName, 1);
}

struct PolyMovieWork;
extern s32 fileIsRequestReadyInCurrentMode(void *file);
extern s32 fileGetResourceHandle(void *file);
extern s32 filePollEntryCleanup(void *file);
extern struct PolyMovieWork *evtPolygonMovieInitWork();

void func_0024DBB8(s32 assetsAddress) {
    EvtWindowContext *assets = (EvtWindowContext *)assetsAddress;

    if (assets->flags & 8) {
        return;
    }
    if (assets->flags & 2) {
        if (assets->first.request == 0) {
            return;
        }
        if (fileIsRequestReadyInCurrentMode(assets->first.request) == 0) {
            return;
        }
        assets->first.resource = fileGetResourceHandle(assets->first.request);
        assets->first.address = sdfResourceRetainAddress(assets->first.resource);
        filePollEntryCleanup(assets->first.request);
        assets->first.request = 0;
        assets->flags &= ~2;
    } else if (assets->flags & 4) {
        if (assets->second.request == 0) {
            return;
        }
        if (fileIsRequestReadyInCurrentMode(assets->second.request) == 0) {
            return;
        }
        assets->second.resource = fileGetResourceHandle(assets->second.request);
        assets->second.address = sdfResourceRetainAddress(assets->second.resource);
        filePollEntryCleanup(assets->second.request);
        assets->second.request = 0;
        assets->flags &= ~4;
    } else if (assets->flags & 0x10) {
        if (assets->third.request == 0) {
            return;
        }
        if (fileIsRequestReadyInCurrentMode(assets->third.request) == 0) {
            return;
        }
        assets->third.resource = fileGetResourceHandle(assets->third.request);
        assets->third.address = sdfResourceRetainAddress(assets->third.resource);
        filePollEntryCleanup(assets->third.request);
        assets->third.request = 0;
        assets->flags &= ~0x10;
    } else if (assets->first.address != NULL && assets->second.address != NULL) {
        evtPolygonMovieInitWork(assets, assets->first.address, assets->second.address, assets->third.address);
        assets->flags |= 8;
    }
}

INCLUDE_RODATA(const s32, "game/code_00247518", evtViewerTaskName);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004372B0);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004372B2);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004372B4);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004372B8);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004372C0);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004372C8);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004372CC);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004372D0);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004372D8);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004372E0);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004372E8);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004372F0);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004372F8);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437300);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437308);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437310);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437318);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437320);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437328);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437330);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437338);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437340);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437348);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437350);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437358);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437360);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437368);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437370);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437378);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437380);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437388);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437390);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437398);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004373A0);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004373A8);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004373B0);

