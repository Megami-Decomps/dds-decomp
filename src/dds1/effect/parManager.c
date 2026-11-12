#include "common.h"
#include "libsdf.h"
#include "ps2asm.h"
#include "effect.h"
#include "billboard.h"
#include "sdfFrame.h"

/* DDS's names for the same objects. */
#define ParRand effEmitterDelayRandomState
#define dds3EffectBase effDefaultRandomState

/* Particle emitters, history ribbons and billboard dispatch, reconstructed from
 * the shared DDS engine and checked against SLUS_209.11. All emitter views
 * retain the native 0x150-byte prefix; the kind-specific tail follows it. */

typedef struct dds3Particle_BlurBuffList_t dds3Particle_BlurBuffList_t;
typedef struct dds3Particle_BlurBuff_t dds3Particle_BlurBuff_t;
typedef struct dds3Particle_Effect_t dds3Particle_Effect_t;
typedef struct parDispatch_t parDispatch_t;
typedef struct effectBufferRecord_t effectBufferRecord_t;
typedef struct dds3Particle_Physics_t dds3Particle_Physics_t;
typedef struct effEmitterHead_t effEmitterHead_t;
typedef struct effTemplatePacketList_t effTemplatePacketList_t;
typedef struct dds3ParticleColorAnim_t dds3ParticleColorAnim_t;
typedef struct dds3Particle_Basic_t dds3Particle_Basic_t;
typedef struct parObjClientView_t parObjClientView_t;
typedef struct parKindResource_t parKindResource_t;
typedef struct dds3ParOffsetBuffParts_t dds3ParOffsetBuffParts_t;
typedef struct dds3ParOffsetBuff_t dds3ParOffsetBuff_t;
typedef struct effParticleRecord_t effParticleRecord_t;
typedef struct effParticleOverlay_t effParticleOverlay_t;
typedef struct effTemplateScaleRecord_t effTemplateScaleRecord_t;
typedef struct dds3Particle_Radiate_t3 dds3Particle_Radiate_t3;
typedef struct dds3Particle_Spiral_t3 dds3Particle_Spiral_t3;
typedef struct dds3Particle_Smoke_t3 dds3Particle_Smoke_t3;
typedef struct dds3Particle_Spark_t3 dds3Particle_Spark_t3;
typedef struct dds3Particle_SphereSpiral_t3 dds3Particle_SphereSpiral_t3;
typedef struct dds3Particle_SphereSmoke_t3 dds3Particle_SphereSmoke_t3;
typedef struct dds3Particle_SphereRing_t3 dds3Particle_SphereRing_t3;
typedef struct dds3Particle_Spark2_t3 dds3Particle_Spark2_t3;
typedef struct dds3Particle_SparkOffset_t3 dds3Particle_SparkOffset_t3;
typedef struct dds3Particle_SmokeOffset_t3 dds3Particle_SmokeOffset_t3;
typedef struct dds3BillResource_tag dds3BillResource_tag;
typedef struct billData_t billData_t;
typedef struct billOut_t billOut_t;
typedef struct effTrackPolyList_t effTrackPolyList_t;


typedef struct {
    struct dds3ModelHandle_t *model;
    s32 idA;
    s32 idB;
    f32 startFrame;
    f32 endFrame;
    u32 sampleInterval;
    s32 historyLength;
    u32 unk1C;
    u32 kind;
    u32 gradientColors[4];
} effTrackPolyParams_t;


typedef struct dds3Particle_BlurBuffList_t {
    u128 *pPos;
    u16 count;
    u8 pad06[2];
    u32 abgr;
    f32 scale;
} dds3Particle_BlurBuffList_t;

typedef struct dds3Particle_BlurBuff_t {
    u16 number;
    u16 point;
    dds3Particle_BlurBuffList_t *blurList;
    billHandle_t **pBill_Handle;
    sdfMemHandle_t *hMem;
} dds3Particle_BlurBuff_t;

typedef struct {
    struct dds3Particle_BlurBuff_t *table; /* +8: kind 1 history source */
    u8 opaque0C[0xC];       /* No other kind-1 payload words are read here. */
} parKindHistoryPayload_t;

typedef struct {
    f32 scale;                      /* +8: shared by kinds 2..4 */
    void *dispatchParameter;        /* +0xC: kinds 2 and 3 */
    struct parSystem_t *drawSystem;   /* +0x10: kind 2 */
    u8 opaque14[4];                 /* Kind 2's final word is unobserved. */
} parKindCrossStripPayload_t;

typedef struct {
    f32 scale;                      /* +8: shared by kinds 2..4 */
    void *dispatchParameter;        /* +0xC: kinds 2 and 3 */
    u8 opaque10[4];                 /* Kind 3's first system word is unobserved. */
    struct parSystem_t *drawSystem;   /* +0x14: kind 3 */
} parKindCrossTrianglePayload_t;

typedef struct {
    f32 scale;                          /* +8: shared by kinds 2..4 */
    u32 firstGradientColor;             /* +0xC: kind 4 */
    u32 secondGradientColor;            /* +0x10: kind 4 */
    struct effTrackPolyList_t *modelList; /* +0x14: kind 4 */
} parKindTrackPolygonPayload_t;

typedef union {
    parKindHistoryPayload_t history;
    parKindCrossStripPayload_t crossStrip;
    parKindCrossTrianglePayload_t crossTriangle;
    parKindTrackPolygonPayload_t trackPolygon;
    u8 opaque[0x10];
} parKindPayload_t;

typedef struct dds3Particle_Effect_t {
    u16 type; /* +0: ParKindStateKind tag */
    u8 pad02[2];
    /* The halfword bitfield preserves native int-alias dependencies; see
     * dds-decomp/docs/idioms.md, "Trace-justified alias sets: emitter subrecord count". */
    u32 templateEntryCount : 16; /* 0x04: subrecords per particle in template clones */
    u32 : 16;
    parKindPayload_t payload; /* +8: tag-specific pointer/scalar view */
} dds3Particle_Effect_t;

typedef struct parDispatch_t {
    void *(*func)(void *);
    void (*update)(struct dds3Particle_Basic_t *);
    void (*destroy)(void *);
} parDispatch_t;

typedef struct effectBufferRecord_t {
    f32 position[4];
    f32 velocity[4]; /* 0x10 */
    s32 age;        /* 0x20: live frame age; negative values encode delays or initialization states. */
    u32 color;      /* 0x24: packed color passed to billboard and subeffect drawing. */
    f32 scale;      /* 0x28: current billboard scale */
    f32 spin;       /* 0x2C: current billboard rotation */
    f32 motionMagnitude;       /* 0x30 */
    u8 pad34[0x0C];
} effectBufferRecord_t;

typedef struct dds3Particle_Physics_t {
    sdfMemHandle_t *hMem;
    effectBufferRecord_t *pPhysics;
} dds3Particle_Physics_t;

typedef struct dds3ParticleColorAnim_t {
    s32 type;          /* 0x00: 1 = no ramp, 2 = color0-to-color1 only */
    u32 color0;        /* 0x04 */
    u32 color1;        /* 0x08 */
    u32 color2;        /* 0x0C */
    u32 alpha;         /* 0x10 */
    s32 alpha_s;  /* 0x14 */
    s32 alpha_e; /* 0x18 */
    s32 frame_end;        /* 0x1C */
    s32 frame_h;    /* 0x20 */
    f32 r01;           /* 0x24 */
    f32 r12;           /* 0x28 */
    f32 g01;           /* 0x2C */
    f32 g12;           /* 0x30 */
    f32 b01;           /* 0x34 */
    f32 b12;           /* 0x38 */
    u32 alpha_s_add;        /* 0x3C */
    u32 alpha_e_sub;       /* 0x40 */
} dds3ParticleColorAnim_t;
typedef struct effEmitterHead_t {
    f32 origin[4];         /* 0x00 */
    f32 scale[3];
    u8 pad1C[4];
    u32 particleCount;
    s32 lifetimeFrames;
    s32 billboardResource;
    s16 drawKind;
    u8 pad2E[2];
    dds3Particle_Effect_t sub;      /* 0x30: kind-tagged shared drawing owner */
    dds3ParticleColorAnim_t fade;   /* 0x48 */
    f32 scaleEnd;          /* 0x8C */
    f32 spinDegrees;       /* 0x90 */
    f32 speedJitter;       /* 0x94 */
    f32 spinJitter;        /* 0x98 */
    u8 pad9C[4];
    s32 templateSize;
    u32 restartStepCount;
    u8 padA8[8];
    f32 matrix[16];        /* 0xB0 */
    u32 colorMask;         /* 0xF0 */
    billHandle_t *billboard; /* 0xF4: owned billboard shared with the particle view */
    dds3Particle_Physics_t *buffer; /* 0xF8 */
    u32 pendingRestartSteps;
    f32 backupMatrix[16];
    u8 pad140[2];
    u16 active;            /* 0x142 */
    u8 pad144[0xC];
} effEmitterHead_t;

typedef struct effTemplatePacketList_t {
    effEmitterHead_t head; /* Common runtime prefix, 0x150 bytes. */
    u8 randomSphere; /* 0x150 */
    u8 restart; /* 0x151 */
    u8 pad152[2];
    union {
        s32 decayStep; /* 0x154: subtraction from each later packet tag */
        u32 delayRange; /* 0x154: bounded-radius spawn delay */
    };
    f32 recordScale; /* 0x158 */
    union {
        f32 tailValues[4]; /* 0x15C-0x168: variant-specific scaled values */
        u32 tailWords[4];
        struct {
            f32 targetRadius; /* 0x15C */
            f32 motionMagnitude; /* 0x160 */
            f32 accelerationPct; /* 0x164 */
            f32 unk168;
        };
    };
    dds3ParOffsetBuffParts_t *recordList; /* 0x16C: start of the history descriptors */
    s32 recordsPerPacket; /* 0x170 */
    sdfMemHandle_t *listAllocation; /* 0x174: allocation descriptor */
    void *auxiliaryData; /* 0x178: optional 16 bytes per packet */
    sdfMemHandle_t *auxiliaryAllocation; /* 0x17C: allocation descriptor */
} effTemplatePacketList_t;


typedef struct dds3Particle_Basic_t {
    f32 origin[4];                /* 0x00 */
    f32 billboardScale;           /* 0x10: radial packet's base scale */
    f32 billboardScaleY;          /* 0x14 */
    u8 pad18[8];
    s32 particleCount;            /* 0x20 */
    s32 lifetimeFrames;           /* 0x24 */
    s32 numberO;     /* 0x28: PAR_BILLBOARD_CLONE_FROM_RESOURCE */
    s16 billboardMode;            /* 0x2C */
    u8 pad2E[2];
    dds3Particle_Effect_t kindState;       /* 0x30 */
    dds3ParticleColorAnim_t colorRamp;       /* 0x48 */
    f32 scaleEnd;                /* 0x8C: rendered scale after interpolation. */
    f32 spinDegrees;             /* 0x90: converted to radians by the client update. */
    f32 billboardScaleJitter;     /* 0x94 */
    f32 initialPhaseJitter;       /* 0x98 */
    u8 alternateSpin;            /* 0x9C: selects the sign of particle rotation. */
    u8 pad9D[7];
    u32 d_frame;         /* 0xA4 */
    u8 padA8[8];
    f32 rot[16];               /* 0xB0 */
    u32 abgr;           /* 0xF0: set through dds3ParticleSetRGBA. */
    billHandle_t *hBillboard;           /* 0xF4 */
    dds3Particle_Physics_t *buffer;     /* 0xF8 */
    u32 pendingRestartSteps;      /* 0xFC: count, not a pointer */
    f32 rot_b[16];         /* 0x100 */
    u16 type;            /* 0x140: object dispatch, distinct from kind */
    u16 state;              /* 0x142 */
    u8 pad144[0xC];
} dds3Particle_Basic_t;

typedef struct parObjClientView_t {
    dds3Particle_Basic_t owner;
    u8 mode150; /* 0x150 */
    u8 mode151; /* 0x151 */
    u8 opaque152[0x26];
} parObjClientView_t;

typedef struct parKindResource_t {
    s32 type;
    s32 offset;
    u8 opaque08[8];
} parKindResource_t;

typedef struct dds3ParOffsetBuffParts_t {
    s32 point;
    s32 count;
    f32 (*points)[3];
} dds3ParOffsetBuffParts_t;

typedef struct dds3ParOffsetBuff_t {
    dds3ParOffsetBuffParts_t *pParts;
    s32 length;
} dds3ParOffsetBuff_t;

typedef struct {
    dds3Particle_Basic_t head;
    u8 axisMode;                 /* 0x150 */
    u8 loop;                     /* 0x151: rearm expired particles */
    u8 pad152[2];
    u32 spawnDelayFrames;        /* 0x154 */
    f32 initialRadius;           /* 0x158 */
    f32 rotationStepDegrees;     /* 0x15C: native updater converts to radians */
    f32 targetRadius;
    f32 initialRadiusJitter;
    f32 targetRadiusJitter;
    dds3ParOffsetBuff_t history;
    sdfMemHandle_t *childHandle;     /* 0x174: additional owned allocation */
    f32 previousOrigin[3];       /* 0x178: native updater caches origin here */
} dds3Particle_SphereSpiral2_t3;

typedef struct {
    f32 position[4];
    f32 radialOffset[3];
    u8 pad1C[4];
    s32 age;
    u32 color;
    f32 billboardScale;
    f32 initialPhaseRadians; /* initialized as a full-turn phase; later use is not established */
    f32 rotationAxis[3];
    f32 radiusStep;
} parBurstPacket_t;

typedef struct effParticleRecord_t {
    f32 x;
    f32 y;
    f32 z;
    u8 opaque0C[0x14];
    s32 frame;
    u32 color;
    f32 speed;
    f32 angle;
    u8 opaque30[0x10];
} effParticleRecord_t;

typedef struct effParticleOverlay_t {
    f32 x;
    f32 y;
    f32 z;
    u8 pad0C[4];
    f32 speed;
    u8 pad14[0x10];
    u32 drawEndFrame;
    u8 pad28[0x24];
    u32 color;
    u8 pad50[8];
    u32 alpha;
    u8 pad5C[0x30];
    f32 lastSpeed;
    u8 pad90[4];
    f32 speedJitter;
    f32 angleJitter;
    u8 pad9C[0x54];
    u32 colorMask;
    u8 padF4[4];
    dds3Particle_Physics_t *buffer;
} effParticleOverlay_t;

typedef struct effTemplateScaleRecord_t {
    u8 opaque00[0x20];
    u32 initialTag;
    u8 opaque24[0x10];
    f32 scale;
    u8 opaque38[8];
} effTemplateScaleRecord_t;

typedef struct dds3Particle_Radiate_t3 {
    effEmitterHead_t head; /* shared emitter prefix, 0x150 bytes */
    u8 mode;             /* 0x150 */
    u8 loop;             /* 0x151 */
    u8 opaque152[10];
    u32 period;          /* 0x15C */
    f32 rotationStepDegrees;            /* 0x160 */
    f32 stepLength;            /* 0x164 */
} dds3Particle_Radiate_t3;

/* Each emitter uses a 64-byte packet, with its own four-word motion state. */
typedef struct {
    f32 pos[4];
    f32 vel[4];
    s32 age;
    u32 color;
    f32 billboardScale;
    f32 spin;
    f32 phase;
    f32 radialDistance;
    f32 verticalSpeed;
    f32 verticalOffset;
} parSpiralPacket_t;

typedef struct {
    f32 pos[4];
    f32 vel[4];
    s32 age;
    u32 color;
    f32 billboardScale;
    f32 spin;
    f32 waveAmplitude;
    f32 wavePhase;
    f32 previousWaveValue;
    f32 verticalVelocity;
} parSmokePacket_t;

typedef struct {
    f32 pos[4];
    f32 vel[4];
    s32 age;
    u32 color;
    f32 billboardScale;
    f32 spin;
    u8 unk30[8];
    f32 verticalAcceleration;
    u8 unk3C[4];
} parSparkPacket_t;

typedef struct {
    f32 pos[4];
    f32 vel[4];
    s32 age;
    u32 color;
    f32 billboardScale;
    f32 spin;
    u8 unk30[4];
    f32 phaseAngle;
    u8 unk38[8];
} parRadiatePacket_t;

typedef struct {
    f32 pos[4];
    f32 vel[4];
    s32 age;
    u32 color;
    f32 billboardScale;
    f32 spin;
    f32 axisX;
    f32 axisY;
    f32 axisZ;
    f32 radiusStep;
} parSphereSpiralPacket_t;

typedef struct {
    f32 pos[4];
    f32 vel[4];
    s32 age;
    u32 color;
    f32 billboardScale;
    f32 spin;
    f32 radius;
    u8 unk34[12];
} parSphereRingPacket_t;

typedef struct {
    f32 pos[4];
    f32 vel[4];
    s32 age;
    u32 color;
    f32 billboardScale;
    f32 spin;
    f32 motionMagnitude;
    f32 boundaryRadius;
    u8 unk38[8];
} parSphereAtomPacket_t;

typedef struct {
    f32 pos[4];
    f32 vel[4];
    s32 age;
    u32 color;
    f32 billboardScale;
    f32 spin;
    f32 offsetX;
    f32 offsetY;
    f32 offsetZ;
    f32 offsetAcceleration;
} parSparkOffsetPacket_t;

typedef struct dds3Particle_Spiral_t3 {
    effEmitterHead_t head;
    u8 flag150;
    u8 loop;
    u8 timing[6];
    f32 radius;      /* 0x158 */
    u32 period;      /* 0x15C */
    f32 verticalSpeed;
    f32 rotationStepDegrees;
    f32 targetRadius;
    f32 verticalAccelerationPct;
    f32 radiusRange; /* 0x170 */
    f32 targetRadiusJitter;  /* 0x174 */
} dds3Particle_Spiral_t3;

typedef struct dds3Particle_Smoke_t3 {
    effEmitterHead_t head;
    u8 loop;
    u8 pad151[3];
    u32 spread;      /* 0x154 */
    f32 radius;      /* 0x158 */
    f32 verticalSpeed;
    f32 phaseStepDegrees;
    f32 waveAmplitude;
    f32 waveGrowth;
    f32 verticalAccelerationPct;
    f32 verticalSpeedJitter;     /* 0x170 */
    f32 waveAmplitudeJitter;     /* 0x174 */
} dds3Particle_Smoke_t3;

typedef struct dds3Particle_Spark_t3 {
    effEmitterHead_t head;
    u8 mode;         /* 0x150 */
    u8 loop;         /* 0x151 */
    u8 pad152[2];
    u32 spread;      /* 0x154 */
    u8 randomDir;    /* 0x158 */
    u8 pad159[3];
    f32 radius;      /* 0x15C */
    f32 cone;        /* 0x160 */
    f32 gravity;     /* 0x164 */
    f32 speed;       /* 0x168 */
    f32 decayPct;    /* 0x16C */
    f32 jitter;      /* 0x170 */
} dds3Particle_Spark_t3;

typedef struct dds3Particle_SphereSpiral_t3 {
    effEmitterHead_t head;
    u8 mode;         /* 0x150 */
    u8 loop;         /* 0x151 */
    u8 pad152[2];
    u32 spread;      /* 0x154 */
    f32 speed;       /* 0x158 */
    f32 spinRate;    /* 0x15C */
    f32 targetRadius;
    f32 jitterA;     /* 0x164 */
    f32 jitterB;     /* 0x168 */
} dds3Particle_SphereSpiral_t3;

typedef struct dds3Particle_SphereSmoke_t3 {
    effEmitterHead_t head;
    u8 loop;
    u8 pad151[3];
    u32 spread;      /* 0x154 */
    f32 radius;      /* 0x158 */
    f32 up_s;
    f32 phaseStepDegrees;
    f32 amplitude;
    f32 waveGrowth;
    f32 verticalAccelerationPct;
    f32 up_s_r;
    f32 ampli_r;
    f32 radius_s_r;
} dds3Particle_SphereSmoke_t3;

typedef struct dds3Particle_SphereRing_t3 {
    effEmitterHead_t head;
    u8 pad150;
    u8 loop;         /* 0x151 */
    u8 timing[6];
    f32 radius;      /* 0x158 */
    f32 radius_r;      /* 0x15C */
    u32 simu_num;      /* 0x160 */
    f32 roll_s;    /* 0x164 */
} dds3Particle_SphereRing_t3;

typedef struct dds3Particle_Spark2_t3 {
    effEmitterHead_t basic;
    u8 mode;         /* 0x150 */
    u8 loop;         /* 0x151 */
    u8 pad152[2];
    u32 spread;      /* 0x154 */
    u8 pad158[4];
    f32 radius;      /* 0x15C */
    f32 cone;        /* 0x160 */
    f32 gravity;     /* 0x164 */
    f32 speed;       /* 0x168 */
    f32 decayPct;    /* 0x16C */
    u8 pad170[4];
    f32 degrees;     /* 0x174 */
} dds3Particle_Spark2_t3;

typedef struct dds3Particle_SparkOffset_t3 {
    effEmitterHead_t head;
    u8 mode;         /* 0x150 */
    u8 loop;         /* 0x151 */
    u8 pad152[2];
    u32 spread;      /* 0x154: random activation delay bound */
    u8 randomDir;    /* 0x158 */
    u8 pad159[3];
    f32 radius;      /* 0x15C */
    f32 cone;        /* 0x160: horizontal direction weight */
    f32 gravity;     /* 0x164: divided by 100 for acceleration */
    f32 speed;       /* 0x168: launch magnitude */
    f32 decayPct;    /* 0x16C */
    f32 jitter;      /* 0x170: launch magnitude jitter */
} dds3Particle_SparkOffset_t3;

typedef struct dds3Particle_SmokeOffset_t3 {
    effEmitterHead_t head;
    u8 loop;
    u8 flag151;
    u8 pad152[2];
    u32 spread;      /* 0x154 */
    f32 radius;      /* 0x158 */
    f32 verticalSpeed;
    f32 phaseStepDegrees;
    f32 waveAmplitude;
    f32 waveGrowth;
    f32 verticalAccelerationPct;
    f32 verticalSpeedJitter;
    f32 waveAmplitudeJitter;
    f32 (*aux)[4];   /* 0x178: 16 bytes per packet */
} dds3Particle_SmokeOffset_t3;


