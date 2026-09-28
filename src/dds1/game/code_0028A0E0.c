#include "common.h"

extern u32 func_0029C230(u32);

extern u64 func_00293D08(u64);

extern u64 func_002D03F8(u64);
extern u64 func_002D0A48(u64);
extern s64 func_002E5B50(u64);
extern u64 func_002E5C88(s64);

extern u32 D_003BC8F8;

extern s32 D_003BD938;

extern s32 D_003BAA00;
extern u32 D_003BC8D8;
extern u32 D_003BC8DC;

extern u32 D_003BC81C;

extern u32 D_003BC84C;

extern u32 D_003BC854;

extern u32 D_003BC80C;
extern u32 D_003BC810;

extern u32 D_003BC7E8;

extern s32 D_003BC850;
extern s32 D_003BC860;
extern u32 D_003BC858;
extern u32 D_003BD904;
extern u32 D_003BD910;
extern u32 D_003BC7F8;

extern u64 func_001978E8(s32, s32, u64, u64, u64, u64);

extern u32 D_003BD8F0;
extern u32 func_001951C8(u32, u32, u32, u32, u32);

extern s32 D_003BC7FC;

extern s8 D_003BC7EC;
extern s32 D_003BC7F0;

/* Loader context at D_0037D4A0. */
typedef struct LoadCtx374A0 {
    u8 unk0[4]; /* 0x00 */
    u32 unk4;   /* 0x04 */
    u8 unk8[4]; /* 0x08 */
    u32 unkC;   /* 0x0C */
    u8 unk10;   /* 0x10 */
    u8 pad11[3]; /* 0x11 */
    u32 unk14;  /* 0x14 */
} LoadCtx374A0;

extern LoadCtx374A0 D_0037D4A0;
/* Far scalar: incomplete array forces non-small-data addressing. */
extern u32 D_0037D4D0[];
extern u32 D_0037E130[];

extern char D_003B2668[];
extern char D_003B26C8[];

typedef struct KwlnTask KwlnTask;
extern KwlnTask *kwlnTaskGetTaskByName(const char *name);
extern u32 func_00288B48(const char *arg0);
extern void func_0029A810(void *arg0);
extern void func_002966D8(s32 arg0);
extern void func_0028B430(void);
extern void func_0028B238(void);
extern s32 func_0028B930(void);
extern void func_0028C888(void);
extern void func_0028BE78(void);
extern s32 D_003BC808;
extern s32 func_00289AA0(void);
extern u8 func_00289B98(s32 arg0);
extern void func_00289A58(s32 arg0);
extern s32 func_0028B508(void);
extern s32 func_0028B328(void);
extern s32 func_0028B370(void);
extern s32 func_0028B900(void);
extern s32 D_003BC844;
extern void func_00289C98(s32 arg0, s32 arg1, s32 arg2);
extern void *func_0028BF08(u32 arg0);
extern void func_0028B590(void);
extern void func_0028BF38(void);
extern s32 func_00289BC0(s32 arg0);
extern u32 D_003BC804;
extern void func_001005B8(void);
extern void func_00289D50(u32 arg0);
extern void func_0028D8F8(void);
extern void func_00293EA0(void *arg0);
extern void func_00293158(void *src);
extern void func_00294798(void *dst, void *src);
extern void func_0029A748(s32 arg0);
extern void func_0029A7C8(void *arg0, u128 *arg1);
extern void func_0029A7F8(void *arg0, u128 *arg1);
extern void func_001028E8(s32 arg0, void *arg1, s32 arg2, s32 arg3);
extern s32 D_003BC848;
extern s32 D_003BC828;
extern char D_003B2658[];
extern void func_0028B1F0(void *arg0);
extern void func_0028A190(void *arg0, s32 arg1);
extern void func_00289DA8(u32 arg0, void *arg1);
extern void *func_0028C180(void);
extern s32 func_00289DC8(void);
extern void func_00289E80(u32 arg0, const char *arg1, void *arg2, s32 arg3);
extern char D_003B2678[];
extern u8 D_003DC780[];
extern void func_0028C208(void);
extern void *func_0028B3F0(void);
extern void *func_0028AF00(void);
extern s32 func_00289EB0(void *arg0);
extern void *func_0028C560(void);
extern void func_0028C4D0(void);
extern void *func_0028C430(void);
extern u32 func_00289CD0(s32 arg0, s32 arg1);
extern s32 func_0028B3B0(void);
extern u32 D_003BC834;
extern s32 func_0028FB48(void *arg0, void *arg1, s32 arg2);
extern void func_0028BAC0(void);
extern void *func_0028C7C8(void);
extern void *func_0028D6A0(void);
extern void func_0028BDA0(void);
extern s32 func_00289F30(void);
extern void *func_0028C9A0(void);
extern u8 func_00289BE8(s32 arg0);
extern u32 D_003DC7C0[];

