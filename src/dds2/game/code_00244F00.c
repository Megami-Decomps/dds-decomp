#include "common.h"
#include "evt_viewer.h"
#include "evt_solar.h"

#define SOLAR_FADE_DRAW_ENABLED 1
#define SOLAR_FADE_IN 2
#define SOLAR_FADE_OUT 4
#define SOLAR_FADE_IN_STATE (SOLAR_FADE_DRAW_ENABLED | SOLAR_FADE_IN)
#define SOLAR_FADE_OUT_STATE (SOLAR_FADE_DRAW_ENABLED | SOLAR_FADE_OUT)

extern s32 evtSolarOverlayFadeCounter;

extern s32 evtSolarOverlayFadeDuration;


extern s8 evtSolarOverlayFadeFlags;


extern char D_004221D8[]; /* "EventTest" */

extern char D_004221E8[]; /* "PolygonMovie" */

void kwlnTaskDestroyWithHierarchyByName(void *name, s32 flag);

void evtDestroySecondaryWorldNode(void);


extern u32 D_00435CBC;

void kwlnTaskCreate(void *name, s32 priority, s32 unk2, s32 unk3, void *update, void *destroy, void *data);

extern s32 D_0043722C;

INCLUDE_ASM(const s32, "game/code_00244F00", evtDrawFadingSolarOverlayFrame);

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

INCLUDE_ASM(const s32, "game/code_00244F00", evtUpdateSolarPhaseTransition);

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

void func_00245810(void) {
}

u32 func_00245818(void) {
    return 0;
}

/* Test-task teardown destroys the named script processes. */
void evtDestroyTestTaskScripts(void) {
    scrDestroyAllNamedProcesses();
}

void evtStartTestTask(void) {
    D_00435CBC = 0x80000000;
    kwlnTaskCreate(D_004221D8, 0x2AF9, 1, 1, func_00245818, evtDestroyTestTaskScripts, 0);
}

void evtStopTestTasks(void) {
    kwlnTaskDestroyWithHierarchyByName(D_004221D8, 1);
    kwlnTaskDestroyWithHierarchyByName(D_004221E8, 0);
    evtDestroySecondaryWorldNode();
}

INCLUDE_ASM(const s32, "game/code_00244F00", func_002458B8);

void func_00245F80(void) {
    D_0043722C = 0;
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

INCLUDE_ASM(const s32, "game/code_00244F00", func_00246108);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_0043722C);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437230);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437234);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437238);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437240);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437248);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437250);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437258);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437260);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437268);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437270);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437278);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437280);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437288);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437290);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437298);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_004372A0);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_004372A8);

