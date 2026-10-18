#ifndef CAMP_MENU_CONTEXT_H
#define CAMP_MENU_CONTEXT_H

#include "common.h"
#include "mnu.h"
#include "mnu_transition.h"
#include "eff.h"

/* The staff and skill menu's context: resource handles and current panel work. */
typedef struct CampMenuContext {
    u8 pad00[8];
    MenuPopupState transitionWork; /* 0x08: saved popup transition state */
    s32 popupState; /* 0x54: popup entry word */
    u8 pad58[8];
    s32 unk60;
    s32 resource;             /* 0x64 */
    s32 unk68;
    s32 displayVariant;       /* 0x6C: passed with display to menu drawing */
    u8 pad70[4];
    s32 option;               /* 0x74 */
    s32 actor;                /* 0x78 */
    EffectSlotSet *staffVariant; /* 0x7C: passed with display to menu drawing */
    s32 staffParam;           /* 0x80 */
    u8 pad84[0x5C];
    s32 variant;              /* 0xE0 */
    s32 unkE4;               /* 0xE4: forwarded to the window resource helper */
    EffectSlotSet *unkE8;     /* 0xE8: grid-slot resource */
    EffectSlotSet *unkEC;     /* 0xEC: window texture variant resource */
    s32 panelResource;        /* 0xF0: grid resource handle */
    u8 padF4[0x24];
    s32 unk118;               /* 0x118: window sprite slot resource */
    const void *partySelectionLayout; /* 0x11C: copied panel layout address */
    s32 unk120;
    s32 panel;                /* 0x124 */
    u8 pad128[4];
    s32 panelList;            /* 0x12C */
    u8 pad130[8];
    s32 display;              /* 0x138 */
    u8 pad13C[0x20];
    MenuPageWindow partyWindow; /* 0x15C..0x7EB: sole primary page owner */
    PartyPanel partyPanel;    /* 0x7EC..0x8F7: occupied-party display owner */
    MenuPanelGroup *sceneGroup; /* 0x8F8 */
    MenuSpriteState *sprite;  /* 0x8FC */
    MenuSimpleSpriteState *effect; /* 0x900 */
    u8 pad904[8];
    void *menu;               /* 0x90C: retained child work; active menu selects its type */
    u8 pad910[0x10];
    MenuProfilePanel *extraResource; /* 0x920 */
} CampMenuContext;

#endif
