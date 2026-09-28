#include "common.h"

extern u64 func_00279DC8(u32, u64, u64, u64, u64, u64);

extern u64 func_0027A628(u32, u64, u64);

extern u32 func_00284AE0(void);

extern u32 func_002843C0(u32);

extern s32 func_0026FDC8(u32, u32);

extern s32 func_00328D68(u32);

void func_003297C8(u32 sprite);

void func_002844E8(u32 sprite);

void func_00284B48(u32 *obj);
u32 func_002850B8(void);
extern void func_00286270();
u32 func_0026F700(u32);
u32 func_0026F778(u32);
extern u32 func_0026FC88(u32, u32, void (*)(), void (*)(), u32 (*)(), void (*)(), u32, u32, u32);
extern void func_00279308(void);
extern u32 func_00279440(u32, u32);
extern u32 func_00279180();
extern u32 func_00279488();
extern u32 func_00279628();
extern void func_002792D8(s32);
void func_00271368();
void func_00271510();
u32 func_002712E0();
void func_0026FBD8(u32);
void func_00271348();
void func_00275358(u32, u32, u32, u32);
extern void func_00270210();
extern void func_00270848();
extern u32 func_00270160();
extern void func_002701D0();
extern void func_00270DD8();
extern void func_00270F10();
extern u32 func_00270D60();
extern void func_00270DB0();
extern void func_00272DA8();
extern void func_00272F08();
extern u32 func_00272D20();
extern void func_00272D80();
extern void func_00273668();
extern void func_00273828();
extern u32 func_002735F0();
extern void func_00273640();
extern void func_002741D0();
extern void func_002743B8();
extern u32 func_00274158();
extern void func_002741A8();
extern void func_00274EA8();
extern void func_00274FF8();
extern u32 func_00274E30(void);
extern void func_00274E88();
extern u32 D_00453D00[12];
extern u8 *D_00435DD0;

typedef struct MenuRecord {
    u16 type;
    u16 flags;
    u8 unk_04[12];
    u32 unk_10;
    u8 unk_14[4];
} MenuRecord;
extern u32 func_002C7FF0(const char *);
extern void *kwlnTaskCreate(const char *, s32, s32, s32, s32 (*)(void),
                            void (*)(), void *);
s32 func_0026E6E0(void);
extern u32 func_0026E788(u32, u32, u32, u32, u32, u32, u32);
void func_00284508(u32, u32, u32, u32, u32, u32);
extern void func_002758B8(s16, s16, u32, s32, u32);
extern char D_004250D8[];
extern void func_002748D0();
extern void func_00274A70();
extern u32 func_00274820();
extern void func_00274890();
void func_00294580(u32, u32);
void func_003054E8(u32);
extern u32 func_003292A8(u32);
extern u32 func_003292A0(u32);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026DBF8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026DC48);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026DE08);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026DF48);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026E060);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026E198);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026E2D8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026E470);

void func_0026E4C8(u32 *values) {
    s32 i;
    for (i = 0; i < 12; i++) {
        if (values[i] != 0) {
            D_00453D00[i] = values[i];
        }
    }
}

void func_0026E508(void) {
    s32 i;
    u32 *slot = D_00453D00;
    for (i = 1; i >= 0; i--, slot++) {
        if (*slot != 0) {
            func_003054E8(*slot);
        }
        *slot = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026E560);

void func_0026E5F8(void) {
    if (D_00453D00[4] == 0) {
        u32 resource = func_002C7FF0("/facility/spr/mantra/sprite_a.lb");
        kwlnTaskCreate(D_004250D8, 0x402, 1, 1, func_0026E6E0, 0,
                       (void *)resource);
    }
}

s32 func_0026E650(void) {
    if (D_00453D00[4] != 0) {
        return 1;
    }
    return func_00101740(D_004250D8) == 0;
}

void func_0026E688(void) {
    s32 i;
    u32 *slot = D_00453D00;
    for (i = 11; i >= 0; i--, slot++) {
        if (*slot != 0) {
            func_003054E8(*slot);
        }
        *slot = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026E6E0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026E788);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026E998);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026EBA8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026EDB0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026EEE8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026F008);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004250D8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004250E8);

u32 func_0026F138(u32 unused1, u32 unused2, u32 third, u32 record,
                  u32 position, u32 packet) {
    char markers[9] = { '\0', '/', '4', '0', '2', '1', '3', '5', '6' };
    u16 index = *(u16 *)(record + 4);
    return func_0026E788(0, 0, third, position, markers[index], 0, packet);
}

u32 func_0026F190(u32 unused1, u32 unused2, u32 third, u32 record,
                  u32 position, u32 packet) {
    char markers[9] = { '\0', '/', '4', '0', '2', '1', '3', '5', '6' };
    u16 index = *(u16 *)(record + 4);
    return func_0026E788(0, 0, third, position, markers[index] + 8, 0,
                          packet);
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026F1F0);

