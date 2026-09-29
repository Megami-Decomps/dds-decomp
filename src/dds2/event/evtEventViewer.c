#include "common.h"

extern s32 func_002467B8(u32);

extern s64 strcmp(u64, s32);

/* Event-viewer entries are linked at +0x7C/+0x80 and keyed by id. */
typedef struct EvtEvEntry {
    s32 id;                  /* 0x00 */
    u8 pad4[0x78];
    struct EvtEvEntry *next; /* 0x7C */
    struct EvtEvEntry *prev; /* 0x80 */
} EvtEvEntry;

/* The name table and entry list have the same offsets in both games. */
typedef struct EvtViewer {
    u8 pad00[0x20];
    s32 nameCount;       /* 0x20 */
    char names[256][32]; /* 0x24 */
    u8 pad2024[0xC];
    s32 entryCount;      /* 0x2030 */
    EvtEvEntry *head;    /* 0x2034 */
    EvtEvEntry *tail;    /* 0x2038 */
    void *slots[1];      /* 0x203C */
} EvtViewer;

/* Bounds are reset from the currently observed value. */
typedef struct EvtRange {
    u8 pad0[0x10];
    s32 min;
    s32 max;
    s32 value;
} EvtRange;

void func_002467A0(void) {
    func_00246108();
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_002467B8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00246820);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00246878);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00246950);

/* Consume queued viewer events until the pending check reports none. */
void evtEventViewerProcessPending(u32 viewer) {
    s64 pending;

    while (pending = func_002467B8(viewer), pending != 0) {
        func_00246878(viewer);
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00246A60);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00246B00);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00246B50);

s32 evtEventViewerCountEntriesById(s32 id, EvtViewer *viewer) {
    EvtEvEntry *node;
    s32 currentId;
    s32 count;

    node = viewer->head;
    count = 0;
    while (node != NULL) {
        currentId = node->id;
        node = node->next;
        if (currentId == id) {
            count++;
        }
    }
    return count;
}

s32 evtEventViewerCountEntries(EvtViewer *viewer) {
    EvtEvEntry *node;
    s32 count;

    count = 0;
    for (node = viewer->head; node != NULL; node = node->next) {
        count++;
    }
    return count;
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00246C38);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00246CB8);

void evtViewerSetMinimumFromCurrent(EvtRange *range) {
    s32 current;

    current = range->value;
    range->min = current;
    if (range->max < current) {
        range->max = current;
    }
}

void evtViewerSetMaximumFromCurrent(EvtRange *range) {
    s32 current;

    current = range->value;
    range->max = current;
    if (current < range->min) {
        range->min = current;
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00246D80);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00246DF0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00246E68);

/* Search the fixed-width (0x20-byte) event-name records. */
s32 evtEventViewerFindNameIndex(u64 name, s32 viewerAddress) {
    s64 comparison;
    s32 nameSlot;
    s32 index;

    index = 0;
    if (0 < *(s32 *)(viewerAddress + 0x20)) {
        nameSlot = viewerAddress + 0x24;
        do {
            comparison = strcmp(name, nameSlot);
            if (comparison == 0) {
                return index;
            }
            index = index + 1;
            nameSlot = nameSlot + 0x20;
        } while (index < *(s32 *)(viewerAddress + 0x20));
    }
    return -1;
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00246F60);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00246FC0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00247028);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00247168);

void evtEventViewerFreeSlot(s32 index, s32 viewerAddress) {
    s32 resource;
    s32 *slot;

    slot = (s32 *)(index * 4 + viewerAddress + 0x203c);
    resource = *slot;
    if (resource != 0) {
        func_00110B50(resource);
        *slot = 0;
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00247400);

void evtEventViewerFreeBuffer(s32 bufferAddress) {
    if (*(s32 *)(bufferAddress + 0x2c) != 0) {
        func_00110B50(*(s32 *)(bufferAddress + 0x2c));
    }
    *(u32 *)(bufferAddress + 0x2c) = 0;
}

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004224A8);