/* Init record at D_0037D4E0. */
typedef struct Init374E0 {
    u8 unk0;        /* 0x00 */
    u8 unk1;        /* 0x01 */
    u8 unk2;        /* 0x02 */
    u8 pad3[0x31];  /* 0x03 */
    u32 unk34;      /* 0x34 */
    u32 unk38;      /* 0x38 */
    u32 unk3C;      /* 0x3C */
} Init374E0;

extern Init374E0 D_0037D4E0;
extern u32 D_0037D4AC[];

/* Callback table at D_0037E14C (0x28 bytes per entry). */
typedef struct Cb3714C {
    void (*cb)(void *arg); /* 0x00 */
    u8 pad4[0x24];         /* 0x04 */
} Cb3714C;

extern Cb3714C D_0037E14C[];

/* Object with loader sub-objects (+0x40...). */
typedef struct LoadObj {
    u8 unk0[8];   /* 0x00 */
    f32 unk8;     /* 0x08 */
    u8 unkC[0x34]; /* 0x0C */
    void *unk40;  /* 0x40 */
    void *unk44;  /* 0x44 */
} LoadObj;

void func_0028A0E0(void) {
    func_002F6670();
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028A0F8);

void func_0028A150(void) {
    if (D_003BC7F0 != 0) {
        kwlnTaskDestroyWithHierarchy(D_003BC7F0, 1);
        D_003BC7F0 = 0;
        D_003BC7EC = 0;
    }
}

s8 func_0028A180(void) {
    return D_003BC7EC;
}

void func_0028A188(void) {
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028A190);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028A1B8);

u32 func_0028A1F8(void) {
    return 0x33600;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028A208);

u8 func_0028A248(s32 arg0) {
    return arg0 != 0 && D_003BC7FC == 1;
}

u8 func_0028A268(s32 arg0) {
    return arg0 != 0 && D_003BC7FC == 1;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028A288);

void func_0028A2D0(s32 arg0, s32 arg1, u32 arg2, u32 arg3) {
    func_00195520(1);
    D_003BD8F0 = func_001951C8(arg3, 0, 0, 0, 0);
    func_00195530(1);
    func_001953A8(D_003BD8F0, 1);
    func_00195450(D_003BD8F0, arg0 << 4, arg1 << 3);
    func_001954C8(D_003BD8F0, arg2);
    func_001958A0(D_003BD8F0, 0, 0x56);
    func_00194920(D_003BD8F0);
    func_00195548(0x54);
}

void func_0028A388(s32 arg0, s32 arg1, u64 arg2, u64 arg3) {
    u64 temp_v0;

    temp_v0 = func_001978E8(arg0 << 4, arg1 << 3, 0, arg2, arg3, 0);
    func_00195880(temp_v0, 1);
    func_00194920(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028A3D8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028A508);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028A5D8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028A5E8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028A7A8);

