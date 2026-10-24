#ifndef FLD_SCENE_REQUEST_H
#define FLD_SCENE_REQUEST_H

#include "fld.h"

/* File-batch chunks use +0x04 as an index into the file-resolver state. */
typedef struct FldFileResolverRecord {
    u32 unk00;
    u32 fileResolverIndex;
    /* Optional path suffixes appended by the field resource resolver. */
    const char *resourceA;
    const char *resourceB;
    const char *resourceC;
    const char *resourceD;
} FldFileResolverRecord;

/* Scene-placement chunks use the same wire size but store a fallback script
 * name at +0x04 rather than a file-resolver index. */
typedef struct FldSceneResourceRecord {
    u32 unk00;
    const char *fallbackResourceName;
    const char *resourceA;
    const char *resourceB;
    const char *resourceC;
    const char *resourceD;
} FldSceneResourceRecord;

typedef char FldFileResolverRecord_size_check[
    (sizeof(FldFileResolverRecord) == 0x18) ? 1 : -1];
typedef char FldSceneResourceRecord_size_check[
    (sizeof(FldSceneResourceRecord) == 0x18) ? 1 : -1];

/* Observed prefix of the transfer payload passed to the resource-batch loader. */
typedef struct FldLoadRequest {
    u32 spawnBatchCount;
    FldSpawnBatch *spawnBatches;
    FldFileResolverRecord *record;
} FldLoadRequest;

/* Counted placement-batch input consumed by both field request paths. */
typedef struct FldSceneRequest {
    u32 spawnBatchCount;
    FldSpawnBatch *spawnBatches;
    FldSceneResourceRecord *record;
    u32 spawnCount;
    struct FldActionSpawn *spawnList;
} FldSceneRequest;

typedef char FldFileResolverRecord_fileResolverIndex_offset_check[
    ((u32)&((FldFileResolverRecord *)0)->fileResolverIndex == 0x04) ? 1 : -1];
typedef char FldFileResolverRecord_resourceA_offset_check[
    ((u32)&((FldFileResolverRecord *)0)->resourceA == 0x08) ? 1 : -1];
typedef char FldSceneResourceRecord_fallbackResourceName_offset_check[
    ((u32)&((FldSceneResourceRecord *)0)->fallbackResourceName == 0x04) ? 1 : -1];
typedef char FldSceneResourceRecord_resourceA_offset_check[
    ((u32)&((FldSceneResourceRecord *)0)->resourceA == 0x08) ? 1 : -1];
typedef char FldLoadRequest_spawnBatchCount_offset_check[
    ((u32)&((FldLoadRequest *)0)->spawnBatchCount == 0x00) ? 1 : -1];
typedef char FldLoadRequest_spawnBatches_offset_check[
    ((u32)&((FldLoadRequest *)0)->spawnBatches == 0x04) ? 1 : -1];
typedef char FldLoadRequest_record_offset_check[
    ((u32)&((FldLoadRequest *)0)->record == 0x08) ? 1 : -1];
typedef char FldSceneRequest_spawnBatchCount_offset_check[
    ((u32)&((FldSceneRequest *)0)->spawnBatchCount == 0x00) ? 1 : -1];
typedef char FldSceneRequest_spawnBatches_offset_check[
    ((u32)&((FldSceneRequest *)0)->spawnBatches == 0x04) ? 1 : -1];
typedef char FldSceneRequest_record_offset_check[
    ((u32)&((FldSceneRequest *)0)->record == 0x08) ? 1 : -1];
typedef char FldSceneRequest_spawnCount_offset_check[
    ((u32)&((FldSceneRequest *)0)->spawnCount == 0x0C) ? 1 : -1];
typedef char FldSceneRequest_spawnList_offset_check[
    ((u32)&((FldSceneRequest *)0)->spawnList == 0x10) ? 1 : -1];

#endif /* FLD_SCENE_REQUEST_H */
