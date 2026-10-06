#ifndef MNU_SHOP_H
#define MNU_SHOP_H

#include "mnu_list.h"

struct MenuPanelSprite;
struct MenuWindowSpriteGroup;

/* DDS1's generic window panel is embedded at +0x4C and copied as 0x38 bytes. */
typedef struct MenuPanelHandles {
    u32 mode;
    u8 pad04[4];
    s32 count;
    struct MenuPanelSprite *handles[6];
    u32 left;
    u32 top;
    u32 right;
    u32 bottom;
    s32 transition;
} MenuPanelHandles;

/* DDS1 mnuCreateWindowContainer allocates 0x8C bytes. This generic menu owner
 * is kept here while mnu.h is frozen; DDS2 has a different window layout. */
typedef struct MenuWindowContainer {
    s32 id;
    u32 flags;
    s32 width;
    s32 height;
    u8 pad10[4];
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
    u8 pad94[4];
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
