#ifndef MNU_H
#define MNU_H

#include "common.h"
#include "dat_stat.h"
#include "mnu_transition.h"
#ifdef VERSION_DDS2
#include "mnu_shop.h"
#endif

/* DDS2 scheduler word: zero or the encoded next-handler address. */
extern s32 func_002C4038(void *work, s32 *entrySlot, s32 mode, void *callback);
#ifdef VERSION_DDS2
void mnuSetPopupEntry(s32 *entrySlot, void *entry);
void mnuSetPopupEntryFlagged(s32 *entrySlot, void *entry);
#endif


static inline s32 menuSetHandler(void *context, s32 mode, void *callback) {
    return func_002C4038((u8 *)context + 8, (s32 *)((u8 *)context + 0x54), mode, callback);
}
/* DDS1 uses the same scheduler-word contract as the DDS2 dispatcher. */
extern s32 func_00285670(void *work, s32 *entrySlot, s32 mode, void *callback);

static inline s32 menuRunPanel(void *context, s32 mode, void *callback) {
    return func_00285670((u8 *)context + 8, (s32 *)((u8 *)context + 0x54), mode, callback);
}

static inline s32 evtMenuSetHandler(void *context, s32 mode, void *callback) {
    return func_002C4038((u8 *)context + 0xC, (s32 *)((u8 *)context + 0x58), mode, callback);
}

static inline void panelSetVec4(u32 *vec, u32 red, u32 green, u32 blue, u32 alpha) {
    vec[0] = red;
    vec[1] = green;
    vec[2] = blue;
    vec[3] = alpha;
}

/* Summary-menu entry count and pass threshold. */
#define MENU_SUM_COUNT DAT_BASE_STAT_COUNT
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

struct EffectSlotSet;

