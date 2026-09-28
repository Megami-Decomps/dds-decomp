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

void evtEventViewerProcessPending(u32 arg0) {
    s64 temp_v0;

    while (temp_v0 = func_002467B8(arg0), temp_v0 != 0) {
        func_00246878(arg0);
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

void func_00246D40(EventViewRange *range) {
    s32 current;

    current = range->current;
    range->minimum = current;
    if (range->maximum < current) {
        range->maximum = current;
    }
}

void func_00246D60(EventViewRange *range) {
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

s32 evtEventViewerFindNameIndex(u64 arg0, s32 arg1) {
    s64 temp_v0;
    s32 temp_v1;
    s32 temp_v2;

    temp_v2 = 0;
    if (0 < *(s32 *)(arg1 + 0x20)) {
        temp_v1 = arg1 + 0x24;
        do {
            temp_v0 = strcmp(arg0, temp_v1);
            if (temp_v0 == 0) {
                return temp_v2;
            }
            temp_v2 = temp_v2 + 1;
            temp_v1 = temp_v1 + 0x20;
        } while (temp_v2 < *(s32 *)(arg1 + 0x20));
    }
    return -1;
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00246F60);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00246FC0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00247028);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_00247168);

void evtEventViewerFreeSlot(s32 arg0, s32 arg1) {
    s32 temp_v0;
    s32 *piVar2;

    piVar2 = (s32 *)(arg0 * 4 + arg1 + 0x203c);
    temp_v0 = *piVar2;
    if (temp_v0 != 0) {
        func_00110B50(temp_v0);
        *piVar2 = 0;
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

