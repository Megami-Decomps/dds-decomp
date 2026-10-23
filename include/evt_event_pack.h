#ifndef EVT_EVENT_PACK_H
#define EVT_EVENT_PACK_H

#include "common.h"

struct sdfMemHandle_t;
struct sdfTexHandle_t;
struct FileRequest;

/* Payload categories used by the fixed-stride BE resource table. */
typedef enum EvtPackKind {
    EVT_PACK_KIND_INVALID = -1,
    EVT_PACK_KIND_ENTRY_POINT = 0,
    EVT_PACK_KIND_AREA_ROOM_IDS = 1,
    EVT_PACK_KIND_AREA_RESOURCE_PRIMARY = 2,
    EVT_PACK_KIND_AREA_RESOURCE_SECONDARY = 3,
    EVT_PACK_KIND_TEXTURE_OFFSETS = 4,
    EVT_PACK_KIND_MODEL = 5,
    EVT_PACK_KIND_MOTION_CUES = 6
} EvtPackKind;

/* BE resource files have a 0x20-byte header and fixed 0x20-byte records. */
typedef struct EvtPackEntry {
    EvtPackKind kind;             /* 0x00 */
    s32 dataSize;                 /* 0x04: package byte count loaded at DDS1 241E7C. */
    s32 cueCount;                 /* 0x08: type-6 count at DDS2 0025D5EC/0025D634. */
    u32 dataOffset;               /* 0x0C: relative to the retained file base */
    s32 secondaryResourceId;      /* 0x10 */
    s32 resourceId;               /* 0x14 */
    u8 pad18[4];
    s32 motionId;                 /* 0x1C: type-6 motion index at DDS2 0025D5D4. */
} EvtPackEntry;

/* Kind-1 BE payload prefix used to select the area and room to load. */
typedef struct EvtAreaRoomIds {
    s32 area;
    s32 room;
} EvtAreaRoomIds;

typedef struct EvtPackHeader {
    u8 pad00[0x10];
    s32 entryCount;               /* 0x10 */
    u8 pad14[0xC];
    EvtPackEntry entries[0];      /* 0x20 */
} EvtPackHeader;

typedef s32 EvtPackLoadPhase;
enum {
    EVT_PACK_LOAD_NOT_STARTED = 0,
    EVT_PACK_LOAD_REQUEST_PENDING = 1,
    EVT_PACK_LOAD_COMPLETE = 2
};

/* The camp constructor allocates/clears 0x48 bytes and publishes this state to
 * the event-pack load and release callbacks. */
typedef struct EvtPackLoadState {
    s32 eventId;                  /* 0x00 */
    EvtPackLoadPhase loadPhase;    /* 0x04 */
    struct FileRequest *pendingRequest; /* 0x08: queued event-script request, then cleanup */
    struct sdfMemHandle_t *resourceAllocation; /* 0x0C: retained descriptor */
    u8 *data;                     /* 0x10 */
    EvtPackHeader *header;        /* 0x14 */
    EvtPackEntry *entries;        /* 0x18 */
    u8 *entryPoint;               /* 0x1C */
    u8 pad20[4];
    struct sdfMemHandle_t *sceneAllocation1; /* 0x24: descriptor released at teardown. */
    u8 pad28[8];
    struct sdfMemHandle_t *sceneAllocation2; /* 0x30: descriptor released at teardown. */
    u8 pad34[4];
    struct sdfTexHandle_t *effect72;      /* 0x38 */
    struct sdfTexHandle_t *effect71;      /* 0x3C: retained channel texture. */
    struct sdfTexHandle_t *effect76;      /* 0x40: retained channel texture. */
    struct sdfTexHandle_t *effect75;      /* 0x44: retained channel texture. */
} EvtPackLoadState;

typedef char EvtPackEntry_size_must_be_0x20[(sizeof(EvtPackEntry) == 0x20) ? 1 : -1];
typedef char EvtAreaRoomIds_size_must_be_0x08[(sizeof(EvtAreaRoomIds) == 0x08) ? 1 : -1];
typedef char EvtPackHeader_size_must_be_0x20[(sizeof(EvtPackHeader) == 0x20) ? 1 : -1];
typedef char EvtPackLoadState_size_must_be_0x48[(sizeof(EvtPackLoadState) == 0x48) ? 1 : -1];
typedef char EvtPackLoadState_loadPhase_at_0x04[
    ((u32)&((EvtPackLoadState *)0)->loadPhase == 0x04) ? 1 : -1];
typedef char EvtPackLoadState_sceneAllocation1_at_0x24[
    ((u32)&((EvtPackLoadState *)0)->sceneAllocation1 == 0x24) ? 1 : -1];
typedef char EvtPackLoadState_sceneAllocation2_at_0x30[
    ((u32)&((EvtPackLoadState *)0)->sceneAllocation2 == 0x30) ? 1 : -1];
typedef char EvtPackLoadState_effect72_at_0x38[
    ((u32)&((EvtPackLoadState *)0)->effect72 == 0x38) ? 1 : -1];
typedef char EvtPackLoadState_effect71_at_0x3C[
    ((u32)&((EvtPackLoadState *)0)->effect71 == 0x3C) ? 1 : -1];
typedef char EvtPackLoadState_effect76_at_0x40[
    ((u32)&((EvtPackLoadState *)0)->effect76 == 0x40) ? 1 : -1];
typedef char EvtPackLoadState_effect75_at_0x44[
    ((u32)&((EvtPackLoadState *)0)->effect75 == 0x44) ? 1 : -1];

/* These numeric task IDs resolve to the camp-owned event-pack state. */
EvtPackLoadState *evtGetPackProcessData(u32 taskId);
s32 evtGetEventPackStat(u32 taskId);

/* Entry payload layouts vary by resource kind and remain opaque here. */
void *evtGetResorceData(u32 taskId, s32 key);

#endif /* EVT_EVENT_PACK_H */