/* Native staff sprite banks; DDS2 retains only two base resources. */
typedef struct StaffSlots {
#ifdef VERSION_DDS2
    u32 baseResources[2];
#else
    u32 baseResources[7];
#endif
    struct EffectSlotSet *pairResources[2];
    struct EffectSlotSet *mainResources[16];
    struct EffectSlotSet *extraResources[5];
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
    s32 level;
    s32 hp;
    s32 mp;
    s32 maxHp;
    s32 maxMp;
    s32 stats[DAT_BASE_STAT_COUNT];
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

/* DDS1 allocates 0x50 bytes; DDS2's expanded sprite banks and byte flags use 0x78. */
typedef struct MenuSprites {
    u32 flags;
    u8 pad04[8];
    s32 unkC;
#ifdef VERSION_DDS2
    void *icon[5];
    void *item[11];
    void *cursor[4];
    s32 fade;
#else
    struct EffectSlotSet *firstSprite;
    struct EffectSlotSet *primarySprite;
    struct EffectSlotSet *sprites[7];
    struct EffectSlotSet *overlaySprites[2];
    s32 profileFade;
#endif
    s32 drawAlpha;
    s32 fadeOut;
    s32 slideOffset;
    s32 slideSpeed;
#ifdef VERSION_DDS2
    s8 unk74;
    s8 unk75;
#endif
} MenuSprites;

#ifndef VERSION_DDS2
typedef char MenuSprites_dds1_size_check[
    sizeof(MenuSprites) == 0x50 ? 1 : -1];
#endif

/* The allocated icon bundle owns its draw fade and fade direction. */
typedef struct MenuIconBundle {
#ifdef VERSION_DDS2
    u32 unk0[3];
    void *sprite[3];
#else
    u8 pad0[0xC];
    struct EffectSlotSet *sprite[4];
#endif
    s32 fade;
    s32 fadeOut;
} MenuIconBundle;

#ifdef VERSION_DDS2
typedef char MenuIconBundle_size_must_be_0x20[
    sizeof(MenuIconBundle) == 0x20 ? 1 : -1];
#else
typedef char MenuIconBundle_size_must_be_0x24[
    sizeof(MenuIconBundle) == 0x24 ? 1 : -1];
typedef char MenuIconBundle_dds1_sprites_offset_check[
    ((u32)&((MenuIconBundle *)0)->sprite == 0xC) ? 1 : -1];
#endif


typedef struct MenuPoint {
    s32 x;
    s32 y;
} MenuPoint;

/* Native sprite-state allocation: three resource-set owners and a state word. */
typedef struct MenuSpriteState {
    u8 pad00[0x10];
    struct EffectSlotSet *resourceSets[3];
    s32 initialValue; /* 0x1C: initialized to 0x100 */
} MenuSpriteState;

typedef char MenuSpriteState_size_must_be_0x20[(sizeof(MenuSpriteState) == 0x20) ? 1 : -1];
typedef char MenuSpriteState_resourceSets_offset_check[
    ((u32)&((MenuSpriteState *)0)->resourceSets == 0x10) ? 1 : -1];
typedef char MenuSpriteState_initialValue_offset_check[
    ((u32)&((MenuSpriteState *)0)->initialValue == 0x1C) ? 1 : -1];

MenuSpriteState *mnuCreateSpriteState(struct EffectSlotSet *, struct EffectSlotSet *, struct EffectSlotSet *);
void mnuFreeSpriteStateWork(MenuSpriteState *);

#ifndef VERSION_DDS2
void mnuDrawPartyInfoSprites(s32, s32, s32, void *, MenuSpriteState *, s32);
#endif

#ifdef VERSION_DDS2
MenuSpriteState *mnuAllocateSimpleSprite(struct EffectSlotSet *, struct EffectSlotSet *,
                                       struct EffectSlotSet *);
void mnuFreeSimpleSpriteWork(MenuSpriteState *);
#endif

#ifndef VERSION_DDS2
/* DDS1's simple sprite owns five effect resource sets and a blend factor. */
typedef struct MenuSimpleSpriteState {
    u8 pad00[0x10];
    struct EffectSlotSet *resourceSets[5];
    u32 blendFactor; /* 0x24: initial color-blend weight */
} MenuSimpleSpriteState;

typedef char MenuSimpleSpriteState_size_must_be_0x28[
    (sizeof(MenuSimpleSpriteState) == 0x28) ? 1 : -1];
typedef char MenuSimpleSpriteState_resourceSets_offset_check[
    ((u32)&((MenuSimpleSpriteState *)0)->resourceSets == 0x10) ? 1 : -1];
typedef char MenuSimpleSpriteState_blendFactor_offset_check[
    ((u32)&((MenuSimpleSpriteState *)0)->blendFactor == 0x24) ? 1 : -1];

MenuSimpleSpriteState *mnuAllocateSimpleSprite(
    struct EffectSlotSet *, struct EffectSlotSet *, struct EffectSlotSet *,
    struct EffectSlotSet *, struct EffectSlotSet *);
void mnuFreeSimpleSpriteWork(MenuSimpleSpriteState *);
#endif

struct MenuPanelItem;

/* Native group allocations own five panel items and retain their selection. */
typedef struct MenuPanelGroup {
    u8 pad00[0x0C];
#ifdef VERSION_DDS2
    s32 texture; /* 0x0C */
    struct MenuPanelItem *entries[5]; /* 0x10 */
    u32 selection; /* 0x24 */
    s32 initialValue; /* 0x28 */
#else
    struct MenuPanelItem *children[5]; /* 0x0C */
    u32 selection; /* 0x20 */
    s32 initialValue; /* 0x24 */
#endif
} MenuPanelGroup;

#ifdef VERSION_DDS2
typedef char MenuPanelGroup_size_must_be_0x2C[(sizeof(MenuPanelGroup) == 0x2C) ? 1 : -1];
typedef char MenuPanelGroup_texture_offset[((u32)&((MenuPanelGroup *)0)->texture == 0x0C) ? 1 : -1];
typedef char MenuPanelGroup_entries_offset[((u32)&((MenuPanelGroup *)0)->entries == 0x10) ? 1 : -1];
typedef char MenuPanelGroup_selection_offset[((u32)&((MenuPanelGroup *)0)->selection == 0x24) ? 1 : -1];
typedef char MenuPanelGroup_initialValue_offset[((u32)&((MenuPanelGroup *)0)->initialValue == 0x28) ? 1 : -1];
#else
typedef char MenuPanelGroup_size_must_be_0x28[(sizeof(MenuPanelGroup) == 0x28) ? 1 : -1];
typedef char MenuPanelGroup_children_offset[((u32)&((MenuPanelGroup *)0)->children == 0x0C) ? 1 : -1];
typedef char MenuPanelGroup_selection_offset[((u32)&((MenuPanelGroup *)0)->selection == 0x20) ? 1 : -1];
typedef char MenuPanelGroup_initialValue_offset[((u32)&((MenuPanelGroup *)0)->initialValue == 0x24) ? 1 : -1];
#endif

#ifdef VERSION_DDS2
extern MenuPanelGroup *mnuCreatePanelGroup(s32 owner, s32 texture, s32 mode);
#else
extern MenuPanelGroup *mnuCreatePanelGroup(s32 parent);
#endif
extern void mnuDestroyPanelGroup(MenuPanelGroup *group);
extern void mnuUpdateFiveListEntries(MenuPanelGroup *group, s32 gridObject);
extern void mnuSetPanelGroupSelection(MenuPanelGroup *group, u32 selection);
extern void mnuClearPanelGroupSelection(MenuPanelGroup *group);
extern u32 mnuGetPanelGroupSelection(MenuPanelGroup *group);
extern void mnuSetGroupSelection(MenuPanelGroup *group, s32 index, s32 selection, u32 option);
#ifdef VERSION_DDS2
extern void mnuApplyPackedGroupValues(MenuPanelGroup *group, s32 itemId);
#else
extern void mnuDrawAndAdvancePanelGroup(s32, s32, s32, void *, MenuPanelGroup *, s32);
#endif

/* An indexed render slot owned by an effect resource set. */
typedef struct MenuGridSlot {
    struct EffectSlotSet *set;
    s32 index;
} MenuGridSlot;

typedef char MenuGridSlot_size_must_be_8[(sizeof(MenuGridSlot) == 8) ? 1 : -1];

/* Native profile-progress panel, including its cached sprite-slot pairs. */
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
    MenuGridSlot fill;
    MenuGridSlot background;
    MenuGridSlot completed;
    s32 unk30;
    s32 phase;
    u32 opacity;
#endif
} MenuProfilePanel;

