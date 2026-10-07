#include "common.h"
#include "sdf.h"
#include "sdf_sif_command.h"

#define SOLAR_FADE_DRAW_ENABLED 1
#define SOLAR_FADE_IN 2
#define SOLAR_FADE_OUT 4
#define SOLAR_FADE_IN_STATE (SOLAR_FADE_DRAW_ENABLED | SOLAR_FADE_IN)
#define SOLAR_FADE_OUT_STATE (SOLAR_FADE_DRAW_ENABLED | SOLAR_FADE_OUT)

extern s8 evtSolarOverlayFadeFlags;

extern u32 D_003BBDF0;

void evtInitializeVisualData(s32 visualAddress);

void evtBeginSolarOverlayFadeIn(s32 fadeDuration);

s32 func_0022AB90(void);
s32 evtGetMirroredSolarPhase(void);
void evtUpdateSolarPhaseTransition(u32 overlayAddress);
void evtDrawFadingSolarOverlayFrame(s32 x, s32 y, s32 z, s32 alpha, s32 mirroredPhase, u32 overlayAddress, s32 renderContext);

extern s32 evtSolarOverlayFadeCounter;

extern s32 evtSolarOverlayFadeDuration;
extern u32 D_003BA8EC;
extern char D_003ACD18[]; /* "EventTest" */
void kwlnTaskCreate(void *name, s32 priority, s32 unk2, s32 unk3, void *update, void *destroy, void *data);
void kwlnTaskDestroyWithHierarchyByName(void *name, s32 flag);
void evtDestroySecondaryWorldNode(void);
s32 evtGetSolarPhase(s32 object);

/* Same visual-value layout as the sequel: two 8-halfword tables and phase. */
typedef struct EventVisualData {
    u8 pad00[0x3C];
    s16 firstValues[8];
    u8 pad4C[0x50];
    s16 secondValues[8];
    u8 padAC[0x50];
    u8 solarPhase;
} EventVisualData;

typedef struct EventListNode {
    u16 orderKey;
    u8 pad02[0x2E];
    struct EventListNode *next; /* 0x30 */
    struct EventListNode *prev; /* 0x34 */
} EventListNode;

typedef struct {
    u8 pad00[0x50];
    s32 count;           /* 0x50 */
    EventListNode *first; /* 0x54 */
    EventListNode *last;  /* 0x58 */
} EventList;

INCLUDE_ASM(const s32, "game/code_0022A248", evtDrawFadingSolarOverlayFrame);

/* Seed both visual-value tables and cache the current solar phase. */
void evtInitializeVisualData(s32 visualAddress) {
    s32 solarPhase = evtGetSolarPhase(visualAddress);
    s16 *visualValues = ((EventVisualData *)visualAddress)->firstValues;
    ((EventVisualData *)visualAddress)->solarPhase = solarPhase;
    visualValues[0] = 0x39;
    visualValues[1] = 0x33;
    visualValues[2] = 0x1D;
    visualValues[3] = 0x23;
    visualValues[5] = 5;
    visualValues[7] = 10;
    visualValues = ((EventVisualData *)visualAddress)->secondValues;
    visualValues[0] = 0x37;
    visualValues[1] = 0x32;
    visualValues[2] = 15;
    visualValues[3] = 15;
    visualValues[5] = 10;
    visualValues[7] = 0;
}

INCLUDE_ASM(const s32, "game/code_0022A248", evtUpdateSolarPhaseTransition);

/* Advance the fade and draw with scaled alpha; renderContext is forwarded unchanged. */
void evtAdvanceSolarOverlayFadeAndDraw(s32 x, s32 y, s32 z, s32 alpha, u32 overlayAddress, s32 renderContext) {
    s32 mirroredPhase;
    /* Preserve signed-byte narrowing before forwarding the mirrored phase. */
    mirroredPhase = (s8)evtGetMirroredSolarPhase();
    evtUpdateSolarPhaseTransition(overlayAddress);
    if ((evtSolarOverlayFadeFlags & SOLAR_FADE_DRAW_ENABLED) != 0) {
        if ((evtSolarOverlayFadeFlags & SOLAR_FADE_IN) != 0) {
            if (evtSolarOverlayFadeCounter < evtSolarOverlayFadeDuration) {
                evtSolarOverlayFadeCounter++;
            } else {
                evtSolarOverlayFadeFlags = 0;
            }
        } else if ((evtSolarOverlayFadeFlags & SOLAR_FADE_OUT) != 0) {
            if (evtSolarOverlayFadeCounter > 0) {
                evtSolarOverlayFadeCounter--;
            } else {
                /* Clears only bit 0 while testing bit 2: the original quirk, kept verbatim. */
                evtSolarOverlayFadeFlags &= ~SOLAR_FADE_DRAW_ENABLED;
            }
        }
        if ((evtSolarOverlayFadeFlags & SOLAR_FADE_DRAW_ENABLED) != 0) {
            f32 fadeRatio = (f32)evtSolarOverlayFadeCounter / (f32)evtSolarOverlayFadeDuration;
            s32 fadedAlpha = (s32)((f32)alpha * fadeRatio);
            evtDrawFadingSolarOverlayFrame(x, y, z, fadedAlpha, mirroredPhase, overlayAddress, renderContext);
            return;
        }
    }
    if ((evtSolarOverlayFadeFlags & SOLAR_FADE_OUT) == 0) {
        evtDrawFadingSolarOverlayFrame(x, y, z, alpha, mirroredPhase, overlayAddress, renderContext);
    }
}