void func_0028AB30(u32 arg0) {
    s32 temp_v0;

    temp_v0 = D_003BC850;
    D_003BC850 = arg0;
    if (temp_v0 == 0) {
        D_003BC860 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028AB48);

void func_0028AE68(void) {
    func_00289C38(D_003BC7E8);
    func_00289C68(D_003BC7E8, 0);
    func_00289C68(D_003BC7E8, 1);
    func_00289C68(D_003BC7E8, 2);
    func_00289C68(D_003BC7E8, 3);
    func_00289C68(D_003BC7E8, 4);
    func_00289C68(D_003BC7E8, 5);
    func_00289C68(D_003BC7E8, 6);
    func_00289C68(D_003BC7E8, 7);
    func_00289C68(D_003BC7E8, 8);
    func_00289C68(D_003BC7E8, 9);
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028AF00);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028AFE0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028B058);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028B0B8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028B1F0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028B238);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028B280);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028B2C0);

void *func_0028B300(void) {
    func_0028AB30(0);
    D_003BC810 = 0;
    return func_0028B930;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028B328);

s32 func_0028B370(void) {
    D_003BC80C = 1;
    func_0028AB30(0);
    D_003BC834 = 5;
    return func_0028FB48(&func_0028B508, &func_0028B328, 1);
}

s32 func_0028B3B0(void) {
    D_003BC80C = 1;
    func_0028AB30(0);
    D_003BC834 = 6;
    return func_0028FB48(&func_0028B508, &func_0028B328, 1);
}

void *func_0028B3F0(void) {
    func_0028AE68();
    func_0028AB30(0);
    D_003BC854 = 1;
    D_003BC858 = 0;
    func_00289A58(D_003BC7E8);
    return func_0028BAC0;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028B430);

u32 func_0028B4B0(void) {
    func_0028AB30(0);
    D_003BC810 = 0;
    D_003BC80C = 0;
    return 0xffffffff;
}

u32 func_0028B4D8(void) {
    u32 temp_v0 [4];

    temp_v0[0] = 0;
    func_001028E8(2, temp_v0, 4, 0);
    return 0;
}

s32 func_0028B508(void) {
    return func_0028B4D8();
}

s32 func_0028B520(void) {
    u32 v = 2;

    D_003BC84C = 1;
    func_001028E8(2, &v, 4, 0);
    return 0;
}

void func_0028B560(void) {
    func_0028AE68();
    func_0028AB30(1);
    D_003BC854 = 0;
    D_003BC810 = 0;
    func_0028B430();
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028B590);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2658);

void func_0028B658(void) {
    if (D_003BC848 == 1 && kwlnTaskGetTaskByName(D_003B2658) == NULL) {
        func_0028B520();
    } else {
        func_0028AB30(0);
        func_0028B1F0(&func_0028B4B0);
    }
}

void func_0028B6A8(void) {
    D_003BD910 = 0;
    D_003BC7F8 = func_00288B48(D_003B2668);
    func_0028B430();
}

void func_0028B6D0(void) {
    func_0028B430();
}

void func_0028B6E8(void) {
    D_003BD910 = 0;
    D_003BC7F8 = func_00288B48(D_003B2668);
    func_0028B238();
}

void func_0028B710(void) {
    func_00289A58(0);
    D_003BC804 = 1;
    D_003BC808 = 0;
    func_0028AB30(0);
    D_003BC810 = 0;
    func_0028B300();
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028B748);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028B890);

s32 func_0028B900(void) {
    return func_00289BC0(D_003BC7E8) > 0x4C3FF;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028B930);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028B9F8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028BAC0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028BCD0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028BDA0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028BE10);

void *func_0028BE60(u32 arg0) {
    D_003BD904 = arg0;
    D_003BC858 = 0;
    return func_0028BE78;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028BE78);

void *func_0028BF08(u32 arg0) {
    D_003BD904 = arg0;
    D_003BC858 = 0;
    func_00289A58(D_003BC7E8);
    return func_0028BF38;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028BF38);

void *func_0028C140(void) {
    u8 buf[0x50];

    buf[0] = 0x2F;
    func_0028A190(&buf[1], D_003BC828);
    func_00289DA8(D_003BC7E8, buf);
    return func_0028C180;
}