#ifndef VERSION_DDS2
typedef char MenuProfilePanel_size_must_be_0x3C[(sizeof(MenuProfilePanel) == 0x3C) ? 1 : -1];
#endif

typedef struct MenuEffectPosition {
    u8 pad00[0x20];
    s32 *coordinates;
} MenuEffectPosition;

typedef struct MenuEffectNode {
    u8 pad00[8];
    MenuEffectPosition *position;
} MenuEffectNode;

/* DDS1 paired numeric-bar effects; callers also set the draw opacity. */
typedef struct MenuEffectPair {
    u8 pad00[0x14];
    s32 *settings;
    u8 settingIndex;
    s8 positionY;
    u8 pad1A[0x1E];
    s32 configurationHandle;
    u8 pad3C[4];
    MenuEffectNode *first;
    MenuEffectNode *second;
    u8 pad48[4];
    u32 opacity;
} MenuEffectPair;

typedef char MenuEffectPair_opacity_offset_check[
    ((u32)&((MenuEffectPair *)0)->opacity == 0x4C) ? 1 : -1];

#ifdef VERSION_DDS2
/* Complete 0x50-byte texture/effect owner embedded in each page bank. */
typedef struct MenuPageBar {
    s32 variant;
    u8 pad04[0x0C];
    s32 quantizedSpan; /* 0x10 */
    s32 *settings; /* 0x14: four selectable effect settings */
    u8 settingIndex;
    s8 positionY;
    u8 pad1A[2];
    s32 textures[7]; /* 0x1C */
    struct MenuEffectNode *effects[2]; /* 0x38 */
    s32 activeEffect;
    s32 fade;
    s32 fadeOut;
    s32 holdEffectUpdate;
} MenuPageBar;

