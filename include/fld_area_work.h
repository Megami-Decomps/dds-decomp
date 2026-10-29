#ifndef FLD_AREA_WORK_H
#define FLD_AREA_WORK_H

#include "common.h"
#include "fld_encounter_runtime_api.h"
#include "fld_special_event_state.h"
#include "fld_scene_bgm_suppression_state.h"
#include "fld_map_banner_fade_mode.h"
#include "fld_solar_overlay_mode.h"
#include "fld_scene_timing_state.h"

struct sdfTexHandle_t;
struct sdfMemHandle_t;
struct SdfTextureFileHeader;
struct dds3Basic_t;

typedef enum FldAreaResourceRequestState {
    FLD_AREA_RESOURCE_REQUEST_IDLE = 0,
    FLD_AREA_RESOURCE_REQUEST_LOAD_PENDING = 1,
    FLD_AREA_RESOURCE_REQUEST_SATISFIED_FROM_CACHE = 2
} FldAreaResourceRequestState;

typedef u32 FldCameraTurnRequestState;
enum FldCameraTurnRequestState {
    FLD_CAMERA_TURN_REQUEST_IDLE = 0,
    FLD_CAMERA_TURN_REQUEST_PENDING = 1,
    FLD_CAMERA_TURN_REQUEST_TURNING = 2
};

typedef u32 FldSceneAdvanceRequestState;
enum FldSceneAdvanceRequestState {
    FLD_SCENE_ADVANCE_REQUEST_NONE = 0,
    FLD_SCENE_ADVANCE_REQUEST_PENDING = 1
};

typedef s32 FldModeOneCameraState;
enum FldModeOneCameraStateValue {
    FLD_MODE_ONE_CAMERA_INACTIVE = 0,
    FLD_MODE_ONE_CAMERA_ACTIVE = 1
};

/* Alternate player model latch; distinct from the cached model variant. */
typedef s32 FldPlayerModelAlternateState;
enum FldPlayerModelAlternateStateValue {
    FLD_PLAYER_MODEL_ALTERNATE_DISABLED = 0,
    FLD_PLAYER_MODEL_ALTERNATE_ENABLED = 1
};

typedef s32 FldPlayerModelVariant;
enum FldPlayerModelVariantValue {
    FLD_PLAYER_MODEL_VARIANT_STANDARD = 0,
    FLD_PLAYER_MODEL_VARIANT_PLAYER_B = 1,
    FLD_PLAYER_MODEL_VARIANT_LOCATION_2 = 2
};

typedef s32 FldAreaResourceReuseState;

enum FldAreaResourceReuseState {
    FLD_AREA_RESOURCE_REUSE_DISABLED = 0,
    FLD_AREA_RESOURCE_REUSE_ENABLED = 1
};

typedef s32 FldTerrainAttritionKind;

enum FldTerrainAttritionKind {
    FLD_TERRAIN_ATTRITION_NONE = 0,
    FLD_TERRAIN_ATTRITION_STANDARD = 1,
    FLD_TERRAIN_ATTRITION_SOUND_CLASS_30 = 2,
    FLD_TERRAIN_ATTRITION_SOUND_CLASS_20 = 3
};

/* Positive values hold player movement until the corresponding gate clears. */
typedef s32 FldMovementGateState;

typedef s32 FldDeferredExitState;
enum FldDeferredExitState {
    FLD_DEFERRED_EXIT_IDLE = 0,
    FLD_DEFERRED_EXIT_REQUESTED = 1,
    FLD_DEFERRED_EXIT_MESSAGE_ACTIVE = 2
};

typedef s32 FldEntryDialogDisposition;

enum FldEntryDialogDisposition {
    FLD_ENTRY_DIALOG_SUPPRESS = -1,
    FLD_ENTRY_DIALOG_NONE = 0,
    FLD_ENTRY_DIALOG_REQUEST = 1
};

typedef s32 FldPositionUpdateState;
enum FldPositionUpdateStateValue {
    FLD_POSITION_UPDATE_NONE = 0,
    FLD_POSITION_UPDATE_PENDING = 1
};

typedef s32 FldCameraMode;

/* Runtime index into the FLDALL.TBL camera-follow row table. */
typedef s32 FldCameraFollowRowIndex;

