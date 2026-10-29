#include "sdk_ps2.h"
#include "scr.h"
#include "common.h"
#include "sdf_file_existence_api.h"
#include "mdl_flag_api.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "sdf_packet_list.h"
#include "sdf.h"
#include "sdf_sif_command.h"
#include "kwln_task_lifecycle.h"
#include "sdf_packet_alloc.h"

extern void scrDestroyAllNamedProcesses(void);

extern s32 dpy;

s32 EventDebugProcess1(dds3ProcessID_t *task);

extern u32 D_003BA8EC;

extern char D_003ACD18[]; /* "EventTest" */

void evtFreeStage(void);

void func_0022AB58(void) {
}

/* After launching a test script, keep the test task idle and return 0. */
s32 evtDebugCameraStep(dds3ProcessID_t *task) {
    func_0022AB58();
    return 0;
}

/* Return the same update callback installed by evtInitEventTest. */
void *evtGetDebugProcess(void) {
    return (void *)EventDebugProcess1;
}


extern dds3ProcessID_t *scrStartScriptByName(u32, const char *, ScrProcedureIndex);

extern SdfPoolNode D_00325708;

extern u8 dds3PadButtonState[2][2][16];

extern s32 D_003BBDEC;

INCLUDE_RODATA(const s32, "game/code_0022AB58", D_003ACC58);

s32 EventDebugProcess1(dds3ProcessID_t *task) {
    char path[0x40];
    sdfDrawTag_t *list;

    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    dds3ConsPanelTagLink(list, 0x84, 0x54, 0x15, 10);
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7840, 0x7BA0, 0xFEFFFF, 0, "Event Debug"));
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7A80, 0x7C60, 0xFEFFFF, 6, "E%03d.BF", D_003BBDEC));
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7900, 0x7D20, 0xFEFFFF, 0, "L,R  = EVENT SELECT"));
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7900, 0x7D80, 0xFEFFFF, 0, "RR   = ENTER"));
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7900, 0x7DE0, 0xFEFFFF, 0, "RU   = SET E500"));
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7900, 0x7E40, 0xFEFFFF, 0, "RL   = SET E600"));
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7900, 0x7EA0, 0xFEFFFF, 0, "RD   = RESET FLAG"));
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7900, 0x7F00, 0xFEFFFF, 0, "L1R1 = +-100"));
    D_00325708.append(&D_00325708, list);

    if ((s8)dds3PadButtonState[1][0][1] < 0) {
        sprintf(path, "/event/e%03d/e%03d/scr/e%03d.bf",
                      D_003BBDEC - D_003BBDEC % 10, D_003BBDEC, D_003BBDEC);
        if (sdfFileCheckExist(path) != SDF_FILE_NOT_FOUND) {
            D_003BA8EC = 0x80000000;
            scrStartScriptByName(0x3EB, path, 0);
            return (s32)evtDebugCameraStep;
        }
    }
    if ((s8)dds3PadButtonState[1][0][3] < 0) {
        evtBitAllOff();
        return (s32)evtGetDebugProcess;
    }
    if (dds3PadButtonState[1][0][5] & 2) {
        D_003BBDEC++;
    }
    if ((dds3PadButtonState[1][0][4] & 2) && D_003BBDEC > 500) {
        D_003BBDEC--;
    }
    if ((s8)dds3PadButtonState[1][0][2] < 0) {
        D_003BBDEC = 500;
    }
    if ((s8)dds3PadButtonState[1][0][0] < 0) {
        D_003BBDEC = 600;
    }
    if ((dds3PadButtonState[1][0][8] & 2) && D_003BBDEC > 500) {
        D_003BBDEC -= 100;
        if (D_003BBDEC < 500) {
            D_003BBDEC = 500;
        }
    }
    if (dds3PadButtonState[1][0][10] & 2) {
        D_003BBDEC += 100;
    }
    return 0;
}

/* Test-task teardown destroys the named script processes. */
void EventDebugShutdown(dds3ProcessID_t *task) {
    scrDestroyAllNamedProcesses();
}

void evtInitEventTest(void) {
    D_003BA8EC = 0x80000000;
    dds3StartProcess(D_003ACD18, 0x2AF9, 1, 1, EventDebugProcess1, EventDebugShutdown, 0);
}

INCLUDE_RODATA(const s32, "game/code_0022AB58", D_003ACD18);

