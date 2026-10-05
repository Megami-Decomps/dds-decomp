#include "common.h"
#include "mnu.h"
#include "sdf.h"
#include "kwln.h"

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
#define CAMP_STATUS_BATCH_COUNT 2
#define CAMP_STATUS_INITIAL_PARAMETER 15
#define CAMP_SLOT_LAST_ROW_INDEX 0x14
#define CAMP_SLOT_LAST_ROW_OFFSET 0x1450
#define CAMP_SLOT_ROW_STRIDE 0x104
#define CAMP_PARTY_SCAN_LAST 4
#define CAMP_PARTY_FLAGS_OFFSET 0xa60
#define CAMP_PARTY_HALFWORD_STRIDE 0xe2
#define CAMP_PARTY_ACTIVE_FLAG 1
#define CAMP_HEAP_STATS_WORD_COUNT 8

extern void func_00101968(KwlnTask *, KwlnTask *);
extern s32 mnuPreparePopupAndDispatchSelection(s32);
extern s32 mnuAdvanceCampPopup(s32);
extern s32 mnuFinishCampPopup(s32);

extern s32 sdfAllocGeneralBlock(s32);
extern u8 *sdfResourceRetainAddress(s32);
extern void mnuClearPanelTransitionState(u8 *);
extern void mnuInitializeShopStatusBatches(u8 *);
extern void func_002945B8(u8 *);
extern void mnuResetGradientFadeColor(u8 *, s32);
extern void sndEnsureMidiBankResident(s32);
extern void sndStartTrackExtended(s32);
extern s32 func_00261198();
extern s32 func_002613C8();
extern s32 ptyCountBulletItem(s32);

extern s8 mnuCampListedItems[34];

extern void func_00246950();

typedef struct CampFlagRow {
    s32 messageSet;  /* 0x00: shop message resource number */
    s16 flag[8];     /* 0x04 */
    u8 value[9];     /* 0x14: [0] default, [i + 1] for flag[i] */
    u8 pad1D[3];
} CampFlagRow;

extern CampFlagRow D_003C9A40[];

extern s32 mdlFlagTest(u32);

extern void mnuSetPopupEntry(s32 *, void *);

extern u8 D_003CE658[];

extern s32 kwlnHeldTextureReference;

/* Camp reads the currency word, byte-sized inventory counts and a tier input. */
typedef struct CampSaveState {
    u8 pad00[0x3C];
    s32 money;
    u8 pad40[0x1300];
    u8 counts[0x100]; /* 0x1340 */
    u8 pad1440[0x1D210];
    u32 unk1E650; /* Compared against the camp tier thresholds. */
} CampSaveState;

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

extern s32 datGameState;

extern s32 func_00261B98(s32);

extern void func_0025FD78(s32);

extern char D_00437838[]; /* "camp" */

extern char D_00424BC0[]; /* "camp_draw" */

extern char D_00424BD0[]; /* "camp_update" */

extern s8 mnuPanelTaskCompletionState;

extern s32 func_002C4038(s32, s32 *, u64, u64);

extern u32 kwlnTaskGetUserValue();

extern void mnuShopReleaseWindowSprites();

extern void sdfReleaseChipBlock();

extern void mnuDestroyWindowContainer();

extern void mnuShopReleaseWindowAndEffectResources();

extern void mnuReleaseWindowTextures();

extern void effDestroyResourceSlotSet();

extern s32 mnuShopReleaseSceneObjects(u8 *);

extern void mnuDrainPanelTransitions(s32, s32);

extern s32 dspCloseChannel(void);

extern void evtReleaseResourcePairHandle();

extern void sdfReleaseResourceAllocation();

extern void mnuDrawAndStepGradientFade(s32, s32);

extern void func_002C1B68(s32, s32);

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

extern void evtTickPackLoad(void);

extern s32 func_002C54B0(s32);
extern s32 mnuIsBulletItemId(s32);
extern s32 func_002C5498(s32);
extern u8 *datItemSkillRecords;

extern u8 D_003CDA88[];

extern s32 itfDrawBankTextWithLayoutFlags(s32, s32, u64, u64, u64, u64);

extern void frFontSetChildColors(s32, u32);

extern s32 func_0019D550(s32, s32, u32);

extern void evtReleaseEventPackResources(void);
extern f32 mnuShopSavedLastTransformVector[];
extern f32 mnuShopSavedMiddleTransformVector[];
extern f32 mnuShopSavedFirstTransformVector[];
extern s32 mnuShopRestoreMiddleVector;
extern u8 *effCreateStatusBatch(s32 kind);
typedef struct BufferDescriptor {
    u8 pad00[0x10];
    void (*open)(struct BufferDescriptor *, s32);
    u8 pad14[0xC];
} BufferDescriptor;
extern s32 sdfAllocPacketAligned(s32 size);
extern void sdfInitPacketList(s32 packet);
extern void itfSendTablePacket(s32 packet, s32 table, s32 mode);
extern void itfQueueTextureBoundQuadPacket(void *, void *, void *, s32, s32, s32, s32);
extern void evtSetDrawSurfaceIndex(u32);
extern void evtSubmitPrimaryAlphaBlendMode(s32);
extern void evtSubmitPrimaryGsTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern s32 D_003C9988[4];
extern s32 D_003C9998[4];
extern s32 D_003C99A8[4];
extern BufferDescriptor kwlnDrawSurfaces[];
extern s32 effDestroyPackedBatch(s32);

typedef struct CampTaskData {
    s32 taskId;
    s32 unk4;
    u8 pad08[0x40];
} CampTaskData;

extern u8 D_003CBB70[];

