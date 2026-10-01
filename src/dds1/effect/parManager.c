#include "common.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"

typedef struct {
    u8 pad[0x30];    /* 0x0 */
    u16 kind;        /* 0x30 */
    u8 pad32[6];     /* 0x32 */
    f32 scale;       /* 0x38 */
    u8 pad3C[0x68];  /* 0x3C */
    u32 unkA4;       /* 0xA4 copied to unkFC by parRestartKind */
    u8 padA8[0x54];  /* 0xA8 */
    void *unkFC;     /* 0xFC */
    u8 pad100[0x40]; /* 0x100 */
    u16 dispatchIndex; /* 0x140 selects D_0034E250/D_0034E258/D_0034E2F0 */
    u16 restartFlag; /* 0x142 read by parGetRestartFlag, set to 1 by parRestartKind */
    u8 pad144[0x30]; /* 0x144 */
    void *child;      /* 0x174 released by parReleaseObject */
} ParObj;

typedef struct {
    u8 pad[0xC]; /* 0x0 */
    void *resource;  /* 0xC released by effParReleaseNodeResource */
} ParNode;

typedef struct {
    u8 pad[4];   /* 0x0 */
    u16 flag;    /* 0x4 cleared by parClearSlotFlag */
    u8 pad6[10]; /* 0x6 */
} ParSlot; /* 0x10 bytes */

typedef struct {
    u8 pad[4];     /* 0x0 */
    ParSlot *slots; /* 0x4 */
} ParTable;


/* Particle dispatch entry (0xC bytes): command func selected by the
   u16 at +0x140. */
typedef struct {
    void *(*func)(); /* 0x0 */
    u32 unk4;        /* 0x4 */
    u32 unk8;        /* 0x8 */
} ParDispatch; /* 0xC bytes */

extern ParDispatch D_0034E250[];
extern ParDispatch D_0034E258[];
extern void (*D_0034E2F0[])();

void func_002D0918(void *arg);
void effDestroyResources(void *arg);
void sdfReleaseChipBlock(void *arg);
extern void sdfComposeVuMatrixFromRegisters(void);

void parReleaseObject(ParObj *obj) {
    if (obj->child != NULL) {
        func_002D0918(obj->child);
    }
    effDestroyResources(obj);
    sdfReleaseChipBlock(obj);
}

typedef struct {
    u8 pad00[4];
    void *records;
} ParBurstBuffer;

typedef struct {
    f32 origin[4];
    f32 headSpeed;
    u8 pad14[0x10];
    s32 frameCount;
    u8 pad28[8];
    u8 sub[0x64];
    f32 speedJitter;
    f32 spinJitter;
    u8 pad9C[0x14];
    f32 matrix[16];
    u8 padF0[8];
    ParBurstBuffer *buffer;
    u8 padFC[0x54];
    u8 mode;
    u8 pad151[3];
    u32 spread;
    f32 speed;
    f32 unk15C;
    f32 f160;
    f32 jitterA;
    f32 jitterB;
} ParBurstEmitter;

typedef struct {
    f32 pos[4];
    f32 vel[3];
    u8 pad1C[4];
    s32 age;
    u32 color;
    f32 speed;
    f32 spin;
    f32 axis[3];
    f32 rate;
} ParBurstPacket;

extern s32 effMiscRand(void *);
extern f32 effMiscRandUnitFloat(void *);
extern u8 D_0034DF38[];
extern u8 D_003D6480[];

