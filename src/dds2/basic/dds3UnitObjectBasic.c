#include "common.h"
#include "dds3obj.h"
#include "eff_transform.h"


void ddsReleaseUnitObject(EffWorldNode *node) {
    dds3RemoveWorldObjectNode(node);
}
