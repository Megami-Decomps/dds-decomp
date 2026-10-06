#include "common.h"
#include "sdf.h"
#include "sdf_draw.h"
#include "dds3obj.h"
#include "evt_world.h"
#include "pcp_vu0.h"
#include "evt_unit.h"
#include "eff_transform.h"

extern void *kwlnTaskGetUserValue(void);

extern s32 datGameState;
extern char evtViewerTaskName[]; /* "EventViewer" */
extern s32 kwlnTaskDestroyWithHierarchyByName(const char *name, s32 arg1);
s32 evtViewerHasUpdateFlag(s32 viewerAddr);
void func_00232720(void);
void fldInitializeCameraColorResource(void);
void func_00101A80(s32 arg0, s32 arg1);
s32 evtCreateFrameVariableTask(void);
void evtEventViewerReset(u64 arg0);
void *evtViewerScheduleFrameVariableTask(s32 arg0);
extern void func_00232E20(s32 arg0);
extern s32 mnuPollTitleStreamStateLocked(void);
extern void mnuMarkTitleStreamResetPending(void);
extern void func_0023EF90(s32 arg0, void *arg1);

/* Effect-channel assignments driven by the viewer's timeline tracks. */
typedef struct EvtCampEntry {
    u8 pad00[0x24];
    s32 value; /* 0x24 */
} EvtCampEntry;

extern void effInitCh71Id(void);
extern void effInitCh72Id(void);
extern void effInitCh75Id(void);
extern void effInitCh76Id(void);
extern void effSetCh71Id(u32 resourceWord);
extern void effSetCh72Id(u32 sourceHandle);
extern void effSetCh75Id(u32 resourceWord);
extern void effSetCh76Id(u32 sourceHandle);
extern void *mnuCampFindEntryByName(void *scene, const char *name);

extern u32 kwlnDrawControlFlags;
extern s8 D_0036876A[];
extern u8 D_003BBE88[3];
extern u32 kwlnGetDrawBufferIndex(void);
extern void kwlnFadeSetColor(s32 red, s32 green, s32 blue, s32 alpha);
s32 evtEventViewerGetPendingNode(s32 arg0);
void func_0022E5A0(s32 arg0, void *arg1);
void evtViewerPushCommandHistory(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void *dds3GetWorldObject(void);
void dds3SetWorldCameraObject(s32 arg0, u32 arg1);
f32 dds3GetCameraFieldOfView(s32 arg0);
s32 func_00106488(f32 arg0);
void mnuStopMovieDrawTask(void);
void mnuCheckMovieDecoderStatus(void);

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
    s32 windowHandle; /* 0x104 */
} EvtWindowContext;

typedef struct EventViewerState {
    u32 resourceHandle; /* 0x00 */
    s32 flags;
    s32 windowContext;  /* 0x08: owns the message-window handle at +0x104 */
    u8 padC[4];
    s32 glyphAdvanceStart;    /* 0x10 */
    s32 glyphAdvanceLimit;    /* 0x14 */
    s32 glyphAdvancePosition; /* 0x18 */
    u8 pad1C[8];
    u8 unitNames[256][32];
    s32 selectedEntry;
    u8 pad2028[4];
    s32 fallbackEntry;
    u8 pad2030[4];
    struct EvtViewTrack *tracks; /* 0x2034 */
    u8 pad2038[4];
    struct EvtWorldLink *objects[127];
    s32 unk2238;
    struct {
        u16 id;
        u16 a;
        u16 b;
        u16 pad6;
    } history[8];
    s32 historyCount;
    u32 currentId;
    u8 pad2284[0xC];
    s32 blurRectangleEnabled;
    s32 texturedBlurEnabled;
    s32 filterBlurEnabled;
    s32 colorRectangleEnabled;
    s32 texturedSquareEnabled;
    s32 staggeredBlurEnabled;
    s32 selectionMode; /* 0x22A8: command mode zero, one or two */
    s32 unk22AC;
    u8 pad22B0[4];
    s32 unk22B4;
    u8 pad22B8[0x14];
    s32 commandResetA; /* 0x22CC: cleared on command mode three */
    s32 commandResetB; /* 0x22D0 */
    u8 commandResetC;  /* 0x22D4 */
    u8 pad22D5[0xB];
    u8 commandResetD;  /* 0x22E0 */
    u8 pad22E1[7];
    char eventName[0x14];
    s32 commandTableOffset; /* 0x22FC: byte offset into command descriptors */
    u8 pad2300[8];
    struct EvtViewTrack *sel; /* 0x2308: selected timeline track */
    s32 commandCategory; /* 0x230C: selected parameter category. */
    u32 commandValue; /* 0x2310: value of the active command */
    u8 pad2314[0x94];
    f32 commandX; /* 0x23A8 */
    f32 commandY; /* 0x23AC */
    u8 pad23B0[0x10];
    s32 updateCount;
    u8 pad23C4;
    u8 windowActive;
    s16 unk23C6;
    u8 pad23C8[8];
    s32 ch71; /* 0x23D0 */
    s32 ch72; /* 0x23D4 */
    s32 ch76; /* 0x23D8 */
    s32 ch75; /* 0x23DC */
    u8 pad23E0[0x10];
    s32 glyphTickCount; /* 0x23F0 */
    u8 pad23F4[4];
    s32 framebufferQuadEnabled;
    u8 pad23FC[0x14];
    u32 glyph; /* 0x2410: FrFontGlyph passed to frFontDrawGlyphInDefaultMode */
    s32 timedActive; /* 0x2414: gated time interval */
    s32 timedStart;  /* 0x2418 */
    s32 timedEnd;    /* 0x241C: negative is an open endpoint */
    u8 slotType; /* 0x2420 */
    u8 slotFlag; /* 0x2421 */
    u8 pad2422[2];
    f32 slotValue; /* 0x2424 */
    s32 pendingWork;  /* 0x2428: reset when pendingResource is released */
    s32 pendingResource; /* 0x242C */
    u8 pad2430[0x10];
    s32 titleStreamWaitFrames; /* 0x2440 */
    u8 pad2444[0x34];
    s32 unk2478;
    u8 pad247C[0x14]; /* allocated as 0x2490 bytes */
} EventViewerState;