/* One command owns both complete 128-entry record arrays. The second
 * initializer explicitly writes all 128 entries, independent of table spacing. */
typedef struct MenuCommandPosition {
    s32 x;
    s32 y;
    s32 targetX;
    s32 targetY;
} MenuCommandPosition;

typedef struct MenuCommandMotion {
    s32 value; /* Phase step for kind 0, opacity factor for kind 1. */
    s32 delay;
    s32 phase;
    s32 scale;
} MenuCommandMotion;

typedef struct MenuQueuedCommand {
    u32 unk0;
    s32 kind;
    s32 option;
    s32 unkC;
    s32 unk10;
    s32 initialValue;
    s32 argument;
    struct EffectSlotSet *resources; /* 0x1C: the real draw resource instance. */
    s32 particleCount;
    MenuCommandPosition positions[128]; /* 0x24 */
    MenuCommandMotion motion[128];      /* 0x824 */
} MenuQueuedCommand;

/* One resource prefix followed by two complete command records. Resource
 * destructors visit this prefix once, while command routines use 0x1024 strides. */
typedef struct MenuPageSlot {
    s32 kind;
    u32 flags;
    u8 pad08[8];
    struct EffectSlotSet *icon[3]; /* 0x10 */
    MenuPageBar hp;               /* 0x1C */
    MenuPageBar mp;               /* 0x6C */
    struct EffectSlotSet *frame[8]; /* 0xBC */
    struct MenuSprites *windowSprites; /* 0xDC */
    MenuIconBundle *iconBundle;
    u32 unkE4;
    u8 padE8[8];
    MenuQueuedCommand commands[2]; /* 0xF0 and 0x1114 */
} MenuPageSlot;

typedef char MenuCommandPosition_size_check[(sizeof(MenuCommandPosition) == 0x10) ? 1 : -1];
typedef char MenuCommandMotion_size_check[(sizeof(MenuCommandMotion) == 0x10) ? 1 : -1];
typedef char MenuQueuedCommand_size_check[(sizeof(MenuQueuedCommand) == 0x1024) ? 1 : -1];
typedef char MenuQueuedCommand_resource_check[((u32)&((MenuQueuedCommand *)0)->resources == 0x1C) ? 1 : -1];
typedef char MenuQueuedCommand_count_check[((u32)&((MenuQueuedCommand *)0)->particleCount == 0x20) ? 1 : -1];
typedef char MenuQueuedCommand_positions_check[((u32)&((MenuQueuedCommand *)0)->positions == 0x24) ? 1 : -1];
typedef char MenuQueuedCommand_motion_check[((u32)&((MenuQueuedCommand *)0)->motion == 0x824) ? 1 : -1];
typedef char MenuPageSlot_size_check[(sizeof(MenuPageSlot) == 0x2138) ? 1 : -1];
typedef char MenuPageSlot_icon_check[((u32)&((MenuPageSlot *)0)->icon == 0x10) ? 1 : -1];
typedef char MenuPageSlot_hp_check[((u32)&((MenuPageSlot *)0)->hp == 0x1C) ? 1 : -1];
typedef char MenuPageSlot_mp_check[((u32)&((MenuPageSlot *)0)->mp == 0x6C) ? 1 : -1];
typedef char MenuPageSlot_frame_check[((u32)&((MenuPageSlot *)0)->frame == 0xBC) ? 1 : -1];
typedef char MenuPageSlot_sprites_check[((u32)&((MenuPageSlot *)0)->windowSprites == 0xDC) ? 1 : -1];
typedef char MenuPageSlot_commands_check[((u32)&((MenuPageSlot *)0)->commands == 0xF0) ? 1 : -1];

