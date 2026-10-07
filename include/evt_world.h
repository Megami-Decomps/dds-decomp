#ifndef EVT_WORLD_H
#define EVT_WORLD_H

#include "common.h"
#include "eff_transform.h"

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

struct SdfTex;

/* The picture-task allocator creates this complete 0x08-byte state. */
typedef struct EvtPictureWork {
    u32 flags;
    struct SdfTex *texture;
} EvtPictureWork;

typedef char EvtPictureWork_size_must_be_0x08[(sizeof(EvtPictureWork) == 0x08) ? 1 : -1];

/* Native resource handle and the loader's corresponding data address. */
typedef struct EvtResourcePair {
    u32 handle;
    u32 unk04;
} EvtResourcePair;

/* The primary object returned by dds3GetWorldObject, not the World/WorldInfo
 * handle that owns it. Event lookup, lip-sync and viewer code share this chain:
 * object +0x18 -> table +0x08 -> 0x0C-byte slot entries, each with head +0x04.
 * DDS1 and DDS2 use the same observed layout. Node payloads depend on the slot. */
enum {
    EVT_WORLD_SLOT_MOVIE = 3, /* viewer movie-time lists: byte offset 0x28 */
    EVT_WORLD_SLOT_UNIT = 5   /* unit/group/lip-sync lists: byte offset 0x40 */
};

struct SdfMemBlock;

/* The object's 18 per-kind intrusive lists. */
typedef struct EvtWorldSlot {
    s32 count;
    EffWorldNode *head;
    EffWorldNode *tail;
} EvtWorldSlot;

typedef struct EvtWorldTable {
    s32 unk00;
    struct SdfMemBlock *resource; /* 0x04: descriptor owning the slot array */
    EvtWorldSlot *slots; /* 0x08; valid index range is a caller contract */
    u32 cameraObject; /* 0x0C */
    u32 playerObject; /* 0x10 */
    u32 indexedHandle; /* 0x14 */
    u32 unk18;
    u32 unk1C;
    s32 drawEnabled; /* 0x20: controls drawing of kind 2 */
    u8 pad24[0x1C]; /* The SDK constructor allocates 0x40 bytes. */
} EvtWorldTable;


#endif /* EVT_WORLD_H */
