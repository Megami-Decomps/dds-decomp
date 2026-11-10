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

extern char D_003BA9A0[];
extern char D_003BA9A8[];
extern char D_003BA9B0[];

/* Each transform section owns its row coordinate; see docs/idioms.md,
 * "Trace-justified diagnostic row coordinates". */
s32 DispBaseData(dds3Basic_t *node, s32 x, s32 y, sdfDrawTag_t *list) {
    dds3PoseSet_t *transform;
    s32 labelX;
    s32 valueX;
    s32 positionY;
    s32 rotationY;
    s32 scaleY;
    s32 lines;

    x *= 12;
    labelX = (x << 4) + 0x7000;
    sdfDrawTagLink(list, (u32)sdfDevPrintf(labelX,
        (y++ * 6 << 4) + 0x7900, 0xFEFFFF, 0, "ID    : 0x%05X", node->key));
    sdfDrawTagLink(list, (u32)sdfDevPrintf(labelX,
        (y++ * 6 << 4) + 0x7900, 0xFEFFFF, 0, "NAME  : %s", node->name));
    transform = node->poseset;
    lines = 2;
    if (transform != NULL) {
        y++;
        positionY = (y * 6 << 4) + 0x7900;
        sdfDrawTagLink(list, (u32)sdfDevPrintf(labelX,
            positionY, 0xFEFFFF, 0, D_003BA9A0));
        valueX = ((x + 84) << 4) + 0x7000;
        sdfDrawTagLink(list, (u32)sdfDevPrintf(valueX,
            positionY, 0xFEFFFF, 0xE, "%3.2f %3.2f %3.2f",
            transform->pos[0], transform->pos[1], transform->pos[2]));
        y += 2;
        rotationY = (y * 6 << 4) + 0x7900;
        sdfDrawTagLink(list, (u32)sdfDevPrintf(labelX,
            rotationY, 0xFEFFFF, 0, D_003BA9A8));
        sdfDrawTagLink(list, (u32)sdfDevPrintf(valueX,
            rotationY, 0xFEFFFF, 0xE, "%3.2f %3.2f %3.2f %3.2f",
            transform->rotation[0], transform->rotation[1],
            transform->rotation[2], transform->rotation[3]));
        y += 2;
        scaleY = (y * 6 << 4) + 0x7900;
        sdfDrawTagLink(list, (u32)sdfDevPrintf(labelX,
            scaleY, 0xFEFFFF, 0, D_003BA9B0));
        sdfDrawTagLink(list, (u32)sdfDevPrintf(valueX,
            scaleY, 0xFEFFFF, 0xE, "%3.1f %3.1f %3.1f",
            transform->scale[0], transform->scale[1], transform->scale[2]));
        lines = 8;
    }
    return lines;
}

extern const char D_0039F5F8[]; /* "FLAG  : 0x%08X" */

u32 DispObjectBaseData(dds3Basic_t *worldNode, s32 x, s32 y, void *list) {
    dds3ObjectBaseControlWorkData_t *handle;
    void *command;

    handle = dds3GetBaseControlWorkData(worldNode);
    command = sdfDevPrintf((x * 3 << 6) + 0x7000,
                                           (y * 3 << 5) + 0x7900,
                                           0xFEFFFF,
                                           0,
                                           D_0039F5F8,
                                           handle->flags);
    sdfDrawTagLink(list, (u32)command);
    return 1;
}


#include "dds3_world_primary.h"
#include "dds3_scene_membership.h"
#include "sdf_packet_alloc.h"
extern sdfGrOt_t D_00325708;
extern s8 D_00324543[];
extern s32 evtSceneDispProcess(void *task);

/* Draw the task's status panel; the returned value is the follow-up handler (or -1 / 0). */
s32 dds3DebugBasic(dds3ProcessID_t *task) {
    s32 width;
    dds3Basic_t *node;
    sdfDrawTag_t *list;
    sdfDrawTag_t *spriteList;

    if (dds3GetActiveScene() == NULL) {
        return -1;
    }
    node = (dds3Basic_t *)dds3GetProcessData(task);
    if (dds3SceneBasicExistBasic(dds3GetActiveScene(), node) == DDS3_SCENE_MEMBERSHIP_ABSENT) {
        return (s32)evtSceneDispProcess;
    }
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    width = DispBaseData(node, 3, 0xA, list) + 0xB;
    if ((node->kindTag >> 24) < 0xA) {
        if ((node->kindTag >> 24) >= 4) {
            width += DispObjectBaseData(node, 3, width, list);
        }
    }
    spriteList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(spriteList);
    dds3ConsPanelTagLink(spriteList, 0x24, 0x72, 0x23, width - 9);
    D_00325708.regFunc(&D_00325708, spriteList);
    D_00325708.regFunc(&D_00325708, list);
    if (D_00324543[0] < 0) {
        return (s32)evtSceneDispProcess;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "basic/dds3DbgBasic", D_0039F5F8);

INCLUDE_SDATA(const s32, "basic/dds3DbgBasic", D_003BA9A0);

INCLUDE_SDATA(const s32, "basic/dds3DbgBasic", D_003BA9A8);

INCLUDE_SDATA(const s32, "basic/dds3DbgBasic", D_003BA9B0);

