#ifndef MNU_SHOP_H
#define MNU_SHOP_H

#include "common.h"
#include "mnu_transition.h"

struct MenuList;
struct EffectSlotSet;
struct EffMappedResource;
struct SdfMemBlock;

/* Shop stock by progress row: unlock flag, row price percent, then 32 stock entries. */
typedef struct ShopRankPriceEntry {
    u16 itemId;
    u8 mode;
    u8 pricePercent; /* 0: use the row's percent */
    u32 flags;
} ShopRankPriceEntry;

typedef struct ShopRankPriceRow {
    s16 unlockFlag;
    u16 pricePercent;
    ShopRankPriceEntry entries[0x20];
} ShopRankPriceRow;

#ifdef VERSION_DDS2
struct MenuIconSprites;

/* The native window constructor allocates 0x98 bytes; panel layout is 0x38. */
struct MenuIconState {
    u32 kind;
    u32 unk4;
    s32 count;
    struct EffectSlotSet *sprite[6];
    u32 left;
    u32 top;
    u32 right;
    u32 bottom;
    s32 fade;
};

/* Mode 5 callers pass only kind and resource; native mode 4 consumes material. */
struct MenuIconState *mnuCreatePanelIconState();
void mnuReleaseResourceList(struct MenuIconState *list);

typedef struct MenuWindowContainer {
    s32 id;                /* 0x00 */
    u32 flags;             /* 0x04 */
    s32 originX;           /* 0x08 */
    s32 originY;           /* 0x0C */
    s32 width;             /* 0x10 */
    s32 height;            /* 0x14 */
    struct MenuList *list; /* 0x18 */
    s32 field1C;           /* 0x1C */
    struct EffectSlotSet *spriteResource; /* 0x20 */
    s32 param24;           /* 0x24 */
    s32 param28;           /* 0x28 */
    struct {
        struct EffectSlotSet *sprite;
        u32 parameter;
    } decorations[3];      /* 0x2C: optional window decoration sprites */
    u32 decorationX[3];    /* 0x44 */
    s32 scale50;           /* 0x50 */
    s32 scale54;           /* 0x54 */
    struct MenuIconState panel; /* 0x58: embedded drawable panel layout */
    struct MenuIconSprites *resource; /* 0x90: owned sprite-resource bundle */
    u32 fadeScale;         /* 0x94: window/list opacity scale; full fade is 0x100 */
} MenuWindowContainer;

/* Sprite slots are resource-set owners; parameters and offsets stay words. */
void mnuInitializeBasicWindowLayout(MenuWindowContainer *menu,
                                    struct EffectSlotSet *firstSprite,
                                    u32 firstParameter);
void mnuSetWindowContainerLayout(MenuWindowContainer *menu,
                                 struct EffectSlotSet *firstSprite,
                                 u32 firstParameter,
                                 struct EffectSlotSet *secondSprite,
                                 u32 secondX,
                                 u32 secondParameter,
                                 struct EffectSlotSet *thirdSprite,
                                 u32 thirdParameter,
                                 u32 thirdX);
void mnuSetWindowOverlaySprite(MenuWindowContainer *menu,
                               struct EffectSlotSet *sprite);
void mnuSetWindowEntryParameters(u32 first, MenuWindowContainer *menu,
                                 struct EffectSlotSet *spriteResource,
                                 u32 third, u32 fourth);

typedef char MenuIconState_size_must_be_0x38[(sizeof(struct MenuIconState) == 0x38) ? 1 : -1];
typedef char MenuWindowContainer_size_must_be_0x98[(sizeof(MenuWindowContainer) == 0x98) ? 1 : -1];
#else
struct MenuWindowSpriteGroup;
struct EffMappedResource;

void mnuConfigureWindowSpriteSlots(struct MenuWindowSpriteGroup *group,
                                    struct EffMappedResource *target);

/* DDS1's generic window panel is embedded at +0x4C and copied as 0x38 bytes. */
typedef struct MenuPanelHandles {
    u32 panelKind;
    u8 pad04[4];
    s32 count;
    struct EffectSlotSet *handles[6];
    u32 left;
    u32 top;
    u32 right;
    u32 bottom;
    s32 transition;
} MenuPanelHandles;

