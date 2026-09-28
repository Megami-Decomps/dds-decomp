#include "common.h"

typedef struct FileRecordSlot {
    u8 pad0[0x10];
    u32 state;
    u8 pad14[0xC];
} FileRecordSlot;

typedef struct FileRecordSlots {
    u16 type;
    u8 pad2[6];
    u32 count;
    u8 padC[4];
    u32 references;
    u8 pad14[4];
    FileRecordSlot *slots;
} FileRecordSlots;

extern u32 func_0029C230(u32);

extern void *func_00293D08(void *);

extern u64 func_002D03F8(u64);
extern u64 func_002D0A48(u64);
extern s64 func_002E5B50(u64);
extern u64 func_002E5C88(s64);

extern u32 D_003BC8F8;

extern s32 D_003BD938;

extern u32 D_003BC888;
extern void func_003014F0(void *dst, const char *fmt, ...);
extern u32 D_003BD924;
extern u32 D_003BD8EC;
extern u32 func_00197760(s32, s32, s32, u32, u32, s32);
extern void func_00105B98(s32, s32, s32, s32);
extern void *D_003BD900;
extern s32 D_003BC800;
extern void *func_0028BE10(void);
extern void *func_0028B748(void);
extern void (*D_0037E550[][4])(void *);
extern void func_00296F18(FileRecordSlots *record);
extern void *func_00293D90(void *entry);
extern char D_003BC940[];
extern char D_003B2688[]; /* "base.ico"; retail record includes padding */
extern u32 D_003BD914;
extern u32 D_003BD918;
extern u32 *D_003BD928;
extern u32 *D_003BD92C;
extern void *D_003BD930;
extern u32 *D_003BD934;
extern u32 D_003BC824;
extern void *func_0028D708(const char *, u32 *, u32 *, void *, u32 *);
extern void func_00289F80(u32, const char *, s32);
extern void func_00289F10(void);
extern void *func_0028D748(void);
extern void *func_0028D7E8(void);
extern void *func_0028C8F8(void);
extern void *func_0028C828(void);
extern s32 D_003BD91C;
extern u32 D_003BD920;
extern s32 func_0028A088(void);
extern void func_0028A008(s32);
extern void *func_0028B0B8(void);
extern s32 func_00289E38(void);
extern void *func_0028C710(void);
extern s32 D_003BAA00;
extern void *func_0028B2C0(void);
extern void *func_0028C670(void);
extern void *func_0028B9F8(void);
extern void *func_0028B280(void);
typedef struct LoadMirror {
    u32 current;
    u32 previous;
} LoadMirror;
extern LoadMirror D_003BC8D8;
extern u32 D_003BC8DC;
extern char D_003BC8E8[];
extern char D_003B29D8[]; /* "config_draw" */
extern char D_003B29E8[]; /* "config_update" */
extern void kwlnTaskDestroyWithHierarchyByName(const char *name, s32 hierarchy);
extern void func_003003F0(void *arg);
extern char D_003BC900[];
extern char D_003BC908[];
extern char D_003BC910[];
extern char D_003BC918[];
extern char D_003BC920[];

extern u32 D_003BC81C;

extern u32 D_003BC84C;

extern u32 D_003BC854;

extern u32 D_003BC80C;
extern u32 D_003BC810;

extern u32 D_003BC7E8;

