#ifndef EFF_PARAM_H
#define EFF_PARAM_H

#include "common.h"

/* The two titles register these eight compact table rows at matching indices.
 * These names describe registrations only; serialized kinds remain u16. */
enum EffParamWorkKind {
    EFF_PARAM_WORK_KIND_EFFECT_NODE = 0,
    EFF_PARAM_WORK_KIND_ANIMATED_BILLBOARD = 1,
    EFF_PARAM_WORK_KIND_CHILD_BILLBOARD = 2,
    EFF_PARAM_WORK_KIND_VIEWER_CONTEXT = 3,
    EFF_PARAM_WORK_KIND_EXTENDED_WORK = 4,
    EFF_PARAM_WORK_KIND_BILLBOARD_UNIT_OBJECT = 5,
    EFF_PARAM_WORK_KIND_EXTENDED_WORK_WITH_MATRIX_CALLBACK = 6,
    EFF_PARAM_WORK_KIND_FILE_QUEUE = 7
};

/* Compact effect owner: its halfword kind selects the opaque payload's operations. */
typedef struct EffParamWork {
    u16 kind;
    u8 pad02[2];
    void *payload;
} EffParamWork;

/* Extended adapter owner: a full-word kind and retained fallback-table index. */
typedef struct EffParamWorkEx {
    u32 kind;
    u32 tableIndex;
    void *payload;
} EffParamWorkEx;

typedef char EffParamWorkEx_size_must_be_0xC[(sizeof(EffParamWorkEx) == 0xC) ? 1 : -1];

typedef char EffParamWork_size_must_be_8[(sizeof(EffParamWork) == 8) ? 1 : -1];

/* The table starts with 0x10 bytes of header, followed by 0x10-byte records.
 * Each blockOffset is signed and relative to the start of the whole table. */
typedef struct EffParamTableRecord {
    u32 word0;
    s32 blockOffset;
    u32 kindAndFlags;
    u32 wordC;
} EffParamTableRecord;

typedef struct EffParamTable {
    u8 header[0x10];
    EffParamTableRecord records[1];
} EffParamTable;

typedef char EffParamTableRecord_size_must_be_0x10[
    (sizeof(EffParamTableRecord) == 0x10) ? 1 : -1];
typedef char EffParamTableRecord_blockOffset_at_4[
    ((u32)&((EffParamTableRecord *)0)->blockOffset == 4) ? 1 : -1];
typedef char EffParamTableRecord_kindAndFlags_at_8[
    ((u32)&((EffParamTableRecord *)0)->kindAndFlags == 8) ? 1 : -1];
typedef char EffParamTable_records_at_0x10[
    ((u32)&((EffParamTable *)0)->records == 0x10) ? 1 : -1];

/* Return a borrowed block at the selected record's signed relative offset. */
void *dds3EffectPCPDataGetData(const EffParamTable *table, s32 recordIndex);
u32 dds3EffectPCPDataGetSize(const EffParamTable *table, s32 recordIndex);
u32 dds3EffectPCPDataGetType(const EffParamTable *table, s32 recordIndex);
EffParamWork *dds3EffectPartCreate(u16 kind, void *source);
EffParamWork *dds3EffectPCPDataPartCreate(const EffParamTable *table, s32 recordIndex);
EffParamWork *dds3EffectPartCopy(EffParamWork *source);
void *effParamWorkGetData(EffParamWork *work);
void dds3EffectPartDraw(EffParamWork *work);
/* Optional source and matrix operations retain their generic pointer domains. */
void dds3EffectPartSetPosition(EffParamWork *work, void *source);
void dds3EffectPartSetScale(EffParamWork *work, f32 scale);
void dds3EffectPartSetRotateMatrix(EffParamWork *work, void *matrix);
void dds3EffectPartSetColor(EffParamWork *work, u32 value);
void dds3EffectPartSetDirection(EffParamWork *work, void *matrix);
void dds3EffectPartSetArea(EffParamWork *work, f32 sizeInput);
void dds3EffectPCPSetPosition(EffParamWorkEx *work, void *source);
void dds3EffectPCPSetScale(EffParamWorkEx *work, f32 scale);
void dds3EffectPCPSetRotateMatrix(EffParamWorkEx *work, void *matrix);
void dds3EffectPCPSetDirection(EffParamWorkEx *work, void *matrix);
/* The extended value adapter uses slot 3 despite its historical suffix 4. */
void dds3EffectPCPSetColor(EffParamWorkEx *work, u32 value);
void dds3EffectPCPSetArea(EffParamWorkEx *work, f32 sizeInput);
void dds3EffectPartDelete(EffParamWork *work);

#endif
