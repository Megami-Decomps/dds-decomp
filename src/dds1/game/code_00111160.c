#include "common.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "dds3_basic_array_cursor.h"
#include "dds3_basic_array_search.h"
#include "dds3_path.h"
#include "evt_action_object.h"

s32 dds3SelectOwnerWorldIndexSlot(u32 nodeKind) {
    s32 result = 0;

    if (nodeKind >= 4) {
        if (nodeKind >= 8) {
            result = nodeKind == 8;
        }
    }
    return result;
}

dds3Basic_t *dds3AddAreaBasic(s32 key, const char *objectName) {
    dds3Basic_t *obj = dds3AddSceneBasicData(EFF_WORLD_KIND_WORLD_INDEX_OWNER);

    obj->key = key;
    obj->name = objectName;
    return obj;
}

void dds3AreaBasicRegistItem(dds3Basic_t *ownerObject, dds3Basic_t *node) {
    WorldIndexNode *slot =
        dds3AreaBasicGetWorldIndexNode(ownerObject, dds3SelectOwnerWorldIndexSlot(node->kindTag >> 24));

    dds3SetFirstBasicArray((dds3BasicArrayHandle_t *)slot);
    if (dds3SetSearchNextBasicArray((dds3BasicArrayHandle_t *)slot, node) != DDS3_BASIC_ARRAY_MATCH_FOUND) {
        dds3AddBasicArray((dds3BasicArrayHandle_t *)slot, 1);
        dds3SetBasicArray((dds3BasicArrayHandle_t *)slot, node);
    }
}
