#ifndef EFFECT_H
#define EFFECT_H

#include "common.h"
#include "effPCPData.h"
#include "libsdf.h"

typedef struct effPCPBossBUNSIN2Param_t
{
    struct nbFormation_t *pStartForm;
    struct nbFormation_t *pForm1;
    struct nbFormation_t *pForm2L;
    struct nbFormation_t *pForm2R;
} effPCPBossBUNSIN2Param_t;

typedef struct effPCPBossONGYParam_t {
    struct nbFormation_t *pFormation;
} effPCPBossONGYParam_t;

struct effPCPBossONGY_t;
struct effPCPBossBUNSIN2_t;
struct effPCPBossONGY_t *effPCPBossONGYCreate(effPCPBossONGYParam_t *param);
struct effPCPBossONGY_t *effPCPBossONGY2Create(effPCPBossONGYParam_t *param);
struct effPCPBossBUNSIN2_t *effPCPBossBUNSIN2Create(effPCPBossBUNSIN2Param_t *param);

typedef struct effPCPBossBUNSINParam_t {
    union { s32 pStartBits; struct nbFormation_t *pStartForm; };
    union { s32 pEndBits; struct nbFormation_t *pEndForm; };
} effPCPBossBUNSINParam_t;

typedef struct nbCameraSkillPtr_t {
    u32 ptr_shot_1;
    u32 ptr_shot_23;
    u32 ptr_angleH;
    u32 ptr_angleW;
    u32 ptr_H;
    u32 ptr_W;
    u32 anim;
} nbCameraSkillPtr_t;

/* Shared effect RNG and retained slot-array header. */
typedef struct effRandState_t {
    u32 words[4];
} effRandState_t;

typedef struct effArrHdr_t {
    void *slots;
    u32 count;
    sdfMemHandle_t *allocation;
} effArrHdr_t;


/* Per-cell vector history and packed color stream (0x14 bytes). */
typedef struct parCell_t {
    u128 *history;
    u32 *colors;
    s32 vertexCount;
    s32 historyAdvanceCountdown;
    u32 color;
} parCell_t;

/* The complete 0x2C-byte cell-system owner shared by PCP effects. */
typedef struct parSystem_t {
    u16 kind;
    u16 bucket;
    s32 cellCount;
    s32 vertexWordCount;
    s32 groupDivisor;
    sdfMemHandle_t *allocation;
    parCell_t *cells;
    u128 *vertices;
    u32 *colors;
    sdfMat_t *asset;
    struct parSystem_t *next;
    s32 unk28;
} parSystem_t;

/* Four XYZ controls, evaluation position and frame step (0x38 bytes). */
typedef struct effCubicBezierSlot_t {
    f32 controlPoints[4][3];
    f32 t;
    f32 step;
} effCubicBezierSlot_t;

typedef struct effBezierPoint_t {
    f32 x, y, z;
} effBezierPoint_t;

/* Seven packed XYZ controls form two cubic segments in each 0x60-byte curve. */
typedef struct effBezierCurve_t {
    effBezierPoint_t controlPoints[7];
    u32 pointIndex;
    f32 t;
    f32 parameterStep;
} effBezierCurve_t;

/* One ST/XYZ2 vertex in a blur tri-strip packet. */
typedef struct effBlurPrimVertex_t {
    f32 s;
    f32 t;
    f32 q;
    s32 unk0C;
    s32 x;
    s32 y;
    s32 z;
    s16 adc;
    s16 unk1E;
} effBlurPrimVertex_t;

/* Nine A+D registers: one colour and four ST/XYZ2 pairs. */
typedef struct effBlurPrimTriStrip_t {
    s32 r;
    s32 g;
    s32 b;
    s32 a;
    effBlurPrimVertex_t vtx[4];
} effBlurPrimTriStrip_t;

/* Shared rectangle and sampling payload, copied as 0x28 bytes. */
typedef struct dds3Effect_Blur_t {
    u32 abgr;
    s32 blend;
    f32 rotate;
    f32 scale;
    s32 center[2];
    s32 xy[2][2];
} dds3Effect_Blur_t;

/* The stored screen record keeps eight bytes beyond the copied blur quad. */
typedef struct effScreenDrawParams_t {
    dds3Effect_Blur_t source;
    u8 pad28[8];
} effScreenDrawParams_t;

/* Phase-driven blur parameters and the scale-slot allocation they own. */
typedef struct dds3Effect_RippleBlurParam_t {
    s32 number;
    f32 speed;
    f32 interval;
    u32 abgr;
    s32 blend;
    f32 rotate;
    f32 power;
    f32 frequency;
    s32 center[2];
    s32 area;
} dds3Effect_RippleBlurParam_t;

typedef struct effBlurScaleSlot_t {
    f32 phase;
    f32 angle;
    dds3Effect_Blur_t quad;
} effBlurScaleSlot_t;

typedef struct effBlurScaleWork_t {
    dds3Effect_RippleBlurParam_t params;
    sdfTexHandle_t *texture;
    sdfMemHandle_t *allocation;
    effBlurScaleSlot_t *slots;
} effBlurScaleWork_t;

/* Random-position blur parameters and their allocated slot owner. */
typedef struct dds3Effect_DistortionBlurParam_t {
    s32 number;
    s32 interval;
    f32 speed;
    u32 abgr;
    s32 blend;
    f32 rotate;
    f32 distortion;
    s32 center[2];
    s32 area;
    s32 width;
} dds3Effect_DistortionBlurParam_t;

typedef struct effBlurScatterSlot_t {
    s32 delay;
    f32 angle;
    dds3Effect_Blur_t quad;
} effBlurScatterSlot_t;

typedef struct effBlurScatterWork_t {
    dds3Effect_DistortionBlurParam_t params;
    sdfTexHandle_t *texture;
    sdfMemHandle_t *allocation;
    effBlurScatterSlot_t *slots;
} effBlurScatterWork_t;

/* A copied 0x2C-byte body followed by its separately acquired texture. */
typedef struct dds3Effect_PartialMultiBlurParam_t {
    s32 area;
    dds3Effect_Blur_t Blur;
} dds3Effect_PartialMultiBlurParam_t;

typedef struct effBlurTemplate_t {
    dds3Effect_PartialMultiBlurParam_t body;
    sdfTexHandle_t *texture;
} effBlurTemplate_t;

typedef struct {
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
} effResourceRectBounds_t;

typedef struct {
    /* Packed movie RGBA and the renderer's channel view share one word. */
    union { u8 color[4]; u32 rgba; };
    s32 blendControl;
    effResourceRectBounds_t bounds;
} effResourceRectDrawParams_t;

/* Nine copied words; the selected source texture belongs to the owner. */
typedef struct {
    s32 area;
    s32 center[2];
    effResourceRectDrawParams_t Filter;
} dds3Effect_PartialMultiFilterParam_t;

typedef struct {
    dds3Effect_PartialMultiFilterParam_t param;
    sdfTexHandle_t *hAlphaTex;
} dds3Effect_PartialMultiFilter_t;

typedef struct dds3Effect_Filter_t {
    u32 abgr;
    s32 blend;
    s32 xy[2][2];
} dds3Effect_Filter_t;

#endif /* EFFECT_H */
