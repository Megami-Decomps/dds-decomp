#ifndef PTY_SKILL_MENU_API_H
#define PTY_SKILL_MENU_API_H

#include "kwln.h"

s32 ptySkillMenuUpdate(dds3ProcessID_t *callback);
s32 ptySkillMenuOpenPartyPage(dds3ProcessID_t *callback);
s32 ptySkillMenuBuildEquippedSlots(s32 selectionMode, dds3ProcessID_t *callback);
void ptySkillMenuHandleSelection(dds3ProcessID_t *callback);
void ptySkillMenuHandleSlotReorder(dds3ProcessID_t *callback);

#endif
