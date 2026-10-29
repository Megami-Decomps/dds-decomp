#ifndef BASIC_H
#define BASIC_H

#include "common.h"
#include "eff_transform.h"

struct dds3Basic_t;
struct sdfMemHandle_t;
struct WorldValueEntry;
struct WorldIndexNode;

/* Complete kind-0 payload: node lists, selected objects and the value-entry pool. */
typedef struct dds3WorldBasic_t {
    struct dds3Basic_t *firstNode;
    struct dds3Basic_t *lastNode;
    void *activeScene;  /* 0x08: selected primary world node */
    void *controlscene; /* 0x0C: selected secondary world node */
    struct sdfMemHandle_t *entryAllocation; /* 0x10: descriptor, distinct from its data */
    struct WorldValueEntry *entries;           /* 0x14: retained allocation address */
    u16 entryCapacity;                  /* 0x18: initial entry count */
    s16 freeHeadIndex;                  /* 0x1A: -1 when the pool is exhausted */
    s16 freeTailIndex;                  /* 0x1C */
    u16 freeEntryCount;                 /* 0x1E */
    struct WorldIndexNode *firstIndex;
    struct WorldIndexNode *lastIndex;
} dds3WorldBasic_t;

typedef char dds3WorldBasic_t_size_must_be_0x28[(sizeof(dds3WorldBasic_t) == 0x28) ? 1 : -1];
typedef char dds3WorldBasic_t_activeScene_at_0x08[((u32)&((dds3WorldBasic_t *)0)->activeScene == 0x08) ? 1 : -1];
typedef char dds3WorldBasic_t_controlscene_at_0x0C[((u32)&((dds3WorldBasic_t *)0)->controlscene == 0x0C) ? 1 : -1];
typedef char dds3WorldBasic_t_entries_at_0x14[((u32)&((dds3WorldBasic_t *)0)->entries == 0x14) ? 1 : -1];
typedef char dds3WorldBasic_t_firstIndex_at_0x20[((u32)&((dds3WorldBasic_t *)0)->firstIndex == 0x20) ? 1 : -1];


#endif /* BASIC_H */