void func_00158FA8(ParBurstEmitter *effect, u32 index) {
    ParBurstPacket *packet = (ParBurstPacket *)effect->buffer->records;
    f32 tmp[4];
    f32 speed;
    f32 jitter;
    f32 length;

    packet += index;
    packet->color = 0;
    packet->age = -(effMiscRand(D_003D6480) % (effect->spread + 1));
    speed = effect->speed;
    jitter = effect->jitterA;
    tmp[0] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    tmp[1] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    tmp[2] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, tmp);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, tmp);
    packet->vel[0] = speed * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter)) * tmp[0];
    packet->vel[1] = speed * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter)) * tmp[1];
    packet->vel[2] = speed * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter)) * tmp[2];
    VU0_LOAD_VF(vf10, packet->vel);
    VU0_LENGTH_VF10(length);
    VU0_LOAD_MATRIX(effect->matrix);
    VU0_LOAD_VF(vf10, packet->vel);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF_UNCLOBBERED(vf10, packet->vel);
    packet->pos[0] = packet->vel[0] + effect->origin[0];
    packet->pos[1] = packet->vel[1] + effect->origin[1];
    packet->pos[2] = packet->vel[2] + effect->origin[2];
    if (effect->mode == 1) {
        tmp[0] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
        tmp[1] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
        tmp[2] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
        VU0_LOAD_VF(vf10, tmp);
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF(vf10, tmp);
        packet->axis[0] = tmp[0];
        packet->axis[1] = tmp[1];
        packet->axis[2] = tmp[2];
    } else {
        packet->axis[0] = 0;
        packet->axis[1] = -1.0f;
        packet->axis[2] = 0;
    }
    jitter = effect->jitterB;
    packet->rate = (effect->f160 * (effMiscRandUnitFloat(D_0034DF38) * jitter +
                                    (1.0f - jitter)) - length) /
                   effect->frameCount;
    jitter = effect->speedJitter;
    packet->speed = effect->headSpeed * (effMiscRandUnitFloat(D_0034DF38) * jitter +
                                         (1.0f - jitter));
    jitter = effect->spinJitter;
    if (jitter != 0) {
        packet->spin = (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter)) *
                       (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    parDispatchKindInit(effect->sub, index);
}

INCLUDE_ASM(const s32, "effect/parManager", func_00159308);

INCLUDE_ASM(const s32, "effect/parManager", func_001596E8);

INCLUDE_ASM(const s32, "effect/parManager", func_00159AB8);

INCLUDE_ASM(const s32, "effect/parManager", func_00159C08);

void effParReleaseNodeResource(ParNode *node) {
    func_002D0918(node->resource);
}

INCLUDE_ASM(const s32, "effect/parManager", func_00159CF0);

INCLUDE_ASM(const s32, "effect/parManager", func_00159D68);

INCLUDE_ASM(const s32, "effect/parManager", func_00159E20);

void parClearSlotFlag(ParTable *table, s32 index) {
    table->slots[index].flag = 0;
}

INCLUDE_ASM(const s32, "effect/parManager", func_00159F48);

INCLUDE_ASM(const s32, "effect/parManager", func_0015A118);

/* vu0 routine: modulate two RGBA8888 colours, (a/128 * b/128) * 128 per channel */
u32 effParModulateColors(u32 colorA, u32 colorB) {
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit = 0x3C000000;
    color1[0] = colorA;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = colorB;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    return blended[0];
}

void parCreateIndexed(s32 index, void *arg) {
    ParObj *newobj;

    newobj = D_0034E250[index].func(arg);
    newobj->dispatchIndex = index;
}

void parDispatchByKind(ParObj *obj) {
    D_0034E258[obj->dispatchIndex].func();
}

INCLUDE_ASM(const s32, "effect/parManager", func_0015A3F8);

void parCloneKind(ParObj *obj) {
    ParObj *newobj;

    newobj = D_0034E250[obj->dispatchIndex].func();
    newobj->dispatchIndex = obj->dispatchIndex;
}

/* Reissues the dispatch callback and arms the restart flag. */

void parRestartKind(ParObj *obj) {
    D_0034E2F0[obj->dispatchIndex]();
    obj->unkFC = (void *)obj->unkA4;
    obj->restartFlag = 1;
}

extern void (*D_0034E328[])(ParObj *, f32);

/* Reissue the dispatch callback, scale the kind-specific value for kinds 2..4, then restart. */
void parScaleAndRestartKind(ParObj *obj, f32 factor) {
    D_0034E328[obj->dispatchIndex](obj, factor);
    switch (obj->kind) {
    case 2:
        obj->scale *= factor;
        break;
    case 3:
        obj->scale *= factor;
        break;
    case 4:
        obj->scale *= factor;
        break;
    }
    parRestartKind(obj);
}

u16 parGetRestartFlag(ParObj *obj) {
    return obj->restartFlag;
}

void func_0015A6E8(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

/* vu0 routine: effect+0xB0 = matrix * effect+0x100 via sdfComposeVuMatrixFromRegisters */
void parComposeEffectTransformMatrices(u8 *effect, void *matrix) {
    VU0_LOAD_MATRIX(matrix);
    VU0_LOAD_MATRIX_B(effect + 0x100);
    sdfComposeVuMatrixFromRegisters();
    VU0_STORE_MATRIX(effect + 0xB0);
}