s32 *func_0026F570(void) {
    s32 *temp_v0 = (s32 *)func_00328D68(0x14);

    memset(temp_v0, 0, 0x14);
    return temp_v0;
}

u32 func_0026F5B0(s32 arg0) {
    u32 temp_v0;

    temp_v0 = *(u32 *)(arg0 + 0x10);
    func_00328E48();
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026F5D8);

u32 func_0026F680(u32 state, s8 selection) {
    u32 item;
    u32 *entries;
    if (*(s16 *)(state + 0x2a) == selection) {
        return 0;
    }
    item = func_0026F7E8(state);
    if (item != 0) {
        u32 *selected;
        u32 *previous;
        entries = (u32 *)(state + 8);
        *(u16 *)(item + 0xc) = 2;
        selected = entries + selection;
        previous = entries + *(s16 *)(state + 0x2a);
        *(s16 *)(state + 0x2a) = selection;
        *(u32 *)(item + 4) = *previous;
        *(u32 *)(item + 8) = *selected;
    }
    return item;
}

u32 func_0026F700(u32 state) {
    u32 item = func_0026F7E8(state);
    u32 *entries = (u32 *)(state + 8);
    if (item != 0) {
        s16 index = *(s16 *)(state + 0x2a);
        s16 count = *(s16 *)(state + 0x28);
        s32 next = index + 1;
        u32 *current;
        u32 *upcoming;
        *(u16 *)(item + 0xc) = 2;
        if (index >= count - 1) {
            next = 0;
        }
        current = entries + *(s16 *)(state + 0x2a);
        upcoming = entries + next;
        *(s16 *)(state + 0x2a) = next;
        *(u32 *)(item + 4) = *current;
        *(u32 *)(item + 8) = *upcoming;
    }
    return item;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026F778);

u32 func_0026F7E8(u32 state) {
    u32 node = *(u32 *)(state + 4);
    if (node == 0) {
        node = (u32)func_0026F570();
        *(u32 *)(state + 4) = node;
    } else {
        while (*(u32 *)(node + 0x10) != 0) {
            node = *(u32 *)(node + 0x10);
        }
        *(u32 *)(node + 0x10) = (u32)func_0026F570();
        node = *(u32 *)(node + 0x10);
    }
    return node;
}

void func_0026F870(u32 state) {
    u32 node = *(u32 *)(state + 4);
    while (node != 0) {
        node = func_0026F5B0(node);
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026F8A0);

void func_0026FAA8(s32 arg0) {
    *(u32 *)(arg0 + 0x2c) = 0x1e;
}

u32 func_0026FAB8(void) {
    return 0;
}

u32 func_0026FAC0(void) {
    return 0;
}

void func_0026FAC8(void) {
}

u32 func_0026FAD0(u32 count) {
    u32 size = count * 0x24 + 0xc;
    u32 handle = func_003292A8(size);
    u32 pool = func_003292A0(handle);
    memset((void *)pool, 0, size);
    *(u32 *)pool = handle;
    *(u32 *)(pool + 8) = count;
    *(u32 *)(pool + 4) = pool + 0xc;
    func_0010AE38("mtrDrawProcessCreate!! num[%d]\n", count);
    return pool;
}

void func_0026FB68(u32 pool) {
    s32 count = *(s32 *)(pool + 8);
    u32 item = *(u32 *)(pool + 4);
    s32 i;
    for (i = 0; i < count; i++, item += 0x24) {
        if (*(u32 *)(item + 4) & 1) {
            func_0026FBD8(item);
        }
    }
    func_003297C8(*(u32 *)pool);
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026FBD8);

s32 func_0026FC40(s32 list) {
    s32 count = *(s32 *)(list + 8);
    s32 i;
    s32 item = *(s32 *)(list + 4);
    for (i = 0; i < count; i++, item += 0x24) {
        if ((*(u32 *)(item + 4) & 1) == 0) {
            return item;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026FC88);

s32 func_0026FDC8(u32 list, u32 kind) {
    s32 count = *(s32 *)(list + 8);
    s32 i;
    s32 item = *(s32 *)(list + 4);
    for (i = 0; i < count; i++, item += 0x24) {
        if ((*(u32 *)(item + 4) & 0x7f9) == 0x21 && *(u32 *)item == kind) {
            return item;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026FE18);

u32 func_00270008(u32 arg0) {
    return func_0026FC88(arg0, 0, func_00270210, func_00270848,
                         func_00270160, func_002701D0, 0, 0, 0);
}

void func_00270050(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 0);
    **(u16 **)(temp_v0 + 0x20) = 3;
}

void func_00270078(u32 arg0, s8 arg1) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 0);
    temp_v0 = *(s32 *)(temp_v0 + 0x20);
    *(u32 *)(temp_v0 + 4) = (*(u32 *)(temp_v0 + 4) & 0xffffff0f) | (((s32)arg1 & 0xfU) << 4);
    *(u8 *)(temp_v0 + 5) = 5;
}

void func_002700D0(u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 0);
    if (obj != 0) {
        *(u32 *)(*(s32 *)(obj + 0x20) + 0x10) = 1;
    }
}

void func_00270100(u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 0);
    if (obj != 0) {
        *(u32 *)(*(s32 *)(obj + 0x20) + 0x10) = 0;
    }
}

