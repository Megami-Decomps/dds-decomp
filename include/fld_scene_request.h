#ifndef FLD_SCENE_REQUEST_H
#define FLD_SCENE_REQUEST_H

#include "fld.h"

/* Observed prefix of the relocated scene-load record. */
typedef struct FldLoadRecord {
    u32 unk00;
    u32 displayState;
    void *resourceA;
    void *resourceB;
    void *resourceC;
    void *resourceD;
} FldLoadRecord;

/* Observed prefix of the transfer payload passed to the resource-batch loader. */
typedef struct FldLoadRequest {
    u32 spawnBatchCount;
    FldSpawnBatch *spawnBatches;
    FldLoadRecord *record;
} FldLoadRequest;

/* The first two words are forwarded to placement spawning without decoding. */
typedef struct FldSceneRequest {
    u32 unk00;
    u32 unk04;
    FldLoadRecord *record;
    u32 spawnCount;
    struct FldActionSpawn *spawnList;
} FldSceneRequest;

typedef char FldLoadRecord_displayState_offset_check[
    ((u32)&((FldLoadRecord *)0)->displayState == 0x04) ? 1 : -1];
typedef char FldLoadRecord_resourceA_offset_check[
    ((u32)&((FldLoadRecord *)0)->resourceA == 0x08) ? 1 : -1];
typedef char FldLoadRequest_spawnBatchCount_offset_check[
    ((u32)&((FldLoadRequest *)0)->spawnBatchCount == 0x00) ? 1 : -1];
typedef char FldLoadRequest_spawnBatches_offset_check[
    ((u32)&((FldLoadRequest *)0)->spawnBatches == 0x04) ? 1 : -1];
typedef char FldLoadRequest_record_offset_check[
    ((u32)&((FldLoadRequest *)0)->record == 0x08) ? 1 : -1];
typedef char FldSceneRequest_unk00_offset_check[
    ((u32)&((FldSceneRequest *)0)->unk00 == 0x00) ? 1 : -1];
typedef char FldSceneRequest_unk04_offset_check[
    ((u32)&((FldSceneRequest *)0)->unk04 == 0x04) ? 1 : -1];
typedef char FldSceneRequest_record_offset_check[
    ((u32)&((FldSceneRequest *)0)->record == 0x08) ? 1 : -1];
typedef char FldSceneRequest_spawnCount_offset_check[
    ((u32)&((FldSceneRequest *)0)->spawnCount == 0x0C) ? 1 : -1];
typedef char FldSceneRequest_spawnList_offset_check[
    ((u32)&((FldSceneRequest *)0)->spawnList == 0x10) ? 1 : -1];

#endif /* FLD_SCENE_REQUEST_H */
