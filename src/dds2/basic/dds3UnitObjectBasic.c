#include "common.h"
#include "dds3obj.h"
#include "eff_transform.h"


void dds3DeleteUnitObjectBasic(EffWorldNode *unitObject) {
    dds3DeleteSceneBasicData(unitObject);
}