void evtChkEndProcess(void) {
    dds3KillProcessName(D_003ACD18, 1);
    dds3KillProcessName("PolygonMovie", 0);
    evtFreeStage();
}

INCLUDE_RODATA(const s32, "game/code_0022AB58", D_003ACD38);

INCLUDE_RODATA(const s32, "game/code_0022AB58", D_003ACD48);

INCLUDE_RODATA(const s32, "game/code_0022AB58", D_003ACD58);

INCLUDE_RODATA(const s32, "game/code_0022AB58", D_003ACD68);

INCLUDE_RODATA(const s32, "game/code_0022AB58", D_003ACD78);

#ifdef NON_MATCHING
#include "common.h"
#include "kwln.h"
#include "sdf.h"
#include "sdf_packet_list.h"
#include "sdf_packet_alloc.h"
#include "sdf_sif_command.h"
#include "evt_world.h"
#include "dds3_world_primary.h"
#include "dds3_node_diagnostic_row.h"

extern SdfPoolNode sdfGrOtTable[];
extern s8 dds3PadBitTable[32];
extern s32 dpy;
extern s32 curbase;
extern s32 curpoint;
extern s32 dds3DebugBasic(void *task);

#define EVT_SCENE_DISP_SURFACE 94 /* the debug panels' draw surface */
#define EVT_SCENE_DISP_LINE_Y(line) (((line) - dpy) * 96 + 0x7900)
#define EVT_SCENE_DISP_TEXT_COLOR 0xFEFFFF
#define EVT_SCENE_DISP_NODE_COLUMNS 4
#define EVT_SCENE_DISP_PAD_REPEAT 2 /* a button's state bit for a press or its repeat */

/* Debug scene display: one line per world object kind of the active scene,
 * its label, node count and nodes' keys, four keys to a line (or NO SCENE
 * without a scene), over a console panel, with the selected kind or node
 * highlighted. The pad moves the selection: up and down between kinds (by
 * four within a kind's nodes), right and left between nodes; confirm enters a
 * kind's nodes, then opens the selected node in dds3DebugBasic (returned as
 * the next handler); cancel leaves the nodes, else ends the task (-1). With
 * nothing selected it resets the selection and returns 0. Upstream:
 * INCLUDE_ASM in game/code_0022AB58 (DDS1) and game/code_00245810 (DDS2),
 * whose routines are the same; written from the retail routine. Kept from
 * retail: stepping between kinds scrolls by the kind's wrapped lines (rows),
 * or one line when it has none, so past a kind that wraps the scroll falls a
 * line short. */

