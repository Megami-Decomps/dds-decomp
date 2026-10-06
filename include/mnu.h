#ifndef MNU_H
#define MNU_H

#include "common.h"
#ifdef VERSION_DDS2
#include "mnu_shop.h"
#endif

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
    PartyPanel *records;
#ifdef VERSION_DDS2
    struct EffectSlotSet *resources; /* 0x0C: resolved base-resource handle */
    s32 slot;
    u32 secondaryResource;
    s32 secondarySlot;
    u32 alternateResource;
    s32 alternateSlot;
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
typedef char MenuPageWindow_size_must_be_0xA6A4[
    sizeof(MenuPageWindow) == 0xA6A4 ? 1 : -1];
typedef char MenuPageWindow_resources_offset_check[
    ((u32)&((MenuPageWindow *)0)->resources == 0x0C) ? 1 : -1];
typedef char MenuPageWindow_slot_offset_check[
    ((u32)&((MenuPageWindow *)0)->slot == 0x10) ? 1 : -1];
typedef char MenuPageWindow_secondaryResource_offset_check[
    ((u32)&((MenuPageWindow *)0)->secondaryResource == 0x14) ? 1 : -1];
typedef char MenuPageWindow_secondarySlot_offset_check[
    ((u32)&((MenuPageWindow *)0)->secondarySlot == 0x18) ? 1 : -1];
typedef char MenuPageWindow_alternateResource_offset_check[
    ((u32)&((MenuPageWindow *)0)->alternateResource == 0x1C) ? 1 : -1];
typedef char MenuPageWindow_alternateSlot_offset_check[
    ((u32)&((MenuPageWindow *)0)->alternateSlot == 0x20) ? 1 : -1];

void func_002BCD90(MenuPageWindow *, PartyPanel *, struct EffectSlotSet *,
                   s32, u32, s32, u32, s32);
void mnuInitializeCampPanelResources(MenuPageWindow *, StaffSlots *, u32, PartyPanel *);
#endif

typedef struct MenuGradientFade {
    s32 active;
    s32 color;
    s32 blend;
} MenuGradientFade;

typedef char MenuGradientFade_size_must_be_0x0C[(sizeof(MenuGradientFade) == 0x0C) ? 1 : -1];

#ifdef VERSION_DDS2
/* The terminal's eight progress thresholds and their selectable rewards. */
typedef struct MnuProgressReward {
    u8 kind;
    s32 value; /* Item ID for kind zero, currency amount otherwise. */
} MnuProgressReward;

typedef struct MnuProgressEntry {
    u32 threshold;
    s32 flag;
    MnuProgressReward rewards[8];
} MnuProgressEntry;

typedef char MnuProgressReward_size_must_be_0x8[(sizeof(MnuProgressReward) == 0x8) ? 1 : -1];
typedef char MnuProgressEntry_size_must_be_0x48[(sizeof(MnuProgressEntry) == 0x48) ? 1 : -1];

extern MnuProgressEntry D_003CE1A8[8];

struct EffectSlotSet;
struct EffMappedResource;

/* Per-list storage allocated and cleared as 0x14 bytes by the terminal window builder. */
typedef struct MenuTerminalWindowState {
    u8 pad00[0xC];
    u16 unk0C;
    u16 unk0E;
    u16 unk10;
    u16 selectedSlot;
} MenuTerminalWindowState;

typedef char MenuTerminalWindowState_size_must_be_0x14[(sizeof(MenuTerminalWindowState) == 0x14) ? 1 : -1];

/* Terminal/shop modes share this complete 0x38C-byte scene allocation. */
typedef struct MenuTerminalContext {
    s32 resourceHandle;
    u8 pad04[4];
    s32 type;
    u8 transitionWork[0x4C];
    s32 popupState;
    u8 pad5C[4];
    s32 messageResources[2];
    struct EffectSlotSet *effectSlots[4];
    s32 state;
    MenuWindowContainer *ownedWindows[1];
    MenuWindowContainer *window;
    struct EffMappedResource *objects[2];
    s32 shopRow;
    s32 multiplier;
    u8 pad94[8];
    s32 availableCount;
    u16 unkA0;
    u16 unkA2;
    u16 unkA4;
    u16 selectedSlot;
    u32 rewardCursor;
    u32 options;
    s32 previousValue;
    s32 elapsedFrames;
    s32 retryFrames;
    u8 padBC[4];
    u32 phase;
    u8 padC4[2];
    s8 pulseFrame;
    s8 unkC7;
    u8 padC8[5];
    s8 unkCD;
    u8 padCE[0x12];
    s32 prepared;
    s32 delayFrames;
    u8 panelWork[2][0x94];
    MenuEffectResources effectResources;
    u8 pad270[0x108];
    u32 windowResource;
    MenuGradientFade gradientFade;
    u8 rewardGranted;
    u8 pad389[3];
} MenuTerminalContext;

typedef char MenuTerminalContext_size_must_be_0x38C[(sizeof(MenuTerminalContext) == 0x38C) ? 1 : -1];

extern MenuTerminalContext *D_00438FC8;


typedef struct MenuIconSprites {
    u32 handle;
    u32 value;
    u32 unk8;
    struct EffectSlotSet *sprite[3];
} MenuIconSprites;

typedef struct MenuFadeFields {
    MenuWindowContainer previousWindow;
    s32 previousVisibleCount;
    MenuIconSprites savedResource;
    u32 hasResourceCopy;
    s32 previousProgress;
    MenuWindowContainer *currentWindow;
    s32 currentProgress;
} MenuFadeFields;

typedef char MenuIconSprites_size_must_be_0x18[(sizeof(MenuIconSprites) == 0x18) ? 1 : -1];
typedef char MenuFadeFields_size_must_be_0xC4[(sizeof(MenuFadeFields) == 0xC4) ? 1 : -1];
#endif

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
    MenuWindowContainer *activeWindow; /* 0x108 */
    u8 pad10C[0xC];
    s32 unk118;
    u8 pad11C[0x168];
    u32 windowFlags;      /* 0x284 */
    u8 pad288[0xA68C];
    struct MenuList *selection; /* 0xA914: retained party-selection list */
    u8 padA918[0x11C];
    void *panelHandle;    /* 0xAA34 */
    void *spriteHandle;   /* 0xAA38 */
    u8 padAA3C[0xC];
    void *menu;           /* 0xAA48: menu-mode-specific child allocation */
    u8 padAA4C[0x3D4];
    u16 catalogOrdinals[0x100]; /* 0xAE20: item-ID-indexed list sorting keys */
    u8 padB020[0xEC];
    u8 tail[4];           /* 0xB10C */
} MenuStaffContext;

/* Five owned windows and their selection/transition state share one 0x54 allocation. */
typedef struct MenuStaffChoices {
    s32 allocation;
    u8 pad04[4];
    MenuWindowContainer *windows[5];
    s32 currentSelection;
    s32 thirdListEnabled;
    s32 previous;
    s32 requested;
    s32 alternatePrevious;
    s32 alternateRequested;
    s32 firstListState;
    s32 secondListState;
    s32 secondListReset;
    s32 thirdListState;
    s32 thirdListIndex;
    s32 thirdListValue;
    s32 thirdListReset;
    u8 pad50[4];
} MenuStaffChoices;

typedef char MenuStaffChoices_size_must_be_0x54[(sizeof(MenuStaffChoices) == 0x54) ? 1 : -1];

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