extern s32 D_003BC850;
extern s32 D_003BC860;
extern u32 D_003BC858;
extern s32 D_003BC864;
extern s8 D_003DC803[];
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
    u32 unk18;  /* 0x18 */
    u32 unk1C;  /* 0x1C */
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
extern void *func_0028B430(void);
extern void *func_0028B238(void);
extern s32 func_0028B930(void);
extern void func_0028C888(void);
extern void func_0028BE78(void);
extern s32 D_003BC808;
extern s32 func_00289AA0(void);
extern u8 func_00289B98(s32 arg0);
extern void func_00289A58(s32 arg0);
extern s32 func_0028B508(void);
extern void *func_0028B328(void);
extern s32 func_0028B370(void);
extern s32 func_0028B900(void);
extern s32 D_003BC844;
extern void func_00289C98(s32 arg0, s32 arg1, s32 arg2);
extern void func_00289C68(s32, s32);
extern void func_00289C38(s32);
extern void *func_0028BF08(u32 arg0);
extern void func_0028B590(void);
extern void func_0028BF38(void);
extern s32 func_00289BC0(s32 arg0);
extern u32 D_003BC804;
extern void func_001005B8(void);
extern void func_00289D50(u32 arg0);
extern void *func_0028D8F8(void);
extern void func_00293EA0(void *arg0);
extern void func_00293158(void *src);
extern void func_00294798(void *dst, void *src);
extern void *func_002CFEB8(s32 size);
extern void *func_002CFF68(s32 size);
extern void func_002CFF98();
extern void func_0029A748(FileRecordSlots *record);
extern void func_0029A7C8(void *arg0, const u128 *arg1);
extern void func_0029A7F8(void *arg0, const u128 *arg1);
extern void func_001028E8(s32 arg0, void *arg1, s32 arg2, s32 arg3);
extern s32 D_003BC848;
extern s32 D_003BC828;
extern char D_003B2658[];
extern void *func_0028B1F0(void *arg0);
extern void func_0028A190(void *arg0, s32 arg1);
extern void func_00289DA8(u32 arg0, void *arg1);
extern void *func_0028C180(void);
extern s32 func_00289DC8(void);
extern void func_00289E80(u32 arg0, const char *arg1, void *arg2, s32 arg3);
extern char D_003B2678[];
extern u8 D_003DC780[];
extern void *func_0028C208(void);
extern void *func_0028B3F0(void);
extern void *func_0028AF00(void);
extern s32 func_00289EB0(void *arg0);
extern void *func_0028C560(void);
extern void *func_0028C4D0(void);
extern void *func_0028C430(void);
extern u32 func_00289CD0(s32 arg0, s32 arg1);
extern s32 func_0028B3B0(void);
extern u32 D_003BC834;
extern s32 func_0028FB48(void *arg0, void *arg1, s32 arg2);
extern void func_0028BAC0(void);
extern void *func_0028C7C8(void);
extern void *func_0028D6A0(void);
extern void func_0028BDA0(void);
extern void *func_0028D850(void);
extern void *func_0028C760(void);
extern s32 func_00289F30(void);
extern void *func_0028C9A0();
extern u8 func_00289BE8(s32 arg0);
extern u32 D_003DC7C0[];
extern void func_00151F00(void *handle);
extern void *func_00151F58(void *name);
extern void *func_00151D88(s32 mode, void *name);
extern void func_001523B0(void *handle);
extern void func_00152050(void *handle, s16 index);
extern void *func_0029A5E0(u16 type, u32 owner, void *data);

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
    void (*cb)(void *arg);    /* 0x00 */
    u8 pad4[8];               /* 0x04 */
    void (*cbC)(void *, void *); /* 0x0C */
    void (*cb10)(void *arg);  /* 0x10 */
    void (*cb14)(void *arg);  /* 0x14 */
    void (*cb18)(void *arg);  /* 0x18 */
    void (*cb1C)(void *arg);  /* 0x1C */
    void (*cb20)(void *arg);  /* 0x20 */
    u32 unk24;                /* 0x24 */
} Cb3714C;

extern Cb3714C D_0037E14C[];
typedef struct FileTypeCallbacks {
    void *(*create)(void *, u16);
    void (*unk4)(void *);
    void (*destroy)(void *);
    void *(*createChild)(void *, u16);
    u8 unk10[0x18];
} FileTypeCallbacks;

extern FileTypeCallbacks D_0037E148[];

/* Object with loader sub-objects (+0x40...). */
typedef struct LoadObj {
    void *owner;        /* 0x00 */
    u32 color;          /* 0x04 */
    f32 scale;          /* 0x08 */
    u8 unkC[0x28];     /* 0x0C */
    void *deviceHandle; /* 0x34 */
    u32 unk38;          /* 0x38 */
    u32 unk3C;          /* 0x3C */
    void *unk40;        /* 0x40 */
    void *unk44;        /* 0x44 */
    s16 unk48;          /* 0x48 */
    u16 unk4A;
} LoadObj;
extern LoadObj *func_00295F58(LoadObj *source);
extern void *func_0028DC08(void);
extern s32 func_0028A020(void);
extern void func_00292720(void *);
extern void func_0027C370(s32, s32, s32, u32, s32);
extern LoadObj *func_00295EF8(void *owner);
extern void func_002961B0(LoadObj *result, LoadObj *owner);

