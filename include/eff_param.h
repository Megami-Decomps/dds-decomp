#ifndef EFF_PARAM_H
#define EFF_PARAM_H

#include "common.h"

/* Compact effect owner: its halfword kind selects the opaque payload's operations. */
typedef struct EffParamWork {
    u16 kind;
    u8 pad02[2];
    void *payload;
} EffParamWork;

typedef char EffParamWork_size_must_be_8[(sizeof(EffParamWork) == 8) ? 1 : -1];

EffParamWork *effParamWorkCreate(u16 kind, void *source);
EffParamWork *effParamCreateFromTable(void *table, s32 index);
EffParamWork *effParamWorkDuplicate(EffParamWork *source);
void *effParamWorkGetData(EffParamWork *work);
void effParamWorkInvokeCallback(EffParamWork *work);
void effDispatchParameterDataAndFreeWork(EffParamWork *work);

#endif
