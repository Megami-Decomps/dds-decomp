#include "common.h"
#include "scr_integer_parameter_api.h"
#include "scr_command_parameter_index.h"
#include "dds3_basic_lifecycle.h"
#include "ps2asm.h"
#include "eff_inner_float_api.h"
#include "sdf_sif_command.h"
#include "scr_integer_return_value_api.h"
#include "sdf_vu_matrix.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "kernel.h"
#include "eff_transform.h"
#include "dds3obj.h"
#include "sdf.h"
#include "pty_read_roster_stat_api.h"
#include "pty_event_penalty_api.h"
#include "dds3_vu_transform.h"
#include "sdf_heap_api.h"
#include "basic.h"
#include "dds3_world_scene_node_view.h"
#include "dds3_world_value.h"
#include "dds3_world_lifecycle.h"
#include "fpu.h"
#include "dds3_world_counter.h"
#include "dds3_basic_array_cursor.h"
#include "dds3_basic_array_search.h"
#include "dds3_world_selection.h"
#include "fld_area_room_resource_load_api.h"
#include "evt_world_object_api.h"
#include "evt_world.h"
#include "sdf_texture_offset_list.h"
#include "dds3_basic_array_visit.h"
#include "scr.h"
#include "dds3_task_lifecycle.h"
#include "dds3_scene_membership.h"
#include "dds3_area_callback.h"
#include "dds3_object_control_slot.h"
#include "dds3_path.h"
#include "evt_action_object.h"
#include "sdk_ps2.h"
#include "dds3_camera_update_flags.h"
#include "eff_object.h"
#include "sdf_model_packet_callback.h"
#include "dds3_object_flags.h"
#include "sdf_model.h"
#include "sdf_model_node_index_api.h"
#include "sdf_model_eval_api.h"
#include "sdf_model_release_api.h"
#include "evt_gradient_rect.h"
#include "sdf_motion.h"
#include "sdf_motion_next_result.h"
#include "dds3_slot1_handler_api.h"
#include "dds3_object_resource_state.h"
#include "dds3_object_base_resource_view.h"
#include "dds3_object_mode.h"
#include "mdl.h"
#include "evt_unit.h"
#include "evt_script_model_api.h"

u32 ptyScriptApplyEventPenalty(void) {
    PtyEventPenaltyMode penaltyMode;

    penaltyMode = scrGetIntPara(0);
    ptyApplyEventPenalty(penaltyMode);
    return 1;
}

extern void ptyRecoverAllUnits(void);

u32 ptyScriptRestoreEntireParty(void) {
    ptyRecoverAllUnits();
    return 1;
}

u32 ptyScriptGetRosterStat(void) {
    s32 value;

    value = scrGetIntPara(0);
    value = ptyReadSignedRosterStatByte((DatBaseStatIndex)value);
    scrSetIntReturnValue(value);
    return 1;
}
