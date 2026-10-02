#include "common.h"
#include "evt_world.h"

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

extern u32 kwlnDrawControlFlags;
s32 evtEventViewerGetPendingNode(s32 arg0);
void func_0022E5A0(s32 arg0, void *arg1);
void evtViewerPushCommandHistory(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 dds3GetWorldObject(void);
void dds3SetWorldCameraObject(s32 arg0, u32 arg1);
f32 dds3GetCameraFieldOfView(s32 arg0);
s32 func_00106488(f32 arg0);
void mnuStopMovieDrawTask(void);
void mnuCheckMovieDecoderStatus(void);

/* Handles retained by the viewer and by its owning task context. */
typedef struct EvtWindowContext {
    s32 flags; /* 0x00 */
    u8 pad04[0x100];
    s32 windowHandle; /* 0x104 */
} EvtWindowContext;

typedef struct EventViewerState {
    u32 resourceHandle; /* 0x00 */
    u32 flags;
    s32 windowContext;  /* 0x08: owns the message-window handle at +0x104 */
    u8 padC[8];
    s32 glyphAdvanceLimit;    /* 0x14 */
    s32 glyphAdvancePosition; /* 0x18 */
    u8 pad1C[0x2008];
    s32 selectedEntry;
    u8 pad2028[4];
    s32 fallbackEntry;
    u8 pad2030[4];
    struct EvtViewNode *nodes; /* 0x2034 */
    u8 pad2038[0x204];
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
    s16 unk23C6;
    u8 pad23C8[0x28];
    s32 glyphTickCount; /* 0x23F0 */
    u8 pad23F4[0x1C];
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
    u8 pad2444[0x4C]; /* allocated as 0x2490 bytes */
} EventViewerState;

typedef struct EvtViewSel {
    s32 fieldSelector; /* 0x00: one-based selector for command parameter field */
    u8 pad04[0x1A];
    s8 mode;  /* 0x1E */
    s8 index; /* 0x1F */
} EvtViewSel;

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
} EvtViewEntry;


u16 evtViewerPopHistory(EventViewerState *viewer);

typedef struct EvtViewGlyph {
    u16 id;       /* 0x00 */
    u8 pad02[6];
    s8 kind;      /* 0x08 */
    u8 pad09[3];
    s8 channel;   /* 0x0C */
    u8 pad0D[3];
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
    s32 keyMode;              /* 0x28: mode 1 uses the viewer's pending key */
    u8 pad2C[0x24];
    s32 hasGlyphs;            /* 0x50 */
    EvtViewGlyph *glyphs;     /* 0x54 */
    EvtViewGlyph *lastGlyph;  /* 0x58 */
    u8 pad5C[0x20];
    struct EvtViewNode *next; /* 0x7C */
} EvtViewNode;


extern void func_00243048(u16 *from, u16 *to, u8 *out, f32 ratio);
extern void func_00243608(s32 handle, u8 *out);

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
        func_00243048(from, to, out, ratio);
        func_00243608(node->unk24, out);
    }
}

void func_0022CC40(EventViewerState *viewer) {
    EvtViewNode *node = viewer->nodes;
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

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022CD30);

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
    dds3SetWorldCameraObject(dds3GetWorldObject(), entry);
    func_00106488(dds3GetCameraFieldOfView(entry));
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022CED0);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022D420);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022D528);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022E098);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022E288);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022E5A0);

extern s32 dds3GetSlot(s32 owner, s32 kind);
extern void evtPolygonMovieClampTime(s32 object, s32 arg1, s32 start, s32 end);


typedef struct EvtWorldLink {
    u8 pad0[0x20];
    s32 next; /* 0x20 */
} EvtWorldLink;