typedef struct FileJobBufferSlot {
    u32 offset;
    u32 size;
    void *allocation;
    u16 selector;
    u16 unkE;
} FileJobBufferSlot;

typedef struct FileJob {
    u32 unk0;
    u16 type;
    u16 unk6;
    void *data;
    u16 option;
    u16 unkE;
    FileJobBufferSlot slots[2];
    u8 unk30[0x60];
    u32 id;
    u32 sector;
    u32 flags;
    u8 unk9C[0x10];
    struct FileJob *next;
    struct FileJob *prev;
} FileJob;
extern FileJob *func_002933A8(u16 type);
extern void func_00293528(FileJob *job);
extern void func_00293568(FileJob *job);
extern FileJob *func_00294070(void);
extern void func_0029A730(s32 arg0);

typedef struct FileQueue {
    u8 unk0[0x80];
    s32 count;
    u32 unk84;
    FileJob *head;
    FileJob *tail;
} FileQueue;
extern void func_00293F60(FileQueue *queue, FileJob *job);

void func_0028A0E0(s32 request, u32 first, u32 second) {
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

void func_0028A190(void *dst, s32 number) {
    func_003014F0(dst, "BASLUS-%05d-new-%d", D_003BC888, number);
}

void func_0028A1B8(void) {
    s32 slot = func_00289D00(D_003BC7E8);
    D_003BC864 = D_003DC803[slot * 0x30];
}

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

void func_0028A288(s32 x, s32 y, u32 first, u32 second) {
    D_003BD8EC = func_00197760(x << 4, y << 3, 0, first, second, 0);
    func_00195880(D_003BD8EC, 1);
    func_00194920(D_003BD8EC);
}

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

s32 func_0028A5D8(void) {
    if (D_003BC860 < 7) {
        return 0;
    }
    return 1;
}

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

void *func_0028B058(void) {
    s32 status = func_0028A088();
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        func_0028A008(D_003BD91C);
        return func_0028B0B8;
    }
    func_002D0918(D_003BD920);
    return func_0028B3F0();
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028B0B8);

void *func_0028B1F0(void *callback) {
    func_00105B98(0, 0, 0, 15);
    D_003BD900 = callback;
    D_003BC800 = 20;
    return func_0028BE10;
}

void *func_0028B238(void) {
    D_003BC848 = 0;
    func_0028AB30(0);
    D_003BC854 = 0;
    D_003BC834 = 4;
    D_003BC810 = 0;
    return (void *)func_0028FB48(&func_0028B430, &func_0028B2C0, 0);
}

void *func_0028B280(void) {
    func_0028AB30(0);
    D_003BC854 = 0;
    D_003BC834 = 9;
    return (void *)func_0028FB48(&func_0028C670, &func_0028B590, 1);
}

void *func_0028B2C0(void) {
    func_0028AB30(0);
    D_003BC854 = 0;
    D_003BC834 = 8;
    D_003BC810 = 0;
    return (void *)func_0028FB48(&func_0028B508, &func_0028B238, 1);
}


void *func_0028B300(void) {
    func_0028AB30(0);
    D_003BC810 = 0;
    return func_0028B930;
}

void *func_0028B328(void) {
    func_00289A58(0);
    D_003BC808 = 30;
    D_003BC804 = 1;
    func_0028AB30(1);
    D_003BC810 = 1;
    return func_0028B9F8;
}

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

