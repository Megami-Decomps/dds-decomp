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

typedef struct EventViewerState {
    u8 pad0[4];
    u32 flags;
    s32 unk8;
    u8 padC[8];
    s32 unk14;
    s32 mesh; /* 0x18 */
    u8 pad1C[0x2008];
    s32 selectedEntry;
    u8 pad2028[4];
    s32 fallbackEntry;
    u8 pad2030[4];
    struct EvtViewNode *nodes; /* 0x2034 */
    u8 pad2038[0x204];
    struct {
        u16 id;
        u8 pad2[6];
    } history[8];
    s32 historyCount;
    u32 currentId;
    u8 pad2284[0x24];
    s32 unk22A8;
    s32 unk22AC;
    u8 pad22B0[4];
    s32 unk22B4;
    u8 pad22B8[0x50];
    struct EvtViewSel *sel; /* 0x2308 */
    u8 pad230C[0xB4];
    s32 updateCount;
    u8 pad23C4;
    u8 windowActive;
    s16 unk23C6;
    u8 pad23C8[0x28];
    s32 glyphTickCount; /* 0x23F0 */
    u8 pad23F4[0x1C];
    u32 glyph; /* 0x2410: FrFontGlyph passed to func_00195868 */
    u8 pad2414[0xC];
    u8 slotType; /* 0x2420 */
    u8 slotFlag; /* 0x2421 */
    u8 pad2422[2];
    f32 slotValue; /* 0x2424 */
} EventViewerState;

typedef struct EvtViewSel {
    u8 pad00[0x1E];
    s8 mode;  /* 0x1E */
    s8 index; /* 0x1F */
} EvtViewSel;

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