void evtViewerClampMovieTimes(s32 endTime, EventViewerState *viewer) {
    s32 table;
    s32 slots;
    s32 object;
    EvtViewNode *node;
    s32 time;

    if (dds3GetWorldObject() != 0) {
        table = (s32)((EvtWorldObject *)dds3GetWorldObject())->table;
        if (table != 0) {
            slots = (s32)((EvtWorldTable *)table)->slots;
            if (slots != 0) {
                object = (s32)((EvtWorldSlot *)slots)[EVT_WORLD_SLOT_MOVIE].head;
                if (object != 0) {
                    do {
                        node = viewer->nodes;
                        while (node != NULL) {
                            if (node->owner != 0 && object == dds3GetSlot(node->owner, 1)) {
                                if (node->kind == 2) {
                                    time = 0;
                                    if (node->hasGlyphs != 0) {
                                        time = node->glyphs->id;
                                    }
                                } else {
                                    time = node->time;
                                }
                                evtPolygonMovieClampTime(object, 0, time, endTime);
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

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022EB10);

extern void func_0022EB10();

void evtViewerSyncWorldGroups(s32 position, EventViewerState *viewer) {
    s32 object;
    EvtViewNode *node;
    EvtViewNode *found;

    if (dds3GetWorldObject() != 0) {
        object = (s32)((EvtWorldObject *)dds3GetWorldObject())->table->slots[EVT_WORLD_SLOT_UNIT].head;
        if (object != 0) {
            do {
                node = viewer->nodes;
                found = NULL;
                while (node != NULL) {
                    if (node->owner == object) {
                        found = node;
                        break;
                    }
                    node = node->next;
                }
                if (found != NULL) {
                    func_0022EB10(position, object, node, viewer, 0);
                }
                object = ((EvtWorldLink *)object)->next;
            } while (object != 0);
        }
    }
}

extern s32 sdfGetLodChunkValue();

void evtViewerApplyGlyphLodChannel(s32 position, EventViewerState *viewer) {
    EvtViewNode *node = viewer->nodes;
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

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022F038);

struct CampPacked;
extern void mnuUnpackNibbleFields(struct CampPacked *, s32 *, s32 *);
extern u32 itfMesGetWindowEntryItems(s32, s32);
void evtViewerMarkWindowActive(EventViewerState *viewer);

void func_0022F1C0(s32 id, EventViewerState *viewer) {
    s32 low;
    s32 high;
    EvtViewNode *node;
    EvtViewGlyph *glyph;

    if (viewer->windowContext != 0) {
        if (((EvtWindowContext *)viewer->windowContext)->windowHandle != -1) {
            for (node = viewer->nodes; node != NULL; node = node->next) {
                if (node->kind != 4) {
                    continue;
                }
                for (glyph = node->glyphs; glyph != NULL; glyph = glyph->next) {
                    if (glyph->id - 30 != id) {
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

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022F2E0);

/* Required to match: the signed raw load generates the original flag test. */
s32 evtViewerHasUpdateFlag(s32 viewerAddr) {
    return (*(s32 *)(viewerAddr + 4) & 0x10) > 0;
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022F418);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022F550);

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

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022FB30);

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

s32 evtViewFindNextGlyph(EventViewerState *viewer) {
    EvtViewGlyph *result = NULL;
    s32 best = 99999;
    EvtViewNode *node = viewer->nodes;

    while (node != NULL) {
        if (node->kind == 2) {
            EvtViewGlyph *glyph = node->glyphs;

            if (glyph != NULL) {
                do {
                    s32 x = glyph->id;

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
    EvtViewGlyph *result = NULL;
    s32 best = 99999;
    EvtViewNode *node = viewer->nodes;

    while (node != NULL) {
        if (node->kind == 2) {
            EvtViewGlyph *glyph = node->glyphs;

            if (glyph != NULL) {
                do {
                    s32 x = glyph->id;

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

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00231840);

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
        slot = viewer->sel->fieldSelector - 1;
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
            func_003014F0((u8 *)viewer + 0x22E8, D_003ADA98, D_003BBE78, D_003BBE7A);
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

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00232438);

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
    func_003014F0((u8 *)viewer + 0x22E8, D_003ADA98, D_003BBE78, D_003BBE7A);
    viewer->flags = 1;
    evtViewerDispatchFlagMode((u32)viewer);
    *(s32 *)((u8 *)viewer + 0x2238) = 0;
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

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00232E20);

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