/* Script-command parameter slots have byte, halfword, word and float views. */
typedef union EvtViewParam {
    f32 f;
    s32 i;
    u16 h[2];
    u8 b[4];
} EvtViewParam;

typedef struct EvtViewEntry {
    u8 pad00[8];
    EvtViewParam p08;
    EvtViewParam p0C;
    EvtViewParam p10;
    EvtViewParam p14;
    u8 pad18[0x14];
    void *payload; /* 0x2C: effect-specific parameter block. */
} EvtViewEntry;


u16 evtViewerPopHistory(EventViewerState *viewer);

/* Timeline key shared by several track kinds, not a rendered font glyph.
 * The selector/channel widths depend on the enclosing track and key kind. */
typedef struct EvtViewKey {
    u16 frame; /* 0x00: track-local frame position. */
    u16 duration;
    u8 pad04[4];
    union {
        s8 kind;
        s16 unitIndex;
    } selector; /* Interpretation depends on the enclosing track kind. */
    s16 enabled;
    union {
        s8 value;
        s16 objectIndex;
    } channel; /* 0x0C: byte value or signed world-object name-table index. */
    u8 pad0E[2];
    s16 condition; /* 0x10 */
    u8 pad12[2];
    s16 unk14; /* 0x14: secondary indexed condition for kind-0x11 tracks. */
    u8 pad16[0x1A];
    struct EvtViewKey *next; /* 0x30 */
    struct EvtViewKey *previous; /* 0x34 */
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
    } owner;                  /* 0x10: payload role is selected by kind */
    u8 pad14[8];
    s16 frameOffset; /* 0x1C: added to relative key frames. */
    s8 playbackTimeMode; /* 0x1E */
    u8 pad1F[5];
    s32 unk24;
    s32 keyMode;              /* 0x28: mode 1 uses the viewer's pending key */
    f32 savedFirstVector[4]; /* 0x2C: restored when detaching a world object. */
    f32 savedSecondVector[4]; /* 0x3C */
    s32 objectAttached; /* 0x4C: world-object attachment needs restoring. */
    s32 hasKeys; /* 0x50 */
    EvtViewKey *keys; /* 0x54 */
    EvtViewKey *lastKey; /* 0x58 */
    u8 pad5C[0x20];
    struct EvtViewTrack *next; /* 0x7C */
} EvtViewTrack;

extern void evtReorderListNodes(EvtViewTrack *track);

extern void func_00243048(u16 *from, u16 *to, u8 *out, f32 ratio);
typedef struct CampDisplayDefaults CampDisplayDefaults;
extern void mnuDrawCampScaledTexture(SdfTex *texture, CampDisplayDefaults *display);

/* Interpolates parameter keys at the viewer's current frame, accounting for
 * the track offset. A missing next key leaves the interpolation ratio at zero. */
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
        func_00243048(from, to, out, ratio);
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
extern u128 *D_00324770[];
extern u128 kwlnDefaultColorVector[];
extern f32 D_003BD358;
extern f32 D_003BD35C;
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
extern void func_00243A18();

void func_0022CD30(EventViewerState *viewer) {
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
    func_00243A18(viewer);
    evtViewerApplyParameterKeyTracks(viewer);
}