void *func_0028C180(void) {
    s32 t = func_00289DC8();

    if (t == 0) {
        return NULL;
    }
    if (t == 1) {
        func_00289C98(D_003BC7E8, D_003BC828, 2);
        func_00289E80(D_003BC7E8, D_003B2678, D_003DC780, 1);
        return func_0028C208;
    }
    if (t == -1) {
        return func_0028B3F0();
    }
    return func_0028AF00();
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028C208);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028C288);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028C358);

void *func_0028C3F0(void) {
    u8 buf[0x50];

    buf[0] = 0x2F;
    func_0028A190(&buf[1], D_003BC828);
    func_00289DA8(D_003BC7E8, buf);
    return func_0028C430;
}

void *func_0028C430(void) {
    s32 t = func_00289DC8();

    if (t == 0) {
        return NULL;
    }
    if (t == 1) {
        func_00289C98(D_003BC7E8, D_003BC828, 2);
        func_00289E80(D_003BC7E8, D_003B2678, D_003DC780, 1);
        return func_0028C4D0;
    }
    if (t == -1) {
        func_0028AB30(0);
        D_003BC810 = 0;
        D_003BC854 = 0;
        return (void *)func_0028B370();
    }
    return func_0028C560();
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028C4D0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028C560);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028C600);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028C670);

void *func_0028C710(void) {
    u8 buf[0x50];
    u32 entry = D_003BC7E8;
    s32 v = func_00289D00(entry);

    buf[0] = 0x2F;
    func_0028A190(&buf[1], v);
    func_00289DA8(entry, buf);
    return func_0028C7C8;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028C760);

void *func_0028C7C8(void) {
    s32 t = func_00289DC8();

    if (t == 0) {
        return NULL;
    }
    if (t == 1) {
        return func_0028D6A0();
    }
    if (t == -1) {
        func_0028AB30(0);
        D_003BC854 = 4;
        return func_0028BDA0;
    }
    return NULL;
}

void *func_0028C828(void) {
    s32 t = func_00289F30();

    if (t == 0) {
        return NULL;
    }
    if (t == 1) {
        return func_0028C9A0();
    }
    if (t == -1) {
        func_0028AB30(0);
        D_003BC854 = 4;
        return func_0028BDA0;
    }
    return NULL;
}

void func_0028C888(void) {
    func_00289C98(D_003BC7E8, D_003BC844, 1);
    func_00289C98(D_003BC7E8, D_003BC844, 8);
    func_0028AB30(4);
    func_0028BF08((u32)func_0028B590);
}

void *func_0028C8D0(void) {
    func_0028A150();
    return func_0028C888;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028C8F8);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2668);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2678);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028C970);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028C9A0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028D6A0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028D708);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028D748);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028D7E8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028D850);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028D8B8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028D8F8);

void *func_0028D978(void) {
    func_0028AB30(5);
    func_001005B8();
    func_00289D50(D_003BC7E8);
    return func_0028D8F8;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028D9B0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028DA10);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028DAF8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028DBA0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028DC08);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028DCC0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028E180);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028EF00);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028EFA0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028F0E0);

void func_0028F440(void) {
    func_0028F460();
}

void func_0028F458(void) {
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028F460);

u32 func_0028F5F8(void) {
    return 1;
}

s32 func_0028F600(void) {
    return kwlnTaskGetTaskByName(D_003B26C8) != NULL;
}

u32 func_0028F628(void) {
    return D_003BC84C;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028F630);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028FB48);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028FB98);

u32 func_00290478(void) {
    return D_003BC81C;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00290480);

void func_002904A8(u32 arg0) {
    D_0037D4D0[0] = arg0;
    D_0037D4A0.unkC = 0x80;
    D_0037D4A0.unk4 = 0;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002904C8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00290520);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002905A8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002905E8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00290788);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00290898);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002908D0);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B26C8);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", jtbl_003B26E0);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2718);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2768);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2810);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00290A88);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00290C58);

