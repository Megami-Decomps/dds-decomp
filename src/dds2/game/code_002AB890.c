#include "mnu.h"

extern s32 datGameState;

extern s32 kwlnTaskGetUserValue();

extern void sdfReleaseResourceAllocation(s32);

extern s64 func_002C4038(s32, s32 *, u64, u64);

extern void func_002AAE80(s32);

extern void mnuCreateStaffImageSprite(s32);


/* Inventory quantities are byte entries indexed by each caller's original item ID. */
typedef struct SaveItemCounts {
    u8 pad00[0x1340];
    u8 counts[0x100];
} SaveItemCounts;

/* The menu buffer carries its allocation ID and five owned window handles. */
typedef struct MenuResourceSet {
    s32 allocation;
    u8 pad04[4];
    u32 first;
    u32 second;
    u32 third;
    u32 fourth;
    u32 fifth;
    u8 pad1C[0x1C];
    s32 selection;
} MenuResourceSet;

extern void func_002AB690(s32, s32, s32, s32, s32, s32, s32);
extern void func_002AB8F0(s32);

extern void func_002BAF50(s32, s32);
extern u8 func_002BDA50(s32 index);


void func_002AB890(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    func_002AB690(arg0, arg1, arg2, 0, arg3, arg4, arg5);
}

void func_002AB8C0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    func_002AB690(arg0, arg1, arg2, 0x200, arg3, arg4, arg5);
}

INCLUDE_ASM(const s32, "game/code_002AB890", func_002AB8F0);

void mnuDestroyResourceOwnerWindowContainers(MenuStaffContext *object) {
    MenuResourceSet *resources;

    resources = (MenuResourceSet *)object->menu;
    mnuDestroyWindowContainer(resources->first);
    mnuDestroyWindowContainer(resources->second);
}

extern void func_002B9720(s32);

s32 mnuIsStaffWindowReadyForItem(s32 itemId, MenuStaffContext *owner) {
    MenuResourceSet *resources = (MenuResourceSet *)owner->menu;

    if (((SaveItemCounts *)datGameState)->counts[itemId & 0xFFFF] == 0) {
        func_002B9720(resources->first);
    }
    return ((MenuStaffList *)resources->first)->window->panelActive != 0;
}

INCLUDE_ASM(const s32, "game/code_002AB890", func_002ABD60);

void func_002ABEB0(MenuStaffContext *object) {
    mnuDestroyWindowContainer(((MenuResourceSet *)object->menu)->third);
}

INCLUDE_ASM(const s32, "game/code_002AB890", func_002ABED8);

INCLUDE_ASM(const s32, "game/code_002AB890", func_002AC050);

INCLUDE_ASM(const s32, "game/code_002AB890", func_002AC408);

void func_002AC660(MenuStaffContext *object) {
    mnuDestroyWindowContainer(((MenuResourceSet *)object->menu)->fourth);
}

