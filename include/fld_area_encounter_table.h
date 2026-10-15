#ifndef FLD_AREA_ENCOUNTER_TABLE_H
#define FLD_AREA_ENCOUNTER_TABLE_H

#include "common.h"

/* One area's 64 room choices and their flag-selected encounter alternatives. */
typedef struct FieldAreaEncounterRow {
    s32 defaultEncounter; /* 0x00: fallback encounter ID */
    s16 lowerPriorityFlagId;      /* 0x04 */
    s16 lowerPriorityEncounterId; /* 0x06: selected by lowerPriorityFlagId */
    s16 higherPriorityFlagId;     /* 0x08: tested before the lower-priority flag */
    s16 higherPriorityEncounterId; /* 0x0A: selected by higherPriorityFlagId */
} FieldAreaEncounterRow;

typedef struct FieldAreaEncounterTable {
    s32 area;                     /* 0x000: area matched against scene state */
    FieldAreaEncounterRow rooms[64]; /* 0x004: per-room encounter choices */
} FieldAreaEncounterTable;

typedef char FieldAreaEncounterRow_size_check[
    (sizeof(FieldAreaEncounterRow) == 0x0C) ? 1 : -1];
typedef char FieldAreaEncounterRow_defaultEncounter_offset_check[
    ((u32)&((FieldAreaEncounterRow *)0)->defaultEncounter == 0x00) ? 1 : -1];
typedef char FieldAreaEncounterRow_flagA_offset_check[
    ((u32)&((FieldAreaEncounterRow *)0)->lowerPriorityFlagId == 0x04) ? 1 : -1];
typedef char FieldAreaEncounterRow_valueA_offset_check[
    ((u32)&((FieldAreaEncounterRow *)0)->lowerPriorityEncounterId == 0x06) ? 1 : -1];
typedef char FieldAreaEncounterRow_flagB_offset_check[
    ((u32)&((FieldAreaEncounterRow *)0)->higherPriorityFlagId == 0x08) ? 1 : -1];
typedef char FieldAreaEncounterRow_valueB_offset_check[
    ((u32)&((FieldAreaEncounterRow *)0)->higherPriorityEncounterId == 0x0A) ? 1 : -1];

typedef char FieldAreaEncounterTable_area_offset_check[
    ((u32)&((FieldAreaEncounterTable *)0)->area == 0x000) ? 1 : -1];
typedef char FieldAreaEncounterTable_rooms_offset_check[
    ((u32)&((FieldAreaEncounterTable *)0)->rooms == 0x004) ? 1 : -1];
typedef char FieldAreaEncounterTable_size_check[
    (sizeof(FieldAreaEncounterTable) == 0x304) ? 1 : -1];

#endif /* FLD_AREA_ENCOUNTER_TABLE_H */
