#include "common.h"
#include "pcp_vu0.h"

extern s32 D_00436530;

extern u32 D_00436534;

extern u32 D_00436538;

typedef struct {
    u8 bytes[0x30];
} __attribute__((packed)) FileRecordHeader;

/* Event work shared by resource setup, teardown and state updates. */
typedef struct EffEventWork {
    u8 pad00[4];
    u32 owner;              /* 0x04 */
    u8 initBlock[0x28];    /* 0x08: file-record header source */
    u32 state;              /* 0x30 */
    u32 effect;             /* 0x34 */
    u8 pad38[0x48];
    u8 flag;                /* 0x80 */
    u8 pad81[3];
    u32 resource;           /* 0x84 */
} EffEventWork;

extern u32 func_00197D38();
extern struct EffEventWork *func_00197D68();
extern void *func_00328D68(s32 size);

extern u8 D_003B2D20[];

/* Release the attached effect before freeing the event work. */
void effEventReleaseNode(EffEventWork *work) {
    func_001686F0(work->effect);
    func_00328E48(work);
}

void effEventCopyFileRecordHeader(FileRecordHeader *destination, const FileRecordHeader *source) {
    *destination = *source;
}

void func_00197ED8(const FileRecordHeader *source, FileRecordHeader *destination) {
    *destination = *source;
}

void func_00197F40(EffEventWork *work) {
    func_00169168(work->effect);
}

void effEventSetState(EffEventWork *work, u32 state) {
    work->state = state;
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00197F60);

/* Init block of the event holder (0x30 bytes, copied to the event's owner record). */
typedef struct {
    f32 pos[3];           /* 0x00 */
    f32 unk0C;            /* 0x0C */
    u32 unk10;            /* 0x10 */
    u32 unk14;            /* 0x14 */
    u32 unk18;            /* 0x18 */
    f32 scale;            /* 0x1C */
    f32 rangeNear;        /* 0x20 */
    f32 rangeFar;         /* 0x24 */
    f32 param;            /* 0x28 */
    u32 color;            /* 0x2C */
} __attribute__((packed)) EffEventInit; /* 0x30 */

/* 0x3C-byte event holder: a handle, the event it owns, an init block copied to the event. */
typedef struct EffEventLight {
    u32 handle;           /* 0x00 */
    struct EffEventWork *owner; /* 0x04 */
    EffEventInit init;    /* 0x08 */
    u8 active;            /* 0x38 */
} EffEventLight; /* 0x3C */

EffEventLight *func_00198278(u32 arg, f32 param) {
    EffEventLight *work = func_00328D68(sizeof(EffEventLight));

    work->init.scale = 1.0f;
    work->init.rangeNear = 50.0f;
    work->init.rangeFar = 180.0f;
    work->init.pos[1] = -90.0f;
    work->init.param = param;
    work->init.color = 0x80808080;
    work->init.unk10 = 0;
    work->init.unk14 = 0;
    work->init.unk18 = 0;
    work->init.pos[0] = 0;
    work->init.pos[2] = 0;
    work->init.unk0C = 0;
    work->handle = func_00197D38(arg);
    work->owner = func_00197D68(work->handle, 0, &work->init);
    work->active = 1;
    return work;
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00198340);

EffEventLight *func_00198380(EffEventLight *src) {
    EffEventInit *block = &src->init;
    EffEventLight *work = func_00328D68(sizeof(EffEventLight));

    work->owner = func_00197D68(src->handle, 0, block);
    work->handle = src->handle;
    work->init = *block;
    work->active = 0;
    return work;
}

void func_00198448(EffEventWork *work) {
    func_00197F60(work->owner);
}

void func_00198460(EffEventLight *work, f32 *vec) {
    work->init.pos[0] = vec[0];
    work->init.pos[1] = vec[1] - work->init.rangeFar * 0.5f;
    work->init.pos[2] = vec[2];
    work->init.unk0C = 0;
    effEventCopyFileRecordHeader((FileRecordHeader *)work->owner, (FileRecordHeader *)&work->init);
}

/* Attach the effect and copy the initial file-record header to its owner. */
void effEventBindEffect(EffEventWork *work, u32 effect) {
    work->effect = effect;
    effEventCopyFileRecordHeader(work->owner, work->initBlock);
}

