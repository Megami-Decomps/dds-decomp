#ifndef MNU_PARTY_MENU_DATA_H
#define MNU_PARTY_MENU_DATA_H

#include "dat_state.h"
#include "mnu.h"

/* DDS2's party-selection allocation is initialized and read by the party
 * menu owner. Its saved panel records are two complete MenuPageBar views. */
typedef struct PartyMenuData {
    struct SdfMemBlock *allocation;
    u8 pad04[4];
    MenuWindowContainer *primaryWindow; /* 0x08 */
    DatPartyRecord original[5];         /* 0x0C */
    DatPartyRecord current[5];          /* 0x8E0 */
    s32 activeCount;                    /* 0x11B4 */
    DatPartyRecord backup[5];           /* 0x11B8 */
    s32 selection;                      /* 0x1A8C */
    MenuPageBar panelSnapshots[5][2];   /* 0x1A90 */
    s32 fadeA;                          /* 0x1DB0 */
    s32 fadeB;                          /* 0x1DB4 */
    s32 previousSelection;              /* 0x1DB8 */
    s32 profileSaved;                   /* 0x1DBC */
    s32 profilePhase;                   /* 0x1DC0 */
    u32 profileWords[5];                /* 0x1DC4 */
    s32 freezePanel;                    /* 0x1DD8 */
} PartyMenuData;

typedef char PartyMenuData_size_must_be_0x1DDC[
    (sizeof(PartyMenuData) == 0x1DDC) ? 1 : -1];
typedef char PartyMenuData_panelSnapshots_offset_must_be_0x1A90[
    ((u32)&((PartyMenuData *)0)->panelSnapshots == 0x1A90) ? 1 : -1];
typedef char PartyMenuData_freezePanel_offset_must_be_0x1DD8[
    ((u32)&((PartyMenuData *)0)->freezePanel == 0x1DD8) ? 1 : -1];

#endif
