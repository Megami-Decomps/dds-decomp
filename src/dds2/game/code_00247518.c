#include "common.h"
#include "evt_world.h"
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

extern s32 evtViewerHasUpdateFlag(s32);

extern s32 datGameState;

s32 evtEventViewerGetPendingNode(s32 arg0);

void func_00249088(s32 arg0, void *arg1);

void evtViewerPushCommandHistory(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

extern void *kwlnTaskGetUserValue(void);

void func_0024D430(void);

void func_00101968(s32 arg0, s32 arg1);

s32 evtCreateFrameVariableTask(void);

void *evtViewerScheduleFrameVariableTask(s32 arg0);

extern u32 kwlnDrawControlFlags;

void fldInitializeCameraColorResource(void);

extern void func_0024DBB8(s32 arg0);
extern s32 mnuPollTitleStreamStateLocked(void);
extern void mnuMarkTitleStreamResetPending(void);
extern void func_0025A280(s32 arg0, void *arg1);
extern s32 func_0024D760(u8 *ctx);

void evtEventViewerReset(u64 arg0);

typedef struct EvtViewerGlyph {
    u16 x;
    u8 pad02[0x2E];
    struct EvtViewerGlyph *next;
} EvtViewerGlyph;

typedef struct EvtViewerGroup {
    s32 type;
    u8 pad04[0x50];
    EvtViewerGlyph *glyphs;
    u8 pad58[0x24];
    struct EvtViewerGroup *next;
} EvtViewerGroup;

struct EffNode;

typedef struct EvtViewerObjectData {
    u8 pad00[0xC];
    struct EffNode *parameterNode;
} EvtViewerObjectData;

typedef struct EvtWorldLink {
    u8 pad00[0x18];
    void *data;
    u8 pad1C[4];
    s32 next;
} EvtWorldLink;

typedef struct EventViewerState {
    u32 resourceHandle; /* 0x00 */
    u32 flags;          /* 0x04 */
    s32 windowContext; /* 0x08: owns the message-window handle at +0x104 */
    u8 padC[8];
    s32 glyphAdvanceLimit;
    s32 glyphAdvancePosition;
    u8 pad1C[0x2008];
    s32 selectedEntry; /* 0x2024 */
    u8 pad2028[4];
    s32 fallbackEntry; /* 0x202C */
    u8 pad2030[4];
    EvtViewerGroup *groups;
    u8 pad2038[4];
    EvtWorldLink *objects[128]; /* 0x203C */
    struct {
        u16 id;
        u16 a;
        u16 b;
        u16 pad6;
    } history[8];
    s32 historyCount;
    u32 currentId;
    u8 pad2284[0x24];
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
    u8 pad22E1[0x27];
    struct EvtViewSel *sel; /* 0x2308 */
    u8 pad230C[4];
    u32 commandValue; /* 0x2310: value of the active command */
    u8 pad2314[0x94];
    f32 commandX; /* 0x23A8 */
    f32 commandY; /* 0x23AC */
    u8 pad23B0[0x10];
    s32 updateCount;
    u8 pad23C4;
    u8 windowActive;
    u8 pad23C6[0x2A];
    s32 glyphTickCount;
    u8 pad23F4[0x1C];
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
    u8 pad2444[0x78]; /* allocated as 0x24BC bytes */
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

typedef struct EvtViewSel {
    s32 fieldSelector; /* 0x00: one-based selector for command parameter field */
} EvtViewSel;

/* Script-command parameter slots hold a float, word, halfwords or bytes
 * depending on the command; only the accessed prefix is modeled here. */
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
} EvtViewEntry;

u16 evtViewerPopHistory(EventViewerState *viewer);

extern char evtViewerTaskName[]; /* "EventViewer" */

extern s32 kwlnTaskDestroyWithHierarchyByName(const char *name, s32 arg1);

typedef struct EvtViewGlyph {
    u16 id;       /* 0x00 */
    u16 duration;
    u8 pad04[4];
    s8 kind;      /* 0x08 */
    u8 pad09[3];
    s8 channel;   /* 0x0C */
    u8 pad0D;
    s16 param;    /* 0x0E */
    s16 condition; /* 0x10 */
    u8 pad12[0x1E];
    struct EvtViewGlyph *next; /* 0x30 */
    struct EvtViewGlyph *previous; /* 0x34 */
} EvtViewGlyph;

typedef struct EvtViewNode {
    s32 kind;                 /* 0x00 */
    u8 pad04[0xC];
    u32 owner;                /* 0x10 */
    u8 pad14[8];
    s16 time;                 /* 0x1C */
    u8 pad1E[6];
    s32 unk24;
    s32 keyMode;              /* 0x28 */
    u8 pad2C[0x24];
    s32 hasGlyphs;            /* 0x50 */
    EvtViewGlyph *glyphs;     /* 0x54 */
    EvtViewGlyph *lastGlyph;  /* 0x58 */
    u8 pad5C[0x20];
    struct EvtViewNode *next; /* 0x7C */
} EvtViewNode;

extern void func_0025E460(u16 *from, u16 *to, u8 *out, f32 ratio);
extern void func_0025E980(s32 handle, u8 *out);

void evtViewerApplyInterpolatedNodeKey(EventViewerState *viewer, EvtViewNode *node, u16 *from, u16 *to) {
    u8 out[0x20];
    f32 ratio = 0.0f;

    if (from != NULL) {
        if (to != NULL) {
            s32 start = *from;
            f32 span = *to - start;
            f32 elapsed = viewer->glyphAdvancePosition - (start + node->time);

            if (span != 0.0f) {
                ratio = elapsed / span;
            }
        }
        func_0025E460(from, to, out, ratio);
        *(s32 *)(out + 4) -= 35;
        func_0025E980(node->unk24, out);
    }
}

void func_002475C8(EventViewerState *viewer) {
    EvtViewNode *node = (EvtViewNode *)viewer->groups;
    s32 position = viewer->glyphAdvancePosition;

    while (node != NULL) {
        if (node->kind == 24) {
            if (node->keyMode == 1) {
                u16 *key = (u16 *)evtEventViewerGetPendingNode((s32)viewer);
                evtViewerApplyInterpolatedNodeKey(viewer, node, key, NULL);
            } else {
                EvtViewGlyph *glyph = node->glyphs;
                EvtViewGlyph *from;

                while (glyph != NULL && position >= glyph->id + node->time) {
                    glyph = glyph->next;
                }
                if (glyph != NULL) {
                    from = glyph->previous;
                } else {
                    from = node->lastGlyph;
                }
                evtViewerApplyInterpolatedNodeKey(viewer, node, (u16 *)from, (u16 *)glyph);
            }
        }
        node = node->next;
    }
}

INCLUDE_ASM(const s32, "game/code_00247518", func_002476B8);

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

void func_00247DE0(EvtViewerGroup *group, EvtViewGlyph *key, s32 unused2, s32 unused3, EventViewerState *viewer) {
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
    if (key->kind < 0 || (s8)key->condition == 0) {
        return;
    }
    elapsed = viewer->glyphAdvancePosition - key->id;
    if (key->duration >= elapsed) {
        node = ((EvtViewerObjectData *)viewer->objects[key->kind]->data)->parameterNode;
        alpha = (s32)((128.0f / key->duration) * elapsed);
        func_0035B6E0("alpha=%d\n", alpha);
        color = ((u32)alpha << 24) | 0x808080;
        effSetNodeParameterValue(node, color);
    }
}

INCLUDE_ASM(const s32, "game/code_00247518", func_00247EE0);

INCLUDE_ASM(const s32, "game/code_00247518", func_00248000);

INCLUDE_ASM(const s32, "game/code_00247518", func_00248B80);

INCLUDE_ASM(const s32, "game/code_00247518", func_00248D70);

INCLUDE_ASM(const s32, "game/code_00247518", func_00249088);

s32 evtViewFindGlyphAtOrBefore(EventViewerState *viewer) {
    EvtViewerGlyph *result = NULL;
    s32 best = 99999;
    EvtViewerGroup *node = viewer->groups;

    while (node != NULL) {
        if (node->type == 2) {
            EvtViewerGlyph *glyph = node->glyphs;

            if (glyph != NULL) {
                do {
                    s32 x = glyph->x;

                    if (x <= viewer->glyphAdvancePosition) {
                        s32 distance = viewer->glyphAdvancePosition - x;

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



void evtViewerClampMovieTimes(s32 endTime, EventViewerState *viewer) {
    s32 table;
    s32 slots;
    s32 object;
    EvtViewNode *node;
    s32 time;
    s32 extra;
    EvtViewGlyph *glyph;

    if (dds3GetWorldObject() != 0) {
        table = (s32)((EvtWorldObject *)dds3GetWorldObject())->table;
        if (table != 0) {
            slots = (s32)((EvtWorldTable *)table)->slots;
            if (slots != 0) {
                object = (s32)((EvtWorldSlot *)slots)[EVT_WORLD_SLOT_MOVIE].head;
                if (object != 0) {
                    do {
                        node = (EvtViewNode *)viewer->groups;
                        while (node != NULL) {
                            if (node->owner != 0 && object == dds3GetSlot(node->owner, 1)) {
                                if (node->kind == 2) {
                                    time = 0;
                                    if (node->hasGlyphs != 0) {
                                        time = node->glyphs->id;
                                    }
                                    glyph = (EvtViewGlyph *)evtViewFindGlyphAtOrBefore(viewer);
                                    extra = 0;
                                    if (glyph != NULL) {
                                        extra = glyph->param;
                                    }
                                } else {
                                    time = node->time;
                                    extra = 0;
                                }
                                evtSetMovieClipPositionClampedToDuration(object, 0, time, endTime, extra);
                                break;
                            }
                            node = node->next;
                        }
                        object = ((EvtWorldLink *)object)->next;
                    } while (object != 0);
                }
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00247518", func_002496B0);

void evtViewerSyncWorldGroups(u32 position, EventViewerState *viewer) {
    u8 *list;
    EvtViewNode *node;
    EvtViewNode *found;

    if (dds3GetWorldObject() != 0) {
        list = (u8 *)((EvtWorldObject *)dds3GetWorldObject())->table->slots[EVT_WORLD_SLOT_UNIT].head;
        while (list != 0) {
            found = 0;
            for (node = (EvtViewNode *)viewer->groups; node != 0; node = node->next) {
                if (node->owner == (u32)list) {
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

void evtViewerApplyGlyphLodChannel(s32 position, EventViewerState *viewer) {
    EvtViewNode *node = (EvtViewNode *)viewer->groups;
    EvtViewGlyph *glyph;
    EvtViewGlyph *best;
    s32 bestId;
    u8 *lod;
    s8 level;

    while (node != NULL) {
        if (node->kind == 1) {
            glyph = node->glyphs;
            bestId = -1;
            best = NULL;
            if (glyph != NULL) {
                do {
                    if (position >= glyph->id && bestId < glyph->id && glyph->kind == 6) {
                        bestId = glyph->id;
                        best = glyph;
                    }
                    glyph = glyph->next;
                } while (glyph != NULL);
            }
            lod = *(u8 **)(*(s32 *)(*(s32 *)(*(s32 *)(node->owner + 0x18) + 0xC) + 0xC) + 0x18);
            if (best == NULL) {
                lod[0x98] = 0;
            } else {
                level = best->channel;
                if (sdfGetLodChunkValue(lod) >= level) {
                    lod[0x98] = best->channel;
                }
            }
        }
        node = node->next;
    }
}

INCLUDE_ASM(const s32, "game/code_00247518", func_00249C40);

typedef struct PackedPair PackedPair;
extern void mnuUnpackNibbleFields(PackedPair *, s32 *, s32 *);
extern u32 itfMesGetWindowEntryItems(s32, s32);
void evtViewerMarkWindowActive(EventViewerState *);

void func_00249DC8(s32 position, EventViewerState *viewer) {
    EvtViewerGroup *group;
    EvtViewerGlyph *glyph;
    s32 entry;
    s32 mode;

    if (viewer->windowContext == 0) {
        return;
    }
    if (((EvtWindowContext *)viewer->windowContext)->windowHandle == -1) {
        return;
    }
    for (group = viewer->groups; group != NULL; group = group->next) {
        if (group->type == 4) {
            for (glyph = group->glyphs; glyph != NULL; glyph = glyph->next) {
                if (glyph->x - 30 == position) {
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

INCLUDE_ASM(const s32, "game/code_00247518", func_00249EE8);

s32 evtViewerHasUpdateFlag(s32 viewer) {
    return (*(s32 *)(viewer + 4) & 0x10) > 0;
}


INCLUDE_ASM(const s32, "game/code_00247518", func_0024A020);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024A158);

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

void func_0024A5F8(void) {
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

void func_0024A668(void) {
}

EvtViewGlyph *evtViewerFindLatestMatchingGlyph(EvtViewNode *group, s32 position, s32 channel) {
    s32 bestId = -1;
    EvtViewGlyph *best = NULL;
    EvtViewGlyph *glyph = group->glyphs;

    if (glyph != NULL) {
        do {
            if (position >= glyph->id && bestId < glyph->id && glyph->kind == 5 &&
                glyph->channel == channel && evtViewerTestIndexedCondition(glyph->condition) == 1) {
                bestId = glyph->id;
                best = glyph;
            }
            glyph = glyph->next;
        } while (glyph != NULL);
    }
    return best;
}

INCLUDE_ASM(const s32, "game/code_00247518", func_0024A738);

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

s32 evtViewFindNextGlyph(EventViewerState *viewer) {
    EvtViewerGlyph *result = NULL;
    s32 best = 99999;
    EvtViewerGroup *node = viewer->groups;

    while (node != NULL) {
        if (node->type == 2) {
            EvtViewerGlyph *glyph = node->glyphs;

            if (glyph != NULL) {
                do {
                    s32 x = glyph->x;

                    if (viewer->glyphAdvancePosition < x) {
                        s32 distance = x - viewer->glyphAdvancePosition;

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

s32 evtViewFindPrevGlyph(EventViewerState *viewer) {
    EvtViewerGlyph *result = NULL;
    s32 best = 99999;
    EvtViewerGroup *node = viewer->groups;

    while (node != NULL) {
        if (node->type == 2) {
            EvtViewerGlyph *glyph = node->glyphs;

            if (glyph != NULL) {
                do {
                    s32 x = glyph->x;

                    if (x < viewer->glyphAdvancePosition) {
                        s32 distance = viewer->glyphAdvancePosition - x;

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
    return (*(s32 *)(datGameState + idx * 4 + 0x35c) ^ lo) == 0;
}

u32 func_0024B078(void) {
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_00247518", D_004227A0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_004227B0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_004227C0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_004227D0);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024B080);

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

INCLUDE_ASM(const s32, "game/code_00247518", func_0024C540);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024C650);

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
    func_00249088(viewer->glyphAdvancePosition, viewer);
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
        slot = viewer->sel->fieldSelector - 1;
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
    EvtViewEntry *entry;

    entry = (EvtViewEntry *)evtEventViewerGetPendingNode((s32)viewer);
    if (entry != 0) {
        entry->p0C.i = viewer->commandValue;
        func_00249088(viewer->glyphAdvancePosition, viewer);
        evtViewerPopHistory(viewer);
        return 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00247518", func_0024CA28);

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
        func_00249088(((EventViewerState *)scene)->glyphAdvancePosition, scene);
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
    EvtViewEntry *entry = (EvtViewEntry *)evtEventViewerGetPendingNode((s32)viewer);

    entry->p0C.b[0] = viewer->slotType;
    entry->p0C.b[1] = viewer->slotFlag;
    entry->p14.f = viewer->slotValue;
    func_00249088(viewer->glyphAdvancePosition, viewer);
    evtViewerPopHistory(viewer);
    return 0;
}

/* Apply one of three viewer selection modes to the selected slots. */
s32 evtViewCmdSelectMode(s32 unused0, s32 unused1, EventViewerState *viewer) {
    u8 *ctx = (u8 *)viewer;
    s32 handled = 0;
    s32 mode = viewer->selectionMode;

    if (mode < 3) {
        if (mode >= 0) {
            func_0035C860(ctx + 0x22E8, D_00423050, D_004372B0, D_004372B2);
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

INCLUDE_ASM(const s32, "game/code_00247518", func_0024D148);

INCLUDE_ASM(const s32, "game/code_00247518", evtViewerPickNextHandler);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024D430);

/* Update the active viewer, then switch to its frame-variable task. */
void *evtViewerScheduleFrameVariableTask(s32 task) {
    void *viewer;

    viewer = kwlnTaskGetUserValue();
    func_00249088(((EventViewerState *)viewer)->glyphAdvancePosition, viewer);
    func_00101968(task, evtCreateFrameVariableTask());
    kwlnDrawControlFlags |= 0x2000000;
    return (void *)func_0024D430;
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
    func_0035C860((u8 *)viewer + 0x22E8, D_00423050, D_004372B0, D_004372B2);
    viewer->flags = 1;
    evtViewerDispatchFlagMode((u32)viewer);
    *(s32 *)((u8 *)viewer + 0x2238) = 0;
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
    u64 viewer;

    viewer = kwlnTaskGetUserValue();
    evtViewerRelease(viewer);
}

void func_0024DAC0(void) {
    u64 viewer;

    viewer = kwlnTaskGetUserValue();
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