void *func_0028B430(void) {
    func_0028AE68();
    D_003DC7C0[0] = 0;
    D_003DC7C0[1] = 0;
    D_003DC7C0[2] = 0;
    D_003DC7C0[3] = 0;
    D_003DC7C0[4] = 0;
    D_003DC7C0[5] = 0;
    D_003DC7C0[6] = 0;
    D_003DC7C0[7] = 0;
    D_003DC7C0[8] = 0;
    D_003DC7C0[9] = 0;
    func_00289A58(0);
    D_003BC808 = 15;
    D_003BC804 = 1;
    func_0028AB30(1);
    D_003BC810 = 1;
    return func_0028B748;
}

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

s32 func_0028B890(void) {
    s32 count = 0;
    s32 index;
    for (index = 0; index < 10; index++) {
        u32 flags = func_00289CD0(D_003BC7E8, index);
        if (flags & 1) {
            if (flags & 2) {
                if (flags & 8) {
                    count++;
                }
            }
        }
    }
    return count;
}

s32 func_0028B900(void) {
    return func_00289BC0(D_003BC7E8) > 0x4C3FF;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028B930);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028B9F8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028BAC0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028BCD0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028BDA0);

void *func_0028BE10(void) {
    s32 remaining = D_003BC800 - 1;
    D_003BC800 = remaining;
    if (remaining <= 0) {
        if (D_003BD904 == -1) {
            return (void *)-1;
        }
        return ((void *(*)(void))D_003BD900)();
    }
    return NULL;
}

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

void *func_0028C208(void) {
    s32 value;
    s32 status = func_00289EB0(&value);
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        if (value == 1) {
            func_00289C98(D_003BC7E8, D_003BC828, 9);
        }
        return func_0028AF00();
    }
    if (status == -1) {
        return func_0028B3F0();
    }
    return func_0028AF00();
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028C288);

void func_0028C358(void) {
    s32 index;
    if (func_00289BE8(D_003BC7E8) != 0) {
        func_0028AB30(1);
        D_003BC828 = 0;
        index = 0;
        do {
            D_003DC7C0[index] = 0;
            func_00289C68(D_003BC7E8, index);
            index++;
        } while (index < 10);
        func_0028C140();
    } else {
        D_003BC804 = 0;
        func_0028AB30(0);
        func_0028B590();
    }
}

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

void *func_0028C4D0(void) {
    s32 value;
    s32 status = func_00289EB0(&value);
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        if (value == 1) {
            func_00289C98(D_003BC7E8, D_003BC828, 9);
        }
        return func_0028C560();
    }
    if (status == -1) {
        func_0028AB30(0);
        D_003BC810 = 0;
        D_003BC854 = 0;
        return (void *)func_0028B370();
    }
    return func_0028C560();
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028C560);

void *func_0028C600(void) {
    s32 index = 0;
    u32 *saved;
    func_0028AB30(1);
    D_003BC810 = 0;
    D_003BC828 = 0;
    func_00289C38(D_003BC7E8);
    saved = D_003DC7C0;
    do {
        *saved++ = 0;
        func_00289C68(D_003BC7E8, index);
        index++;
    } while (index < 10);
    return func_0028C3F0();
}

void *func_0028C670(void) {
    u8 name[0x50];
    u32 entry = D_003BC7E8;
    s32 slot = func_00289D00(entry);
    func_00289CD0(entry, slot);
    func_0028AB30(2);
    func_00289C38(entry);
    if (func_00289CD0(entry, slot) & 2) {
        return func_0028C710();
    }
    name[0] = '/';
    func_0028A190(name + 1, slot);
    func_00289E18(entry, name);
    return func_0028C760;
}

void *func_0028C710(void) {
    u8 buf[0x50];
    u32 entry = D_003BC7E8;
    s32 v = func_00289D00(entry);

    buf[0] = 0x2F;
    func_0028A190(&buf[1], v);
    func_00289DA8(entry, buf);
    return func_0028C7C8;
}

void *func_0028C760(void) {
    s32 status = func_00289E38();
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        func_00289C98(D_003BC7E8, D_003BC844, 2);
        return func_0028C710();
    }
    func_0028AB30(0);
    D_003BC854 = 4;
    return func_0028BDA0;
}

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

void *func_0028C970(void) {
    return func_0028D708(D_003B2688, &D_003BD914, &D_003BD918,
                          func_0028C8F8, &D_003BC7F8);
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028C9A0);

