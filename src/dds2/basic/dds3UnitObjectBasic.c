#include "common.h"
#include "dds3obj.h"
#include "eff_transform.h"


void ddsReleaseUnitObject(EffWorldNode *unitObject) {
    dds3RemoveWorldObjectNode(unitObject);
}