enum ParKindStateKind {
    PAR_KIND_STATE_HISTORY = 1,
    PAR_KIND_STATE_BILLBOARD_CROSS_STRIP = 2,
    PAR_KIND_STATE_BILLBOARD_CROSS_TRIANGLE = 3,
    PAR_KIND_STATE_TRACK_POLYGON_CROSS_AXES = 4
};

enum { PAR_BILLBOARD_CLONE_FROM_RESOURCE = -1 };

enum {
    PAR_BURST_RANDOM_AXIS = 1
};

#define EFF_TEMPLATE_TAIL_OFFSET 0x150
#define EFF_PACKET_INITIAL_TAG 0xF0000001
#define PAR_PACKET_INITIAL_TAG 0xF0000001

extern f32 parTrack_w[];
extern f32 parLook[];
extern u8 aViewTarget[], aViewEye[];
extern parDispatch_t dds3ParticleFuncTable[];
extern effRandState_t dds3EffectBase[];
extern s32 ParRand[];
extern f32 dds3ParticleSparkDirTable[][4];
extern f32 dds3ParticleSparkOffsetDirTable[][4];
extern f32 dds3ParticleNegOneVec[4];
extern u8 aViewDown[];
extern void (*dds3ParticleAllBuffInitTable[])(void *);
extern void (*dds3ParticleSetScaleTable[])(dds3Particle_Basic_t *, f32);

extern void *memset(void *a, s32 b, size_t c);
extern void *memcpy(void *a, const void *b, size_t c);

void dds3ParticleManagerInit(void);
dds3Particle_Physics_t *dds3Particle_PhysicsCreate(s32 number);
void dds3Particle_PhysicsDelete(dds3Particle_Physics_t *hPhysics);
void dds3Particle_BasicCreate(void *effect);
void dds3Particle_BasicDelete(void *pParBasic);
void dds3Particle_SpiralAllBuffInit(effTemplatePacketList_t *hData);
void dds3Particle_SpiralSetScale(effTemplatePacketList_t *hData, float scale);
effTemplatePacketList_t *dds3Particle_SpiralCreate(effTemplatePacketList_t *source);
void dds3Particle_SpiralDelete(void *hData);
void dds3Particle_SpiralBuffInit(dds3Particle_Spiral_t3 *pSpiral, u32 number);
void dds3Particle_SpiralDraw(dds3Particle_Spiral_t3 *hData);
void dds3Particle_SmokeAllBuffInit(effTemplatePacketList_t *hData);
void dds3Particle_SmokeSetScale(effTemplatePacketList_t *hData, float scale);
effTemplatePacketList_t *dds3Particle_SmokeCreate(effTemplatePacketList_t *source);
void dds3Particle_SmokeDelete(void *hData);
void dds3Particle_SmokeBuffInit(dds3Particle_Smoke_t3 *pSmoke, u32 number);
void dds3Particle_SmokeDraw(dds3Particle_Smoke_t3 *hData);
void dds3Particle_SparkAllBuffInit(effTemplatePacketList_t *hData);
void dds3Particle_SparkSetScale(effTemplatePacketList_t *hData, float scale);
effTemplatePacketList_t *dds3Particle_SparkCreate(effTemplatePacketList_t *source);
void dds3Particle_SparkDelete(void *hData);
void dds3Particle_SparkBuffInit(dds3Particle_Spark_t3 *pSpark, s32 number);
void dds3Particle_SparkDraw(dds3Particle_Spark_t3 *hData);
void dds3Particle_RadiateAllBuffInit(effTemplatePacketList_t *hData);
void dds3Particle_RadiateSetScale(effTemplatePacketList_t *hData, float scale);
effTemplatePacketList_t *dds3Particle_RadiateCreate(effTemplatePacketList_t *source);
void dds3Particle_RadiateDelete(void *hData);
void dds3Particle_RadiateBuffInit(dds3Particle_Radiate_t3 *pRadiate, u32 number);
void dds3Particle_RadiateDraw(dds3Particle_Radiate_t3 *hData);
void dds3Particle_SphereSpiralAllBuffInit(effTemplatePacketList_t *hData);
void dds3Particle_SphereSpiralSetScale(effTemplatePacketList_t *hData, float scale);
effTemplatePacketList_t *dds3Particle_SphereSpiralCreate(effTemplatePacketList_t *source);
void dds3Particle_SphereSpiralDelete(void *hData);
void dds3Particle_SphereSpiralBuffInit(dds3Particle_SphereSpiral_t3 *pSphereSpiral, u32 number);
void dds3Particle_SphereSpiralDraw(dds3Particle_SphereSpiral_t3 *hData);
void dds3Particle_SphereSmokeAllBuffInit(effTemplatePacketList_t *hData);
void dds3Particle_SphereSmokeSetScale(effTemplatePacketList_t *hData, float scale);
effTemplatePacketList_t *dds3Particle_SphereSmokeCreate(effTemplatePacketList_t *source);
void dds3Particle_SphereSmokeDelete(void *hData);
void dds3Particle_SphereSmokeBuffInit(dds3Particle_SphereSmoke_t3 *pSphereSmoke, u32 number);
void dds3Particle_SphereSmokeDraw(dds3Particle_SphereSmoke_t3 *hData);
void dds3Particle_SphereRingAllBuffInit(effTemplatePacketList_t *hData);
void dds3Particle_SphereRingSetScale(effTemplatePacketList_t *hData, float scale);
effTemplatePacketList_t *dds3Particle_SphereRingCreate(effTemplatePacketList_t *source);
void dds3Particle_SphereRingDelete(void *hData);
void dds3Particle_SphereRingBuffInit(dds3Particle_SphereRing_t3 *pSphereRing, u32 number);
void dds3Particle_SphereRingDraw(dds3Particle_SphereRing_t3 *hData);
void dds3Particle_Spark2AllBuffInit(effTemplatePacketList_t *hData);
void dds3Particle_Spark2SetScale(effTemplatePacketList_t *hData, float scale);
effTemplatePacketList_t *dds3Particle_Spark2Create(effTemplatePacketList_t *source);
void dds3Particle_Spark2Delete(void *hData);
void dds3Particle_Spark2BuffInit(dds3Particle_Spark2_t3 *pSpark2, s32 number);
void dds3Particle_Spark2Draw(dds3Particle_Spark2_t3 *hData);
void dds3Particle_SphereAtomAllBuffInit(effTemplatePacketList_t *hData);
void dds3Particle_SphereAtomSetScale(effTemplatePacketList_t *hData, float scale);
effTemplatePacketList_t *dds3Particle_SphereAtomCreate(effTemplatePacketList_t *source);
void dds3Particle_SphereAtomDelete(void *hData);
void dds3Particle_SphereAtomBuffInit(effTemplatePacketList_t *pSphereAtom, s32 number);
void dds3Particle_SphereAtomDraw(effTemplatePacketList_t *hData);
void dds3Particle_SpiralBlendColor(effParticleOverlay_t *effect);
void dds3Particle_SparkOffsetAllBuffInit(effTemplatePacketList_t *hData);
void dds3Particle_SparkOffsetSetScale(effTemplatePacketList_t *hData, float scale);
effTemplatePacketList_t *dds3Particle_SparkOffsetCreate(effTemplatePacketList_t *source);
void dds3Particle_SparkOffsetDelete(void *hData);
void dds3Particle_SparkOffsetBuffInit(dds3Particle_SparkOffset_t3 *pSparkOffset, s32 number);
void dds3Particle_SparkOffsetDraw(dds3Particle_SparkOffset_t3 *hData);
void dds3Particle_SmokeOffsetAllBuffInit(effTemplatePacketList_t *hData);
void dds3Particle_SmokeOffsetSetScale(effTemplatePacketList_t *hData, float scale);
effTemplatePacketList_t *dds3Particle_SmokeOffsetCreate(effTemplatePacketList_t *source);
void dds3Particle_SmokeOffsetDelete(effTemplatePacketList_t *hData);
void dds3Particle_SmokeOffsetBuffInit(dds3Particle_SmokeOffset_t3 *pSmokeOffset, u32 number);
void dds3Particle_SmokeOffsetDraw(dds3Particle_SmokeOffset_t3 *hData);
void dds3Particle_SphereSpiral2AllBuffInit(effTemplatePacketList_t *hData);
void dds3Particle_SphereSpiral2SetScale(effTemplatePacketList_t *hData, float scale);
effTemplatePacketList_t *dds3Particle_SphereSpiral2Create(effTemplatePacketList_t *source);
void dds3Particle_SphereSpiral2Delete(dds3Particle_SphereSpiral2_t3 *hData);
void dds3Particle_SphereSpiral2BuffInit(dds3Particle_SphereSpiral2_t3 *pSphereSpiral2, u32 number);
void dds3Particle_SphereSpiral2Draw(dds3Particle_SphereSpiral2_t3 *hData);
void dds3ParticleSetColorTable(dds3ParticleColorAnim_t *pColAnim, s32 frame);
dds3Particle_BlurBuff_t *dds3Particle_BlurCreate(u32 number, u16 length, billHandle_t **pBill_Handle);
void dds3Particle_BlurDelete(dds3Particle_BlurBuff_t *hBlur);
void dds3Particle_BlurSet(dds3Particle_BlurBuff_t *table, s32 slotIndex, u32 color, f32 billboardScale);
void dds3Particle_BlurSetExp(dds3Particle_BlurBuff_t *hBlur, s32 index, const f32 *pPosBase, u32 abgr,
                   dds3ParOffsetBuff_t *pOffBuff, f32 scale);
void dds3Particle_BlurDraw(dds3Particle_BlurBuff_t *hBlur);
void dds3Particle_BlurClearItem(dds3Particle_BlurBuff_t *table, s32 slotIndex);
void dds3Particle_TrackSetExp(parSystem_t *hTrackPoly, s32 index, const f32 *pPos, u32 abgr, dds3ParOffsetBuff_t *pOffBuff);
void dds3Particle_Track2SetExp(parSystem_t *hTrackPoly, s32 index, const f32 *pPos, u32 abgr, dds3ParOffsetBuff_t *pOffBuff);
u32 dds3ParticleColorBlend(u32 color0, u32 color1);
dds3Particle_Basic_t *dds3ParticleCreate(s32 dispatchIndex, void *creationData);
void dds3ParticleDelete(dds3Particle_Basic_t *hData);
void dds3ParticleDraw(dds3Particle_Basic_t *hData);
dds3Particle_Basic_t *dds3ParticleCopy(dds3Particle_Basic_t *obj);
void dds3ParticleReset(dds3Particle_Basic_t *hData);
void dds3ParticleSetScale(dds3Particle_Basic_t *hData, f32 scale);
u16 dds3ParticleGetState(dds3Particle_Basic_t *hData);
void dds3ParticleSetPosition(void *hData, void *pPos);
void dds3ParticleSetRotateMatrix(dds3Particle_Basic_t *hData, void *pMatrix);
void dds3ParticleSetRGBA(dds3Particle_Basic_t *hData, u32 abgr);
void dds3ParticleSetTypeFlag(dds3Particle_Basic_t *object, s32 mode);
u8 dds3ParticleGetLoop(dds3Particle_Basic_t *hData);
dds3Particle_Basic_t *dds3ParticleExpCreate(s32 kind, parKindResource_t *resource);
dds3Particle_Basic_t *dds3ParticleExpCopy(dds3Particle_Basic_t *source);
void dds3ParticleExpDelete(dds3Particle_Basic_t *hData);
void dds3ParticleExpDraw(dds3Particle_Basic_t *hData);
void dds3ParticleExpReset(dds3Particle_Basic_t *hData);
void dds3ParticleExpSetScale(dds3Particle_Basic_t *hData, float scale);
u16 dds3ParticleExpGetState(dds3Particle_Basic_t *hData);
void dds3ParticleExpSetPosition(void *hData, void *pPos);
void dds3ParticleExpSetRotateMatrix(dds3Particle_Basic_t *hData, void *pMatrix);
void dds3ParticleExpSetRGBA(dds3Particle_Basic_t *hData, u32 abgr);
void dds3ParticleExpSetLoop(dds3Particle_Basic_t *hData, u8 loop);
u8 dds3ParticleExpGetLoop(dds3Particle_Basic_t *hData);
void dds3Particle_EffectInit(dds3Particle_Effect_t *obj);
void dds3Particle_EffectSet(dds3Particle_Effect_t *pEffect, s32 index, u32 abgr, f32 scale);
void dds3Particle_EffectReset(dds3Particle_Effect_t *pEffect, s32 number);
void dds3Particle_TrackSet(parSystem_t *hTrackPoly, s32 i, u32 abgr);
void dds3Particle_Track2Set(parSystem_t *hTrackPoly, s32 i, u32 abgr);
void dds3Particle_Track3Set(effTrackPolyList_t *hTrackPoly, s32 i, u32 abgr);
u32 dds3Particle_TrackPolyGetRecTiming();
void sdfRandomizeByTime(effRandState_t *aCB);
sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);
void *sdfMemLock(sdfMemHandle_t *aHandle);
void sdfMemFreeDirect(sdfMemHandle_t *aHandle);
billHandle_t *dds3BillboardGeneral(s32 number);
void dds3BillboardSetScale(billHandle_t *hBillboard, float x, float y);
void dds3BillboardSetMode(billHandle_t *hBillboard, s32 mode);
parSystem_t *dds3TrackPolyCreate(s32 number, s32 face, s32 rec, u32 type);
void dds3TrackPolyVCSet(void *hTrackPoly, s32 type, void *abgr_in, void *abgr_out);
void dds3TrackPolySetMode(parSystem_t *hTrackPoly, u16 mode);
effTrackPolyList_t *Eff_ModelTrackPolyListCreate(effTrackPolyParams_t *pParam, u32 number);
void dds3TrackPolyDelete(parSystem_t *hTrackPoly);
void Eff_ModelTrackPolyListDelete(effTrackPolyList_t *list);
void dds3BillboardDelete(struct billHandle_t *hData);
void *sdfChipAllocDirect(s32 aSize);
void sdfChipFreeDirect(void *aPtr);
f32 dds3GetRandFloatA(effRandState_t *name);
f32 sdfCos(f32 aRadius);
f32 sdfSin(f32 aRadius);
u32 dds3ParticleColorAnim(dds3ParticleColorAnim_t *pColAnim, u32 color_now, s32 frame);
u32 dds3GetRandA(effRandState_t *name);
void LookAtMatrix(const void *aTarget, const void *aEye, const void *aDown);
void ReverseMatrix(void);
void SetRotateMatrixAxis(f32 aAngle, const sdfVector3_t *aAxis);
extern void dds3BillboardDraw(billHandle_t *hData);
void dds3TrackPolyVCRefresh_ParType1(parSystem_t *hTrackPoly, s32 index);
extern void dds3TrackPolyColorSet(parSystem_t *hTrackPoly, s32 number, u32 color);
void dds3TrackPolyVCRefresh_ParType2(parSystem_t *hTrackPoly, s32 index);
void dds3TrackPolyDraw(parSystem_t *node);
void Eff_ModelTrackPolyListDrawOnly(effTrackPolyList_t *hMTPolyList);
void MultMatrixNoArg(void);
struct billHandle_t *dds3BillboardCreate(s32 kind, addr_t data);
void dds3BillboardSetAnimColorAdvMode(billHandle_t *hBillboard);
struct billHandle_t *dds3BillboardCopy(const struct billHandle_t *hBillboard);
void dds3BillboardSetAnimKey(billHandle_t *hBillboard, u32 frame);
void dds3TrackPolyListInit(parSystem_t *hTrackPoly, s32 number);
void Eff_ModelTrackPolyListReset(effTrackPolyList_t *hMTPolyList, s32 index);
void dds3TrackPolySet(parSystem_t *hTrackPoly, s32 number, const u128 *pV);
void dds3TrackPolySet2(parSystem_t *hTrackPoly, s32 number, const u128 *pV);
void Eff_ModelTrackPolyListVertexSet(effTrackPolyList_t *hMTPolyList, s32 index, const u128 *pVertex);
void Eff_ModelTrackPolyListColorSet(effTrackPolyList_t *hMTPolyList, s32 index, u32 abgr);
void effInitParticleRecord(effParticleOverlay_t *effect);

/* Both draw paths submit the resource owned by the selected particle kind. */
static __inline__ void SubmitKindDrawing(dds3Particle_Effect_t *kind) {
    switch (kind->type) {
    case PAR_KIND_STATE_HISTORY:
        dds3Particle_BlurDraw(kind->payload.history.table);
        break;
    case PAR_KIND_STATE_BILLBOARD_CROSS_STRIP:
        dds3TrackPolyDraw(kind->payload.crossStrip.drawSystem);
        break;
    case PAR_KIND_STATE_BILLBOARD_CROSS_TRIANGLE:
        dds3TrackPolyDraw(kind->payload.crossTriangle.drawSystem);
        break;
    case PAR_KIND_STATE_TRACK_POLYGON_CROSS_AXES:
        Eff_ModelTrackPolyListDrawOnly(kind->payload.trackPolygon.modelList);
        break;
    }
}

/* Seed the generator used for per-particle launch delays. */
void dds3ParticleManagerInit(void) {
    sdfRandomizeByTime((effRandState_t *)&ParRand);
}

void dds3ParticleManagerExit(void) {
}

/* Allocate records followed by their owner tail; clear only two native state words per record. */
dds3Particle_Physics_t *dds3Particle_PhysicsCreate(s32 number) {
    s32 recordBytes = number * sizeof(effectBufferRecord_t);
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(recordBytes + sizeof(dds3Particle_Physics_t));
    effectBufferRecord_t *recordCursor = (void *)sdfMemLock(allocationHandle);
    dds3Particle_Physics_t *bufferTail = (dds3Particle_Physics_t *)((u8 *)recordCursor + recordBytes);

    bufferTail->hMem = allocationHandle;
    bufferTail->pPhysics = recordCursor;
    if (number > 0) {
        s32 remaining = number;
        do {
            remaining--;
            recordCursor->age = 0;
            recordCursor->color = 0;
            recordCursor++;
        } while (remaining != 0);
    }
    return bufferTail;
}

void dds3Particle_PhysicsDelete(dds3Particle_Physics_t *hPhysics) {
    sdfMemFreeDirect(hPhysics->hMem);
}

/* Allocate drawing resources and reset the emitter's common runtime state. */
/* The subrecord bitfield keeps native int-alias dependencies; see
 * dds-decomp/docs/idioms.md, "Trace-justified alias sets: emitter subrecord count". */
void dds3Particle_BasicCreate(void *effect) {
    effEmitterHead_t *owner = effect;
    effTrackPolyParams_t params;

    owner->templateSize = 0x150;
    owner->buffer = dds3Particle_PhysicsCreate(owner->particleCount);
    if (owner->billboardResource != -1) {
        owner->billboard = dds3BillboardGeneral(owner->billboardResource);
        dds3BillboardSetScale(owner->billboard, owner->scale[0], owner->scale[1]);
        dds3BillboardSetMode(owner->billboard, owner->drawKind);
    }
    dds3ParticleSetColorTable(&owner->fade, owner->lifetimeFrames);
    switch (owner->sub.type) {
    case PAR_KIND_STATE_HISTORY:
        owner->sub.payload.history.table = dds3Particle_BlurCreate(owner->particleCount,
                                             owner->sub.templateEntryCount,
                                             &owner->billboard);
        break;
    case PAR_KIND_STATE_BILLBOARD_CROSS_STRIP:
        owner->sub.payload.crossStrip.drawSystem = dds3TrackPolyCreate(
            owner->particleCount, owner->sub.templateEntryCount,
            dds3Particle_TrackPolyGetRecTiming(owner->sub.templateEntryCount), 0);
        dds3TrackPolyVCSet(owner->sub.payload.crossStrip.drawSystem, 1,
                       owner->sub.payload.crossStrip.dispatchParameter, owner->sub.payload.crossStrip.dispatchParameter);
        dds3TrackPolySetMode(owner->sub.payload.crossStrip.drawSystem, (u16)owner->drawKind);
        break;
    case PAR_KIND_STATE_BILLBOARD_CROSS_TRIANGLE:
        owner->sub.payload.crossTriangle.drawSystem = dds3TrackPolyCreate(
            owner->particleCount, owner->sub.templateEntryCount,
            dds3Particle_TrackPolyGetRecTiming(owner->sub.templateEntryCount), 1);
        dds3TrackPolyVCSet(owner->sub.payload.crossTriangle.drawSystem, 1,
                       owner->sub.payload.crossTriangle.dispatchParameter, owner->sub.payload.crossStrip.drawSystem);
        dds3TrackPolySetMode(owner->sub.payload.crossTriangle.drawSystem, (u16)owner->drawKind);
        break;
    case PAR_KIND_STATE_TRACK_POLYGON_CROSS_AXES:
        /* These tracks receive endpoints directly; model sampling is unused. */
        params.sampleInterval = 1;
        params.historyLength = owner->sub.templateEntryCount;
        params.unk1C = 10;
        params.kind = (u16)owner->drawKind;
        params.gradientColors[0] = owner->sub.payload.trackPolygon.firstGradientColor;
        params.gradientColors[1] = owner->sub.payload.trackPolygon.firstGradientColor;
        params.gradientColors[2] = owner->sub.payload.trackPolygon.secondGradientColor;
        params.gradientColors[3] = owner->sub.payload.trackPolygon.secondGradientColor;
        owner->sub.payload.trackPolygon.modelList =
            Eff_ModelTrackPolyListCreate(&params, owner->particleCount);
        break;
    }
    owner->pendingRestartSteps = SDF_FRAMES(owner->restartStepCount);
    SDF_VU0_LOADMATRIX(owner->matrix);
    SDF_VU0_STOREMATRIX(owner->backupMatrix);
    owner->active = 1;
    owner->colorMask = 0x80808080;
}

/* Release the selected sub-effect resource, billboard and packet-buffer
 * allocation described by the common emitter prefix. */