enum FldCameraMode {
    FLD_CAMERA_MODE_PLAYER_FOLLOW = 0,
    FLD_CAMERA_MODE_OBJECT_FOCUS = 1,
    FLD_CAMERA_MODE_ALTERNATE_FOCUS = 3,
    FLD_CAMERA_MODE_FOLLOW_UPDATE = 4,
    FLD_CAMERA_MODE_NPC_REQUEST = 5
};

/* Each resourceData pointer is a read-only view into its retained allocation. */
typedef struct FldResourceBlock {
    struct sdfMemHandle_t *allocation;
    const struct SdfTextureFileHeader *resourceData;
} FldResourceBlock;

typedef struct FldTextureResource {
    struct sdfMemHandle_t *allocation;
    const struct SdfTextureFileHeader *resourceData;
    struct sdfTexHandle_t *texture;
} FldTextureResource;

/* Shared field controller, camera/motion state and retained map resources.
 * XYZ is followed by saved XYZ history, not a homogeneous fourth component. */
typedef struct fldAreaWork_t {
    struct dds3Basic_t *taskHandle; /* 0x00: first spawned area basic object */
    const char *fallbackResourceTaskName;
    u8 pad08[4];
    s32 consumedFlags;
    s32 area;                    /* 0x10 */
    s32 floor;                   /* Zero-based; room lookups use floor + 1. */
    s32 unk18;
    s32 cachedArea;
    FldAreaResourceReuseState reuseAreaResources; /* 0x20: preserves same-area resources across transitions */
    s32 titleFade;
    s32 sequenceCode;            /* 0x28: retained scene/sequence index */
    s32 unk2C;
    f32 focusPos[3];
    u8 pad3C[4];
    char requestedSceneName[0x10];
    FldCameraMode mode;          /* 0x50: camera follow/focus state */
    s32 unk54;
    FldCameraFollowRowIndex cameraFollowRowIndex; /* 0x58 */
    f32 cameraSupportHeightOffset;
    f32 unk60;
    f32 negatedAngle;
    f32 cameraTargetAngle;       /* 0x68 */
    f32 dist;
    FldSceneTransitionCooldown sceneTransitionCooldown;
    FldMovementInputSuppressionFrames movementInputSuppressionFrames;
    FldAreaResourceRequestState resourceRequestState; /* 0x78: local-map request state. */
    s32 resourceArea;
    /* Resource/cache key; (0, 0) is the battle-return no-request sentinel. */
    s32 resourceRoomKey;
    FldPositionUpdateState positionPending; /* 0x84: corrected pose awaits application. */
    s32 selectedRoomIndex;
    s32 unk8C;
    s32 primaryMovingMatterIndex;   /* 0x90: -1 when no moving matter record is selected. */
    s32 secondaryMovingMatterIndex; /* 0x94: optional second moving matter record. */
    s32 unk98;
    f32 unk9C;
    s32 unkA0;
    s32 unkA4;
    s32 overrideSupportRecordIndex; /* -1 selects the default support record. */
    s32 unkAC;
    s32 cameraTiltActive;        /* 0xB0 */
    f32 unkB4;
    f32 cameraTilt;              /* 0xB8 */
    s32 flagNumber;
    s32 flagStage;                /* 0xC0: one-based stage associated with the current floor flag. */
    s32 primaryMapLabelSelection;   /* -1 means no primary label selection. */
    s32 secondaryMapLabelSelection; /* -1 means no secondary label selection. */
    FldSolarOverlayMode overlayMode;
    s32 overlayCounter;
    FldMapBannerFadeMode mapBannerFadeMode;
    s32 unkD8;
    s32 defaultEncounterId;      /* 0xDC: selected room encounter; D_0038984C in DDS2. */
    s32 encounterOverrideId;
    FldEncounterRuntimeMode encounterMode;
    s32 cameraTargetGraceTicks; /* 0xE8: suppresses travel deltas after target install. */
    u32 transitionCount;
    s32 nextArea;
    s32 nextFloor;
    u8 padF8[4];
    f32 frameTravelDistance;     /* 0xFC: current player displacement length. */
    FldEntryDialogDisposition pendingEntryDialogDisposition;
    FldSpecialEventState specialEventActive; /* 0x104: Spark/Devilize field event latch. */
    u16 collectedCount;          /* 0x106: spark pickups. */
    s32 score;                   /* 0x108: pickups add 50. */
    f32 unk10C;                  /* 0x10C: cleared when a spark run starts. */
    FldSceneAdvanceRequestState pendingSceneRequest; /* one-shot request to advance to the next scene */
    FldDeferredExitState deferredExit; /* 0x114: idle, requested, or waiting for the exit message to finish. */
    FldPlayerModelAlternateState playerModelAlternate; /* 0x118: selects the alternate player model. */
    FldTerrainAttritionKind terrainAttritionKind; /* 0x11C: collision-zone category for HP attrition. */
    f32 attritionDistance;      /* 0x120: movement toward the next HP attrition tick. */
#ifdef VERSION_DDS2
    u8 pad124[2];
    s16 movementSoundActive;    /* 0x126 */
#else
    u8 pad124[4];
#endif
    /* Zero means no scene-light override; nonzero selects a light-set index.
     * Command-active state is tracked separately. */
    s16 sceneLightSetOverrideIndex; /* 0x128 */
    s16 colorEffectSuppressed;
    s32 commandEnabled;
    FldSceneBgmSuppressionState sceneBgmSuppressed;
    s32 skipFade;
    s32 unk138;
#ifdef VERSION_DDS2
    s32 targetGuideEnabled; /* Current scene permits target-guide interaction and updates. */
    s32 targetGuideActive;
    s32 overlayFadeInFrames; /* Remaining frames of the 20-frame overlay fade-in. */
#endif
    FldPlayerModelVariant playerModelVariant; /* 0x13C in DDS1, 0x148 in DDS2 */
    f32 x;
    f32 y;
    f32 z;
    f32 previousX;
    f32 previousY;
    f32 previousZ;
    f32 targetX;
    f32 targetY;
    f32 targetZ;
    f32 angle;
    f32 targetAngle;
#ifdef VERSION_DDS1
    f32 previousCameraModelFrame;
    f32 cameraSupportHeightDelta;
    f32 modeOneCameraHeadingTarget;
#else
    f32 previousCameraModelFrame;
    f32 cameraSupportHeightDelta;
    f32 modeOneCameraHeadingTarget;
#endif
    FldModeOneCameraState modeOneCameraState; /* Special door-transition camera pose is active. */
#ifdef VERSION_DDS1
    FldMovementGateState fieldTransitionMovementGate; /* 0x17C */
#else
    FldMovementGateState fieldTransitionMovementGate; /* 0x188 */
#endif
    s32 verticalStepDirection;
#ifdef VERSION_DDS1
    FldMovementGateState areaDamageReactionMovementGate; /* 0x184 */
#else
    FldMovementGateState areaDamageReactionMovementGate; /* 0x190 */
#endif
    FldCameraTurnRequestState facingPointState;
    f32 facingPointX;
    f32 facingPointZ;
    FldCameraTurnRequestState headingState;
    f32 overrideAngle;
#ifdef VERSION_DDS1
    FldResourceBlock mapResources[4]; /* 0x19C: fldmix.LB nodes 10..13 */
#else
    FldResourceBlock mapResources[8]; /* 0x1A8: autmap_1,2,3,5,6,7,8,9 */
    FldTextureResource fieldTextures[4]; /* 0x1E8: d2_fild1..4.tmx */
#endif
} fldAreaWork_t;

