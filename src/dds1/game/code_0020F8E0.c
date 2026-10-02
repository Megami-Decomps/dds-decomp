#include "pcp_vu0.h"
#include "common.h"
#include "btl_state.h"
#include "btl_command.h"
#include "ee_mmi.h"

extern void func_003014F0(char *, const char *, const char *, s32);

extern s32 btlGetRuntime(void);

extern s32 kwlnTaskIsRegistered(s32);

s32 btlFindScriptResource(char *name) {
    char path[128];
    BtlState *battle = (BtlState *)btlGetRuntime();
    if (battle->scriptHandle == 0) {
        return -1;
    }
    func_003014F0(path, "e%03d_%s", battle->eventTaskId, name);
    return bfFindScriptIndexByName(battle->scriptHandle, path);
}

void btlStartSkillEventTask(s32 skill) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    s32 handle;
    if (battle->eventTaskId == -1) {
        return;
    }
    handle = scrCreateTaskForProcessId(*(s32 *)((u8 *)battle->scriptOwner + 0x20) - 1,
                            battle->scriptHandle, skill);
    scrSetCurrentActor(handle, 0);
    func_00101A80(battle->scriptOwner, handle);
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

extern s32 scrCreateTaskForProcessId(s32, s32, s32);

void btlStartPrimaryScriptTask(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    s32 skill;
    s32 handle;
    if (battle->eventTaskId == -1) {
        return;
    }
    skill = btlFindScriptResource(btlPrimaryScriptResourceName);
    if (skill == -1) {
        return;
    }
    handle = scrCreateTaskForProcessId(*(s32 *)((u8 *)battle->scriptOwner + 0x20) - 1,
                            battle->scriptHandle, skill);
    scrSetCurrentActor(handle, 0);
    func_00101A80(battle->scriptOwner, handle);
    battle->scriptTask = handle;
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

void btlStartSecondaryScriptTask(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    s32 skill;
    s32 handle;
    if (battle->eventTaskId == -1) {
        return;
    }
    skill = btlFindScriptResource(btlSecondaryScriptResourceName);
    if (skill == -1) {
        return;
    }
    handle = scrCreateTaskForProcessId(*(s32 *)((u8 *)battle->scriptOwner + 0x20) - 1,
                            battle->scriptHandle, skill);
    scrSetCurrentActor(handle, 0);
    func_00101A80(battle->scriptOwner, handle);
    battle->scriptTask = handle;
    battle->scriptFlags |= 2;
}

s32 btlReleaseScriptResourceB(void) {
    return btlReleaseScriptResource();
}
