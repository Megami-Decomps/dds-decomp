#include "sdk_ps2.h"
#include "evt_run_context_api.h"
#include "common.h"
#include "scr.h"
#include "ps2asm.h"
#include "sdf_channel.h"
#include "scr_work_task_api.h"
#include "dat_save_words.h"
#include "sdf_float_counter.h"
#include "eff_random_mod.h"
#include "dds3_task_create.h"
#include "dds3obj.h"
#include "sdf_heap_api.h"
#include "mnu_flag_snapshot.h"
#include "dds3_path.h"
#include "eff_transform.h"
#include "evt_world_source_transform.h"
#include "btl_action.h"
#include "dat_state.h"
#include "dat_affinity_selector.h"
#include "dat_effective_affinity_api.h"
#include "dat_world_field_flags.h"
#include "dat_unit_has_skill_api.h"
#include "evt_randomize_entry_api.h"
#include "dat_command.h"
#include "sdf.h"
#include "sdf_dev_state.h"
#include "dds3_task_lifecycle.h"
#include "fld_runtime_reset_api.h"
#include "mdl_flag_api.h"

#include "sdf_debug_api.h"

dds3Basic_t *dds3AddPointBasic(u32 key, EvtWorldSourceTransformPrefix *source, const char *name) {
    dds3Basic_t *obj = dds3AddSceneBasicData(EFF_WORLD_KIND_TRANSFORM_SOURCE);

    obj->workdata = source;
    obj->key = key;
    obj->name = name;
    return obj;
}

f32 *dds3GetTransformSourceData(dds3Basic_t *node) {
    return node->workdata;
}

void *dds3GetWorldNodeData(dds3Basic_t *node) {
    return node->workdata;
}

/* Load the world node's borrowed data vector into VF10. */
void evtLoadValueVectorIntoVu(dds3Basic_t *node) {
    void *vec = dds3GetWorldNodeData(node);

    SDF_VU0_LOADVECTOR_BARRIER(vec);

}

/* Same, from the second quadword in the borrowed vector. */
void evtLoadValueSecondaryVectorIntoVu(dds3Basic_t *node) {
    void *vec = (void *)((u8 *)dds3GetWorldNodeData(node) + 0x10);

    SDF_VU0_LOADVECTOR_BARRIER(vec);

}