typedef char fldAreaWork_t_frameTravelDistance_at_FC[((u32)&((fldAreaWork_t *)0)->frameTravelDistance == 0xFC) ? 1 : -1];
typedef char fldAreaWork_t_attritionDistance_at_120[((u32)&((fldAreaWork_t *)0)->attritionDistance == 0x120) ? 1 : -1];

extern fldAreaWork_t fldGlobalBase;

#define FLD_AREA_OFFSET(member) ((u32)&((fldAreaWork_t *)0)->member)
typedef char FldResourceBlock_size_check[(sizeof(FldResourceBlock) == 8) ? 1 : -1];
typedef char FldTextureResource_size_check[(sizeof(FldTextureResource) == 0xC) ? 1 : -1];
typedef char FldResourceBlock_resourceData_offset_check[
    ((u32)&((FldResourceBlock *)0)->resourceData == 0x04) ? 1 : -1];
typedef char FldTextureResource_resourceData_offset_check[
    ((u32)&((FldTextureResource *)0)->resourceData == 0x04) ? 1 : -1];
typedef char fldAreaWork_t_area_offset_check[(FLD_AREA_OFFSET(area) == 0x10) ? 1 : -1];
typedef char fldAreaWork_t_defaultEncounterId_offset_check[(FLD_AREA_OFFSET(defaultEncounterId) == 0xDC) ? 1 : -1];
typedef char fldAreaWork_t_resourceRequestState_offset_check[(FLD_AREA_OFFSET(resourceRequestState) == 0x78) ? 1 : -1];
typedef char fldAreaWork_t_positionPending_offset_check[
    (FLD_AREA_OFFSET(positionPending) == 0x84) ? 1 : -1];
