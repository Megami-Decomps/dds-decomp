#ifndef MNU_H
#define MNU_H

#include "common.h"

/* DDS2 scheduler word: zero or the encoded next-handler address. */
extern s32 func_002C4038(s32, s32 *, u64, u64);

static inline s32 menuSetHandler(s32 context, u64 mode, s32 callback) {
    return func_002C4038(context + 8, (s32 *)(context + 0x54), mode, callback);
}
/* DDS1 uses the same scheduler-word contract as the DDS2 dispatcher. */
extern s32 func_00285670(s32, s32 *, u64, u64);

static inline s32 menuRunPanel(s32 context, u64 mode, u64 arg) {
    return func_00285670(context + 8, (s32 *)(context + 0x54), mode, arg);
}

static inline s32 evtMenuSetHandler(s32 context, u64 mode, s32 callback) {
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), mode, callback);
}

static inline void panelSetVec4(u32 *vec, u32 red, u32 green, u32 blue, u32 alpha) {
    vec[0] = red;
    vec[1] = green;
    vec[2] = blue;
    vec[3] = alpha;
}

/* Summary-menu entry count and pass threshold. */
#define MENU_SUM_COUNT 5
#define MENU_SUM_MINIMUM 99

/* Camp task priority for the menu task family. */
#define CAMP_TASK_PRIORITY 0x3EC

/* Serialized map arguments and the two four-word rows used by camp effects. */
typedef struct CampMapArguments {
    u32 values[11];
} CampMapArguments;

typedef struct CampEffectRows {
    u32 values[2][4];
} CampEffectRows;

#ifdef VERSION_DDS2
/* Staff menu task context (DDS2 layout). */
typedef struct MenuStaffWindow MenuStaffWindow;
typedef struct MenuStaffNode MenuStaffNode;
typedef struct MenuStaffList MenuStaffList;

typedef struct MenuStaffContext {
    u8 pad00[0x60];
    s32 group;            /* 0x60 */
    s32 spriteArg0;       /* 0x64 */
    s32 spriteArg1;       /* 0x68 */
    s32 windowResource; /* Source resource for the window's fixed sprite slots. */
    u8 pad70[0x54];
    s32 spriteArg2;       /* 0xC4 */
    u8 padC8[0x2C];
    void *panelLayout;    /* 0xF4: layout used by staff panel construction */
    u8 padF8[0x10];
    MenuStaffList *activeWindow; /* 0x108: window used by staff image states */
    u8 pad10C[0x178];
    u32 windowFlags;      /* 0x284 */
    u8 pad288[0xA68C];
    s32 selection;        /* 0xA914 */
    u8 padA918[0x11C];
    void *panelHandle;    /* 0xAA34 */
    void *spriteHandle;   /* 0xAA38 */
    u8 padAA3C[0xC];
    u8 *menu;             /* 0xAA48 */
    u8 padAA4C[0x6C0];
    u8 tail[4];           /* 0xB10C */
} MenuStaffContext;

/* Each staff list owns a cursor-bearing window at +0x18. */
struct MenuStaffList {
    u8 pad00[0x18];
    MenuStaffWindow *window;
};

struct MenuStaffWindow {
    s32 flags; /* Selection-control bits, including mask 0x8. */
    u8 pad04[0x0C];
    MenuStaffNode *head; /* 0x10 */
    u8 pad14[4];
    s32 *cursor;
    MenuStaffNode *selectedNode; /* 0x1C */
    s32 panelActive; /* 0x20: selects the alternate panel drawing path */
    s32 rowCount; /* 0x24 */
    u8 pad28[4];
    void (*drawEntry)(); /* +0x2C: caller supplies the list and current node. */
    MenuStaffContext *owner; /* +0x30 */
    u8 pad34[8];
    s32 drawAlpha; /* +0x3C: 8.8 fixed-point drawing level. */
};

struct MenuStaffNode {
    u8 pad00[0x48];
    u32 flags; /* 0x48 */
    u8 pad4C[0x0C];
    MenuStaffNode *next; /* 0x58 */
    u8 pad5C[4];
    s32 label;
    s32 entryIndex; /* 0x64 */
    u32 catalogOrdinal; /* +0x68: stable index in the source catalog. */
};

#endif /* VERSION_DDS2 */

#endif /* MNU_H */