void dds3Particle_BasicDelete(void *pParBasic)
{
    effEmitterHead_t *owner = (effEmitterHead_t *)pParBasic;
    switch (owner->sub.type) {
    case PAR_KIND_STATE_HISTORY:
        dds3Particle_BlurDelete(owner->sub.payload.history.table);
        break;
    case PAR_KIND_STATE_BILLBOARD_CROSS_STRIP:
        dds3TrackPolyDelete(owner->sub.payload.crossStrip.drawSystem);
        break;
    case PAR_KIND_STATE_BILLBOARD_CROSS_TRIANGLE:
        dds3TrackPolyDelete(owner->sub.payload.crossTriangle.drawSystem);
        break;
    case PAR_KIND_STATE_TRACK_POLYGON_CROSS_AXES:
        Eff_ModelTrackPolyListDelete(owner->sub.payload.trackPolygon.modelList);
        break;
    }
    dds3BillboardDelete(owner->billboard);
    dds3Particle_PhysicsDelete(owner->buffer);
}

/* Stagger native tags in period-sized groups; a nonempty buffer requires a nonzero period. */
void dds3Particle_SpiralAllBuffInit(effTemplatePacketList_t *hData) {
    effectBufferRecord_t *packetCursor;
    u32 packetIndex;
    u32 nextPacketIndex;
    s32 packetTag;

    packetTag = EFF_PACKET_INITIAL_TAG;
    packetIndex = 0;
    packetCursor = hData->head.buffer->pPhysics;
    if (hData->head.particleCount != 0) {
        do {
            packetCursor->age = packetTag;
            nextPacketIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
            if (nextPacketIndex % hData->tailWords[0] == 0) {
                packetTag = packetTag - SDF_FRAMES(hData->decayStep);
            }
            packetIndex = nextPacketIndex;
        } while (packetIndex < hData->head.particleCount);
    }
}

/* Scale the position, record scale, and this variant's two tail floats. */
void dds3Particle_SpiralSetScale(effTemplatePacketList_t *hData, float scale) {
    hData->head.scale[0] = hData->head.scale[0] * scale;
    hData->head.scale[1] = hData->head.scale[1] * scale;
    hData->head.scale[2] = hData->head.scale[2] * scale;
    hData->recordScale = hData->recordScale * scale;
    hData->tailValues[1] = hData->tailValues[1] * scale;
    hData->tailValues[3] = hData->tailValues[3] * scale;
}

/* Clone the template prefix and appended tail, then allocate its packet records. */
effTemplatePacketList_t *dds3Particle_SpiralCreate(effTemplatePacketList_t *source) {
    effTemplatePacketList_t *clone = sdfChipAllocDirect(0x180);
    s32 tailBytes = 0x30;

    memset(clone, 0, 0x180);
    memcpy(clone, source, source->head.templateSize);
    memcpy((u8 *)clone + EFF_TEMPLATE_TAIL_OFFSET, (u8 *)source + source->head.templateSize, tailBytes);
    dds3Particle_BasicCreate(clone);
    dds3Particle_SpiralAllBuffInit(clone);
    return clone;
}

void dds3Particle_SpiralDelete(void *hData) {
    dds3Particle_BasicDelete(hData);
    sdfChipFreeDirect(hData);
}

/* Seed one ring packet's phase, radius and vertical motion, then its subeffect. */
void dds3Particle_SpiralBuffInit(dds3Particle_Spiral_t3 *pSpiral, u32 number) {
    parSpiralPacket_t *packet = (parSpiralPacket_t *)pSpiral->head.buffer->pPhysics;
    u32 phaseIndex;
    f32 phase;
    f32 jitter;

    packet += number;
    packet->color = 0;
    packet->age = 0;
    phase = 0.0f;
    phaseIndex = number % pSpiral->period;
    if (phaseIndex != 0) {
        phase = (3.14159265f * 2.0f) / pSpiral->period * phaseIndex;
    }
    packet->phase = phase;
    jitter = pSpiral->radiusRange;
    packet->radialDistance = pSpiral->radius * (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter));
    jitter = pSpiral->targetRadiusJitter;
    packet->vel[0] = pSpiral->targetRadius * (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter));
    packet->verticalSpeed = pSpiral->verticalSpeed;
    packet->pos[0] = sdfCos(phase) * pSpiral->radius;
    packet->pos[1] = 0;
    packet->pos[2] = sdfSin(phase) * pSpiral->radius;
    SDF_VU0_LOADMATRIX(pSpiral->head.matrix);
    SDF_VU0_LOADVECTOR(packet->pos);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(packet->pos);
    packet->verticalOffset = 0;
    packet->pos[0] += pSpiral->head.origin[0];
    packet->pos[1] += pSpiral->head.origin[1];
    packet->pos[2] += pSpiral->head.origin[2];
    jitter = pSpiral->head.speedJitter;
    packet->billboardScale = pSpiral->head.scale[0] * (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter));
    jitter = pSpiral->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    dds3Particle_EffectReset(&pSpiral->head.sub, number);
}

/* Advance ring motion and ages; repeat by tagging packets for the next update.
 * completedCount counts expired nonrepeating packets in this update only. */
/* vf12 keeps the packet's previous position and vf10 the new one for
 * dds3Particle_EffectSet, which reads them as implicit arguments. */