void func_00270128(u32 arg0, u32 value) {
    s32 obj = func_0026FDC8(arg0, 0);
    if (obj != 0) {
        *(u32 *)(*(s32 *)(obj + 0x20) + 0x14) = value;
    }
}


u32 func_00270160(void) {
    u32 data = func_00328D68(0x1c);
    memset((void *)data, 0, 0x1c);
    *(u16 *)data = 1;
    *(u32 *)(data + 4) &= ~0xf;
    *(u8 *)(data + 5) = 0;
    *(u32 *)(data + 0x18) = func_002850B8();
    func_0010AE38("BG Draw Init\n");
    return data;
}

void func_002701D0(u32 obj) {
    s32 data = *(s32 *)(obj + 0x20);
    func_00285120(*(u32 **)(data + 0x18));
    func_00328E48(data);
    func_0010AE38("BG Draw Release\n");
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00270210);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00270390);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00270568);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00270848);

u32 func_00270C78(u32 arg0) {
    return func_0026FC88(arg0, 4, func_00270DD8, func_00270F10,
                         func_00270D60, func_00270DB0, 0, 0, 0);
}

void func_00270CC0(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 4);
    **(u16 **)(temp_v0 + 0x20) = 3;
}

void func_00270CE8(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 4);
    **(u16 **)(temp_v0 + 0x20) = 6;
}

void func_00270D10(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 4);
    **(u16 **)(temp_v0 + 0x20) = 5;
}

void func_00270D38(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 4);
    **(u16 **)(temp_v0 + 0x20) = 2;
}

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004251C8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425258);

u32 func_00270D60(void) {
    u32 data = func_00328D68(0xc);
    memset((void *)data, 0, 0xc);
    *(u16 *)data = 1;
    func_0010AE38("BGMask Draw Init\n");
    return data;
}

void func_00270DB0(u32 obj) {
    func_00328E48(*(u32 *)(obj + 0x20));
    func_0010AE38("BGMask Draw Release\n");
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00270DD8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00270F10);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00270FA8);

void func_00271020(u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 1);
    if (obj != 0) {
        s32 data = *(s32 *)(obj + 0x20);
        if (*(u16 *)data == 4) {
            *(u16 *)(data + 2) = 5;
        }
        *(u16 *)data = 3;
    }
}

void func_00271068(u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 1);
    if (obj != 0) {
        *(u16 *)*(s32 *)(obj + 0x20) = 6;
        func_0010AE38("LimitLine Draw Show\n");
    }
}

void func_002710A0(u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 1);
    if (obj != 0) {
        *(u16 *)*(s32 *)(obj + 0x20) = 5;
        func_0010AE38("LimitLine Draw Hide\n");
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002710D8);

void func_002711F8(s16 x, s16 y, u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 1);
    if (obj != 0) {
        s32 data = *(s32 *)(obj + 0x20);
        *(s16 *)(data + 4) = x;
        *(s16 *)(data + 6) = y;
    }
}

void func_00271250(u32 arg0, u32 value) {
    s32 obj = func_0026FDC8(arg0, 1);
    if (obj != 0) {
        s32 data = *(s32 *)(obj + 0x20);
        *(u32 *)(data + 0x10) = value;
        *(u32 *)(data + 0x14) = 0x15;
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00271290);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002712E0);

void func_00271348(u32 obj) {
    func_00328E48(*(u32 *)(obj + 0x20));
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00271368);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00271510);

u32 func_00272BD0(u32 arg0) {
    return func_0026FC88(arg0, 6, func_00272DA8, func_00272F08,
                         func_00272D20, func_00272D80, 0, 0, 0);
}

void func_00272C18(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 6);
    **(u16 **)(temp_v0 + 0x20) = 3;
}

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425338);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425348);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425370);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425380);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004253A8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004253B8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425408);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425420);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425448);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425458);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425490);

void func_00272C40(u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 6);
    if (obj != 0) {
        *(u16 *)*(s32 *)(obj + 0x20) = 5;
        func_0010AE38("Title Draw Hide\n");
    }
}

