#include "common.h"

extern s32 func_002467B8(u32);

extern s64 strcmp(u64, s32);

typedef struct EventViewNode {
    s32 id;
    u8 pad4[0x78];
    struct EventViewNode *next;
} EventViewNode;

typedef struct EventView {
    u8 pad0[0x2034];
    EventViewNode *first;
} EventView;

typedef struct EventViewRange {
    u8 pad0[0x10];
    s32 minimum;
    s32 maximum;
    s32 current;
} EventViewRange;

void func_002467A0(void) {
    func_00246108();
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_002467B8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00246820);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00246878);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00246950);

void evtEventViewerProcessPending(u32 viewer) {
    s64 pending;

    while (pending = func_002467B8(viewer), pending != 0) {
        func_00246878(viewer);
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00246A60);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00246B00);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00246B50);

s32 evtEventViewerCountEntriesById(s32 id, EventView *viewer) {
    EventViewNode *node;
    s32 currentId;
    s32 count;

    node = viewer->first;
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

s32 evtEventViewerCountEntries(EventView *viewer) {
    EventViewNode *node;
    s32 count;

    count = 0;
    for (node = viewer->first; node != NULL; node = node->next) {
        count++;
    }
    return count;
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00246C38);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00246CB8);

void evtViewerSetMinimumFromCurrent(EventViewRange *range) {
    s32 current;

    current = range->current;
    range->minimum = current;
    if (range->maximum < current) {
        range->maximum = current;
    }
}

void evtViewerSetMaximumFromCurrent(EventViewRange *range) {
    s32 current;

    current = range->current;
    range->maximum = current;
    if (current < range->minimum) {
        range->minimum = current;
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00246D80);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00246DF0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00246E68);

s32 evtEventViewerFindNameIndex(u64 name, s32 table) {
    s64 comparison;
    s32 entry;
    s32 index;

    index = 0;
    if (0 < *(s32 *)(table + 0x20)) {
        entry = table + 0x24;
        do {
            comparison = strcmp(name, entry);
            if (comparison == 0) {
                return index;
            }
            index = index + 1;
            entry = entry + 0x20;
        } while (index < *(s32 *)(table + 0x20));
    }
    return -1;
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00246F60);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00246FC0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00247028);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00247168);

void evtEventViewerFreeSlot(s32 index, s32 viewer) {
    s32 unit;
    s32 *slot;

    slot = (s32 *)(index * 4 + viewer + 0x203c);
    unit = *slot;
    if (unit != 0) {
        func_00110B50(unit);
        *slot = 0;
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00247400);

void evtEventViewerFreeBuffer(s32 arg0) {
    if (*(s32 *)(arg0 + 0x2c) != 0) {
        func_00110B50(*(s32 *)(arg0 + 0x2c));
    }
    *(u32 *)(arg0 + 0x2c) = 0;
}

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004224A8);

