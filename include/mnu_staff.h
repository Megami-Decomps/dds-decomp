#ifndef MNU_STAFF_H
#define MNU_STAFF_H

#include "mnu.h"

/* Staff-list nodes with unavailable entries carry this lifecycle flag. */
#define MNU_STAFF_NODE_UNAVAILABLE 1

struct DatPartyRecord;
/* Field-use skills operate on the selected party records and their page. */
s32 ptySkillApplyFieldUseEffect(MenuPageWindow *page, u16 ability,
                               struct DatPartyRecord *target,
                               struct DatPartyRecord *selected);

/* Rebuild the five party-panel rows from occupied party records. */
void mnuInitPartyPanelSlots(PartyPanel *party);

void mnuStepPartyPanelListFromInput(s32 mode, MenuPageWindow *page);
struct KwlnTask;
/* Test whether the skill menu retains a selected list-node ID. */
u32 mnuHasSelectedListNodeId(struct KwlnTask *callback);
#ifdef VERSION_DDS1
void mnuClearListFlags(s32 which, MenuPageWindow *page);
#endif

#ifdef VERSION_DDS2
struct MenuPanelState;

/* Native staff components: page 0x284, party 0xA928 and fade 0xB10C. */
void mnuInitializeWindowFadeState(MenuFadeFields *fade);
/* Generic task/resource addresses are recovered as window and fade owners inside. */
void mnuBeginWindowFadeTransition(void *windowAddress, void *work);
void mnuUpdateAndDrawWindowTransition(s32 x, s32 y, s32 depth,
                                     MenuFadeFields *fade, s32 option);
void mnuClearActionFlags(s32 kind, MenuPageWindow *page);
s32 mnuUseStaffItem(s32 itemId, MenuStaffContext *context);
void mnuApplyResourceSelection(s32 itemId, MenuStaffContext *context);
s32 mnuIsStaffWindowReadyForItem(s32 itemId, MenuStaffContext *context);
s32 mnuGetAbilityTargetCategory(u16 commandId);
s32 mnuGetMatchingPartyEntryMask(struct DatPartyRecord *entry);
s32 mnuFindMatchingPartyEntryIndex(struct DatPartyRecord *entry);
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
MenuWindowContainer *mnuCreateStaffResourceListWindow(
    void *entries, s32 entryCount, s32 width, s32 rowHeight, u8 *work,
    void *requiredFlags);
void mnuSetWindowContainerState(MenuWindowContainer *window, u32 state);
void mnuDestroyWindowContainer(MenuWindowContainer *window);
void mnuDestroyPanelState(struct MenuPanelState *panel);
s32 mnuConsumeEntryCost(s32 commandId, struct DatPartyRecord *entry);
#endif

#endif