void evtViewerApplySelectedEntry(EventViewerState *viewer) {
    s32 entry;
    s32 selected;

    selected = viewer->selectedEntry;
    if (selected != 0) {
        entry = selected;
    } else {
        entry = viewer->fallbackEntry;
    }
    if (entry == 0) {
        return;
    }
    dds3SetWorldCameraObject((s32)dds3GetWorldObject(), entry);
    func_00106488(dds3GetCameraFieldOfView(entry));
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022CED0);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022D420);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022D528);

typedef struct EvtWorldLink {
    u8 pad00[8];
    u8 *name;
    u8 pad0C[0xC];
    void *data;
    u8 pad1C[4];
    struct EvtWorldLink *next;
} EvtWorldLink;

extern EvtWorldLink *dds3FindIndexedObjectChainNodeByName(EvtWorldObject *, s32, const u8 *);
extern s32 evtUnitGetNestedValue(u8 *);
extern void evtSetUnitValueTransition(EvtUnit *, s32, s32);
extern void evtEndUnitValueTransition(EvtUnit *, s32);

/* At time, selects each named unit's latest kind-9 transition key and starts,
 * ends or leaves its value transition according to the selected key flags. */
void func_0022E098(s32 time, EventViewerState *viewer) {
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
                            if (time >= key->frame + node->frameOffset && key->selector.unitIndex >= 0 &&
                                object == dds3FindIndexedObjectChainNodeByName(dds3GetWorldObject(),
                                    EVT_WORLD_SLOT_UNIT, viewer->unitNames[key->selector.unitIndex])) {
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
                    if (selected->enabled != 0) {
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

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022E288);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022E5A0);

extern s32 dds3GetSlot(s32 owner, s32 kind);
extern void evtPolygonMovieClampTime(s32 object, s32 arg1, s32 start, s32 end);

/* Clamps each movie object's playback interval using its linked track's first
 * key frame (or track offset) and the supplied endTime. */
void evtViewerClampMovieTimes(s32 endTime, EventViewerState *viewer) {
    s32 table;
    s32 slots;
    s32 object;
    EvtViewTrack *node;
    s32 time;

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
                                } else {
                                    time = node->frameOffset;
                                }
                                evtPolygonMovieClampTime(object, 0, time, endTime);
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

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022EB10);

extern void func_0022EB10();

/* Updates world units at position using the first track whose owner word
 * matches that unit's object-chain node. */
void evtViewerSyncWorldGroups(s32 position, EventViewerState *viewer) {
    s32 object;
    EvtViewTrack *node;
    EvtViewTrack *found;

    if (dds3GetWorldObject() != 0) {
        object = (s32)((EvtWorldObject *)dds3GetWorldObject())->table->slots[EVT_WORLD_SLOT_UNIT].head;
        if (object != 0) {
            do {
                node = viewer->tracks;
                found = NULL;
                while (node != NULL) {
                    if (node->owner.handle == object) {
                        found = node;
                        break;
                    }
                    node = node->next;
                }
                if (found != NULL) {
                    func_0022EB10(position, object, node, viewer, 0);
                }
                object = (s32)((EvtWorldLink *)object)->next;
            } while (object != 0);
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
                    if (position >= glyph->frame && bestFrame < glyph->frame && glyph->selector.kind == 6) {
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
                level = best->channel.value;
                if (sdfGetLodChunkValue(lod) >= level) {
                    lod[0x98] = best->channel.value;
                }
            }
        }
        node = node->next;
    }
}

extern void effObjSetInnerFirstVec(EffTransformNode *, u128 *);
extern void effObjSetInnerSecondVec(EffTransformNode *, u128 *);
extern void effObjFetchInnerFirstVec(EffTransformNode *);
extern u32 *dds3FindObjectChainNodeByName(EvtWorldObject *, const u8 *);
extern void mdlAttachWorldObjectToSourceVector(s32, s32);

/* At an exact kind-7 key frame, attaches the indexed world object. Index -1
 * restores the saved vectors; losing the active key restores them once too. */
