#include "common.h"

extern void *func_00101A70(void);

extern s32 D_003BAA00;
extern char D_003ADB20[]; /* "EventViewer" */
extern s32 kwlnTaskDestroyWithHierarchyByName(const char *name, s32 arg1);
s32 evtViewerHasUpdateFlag(s32 arg0);
void func_00232720(void);
void func_00134C68(void);
void func_00101A80(s32 arg0, s32 arg1);
s32 evtCreateFrameVariableTask(void);
void func_0022C408(u64 arg0);
void *func_002329A0(s32 arg0);
extern u32 D_003BA904;
s32 func_0022BE40(s32 arg0);
void func_0022E5A0(s32 arg0, void *arg1);
void func_0022FF30(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 dds3GetWorldObject(void);
void func_001109B8(s32 arg0, u32 arg1);
f32 dds3GetCameraValue(s32 arg0);
s32 func_00106488(f32 arg0);
void func_00270030(void);
void func_00270068(void);

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
    u32 glyph; /* 0x2410: FrFontGlyph passed to func_00195868 */
    s32 timedActive; /* 0x2414: gated time interval */
    s32 timedStart;  /* 0x2418 */
    s32 timedEnd;    /* 0x241C: negative is an open endpoint */
    u8 slotType; /* 0x2420 */
    u8 slotFlag; /* 0x2421 */
    u8 pad2422[2];
    f32 slotValue; /* 0x2424 */
    s32 pendingWork;  /* 0x2428: reset when pendingResource is released */
    s32 pendingResource; /* 0x242C */
    u8 pad2430[0x60]; /* allocated as 0x2490 bytes */
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
} EvtViewGlyph;

typedef struct EvtViewNode {
    s32 kind;                 /* 0x00 */
    u8 pad04[0xC];
    u32 owner;                /* 0x10 */
    u8 pad14[8];
    s16 time;                 /* 0x1C */
    u8 pad1E[0x32];
    s32 hasGlyphs;            /* 0x50 */
    EvtViewGlyph *glyphs;     /* 0x54 */
    u8 pad58[0x24];
    struct EvtViewNode *next; /* 0x7C */
} EvtViewNode;


INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022CBA0);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022CC40);

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
    func_001109B8(dds3GetWorldObject(), entry);
    func_00106488(dds3GetCameraValue(entry));
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022CED0);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022D420);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022D528);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022E098);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022E288);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022E5A0);

extern s32 dds3GetSlot(s32 owner, s32 kind);
extern void evtPolygonMovieClampTime(s32 object, s32 arg1, s32 start, s32 end);

/* The viewer follows two lists within the same world layer. These partial
 * layouts name only offsets traversed here; the owning world remains opaque. */
typedef struct EvtWorldRoot {
    u8 pad0[0x18];
    s32 scene; /* 0x18 */
} EvtWorldRoot;

typedef struct EvtWorldScene {
    u8 pad0[8];
    s32 layer; /* 0x08 */
} EvtWorldScene;

typedef struct EvtWorldLayer {
    u8 pad0[0x28];
    s32 movieObjects; /* 0x28 */
    u8 pad2C[0x14];
    s32 groupObjects; /* 0x40 */
} EvtWorldLayer;

typedef struct EvtWorldLink {
    u8 pad0[0x20];
    s32 next; /* 0x20 */
} EvtWorldLink;