void func_00272C78(u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 6);
    if (obj != 0) {
        *(u16 *)*(s32 *)(obj + 0x20) = 6;
        func_0010AE38("Title Draw Show\n");
    }
}

void func_00272CB0(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 6);
    temp_v0 = *(s32 *)(temp_v0 + 0x20);
    *(u16 *)(temp_v0 + 0xc) = 10;
    *(u16 *)(temp_v0 + 2) = *(u16 *)(temp_v0 + 2) ^ 1;
}

void func_00272CE8(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 6);
    temp_v0 = *(s32 *)(temp_v0 + 0x20);
    *(u16 *)(temp_v0 + 0xc) = 10;
    *(u16 *)(temp_v0 + 0xe) = *(u16 *)(temp_v0 + 0xe) ^ 1;
}

u32 func_00272D20(void) {
    u32 data = func_00328D68(0x10);
    memset((void *)data, 0, 0x10);
    *(u16 *)(data + 2) = 0;
    *(u16 *)(data + 4) = 0;
    *(u16 *)data = 1;
    *(u16 *)(data + 6) = 0;
    func_0010AE38("Title Draw Init\n");
    return data;
}

void func_00272D80(u32 obj) {
    func_00328E48(*(u32 *)(obj + 0x20));
    func_0010AE38("Title Draw Release\n");
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00272DA8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00272F08);

u32 func_00273450(u32 arg0) {
    return func_0026FC88(arg0, 7, func_00273668, func_00273828,
                         func_002735F0, func_00273640, 10, 0, 0);
}

void func_00273498(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 7);
    **(u16 **)(temp_v0 + 0x20) = 3;
}

void func_002734C0(u32 ctx) {
    s32 obj = func_0026FDC8(ctx, 7);
    if (obj != 0) {
        s32 data = *(s32 *)(obj + 0x20);
        if (*(u16 *)data == 6) {
            *(u32 *)data = ((*(u32 *)data | 0x10000) & 0x1ffff) | 0xa0000;
        } else {
            *(u16 *)data = 5;
        }
        func_0010AE38("Info Draw Hide\n");
    }
}

void func_00273530(u32 ctx) {
    s32 obj = func_0026FDC8(ctx, 7);
    if (obj != 0) {
        s32 data = *(s32 *)(obj + 0x20);
        if (*(u16 *)data == 5) {
            *(u32 *)data = ((*(u32 *)data | 0x10000) & 0x1ffff) | 0xa0000;
        } else {
            *(u16 *)data = 6;
        }
        func_0010AE38("Info Draw Show\n");
    }
}

void func_002735A0(u32 arg0, s16 x, u16 y) {
    s32 obj = func_0026FDC8(arg0, 7);
    if (obj != 0) {
        s32 data = *(s32 *)(obj + 0x20);
        *(u16 *)(data + 8) = x;
        *(u16 *)(data + 0xa) = y;
    }
}

u32 func_002735F0(void) {
    u32 data = func_00328D68(0x10);
    memset((void *)data, 0, 0x10);
    *(u16 *)data = 1;
    func_0010AE38("Info Draw Init\n");
    return data;
}

void func_00273640(u32 obj) {
    func_00328E48(*(u32 *)(obj + 0x20));
    func_0010AE38("Info Draw Release\n");
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00273668);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00273828);

u32 func_00273F88(u32 arg0) {
    return func_0026FC88(arg0, 8, func_002741D0, func_002743B8,
                         func_00274158, func_002741A8, 0, 0, 0);
}

void func_00273FD0(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 8);
    **(u16 **)(temp_v0 + 0x20) = 3;
}

void func_00273FF8(u32 ctx) {
    s32 obj = func_0026FDC8(ctx, 8);
    if (obj != 0) {
        s32 data = *(s32 *)(obj + 0x20);
        u16 state = *(u16 *)data;
        if (state == 6) {
            *(u32 *)(data + 8) =
                ((*(u32 *)(data + 8) | 0x10000) & 0xfe01ffff) | 0xa0000;
        } else if (state != 4) {
            *(u16 *)data = 5;
        }
        func_0010AE38("ScrollCursor Draw Hide\n");
    }
}

void func_00274070(u32 ctx) {
    s32 obj = func_0026FDC8(ctx, 8);
    if (obj != 0) {
        s32 data = *(s32 *)(obj + 0x20);
        u16 state = *(u16 *)data;
        if (state == 5) {
            *(u32 *)(data + 8) =
                ((*(u32 *)(data + 8) | 0x10000) & 0xfe01ffff) | 0xa0000;
        } else if (state != 2) {
            *(u16 *)data = 6;
        }
        func_0010AE38("ScrollCursor Draw Show\n");
    }
}

