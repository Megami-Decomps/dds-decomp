#include "common.h"
#include "btl_state.h"
#include "btl_command.h"
#include "pcp_vu0.h"

extern s32 btlGetRuntime(void);

extern void btlStartSkillEventTask(u32);

extern void scrSetCurrentActor(u32, u32);

extern s32 btlReleaseScriptResource(void);

extern void func_0035C860(char *, const char *, const char *, s32);

extern s32 kwlnTaskIsRegistered(s32);

extern char btlPrimaryScriptResourceName[];

extern s32 scrCreateTaskForProcessId(s32, s32, s32);

extern char btlSecondaryScriptResourceName[];

/* The owner stores the one-based process number used to launch script tasks. */
typedef struct BattleScriptOwner {
    u8 pad00[0x20];
    s32 processNumber; /* +0x20; script process ID is processNumber - 1 */
} BattleScriptOwner;

s32 btlFindScriptResource(char *name) {
    char path[128];
    BtlState *battle = (BtlState *)btlGetRuntime();
    if (battle->scriptHandle == 0) {
        return -1;
    }
    func_0035C860(path, "e%03d_%s", battle->eventTaskId, name);
    return bfFindScriptIndexByName(battle->scriptHandle, path);
}

void btlStartSkillEventTask(u32 skill) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    s32 handle;
    if (battle->eventTaskId == -1) {
        return;
    }
    handle = scrCreateTaskForProcessId(((BattleScriptOwner *)battle->scriptOwner)->processNumber - 1,
                            battle->scriptHandle, skill);
    scrSetCurrentActor(handle, 0);
    func_00101968((s32)battle->scriptOwner, handle);
    battle->scriptTask = handle;
}

s32 btlReleaseScriptResource(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    if (battle->eventTaskId == -1) {
        return 1;
    }
    if (kwlnTaskIsRegistered(battle->scriptTask) == 0) {
        battle->scriptTask = 0;
        return 1;
    }
    return 0;
}

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

void btlStartPrimaryScriptTask(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    s32 scriptId;
    s32 taskId;
    if (battle->eventTaskId == -1) {
        return;
    }
    scriptId = btlFindScriptResource(btlPrimaryScriptResourceName);
    if (scriptId == -1) {
        return;
    }
    taskId = scrCreateTaskForProcessId(((BattleScriptOwner *)battle->scriptOwner)->processNumber - 1,
                            battle->scriptHandle, scriptId);
    scrSetCurrentActor(taskId, 0);
    func_00101968((s32)battle->scriptOwner, taskId);
    battle->scriptTask = taskId;
    battle->scriptFlags |= 1;
}

s32 btlReleaseScriptResourceA(void) {
    return btlReleaseScriptResource();
}

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

void btlStartSecondaryScriptTask(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    s32 scriptId;
    s32 taskId;
    if (battle->eventTaskId == -1) {
        return;
    }
    scriptId = btlFindScriptResource(btlSecondaryScriptResourceName);
    if (scriptId == -1) {
        return;
    }
    taskId = scrCreateTaskForProcessId(((BattleScriptOwner *)battle->scriptOwner)->processNumber - 1,
                            battle->scriptHandle, scriptId);
    scrSetCurrentActor(taskId, 0);
    func_00101968((s32)battle->scriptOwner, taskId);
    battle->scriptTask = taskId;
    battle->scriptFlags |= 2;
}

s32 btlReleaseScriptResourceB(void) {
    return btlReleaseScriptResource();
}
