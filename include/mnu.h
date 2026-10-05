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

/* Native 0x10-byte list-node payload. Its first word is an entry index or
 * displayed value according to the list; price is the preserved base price. */
typedef struct CampWindowParams {
    s32 value;
    s32 id;
    s32 price;
    s32 mode;
} CampWindowParams;

typedef char CampWindowParams_size_must_be_0x10[(sizeof(CampWindowParams) == 0x10) ? 1 : -1];

/* Serialized map arguments and the two four-word rows used by camp effects. */
typedef struct CampMapArguments {
    u32 values[11];
} CampMapArguments;

typedef struct CampEffectRows {
    u32 values[2][4];
} CampEffectRows;

/* The occupied-party display and its five native 0x34-byte entries. */
typedef struct PartyPanelEntry {
    s32 unk0;
    s32 index;
    s32 unk8;
    s32 pad[10];
} PartyPanelEntry;

typedef struct PartyPanel {
    s32 unk0;
    s32 unk4;
    PartyPanelEntry slots[5];
} PartyPanel;

typedef struct MenuPageGauge {
    s32 resourceIndex;
    u8 pad4[4];
    s32 hp;
    s32 mp;
    s32 maxHp;
    s32 maxMp;
    u8 pad18[0x18];
} MenuPageGauge;

typedef struct MenuPageEntry {
    s32 partyIndex;
    MenuPageGauge gauge;
} MenuPageEntry;

typedef struct MenuPageRecord {
    s32 visibleCount;
    s32 additionalCount;
    u32 unk8;
    MenuPageEntry entries[5];
} MenuPageRecord;

#ifdef VERSION_DDS2
typedef struct MenuPageBar {
    u8 pad00[0x10];
    s32 percentage;
    u8 pad14[0x30];
    u32 unk44;
    u32 unk48;
    u8 pad4C[4];
} MenuPageBar;

typedef struct MenuQueuedCommand {
    u32 unk0;
    s32 kind;
    s32 option;
    s32 unkC;
    s32 unk10;
    s32 initialValue;
    s32 argument;
} MenuQueuedCommand;

/* A page owns two content banks; sprite/stat fields belong to each bank. */
typedef struct MenuPageSlotContent {
    u8 pad00[4];
    u32 icon[3];
    MenuPageBar hp;
    MenuPageBar mp;
    u32 frame[8];
    struct MenuSprites *windowSprites;
    u32 iconBundle;
    u8 padD8[0xC];
    MenuQueuedCommand command;
    s32 unk100;
    u8 pad104[0xF20];
} MenuPageSlotContent;

typedef struct MenuPageSlot {
    s32 kind;
    u32 flags;
    u8 pad08[4];
    MenuPageSlotContent contents[2];
    u8 pad2054[0xE4];
} MenuPageSlot;
#else
typedef struct MenuPageSlot {
    s32 kind;
    u32 flags;
    u8 pad08[8];
    s32 icon[3];
    u8 pad1C[0x4C];
    s32 scaleA;
    s32 offsetA;
    u8 pad70[0x4C];
    s32 scaleB;
    s32 offsetB;
    s32 frame[6];
    struct MenuPageResources *resources;
    struct MenuSprites *windowSprites;
    u32 iconBundle;
    u8 padE8[0x4C];
} MenuPageSlot;
#endif

typedef struct MenuPageWindow {
    u32 flags;
    s32 transitionValue;
    MenuPageRecord *records;
#ifdef VERSION_DDS2
    u8 pad0C[0x18];
#else
    s32 source;
    s32 slot;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
#endif
    s32 handlesA[8];
    s32 handlesB[8];
    s32 handlesC[5];
    MenuPageSlot slots[5];
    struct MenuList *lists[2];
    s32 selected;
    s32 scrollOffset;
    s32 fade;
} MenuPageWindow;

#ifdef VERSION_DDS2
/* The large progress display is distinct from the 0x3F8 terminal scene. */
typedef struct MenuProgressHost {
    s32 heapHandle;
    s32 titleEffectHandle;
    s32 resourceHandle;
    u8 pad0C[8];
    s32 unk14;
    u8 pad18[0x54];
    s32 loadState;
    PartyPanel partyPanel;
    MenuPageWindow partyWindow;
    u8 padA820[4];
    s32 effectResource;
    s32 currentEffect;
} MenuProgressHost;
#endif

#ifndef VERSION_DDS2
/* Mantra-scene state shared by its controller and animated currency display. */
typedef struct MenuSceneMetadata {
    u8 pad00[0x0C];
    s32 messageWindowResource;
    u8 pad10[8];
    s32 state;
    u8 pad1C[4];
    s32 messageShadeFrames;
    s32 attachedEffect;
    u32 attachedEffectControl;
    u16 entryId;
    u8 pad02E[0x20E];
    s32 displayedCurrency;
    s32 currencyFrame;
    u16 pendingProfileId;
    s8 stageFinished;
    s8 stageStarted;
} MenuSceneMetadata;
#endif

#ifdef VERSION_DDS2
/* Staff menu task context (DDS2 layout). */
typedef struct MenuStaffWindow MenuStaffWindow;
typedef struct MenuStaffNode MenuStaffNode;
typedef struct MenuStaffList MenuStaffList;

typedef struct MenuStaffContext {
    u8 pad00[0x54];
    s32 popupState;       /* 0x54 */
    u8 pad58[8];
    s32 group;            /* 0x60 */
    s32 spriteArg0;       /* 0x64 */
    s32 spriteArg1;       /* 0x68 */
    s32 windowResource; /* Source resource for the window's fixed sprite slots. */
    u8 pad70[0x54];
    s32 spriteArg2;       /* 0xC4 */
    u8 padC8[0x2C];
    void *panelLayout;    /* 0xF4: layout used by staff panel construction */
    u8 padF8[0xC];
    s32 unk104;
    MenuStaffList *activeWindow; /* 0x108: window used by staff image states */
    u8 pad10C[0xC];
    s32 unk118;
    u8 pad11C[0x168];
    u32 windowFlags;      /* 0x284 */
    u8 pad288[0xA68C];
    s32 selection;        /* 0xA914 */
    u8 padA918[0x11C];
    void *panelHandle;    /* 0xAA34 */
    void *spriteHandle;   /* 0xAA38 */
    u8 padAA3C[0xC];
    u8 *menu;             /* 0xAA48 */
    u8 padAA4C[0x3D4];
    u16 catalogOrdinals[0x100]; /* 0xAE20: item-ID-indexed list sorting keys */
    u8 padB020[0xEC];
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
