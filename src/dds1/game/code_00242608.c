#include "mnu.h"
#include "mnu_list.h"
#include "mnu_shop.h"
#include "sdf.h"
#include "evt_unit.h"
#include "dat_state.h"
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

#define CAMP_REGISTERED_ID_LIMIT 10
#define CAMP_STATUS_BATCH_COUNT 2
#define CAMP_STATUS_INITIAL_PARAMETER 15
#define CAMP_SLOT_LAST_ROW_INDEX 4
#define CAMP_SLOT_LAST_ROW_OFFSET 0x410
#define CAMP_SLOT_ROW_STRIDE 0x104
#define CAMP_PARTY_SCAN_LAST 4
#define CAMP_PARTY_ACTIVE_FLAG 1
#define CAMP_HEAP_STATS_WORD_COUNT 8


extern s32 sdfAllocGeneralBlock(s32);
extern u8 *sdfResourceRetainAddress(s32);
extern void mnuClearPanelTransitionState(u8 *);
extern void evtLoadResourcePair(const char *, u8 *);
extern s32 evtCreateMessageWindowIfMissing(s32);
extern s32 func_00244848();
extern s32 D_003BC520;
extern s32 itfMesGetWindowEntryItems(s32, s32);
extern void mnuUnpackNibbleFields();

extern u8 D_00368C40[];

extern s32 ptyCountBulletItem(s32);

extern s8 D_003BC39C;

extern s32 kwlnTaskFindByPriority(u32);

extern s64 evtFindTaskById(void);


extern s32 kwlnTaskGetUserValue();

extern char D_003BC3A0[]; /* "camp" */

extern char D_003AF418[]; /* "camp_draw" */

extern char D_003AF428[]; /* "camp_update" */

extern void evtFormatTaskName(s32 taskId, void *name);
extern void *sdfAllocSizeClassBlock(s32 size);
extern void *memset(void *dst, s32 c, u32 n);
extern s32 kwlnTaskCreate(void *name, s32 priority, s32 group, s32 flags, void *update, void *destroy, void *data);
extern f32 mnuShopSavedLastTransformVector[];
extern f32 mnuShopSavedMiddleTransformVector[];
extern f32 mnuShopSavedFirstTransformVector[];
extern s32 mnuShopRestoreMiddleVector;
extern s32 evtQueueValidatedBgmSoundCode(s32, s32);
extern u8 *effCreateStatusBatch(s32 kind);
extern s32 effDestroyPackedBatch(s32);
extern s32 D_0036AA60[];
extern s32 effLoadIndexedResource(const char *, s32, s32);

#define CAMP_TASK_PRIORITY 0x3EC


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

    while (taskHandle = kwlnTaskFindByPriority(CAMP_TASK_PRIORITY), taskHandle != 0) {
        kwlnTaskDestroyWithHierarchy(taskHandle, 0);
    }
}


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
    EvtBlendKey *blendData;   /* 0x2C */
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
typedef struct CampScene {
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
    s32 sceneMode; /* 0x23CC: checked after func_00243818 */
    u8 pad23D0[0x10];
    s32 pendingValue; /* 0x23E0 */
    u8 pad23E4[0x28];
    u32 state; /* 0x240C */
    u32 fontResource; /* 0x2410: returned by func_001951C8 */
    u8 pad2414[0x14];
    s32 descriptorResource; /* 0x2428 */
    s32 descriptorBackingHandle; /* 0x242C */
    s32 menuState; /* 0x2430 */
    s32 shopFlag;  /* 0x2434: 1 once the shop descriptor was submitted */
    u32 auxResource; /* 0x2438 */
    u32 options; /* 0x243C: two two-bit fields */
    u8 pad2440[4];
    s32 registeredCount; /* 0x2444 */
    s32 registeredIds[10]; /* 0x2448 */
} CampScene;

extern void fldApplyCameraColorKeyWords(CampScene *scene, const EvtBlendKey *source);

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

extern void func_0022BFD8();

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
                func_0022BFD8(scene, track, key);
                key = track->first;
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002429F0);

void mnuInitializeCampPanelVisualDefaults(f32 *a, f32 *b, f32 *c, f32 *d, f32 *e) {
    a[0] = 0.7f;
    a[1] = 0.7f;
    a[2] = 0.7f;
    a[3] = 0.0f;
    b[0] = 0.65f;
    b[1] = 0.39f;
    b[2] = 0.65f;
    b[3] = 0.0f;
    c[0] = 0.2f;
    c[1] = 0.2f;
    c[2] = 0.2f;
    c[3] = 1.0f;
    d[0] = 7.0f;
    *e = 0.0f;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00242C30);

