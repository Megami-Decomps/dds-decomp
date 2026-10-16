#include "common.h"
#include "sdf_chip.h"
#include "dds3obj.h"
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

EffWorldNode *evtSpawnActionObj2(s32 key, s32 objectName) {
    EffWorldNode *obj = dds3AppendWorldObjectNode(EFF_WORLD_KIND_WORLD_INDEX_OWNER);

    obj->key = key;
    obj->value = (const char *)objectName;
    return obj;
}

void dds3EnsureWorldNodeInSlot(EffWorldNode *ownerObject, EffWorldNode *node) {
    WorldIndexNode *slot = dds3GetWorldSlotValue(ownerObject, dds3SelectOwnerWorldIndexSlot(((u8 *)node)[0xF]));

    dds3ResetObjectValueCursor((WorldValueIndices *)slot);
    if (dds3SeekWorldNode((WorldValueIndices *)slot, node) != 1) {
        dds3GrowWorldValueChain((WorldValueIndices *)slot, 1);
        dds3WriteIndexedWorldObjectWord((WorldValueIndices *)slot, (u32)node);
    }
}