void *func_0028D6A0(void) {
    u32 entry = D_003BC7E8;
    s32 slot = func_00289D00(entry);
    u32 flags = func_00289CD0(entry, slot);
    if (!(flags & 8)) {
        return func_0028C9A0(entry, D_003B2678);
    }
    func_00289F10();
    return func_0028C828;
}

void *func_0028D708(const char *name, u32 *first, u32 *second, void *callback, u32 *status) {
    D_003BD928 = first;
    D_003BD92C = second;
    D_003BD930 = callback;
    D_003BD934 = status;
    func_00289F80(D_003BC7E8, name, 0x203);
    return func_0028D748;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028D748);

void *func_0028D7E8(void) {
    s32 status = func_0028A0F8();
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        func_0028A008(D_003BD91C);
        return func_0028D850;
    }
    if (status == -1) {
        func_0028AB30(0);
        D_003BC854 = 4;
        return func_0028BDA0;
    }
    return NULL;
}

void *func_0028D850(void) {
    s32 status = func_0028A020();
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        return ((void *(*)(void))D_003BD930)();
    }
    if (status == -1) {
        func_0028AB30(0);
        D_003BC854 = 4;
        return func_0028BDA0;
    }
    return NULL;
}

void *func_0028D8B8(void) {
    if (D_003BC7F8 != 0) {
        return NULL;
    }
    func_0028A0E0(D_003BD91C, *D_003BD928, *D_003BD92C);
    return func_0028D7E8;
}

void *func_0028D8F8(void) {
    s32 status = func_00289D68();
    if (status == 0) {
        return NULL;
    }
    func_001005B0();
    if (status < 0) {
        func_0028AB30(0);
        D_003BC854 = 6;
        return func_0028BDA0;
    }
    func_0028AB30(6);
    return func_0028BF08((u32)func_0028B560);
}

void *func_0028D978(void) {
    func_0028AB30(5);
    func_001005B8();
    func_00289D50(D_003BC7E8);
    return func_0028D8F8;
}

s32 func_0028D9B0(void) {
    if (D_003BC824 != 0) {
        D_003BC834 = 7;
        return func_0028FB48(func_0028B2C0, func_0028B430, 1);
    }
    D_003BC834 = 7;
    return func_0028FB48(func_0028B4B0, func_0028B430, 1);
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028DA10);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028DAF8);

void *func_0028DBA0(void) {
    s32 status = func_0028A088();
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        func_0028A008(D_003BD91C);
        return func_0028DC08;
    }
    func_002D0918(D_003BD920);
    func_0028AB30(0);
    D_003BC854 = 3;
    return func_0028BDA0;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028DC08);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028DCC0);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2658);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2668);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2678);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2688);

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

void func_00290C58(s8 mode) {
    D_0037D4A0.unk14 = 0;
    D_0037D4A0.unk10 = mode;
    if (mode == 1) {
        D_0037D4A0.unk1C = 0;
        D_0037D4A0.unk18 = 0x74;
    } else {
        D_0037D4A0.unk18 = 0;
        D_0037D4A0.unk1C = 0x74;
    }
}

void func_00290C98(void) {
    D_0037D4A0.unk14 = 0;
    D_0037D4A0.unk10 = 0;
    D_0037D4A0.unk4 = 0;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00290CB0);

s32 func_00290E20(void) {
    return D_003BC8D8.current != D_003BC8D8.previous;
}

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

void func_00291378(void) {
    kwlnTaskDestroyWithHierarchyByName(D_003BC8E8, 1);
    kwlnTaskDestroyWithHierarchyByName(D_003B29D8, 1);
    kwlnTaskDestroyWithHierarchyByName(D_003B29E8, 1);
}

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

s32 func_00291880(void) {
    if (*(u32 *)(D_003BD938 + 0x34) == 0) {
        return 0;
    }
    if (*(s32 *)(D_003BD938 + 0x24) < 0) {
        return -1;
    }
    func_00292720((void *)D_003BD938);
    func_0027C370(0x400, 0x400, 0, *(u32 *)(D_003BD938 + 0xC), 0x53);
    return 0;
}

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

