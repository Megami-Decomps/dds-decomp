#include "common.h"
#include "evt_viewer.h"
#include "sdf.h"
#include "sdf_sif_command.h"
#include "evt_solar.h"
#include "eff.h"
#include "eff_blur.h"
#include "eff_event_draw.h"
#include "kwln_task_lifecycle.h"

#define SOLAR_FADE_DRAW_ENABLED 1
#define SOLAR_FADE_IN 2
#define SOLAR_FADE_OUT 4
#define SOLAR_FADE_IN_STATE (SOLAR_FADE_DRAW_ENABLED | SOLAR_FADE_IN)
#define SOLAR_FADE_OUT_STATE (SOLAR_FADE_DRAW_ENABLED | SOLAR_FADE_OUT)

extern s8 evtSolarOverlayFadeFlags;

extern u32 D_003BBDF0;


void evtBeginSolarOverlayFadeIn(s32 fadeDuration);

s32 func_0022AB90(void);

extern s32 evtSolarOverlayFadeCounter;

extern s32 evtSolarOverlayFadeDuration;
extern u32 D_003BA8EC;
extern char D_003ACD18[]; /* "EventTest" */
void kwlnTaskCreate(void *name, s32 priority, s32 unk2, s32 unk3, void *update, void *destroy, void *data);
void evtDestroySecondaryWorldNode(void);





INCLUDE_ASM(const s32, "game/code_0022A248", evtDrawFadingSolarOverlayFrame);

/* Seed both noise-state geometries and cache the current solar phase. */
void evtInitializeVisualData(SolarOverlayWork *overlay) {
    u8 solarPhase = evtGetSolarPhase();
    SolarNoiseState *noise = &overlay->state.firstNoise;
    overlay->state.solarPhase = solarPhase;
    noise->centerX = 0x39;
    noise->centerY = 0x33;
    noise->radiusX = 0x1D;
    noise->radiusY = 0x23;
    noise->spawnInterval = 5;
    noise->unk0E = 10;
    noise = &overlay->state.secondNoise;
    noise->centerX = 0x37;
    noise->centerY = 0x32;
    noise->radiusX = 15;
    noise->radiusY = 15;
    noise->spawnInterval = 10;
    noise->unk0E = 0;
}

INCLUDE_ASM(const s32, "game/code_0022A248", evtUpdateSolarPhaseTransition);

