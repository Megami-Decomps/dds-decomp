#ifndef PTY_SKILL_MENU_API_H
#define PTY_SKILL_MENU_API_H

#include "kwln.h"

s32 ptySkillMenuUpdate(KwlnTask *callback);
s32 ptySkillMenuOpenPartyPage(KwlnTask *callback);
s32 ptySkillMenuBuildEquippedSlots(s32 selectionMode, KwlnTask *callback);
void ptySkillMenuHandleSelection(KwlnTask *callback);
void ptySkillMenuHandleSlotReorder(KwlnTask *callback);

#endif
