#include "common.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "dds3_path.h"
#include "evt_action_object.h"

s32 dds3SelectOwnerWorldIndexSlot(u32 kind) {
    s32 result = 0;

    if (kind >= 4) {
        if (kind >= 8) {
            result = kind == 8;
        }
    }
    return result;
}

EffWorldNode *evtSpawnActionObj2(s32 key, s32 value) {
    EffWorldNode *obj = dds3AppendWorldObjectNode(EFF_WORLD_KIND_WORLD_INDEX_OWNER);

    obj->key = key;
    obj->value = (const char *)value;
    return obj;
}

void dds3EnsureWorldNodeInSlot(EffWorldNode *object, EffWorldNode *node) {
    WorldIndexNode *slot = dds3GetWorldSlotValue(object, dds3SelectOwnerWorldIndexSlot(((u8 *)node)[0xF]));

    dds3ResetObjectValueCursor((WorldValueIndices *)slot);
    if (dds3SeekWorldNode((WorldValueIndices *)slot, (u32)node) != 1) {
        dds3GrowWorldValueChain((WorldValueIndices *)slot, 1);
        dds3WriteIndexedWorldObjectWord((WorldValueIndices *)slot, (u32)node);
    }
}