INCLUDE_ASM(const s32, "effect/effEvent", func_001984D8);

/* 0x7C-byte parameter block; the counters at 0x18/0x2C/0x30 are clamped to at least 1 on create/clone. */
typedef struct {
    u8 pad00[0x10];
    u32 count;            /* 0x10 particle count */
    u8 pad14[4];
    s32 unk18;            /* 0x18 modulus of the first delay */
    s32 unk1C;            /* 0x1C modulus of the second delay */
    f32 range;            /* 0x20 spawn radius */
    u8 pad24[8];
    s32 unk2C;
    s32 unk30;
    u8 pad34[4];
    f32 f38;              /* 0x38 */
    f32 blend3C;          /* 0x3C */
    f32 f40;              /* 0x40 */
    f32 f44;              /* 0x44 */
    f32 f48;              /* 0x48 */
    f32 f4C;              /* 0x4C */
    f32 f50;              /* 0x50 */
    f32 blend54;          /* 0x54 */
    f32 f58;              /* 0x58 */
    f32 blend5C;          /* 0x5C */
    u8 pad60[8];
    f32 f68;              /* 0x68 */
    u8 pad6C[4];
    f32 blend70;          /* 0x70 */
    u8 pad74[8];
} EffEventBlock7C;

typedef struct {
    f32 pos[3];           /* 0x00 */
    u8 pad0C[4];
    f32 dir[3];           /* 0x10 */
    u8 pad1C[4];
    s32 delayA;           /* 0x20 */
    s32 delayB;           /* 0x24 */
    f32 f28;              /* 0x28 */
    f32 f2C;              /* 0x2C */
    f32 f30;              /* 0x30 */
    f32 f34;              /* 0x34 */
    f32 f38;              /* 0x38 */
    f32 f3C;              /* 0x3C */
    f32 f40;              /* 0x40 */
    f32 f44;              /* 0x44 */
    f32 f48;              /* 0x48 */
    f32 f4C;              /* 0x4C */
    f32 f50;              /* 0x50 */
    f32 f54;              /* 0x54 */
    f32 f58;              /* 0x58 */
    u8 pad5C[4];
} EffEventBillParticle; /* 0x60 */

typedef struct {
    EffEventBlock7C head;
    EffEventBillParticle *particles; /* 0x7C */
    u8 flag;              /* 0x80 */
    u8 pad81[3];
    u32 handle;           /* 0x84 */
} EffEventBillSet; /* 0x88 */

extern u32 func_003292A8(s32 size);
extern u8 *sdfResourceRetainAddress(u32 handle);
extern void *billCreateFromResource(s32 kind, const char *path);

INCLUDE_RODATA(const s32, "effect/effEvent", D_00414A00);

EffEventBillSet *func_00198710(EffEventBlock7C *src) {
    u32 count = src->count;
    u32 size = count * sizeof(EffEventBillParticle);
    u32 handle = func_003292A8(size + sizeof(EffEventBillSet));
    EffEventBillParticle *particle = (EffEventBillParticle *)sdfResourceRetainAddress(handle);
    EffEventBillSet *work = (EffEventBillSet *)((u8 *)particle + size);
    u32 i;

    work->head = *src;
    work->handle = handle;
    work->particles = particle;
    work->flag = 0;
    if (work->head.unk2C <= 0) {
        work->head.unk2C = 1;
    }
    if (work->head.unk30 <= 0) {
        work->head.unk30 = 1;
    }
    if (work->head.unk18 <= 0) {
        work->head.unk18 = 1;
    }
    D_00436530 = D_00436530 + 1;
    if (D_00436530 == 1) {
        D_00436534 = (u32)billCreateFromResource(0, "/efftool/bill/dbball01.tmx");
        D_00436538 = (u32)billCreateFromResource(0, "/efftool/bill/dbball02.tmx");
    }
    count = work->head.count;
    for (i = 0; i < count; i++) {
        particle->delayA = 0;
        particle++;
    }
    return work;
}

void effEventReleaseSharedResources(EffEventWork *work) {
    D_00436530 = D_00436530 - 1;
    if (D_00436530 == 0) {
        billDispatchByKind(D_00436534);
        billDispatchByKind(D_00436538);
    }
    func_003297C8(work->resource);
}

