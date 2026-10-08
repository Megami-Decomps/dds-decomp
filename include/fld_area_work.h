#ifndef FLD_AREA_WORK_H
#define FLD_AREA_WORK_H

#include "common.h"

struct SdfTex;

/* Field loaders retain allocation/address words in these resource records. */
typedef struct FldResourceBlock {
    s32 unk0;
    s32 block;
} FldResourceBlock;

typedef struct FldTextureResource {
    s32 unk0;
    s32 block;
    struct SdfTex *texture;
} FldTextureResource;

/* Shared field controller, camera/motion state and retained map resources.
 * XYZ is followed by saved XYZ history, not a homogeneous fourth component. */
typedef struct FldAreaWork {
    s32 taskHandle;              /* 0x00: stored task handle word */
    char *fallbackResourceName;
    u8 pad08[4];
    s32 consumedFlags;
    s32 area;                    /* 0x10 */
    s32 floor;                   /* Zero-based; room lookups use floor + 1. */
    s32 unk18;
    s32 cachedArea;
    s32 unk20;
    s32 titleFade;
    s32 sequenceCode;            /* 0x28: retained scene/sequence index */
    u8 pad2C[4];
    f32 focusPos[3];
    u8 pad3C[4];
    char requestedSceneName[0x10];
    s32 mode;                    /* 0x50: includes the focus modes */
    u8 pad54[4];
    s32 rowIdx;
    f32 unk5C;
    f32 unk60;
    f32 negatedAngle;
    u8 pad68[4];
    f32 dist;
    s32 sceneMode;
    s32 sceneState;
    s32 resourceFlag;            /* 0x78: local-map resource state */
    s32 resourceArea;
    s32 resourceFloor;
    s32 positionPending;
    s32 unk88;
    s32 unk8C;
    s32 unk90;
    s32 unk94;
    u8 pad98[8];
    s32 unkA0;
    s32 unkA4;
    u8 padA8[4];
    s32 unkAC;
    u8 padB0[0xC];
    s32 flagNumber;
    s32 unkC0;
    u8 padC4[8];
    s32 overlayMode;
    s32 overlayCounter;
    s32 unkD4;
    u8 padD8[0xC];
    s32 encounterMode;
    s32 unkE8;
    u32 transitionCount;
    s32 nextArea;
    s32 nextFloor;
    u8 padF8[8];
    s32 unk100;
    s16 transitionMode;          /* 0x104 */
    u8 pad106[0xA];
    u32 pendingSceneRequest;
    s32 deferredExit;
    s32 unk118;
    u8 pad11C[0xC];
    s16 sceneCommand;
    s16 colorEffectSuppressed;
    s32 commandEnabled;
    s32 unk130;
    s32 skipFade;
    s32 unk138;
#ifdef VERSION_DDS2
    s32 unk13C;
    s32 targetGuideActive;
    s32 unk144;
#endif
    s32 playerModelVariant;      /* 0x13C in DDS1, 0x148 in DDS2 */
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
    f32 unk16C;
    f32 unk170;
    f32 unk174;
#else
    f32 unk178;
    f32 unk17C;
    f32 unk180;
#endif
    s32 positionMode;
#ifdef VERSION_DDS1
    s32 unk17C;
#else
    s32 unk188;
#endif
    s32 verticalStepDirection;
#ifdef VERSION_DDS1
    s32 unk184;
#else
    s32 unk190;
#endif
    u32 pointState;
    f32 facingPointX;
    f32 facingPointZ;
    u32 angleState;
    f32 overrideAngle;
#ifdef VERSION_DDS1
    FldResourceBlock mapResources[4]; /* 0x19C: fldmix.LB nodes 10..13 */
#else
    FldResourceBlock mapResources[8]; /* 0x1A8: autmap_1,2,3,5,6,7,8,9 */
    FldTextureResource fieldTextures[4]; /* 0x1E8: d2_fild1..4.tmx */
#endif
} FldAreaWork;

extern FldAreaWork fldAreaState;

#define FLD_AREA_OFFSET(member) ((u32)&((FldAreaWork *)0)->member)
typedef char FldResourceBlock_size_check[(sizeof(FldResourceBlock) == 8) ? 1 : -1];
typedef char FldTextureResource_size_check[(sizeof(FldTextureResource) == 0xC) ? 1 : -1];
typedef char FldAreaWork_area_offset_check[(FLD_AREA_OFFSET(area) == 0x10) ? 1 : -1];
typedef char FldAreaWork_resourceFlag_offset_check[(FLD_AREA_OFFSET(resourceFlag) == 0x78) ? 1 : -1];
typedef char FldAreaWork_transitionMode_offset_check[(FLD_AREA_OFFSET(transitionMode) == 0x104) ? 1 : -1];
typedef char FldAreaWork_pendingSceneRequest_offset_check[(FLD_AREA_OFFSET(pendingSceneRequest) == 0x110) ? 1 : -1];
typedef char FldAreaWork_colorEffectSuppressed_offset_check[(FLD_AREA_OFFSET(colorEffectSuppressed) == 0x12A) ? 1 : -1];
#ifdef VERSION_DDS1
typedef char FldAreaWork_size_check[(sizeof(FldAreaWork) == 0x1BC) ? 1 : -1];
typedef char FldAreaWork_x_offset_check[(FLD_AREA_OFFSET(x) == 0x140) ? 1 : -1];
typedef char FldAreaWork_mapResources_offset_check[(FLD_AREA_OFFSET(mapResources) == 0x19C) ? 1 : -1];
#else
typedef char FldAreaWork_size_check[(sizeof(FldAreaWork) == 0x218) ? 1 : -1];
typedef char FldAreaWork_x_offset_check[(FLD_AREA_OFFSET(x) == 0x14C) ? 1 : -1];
typedef char FldAreaWork_mapResources_offset_check[(FLD_AREA_OFFSET(mapResources) == 0x1A8) ? 1 : -1];
typedef char FldAreaWork_fieldTextures_offset_check[(FLD_AREA_OFFSET(fieldTextures) == 0x1E8) ? 1 : -1];
#endif
#undef FLD_AREA_OFFSET

#endif