void dds3Particle_SpiralDraw(dds3Particle_Spiral_t3 *hData) {
    parSpiralPacket_t *packet = (parSpiralPacket_t *)hData->head.buffer->pPhysics;
    u32 subeffectKind = hData->head.sub.type;
    sdfVector4_t origin;
    sdfVector4_t offset;
    f32 verticalScale;
    f32 phaseStep;
    s32 completedCount;
    s32 repeatEnabled;
    s32 packetCount;
    s32 lifetimeFrames;
    s32 packetIndex;

    dds3Particle_EffectInit(&hData->head.sub);
    SDF_VU0_LOADMATRIX(hData->head.matrix);
    phaseStep = SDF_FRAME_STEP(hData->rotationStepDegrees * (3.14159265f / 180.0f));
    verticalScale = SDF_FRAME_DECAY(hData->verticalAccelerationPct / 100.0f + 1.0f);
    completedCount = 0;
    packetCount = hData->head.particleCount;
    lifetimeFrames = SDF_FRAMES(hData->head.lifetimeFrames);
    repeatEnabled = hData->loop;
    SDF_VU0_COPYVECTOR(&origin, hData->head.origin);
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++) {
        s32 age = packet->age;

        if (age == EFF_PACKET_INITIAL_TAG) {
            dds3Particle_SpiralBuffInit(hData, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            f32 theta;
            f32 radius;

            /* dds3Particle_BasicCreate initializes this opaque slot as a dds3ParticleColorAnim_t. */
            packet->color = dds3ParticleColorAnim(&hData->head.fade, packet->color, age);
            packet->color = dds3ParticleColorBlend(packet->color, hData->head.colorMask);
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR_C(packet);
            }
            packet->radialDistance += (packet->vel[0] - hData->radius) / lifetimeFrames;
            packet->phase += phaseStep;
            if (packet->radialDistance < 0) {
                packet->radialDistance = 0;
            }
            packet->verticalOffset += SDF_FRAME_STEP(packet->verticalSpeed);
            theta = packet->phase;
            radius = packet->radialDistance;
            offset.x = sdfCos(theta) * radius;
            offset.y = packet->verticalOffset;
            offset.z = sdfSin(theta) * radius;
            SDF_VU0_LOADVECTOR(&offset);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_STOREVECTOR(&offset);
            packet->pos[0] = origin.x + offset.x;
            packet->pos[1] = origin.y + offset.y;
            packet->pos[2] = origin.z + offset.z;
            packet->verticalSpeed *= verticalScale;
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR(packet);
                dds3Particle_EffectSet(&hData->head.sub, packetIndex, packet->color, packet->billboardScale);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                completedCount++;
                dds3Particle_EffectReset(&hData->head.sub, packetIndex);
                if (completedCount >= packetCount) {
                    hData->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

/* Initialize only the native age/tag word of each packet record. */
void dds3Particle_SmokeAllBuffInit(effTemplatePacketList_t *hData) {
    effectBufferRecord_t *packetCursor;
    u32 packetIndex;

    packetIndex = 0;
    packetCursor = hData->head.buffer->pPhysics;
    if (hData->head.particleCount != 0) {
        do {
            packetCursor->age = EFF_PACKET_INITIAL_TAG;
            packetIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
        } while (packetIndex < hData->head.particleCount);
    }
}

/* Scale template position, record scale, and this variant's two tail values. */
void dds3Particle_SmokeSetScale(effTemplatePacketList_t *hData, float scale) {
    hData->head.scale[0] = hData->head.scale[0] * scale;
    hData->head.scale[1] = hData->head.scale[1] * scale;
    hData->head.scale[2] = hData->head.scale[2] * scale;
    hData->recordScale = hData->recordScale * scale;
    hData->tailValues[0] = hData->tailValues[0] * scale;
    hData->tailValues[2] = hData->tailValues[2] * scale;
}

/* Clone the disc prefix/tail, allocate packet records, and initialize their native age tags. */
effTemplatePacketList_t *dds3Particle_SmokeCreate(effTemplatePacketList_t *source) {
    effTemplatePacketList_t *clone = sdfChipAllocDirect(0x180);
    s32 tailBytes = 0x30;

    memset(clone, 0, 0x180);
    memcpy(clone, source, source->head.templateSize);
    memcpy((u8 *)clone + EFF_TEMPLATE_TAIL_OFFSET, (u8 *)source + source->head.templateSize, tailBytes);
    dds3Particle_BasicCreate(clone);
    dds3Particle_SmokeAllBuffInit(clone);
    return clone;
}

void dds3Particle_SmokeDelete(void *hData) {
    dds3Particle_BasicDelete(hData);
    sdfChipFreeDirect(hData);
}

/* Seed a random planar position and direction; radialDistance may be negative. */
void dds3Particle_SmokeBuffInit(dds3Particle_Smoke_t3 *pSmoke, u32 number) {
    parSmokePacket_t *packet = (parSmokePacket_t *)pSmoke->head.buffer->pPhysics;
    sdfVector4_t directionVector;
    f32 radialDistance;
    f32 jitter;

    packet += number;
    radialDistance = pSmoke->radius * ((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f);
    directionVector.x = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
    directionVector.y = 0;
    directionVector.z = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
    SDF_VU0_LOADVECTOR(&directionVector);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&directionVector);
    packet->pos[0] = directionVector.x * radialDistance;
    packet->pos[1] = 0;
    packet->pos[2] = directionVector.z * radialDistance;
    SDF_VU0_LOADMATRIX(pSmoke->head.matrix);
    SDF_VU0_LOADVECTOR(packet->pos);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(packet->pos);
    packet->pos[0] += pSmoke->head.origin[0];
    packet->pos[1] += pSmoke->head.origin[1];
    packet->pos[2] += pSmoke->head.origin[2];
    directionVector.x = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
    directionVector.y = 0;
    directionVector.z = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
    SDF_VU0_LOADVECTOR(&directionVector);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(packet->vel);
    packet->waveAmplitude = pSmoke->waveAmplitude * (dds3GetRandFloatA(dds3EffectBase) * pSmoke->waveAmplitudeJitter + (1.0f - pSmoke->waveAmplitudeJitter));
    packet->wavePhase = 0;
    packet->previousWaveValue = 0;
    packet->verticalVelocity = pSmoke->verticalSpeed * (dds3GetRandFloatA(dds3EffectBase) * pSmoke->verticalSpeedJitter + (1.0f - pSmoke->verticalSpeedJitter));
    packet->age = -(dds3GetRandA((effRandState_t *)ParRand) % (SDF_FRAMES(pSmoke->spread) + 1));
    packet->color = 0;
    jitter = pSmoke->head.speedJitter;
    packet->billboardScale = pSmoke->head.scale[0] * (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter));
    jitter = pSmoke->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    dds3Particle_EffectReset(&pSmoke->head.sub, number);
}

/* Advance sinusoidal planar displacement and scaled vertical steps, then ages. */
void dds3Particle_SmokeDraw(dds3Particle_Smoke_t3 *hData) {
    parSmokePacket_t *packet = (parSmokePacket_t *)hData->head.buffer->pPhysics;
    u32 subeffectKind = hData->head.sub.type;
    sdfVector4_t positionDelta;
    f32 verticalScale;
    f32 phaseStep;
    f32 waveRate;
    s32 completedCount;
    s32 lifetimeFrames;
    s32 packetCount;
    s32 packetIndex;
    u32 repeatEnabled;

    dds3Particle_EffectInit(&hData->head.sub);
    SDF_VU0_LOADMATRIX(hData->head.matrix);
    phaseStep = SDF_FRAME_STEP(hData->phaseStepDegrees * (3.14159265f / 180.0f));
    verticalScale = SDF_FRAME_DECAY(hData->verticalAccelerationPct / 100.0f + 1.0f);
    lifetimeFrames = SDF_FRAMES(hData->head.lifetimeFrames);
    packetCount = hData->head.particleCount;
    repeatEnabled = hData->loop;
    waveRate = SDF_FRAME_STEP(hData->waveGrowth);
    completedCount = 0;
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++) {
        s32 age = packet->age;

        if (age == EFF_PACKET_INITIAL_TAG) {
            dds3Particle_SmokeBuffInit(hData, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            f32 verticalStep;
            f32 previousWaveValue;
            f32 factor;

            packet->color = dds3ParticleColorAnim(&hData->head.fade, packet->color, age);
            packet->color = dds3ParticleColorBlend(packet->color, hData->head.colorMask);
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR_C(packet);
            }
            packet->waveAmplitude += waveRate;
            verticalStep = packet->verticalVelocity;
            packet->wavePhase += phaseStep;
            factor = sdfSin(packet->wavePhase);
            previousWaveValue = packet->previousWaveValue;
            packet->previousWaveValue = factor;
            factor = (factor - previousWaveValue) * packet->waveAmplitude;
            positionDelta.x = packet->vel[0] * factor;
            positionDelta.y = SDF_FRAME_STEP(verticalStep);
            positionDelta.z = packet->vel[2] * factor;
            SDF_VU0_LOADVECTOR(&positionDelta);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_STOREVECTOR(&positionDelta);
            packet->pos[0] += positionDelta.x;
            packet->pos[1] += positionDelta.y;
            packet->pos[2] += positionDelta.z;
            packet->verticalVelocity = verticalStep * verticalScale;
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR(packet);
                dds3Particle_EffectSet(&hData->head.sub, packetIndex, packet->color, packet->billboardScale);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                dds3Particle_EffectReset(&hData->head.sub, packetIndex);
                completedCount++;
                if (completedCount >= packetCount) {
                    hData->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

/* Initialize only the native age/tag word of each packet record. */
void dds3Particle_SparkAllBuffInit(effTemplatePacketList_t *hData) {
    effectBufferRecord_t *packetCursor;
    u32 packetIndex;

    packetIndex = 0;
    packetCursor = hData->head.buffer->pPhysics;
    if (hData->head.particleCount != 0) {
        do {
            packetCursor->age = EFF_PACKET_INITIAL_TAG;
            packetIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
        } while (packetIndex < hData->head.particleCount);
    }
}

/* Scale the position and three variant-specific tail values. */
void dds3Particle_SparkSetScale(effTemplatePacketList_t *hData, float scale) {
    hData->head.scale[0] = hData->head.scale[0] * scale;
    hData->head.scale[1] = hData->head.scale[1] * scale;
    hData->head.scale[2] = hData->head.scale[2] * scale;
    hData->tailValues[0] = hData->tailValues[0] * scale;
    hData->tailValues[2] = hData->tailValues[2] * scale;
    hData->tailValues[3] = hData->tailValues[3] * scale;
}

/* Clone the ballistic prefix/tail, allocate packet records, and initialize their native age tags. */
effTemplatePacketList_t *dds3Particle_SparkCreate(effTemplatePacketList_t *source) {
    effTemplatePacketList_t *clone = sdfChipAllocDirect(0x180);
    s32 tailBytes = 0x30;

    memset(clone, 0, 0x180);
    memcpy(clone, source, source->head.templateSize);
    memcpy((u8 *)clone + EFF_TEMPLATE_TAIL_OFFSET, (u8 *)source + source->head.templateSize, tailBytes);
    dds3Particle_BasicCreate(clone);
    dds3Particle_SparkAllBuffInit(clone);
    return clone;
}

void dds3Particle_SparkDelete(void *hData) {
    dds3Particle_BasicDelete(hData);
    sdfChipFreeDirect(hData);
}

/* vu0 routine: seed a ballistic packet; positions are emitter-relative and
 * then offset by the origin. */
/* NON_MATCHING in SMT3: 22 of 512 words differ (first at +0x90). */
void dds3Particle_SparkBuffInit(dds3Particle_Spark_t3 *pSpark, s32 number) {
    parSparkPacket_t *packet = (parSparkPacket_t *)pSpark->head.buffer->pPhysics;
    sdfVector4_t position;
    sdfVector4_t direction;
    f32 gravity;
    f32 radius;
    f32 cone;
    f32 speed;
    f32 jitter;
    f32 decay;
    f32 angle;
    f32 angleCos;
    f32 angleSin;
    s32 steps;
    s32 step;

    packet += number;
    gravity = pSpark->gravity / 100.0f;
    radius = pSpark->radius;
    cone = pSpark->cone;
    speed = pSpark->speed;
    switch (pSpark->mode) {
    case 0:
        if (pSpark->randomDir != 0) {
            direction.x = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
            direction.y = 0;
            direction.z = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
            SDF_VU0_LOADVECTOR(&direction);
            SDF_VU0_NORMALIZE();
            SDF_VU0_STOREVECTOR(&direction);
            packet->pos[0] = direction.x * radius;
            packet->pos[1] = 0;
            packet->pos[2] = direction.z * radius;
            SDF_VU0_LOADMATRIX(pSpark->head.matrix);
            SDF_VU0_LOADVECTOR(packet->pos);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_STOREVECTOR(packet->pos);
            packet->pos[0] += pSpark->head.origin[0];
            packet->pos[1] += pSpark->head.origin[1];
            packet->pos[2] += pSpark->head.origin[2];
            direction.x = (dds3GetRandFloatA((effRandState_t *)ParRand) - 0.5f) * 2.0f * cone;
            direction.y = -(1.0f - cone);
            direction.z = (dds3GetRandFloatA((effRandState_t *)ParRand) - 0.5f) * 2.0f * cone;
            SDF_VU0_LOADVECTOR(&direction);
            SDF_VU0_NORMALIZE();
            SDF_VU0_STOREVECTOR(&direction);
            jitter = dds3GetRandFloatA(dds3EffectBase) * pSpark->jitter + (1.0f - pSpark->jitter);
            packet->vel[0] = direction.x * speed * jitter;
            packet->vel[1] = direction.y * speed * jitter;
            packet->vel[2] = direction.z * speed * jitter;
            packet->verticalAcceleration = (dds3GetRandFloatA(dds3EffectBase) * 0.5f + 0.5f) * gravity;
        } else {
            angle = (3.14159265f * 2.0f) / pSpark->head.particleCount * number;
            angleCos = sdfCos(angle);
            angleSin = sdfSin(angle);
            packet->pos[0] = angleCos * radius;
            packet->pos[1] = 0;
            packet->pos[2] = angleSin * radius;
            SDF_VU0_LOADMATRIX(pSpark->head.matrix);
            SDF_VU0_LOADVECTOR(packet->pos);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_STOREVECTOR(packet->pos);
            packet->pos[0] += pSpark->head.origin[0];
            packet->pos[1] += pSpark->head.origin[1];
            packet->pos[2] += pSpark->head.origin[2];
            packet->vel[0] = angleCos * cone * speed;
            packet->vel[1] = -speed * (1.0f - cone);
            packet->vel[2] = angleSin * cone * speed;
            packet->verticalAcceleration = gravity;
        }
        break;
    case 1:
        if (pSpark->randomDir != 0) {
            packet->pos[0] = pSpark->head.origin[0];
            packet->pos[1] = pSpark->head.origin[1];
            packet->pos[2] = pSpark->head.origin[2];
            direction.x = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
            direction.y = -((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f);
            direction.z = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
            SDF_VU0_LOADVECTOR(&direction);
            SDF_VU0_NORMALIZE();
            SDF_VU0_STOREVECTOR(&direction);
            jitter = dds3GetRandFloatA(dds3EffectBase) * pSpark->jitter + (1.0f - pSpark->jitter);
            packet->vel[0] = direction.x * speed * jitter;
            packet->vel[1] = direction.y * speed * jitter;
            packet->vel[2] = direction.z * speed * jitter;
            packet->verticalAcceleration = gravity;
        } else {
            packet->pos[0] = pSpark->head.origin[0];
            packet->pos[1] = pSpark->head.origin[1];
            packet->pos[2] = pSpark->head.origin[2];
            SDF_VU0_STOREIDENTITY(pSpark->head.matrix);
            packet->vel[0] = dds3ParticleSparkDirTable[number % 12][0] * speed;
            packet->vel[1] = dds3ParticleSparkDirTable[number % 12][1] * speed;
            packet->vel[2] = dds3ParticleSparkDirTable[number % 12][2] * speed;
            packet->verticalAcceleration = gravity;
        }
        break;
    case 2:
        if (pSpark->randomDir != 0) {
            decay = pSpark->decayPct / 100.0f + 1.0f;
            SDF_VU0_STOREIDENTITY(pSpark->head.matrix);
            direction.x = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
            direction.y = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
            direction.z = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
            SDF_VU0_LOADVECTOR(&direction);
            SDF_VU0_NORMALIZE();
            SDF_VU0_STOREVECTOR(&direction);
            jitter = dds3GetRandFloatA(dds3EffectBase) * pSpark->jitter + (1.0f - pSpark->jitter);
            steps = pSpark->head.lifetimeFrames;
            direction.x = direction.x * (speed * jitter);
            direction.y = direction.y * (speed * jitter);
            direction.z = direction.z * (speed * jitter);
            packet->vel[0] = -direction.x;
            packet->vel[1] = -direction.y;
            packet->vel[2] = -direction.z;
            position.x = pSpark->head.origin[0];
            position.y = pSpark->head.origin[1];
            position.z = pSpark->head.origin[2];
            step = 0;
            do {
                position.x += direction.x;
                position.y += direction.y;
                position.z += direction.z;
                direction.x = direction.x * decay;
                direction.y = direction.y * decay;
                direction.z = direction.z * decay;
                direction.y += gravity;
                step++;
            } while (step < steps);
            packet->pos[0] = position.x;
            packet->pos[1] = position.y;
            packet->pos[2] = position.z;
            packet->verticalAcceleration = -gravity;
        }
        break;
    }
    packet->age = -(dds3GetRandA((effRandState_t *)ParRand) % (SDF_FRAMES(pSpark->spread) + 1));
    packet->color = 0;
    jitter = pSpark->head.speedJitter;
    packet->billboardScale = pSpark->head.scale[0] * (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter));
    jitter = pSpark->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    dds3Particle_EffectReset(&pSpark->head.sub, number);
}


/* Spark launch initialization retains an s4/s5 RNG-address allocation residual. */
/* Scale velocity, add its stored vertical acceleration, and advance position.
 * Only mode zero transforms the displacement through the emitter matrix. */
void dds3Particle_SparkDraw(dds3Particle_Spark_t3 *hData) {
    parSparkPacket_t *packet = (parSparkPacket_t *)hData->head.buffer->pPhysics;
    u32 subeffectKind = hData->head.sub.type;
    u32 mode;
    sdfVector4_t positionDelta;
    f32 velocityScale;
    s32 completedCount;
    s32 lifetimeFrames;
    s32 packetCount;
    s32 repeatEnabled;
    s32 packetIndex;

    dds3Particle_EffectInit(&hData->head.sub);
    mode = hData->mode;
    if (mode == 0) {
        SDF_VU0_LOADMATRIX(hData->head.matrix);
    }
    velocityScale = SDF_FRAME_DECAY(hData->decayPct / 100.0f + 1.0f);
    completedCount = 0;
    lifetimeFrames = SDF_FRAMES(hData->head.lifetimeFrames);
    packetCount = hData->head.particleCount;
    repeatEnabled = hData->loop;
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++) {
        s32 age = packet->age;

        if (age == EFF_PACKET_INITIAL_TAG) {
            dds3Particle_SparkBuffInit(hData, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            packet->color = dds3ParticleColorAnim(&hData->head.fade, packet->color, age);
            packet->color = dds3ParticleColorBlend(packet->color, hData->head.colorMask);
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR_C(packet);
            }
            packet->vel[1] = packet->vel[1] * velocityScale + SDF_FRAME_STEP(packet->verticalAcceleration);
            packet->vel[0] *= velocityScale;
            packet->vel[2] *= velocityScale;
            SDF_VU0_LOADVECTOR(packet->vel);
            if (mode == 0) {
                SDF_VU0_APPLYMATRIX();
            }
            SDF_VU0_STOREVECTOR(&positionDelta);
            packet->pos[0] += SDF_FRAME_STEP(positionDelta.x);
            packet->pos[1] += SDF_FRAME_STEP(positionDelta.y);
            packet->pos[2] += SDF_FRAME_STEP(positionDelta.z);
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR(packet);
                dds3Particle_EffectSet(&hData->head.sub, packetIndex, packet->color, packet->billboardScale);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                dds3Particle_EffectReset(&hData->head.sub, packetIndex);
                completedCount++;
                if (completedCount >= packetCount) {
                    hData->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

/* Spawn each packet, then overwrite its tag with a zero-based, period-grouped schedule. */
void dds3Particle_RadiateAllBuffInit(effTemplatePacketList_t *hData) {
    effectBufferRecord_t *packetCursor;
    u32 packetIndex;
    u32 nextPacketIndex;
    s32 packetTag;

    packetTag = 0;
    packetIndex = 0;
    packetCursor = hData->head.buffer->pPhysics;
    if (hData->head.particleCount != 0) {
        do {
            dds3Particle_RadiateBuffInit((dds3Particle_Radiate_t3 *)hData, packetIndex);
            packetCursor->age = packetTag;
            nextPacketIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
            if (nextPacketIndex % hData->tailWords[0] == 0) {
                packetTag = packetTag - SDF_FRAMES(hData->decayStep);
            }
            packetIndex = nextPacketIndex;
        } while (packetIndex < hData->head.particleCount);
    }
}

/* Scale position and the third variant-specific tail value. */
void dds3Particle_RadiateSetScale(effTemplatePacketList_t *hData, float scale) {
    hData->head.scale[0] = hData->head.scale[0] * scale;
    hData->head.scale[1] = hData->head.scale[1] * scale;
    hData->head.scale[2] = hData->head.scale[2] * scale;
    hData->tailValues[2] = hData->tailValues[2] * scale;
}

/* Clone the look-at ring prefix/tail and spawn its staggered packet schedule. */
effTemplatePacketList_t *dds3Particle_RadiateCreate(effTemplatePacketList_t *source) {
    effTemplatePacketList_t *clone = sdfChipAllocDirect(0x170);
    s32 tailBytes = 0x20;

    memset(clone, 0, 0x170);
    memcpy(clone, source, source->head.templateSize);
    memcpy((u8 *)clone + EFF_TEMPLATE_TAIL_OFFSET, (u8 *)source + source->head.templateSize, tailBytes);
    dds3Particle_BasicCreate(clone);
    dds3Particle_RadiateAllBuffInit(clone);
    return clone;
}

void dds3Particle_RadiateDelete(void *hData) {
    dds3Particle_BasicDelete(hData);
    sdfChipFreeDirect(hData);
}

/* Seed a view-oriented ring packet and initialize its subeffect. */
void dds3Particle_RadiateBuffInit(dds3Particle_Radiate_t3 *pRadiate, u32 number) {
    parRadiatePacket_t *packet = (parRadiatePacket_t *)pRadiate->head.buffer->pPhysics;
    u32 period = 0;
    f32 angle;
    f32 scale;
    f32 jitter;

    packet += number;
    LookAtMatrix(aViewTarget, aViewEye, aViewDown);
    ReverseMatrix();
    if (pRadiate->mode == 0) {
        SDF_VU0_COPYVECTOR(packet, pRadiate->head.origin);
        period = pRadiate->period;
        angle = (3.14159265f * 2.0f) / period * (number % period);
        packet->vel[0] = sdfCos(angle);
        packet->vel[1] = sdfSin(angle);
        packet->vel[2] = 0;
        SDF_VU0_LOADVECTOR(packet->vel);
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_STOREVECTOR(packet->vel);
        packet->phaseAngle = angle;
        packet->age = 0;
        packet->color = 0;
    } else {
        f32 lifetimeFrames = pRadiate->head.lifetimeFrames;

        scale = pRadiate->stepLength;
#ifdef FIX_BUGS
        /* BUG: period is still zero here, so the modulo is a divide by zero (a trap on the EE). */
        angle = 0.0f;
#else
        angle = (3.14159265f * 2.0f) / period * (number % period);
#endif
        scale *= lifetimeFrames;
        angle += pRadiate->rotationStepDegrees * (3.14159265f / 180.0f) * lifetimeFrames;
        packet->vel[0] = sdfCos(angle);
        packet->vel[1] = 0;
        packet->vel[2] = sdfSin(angle);
        packet->phaseAngle = angle;
        packet->pos[0] = packet->vel[0] * scale;
        packet->pos[1] = packet->vel[1] * scale;
        packet->pos[2] = packet->vel[2] * scale;
        packet->age = 0;
        packet->color = 0;
    }
    jitter = pRadiate->head.speedJitter;
    packet->billboardScale = pRadiate->head.scale[0] * (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter));
    jitter = pRadiate->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    dds3Particle_EffectReset(&pRadiate->head.sub, number);
}

/* Advance view-oriented motion and ages; repeated packets respawn immediately. */
void dds3Particle_RadiateDraw(dds3Particle_Radiate_t3 *hData) {
    parRadiatePacket_t *packet = (parRadiatePacket_t *)hData->head.buffer->pPhysics;
    u32 subeffectKind = hData->head.sub.type;
    f32 phaseStep;
    f32 stepScale;
    s32 completedCount = 0;
    s32 repeatEnabled;
    s32 packetCount;
    s32 lifetimeFrames;
    s32 packetIndex;

    dds3Particle_EffectInit(&hData->head.sub);
    LookAtMatrix(aViewTarget, aViewEye, aViewDown);
    ReverseMatrix();
    lifetimeFrames = SDF_FRAMES(hData->head.lifetimeFrames);
    phaseStep = SDF_FRAME_STEP(hData->rotationStepDegrees * (3.14159265f / 180.0f));
    packetCount = hData->head.particleCount;
    repeatEnabled = hData->loop;
    stepScale = SDF_FRAME_STEP(hData->stepLength);
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++) {
        s32 age = packet->age;
        f32 angle;

        if (age >= 0) {
            packet->color = dds3ParticleColorAnim(&hData->head.fade, packet->color, age);
            packet->color = dds3ParticleColorBlend(packet->color, hData->head.colorMask);
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR_C(packet);
            }
            packet->pos[0] += packet->vel[0] * stepScale;
            packet->pos[1] += packet->vel[1] * stepScale;
            packet->pos[2] += packet->vel[2] * stepScale;
            angle = packet->phaseAngle;
            packet->vel[0] = sdfCos(angle);
            packet->vel[1] = sdfSin(angle);
            packet->vel[2] = 0;
            SDF_VU0_LOADVECTOR(packet->vel);
            SDF_VU0_APPLYMATRIX_XYZ();
            SDF_VU0_STOREVECTOR(packet->vel);
            packet->phaseAngle += phaseStep;
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR(packet);
                dds3Particle_EffectSet(&hData->head.sub, packetIndex, packet->color, packet->billboardScale);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                dds3Particle_RadiateBuffInit((dds3Particle_Radiate_t3 *)hData, packetIndex);
                age = packet->age;
            } else {
                dds3Particle_EffectReset(&hData->head.sub, packetIndex);
                completedCount++;
                if (completedCount >= packetCount) {
                    hData->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

/* Initialize only the native age/tag word of each packet record. */
void dds3Particle_SphereSpiralAllBuffInit(effTemplatePacketList_t *hData) {
    effectBufferRecord_t *packetCursor;
    u32 packetIndex;

    packetIndex = 0;
    packetCursor = hData->head.buffer->pPhysics;
    if (hData->head.particleCount != 0) {
        do {
            packetCursor->age = EFF_PACKET_INITIAL_TAG;
            packetIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
        } while (packetIndex < hData->head.particleCount);
    }
}

/* Scale position, record scale, and the second tail value. */
void dds3Particle_SphereSpiralSetScale(effTemplatePacketList_t *hData, float scale) {
    hData->head.scale[0] = hData->head.scale[0] * scale;
    hData->head.scale[1] = hData->head.scale[1] * scale;
    hData->head.scale[2] = hData->head.scale[2] * scale;
    hData->recordScale = hData->recordScale * scale;
    hData->tailValues[1] = hData->tailValues[1] * scale;
}

/* Clone the burst prefix/tail, allocate packet records, and initialize their native age tags. */
effTemplatePacketList_t *dds3Particle_SphereSpiralCreate(effTemplatePacketList_t *source) {
    effTemplatePacketList_t *clone = sdfChipAllocDirect(0x170);
    s32 tailBytes = 0x20;

    memset(clone, 0, 0x170);
    memcpy(clone, source, source->head.templateSize);
    memcpy((u8 *)clone + EFF_TEMPLATE_TAIL_OFFSET, (u8 *)source + source->head.templateSize, tailBytes);
    dds3Particle_BasicCreate(clone);
    dds3Particle_SphereSpiralAllBuffInit(clone);
    return clone;
}

void dds3Particle_SphereSpiralDelete(void *hData) {
    dds3Particle_BasicDelete(hData);
    sdfChipFreeDirect(hData);
}

/* Seed burst velocity and an optional random axis; retain the native spawn delay. */
void dds3Particle_SphereSpiralBuffInit(dds3Particle_SphereSpiral_t3 *pSphereSpiral, u32 number) {
    parSphereSpiralPacket_t *packet = (parSphereSpiralPacket_t *)pSphereSpiral->head.buffer->pPhysics;
    sdfVector4_t directionVector;
    f32 speed;
    f32 jitter;
    f32 velocityLength;

    packet += number;
    packet->color = 0;
    packet->age = -(dds3GetRandA((effRandState_t *)ParRand) % (SDF_FRAMES(pSphereSpiral->spread) + 1));
    speed = pSphereSpiral->speed;
    jitter = pSphereSpiral->jitterA;
    directionVector.x = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
    directionVector.y = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
    directionVector.z = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
    SDF_VU0_LOADVECTOR(&directionVector);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&directionVector);
    packet->vel[0] = speed * (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter)) * directionVector.x;
    packet->vel[1] = speed * (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter)) * directionVector.y;
    packet->vel[2] = speed * (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter)) * directionVector.z;
    SDF_VU0_LOADVECTOR(packet->vel);
    SDF_VU0_GETLENGTH(velocityLength);
    SDF_VU0_LOADMATRIX(pSphereSpiral->head.matrix);
    SDF_VU0_LOADVECTOR(packet->vel);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR_NOMEM(packet->vel);
    packet->pos[0] = packet->vel[0] + pSphereSpiral->head.origin[0];
    packet->pos[1] = packet->vel[1] + pSphereSpiral->head.origin[1];
    packet->pos[2] = packet->vel[2] + pSphereSpiral->head.origin[2];
    if (pSphereSpiral->mode == 1) {
        directionVector.x = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
        directionVector.y = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
        directionVector.z = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
        SDF_VU0_LOADVECTOR(&directionVector);
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR(&directionVector);
        packet->axisX = directionVector.x;
        packet->axisY = directionVector.y;
        packet->axisZ = directionVector.z;
    } else {
        packet->axisX = 0;
        packet->axisY = -1.0f;
        packet->axisZ = 0;
    }
    jitter = pSphereSpiral->jitterB;
    packet->radiusStep = (pSphereSpiral->targetRadius * (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter)) - velocityLength) / pSphereSpiral->head.lifetimeFrames;
    jitter = pSphereSpiral->head.speedJitter;
    packet->billboardScale = pSphereSpiral->head.scale[0] * (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter));
    jitter = pSphereSpiral->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    dds3Particle_EffectReset(&pSphereSpiral->head.sub, number);
}

/* Adjust velocity length, rotate around the packet axis, and rebuild position. */
void dds3Particle_SphereSpiralDraw(dds3Particle_SphereSpiral_t3 *hData) {
    parSphereSpiralPacket_t *packet = (parSphereSpiralPacket_t *)hData->head.buffer->pPhysics;
    u32 subeffectKind = hData->head.sub.type;
    sdfVector4_t origin;
    sdfVector4_t rotationAxis;
    sdfVector4_t scaleVector;
    f32 velocityLength;
    f32 rotationStep;
    s32 completedCount;
    s32 lifetimeFrames;
    s32 repeatEnabled;
    s32 packetCount;
    s32 packetIndex;

    dds3Particle_EffectInit(&hData->head.sub);
    SDF_VU0_LOADMATRIX_B(hData->head.matrix);
    packetCount = hData->head.particleCount;
    lifetimeFrames = SDF_FRAMES(hData->head.lifetimeFrames);
    completedCount = 0;
    rotationStep = SDF_FRAME_STEP(hData->spinRate * (3.14159265f / 180.0f));
    repeatEnabled = hData->loop;
    SDF_VU0_COPYVECTOR(&origin, hData->head.origin);
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++) {
        s32 age = packet->age;

        if (age == EFF_PACKET_INITIAL_TAG) {
            dds3Particle_SphereSpiralBuffInit(hData, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            packet->color = dds3ParticleColorAnim(&hData->head.fade, packet->color, age);
            packet->color = dds3ParticleColorBlend(packet->color, hData->head.colorMask);
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR_C(packet);
            }
            SDF_VU0_LOADVECTOR(packet->vel);
            SDF_VU0_GETLENGTH(velocityLength);
            velocityLength += SDF_FRAME_STEP(packet->radiusStep);
            scaleVector.x = velocityLength;
            scaleVector.y = velocityLength;
            scaleVector.z = velocityLength;
            SDF_VU0_NORMALIZE();
            SDF_VU0_LOADVECTOR_B(&scaleVector);
            SDF_VU0_MULVECTOR();
            SDF_VU0_STOREVECTOR(packet->vel);
            SDF_VU0_COPYMATRIX_B_TO_A();
            rotationAxis.x = packet->axisX;
            rotationAxis.y = packet->axisY;
            rotationAxis.z = packet->axisZ;
            rotationAxis.w = 0;
            SDF_VU0_LOADVECTOR(&rotationAxis);
            SDF_VU0_APPLYMATRIX_XYZ();
            SDF_VU0_STOREVECTOR_NOMEM(&rotationAxis);
            SetRotateMatrixAxis(rotationStep, (const sdfVector3_t *)&rotationAxis);
            SDF_VU0_LOADVECTOR(packet->vel);
            SDF_VU0_APPLYMATRIX_XYZ();
            SDF_VU0_LOADVECTOR_B(&origin);
            SDF_VU0_STOREVECTOR(packet->vel);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(packet);
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR(packet);
                dds3Particle_EffectSet(&hData->head.sub, packetIndex, packet->color, packet->billboardScale);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                dds3Particle_EffectReset(&hData->head.sub, packetIndex);
                completedCount++;
                if (completedCount >= packetCount) {
                    hData->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

/* Initialize only the native age/tag word of each packet record. */
void dds3Particle_SphereSmokeAllBuffInit(effTemplatePacketList_t *hData) {
    effectBufferRecord_t *packetCursor;
    u32 packetIndex;

    packetIndex = 0;
    packetCursor = hData->head.buffer->pPhysics;
    if (hData->head.particleCount != 0) {
        do {
            packetCursor->age = EFF_PACKET_INITIAL_TAG;
            packetIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
        } while (packetIndex < hData->head.particleCount);
    }
}

/* Scale template position, record scale, and this variant's tail values. */
void dds3Particle_SphereSmokeSetScale(effTemplatePacketList_t *hData, float scale) {
    hData->head.scale[0] = hData->head.scale[0] * scale;
    hData->head.scale[1] = hData->head.scale[1] * scale;
    hData->head.scale[2] = hData->head.scale[2] * scale;
    hData->recordScale = hData->recordScale * scale;
    hData->tailValues[0] = hData->tailValues[0] * scale;
    hData->tailValues[2] = hData->tailValues[2] * scale;
}

/* Clone the sphere prefix/tail, allocate packet records, and initialize their native age tags. */
effTemplatePacketList_t *dds3Particle_SphereSmokeCreate(effTemplatePacketList_t *source) {
    effTemplatePacketList_t *clone = sdfChipAllocDirect(0x180);
    s32 tailBytes = 0x30;

    memset(clone, 0, 0x180);
    memcpy(clone, source, source->head.templateSize);
    memcpy((u8 *)clone + EFF_TEMPLATE_TAIL_OFFSET, (u8 *)source + source->head.templateSize, tailBytes);
    dds3Particle_BasicCreate(clone);
    dds3Particle_SphereSmokeAllBuffInit(clone);
    return clone;
}

void dds3Particle_SphereSmokeDelete(void *hData) {
    dds3Particle_BasicDelete(hData);
    sdfChipFreeDirect(hData);
}

/* Seed a jittered spherical position and planar direction.
 * Native delay uses bitwise complement, not unary negation. */
void dds3Particle_SphereSmokeBuffInit(dds3Particle_SphereSmoke_t3 *pSphereSmoke, u32 number) {
    parSmokePacket_t *packet = (parSmokePacket_t *)pSphereSmoke->head.buffer->pPhysics;
    sdfVector4_t directionVector;
    f32 radius;
    f32 jitter;

    packet += number;
    radius = pSphereSmoke->radius;
    jitter = pSphereSmoke->radius_s_r;
    directionVector.x = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
    directionVector.y = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
    directionVector.z = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
    SDF_VU0_LOADVECTOR(&directionVector);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&directionVector);
    packet->pos[0] = radius * (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter)) * directionVector.x;
    packet->pos[1] = radius * (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter)) * directionVector.y;
    packet->pos[2] = radius * (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter)) * directionVector.z;
    SDF_VU0_LOADMATRIX(pSphereSmoke->head.matrix);
    SDF_VU0_LOADVECTOR(packet->pos);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(packet->pos);
    packet->pos[0] += pSphereSmoke->head.origin[0];
    packet->pos[1] += pSphereSmoke->head.origin[1];
    packet->pos[2] += pSphereSmoke->head.origin[2];
    packet->vel[0] = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
    packet->vel[1] = 0;
    packet->vel[2] = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
    SDF_VU0_LOADVECTOR(packet->vel);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(packet->vel);
    packet->waveAmplitude = pSphereSmoke->amplitude * (dds3GetRandFloatA(dds3EffectBase) * pSphereSmoke->ampli_r + (1.0f - pSphereSmoke->ampli_r));
    packet->wavePhase = 0;
    packet->previousWaveValue = 0;
    packet->verticalVelocity = pSphereSmoke->up_s * (dds3GetRandFloatA(dds3EffectBase) * pSphereSmoke->up_s_r + (1.0f - pSphereSmoke->up_s_r));
    packet->age = ~(dds3GetRandA((effRandState_t *)ParRand) % (SDF_FRAMES(pSphereSmoke->spread) + 1));
    packet->color = 0;
    jitter = pSphereSmoke->head.speedJitter;
    packet->billboardScale = pSphereSmoke->head.scale[0] * (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter));
    jitter = pSphereSmoke->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    dds3Particle_EffectReset(&pSphereSmoke->head.sub, number);
}

/* Advance sinusoidal planar displacement and scaled vertical steps, then ages. */
void dds3Particle_SphereSmokeDraw(dds3Particle_SphereSmoke_t3 *hData) {
    parSmokePacket_t *packet = (parSmokePacket_t *)hData->head.buffer->pPhysics;
    u32 subeffectKind = hData->head.sub.type;
    sdfVector4_t positionDelta;
    f32 verticalScale;
    f32 phaseStep;
    f32 waveRate;
    s32 completedCount;
    s32 lifetimeFrames;
    s32 packetCount;
    s32 packetIndex;
    u32 repeatEnabled;

    dds3Particle_EffectInit(&hData->head.sub);
    SDF_VU0_LOADMATRIX(hData->head.matrix);
    phaseStep = SDF_FRAME_STEP(hData->phaseStepDegrees * (3.14159265f / 180.0f));
    verticalScale = SDF_FRAME_DECAY(hData->verticalAccelerationPct / 100.0f + 1.0f);
    lifetimeFrames = SDF_FRAMES(hData->head.lifetimeFrames);
    packetCount = hData->head.particleCount;
    repeatEnabled = hData->loop;
    waveRate = SDF_FRAME_STEP(hData->waveGrowth);
    completedCount = 0;
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++) {
        s32 age = packet->age;

        if (age == EFF_PACKET_INITIAL_TAG) {
            dds3Particle_SphereSmokeBuffInit(hData, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            f32 verticalStep;
            f32 previousWaveValue;
            f32 factor;

            packet->color = dds3ParticleColorAnim(&hData->head.fade, packet->color, age);
            packet->color = dds3ParticleColorBlend(packet->color, hData->head.colorMask);
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR_C(packet);
            }
            packet->waveAmplitude += waveRate;
            verticalStep = packet->verticalVelocity;
            packet->wavePhase += phaseStep;
            factor = sdfSin(packet->wavePhase);
            previousWaveValue = packet->previousWaveValue;
            packet->previousWaveValue = factor;
            factor = (factor - previousWaveValue) * packet->waveAmplitude;
            positionDelta.x = packet->vel[0] * factor;
            positionDelta.y = SDF_FRAME_STEP(verticalStep);
            positionDelta.z = packet->vel[2] * factor;
            SDF_VU0_LOADVECTOR(&positionDelta);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_STOREVECTOR(&positionDelta);
            packet->pos[0] += positionDelta.x;
            packet->pos[1] += positionDelta.y;
            packet->pos[2] += positionDelta.z;
            packet->verticalVelocity = verticalStep * verticalScale;
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR(packet);
                dds3Particle_EffectSet(&hData->head.sub, packetIndex, packet->color, packet->billboardScale);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                dds3Particle_EffectReset(&hData->head.sub, packetIndex);
                completedCount++;
                if (completedCount >= packetCount) {
                    hData->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

/* Stagger native tags using this variant's second tail word as the group period. */
void dds3Particle_SphereRingAllBuffInit(effTemplatePacketList_t *hData) {
    effectBufferRecord_t *packetCursor;
    u32 packetIndex;
    u32 nextPacketIndex;
    s32 packetTag;

    packetTag = EFF_PACKET_INITIAL_TAG;
    packetIndex = 0;
    packetCursor = hData->head.buffer->pPhysics;
    if (hData->head.particleCount != 0) {
        do {
            packetCursor->age = packetTag;
            nextPacketIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
            if (nextPacketIndex % hData->tailWords[1] == 0) {
                packetTag = packetTag - SDF_FRAMES(hData->decayStep);
            }
            packetIndex = nextPacketIndex;
        } while (packetIndex < hData->head.particleCount);
    }
}

/* Scale the position and record scale of a short template. */
void dds3Particle_SphereRingSetScale(effTemplatePacketList_t *hData, float scale) {
    hData->head.scale[0] = hData->head.scale[0] * scale;
    hData->head.scale[1] = hData->head.scale[1] * scale;
    hData->head.scale[2] = hData->head.scale[2] * scale;
    hData->recordScale = hData->recordScale * scale;
}

/* Clone the expanding-ring prefix/tail and initialize its period-grouped packet tags. */
effTemplatePacketList_t *dds3Particle_SphereRingCreate(effTemplatePacketList_t *source) {
    effTemplatePacketList_t *clone = sdfChipAllocDirect(0x170);
    s32 tailBytes = 0x20;

    memset(clone, 0, 0x170);
    memcpy(clone, source, source->head.templateSize);
    memcpy((u8 *)clone + EFF_TEMPLATE_TAIL_OFFSET, (u8 *)source + source->head.templateSize, tailBytes);
    dds3Particle_BasicCreate(clone);
    dds3Particle_SphereRingAllBuffInit(clone);
    return clone;
}

void dds3Particle_SphereRingDelete(void *hData) {
    dds3Particle_BasicDelete(hData);
    sdfChipFreeDirect(hData);
}

/* Seed radius and ring direction, placing the packet at its rotated vertical offset. */
void dds3Particle_SphereRingBuffInit(dds3Particle_SphereRing_t3 *pSphereRing, u32 number) {
    parSphereRingPacket_t *packet = (parSphereRingPacket_t *)pSphereRing->head.buffer->pPhysics;
    sdfVector4_t offsetVector;
    f32 jitter;
    f32 radius;
    f32 angle;

    packet += number;
    packet->age = -1;
    packet->color = 0;
    jitter = pSphereRing->radius_r;
    radius = pSphereRing->radius * (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter));
    packet->radius = radius;
    angle = (3.14159265f * 2.0f) / pSphereRing->simu_num * (number % pSphereRing->simu_num);
    packet->vel[0] = sdfCos(angle);
    packet->vel[1] = 0;
    packet->vel[2] = sdfSin(angle);
    SDF_VU0_LOADMATRIX(pSphereRing->head.matrix);
    offsetVector.y = radius;
    offsetVector.x = 0;
    offsetVector.z = 0;
    SDF_VU0_LOADVECTOR(&offsetVector);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_LOADVECTOR_B(pSphereRing->head.origin);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(packet);
    jitter = pSphereRing->head.speedJitter;
    packet->billboardScale = pSphereRing->head.scale[0] * (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter));
    jitter = pSphereRing->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    dds3Particle_EffectReset(&pSphereRing->head.sub, number);
}

/* Build age-phase ring positions and rotate the stored X/Z directions.
 * vector is reused for the position offset and old direction components. */
void dds3Particle_SphereRingDraw(dds3Particle_SphereRing_t3 *hData) {
    parSphereRingPacket_t *packet = (parSphereRingPacket_t *)hData->head.buffer->pPhysics;
    u32 subeffectKind = hData->head.sub.type;
    sdfVector4_t origin;
    sdfVector4_t vector;
    f32 rotationStep;
    f32 rotationCos;
    f32 rotationSin;
    s32 completedCount;
    s32 repeatEnabled;
    s32 packetCount;
    s32 lifetimeFrames;
    s32 packetIndex;

    dds3Particle_EffectInit(&hData->head.sub);
    SDF_VU0_LOADMATRIX(hData->head.matrix);
    rotationStep = SDF_FRAME_STEP(hData->roll_s * (3.14159265f / 180.0f));
    completedCount = 0;
    packetCount = hData->head.particleCount;
    lifetimeFrames = SDF_FRAMES(hData->head.lifetimeFrames);
    repeatEnabled = hData->loop;
    rotationCos = sdfCos(rotationStep);
    rotationSin = sdfSin(rotationStep);
    SDF_VU0_COPYVECTOR(&origin, hData->head.origin);
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++) {
        s32 age = packet->age;

        if (age == EFF_PACKET_INITIAL_TAG) {
            dds3Particle_SphereRingBuffInit(hData, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            f32 angle;
            f32 radius;
            f32 phaseCos;
            f32 phaseSin;

            packet->color = dds3ParticleColorAnim(&hData->head.fade, packet->color, age);
            packet->color = dds3ParticleColorBlend(packet->color, hData->head.colorMask);
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR_C(packet);
            }
            angle = (f32)age / (f32)lifetimeFrames * 3.14159265f;
            radius = packet->radius;
            phaseCos = sdfCos(angle);
            phaseSin = sdfSin(angle);
            vector.x = packet->vel[0] * (radius * phaseSin);
            vector.y = radius * phaseCos;
            vector.z = packet->vel[2] * (radius * phaseSin);
            SDF_VU0_LOADVECTOR(&vector);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_LOADVECTOR_B(&origin);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(packet);
            vector.x = packet->vel[0];
            vector.z = packet->vel[2];
            packet->vel[0] = rotationCos * vector.x + rotationSin * vector.z;
            packet->vel[2] = rotationCos * vector.z - rotationSin * vector.x;
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR(packet);
                dds3Particle_EffectSet(&hData->head.sub, packetIndex, packet->color, packet->billboardScale);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                dds3Particle_EffectReset(&hData->head.sub, packetIndex);
                completedCount++;
                if (completedCount >= packetCount) {
                    hData->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

/* Initialize only the native age/tag word of each packet record. */
void dds3Particle_Spark2AllBuffInit(effTemplatePacketList_t *hData) {
    effectBufferRecord_t *packetCursor;
    u32 packetIndex;

    packetIndex = 0;
    packetCursor = hData->head.buffer->pPhysics;
    if (hData->head.particleCount != 0) {
        do {
            packetCursor->age = EFF_PACKET_INITIAL_TAG;
            packetIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
        } while (packetIndex < hData->head.particleCount);
    }
}

/* Scale template position and three variant-specific tail values. */
void dds3Particle_Spark2SetScale(effTemplatePacketList_t *hData, float scale) {
    hData->head.scale[0] = hData->head.scale[0] * scale;
    hData->head.scale[1] = hData->head.scale[1] * scale;
    hData->head.scale[2] = hData->head.scale[2] * scale;
    hData->tailValues[0] = hData->tailValues[0] * scale;
    hData->tailValues[2] = hData->tailValues[2] * scale;
    hData->tailValues[3] = hData->tailValues[3] * scale;
}

/* Clone the cone prefix/tail, allocate packet records, and initialize their native age tags. */
effTemplatePacketList_t *dds3Particle_Spark2Create(effTemplatePacketList_t *source) {
    effTemplatePacketList_t *clone = sdfChipAllocDirect(0x180);
    s32 tailBytes = 0x30;

    memset(clone, 0, 0x180);
    memcpy(clone, source, source->head.templateSize);
    memcpy((u8 *)clone + EFF_TEMPLATE_TAIL_OFFSET, (u8 *)source + source->head.templateSize, tailBytes);
    dds3Particle_BasicCreate(clone);
    dds3Particle_Spark2AllBuffInit(clone);
    return clone;
}

void dds3Particle_Spark2Delete(void *hData) {
    dds3Particle_BasicDelete(hData);
    sdfChipFreeDirect(hData);
}

/* Seed mode-zero cone position/velocity; every mode initializes age and subeffect. */
void dds3Particle_Spark2BuffInit(dds3Particle_Spark2_t3 *pSpark2, s32 number) {
    parSparkPacket_t *packet = (parSparkPacket_t *)pSpark2->basic.buffer->pPhysics;
    f32 gravity;
    f32 sweepRadians;
    f32 angle;
    f32 radius;
    f32 cone;
    f32 speed;
    f32 angleCos;
    f32 angleSin;
    f32 jitter;

    packet += number;
    gravity = pSpark2->gravity / 100.0f;
    sweepRadians = pSpark2->degrees * (3.14159265f / 180.0f);
    radius = pSpark2->radius;
    cone = pSpark2->cone;
    speed = pSpark2->speed;
    if (pSpark2->mode == 0) {
        angle = sweepRadians / (u32)pSpark2->basic.particleCount * number;
        angleCos = sdfCos(angle);
        angleSin = sdfSin(angle);
        packet->pos[0] = angleCos * radius;
        packet->pos[1] = 0;
        packet->pos[2] = angleSin * radius;
        SDF_VU0_LOADMATRIX(pSpark2->basic.matrix);
        SDF_VU0_LOADVECTOR(packet->pos);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(packet->pos);
        packet->pos[0] += pSpark2->basic.origin[0];
        packet->pos[1] += pSpark2->basic.origin[1];
        packet->pos[2] += pSpark2->basic.origin[2];
        packet->vel[0] = angleCos * cone * speed;
        packet->vel[1] = -speed * (1.0f - cone);
        packet->vel[2] = angleSin * cone * speed;
        packet->verticalAcceleration = gravity;
    }
    packet->age = -(dds3GetRandA((effRandState_t *)ParRand) % (SDF_FRAMES(pSpark2->spread) + 1));
    packet->color = 0;
    jitter = pSpark2->basic.speedJitter;
    packet->billboardScale = pSpark2->basic.scale[0] * (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter));
    jitter = pSpark2->basic.spinJitter;
    if (jitter != 0) {
        packet->spin = (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    dds3Particle_EffectReset(&pSpark2->basic.sub, number);
}

/* Scale velocity, apply stored gravity, and transform each motion step before aging. */
void dds3Particle_Spark2Draw(dds3Particle_Spark2_t3 *hData) {
    parSparkPacket_t *packet = (parSparkPacket_t *)hData->basic.buffer->pPhysics;
    u32 subeffectKind = hData->basic.sub.type;
    sdfVector4_t positionDelta;
    f32 velocityScale;
    s32 completedCount;
    s32 lifetimeFrames;
    s32 packetCount;
    s32 repeatEnabled;
    s32 packetIndex;

    dds3Particle_EffectInit(&hData->basic.sub);
    SDF_VU0_LOADMATRIX(hData->basic.matrix);
    velocityScale = SDF_FRAME_DECAY(hData->decayPct / 100.0f + 1.0f);
    completedCount = 0;
    lifetimeFrames = SDF_FRAMES(hData->basic.lifetimeFrames);
    packetCount = hData->basic.particleCount;
    repeatEnabled = hData->loop;
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++) {
        s32 age = packet->age;

        if (age == EFF_PACKET_INITIAL_TAG) {
            dds3Particle_Spark2BuffInit(hData, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            packet->color = dds3ParticleColorAnim(&hData->basic.fade, packet->color, age);
            packet->color = dds3ParticleColorBlend(packet->color, hData->basic.colorMask);
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR_C(packet);
            }
            packet->vel[0] *= velocityScale;
            packet->vel[1] = packet->vel[1] * velocityScale + SDF_FRAME_STEP(packet->verticalAcceleration);
            packet->vel[2] *= velocityScale;
            SDF_VU0_LOADVECTOR(packet->vel);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_STOREVECTOR(&positionDelta);
            packet->pos[0] += SDF_FRAME_STEP(positionDelta.x);
            packet->pos[1] += SDF_FRAME_STEP(positionDelta.y);
            packet->pos[2] += SDF_FRAME_STEP(positionDelta.z);
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR(packet);
                dds3Particle_EffectSet(&hData->basic.sub, packetIndex, packet->color, packet->billboardScale);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                dds3Particle_EffectReset(&hData->basic.sub, packetIndex);
                completedCount++;
                if (completedCount >= packetCount) {
                    hData->basic.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

/* Initialize each packet's native tag and its template-selected uniform scale. */
void dds3Particle_SphereAtomAllBuffInit(effTemplatePacketList_t *hData) {
    s32 packetIndex = 0;
    effTemplateScaleRecord_t *packetCursor = (effTemplateScaleRecord_t *)hData->head.buffer->pPhysics;
    if (hData->head.particleCount != 0) {
        do {
            packetCursor->initialTag = EFF_PACKET_INITIAL_TAG;
            packetCursor->scale = hData->recordScale;
            packetIndex++;
            packetCursor++;
        } while ((u32)packetIndex < hData->head.particleCount);
    }
}

/* Scale the position, record scale, and first two tail values. */
void dds3Particle_SphereAtomSetScale(effTemplatePacketList_t *hData, float scale) {
    hData->head.scale[0] = hData->head.scale[0] * scale;
    hData->head.scale[1] = hData->head.scale[1] * scale;
    hData->head.scale[2] = hData->head.scale[2] * scale;
    hData->recordScale = hData->recordScale * scale;
    hData->tailValues[0] = hData->tailValues[0] * scale;
    hData->tailValues[1] = hData->tailValues[1] * scale;
}

/* Clone the prefix/tail and initialize each packet record with the template's uniform scale. */
effTemplatePacketList_t *dds3Particle_SphereAtomCreate(effTemplatePacketList_t *source) {
    effTemplatePacketList_t *clone = sdfChipAllocDirect(0x170);
    s32 tailBytes = 0x20;

    memset(clone, 0, 0x170);
    memcpy(clone, source, source->head.templateSize);
    memcpy((u8 *)clone + EFF_TEMPLATE_TAIL_OFFSET, (u8 *)source + source->head.templateSize, tailBytes);
    dds3Particle_BasicCreate(clone);
    dds3Particle_SphereAtomAllBuffInit(clone);
    return clone;
}

void dds3Particle_SphereAtomDelete(void *hData) {
    dds3Particle_BasicDelete(hData);
    sdfChipFreeDirect(hData);
}

/* vu0 routine: seed radial placement, normalized velocity and activation age. */
void dds3Particle_SphereAtomBuffInit(effTemplatePacketList_t *pSphereAtom, s32 number) {
    sdfVector4_t direction;
    effectBufferRecord_t *packet = pSphereAtom->head.buffer->pPhysics;
    f32 radius;
    f32 value;

    packet += number;
    if (pSphereAtom->randomSphere) {
        radius = pSphereAtom->recordScale;
        direction.x = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
        direction.y = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
        direction.z = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
        SDF_VU0_LOADVECTOR(&direction);
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR(&direction);
        packet->position[0] = radius * dds3GetRandFloatA(dds3EffectBase) * direction.x;
        packet->position[1] = radius * dds3GetRandFloatA(dds3EffectBase) * direction.y;
        packet->position[2] = radius * dds3GetRandFloatA(dds3EffectBase) * direction.z;
        SDF_VU0_LOADMATRIX(pSphereAtom->head.matrix);
        SDF_VU0_LOADVECTOR(packet->position);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(packet->position);
        packet->position[0] += pSphereAtom->head.origin[0];
        packet->position[1] += pSphereAtom->head.origin[1];
        packet->position[2] += pSphereAtom->head.origin[2];
        direction.x = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
        direction.y = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
        direction.z = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
        SDF_VU0_LOADVECTOR(&direction);
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR(packet->velocity);
        packet->age = -(dds3GetRandA((effRandState_t *)ParRand) % (SDF_FRAMES(pSphereAtom->delayRange) + 1));
    } else {
        SDF_VU0_COPYVECTOR(packet->position, pSphereAtom->head.origin);
        value = (3.14159265f * 2.0f) / pSphereAtom->head.particleCount * number;
        direction.x = sdfCos(value);
        direction.y = sdfSin(value);
        direction.z = 0;
        SDF_VU0_LOADVECTOR(&direction);
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR(packet->velocity);
        packet->age = -(dds3GetRandA((effRandState_t *)ParRand) % (SDF_FRAMES(pSphereAtom->delayRange) + 1));
    }
    packet->motionMagnitude = pSphereAtom->motionMagnitude;
    packet->color = 0;
    value = pSphereAtom->head.speedJitter;
    packet->scale = pSphereAtom->head.scale[0] * (dds3GetRandFloatA(dds3EffectBase) * value + (1.0f - value));
    value = pSphereAtom->head.spinJitter;
    if (value != 0) {
        packet->spin = (dds3GetRandFloatA(dds3EffectBase) * value + (1.0f - value)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    dds3Particle_EffectReset(&pSphereAtom->head.sub, number);
}

/* Reflect moving packets at the frame's entry radius. The first record's
 * updated radius becomes the boundary on the following frame. */
void dds3Particle_SphereAtomDraw(effTemplatePacketList_t *hData) {
    sdfVector4_t origin;
    sdfVector4_t previousPosition;
    /* Retail leaves the fourth broadcast components uninitialized. The
     * XYZW operations propagate those W lanes into the packet stores. */
    sdfVector4_t reflectionScale;
    sdfVector4_t normal;
    sdfVector4_t boundaryScale;
    s32 completedCount = 0;
    s32 repeatEnabled;
    parSphereAtomPacket_t *packet = (parSphereAtomPacket_t *)hData->head.buffer->pPhysics;
    u32 subeffectKind = hData->head.sub.type;
    s32 lifetimeFrames;
    s32 packetCount;
    s32 index;
    f32 radiusStep;
    f32 targetRadius;
    f32 radius;
    f32 velocityScale;

    dds3Particle_EffectInit(&hData->head.sub);
    lifetimeFrames = SDF_FRAMES((s32)hData->head.lifetimeFrames);
    targetRadius = hData->targetRadius;
    radiusStep = (targetRadius - hData->recordScale) / lifetimeFrames;
    velocityScale = SDF_FRAME_DECAY(hData->accelerationPct / 100.0f + 1.0f);
    packetCount = (s32)hData->head.particleCount;
    repeatEnabled = hData->restart;
    radius = packet->boundaryRadius;
    if (radiusStep >= 0.0f) {
        if (radius < targetRadius) {
            packet->boundaryRadius = radius + radiusStep;
            if (packet->boundaryRadius > targetRadius) {
                packet->boundaryRadius = targetRadius;
            }
        }
    } else {
        if (radius > targetRadius) {
            packet->boundaryRadius = radius + radiusStep;
            if (packet->boundaryRadius < targetRadius) {
                packet->boundaryRadius = targetRadius;
            }
        }
    }
    SDF_VU0_COPYVECTOR(&origin, hData->head.origin);
    for (index = 0; index < packetCount; index++, packet++) {
        s32 age = packet->age;
        f32 distance;
        f32 dot;
        f32 dx, dy, dz;
        f32 magnitude;

        if (age == EFF_PACKET_INITIAL_TAG) {
            dds3Particle_SphereAtomBuffInit(hData, index);
            age = packet->age;
        }
        if (age >= 0) {
            packet->color = dds3ParticleColorAnim(&hData->head.fade, packet->color, age);
            packet->color = dds3ParticleColorBlend(packet->color, hData->head.colorMask);
            if (subeffectKind != 0) {
                SDF_VU0_COPYVECTOR(&previousPosition, packet->pos);
            }
            magnitude = packet->motionMagnitude;
            dx = packet->vel[0] * magnitude;
            dy = packet->vel[1] * magnitude;
            dz = packet->vel[2] * magnitude;
            packet->pos[0] += SDF_FRAME_STEP(dx);
            packet->pos[1] += SDF_FRAME_STEP(dy);
            packet->pos[2] += SDF_FRAME_STEP(dz);
            magnitude *= velocityScale;
            packet->motionMagnitude = magnitude;
            SDF_VU0_LOADVECTOR(packet->pos);
            SDF_VU0_LOADVECTOR_B(&origin);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_COPYVECTOR_A_TO_C();
            SDF_VU0_GETLENGTH(distance);
            SDF_VU0_COPYVECTOR_C_TO_A();
            SDF_VU0_NORMALIZE();
            SDF_VU0_STOREVECTOR(&normal);
            if (distance > radius) {
                SDF_VU0_LOADVECTOR(packet->vel);
                SDF_VU0_LOADVECTOR_B(dds3ParticleNegOneVec);
                SDF_VU0_MULVECTOR();
                SDF_VU0_COPYVECTOR_A_TO_C();
                SDF_VU0_LOADVECTOR(&origin);
                SDF_VU0_LOADVECTOR_B(packet->pos);
                SDF_VU0_SUBVECTOR();
                SDF_VU0_NORMALIZE();
                SDF_VU0_COPYVECTOR_C_TO_B();
                SDF_VU0_COPYVECTOR_A_TO_C();
                SDF_VU0_INNERPRODUCT(dot);
                reflectionScale.x = dot * 2.0f;
                reflectionScale.y = dot * 2.0f;
                reflectionScale.z = dot * 2.0f;
                SDF_VU0_LOADVECTOR(&reflectionScale);
                SDF_VU0_COPYVECTOR_C_TO_B();
                SDF_VU0_MULVECTOR();
                SDF_VU0_LOADVECTOR_B(packet->vel);
                SDF_VU0_ADDVECTOR();
                SDF_VU0_NORMALIZE();
                SDF_VU0_STOREVECTOR(packet->vel);
                boundaryScale.x = radius;
                boundaryScale.y = radius;
                boundaryScale.z = radius;
                SDF_VU0_LOADVECTOR(&normal);
                SDF_VU0_LOADVECTOR_B(&boundaryScale);
                SDF_VU0_MULVECTOR();
                SDF_VU0_LOADVECTOR_B(&origin);
                SDF_VU0_ADDVECTOR();
                SDF_VU0_STOREVECTOR(packet->pos);
            }
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR_C(&previousPosition);
                SDF_VU0_LOADVECTOR(packet->pos);
                dds3Particle_EffectSet(&hData->head.sub, index, packet->color, packet->billboardScale);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                dds3Particle_EffectReset(&hData->head.sub, index);
                completedCount++;
                if (completedCount >= packetCount) {
                    hData->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

void dds3Particle_SpiralRandomize(effParticleOverlay_t *effect) {
    effInitParticleRecord(effect);
}

void effScaleSingleParticleTemplate(effEmitterHead_t *head, f32 factor) {
    head->scale[0] *= factor;
    head->scale[1] *= factor;
    head->scale[2] *= factor;
}

effEmitterHead_t *effCloneSingleParticleTemplate(effEmitterHead_t *source) {
    effEmitterHead_t *clone = sdfChipAllocDirect(0x150);
    /* This variant has no appended tail; retail retains the template-tail copy. */
    s32 tailBytes = 0;

    memset(clone, 0, 0x150);
    memcpy(clone, source, source->templateSize);
    memcpy((u8 *)clone + 0x150, (u8 *)source + source->templateSize, tailBytes);
    clone->particleCount = 1;
    clone->lifetimeFrames = 0;
    dds3Particle_BasicCreate(clone);
    return clone;
}

void effFreeSingleParticleTemplate(effEmitterHead_t *object) {
    dds3Particle_BasicDelete(object);
    sdfChipFreeDirect(object);
}

void effInitParticleRecord(effParticleOverlay_t *effect) {
    effParticleRecord_t *record = (effParticleRecord_t *)effect->buffer->pPhysics;
    f32 jitter;

    record->x = effect->x;
    record->color = effect->color | (effect->alpha << 24);
    record->frame = -1;
    record->y = effect->y;
    record->z = effect->z;
    record->speed = 1.0f;
    record->angle = 0.0f;
    effect->lastSpeed = effect->speed;
    jitter = effect->speedJitter;
    record->speed = effect->speed * (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter));
    jitter = effect->angleJitter;
    if (jitter != 0.0f) {
        record->angle = (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        record->angle = 0.0f;
    }
}

/* Frame zero reinitializes to -1 before incrementing; refresh position and modulated packed color. */
void dds3Particle_SpiralBlendColor(effParticleOverlay_t *effect) {
    effParticleRecord_t *record = (effParticleRecord_t *)effect->buffer->pPhysics;
    s32 frame = record->frame;
    u32 baseColor;

    if (frame == 0) {
        effInitParticleRecord(effect);
        frame = record->frame;
    }
    record->frame = frame + 1;
    record->x = effect->x;
    effect->drawEndFrame = frame + 2;
    record->y = effect->y;
    baseColor = effect->color | (effect->alpha << 24);
    record->z = effect->z;
    record->color = baseColor;
    record->color = dds3ParticleColorBlend(baseColor, effect->colorMask);
}

/* Initialize only the native age/tag word of each packet record. */
void dds3Particle_SparkOffsetAllBuffInit(effTemplatePacketList_t *hData) {
    effectBufferRecord_t *packetCursor;
    u32 packetIndex;

    packetIndex = 0;
    packetCursor = hData->head.buffer->pPhysics;
    if (hData->head.particleCount != 0) {
        do {
            packetCursor->age = EFF_PACKET_INITIAL_TAG;
            packetIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
        } while (packetIndex < hData->head.particleCount);
    }
}

/* Scale the position and three variant-specific tail values. */
void dds3Particle_SparkOffsetSetScale(effTemplatePacketList_t *hData, float scale) {
    hData->head.scale[0] = hData->head.scale[0] * scale;
    hData->head.scale[1] = hData->head.scale[1] * scale;
    hData->head.scale[2] = hData->head.scale[2] * scale;
    hData->tailValues[0] = hData->tailValues[0] * scale;
    hData->tailValues[2] = hData->tailValues[2] * scale;
    hData->tailValues[3] = hData->tailValues[3] * scale;
}

/* Clone the offset-gravity prefix/tail, allocate records, and initialize their native age tags. */
effTemplatePacketList_t *dds3Particle_SparkOffsetCreate(effTemplatePacketList_t *source) {
    effTemplatePacketList_t *clone = sdfChipAllocDirect(0x190);
    s32 tailBytes = 0x40;

    memset(clone, 0, 0x190);
    memcpy(clone, source, source->head.templateSize);
    memcpy((u8 *)clone + EFF_TEMPLATE_TAIL_OFFSET, (u8 *)source + source->head.templateSize, tailBytes);
    dds3Particle_BasicCreate(clone);
    dds3Particle_SparkOffsetAllBuffInit(clone);
    return clone;
}

void dds3Particle_SparkOffsetDelete(void *hData) {
    dds3Particle_BasicDelete(hData);
    sdfChipFreeDirect(hData);
}

/* vu0 routine: seed an offset-gravity packet; the displacement is tracked
 * apart from the emitter origin. */
/* NON_MATCHING in SMT3: 22 of 484 words differ (first at +0x90). */
void dds3Particle_SparkOffsetBuffInit(dds3Particle_SparkOffset_t3 *pSparkOffset, s32 number) {
    parSparkOffsetPacket_t *packet = (parSparkOffsetPacket_t *)pSparkOffset->head.buffer->pPhysics;
    sdfVector4_t position;
    sdfVector4_t direction;
    f32 gravity;
    f32 radius;
    f32 cone;
    f32 speed;
    f32 jitter;
    f32 decay;
    f32 angle;
    f32 angleCos;
    f32 angleSin;
    s32 steps;
    s32 step;

    packet += number;
    gravity = pSparkOffset->gravity / 100.0f;
    radius = pSparkOffset->radius;
    cone = pSparkOffset->cone;
    speed = pSparkOffset->speed;
    switch (pSparkOffset->mode) {
    case 0:
        if (pSparkOffset->randomDir != 0) {
            direction.x = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
            direction.y = 0;
            direction.z = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
            SDF_VU0_LOADVECTOR(&direction);
            SDF_VU0_NORMALIZE();
            SDF_VU0_STOREVECTOR(&direction);
            packet->offsetX = direction.x * radius;
            packet->offsetY = 0;
            packet->offsetZ = direction.z * radius;
            SDF_VU0_COPYVECTOR(packet, pSparkOffset->head.origin);
            SDF_VU0_LOADMATRIX(pSparkOffset->head.matrix);
            SDF_VU0_LOADVECTOR(&packet->offsetX);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_STOREVECTOR(&packet->offsetX);
            direction.x = (dds3GetRandFloatA((effRandState_t *)ParRand) - 0.5f) * 2.0f * cone;
            direction.y = -(1.0f - cone);
            direction.z = (dds3GetRandFloatA((effRandState_t *)ParRand) - 0.5f) * 2.0f * cone;
            SDF_VU0_LOADVECTOR(&direction);
            SDF_VU0_NORMALIZE();
            SDF_VU0_STOREVECTOR(&direction);
            jitter = dds3GetRandFloatA(dds3EffectBase) * pSparkOffset->jitter + (1.0f - pSparkOffset->jitter);
            packet->vel[0] = direction.x * speed * jitter;
            packet->vel[1] = direction.y * speed * jitter;
            packet->vel[2] = direction.z * speed * jitter;
            packet->offsetAcceleration = (dds3GetRandFloatA(dds3EffectBase) * 0.5f + 0.5f) * gravity;
        } else {
            angle = (3.14159265f * 2.0f) / pSparkOffset->head.particleCount * number;
            angleCos = sdfCos(angle);
            angleSin = sdfSin(angle);
            packet->pos[0] = angleCos * radius;
            packet->pos[1] = 0;
            packet->pos[2] = angleSin * radius;
            SDF_VU0_LOADMATRIX(pSparkOffset->head.matrix);
            SDF_VU0_LOADVECTOR(packet->pos);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_STOREVECTOR(packet->pos);
            packet->vel[0] = angleCos * cone * speed;
            packet->vel[1] = -speed * (1.0f - cone);
            packet->vel[2] = angleSin * cone * speed;
            packet->offsetAcceleration = gravity;
        }
        break;
    case 1:
        if (pSparkOffset->randomDir != 0) {
            packet->offsetX = 0;
            packet->offsetY = 0;
            packet->offsetZ = 0;
            SDF_VU0_COPYVECTOR(packet, pSparkOffset->head.origin);
            direction.x = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
            direction.y = -((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f);
            direction.z = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
            SDF_VU0_LOADVECTOR(&direction);
            SDF_VU0_NORMALIZE();
            SDF_VU0_STOREVECTOR(&direction);
            jitter = dds3GetRandFloatA(dds3EffectBase) * pSparkOffset->jitter + (1.0f - pSparkOffset->jitter);
            packet->vel[0] = direction.x * speed * jitter;
            packet->vel[1] = direction.y * speed * jitter;
            packet->vel[2] = direction.z * speed * jitter;
            packet->offsetAcceleration = gravity;
        } else {
            packet->pos[0] = 0;
            packet->pos[1] = 0;
            packet->pos[2] = 0;
            SDF_VU0_STOREIDENTITY(pSparkOffset->head.matrix);
            packet->vel[0] = dds3ParticleSparkOffsetDirTable[number % 12][0] * speed;
            packet->vel[1] = dds3ParticleSparkOffsetDirTable[number % 12][1] * speed;
            packet->vel[2] = dds3ParticleSparkOffsetDirTable[number % 12][2] * speed;
            packet->offsetAcceleration = gravity;
        }
        break;
    case 2:
        if (pSparkOffset->randomDir != 0) {
            decay = pSparkOffset->decayPct / 100.0f + 1.0f;
            SDF_VU0_STOREIDENTITY(pSparkOffset->head.matrix);
            direction.x = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
            direction.y = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
            direction.z = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
            SDF_VU0_LOADVECTOR(&direction);
            SDF_VU0_NORMALIZE();
            SDF_VU0_STOREVECTOR(&direction);
            jitter = dds3GetRandFloatA(dds3EffectBase) * pSparkOffset->jitter + (1.0f - pSparkOffset->jitter);
            steps = pSparkOffset->head.lifetimeFrames;
            direction.x = direction.x * (speed * jitter);
            direction.y = direction.y * (speed * jitter);
            direction.z = direction.z * (speed * jitter);
            packet->vel[0] = -direction.x;
            packet->vel[1] = -direction.y;
            packet->vel[2] = -direction.z;
            position.x = 0;
            position.y = 0;
            position.z = 0;
            step = 0;
            do {
                position.x += direction.x;
                position.y += direction.y;
                position.z += direction.z;
                direction.x = direction.x * decay;
                direction.y = direction.y * decay;
                direction.z = direction.z * decay;
                direction.y += gravity;
                step++;
            } while (step < steps);
            packet->pos[0] = position.x;
            packet->pos[1] = position.y;
            packet->pos[2] = position.z;
            packet->offsetAcceleration = -gravity;
        }
        break;
    }
    packet->age = -(dds3GetRandA((effRandState_t *)ParRand) % (SDF_FRAMES(pSparkOffset->spread) + 1));
    packet->color = 0;
    jitter = pSparkOffset->head.speedJitter;
    packet->billboardScale = pSparkOffset->head.scale[0] * (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter));
    jitter = pSparkOffset->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    dds3Particle_EffectReset(&pSparkOffset->head.sub, number);
}


/* Accumulate motion in the stored offset, then add the emitter origin.
 * Only mode zero transforms the per-frame displacement through the matrix. */
void dds3Particle_SparkOffsetDraw(dds3Particle_SparkOffset_t3 *hData) {
    parSparkOffsetPacket_t *packet = (parSparkOffsetPacket_t *)hData->head.buffer->pPhysics;
    u32 subeffectKind = hData->head.sub.type;
    u32 mode;
    sdfVector4_t positionDelta;
    sdfVector4_t origin;
    f32 velocityScale;
    s32 completedCount;
    s32 lifetimeFrames;
    s32 packetCount;
    s32 repeatEnabled;
    s32 packetIndex;

    dds3Particle_EffectInit(&hData->head.sub);
    mode = hData->mode;
    if (mode == 0) {
        SDF_VU0_LOADMATRIX(hData->head.matrix);
    }
    velocityScale = SDF_FRAME_DECAY(hData->decayPct / 100.0f + 1.0f);
    completedCount = 0;
    packetCount = hData->head.particleCount;
    lifetimeFrames = SDF_FRAMES(hData->head.lifetimeFrames);
    repeatEnabled = hData->loop;
    SDF_VU0_COPYVECTOR(&origin, hData->head.origin);
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++) {
        s32 age = packet->age;

        if (age == EFF_PACKET_INITIAL_TAG) {
            dds3Particle_SparkOffsetBuffInit(hData, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            packet->color = dds3ParticleColorAnim(&hData->head.fade, packet->color, age);
            packet->color = dds3ParticleColorBlend(packet->color, hData->head.colorMask);
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR_C(packet);
            }
            packet->vel[1] = packet->vel[1] * velocityScale + SDF_FRAME_STEP(packet->offsetAcceleration);
            packet->vel[0] *= velocityScale;
            packet->vel[2] *= velocityScale;
            SDF_VU0_LOADVECTOR(packet->vel);
            if (mode == 0) {
                SDF_VU0_APPLYMATRIX();
            }
            SDF_VU0_STOREVECTOR(&positionDelta);
            packet->offsetX += SDF_FRAME_STEP(positionDelta.x);
            packet->offsetY += SDF_FRAME_STEP(positionDelta.y);
            packet->offsetZ += SDF_FRAME_STEP(positionDelta.z);
            SDF_VU0_LOADVECTOR(&packet->offsetX);
            SDF_VU0_LOADVECTOR_B(&origin);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(packet);
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR(packet);
                dds3Particle_EffectSet(&hData->head.sub, packetIndex, packet->color, packet->billboardScale);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                dds3Particle_EffectReset(&hData->head.sub, packetIndex);
                completedCount++;
                if (completedCount >= packetCount) {
                    hData->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

/* Initialize only the native age/tag word of each packet record. */
void dds3Particle_SmokeOffsetAllBuffInit(effTemplatePacketList_t *hData) {
    effectBufferRecord_t *packetCursor;
    u32 packetIndex;

    packetIndex = 0;
    packetCursor = hData->head.buffer->pPhysics;
    if (hData->head.particleCount != 0) {
        do {
            packetCursor->age = EFF_PACKET_INITIAL_TAG;
            packetIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
        } while (packetIndex < hData->head.particleCount);
    }
}

/* Scale position, record scale, and two tail values. */
void dds3Particle_SmokeOffsetSetScale(effTemplatePacketList_t *hData, float scale) {
    hData->head.scale[0] = hData->head.scale[0] * scale;
    hData->head.scale[1] = hData->head.scale[1] * scale;
    hData->head.scale[2] = hData->head.scale[2] * scale;
    hData->recordScale = hData->recordScale * scale;
    hData->tailValues[0] = hData->tailValues[0] * scale;
    hData->tailValues[2] = hData->tailValues[2] * scale;
}

/* Clone the disc prefix/tail plus a separate 16-byte-per-packet auxiliary allocation. */
effTemplatePacketList_t *dds3Particle_SmokeOffsetCreate(effTemplatePacketList_t *source) {
    effTemplatePacketList_t *clone = sdfChipAllocDirect(0x200);
    sdfMemHandle_t *auxAllocationHandle;
    s32 tailBytes = 0xB0;

    memset(clone, 0, 0x200);
    memcpy(clone, source, source->head.templateSize);
    memcpy((u8 *)clone + EFF_TEMPLATE_TAIL_OFFSET, (u8 *)source + source->head.templateSize, tailBytes);
    auxAllocationHandle = sdfMemAllocDirect(clone->head.particleCount << 4);
    clone->auxiliaryAllocation = auxAllocationHandle;
    clone->auxiliaryData = (void *)sdfMemLock(auxAllocationHandle);
    dds3Particle_BasicCreate(clone);
    dds3Particle_SmokeOffsetAllBuffInit(clone);
    return clone;
}

void dds3Particle_SmokeOffsetDelete(effTemplatePacketList_t *hData) {
    sdfMemFreeDirect(hData->auxiliaryAllocation);
    dds3Particle_BasicDelete(hData);
    sdfChipFreeDirect(hData);
}

/* Seed the auxiliary position and packet direction separately, then the subeffect. */
void dds3Particle_SmokeOffsetBuffInit(dds3Particle_SmokeOffset_t3 *pSmokeOffset, u32 number) {
    parSmokePacket_t *packet = (parSmokePacket_t *)pSmokeOffset->head.buffer->pPhysics;
    f32 (*auxiliaryCursor)[4] = pSmokeOffset->aux;
    sdfVector4_t directionVector;
    f32 radialDistance;
    f32 jitter;

    packet += number;
    auxiliaryCursor += number;
    radialDistance = pSmokeOffset->radius * ((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f);
    directionVector.x = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
    directionVector.y = 0;
    directionVector.z = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
    SDF_VU0_LOADVECTOR(&directionVector);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&directionVector);
    (*auxiliaryCursor)[0] = directionVector.x * radialDistance;
    (*auxiliaryCursor)[1] = 0;
    (*auxiliaryCursor)[2] = directionVector.z * radialDistance;
    SDF_VU0_LOADMATRIX(pSmokeOffset->head.matrix);
    SDF_VU0_LOADVECTOR(*auxiliaryCursor);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(*auxiliaryCursor);
    directionVector.x = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
    directionVector.y = 0;
    directionVector.z = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
    SDF_VU0_LOADVECTOR(&directionVector);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(packet->vel);
    packet->waveAmplitude = pSmokeOffset->waveAmplitude * (dds3GetRandFloatA(dds3EffectBase) * pSmokeOffset->waveAmplitudeJitter + (1.0f - pSmokeOffset->waveAmplitudeJitter));
    packet->wavePhase = 0;
    packet->previousWaveValue = 0;
    packet->verticalVelocity = pSmokeOffset->verticalSpeed * (dds3GetRandFloatA(dds3EffectBase) * pSmokeOffset->verticalSpeedJitter + (1.0f - pSmokeOffset->verticalSpeedJitter));
    packet->age = -(dds3GetRandA((effRandState_t *)ParRand) % (SDF_FRAMES(pSmokeOffset->spread) + 1));
    packet->color = 0;
    SDF_VU0_COPYVECTOR(packet, pSmokeOffset->head.origin);
    jitter = pSmokeOffset->head.speedJitter;
    packet->billboardScale = pSmokeOffset->head.scale[0] * (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter));
    jitter = pSmokeOffset->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (dds3GetRandFloatA(dds3EffectBase) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    dds3Particle_EffectReset(&pSmokeOffset->head.sub, number);
}

/* Advance auxiliary positions, then transform/store packet positions with origin.
 * Auxiliary accumulation remains separate from the transformed packet position. */
void dds3Particle_SmokeOffsetDraw(dds3Particle_SmokeOffset_t3 *hData) {
    parSmokePacket_t *packet = (parSmokePacket_t *)hData->head.buffer->pPhysics;
    u32 subeffectKind = hData->head.sub.type;
    f32 (*auxiliaryCursor)[4] = hData->aux;
    sdfVector4_t positionVector;
    sdfVector4_t origin;
    f32 verticalScale;
    f32 phaseStep;
    f32 waveRate;
    s32 completedCount;
    s32 lifetimeFrames;
    s32 packetCount;
    s32 packetIndex;
    u32 repeatEnabled;

    dds3Particle_EffectInit(&hData->head.sub);
    SDF_VU0_LOADMATRIX(hData->head.matrix);
    phaseStep = SDF_FRAME_STEP(hData->phaseStepDegrees * (3.14159265f / 180.0f));
    verticalScale = SDF_FRAME_DECAY(hData->verticalAccelerationPct / 100.0f + 1.0f);
    completedCount = 0;
    packetCount = hData->head.particleCount;
    lifetimeFrames = SDF_FRAMES(hData->head.lifetimeFrames);
    repeatEnabled = hData->loop;
    waveRate = SDF_FRAME_STEP(hData->waveGrowth);
    SDF_VU0_COPYVECTOR(&origin, hData->head.origin);
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++, auxiliaryCursor++) {
        s32 age = packet->age;

        if (age == EFF_PACKET_INITIAL_TAG) {
            dds3Particle_SmokeOffsetBuffInit(hData, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            f32 verticalStep;
            f32 previousWaveValue;
            f32 factor;

            packet->color = dds3ParticleColorAnim(&hData->head.fade, packet->color, age);
            packet->color = dds3ParticleColorBlend(packet->color, hData->head.colorMask);
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR_C(packet);
            }
            packet->waveAmplitude += waveRate;
            verticalStep = packet->verticalVelocity;
            packet->wavePhase += phaseStep;
            factor = sdfSin(packet->wavePhase);
            previousWaveValue = packet->previousWaveValue;
            packet->previousWaveValue = factor;
            factor = (factor - previousWaveValue) * packet->waveAmplitude;
            positionVector.x = (*auxiliaryCursor)[0] + packet->vel[0] * factor;
            positionVector.y = (*auxiliaryCursor)[1] + SDF_FRAME_STEP(verticalStep);
            positionVector.z = (*auxiliaryCursor)[2] + packet->vel[2] * factor;
            SDF_VU0_LOADVECTOR(&positionVector);
            SDF_VU0_STOREVECTOR(*auxiliaryCursor);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_LOADVECTOR_B(&origin);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(packet);
            packet->verticalVelocity = verticalStep * verticalScale;
            if (subeffectKind != 0) {
                SDF_VU0_LOADVECTOR(packet);
                dds3Particle_EffectSet(&hData->head.sub, packetIndex, packet->color, packet->billboardScale);
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = EFF_PACKET_INITIAL_TAG;
            } else {
                dds3Particle_EffectReset(&hData->head.sub, packetIndex);
                completedCount++;
                if (completedCount >= packetCount) {
                    hData->head.active = 0;
                }
            }
        }
        packet->age = age;
    }
}

/* Initialize only the native age/tag word of each packet record. */
void dds3Particle_SphereSpiral2AllBuffInit(effTemplatePacketList_t *hData) {
    effectBufferRecord_t *packetCursor;
    u32 packetIndex;

    packetIndex = 0;
    packetCursor = hData->head.buffer->pPhysics;
    if (hData->head.particleCount != 0) {
        do {
            packetCursor->age = EFF_PACKET_INITIAL_TAG;
            packetIndex = packetIndex + 1;
            packetCursor = packetCursor + 1;
        } while (packetIndex < hData->head.particleCount);
    }
}

/* Scale template position, record scale, and the second tail value. */
void dds3Particle_SphereSpiral2SetScale(effTemplatePacketList_t *hData, float scale) {
    hData->head.scale[0] = hData->head.scale[0] * scale;
    hData->head.scale[1] = hData->head.scale[1] * scale;
    hData->head.scale[2] = hData->head.scale[2] * scale;
    hData->recordScale = hData->recordScale * scale;
    hData->tailValues[1] = hData->tailValues[1] * scale;
}

/* Allocate one 12-byte descriptor per packet and its attached 16-byte records. */
effTemplatePacketList_t *dds3Particle_SphereSpiral2Create(effTemplatePacketList_t *source) {
    effTemplatePacketList_t *clone = sdfChipAllocDirect(0x190);
    s32 tailLen = 0x40;
    s32 count;
    s32 perRecord;
    s32 listBytes;
    dds3ParOffsetBuffParts_t *history;
    dds3ParOffsetBuffParts_t *historyBase;
    s32 step;
    u8 *pointBase;
    s32 particleIndex;

    memset(clone, 0, 0x190);
    memcpy(clone, source, source->head.templateSize);
    memcpy((u8 *)clone + 0x150, (u8 *)source + source->head.templateSize, tailLen);
    dds3Particle_BasicCreate(clone);
    perRecord = 0;
    if (clone->head.sub.type != 0) {
        count = clone->head.particleCount;
        /* All four supported resource kinds use the same subrecord count. */
        switch (clone->head.sub.type) {
        case PAR_KIND_STATE_HISTORY:
            perRecord = clone->head.sub.templateEntryCount;
            break;
        case PAR_KIND_STATE_BILLBOARD_CROSS_STRIP:
            perRecord = clone->head.sub.templateEntryCount;
            break;
        case PAR_KIND_STATE_BILLBOARD_CROSS_TRIANGLE:
            perRecord = clone->head.sub.templateEntryCount;
            break;
        case PAR_KIND_STATE_TRACK_POLYGON_CROSS_AXES:
            perRecord = clone->head.sub.templateEntryCount;
            break;
        }
        listBytes = count * 12;
        clone->recordsPerPacket = perRecord;
        clone->listAllocation = sdfMemAllocDirect(listBytes + perRecord * count * 16);
        historyBase = sdfMemLock(clone->listAllocation);
        particleIndex = 0;
        pointBase = (u8 *)historyBase;
        clone->recordList = historyBase;
        pointBase += listBytes;
        if (count > 0) {
            step = perRecord * 16;
            history = historyBase;
            do {
                particleIndex++;
                history->points = (f32 (*)[3])pointBase;
                history->point = 0;
                history->count = 0;
                history++;
                pointBase += step;
            } while (particleIndex < count);
        }
    } else {
        clone->listAllocation = 0;
    }
    dds3Particle_SphereSpiral2AllBuffInit(clone);
    return clone;
}

/* Release the radial emitter's extra allocation, shared resources, and block. */
void dds3Particle_SphereSpiral2Delete(dds3Particle_SphereSpiral2_t3 *hData) {
    if (hData->childHandle != 0) {
        sdfMemFreeDirect(hData->childHandle);
    }
    dds3Particle_BasicDelete(hData);
    sdfChipFreeDirect(hData);
}

/* Initialize one 64-byte particle record and its kind-specific state. Negative
   ages delay activation; radial distance changes toward a jittered target over
   lifetimeFrames. RNG calls and the pre-transform length measurement stay in
   their original order. */
void dds3Particle_SphereSpiral2BuffInit(dds3Particle_SphereSpiral2_t3 *pSphereSpiral2, u32 number) {
    parBurstPacket_t *packet = (parBurstPacket_t *)pSphereSpiral2->head.buffer->pPhysics;
    sdfVector4_t direction;
    f32 initialRadius;
    f32 jitterFactor;
    f32 initialRadiusLength;

    packet += number;
    packet->color = 0;
    packet->age = -(dds3GetRandA((effRandState_t *)ParRand) % (SDF_FRAMES(pSphereSpiral2->spawnDelayFrames) + 1));
    initialRadius = pSphereSpiral2->initialRadius;
    jitterFactor = pSphereSpiral2->initialRadiusJitter;
    direction.x = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
    direction.y = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
    direction.z = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
    SDF_VU0_LOADVECTOR(&direction);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&direction);
    /* Each component gets a separate jitter sample, not one shared radius draw. */
    packet->radialOffset[0] = initialRadius * (dds3GetRandFloatA(dds3EffectBase) * jitterFactor + (1.0f - jitterFactor)) * direction.x;
    packet->radialOffset[1] = initialRadius * (dds3GetRandFloatA(dds3EffectBase) * jitterFactor + (1.0f - jitterFactor)) * direction.y;
    packet->radialOffset[2] = initialRadius * (dds3GetRandFloatA(dds3EffectBase) * jitterFactor + (1.0f - jitterFactor)) * direction.z;
    SDF_VU0_LOADVECTOR(packet->radialOffset);
    SDF_VU0_GETLENGTH(initialRadiusLength);
    SDF_VU0_LOADMATRIX(pSphereSpiral2->head.rot);
    SDF_VU0_LOADVECTOR(packet->radialOffset);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR_NOMEM(packet->radialOffset);
    packet->position[0] = packet->radialOffset[0] + pSphereSpiral2->head.origin[0];
    packet->position[1] = packet->radialOffset[1] + pSphereSpiral2->head.origin[1];
    packet->position[2] = packet->radialOffset[2] + pSphereSpiral2->head.origin[2];
    if (pSphereSpiral2->axisMode == PAR_BURST_RANDOM_AXIS) {
        direction.x = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
        direction.y = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
        direction.z = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
        SDF_VU0_LOADVECTOR(&direction);
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR(&direction);
        packet->rotationAxis[0] = direction.x;
        packet->rotationAxis[1] = direction.y;
        packet->rotationAxis[2] = direction.z;
    } else {
        packet->rotationAxis[0] = 0;
        packet->rotationAxis[1] = -1.0f;
        packet->rotationAxis[2] = 0;
    }
    jitterFactor = pSphereSpiral2->targetRadiusJitter;
    packet->radiusStep = (pSphereSpiral2->targetRadius * (dds3GetRandFloatA(dds3EffectBase) * jitterFactor +
                                    (1.0f - jitterFactor)) - initialRadiusLength) /
                   pSphereSpiral2->head.lifetimeFrames;
    jitterFactor = pSphereSpiral2->head.billboardScaleJitter;
    packet->billboardScale = pSphereSpiral2->head.billboardScale * (dds3GetRandFloatA(dds3EffectBase) * jitterFactor +
                                         (1.0f - jitterFactor));
    jitterFactor = pSphereSpiral2->head.initialPhaseJitter;
    if (jitterFactor != 0) {
        packet->initialPhaseRadians = (dds3GetRandFloatA(dds3EffectBase) * jitterFactor + (1.0f - jitterFactor)) *
                       (3.14159265f * 2.0f);
    } else {
        packet->initialPhaseRadians = 0;
    }
    dds3Particle_EffectReset(&pSphereSpiral2->head.kindState, number);
}

/* Grow each radial offset toward its target radius, spin it about the particle axis,
 * rebuild position from the origin, and feed the kind's point history. */
void dds3Particle_SphereSpiral2Draw(dds3Particle_SphereSpiral2_t3 *hData) {
    parBurstPacket_t *packet = (parBurstPacket_t *)hData->head.buffer->pPhysics;
    u32 kind;
    dds3ParOffsetBuffParts_t *history = hData->history.pParts;
    sdfVector4_t origin;
    sdfVector4_t previousOrigin;
    sdfVector4_t rotationAxis;
    sdfVector4_t scaleVector;
    f32 radius;
    f32 rotationStep;
    s32 completedCount;
    s32 lifetimeFrames;
    u32 capacity;
    s32 repeatEnabled;
    s32 packetCount;
    s32 packetIndex;

    capacity = hData->history.length;
    kind = hData->head.kindState.type;
    dds3Particle_EffectInit(&hData->head.kindState);
    SDF_VU0_LOADMATRIX_B(hData->head.rot);
    packetCount = hData->head.particleCount;
    lifetimeFrames = SDF_FRAMES(hData->head.lifetimeFrames);
    completedCount = 0;
    rotationStep = SDF_FRAME_STEP(hData->rotationStepDegrees * (3.14159265f / 180.0f));
    repeatEnabled = hData->loop;
    SDF_VU0_COPYVECTOR(&origin, hData->head.origin);
    previousOrigin.x = hData->previousOrigin[0];
    previousOrigin.y = hData->previousOrigin[1];
    previousOrigin.z = hData->previousOrigin[2];
    previousOrigin.w = 0;
    for (packetIndex = 0; packetIndex < packetCount; packetIndex++, packet++, history++) {
        s32 age = packet->age;

        if (age == PAR_PACKET_INITIAL_TAG) {
            dds3Particle_SphereSpiral2BuffInit(hData, packetIndex);
            age = packet->age;
        }
        if (age >= 0) {
            packet->color = dds3ParticleColorAnim(&hData->head.colorRamp, packet->color, age);
            packet->color = dds3ParticleColorBlend(packet->color, hData->head.abgr);
            if (kind != 0) {
                SDF_VU0_LOADVECTOR_C(packet->radialOffset);
            }
            SDF_VU0_LOADVECTOR(packet->radialOffset);
            SDF_VU0_GETLENGTH(radius);
            radius += SDF_FRAME_STEP(packet->radiusStep);
            scaleVector.x = radius;
            scaleVector.y = radius;
            scaleVector.z = radius;
            SDF_VU0_NORMALIZE();
            SDF_VU0_LOADVECTOR_B(&scaleVector);
            SDF_VU0_MULVECTOR();
            SDF_VU0_STOREVECTOR(packet->radialOffset);
            SDF_VU0_COPYMATRIX_B_TO_A();
            rotationAxis.x = packet->rotationAxis[0];
            rotationAxis.y = packet->rotationAxis[1];
            rotationAxis.z = packet->rotationAxis[2];
            rotationAxis.w = 0;
            SDF_VU0_LOADVECTOR(&rotationAxis);
            SDF_VU0_APPLYMATRIX_XYZ();
            SDF_VU0_STOREVECTOR_NOMEM(&rotationAxis);
            SetRotateMatrixAxis(rotationStep, (const sdfVector3_t *)&rotationAxis);
            SDF_VU0_LOADVECTOR(packet->radialOffset);
            SDF_VU0_APPLYMATRIX_XYZ();
            SDF_VU0_LOADVECTOR_B(&origin);
            SDF_VU0_STOREVECTOR(packet->radialOffset);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(packet->position);
            if (kind != 0) {
                u32 color;

                SDF_FRAME_WHEN(history->points[history->point][0] = packet->radialOffset[0]);
                SDF_FRAME_WHEN(history->points[history->point][1] = packet->radialOffset[1]);
                SDF_FRAME_WHEN(history->points[history->point][2] = packet->radialOffset[2]);
                color = (packet->color & 0xFF000000) | (hData->head.abgr & 0xFFFFFF);
                history->point += SDF_FRAME_DUE();
                if ((u32)history->point >= capacity) {
                    history->point = 0;
                }
                if ((u32)history->count < capacity) {
                    history->count += SDF_FRAME_DUE();
                }
                switch (hData->head.kindState.type) {
                case 1:
                    dds3Particle_BlurSetExp(hData->head.kindState.payload.history.table, packetIndex,
                                               &previousOrigin.x, packet->color, &hData->history,
                                               packet->billboardScale);
                    break;
                case 2:
                    dds3Particle_TrackSetExp(hData->head.kindState.payload.crossStrip.drawSystem,
                                               packetIndex, &previousOrigin.x, color, &hData->history);
                    break;
                case 3:
                    dds3Particle_Track2SetExp(hData->head.kindState.payload.crossTriangle.drawSystem,
                                                  packetIndex, &previousOrigin.x, color, &hData->history);
                    break;
                }
            }
        }
        age++;
        if (age >= lifetimeFrames) {
            if (repeatEnabled) {
                age = PAR_PACKET_INITIAL_TAG;
            } else {
                dds3Particle_EffectReset(&hData->head.kindState, packetIndex);
                completedCount++;
                if (completedCount >= packetCount) {
                    hData->head.state = 0;
                }
            }
            if (kind != 0) {
                history->point = 0;
                history->count = 0;
            }
        }
        packet->age = age;
    }
    hData->previousOrigin[0] = origin.x;
    hData->previousOrigin[1] = origin.y;
    hData->previousOrigin[2] = origin.z;
}

void dds3ParticleSetColorTable(dds3ParticleColorAnim_t *pColAnim, s32 frame) {
    u32 alpha;
    f32 span;
    u32 a, b;

    pColAnim->color0 &= 0xFFFFFF;
    pColAnim->color1 &= 0xFFFFFF;
    pColAnim->color2 &= 0xFFFFFF;
    alpha = pColAnim->alpha << 24;
    pColAnim->alpha_s_add = alpha;
    if (pColAnim->alpha_s > 0) {
        pColAnim->alpha_s_add = alpha / pColAnim->alpha_s;
    }
    pColAnim->alpha_e_sub = alpha;
    if (pColAnim->alpha_e > 0) {
        pColAnim->alpha_e_sub = alpha / pColAnim->alpha_e;
    }
    pColAnim->frame_end = frame;
    if (pColAnim->type == 1) {
        return;
    }
    if (pColAnim->type == 2) {
        pColAnim->frame_h = frame;
    } else {
        pColAnim->frame_h = frame >> 1;
    }
    span = pColAnim->frame_h;
    if (span == 0) {
        span = 1.0f;
    }
    a = pColAnim->color1;
    b = pColAnim->color0;
    pColAnim->r01 = ((f32)(a & 0xFF) - (f32)(b & 0xFF)) / span;
    pColAnim->g01 = ((f32)((a >> 8) & 0xFF) - (f32)((b >> 8) & 0xFF)) / span;
    pColAnim->b01 = ((f32)((a >> 16) & 0xFF) - (f32)((b >> 16) & 0xFF)) / span;
    if (pColAnim->type == 2) {
        return;
    }
    a = pColAnim->color2;
    b = pColAnim->color1;
    pColAnim->r12 = ((f32)(a & 0xFF) - (f32)(b & 0xFF)) / span;
    pColAnim->g12 = ((f32)((a >> 8) & 0xFF) - (f32)((b >> 8) & 0xFF)) / span;
    pColAnim->b12 = ((f32)((a >> 16) & 0xFF) - (f32)((b >> 16) & 0xFF)) / span;
}

u32 dds3ParticleColorAnim(dds3ParticleColorAnim_t *pColAnim, u32 color_now, s32 frame) {
    s32 components[3];
    u32 alpha;
    u32 sum;
    f32 t;

    if (frame >= SDF_FRAMES(pColAnim->frame_end - pColAnim->alpha_e)) {
        if (SDF_FRAMES(pColAnim->frame_end) < frame) {
            return 0;
        }
        alpha = pColAnim->alpha_e_sub * SDF_FRAME_STEP(SDF_FRAMES(pColAnim->frame_end) - frame);
        alpha &= 0xFF000000;
    } else if (frame <= SDF_FRAMES(pColAnim->alpha_s)) {
        alpha = pColAnim->alpha_s_add * SDF_FRAME_STEP(frame);
        alpha &= 0xFF000000;
    } else {
        alpha = pColAnim->alpha << 24;
    }
    if (pColAnim->type == 1) {
        return pColAnim->color0 | alpha;
    }
    t = SDF_FRAME_STEP(frame);
    if (frame < SDF_FRAMES(pColAnim->frame_h)) {
        s32 red = pColAnim->r01 * t;
        s32 green = pColAnim->g01 * t;
        s32 blue = pColAnim->b01 * t;
        sum = pColAnim->color0;
        components[0] = red;
        components[1] = green;
        components[2] = blue;
        sum += components[0];
        sum += (u32)components[1] << 8;
        sum += (u32)components[2] << 16;
        return (sum & 0xFFFFFF) | alpha;
    } else {
        s32 red, green, blue;
        t -= (f32)pColAnim->frame_h;
        red = pColAnim->r12 * t;
        green = pColAnim->g12 * t;
        blue = pColAnim->b12 * t;
        sum = pColAnim->color1;
        components[0] = red;
        components[1] = green;
        components[2] = blue;
        sum += components[0];
        sum += (u32)components[1] << 8;
        sum += (u32)components[2] << 16;
        return (sum & 0xFFFFFF) | alpha;
    }
}

/* Allocate contiguous point slabs, slot records and the trailing table header. */
dds3Particle_BlurBuff_t *dds3Particle_BlurCreate(u32 number, u16 length, billHandle_t **pBill_Handle) {
    u32 pointBytes = length * sizeof(u128);
    u32 slabBytes = pointBytes + sizeof(dds3Particle_BlurBuffList_t);
    u32 totalBytes = slabBytes * number;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(totalBytes + sizeof(dds3Particle_BlurBuff_t));
    u8 *base = (u8 *)sdfMemLock(allocation);
    dds3Particle_BlurBuff_t *table = (dds3Particle_BlurBuff_t *)(base + totalBytes);
    u128 *points = (u128 *)base;
    dds3Particle_BlurBuffList_t *slotBase = (dds3Particle_BlurBuffList_t *)(points + length * number);
    dds3Particle_BlurBuffList_t *slots;
    u32 index = 0;

    table->point = length;
    table->pBill_Handle = pBill_Handle;
    table->hMem = allocation;
    table->number = number;
    table->blurList = slotBase;
    if (number != 0) {
        slots = table->blurList;
        do {
            slots[index].pPos = points;
            slots[index].count = 0;
            slots[index].abgr = 0;
            points += length;
            index++;
        } while (index < number);
    }
    return table;
}

/* Release the slot table's allocation handle, not a separate node object. */
void dds3Particle_BlurDelete(dds3Particle_BlurBuff_t *hBlur) {
    sdfMemFreeDirect(hBlur->hMem);
}

void dds3Particle_BlurSet(dds3Particle_BlurBuff_t *table, s32 slotIndex, u32 color, f32 billboardScale) {
    dds3Particle_BlurBuffList_t *slot = &table->blurList[slotIndex];
    s32 last = table->point - 1;
    u128 *point = slot->pPos + last;
    s32 i;

    for (i = 0; i < last * SDF_FRAME_DUE(); i++) {
        point--;
        SDF_VU0_COPYVECTOR(point + 1, point);
    }
    if (slot->count <= last) {
        slot->count += SDF_FRAME_DUE();
    }
    SDF_FRAME_WHEN(SDF_VU0_STOREVECTOR_C(point));
    slot->abgr = color;
    slot->scale = billboardScale;
}

void dds3Particle_BlurSetExp(dds3Particle_BlurBuff_t *hBlur, s32 index, const f32 *pPosBase, u32 abgr,
                   dds3ParOffsetBuff_t *pOffBuff, f32 scale) {
    sdfVector4_t localOrigin;
    dds3ParOffsetBuffParts_t *history;
    dds3Particle_BlurBuffList_t *slot;
    f32 (*destination)[4];
    s32 pointCount;
    s32 pointCapacity;
    s32 pointIndex;

    SDF_VU0_COPYVECTOR(&localOrigin, pPosBase);
    history = &pOffBuff->pParts[index];
    slot = &hBlur->blurList[index];
    pointCount = history->count;
    pointCapacity = pOffBuff->length;
    slot->count = pointCount;
    destination = (f32 (*)[4])slot->pPos;
    pointIndex = history->point;
    if (pointCount > 0) {
        f32 (*points)[3] = history->points;
        s32 lastIndex = pointCapacity - 1;
        s32 remaining = pointCount;

        do {
            --remaining;
            pointIndex = pointIndex == 0 ? lastIndex : pointIndex - 1;
            (*destination)[0] = localOrigin.x + points[pointIndex][0];
            (*destination)[1] = localOrigin.y + points[pointIndex][1];
            (*destination)[2] = localOrigin.z + points[pointIndex][2];
            ++destination;
        } while (remaining != 0);
    }
    slot->abgr = abgr;
    slot->scale = scale;
}

void dds3Particle_BlurDraw(dds3Particle_BlurBuff_t *hBlur) {
    billHandle_t *billboard;
    dds3Particle_BlurBuffList_t *slot;
    s32 remainingSlots;
    s32 pointCount;
    s32 remainingPoints;
    u32 color;
    u32 alpha;
    u32 alphaStep;
    u128 *point;

    billboard = *hBlur->pBill_Handle;
    slot = hBlur->blurList;

    if (hBlur->number != 0) {
        remainingSlots = hBlur->number;

        do {
            pointCount = slot->count;
            color = slot->abgr;
            point = slot->pPos;
            alpha = color & 0xFF000000;
            alphaStep = alpha / (pointCount + 1);

            dds3BillboardSetScale(billboard, slot->scale, slot->scale);
            color &= 0xFFFFFF;
            if (pointCount != 0) {
                remainingPoints = pointCount;
                do {
                    SDF_VU0_COPYVECTOR(billboard, point);
                    alpha -= alphaStep;
                    --remainingPoints;
                    billboard->abgr = color | (alpha & 0xFF000000);
                    ++point;
                    dds3BillboardDraw(billboard);
                } while (remainingPoints != 0);
            }
            --remainingSlots;
            ++slot;
        } while (remainingSlots != 0);
    }
}

/* Disable this slot's billboard points by clearing its count, not a bit flag. */
void dds3Particle_BlurClearItem(dds3Particle_BlurBuff_t *table, s32 slotIndex) {
    table->blurList[slotIndex].count = 0;
}

/* Rebuild one slot's two-vertex strip cells from its point history: each cell is
   the offset point +/- the cross product of the segment direction with the view axis,
   scaled by the strip width. */
void dds3Particle_TrackSetExp(parSystem_t *hTrackPoly, s32 index, const f32 *pPos, u32 abgr, dds3ParOffsetBuff_t *pOffBuff) {
    dds3ParOffsetBuffParts_t *history = &pOffBuff->pParts[index];
    parCell_t *cell = &hTrackPoly->cells[index];
    u128 *vertex = cell->history;
    u32 count = history->count;
    sdfVector4_t point;
    sdfVector4_t localOrigin;

    if (count >= 2) {
        s32 segments;
        s32 segmentIndex;
        s32 lastIndex;
        s32 ring;

        SDF_VU0_COPYVECTOR(&localOrigin, pPos);
        segments = count - 1;
        cell->vertexCount = segments * 2;
        ring = history->point;
        lastIndex = pOffBuff->length - 1;
        ring = ring == 0 ? lastIndex : ring - 1;
        point.x = localOrigin.x + history->points[ring][0];
        point.y = localOrigin.y + history->points[ring][1];
        point.z = localOrigin.z + history->points[ring][2];
        point.w = 0;
        for (segmentIndex = 0; segmentIndex < segments; segmentIndex++) {
            SDF_VU0_LOADVECTOR(&point);
            ring = ring == 0 ? lastIndex : ring - 1;
            point.x = localOrigin.x + history->points[ring][0];
            point.y = localOrigin.y + history->points[ring][1];
            point.z = localOrigin.z + history->points[ring][2];
            SDF_VU0_LOADVECTOR_C(&point);
            SDF_VU0_COPYVECTOR_C_TO_B();
            SDF_VU0_SUBVECTOR();
            SDF_VU0_LOADVECTOR_B(parLook);
            SDF_VU0_OUTERPRODUCT();
            SDF_VU0_NORMALIZE();
            SDF_VU0_LOADVECTOR_B(parTrack_w);
            SDF_VU0_MULVECTOR();
            SDF_VU0_SWAPVECTOR_A_C();
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_ADDVECTOR_A_C();
            SDF_VU0_STOREVECTOR_NOMEM(vertex);
            SDF_VU0_COPYVECTOR_C_TO_A();
            SDF_VU0_SUBVECTOR_B_A();
            SDF_VU0_STOREVECTOR_B_NOMEM(vertex + 1);
            vertex += 2;
        }
        dds3TrackPolyVCRefresh_ParType1(hTrackPoly, index);
    }
    dds3TrackPolyColorSet(hTrackPoly, index, abgr);
}

/* Triangle-cell variant of dds3Particle_TrackSetExp: three vertices per history segment. */
void dds3Particle_Track2SetExp(parSystem_t *hTrackPoly, s32 index, const f32 *pPos, u32 abgr, dds3ParOffsetBuff_t *pOffBuff) {
    dds3ParOffsetBuffParts_t *history = &pOffBuff->pParts[index];
    parCell_t *cell = &hTrackPoly->cells[index];
    u128 *vertex = cell->history;
    u32 count = history->count;
    sdfVector4_t point;
    sdfVector4_t localOrigin;

    if (count >= 2) {
        s32 segments;
        s32 segmentIndex;
        s32 lastIndex;
        s32 ring;

        SDF_VU0_COPYVECTOR(&localOrigin, pPos);
        segments = count - 1;
        cell->vertexCount = segments * 3;
        ring = history->point;
        lastIndex = pOffBuff->length - 1;
        ring = ring == 0 ? lastIndex : ring - 1;
        point.x = localOrigin.x + history->points[ring][0];
        point.y = localOrigin.y + history->points[ring][1];
        point.z = localOrigin.z + history->points[ring][2];
        point.w = 0;
        for (segmentIndex = 0; segmentIndex < segments; segmentIndex++) {
            SDF_VU0_LOADVECTOR(&point);
            ring = ring == 0 ? lastIndex : ring - 1;
            point.x = localOrigin.x + history->points[ring][0];
            point.y = localOrigin.y + history->points[ring][1];
            point.z = localOrigin.z + history->points[ring][2];
            SDF_VU0_LOADVECTOR_C(&point);
            SDF_VU0_COPYVECTOR_C_TO_B();
            SDF_VU0_SUBVECTOR();
            SDF_VU0_LOADVECTOR_B(parLook);
            SDF_VU0_OUTERPRODUCT();
            SDF_VU0_NORMALIZE();
            SDF_VU0_LOADVECTOR_B(parTrack_w);
            SDF_VU0_MULVECTOR();
            SDF_VU0_SWAPVECTOR_A_C();
            SDF_VU0_STOREVECTOR_NOMEM(vertex + 1);
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_ADDVECTOR_A_C();
            SDF_VU0_STOREVECTOR_NOMEM(vertex);
            SDF_VU0_COPYVECTOR_C_TO_A();
            SDF_VU0_SUBVECTOR_B_A();
            SDF_VU0_STOREVECTOR_B_NOMEM(vertex + 2);
            vertex += 3;
        }
        dds3TrackPolyVCRefresh_ParType2(hTrackPoly, index);
    }
    dds3TrackPolyColorSet(hTrackPoly, index, abgr);
}

/* The colour inlines read and write a 16-byte slot, one word of it used. */
typedef struct {
    u32 word;
    u8 pad04[12];
} parColorWork_t;

/* vu0 routine: unpack two colours, multiply the lanes and pack */
u32 dds3ParticleColorBlend(u32 color0, u32 color1) {
    parColorWork_t work[3];
    u32 unit = 0x3C000000;
    u32 out;

    work[0].word = color0;
    MMI_RGBA_UNPACK(&work[0], unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    work[1].word = color1;
    MMI_RGBA_UNPACK(&work[1], unit);
    SDF_VU0_MULVECTOR();
    SDF_VU0_dds3ColorFtoI(out, 128.0f);
    return out;
}

/* Create through the selected dispatch entry and record its index; returns the
 * new instance, which dds3EffectCreate keeps. */
dds3Particle_Basic_t *dds3ParticleCreate(s32 dispatchIndex, void *creationData) {
    dds3Particle_Basic_t *createdObject;

    createdObject = dds3ParticleFuncTable[dispatchIndex].func(creationData);
    createdObject->type = dispatchIndex;
    return createdObject;
}

/* Invoke the selected destructor on the particle. */
void dds3ParticleDelete(dds3Particle_Basic_t *hData) {
    dds3ParticleFuncTable[hData->type].destroy(hData);
}

/* Draw live particles after applying the requested number of emitter updates. */
void dds3ParticleDraw(dds3Particle_Basic_t *hData) {
    effectBufferRecord_t *record;
    billHandle_t *billboard;
    u32 step;
    s32 particleCount;
    s32 lifetime;

    if (hData->state == 0) {
        return;
    }
    if (hData->pendingRestartSteps == 0) {
        dds3ParticleFuncTable[hData->type].update(hData);
    } else {
        for (step = 0; step < hData->pendingRestartSteps; step++) {
            dds3ParticleFuncTable[hData->type].update(hData);
        }
        hData->pendingRestartSteps = 0;
    }
    billboard = hData->hBillboard;
    record = hData->buffer->pPhysics;
    particleCount = hData->particleCount;
    lifetime = SDF_FRAMES(hData->lifetimeFrames);
    dds3BillboardSetScale(billboard, hData->billboardScale, hData->billboardScaleY);
    if (particleCount > 0) {
        step = particleCount;
        do {
            s32 age = record->age;

            if (age < lifetime && age >= 0) {
                SDF_VU0_COPYVECTOR(billboard, record->position);
                billboard->abgr = record->color;
                dds3BillboardDraw(billboard);
            }
            step--;
            record++;
        } while (step != 0);
    }
    SubmitKindDrawing(&hData->kindState);
}

/* Create another object of the same dispatch index and return the new instance. */
dds3Particle_Basic_t *dds3ParticleCopy(dds3Particle_Basic_t *obj) {
    dds3Particle_Basic_t *createdObject;

    createdObject = dds3ParticleFuncTable[obj->type].func(obj);
    createdObject->type = obj->type;
    return createdObject;
}

/* Reissue the callback, reload the native updater's repeat count, and arm restart. */
void dds3ParticleReset(dds3Particle_Basic_t *hData) {
    dds3ParticleAllBuffInitTable[hData->type](hData);
    hData->pendingRestartSteps = SDF_FRAMES(hData->d_frame);
    hData->state = 1;
}

/* Call the selected entry with factor, additionally scale kinds 2..4, then restart. */
void dds3ParticleSetScale(dds3Particle_Basic_t *hData, f32 scale) {
    dds3ParticleSetScaleTable[hData->type](hData, scale);
    switch (hData->kindState.type) {
    case PAR_KIND_STATE_BILLBOARD_CROSS_STRIP:
        hData->kindState.payload.crossStrip.scale *= scale;
        break;
    case PAR_KIND_STATE_BILLBOARD_CROSS_TRIANGLE:
        hData->kindState.payload.crossTriangle.scale *= scale;
        break;
    case PAR_KIND_STATE_TRACK_POLYGON_CROSS_AXES:
        hData->kindState.payload.trackPolygon.scale *= scale;
        break;
    }
    dds3ParticleReset(hData);
}

/* Return the full 16-bit restart state without converting it to a boolean. */
u16 dds3ParticleGetState(dds3Particle_Basic_t *hData) {
    return hData->state;
}

/* Copy one 16-byte vector with the existing EE/VU copy primitive. */
void dds3ParticleSetPosition(void *hData, void *pPos) {
    SDF_VU0_COPYVECTOR(hData, pPos);
}

/* vu0 routine: compose the supplied matrix with the emitter's source matrix. */
void dds3ParticleSetRotateMatrix(dds3Particle_Basic_t *hData, void *pMatrix) {
    SDF_VU0_LOADMATRIX(pMatrix);
    SDF_VU0_LOADMATRIX_B(hData->rot_b);
    MultMatrixNoArg();
    SDF_VU0_STOREMATRIX(hData->rot);
}

void dds3ParticleSetRGBA(dds3Particle_Basic_t *hData, u32 abgr) {
    hData->abgr = abgr;
}

void dds3ParticleSetTypeFlag(dds3Particle_Basic_t *object, s32 mode) {
    mode &= 0xFF;
    switch (object->type) {
    case 1:
    case 5:
    case 11:
        ((parObjClientView_t *)object)->mode150 = mode;
        break;
    case 0:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
    case 8:
    case 10:
    case 12:
        ((parObjClientView_t *)object)->mode151 = mode;
        break;
    case 9:
        break;
    }
    object->state = 1;
}

u8 dds3ParticleGetLoop(dds3Particle_Basic_t *hData) {
    switch (hData->type) {
    case 1:
    case 5:
    case 11:
        return ((parObjClientView_t *)hData)->mode150;
    case 0:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
    case 8:
    case 10:
    case 12:
        return ((parObjClientView_t *)hData)->mode151;
    default:
        return 0;
    }
}

dds3Particle_Basic_t *dds3ParticleExpCreate(s32 kind, parKindResource_t *resource) {
    dds3Particle_Basic_t *object = (dds3Particle_Basic_t *)((u8 *)resource + resource->offset + 0x10);
    billHandle_t *billboard;

    if (resource->type != 3 || resource->offset != 0) {
        object->numberO = PAR_BILLBOARD_CLONE_FROM_RESOURCE;
        object = dds3ParticleFuncTable[kind].func(object);
        billboard = dds3BillboardCreate(resource->type, EE_ADDR((resource + 1)));
        dds3BillboardSetScale(billboard, object->billboardScale, object->billboardScaleY);
        dds3BillboardSetMode(billboard, object->billboardMode);
        dds3BillboardSetAnimColorAdvMode(billboard);
        object->hBillboard = billboard;
    } else {
        object = dds3ParticleFuncTable[kind].func(object);
    }
    object->type = kind;
    return object;
}

dds3Particle_Basic_t *dds3ParticleExpCopy(dds3Particle_Basic_t *source) {
    dds3Particle_Basic_t *particle = dds3ParticleFuncTable[source->type].func(source);
    particle->type = source->type;
    if (source->numberO == PAR_BILLBOARD_CLONE_FROM_RESOURCE) {
        billHandle_t *billboard = dds3BillboardCopy(source->hBillboard);
        dds3BillboardSetScale(billboard, particle->billboardScale, particle->billboardScaleY);
        dds3BillboardSetMode(billboard, particle->billboardMode);
        dds3BillboardSetAnimColorAdvMode(billboard);
        particle->hBillboard = billboard;
    }
    return particle;
}

void dds3ParticleExpDelete(dds3Particle_Basic_t *hData) {
    dds3ParticleFuncTable[hData->type].destroy(hData);
}

/* Apply queued updates, then draw live records and submit the kind's resource. */
void dds3ParticleExpDraw(dds3Particle_Basic_t *hData) {
    effectBufferRecord_t *record;
    billHandle_t *billboard;
    u32 step;
    s32 particleCount;
    s32 lifetime;
    f32 scaleStep;
    f32 spinStep;
    f32 spinDirection;
    f32 alternate;

    if (hData->state == 0) {
        return;
    }
    if (hData->pendingRestartSteps == 0) {
        dds3ParticleFuncTable[hData->type].update(hData);
    } else {
        for (step = 0; step < hData->pendingRestartSteps; step++) {
            dds3ParticleFuncTable[hData->type].update(hData);
        }
        particleCount = hData->particleCount;
        scaleStep = (hData->scaleEnd - hData->billboardScale) / SDF_FRAMES(hData->lifetimeFrames);
        spinStep = SDF_FRAME_STEP(hData->spinDegrees * (3.14159265f / 180.0f));
        alternate = hData->alternateSpin ? -1.0f : 1.0f;
        for (step = 0; step < hData->pendingRestartSteps; step++) {
            s32 recordIndex;

            record = hData->buffer->pPhysics;
            spinDirection = 1.0f;
            for (recordIndex = 0; recordIndex < particleCount; recordIndex++, record++) {
                if (record->age >= 0) {
                    record->scale += scaleStep;
                    record->spin += spinStep * spinDirection;
                }
                spinDirection *= alternate;
            }
        }
        hData->pendingRestartSteps = 0;
    }
    /* Capture the draw owners before deriving this frame's scale/spin steps. */
    record = hData->buffer->pPhysics;
    billboard = hData->hBillboard;
    particleCount = hData->particleCount;
    scaleStep = (hData->scaleEnd - hData->billboardScale) / SDF_FRAMES(hData->lifetimeFrames);
    lifetime = SDF_FRAMES(hData->lifetimeFrames);
    spinStep = SDF_FRAME_STEP(hData->spinDegrees * (3.14159265f / 180.0f));
    alternate = hData->alternateSpin ? -1.0f : 1.0f;
    spinDirection = 1.0f;
    if (particleCount > 0) {
        step = particleCount;
        do {
            s32 age = record->age;

            if (age < lifetime && age >= 0) {
                SDF_VU0_COPYVECTOR(billboard, record->position);
                billboard->rot = record->spin;
                billboard->abgr = record->color;
                billboard->childScaleX = billboard->childScaleY = record->scale;
                record->scale += scaleStep;
                if (record->scale < 0) {
                    record->scale = 0;
                }
                record->spin += spinStep * spinDirection;
                dds3BillboardSetAnimKey(billboard, SDF_FRAME_STEP(age));
                dds3BillboardDraw(billboard);
            }
            spinDirection *= alternate;
            --step;
            ++record;
        } while (step != 0);
    }
    SubmitKindDrawing(&hData->kindState);
}

void dds3ParticleExpReset(dds3Particle_Basic_t *hData) {
    dds3ParticleReset(hData);
}

void dds3ParticleExpSetScale(dds3Particle_Basic_t *hData, float scale) {
    dds3ParticleSetScale(hData, scale);
    hData->scaleEnd *= scale;
}

/* Descriptor query: return the complete 16-bit restart state. */
u16 dds3ParticleExpGetState(dds3Particle_Basic_t *hData) {
    return dds3ParticleGetState(hData);
}

void dds3ParticleExpSetPosition(void *hData, void *pPos) {
    SDF_VU0_COPYVECTOR(hData, pPos);
}

void dds3ParticleExpSetRotateMatrix(dds3Particle_Basic_t *hData, void *pMatrix) {
    dds3ParticleSetRotateMatrix(hData, pMatrix);
}

void dds3ParticleExpSetRGBA(dds3Particle_Basic_t *hData, u32 abgr) {
    hData->abgr = abgr;
}

void dds3ParticleExpSetLoop(dds3Particle_Basic_t *hData, u8 loop) {
    dds3ParticleSetTypeFlag(hData, loop);
}

/* Descriptor query: select the kind's current mode byte. */
u8 dds3ParticleExpGetLoop(dds3Particle_Basic_t *hData) {
    return dds3ParticleGetLoop(hData);
}

/* Kinds 2-4 keep the scale at +8 of their own record; copy it into the
 * shared vector and store the (vf10 - vf11) difference. */
void dds3Particle_EffectInit(dds3Particle_Effect_t *obj) {
    f32 scale;

    switch (obj->type) {
    case 0:
    case PAR_KIND_STATE_HISTORY:
        return;
    case PAR_KIND_STATE_BILLBOARD_CROSS_STRIP:
        scale = obj->payload.crossStrip.scale;
        parTrack_w[0] = parTrack_w[1] = parTrack_w[2] = scale;
        break;
    case PAR_KIND_STATE_BILLBOARD_CROSS_TRIANGLE:
        scale = obj->payload.crossTriangle.scale;
        parTrack_w[0] = parTrack_w[1] = parTrack_w[2] = scale;
        break;
    case PAR_KIND_STATE_TRACK_POLYGON_CROSS_AXES:
        scale = obj->payload.trackPolygon.scale;
        parTrack_w[0] = parTrack_w[1] = parTrack_w[2] = scale;
        break;
    default:
        return;
    }
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(parLook);
}

void dds3Particle_EffectSet(dds3Particle_Effect_t *pEffect, s32 index, u32 abgr, f32 scale) {
    switch ((u16)pEffect->type) {
    case 1:
        dds3Particle_BlurSet(pEffect->payload.history.table, index, abgr, scale);
        return;
    case 2:
        dds3Particle_TrackSet(pEffect->payload.crossStrip.drawSystem, index, abgr);
        return;
    case 3:
        dds3Particle_Track2Set(pEffect->payload.crossTriangle.drawSystem, index, abgr);
        return;
    case 4:
        dds3Particle_Track3Set(pEffect->payload.trackPolygon.modelList, index, abgr);
        break;
    }
}

void dds3Particle_EffectReset(dds3Particle_Effect_t *pEffect, s32 number) {
    switch ((u16)pEffect->type) {
    case 1:
        dds3Particle_BlurClearItem(pEffect->payload.history.table, number);
        return;
    case 2:
        dds3TrackPolyListInit(pEffect->payload.crossStrip.drawSystem, number);
        return;
    case 3:
        dds3TrackPolyListInit(pEffect->payload.crossTriangle.drawSystem, number);
        return;
    case 4:
        Eff_ModelTrackPolyListReset(pEffect->payload.trackPolygon.modelList, number);
        break;
    }
}

void dds3Particle_TrackSet(parSystem_t *hTrackPoly, s32 i, u32 abgr) {
    u128 axis[2];
    SDF_VU0_COPYVECTOR_C_TO_B();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(parLook);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(parTrack_w);
    SDF_VU0_MULVECTOR();
    SDF_VU0_SWAPVECTOR_A_C();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(&axis[0]);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_SUBVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B(&axis[1]);
    dds3TrackPolySet(hTrackPoly, i, axis);
    dds3TrackPolyVCRefresh_ParType1(hTrackPoly, i);
    dds3TrackPolyColorSet(hTrackPoly, i, (abgr & 0xFF000000) | 0x808080);
}

void dds3Particle_Track2Set(parSystem_t *hTrackPoly, s32 i, u32 abgr) {
    u128 axis[3];
    SDF_VU0_COPYVECTOR_C_TO_B();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(parLook);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(parTrack_w);
    SDF_VU0_MULVECTOR();
    SDF_VU0_SWAPVECTOR_A_C();
    SDF_VU0_STOREVECTOR(&axis[1]);
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(&axis[0]);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_SUBVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B(&axis[2]);
    dds3TrackPolySet2(hTrackPoly, i, axis);
    dds3TrackPolyVCRefresh_ParType2(hTrackPoly, i);
    dds3TrackPolyColorSet(hTrackPoly, i, (abgr & 0xFF000000) | 0x808080);
}

void dds3Particle_Track3Set(effTrackPolyList_t *hTrackPoly, s32 i, u32 abgr) {
    u128 axis[2];
    SDF_VU0_COPYVECTOR_C_TO_B();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(parLook);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(parTrack_w);
    SDF_VU0_MULVECTOR();
    SDF_VU0_SWAPVECTOR_A_C();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(&axis[0]);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_SUBVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B(&axis[1]);
    Eff_ModelTrackPolyListVertexSet(hTrackPoly, i, axis);
    Eff_ModelTrackPolyListColorSet(hTrackPoly, i, (abgr & 0xFF000000) | 0x808080);
}

u32 dds3Particle_TrackPolyGetRecTiming(void) {
    return 0;
}