void func_0022EA18(s32 arg0, EventViewerState *viewer) {
    s32 scene;
    s32 world;
    s32 object;
    EvtViewNode *node;
    s32 time;

    if (dds3GetWorldObject() != 0) {
        scene = *(s32 *)(dds3GetWorldObject() + 0x18);
        if (scene != 0) {
            world = *(s32 *)(scene + 8);
            if (world != 0) {
                object = *(s32 *)(world + 0x28);
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
                        object = *(s32 *)(object + 0x20);
                    } while (object != 0);
                }
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022EB10);

extern void func_0022EB10();

void func_0022EE90(s32 arg0, EventViewerState *viewer) {
    s32 object;
    EvtViewNode *node;
    EvtViewNode *found;

    if (dds3GetWorldObject() != 0) {
        object = *(s32 *)(*(s32 *)(*(s32 *)(dds3GetWorldObject() + 0x18) + 8) + 0x40);
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
                object = *(s32 *)(object + 0x20);
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

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022F778);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022F7F8);

void func_0022F9F0(void) {
}

void evtViewerAdvanceGlyphTick(EventViewerState *viewer) {
    s32 nextTick;

    if ((*(s32 *)((u8 *)viewer + 0x18) < *(s32 *)((u8 *)viewer + 0x14) - 3) && (0 < viewer->glyphTickCount))
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

void func_0022FDE8(u32 arg0) {
    s32 temp_v0;

    temp_v0 = (s32)arg0;
    if ((*(u32 *)(temp_v0 + 4) & 1) != 0) {
        func_0022FB30(0, *(u32 *)(temp_v0 + 0x18), arg0);
        return;
    }
    func_0022FB30(1, *(u32 *)(temp_v0 + 0x18), arg0);
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", evtViewFindNextGlyph);

INCLUDE_ASM(const s32, "game/code_0022CBA0", evtViewFindPrevGlyph);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_0022FF30);

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
    func_0019B4A0(*(s32 *)(v0 + 0x104));
    v0 = *(s32 *)(arg0 + 8);
    itfPanelSetPairFirst(*(s32 *)(v0 + 0x104), 0);
    v0 = *(s32 *)(arg0 + 8);
    itfMesResetWindow(*(s32 *)(v0 + 0x104));
    ((EventViewerState *)arg0)->windowActive = 0;
    *(u8 *)(arg0 + 0x23c4) = 0;
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

INCLUDE_ASM(const s32, "game/code_0022CBA0", evtViewCmdSetValue);

u32 func_00231C18(u32 arg0, u32 arg1, u32 arg2) {
    s32 p;
    u32 v;
    s32 idx;

    v = *(u32 *)(arg2 + 0x2310);
    p = func_0022BE40(arg2);
    if (p != 0) {
        idx = *(s32 *)(*(s32 *)(arg2 + 0x2308)) - 1;
        if ((u32)idx < 0x11u) {
            switch (idx) {
            case 3:
                *(u16 *)(p + 0xa) = v;
                break;
            case 1:
                *(u16 *)(p + 0xc) = v;
                break;
            case 0:
                *(u16 *)(p + 0x10) = v;
                break;
            case 11:
            case 15:
            case 16:
                *(u16 *)(p + 0x14) = v;
                break;
            }
        }
        func_0022E5A0(*(s32 *)(arg2 + 0x18), (void *)arg2);
        evtViewerPopHistory((EventViewerState *)arg2);
        return 0;
    }
}

u32 func_00231CC0(void) {
    return 0;
}

u32 func_00231CC8(u32 arg0, u32 arg1, u32 arg2) {
    s32 p;

    p = func_0022BE40(arg2);
    if (p != 0) {
        *(s32 *)(p + 0xc) = *(s32 *)(arg2 + 0x2310);
        func_0022E5A0(*(s32 *)(arg2 + 0x18), (void *)arg2);
        evtViewerPopHistory((EventViewerState *)arg2);
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

INCLUDE_ASM(const s32, "game/code_0022CBA0", evtViewCmdSetPosition);

u32 func_00231EF8(u32 arg0, u32 arg1, u32 arg2) {
    evtViewerPopHistory((EventViewerState *)arg2);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00231F18);

INCLUDE_ASM(const s32, "game/code_0022CBA0", evtViewCmdSetSlot);

extern char D_003ADA98[]; /* "E%3d_%03d" */
extern u16 D_003BBE78;
extern u16 D_003BBE7A;
extern void func_003014F0();
extern void func_0023E7F8(s32 slot, EventViewerState *viewer);
extern void func_002416E0(s32 slot, EventViewerState *viewer);

s32 evtViewCmdSelectMode(u32 arg0, u32 arg1, EventViewerState *viewer) {
    s32 applied = 0;
    s32 mode = viewer->unk22A8;

    if (mode < 3) {
        if (mode >= 0) {
            func_003014F0((u8 *)viewer + 0x22E8, D_003ADA98, D_003BBE78, D_003BBE7A);
            if (viewer->unk22A8 == 0) {
                func_0023E7F8(0, viewer);
                func_0023E7F8(1, viewer);
            } else if (viewer->unk22A8 == 1) {
                func_002416E0(0, viewer);
            } else if (viewer->unk22A8 == 2) {
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
        *(s32 *)(arg2 + 0x22ac) = 0;
        *(s32 *)(arg2 + 0x22b4) = 0;
        func_0022FF30(0xa, 0x9c, 0x54, arg2);
        return 0;
    }
}

INCLUDE_RODATA(const s32, "game/code_0022CBA0", D_003ADA98);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00232438);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_002326F8);

INCLUDE_ASM(const s32, "game/code_0022CBA0", func_00232720);

void *func_002329A0(s32 arg0) {
    void *temp_v0;

    temp_v0 = func_00101A70();
    func_0022E5A0(*(s32 *)((u8 *)temp_v0 + 0x18), temp_v0);
    func_00101A80(arg0, evtCreateFrameVariableTask());
    D_003BA904 |= 0x2000000;
    return (void *)func_00232720;
}

void *func_00232A00(void) {
    u64 temp_v0;

    temp_v0 = func_00101A70();
    func_00134C68();
    func_0022C408(temp_v0);
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
extern void func_001055C0();
extern void func_0022C4F0();
extern void func_0022C478();
extern void func_002D0918();
extern void func_00134CF0();
extern void func_00106240();
void func_002300B8(s32 arg0);

void func_00232BC0(viewer)
    EventViewerState *viewer;
{
    func_002441E8();
    func_00223540();
    func_002300B8((s32)viewer);
    mnuCampLinkFontGlyph(viewer);
    func_0014A298(0);
    func_00105888();
    D_00324590[0] = D_00324590[1] = D_00324590[2] = D_00324590[3] = 0.0f;
    if (*(s32 *)((u8 *)viewer + 0x2414) == 1) {
        func_00270030();
        *(s32 *)((u8 *)viewer + 0x2414) = 0;
    }
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    evtDestroySecondaryWorldNode();
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    if (*(s32 *)((u8 *)viewer + 0x242C) != 0) {
        func_002D0A10(*(s32 *)((u8 *)viewer + 0x242C));
        *(s32 *)((u8 *)viewer + 0x242C) = 0;
        *(s32 *)((u8 *)viewer + 0x2428) = 0;
    }
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    func_001055C0();
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    func_0022C4F0(viewer);
    func_0022C478(viewer);
    func_002D0918(*(s32 *)viewer);
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    func_00134CF0();
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    func_00106240(0);
    D_003BA904 |= 0x2000000;
}

void func_00232D08(void) {
    u64 temp_v0;

    temp_v0 = func_00101A70();
    func_00232BC0(temp_v0);
}

void func_00232D28(void) {
    u64 temp_v0;

    temp_v0 = func_00101A70();
    func_00232BC0(temp_v0);
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

