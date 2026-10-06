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

/* The backdrop packet's complete resource payload, also used by shop callbacks. */
typedef struct MapPacket {
    u32 type;
    u32 value;
    u32 sheets[1]; /* The resource destructor iterates this one-sheet bank. */
    u32 items[11];
    s32 count;
} MapPacket;

typedef struct MenuEffectResources {
    MapPacket packet;
    u32 animationHandle;
    CampEffectRows rows;
} MenuEffectResources;

/* DDS2 result/camp backdrop: resources, sixteen sparks and the badge fade. */
typedef struct MenuCampEffect {
    MenuEffectResources resources;
    s32 direction[16];
    s32 velocity[16][2];
    s32 life[16];
    s32 count;
    s32 fade;
} MenuCampEffect;

typedef char MapPacket_size_must_be_0x3C[(sizeof(MapPacket) == 0x3C) ? 1 : -1];
typedef char MenuEffectResources_size_must_be_0x60[(sizeof(MenuEffectResources) == 0x60) ? 1 : -1];
typedef char MenuCampEffect_size_must_be_0x168[(sizeof(MenuCampEffect) == 0x168) ? 1 : -1];

struct EffPayload;

/* DDS1's complete two-layer backdrop asset set. */
typedef struct MenuAssets {
    u32 sprites[5];
    u32 material;
    struct EffPayload *layerA;
    struct EffPayload *layerB;
} MenuAssets;

typedef char MenuAssets_size_must_be_0x20[(sizeof(MenuAssets) == 0x20) ? 1 : -1];

/* Native staff sprite banks; DDS2 retains only two base resources. */
typedef struct StaffSlots {
#ifdef VERSION_DDS2
    u32 baseResources[2];
#else
    u32 baseResources[7];
#endif
    u32 pairResources[2];
    u32 mainResources[16];
    u32 extraResources[5];
} StaffSlots;

#ifdef VERSION_DDS2
typedef char StaffSlots_size_must_be_0x64[(sizeof(StaffSlots) == 0x64) ? 1 : -1];
#else
typedef char StaffSlots_size_must_be_0x78[(sizeof(StaffSlots) == 0x78) ? 1 : -1];
#endif

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

/* Native result-menu row shared by the level and profile progress bars. */
struct DatPartyRecord;

typedef struct BrsProgressRow {
    s32 flags;
    s32 amount;
    struct DatPartyRecord *unit; /* 0x08: profile record for the row. */
    u32 levelProgress[4];
    u32 profileProgress[4];
} BrsProgressRow;

typedef char BrsProgressRow_size_must_be_0x2C[(sizeof(BrsProgressRow) == 0x2C) ? 1 : -1];

/* Page sprites and their fade/slide state; DDS2 expanded the sprite banks. */
typedef struct MenuSprites {
    u8 pad00[0xC];
    s32 unkC;
#ifdef VERSION_DDS2
    void *icon[5];
    void *item[11];
    void *cursor[4];
    s32 fade;
#else
    s32 firstSprite;
    s32 primarySprite;
    s32 sprites[7];
    s32 overlaySprites[2];
    u8 pad3C[4];
#endif
    s32 drawAlpha;
    s32 fadeOut;
    s32 slideOffset;
    s32 slideSpeed;
#ifdef VERSION_DDS2
    u8 unk74;
    u8 unk75;
#endif
} MenuSprites;

/* The allocated icon bundle owns its draw fade and fade direction. */
typedef struct MenuIconBundle {
#ifdef VERSION_DDS2
    u32 unk0[3];
    void *sprite[3];
#else
    u8 pad0[0xC];
    s32 sprite[4];
#endif
    s32 fade;
    s32 fadeOut;
} MenuIconBundle;

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

typedef struct MenuPoint {
    s32 x;
    s32 y;
} MenuPoint;

/* Profile helpers expose the game's native panel allocation as a word buffer. */
typedef struct MenuProfilePanel {
    u8 pad00[0x10];
#ifdef VERSION_DDS2
    u32 unk10;
    s32 unk14;
    u32 resourceHandle; /* 0x18: set from the owning progress host */
    u32 unk1C;
    u32 unk20;
    u32 unk24;
    u32 unk28;
    u32 unk2C[5];
    s32 phase;
    u32 unk44;
#else
    s32 capValue;
    s32 option;
    MenuPoint gridOrigin;
    u8 pad20[0x1C];
#endif
} MenuProfilePanel;

