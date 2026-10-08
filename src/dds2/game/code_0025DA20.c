#include "common.h"
#include "evt_viewer.h"
#include "mnu.h"
#include "mnu_staff.h"
#include "mnu_shop.h"
#include "sdf.h"
#include "sdf_linked_packet.h"
#include "sdf_packet_builders.h"
#include "kwln.h"
#include "evt_world.h"
#include "evt_unit.h"
#include "dat_state.h"
#include "mnu_list.h"
#include "eff.h"
#include "fld.h"

#define CAMP_TASK_NAME_BYTES 0x20
#define CAMP_TASK_DATA_BYTES 0x48
#define CAMP_TIMELINE_WRAP_FRAME 10
#define CAMP_TRACK_FIRST_OFFSET_TYPE 0x12
#define CAMP_CAMERA_COLOR_TRACK_TYPE 0x19
#define CAMP_KEY_LOW_BITS_MASK 0xFFF
#define CAMP_KEY_HIGH_BITS_SHIFT 12
#define CAMP_ENTRY_VALUE_FOLLOW_ALT 0
#define CAMP_ENTRY_VALUE_CLEAR 1
#define CAMP_ENTRY_NAME_CODE_BASE 2
#define CAMP_FONT_CONTEXT_WIDTH 0x960
#define CAMP_FONT_CONTEXT_HEIGHT 0x70
#define CAMP_DESCRIPTOR_WIDTH 0x200
#define CAMP_DESCRIPTOR_HEIGHT 0xE0

#define CAMP_OPTION_VALUE_MASK 3
#define CAMP_PRIMARY_OPTION_CLEAR_MASK 0xfffffffc
#define CAMP_SECONDARY_OPTION_CLEAR_MASK 0xfffffff3
#define CAMP_SECONDARY_OPTION_SHIFT 2
#define CAMP_SECONDARY_OPTION_MASK 0xc
#define CAMP_TRANSFORM_COMPONENT_COUNT 4
#define CAMP_TRANSFORM_MIDDLE_START 4
#define CAMP_TRANSFORM_LAST_START 8

#define CAMP_REGISTERED_ID_LIMIT 20
#define CAMP_TIMELINE_SCRIPT_REGISTER_BASE 200
#define CAMP_TIMELINE_SCRIPT_SLOT_COUNT 10
#define CAMP_STATUS_BATCH_COUNT 2
#define CAMP_STATUS_INITIAL_PARAMETER 15
#define CAMP_SLOT_LAST_ROW_INDEX 0x14
#define CAMP_PARTY_SCAN_LAST 4
#define CAMP_PARTY_ACTIVE_FLAG 1
#define CAMP_HEAP_STATS_WORD_COUNT 8

extern void func_00101968(KwlnTask *, KwlnTask *);
extern s32 mnuPreparePopupAndDispatchSelection(s32);
extern s32 mnuAdvanceCampPopup(s32);
extern s32 mnuFinishCampPopup(s32);

extern s32 sdfAllocGeneralBlock(s32);
extern u8 *sdfResourceRetainAddress(s32);
extern void mnuInitializeShopStatusBatches(MenuTerminalContext *);
extern void func_002945B8(MenuTerminalContext *);
extern void mnuResetGradientFadeColor(MenuGradientFade *, s32);
extern void sndEnsureMidiBankResident(s32);
extern void sndStartTrackExtended(s32);
extern s32 func_00261198();
extern s32 func_002613C8();
extern s32 ptyCountBulletItem(s32);

extern s8 mnuCampListedItems[34];

extern void func_00246950();

typedef struct CampFlagRow {
    u32 messageSet;  /* 0x00: shop message resource number */
    s16 flag[8];     /* 0x04 */
    u8 value[9];     /* 0x14: [0] default, [i + 1] for flag[i] */
    u8 pad1D[3];
} CampFlagRow;

extern CampFlagRow D_003C9A40[];

extern s32 mdlFlagTest(u32);


extern u8 D_003CE658[];

extern SdfTex *kwlnHeldTextureReference;


extern s32 sdfAllocatePacketList();

extern void sdfCreateDescriptorPacket();

extern s32 evtQueueValidatedBgmSoundCode(s32, s32);

extern s32 strcmp(const char *a, const char *b);

extern s32 func_0019CE78(s32 *, s32, s32, s32, s32);

extern void frFontSetContextPair(s32, s32, s32);

extern s32 D_003C99B8[];

extern void evtViewerCleanupMessageWindow();

extern void evtViewerDispatchFlagMode();

extern s64 evtFindTaskById(void);

extern s32 func_00101820(u32);


extern s32 func_00261B98(s32);


extern char D_00437838[]; /* "camp" */

extern char D_00424BC0[]; /* "camp_draw" */

extern char D_00424BD0[]; /* "camp_update" */

extern s8 mnuPanelTaskCompletionState;


extern u32 kwlnTaskGetUserValue();

extern void mnuShopReleaseWindowSprites();

extern void sdfReleaseChipBlock();


extern void mnuShopReleaseWindowAndEffectResources();

extern void effDestroyResourceSlotSet();

extern s32 mnuShopReleaseSceneObjects(MenuTerminalContext *);

extern s32 dspCloseChannel(void);

extern void evtReleaseResourcePairHandle();

extern void sdfReleaseResourceAllocation();

extern void mnuDrawAndStepGradientFade(MenuGradientFade *, s32);

extern void func_002C1B68(u32 *, u32);

extern s32 evtGetMessageWindowControlState(void);

extern u8 D_003CD8D0[];

extern s32 effMiscRand(s32);

extern u8 D_003CDA8C[];

extern u8 mnuCampCompactEntries[];

extern s32 evtGetMirroredSolarPhase(void);

extern u8 D_003CD8DD[];

extern void evtFormatTaskName(s32 taskId, void *name);

extern void *sdfAllocSizeClassBlock(s32 size);

extern void *memset(void *dst, s32 c, u32 n);

extern KwlnTask *kwlnTaskCreate();


extern s32 func_002C54B0(s32);
extern s32 mnuIsBulletItemId(s32);
extern s32 func_002C5498(s32);
extern u8 *datItemSkillRecords;

extern u8 D_003CDA88[];

extern s32 itfDrawBankTextWithLayoutFlags(s32, s32, u64, u64, u64, u64);

extern void frFontSetChildColors(s32, u32);

extern s32 func_0019D550(s32, s32, u32);

extern f32 mnuShopSavedLastTransformVector[];
extern f32 mnuShopSavedMiddleTransformVector[];
extern f32 mnuShopSavedFirstTransformVector[];
extern s32 mnuShopRestoreMiddleVector;
extern EffMappedResource *effCreateStatusBatch(s32 kind);
extern s32 sdfAllocPacketAligned(s32 size);
extern void sdfInitPacketList(SdfListHead *packet);
extern void itfSendTablePacket(SdfListHead *packet, s32 table, s32 mode);
extern void itfQueueTextureBoundQuadPacket(void *, void *, void *, s32, SdfTex *, s32, SdfListHead *);
extern void evtSetDrawSurfaceIndex(u32);
extern void evtSubmitPrimaryAlphaBlendMode(s32);
extern void evtSubmitPrimaryGsTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern s32 D_003C9988[4];
extern s32 D_003C9998[4];
extern s32 D_003C99A8[4];
extern SdfPoolNode kwlnDrawSurfaces[];
extern s32 effDestroyPackedBatch(s32);


extern ShopRankPriceRow D_003CBB70[];

/* Schedule the camp task only if no task currently owns this event ID. */
void mnuCampCreateTask(s32 taskId) {
    char taskName[CAMP_TASK_NAME_BYTES];
    EvtPackLoadState *taskData;

    if (evtFindTaskById() == 0) {
        evtFormatTaskName(taskId, taskName);
        taskData = sdfAllocSizeClassBlock(CAMP_TASK_DATA_BYTES);
        memset(taskData, 0, CAMP_TASK_DATA_BYTES);
        taskData->eventId = taskId;
        taskData->loaded = 0;
        kwlnTaskCreate(taskName, CAMP_TASK_PRIORITY, 1, 1, evtTickPackLoad, evtReleaseEventPackResources, taskData);
    }
}

void mnuCampDestroyTaskById(void) {
    s64 taskHandle;

    taskHandle = evtFindTaskById();
    if (taskHandle != 0) {
        kwlnTaskDestroyWithHierarchy(taskHandle, 0);
        return;
    }
}

