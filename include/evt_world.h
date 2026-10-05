#ifndef EVT_WORLD_H
#define EVT_WORLD_H

#include "common.h"

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

/* Element of the table's indexed slot array; only its list head is identified. */
typedef struct EvtWorldSlot {
    u8 pad00[4];
    void *head; /* 0x04: interpreted by the slot-specific node type */
    u8 pad08[4];
} EvtWorldSlot;

typedef struct EvtWorldTable {
    u8 pad00[8];
    EvtWorldSlot *slots; /* 0x08; valid index range is a caller contract */
} EvtWorldTable;

typedef struct EvtWorldObject {
    u8 pad00[0x18];
    EvtWorldTable *table; /* 0x18 */
} EvtWorldObject;

#endif /* EVT_WORLD_H */
