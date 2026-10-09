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

EffParamWork *effParamWorkCreate(u16 kind, void *source);
EffParamWork *effParamCreateFromTable(void *table, s32 index);
EffParamWork *effParamWorkDuplicate(EffParamWork *source);
void *effParamWorkGetData(EffParamWork *work);
void effParamWorkInvokeCallback(EffParamWork *work);
/* Optional source and matrix operations retain their generic pointer domains. */
void effParamWorkCallback0(EffParamWork *work, void *source);
void effParamWorkCallback1(EffParamWork *work, f32 scale);
void effParamWorkCallback2(EffParamWork *work, void *matrix);
void effParamWorkCallback3(EffParamWork *work, u32 value);
void effParamWorkCallback4(EffParamWork *work, void *matrix);
void effParamWorkCallback5(EffParamWork *work, f32 sizeInput);
void effParamWorkExCallback0(EffParamWorkEx *work, void *source);
void effParamWorkExCallback1(EffParamWorkEx *work, f32 scale);
void effParamWorkExCallback2(EffParamWorkEx *work, void *matrix);
void effParamWorkExCallback3(EffParamWorkEx *work, void *matrix);
/* The extended value adapter uses slot 3 despite its historical suffix 4. */
void effParamWorkExCallback4(EffParamWorkEx *work, u32 value);
void effParamWorkExCallback5(EffParamWorkEx *work, f32 sizeInput);
void effDispatchParameterDataAndFreeWork(EffParamWork *work);

#endif