/* Drain every camp task at the scheduler priority used during creation. */
void mnuCampDestroyAllTasks(void) {
    s64 taskHandle;

    while (taskHandle = func_00101820(CAMP_TASK_PRIORITY), taskHandle != 0) {
        kwlnTaskDestroyWithHierarchy(taskHandle, 0);
    }
}


/* Timeline keys carry type-dependent payloads as well as their common links. */


/* The same track owns its key list, name/value state and next-track link. */




/* Event-viewer timeline data and the camp/shop state that owns those tracks. */


extern void fldApplyCameraColorKeyWords(EvtRuntime *scene, const EvtBlendKey *source);

/* Shift the selected track's frames; values exactly at the upper limit are kept. */
void mnuShopScrollList(EvtRuntime *scene, s32 delta) {
    EvtRuntimeGroup *track = scene->frameGroup;
    EvtRuntimeChild *key;
    s32 absoluteFrame;

    if (track == NULL) {
        return;
    }
    for (key = track->children; key != NULL; key = key->next) {
        absoluteFrame = key->frame + track->metadataValue + delta;
        if (absoluteFrame < track->metadataValue) {
            key->frame = 0;
        } else if (scene->headerThird < absoluteFrame) {
            key->frame = (u16)scene->headerThird - (u16)track->metadataValue - 1;
        } else {
            key->frame = key->frame + delta;
        }
    }
}

/* Shift qualifying keys and their supported payload offsets. Preserve the
 * incoming base/offset/ubase arguments when no key overwrites them: the native
 * cleanup call receives those values even after an empty track traversal. */
void mnuFxWorldScrollDelta(EvtRuntime *scene, s32 delta, s32 threshold, s32 base, s32 offset, s32 ubase) {
    EvtRuntimeGroup *track;
    EvtRuntimeChild *key;
    s32 absoluteFrame;

    if (scene->entryCount <= 0) {
        return;
    }
    if (scene->frameRange.word + delta < 0) {
        scene->frameRange.word = CAMP_TIMELINE_WRAP_FRAME;
    } else {
        scene->frameRange.word += delta;
    }
    if (scene->headerThird + delta < 0) {
        scene->headerThird = CAMP_TIMELINE_WRAP_FRAME;
    } else {
        scene->headerThird += delta;
    }
    if (scene->curFrame > scene->headerThird) {
        scene->curFrame = scene->headerThird;
    }
    track = scene->groups;
    while (track != NULL) {
        for (key = track->children; key != NULL; key = key->next) {
            offset = key->frame;
            base = track->metadataValue;
            ubase = (u16)track->metadataValue;
            absoluteFrame = offset + base;
            if (absoluteFrame < threshold) {
                continue;
            }
            absoluteFrame += delta;
            if (absoluteFrame < base) {
                key->frame = 0;
            } else if (scene->headerThird < absoluteFrame) {
                key->frame = (u16)scene->headerThird - ubase - 1;
            } else {
                key->frame = offset + delta;
            }
            /* Payload bounds test the updated offset plus delta a second time. */
            switch (track->type) {
            case CAMP_TRACK_FIRST_OFFSET_TYPE:
                if (key->p08.sh[0] != 0) {
                    key->p08.sh[0] += delta;
                    if (key->p08.sh[0] < 0) {
                        key->p08.sh[0] = 0;
                    }
                    if (scene->headerThird < key->p08.sh[0] + delta) {
                        key->p08.sh[0] = scene->headerThird - 1;
                    }
                }
                break;
            case 3:
            case 0x14:
            case 0x15:
            case 0x1A:
                if (key->p08.sh[1] != 0) {
                    key->p08.sh[1] += delta;
                    if (key->p08.sh[1] < 0) {
                        key->p08.sh[1] = 0;
                    }
                    if (scene->headerThird < key->p08.sh[1] + delta) {
                        key->p08.sh[1] = scene->headerThird - 1;
                    }
                }
                break;
            }
        }
        track = track->next;
    }
    scene->previousGlyphPosition -= 1;
    evtViewerCleanupMessageWindow(scene, delta, threshold, base, offset, ubase, track);
    evtViewerDispatchFlagMode(scene);
}

