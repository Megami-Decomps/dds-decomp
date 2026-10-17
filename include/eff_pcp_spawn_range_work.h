#ifndef EFF_PCP_SPAWN_RANGE_WORK_H
#define EFF_PCP_SPAWN_RANGE_WORK_H

#include "common.h"
#include "eff_pcp_spawn_range_event_row.h"

struct SdfMemBlock;
struct SoundMixer;

/* The radial event setup reads the typed tail and preserves its opaque prefix. */
typedef struct EffPCPSpawnRangeParams {
    f32 origin[4];
    u8 pad10[0x44];
    s32 duration;
    u32 count;
    s32 delaySpread;
    s32 fadeIn;
    s32 fadeOut;
    f32 initialAngularStep;
    f32 angularJitter;
    f32 angularDamping;
    f32 initialHeightStep;
    f32 heightJitter;
    f32 heightDamping;
    f32 startPosition;
    f32 endPosition;
    f32 startJitter;
    f32 endJitter;
    f32 startHeightRange;
} EffPCPSpawnRangeParams;

/* The 0xAC-byte owner precedes its count * 0x20 event rows in one allocation. */
typedef struct EffPCPSpawnRangeWork {
    EffPCPSpawnRangeParams params;
    EffPCPSpawnRangeEventRow *entries;
    struct SoundMixer *owner;
    u8 unk9C;
    u8 pad9D[3];
    f32 scale;
    u32 color;
    struct SdfMemBlock *handle;
} EffPCPSpawnRangeWork;

typedef char EffPCPSpawnRangeParams_size_must_be_0x94[
    (sizeof(EffPCPSpawnRangeParams) == 0x94) ? 1 : -1];
typedef char EffPCPSpawnRangeWork_size_must_be_0xAC[
    (sizeof(EffPCPSpawnRangeWork) == 0xAC) ? 1 : -1];

#define EFF_PCP_SPAWN_RANGE_ASSERT_FIELD(type, field, expectedOffset, expectedSize) \
    typedef char type##_##field##_offset[(unsigned long)&((type *)0)->field == (expectedOffset) ? 1 : -1]; \
    typedef char type##_##field##_size[(sizeof(((type *)0)->field) == (expectedSize)) ? 1 : -1]

EFF_PCP_SPAWN_RANGE_ASSERT_FIELD(EffPCPSpawnRangeParams, origin, 0x00, 0x10);
EFF_PCP_SPAWN_RANGE_ASSERT_FIELD(EffPCPSpawnRangeParams, pad10, 0x10, 0x44);
EFF_PCP_SPAWN_RANGE_ASSERT_FIELD(EffPCPSpawnRangeParams, duration, 0x54, 4);
EFF_PCP_SPAWN_RANGE_ASSERT_FIELD(EffPCPSpawnRangeParams, count, 0x58, 4);
EFF_PCP_SPAWN_RANGE_ASSERT_FIELD(EffPCPSpawnRangeParams, delaySpread, 0x5C, 4);
EFF_PCP_SPAWN_RANGE_ASSERT_FIELD(EffPCPSpawnRangeParams, fadeIn, 0x60, 4);
EFF_PCP_SPAWN_RANGE_ASSERT_FIELD(EffPCPSpawnRangeParams, fadeOut, 0x64, 4);
EFF_PCP_SPAWN_RANGE_ASSERT_FIELD(EffPCPSpawnRangeParams, initialAngularStep, 0x68, 4);
EFF_PCP_SPAWN_RANGE_ASSERT_FIELD(EffPCPSpawnRangeParams, angularJitter, 0x6C, 4);
EFF_PCP_SPAWN_RANGE_ASSERT_FIELD(EffPCPSpawnRangeParams, angularDamping, 0x70, 4);
EFF_PCP_SPAWN_RANGE_ASSERT_FIELD(EffPCPSpawnRangeParams, initialHeightStep, 0x74, 4);
EFF_PCP_SPAWN_RANGE_ASSERT_FIELD(EffPCPSpawnRangeParams, heightJitter, 0x78, 4);
EFF_PCP_SPAWN_RANGE_ASSERT_FIELD(EffPCPSpawnRangeParams, heightDamping, 0x7C, 4);
EFF_PCP_SPAWN_RANGE_ASSERT_FIELD(EffPCPSpawnRangeParams, startPosition, 0x80, 4);
EFF_PCP_SPAWN_RANGE_ASSERT_FIELD(EffPCPSpawnRangeParams, endPosition, 0x84, 4);
EFF_PCP_SPAWN_RANGE_ASSERT_FIELD(EffPCPSpawnRangeParams, startJitter, 0x88, 4);
EFF_PCP_SPAWN_RANGE_ASSERT_FIELD(EffPCPSpawnRangeParams, endJitter, 0x8C, 4);
EFF_PCP_SPAWN_RANGE_ASSERT_FIELD(EffPCPSpawnRangeParams, startHeightRange, 0x90, 4);

EFF_PCP_SPAWN_RANGE_ASSERT_FIELD(EffPCPSpawnRangeWork, params, 0x00, 0x94);
EFF_PCP_SPAWN_RANGE_ASSERT_FIELD(EffPCPSpawnRangeWork, entries, 0x94, 4);
EFF_PCP_SPAWN_RANGE_ASSERT_FIELD(EffPCPSpawnRangeWork, owner, 0x98, 4);
EFF_PCP_SPAWN_RANGE_ASSERT_FIELD(EffPCPSpawnRangeWork, unk9C, 0x9C, 1);
EFF_PCP_SPAWN_RANGE_ASSERT_FIELD(EffPCPSpawnRangeWork, pad9D, 0x9D, 3);
EFF_PCP_SPAWN_RANGE_ASSERT_FIELD(EffPCPSpawnRangeWork, scale, 0xA0, 4);
EFF_PCP_SPAWN_RANGE_ASSERT_FIELD(EffPCPSpawnRangeWork, color, 0xA4, 4);
EFF_PCP_SPAWN_RANGE_ASSERT_FIELD(EffPCPSpawnRangeWork, handle, 0xA8, 4);

#undef EFF_PCP_SPAWN_RANGE_ASSERT_FIELD

#endif
