#ifndef MNU_SHOP_H
#define MNU_SHOP_H

#include "common.h"

struct MenuList;
struct EffectSlotSet;

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

typedef struct MenuWindowContainer {
    s32 id;                /* 0x00 */
    u32 flags;             /* 0x04 */
    s32 originX;           /* 0x08 */
    s32 originY;           /* 0x0C */
    s32 width;             /* 0x10 */
    s32 height;            /* 0x14 */
    struct MenuList *list; /* 0x18 */
    s32 field1C;           /* 0x1C */
    s32 sprite20;          /* 0x20 */
    s32 param24;           /* 0x24 */
    s32 param28;           /* 0x28 */
    struct {
        u32 sprite;
        u32 parameter;
    } decorations[3];      /* 0x2C: optional window decoration sprites */
    u32 decorationX[3];    /* 0x44 */
    s32 scale50;           /* 0x50 */
    s32 scale54;           /* 0x54 */
    struct MenuIconState panel; /* 0x58: embedded drawable panel layout */
    struct MenuIconSprites *resource; /* 0x90: owned sprite-resource bundle */
    u32 state;             /* 0x94 */
} MenuWindowContainer;

typedef char MenuIconState_size_must_be_0x38[(sizeof(struct MenuIconState) == 0x38) ? 1 : -1];
typedef char MenuWindowContainer_size_must_be_0x98[(sizeof(MenuWindowContainer) == 0x98) ? 1 : -1];
#else
struct MenuWindowSpriteGroup;

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
    s32 x;
    s32 y;
    u32 sprite;
    u32 effect;
    u32 overlaySprite;
    u32 overlayColor;
    u8 pad44[8];
    MenuPanelHandles panel;
    struct MenuWindowSpriteGroup *textures;
    s32 fade;
} MenuWindowContainer;

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
    s32 resourceHandle;
    u8 pad04[4];
    u8 transitionWork[0x4C];
    s32 dispatchState;
    s32 stateTable;
    u8 resourcePair[4];
    s32 pairedHandle;
    u32 spriteResource;
    s32 batchState;
    MenuWindowContainer *sprite;
    MenuWindowContainer *window;
    void *batches[2];
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

typedef char MnuShopListContext_size_must_be_0x10[(sizeof(MnuShopListContext) == 0x10) ? 1 : -1];

typedef char MenuPanelHandles_size_must_be_0x38[(sizeof(MenuPanelHandles) == 0x38) ? 1 : -1];
typedef char MenuWindowContainer_size_must_be_0x8C[(sizeof(MenuWindowContainer) == 0x8C) ? 1 : -1];
typedef char ShopScene_size_must_be_0xB4[(sizeof(ShopScene) == 0xB4) ? 1 : -1];
#endif

#endif