void func_002740E8(u32 ctx, u16 flags) {
    s32 obj = func_0026FDC8(ctx, 8);
    if (obj != 0) {
        u8 *entry = (u8 *)(*(u32 *)(obj + 0x20) + 2);
        s32 i;
        for (i = 0; i < 4; i++, entry += 2) {
            if ((flags >> i) & 1) {
                *entry = 1;
            } else {
                *entry = 0;
            }
        }
    }
}

u32 func_00274158(void) {
    u32 data = func_00328D68(0x18);
    memset((void *)data, 0, 0x18);
    *(u16 *)data = 4;
    func_0010AE38("ScrollCursor Draw Init\n");
    return data;
}

void func_002741A8(u32 obj) {
    func_00328E48(*(u32 *)(obj + 0x20));
    func_0010AE38("ScrollCursor Draw Release\n");
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002741D0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002743B8);

u32 func_002745E0(u32 arg0, u32 arg1) {
    return func_0026FC88(arg0, 9, func_002748D0, func_00274A70,
                         func_00274820, func_00274890, 10, 0, arg1);
}

void func_00274628(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 9);
    **(u16 **)(temp_v0 + 0x20) = 3;
}

void func_00274650(u32 arg0, s16 value) {
    s32 obj = func_0026FDC8(arg0, 9);
    if (obj != 0) {
        *(s16 *)(*(s32 *)(obj + 0x20) + 2) = value;
    }
}

void func_00274690(u32 ctx) {
    s32 obj = func_0026FDC8(ctx, 9);
    if (obj != 0) {
        s32 data = *(s32 *)(obj + 0x20);
        if (*(u16 *)data == 6) {
            *(u32 *)(data + 0x3c) =
                ((*(u32 *)(data + 0x3c) | 1) & 0xffff0001) | 0xa;
        } else {
            *(u16 *)data = 5;
        }
        func_0010AE38("UnitPanel Draw Hide\n");
    }
}

void func_002746F8(u32 ctx) {
    s32 obj = func_0026FDC8(ctx, 9);
    if (obj != 0) {
        s32 data = *(s32 *)(obj + 0x20);
        if (*(u16 *)data == 5) {
            *(u32 *)(data + 0x3c) =
                ((*(u32 *)(data + 0x3c) | 1) & 0xffff0001) | 0xa;
        } else {
            *(u16 *)data = 6;
        }
        func_0010AE38("UnitPanel Draw Show\n");
    }
}


void func_00274760(u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 9);
    func_0026F778(*(u32 *)(obj + 0x20) + 0xc);
}

void func_00274788(u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 9);
    func_0026F700(*(u32 *)(obj + 0x20) + 0xc);
}

u32 func_002747B0(u32 arg0, s8 value) {
    s32 obj = func_0026FDC8(arg0, 9);
    return func_0026F680(*(u32 *)(obj + 0x20) + 0xc, value) != 0;
}

u32 func_002747F0(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 9);
    func_0026FAA8(*(s32 *)(temp_v0 + 0x20) + 0xc);
    return 0;
}

u32 func_00274820(u32 ctx, u32 resources) {
    u32 data = func_00328D68(0x40);
    memset((void *)data, 0, 0x40);
    *(u16 *)data = 1;
    func_0026F5D8(data + 0xc, resources,
                  *(u32 *)(resources + 0x24), *(u32 *)(resources + 0x20));
    func_0010AE38("UnitPanel Draw Init\n");
    return data;
}

void func_00274890(u32 obj) {
    s32 data = *(s32 *)(obj + 0x20);
    func_0026F870(data + 0xc);
    func_00328E48(data);
    func_0010AE38("UnitPanel Draw Release\n");
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002748D0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00274A70);

u32 func_00274D88(u32 arg0) {
    return func_0026FC88(arg0, 10, func_00274EA8, func_00274FF8,
                         func_00274E30, func_00274E88, 0, 0, 0);
}

void func_00274DD0(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 10);
    **(u16 **)(temp_v0 + 0x20) = 3;
}

void func_00274DF8(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0026FDC8(arg0, 10);
    temp_v0 = *(s32 *)(temp_v0 + 0x20);
    *(u32 *)(temp_v0 + 0xc) = 0xf;
    *(u16 *)(temp_v0 + 2) = *(u16 *)(temp_v0 + 2) ^ 1;
}

u32 func_00274E30(void) {
    MenuRecord *record = (MenuRecord *)func_00328D68(sizeof(MenuRecord));
    memset(record, 0, sizeof(MenuRecord));
    record->type = 1;
    record->flags = 0;
    record->unk_10 = *(u32 *)(D_00435DD0 + 0x3c);
    return (u32)record;
}