/* Process keys at or beyond threshold, restarting at the head after each call. */
void mnuFxWorldDropOutOfRange(EvtRuntime *scene, s32 threshold) {
    EvtRuntimeGroup *track;
    EvtRuntimeChild *key;

    if (scene->entryCount <= 0) {
        return;
    }
    for (track = scene->groups; track != NULL; track = track->next) {
        key = track->children;
        while (key != NULL) {
            if (key->frame + track->metadataValue < threshold) {
                key = key->next;
            } else {
                func_00246950(scene, track, key);
                key = track->children;
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025DE08);

/* Default three-vector slot contents; the trailing two scalars have unknown roles. */
void mnuInitializeCampPanelVisualDefaults(f32 *firstVector, f32 *secondVector, f32 *thirdVector, f32 *scalarA, f32 *scalarB) {
    firstVector[0] = 0.7f;
    firstVector[1] = 0.7f;
    firstVector[2] = 0.7f;
    firstVector[3] = 0.0f;
    secondVector[0] = 0.65f;
    secondVector[1] = 0.39f;
    secondVector[2] = 0.65f;
    secondVector[3] = 0.0f;
    thirdVector[0] = 0.2f;
    thirdVector[1] = 0.2f;
    thirdVector[2] = 0.2f;
    thirdVector[3] = 1.0f;
    *scalarA = 7.0f;
    *scalarB = 0.0f;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E048);

typedef struct CampDisplayDefaults {
    s32 x;
    s32 y;
    u8 color[4];
    f32 scaleY;
    f32 scaleX;
    s32 enabled;
    s32 variant;
    s32 unk1C;
} CampDisplayDefaults;

void mnuCampInitializeDisplayDefaults(CampDisplayDefaults *display) {
    display->x = 0x100;
    display->y = 0xE0;
    display->color[0] = 0x80;
    display->color[1] = 0x80;
    display->color[2] = 0x80;
    display->color[3] = 0x80;
    display->scaleX = 1.0f;
    display->scaleY = 1.0f;
    display->enabled = 1;
    display->variant = 0;
    display->unk1C = 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E288);

typedef struct CampListLayout {
    s32 width0;
    s32 width1;
    s32 width2;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[8];
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
} CampListLayout;

void mnuInitializeCampListLayoutDefaults(CampListLayout *layout) {
    layout->width0 = 150;
    layout->width1 = 150;
    layout->unk10 = 80;
    layout->width2 = 150;
    layout->unkC = 30;
    layout->unk14 = 1;
    layout->unk20 = 7;
    layout->unk24 = 4;
    layout->unk28 = 10;
    layout->unk2C = 32;
    layout->unk30 = 16;
    layout->unk34 = 16;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E390);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025E460);

void mnuFindCampKeyTrackNeighbors(EvtRuntimeGroup *track, s32 value, EvtRuntimeChild **out1, EvtRuntimeChild **out2) {
    s32 base;

    *out1 = 0;
    *out2 = 0;
    if (track == 0) {
        return;
    }
    base = track->metadataValue;
    *out2 = track->children;
    while (*out2 != 0) {
        if (value < (*out2)->frame + base) {
            break;
        }
        *out2 = (*out2)->next;
    }
    if (*out2 != 0) {
        *out1 = (*out2)->prev;
    } else {
        *out1 = track->lastChild;
    }
    if (track->type == 2) {
        while (*out1 != 0 && evtViewerTestIndexedCondition((*out1)->p0C.sh[0]) != 1) {
            *out1 = (*out1)->prev;
        }
    }
}

u32 func_0025E7B0(void) {
    return 0;
}

/* Split the key's packed halfword into its low 12 bits and upper four bits. */
void mnuUnpackNibbleFields(EvtRuntimeChild *key, s32 *lowBitsOut, s32 *highBitsOut) {
    *lowBitsOut = key->p08.h[0] & CAMP_KEY_LOW_BITS_MASK;
    *highBitsOut = key->p08.h[0] >> CAMP_KEY_HIGH_BITS_SHIFT;
}

typedef struct CampNameLookup {
    u8 pad00[0x7C];
    char (*nameTable)[32]; /* 0x7C: fixed-width names indexed by nameIndex */
} CampNameLookup;

/* Return the scene's matching name index, or -1 when the track list has no match. */
s32 mnuCampFindMatchingEntryIndex(CampNameLookup *lookup, EvtRuntime *scene, s32 nameIndex) {
    EvtRuntimeGroup *track = scene->groups;
    while (track != NULL) {
        if (strcmp(scene->entryName[track->entryHeader.word],
                   lookup->nameTable[nameIndex]) == 0) {
            return track->entryHeader.word;
        }
        track = track->next;
    }
    return -1;
}

/* Return the first track with this fixed-width name, or NULL when absent. */
void *mnuCampFindEntryByName(EvtRuntime *scene, const char *name) {
    EvtRuntimeGroup *track = scene->groups;
    while (track != NULL) {
        if (strcmp(scene->entryName[track->entryHeader.word], name) == 0) {
            return track;
        }
        track = track->next;
    }
    return NULL;
}

/* Follow zero-coded alternatives, clear on code 1, otherwise resolve a name.
 * Keep the native signed code and separately retained halfword bits distinct. */
void campResolvePendingValue(EvtRuntime *scene, EvtRuntimeChild *cue) {
    EvtRuntimeChild *linkedKey;
    s32 entryCode;
    u16 entryCodeBits;

    if (cue == NULL) {
        return;
    }
    entryCode = cue->p10.sh[0];
    entryCodeBits = cue->p10.sh[0];
    if (entryCode == CAMP_ENTRY_VALUE_CLEAR) {
        scene->selectedEntry = 0;
        return;
    }
    if (entryCode == CAMP_ENTRY_VALUE_FOLLOW_ALT) {
        linkedKey = cue->prev;
        scene->selectedEntry = 0;
        for (; ; linkedKey = linkedKey->prev) {
            s32 linkedEntryCode;

            if (linkedKey == NULL) {
                return;
            }
            linkedEntryCode = linkedKey->p10.sh[0];
            if (linkedEntryCode != CAMP_ENTRY_VALUE_FOLLOW_ALT) {
                if (linkedEntryCode == CAMP_ENTRY_VALUE_CLEAR) {
                    scene->selectedEntry = 0;
                    return;
                }
                scene->selectedEntry = ((EvtRuntimeGroup *)mnuCampFindEntryByName(scene, scene->entryName[linkedEntryCode - CAMP_ENTRY_NAME_CODE_BASE]))->entryValue;
                return;
            }
        }
    } else {
        scene->selectedEntry = ((EvtRuntimeGroup *)mnuCampFindEntryByName(scene, scene->entryName[(s16)entryCodeBits - CAMP_ENTRY_NAME_CODE_BASE]))->entryValue;
    }
}

void mnuDrawCampScaledTexture(SdfTex *texture, CampDisplayDefaults *display) {
    s32 halfWidth;
    s32 halfHeight;
    s32 variant;
    SdfListHead *packet;
    s32 surfaceIndex = 0x53;
    SdfPoolNode *surface;

    D_003C9998[2] = texture->width << 4;
    D_003C9998[3] = texture->height << 4;
    halfWidth = (s32)(texture->width * display->scaleX) / 2;
    halfHeight = (s32)(texture->height * display->scaleY) / 2;
    D_003C9988[0] = (display->x - halfWidth) << 4;
    D_003C9988[1] = (display->y - halfHeight) << 3;
    D_003C9988[2] = (display->x + halfWidth) << 4;
    D_003C9988[3] = (display->y + halfHeight) << 3;
    D_003C99A8[0] = display->color[0];
    D_003C99A8[1] = display->color[1];
    D_003C99A8[2] = display->color[2];
    D_003C99A8[3] = display->color[3];
    variant = display->variant;
    if (display->color[3] != 0) {
        packet = (SdfListHead *)sdfAllocPacketAligned(0x20);
        sdfInitPacketList(packet);
        if (variant == 0) {
            itfSendTablePacket(packet, 0, 0);
        } else if (variant == 1) {
            itfSendTablePacket(packet, 1, 0);
        } else if (variant == 2) {
            itfSendTablePacket(packet, 2, 0);
        }
        if (display->unk1C != 0) {
            surfaceIndex = 0x3E;
            evtSetDrawSurfaceIndex(0x3E);
            evtSubmitPrimaryAlphaBlendMode(0);
            evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
            itfQueueTextureBoundQuadPacket(D_003C9988, D_003C9998, D_003C99A8,
                                          0xFF, texture, 0, packet);
        } else {
            itfQueueTextureBoundQuadPacket(D_003C9988, D_003C9998, D_003C99A8,
                                          0xFF, texture, 0, packet);
        }
        surface = &kwlnDrawSurfaces[surfaceIndex];
        surface->append((SdfListHead *)surface, packet);
    }
}

/* Clear each track's unknown word and the scene state; do not alter links or values. */
void fldResetCampSceneEntries(EvtRuntime *scene) {
    EvtRuntimeGroup *track;

    track = scene->groups;
    if (track != 0) {
        track->unk28 = 0;
        while (track = track->next, track != 0) {
            track->unk28 = 0;
        }
    }
    scene->colorEditorActive = 0;
}

/* Blend the first camera-color track at the clamped frame. Lookup includes
 * the track base, but interpolation retains the native unre-based key frames.
 * Without a future key, pass zero blend and the uninitialized fallback block. */
void func_0025EC00(EvtRuntime *scene) {
    EvtRuntimeGroup *track;
    EvtRuntimeChild *selectedKey;
    EvtRuntimeChild *futureKey;
    EvtBlendKey fallbackBlend;
    EvtBlendKey blendedParameters;
    EvtBlendKey *futureBlend;
    s32 frameValue;
    u16 selectedFrame;
    u16 futureFrame;
    f32 blendFraction;

    selectedKey = NULL;
    futureKey = NULL;
    frameValue = scene->curFrame;
    if (scene->colorEditorActive == 1) {
        return;
    }
    track = scene->groups;
    while (track != NULL) {
        if (track->type == CAMP_CAMERA_COLOR_TRACK_TYPE) {
            mnuFindCampKeyTrackNeighbors(track, frameValue, &selectedKey, &futureKey);
            break;
        }
        track = track->next;
    }
    if (selectedKey == NULL) {
        scene->cameraColorActive = 0;
        return;
    }

    if (futureKey == NULL) {
        blendFraction = 0.0f;
        futureBlend = &fallbackBlend;
    } else {
        selectedFrame = selectedKey->frame;
        futureFrame = futureKey->frame;
        if (futureFrame != selectedFrame) {
            blendFraction = (f32)(frameValue - selectedFrame) / (f32)(futureFrame - selectedFrame);
        } else {
            blendFraction = 0.0f;
        }
        futureBlend = &((EvtCameraColorPayload *)futureKey->payload)->parameters;
    }
    evtBlendParamsH(selectedKey->p0C.sh[0], blendFraction, &((EvtCameraColorPayload *)selectedKey->payload)->parameters, futureBlend, &blendedParameters);
    fldApplyCameraColorKeyWords(scene, &blendedParameters);
}


/* The genuine three-row typed candidate remains non-matching. */
INCLUDE_ASM(const s32, "game/code_0025DA20", fldApplyCameraColorKeyWords);

void func_0025EE00(EvtRuntime *scene) {
    func_0025EC00(scene);
    if (scene->cameraColorActive == 1) {
        func_00137888();
        return;
    }
}

void mnuCampInitFontResource(EvtRuntime *scene) {
    s32 fontHandle;
    scene->glyph = 0;
    fontHandle = func_0019CE78(D_003C99B8, 0, 0, 0, 0);
    scene->glyph = fontHandle;
    frFontSetContextPair(fontHandle, CAMP_FONT_CONTEXT_WIDTH, CAMP_FONT_CONTEXT_HEIGHT);
}

void mnuCampLinkFontGlyph(EvtRuntime *scene) {
    frFontQueueGlyphInSelectedSlot(scene->glyph);
    scene->glyph = 0;
}

extern void sdfGetGeneralHeapStats(s32 *);

/* Retail keeps only the divide-by-zero check (break 7) of a division whose result is never used. */
void mnuCampCheckClockDivisor(void) {
    s32 heapStats[CAMP_HEAP_STATS_WORD_COUNT];
    s32 quotient;

    sdfGetGeneralHeapStats(heapStats);
    quotient = 1 / heapStats[0];
}

void mnuEnterCampSceneMenuState(EvtRuntime *scene) {
    if ((scene->menuState == 0) || (scene->menuState == 5)) {
        scene->menuState = 1;
    }
}

extern s32 kwlnCreateHeldTextureBuffer(u16 width, u16 height, f32 value);
extern void func_0025EFD8(EvtRuntime *scene);
extern void mnuShopSubmitDescriptor(u8 *work);
extern void func_0025F2B0(EvtRuntime *scene);

/* Stages 1, 3, 4 and 5 fall through; stage 0 stops. All other stages,
 * including 2, advance once through the default arm. */
void mnuAdvanceShopMenuState(EvtRuntime *scene) {
    switch (scene->menuState) {
    case 1:
        if (kwlnHeldTextureReference == 0) {
            kwlnCreateHeldTextureBuffer(CAMP_DESCRIPTOR_WIDTH, CAMP_DESCRIPTOR_HEIGHT, 100.75f);
        }
        scene->menuState = scene->menuState + 1;
    case 3:
        func_0025EFD8(scene);
        scene->menuState = scene->menuState + 1;
    case 4:
        mnuShopSubmitDescriptor((u8 *)scene);
        scene->menuState = scene->menuState + 1;
    case 5:
        if (scene->shopFlag == 1) {
            func_0025F2B0(scene);
        }
    case 0:
        break;
    default:
        scene->menuState = scene->menuState + 1;
        break;
    }
}


extern SdfPoolNode D_00380708;
extern u8 D_00380860[];
extern void *sdfAllocGeneralBlockHigh(s32 size);
extern s32 sdfAllocatePacketList(s32 (*alloc)(s32));
extern void sdfClearLinkedPacketList(SdfLinkedPacketList *list);
extern void sdfAppendPacketChainNode(SdfPacketChain *head, SdfLinkedPacketList *node);

void func_0025EFD8(EvtRuntime *scene) {
    s32 surface;
    SdfLinkedPacketList *context;
    s32 handle;

    if (scene->pendingResource == 0) {
        handle = (s32)sdfAllocGeneralBlockHigh(0x70000);
        scene->pendingResource = handle;
        scene->pendingWork = (s32)sdfResourceRetainAddress(handle);
    }
    memset((void *)scene->pendingWork, 0x40, 0x70000);
    surface = sdfAllocatePacketList(0);
    context = (SdfLinkedPacketList *)sdfAllocPacketAligned(0x10);
    sdfClearLinkedPacketList(context);
    sdfCreatePatchableResourcePacket((SdfListHead *)surface, context, 0, 0, 0x200, 0xE0,
                                    scene->pendingWork, 0, 0, 0);
    sdfAppendPacketChainNode((SdfPacketChain *)D_00380860, context);
    D_00380708.append((SdfListHead *)&D_00380708, (SdfListHead *)surface);
}

void mnuShopSubmitDescriptor(u8 *scene) {
    s32 drawPacket;

    if (((EvtRuntime *)scene)->pendingWork != 0) {
        drawPacket = sdfAllocatePacketList(0);
        sdfCreateDescriptorPacket(drawPacket, (s32)kwlnHeldTextureReference->primaryResource, 0, 0, CAMP_DESCRIPTOR_WIDTH, CAMP_DESCRIPTOR_HEIGHT, ((EvtRuntime *)scene)->pendingWork, 0);
        D_00380708.append((SdfListHead *)&D_00380708, (SdfListHead *)drawPacket);
    }
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F130);

void func_0025F2B0(EvtRuntime *scene) {
    func_0025F130(scene->auxResource);
}

void func_0025F2C8(void) {
}

void mnuCampSetPrimaryOption(EvtRuntime *scene, u32 option) {
    scene->optionFlags = (scene->optionFlags & CAMP_PRIMARY_OPTION_CLEAR_MASK) | (option & CAMP_OPTION_VALUE_MASK);
}

u32 mnuCampGetPrimaryOption(EvtRuntime *scene) {
    return scene->optionFlags & CAMP_OPTION_VALUE_MASK;
}

void mnuCampSetSecondaryOption(EvtRuntime *scene, u32 option) {
    scene->optionFlags = (scene->optionFlags & CAMP_SECONDARY_OPTION_CLEAR_MASK) | ((option & CAMP_OPTION_VALUE_MASK) << CAMP_SECONDARY_OPTION_SHIFT);
}

u32 mnuCampGetSecondaryOption(EvtRuntime *scene) {
    return (scene->optionFlags & CAMP_SECONDARY_OPTION_MASK) >> CAMP_SECONDARY_OPTION_SHIFT;
}

/* Clear script registers 200..209 whose type-4 key is not active at this frame. */
void func_0025F330(EvtRuntime *scene) {
    EvtRuntimeGroup *track = scene->groups;
    s32 slotIndex;

    while (track != NULL) {
        if (track->type == 4) {
            break;
        }
        track = track->next;
    }
    if (track != NULL) {
        for (slotIndex = 0; slotIndex < CAMP_TIMELINE_SCRIPT_SLOT_COUNT; slotIndex++) {
            EvtRuntimeChild *key = track->children;
            s32 found = 0;
            while (key != NULL) {
                s32 low;
                s32 high;
                mnuUnpackNibbleFields(key, &low, &high);
                if (key->frame > scene->curFrame) {
                    break;
                }
                if (high - 1 == slotIndex) {
                    found = 1;
                    break;
                }
                key = key->next;
            }
            if (!found) {
                if (datGameState->script.ints[CAMP_TIMELINE_SCRIPT_REGISTER_BASE + slotIndex] != -1) {
                    datGameState->script.ints[CAMP_TIMELINE_SCRIPT_REGISTER_BASE + slotIndex] = -1;
                }
            }
        }
    }
}

/* Save the first and last four-component vectors; leave the middle unsaved. */
void mnuShopSavePrimaryTransform(u8 *scene) {
    s32 componentIndex;
    f32 *transformComponents = ((EvtRuntime *)scene)->commandMatrix;
    for (componentIndex = 0; componentIndex < CAMP_TRANSFORM_COMPONENT_COUNT; componentIndex++) {
        mnuShopSavedLastTransformVector[componentIndex] = transformComponents[componentIndex + CAMP_TRANSFORM_LAST_START];
        mnuShopSavedFirstTransformVector[componentIndex] = transformComponents[componentIndex];
    }
    mnuShopRestoreMiddleVector = 0;
}

/* Save all three vectors and enable the middle-vector restore path. */
void mnuShopSaveFullTransform(u8 *scene) {
    s32 componentIndex;
    f32 *transformComponents = ((EvtRuntime *)scene)->commandMatrix;
    for (componentIndex = 0; componentIndex < CAMP_TRANSFORM_COMPONENT_COUNT; componentIndex++) {
        mnuShopSavedLastTransformVector[componentIndex] = transformComponents[componentIndex + CAMP_TRANSFORM_LAST_START];
        mnuShopSavedMiddleTransformVector[componentIndex] = transformComponents[componentIndex + CAMP_TRANSFORM_MIDDLE_START];
        mnuShopSavedFirstTransformVector[componentIndex] = transformComponents[componentIndex];
    }
    mnuShopRestoreMiddleVector = 1;
}

/* Keep the native restore-mode snapshot and last/middle/first write order. */
void mnuShopRestoreTransform(u8 *scene) {
    s32 componentIndex;
    f32 *transformComponents = ((EvtRuntime *)scene)->commandMatrix;
    s32 restoreMiddle = mnuShopRestoreMiddleVector;
    for (componentIndex = 0; componentIndex < CAMP_TRANSFORM_COMPONENT_COUNT; componentIndex++) {
        transformComponents[componentIndex + CAMP_TRANSFORM_LAST_START] = mnuShopSavedLastTransformVector[componentIndex];
        if (restoreMiddle != 0) {
            transformComponents[componentIndex + CAMP_TRANSFORM_MIDDLE_START] = mnuShopSavedMiddleTransformVector[componentIndex];
        }
        transformComponents[componentIndex] = mnuShopSavedFirstTransformVector[componentIndex];
    }
}

/* Append a distinct identifier only while the native registry has room. */
void fldRegisterCampSceneId(EvtRuntime *scene, s32 identifier) {
    s32 registeredCount = scene->registeredCount;
    s32 idIndex = 0;
    if (registeredCount > 0) {
        s32 *idCursor = scene->registeredIds;
        s32 registeredId = *idCursor;
        do {
            idCursor++;
            if (registeredId == identifier) {
                return;
            }
            idIndex++;
            if (idIndex >= registeredCount) {
                break;
            }
            registeredId = *idCursor;
        } while (1);
    }
    if (registeredCount < CAMP_REGISTERED_ID_LIMIT) {
        scene->registeredIds[registeredCount] = identifier;
        ++scene->registeredCount;
    }
}

/* Queue the scene's BGM ID with each registered variation, then clear the list.
 * The loop bound is read again after each queue call, not cached up front. */
void mnuReleaseCampSceneRegisteredIds(EvtRuntime *scene) {
    s32 processedCount = 0;
    if (scene->registeredCount > 0) {
        s32 *idCursor = scene->registeredIds;
        do {
            s32 identifier = *idCursor++;
            processedCount++;
            evtQueueValidatedBgmSoundCode(scene->windowContext->eventId, identifier);
        } while (processedCount < scene->registeredCount);
    }
    scene->registeredCount = 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F640);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F708);


void mnuInitializeShopStatusBatches(MenuTerminalContext *scene) {
    EffMappedResource *batchObject;
    EffMappedRecord *batchGraphics;
    s32 *batchParameters;
    s32 initialParameter = CAMP_STATUS_INITIAL_PARAMETER;
    scene->state = 0;
    batchObject = effCreateStatusBatch(6);
    batchGraphics = batchObject->records;
    scene->objects[0] = batchObject;
    batchParameters = (s32 *)batchGraphics->status;
    batchParameters[0] = initialParameter;
    batchParameters[1] = 0;
    batchParameters[2] = 0;
    batchParameters[3] = 0;
    batchParameters[4] = 0;
    batchObject = effCreateStatusBatch(1);
    batchGraphics = batchObject->records;
    scene->objects[1] = batchObject;
    batchParameters = (s32 *)batchGraphics->status;
    batchParameters[0] = initialParameter;
    batchParameters[1] = 0;
}

/* Destroy both batches and return the second destruction result. */
s32 mnuShopReleaseSceneObjects(MenuTerminalContext *scene) {
    EffMappedResource **batchCursor = scene->objects;
    s32 destroyResult;
    u32 batchIndex;
    for (batchIndex = 0; batchIndex < CAMP_STATUS_BATCH_COUNT; batchIndex++) {
        destroyResult = effDestroyPackedBatch((s32)*batchCursor++);
    }
    return destroyResult;
}


extern const CampMapArguments D_00424A90;
extern const CampEffectRows D_00424AC0;
extern const char D_00424AE0[];
extern u64 sdfReadNamedResource();
extern u32 effCreateMappedResource(u32);
extern void mnuInitializeMapPacket(u32, u32 *, s32, MapPacket *);
extern void mnuSetCampEffectResourceHandles(u32, u32, MenuEffectResources *);
extern void mnuCopyCampEffectRowData(const CampEffectRows *, MenuEffectResources *);
extern void mnuOrEntryFlags(u32, u32 *);

void func_0025F8B8(u32 object, MenuEffectResources *resources) {
    CampMapArguments mapArguments = D_00424A90;
    CampEffectRows rows = D_00424AC0;
    u32 dataAddress;
    u64 allocation;
    u32 mappedResource;

    allocation = sdfReadNamedResource(D_00424AE0, &dataAddress, 0);
    mappedResource = effCreateMappedResource(dataAddress);
    sdfReleaseResourceAllocation(allocation);
    mnuInitializeMapPacket(2, mapArguments.values, 11, &resources->packet);
    mnuSetCampEffectResourceHandles(object, mappedResource, resources);
    mnuCopyCampEffectRowData(&rows, resources);
    mnuOrEntryFlags(7, &resources->packet.type);
}


void mnuShopDestroyNestedEffectBatch(MenuEffectResources *resources) {
    effDestroyPackedBatch(resources->animationHandle);
}

extern s32 mnuFirstPresentMainCharacterIndex(void);
extern u32 effLoadIndexedResource(s32 category, s32 index, s32 keepAllocation);
extern const char *D_003CE470[4];
extern const char *D_003CE480[]; /* Two entries in external .data, not small data. */
extern const char *D_003CE488[4];

/* Load the shop resource sets and initialize its two scrolling panels. */
INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A00);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A10);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A20);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A30);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A40);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A50);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A60);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A70);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A80);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424A90);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424AC0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424AE0);

