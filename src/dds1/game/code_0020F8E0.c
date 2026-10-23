#include "sdk_ps2.h"
#include "pcp_vu0.h"
#include "common.h"
#include "btl_script_resource_api.h"
#include "kwln_task_add_child.h"
#include "btl_state.h"
#include "btl_command.h"
#include "ee_mmi.h"
#include "kwln.h"
#include "scr.h"
#include "bf_flw0_header.h"

s32 btlFindScriptResource(char *name) {
    char path[128];
    BtlState *battle = (BtlState *)btlGetRuntime();
    if (battle->scriptHandle == 0) {
        return -1;
    }
    sprintf(path, "e%03d_%s", battle->eventTaskId, name);
    return scrSearchProcedureName((BfFlw0Header *)battle->scriptHandle, path);
}

extern dds3ProcessID_t *scrStartScript2(s32 priority, BfFlw0Header *script, s32 option);

/* Start the requested skill script as a child of the battle owner task.
 * The new task uses the owner's scheduler priority minus one. */
void btlStartSkillEventTask(s32 skill) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    dds3ProcessID_t *taskId;
    if (battle->eventTaskId == -1) {
        return;
    }
    taskId = scrStartScript2((s32)battle->scriptOwner->priority - 1,
                            (BfFlw0Header *)battle->scriptHandle, skill);
    scrSetUserData(taskId, NULL);
    dds3LinkProcess(battle->scriptOwner, taskId);
    battle->scriptTask = (s32)taskId;
}

s32 btlReleaseScriptResource(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    if (battle->eventTaskId == -1) {
        return 1;
    }
    if (dds3ExistProcess((dds3ProcessID_t *)battle->scriptTask) == 0) {
        battle->scriptTask = 0;
        return 1;
    }
    return 0;
}

extern char btlPrimaryScriptResourceName[];

s32 btlCanStartPrimaryScriptTask(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    if (battle->eventTaskId == -1) {
        return 0;
    }
    if (battle->scriptFlags & 1) {
        return 0;
    }
    if (battle->scriptHandle == 0) {
        return 0;
    }
    return btlFindScriptResource(btlPrimaryScriptResourceName) != -1;
}


/* Find the primary script entry, launch it at owner priority minus one, and mark it started. */
void btlStartPrimaryScriptTask(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    s32 scriptId;
    dds3ProcessID_t *taskId;
    if (battle->eventTaskId == -1) {
        return;
    }
    scriptId = btlFindScriptResource(btlPrimaryScriptResourceName);
    if (scriptId == -1) {
        return;
    }
    taskId = scrStartScript2((s32)battle->scriptOwner->priority - 1,
                            (BfFlw0Header *)battle->scriptHandle, scriptId);
    scrSetUserData(taskId, NULL);
    dds3LinkProcess(battle->scriptOwner, taskId);
    battle->scriptTask = (s32)taskId;
    battle->scriptFlags |= 1;
}

s32 btlReleaseScriptResourceA(void) {
    return btlReleaseScriptResource();
}

extern char btlSecondaryScriptResourceName[];

s32 btlHasScriptResource(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();

    if (battle->eventTaskId == -1) {
        return 0;
    }
    if (battle->scriptHandle == 0) {
        return 0;
    }
    if ((battle->battleFlags & 0x800) == 0 || battle->eventReady != 1) {
        return 0;
    }
    return btlFindScriptResource(btlSecondaryScriptResourceName) != -1;
}

/* Find the secondary script entry, launch it at owner priority minus one, and mark it started. */
void btlStartSecondaryScriptTask(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    s32 scriptId;
    dds3ProcessID_t *taskId;
    if (battle->eventTaskId == -1) {
        return;
    }
    scriptId = btlFindScriptResource(btlSecondaryScriptResourceName);
    if (scriptId == -1) {
        return;
    }
    taskId = scrStartScript2((s32)battle->scriptOwner->priority - 1,
                            (BfFlw0Header *)battle->scriptHandle, scriptId);
    scrSetUserData(taskId, NULL);
    dds3LinkProcess(battle->scriptOwner, taskId);
    battle->scriptTask = (s32)taskId;
    battle->scriptFlags |= 2;
}

s32 btlReleaseScriptResourceB(void) {
    return btlReleaseScriptResource();
}