void func_00274E88(u32 obj) {
    func_00328E48(*(u32 *)(obj + 0x20));
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00274EA8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00274FF8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00275218);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00275358);

u32 func_00275510(u32 count) {
    u32 size = count * 12 + 0x14;
    u32 handle = func_003292A8(size);
    u32 block = func_003292A0(handle);
    memset((void *)block, 0, size);
    *(u32 *)block = handle;
    *(u32 *)(block + 0x10) = count;
    *(u32 *)(block + 4) = block + 0x14;
    return block;
}

void func_00275598(u32 *sprite) {
    func_003297C8(*sprite);
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002755B8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002756E0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002758B8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002759F8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00275B70);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425828);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00275CE8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00277F38);

u32 func_00278D80(u32 arg0, u32 arg1) {
    return func_0026FC88(arg0, 2, func_00279308, func_00279440,
                         func_00279180, func_002792D8, 0, 0, arg1);
}

void func_00278DC8(u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 2);
    if (obj != 0) {
        *(u16 *)(*(s32 *)(obj + 0x20) + 4) = 3;
    }
}

void func_00278DF8(s16 x, s16 y, u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 2);
    if (obj != 0) {
        s32 data = *(s32 *)(obj + 0x20);
        *(s16 *)(data + 0xc) = x;
        *(s16 *)(data + 0xe) = y;
    }
}

void func_00278E50(u32 a, u32 b, u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 2);
    if (obj != 0) {
        func_00275358(a, b, *(u32 *)*(s32 *)(obj + 0x20), 0x10);
    }
}

void func_00278EA8(u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 2);
    if (obj != 0) {
        func_00275358(0, 0, *(u32 *)*(s32 *)(obj + 0x20), 0x18);
    }
}

void func_00278EE0(u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 2);
    if (obj != 0) {
        func_00275358(0, 0, *(u32 *)*(s32 *)(obj + 0x20), 0x14);
    }
}

void func_00278F18(u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 2);
    if (obj != 0) {
        func_00275358(0, 0, *(u32 *)*(s32 *)(obj + 0x20), 0x11);
        func_00294580(0x20003, 5);
    }
}

void func_00278F60(u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 2);
    if (obj != 0) {
        func_00275358(0, 0, *(u32 *)*(s32 *)(obj + 0x20), 0x91);
        func_00294580(0x20003, 5);
    }
}

void func_00278FA8(u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 2);
    if (obj != 0) {
        func_00275358(0, 0, *(u32 *)*(s32 *)(obj + 0x20), 0x92);
        func_00294580(0x20002, 0);
    }
}

void func_00278FF0(u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 2);
    if (obj != 0) {
        func_00275358(0, 0, *(u32 *)*(s32 *)(obj + 0x20), 0x12);
    }
}

void func_00279028(u32 a, u32 b, u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 2);
    if (obj != 0) {
        func_00275358(a, b, *(u32 *)*(s32 *)(obj + 0x20), 0x20);
    }
}

void func_00279080(u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 2);
    if (obj != 0) {
        func_00275358(0, 0, *(u32 *)*(s32 *)(obj + 0x20), 0x28);
    }
}

void func_002790B8(u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 2);
    if (obj != 0) {
        func_00275358(0, 0, *(u32 *)*(s32 *)(obj + 0x20), 0x24);
    }
}

void func_002790F0(u32 a, u32 b, u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 2);
    if (obj != 0) {
        func_00275358(a, b, *(u32 *)*(s32 *)(obj + 0x20), 0x21);
    }
}

void func_00279148(u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 2);
    if (obj != 0) {
        func_00275358(0, 0, *(u32 *)*(s32 *)(obj + 0x20), 0x22);
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00279180);

void func_002792D8(s32 obj) {
    u32 *data = *(u32 **)(obj + 0x20);
    func_00275598((u32 *)*data);
    func_00328E48(data);
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00279308);

u32 func_00279440(u32 unused, u32 obj) {
    u32 data = *(u32 *)(obj + 0x20);
    s32 scale = (s32)(*(f32 *)(data + 8) * 128.0f);
    func_002758B8(*(s16 *)(data + 0xc), *(s16 *)(data + 0xe), 0, scale,
                  *(u32 *)data);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00279488);

u32 func_002795E0(u32 arg0, u32 arg1) {
    return func_0026FC88(arg0, 3, func_00279308, func_00279440,
                         func_00279488, func_002792D8, 0, 0, arg1);
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00279628);

u32 func_00279778(u32 arg0, u32 arg1) {
    return func_0026FC88(arg0, 3, func_00279308, func_00279440,
                         func_00279628, func_002792D8, 0, 0, arg1);
}

void func_002797C0(u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 3);
    if (obj != 0) {
        *(u16 *)(*(s32 *)(obj + 0x20) + 4) = 3;
    }
}