void func_0025FA28(MenuTerminalContext *scene) {
    s32 indices[3] = {77, 78, 76};
    DspScrollingStripState *firstPanel = &scene->panelWork[0];
    DspScrollingStripState *secondPanel = &scene->panelWork[1];

    scene->effectSlots[0] = (struct EffectSlotSet *)effLoadIndexedResource(
        (s32)"/facility/spr/shop/", (s32)D_003CE470[0], 0);
    switch (scene->type) {
    case 0:
    case 2:
        scene->effectSlots[1] = (struct EffectSlotSet *)effLoadIndexedResource(
            (s32)"/facility/spr/shop/", (s32)D_003CE470[1], 0);
        break;
    case 1:
    case 3:
        scene->effectSlots[1] = (struct EffectSlotSet *)effLoadIndexedResource(
            (s32)"/facility/spr/shop/", (s32)D_003CE470[2], 0);
        func_0025F8B8((u32)scene->effectSlots[1], &scene->campEffect.resources);
        break;
    }
    scene->effectSlots[2] = (struct EffectSlotSet *)effLoadIndexedResource(
        (s32)"/facility/spr/shop/", (s32)D_003CE480[0], 0);
    scene->effectSlots[3] = (struct EffectSlotSet *)effLoadIndexedResource(
        (s32)"/facility/spr/shop/", (s32)D_003CE488[mnuFirstPresentMainCharacterIndex()], 0);
    mnuInitScrollingStripState(firstPanel, 0, scene->effectSlots[0], 0x46, 0x43);
    func_0026BE28(firstPanel, 1, 0x10, 0x20);
    mnuInitScrollingStripState(secondPanel, 0, scene->effectSlots[0], 0x46, 0x43);
    func_0026BE28(secondPanel, 0, 0x10, 0x20);
    func_0026BEB0(secondPanel, 0x1150, 0xCB8, 0);
    scene->windowResource = mnuCreateWindowSpriteResources(
        0, 0, 0, (u32)scene->effectSlots[0], indices, 3);
}