/* Advance the fade and draw with scaled alpha; renderContext is forwarded unchanged. */
void evtAdvanceSolarOverlayFadeAndDraw(s32 x, s32 y, s32 z, s32 alpha, SolarOverlayWork *overlay, s32 renderContext) {
    s32 mirroredPhase;
    /* Preserve signed-byte narrowing before forwarding the mirrored phase. */
    mirroredPhase = (s8)evtGetMirroredSolarPhase();
    evtUpdateSolarPhaseTransition(overlay);
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
            evtDrawFadingSolarOverlayFrame(x, y, z, fadedAlpha, mirroredPhase, overlay, renderContext);
            return;
        }
    }
    if ((evtSolarOverlayFadeFlags & SOLAR_FADE_OUT) == 0) {
        evtDrawFadingSolarOverlayFrame(x, y, z, alpha, mirroredPhase, overlay, renderContext);
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
void evtInsertListNodeByOrderKey(EvtRuntimeGroup *list, EvtRuntimeChild *insertedNode) {
    EvtRuntimeChild *cursor = list->children;

    if (cursor == 0) {
        list->children = insertedNode;
        list->lastChild = insertedNode;
        insertedNode->next = 0;
        insertedNode->prev = 0;
    } else {
        while (cursor != 0) {
            if (insertedNode->frame < cursor->frame) {
                if (cursor->prev == 0) {
                    list->children = insertedNode;
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
            EvtRuntimeChild *tailNode = list->lastChild;
            tailNode->next = insertedNode;
            insertedNode->prev = list->lastChild;
            insertedNode->next = 0;
            list->lastChild = insertedNode;
        }
    }
    list->childCount++;
}

/* Unlink an attached node, clear its links, and decrement the owning list's count. */
void evtUnlinkListNode(EvtRuntimeGroup *list, EvtRuntimeChild *removedNode) {
    EvtRuntimeChild *nextNode = removedNode->next;
    EvtRuntimeChild *previousNode = removedNode->prev;
    if (previousNode == 0) {
        list->children = nextNode;
    } else {
        previousNode->next = nextNode;
    }
    {
        EvtRuntimeChild *previousNeighbor = removedNode->prev;
        EvtRuntimeChild *nextNeighbor = removedNode->next;
        if (nextNeighbor == 0) {
            list->lastChild = previousNeighbor;
        } else {
            nextNeighbor->prev = previousNeighbor;
        }
    }
    {
        s32 nodeCount = list->childCount;
        removedNode->prev = 0;
        removedNode->next = 0;
        list->childCount = nodeCount - 1;
    }
}

/* Walk the linked list and reinsert the first out-of-order successor. */
void evtReorderListNodes(EvtRuntimeGroup *list) {
    if (list != 0) {
        EvtRuntimeChild *anchorNode = list->children;
        while (anchorNode != 0) {
            EvtRuntimeChild *resumeNode = anchorNode->next;
            EvtRuntimeChild *candidateNode = resumeNode;
            while (candidateNode != 0) {
                if (candidateNode->frame < anchorNode->frame) {
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

typedef struct CampDisplayDefaults {
    s32 x, y;
    u8 color[4];
    f32 scaleY, scaleX;
    s32 enabled, variant;
} CampDisplayDefaults;

typedef struct CampListLayout {
    s32 width0, width1, width2;
    s32 unkC, unk10, unk14;
    u8 pad18[8];
    union {
        struct { s32 unk20, unk24, unk28; };
        s32 firstValues[3];
    };
    union {
        struct { s32 unk2C, unk30, unk34; };
        s32 secondValues[3];
    };
} CampListLayout;

typedef struct EvtViewerDrawVector {
    f32 x, y, z, w;
    s32 mode;
} EvtViewerDrawVector;
typedef struct EvtViewerDrawPayload {
    f32 x, y, z, w;
    s32 mode;
    u8 unknown14[0xC];
} EvtViewerDrawPayload;

extern void *sdfAllocSizeClassBlock(s32);
extern s32 evtEventViewerAddName(const char *, EvtRuntime *);
extern void func_00242C30(EvtRuntime *, EvtRuntimeGroup *, s32, f32 (*)[4], f32 *, f32 *);
extern void func_00242E70(EvtRuntime *, EvtRuntimeGroup *, s32, CampDisplayDefaults *);
extern void func_00242F78(EvtRuntime *, EvtRuntimeGroup *, CampListLayout *);
extern EffBlurTemplate *effGetCh71Work(void);
extern EffBlurScatterWork *effGetCh72Work(void);
extern EffBlurScaleWork *effGetCh76Work(void);
extern EffResourceRectWork *effGetCh75Work(void);
extern EvtViewerDrawVector kwlnDrawVector;

/* Capture kind-specific defaults before adding a new key to its track. */
EvtRuntimeChild *func_0022B7A0(EvtRuntimeGroup *group, s32 frame, EvtRuntime *viewer) {
    CampDisplayDefaults display;
    CampListLayout layout;
    f32 first, second;
    f32 (*vectors)[4] = NULL;
    EvtRuntimeChild *key;

    switch (group->type) {
    case 10:
        vectors = sdfAllocSizeClassBlock(0x30);
        func_00242C30(viewer, group, frame, vectors, &first, &second);
        break;
    case 24:
        func_00242E70(viewer, viewer->frameGroup, viewer->curFrame, &display);
        break;
    case 25:
        func_00242F78(viewer, viewer->frameGroup, &layout);
        break;
    }
    key = sdfAllocSizeClassBlock(sizeof(*key));
    memset(key, 0, sizeof(*key));
    key->frame = frame;
    key->interpolationMode = -1;
    evtInsertListNodeByOrderKey(group, key);

    switch (group->type) {
    case 2:
        key->p08.f = 0.5235987306f;
        key->interpolationMode = evtEventViewerAddName(viewer->entryName[group->entryHeader.word], viewer);
        break;
    case 1:
        key->interpolationMode = evtEventViewerAddName(viewer->entryName[group->entryHeader.word], viewer);
        key->p08.sb[0] = 0;
        key->p0C.sb[2] = 1;
        key->p0C.sb[3] = 20;
        break;
    case 18:
        key->interpolationMode = evtEventViewerAddName(viewer->entryName[group->entryHeader.word], viewer);
        key->p08.h[0] = 0;
        key->p08.h[1] = -1;
        key->p08.sb[0] = 0;
        break;
    case 20: case 21:
        key->interpolationMode = evtEventViewerAddName(viewer->entryName[group->entryHeader.word], viewer);
        key->p08.sb[0] = -1;
        key->p08.sb[1] = 0;
        key->p08.h[1] = 0;
        key->p0C.sb[0] = -1;
        key->p0C.sb[1] = -1;
        key->p0C.sb[2] = -1;
        key->p0C.sb[3] = -1;
        break;
    case 3: case 26:
        key->interpolationMode = evtEventViewerAddName(viewer->entryName[group->entryHeader.word], viewer);
        key->p08.sb[0] = -1;
        key->p08.h[1] = 0;
        key->p0C.sb[0] = 0;
        key->p0C.sb[1] = -1;
        key->p0C.h[1] = -1;
        break;
    case 9:
        key->interpolationMode = evtEventViewerAddName(viewer->entryName[group->entryHeader.word], viewer);
        key->p08.h[0] = -1;
        key->p08.h[1] = 1;
        break;
    case 12:
        key->p08.h[0] = 1;
        key->p08.h[1] = 128;
        key->p0C.i = 0;
        break;
    case 22:
        key->p08.h[0] = 1;
        break;
    case 10:
        key->p08.f = first;
        key->payload = vectors;
        key->p0C.f = second;
        key->p10.h[0] = (u16)group->setterId;
        key->interpolationMode = 0;
        break;
    case 11: {
        EvtViewerDrawPayload *draw = sdfAllocSizeClassBlock(0x20);
        draw->x = kwlnDrawVector.y;
        draw->mode = kwlnDrawVector.mode;
        key->payload = draw;
        draw->y = kwlnDrawVector.x;
        draw->z = kwlnDrawVector.z;
        draw->w = kwlnDrawVector.w;
        key->p08.h[0] = 1;
        break;
    }
    case 13: {
        void *payload = sdfAllocSizeClassBlock(0x28);
        key->payload = payload;
        memcpy(payload, effGetLoadDescA(), 0x28);
        key->p08.h[0] = 1;
        break;
    }
    case 14: {
        void *payload = sdfAllocSizeClassBlock(0x2C);
        key->payload = payload;
        memcpy(payload, effGetCh71Work(), 0x2C);
        key->p08.h[0] = 1;
        key->p10.h[0] = 0;
        break;
    }
    case 15: {
        void *payload = sdfAllocSizeClassBlock(0x2C);
        key->payload = payload;
        memcpy(payload, effGetCh72Work(), 0x2C);
        key->p08.h[0] = 1;
        break;
    }
    case 23: {
        void *payload = sdfAllocSizeClassBlock(0x2C);
        key->payload = payload;
        memcpy(payload, effGetCh76Work(), 0x2C);
        key->p08.h[0] = 1;
        break;
    }
    case 27: {
        void *payload = sdfAllocSizeClassBlock(0x28);
        key->payload = payload;
        memcpy(payload, effGetCh73Params(), 0x28);
        key->p08.h[0] = 1;
        break;
    }
    case 16: {
        void *payload = sdfAllocSizeClassBlock(0x18);
        key->payload = payload;
        memcpy(payload, effGetCh74Params(), 0x18);
        key->p08.h[0] = 1;
        break;
    }
    case 17: {
        void *payload = sdfAllocSizeClassBlock(0x24);
        key->payload = payload;
        memcpy(payload, effGetCh75Work(), 0x24);
        key->p08.h[0] = 1;
        break;
    }
    case 24:
        key->interpolationMode = evtEventViewerAddName(viewer->entryName[group->entryHeader.word], viewer);
        key->p08.sb[0] = display.enabled;
        key->p08.sb[1] = display.variant;
        key->p0C.h[0] = display.x;
        key->p0C.h[1] = display.y;
        key->p10.sb[0] = display.color[0];
        key->p10.sb[1] = display.color[1];
        key->p10.sb[2] = display.color[2];
        key->p10.sb[3] = display.color[3];
        key->p14.f = display.scaleX;
        key->p18.f = display.scaleY;
        break;
    case 25: {
        EvtCameraColorPayload *payload = sdfAllocSizeClassBlock(0x40);
        s32 i;
        key->payload = payload;
        payload->parameters.x = layout.unk10;
        payload->parameters.w[0] = layout.width0;
        payload->parameters.w[1] = layout.width1;
        payload->parameters.w[2] = layout.width2;
        payload->parameters.w[3] = layout.unkC;
        payload->parameters.flagWord = layout.unk14;
        for (i = 0; i < 3; i++) {
            payload->parameters.y[i] = layout.firstValues[i];
            payload->parameters.z[i] = layout.secondValues[i];
        }
        break;
    }
    case 0: case 4: case 5: case 6: case 7: case 8: case 19:
    case 28: case 29: case 30: case 31: case 32:
    default:
        break;
    }
    return key;
}

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

