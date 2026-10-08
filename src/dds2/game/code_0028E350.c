#include "common.h"
#include "dat_state.h"
#include "mnu_list.h"
#include "mnu_mantra.h"

extern MantraNodePos *mnuGetMantraNodePositionRecord(s16);
extern u32 ptyGetProfileRecordCap(u16);
extern DatProfileRecord *ptyGetProfileRecordPointer(DatPartyRecord *, u16);
struct MantraPanelAnimation;
extern struct MantraPanelAnimation *mnuFindPanelSlotById(MantraPanelPool *, s32, s8);
extern u32 mnuQueuePanelAnimationTransition(struct MantraPanelAnimation *, u32, s16);
extern void func_00314868(DatPartyRecord *, u16);
extern void mnuStorePanelEntry(s32, s32);

static inline u16 *mantraFlagEntry(MantraFlagResource *work, s32 id) {
    return work->flags + id;
}

void func_0028E350(MnuStatusResource *resource, u16 id) {
    MantraNodePos *position = mnuGetMantraNodePositionRecord((s16)id);
    struct MenuListNode *node = resource->list->first;
    u32 flags = 0;
    s32 row;

    if (node != 0) {
        row = 0;
        do {
            DatPartyRecord *party = node->partyRecord;
            MantraFlagResource *work = resource->menu.slots[row];
            struct MantraPanelAnimation *panel;
            u16 *selected;

            if (position->selector.packed & 0x100) {
                DatProfileRecord *profile;

                flags |= 1;
                panel = mnuFindPanelSlotById(resource->menu.resource, id, 6);
                mnuQueuePanelAnimationTransition(panel, 2, 0);
                selected = mantraFlagEntry(work, id);
                *selected = (*selected & 0xFFF0) | 0x901;
                profile = ptyGetProfileRecordPointer(party, id);
                profile->value = ptyGetProfileRecordCap(id);
                func_00314868(party, id);
            } else {
                MantraNodePos **neighbors;
                MantraNodePos *neighbor;
                s32 i;

                flags |= 2;
                panel = mnuFindPanelSlotById(resource->menu.resource, id, 6);
                mnuQueuePanelAnimationTransition(panel, 1, 0);
                selected = mantraFlagEntry(work, id);
                *selected = (*selected & 0xFFF2) | 0x802;
                i = 0;
                neighbors = position->neighbors;
                do {
                    neighbor = *neighbors++;
                    if (neighbor != 0 && ((*mantraFlagEntry(work, neighbor->id) >> 8) & 1)) {
                        *selected = (*selected & 0xFFF0) | 1;
                        break;
                    }
                    i++;
                } while (i < 6);
            }
            node = node->next;
            row++;
        } while (node != 0);
    }

    if (flags & 1) {
        mnuStorePanelEntry(0x20008, 0);
        return;
    }
    if (flags & 2) {
        mnuStorePanelEntry(0x20007, 0x14);
    }
}