extern void evtLoadResourcePair();
extern void evtCreateMessageWindowIfMissing();

void mnuShopLoadMessageResource(MenuTerminalContext *scene) {
    scene->type = D_003C9A40[scene->shopRow].messageSet;
    switch (scene->type) {
    case 0:
        evtLoadResourcePair("/facility/msg/shop/mes_01.bmd", scene->messageResources);
        break;
    case 1:
        evtLoadResourcePair("/facility/msg/shop/mes_02.bmd", scene->messageResources);
        break;
    case 2:
        evtLoadResourcePair("/facility/msg/shop/mes_03.bmd", scene->messageResources);
        break;
    case 3:
        evtLoadResourcePair("/facility/msg/shop/mes_04.bmd", scene->messageResources);
        break;
    }
    evtCreateMessageWindowIfMissing(scene->messageResources[1]);
}


void mnuShopReleaseWindowAndEffectResources(MenuTerminalContext *scene) {
    s32 i;
    struct EffectSlotSet **slot = scene->effectSlots;

    mnuReleaseWindowTextures(scene->windowResource);
    for (i = 1; i >= 0; i--) {
        effDestroyResourceSlotSet(*slot++);
    }
    effDestroyResourceSlotSet(scene->effectSlots[2]);
    effDestroyResourceSlotSet(scene->effectSlots[3]);
    switch (scene->type) {
    case 1:
    case 3:
        mnuShopDestroyNestedEffectBatch(&scene->campEffect.resources);
        break;
    case 0:
    case 2:
        break;
    default:
        return;
    }
}

extern void func_00306CD0(s32, s32, s32, u32, s32, EffectSlotSet *, s32, s32);
extern void mnuDrawCampIconBackdrop(MenuCampEffect *, s32);

/* The shop layouts use slot 1 as their background and slot 3 as the rotated portrait. */
void func_0025FD78(MenuTerminalContext *scene) {
    switch (scene->type) {
    case 0:
    case 2:
        func_00306CD0(0, 0, 0, 0x100, 0, scene->effectSlots[1], 0, 0x53);
        func_00306CD0(0xE30, 0x610, 0, 0x100, 0, scene->effectSlots[2], 0, 0x53);
        scene->effectSlots[3]->workEntries[0].geometry.angleDegrees = 90.0f;
        func_00306CD0(0x9F0, 0x610, 0, 0x100, 2, scene->effectSlots[3], 0, 0x53);
        return;
    case 1:
    case 3:
        mnuDrawCampIconBackdrop(&scene->campEffect, 0x53);
        break;
    }
}

s32 mnuCampHasEligibleOwnedItems(void) {
    s32 result = 0;
    s32 i;

    for (i = 0; i < 0x100; i++) {
        if (mnuIsBulletItemId(i) != 0) {
            continue;
        }
        if (func_002C54B0(i) != 0) {
            continue;
        }
        if (datGameState->inventory.counts[i] == 0) {
            continue;
        }
        if ((datItemSkillRecords[i * 8] & 3) != 0) {
            result = 1;
            break;
        }
        if (func_002C5498(i) != 0) {
            result = 1;
            break;
        }
    }
    return result;
}

