#ifndef MNU_STAFF_H
#define MNU_STAFF_H

#include "mnu.h"

#ifdef VERSION_DDS2
struct DatPartyRecord;
struct MenuPanelState;

/* Native staff components: page 0x284, party 0xA928 and fade 0xB10C. */
void mnuInitializeWindowFadeState(MenuFadeFields *fade);
/* Generic task/resource addresses are recovered as window and fade owners inside. */
void mnuBeginWindowFadeTransition(void *windowAddress, void *work);
void mnuUpdateAndDrawWindowTransition(s32 x, s32 y, s32 depth,
                                     MenuFadeFields *fade, s32 option);
void mnuClearActionFlags(s32 kind, MenuPageWindow *page);
void mnuStepPartyPanelListFromInput(s32 mode, MenuPageWindow *page);
void mnuInitPartyPanelSlots(PartyPanel *party);
s32 mnuUseStaffItem(s32 itemId, MenuStaffContext *context);
void mnuApplyResourceSelection(s32 itemId, MenuStaffContext *context);
s32 mnuIsStaffWindowReadyForItem(s32 itemId, MenuStaffContext *context);
s32 mnuGetAbilityTargetCategory(u16 commandId);
s32 mnuGetMatchingPartyEntryMask(struct DatPartyRecord *entry);
s32 mnuFindMatchingPartyEntryIndex(struct DatPartyRecord *entry);
s32 ptySkillApplyFieldUseEffect(MenuPageWindow *page, u16 ability,
                               struct DatPartyRecord *target,
                               struct DatPartyRecord *selected);
s32 func_002C5A28(MenuPageWindow *page, u16 ability,
                  struct DatPartyRecord *target, struct DatPartyRecord *selected);
s32 mnuTryUseFieldSkill(PartyPanel *party, MenuPageWindow *page,
                       struct DatPartyRecord *entry, s32 commit);
s32 mnuUseFieldSkillOnParty(PartyPanel *party, MenuPageWindow *page, s32 commit);
s32 mnuIsEntryCostUnaffordable(u16 commandId, struct DatPartyRecord *entry);
void func_002BCA98(MenuPageWindow *page);
void func_002BCAB0(MenuPageWindow *page);
s32 func_002C6008(PartyPanel *party, MenuPageWindow *page,
                 struct DatPartyRecord *entry, s32 commit);
MenuWindowContainer *func_002A9BF8(void *labels, s32 count, s32 width,
                                 s32 rowHeight, u8 *work, void *options);
void mnuSetWindowContainerState(MenuWindowContainer *window, u32 state);
void mnuDestroyWindowContainer(MenuWindowContainer *window);
void mnuDestroyPanelState(struct MenuPanelState *panel);
s32 mnuConsumeEntryCost(s32 commandId, struct DatPartyRecord *entry);
#endif

#endif