extern s32 strcmp(const char *a, const char *b);

extern s32 sdfAllocPacketAligned(s32 size);
extern void sdfInitPacketList(SdfListHead *packet);
extern void itfSendTablePacket(SdfListHead *packet, s32 table, s32 mode);
extern void itfQueueTextureBoundQuadPacket(void *, void *, void *, s32, SdfTex *, s32, SdfListHead *);
extern s32 D_00368BA8[4];
extern s32 D_00368BB8[4];
extern s32 D_00368BC8[4];
extern SdfPoolNode D_003255A8;

typedef struct CampDisplayDefaults {
    s32 x;
    s32 y;
    u8 color[4];
    f32 scaleY;
    f32 scaleX;
    s32 enabled;
    s32 variant;
} CampDisplayDefaults;

void mnuCampInitDisplayDefaults(CampDisplayDefaults *display) {
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
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00242E70);

typedef struct CampListLayout {
    s32 width0;
    s32 width1;
    s32 width2;
    s32 unkC;               /* 0xC: layout default, no reader in this unit */
    s32 unk10;              /* 0x10: layout default, no reader in this unit */
    s32 unk14;              /* 0x14: layout default, no reader in this unit */
    u8 pad18[8];
    s32 unk20;              /* 0x20: layout default, no reader in this unit */
    s32 unk24;              /* 0x24: layout default, no reader in this unit */
    s32 unk28;              /* 0x28: layout default, no reader in this unit */
    s32 unk2C;              /* 0x2C: layout default, no reader in this unit */
    s32 unk30;              /* 0x30: layout default, no reader in this unit */
    s32 unk34;              /* 0x34: layout default, no reader in this unit */
} CampListLayout;

void mnuInitializeCampListLayoutDefaults(CampListLayout *layout) {
    layout->width0 = 0x96;
    layout->width1 = 0x96;
    layout->unk10 = 0x50;
    layout->width2 = 0x96;
    layout->unkC = 0x1E;
    layout->unk14 = 1;
    layout->unk20 = 7;
    layout->unk24 = 4;
    layout->unk28 = 0xA;
    layout->unk2C = 0x20;
    layout->unk30 = 0x10;
    layout->unk34 = 0x10;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00242F78);

INCLUDE_ASM(const s32, "game/code_00242608", func_00243048);

extern s32 evtViewerTestIndexedCondition(u32);

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