void func_002797F0(s16 x, s16 y, u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 3);
    if (obj != 0) {
        s32 data = *(s32 *)(obj + 0x20);
        *(s16 *)(data + 0xc) = x;
        *(s16 *)(data + 0xe) = y;
    }
}

void func_00279848(u32 a, u32 b, u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 3);
    if (obj != 0) {
        func_00275358(a, b, *(u32 *)*(s32 *)(obj + 0x20), 0x50);
    }
}

void func_002798A0(u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 3);
    if (obj != 0) {
        func_00275358(0, 0, *(u32 *)*(s32 *)(obj + 0x20), 0x58);
    }
}

void func_002798D8(u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 3);
    if (obj != 0) {
        func_00275358(0, 0, *(u32 *)*(s32 *)(obj + 0x20), 0x54);
    }
}

void func_00279910(u32 a, u32 b, u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 3);
    if (obj != 0) {
        func_00275358(a, b, *(u32 *)*(s32 *)(obj + 0x20), 0x60);
    }
}

void func_00279968(u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 3);
    if (obj != 0) {
        func_00275358(0, 0, *(u32 *)*(s32 *)(obj + 0x20), 0x68);
    }
}

void func_002799A0(u32 arg0) {
    s32 obj = func_0026FDC8(arg0, 3);
    if (obj != 0) {
        func_00275358(0, 0, *(u32 *)*(s32 *)(obj + 0x20), 0x64);
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002799D8);

void func_00279C38(u32 *sprite) {
    if (sprite != 0) {
        func_003297C8(*sprite);
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00279C58);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00279DC8);

void func_00279F30(s32 obj, s32 dx, s32 dy, u32 a, u32 b, u32 c, u8 d, u8 e) {
    dx = (s16)dx + *(u16 *)(obj + 0xc);
    dy = (s16)dy + *(u16 *)(obj + 0xe);
    *(u16 *)(obj + 0xc) = dx;
    *(u16 *)(obj + 0xe) = dy;
    *(u32 *)(obj + 0x10) = a;
    *(u32 *)(obj + 0x14) = b;
    *(u32 *)(obj + 0x18) = c;
    *(u8 *)(obj + 0x1c) = d;
    *(u8 *)(obj + 0x1d) = e;
}

void func_00279F70(u32 *arg0) {
    *arg0 = (*arg0 & 0xfffc03ff) | 0x800;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00279F90);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004258B8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004258F0);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425928);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027A080);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027A198);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027A4C0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027A628);

u32 func_0027A798(u8 *obj, u32 bits, s16 length) {
    u32 flags = (*(u32 *)obj & 0xF87FFFFF) | ((bits & 0xF) << 23);
    *(s16 *)(obj + 0x2E) = length;
    *(u32 *)obj = flags;
    if (length == 0) {
        *(s16 *)(obj + 0x1E) = 0;
        *(u32 *)obj = (flags & 0xFF87FFFF) | ((bits & 0xF) << 19);
    }
    return 0;
}

void func_0027A7F0(void) {
}

void func_0027A7F8(void) {
}

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425988);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004259C0);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004259F8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425A30);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027A800);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027AF40);

void func_0027AFD8(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027AFE0);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425A98);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425AB8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425AC8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027B678);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027C078);

void func_0027C108(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027C110);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027C2F0);

void func_0027C418(u32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = func_002843C0(1);
    *(u32 *)(arg1 + 0x24) = temp_v0;
}

void func_0027C448(u32 obj) {
    func_002844E8(*(u32 *)(obj + 0x24));
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027C468);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027C558);

void func_0027CD78(u32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = func_002843C0(2);
    *(u32 *)(arg1 + 0x24) = temp_v0;
    *(u8 *)(arg1 + 0x20) = 0;
    *(u8 *)(arg1 + 0x21) = 0;
}

void func_0027CDB0(u32 obj) {
    func_002844E8(*(u32 *)(obj + 0x24));
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027CDD0);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425B68);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425B78);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425B88);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027D3D8);

void func_0027DE30(void) {
}

void func_0027DE38(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027DE40);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425BC8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027E0E0);

void func_0027E208(u32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = func_002843C0(0);
    *(u32 *)(arg1 + 0x24) = temp_v0;
    temp_v0 = func_00284AE0();
    *(u32 *)(arg1 + 0x28) = temp_v0;
}

void func_0027E240(s32 obj) {
    func_002844E8(*(u32 *)(obj + 0x24));
    func_00284B48(*(u32 **)(obj + 0x28));
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027E270);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027E360);

void func_0027FB60(void) {
}

void func_0027FB68(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027FB70);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027FC90);

void func_0027FDB8(void) {
}

void func_0027FDC0(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027FDC8);

