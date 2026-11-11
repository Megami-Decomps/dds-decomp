#ifndef EFF_PCP_DATA_H
#define EFF_PCP_DATA_H

#include "common.h"

typedef struct dds3EffectPCPData_Part_t {
    u32 size;
    u32 offset;
    u32 type;
    u32 reserve;
} dds3EffectPCPData_Part_t;

typedef struct dds3EffectPCPData_t {
    u32 kateg;
    u32 param;
    u32 part;
    u32 reserve;
    dds3EffectPCPData_Part_t pcpData[1];
} dds3EffectPCPData_t;

u32 dds3EffectPCPDataGetKateg(dds3EffectPCPData_t *data);
u32 dds3EffectPCPDataGetParam(dds3EffectPCPData_t *data);
u32 dds3EffectPCPDataGetType(dds3EffectPCPData_t *data, u32 number);
u32 dds3EffectPCPDataGetSize(dds3EffectPCPData_t *data, u32 number);
void *dds3EffectPCPDataGetData(dds3EffectPCPData_t *data, u32 number);

#endif
