#ifndef EVT_WORLD_H
#define EVT_WORLD_H

#include "common.h"
#include "eff_transform.h"

/* Result of querying a path object's movement state. */
typedef enum EvtPathStatus {
    EVT_PATH_STATUS_INVALID = -1,
    EVT_PATH_STATUS_AT_ENDPOINT = 0,
    EVT_PATH_STATUS_IN_PROGRESS = 1
} EvtPathStatus;

typedef s32 EvtPathLinkResult;

enum EvtPathLinkResult {
    EVT_PATH_LINK_FAILED = 0,
    EVT_PATH_LINK_SUCCEEDED = 1
};

/* Native 0x0A-byte frame editor row: count followed by eight signed column kinds. */
typedef struct EvtFrameTableEntry {
    s16 columns;
    s8 columnTypes[8];
} EvtFrameTableEntry;

typedef char EvtFrameTableEntry_size_must_be_0x0A[(sizeof(EvtFrameTableEntry) == 0x0A) ? 1 : -1];

#ifdef VERSION_DDS1
extern EvtFrameTableEntry D_00368768[];
#elif VERSION_DDS2
extern EvtFrameTableEntry D_003C9538[];
#endif

struct sdfTexHandle_t;
struct sdfMemHandle_t;
struct ItfMesSub;

/* The picture-task allocator creates this complete 0x08-byte state. */
typedef struct EvtPictureWork {
    u32 drawEnabled; /* Only EVT_PICTURE_FLAG_DRAW_ENABLED is set or tested. */
    struct sdfTexHandle_t *texture;
} EvtPictureWork;

typedef char EvtPictureWork_size_must_be_0x08[(sizeof(EvtPictureWork) == 0x08) ? 1 : -1];
typedef char EvtPictureWork_drawEnabled_at_0x00[
    ((u32)&((EvtPictureWork *)0)->drawEnabled == 0x00) ? 1 : -1];
typedef char EvtPictureWork_texture_at_0x04[
    ((u32)&((EvtPictureWork *)0)->texture == 0x04) ? 1 : -1];

/* Native resource handle and the relocatable message data it owns. */
typedef struct EvtResourcePair {
    struct sdfMemHandle_t *handle;
    struct ItfMesSub *messageData;
} EvtResourcePair;

typedef char EvtResourcePair_size_must_be_0x08[(sizeof(EvtResourcePair) == 0x08) ? 1 : -1];
typedef char EvtResourcePair_messageData_offset_must_be_0x04[
    ((u32)&((EvtResourcePair *)0)->messageData == 0x04) ? 1 : -1];

/* Loaded resource table header: the resource handle, the origin of a
 * 32-byte-stride record array, and its record count. */
typedef struct EvtLoadedRecord {
    struct sdfMemHandle_t *handle; /* 0x0: released by sdfMemFreeDirect */
    u8 *records; /* 0x4: origin of a 32-byte-stride lookup */
    u32 recordCount; /* 0x8: bound used by the coordinate search */
} EvtLoadedRecord;

typedef char EvtLoadedRecord_size_must_be_0x0C[(sizeof(EvtLoadedRecord) == 0x0C) ? 1 : -1];

/* The primary object returned by dds3GetActiveScene is distinct from the active
 * world's WorldInfo payload. Event lookup, lip-sync and viewer code share this chain:
 * object +0x18 -> table +0x08 -> 0x0C-byte slot entries, each with head +0x04.
 * DDS1 and DDS2 use the same observed layout. Node payloads depend on the slot. */
enum {
    EVT_WORLD_SLOT_MOVIE = 3, /* viewer movie-time lists: byte offset 0x28 */
    EVT_WORLD_SLOT_CAMERA = 4, /* linked cameras; active nodes supply the view transform */
    EVT_WORLD_SLOT_UNIT = 5   /* unit/group/lip-sync lists: byte offset 0x40 */
};

struct BfFlw0Header;
struct dds3ProcessID_t;

/* The object's 18 per-kind intrusive lists. */
typedef struct EvtWorldSlot {
    s32 count;
    dds3Basic_t *head;
    dds3Basic_t *tail;
} EvtWorldSlot;

typedef struct EvtWorldTable {
    s32 areaRoomKey; /* 0x00: packed area in the high halfword and room in the low halfword; -1 before assignment. */
    struct sdfMemHandle_t *resource; /* 0x04: descriptor owning the slot array */
    EvtWorldSlot *slots; /* 0x08; valid index range is a caller contract */
    dds3Basic_t *cameraObject; /* 0x0C: retained camera world node */
    dds3Basic_t *playerObject; /* 0x10 */
    const struct sdfMemHandle_t *fieldResourceHandle; /* 0x14: borrowed alias; fldReleaseFieldResources owns its release. */
    struct sdfMemHandle_t *sceneResourceHandle; /* 0x18: retained allocation descriptor; released at teardown. */
    struct BfFlw0Header *sceneScript; /* 0x1C: resolved FLW0 header used for script lookup. */
    s32 drawEnabled; /* 0x20: controls drawing of kind 2 */
    u8 pad24[0x1C]; /* The SDK constructor allocates 0x40 bytes. */
} EvtWorldTable;

typedef char EvtWorldTable_sceneScript_at_0x1C[
    ((u32)&((EvtWorldTable *)0)->sceneScript == 0x1C) ? 1 : -1];
typedef char EvtWorldTable_sceneResourceHandle_at_0x18[
    ((u32)&((EvtWorldTable *)0)->sceneResourceHandle == 0x18) ? 1 : -1];
typedef char EvtWorldTable_fieldResourceHandle_at_0x14[
    ((u32)&((EvtWorldTable *)0)->fieldResourceHandle == 0x14) ? 1 : -1];
typedef char EvtWorldTable_size_must_be_0x40[
    sizeof(EvtWorldTable) == 0x40 ? 1 : -1];


/* Preserve the signed-byte drawing gate; only a missing payload is ignored. */
void dds3VisibleSceneBasic(dds3Basic_t *object, s8 enabled);
void dds3SetMapNo(dds3Basic_t *worldNode, u32 areaRoomKey);
s32 dds3GetMapNo(dds3Basic_t *worldNode);

/* Scene resource scripts are selected by name on a world node. The start
 * result is the scheduler task created for the script, or NULL on failure. */
struct dds3ProcessID_t *evtStartSceneResourceTask(dds3Basic_t *worldNode, const char *taskName);
void dds3SceneBasicEndScript(void *unusedContext, const char *taskName);

#endif /* EVT_WORLD_H */