void func_00280390(u32 arg0, s32 arg1) {
    *(u8 *)(arg1 + 0x20) = 0;
    *(u8 *)(arg1 + 0x21) = 0;
    *(u8 *)(arg1 + 0x22) = 0;
}

void func_002803A0(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002803A8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425C98);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425CA8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425CB8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002805E0);

void func_002817B8(u32 arg0, s32 arg1) {
    *(u8 *)(arg1 + 0x20) = 0;
}

void func_002817C0(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002817C8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425CF8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00281B60);

void func_00281C88(u32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = func_002843C0(1);
    *(u32 *)(arg1 + 0x24) = temp_v0;
}

void func_00281CB8(u32 obj) {
    func_002844E8(*(u32 *)(obj + 0x24));
}

u32 func_00281CD8(u32 ctx, u32 x, u32 y, u32 direction, u32 unused,
                  u32 sprite, u32 animation) {
    func_0026E788(ctx, x, y, direction, 0x77, 0, animation);
    func_0026E788(ctx, x, y, direction, 0xf7, 0, animation);
    func_0026E788(ctx, x, y, direction, 0xf8, 0, animation);
    func_0026E788(ctx, x, y, direction, 0xf9, 0, animation);
    func_00284508(ctx, x, 0, direction, *(u32 *)(sprite + 0x24), animation);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00281DC0);

void func_00282B50(u32 arg0, s32 arg1) {
    *(u8 *)(arg1 + 0x20) = 0;
    *(u8 *)(arg1 + 0x21) = 0;
}

void func_00282B60(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00282B68);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425D68);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00283090);

void func_00283F90(u32 arg0, s32 arg1) {
    *(u8 *)(arg1 + 0x20) = 0;
    *(u8 *)(arg1 + 0x21) = 0;
}

void func_00283FA0(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00283FA8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00284298);
INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002843C0);


void func_002844E8(u32 sprite) {
    if (sprite != 0) {
        func_00328E48(sprite);
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00284508);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00284818);

u32 func_00284AE0(void) {
    u32 handle = func_003292A8(0x650);
    u32 block = func_003292A0(handle);
    memset((void *)block, 0, 0x650);
    *(u32 *)block = handle;
    *(u32 *)(block + 4) = block + 0x10;
    *(u32 *)(block + 8) = 0x64;
    return block;
}

void func_00284B48(u32 *obj) {
    if (*obj != 0) {
        func_003297C8(*obj);
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00284B70);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00284F00);

u32 func_002850B8(void) {
    u32 handle = func_003292A8(0x650);
    u32 block = func_003292A0(handle);
    memset((void *)block, 0, 0x650);
    *(u32 *)block = handle;
    *(u32 *)(block + 4) = block + 0x10;
    *(u32 *)(block + 8) = 0x64;
    return block;
}

void func_00285120(u32 *obj) {
    if (*obj != 0) {
        func_003297C8(*obj);
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00285148);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00285500);

u32 func_002856E0(u32 ctx, u32 config) {
    u32 data = func_00328D68(0x70);
    memset((void *)data, 0, 0x70);
    requestEffectResourceByMode(ctx, config, 0, data + 0x6c);
    return data;
}

u8 func_00285748(s32 arg0) {
    return *(s32 *)(arg0 + 0x6c) != 0;
}

void func_00285758(u32 obj) {
    func_003054E8(*(u32 *)(obj + 0x6c));
    func_00328E48(obj);
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00285788);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00285800);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002858A0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00285A60);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00285AC0);

u32 func_00285BD8(u32 count) {
    u32 size = count * 12 + 0x28;
    u32 handle = func_003292A8(size);
    u32 block = func_003292A0(handle);
    memset((void *)block, 0, size);
    *(u32 *)block = handle;
    *(u32 *)(block + 4) = block + 0x28;
    *(u32 *)(block + 8) = count;
    *(u16 *)(block + 0x16) = 10;
    *(u16 *)(block + 0x18) = 1;
    *(u16 *)(block + 0x1a) = 60;
    *(u8 *)(block + 0x22) = 5;
    *(u16 *)(block + 0x20) = 5;
    *(u16 *)(block + 0x10) = 0x200;
    *(u16 *)(block + 0x12) = 0x1c0;
    *(u16 *)(block + 0x1c) = 13;
    *(u16 *)(block + 0x1e) = 9;
    *(void (**)(void))(block + 0x24) = func_00286270;
    *(u16 *)(block + 0xc) = 0;
    *(u16 *)(block + 0xe) = 0;
    return block;
}

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378A0);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378A8);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378B0);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378B8);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378C0);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378C8);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378D0);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378D8);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378E0);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378E8);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378F0);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378F8);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_00437900);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_00437908);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_00437910);

