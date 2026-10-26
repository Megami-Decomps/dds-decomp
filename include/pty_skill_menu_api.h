#ifndef PTY_SKILL_MENU_API_H
#define PTY_SKILL_MENU_API_H

#include "kwln.h"

typedef s32 PtySkillUnavailableMarking;
enum {
    PTY_SKILL_UNAVAILABLE_MARKING_DISABLED = 0,
    PTY_SKILL_UNAVAILABLE_MARKING_ENABLED = 1
};

s32 ptySkillMenuUpdate(dds3ProcessID_t *callback);
s32 ptySkillMenuOpenPartyPage(dds3ProcessID_t *callback);
s32 ptySkillMenuBuildEquippedSlots(
    PtySkillUnavailableMarking markUnavailableSkills, dds3ProcessID_t *callback);
void ptySkillMenuHandleSelection(dds3ProcessID_t *callback);
void ptySkillMenuHandleSlotReorder(dds3ProcessID_t *callback);

#endif