#else
typedef struct MenuPageSlot {
    s32 kind;
    u32 flags;
    u8 pad08[8];
    struct EffectSlotSet *icon[3];
    u8 pad1C[0x4C];
    s32 scaleA;
    s32 offsetA;
    u8 pad70[0x4C];
    s32 scaleB;
    s32 offsetB;
    struct EffectSlotSet *frame[6];
    struct MenuPageResources *resources;
    struct MenuSprites *windowSprites;
    MenuIconBundle *iconBundle;
    u8 padE8[0x4C];
} MenuPageSlot;
typedef char MenuPageSlot_size_check_dds1[(sizeof(MenuPageSlot) == 0x134) ? 1 : -1];
typedef char MenuPageSlot_icon_check_dds1[
    ((u32)&((MenuPageSlot *)0)->icon == 0x10) ? 1 : -1];
typedef char MenuPageSlot_frame_check_dds1[
    ((u32)&((MenuPageSlot *)0)->frame == 0xC4) ? 1 : -1];
#endif

typedef struct MenuPageWindow {
    u32 flags;
    s32 transitionValue;
    PartyPanel *records;
#ifdef VERSION_DDS2
    struct EffectSlotSet *resources; /* 0x0C: resolved base-resource handle */
    s32 slot;
    struct EffectSlotSet *secondaryResource;
    s32 secondarySlot;
    struct EffectSlotSet *alternateResource;
    s32 alternateSlot;
#else
    s32 source;
    s32 slot;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
#endif
#ifdef VERSION_DDS2
    /* One main resource bank, copied and consumed in two eight-entry halves. */
    struct EffectSlotSet *mainResources[16];
    struct EffectSlotSet *handlesC[5];
#else
    struct EffectSlotSet *handlesA[8];
    struct EffectSlotSet *handlesB[8];
    struct EffectSlotSet *handlesC[5];
#endif
    MenuPageSlot slots[5];
    struct MenuList *lists[2];
    s32 selected;
    s32 scrollOffset;
    s32 fade;
} MenuPageWindow;

#ifdef VERSION_DDS2
typedef char MenuPageWindow_size_must_be_0xA6A4[
    sizeof(MenuPageWindow) == 0xA6A4 ? 1 : -1];
typedef char MenuPageWindow_mainResources_offset_check[
    ((u32)&((MenuPageWindow *)0)->mainResources == 0x24) ? 1 : -1];
typedef char MenuPageWindow_mainResources_extent_check[
    sizeof(((MenuPageWindow *)0)->mainResources) == 0x40 ? 1 : -1];
typedef char MenuPageWindow_extraResources_offset_check[
    ((u32)&((MenuPageWindow *)0)->handlesC == 0x64) ? 1 : -1];
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
                   s32, struct EffectSlotSet *, s32, struct EffectSlotSet *, s32);
void mnuSetPanelSlotValues(MenuPageWindow *, struct EffectSlotSet *);
void mnuInitializeCampPanelResources(MenuPageWindow *, StaffSlots *, u32, PartyPanel *);
#else
typedef char MenuPageWindow_handlesA_offset_check[
    ((u32)&((MenuPageWindow *)0)->handlesA == 0x24) ? 1 : -1];
typedef char MenuPageWindow_handlesA_extent_check[
    sizeof(((MenuPageWindow *)0)->handlesA) == 0x20 ? 1 : -1];
typedef char MenuPageWindow_handlesB_offset_check[
    ((u32)&((MenuPageWindow *)0)->handlesB == 0x44) ? 1 : -1];
typedef char MenuPageWindow_handlesC_offset_check[
    ((u32)&((MenuPageWindow *)0)->handlesC == 0x64) ? 1 : -1];
typedef char MenuPageWindow_handlesC_extent_check[
    sizeof(((MenuPageWindow *)0)->handlesC) == 0x14 ? 1 : -1];
void mnuReleasePageHandlesAndClearSelection(MenuPageWindow *);
void mnuShutdownContext(MenuPageWindow *);
void mnuReleaseSpriteTextures(s32 *objectWords);
#endif

