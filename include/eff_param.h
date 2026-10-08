#ifndef EFF_PARAM_H
#define EFF_PARAM_H

#include "common.h"

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
void effDispatchParameterDataAndFreeWork(EffParamWork *work);

#endif
