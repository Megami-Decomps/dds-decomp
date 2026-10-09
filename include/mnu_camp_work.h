#ifndef MNU_CAMP_WORK_H
#define MNU_CAMP_WORK_H

#include "mnu.h"
#include "mnu_shop.h"
#include "mnu_scroll_panel.h"

struct EffectList;
struct EffectSlotSet;
struct EffMappedResource;

#ifndef VERSION_DDS2
/* The camp task userdata owner allocated and shared by the input, draw, and
 * teardown tasks. Its complete native allocation is 0x924 bytes. */
typedef struct StaffMenuWork {
    struct SdfMemBlock *resource;
    u8 pad04[4];
    MenuPopupState panel;
    s32 value54;
    u8 pad58[4];
    struct EffectList *resourceQueue;
    StaffSlots staffSlots;
    struct EffectSlotSet *categoryPair[2];
    struct EffectSlotSet *categoryGroup[4];
    struct EffectSlotSet *partyModels[9];
    struct EffectSlotSet *singleResource;
    struct EffMappedResource *primaryImage;
    MenuPanelHandles *resourceList;
    struct EffMappedResource *secondaryImage;
    MenuWindowContainer *images[3];
    struct EffMappedResource *extraImages[2];
    MenuScrollPanel *scrollPanel;
    u8 background[0x6B0];
    PartyPanel partyPanel; /* 0x7EC: initialized by mnuInitPartyPanelSlots */
    u8 pad8F8[0x18];
    s32 displayMode;
    u8 timer[0x10];
} StaffMenuWork;

typedef char StaffMenuWork_size_must_be_0x924[
    (sizeof(StaffMenuWork) == 0x924) ? 1 : -1];
typedef char StaffMenuWork_staffSlots_offset_check[
    ((u32)&((StaffMenuWork *)0)->staffSlots == 0x60) ? 1 : -1];
#endif

#endif /* MNU_CAMP_WORK_H */