s32 evtSceneDispProcess(void *task) {
    NodeDiagnosticRow table[17] = {
        { "AREA", 2 },      { "MOVER", 3 },   { "CAMERA_O", 4 }, { "UNIT_O", 5 },
        { "MATTER_O", 6 },  { "EFFECT_O", 7 }, { "EVENT_O", 8 },  { "LIGHT_O", 9 },
        { "STATIC_M", 10 }, { "MODEL_R", 11 }, { "EFFECT", 12 },  { "EVENT", 13 },
        { "FOG", 14 },      { "LIGHT", 15 },   { "PATH", 16 },    { "POINT", 17 },
        { "", 0 },
    };
    NodeDiagnosticRow *entry;
    dds3Basic_t *world;
    dds3Basic_t *node;
    EvtWorldTable *data;
    sdfDrawTag_t *list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTag_t *background;
    SdfPoolNode *surface;
    s32 kindCount;
    s32 selected = 0;
    s32 line;
    s32 style;
    s32 column;
    s32 textX;

    sdfDrawTagInit(list);
    world = dds3GetActiveScene();
    if (world == NULL) {
        kindCount = 0;
        data = NULL;
        line = 3;
        sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7180, EVT_SCENE_DISP_LINE_Y(4), EVT_SCENE_DISP_TEXT_COLOR, 0,
                                               "NO SCENE"));
    } else {
        line = 2;
        kindCount = 0;
        data = (EvtWorldTable *)world->data;
        for (entry = table; entry->kind != 0; entry++) {
            style = 0;
            entry->count = 0;
            entry->rows = 0;
            if (curbase == kindCount) {
                if (curpoint == 0) {
                    style = 4;
                    selected = 1;
                } else {
                    style = 5;
                }
            }
            column = 0;
            sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7180, EVT_SCENE_DISP_LINE_Y(line),
                                                   EVT_SCENE_DISP_TEXT_COLOR, style, entry->label));
            sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7900, EVT_SCENE_DISP_LINE_Y(line),
                                                   EVT_SCENE_DISP_TEXT_COLOR, style, "[%2d]",
                                                   data->slots[entry->kind].count));
            node = data->slots[entry->kind].head;
            if (node != NULL) {
                textX = 0x7CC0;
                do {
                    style = 14;
                    if (curbase == kindCount && entry->count == curpoint - 1) {
                        style = 4;
                        selected = 1;
                    }
                    column++;
                    sdfDrawTagLink(list, (u32)sdfDevPrintf(textX, EVT_SCENE_DISP_LINE_Y(line),
                                                           EVT_SCENE_DISP_TEXT_COLOR, style, "%05X,",
                                                           node->key));
                    textX += 0x480;
                    if (column >= EVT_SCENE_DISP_NODE_COLUMNS) {
                        textX = 0x7CC0;
                        column = 0;
                        line++;
                        entry->rows++;
                    }
                    node = node->next;
                    entry->count++;
                } while (node != NULL);
            }
            if (entry->count > 0 && ((u16)entry->count & 3) == 0) {
                line--;
                entry->rows--;
            }
            line++;
            kindCount++;
        }
    }

    background = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(background);
    dds3ConsPanelTagLink(background, 24, (2 - dpy) * 12, 39, line);
    surface = &sdfGrOtTable[EVT_SCENE_DISP_SURFACE];
    surface->append(surface, background);
    surface->append(surface, list);
    if (!selected) {
        curbase = 0;
        curpoint = 0;
        return 0;
    }

    if (dds3PadBitTable[0x17] & EVT_SCENE_DISP_PAD_REPEAT) {
        if (curpoint == 0) {
            if (curbase < kindCount - 1) {
                s16 rows = table[curbase].rows;
                s32 scroll = dpy + rows;

                if (rows == 0) {
                    scroll = dpy + 1;
                }
                dpy = scroll;
                curbase++;
            }
        } else {
            s32 next = curpoint + 4;

            if (table[curbase].count >= next) {
                curpoint = next;
            }
        }
    }
    if (dds3PadBitTable[0x16] & EVT_SCENE_DISP_PAD_REPEAT) {
        if (curpoint != 0) {
            s32 next = curpoint - 4;

            if (next > 0) {
                curpoint = next;
            }
        } else if (curbase > 0) {
            s16 rows;
            s32 scroll;

            curbase--;
            rows = table[curbase].rows;
            scroll = dpy - rows;
            if (rows == 0) {
                scroll--;
            }
            dpy = scroll;
        }
    }
    if (dds3PadBitTable[0x15] & EVT_SCENE_DISP_PAD_REPEAT) {
        if (curpoint > 0) {
            s32 next = curpoint + 1;

            if (table[curbase].count >= next) {
                curpoint = next;
            }
        }
    }
    if (dds3PadBitTable[0x14] & EVT_SCENE_DISP_PAD_REPEAT) {
        if (curpoint > 0) {
            s32 next = curpoint - 1;

            if (next > 0) {
                curpoint = next;
            }
        }
    }
    if (dds3PadBitTable[0x11] < 0 && world != NULL) {
        if (curpoint == 0) {
            if (table[curbase].count > 0) {
                curpoint = 1;
            }
        } else {
            s32 skip = curpoint - 1;
            s32 i;

            node = data->slots[table[curbase].kind].head;
            for (i = 0; i < skip; i++) {
                node = node->next;
            }
            dds3SetProcessData(task, (u32)node);
            return (s32)dds3DebugBasic;
        }
    }
    if (dds3PadBitTable[0x13] < 0) {
        if (curpoint != 0) {
            curpoint = 0;
            return 0;
        }
        return -1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "game/code_0022AB58", evtSceneDispProcess);
#endif

void func_0022B618(void) {
    dpy = 0;
}

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBDE0);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBDEC);

INCLUDE_SDATA(const s32, "game/code_0022AB58", dpy);

INCLUDE_SDATA(const s32, "game/code_0022AB58", curbase);

INCLUDE_SDATA(const s32, "game/code_0022AB58", curpoint);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBE00);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBE08);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBE10);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBE18);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBE20);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBE28);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBE30);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBE38);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBE40);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBE48);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBE50);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBE58);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBE60);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBE68);

INCLUDE_SDATA(const s32, "game/code_0022AB58", D_003BBE70);