void mnuCopyPrimaryWindowHandles(MenuPageWindow *, struct EffectSlotSet **);
void mnuCopySecondaryWindowHandles(MenuPageWindow *, struct EffectSlotSet **);
void mnuRegisterResourceHandles(MenuPageWindow *, struct EffectSlotSet **);

typedef struct MenuGradientFade {
    u32 active;
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

typedef struct MenuAction {
    s32 value;
    u32 mode;
} MenuAction;

/* Per-list storage allocated and cleared as 0x14 bytes by the terminal window builder. */
typedef struct MenuTerminalWindowState {
    MenuAction command;
    s32 pulseFrame;
    u16 unk0C;
    u16 unk0E;
    u16 unk10;
    u16 selectedSlot;
} MenuTerminalWindowState;

typedef char MenuTerminalWindowState_size_must_be_0x14[(sizeof(MenuTerminalWindowState) == 0x14) ? 1 : -1];


typedef struct DspScrollingStrip {
    void *resource;
    s32 frameIndex;
    s32 horizontalOffset;
    s32 verticalOffset;
    s32 scrollSpeed;
} DspScrollingStrip;

typedef struct DspScrollingStripState {
    s32 unk0;
    s32 layout;
    s32 unk8;
    void *resource;
    s32 layer;
    s32 unk14;
    s32 unk18;
    DspScrollingStrip strips[6];
} DspScrollingStripState;

typedef char DspScrollingStrip_size_must_be_0x14[(sizeof(DspScrollingStrip) == 0x14) ? 1 : -1];
typedef char DspScrollingStripState_size_must_be_0x94[(sizeof(DspScrollingStripState) == 0x94) ? 1 : -1];
typedef char DspScrollingStripState_strips_offset[((u32)&((DspScrollingStripState *)0)->strips == 0x1C) ? 1 : -1];
typedef char DspScrollingStripState_firstSpeed_offset[((u32)&((DspScrollingStripState *)0)->strips[0].scrollSpeed == 0x2C) ? 1 : -1];

void mnuInitScrollingStripState(DspScrollingStripState *state, s32 layout, void *resource, s32 firstFrame, s32 layer);
void func_0026BE28(DspScrollingStripState *state, s32 negate, s32 minimum, s32 maximum);
void func_0026BEB0(DspScrollingStripState *state, s32 vertical, s32 horizontal, s32 unused);
void func_0026BEC0(s32 x, s32 y, s32 flags, s32 scale, DspScrollingStripState *state, s32 option);

struct MenuIconSprites;

/* Terminal/shop modes share this complete 0x38C-byte scene allocation. */
typedef struct MenuTerminalContext {
    s32 resourceHandle;
    u8 pad04[4];
    s32 type;
    MenuPopupState transitionWork;
    s32 popupState;
    s32 stateTable;
    s32 messageResources[2];
    struct EffectSlotSet *effectSlots[4];
    s32 state;
    MenuWindowContainer *ownedWindows[1];
    MenuWindowContainer *window;
    struct EffMappedResource *objects[2];
    s32 shopRow;
    s32 multiplier;
    s32 dispatchMode;
    u8 pad98[4];
    s32 availableCount;
    s16 unkA0;
    s16 unkA2;
    s16 unkA4;
    u16 selectedSlot;
    u32 rewardCursor;
    u32 options;
    s32 previousValue;
    s32 elapsedFrames;
    s32 retryFrames;
    s32 commandFrames; /* 0xBC */
    s32 phase;
    u16 stateStep;
    s8 pulseFrame;
    s8 unkC7;
    s32 unkC8;
    s8 advancedSlots;
    s8 unkCD;
    s8 rewardMode;
    u8 padCF;
    s8 rewardRow;
    s8 remainingRewards;
    s8 rewardIndex;
    s8 announceNextReward;
    s8 grantPendingReward;
    s8 rewardDelay;
    u8 padD6[2];
    s32 rewardKind;
    s32 rewardValue;
    s32 prepared;
    s32 delayFrames;
    DspScrollingStripState panelWork[2];
    MenuCampEffect campEffect; /* 0x210: resources, sixteen sparks and badge fade; DDS2 0025FE58. */
    struct MenuIconSprites *windowResource;
    MenuGradientFade gradientFade;
    u8 rewardGranted;
    s8 sceneReady;
    u8 pad38A[2];
} MenuTerminalContext;

typedef char MenuTerminalContext_size_must_be_0x38C[(sizeof(MenuTerminalContext) == 0x38C) ? 1 : -1];
typedef char MenuTerminalContext_firstPanel_offset[((u32)&((MenuTerminalContext *)0)->panelWork[0] == 0xE8) ? 1 : -1];
typedef char MenuTerminalContext_secondPanel_offset[((u32)&((MenuTerminalContext *)0)->panelWork[1] == 0x17C) ? 1 : -1];
typedef char MenuTerminalContext_campEffect_offset[((u32)&((MenuTerminalContext *)0)->campEffect == 0x210) ? 1 : -1];
typedef char MenuTerminalContext_windowResource_offset[
    ((u32)&((MenuTerminalContext *)0)->windowResource == 0x378) ? 1 : -1];

extern MenuTerminalContext *D_00438FC8;
void func_0025FD78(MenuTerminalContext *scene);


typedef struct MenuIconSprites {
    u32 handle;
    u32 value;
    u32 unk8;
    struct EffectSlotSet *sprite[3];
} MenuIconSprites;

MenuIconSprites *mnuCreateWindowSpriteResources(u32 width, u32 height, u32 value,
                    u32 resourceHandle, s32 *indices, u32 unused);
void mnuReleaseWindowTextures(MenuIconSprites *menu);

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
    MenuPanelGroup *panelGroup;
#ifdef VERSION_DDS2
    MenuSpriteState *effectResource;
    MenuProfilePanel *currentEffect;
#else
    MenuSimpleSpriteState *effectResource;
    MenuProfilePanel *currentEffect;
#endif
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
    u8 pad20[0x0C];
    struct MenuList *selectionList; /* 0x2C: created by the movie selection-list builder */
    void *resources;     /* 0x30 */
    s32 unk34;           /* 0x34 */
    u8 pad38[8];
} MovieMenuState;