void func_0022F038(s32 position, EventViewerState *viewer) {
    EvtViewTrack *node = viewer->tracks;
    EvtViewKey *glyph;
    EvtViewKey *best;
    s32 bestFrame;

    while (node != NULL) {
        if (node->kind == 1) {
            glyph = node->keys;
            bestFrame = -1;
            best = NULL;
            if (glyph != NULL) {
                do {
                    if (glyph->selector.kind == 7 && position >= glyph->frame &&
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
                s16 channel = best->channel.objectIndex;

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

struct CampPacked;
extern void mnuUnpackNibbleFields(struct CampPacked *, s32 *, s32 *);
extern u32 itfMesGetWindowEntryItems(s32, s32);
void evtViewerMarkWindowActive(EventViewerState *viewer);

/* Activates the message window thirty frames before a kind-4 key when its
 * unpacked entry is ready. id is the current timeline position, not a glyph ID. */
void evtViewerActivateWindowForGlyphEntry(s32 id, EventViewerState *viewer) {
    s32 low;
    s32 high;
    EvtViewTrack *node;
    EvtViewKey *glyph;

    if (viewer->windowContext != 0) {
        if (((EvtWindowContext *)viewer->windowContext)->windowHandle != -1) {
            for (node = viewer->tracks; node != NULL; node = node->next) {
                if (node->kind != 4) {
                    continue;
                }
                for (glyph = node->keys; glyph != NULL; glyph = glyph->next) {
                    if (glyph->frame - 30 != id) {
                        continue;
                    }
                    mnuUnpackNibbleFields((struct CampPacked *)glyph, &low, &high);
                    if (itfMesGetWindowEntryItems(
                            ((EvtWindowContext *)viewer->windowContext)->windowHandle, low) != 1) {
                        continue;
                    }
                    evtViewerMarkWindowActive(viewer);
                    break;
                }
                break;
            }
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

extern s32 campAnyPackedFlagSet(void *scene);
extern s32 scrCommandIsProcessControlFlagClear(void);
extern u32 mnuCampGetSecondaryOption(void *scene);
extern void mnuReleaseCampSceneRegisteredIds();
extern s32 mnuQueryTitleSoundBusy(void);
extern void mnuStopTitleVoicePlayback(void);
extern void sdfSoundSetChannelCount(u32 channels);
extern s32 fldTitleIsActive(void);
extern void func_0014A298(s32 active);
void evtViewerCleanupMessageWindow(s32 viewerAddr);

void func_0022F2E0(EventViewerState *viewer) {
    if (campAnyPackedFlagSet(viewer) == 1 || scrCommandIsProcessControlFlagClear() == 0) {
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
        func_0014A298(0);
    }
    evtViewerCleanupMessageWindow((s32)viewer);
}

/* Test the viewer's update flag. */
s32 evtViewerHasUpdateFlag(s32 viewerAddr) {
    return (((EventViewerState *)viewerAddr)->flags & 0x10) > 0;
}

/* Fades the viewer's background colour over its update countdown. */
void func_0022F418(EventViewerState *viewer) {
    s32 fade = 0;

    kwlnGetDrawBufferIndex();
    if (viewer->updateCount != 0x19) {
        fade = (s32)(128.0f - ((f32)viewer->unk2478 +
                               ((128.0f - (f32)viewer->unk2478) / 25.0f) * (f32)viewer->updateCount));
    }
    if (fade < 0) {
        fade = 0;
    }
    if (fade < 0x80 && evtViewerHasUpdateFlag((s32)viewer) != 0) {
        s32 option = (s32)mnuCampGetSecondaryOption(viewer);

        switch (option) {
        case 0:
            D_003BBE88[0] = D_003BBE88[1] = D_003BBE88[2] = 0;
            break;
        case 1:
            D_003BBE88[0] = D_003BBE88[1] = D_003BBE88[2] = 0xFF;
            break;
        case 2:
            return;
        }
        kwlnFadeSetColor(D_003BBE88[0], D_003BBE88[1], D_003BBE88[2], 0x80 - fade);
    }
}

/* Applies the effect-channel assignments driven by the viewer's tracks. */
void func_0022F550(s32 frame, EventViewerState *viewer) {
    EvtViewTrack *track = viewer->tracks;

    while (track != NULL) {
        if ((u32)(track->kind - 0xE) < 2 || track->kind == 0x17 || track->kind == 0x11) {
            EvtViewKey *key = track->keys;
            s32 value = 0;

            while (key != NULL) {
                if (track->kind != 0x11 || evtViewerTestIndexedCondition(key->unk14) != 0) {
                    if (frame < key->frame + track->frameOffset) {
                        break;
                    }
                    if (key->condition != 0) {
                        value = 0;
                        if (key->condition != 1) {
                            value = ((EvtCampEntry *)mnuCampFindEntryByName(
                                         viewer, (char *)viewer->unitNames[key->condition - 2]))->value;
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

/* Advance or stop the timed viewer action according to the current position. */
s32 evtViewerUpdateTimedAction(EventViewerState *viewer) {
    if (viewer->timedActive == 1) {
        if (viewer->glyphAdvancePosition < viewer->timedStart) {
            mnuStopMovieDrawTask();
            viewer->timedActive = 0;
            viewer->timedStart = 0;
            viewer->timedEnd = 0;
        } else if (viewer->timedEnd == -1) {
            mnuCheckMovieDecoderStatus();
        } else if (viewer->glyphAdvancePosition >= viewer->timedEnd) {
            mnuStopMovieDrawTask();
            viewer->timedActive = 0;
            viewer->timedStart = 0;
            viewer->timedEnd = 0;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022F7F8);

void func_0022F9F0(void) {
}

/* Tick the current glyph while text is advancing; wrap after thirty ticks. */
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

void func_0022FA60(void) {
}

s32 evtViewerTestIndexedCondition(u32 condition);

/* Returns the latest eligible kind-5 key at/before position for channel, or
 * NULL. The condition must pass; equal-frame ties retain the first key visited. */
EvtViewKey *evtViewerFindLatestMatchingGlyph(EvtViewTrack *group, s32 position, s32 channel) {
    s32 bestFrame = -1;
    EvtViewKey *best = NULL;
    EvtViewKey *glyph = group->keys;

    if (glyph != NULL) {
        do {
            if (position >= glyph->frame && bestFrame < glyph->frame && glyph->selector.kind == 5 &&
                glyph->channel.value == channel && evtViewerTestIndexedCondition(glyph->condition) == 1) {
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
extern ObjBase *dds3GetObjectOwnedHandle(void *object);
extern s32 evtPolygonMovieScaleByProgress(void *movie, s32 mode, s32 start, s32 end);

/* Apply the viewer playback mode to unit, motion and movie-object tracks. */
void func_0022FB30(s32 mode, u32 frame, s32 viewerAddr) {
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
            func_0022EB10(frame, object, track, viewer, 1);
        }
        object = object->next;
    }
    object = table->slots[6].head;
    while (object != NULL) {
        motion = ((EvtViewerPlaybackData *)object->data)->object->unk38;
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
                    if (key->selector.kind < 0) {
                        break;
                    }
                    object = viewer->objects[key->selector.kind];
                    goto updateMovie;
                case 18:
                    object = ((EvtViewEntry *)key)->payload;
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
        func_0022FB30(0, ((EventViewerState *)viewer)->glyphAdvancePosition, viewerAddr);
        return;
    }
    func_0022FB30(1, ((EventViewerState *)viewer)->glyphAdvancePosition, viewerAddr);
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

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022FFC8);

void evtViewerCleanupMessageWindow(s32 viewerAddr) {
    s32 windowContext;
    s32 window;

    windowContext = ((EventViewerState *)viewerAddr)->windowContext;
    if (windowContext == 0) {
        return;
    }
    window = ((EvtWindowContext *)windowContext)->windowHandle;
    if (window == -1) {
        return;
    }
    itfMesCleanupWindow(window, 1);
    windowContext = ((EventViewerState *)viewerAddr)->windowContext;
    itfMesFinishWindowAndClearStatus(((EvtWindowContext *)windowContext)->windowHandle);
    windowContext = ((EventViewerState *)viewerAddr)->windowContext;
    itfPanelSetPairFirst(((EvtWindowContext *)windowContext)->windowHandle, 0);
    windowContext = ((EventViewerState *)viewerAddr)->windowContext;
    itfMesResetWindow(((EvtWindowContext *)windowContext)->windowHandle);
    ((EventViewerState *)viewerAddr)->windowActive = 0;
    ((EventViewerState *)viewerAddr)->pad23C4 = 0;
}

void evtViewerMarkWindowActive(EventViewerState *viewer) {
    viewer->windowActive = 1;
}

void evtViewerMarkWindowInactive(EventViewerState *viewer) {
    viewer->windowActive = 0;
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00230140);

s32 evtViewerTestIndexedCondition(u32 condition) {
    u32 index;
    u32 lowBits;

    index = (condition << 16) >> 28;
    lowBits = condition & 0xfff;
    if (index == 0) {
        return 1;
    }
    return (*(s32 *)(datGameState + index * 4 + 0x35c) ^ lowBits) == 0;
}

u32 func_00230470(void) {
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD2B0);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD2C0);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD2D0);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD2E0);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00230478);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00230660);

u32 func_00230A38(u32 unused0, u32 unused1, u32 viewerAddr) {
    evtViewerPushCommandHistory(5, 0x90, 0x48, viewerAddr);
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD4B0);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD4C0);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD4D0);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD4E0);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD4F0);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD500);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD510);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD520);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD530);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD540);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD550);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD560);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003AD570);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00230A68);

/* Store the edited timing or selector halfword in the pending timeline key. */
s32 evtViewerStoreKeyTimingOrSelector(s32 unused0, s32 unused1, EventViewerState *viewer) {
    EvtViewKey *key = (EvtViewKey *)evtEventViewerGetPendingNode((s32)viewer);
    EvtViewTrack *track;
    s32 kind;
    s8 category;

    if (key != NULL) {
        track = viewer->sel;
        kind = track->kind;
        category = D_0036876A[viewer->commandTableOffset + kind * 10];
        switch (category) {
        case 0:
            key->frame = viewer->commandValue - track->frameOffset;
            evtReorderListNodes(track);
            func_0022E5A0(viewer->glyphAdvancePosition, viewer);
            break;
        case 9:
            switch (kind) {
            case 3:
            case 20:
            case 21:
            case 26:
                key->enabled = viewer->commandValue;
                break;
            case 18:
                key->selector.unitIndex = viewer->commandValue;
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

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00231950);

/* Store the command value as a halfword and clear its extra halfword when tagged. */
s32 evtViewCmdSetValue(s32 unused0, s32 unused1, EventViewerState *viewer) {
    s32 value = viewer->commandValue;
    EvtViewEntry *entry = (EvtViewEntry *)evtEventViewerGetPendingNode((s32)viewer);

    if (entry == NULL) {
        return 0;
    }
    entry->p08.h[0] = value;
    if (((u32)(value << 16) >> 28) != 0) {
        entry->p08.h[1] = 0;
    }
    func_0022E5A0(viewer->glyphAdvancePosition, viewer);
    evtViewerPopHistory(viewer);
    return 0;
}

/* Store the command value in the selected halfword of a viewer entry. */
u32 evtViewerStoreCommandInSelectedField(u32 unused0, u32 unused1, EventViewerState *viewer) {
    EvtViewEntry *entry;
    u32 value;
    s32 slot;

    value = viewer->commandValue;
    entry = (EvtViewEntry *)evtEventViewerGetPendingNode((s32)viewer);
    if (entry != 0) {
        slot = viewer->sel->kind - 1;
        if ((u32)slot < 0x11u) {
            switch (slot) {
            case 3:
                entry->p08.h[1] = value;
                break;
            case 1:
                entry->p0C.h[0] = value;
                break;
            case 0:
                entry->p10.h[0] = value;
                break;
            case 11:
            case 15:
            case 16:
                entry->p14.h[0] = value;
                break;
            }
        }
        func_0022E5A0(viewer->glyphAdvancePosition, viewer);
        evtViewerPopHistory(viewer);
        return 0;
    }
}

u32 func_00231CC0(void) {
    return 0;
}

/* Copy the current command word into the selected script entry. */
u32 evtViewerStoreCommandInEntryWord(u32 unused0, u32 unused1, EventViewerState *viewer) {
    EvtViewEntry *entry;

    entry = (EvtViewEntry *)evtEventViewerGetPendingNode((s32)viewer);
    if (entry != 0) {
        entry->p0C.i = viewer->commandValue;
        func_0022E5A0(viewer->glyphAdvancePosition, viewer);
        evtViewerPopHistory(viewer);
        return 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00231D18);

u32 kwlnBattleCopyMatrix(u32 unused0, u32 unused1, u8 *scene) {
    u8 *record = (u8 *)evtEventViewerGetPendingNode((s32)scene);
    if (record != NULL) {
        f32 *dst = *(f32 **)(record + 0x2C);
        f32 *src = (f32 *)(scene + 0x2350);
        s32 index = 3;
        do {
            index--;
            dst[0] = src[-8];
            dst[4] = src[-4];
            dst[8] = src[0];
            src++;
            dst++;
        } while (index >= 0);
        func_0022E5A0(*(s32 *)(scene + 0x18), scene);
        evtViewerPopHistory((EventViewerState *)scene);
        return 0;
    }
    return (u32)record;
}

/* Transfer a selected two-component viewer position to the command entry. */
s32 evtViewCmdSetPosition(s32 unused0, s32 unused1, EventViewerState *viewer) {
    EvtViewEntry *entry = (EvtViewEntry *)evtEventViewerGetPendingNode((s32)viewer);

    if (entry == NULL) {
        return 0;
    }
    if (viewer->sel == 0) {
        return 0;
    }
    entry->p08.f = viewer->commandX;
    entry->p0C.f = viewer->commandY;
    func_0022E5A0(viewer->glyphAdvancePosition, viewer);
    evtViewerPopHistory(viewer);
    return 0;
}

u32 evtViewCmdCancelSelection(u32 unused0, u32 unused1, u32 viewerAddr) {
    evtViewerPopHistory((EventViewerState *)viewerAddr);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", evtViewCmdResolveSlot);

/* Copy the selected slot descriptor and numeric value into the script entry. */
s32 evtViewCmdSetSlot(s32 unused0, s32 unused1, EventViewerState *viewer) {
    EvtViewEntry *entry = (EvtViewEntry *)evtEventViewerGetPendingNode((s32)viewer);

    entry->p0C.b[0] = viewer->slotType;
    entry->p0C.b[1] = viewer->slotFlag;
    entry->p14.f = viewer->slotValue;
    func_0022E5A0(viewer->glyphAdvancePosition, viewer);
    evtViewerPopHistory(viewer);
    return 0;
}

extern char D_003ADA98[]; /* "E%3d_%03d" */
extern u16 D_003BBE78;
extern u16 D_003BBE7A;
extern s32 func_003014F0(char *buffer, const char *format, ...);
extern void func_0023E7F8(s32 slot, EventViewerState *viewer);
extern void evtReloadEventViewer(s32 slot, EventViewerState *viewer);

/* Apply one of three viewer selection modes to the selected slots. */
s32 evtViewCmdSelectMode(u32 unused0, u32 unused1, EventViewerState *viewer) {
    s32 applied = 0;
    s32 mode = viewer->selectionMode;

    if (mode < 3) {
        if (mode >= 0) {
            func_003014F0(viewer->eventName, D_003ADA98, D_003BBE78, D_003BBE7A);
            if (viewer->selectionMode == 0) {
                func_0023E7F8(0, viewer);
                func_0023E7F8(1, viewer);
            } else if (viewer->selectionMode == 1) {
                evtReloadEventViewer(0, viewer);
            } else if (viewer->selectionMode == 2) {
                evtReloadEventViewer(1, viewer);
            }
            applied = 1;
        }
    }
    return applied ? -1 : 0;
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00232108);

u32 evtViewerClearPendingNodeAndPushHistory(u32 unused0, u32 unused1, u32 viewerAddr) {
    if (evtEventViewerGetPendingNode(viewerAddr) != 0) {
        ((EventViewerState *)viewerAddr)->unk22AC = 0;
        ((EventViewerState *)viewerAddr)->unk22B4 = 0;
        evtViewerPushCommandHistory(0xa, 0x9c, 0x54, viewerAddr);
        return 0;
    }
}

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003ADA98);

/* Restore default effect parameters for the selected timeline key. */
s32 func_00232438(s32 unused0, s32 unused1, EventViewerState *viewer) {
    EvtViewEntry *entry = (EvtViewEntry *)evtEventViewerGetPendingNode((s32)viewer);
    u128 *destination;
    u128 *source;
    EvtViewerDrawVector *draw;

    switch (viewer->sel->kind) {
    case 10:
        switch (viewer->commandCategory) {
        case 13:
            destination = entry->payload;
            source = D_00324770[0];
            PCP_COPY_VECTOR(destination, source);
            PCP_COPY_VECTOR(destination + 1, source + 1);
            PCP_COPY_VECTOR(destination + 2, kwlnDefaultColorVector);
            break;
        case 14:
            entry->p08.f = D_003BD358;
            entry->p0C.f = D_003BD35C;
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
    func_0022E5A0(viewer->glyphAdvancePosition, viewer);
    evtViewerPopHistory(viewer);
    return 0;
}


INCLUDE_ASM(const s32, "game/code_0022CBA0", evtViewerPickNextHandler);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00232720);

/* Update the active viewer, then switch to its frame-variable task. */
void *evtViewerScheduleFrameVariableTask(s32 task) {
    void *viewer;

    viewer = kwlnTaskGetUserValue();
    func_0022E5A0(((EventViewerState *)viewer)->glyphAdvancePosition, viewer);
    func_00101A80(task, evtCreateFrameVariableTask());
    kwlnDrawControlFlags |= 0x2000000;
    return (void *)func_00232720;
}

/* Initialize the active viewer and schedule its next update callback. */
void *evtViewerInitializeUpdateSequence(void) {
    u64 viewer;

    viewer = kwlnTaskGetUserValue();
    fldInitializeCameraColorResource();
    evtEventViewerReset(viewer);
    kwlnDrawControlFlags |= 0x2000000;
    return (void *)evtViewerScheduleFrameVariableTask;
}

/* Advance the viewer update: tick the timed action or hand over to the next task. */
void *evtViewerAdvanceUpdate(void) {
    EventViewerState *viewer = (EventViewerState *)kwlnTaskGetUserValue();
    EvtWindowContext *window;
    s32 windowFlags;

    func_00232E20(viewer->windowContext);
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
        if ((u32)(mnuPollTitleStreamStateLocked() - 3) < 2) {
            if (viewer->titleStreamWaitFrames == 0x78) {
                mnuMarkTitleStreamResetPending();
            }
            viewer->titleStreamWaitFrames++;
            kwlnDrawControlFlags |= 0x2000000;
            return 0;
        }
        func_0023EF90(viewer->windowContext, viewer);
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
    D_003BBE78 = eventId;
    D_003BBE7A = sceneId;
    func_003014F0(viewer->eventName, D_003ADA98, D_003BBE78, D_003BBE7A);
    viewer->flags = 1;
    evtViewerDispatchFlagMode((u32)viewer);
    viewer->unk2238 = 0;
    viewer->flags |= 8;
    kwlnDrawControlFlags |= 0x2000000;
    return evtViewerAdvanceUpdate;
}

extern f32 D_00324590[];
extern void mnuReleaseCampSceneRegisteredIds();
extern void evtResetUnitVectorSlots();
extern void mnuCampLinkFontGlyph();
extern void func_0014A298();
extern void kwlnCancelConfiguredFadeFrames();
extern void mnuStopMovieDrawTask();
extern s32 sdfCheckPendingWorkWithInterrupts();
extern void evtDestroySecondaryWorldNode();
extern void sdfQueueNonzeroResourceId();
extern void kwlnTextureReleaseHeldReference();
extern void evtEventViewerReleaseGroups();
extern void evtEventViewerShutdown();
extern void sdfReleaseResourceAllocation();
extern void fldReleaseCameraColorEffect();
extern void kwlnFadeSetMode();
void evtViewerCleanupMessageWindow(s32 viewerAddr);

void evtViewerReleaseResources(viewer)
    EventViewerState *viewer;
{
    mnuReleaseCampSceneRegisteredIds();
    evtResetUnitVectorSlots();
    evtViewerCleanupMessageWindow((s32)viewer);
    mnuCampLinkFontGlyph(viewer);
    func_0014A298(0);
    kwlnCancelConfiguredFadeFrames();
    D_00324590[0] = D_00324590[1] = D_00324590[2] = D_00324590[3] = 0.0f;
    if (viewer->timedActive == 1) {
        mnuStopMovieDrawTask();
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

/* Destroy the currently active event viewer. */
void func_00232D08(void) {
    u64 viewer;

    viewer = kwlnTaskGetUserValue();
    evtViewerReleaseResources(viewer);
}

/* Alternate destroy callback for the same active viewer. */
void func_00232D28(void) {
    u64 viewer;

    viewer = kwlnTaskGetUserValue();
    evtViewerReleaseResources(viewer);
}

void func_00232D48(void *unused) {
    mnuCampInitFontResource();
}

extern u32 D_003BA8EC;
extern s32 sdfAllocGeneralBlock(s32 size);
extern u32 *sdfResourceRetainAddress(s32 handle);
extern void *memset(void *dst, s32 value, u32 size);
extern void *kwlnTaskCreate(const char *name, s32 id, s32 arg2, s32 arg3, void *update, void *destroy, void *data);
extern s32 evtCreateSkyTask(void);

void evtViewerCreateTaskWithSky(void) {
    s32 viewerHandle;
    u32 *viewer;
    void *viewerTask;

    D_003BA8EC = 0x80000000;
    viewerHandle = sdfAllocGeneralBlock(0x2490);
    viewer = sdfResourceRetainAddress(viewerHandle);
    memset(viewer, 0, 0x2490);
    *viewer = viewerHandle;
    viewerTask = kwlnTaskCreate(evtViewerTaskName, 0x3EB, 1, 1, evtViewerInitializeUpdateSequence, func_00232D08, viewer);
    func_00101A80((s32)viewerTask, evtCreateSkyTask());
    func_00232D48(viewer);
}

void evtEventViewerDestroyTask(void) {
    kwlnTaskDestroyWithHierarchyByName(evtViewerTaskName, 1);
}

struct PolyMovieWork;
extern s32 fileIsRequestReadyInCurrentMode(void *file);
extern s32 fileGetResourceHandle(void *file);
extern s32 filePollEntryCleanup(void *file);
extern struct PolyMovieWork *evtPolygonMovieInitWork();

void func_00232E20(s32 assetsAddress) {
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

INCLUDE_RODATA(const s32, "game/code_0022CBA0", evtViewerTaskName);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBE78);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBE7A);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBE7C);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBE80);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBE88);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBE90);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBE94);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBE98);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBEA0);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBEA8);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBEB0);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBEB8);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBEC0);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBEC8);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBED0);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBED8);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBEE0);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBEE8);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBEF0);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBEF8);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBF00);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBF08);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBF10);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBF18);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBF20);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBF28);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBF30);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBF38);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBF40);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBF48);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBF50);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBF58);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBF60);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBF68);

INCLUDE_SDATA(const s32, "game/code_0022CBA0", D_003BBF70);

