#include "common.h"
#include "dds3obj.h"
#include "eff_transform.h"


void dds3DeleteUnitObjectBasic(dds3Basic_t *unitObject) {
    dds3DeleteSceneBasicData(unitObject);
}
