#include "common.h"
extern u16 D_004372B0;
extern u16 D_004372B2;
extern u8 D_00423050[];
extern void func_0035C860();
extern void func_00259AE8();
extern void func_0025CAF8();
extern void *dds3GetWorldObject(void);
extern void func_00110BE0(void *, s32);
extern f32 dds3GetCameraValue(s32);
extern void func_001063A8(f32);

extern s32 evtViewerHasUpdateFlag(s32);

extern s32 D_00435DD0;

s32 func_002467B8(s32 arg0);

void func_00249088(s32 arg0, void *arg1);

void func_0024AB38(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

extern void *func_00101958(void);

void func_0024D430(void);

void func_00101968(s32 arg0, s32 arg1);

s32 evtCreateFrameVariableTask(void);

void *func_0024D6B0(s32 arg0);

extern u32 D_00435CD4;

void func_00137818(void);

void func_00246D80(u64 arg0);

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

typedef struct EventViewerState {
    u8 pad0[0x14];
    s32 glyphAdvanceLimit;
    s32 glyphAdvancePosition;
    u8 pad1C[0x2008];
    s32 selectedEntry; /* 0x2024 */
    u8 pad2028[4];
    s32 fallbackEntry; /* 0x202C */
    u8 pad2030[4];
    EvtViewerGroup *groups;
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
    u8 pad22B8[0x50];
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
} EventViewerState;

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

extern char D_004230D0[]; /* "EventViewer" */

extern s32 kwlnTaskDestroyWithHierarchyByName(const char *name, s32 arg1);

typedef struct EvtViewGlyph {
    u16 id;       /* 0x00 */
    u8 pad02[6];
    s8 kind;      /* 0x08 */
    u8 pad09[3];
    s8 channel;   /* 0x0C */
    u8 pad0D;
    s16 param;    /* 0x0E */
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

INCLUDE_ASM(const s32, "game/code_00247518", func_00247518);

INCLUDE_ASM(const s32, "game/code_00247518", func_002475C8);

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
        func_00110BE0(dds3GetWorldObject(), unit);
        func_001063A8(dds3GetCameraValue(unit));
    }
}

INCLUDE_ASM(const s32, "game/code_00247518", func_00247858);

INCLUDE_ASM(const s32, "game/code_00247518", func_00247DE0);

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
extern void func_0024F130(s32 object, s32 arg1, s32 start, s32 end, s32 extra);

void func_00249598(s32 arg0, EventViewerState *viewer) {
    s32 scene;
    s32 world;
    s32 object;
    EvtViewNode *node;
    s32 time;
    s32 extra;
    EvtViewGlyph *glyph;

    if (dds3GetWorldObject() != 0) {
        scene = *(s32 *)((u8 *)dds3GetWorldObject() + 0x18);
        if (scene != 0) {
            world = *(s32 *)(scene + 8);
            if (world != 0) {
                object = *(s32 *)(world + 0x28);
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
                                func_0024F130(object, 0, time, arg0, extra);
                                break;
                            }
                            node = node->next;
                        }
                        object = *(s32 *)(object + 0x20);
                    } while (object != 0);
                }
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00247518", func_002496B0);

void func_00249A98(u32 arg0, EventViewerState *viewer) {
    u8 *list;
    EvtViewNode *node;
    EvtViewNode *found;

    if (dds3GetWorldObject() != 0) {
        list = *(u8 **)(*(s32 *)(*(s32 *)((u8 *)dds3GetWorldObject() + 0x18) + 8) + 0x40);
        while (list != 0) {
            found = 0;
            for (node = (EvtViewNode *)viewer->groups; node != 0; node = node->next) {
                if (node->owner == (u32)list) {
                    found = node;
                    break;
                }
            }
            if (found != 0) {
                func_002496B0(arg0, list, node, viewer, 0);
            }
            list = *(u8 **)(list + 0x20);
        }
    }
}

extern s32 sdfGetLodChunkValue();