/* Schedule the camp task only if no task currently owns this event ID. */
void mnuCampCreateTask(s32 taskId) {
    char taskName[CAMP_TASK_NAME_BYTES];
    CampTaskData *taskData;

    if (evtFindTaskById() == 0) {
        evtFormatTaskName(taskId, taskName);
        taskData = sdfAllocSizeClassBlock(CAMP_TASK_DATA_BYTES);
        memset(taskData, 0, CAMP_TASK_DATA_BYTES);
        taskData->taskId = taskId;
        taskData->unk4 = 0;
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

typedef struct EvtBlendH {
    s32 w[4];
    s32 x;
    u32 flagWord;
    u8 pad[8];
    s32 y[3];
    s32 z[3];
} EvtBlendH;

/* Timeline keys carry type-dependent payloads as well as their common links. */
typedef struct CampKeyNode {
    u16 frame;                 /* 0x00 */
    u8 pad02[6];
    union {
        s16 offset;
        u16 packed;
    } firstValue;              /* 0x08 */
    s16 offsetB;               /* 0x0A */
    s16 condition;             /* 0x0C */
    u8 pad0E[2];
    s16 entryCode;             /* 0x10: 0 follows alt, 1 clears, >=2 names an entry */
    u8 pad12[0x1A];
    EvtBlendH *blendData;      /* 0x2C */
    struct CampKeyNode *next;  /* 0x30 */
    struct CampKeyNode *alt;   /* 0x34 */
} CampKeyNode;

/* The same track owns its key list, name/value state and next-track link. */
typedef struct CampKeyTrack {
    s32 type;                 /* 0x00 */
    u8 pad04[4];
    s32 nameIndex;             /* 0x08: 32-byte name in the owning scene */
    u8 pad0C[0x10];
    s16 base;                 /* 0x1C */
    u8 pad1E[6];
    s32 value;                /* 0x24 */
    u32 unk28;                /* Reset by fldResetCampSceneEntries; no reader here. */
    u8 pad2C[0x28];
    CampKeyNode *first;       /* 0x54 */
    CampKeyNode *fallback;    /* 0x58 */
    u8 pad5C[0x20];
    struct CampKeyTrack *next; /* 0x7C */
} CampKeyTrack;

typedef struct CampOwner {
    u8 pad00[0x104];
    s32 handle; /* 0x104 */
    u8 pad108[4];
    s32 bgmId; /* 0x10C: validated event BGM ID used with registered variations */
} CampOwner;

/* Event-viewer timeline data and the camp/shop state that owns those tracks. */
typedef struct {
    u8 pad00[8];
    CampOwner *owner; /* 0x08 */
    union {
        s32 whole;
        u16 low;
    } limitv; /* 0x0C */
    u8 pad10[4];
    s32 scrollOffset; /* 0x14: shifted by delta, wraps to 10 below zero */
    s32 clampedOffset; /* 0x18: cannot exceed the current limit */
    s32 unk1C;
    u8 pad20[4];
    char names[256][32]; /* 0x24: fixed-width names addressed by each track */
    u8 pad2024[0xC];
    s32 entryCount; /* 0x2030 */
    CampKeyTrack *entries; /* 0x2034 */
    u8 pad2038[0x2D0];
    CampKeyTrack *scrollTrack; /* 0x2308 */
    u8 pad230C[0x24];
    f32 transform[12]; /* 0x2330: three four-component vectors saved by shop */
    u8 pad2360[0x6C];
    s32 sceneMode; /* 0x23CC */
    u8 pad23D0[0x10];
    s32 pendingValue; /* 0x23E0 */
    u8 pad23E4[0x28];
    u32 state; /* 0x240C */
    u32 fontDrawHandle; /* 0x2410: font draw handle created by func_0019CE78 */
    u8 pad2414[0x14];
    s32 descriptorHandle; /* 0x2428: submitted to the drawing packet */
    s32 descriptorBackingHandle; /* 0x242C */
    u32 menuState; /* 0x2430 */
    s32 shopFlag;  /* 0x2434: 1 once the shop descriptor was submitted */
    u32 linkedHandle; /* 0x2438: passed to func_0025F130 */
    u32 optionFlags; /* 0x243C */
    u8 pad2440[4];
    s32 idCount; /* 0x2444 */
    s32 registeredIds[20]; /* 0x2448 */
} CampScene;

extern void evtBlendParamsH(s32 enable, f32 t, EvtBlendH *a, EvtBlendH *b, EvtBlendH *out);
extern void fldApplyCameraColorKeyWords(CampScene *scene, const s32 *colorSettings);

/* Shift the selected track's frames; values exactly at the upper limit are kept. */
void mnuShopScrollList(CampScene *scene, s32 delta) {
    CampKeyTrack *track = scene->scrollTrack;
    CampKeyNode *key;
    s32 absoluteFrame;

    if (track == NULL) {
        return;
    }
    for (key = track->first; key != NULL; key = key->next) {
        absoluteFrame = key->frame + track->base + delta;
        if (absoluteFrame < track->base) {
            key->frame = 0;
        } else if (scene->limitv.whole < absoluteFrame) {
            key->frame = (u16)scene->limitv.whole - (u16)track->base - 1;
        } else {
            key->frame = key->frame + delta;
        }
    }
}

/* Shift qualifying keys and their supported payload offsets. Preserve the
 * incoming base/offset/ubase arguments when no key overwrites them: the native
 * cleanup call receives those values even after an empty track traversal. */
void mnuFxWorldScrollDelta(CampScene *scene, s32 delta, s32 threshold, s32 base, s32 offset, s32 ubase) {
    CampKeyTrack *track;
    CampKeyNode *key;
    s32 absoluteFrame;

    if (scene->entryCount <= 0) {
        return;
    }
    if (scene->scrollOffset + delta < 0) {
        scene->scrollOffset = CAMP_TIMELINE_WRAP_FRAME;
    } else {
        scene->scrollOffset += delta;
    }
    if (scene->limitv.whole + delta < 0) {
        scene->limitv.whole = CAMP_TIMELINE_WRAP_FRAME;
    } else {
        scene->limitv.whole += delta;
    }
    if (scene->clampedOffset > scene->limitv.whole) {
        scene->clampedOffset = scene->limitv.whole;
    }
    track = scene->entries;
    while (track != NULL) {
        for (key = track->first; key != NULL; key = key->next) {
            offset = key->frame;
            base = track->base;
            ubase = (u16)track->base;
            absoluteFrame = offset + base;
            if (absoluteFrame < threshold) {
                continue;
            }
            absoluteFrame += delta;
            if (absoluteFrame < base) {
                key->frame = 0;
            } else if (scene->limitv.whole < absoluteFrame) {
                key->frame = scene->limitv.low - ubase - 1;
            } else {
                key->frame = offset + delta;
            }
            /* Payload bounds test the updated offset plus delta a second time. */
            switch (track->type) {
            case CAMP_TRACK_FIRST_OFFSET_TYPE:
                if (key->firstValue.offset != 0) {
                    key->firstValue.offset += delta;
                    if (key->firstValue.offset < 0) {
                        key->firstValue.offset = 0;
                    }
                    if (scene->limitv.whole < key->firstValue.offset + delta) {
                        key->firstValue.offset = scene->limitv.whole - 1;
                    }
                }
                break;
            case 3:
            case 0x14:
            case 0x15:
            case 0x1A:
                if (key->offsetB != 0) {
                    key->offsetB += delta;
                    if (key->offsetB < 0) {
                        key->offsetB = 0;
                    }
                    if (scene->limitv.whole < key->offsetB + delta) {
                        key->offsetB = scene->limitv.whole - 1;
                    }
                }
                break;
            }
        }
        track = track->next;
    }
    scene->unk1C -= 1;
    evtViewerCleanupMessageWindow(scene, delta, threshold, base, offset, ubase, track);
    evtViewerDispatchFlagMode(scene);
}

/* Process keys at or beyond threshold, restarting at the head after each call. */
void mnuFxWorldDropOutOfRange(CampScene *scene, s32 threshold) {
    CampKeyTrack *track;
    CampKeyNode *key;

    if (scene->entryCount <= 0) {
        return;
    }
    for (track = scene->entries; track != NULL; track = track->next) {
        key = track->first;
        while (key != NULL) {
            if (key->frame + track->base < threshold) {
                key = key->next;
            } else {
                func_00246950(scene, track, key);
                key = track->first;
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

INCLUDE_ASM(s32, "game/code_0025DA20", func_0025E048);

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

void mnuFindCampKeyTrackNeighbors(CampKeyTrack *track, s32 value, CampKeyNode **out1, CampKeyNode **out2) {
    s32 base;

    *out1 = 0;
    *out2 = 0;
    if (track == 0) {
        return;
    }
    base = track->base;
    *out2 = track->first;
    while (*out2 != 0) {
        if (value < (*out2)->frame + base) {
            break;
        }
        *out2 = (*out2)->next;
    }
    if (*out2 != 0) {
        *out1 = (*out2)->alt;
    } else {
        *out1 = track->fallback;
    }
    if (track->type == 2) {
        while (*out1 != 0 && evtViewerTestIndexedCondition((*out1)->condition) != 1) {
            *out1 = (*out1)->alt;
        }
    }
}

u32 func_0025E7B0(void) {
    return 0;
}

/* Split the key's packed halfword into its low 12 bits and upper four bits. */
void mnuUnpackNibbleFields(CampKeyNode *key, s32 *lowBitsOut, s32 *highBitsOut) {
    *lowBitsOut = key->firstValue.packed & CAMP_KEY_LOW_BITS_MASK;
    *highBitsOut = key->firstValue.packed >> CAMP_KEY_HIGH_BITS_SHIFT;
}

typedef struct CampNameLookup {
    u8 pad00[0x7C];
    char (*nameTable)[32]; /* 0x7C: fixed-width names indexed by nameIndex */
} CampNameLookup;

/* Return the scene's matching name index, or -1 when the track list has no match. */
s32 mnuCampFindMatchingEntryIndex(CampNameLookup *lookup, CampScene *scene, s32 nameIndex) {
    CampKeyTrack *track = scene->entries;
    while (track != NULL) {
        if (strcmp(scene->names[track->nameIndex],
                   lookup->nameTable[nameIndex]) == 0) {
            return track->nameIndex;
        }
        track = track->next;
    }
    return -1;
}

/* Return the first track with this fixed-width name, or NULL when absent. */
void *mnuCampFindEntryByName(CampScene *scene, const char *name) {
    CampKeyTrack *track = scene->entries;
    while (track != NULL) {
        if (strcmp(scene->names[track->nameIndex], name) == 0) {
            return track;
        }
        track = track->next;
    }
    return NULL;
}

/* Follow zero-coded alternatives, clear on code 1, otherwise resolve a name.
 * Keep the native signed code and separately retained halfword bits distinct. */
void campResolvePendingValue(CampScene *scene, CampKeyNode *cue) {
    CampKeyNode *linkedKey;
    s32 entryCode;
    u16 entryCodeBits;

    if (cue == NULL) {
        return;
    }
    entryCode = cue->entryCode;
    entryCodeBits = cue->entryCode;
    if (entryCode == CAMP_ENTRY_VALUE_CLEAR) {
        scene->pendingValue = 0;
        return;
    }
    if (entryCode == CAMP_ENTRY_VALUE_FOLLOW_ALT) {
        linkedKey = cue->alt;
        scene->pendingValue = 0;
        for (; ; linkedKey = linkedKey->alt) {
            s32 linkedEntryCode;

            if (linkedKey == NULL) {
                return;
            }
            linkedEntryCode = linkedKey->entryCode;
            if (linkedEntryCode != CAMP_ENTRY_VALUE_FOLLOW_ALT) {
                if (linkedEntryCode == CAMP_ENTRY_VALUE_CLEAR) {
                    scene->pendingValue = 0;
                    return;
                }
                scene->pendingValue = ((CampKeyTrack *)mnuCampFindEntryByName(scene, scene->names[linkedEntryCode - CAMP_ENTRY_NAME_CODE_BASE]))->value;
                return;
            }
        }
    } else {
        scene->pendingValue = ((CampKeyTrack *)mnuCampFindEntryByName(scene, scene->names[(s16)entryCodeBits - CAMP_ENTRY_NAME_CODE_BASE]))->value;
    }
}

void func_0025E980(SdfTex *texture, CampDisplayDefaults *display) {
    s32 halfWidth;
    s32 halfHeight;
    s32 variant;
    s32 packet;
    s32 surfaceIndex = 0x53;
    BufferDescriptor *surface;

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
        packet = sdfAllocPacketAligned(0x20);
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
                                          0xFF, (s32)texture, 0, packet);
        } else {
            itfQueueTextureBoundQuadPacket(D_003C9988, D_003C9998, D_003C99A8,
                                          0xFF, (s32)texture, 0, packet);
        }
        surface = &kwlnDrawSurfaces[surfaceIndex];
        surface->open(surface, packet);
    }
}

/* Clear each track's unknown word and the scene state; do not alter links or values. */
void fldResetCampSceneEntries(CampScene *scene) {
    CampKeyTrack *track;

    track = scene->entries;
    if (track != 0) {
        track->unk28 = 0;
        while (track = track->next, track != 0) {
            track->unk28 = 0;
        }
    }
    scene->state = 0;
}

/* Blend the first camera-color track at the clamped frame. Lookup includes
 * the track base, but interpolation retains the native unre-based key frames.
 * Without a future key, pass zero blend and the uninitialized fallback block. */
void func_0025EC00(CampScene *scene) {
    CampKeyTrack *track;
    CampKeyNode *selectedKey;
    CampKeyNode *futureKey;
    EvtBlendH fallbackBlend;
    EvtBlendH blendedParameters;
    EvtBlendH *futureBlend;
    s32 frameValue;
    u16 selectedFrame;
    u16 futureFrame;
    f32 blendFraction;

    selectedKey = NULL;
    futureKey = NULL;
    frameValue = scene->clampedOffset;
    if (scene->state == 1) {
        return;
    }
    track = scene->entries;
    while (track != NULL) {
        if (track->type == CAMP_CAMERA_COLOR_TRACK_TYPE) {
            mnuFindCampKeyTrackNeighbors(track, frameValue, &selectedKey, &futureKey);
            break;
        }
        track = track->next;
    }
    if (selectedKey == NULL) {
        scene->sceneMode = 0;
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
        futureBlend = futureKey->blendData;
    }
    evtBlendParamsH(selectedKey->condition, blendFraction, selectedKey->blendData, futureBlend, &blendedParameters);
    fldApplyCameraColorKeyWords(scene, (const s32 *)&blendedParameters);
}

extern void fldCopyCameraSetting(void *setting);
extern void fldUpdateCameraColorEffect(void *setting);

void fldApplyCameraColorKeyWords(CampScene *scene, const s32 *colorSettings) {
    s32 cameraSettings[0x54 / sizeof(s32)];
    u8 *cameraColorA;
    u8 *sourceColorA;
    u8 *cameraColorB;
    s32 sourceOffset;
    s32 cameraOffset;
    s32 i;

    fldCopyCameraSetting(cameraSettings);
    cameraSettings[0x20 / sizeof(s32)] = colorSettings[0x10 / sizeof(s32)];
    cameraSettings[0x10 / sizeof(s32)] = colorSettings[0];
    cameraSettings[0x14 / sizeof(s32)] = colorSettings[1];
    cameraSettings[0x18 / sizeof(s32)] = colorSettings[2];
    cameraSettings[0x1C / sizeof(s32)] = colorSettings[3];
    cameraSettings[0x0C / sizeof(s32)] = colorSettings[0x14 / sizeof(s32)];
    cameraColorA = (u8 *)cameraSettings + 8;
    sourceColorA = (u8 *)colorSettings + 0x0C;
    cameraColorB = (u8 *)cameraSettings + 0x0C;
    sourceOffset = 0x20;
    cameraOffset = 0x20;
    /* Copy the three color rows into the camera setting's 0x10-byte slots. */
    for (i = 2; i >= 0; i--) {
        *(s32 *)(cameraColorA + cameraOffset) =
            *(s32 *)(sourceColorA + sourceOffset);
        *(s32 *)(cameraColorB + cameraOffset) =
            *(s32 *)((u8 *)colorSettings + sourceOffset);
        sourceOffset += 4;
        cameraOffset += 0x10;
    }
    fldUpdateCameraColorEffect(cameraSettings);
    if (colorSettings[0x0C / sizeof(s32)] == 0 &&
        colorSettings[0x2C / sizeof(s32)] == 0 &&
        colorSettings[0x30 / sizeof(s32)] == 0 &&
        colorSettings[0x34 / sizeof(s32)] == 0) {
        scene->sceneMode = 0;
    } else {
        scene->sceneMode = 1;
    }
}

void func_0025EE00(CampScene *scene) {
    func_0025EC00(scene);
    if (scene->sceneMode == 1) {
        func_00137888();
        return;
    }
}

void mnuCampInitFontResource(CampScene *scene) {
    s32 fontHandle;
    scene->fontDrawHandle = 0;
    fontHandle = func_0019CE78(D_003C99B8, 0, 0, 0, 0);
    scene->fontDrawHandle = fontHandle;
    frFontSetContextPair(fontHandle, CAMP_FONT_CONTEXT_WIDTH, CAMP_FONT_CONTEXT_HEIGHT);
}

void mnuCampLinkFontGlyph(CampScene *scene) {
    frFontQueueGlyphInSelectedSlot(scene->fontDrawHandle);
    scene->fontDrawHandle = 0;
}

extern void sdfGetGeneralHeapStats(s32 *);

/* Retail keeps only the divide-by-zero check (break 7) of a division whose result is never used. */
void mnuCampCheckClockDivisor(void) {
    s32 heapStats[CAMP_HEAP_STATS_WORD_COUNT];
    s32 quotient;

    sdfGetGeneralHeapStats(heapStats);
    quotient = 1 / heapStats[0];
}

void mnuEnterCampSceneMenuState(CampScene *scene) {
    if ((scene->menuState == 0) || (scene->menuState == 5)) {
        scene->menuState = 1;
    }
}

extern void kwlnCreateHeldTextureBuffer(s32, s32, f32);
extern void func_0025EFD8(CampScene *scene);
extern void mnuShopSubmitDescriptor(u8 *work);
extern void func_0025F2B0(CampScene *scene);

/* Stages 1, 3, 4 and 5 fall through; stage 0 stops. All other stages,
 * including 2, advance once through the default arm. */
void mnuAdvanceShopMenuState(CampScene *scene) {
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


extern BufferDescriptor D_00380708;
extern u8 D_00380860[];
extern void *sdfAllocGeneralBlockHigh(s32 size);
extern s32 sdfAllocatePacketList(s32 (*alloc)(s32));
extern void sdfClearLinkedPacketList(void *list);
extern void sdfCreatePatchableResourcePacket(void *list, void *linkedList, s32 arg2, s32 arg3,
                                            s32 width, s32 height, void *resource, s32 arg7,
                                            s32 arg8, s32 (*alloc)(s32));
extern void sdfAppendPacketChainNode(void *head, void *node);

void func_0025EFD8(CampScene *scene) {
    s32 surface;
    s32 context;
    s32 handle;

    if (scene->descriptorBackingHandle == 0) {
        handle = (s32)sdfAllocGeneralBlockHigh(0x70000);
        scene->descriptorBackingHandle = handle;
        scene->descriptorHandle = (s32)sdfResourceRetainAddress(handle);
    }
    memset((void *)scene->descriptorHandle, 0x40, 0x70000);
    surface = sdfAllocatePacketList(0);
    context = sdfAllocPacketAligned(0x10);
    sdfClearLinkedPacketList((void *)context);
    sdfCreatePatchableResourcePacket((void *)surface, (void *)context, 0, 0, 0x200, 0xE0,
                                    (void *)scene->descriptorHandle, 0, 0, 0);
    sdfAppendPacketChainNode(D_00380860, (void *)context);
    D_00380708.open(&D_00380708, surface);
}

void mnuShopSubmitDescriptor(u8 *scene) {
    s32 drawPacket;

    if (((CampScene *)scene)->descriptorHandle != 0) {
        drawPacket = sdfAllocatePacketList(0);
        sdfCreateDescriptorPacket(drawPacket, (s32)((SdfTex *)kwlnHeldTextureReference)->primaryResource, 0, 0, CAMP_DESCRIPTOR_WIDTH, CAMP_DESCRIPTOR_HEIGHT, ((CampScene *)scene)->descriptorHandle, 0);
        D_00380708.open(&D_00380708, drawPacket);
    }
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F130);

void func_0025F2B0(CampScene *scene) {
    func_0025F130(scene->linkedHandle);
}

void func_0025F2C8(void) {
}

void mnuCampSetPrimaryOption(CampScene *scene, u32 option) {
    scene->optionFlags = (scene->optionFlags & CAMP_PRIMARY_OPTION_CLEAR_MASK) | (option & CAMP_OPTION_VALUE_MASK);
}

u32 mnuCampGetPrimaryOption(CampScene *scene) {
    return scene->optionFlags & CAMP_OPTION_VALUE_MASK;
}

void mnuCampSetSecondaryOption(CampScene *scene, u32 option) {
    scene->optionFlags = (scene->optionFlags & CAMP_SECONDARY_OPTION_CLEAR_MASK) | ((option & CAMP_OPTION_VALUE_MASK) << CAMP_SECONDARY_OPTION_SHIFT);
}

u32 mnuCampGetSecondaryOption(CampScene *scene) {
    return (scene->optionFlags & CAMP_SECONDARY_OPTION_MASK) >> CAMP_SECONDARY_OPTION_SHIFT;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F330);

/* Save the first and last four-component vectors; leave the middle unsaved. */
void mnuShopSavePrimaryTransform(u8 *scene) {
    s32 componentIndex;
    f32 *transformComponents = ((CampScene *)scene)->transform;
    for (componentIndex = 0; componentIndex < CAMP_TRANSFORM_COMPONENT_COUNT; componentIndex++) {
        mnuShopSavedLastTransformVector[componentIndex] = transformComponents[componentIndex + CAMP_TRANSFORM_LAST_START];
        mnuShopSavedFirstTransformVector[componentIndex] = transformComponents[componentIndex];
    }
    mnuShopRestoreMiddleVector = 0;
}

/* Save all three vectors and enable the middle-vector restore path. */
void mnuShopSaveFullTransform(u8 *scene) {
    s32 componentIndex;
    f32 *transformComponents = ((CampScene *)scene)->transform;
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
    f32 *transformComponents = ((CampScene *)scene)->transform;
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
void fldRegisterCampSceneId(CampScene *scene, s32 identifier) {
    s32 registeredCount = scene->idCount;
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
        ++scene->idCount;
    }
}

/* Queue the scene's BGM ID with each registered variation, then clear the list.
 * The loop bound is read again after each queue call, not cached up front. */
void mnuReleaseCampSceneRegisteredIds(CampScene *scene) {
    s32 processedCount = 0;
    if (scene->idCount > 0) {
        s32 *idCursor = scene->registeredIds;
        do {
            s32 identifier = *idCursor++;
            processedCount++;
            evtQueueValidatedBgmSoundCode(scene->owner->bgmId, identifier);
        } while (processedCount < scene->idCount);
    }
    scene->idCount = 0;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F640);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025F708);

typedef struct ShopEffectGraphics {
    u8 pad00[0x20];
    s32 *params; /* 0x20 */
} ShopEffectGraphics;

typedef struct ShopEffectObject {
    u8 pad00[8];
    ShopEffectGraphics *graphics; /* 0x08 */
} ShopEffectObject;

typedef struct ShopEffectScene {
    s32 resourceHandle; /* 0x00 */
    u8 pad04[0x74];
    s32 state;          /* 0x78 */
    u8 pad7C[8];
    u8 *objects[2];    /* 0x84 */
    u8 pad8C[0x10];
    s32 availableCount; /* 0x9C */
    u16 unkA0;
    u16 unkA2;
    u16 unkA4;
    u8 padA6[6];
    u32 options;        /* 0xAC */
    u8 padB0[0x34];
    s32 mode;           /* 0xE4 */
} ShopEffectScene;

void mnuInitializeShopStatusBatches(u8 *scene) {
    u8 *batchObject;
    u8 *batchGraphics;
    s32 *batchParameters;
    s32 initialParameter = CAMP_STATUS_INITIAL_PARAMETER;
    ((ShopEffectScene *)scene)->state = 0;
    batchObject = effCreateStatusBatch(6);
    batchGraphics = (u8 *)((ShopEffectObject *)batchObject)->graphics;
    ((ShopEffectScene *)scene)->objects[0] = batchObject;
    batchParameters = ((ShopEffectGraphics *)batchGraphics)->params;
    batchParameters[0] = initialParameter;
    batchParameters[1] = 0;
    batchParameters[2] = 0;
    batchParameters[3] = 0;
    batchParameters[4] = 0;
    batchObject = effCreateStatusBatch(1);
    batchGraphics = (u8 *)((ShopEffectObject *)batchObject)->graphics;
    ((ShopEffectScene *)scene)->objects[1] = batchObject;
    batchParameters = ((ShopEffectGraphics *)batchGraphics)->params;
    batchParameters[0] = initialParameter;
    batchParameters[1] = 0;
}

/* Destroy both batches and return the second destruction result. */
s32 mnuShopReleaseSceneObjects(u8 *scene) {
    s32 *batchCursor = (s32 *)((ShopEffectScene *)scene)->objects;
    s32 destroyResult;
    u32 batchIndex;
    for (batchIndex = 0; batchIndex < CAMP_STATUS_BATCH_COUNT; batchIndex++) {
        destroyResult = effDestroyPackedBatch(*batchCursor++);
    }
    return destroyResult;
}

typedef struct MapPacket MapPacket;

extern const CampMapArguments D_00424A90;
extern const CampEffectRows D_00424AC0;
extern const char D_00424AE0[];
extern u64 sdfReadNamedResource();
extern u32 effCreateMappedResource(u32);
extern void mnuInitializeMapPacket(u32, u32 *, s32, MapPacket *);
extern void mnuSetCampEffectResourceHandles(u32, u32, u32 *);
extern void mnuCopyCampEffectRowData(s32, s32);
extern void mnuOrEntryFlags(u32, u32 *);

void func_0025F8B8(u32 object, MapPacket *packet) {
    CampMapArguments mapArguments = D_00424A90;
    CampEffectRows rows = D_00424AC0;
    u32 dataAddress;
    u64 allocation;
    u32 mappedResource;

    allocation = sdfReadNamedResource(D_00424AE0, &dataAddress, 0);
    mappedResource = effCreateMappedResource(dataAddress);
    sdfReleaseResourceAllocation(allocation);
    mnuInitializeMapPacket(2, mapArguments.values, 11, packet);
    mnuSetCampEffectResourceHandles(object, mappedResource, (u32 *)packet);
    mnuCopyCampEffectRowData((s32)&rows, (s32)packet);
    mnuOrEntryFlags(7, (u32 *)packet);
}

/* The background-effect packet owns the packed animation created for it. */
typedef struct MenuEffectResources {
    u8 pad00[0x3C];
    u32 animationHandle;
} MenuEffectResources;

void mnuShopDestroyNestedEffectBatch(s32 object) {
    effDestroyPackedBatch(((MenuEffectResources *)object)->animationHandle);
}

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

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025FA28);

/* DDS2 shop scene fields used while choosing the message resource. */
typedef struct ShopMessageScene {
    u8 pad00[8];
    s32 messageSet;     /* 0x08 */
    u8 pad0C[0x54];
    u8 resourcePair[4]; /* 0x60 */
    s32 pairedHandle;   /* 0x64 */
    u8 pad68[0x24];
    s32 shopRow;        /* 0x8C */
} ShopMessageScene;

extern void evtLoadResourcePair();
extern void evtCreateMessageWindowIfMissing();

void mnuShopLoadMessageResource(ShopMessageScene *scene) {
    scene->messageSet = D_003C9A40[scene->shopRow].messageSet;
    switch (scene->messageSet) {
    case 0:
        evtLoadResourcePair("/facility/msg/shop/mes_01.bmd", scene->resourcePair);
        break;
    case 1:
        evtLoadResourcePair("/facility/msg/shop/mes_02.bmd", scene->resourcePair);
        break;
    case 2:
        evtLoadResourcePair("/facility/msg/shop/mes_03.bmd", scene->resourcePair);
        break;
    case 3:
        evtLoadResourcePair("/facility/msg/shop/mes_04.bmd", scene->resourcePair);
        break;
    }
    evtCreateMessageWindowIfMissing(scene->pairedHandle);
}

typedef struct ShopSceneCleanup {
    u8 pad00[8];
    s32 mode;             /* 0x08 */
    u8 pad0C[0x5C];
    s32 handles[4];       /* 0x68 through 0x74 */
    u8 pad78[0x300];
    s32 resourceHandle;   /* 0x378 */
} ShopSceneCleanup;

void mnuShopReleaseWindowAndEffectResources(s32 scene) {
    s32 i;
    s32 *slot = ((ShopSceneCleanup *)scene)->handles;

    mnuReleaseWindowTextures(((ShopSceneCleanup *)scene)->resourceHandle);
    for (i = 1; i >= 0; i--) {
        effDestroyResourceSlotSet(*slot++);
    }
    effDestroyResourceSlotSet(((ShopSceneCleanup *)scene)->handles[2]);
    effDestroyResourceSlotSet(((ShopSceneCleanup *)scene)->handles[3]);
    switch (((ShopSceneCleanup *)scene)->mode) {
    case 1:
    case 3:
        mnuShopDestroyNestedEffectBatch(scene + 0x210);
        break;
    case 0:
    case 2:
        break;
    default:
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_0025FD78);

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
        if (((CampSaveState *)datGameState)->counts[i] == 0) {
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

typedef struct CampWindowContainer CampWindowContainer;

/* The first payload word is list-specific (entry index or displayed value). */
typedef struct CampWindowParams {
    s32 value;
    s32 id;
    s32 price; /* Base price used for affordability and quantity-dependent discounts. */
    s32 mode;
} CampWindowParams;

typedef struct CampWindowNode {
    u8 pad00[0x48];
    u32 flags;
    u8 pad4C[0xC];
    struct CampWindowNode *next; /* 0x58 */
    u8 pad5C[4];
    CampWindowParams params; /* 0x60 */
} CampWindowNode;

typedef struct CampWindowBuffer {
    u8 pad00[0xC];
    u16 unk0C;
    u16 unk0E;
    u16 unk10;
    u8 pad12[2];
} CampWindowBuffer;

typedef struct CampWindowListData {
    u8 pad00[0x10];
    CampWindowNode *first;
    u8 pad14[8];
    CampWindowNode *cursor; /* 0x1C */
    s32 count;
    u8 pad24[8];
    s32 (*callback)();
    void *buffer;
} CampWindowListData;

struct CampWindowContainer {
    u8 pad00[0x18];
    CampWindowListData *list;
};

struct MenuWindowContainer;
struct MenuListNode;
extern struct MenuListNode *mnuAppendWindowListNode(struct MenuWindowContainer *window, s32 value);
extern void func_00295400(void);

s32 mnuCreateEnabledCampEntryWindow(s32 count, s32 *enabled, u8 *settings) {
    CampWindowContainer *window;
    CampWindowBuffer *storage;
    s32 i;

    window = (CampWindowContainer *)mnuCreateWindowContainer(0, 0x260, 0x10, count, 0x16);
    mnuSetWindowEntryParameters(0, window, 0, 8, 0xA);
    for (i = 0; i < count; i++) {
        if (enabled[i] != 0) {
            ((CampWindowNode *)mnuAppendWindowListNode((struct MenuWindowContainer *)window, 0))
                ->params.value = i;
        }
    }
    window->list->callback = func_00295400;
    storage = sdfAllocSizeClassBlock(0x14);
    memset(storage, 0, 0x14);
    window->list->buffer = storage;
    storage->unk0C = ((ShopEffectScene *)settings)->unkA0;
    storage->unk0E = ((ShopEffectScene *)settings)->unkA2;
    storage->unk10 = ((ShopEffectScene *)settings)->unkA4;
    return (s32)window;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260020);

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
    u8 *slotTable = D_003CBB70;
    s16 *flagId = (s16 *)(slotTable + CAMP_SLOT_LAST_ROW_OFFSET);
    for (rowIndex = CAMP_SLOT_LAST_ROW_INDEX; rowIndex >= 0; rowIndex--, flagId = (s16 *)((u8 *)flagId - CAMP_SLOT_ROW_STRIDE)) {
        if (*flagId > 0 && mdlFlagTest(*flagId)) {
            return rowIndex;
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00260250);

typedef struct ShopScene {
    u8 pad00[0x7C];
    CampWindowContainer *sprites[1]; /* 0x7C */
    CampWindowContainer *extra;      /* 0x80 */
    u8 pad84[8];
    s32 stockGroup;         /* 0x8C */
    s32 quantity;           /* 0x90: clamped to [1, maximum] */
    u8 pad94[0x33];
    u8 atLimit;             /* 0xC7 */
} ShopScene;

void mnuShopReleaseWindowSprites(s32 keepExtra, ShopScene *scene) {
    CampWindowContainer **slot = scene->sprites;
    CampWindowContainer *sprite;
    u32 i;

    for (i = 0; i < 1; i++) {
        sprite = *slot;
        if (sprite != NULL) {
            if (sprite->list->buffer != NULL) {
                sdfReleaseChipBlock(sprite->list->buffer);
                sprite = *slot;
                sprite->list->buffer = NULL;
            }
            mnuDestroyWindowContainer(sprite);
        }
        slot++;
    }
    if (keepExtra == 0) {
        if (scene->extra != NULL) {
            if (scene->extra->list->buffer != NULL) {
                sdfReleaseChipBlock(scene->extra->list->buffer);
                scene->extra->list->buffer = NULL;
            }
            mnuDestroyWindowContainer(scene->extra);
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
    u16 *entryFlagsCursor;
    s32 entryCountdown;
    s32 enabledCount;

    enabledCount = 0;
    entryCountdown = CAMP_PARTY_SCAN_LAST;
    entryFlagsCursor = (u16 *)(datGameState + CAMP_PARTY_FLAGS_OFFSET);
    do {
        entryFlags = *entryFlagsCursor;
        entryFlagsCursor = entryFlagsCursor + CAMP_PARTY_HALFWORD_STRIDE;
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
            if (D_003CE408[i].threshold > ((CampSaveState *)datGameState)->unk1E650) {
                break;
            }
        } else if (D_003CE408[i].threshold <= ((CampSaveState *)datGameState)->unk1E650) {
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
        ((CampSaveState *)datGameState)->counts[(u32)entryId] = 0;
    } while (index < 3);
}

u8 *mnuTerminalCreateContext(void) {
    s32 handle;
    u8 *obj;

    handle = sdfAllocGeneralBlock(0x38C);
    obj = sdfResourceRetainAddress(handle);
    memset(obj, 0, 0x38C);
    ((ShopEffectScene *)obj)->resourceHandle = handle;
    mnuClearPanelTransitionState(obj + 0xC);
    mnuInitializeShopStatusBatches(obj);
    ((ShopEffectScene *)obj)->options = func_00260460();
    ((ShopEffectScene *)obj)->availableCount = mnuCountActivePartyEntries();
    ((ShopEffectScene *)obj)->mode = 0xF;
    func_002945B8(obj);
    mnuResetGradientFadeColor(obj + 0x37C, 0x60);
    mnuCampClearListedItemCounts();
    sndEnsureMidiBankResident(0x300000);
    sndStartTrackExtended(0x300000);
    return obj;
}

void mnuTerminalReleaseContextAndResources(s32 arg) {
    s32 scene = kwlnTaskGetUserValue();

    if (scene != 0) {
        mnuShopReleaseWindowSprites(0, scene);
        mnuShopReleaseWindowAndEffectResources(scene);
        mnuShopReleaseSceneObjects((u8 *)scene);
        mnuDrainPanelTransitions(scene + 0xC, arg);
        dspCloseChannel();
        evtReleaseResourcePairHandle(scene + 0x60);
        sdfReleaseResourceAllocation(*(s32 *)scene);
        mnuPanelTaskCompletionState = 2;
    }
}

s32 mnuTerminalSyncMessageWindowControl(void) {
    s32 state = kwlnTaskGetUserValue() + 0x37C;
    mnuDrawAndStepGradientFade(state, 0x53);
    if (evtGetMessageWindowControlState() != 0) {
        func_002C1B68(state, 1);
    } else {
        func_002C1B68(state, 0);
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424BA0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424BC0);

INCLUDE_RODATA(const s32, "game/code_0025DA20", D_00424BD0);

void func_00260708(const s32 *stockGroup) {
    ShopScene *scene = (ShopScene *)mnuTerminalCreateContext();
    KwlnTask *drawTask;
    KwlnTask *fadeTask;

    if (stockGroup != NULL) {
        scene->stockGroup = *stockGroup;
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

static inline s32 campSetHandler(s32 context, u64 mode, s32 callback) {
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), mode, callback);
}

s32 mnuPreparePopupAndDispatchSelection(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    mnuSetPopupEntry((s32 *)(context + 0x58), D_003CE658);
    return campSetHandler(context, 0, callback);
}

s32 mnuAdvanceCampPopup(s32 callback) {
    return campSetHandler(kwlnTaskGetUserValue(), 1, callback);
}

s32 mnuFinishCampPopup(s32 callback) {
    return campSetHandler(kwlnTaskGetUserValue(), 2, callback);
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

    if (entry[1] == 0 && func_002C54B0(id) != 0 && ((CampSaveState *)datGameState)->counts[id] != 0) {
        id = *(u16 *)(entry + 8);
    }
    return id;
}

s32 mnuCampGetCompactEntryId(s32 row, s32 column) {
    return *(s32 *)(mnuCampCompactEntries + row * 0x44 + column * 8);
}

s32 mnuCampCountRemainingUses(s32 mode, s32 id, s32 record) {
    s32 value = ((ShopEffectScene *)record)->availableCount;

    if (mode == 1) {
        value -= ptyCountBulletItem(id);
    } else if (mode == 3) {
        value = 1 - ((CampSaveState *)datGameState)->counts[id];
    } else if (mode == 2) {
        value = 1 - ((CampSaveState *)datGameState)->counts[id];
    } else {
        value = 99 - ((CampSaveState *)datGameState)->counts[id];
    }
    if (mnuCampFindListedItemIndex(id) >= 0) {
        if (value >= 2) {
            value = ((CampSaveState *)datGameState)->counts[id] == 0;
        }
    }
    return value < 0 ? 0 : value;
}

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261198);

INCLUDE_ASM(const s32, "game/code_0025DA20", func_00261290);

/* Mark rows unavailable when neither currency nor inventory capacity permits a use. */
void mnuCampDisableUnavailableItemEntries(u8 *scene) {
    CampWindowNode *node;
    CampWindowParams *item;
    s32 i;
    s32 count;
    s32 remaining;

    node = ((ShopScene *)scene)->extra->list->first;
    for (i = 0; i < ((ShopScene *)scene)->extra->list->count; i++) {
        item = &node->params;
        /* Keep the unchecked division: retail traps when the row price is zero. */
        count = ((CampSaveState *)datGameState)->money / item->price;
        remaining = mnuCampCountRemainingUses(item->mode, item->id, (s32)scene);
        if (remaining < count) {
            count = remaining;
        }
        if (count == 0) {
            node->flags = 1;
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
        price = price * D_003CE150[*(s32 *)(datGameState + 0x1E658)].percent / 100;
    }
    return price;
}

s32 mnuCampAdvanceCounter(s32 delta, u8 *scene) {
    s32 max = func_00261198(scene);
    CampWindowParams *slot = &((ShopScene *)scene)->extra->list->cursor->params;
    s32 sum = ((ShopScene *)scene)->quantity + delta;
    s32 cur;

    ((ShopScene *)scene)->quantity = sum;
    if (sum <= 0) {
        ((ShopScene *)scene)->quantity = 1;
    }
    cur = ((ShopScene *)scene)->quantity;
    if (cur >= max) {
        ((ShopScene *)scene)->atLimit = 1;
        ((ShopScene *)scene)->quantity = max;
        cur = max;
    } else {
        ((ShopScene *)scene)->atLimit = 0;
    }
    if (((ShopScene *)scene)->sprites[0]->list->cursor->params.value == 3) {
        slot->value = func_002613C8(cur, slot->price);
        cur = ((ShopScene *)scene)->quantity;
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

s32 func_00261538(ShopScene *scene) {
    s32 rowIndex;
    u8 *entry;
    CampWindowContainer *window;
    u32 i;

    rowIndex = mnuCampFindActiveSlot();
    if (rowIndex == -1) {
        return 0;
    }
    entry = D_003CBB70 + rowIndex * 0x104;
    for (i = 0; i < 0x20; i++, entry += 8) {
        u32 id = *(u16 *)(entry + 4);
        u32 mode = entry[6];
        CampWindowNode *node;
        CampWindowParams *params;
        s32 price;

        if (mnuIsBulletItemId(id) != 0 || id == 0) {
            continue;
        }
        node = (CampWindowNode *)mnuAppendWindowListNode((struct MenuWindowContainer *)scene->extra,
            (s32)(D_00435E5C + id * 0x19));
        func_002B9808((struct MenuWindowContainer *)scene->extra);
        window = scene->extra;
        mnuAdvanceListCursorDefault(window->list);
        params = &node->params;
        price = func_00260A58(i, 0);
        params->id = id;
        params->value = price;
        params->price = price;
        params->mode = mode;
        if (func_00261198(scene) == 0) {
            node->flags = 1;
        }
    }
    mnuSelectFirstListNode(scene->extra->list);
    return 0;
}

s32 func_00261850(ShopScene *scene, s32 filterMode);

s32 func_00261670(ShopScene *scene) {
    CampWindowBuffer *buffer;
    u8 *entry;
    u32 i;
    s32 row;

    if (scene->extra != NULL) {
        if (scene->extra->list->buffer != NULL) {
            sdfReleaseChipBlock(scene->extra->list->buffer);
            scene->extra->list->buffer = NULL;
        }
        mnuDestroyWindowContainer(scene->extra);
    }
    row = mnuCampResolveFlagRowValue(scene->stockGroup);
    scene->extra = (CampWindowContainer *)mnuCreateWindowContainer(1, 0x260, 0x10, 8, 0x16);
    /* Each packed stock row spans 0xC1 halfwords. */
    entry = D_003C9BC4 + (row << 8) + (((row << 6) + row) << 1);
    for (i = 0; i < 0x60; i++, entry += 4) {
        u32 id = *(u16 *)(entry - 2);
        u32 mode = entry[0];
        CampWindowNode *node;
        CampWindowParams *params;
        s32 price;

        if (mnuIsBulletItemId(id) != 0 || func_002C54B0(id) != 0 || id == 0) {
            continue;
        }
        node = (CampWindowNode *)mnuAppendWindowListNode((struct MenuWindowContainer *)scene->extra,
            (s32)(D_00435E5C + id * 0x19));
        func_002B9808((struct MenuWindowContainer *)scene->extra);
        mnuAdvanceListCursorDefault(scene->extra->list);
        params = &node->params;
        price = mnuCampGetSourceItemPrice(i, scene->stockGroup, 0);
        params->value = price;
        params->id = id;
        params->price = price;
        params->mode = mode;
        if (func_00261198(scene) == 0) {
            node->flags = 1;
        }
    }
    func_00261538(scene);
    mnuSelectFirstListNode(scene->extra->list);
    scene->extra->list->callback = func_002958B0;
    buffer = sdfAllocSizeClassBlock(0x14);
    memset(buffer, 0, 0x14);
    scene->extra->list->buffer = buffer;
    return scene->extra->list->count;
}

s32 func_00261850(ShopScene *scene, s32 filterMode) {
    s32 rowIndex;
    u8 *entry;
    u32 i;

    rowIndex = mnuCampFindActiveSlot();
    if (rowIndex == -1) {
        return 0;
    }
    entry = D_003CBB70 + rowIndex * 0x104;
    for (i = 0; i < 0x20; i++, entry += 8) {
        u32 id = *(u16 *)(entry + 4);
        u32 mode = entry[6];
        CampWindowNode *node;
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
        node = (CampWindowNode *)mnuAppendWindowListNode((struct MenuWindowContainer *)scene->extra,
            (s32)(D_00435E5C + id * 0x19));
        func_002B9808((struct MenuWindowContainer *)scene->extra);
        mnuAdvanceListCursorDefault(scene->extra->list);
        params = &node->params;
        price = func_00260A58(i, 0);
        params->value = price;
        params->id = id;
        params->price = price;
        params->mode = mode;
        if (func_00261198(scene) == 0) {
            node->flags = 1;
        }
    }
    mnuSelectFirstListNode(scene->extra->list);
    return 0;
}

s32 func_002619A8(ShopScene *scene, s32 filterMode) {
    CampWindowBuffer *buffer;
    CampWindowNode *node;
    u8 *entry;
    u32 i;
    s32 row;

    if (scene->extra != NULL) {
        if (scene->extra->list->buffer != NULL) {
            sdfReleaseChipBlock(scene->extra->list->buffer);
            scene->extra->list->buffer = NULL;
        }
        mnuDestroyWindowContainer(scene->extra);
    }
    row = mnuCampResolveFlagRowValue(scene->stockGroup);
    scene->extra = (CampWindowContainer *)mnuCreateWindowContainer(1, 0x260, 0x10, 8, 0x16);
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
        node = (CampWindowNode *)mnuAppendWindowListNode((struct MenuWindowContainer *)scene->extra,
            (s32)(D_00435E5C + id * 0x19));
        func_002B9808((struct MenuWindowContainer *)scene->extra);
        mnuAdvanceListCursorDefault(scene->extra->list);
        params = &node->params;
        price = mnuCampGetSourceItemPrice(i, scene->stockGroup, 0);
        params->value = price;
        params->id = id;
        params->price = price;
        params->mode = mode;
        if (func_00261198(scene) == 0) {
            node->flags = 1;
        }
    }
    func_00261850(scene, filterMode);
    mnuSelectFirstListNode(scene->extra->list);
    scene->extra->list->callback = func_002958B0;
    buffer = sdfAllocSizeClassBlock(0x14);
    memset(buffer, 0, 0x14);
    scene->extra->list->buffer = buffer;
    return scene->extra->list->count;
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