extern MenuWindowContainer *mnuCreateWindowContainer(s32 id, s32 width, s32 height,
                                                       s32 visibleCount, s32 rowSpacing);
extern void func_00295400(void);

s32 mnuCreateEnabledCampEntryWindow(s32 count, s32 *enabled, MenuTerminalContext *settings) {
    MenuWindowContainer *window;
    MenuTerminalWindowState *storage;
    s32 i;

    window = mnuCreateWindowContainer(0, 0x260, 0x10, count, 0x16);
    mnuSetWindowEntryParameters(0, window, 0, 8, 0xA);
    for (i = 0; i < count; i++) {
        if (enabled[i] != 0) {
            mnuAppendWindowListNode(window, 0)->camp.value = i;
        }
    }
    window->list->drawCallback = func_00295400;
    storage = sdfAllocSizeClassBlock(0x14);
    memset(storage, 0, 0x14);
    window->list->context = storage;
    storage->unk0C = settings->unkA0;
    storage->unk0E = settings->unkA2;
    storage->unk10 = settings->unkA4;
    return (s32)window;
}

typedef struct CampEntryEnableSet {
    s32 enabled[7];
} CampEntryEnableSet;

extern const CampEntryEnableSet D_00424BA0;
extern s32 func_00260250(MenuTerminalContext *, s32);

void func_00260020(MenuTerminalContext *scene) {
    CampEntryEnableSet options = D_00424BA0;
    scene->unkA0 = func_00260250(scene, 1);
    if (mdlFlagTest(0x901)) {
        scene->unkA2 = func_00260250(scene, 3);
    }
    switch (D_003C9A40[scene->shopRow].messageSet) {
    case 1:
    case 2:
    case 3:
        scene->unkA4 = 1;
        break;
    }
    if (scene->unkA0 != 0) {
        options.enabled[1] = 1;
    }
    if (scene->unkA2 != 0) {
        options.enabled[2] = 1;
    }
    if (datGameState->progressTotal != 0) {
        options.enabled[4] = 1;
    }
    if (scene->unkA4 != 0) {
        options.enabled[5] = 1;
    }
    scene->ownedWindows[0] = (MenuWindowContainer *)mnuCreateEnabledCampEntryWindow(7, options.enabled, scene);
}

s32 mnuCampResolveFlagRowValue(s32 row) {
    s32 i;

    for (i = 7; i >= 0; i--) {
        if (D_003C9A40[row].flag[i] > 0 && mdlFlagTest(D_003C9A40[row].flag[i])) {
            return D_003C9A40[row].value[i + 1];
        }
    }
    return D_003C9A40[row].value[0];
}