void func_00292BA8(void) {
    func_003003F0(D_003BC900);
    func_002A4318();
    func_003003F0(D_003BC908);
    func_002AE8A8();
    func_003003F0(D_003BC910);
    func_002B1170();
    func_003003F0(D_003BC918);
    func_00292C40();
    func_003003F0(D_003BC920);
}

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

FileJob *func_002933A8(u16 type) {
    u16 kind = type;
    FileJob *job = func_002CFEB8(0x2C);
    memset(job, 0, 0x2C);
    job->unk0 = 200;
    job->type = kind;
    return job;
}

void *func_00293400(FileJob *job) {
    if (job->slots[0].allocation != NULL) {
        return (void *)job->slots[0].offset;
    }
    if (job->slots[0].offset != 0) {
        return (u8 *)job + job->slots[0].offset;
    }
    return NULL;
}

void *func_00293428(FileJob *job) {
    if (job->slots[1].allocation != NULL) {
        return (void *)job->slots[1].offset;
    }
    if (job->slots[1].offset != 0) {
        return (u8 *)job + job->slots[1].offset;
    }
    return NULL;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00293450);

void func_002934C0(FileJob *job) {
    void *data = *(void **)((u8 *)job + 8);
    if (data != NULL) {
        u16 index = *(u16 *)((u8 *)job + 4);
        D_0037E148[index].destroy(data);
    }
    func_00293528(job);
    func_00293568(job);
    func_002CFF98(job);
}

void func_00293528(FileJob *job) {
    void *buffer = job->slots[0].allocation;
    if (buffer != NULL) {
        func_002D0918(buffer);
        job->slots[0].offset = 0;
        job->slots[0].size = 0;
        job->slots[0].allocation = NULL;
    }
}

void func_00293568(FileJob *job) {
    void *buffer = job->slots[1].allocation;
    if (buffer != NULL) {
        func_002D0918(buffer);
        job->slots[1].offset = 0;
        job->slots[1].size = 0;
        job->slots[1].allocation = NULL;
    }
}

FileJob *func_002935A8(FileJob *request) {
    FileJob *job = func_002933A8(request->type);
    job->option = request->option;
    job->slots[0].selector = request->slots[0].selector;
    job->data = D_0037E148[job->type].createChild(request->data, job->type);
    return job;
}

void func_00293620(FileJob *left, FileJob *right) {
    void (*cb)(void *, void *) = D_0037E14C[right->type].cbC;
    if (cb != NULL) {
        cb(left->data, right->data);
    }
}

void func_00293668(void *arg0) {
    u16 idx = *(u16 *)((u8 *)arg0 + 4);
    void (*cb)(void *) = D_0037E14C[idx].cb10;
    if (cb != NULL) {
        cb(*(void **)((u8 *)arg0 + 8));
    }
}

void func_002936A8(void *arg0) {
    u16 idx = *(u16 *)((u8 *)arg0 + 4);
    void *data = *(void **)((u8 *)arg0 + 8);

    D_0037E14C[idx].cb(data);
}

void func_002936E0(void *arg0) {
    u16 idx = *(u16 *)((u8 *)arg0 + 4);
    void (*cb)(void *) = D_0037E14C[idx].cb14;
    if (cb != NULL) {
        cb(*(void **)((u8 *)arg0 + 8));
    }
}

void func_00293720(void *arg0) {
    u16 idx = *(u16 *)((u8 *)arg0 + 4);
    void (*cb)(void *) = D_0037E14C[idx].cb18;
    if (cb != NULL) {
        cb(*(void **)((u8 *)arg0 + 8));
    }
}

void func_00293760(void *arg0) {
    u16 idx = *(u16 *)((u8 *)arg0 + 4);
    void (*cb)(void *) = D_0037E14C[idx].cb1C;
    if (cb != NULL) {
        cb(*(void **)((u8 *)arg0 + 8));
    }
}

void func_002937A0(void *arg0) {
    u16 idx = *(u16 *)((u8 *)arg0 + 4);
    void (*cb)(void *) = D_0037E14C[idx].cb20;
    if (cb != NULL) {
        cb(*(void **)((u8 *)arg0 + 8));
    }
}

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

