#ifndef MNU_CAMP_WORK_H
#define MNU_CAMP_WORK_H

#include "mnu.h"
#include "mnu_shop.h"
#include "mnu_scroll_panel.h"

struct EffectList;

#ifndef VERSION_DDS2
/* The camp task userdata owner allocated and shared by the input, draw, and
 * teardown tasks. Its complete native allocation is 0x924 bytes. */
typedef struct StaffMenuWork {
    u32 resource;
    u8 pad04[4];
    MenuPopupState panel;
    s32 value54;
    u8 pad58[4];
    struct EffectList *resourceQueue;
    StaffSlots staffSlots;
    u32 categoryPair[2];
    u32 categoryGroup[4];
    u32 partyModels[9];
    u32 singleResource;
    u32 primaryImage;
    MenuPanelHandles *resourceList;
    u32 secondaryImage;
    u32 images[3];
    u32 extraImages[2];
    MenuScrollPanel *scrollPanel;
    u8 background[0x6B0];
    u8 partyPanel[0x124];
    s32 displayMode;
    u8 timer[0x10];
} StaffMenuWork;

typedef char StaffMenuWork_size_must_be_0x924[
    (sizeof(StaffMenuWork) == 0x924) ? 1 : -1];
typedef char StaffMenuWork_staffSlots_offset_check[
    ((u32)&((StaffMenuWork *)0)->staffSlots == 0x60) ? 1 : -1];
#endif

#endif /* MNU_CAMP_WORK_H */
