#include "common.h"
#include "sdf_chip.h"
#include "eff_transform.h"

void dds3PathBasicFreeControl(dds3Basic_t *object) {
    sdfChipFreeDirect(object->workdata);
}