void *func_00293D08(void *source) {
    FileJob *request = source;
    FileJob *job = func_002933A8(request->type);
    if (request->slots[0].size != 0) {
        func_002937E0(job, func_00293400(request), request->slots[0].size, request->option);
    }
    if (request->slots[1].size != 0) {
        func_00293960(job, func_00293428(request), request->slots[1].size, request->slots[0].selector);
    }
    return job;
}

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

void func_00293F60(FileQueue *queue, FileJob *job) {
    job->next = NULL;
    if (queue->head != NULL) {
        queue->head->next = job;
        job->prev = queue->head;
    } else {
        queue->tail = job;
        job->prev = NULL;
    }
    queue->head = job;
    queue->count++;
}

void func_00293F98(FileQueue *queue, FileJob *after, FileJob *job) {
    if (after->next != NULL) {
        after->next->prev = job;
        job->next = after->next;
    } else {
        job->next = NULL;
        queue->head = job;
    }
    after->next = job;
    job->prev = after;
    queue->count++;
}

void func_00293FD8(FileQueue *queue, FileJob *job) {
    if (job->prev != NULL) {
        job->prev->next = job->next;
    } else {
        queue->tail = job->next;
    }
    if (job->next != NULL) {
        job->next->prev = job->prev;
    } else {
        queue->head = job->prev;
    }
    queue->count--;
}

FileQueue *func_00294020(void) {
    FileQueue *queue = func_002CFEB8(0x90);
    memset(queue, 0, 0x90);
    queue->count = 0;
    queue->unk84 = 0;
    func_00293EA0(queue);
    return queue;
}

FileJob *func_00294070(void) {
    FileJob *job = func_002CFEB8(0xC0);
    memset(job, 0, 0xC0);
    func_00293F18(job);
    return job;
}

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

void func_00294630(FileQueue *queue) {
    FileJob *job;

    for (job = queue->tail; job != NULL; job = job->next) {
        func_00293668((void *)job->id);
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

FileJob *func_00294A50(FileQueue *queue, u32 id) {
    FileJob *job = func_00294070();
    job->id = id;
    func_00293F60(queue, job);
    return job;
}

void func_00294AA0(FileQueue *queue, void *source) {
    void *job = func_00293D08(source);
    func_00294A50(queue, (u32)job);
}

FileJob *func_00294AD0(FileQueue *queue, void *entry) {
    func_003003F0(D_003BC940);
    return func_00294A50(queue, (u32)func_00293D90(entry));
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00294B18);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00294C30);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00294DA0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00294E50);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00294F50);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00295018);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002954F0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002959E8);

FileJob *func_00295B70(FileQueue *queue, u32 id) {
    FileJob *job = queue->tail;
    while (job != NULL) {
        if (job->id == id) {
            return job;
        }
        job = job->next;
    }
    return NULL;
}

FileJob *func_00295BB0(FileQueue *queue, u32 id) {
    FileJob *job = queue->tail;
    while (job != NULL) {
        if ((job->flags & 1) != 0 && job->id == id) {
            return job;
        }
        job = job->next;
    }
    return NULL;
}

FileJob *func_00295BF8(FileQueue *queue, u32 sector) {
    FileJob *job = queue->tail;
    while (job != NULL) {
        if ((job->flags & 3) == 2 && job->sector == sector) {
            return job;
        }
        job = job->next;
    }
    return NULL;
}

FileJob *func_00295C48(FileQueue *queue, s32 index) {
    FileJob *job = queue->tail;
    while (job != NULL) {
        if (index-- == 0) {
            return job;
        }
        job = job->next;
    }
    return NULL;
}

s32 func_00295C90(FileQueue *queue, FileJob *target) {
    FileJob *job = queue->tail;
    s32 index = 0;
    while (job != NULL) {
        if (job == target) {
            return index;
        }
        job = job->next;
        index++;
    }
    return 0;
}

s32 func_00295CD0(FileQueue *queue) {
    FileJob *job;
    s32 count = 0;
    for (job = queue->tail; job != NULL; job = job->next) {
        count++;
    }
    return count;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00295D08);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00295E00);