/* Return 1 if a type-4 key's low 12-bit index yields 1 from the owner's item query. */
s32 campAnyPackedFlagSet(CampScene *scene) {
    CampKeyTrack *node;
    CampKeyNode *child;
    s32 low;
    s32 high;

    for (node = scene->entries; node != NULL; node = node->next) {
        if (node->type == 4) {
            for (child = node->first; child != NULL; child = child->next) {
                mnuUnpackNibbleFields(child, &low, &high);
                if (itfMesGetWindowEntryItems(scene->owner->handle, low) == 1) {
                    return 1;
                }
            }
        }
    }
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

void mnuDrawCampScaledTexture(SdfTex *texture, CampDisplayDefaults *display) {
    s32 halfWidth;
    s32 halfHeight;
    s32 variant;
    SdfListHead *packetList;

    D_00368BB8[2] = texture->width << 4;
    D_00368BB8[3] = texture->height << 4;
    halfWidth = (s32)(texture->width * display->scaleX) / 2;
    halfHeight = (s32)(texture->height * display->scaleY) / 2;
    D_00368BA8[0] = (display->x - halfWidth) << 4;
    D_00368BA8[1] = (display->y - halfHeight) << 3;
    D_00368BA8[2] = (display->x + halfWidth) << 4;
    D_00368BA8[3] = (display->y + halfHeight) << 3;
    D_00368BC8[0] = display->color[0];
    D_00368BC8[1] = display->color[1];
    D_00368BC8[2] = display->color[2];
    D_00368BC8[3] = display->color[3];
    variant = display->variant;
    if (display->color[3] != 0) {
        packetList = (SdfListHead *)sdfAllocPacketAligned(0x20);
        sdfInitPacketList(packetList);
        if (variant == 0) {
            itfSendTablePacket(packetList, 0, 0);
        } else if (variant == 1) {
            itfSendTablePacket(packetList, 1, 0);
        } else if (variant == 2) {
            itfSendTablePacket(packetList, 2, 0);
        }
        itfQueueTextureBoundQuadPacket(D_00368BA8, D_00368BB8, D_00368BC8,
                                      0xFF, texture, 0, packetList);
        D_003255A8.append((SdfListHead *)&D_003255A8, packetList);
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
void func_00243818(CampScene *scene) {
    CampKeyTrack *track;
    CampKeyNode *selectedKey;
    CampKeyNode *futureKey;
    EvtBlendKey fallbackBlend;
    EvtBlendKey blendedParameters;
    EvtBlendKey *futureBlend;
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
    fldApplyCameraColorKeyWords(scene, &blendedParameters);
}


/* The genuine three-row typed candidate remains non-matching. */
INCLUDE_ASM(const s32, "game/code_00242608", fldApplyCameraColorKeyWords);

void func_00243A18(CampScene *scene) {
    func_00243818(scene);
    if (scene->sceneMode == 1) {
        func_00134CD8();
        return;
    }
}

extern s32 D_00368BD8[];
extern s32 func_001951C8(s32 *resources, s32, s32, s32, s32);
extern void frFontSetContextPair(s32 resource, s32 width, s32 height);

void mnuCampInitFontResource(CampScene *scene) {
    s32 fontHandle;
    scene->fontResource = 0;
    fontHandle = func_001951C8(D_00368BD8, 0, 0, 0, 0);
    scene->fontResource = fontHandle;
    frFontSetContextPair(fontHandle, CAMP_FONT_CONTEXT_WIDTH, CAMP_FONT_CONTEXT_HEIGHT);
}

void mnuCampLinkFontGlyph(CampScene *scene) {
    frFontQueueGlyphInSelectedSlot(scene->fontResource);
    scene->fontResource = 0;
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

extern s32 kwlnHeldTextureReference;
extern void kwlnCreateHeldTextureBuffer(s32, s32, f32);
extern void func_00243BF0(CampScene *scene);
extern void mnuShopSubmitDescriptor(CampScene *scene);
extern void func_00243EC8(CampScene *scene);

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
        func_00243BF0(scene);
        scene->menuState = scene->menuState + 1;
    case 4:
        mnuShopSubmitDescriptor(scene);
        scene->menuState = scene->menuState + 1;
    case 5:
        if (scene->shopFlag == 1) {
            func_00243EC8(scene);
        }
    case 0:
        break;
    default:
        scene->menuState = scene->menuState + 1;
        break;
    }
}


extern SdfPoolNode D_00325708;
extern u8 D_00325860[];
extern void *sdfAllocGeneralBlockHigh(s32 size);
extern s32 sdfAllocatePacketList(s32 (*alloc)(s32));
extern void sdfClearLinkedPacketList(void *list);
extern void sdfCreatePatchableResourcePacket(void *list, void *linkedList, s32 arg2, s32 arg3,
                                            s32 width, s32 height, void *resource, s32 arg7,
                                            s32 arg8, s32 (*alloc)(s32));
extern void sdfAppendPacketChainNode(void *head, void *node);
extern void sdfCreateDescriptorPacket();

void func_00243BF0(CampScene *scene) {
    s32 surface;
    s32 context;
    s32 handle;

    if (scene->descriptorBackingHandle == 0) {
        handle = (s32)sdfAllocGeneralBlockHigh(0x70000);
        scene->descriptorBackingHandle = handle;
        scene->descriptorResource = (s32)sdfResourceRetainAddress(handle);
    }
    memset((void *)scene->descriptorResource, 0x40, 0x70000);
    surface = sdfAllocatePacketList(0);
    context = sdfAllocPacketAligned(0x10);
    sdfClearLinkedPacketList((void *)context);
    sdfCreatePatchableResourcePacket((void *)surface, (void *)context, 0, 0, 0x200, 0xE0,
                                    (void *)scene->descriptorResource, 0, 0, 0);
    sdfAppendPacketChainNode(D_00325860, (void *)context);
    D_00325708.append((SdfListHead *)&D_00325708, (SdfListHead *)surface);
}

void mnuShopSubmitDescriptor(CampScene *scene) {
    s32 drawPacket;

    if (scene->descriptorResource != 0) {
        drawPacket = sdfAllocatePacketList(0);
        sdfCreateDescriptorPacket(drawPacket, (s32)((SdfTex *)kwlnHeldTextureReference)->primaryResource, 0, 0, CAMP_DESCRIPTOR_WIDTH, CAMP_DESCRIPTOR_HEIGHT, scene->descriptorResource, 0);
        D_00325708.append((SdfListHead *)&D_00325708, (SdfListHead *)drawPacket);
    }
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00243D48);

void func_00243EC8(CampScene *scene) {
    func_00243D48(scene->auxResource);
}

void func_00243EE0(void) {
}

void mnuCampSetPrimaryOption(CampScene *scene, u32 option) {
    scene->options = (scene->options & CAMP_PRIMARY_OPTION_CLEAR_MASK) | (option & CAMP_OPTION_VALUE_MASK);
}

u32 mnuCampGetPrimaryOption(CampScene *scene) {
    return scene->options & CAMP_OPTION_VALUE_MASK;
}

void mnuCampSetSecondaryOption(CampScene *scene, u32 option) {
    scene->options = (scene->options & CAMP_SECONDARY_OPTION_CLEAR_MASK) | ((option & CAMP_OPTION_VALUE_MASK) << CAMP_SECONDARY_OPTION_SHIFT);
}

u32 mnuCampGetSecondaryOption(CampScene *scene) {
    return (scene->options & CAMP_SECONDARY_OPTION_MASK) >> CAMP_SECONDARY_OPTION_SHIFT;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00243F48);

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
void mnuShopRegisterSceneObject(CampScene *scene, s32 identifier) {
    s32 registeredCount = scene->registeredCount;
    s32 idIndex = 0;
    if (registeredCount > 0) {
        s32 *idCursor = scene->registeredIds;
        do {
            if (*idCursor == identifier) {
                return;
            }
            idCursor++;
            idIndex++;
        } while (idIndex < registeredCount);
    }
    if (registeredCount < CAMP_REGISTERED_ID_LIMIT) {
        scene->registeredIds[registeredCount] = identifier;
        scene->registeredCount++;
    }
}

/* Queue the scene's BGM ID with each registered variation, then clear the list.
 * The loop bound is read again after each queue call, not cached up front. */
void mnuReleaseCampSceneRegisteredIds(CampScene *scene) {
    s32 processedCount = 0;
    if (scene->registeredCount > 0) {
        s32 *idCursor = scene->registeredIds;
        do {
            s32 identifier = *idCursor++;
            processedCount++;
            evtQueueValidatedBgmSoundCode(scene->owner->bgmId, identifier);
        } while (processedCount < scene->registeredCount);
    }
    scene->registeredCount = 0;
}


typedef struct ShopBatchGraphics {
    u8 pad00[0x20];
    s32 *params; /* 0x20 */
} ShopBatchGraphics;

typedef struct ShopBatch {
    u8 pad00[8];
    ShopBatchGraphics *graphics; /* 0x08 */
} ShopBatch;

void mnuInitializeShopStatusBatches(ShopScene *scene) {
    ShopBatch *batchObject;
    ShopBatchGraphics *batchGraphics;
    s32 *batchParameters;
    s32 initialParameter = CAMP_STATUS_INITIAL_PARAMETER;
    scene->batchState = 0;
    batchObject = (ShopBatch *)effCreateStatusBatch(6);
    batchGraphics = batchObject->graphics;
    scene->batches[0] = (u8 *)batchObject;
    batchParameters = batchGraphics->params;
    batchParameters[0] = initialParameter;
    batchParameters[1] = 0;
    batchParameters[2] = 0;
    batchParameters[3] = 0;
    batchParameters[4] = 0;
    batchObject = (ShopBatch *)effCreateStatusBatch(1);
    batchGraphics = batchObject->graphics;
    scene->batches[1] = (u8 *)batchObject;
    batchParameters = batchGraphics->params;
    batchParameters[0] = initialParameter;
    batchParameters[1] = 0;
}

/* Destroy both batches and return the second destruction result. */
s32 mnuShopReleaseSceneObjects(ShopScene *scene) {
    s32 *batchCursor = (s32 *)scene->batches;
    s32 destroyResult;
    u32 batchIndex;
    for (batchIndex = 0; batchIndex < CAMP_STATUS_BATCH_COUNT; batchIndex++) {
        destroyResult = effDestroyPackedBatch(*batchCursor++);
    }
    return destroyResult;
}

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF3D0);

void mnuShopLoadSpriteAssets(ShopScene *scene) {
    u32 *resource = &scene->spriteResource;
    *resource = effLoadIndexedResource("/facility/spr/shop/", D_0036AA60[0], 0);
}

INCLUDE_ASM(const s32, "game/code_00242608", mnuReleaseShopSceneSpriteResources);

extern u8 *datItemSkillRecords;

s32 mnuShopHasPendingFlag(void) {
    u8 *flags = datGameState->inventory.counts;
    u8 *entry = datItemSkillRecords;
    s32 found = 0;
    s32 i;

    for (i = 0; i < 0xC0; flags++, i++) {
        if ((u32)(i - 0xA0) >= 0x20 && *flags != 0) {
            if ((*entry & 3) != 0) {
                found = 1;
                break;
            }
            if ((u32)(i - 0x60) < 0x20) {
                found = 1;
                break;
            }
        }
        entry += 8;
    }
    return found;
}



extern struct MenuListNode *mnuAppendWindowListNode(struct MenuWindowContainer *window, s32 value);
extern void func_0025E820();

MenuWindowContainer *func_002443F8(const void *unused, s32 count, ShopScene *settings) {
    MenuWindowContainer *window;
    MnuShopListContext *buffer;
    s32 i;

    if (settings->extraOption != 0) {
        count++;
    }
    window = (MenuWindowContainer *)mnuCreateWindowContainer(0, 0x260, 0x10, count, 0x15);
    mnuInitializeWindowEntryPlacement(0, window, 0, 8, 0xA);
    for (i = 0; i < count; i++) {
        mnuAppendWindowListNode(window, 0);
    }
    window->list->drawCallback = func_0025E820;
    buffer = sdfAllocSizeClassBlock(0x10);
    memset(buffer, 0, 0x10);
    window->list->context = buffer;
    buffer->extraOption = settings->extraOption;
    return window;
}


void func_002444D0(ShopScene *scene) {
    scene->sprite = func_002443F8(D_00368C40, 3, scene);
}

typedef struct CampFlagRow {
    s16 flag[8]; /* 0x00 */
    u8 value[9]; /* 0x10: [0] default, [i + 1] for flag[i] */
    u8 pad19;
} CampFlagRow;

extern CampFlagRow D_00368C50[];

s32 campFlagRowValue(s32 row) {
    s32 i;

    for (i = 7; i >= 0; i--) {
        if (D_00368C50[row].flag[i] > 0 && mdlFlagTest(D_00368C50[row].flag[i])) {
            return D_00368C50[row].value[i + 1];
        }
    }
    return D_00368C50[row].value[0];
}

extern u8 D_00369A88[];

/* Highest matching row wins; rows with nonpositive flags are not queried. */
s32 mnuCampFindActiveSlot(void) {
    s32 rowIndex;
    u8 *slotTable = D_00369A88;
    s16 *flagId = (s16 *)(slotTable + CAMP_SLOT_LAST_ROW_OFFSET);
    for (rowIndex = CAMP_SLOT_LAST_ROW_INDEX; rowIndex >= 0; rowIndex--, flagId = (s16 *)((u8 *)flagId - CAMP_SLOT_ROW_STRIDE)) {
        if (*flagId > 0 && mdlFlagTest(*flagId)) {
            return rowIndex;
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00244658);


extern void mnuDestroyWindowContainer(MenuWindowContainer *);
extern void sdfReleaseChipBlock();

void mnuShopReleaseSprites(ShopScene *scene) {
    MenuWindowContainer **slot = &scene->sprite;
    u32 i;

    for (i = 0; i < 1; i++) {
        MenuWindowContainer *sprite = *slot;

        if (sprite->list->context != NULL) {
            sdfReleaseChipBlock(sprite->list->context);
            sprite = *slot;
            sprite->list->context = NULL;
        }
        mnuDestroyWindowContainer(sprite);
        slot++;
    }
    if (scene->window != 0) {
        mnuDestroyWindowContainer(scene->window);
    }
}

typedef struct ShopProgressPriceScale {
    f32 percent;
    u32 threshold;
} ShopProgressPriceScale;

extern ShopProgressPriceScale D_0036A234[];

s32 mnuCampGetProgressStage(void) {
    s32 result = 0;
    if (mdlFlagTest(0x970)) {
        result = 1;
    }
    if (mdlFlagTest(0x971)) {
        result = 2;
    }
    if (mdlFlagTest(0x972)) {
        result = 3;
    }
    if (mdlFlagTest(0x973)) {
        result = 4;
    }
    if (mdlFlagTest(0x974)) {
        result = 5;
    }
    return result;
}

s32 func_00244848(void) {
    u32 stage = mnuCampGetProgressStage();

    if (stage < 5) {
        if (datGameState->world.score >= D_0036A234[stage].threshold) {
            stage++;
        } else {
            stage = 0;
        }
    } else {
        stage = 0;
    }
    return stage;
}

/* Sum the active low bit across the five party entries. */
s32 mnuCountActivePartyEntries(void) {
    u16 entryFlags;
    DatPartyRecord *entryCursor;
    s32 entryCountdown;
    s32 enabledCount;

    enabledCount = 0;
    entryCountdown = CAMP_PARTY_SCAN_LAST;
    entryCursor = datGameState->party;
    do {
        entryFlags = entryCursor->flags;
        entryCursor++;
        entryCountdown--;
        enabledCount += entryFlags & CAMP_PARTY_ACTIVE_FLAG;
    } while (entryCountdown >= 0);
    return enabledCount;
}

ShopScene *mnuShopCreateScene(void) {
    s32 handle;
    ShopScene *obj;

    handle = sdfAllocGeneralBlock(0xB4);
    obj = (ShopScene *)sdfResourceRetainAddress(handle);
    memset(obj, 0, 0xB4);
    obj->resourceHandle = handle;
    mnuClearPanelTransitionState(obj->transitionWork);
    mnuShopLoadSpriteAssets(obj);
    mnuInitializeShopStatusBatches(obj);
    evtLoadResourcePair("/facility/msg/shop/mes_data.bmd", obj->resourcePair);
    evtCreateMessageWindowIfMissing(obj->pairedHandle);
    D_003BC520 = obj->spriteResource;
    obj->count98 = func_00244848();
    obj->count8C = mnuCountActivePartyEntries();
    return obj;
}

extern s32 kwlnTaskGetUserValue();
extern void mnuDrainPanelTransitions();
extern void dspCloseChannel();
extern void evtReleaseResourcePairHandle();
extern void sdfReleaseResourceAllocation();

void mnuShopDestroyScene(s32 arg) {
    ShopScene *scene = (ShopScene *)kwlnTaskGetUserValue();

    if (scene != NULL) {
        mnuShopReleaseSprites(scene);
        mnuReleaseShopSceneSpriteResources(scene);
        mnuShopReleaseSceneObjects(scene);
        mnuDrainPanelTransitions((u8 *)scene + 8, arg);
        dspCloseChannel();
        evtReleaseResourcePairHandle(scene->resourcePair);
        sdfReleaseResourceAllocation(scene->resourceHandle);
        D_003BC39C = 2;
    }
}

extern s32 mnuCampRunPanel0(void *request);
extern s32 mnuCampRunPanel1(void *request);
extern s32 mnuCampRunPanel2(void *request);

/* Create the camp context and its three scheduler tasks (main, draw, update).
 * Optionally seed the initial selection from the caller. */

s32 mnuOpenShopSceneWithInitialSelection(s32 *initialSelection) {
    ShopScene *ctx = mnuShopCreateScene();
    s32 result;

    if (initialSelection != 0) {
        ctx->initialSelection = *initialSelection;
    }
    kwlnTaskCreate(D_003BC3A0, 0x402, 1, 1, mnuCampRunPanel0, 0, ctx);
    kwlnTaskCreate(D_003AF418, 0x2B12, 1, 1, mnuCampRunPanel1, 0, ctx);
    result = kwlnTaskCreate(D_003AF428, 0x520E, 1, 1, mnuCampRunPanel2, mnuShopDestroyScene, ctx);
    D_003BC39C = 1;
    return result;
}

void mnuCampDestroyPanelTasks(void) {
    kwlnTaskDestroyWithHierarchyByName(D_003BC3A0, 0);
    kwlnTaskDestroyWithHierarchyByName(D_003AF418, 0);
    kwlnTaskDestroyWithHierarchyByName(D_003AF428, 0);
}

s32 mnuPollTaskState(void) {
    s32 state = D_003BC39C;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        D_003BC39C = 0;
    }
    return 0;
}

extern void mnuSetPopupEntry(s32 *, void *);
extern u8 D_0036AB48[];

s32 mnuCampRunPanel0(void *request) {
    s32 state = kwlnTaskGetUserValue();
    s32 *panel = (s32 *)(state + 0x54);
    mnuSetPopupEntry(panel, D_0036AB48);
    return menuRunPanel((void *)state, 0, request);
}


s32 mnuCampRunPanel1(void *request) {
    s32 state = kwlnTaskGetUserValue();
    return menuRunPanel((void *)state, 1, request);
}

s32 mnuCampRunPanel2(void *request) {
    s32 state = kwlnTaskGetUserValue();
    return menuRunPanel((void *)state, 2, request);
}

typedef struct ShopSourcePriceEntry {
    u16 itemId;
    u8 type;
    u8 flags;
    u16 pricePercent;
} ShopSourcePriceEntry;

typedef struct ShopSourcePriceRow {
    u16 pricePercent;
    ShopSourcePriceEntry entries[0x40];
} ShopSourcePriceRow;

typedef struct ShopItemPriceRecord {
    u8 pad00[4];
    s32 price;
} ShopItemPriceRecord;

extern ShopSourcePriceRow D_00368CF0[];

s32 func_00244C00(s32 index, s32 source, s32 halfPrice) {
    u32 rowIndex;
    u32 itemId;
    u32 rowPercent;
    u32 pricePercent;
    s32 itemPrice;
    s32 price;
    s32 progressStage;

    rowIndex = (u8)campFlagRowValue(source);
    itemId = D_00368CF0[rowIndex].entries[index].itemId;
    if (halfPrice == 0) {
        itemPrice = ((ShopItemPriceRecord *)datItemSkillRecords)[itemId].price;
        rowPercent = D_00368CF0[rowIndex].pricePercent;
        pricePercent = D_00368CF0[rowIndex].entries[index].pricePercent;
        if (pricePercent == 0) {
            pricePercent = rowPercent;
        }
        price = itemPrice * pricePercent / 100;
        progressStage = mnuCampGetProgressStage();
        if (progressStage != 0) {
            price = price * (s32)D_0036A234[progressStage].percent / 100;
        }
    } else {
        price = (u32)((ShopItemPriceRecord *)datItemSkillRecords)[itemId].price >> 1;
    }
    return price;
}

s32 func_00244D10(s32 index, s32 halfPrice) {
    u32 rowOffset;
    u8 *row;
    u8 *entry;
    u32 itemId;
    u32 rowPercent;
    u32 pricePercent;
    s32 itemPrice;
    s32 price;
    s32 progressStage;

    rowOffset = (u8)mnuCampFindActiveSlot() * sizeof(ShopRankPriceRow);
    entry = D_00369A88 + index * sizeof(ShopRankPriceEntry) + rowOffset;
    row = D_00369A88 + rowOffset;
    itemId = *(u16 *)(entry + 4);
    if (halfPrice == 0) {
        itemPrice = ((ShopItemPriceRecord *)datItemSkillRecords)[itemId].price;
        rowPercent = *(u16 *)(row + 2);
        pricePercent = entry[7];
        if (pricePercent == 0) {
            pricePercent = rowPercent;
        }
        price = itemPrice * pricePercent / 100;
        progressStage = mnuCampGetProgressStage();
        if (progressStage != 0) {
            price = price * (s32)D_0036A234[progressStage].percent / 100;
        }
    } else {
        price = (u32)((ShopItemPriceRecord *)datItemSkillRecords)[itemId].price >> 1;
    }
    return price;
}

extern const s32 D_003BC3A8[];

s32 mnuShopGetTransactionLimit(ShopScene *scene) {
    MenuWindowContainer *window = scene->window;
    CampWindowParams *parameters = &window->list->cursor->camp;
    s16 extraOption = scene->extraOption;
    s32 itemId = parameters->id;
    s32 price = parameters->price;
    s32 kind = parameters->mode;
    s32 operations[2];
    s32 operation;
    s32 limit = 0;
    s32 capacity;
    s32 available;
    s32 affordable;

    memcpy(operations, D_003BC3A8, sizeof(operations));
    if (extraOption != 0) {
        operation = scene->sprite->list->cursor->index + 1;
    } else {
        operation = operations[scene->sprite->list->cursor->index];
    }
    capacity = scene->count8C;
    switch (operation) {
    case 1:
    case 2: {
        DatGameState *globalState = datGameState;

        affordable = globalState->header.currency / price;
        limit = affordable;
        if (kind == 2) {
            available = capacity - ptyCountBulletItem(itemId);
        } else if (kind == 3) {
            available = 1 - globalState->inventory.counts[itemId];
        } else {
            available = 99 - globalState->inventory.counts[itemId];
        }
        if (available < 0) {
            available = 0;
        }
        if (available < affordable) {
            limit = available;
        }
        break;
    }
    case 3: {
        s32 quantity = datGameState->inventory.counts[itemId];

        if (quantity * price > 9999999) {
            limit = (s32)(9999999.0f / (f32)price);
        } else {
            limit = quantity;
        }
        break;
    }
    }
    return limit;
}

s32 func_00244FA0(ShopScene *context) {
    DatGameState *globalState = datGameState;
    MenuWindowContainer *itemObject = context->window;
    struct MenuList *record = itemObject->list;
    CampWindowParams *parameters = &record->cursor->camp;
    s32 itemId = parameters->id;
    s32 divisor = parameters->price;
    s32 kind = parameters->mode;
    s32 limit = globalState->header.currency / divisor;
    s32 quantity = context->count8C;
    s32 available;

    if (kind == 2) {
        available = quantity - ptyCountBulletItem(itemId);
    } else if (kind == 3) {
        available = 1 - globalState->inventory.counts[itemId];
    } else {
        available = 0x63 - globalState->inventory.counts[itemId];
    }
    if (available < 0) {
        available = 0;
    }
    if (limit == 0) {
        return -1;
    }
    if (available == 0) {
        return -2;
    }
    if (available < limit) {
        return available;
    }
    return limit;
}

void func_00245068(ShopScene *scene) {
    s32 index = 0;
    s32 capacity = scene->count8C;
    struct MenuListNode *node = scene->window->list->first;
    for (; index < scene->window->list->count; index++) {
        CampWindowParams *parameters = &node->camp;
        DatGameState *globalState = datGameState;
        s32 itemId = parameters->id;
        s32 divisor = parameters->price;
        s32 kind = parameters->mode;
        s32 affordable = globalState->header.currency / divisor;
        s32 available;
        s32 limit;
        if (kind == 2) {
            available = capacity - ptyCountBulletItem(itemId);
        } else if (kind == 3) {
            available = 1 - globalState->inventory.counts[itemId];
        } else {
            available = 99 - globalState->inventory.counts[itemId];
        }
        if (available < 0) {
            available = 0;
        }
        limit = available < affordable ? available : affordable;
        if (limit == 0) {
            node->flags48 = 1;
        }
        node = node->next;
        if (node == NULL) {
            break;
        }
    }
}


s32 mnuCampClampSceneCounter(s32 delta, ShopScene *scene) {
    s32 limit = mnuShopGetTransactionLimit(scene);
    s32 sum = scene->counter + delta;
    s32 current;
    scene->counter = sum;
    if (sum <= 0) {
        scene->counter = 1;
    }
    current = scene->counter;
    if (current >= limit) {
        scene->atLimit = 1;
        scene->counter = limit;
        current = limit;
    } else {
        scene->atLimit = 0;
    }
    return current;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00245208);

INCLUDE_ASM(const s32, "game/code_00242608", func_002453C8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00245628);

INCLUDE_ASM(const s32, "game/code_00242608", func_002457E8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00245A40);

extern s32 itfDrawBankTextWithLayoutFlags(s32, s32, s32, s32, s32, s32);

extern void frFontSetChildColors(s32, u32);

extern void func_001958A0(s32, s32, s32);

extern void frFontQueueGlyphInSelectedSlot(s32);

void mnuQueueCampTextGlyphWithChildColor(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5) {
    s32 handle;

    if (a1 != 0) {
        handle = itfDrawBankTextWithLayoutFlags(0x970, 0xB58, 1, (u16)a0, a1, a4);
        frFontSetChildColors(handle, 0x80808040);
        func_001958A0(handle, 0, a5);
        frFontQueueGlyphInSelectedSlot(handle);
    }
}

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF418);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF428);

INCLUDE_SDATA(const s32, "game/code_00242608", mnuShopRestoreMiddleVector);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC388);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC390);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC398);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC39C);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC3A0);

INCLUDE_SDATA(const s32, "game/code_00242608", D_003BC3A8);

