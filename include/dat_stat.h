#ifndef DAT_STAT_H
#define DAT_STAT_H

/* The five base-stat slots used by party/enemy records and matching stat
 * vectors. These are not affinity-table selectors or script IDs. */
typedef enum DatBaseStatIndex {
    DAT_BASE_STAT_STRENGTH = 0,
    DAT_BASE_STAT_VITALITY = 1,
    DAT_BASE_STAT_MAGIC = 2,
    DAT_BASE_STAT_AGILITY = 3,
    DAT_BASE_STAT_LUCK = 4,
    DAT_BASE_STAT_COUNT = 5
} DatBaseStatIndex;

#endif