LoadObj *func_00295EF8(void *owner) {
    LoadObj *obj = func_002CFF68(0x4C);
    obj->owner = owner;
    obj->color = 0x80808080;
    obj->scale = 1.0f;
    obj->unk44 = NULL;
    obj->deviceHandle = NULL;
    obj->unk38 = 0;
    obj->unk3C = 0;
    obj->unk48 = 1;
    return obj;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00295F58);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2A18);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00295F90);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00296088);

LoadObj *func_00296148(LoadObj *owner) {
    LoadObj *source = *(LoadObj **)((u8 *)owner->unk44 + 0x24);
    LoadObj *result = func_00295F58(source);
    func_00296358(result, *(u16 *)owner->unk44, source);
    func_002961B0(result, owner);
    return result;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002961B0);

void func_00296358(LoadObj *obj, u32 type, void *data) {
    if (obj->unk44 != NULL) {
        func_0029A730((s32)obj->unk44);
    }
    obj->unk44 = func_0029A5E0(type, (u32)obj->owner, data);
}

void func_002963B8(LoadObj *obj, void *name) {
    void *handle;
    if (obj->deviceHandle != NULL) {
        func_00151F00(obj->deviceHandle);
    }
    handle = func_00151F58(name);
    obj->deviceHandle = handle;
    if (obj->unk44 != NULL) {
        void *record = *(void **)((u8 *)obj->unk44 + 0x20);
        func_00152050(handle, *(s16 *)((u8 *)record + 0x54));
    }
}

void func_00296430(LoadObj *obj, void *name) {
    void *handle;
    if (obj->deviceHandle != NULL) {
        func_00151F00(obj->deviceHandle);
    }
    handle = func_00151D88(0, name);
    obj->deviceHandle = handle;
    if (obj->unk44 != NULL) {
        void *record = *(void **)((u8 *)obj->unk44 + 0x20);
        func_00152050(handle, *(s16 *)((u8 *)record + 0x54));
    }
}

void func_002964B0(LoadObj *obj, void *name) {
    if (obj->deviceHandle != NULL) {
        func_00151F00(obj->deviceHandle);
    }
    obj->deviceHandle = func_00151D88(1, name);
    func_001523B0(obj->deviceHandle);
    if (obj->unk44 != NULL) {
        void *record = *(void **)((u8 *)obj->unk44 + 0x20);
        func_00152050(obj->deviceHandle, *(s16 *)((u8 *)record + 0x54));
    }
}

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
        func_0029A748((FileRecordSlots *)*(s32 *)(arg0 + 0x44));
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
    arg0->scale = arg1;
    func_0029A810(arg0->unk44);
}

void func_00296F18(FileRecordSlots *record) {
    u32 count = record->count;
    u32 index = 0;
    FileRecordSlot *slot = record->slots;
    if (count != 0) {
        do {
            index++;
            slot->state = 0xffffffff;
            slot++;
        } while (index < count);
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

void func_0029A748(FileRecordSlots *record) {
    record->references = 0;
}

void func_0029A750(FileRecordSlots *record) {
    if (record->references == 0) {
        func_00296F18(record);
    }
    D_0037E550[record->type][0](record);
    record->references++;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0029A7B0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0029A7C8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0029A7E0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0029A7F8);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC7E8);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC7EC);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC7ED);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC7F0);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC7F8);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC7FC);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC800);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC804);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC808);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC80C);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC810);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC814);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC818);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC81C);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC820);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC824);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC828);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC82C);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC830);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC834);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC838);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC83C);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC840);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC844);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC848);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC84C);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC850);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC854);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC858);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC85C);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC860);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC864);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC868);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC86C);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC870);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC874);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC878);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC87C);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC880);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC884);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC888);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC88C);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC890);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC894);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC898);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC8A0);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC8A8);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC8B0);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC8B8);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC8C0);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC8D0);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC8D5);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC8D8);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC8DC);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC8E0);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC8E8);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC8F0);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC8F8);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC900);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC908);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC910);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC918);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC920);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC928);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC930);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC938);


INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC940);

