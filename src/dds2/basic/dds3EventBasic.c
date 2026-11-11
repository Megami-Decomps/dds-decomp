#include "sdk_ps2.h"
#include "common.h"
#include "sdf_chip.h"
#include "dds3_path.h"
#include "dds3obj.h"
#include "eff_transform.h"
#include "eff.h"
#include "dds3_world_secondary.h"
#include "dds3_path_state.h"

dds3Basic_t *dds3AddEventBasic(s32 key, const char *resourcePath, const char *name) {
    dds3Basic_t *obj = dds3AddSceneBasicData(EFF_WORLD_KIND_ACTION_D);

    obj->workdata = (void *)resourcePath;
    obj->key = key;
    obj->name = name;
    return obj;
}

const char *dds3EventBasicGetResource(dds3Basic_t *object) {
    return object->workdata;
}