void func_00290C98(void) {
    D_0037D4A0.unk14 = 0;
    D_0037D4A0.unk10 = 0;
    D_0037D4A0.unk4 = 0;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00290CB0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00290E20);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00290E38);

void func_00290E50(void) {
    *(u32 *)(D_003BAA00 + 0xa54) = D_003BC8DC;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00290E60);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B28A0);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B28C0);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B28D0);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B28E8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00290F40);

void func_00290FC0(u32 arg0) {
    func_00290F40(arg0, D_003BAA00 + 0xa54);
}

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2920);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2940);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2960);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2980);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B29A0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00290FE0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002911B8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002912C8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00291378);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002913B8);

u32 func_002913F0(s32 arg0) {
    if (arg0 < 4) {
        return *(u32 *)((s32)arg0 * 4 + D_003BD938 + 0x10);
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B29D8);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B29E8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00291418);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00291880);

u32 func_002918D8(void) {
    u32 temp_v0;

    temp_v0 = 0xffffffff;
    if ((*(u32 *)(D_003BD938 + 0x24) & 0x80000000) == 0) {
        temp_v0 = 0;
    }
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002918F8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00292720);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00292BA8);

void func_00292C18(u32 arg0) {
    D_003BC8F8 = D_003BC8F8 | arg0;
}

void func_00292C28(u32 arg0) {
    D_003BC8F8 = D_003BC8F8 & ~arg0;
}

void func_00292C40(void) {
    D_003BC8F8 = 0;
}

u32 func_00292C48(s32 arg0) {
    return D_0037E130[arg0];
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00292C60);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00292CE0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00292E50);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00292F48);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002930C0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00293158);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002933A8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00293400);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00293428);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00293450);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002934C0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00293528);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00293568);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002935A8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00293620);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00293668);

void func_002936A8(void *arg0) {
    u16 idx = *(u16 *)((u8 *)arg0 + 4);
    void *data = *(void **)((u8 *)arg0 + 8);

    D_0037E14C[idx].cb(data);
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002936E0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00293720);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00293760);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002937A0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002937E0);

void func_00293880(u64 arg0, u64 arg1, u16 arg2) {
    s64 temp_v0;
    u64 temp_v1;
    u64 temp_v2;
    u64 temp_v3;

    temp_v0 = func_002E5B50(arg1);
    if (temp_v0 != 0) {
        temp_v1 = func_002E5C88(temp_v0);
        temp_v2 = func_002D03F8(temp_v1);
        temp_v3 = func_002D0A48(temp_v2);
        func_002E5C68(temp_v0, temp_v3, temp_v1);
        func_002E5C38(temp_v0);
        func_002937E0(arg0, temp_v3, temp_v1, arg2);
        func_002D0918(temp_v2);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00293960);

void func_00293A00(u64 arg0, u64 arg1, u16 arg2) {
    s64 temp_v0;
    u64 temp_v1;
    u64 temp_v2;
    u64 temp_v3;

    temp_v0 = func_002E5B50(arg1);
    if (temp_v0 != 0) {
        temp_v1 = func_002E5C88(temp_v0);
        temp_v2 = func_002D03F8(temp_v1);
        temp_v3 = func_002D0A48(temp_v2);
        func_002E5C68(temp_v0, temp_v3, temp_v1);
        func_002E5C38(temp_v0);
        func_00293960(arg0, temp_v3, temp_v1, arg2);
        func_002D0918(temp_v2);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00293AE0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00293C50);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00293D08);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00293D90);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00293E30);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00293EA0);

void func_00293F18(void *arg0) {
    memset(arg0, 0, 0x90);
    *(u32 *)((u8 *)arg0 + 0x84) = 1;
    *(u8 *)((u8 *)arg0 + 0x88) = 8;
    *(u8 *)((u8 *)arg0 + 0x89) = 0;
    *(u8 *)((u8 *)arg0 + 0x8A) = 0;
    func_00293EA0(arg0);
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00293F60);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00293F98);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00293FD8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00294020);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00294070);

void func_002940B8(void) {
    func_002CFF98();
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002940D0);