extern u32 effMiscRand(void *state);
extern f32 func_00341240(void *state);
extern u8 D_003AA868[];

/* Randomize one billboard particle: delays, spin rates, radius and two unit direction vectors. */
void func_00198950(EffEventBillSet *work, s32 index) {
    EffEventBillParticle *p = &work->particles[index];
    f32 dir[4];
    f32 scale;
    f32 range;
    s32 periodA = work->head.unk18;
    s32 periodB = work->head.unk1C;

    p->delayA = -(effMiscRand(D_003AA868) % periodA);
    p->delayB = -(effMiscRand(D_003AA868) % periodB);
    scale = func_00341240(D_003AA868) * work->head.blend3C + (1.0f - work->head.blend3C);
    p->f28 = work->head.f38 * scale;
    p->f2C = func_00341240(D_003AA868) * (3.14159265f * 2.0f);
    p->f38 = func_00341240(D_003AA868) * (3.14159265f * 2.0f);
    p->f30 = work->head.f48 * (func_00341240(D_003AA868) * 0.5f + 0.5f);
    p->f3C = work->head.f48 * (func_00341240(D_003AA868) * 0.5f + 0.5f);
    p->f34 = work->head.f40 * (func_00341240(D_003AA868) * 0.3f + 0.7f) * scale;
    p->f40 = work->head.f44 * (func_00341240(D_003AA868) * 0.3f + 0.7f) * scale;
    p->f44 = func_00341240(D_003AA868) * (3.14159265f * 2.0f);
    if (effMiscRand(D_003AA868) & 1) {
        p->f48 = work->head.f4C * (func_00341240(D_003AA868) * 0.3f + 0.7f);
    } else {
        p->f48 = -(work->head.f4C * (func_00341240(D_003AA868) * 0.3f + 0.7f));
    }
    range = work->head.range;
    dir[0] = (func_00341240(D_003AA868) - 0.5f) * 2.0f;
    dir[1] = (func_00341240(D_003AA868) - 0.5f) * 2.0f;
    dir[2] = (func_00341240(D_003AA868) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, dir);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, dir);
    p->pos[0] = range * func_00341240(D_003AA868) * dir[0];
    p->pos[1] = range * func_00341240(D_003AA868) * dir[1];
    p->pos[2] = range * func_00341240(D_003AA868) * dir[2];
    p->dir[0] = (func_00341240(D_003AA868) - 0.5f) * 2.0f;
    p->dir[1] = 0;
    p->dir[2] = (func_00341240(D_003AA868) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, p->dir);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, p->dir);
    p->f4C = work->head.f68 * (func_00341240(D_003AA868) * work->head.blend70 + (1.0f - work->head.blend70));
    p->f50 = 0;
    p->f54 = work->head.f50 * (func_00341240(D_003AA868) * work->head.blend54 + (1.0f - work->head.blend54));
    p->f58 = work->head.f58 * (func_00341240(D_003AA868) * work->head.blend5C + (1.0f - work->head.blend5C));
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00198D00);

INCLUDE_ASM(const s32, "effect/effEvent", func_00199118);

void func_00199C50(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00199C60(EffEventWork *work, u8 flag) {
    work->flag = flag;
}

void func_00199C68(const EffEventBlock7C *source, EffEventBlock7C *destination) {
    *destination = *source;
}

void func_00199D48(EffEventBlock7C *destination, const EffEventBlock7C *source) {
    *destination = *source;
    if (destination->unk2C <= 0) {
        destination->unk2C = 1;
    }
    if (destination->unk30 <= 0) {
        destination->unk30 = 1;
    }
    if (destination->unk18 <= 0) {
        destination->unk18 = 1;
    }
}

void func_00199E68(void) {
    func_00198710(D_003B2D20);
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00199E88);

extern void *effParamTableGetBlock(void *data, s32 index);
extern u32 effParamTableGetWord2(void *data, s32 index);
extern void func_00199E88(void *src, u16 kind, void *params);

void func_0019A058(void *data) {
    void *block0 = effParamTableGetBlock(data, 0);
    void *block1 = effParamTableGetBlock(data, 1);

    func_00199E88(block0, effParamTableGetWord2(data, 1), block1);
}

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436530);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436534);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436538);

INCLUDE_SDATA(const s32, "effect/effEvent", D_0043653C);