void evtViewerClampMovieTimes(s32 arg0, EventViewerState *viewer) {
    s32 scene;
    s32 world;
    s32 object;
    EvtViewNode *node;
    s32 time;

    if (dds3GetWorldObject() != 0) {
        scene = ((EvtWorldRoot *)dds3GetWorldObject())->scene;
        if (scene != 0) {
            world = ((EvtWorldScene *)scene)->layer;
            if (world != 0) {
                object = ((EvtWorldLayer *)world)->movieObjects;
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
                                evtPolygonMovieClampTime(object, 0, time, arg0);
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

void evtViewerSyncWorldGroups(s32 arg0, EventViewerState *viewer) {
    s32 object;
    EvtViewNode *node;
    EvtViewNode *found;

    if (dds3GetWorldObject() != 0) {
        object = ((EvtWorldLayer *)((EvtWorldScene *)((EvtWorldRoot *)dds3GetWorldObject())->scene)->layer)->groupObjects;
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
                    func_0022EB10(arg0, object, node, viewer, 0);
                }
                object = ((EvtWorldLink *)object)->next;
            } while (object != 0);
        }
    }
}

extern s32 sdfGetLodChunkValue();

void func_0022EF38(s32 position, EventViewerState *viewer) {
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

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022F1C0);

void evtViewerCountFlaggedUpdates(EventViewerState *viewer) {
    s64 active;

    active = evtViewerHasUpdateFlag((s32)viewer);
    if (active != 0) {
        viewer->updateCount = viewer->updateCount + 1;
    }
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022F2E0);

/* Required to match: the signed raw load generates the original flag test. */
s32 evtViewerHasUpdateFlag(s32 arg0) {
    return (*(s32 *)(arg0 + 4) & 0x10) > 0;
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022F418);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022F550);