/* The whole flags byte is tested: a latched fade-out flag still reports state. */
s32 evtHasSolarOverlayTransitionState(void) {
    return evtSolarOverlayFadeFlags != 0;
}

/* Zero cancels the transition; otherwise start from zero for the supplied duration. */
void evtBeginSolarOverlayFadeIn(s32 fadeDuration) {
    if (fadeDuration == 0) {
        evtSolarOverlayFadeFlags = 0;
        evtSolarOverlayFadeCounter = 0;
        evtSolarOverlayFadeDuration = 0;
        return;
    }
    evtSolarOverlayFadeDuration = (s32)fadeDuration;
    evtSolarOverlayFadeFlags = SOLAR_FADE_IN_STATE;
    evtSolarOverlayFadeCounter = 0;
}

/* Zero requests an immediate hidden state, retaining the fade-out direction flag. */
void evtBeginSolarOverlayFadeOut(s32 fadeDuration) {
    if (fadeDuration == 0) {
        evtSolarOverlayFadeFlags = SOLAR_FADE_OUT_STATE;
        evtSolarOverlayFadeDuration = 1;
        evtSolarOverlayFadeCounter = 0;
    } else {
        evtSolarOverlayFadeCounter = fadeDuration;
        evtSolarOverlayFadeFlags = SOLAR_FADE_OUT_STATE;
        evtSolarOverlayFadeDuration = fadeDuration;
    }
}

void func_0022AB58(void) {
}

u32 func_0022AB60(void) {
    func_0022AB58();
    return 0;
}

/* Return the same update callback installed by evtStartTestTask. */
void *evtGetTestTaskUpdateCallback(void) {
    return (void *)func_0022AB90;
}

extern s32 sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(SdfListHead *);
extern void sdfAppendPacket(SdfListHead *, u32);
extern void kwlnDrawSpriteCell(u32, s32, s32, s32, s32);
extern s32 func_003014F0(char *, const char *, ...);
extern s32 sdfPathExists(char *);
extern s32 scrCreateProcessTaskFromResource(s32, const char *, s32);
extern void mdlFlagClearAll(void);
extern SdfPoolNode D_00325708;
extern u8 D_00324510[2][2][16];
extern s32 D_003BBDEC;
extern const char D_003ACC68[];
extern const char D_003ACC78[];
extern const char D_003ACC88[];

s32 func_0022AB90(void) {
    char path[0x40];
    SdfListHead *list;

    list = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    kwlnDrawSpriteCell((u32)list, 0x84, 0x54, 0x15, 10);
    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(0x7840, 0x7BA0, 0xFEFFFF, 0, D_003ACC68));
    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(0x7A80, 0x7C60, 0xFEFFFF, 6, D_003ACC78, D_003BBDEC));
    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(0x7900, 0x7D20, 0xFEFFFF, 0, D_003ACC88));
    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(0x7900, 0x7D80, 0xFEFFFF, 0, "RR   = ENTER"));
    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(0x7900, 0x7DE0, 0xFEFFFF, 0, "RU   = SET E500"));
    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(0x7900, 0x7E40, 0xFEFFFF, 0, "RL   = SET E600"));
    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(0x7900, 0x7EA0, 0xFEFFFF, 0, "RD   = RESET FLAG"));
    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(0x7900, 0x7F00, 0xFEFFFF, 0, "L1R1 = +-100"));
    D_00325708.append((SdfListHead *)&D_00325708, list);

    if ((s8)D_00324510[1][0][1] < 0) {
        func_003014F0(path, "/event/e%03d/e%03d/scr/e%03d.bf",
                      D_003BBDEC - D_003BBDEC % 10, D_003BBDEC, D_003BBDEC);
        if (sdfPathExists(path) != 0) {
            D_003BA8EC = 0x80000000;
            scrCreateProcessTaskFromResource(0x3EB, path, 0);
            return (s32)func_0022AB60;
        }
    }
    if ((s8)D_00324510[1][0][3] < 0) {
        mdlFlagClearAll();
        return (s32)evtGetTestTaskUpdateCallback;
    }
    if (D_00324510[1][0][5] & 2) {
        D_003BBDEC++;
    }
    if ((D_00324510[1][0][4] & 2) && D_003BBDEC > 500) {
        D_003BBDEC--;
    }
    if ((s8)D_00324510[1][0][2] < 0) {
        D_003BBDEC = 500;
    }
    if ((s8)D_00324510[1][0][0] < 0) {
        D_003BBDEC = 600;
    }
    if ((D_00324510[1][0][8] & 2) && D_003BBDEC > 500) {
        D_003BBDEC -= 100;
        if (D_003BBDEC < 500) {
            D_003BBDEC = 500;
        }
    }
    if (D_00324510[1][0][10] & 2) {
        D_003BBDEC += 100;
    }
    return 0;
}