typedef char MovieMenuState_size_must_be_0x40[(sizeof(MovieMenuState) == 0x40) ? 1 : -1];

#include "mnu_scene.h"
#endif

#ifdef VERSION_DDS2
/* Staff menu task context (DDS2 layout). */
struct MenuIconState;


typedef struct MenuStaffContext {
    u8 pad00[8];
    MenuPopupState transitionWork; /* +0x08: native saved-entry transition state */
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
    MenuWindowContainer *skillWindow; /* 0x104: field-skill window; 002AAEA0 reads list->last. */
    MenuWindowContainer *activeWindow; /* 0x108 */
    u8 pad10C[0xC];
    s32 unk118;
    u8 pad11C[0x168];
    /* 002BD480 consumes the full page owner; its first list is at 0xA914. */
    MenuPageWindow partyWindow; /* 0x284..0xA927 */
    PartyPanel partyPanel;     /* 0xA928: initialized by 002A9068 and 002ACF38. */
    MenuPanelGroup *panelHandle; /* 0xAA34 */
    MenuSpriteState *spriteHandle; /* 0xAA38 */
    u8 padAA3C[0xC];
    void *menu;           /* 0xAA48: menu-mode-specific child allocation */
    u8 padAA4C[4];
    u32 unkAA50;
    u8 padAA54[0x3CC];
    u16 catalogOrdinals[0x100]; /* 0xAE20: item-ID-indexed list sorting keys */
    u8 padB020[0xEC];
    MenuFadeFields fade; /* 0xB10C: initialized by 002A9068; used by 002BAF50/002BB0E8. */
    u8 tail[0x10];        /* 0xB1D0: remaining opaque bytes of the 0xB1E0 allocation. */
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
