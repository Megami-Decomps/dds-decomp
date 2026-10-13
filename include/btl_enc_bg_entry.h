#ifndef BTL_ENC_BG_ENTRY_H
#define BTL_ENC_BG_ENTRY_H

#include "common.h"

/* Conditional parameter entry read by the queued battle-scene resolver. */
typedef struct EncBgEntry {
    s32 fallbackParameter;
    s16 flagA;
    s16 parameterA;
    s16 flagB;
    s16 parameterB;
} EncBgEntry;

typedef char EncBgEntry_size_must_be_0xC[
    (sizeof(EncBgEntry) == 0xC) ? 1 : -1];
typedef char EncBgEntry_flagA_offset_must_be_4[
    ((u32)&((EncBgEntry *)0)->flagA == 0x4) ? 1 : -1];
typedef char EncBgEntry_parameterA_offset_must_be_6[
    ((u32)&((EncBgEntry *)0)->parameterA == 0x6) ? 1 : -1];
typedef char EncBgEntry_flagB_offset_must_be_8[
    ((u32)&((EncBgEntry *)0)->flagB == 0x8) ? 1 : -1];
typedef char EncBgEntry_parameterB_offset_must_be_A[
    ((u32)&((EncBgEntry *)0)->parameterB == 0xA) ? 1 : -1];

typedef struct EncBgRow {
    s32 mapIdBase;
    EncBgEntry entries[64];
} EncBgRow;

typedef char EncBgRow_entries_offset_must_be_4[
    ((u32)&((EncBgRow *)0)->entries == 0x4) ? 1 : -1];
typedef char EncBgRow_size_must_be_0x304[
    (sizeof(EncBgRow) == 0x304) ? 1 : -1];

#endif /* BTL_ENC_BG_ENTRY_H */
