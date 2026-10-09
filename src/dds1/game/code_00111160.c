#include "common.h"
#include "sdf_chip.h"
#include "dds3obj.h"
#include "dds3_path.h"

s32 func_00111160(u32 kind) {
    s32 result = 0;

    if (kind >= 4) {
        if (kind >= 8) {
            result = kind == 8;
        }
    }
    return result;
}

EffWorldNode *evtSpawnActionObj2(s32 firstValue, s32 secondValue) {
    EffWorldNode *obj = dds3AppendWorldObjectNode(2);

    obj->key = firstValue;
    obj->value = secondValue;
    return obj;
}

void dds3EnsureWorldNodeInSlot(EffWorldNode *object, EffWorldNode *node) {
    WorldIndexNode *slot = dds3GetWorldSlotValue(object, func_00111160(((u8 *)node)[0xF]));

    dds3ResetObjectValueCursor((WorldValueIndices *)slot);
    if (dds3SeekWorldNode((WorldValueIndices *)slot, (u32)node) != 1) {
        dds3GrowWorldValueChain((WorldValueIndices *)slot, 1);
        dds3WriteIndexedWorldObjectWord((WorldValueIndices *)slot, (u32)node);
    }
}