#ifdef VERSION_DDS2
typedef struct MenuPageBar {
    u8 pad00[0x10];
    s32 percentage;
    u8 pad14[0x2C];
    s32 unk40;
    u32 unk44;
    u32 unk48;
    u32 unk4C;
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

typedef struct MenuGradientFade {
    s32 active;
    s32 color;
    s32 blend;
} MenuGradientFade;

typedef char MenuGradientFade_size_must_be_0x0C[(sizeof(MenuGradientFade) == 0x0C) ? 1 : -1];

struct EffectList;

/* The allocated progress display owns its request list and staff sprite banks. */
typedef struct MenuProgressHost {
    s32 heapHandle;
    struct EffectList *titleEffectHandle;
    StaffSlots staffSlots;
    s32 loadState;
    PartyPanel partyPanel;
    MenuPageWindow partyWindow;
    u32 panelGroup;
    s32 effectResource;
    s32 currentEffect;
} MenuProgressHost;

#ifdef VERSION_DDS2
typedef char MenuProgressHost_size_must_be_0xA82C[(sizeof(MenuProgressHost) == 0xA82C) ? 1 : -1];
#else
typedef char MenuProgressHost_size_must_be_0x82C[(sizeof(MenuProgressHost) == 0x82C) ? 1 : -1];
#endif

#ifndef VERSION_DDS2
/* DDS1 staff movie task work allocated by mnuMovieCreateTask (0x20 bytes). */
typedef struct MnuStaffMovieWork {
    u32 allocation;      /* 0x00 */
    u32 spriteSet;       /* 0x04: resource passed to mnuDrawIconAlphaSprite */
    s32 phase;           /* 0x08 */
    s32 scrollTicks;     /* 0x0C */
    u32 unk10;           /* 0x10 */
    u32 imageIndex;      /* 0x14 */
    u8 pad18[8];
} MnuStaffMovieWork;

typedef char MnuStaffMovieWork_size_must_be_0x20[(sizeof(MnuStaffMovieWork) == 0x20) ? 1 : -1];

/* DDS1 title-movie menu allocation (0x40 bytes). */
typedef struct MovieMenuState {
    s32 allocation;      /* 0x00 */
    u8 pad04[0x0C];
    s32 state;           /* 0x10 */
    s32 cursor;          /* 0x14 */
    s32 mode;            /* 0x18 */
    s32 unk1C;           /* 0x1C */
    u8 pad20[0x10];
    void *resources;     /* 0x30 */
    s32 unk34;           /* 0x34 */
    u8 pad38[8];
} MovieMenuState;

typedef char MovieMenuState_size_must_be_0x40[(sizeof(MovieMenuState) == 0x40) ? 1 : -1];

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
struct MenuIconState;


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
    struct MenuIconState *panelLayout; /* 0xF4: layout used by staff panel construction */
    struct MenuIconState *unkF8; /* 0xF8: second panel layout */
    struct MenuIconState *unkFC; /* 0xFC: third panel layout */
    u8 pad100[4];
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
    u32 flags; /* Selection-control bits, including mask 0x8. */
    u8 pad04[0x0C];
    MenuStaffNode *head; /* 0x10 */
    u8 pad14[4];
    MenuStaffNode *cursor; /* 0x18 */
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
    s32 index; /* List position, saved when switching staff pages. */
    s32 value; /* Entry payload supplied to mnuAppendWindowListNode. */
    u8 pad08[0x40];
    u32 flags; /* 0x48 */
    u8 pad4C[0x0C];
    MenuStaffNode *next; /* 0x58 */
    u8 pad5C[4];
    s32 label;
    s32 entryIndex; /* 0x64 */
    u32 catalogOrdinal; /* +0x68: stable index in the source catalog. */
};

/* Title movie menu bars (producer: code_002A3AE8). Sliding bar: direction flag and 0..max position. */
typedef struct SlideBar {
    s32 active;
    s32 pos;
} SlideBar;

/* Sliding bar that switches to a queued mode after a countdown. */
typedef struct SlideBarTimed {
    s32 active;
    s32 pos;
    s32 id;
    s32 timer;
} SlideBarTimed;

typedef struct MovieMenuEffectSlot {
    s32 active;
    s32 counter;
} MovieMenuEffectSlot;

typedef struct PickEntry {
    s8 id;
    u8 unk1;
    u8 unk2;
} PickEntry;

/* Paired bar with the randomly picked title-movie slots (0x94 bytes). */
typedef struct PickList {
    SlideBar control;
    u8 count;
    PickEntry entry[32];
    u8 pad69[3];
    s32 pathProgress; /* 0x6C: normalized 0..4096 path position */
    s32 unk70;
    MovieMenuEffectSlot effectSlots[4]; /* 0x74 */
} PickList;

#endif /* VERSION_DDS2 */

#endif /* MNU_H */
