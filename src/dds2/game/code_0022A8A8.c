#include "sdk_ps2.h"
#include "common.h"
#include "btl_script_resource_api.h"
#include "kwln_task_add_child.h"
#include "btl_state.h"
#include "btl_event_pack_id_api.h"
#include "btl_command.h"
#include "pcp_vu0.h"
#include "kwln.h"
#include "kwln_process_registration.h"
#include "scr.h"
#include "bf_flw0_header.h"
extern void btlStartSkillEventTask(u32);





extern char btlPrimaryScriptResourceName[];

extern dds3ProcessID_t *scrStartScript2(u32 priority, const BfFlw0Header *script, ScrProcedureIndex procedureIndex);

extern char btlSecondaryScriptResourceName[];

ScrProcedureIndex btlFindScriptResource(const char *name) {
    char path[128];
    BtlState *battle = (BtlState *)btlGetRuntime();
    if (battle->scriptHandle == 0) {
        return SCR_PROCEDURE_INDEX_NOT_FOUND;
    }
    sprintf(path, "e%03d_%s", battle->eventPackId, name);
    return scrSearchProcedureName((const BfFlw0Header *)battle->scriptHandle, path);
}

/* Start the requested skill script as a child of the battle owner task.
 * The new task uses the owner's scheduler priority minus one. */
void btlStartSkillEventTask(u32 skill) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    dds3ProcessID_t *taskId;
    if (battle->eventPackId == BTL_EVENT_PACK_ID_NONE) {
        return;
    }
    taskId = scrStartScript2(battle->scriptOwner->priority - 1,
                            (const BfFlw0Header *)battle->scriptHandle, skill);
    scrSetUserData(taskId, NULL);
    dds3LinkProcess(battle->scriptOwner, taskId);
    battle->scriptTask = taskId;
}

BtlScriptResourceReleaseReadiness btlReleaseScriptResource(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    if (battle->eventPackId == BTL_EVENT_PACK_ID_NONE) {
        return BTL_SCRIPT_RESOURCE_RELEASE_READY;
    }
    if (dds3ExistProcess(battle->scriptTask) == KWLN_PROCESS_NOT_REGISTERED) {
        battle->scriptTask = 0;
        return BTL_SCRIPT_RESOURCE_RELEASE_READY;
    }
    return BTL_SCRIPT_RESOURCE_RELEASE_PENDING;
}

BtlPrimaryScriptTaskAvailability btlCanStartPrimaryScriptTask(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    if (battle->eventPackId == BTL_EVENT_PACK_ID_NONE) {
        return BTL_PRIMARY_SCRIPT_TASK_CANNOT_START;
    }
    if (battle->scriptFlags & 1) {
        return BTL_PRIMARY_SCRIPT_TASK_CANNOT_START;
    }
    if (battle->scriptHandle == 0) {
        return BTL_PRIMARY_SCRIPT_TASK_CANNOT_START;
    }
    return btlFindScriptResource(btlPrimaryScriptResourceName) != SCR_PROCEDURE_INDEX_NOT_FOUND
        ? BTL_PRIMARY_SCRIPT_TASK_CAN_START
        : BTL_PRIMARY_SCRIPT_TASK_CANNOT_START;
}

/* Find the primary script entry, launch it at owner priority minus one, and mark it started. */
void btlStartPrimaryScriptTask(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    ScrProcedureIndex scriptId;
    dds3ProcessID_t *taskId;
    if (battle->eventPackId == BTL_EVENT_PACK_ID_NONE) {
        return;
    }
    scriptId = btlFindScriptResource(btlPrimaryScriptResourceName);
    if (scriptId == SCR_PROCEDURE_INDEX_NOT_FOUND) {
        return;
    }
    taskId = scrStartScript2(battle->scriptOwner->priority - 1,
                            (const BfFlw0Header *)battle->scriptHandle, scriptId);
    scrSetUserData(taskId, NULL);
    dds3LinkProcess(battle->scriptOwner, taskId);
    battle->scriptTask = taskId;
    battle->scriptFlags |= 1;
}

BtlScriptResourceReleaseReadiness btlReleaseScriptResourceA(void) {
    return btlReleaseScriptResource();
}

BtlSecondaryScriptResourceAvailability btlHasScriptResource(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();

    if (battle->eventPackId == BTL_EVENT_PACK_ID_NONE) {
        return 0;
    }
    if (battle->scriptHandle == 0) {
        return 0;
    }
    if ((battle->battleFlags & 0x800) == 0 || battle->terminalOutcome != 1) {
        return 0;
    }
    return btlFindScriptResource(btlSecondaryScriptResourceName) != SCR_PROCEDURE_INDEX_NOT_FOUND;
}

/* Find the secondary script entry, launch it at owner priority minus one, and mark it started. */
void btlStartSecondaryScriptTask(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    ScrProcedureIndex scriptId;
    dds3ProcessID_t *taskId;
    if (battle->eventPackId == BTL_EVENT_PACK_ID_NONE) {
        return;
    }
    scriptId = btlFindScriptResource(btlSecondaryScriptResourceName);
    if (scriptId == SCR_PROCEDURE_INDEX_NOT_FOUND) {
        return;
    }
    taskId = scrStartScript2(battle->scriptOwner->priority - 1,
                            (const BfFlw0Header *)battle->scriptHandle, scriptId);
    scrSetUserData(taskId, NULL);
    dds3LinkProcess(battle->scriptOwner, taskId);
    battle->scriptTask = taskId;
    battle->scriptFlags |= 2;
}

BtlScriptResourceReleaseReadiness btlReleaseScriptResourceB(void) {
    return btlReleaseScriptResource();
}
