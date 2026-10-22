#include "common.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "dds3_path.h"
#include "evt_action_object.h"

s32 dds3AreaBasicTypeHasItems(u32 nodeKind) {
    s32 result = 0;

    if (nodeKind >= 4) {
        if (nodeKind >= 8) {
            result = nodeKind == 8;
        }
    }
    return result;
}

EffWorldNode *dds3AddAreaBasic(s32 key, s32 objectName) {
    EffWorldNode *obj = dds3AddSceneBasicData(EFF_WORLD_KIND_WORLD_INDEX_OWNER);

    obj->key = key;
    obj->value = (const char *)objectName;
    return obj;
}

void dds3AreaBasicRegistItem(EffWorldNode *ownerObject, EffWorldNode *node) {
    WorldIndexNode *slot = dds3AreaBasicReleaseItem(ownerObject, dds3AreaBasicTypeHasItems(((u8 *)node)[0xF]));

    dds3SetFirstBasicArray((WorldValueIndices *)slot);
    if (dds3SetSearchNextBasicArray((WorldValueIndices *)slot, node) != 1) {
        dds3AddBasicArray((WorldValueIndices *)slot, 1);
        dds3SetBasicArray((WorldValueIndices *)slot, (u32)node);
    }
}
