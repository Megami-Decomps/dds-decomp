#include "common.h"

/* Event-viewer entry: doubly linked through next/prev, keyed by id. */
typedef struct EvtEvEntry {
    s32 id;                    /* 0x0 */
    u8 pad[0x78];              /* 0x4 */
    struct EvtEvEntry *next;   /* 0x7c */
    struct EvtEvEntry *prev;   /* 0x80 */
} EvtEvEntry;

/* Event viewer: name table at 0x20, entry list at 0x2030. */
typedef struct EvtViewer {
    u8 unk00[0x20];      /* 0x0 */
    s32 nameCount;       /* 0x20 */
    char names[256][32]; /* 0x24: fixed-width names */
    u8 pad2024[0xC];
    s32 entryCount;      /* 0x2030 */
    EvtEvEntry *head;    /* 0x2034 */
    EvtEvEntry *tail;    /* 0x2038 */
    void *slots[1];      /* 0x203c */
} EvtViewer;

/* Min/max tracker fed from a live value. */
typedef struct EvtRange {
    u8 unk00[0x10];   /* 0x0 */
    s32 min;          /* 0x10 */
    s32 max;          /* 0x14 */
    s32 value;        /* 0x18 */
} EvtRange;

typedef struct EvtViewBuf {
    u8 unk00[0x2c];   /* 0x0 */
    void *buf;        /* 0x2c */
} EvtViewBuf;

void func_0022B7A0(void);
s32 func_0022BE40(s32 arg0);
void func_0022BF00(s32 arg0);
void func_00110928(void *ptr);
s32 strcmp(const char *a, const char *b);

void func_0022BE28(void)
{
    func_0022B7A0();
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022BE40);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022BEA8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022BF00);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022BFD8);

void evtEventViewerProcessPending(s32 arg0)
{
    while (func_0022BE40(arg0) != 0) {
        func_0022BF00(arg0);
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022C0E8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022C188);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022C1D8);

s32 evtEventViewerCountEntriesById(s32 id, EvtViewer *viewer)
{
    EvtEvEntry *entry;
    s32 count;

    count = 0;
    entry = viewer->head;
    while (entry != NULL) {
        if (entry->id == id) {
            count = count + 1;
        }
        entry = entry->next;
    }
    return count;
}

s32 evtEventViewerCountEntries(EvtViewer *viewer)
{
    EvtEvEntry *entry;
    s32 count;

    count = 0;
    entry = viewer->head;
    while (entry != NULL) {
        count = count + 1;
        entry = entry->next;
    }
    return count;
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022C2C0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022C340);

void evtSetRangeMinimumFromCurrent(EvtRange *range)
{
    s32 value;

    value = range->value;
    range->min = value;
    if (range->max < value) {
        range->max = value;
    }
}

void evtSetRangeMaximumFromCurrent(EvtRange *range)
{
    s32 value;

    value = range->value;
    range->max = value;
    if (value < range->min) {
        range->min = value;
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022C408);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022C478);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022C4F0);

s32 evtEventViewerFindNameIndex(const char *name, EvtViewer *viewer)
{
    const char *slot;
    s32 index;

    index = 0;
    if (0 < viewer->nameCount) {
        slot = viewer->names[0];
        do {
            if (strcmp(name, slot) == 0) {
                return index;
            }
            index = index + 1;
            slot = slot + 0x20;
        } while (index < viewer->nameCount);
    }
    return -1;
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022C5E8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022C648);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022C6B0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022C7F0);

void evtEventViewerFreeSlot(s32 index, EvtViewer *viewer)
{
    void **slot;

    slot = (void **)(index * 4 + (s32)viewer + 0x203c);
    if (*slot != NULL) {
        func_00110928(*slot);
        *slot = NULL;
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022CA88);

void evtEventViewerFreeBuffer(EvtViewBuf *work)
{
    if (work->buf != NULL) {
        func_00110928(work->buf);
    }
    work->buf = NULL;
}

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_003ACFE8);