typedef char fldAreaWork_t_cameraTargetGraceTicks_offset_check[
    (FLD_AREA_OFFSET(cameraTargetGraceTicks) == 0xE8) ? 1 : -1];
typedef char fldAreaWork_t_specialEventActive_offset_check[
    (FLD_AREA_OFFSET(specialEventActive) == 0x104) ? 1 : -1];
typedef char fldAreaWork_t_pendingSceneRequest_offset_check[(FLD_AREA_OFFSET(pendingSceneRequest) == 0x110) ? 1 : -1];
typedef char fldAreaWork_t_deferredExit_offset_check[(FLD_AREA_OFFSET(deferredExit) == 0x114) ? 1 : -1];
typedef char fldAreaWork_t_flagStage_offset_check[(FLD_AREA_OFFSET(flagStage) == 0xC0) ? 1 : -1];
typedef char fldAreaWork_t_playerModelAlternate_offset_check[(FLD_AREA_OFFSET(playerModelAlternate) == 0x118) ? 1 : -1];
typedef char fldAreaWork_t_terrainAttritionKind_offset_check[(FLD_AREA_OFFSET(terrainAttritionKind) == 0x11C) ? 1 : -1];
typedef char fldAreaWork_t_colorEffectSuppressed_offset_check[(FLD_AREA_OFFSET(colorEffectSuppressed) == 0x12A) ? 1 : -1];
typedef char fldAreaWork_t_sceneBgmSuppressed_offset_check[(FLD_AREA_OFFSET(sceneBgmSuppressed) == 0x130) ? 1 : -1];
#ifdef VERSION_DDS1
typedef char fldAreaWork_t_size_check[(sizeof(fldAreaWork_t) == 0x1BC) ? 1 : -1];
typedef char fldAreaWork_t_fieldTransitionMovementGate_offset_check[
    (FLD_AREA_OFFSET(fieldTransitionMovementGate) == 0x17C) ? 1 : -1];
typedef char fldAreaWork_t_areaDamageReactionMovementGate_offset_check[
    (FLD_AREA_OFFSET(areaDamageReactionMovementGate) == 0x184) ? 1 : -1];
typedef char fldAreaWork_t_x_offset_check[(FLD_AREA_OFFSET(x) == 0x140) ? 1 : -1];
typedef char fldAreaWork_t_previousCameraModelFrame_offset_check[(FLD_AREA_OFFSET(previousCameraModelFrame) == 0x16C) ? 1 : -1];
typedef char fldAreaWork_t_modeOneCameraState_offset_check[
    (FLD_AREA_OFFSET(modeOneCameraState) == 0x178) ? 1 : -1];
typedef char fldAreaWork_t_mapResources_offset_check[(FLD_AREA_OFFSET(mapResources) == 0x19C) ? 1 : -1];
#else
typedef char fldAreaWork_t_size_check[(sizeof(fldAreaWork_t) == 0x218) ? 1 : -1];
typedef char fldAreaWork_t_fieldTransitionMovementGate_offset_check[
    (FLD_AREA_OFFSET(fieldTransitionMovementGate) == 0x188) ? 1 : -1];
typedef char fldAreaWork_t_areaDamageReactionMovementGate_offset_check[
    (FLD_AREA_OFFSET(areaDamageReactionMovementGate) == 0x190) ? 1 : -1];
typedef char fldAreaWork_t_x_offset_check[(FLD_AREA_OFFSET(x) == 0x14C) ? 1 : -1];
typedef char fldAreaWork_t_previousCameraModelFrame_offset_check[(FLD_AREA_OFFSET(previousCameraModelFrame) == 0x178) ? 1 : -1];
typedef char fldAreaWork_t_modeOneCameraState_offset_check[
    (FLD_AREA_OFFSET(modeOneCameraState) == 0x184) ? 1 : -1];
typedef char fldAreaWork_t_mapResources_offset_check[(FLD_AREA_OFFSET(mapResources) == 0x1A8) ? 1 : -1];
typedef char fldAreaWork_t_fieldTextures_offset_check[(FLD_AREA_OFFSET(fieldTextures) == 0x1E8) ? 1 : -1];
#endif
#undef FLD_AREA_OFFSET

#endif
