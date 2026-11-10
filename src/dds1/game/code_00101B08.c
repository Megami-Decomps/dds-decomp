#include "sdk_ps2.h"
#include "common.h"
#include "sdf_vu0.h"
#include "sdf_column_packet_api.h"
#include "dds3_draw_buffer.h"
#include "dds3_frame_draw_buffer.h"
#include "sdf_packet_list.h"
#include "sdf_draw_packet_groups.h"
#include "dds3_frame_draw_bank.h"
#include "dds3_effect_sprite.h"
#include "dds3Admin.h"
#include "sdf_linked_packet.h"
#include "sdf_dev_start_load_api.h"
#include "kernel.h"
#include "dds3_task_flags.h"
#include "sdf.h"
#include "sdf_pad_mode_api.h"
#include "sdf_thread.h"
#include "sdf_thread_sleep.h"
#include "sdf_projection.h"
#include "sdf_draw.h"
#include "pcp_vu0.h"
#include "sdf_packet_alloc.h"
#include "sdf_message.h"

/* Detach a task from its parent's child list. Detached tasks are left untouched;
 * an attached task must already occur in that parent's sibling chain.
 * The hierarchy link is nextSibling, separate from scheduler listNext/listPrev. */
void dds3UnlinkProcess(dds3ProcessID_t *task) {
    dds3ProcessID_t *parent = task->parent;

    if (parent == 0) {
        return;
    }
    if (parent->child == task) {
        parent->child = task->sibling;
    } else {
        dds3ProcessID_t *previousSibling = parent->child;

        while (previousSibling->sibling != task) {
            previousSibling = previousSibling->sibling;
        }
        previousSibling->sibling = task->sibling;
    }
    task->parent = 0;
    task->sibling = 0;
}

/* Set mask bits on the object selection described by object and scope. */
void dds3SetProcessFlag(dds3ProcessID_t *object, u32 mask, dds3TaskFlagScope_t scope) {
    ChangeProcessFlag(DDS3_TASK_FLAG_UPDATE_SET, object, mask, scope);
}

/* Clear the same scoped object mask bits without changing the selection. */
void dds3ResetProcessFlag(dds3ProcessID_t *object, u32 mask, dds3TaskFlagScope_t scope) {
    ChangeProcessFlag(DDS3_TASK_FLAG_UPDATE_CLEAR, object, mask, scope);
}
