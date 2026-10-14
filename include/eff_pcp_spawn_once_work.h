#ifndef EFF_PCP_SPAWN_ONCE_WORK_H
#define EFF_PCP_SPAWN_ONCE_WORK_H

#include "common.h"
#include "eff_param.h"

/* Shared spawn-once work record used by the paired effect implementations. */
typedef struct EffPCPSpawnOnceWork {
    u32 unk00;
    u32 unk04;
    u32 unk08;
    u8 pad0C[4];
    u32 color; /* 0x10 */
    EffParamWork *primaryHandle; /* 0x14 */
    EffParamWork *secondaryHandle; /* 0x18 */
} EffPCPSpawnOnceWork;

typedef char EffPCPSpawnOnceWork_size_must_be_0x1C[
    sizeof(EffPCPSpawnOnceWork) == 0x1C ? 1 : -1];
typedef char EffPCPSpawnOnceWork_color_offset_must_be_0x10[
    ((u32)&((EffPCPSpawnOnceWork *)0)->color) == 0x10 ? 1 : -1];
typedef char EffPCPSpawnOnceWork_primaryHandle_offset_must_be_0x14[
    ((u32)&((EffPCPSpawnOnceWork *)0)->primaryHandle) == 0x14 ? 1 : -1];
typedef char EffPCPSpawnOnceWork_secondaryHandle_offset_must_be_0x18[
    ((u32)&((EffPCPSpawnOnceWork *)0)->secondaryHandle) == 0x18 ? 1 : -1];

#endif
