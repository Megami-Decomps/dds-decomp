#ifndef MNU_SCROLL_PANEL_H
#define MNU_SCROLL_PANEL_H

#include "mnu.h"

struct EffMappedResource;

/* The retained scroll-panel allocation has title-specific position storage. */
typedef struct MenuScrollPanel {
    u8 pad00[4];
#ifdef VERSION_DDS2
    u32 color; /* 0x04 */
    u32 firstSprite; /* 0x08 */
    u32 secondSprite; /* 0x0C */
    s32 animationFrame; /* 0x10: signed opening/closing progress, clamped to 0..181. */
    MenuGridSlot positions[3]; /* 0x14 */
    MenuGridSlot active; /* 0x2C */
    MenuGridSlot pending; /* 0x34 */
    struct EffMappedResource *handles[3]; /* 0x3C */
} MenuScrollPanel;

typedef char MenuScrollPanel_dds2_size_check[
    (sizeof(MenuScrollPanel) == 0x48) ? 1 : -1];
typedef char MenuScrollPanel_dds2_positions_check[
    ((u32)&((MenuScrollPanel *)0)->positions == 0x14) ? 1 : -1];
typedef char MenuScrollPanel_dds2_handles_check[
    ((u32)&((MenuScrollPanel *)0)->handles == 0x3C) ? 1 : -1];

MenuScrollPanel *mnuCreateScrollPanel(struct EffectSlotSet *owner);
void mnuDestroyScrollPanel(MenuScrollPanel *menu);
void mnuReleaseScrollPanelAnimations(MenuScrollPanel *menu);
void mnuConfigurePanelResource(MenuScrollPanel *menu,
                               struct EffectSlotSet *model, u32 value,
                               u32 color);
#else
    u32 selection; /* 0x04 */
    u32 firstSprite; /* 0x08 */
    u32 secondSprite; /* 0x0C */
    MenuGridSlot positions[2]; /* 0x10 */
    MenuGridSlot active[2]; /* 0x20 */
    MenuGridSlot pending[2]; /* 0x30 */
    struct EffMappedResource *handles[3]; /* 0x40 */
} MenuScrollPanel;

typedef char MenuScrollPanel_dds1_size_check[
    (sizeof(MenuScrollPanel) == 0x4C) ? 1 : -1];
typedef char MenuScrollPanel_dds1_positions_check[
    ((u32)&((MenuScrollPanel *)0)->positions == 0x10) ? 1 : -1];
typedef char MenuScrollPanel_dds1_handles_check[
    ((u32)&((MenuScrollPanel *)0)->handles == 0x40) ? 1 : -1];

MenuScrollPanel *mnuCreateScrollPanel(struct EffectSlotSet *firstResource,
                                      s32 firstIndex,
                                      struct EffectSlotSet *secondResource,
                                      s32 secondIndex);
void mnuDestroyScrollPanel(MenuScrollPanel *panel);
void mnuStoreScrollPanelSelectionAndGridPosition(MenuScrollPanel *panel,
                                                  struct EffectSlotSet *resource,
                                                  s32 resourceIndex, u32 selection);
void mnuActivatePanelAndConfigureGridResources(MenuScrollPanel *menu,
                                               struct EffectSlotSet *sprite,
                                               s32 y, s32 color);
#endif

#endif