MenuPanelHandles *mnuCreatePanelSpriteHandles(u32 panelKind,
                                              struct EffectSlotSet *resource,
                                              struct EffMappedResource *target);

/* The DDS1 generic window constructor allocates and clears 0x8C bytes. */
typedef struct MenuWindowContainer {
    s32 id;
    u32 flags;
    s32 width;
    s32 height;
    u32 unk10; /* Set by staff-panel setup; meaning unknown. */
    struct MenuList *list;
    s32 entryValue;
    s32 entryX;
    s32 entryY;
    s32 alternateEntryY;
    s32 entryOption;
    struct EffectSlotSet *frameResources;
    s32 frameSlot;
    struct EffectSlotSet *spriteResources;
    u32 spriteSlot;
    struct EffectSlotSet *overlayResources;
    u32 overlaySlot;
    s32 firstDecorationFade;
    s32 secondDecorationFade;
    MenuPanelHandles panel;
    struct MenuWindowSpriteGroup *textures;
    s32 fadeScale; /* 0x88: window/list opacity scale; full fade is 0x100 */
} MenuWindowContainer;

void mnuSetWindowOverlaySprite(MenuWindowContainer *window,
                                struct EffectSlotSet *resources);
void mnuConfigureWindowSpriteAndGrid(MenuWindowContainer *window,
                                     struct EffectSlotSet *frameResources, s32 frameSlot,
                                     struct EffectSlotSet *spriteResources, u32 spriteSlot,
                                     u32 overlaySlot);
void mnuForwardDupArg(MenuWindowContainer *window,
                      struct EffectSlotSet *frameResources, s32 frameSlot,
                      struct EffectSlotSet *spriteResources, s32 spriteSlot);

/* 2443F8 allocates 0x10 bytes. The operation-row renderer 25E820 reads
 * countdown/mode; 245C98's initialization stores the pending selection. */
typedef struct MnuShopListContext {
    s32 countdown;
    s32 mode;
    s32 unk08;
    u16 extraOption;
    s16 pendingSelection;
} MnuShopListContext;

/* DDS1 mnuShopCreateScene allocates and clears this complete 0xB4-byte owner. */
typedef struct ShopScene {
    struct SdfMemBlock *resourceHandle;
    u8 pad04[4];
    MenuPopupState transitionWork;
    s32 dispatchState;
    s32 stateTable;
    u8 resourcePair[4];
    s32 pairedHandle;
    struct EffectSlotSet *spriteResource;
    s32 batchState;
    MenuWindowContainer *sprite;
    MenuWindowContainer *window;
    struct EffMappedResource *batches[2];
    s32 initialSelection;
    s32 counter;
    s32 menuMode;
    u8 pad88[4];
    s32 count8C;
    s16 extraOption;
    s16 pendingSelection;
    s32 scanIndex;
    s32 count98;
    s32 previousValue;
    s32 elapsedFrames;
    s32 progressTicks;
    s32 frames;
    s32 action;
    s16 substate;
    s8 pulseFrame;
    s8 atLimit;
} ShopScene;

s32 mnuShopHasPendingFlag(ShopScene *unused);

typedef char MnuShopListContext_size_must_be_0x10[(sizeof(MnuShopListContext) == 0x10) ? 1 : -1];

typedef char MenuPanelHandles_size_must_be_0x38[(sizeof(MenuPanelHandles) == 0x38) ? 1 : -1];
typedef char MenuWindowContainer_size_must_be_0x8C[(sizeof(MenuWindowContainer) == 0x8C) ? 1 : -1];
typedef char ShopScene_size_must_be_0xB4[(sizeof(ShopScene) == 0xB4) ? 1 : -1];
#endif

/* Draw a window container owned by the menu window subsystem. */
void mnuDrawWindowContainer(s32 x, s32 y, s32 depth, MenuWindowContainer *menu, s32 drawArg);

#endif
