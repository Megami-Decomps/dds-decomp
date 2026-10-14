#ifndef MNU_PANEL_POSITION_RECORD_H
#define MNU_PANEL_POSITION_RECORD_H

#include "common.h"

/* Script-addressed panel state shared by mantra drawing and menu updates. */
typedef struct MenuPanelPositionRecord {
    s16 id;
    union {
        u16 stateFlags;
        struct {
            u8 state;
            s8 flags;
        };
    };
} MenuPanelPositionRecord;

typedef char MenuPanelPositionRecord_size_must_be_0x04[
    (sizeof(MenuPanelPositionRecord) == 0x04) ? 1 : -1
];
typedef char MenuPanelPositionRecord_id_at_0x00[
    ((u32)&((MenuPanelPositionRecord *)0)->id == 0x00) ? 1 : -1
];
typedef char MenuPanelPositionRecord_stateFlags_at_0x02[
    ((u32)&((MenuPanelPositionRecord *)0)->stateFlags == 0x02) ? 1 : -1
];
typedef char MenuPanelPositionRecord_state_at_0x02[
    ((u32)&((MenuPanelPositionRecord *)0)->state == 0x02) ? 1 : -1
];
typedef char MenuPanelPositionRecord_flags_at_0x03[
    ((u32)&((MenuPanelPositionRecord *)0)->flags == 0x03) ? 1 : -1
];

MenuPanelPositionRecord *mnuFindPanelPositionRecordByScriptId(s32 selector, u16 id);

#endif /* MNU_PANEL_POSITION_RECORD_H */