/* Scan the twenty-one flag IDs in descending slot order. */
/* Highest matching row wins; rows with nonpositive flags are not queried. */
s32 mnuCampFindActiveSlot(void) {
    s32 rowIndex;
    for (rowIndex = CAMP_SLOT_LAST_ROW_INDEX; rowIndex >= 0; rowIndex--) {
        if (D_003CBB70[rowIndex].unlockFlag > 0 && mdlFlagTest(D_003CBB70[rowIndex].unlockFlag)) {
            return rowIndex;
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260250);


void mnuShopReleaseWindowSprites(s32 keepExtra, MenuTerminalContext *scene) {
    MenuWindowContainer **slot = scene->ownedWindows;
    MenuWindowContainer *sprite;
    u32 i;

    for (i = 0; i < 1; i++) {
        sprite = *slot;
        if (sprite != NULL) {
            if (sprite->list->context != NULL) {
                sdfReleaseChipBlock(sprite->list->context);
                sprite = *slot;
                sprite->list->context = NULL;
            }
            mnuDestroyWindowContainer(sprite);
        }
        slot++;
    }
    if (keepExtra == 0) {
        if (scene->window != NULL) {
            if (scene->window->list->context != NULL) {
                sdfReleaseChipBlock(scene->window->list->context);
                scene->window->list->context = NULL;
            }
            mnuDestroyWindowContainer(scene->window);
        }
    }
}

u32 func_00260458(void) {
    return 0;
}

u32 func_00260460(void) {
    return 0;
}

/* Sum the active low bit across five entries using the native halfword stride. */
s32 mnuCountActivePartyEntries(void) {
    u16 entryFlags;
    DatPartyRecord *entryFlagsCursor;
    s32 entryCountdown;
    s32 enabledCount;

    enabledCount = 0;
    entryCountdown = CAMP_PARTY_SCAN_LAST;
    entryFlagsCursor = datGameState->party;
    do {
        entryFlags = entryFlagsCursor->flags;
        entryFlagsCursor++;
        entryCountdown = entryCountdown - 1;
        enabledCount = enabledCount + (entryFlags & CAMP_PARTY_ACTIVE_FLAG);
    } while (-1 < entryCountdown);
    return enabledCount;
}

typedef struct CampTier {
    u32 threshold;
    s32 value;
} CampTier;

extern CampTier D_003CE408[];

s32 mnuCampResolveProgressTierValue(void) {
    u32 i;

    for (i = 0; i < 3; i++) {
        if (i + 1 < 3) {
            if (D_003CE408[i].threshold > datGameState->savedCurrency) {
                break;
            }
        } else if (D_003CE408[i].threshold <= datGameState->savedCurrency) {
            break;
        }
    }
    return D_003CE408[i].value;
}

void mnuCampClearListedItemCounts(void) {
    u16 entryId;
    s8 *entry;
    u32 index;

    index = 0;
    entry = mnuCampListedItems;
    do {
        entryId = *(u16 *)entry;
        entry = (s8 *)((s32)entry + 8);
        index = index + 1;
        datGameState->inventory.counts[(u32)entryId] = 0;
    } while (index < 3);
}

MenuTerminalContext *mnuTerminalCreateContext(void) {
    s32 handle;
    MenuTerminalContext *obj;

    handle = sdfAllocGeneralBlock(0x38C);
    obj = (MenuTerminalContext *)sdfResourceRetainAddress(handle);
    memset(obj, 0, 0x38C);
    obj->resourceHandle = handle;
    mnuClearPanelTransitionState(&obj->transitionWork);
    mnuInitializeShopStatusBatches(obj);
    obj->options = func_00260460();
    obj->availableCount = mnuCountActivePartyEntries();
    obj->delayFrames = 0xF;
    func_002945B8(obj);
    mnuResetGradientFadeColor(&obj->gradientFade, 0x60);
    mnuCampClearListedItemCounts();
    sndEnsureMidiBankResident(0x300000);
    sndStartTrackExtended(0x300000);
    return obj;
}

void mnuTerminalReleaseContextAndResources(s32 arg) {
    MenuTerminalContext *scene = (MenuTerminalContext *)kwlnTaskGetUserValue();

    if (scene != 0) {
        mnuShopReleaseWindowSprites(0, scene);
        mnuShopReleaseWindowAndEffectResources(scene);
        mnuShopReleaseSceneObjects(scene);
        mnuDrainPanelTransitions(&scene->transitionWork, arg);
        dspCloseChannel();
        evtReleaseResourcePairHandle(scene->messageResources);
        sdfReleaseResourceAllocation(scene->resourceHandle);
        mnuPanelTaskCompletionState = 2;
    }
}

s32 mnuTerminalSyncMessageWindowControl(void) {
    MenuGradientFade *state = &((MenuTerminalContext *)kwlnTaskGetUserValue())->gradientFade;
    mnuDrawAndStepGradientFade(state, 0x53);
    if (evtGetMessageWindowControlState() != 0) {
        func_002C1B68(&state->active, 1);
    } else {
        func_002C1B68(&state->active, 0);
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424BA0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424BC0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424BD0);

void func_00260708(const s32 *stockGroup) {
    MenuTerminalContext *scene = mnuTerminalCreateContext();
    KwlnTask *drawTask;
    KwlnTask *fadeTask;

    if (stockGroup != NULL) {
        scene->shopRow = *stockGroup;
    }
    kwlnTaskCreate(D_00437838, 0x402, 1, 1,
        mnuPreparePopupAndDispatchSelection, NULL, (u32)scene);
    drawTask = kwlnTaskCreate(D_00424BC0, 0x2B12, 1, 1,
        mnuAdvanceCampPopup, NULL, (u32)scene);
    kwlnTaskCreate(D_00424BD0, 0x520E, 1, 1,
        mnuFinishCampPopup, mnuTerminalReleaseContextAndResources, (u32)scene);
    fadeTask = kwlnTaskCreate("shop_fade", 0x2B13, 1, 0,
        mnuTerminalSyncMessageWindowControl, NULL, (u32)scene);
    func_00101968(drawTask, fadeTask);
    mnuPanelTaskCompletionState = 1;
}

void mnuCampDestroyPanelTasks(void) {
    kwlnTaskDestroyWithHierarchyByName(D_00437838, 0);
    kwlnTaskDestroyWithHierarchyByName(D_00424BC0, 0);
    kwlnTaskDestroyWithHierarchyByName(D_00424BD0, 0);
}

s32 mnuCampConsumePanelTaskCompletion(void) {
    s32 state = mnuPanelTaskCompletionState;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        mnuPanelTaskCompletionState = 0;
    }
    return 0;
}

static inline s32 campSetHandler(MenuTerminalContext *context, s32 mode, void *callback) {
    return func_002C4038(&context->transitionWork, &context->popupState, mode, callback);
}

s32 mnuPreparePopupAndDispatchSelection(s32 callback) {
    MenuTerminalContext *context = (MenuTerminalContext *)kwlnTaskGetUserValue();
    mnuSetPopupEntry(&context->popupState, D_003CE658);
    return campSetHandler(context, 0, (void *)callback);
}

s32 mnuAdvanceCampPopup(s32 callback) {
    return campSetHandler((MenuTerminalContext *)kwlnTaskGetUserValue(), 1, (void *)callback);
}

s32 mnuFinishCampPopup(s32 callback) {
    return campSetHandler((MenuTerminalContext *)kwlnTaskGetUserValue(), 2, (void *)callback);
}

typedef struct ShopSourcePriceEntry {
    u16 itemId;
    u8 type;
    u8 pricePercent;
} ShopSourcePriceEntry;

typedef struct ShopSourcePriceRow {
    u16 pricePercent;
    ShopSourcePriceEntry entries[0x60];
} ShopSourcePriceRow;

typedef struct ShopItemPriceRecord {
    u8 pad00[4];
    s32 price;
} ShopItemPriceRecord;

typedef struct ShopProgressPrice {
    f32 percent;
    u32 unk4;
} ShopProgressPrice;

extern ShopSourcePriceRow D_003C9BC0[];
extern ShopProgressPrice D_003CD0C4[];

s32 mnuCampGetSourceItemPrice(s32 index, s32 source, s32 halfPrice) {
    u32 rowIndex;
    u32 itemId;
    u32 rowPercent;
    u32 pricePercent;
    s32 itemPrice;
    s32 price;
    s32 progressStage;

    rowIndex = (u8)mnuCampResolveFlagRowValue(source);
    itemId = D_003C9BC0[rowIndex].entries[index].itemId;
    if (halfPrice == 0) {
        itemPrice = ((ShopItemPriceRecord *)datItemSkillRecords)[itemId].price;
        rowPercent = D_003C9BC0[rowIndex].pricePercent;
        pricePercent = D_003C9BC0[rowIndex].entries[index].pricePercent;
        if (pricePercent == 0) {
            pricePercent = rowPercent;
        }
        price = itemPrice * pricePercent / 100;
        progressStage = func_00260458();
        if (progressStage != 0) {
            price = price * (s32)D_003CD0C4[progressStage].percent / 100;
        }
    } else {
        price = (u32)((ShopItemPriceRecord *)datItemSkillRecords)[itemId].price >> 1;
    }
    return price;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260A58);

s32 mnuCampFindListedItemIndex(s32 id) {
    u16 *entry = (u16 *)mnuCampListedItems;
    u32 index = 0;
    do {
        if (id == *entry) {
            return index;
        }
        entry += 4;
        index++;
    } while (index < 3);
    return -1;
}

typedef struct EffItemSlot44 {
    u32 id;
    u8 pad_04[0x40];
} EffItemSlot44;

extern EffItemSlot44 D_003CD8F0[];
extern void mdlFlagSet(u32);

s32 itmClaimFreeSlot(s32 row) {
    s32 first = row * 2;
    EffItemSlot44 *slot = &D_003CD8F0[first];
    s32 i = 0;

    do {
        u32 id = slot->id;

        slot++;
        if (id != 0) {
            if (mdlFlagTest(id) == 0) {
                mdlFlagSet(id);
                return i;
            }
        }
        i++;
    } while (i < 2);
    return -1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260C28);

s32 func_00260C38(s32 slot, s32 row) {
    return row * 2 + slot;
}

u32 mnuCampChooseSolarWeightedOption(s32 row) {
    s32 chance = effMiscRand(0) & 0xFF;
    u8 *item = D_003CD8DD + row * 8;
    s32 total = 0;
    u32 index = 0;
    do {
        if (evtGetMirroredSolarPhase() == 8) {
            total += item[-3];
        } else {
            total += item[0];
        }
        if (total >= chance) {
            return index;
        }
        index++;
        item++;
    } while (index < 3);
    return 0;
}

u8 mnuCampChooseWeightedTableValue(void) {
    s32 chance = effMiscRand(0) & 0xFF;
    s32 total = 0;
    u8 *item = D_003CD8D0;
    u32 index = 0;
    do {
        total += item[1];
        if (total >= chance) {
            return item[0];
        }
        index++;
        item += 2;
    } while (index < 3);
    return 1;
}

s32 mnuCampFindFreeWideEntryIndex(s32 row) {
    s32 *entry = (s32 *)(D_003CDA8C + row * 0xC0);
    s32 index = 0;
    do {
        s32 value = *entry;
        entry += 3;
        if (value == 0) {
            return index;
        }
        index++;
    } while (index < 16);
    return 15;
}

s32 mnuCampFindFreeCompactEntryIndex(s32 unused, s32 row) {
    s32 *entry = (s32 *)(mnuCampCompactEntries + row * 0x44);
    s32 index = 0;
    do {
        s32 value = *entry;
        entry += 2;
        if (value == 0) {
            return index;
        }
        index++;
    } while (index < 8);
    return 7;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260DF0);

extern u8 D_003CDA89[];

u8 func_00260FE8(s32 row, s32 column) {
    return D_003CDA89[column * 0xC + row * 0xC0];
}

extern u8 D_003CD8F4[];

u8 func_00261018(s32 row, s32 column) {
    return D_003CD8F4[column * 8 + row * 0x44];
}

s32 mnuCampResolveOwnedItemVariant(s32 row, s32 column) {
    u8 *entry = D_003CDA88 + column * 0xC + row * 0xC0;
    s32 id = *(s32 *)(D_003CDA88 + column * 0xC + row * 0xC0 + 4);

    if (entry[1] == 0 && func_002C54B0(id) != 0 && datGameState->inventory.counts[id] != 0) {
        id = *(u16 *)(entry + 8);
    }
    return id;
}

s32 mnuCampGetCompactEntryId(s32 row, s32 column) {
    return *(s32 *)(mnuCampCompactEntries + row * 0x44 + column * 8);
}

s32 mnuCampCountRemainingUses(s32 mode, s32 id, MenuTerminalContext *record) {
    s32 value = record->availableCount;

    if (mode == 1) {
        value -= ptyCountBulletItem(id);
    } else if (mode == 3) {
        value = 1 - datGameState->inventory.counts[id];
    } else if (mode == 2) {
        value = 1 - datGameState->inventory.counts[id];
    } else {
        value = 99 - datGameState->inventory.counts[id];
    }
    if (mnuCampFindListedItemIndex(id) >= 0) {
        if (value >= 2) {
            value = datGameState->inventory.counts[id] == 0;
        }
    }
    return value < 0 ? 0 : value;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261198);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261290);

/* Mark rows unavailable when neither currency nor inventory capacity permits a use. */
void mnuCampDisableUnavailableItemEntries(MenuTerminalContext *scene) {
    struct MenuListNode *node;
    CampWindowParams *item;
    s32 i;
    s32 count;
    s32 remaining;

    node = scene->window->list->first;
    for (i = 0; i < scene->window->list->count; i++) {
        item = &node->camp;
        /* Keep the unchecked division: retail traps when the row price is zero. */
        count = datGameState->header.currency / item->price;
        remaining = mnuCampCountRemainingUses(item->mode, item->id, scene);
        if (remaining < count) {
            count = remaining;
        }
        if (count == 0) {
            node->flags48 = 1;
        }
        node = node->next;
        if (node == NULL) {
            break;
        }
    }
}

typedef struct CampPriceTier {
    u8 limit;
    u8 percent;
} CampPriceTier;

extern CampPriceTier D_003CE3E8[];

typedef struct CampRateRow {
    u16 percent;
    u8 pad02[10];
} CampRateRow;

extern CampRateRow D_003CE150[];

s32 func_002613C8(s32 level, s32 price) {
    u32 i;

    if (level != 0) {
        for (i = 0; i < 8; i++) {
            if (D_003CE3E8[i].limit != 0 && D_003CE3E8[i].limit >= level) {
                price = price * D_003CE3E8[i].percent / 100;
                break;
            }
        }
    } else {
        price = price * D_003CE150[datGameState->progressSlot].percent / 100;
    }
    return price;
}

s32 mnuCampAdvanceCounter(s32 delta, MenuTerminalContext *scene) {
    s32 max = func_00261198(scene);
    CampWindowParams *slot = &scene->window->list->cursor->camp;
    s32 sum = scene->multiplier + delta;
    s32 cur;

    scene->multiplier = sum;
    if (sum <= 0) {
        scene->multiplier = 1;
    }
    cur = scene->multiplier;
    if (cur >= max) {
        scene->unkC7 = 1;
        scene->multiplier = max;
        cur = max;
    } else {
        scene->unkC7 = 0;
    }
    if (scene->ownedWindows[0]->list->cursor->camp.value == 3) {
        slot->value = func_002613C8(cur, slot->price);
        cur = scene->multiplier;
    }
    return cur;
}

extern u8 *D_00435E5C;
extern u8 D_003C9BC4[];
extern s32 mnuAdvanceListCursorDefault();
extern void mnuSelectFirstListNode();
extern void func_002B9808();
extern s32 func_002958B0();
extern s32 mnuCampGetSourceItemPrice(s32, s32, s32);

s32 func_00261538(MenuTerminalContext *scene) {
    s32 rowIndex;
    ShopRankPriceRow *row;
    MenuWindowContainer *window;
    u32 i;

    rowIndex = mnuCampFindActiveSlot();
    if (rowIndex == -1) {
        return 0;
    }
    row = &D_003CBB70[rowIndex];
    for (i = 0; i < 0x20; i++) {
        u32 id = row->entries[i].itemId;
        u32 mode = row->entries[i].mode;
        struct MenuListNode *node;
        CampWindowParams *params;
        s32 price;

        if (mnuIsBulletItemId(id) != 0 || id == 0) {
            continue;
        }
        node = mnuAppendWindowListNode(scene->window,
            D_00435E5C + id * 0x19);
        func_002B9808(scene->window);
        window = scene->window;
        mnuAdvanceListCursorDefault(window->list);
        params = &node->camp;
        price = func_00260A58(i, 0);
        params->id = id;
        params->value = price;
        params->price = price;
        params->mode = mode;
        if (func_00261198(scene) == 0) {
            node->flags48 = 1;
        }
    }
    mnuSelectFirstListNode(scene->window->list);
    return 0;
}

s32 func_00261850(MenuTerminalContext *scene, s32 filterMode);

s32 func_00261670(MenuTerminalContext *scene) {
    MenuTerminalWindowState *buffer;
    u8 *entry;
    u32 i;
    s32 row;

    if (scene->window != NULL) {
        if (scene->window->list->context != NULL) {
            sdfReleaseChipBlock(scene->window->list->context);
            scene->window->list->context = NULL;
        }
        mnuDestroyWindowContainer(scene->window);
    }
    row = mnuCampResolveFlagRowValue(scene->shopRow);
    scene->window = mnuCreateWindowContainer(1, 0x260, 0x10, 8, 0x16);
    /* Each packed stock row spans 0xC1 halfwords. */
    entry = D_003C9BC4 + (row << 8) + (((row << 6) + row) << 1);
    for (i = 0; i < 0x60; i++, entry += 4) {
        u32 id = *(u16 *)(entry - 2);
        u32 mode = entry[0];
        struct MenuListNode *node;
        CampWindowParams *params;
        s32 price;

        if (mnuIsBulletItemId(id) != 0 || func_002C54B0(id) != 0 || id == 0) {
            continue;
        }
        node = mnuAppendWindowListNode(scene->window,
            D_00435E5C + id * 0x19);
        func_002B9808(scene->window);
        mnuAdvanceListCursorDefault(scene->window->list);
        params = &node->camp;
        price = mnuCampGetSourceItemPrice(i, scene->shopRow, 0);
        params->value = price;
        params->id = id;
        params->price = price;
        params->mode = mode;
        if (func_00261198(scene) == 0) {
            node->flags48 = 1;
        }
    }
    func_00261538(scene);
    mnuSelectFirstListNode(scene->window->list);
    scene->window->list->drawCallback = func_002958B0;
    buffer = sdfAllocSizeClassBlock(0x14);
    memset(buffer, 0, 0x14);
    scene->window->list->context = buffer;
    return scene->window->list->count;
}

s32 func_00261850(MenuTerminalContext *scene, s32 filterMode) {
    s32 rowIndex;
    ShopRankPriceRow *row;
    u32 i;

    rowIndex = mnuCampFindActiveSlot();
    if (rowIndex == -1) {
        return 0;
    }
    row = &D_003CBB70[rowIndex];
    for (i = 0; i < 0x20; i++) {
        u32 id = row->entries[i].itemId;
        u32 mode = row->entries[i].mode;
        struct MenuListNode *node;
        CampWindowParams *params;
        s32 price;
        s32 include;

        if (filterMode == 1) {
            include = mnuIsBulletItemId(id);
        } else {
            include = func_002C54B0(id);
        }
        if (include == 0) {
            continue;
        }
        node = mnuAppendWindowListNode(scene->window,
            D_00435E5C + id * 0x19);
        func_002B9808(scene->window);
        mnuAdvanceListCursorDefault(scene->window->list);
        params = &node->camp;
        price = func_00260A58(i, 0);
        params->value = price;
        params->id = id;
        params->price = price;
        params->mode = mode;
        if (func_00261198(scene) == 0) {
            node->flags48 = 1;
        }
    }
    mnuSelectFirstListNode(scene->window->list);
    return 0;
}

s32 func_002619A8(MenuTerminalContext *scene, s32 filterMode) {
    MenuTerminalWindowState *buffer;
    struct MenuListNode *node;
    u8 *entry;
    u32 i;
    s32 row;

    if (scene->window != NULL) {
        if (scene->window->list->context != NULL) {
            sdfReleaseChipBlock(scene->window->list->context);
            scene->window->list->context = NULL;
        }
        mnuDestroyWindowContainer(scene->window);
    }
    row = mnuCampResolveFlagRowValue(scene->shopRow);
    scene->window = mnuCreateWindowContainer(1, 0x260, 0x10, 8, 0x16);
    /* Each packed stock row spans 0xC1 halfwords. */
    entry = D_003C9BC4 + (row << 8) + (((row << 6) + row) << 1);
    for (i = 0; i < 0x60; i++, entry += 4) {
        u32 id = *(u16 *)(entry - 2);
        u32 mode = entry[0];
        CampWindowParams *params;
        s32 price;
        s32 include;

        if (filterMode == 1) {
            include = mnuIsBulletItemId(id);
        } else {
            include = func_002C54B0(id);
        }
        if (include == 0) {
            continue;
        }
        node = mnuAppendWindowListNode(scene->window,
            D_00435E5C + id * 0x19);
        func_002B9808(scene->window);
        mnuAdvanceListCursorDefault(scene->window->list);
        params = &node->camp;
        price = mnuCampGetSourceItemPrice(i, scene->shopRow, 0);
        params->value = price;
        params->id = id;
        params->price = price;
        params->mode = mode;
        if (func_00261198(scene) == 0) {
            node->flags48 = 1;
        }
    }
    func_00261850(scene, filterMode);
    mnuSelectFirstListNode(scene->window->list);
    scene->window->list->drawCallback = func_002958B0;
    buffer = sdfAllocSizeClassBlock(0x14);
    memset(buffer, 0, 0x14);
    scene->window->list->context = buffer;
    return scene->window->list->count;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261B98);

void mnuQueueCampTextGlyphWithChildColor(s32 fontValue, s32 enabled, s32 unused2, s32 unused3, s32 fontArg, s32 flags) {
    s32 handle;

    if (enabled != 0) {
        handle = itfDrawBankTextWithLayoutFlags(0x970, 0xB58, 1, (u16)fontValue, enabled, fontArg);
        frFontSetChildColors(handle, 0x80808040);
        func_0019D550(handle, 0, flags);
        frFontQueueGlyphInSelectedSlot(handle);
    }
}

INCLUDE_SDATA(const s32, "game/code_0025DA20", mnuShopRestoreMiddleVector);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_004377F8);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437800);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437808);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437810);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437818);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437820);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437828);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437830);

INCLUDE_SDATA(const s32, "game/code_0025DA20", D_00437838);