void func_00249B40(s32 position, EventViewerState *viewer) {
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

INCLUDE_ASM(const s32, "game/code_00247518", func_00249DC8);

void evtViewerCountFlaggedUpdates(EventViewerState *viewer) {
    s64 active;

    active = evtViewerHasUpdateFlag((s32)viewer);
    if (active != 0) {
        viewer->updateCount = viewer->updateCount + 1;
    }
}

INCLUDE_ASM(const s32, "game/code_00247518", func_00249EE8);

s32 evtViewerHasUpdateFlag(s32 arg0) {
    return (*(s32 *)(arg0 + 4) & 0x10) > 0;
}


INCLUDE_ASM(const s32, "game/code_00247518", func_0024A020);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024A158);

extern void func_002A8008(void);
extern void func_002A7FD0(void);
/* Advance or stop the timed viewer action according to the current position. */
s32 func_0024A380(EventViewerState *viewer) {
    if (viewer->timedActive == 1) {
        if (viewer->glyphAdvancePosition < viewer->timedStart) {
            func_002A7FD0();
            viewer->timedActive = 0;
            viewer->timedStart = 0;
            viewer->timedEnd = 0;
        } else if (viewer->timedEnd < 0) {
            func_002A8008();
        } else if (viewer->glyphAdvancePosition >= viewer->timedEnd) {
            func_002A7FD0();
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
        func_0019D518(viewer->glyph);
        nextTick = viewer->glyphTickCount + 1;
        viewer->glyphTickCount = nextTick;
        if (0x1d < nextTick) {
            viewer->glyphTickCount = 0;
        }
    }
}

void func_0024A668(void) {
}

EvtViewGlyph *func_0024A670(EvtViewNode *group, s32 position, s32 channel) {
    s32 bestId = -1;
    EvtViewGlyph *best = NULL;
    EvtViewGlyph *glyph = group->glyphs;

    if (glyph != NULL) {
        do {
            if (position >= glyph->id && bestId < glyph->id && glyph->kind == 5 &&
                glyph->channel == channel && func_0024B040(glyph->condition) == 1) {
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
void func_0024A9F0(u32 viewerAddr) {
    s32 viewer;

    viewer = (s32)viewerAddr;
    if ((*(u32 *)(viewer + 4) & 1) != 0) {
        func_0024A738(0, *(u32 *)(viewer + 0x18), viewerAddr);
        return;
    }
    func_0024A738(1, *(u32 *)(viewer + 0x18), viewerAddr);
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

void func_0024AB38(s32 mode, s32 arg1, s32 arg2, s32 viewerAddr) {
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
                *(s32 *)((u8 *)viewer + 0x22CC) = 0;
                *(s32 *)((u8 *)viewer + 0x22D0) = 0;
                *(u8 *)((u8 *)viewer + 0x22D4) = 0;
                *(u8 *)((u8 *)viewer + 0x22E0) = 0;
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

void func_0024ACC0(s32 arg0) {
    s32 v0;
    s32 v1;

    v0 = *(s32 *)(arg0 + 8);
    if (v0 == 0) {
        return;
    }
    v1 = *(s32 *)(v0 + 0x104);
    if (v1 == -1) {
        return;
    }
    itfMesCleanupWindow(v1, 1);
    v0 = *(s32 *)(arg0 + 8);
    func_001A34D0(*(s32 *)(v0 + 0x104));
    v0 = *(s32 *)(arg0 + 8);
    itfPanelSetPairFirst(*(s32 *)(v0 + 0x104), 0);
    v0 = *(s32 *)(arg0 + 8);
    itfMesResetWindow(*(s32 *)(v0 + 0x104));
    ((EventViewerState *)arg0)->windowActive = 0;
    *(u8 *)(arg0 + 0x23c4) = 0;
}

void func_0024AD30(EventViewerState *viewer) {
    viewer->windowActive = 1;
}

void func_0024AD40(EventViewerState *viewer) {
    viewer->windowActive = 0;
}

INCLUDE_ASM(const s32, "game/code_00247518", func_0024AD48);

s32 func_0024B040(u32 arg0) {
    u32 idx;
    u32 lo;

    idx = (arg0 << 16) >> 28;
    lo = arg0 & 0xfff;
    if (idx == 0) {
        return 1;
    }
    return (*(s32 *)(D_00435DD0 + idx * 4 + 0x35c) ^ lo) == 0;
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

u32 func_0024B678(u32 arg0, u32 arg1, u32 arg2) {
    func_0024AB38(5, 0x90, 0x48, arg2);
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
s32 evtViewCmdSetValue(s32 arg0, s32 arg1, EventViewerState *viewer) {
    s32 value = viewer->commandValue;
    EvtViewEntry *entry = (EvtViewEntry *)func_002467B8((s32)viewer);

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
u32 func_0024C928(u32 unused0, u32 unused1, EventViewerState *viewer) {
    EvtViewEntry *entry;
    u32 value;
    s32 slot;

    value = viewer->commandValue;
    entry = (EvtViewEntry *)func_002467B8((s32)viewer);
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
u32 func_0024C9D8(u32 unused0, u32 unused1, EventViewerState *viewer) {
    EvtViewEntry *entry;

    entry = (EvtViewEntry *)func_002467B8((s32)viewer);
    if (entry != 0) {
        entry->p0C.i = viewer->commandValue;
        func_00249088(viewer->glyphAdvancePosition, viewer);
        evtViewerPopHistory(viewer);
        return 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00247518", func_0024CA28);

u32 kwlnBattleCopyMatrix(u32 arg0, u32 arg1, u8 *scene) {
    u8 *record = (u8 *)func_002467B8((s32)scene);
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
        func_00249088(*(s32 *)(scene + 0x18), scene);
        evtViewerPopHistory((EventViewerState *)scene);
        return 0;
    }
    return (u32)record;
}

/* Transfer a selected two-component viewer position to the command entry. */
s32 evtViewCmdSetPosition(s32 arg0, s32 arg1, EventViewerState *viewer) {
    EvtViewEntry *entry = (EvtViewEntry *)func_002467B8((s32)viewer);

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

u32 func_0024CC08(u32 arg0, u32 arg1, u32 arg2) {
    evtViewerPopHistory((EventViewerState *)arg2);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00247518", func_0024CC28);

/* Copy the selected slot descriptor and numeric value into the script entry. */
s32 evtViewCmdSetSlot(s32 arg0, s32 arg1, EventViewerState *viewer) {
    EvtViewEntry *entry = (EvtViewEntry *)func_002467B8((s32)viewer);

    entry->p0C.b[0] = viewer->slotType;
    entry->p0C.b[1] = viewer->slotFlag;
    entry->p14.f = viewer->slotValue;
    func_00249088(viewer->glyphAdvancePosition, viewer);
    evtViewerPopHistory(viewer);
    return 0;
}

/* Apply one of three viewer selection modes to the selected slots. */
s32 evtViewCmdSelectMode(s32 arg0, s32 arg1, EventViewerState *viewer) {
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
                func_0025CAF8(0, viewer);
            } else if (mode == 2) {
                func_0025CAF8(1, viewer);
            }
            handled = 1;
        }
    }
    return handled ? -1 : 0;
}

INCLUDE_ASM(const s32, "game/code_00247518", func_0024CE18);

u32 func_0024D0F8(u32 arg0, u32 arg1, u32 arg2) {
    if (func_002467B8(arg2) != 0) {
        *(s32 *)(arg2 + 0x22ac) = 0;
        *(s32 *)(arg2 + 0x22b4) = 0;
        func_0024AB38(0xa, 0x9c, 0x54, arg2);
        return 0;
    }
}

INCLUDE_RODATA(const s32, "game/code_00247518", D_00423050);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024D148);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024D408);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024D430);

/* Update the active viewer, then switch to its frame-variable task. */
void *func_0024D6B0(s32 task) {
    void *viewer;

    viewer = func_00101958();
    func_00249088(*(s32 *)((u8 *)viewer + 0x18), viewer);
    func_00101968(task, evtCreateFrameVariableTask());
    D_00435CD4 |= 0x2000000;
    return (void *)func_0024D430;
}

/* Initialize the active viewer and schedule its next update callback. */
void *func_0024D710(void) {
    u64 viewer;

    viewer = func_00101958();
    func_00137818();
    func_00246D80(viewer);
    D_00435CD4 |= 0x2000000;
    return (void *)func_0024D6B0;
}

s32 func_0024D760(u8 *ctx) {
    s32 id = *(s32 *)(ctx + 0x10C);

    if (id == 0x28B || id == 0x28E) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00247518", func_0024D788);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024D878);

u8 func_0024D908(s32 arg0) {
    return *(s32 *)(arg0 + 0x10c) == 0x263;
}

extern f32 D_0037F590[];
extern void func_0023E178();
extern void mnuCampReleaseEffectHandle();
extern void func_0014E668();
extern void func_001057A8();
extern s32 sdfGraphHasPendingWorkInterruptSafe();
extern void evtDestroyWorldSecondaryNode();
extern void func_003298C0();
extern void func_001054E0();
extern void func_00246E68();
extern void func_00246DF0();
extern void func_003297C8();
extern void func_001378A0();
extern void func_00106160();
extern void func_0025F5D0();
void func_0024ACC0(s32 arg0);

void func_0024D918(viewer)
    EventViewerState *viewer;
{
    if (func_0024D908(*(s32 *)((u8 *)viewer + 8)) == 0) {
        func_0025F5D0(viewer);
    }
    func_0023E178();
    func_0024ACC0((s32)viewer);
    mnuCampReleaseEffectHandle(viewer);
    func_0014E668(0);
    func_001057A8();
    D_0037F590[0] = D_0037F590[1] = D_0037F590[2] = D_0037F590[3] = 0.0f;
    if (viewer->timedActive == 1) {
        if (viewer->timedEnd != -2 || evtViewerHasUpdateFlag((s32)viewer) == 1) {
            func_002A7FD0();
        }
        viewer->timedActive = 0;
    }
    while (sdfGraphHasPendingWorkInterruptSafe() != 0) {
    }
    evtDestroyWorldSecondaryNode();
    while (sdfGraphHasPendingWorkInterruptSafe() != 0) {
    }
    if (*(s32 *)((u8 *)viewer + 0x242C) != 0) {
        func_003298C0(*(s32 *)((u8 *)viewer + 0x242C));
        *(s32 *)((u8 *)viewer + 0x242C) = 0;
        *(s32 *)((u8 *)viewer + 0x2428) = 0;
    }
    while (sdfGraphHasPendingWorkInterruptSafe() != 0) {
    }
    func_001054E0();
    while (sdfGraphHasPendingWorkInterruptSafe() != 0) {
    }
    func_00246E68(viewer);
    func_00246DF0(viewer);
    func_003297C8(*(s32 *)viewer);
    while (sdfGraphHasPendingWorkInterruptSafe() != 0) {
    }
    func_001378A0();
    while (sdfGraphHasPendingWorkInterruptSafe() != 0) {
    }
    func_00106160(0);
    D_00435CD4 |= 0x2000000;
}

void func_0024DAA0(void) {
    u64 temp_v0;

    temp_v0 = func_00101958();
    func_0024D918(temp_v0);
}

void func_0024DAC0(void) {
    u64 temp_v0;

    temp_v0 = func_00101958();
    func_0024D918(temp_v0);
}

void func_0024DAE0() {
    mnuCampInitFontResource();
}

extern u32 D_00435CBC;
extern s32 func_003292A8(s32 size);
extern u32 *sdfResourceRetainAddress(s32 handle);
extern void *memset(void *dst, s32 value, u32 size);
extern void *kwlnTaskCreate(const char *name, s32 id, s32 arg2, s32 arg3, void *update, void *destroy, void *data);
void func_0024DAF8(void) {
    u32 *state;
    s32 handle;
    void *task;

    D_00435CBC = 0x80000000;
    handle = func_003292A8(0x24BC);
    state = sdfResourceRetainAddress(handle);
    memset(state, 0, 0x24BC);
    *state = handle;
    task = kwlnTaskCreate(D_004230D0, 0x3EB, 1, 1, func_0024D710, func_0024DAA0, state);
    func_00101968((s32)task, evtCreateSkyTask());
    func_0024DAE0(state);
}

void evtEventViewerDestroyTask(void) {
    kwlnTaskDestroyWithHierarchyByName(D_004230D0, 1);
}

INCLUDE_ASM(const s32, "game/code_00247518", func_0024DBB8);

INCLUDE_RODATA(const s32, "game/code_00247518", D_004230D0);

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

