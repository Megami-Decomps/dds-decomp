#include "common.h"

/* Sub-record behind MdlCtx.sub (+0x8/+0xA read by func_002183D0/E0). */
typedef struct MdlSub {
    u8 unk0[8]; /* 0x0 */
    u16 unk8;   /* 0x8 */
    u16 unkA;   /* 0xA */
} MdlSub;

/* Record behind MdlCtx.inner. */
typedef struct MdlInner {
    u8 unk0[8];  /* 0x0 */
    u32 unk8;    /* 0x8: freed through func_002DA1B0 */
    u8 unkC[8];  /* 0xC */
    u32 *list;   /* 0x14: intrusive list walked by func_00218320/368 */
    u8 unk18[4]; /* 0x18 */
    u32 unk1C;   /* 0x1C */
} MdlInner;

/* Context shared by the matched mdlManager helpers. */
typedef struct MdlCtx {
    u8 unk0[0xC];      /* 0x0 */
    MdlSub *sub;       /* 0xC */
    u32 unk10;         /* 0x10 */
    u32 *list14;       /* 0x14: intrusive list walked by func_00218050 */
    MdlInner *inner;   /* 0x18 */
} MdlCtx;

/* Packet parsed by func_00216EC0. */
typedef struct MdlPacket {
    u16 unk0;     /* 0x0 */
    u16 unk2;     /* 0x2 */
    u8 unk4[4];   /* 0x4 */
    u32 unk8;     /* 0x8 */
    u16 extra[1]; /* 0xC: start of the variable payload */
} MdlPacket;

/* Load request touched by func_00216F18. */
typedef struct MdlLoadReq {
    u8 unk0[0xC]; /* 0x0 */
    u32 unkC;     /* 0xC */
} MdlLoadReq;

/* Resource released by func_002189D8. */
typedef struct MdlRes {
    u8 unk0[8]; /* 0x0 */
    u32 unk8;   /* 0x8 */
} MdlRes;

extern s32 func_00217E10(void);

extern void *func_00288B90(void);
extern u32 func_002EB090(void *);

extern u32 D_003BD878;

extern void *func_00216708(void);

INCLUDE_ASM(const s32, "model/mdlManager", func_00216B78);

INCLUDE_ASM(const s32, "model/mdlManager", func_00216BB0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00216C00);

void func_00216CC8(void *arg0, void *arg1, void *arg2) {
    void *handle;

    /* arg0/arg1 are ignored. */
    handle = func_00216708();
    func_00216C00(handle, arg2);
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00216CF8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00216DB8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00216E68);

void func_00216EC0(MdlPacket *packet) {
    func_00216DB8(packet->unk0, packet->unk2, packet->unk8, packet->extra);
    WaitSema(D_003BD878);
    func_002167E0(packet->unk0, packet->unk2);
    SignalSema(D_003BD878);
    func_002CFF98(packet);
}

void func_00216F18(void *arg0, MdlLoadReq *req) {
    void *handle;
    u32 size;

    handle = func_00288B90();
    size = func_002EB090(handle);
    req->unkC = size;
    handle = func_00288B88(arg0);
    func_002D0918(handle);
    func_002887A0(arg0);
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00216F68);

INCLUDE_ASM(const s32, "model/mdlManager", func_00216FE0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217038);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217068);

void func_00217298(u32 arg0, u32 arg1) {
    func_00217068(arg0, arg1, 1);
}

INCLUDE_ASM(const s32, "model/mdlManager", func_002172B0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217310);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217438);

INCLUDE_ASM(const s32, "model/mdlManager", func_002174C0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217680);

INCLUDE_ASM(const s32, "model/mdlManager", func_002177D0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217878);

INCLUDE_ASM(const s32, "model/mdlManager", func_002179A8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217C60);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217CA8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217DA0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217DC0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217DE0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217DF8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217E10);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217E58);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217E88);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217EB0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217EE0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217F18);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217F40);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217F70);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217F88);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217FA0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217FB8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00218010);

INCLUDE_ASM(const s32, "model/mdlManager", func_00218028);

u32 func_00218040(MdlCtx *ctx) {
    return ctx->inner->unk1C;
}

void func_00218050(MdlCtx *ctx, u32 arg1) {
    u32 *node;

    for (node = ctx->list14; node != NULL; node = (u32 *)*node) {
        func_0021A560(ctx, node, arg1);
    }
}

INCLUDE_ASM(const s32, "model/mdlManager", func_002180A8);

INCLUDE_ASM(const s32, "model/mdlManager", func_002180E0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00218100);

INCLUDE_ASM(const s32, "model/mdlManager", func_00218158);

INCLUDE_ASM(const s32, "model/mdlManager", func_00218228);

void func_00218320(MdlCtx *ctx) {
    u32 *node;

    for (node = ctx->inner->list; node != NULL; node = (u32 *)*node) {
        func_002DB768(node);
    }
}

void func_00218368(MdlCtx *ctx) {
    u32 *node;

    for (node = ctx->inner->list; node != NULL; node = (u32 *)*node) {
        func_002DB788(node);
    }
}

u8 func_002183B0(void) {
    s32 ready;

    ready = func_00217E10();
    return ready != 0;
}

u16 func_002183D0(MdlCtx *ctx) {
    return ctx->sub->unk8;
}

u16 func_002183E0(MdlCtx *ctx) {
    return ctx->sub->unkA;
}

u32 func_002183F0(void) {
    return 8;
}

INCLUDE_ASM(const s32, "model/mdlManager", func_002183F8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00218410);

void func_00218440(MdlCtx *ctx) {
    func_002DA1B0(ctx->inner->unk8);
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00218460);

INCLUDE_ASM(const s32, "model/mdlManager", func_00218580);

INCLUDE_ASM(const s32, "model/mdlManager", func_002185B0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00218768);

void func_002189D8(MdlRes *res) {
    func_00288788(res->unk8);
    func_002CFF98(res);
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00218A08);

INCLUDE_ASM(const s32, "model/mdlManager", func_00218A88);
