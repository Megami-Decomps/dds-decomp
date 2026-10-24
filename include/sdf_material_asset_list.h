#ifndef SDF_MATERIAL_ASSET_LIST_H
#define SDF_MATERIAL_ASSET_LIST_H

#include "common.h"

struct DevRequest;

/* Count-prefixed sequence of variable-size serialized material records. */
typedef struct SdfMaterialAssetList {
    u32 assetCount;
    u8 records[0];
} SdfMaterialAssetList;

typedef char SdfMaterialAssetList_size_must_be_4[
    (sizeof(SdfMaterialAssetList) == 4) ? 1 : -1];
typedef char SdfMaterialAssetList_records_must_start_at_4[
    ((u32)&((SdfMaterialAssetList *)0)->records == 4) ? 1 : -1];

struct DevRequest *sdfMatListMake(
    struct DevRequest *lookup, SdfMaterialAssetList *serializedAssets);

#endif /* SDF_MATERIAL_ASSET_LIST_H */