/* Test-task teardown destroys the named script processes. */
void evtDestroyTestTaskScripts(void) {
    scrDestroyAllNamedProcesses();
}

void evtStartTestTask(void) {
    D_003BA8EC = 0x80000000;
    kwlnTaskCreate(D_003ACD18, 0x2AF9, 1, 1, func_0022AB90, evtDestroyTestTaskScripts, 0);
}

INCLUDE_RODATA(const s32, "game/code_0022A248", D_003ACD18);

void evtStopTestTasks(void) {
    kwlnTaskDestroyWithHierarchyByName(D_003ACD18, 1);
    kwlnTaskDestroyWithHierarchyByName("PolygonMovie", 0);
    evtDestroySecondaryWorldNode();
}

INCLUDE_RODATA(const s32, "game/code_0022A248", D_003ACD38);

INCLUDE_RODATA(const s32, "game/code_0022A248", D_003ACD48);

INCLUDE_RODATA(const s32, "game/code_0022A248", D_003ACD58);

INCLUDE_RODATA(const s32, "game/code_0022A248", D_003ACD68);

INCLUDE_RODATA(const s32, "game/code_0022A248", D_003ACD78);

INCLUDE_ASM(const s32, "game/code_0022A248", func_0022AF50);

void func_0022B618(void) {
    D_003BBDF0 = 0;
}

/* Insert after existing equal keys, keeping ascending order and incrementing count. */
void evtInsertListNodeByOrderKey(EventList *list, EventListNode *insertedNode) {
    EventListNode *cursor = list->first;

    if (cursor == 0) {
        list->first = insertedNode;
        list->last = insertedNode;
        insertedNode->next = 0;
        insertedNode->prev = 0;
    } else {
        while (cursor != 0) {
            if (insertedNode->orderKey < cursor->orderKey) {
                if (cursor->prev == 0) {
                    list->first = insertedNode;
                    cursor->prev = insertedNode;
                    insertedNode->next = cursor;
                    insertedNode->prev = 0;
                } else {
                    cursor->prev->next = insertedNode;
                    insertedNode->prev = cursor->prev;
                    insertedNode->next = cursor;
                    cursor->prev = insertedNode;
                }
                break;
            }
            cursor = cursor->next;
        }
        if (cursor == 0) {
            EventListNode *tailNode = list->last;
            tailNode->next = insertedNode;
            insertedNode->prev = list->last;
            insertedNode->next = 0;
            list->last = insertedNode;
        }
    }
    list->count++;
}

/* Unlink an attached node, clear its links, and decrement the owning list's count. */
void evtUnlinkListNode(EventList *list, EventListNode *removedNode) {
    EventListNode *nextNode = removedNode->next;
    EventListNode *previousNode = removedNode->prev;
    if (previousNode == 0) {
        list->first = nextNode;
    } else {
        previousNode->next = nextNode;
    }
    {
        EventListNode *previousNeighbor = removedNode->prev;
        EventListNode *nextNeighbor = removedNode->next;
        if (nextNeighbor == 0) {
            list->last = previousNeighbor;
        } else {
            nextNeighbor->prev = previousNeighbor;
        }
    }
    {
        s32 nodeCount = list->count;
        removedNode->prev = 0;
        removedNode->next = 0;
        list->count = nodeCount - 1;
    }
}

/* Walk the linked list and reinsert the first out-of-order successor. */
void evtReorderListNodes(EventList *list) {
    if (list != 0) {
        EventListNode *anchorNode = list->first;
        while (anchorNode != 0) {
            EventListNode *resumeNode = anchorNode->next;
            EventListNode *candidateNode = resumeNode;
            while (candidateNode != 0) {
                if (candidateNode->orderKey < anchorNode->orderKey) {
                    evtUnlinkListNode(list, candidateNode);
                    evtInsertListNodeByOrderKey(list, candidateNode);
                    /* Resume from the relocated node's new successor, not its old one. */
                    resumeNode = candidateNode->next;
                    break;
                }
                candidateNode = candidateNode->next;
            }
            anchorNode = resumeNode;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0022A248", func_0022B7A0);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBDEC);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBDF0);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBDF4);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBDF8);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE00);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE08);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE10);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE18);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE20);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE28);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE30);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE38);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE40);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE48);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE50);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE58);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE60);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE68);

INCLUDE_SDATA(const s32, "game/code_0022A248", D_003BBE70);

