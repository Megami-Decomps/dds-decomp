#include "common.h"
#include "eff_transform.h"

void dds3RemoveWorldObjectNode(EffWorldNode *node);

void ddsReleaseUnitObject(EffWorldNode *node) {
    dds3RemoveWorldObjectNode(node);
}
