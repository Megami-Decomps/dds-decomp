#ifndef EVT_EVENT_PACK_H
#define EVT_EVENT_PACK_H

#include "common.h"

struct SdfMemBlock;
struct SdfTex;

/* BE resource files have a 0x20-byte header and fixed 0x20-byte records. */
typedef struct EvtPackEntry {
    s32 kind;                     /* 0x00 */
    s32 dataSize;                 /* 0x04: package byte count loaded at DDS1 241E7C. */
    s32 cueCount;                 /* 0x08: type-6 count at DDS2 0025D5EC/0025D634. */
    u32 dataOffset;               /* 0x0C: relative to the retained file base */
    s32 secondaryResourceId;      /* 0x10 */
    s32 resourceId;               /* 0x14 */
    u8 pad18[4];
    s32 motionId;                 /* 0x1C: type-6 motion index at DDS2 0025D5D4. */
} EvtPackEntry;

typedef struct EvtPackHeader {
    u8 pad00[0x10];
    s32 entryCount;               /* 0x10 */
    u8 pad14[0xC];
    EvtPackEntry entries[0];      /* 0x20 */
} EvtPackHeader;

/* The camp constructor allocates/clears 0x48 bytes and publishes this state to
 * the event-pack load and release callbacks. */
typedef struct EvtPackLoadState {
    s32 eventId;                  /* 0x00 */
    s32 loaded;                   /* 0x04 */
    void *pendingRequest;         /* 0x08: queued callback work, then cleanup */
    struct SdfMemBlock *resourceAllocation; /* 0x0C: retained descriptor */
    u8 *data;                     /* 0x10 */
    EvtPackHeader *header;        /* 0x14 */
    EvtPackEntry *entries;        /* 0x18 */
    u8 *entryPoint;               /* 0x1C */
    u8 pad20[4];
    s32 sceneAllocation1;         /* 0x24 */
    u8 pad28[8];
    s32 sceneAllocation2;         /* 0x30 */
    u8 pad34[4];
    struct SdfTex *effect72;      /* 0x38 */
    s32 effect71;                 /* 0x3C */
    s32 effect76;                 /* 0x40 */
    s32 effect75;                 /* 0x44 */
} EvtPackLoadState;

typedef char EvtPackEntry_size_must_be_0x20[(sizeof(EvtPackEntry) == 0x20) ? 1 : -1];
typedef char EvtPackHeader_size_must_be_0x20[(sizeof(EvtPackHeader) == 0x20) ? 1 : -1];
typedef char EvtPackLoadState_size_must_be_0x48[(sizeof(EvtPackLoadState) == 0x48) ? 1 : -1];

/* These numeric task IDs resolve to the camp-owned event-pack state. */
EvtPackLoadState *evtGetEventPackLoadState(u32 taskId);
s32 evtGetEventPackLoadedState(u32 taskId);

/* Entry payload layouts vary by resource kind and remain opaque here. */
void *evtFindTaskResourceEntryByKey(u32 taskId, s32 key);

#endif /* EVT_EVENT_PACK_H */