s32 mnuUpdateStaffEntrySelectionFlags(s32 previousIndex, s32 selectedIndex, MenuStaffContext *owner) {
    MenuStaffNode *node = ((MenuStaffList *)((MenuResourceSet *)owner->menu)->fourth)->window->head;
    s32 index;

    if (node != NULL) {
        do {
            index = node->entryIndex;
            if (index == previousIndex) {
                node->flags &= ~1;
                if (func_002BDA50(index) != 0) {
                    node->flags |= 4;
                }
            }
            if (index == selectedIndex) {
                node->flags |= 1;
                node->flags &= ~4;
            }
            node = node->next;
        } while (node != NULL);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002AB890", func_002AC750);

INCLUDE_ASM(const s32, "game/code_002AB890", func_002AC8F0);

void func_002ACA98(MenuStaffContext *object) {
    mnuDestroyWindowContainer(((MenuResourceSet *)object->menu)->fifth);
}

s32 func_002ACAC0(s32 itemId, MenuStaffContext *owner) {
    MenuResourceSet *resources = (MenuResourceSet *)owner->menu;

    if (((SaveItemCounts *)datGameState)->counts[itemId] == 0) {
        func_002B9720(resources->fifth);
    }
    return ((MenuStaffList *)resources->fifth)->window->panelActive != 0;
}

void func_002ACB18(u32 arg0) {
    mnuSwitchCampVisualCategory(2, arg0);
}

void func_002ACB38(s32 object) {
}

u32 mnuInitializeWindowOwnerResourceSet(void) {
    MenuStaffContext *context = (MenuStaffContext *)kwlnTaskGetUserValue();
    s32 handle = sdfAllocGeneralBlock(0x54);
    MenuResourceSet *resource = sdfResourceRetainAddress(handle);

    context->menu = (u8 *)resource;
    memset(resource, 0, 0x54);
    resource->allocation = handle;
    func_002ACB18((u32)context);
    func_002AB8F0((s32)context);
    /* This input word at +0x118 is not identified in the shared context yet. */
    mnuConfigurePanelResource(*(s32 *)((u8 *)context + 0x118), context->spriteArg2, 0, 0);
    func_002BAF50((s32)context->activeWindow, (s32)context->tail);
    mnuSeekListNode(0, context->activeWindow->window);
    return 1;
}

/* Close the staff selection state: drop the owner's window containers, run
 * the owner's teardown hook, then close the party's resource menu. */
s32 mnuDestroyWindowOwnerResourceSet(void) {
    s32 context = kwlnTaskGetUserValue();
    MenuStaffContext *owner = (MenuStaffContext *)context;
    MenuResourceSet *party = (MenuResourceSet *)owner->menu;

    mnuDestroyResourceOwnerWindowContainers(owner);
    func_002ACB38(context);
    sdfReleaseResourceAllocation(party->allocation);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002AB890", func_002ACC50);

extern void func_002AAC98(s32, s32, s32, s32, s32, s32);
extern void mnuUpdateAndDrawWindowTransition(s32, s32, s32, s32, s32);
extern void func_002AA7A0(s32, s32);
extern u8 D_003E7050[];

s64 func_002ACE58(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_002AAE80(callback);
    mnuCreateStaffImageSprite(4);
    func_002AAC98(0,
        ((MenuStaffContext *)context)->activeWindow->window->selectedNode->label,
        (s32)D_003E7050, context, 1, 0x53);
    mnuUpdateAndDrawWindowTransition(0x1e0, 0x350, 0, (s32)((MenuStaffContext *)context)->tail, 0x53);
    func_002AA7A0(0, ((MenuStaffContext *)context)->group);
    return menuSetHandler(context, 1, callback);
}

s64 mnuFinishStaffReturnPopup(s32 callback) {
    return menuSetHandler(kwlnTaskGetUserValue(), 2, callback);
}

extern s32 func_002C5A28(s32, s32, s32, s32);
extern s32 evtGetIndexedEventRecordId(s32);
extern s32 ptySkillApplyFieldUseEffect(s32, s32, s32, s32);
extern void func_0011A118(s32, s32);
extern void mnuInitPartyPanelSlots(s32);
extern void func_002BCA98(s32);
extern void func_002BCAB0(s32);

typedef struct StaffUseSelectionList {
    u8 pad00[0x1C];
    s32 *selectedIndex; /* 0x1C */
} StaffUseSelectionList;

typedef struct StaffUseContext {
    u8 pad00[0xA914];
    StaffUseSelectionList *list; /* 0xA914 */
} StaffUseContext;

s32 mnuUseStaffItem(itemId, context)
s32 itemId;
s32 context;
{
    s32 partyPanel = context + 0x284;
    s32 targetUnit = datGameState + *(((StaffUseContext *)context)->list->selectedIndex) * 0x1C4 + 0xA60;
    s32 result = func_002C5A28(partyPanel, itemId & 0xFFFF, targetUnit, targetUnit);

    if (result != 1) {
        if (result == 2) {
            return 0;
        }
        if (ptySkillApplyFieldUseEffect(partyPanel, evtGetIndexedEventRecordId(itemId) & 0xFFFF, targetUnit, targetUnit) == 0) {
            return 0;
        }
    }
    func_0011A118(itemId, -1);
    mnuInitPartyPanelSlots(context + 0xA928);
    func_002BCA98(partyPanel);
    func_002BCAB0(partyPanel);
    return 1;
}

/* On successful item use, publish the remaining count and record the selected item. */
void mnuApplyResourceSelection(s32 index, s32 context) {
    MenuResourceSet *resources;
    s32 consumed;

    resources = (MenuResourceSet *)((MenuStaffContext *)context)->menu;
    consumed = mnuUseStaffItem();
    if (consumed != 0) {
        ((MenuStaffList *)resources->first)->window->selectedNode->label =
                  (u32)((SaveItemCounts *)datGameState)->counts[index];
        resources->selection = index;
    }
    func_002C1B68(context + 0xaa50, 1);
}

u32 func_002AD0A8(void) {
    s32 context;

    context = kwlnTaskGetUserValue();
    func_002BAF50(((MenuResourceSet *)((MenuStaffContext *)context)->menu)->first, context + 0xb10c);
    return 1;
}

u32 func_002AD0E8(void) {
    s32 context;

    context = kwlnTaskGetUserValue();
    func_002BAF50((u32)((MenuStaffContext *)context)->activeWindow, (s32)((MenuStaffContext *)context)->tail);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002AB890", func_002AD118);

INCLUDE_ASM(const s32, "game/code_002AB890", func_002AD330);