/* Advance or stop the timed viewer action according to the current position. */
s32 func_0022F778(EventViewerState *viewer) {
    if (viewer->timedActive == 1) {
        if (viewer->glyphAdvancePosition < viewer->timedStart) {
            func_00270030();
            viewer->timedActive = 0;
            viewer->timedStart = 0;
            viewer->timedEnd = 0;
        } else if (viewer->timedEnd == -1) {
            func_00270068();
        } else if (viewer->glyphAdvancePosition >= viewer->timedEnd) {
            func_00270030();
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
        func_00195868(viewer->glyph);
        nextTick = viewer->glyphTickCount + 1;
        viewer->glyphTickCount = nextTick;
        if (0x1d < nextTick) {
            viewer->glyphTickCount = 0;
        }
    }
}

void func_0022FA60(void) {
}

s32 func_00230438(u32 condition);

EvtViewGlyph *func_0022FA68(EvtViewNode *group, s32 position, s32 channel) {
    s32 bestId = -1;
    EvtViewGlyph *best = NULL;
    EvtViewGlyph *glyph = group->glyphs;

    if (glyph != NULL) {
        do {
            if (position >= glyph->id && bestId < glyph->id && glyph->kind == 5 &&
                glyph->channel == channel && func_00230438(glyph->condition) == 1) {
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
    if ((*(u32 *)(viewer + 4) & 1) != 0) {
        func_0022FB30(0, *(u32 *)(viewer + 0x18), viewerAddr);
        return;
    }
    func_0022FB30(1, *(u32 *)(viewer + 0x18), viewerAddr);
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

void func_0022FF30(s32 mode, s32 arg1, s32 arg2, s32 viewerAddr) {
    EventViewerState *viewer = (EventViewerState *)viewerAddr;
    s32 count = viewer->historyCount + 1;

    viewer->currentId = mode;
    viewer->historyCount = count;
    viewer->history[count].id = mode;
    viewer->history[count].a = arg1;
    viewer->history[count].b = arg2;
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

void func_002300B8(s32 arg0) {
    s32 v0;
    s32 v1;

    v0 = ((EventViewerState *)arg0)->windowContext;
    if (v0 == 0) {
        return;
    }
    v1 = *(s32 *)(v0 + 0x104);
    if (v1 == -1) {
        return;
    }
    itfMesCleanupWindow(v1, 1);
    v0 = ((EventViewerState *)arg0)->windowContext;
    func_0019B4A0(*(s32 *)(v0 + 0x104));
    v0 = ((EventViewerState *)arg0)->windowContext;
    itfPanelSetPairFirst(*(s32 *)(v0 + 0x104), 0);
    v0 = ((EventViewerState *)arg0)->windowContext;
    itfMesResetWindow(*(s32 *)(v0 + 0x104));
    ((EventViewerState *)arg0)->windowActive = 0;
    ((EventViewerState *)arg0)->pad23C4 = 0;
}

void func_00230128(EventViewerState *viewer) {
    viewer->windowActive = 1;
}

void func_00230138(EventViewerState *viewer) {
    viewer->windowActive = 0;
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00230140);

s32 func_00230438(u32 arg0) {
    u32 idx;
    u32 lo;

    idx = (arg0 << 16) >> 28;
    lo = arg0 & 0xfff;
    if (idx == 0) {
        return 1;
    }
    return (*(s32 *)(D_003BAA00 + idx * 4 + 0x35c) ^ lo) == 0;
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

u32 func_00230A38(u32 arg0, u32 arg1, u32 arg2) {
    func_0022FF30(5, 0x90, 0x48, arg2);
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
s32 evtViewCmdSetValue(s32 arg0, s32 arg1, EventViewerState *viewer) {
    s32 value = viewer->commandValue;
    EvtViewEntry *entry = (EvtViewEntry *)func_0022BE40((s32)viewer);

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
u32 func_00231C18(u32 unused0, u32 unused1, EventViewerState *viewer) {
    EvtViewEntry *entry;
    u32 value;
    s32 slot;

    value = viewer->commandValue;
    entry = (EvtViewEntry *)func_0022BE40((s32)viewer);
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
u32 func_00231CC8(u32 unused0, u32 unused1, EventViewerState *viewer) {
    EvtViewEntry *entry;

    entry = (EvtViewEntry *)func_0022BE40((s32)viewer);
    if (entry != 0) {
        entry->p0C.i = viewer->commandValue;
        func_0022E5A0(viewer->glyphAdvancePosition, viewer);
        evtViewerPopHistory(viewer);
        return 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00231D18);

u32 kwlnBattleCopyMatrix(u32 arg0, u32 arg1, u8 *scene) {
    u8 *record = (u8 *)func_0022BE40((s32)scene);
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
s32 evtViewCmdSetPosition(s32 arg0, s32 arg1, EventViewerState *viewer) {
    EvtViewEntry *entry = (EvtViewEntry *)func_0022BE40((s32)viewer);

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

u32 func_00231EF8(u32 arg0, u32 arg1, u32 arg2) {
    evtViewerPopHistory((EventViewerState *)arg2);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00231F18);

/* Copy the selected slot descriptor and numeric value into the script entry. */
s32 evtViewCmdSetSlot(s32 arg0, s32 arg1, EventViewerState *viewer) {
    EvtViewEntry *entry = (EvtViewEntry *)func_0022BE40((s32)viewer);

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
extern void func_003014F0();
extern void func_0023E7F8(s32 slot, EventViewerState *viewer);
extern void func_002416E0(s32 slot, EventViewerState *viewer);

/* Apply one of three viewer selection modes to the selected slots. */
s32 evtViewCmdSelectMode(u32 arg0, u32 arg1, EventViewerState *viewer) {
    s32 applied = 0;
    s32 mode = viewer->selectionMode;

    if (mode < 3) {
        if (mode >= 0) {
            func_003014F0((u8 *)viewer + 0x22E8, D_003ADA98, D_003BBE78, D_003BBE7A);
            if (viewer->selectionMode == 0) {
                func_0023E7F8(0, viewer);
                func_0023E7F8(1, viewer);
            } else if (viewer->selectionMode == 1) {
                func_002416E0(0, viewer);
            } else if (viewer->selectionMode == 2) {
                func_002416E0(1, viewer);
            }
            applied = 1;
        }
    }
    return applied ? -1 : 0;
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00232108);

u32 func_002323E8(u32 arg0, u32 arg1, u32 arg2) {
    if (func_0022BE40(arg2) != 0) {
        ((EventViewerState *)arg2)->unk22AC = 0;
        ((EventViewerState *)arg2)->unk22B4 = 0;
        func_0022FF30(0xa, 0x9c, 0x54, arg2);
        return 0;
    }
}

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003ADA98);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00232438);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_002326F8);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00232720);

/* Update the active viewer, then switch to its frame-variable task. */
void *func_002329A0(s32 task) {
    void *viewer;

    viewer = func_00101A70();
    func_0022E5A0(((EventViewerState *)viewer)->glyphAdvancePosition, viewer);
    func_00101A80(task, evtCreateFrameVariableTask());
    D_003BA904 |= 0x2000000;
    return (void *)func_00232720;
}

/* Initialize the active viewer and schedule its next update callback. */
void *func_00232A00(void) {
    u64 viewer;

    viewer = func_00101A70();
    func_00134C68();
    func_0022C408(viewer);
    D_003BA904 |= 0x2000000;
    return (void *)func_002329A0;
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00232A50);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00232B30);

extern f32 D_00324590[];
extern void func_002441E8();
extern void func_00223540();
extern void mnuCampLinkFontGlyph();
extern void func_0014A298();
extern void func_00105888();
extern void func_00270030();
extern s32 sdfCheckPendingWorkWithInterrupts();
extern void evtDestroySecondaryWorldNode();
extern void func_002D0A10();
extern void kwlnTextureReleaseHeldReference();
extern void func_0022C4F0();
extern void func_0022C478();
extern void func_002D0918();
extern void func_00134CF0();
extern void func_00106240();
void func_002300B8(s32 arg0);

void evtViewerReleaseResources(viewer)
    EventViewerState *viewer;
{
    func_002441E8();
    func_00223540();
    func_002300B8((s32)viewer);
    mnuCampLinkFontGlyph(viewer);
    func_0014A298(0);
    func_00105888();
    D_00324590[0] = D_00324590[1] = D_00324590[2] = D_00324590[3] = 0.0f;
    if (viewer->timedActive == 1) {
        func_00270030();
        viewer->timedActive = 0;
    }
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    evtDestroySecondaryWorldNode();
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    if (viewer->pendingResource != 0) {
        func_002D0A10(viewer->pendingResource);
        viewer->pendingResource = 0;
        viewer->pendingWork = 0;
    }
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    kwlnTextureReleaseHeldReference();
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    func_0022C4F0(viewer);
    func_0022C478(viewer);
    func_002D0918(viewer->resourceHandle);
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    func_00134CF0();
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    func_00106240(0);
    D_003BA904 |= 0x2000000;
}

/* Destroy the currently active event viewer. */
void func_00232D08(void) {
    u64 viewer;

    viewer = func_00101A70();
    evtViewerReleaseResources(viewer);
}

/* Alternate destroy callback for the same active viewer. */
void func_00232D28(void) {
    u64 viewer;

    viewer = func_00101A70();
    evtViewerReleaseResources(viewer);
}

void func_00232D48(void *unused) {
    mnuCampInitFontResource();
}

extern u32 D_003BA8EC;
extern s32 func_002D03F8(s32 size);
extern u32 *sdfResourceRetainAddress(s32 handle);
extern void *memset(void *dst, s32 value, u32 size);
extern void *kwlnTaskCreate(const char *name, s32 id, s32 arg2, s32 arg3, void *update, void *destroy, void *data);
extern s32 evtCreateSkyTask(void);

void func_00232D60(void) {
    s32 handle;
    u32 *state;
    void *task;

    D_003BA8EC = 0x80000000;
    handle = func_002D03F8(0x2490);
    state = sdfResourceRetainAddress(handle);
    memset(state, 0, 0x2490);
    *state = handle;
    task = kwlnTaskCreate(D_003ADB20, 0x3EB, 1, 1, func_00232A00, func_00232D08, state);
    func_00101A80((s32)task, evtCreateSkyTask());
    func_00232D48(state);
}

void evtEventViewerDestroyTask(void) {
    kwlnTaskDestroyWithHierarchyByName(D_003ADB20, 1);
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00232E20);

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003ADB20);

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