void func_00294318(u32 arg0, u32 arg1) {
    func_002940D0(arg1);
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00294330);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002944D8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00294548);

void func_00294630(s32 arg0) {
    s32 temp_v0;

    for (temp_v0 = *(s32 *)(arg0 + 0x8c); temp_v0 != 0; temp_v0 = *(s32 *)(temp_v0 + 0xac)) {
        func_00293668(*(u32 *)(temp_v0 + 0x90));
    }
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00294670);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00294798);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00294850);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00294938);

void func_00294A18(void *dst, void *src) {
    s128 vec;
    func_00293158(src);
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(&vec) : "memory");
    func_00294798(dst, &vec);
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00294A50);

void func_00294AA0(u64 arg0, u64 arg1) {
    u64 temp_v0;

    temp_v0 = func_00293D08(arg1);
    func_00294A50(arg0, temp_v0);
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00294AD0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00294B18);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00294C30);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00294DA0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00294E50);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00294F50);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00295018);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002954F0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002959E8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00295B70);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00295BB0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00295BF8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00295C48);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00295C90);

s32 func_00295CD0(s32 arg0) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = 0;
    for (temp_v0 = *(s32 *)(arg0 + 0x8c); temp_v0 != 0; temp_v0 = *(s32 *)(temp_v0 + 0xac)) {
        temp_v1 = temp_v1 + 1;
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00295D08);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00295E00);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00295EF8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00295F58);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2A18);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00295F90);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00296088);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00296148);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002961B0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00296358);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002963B8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00296430);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002964B0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00296530);

void func_00296628(s32 arg0, u32 arg1) {
    u32 temp_v0;

    if (*(s32 *)(arg0 + 0x40) != 0) {
        func_0029C378(*(s32 *)(arg0 + 0x40));
    }
    temp_v0 = func_0029C230(arg1);
    *(u32 *)(arg0 + 0x40) = temp_v0;
}

void func_00296678(s32 arg0) {
    if (*(s32 *)(arg0 + 0x44) != 0) {
        func_0029A748(*(s32 *)(arg0 + 0x44));
        return;
    }
}

void func_002966A8(s32 arg0) {
    if (*(s32 *)(arg0 + 0x44) != 0) {
        func_0029A750(*(s32 *)(arg0 + 0x44));
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002966D8);

void func_00296E98(s32 arg0) {
    func_002966A8(arg0);
    func_002966D8(arg0);
}

void func_00296EC0(LoadObj *arg0, u128 *arg1) {
    func_0029A7C8(arg0->unk44, arg1);
}

void func_00296ED8(LoadObj *arg0, u128 *arg1) {
    func_0029A7F8(arg0->unk44, arg1);
}

void func_00296EF0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 4) = arg1;
}

void func_00296EF8(LoadObj *arg0, f32 arg1) {
    arg0->unk8 = arg1;
    func_0029A810(arg0->unk44);
}

void func_00296F18(s32 arg0) {
    u32 temp_v0;
    s32 temp_v1;
    u32 temp_v2;

    temp_v0 = *(u32 *)(arg0 + 8);
    temp_v2 = 0;
    temp_v1 = *(s32 *)(arg0 + 0x18);
    if (temp_v0 != 0) {
        do {
            temp_v2 = temp_v2 + 1;
            *(u32 *)(temp_v1 + 0x10) = 0xffffffff;
            temp_v1 = temp_v1 + 0x20;
        } while (temp_v2 < temp_v0);
    }
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00296F58);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00297270);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002973E8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00297558);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002975C8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00297658);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00297CB0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00298538);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002985D0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00298CA0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00298D28);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00299560);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002995F8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00299DD8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00299E58);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0029A558);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0029A5E0);

void func_0029A730(s32 arg0) {
    func_002D0918(*(u32 *)(arg0 + 0x28));
}

void func_0029A748(s32 arg0) {
    *(u32 *)(arg0 + 0x10) = 0;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0029A750);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0029A7B0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0029A7C8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0029A7E0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0029A7F8);
