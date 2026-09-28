#include "common.h"

extern s32 kwlnTaskGetTaskByName(u32);

typedef struct ShortPair2C {
    u8 pad_0x00[0x2C]; // 0x00
    s16 h2C;           // 0x2C
    s16 h2E;           // 0x2E
} ShortPair2C; // 0x30

extern void func_002CAF78(void *, void *);
extern u32 func_002CB5F0(u32 *);
extern u32 func_002CAD30(u32, u32, u32);

extern void kwlnTaskDestroyWithHierarchyByName(char *, s32);

float func_002C97E8(float *arg0) {
    return *arg0 * *arg0 + arg0[1] * arg0[1] + arg0[2] * arg0[2] +
                  arg0[3] * arg0[3];
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002C9818);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002C9838);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002C98D0);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002C9948);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002C9A08);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002C9AD0);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002C9BF0);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002C9D10);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002C9D98);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002C9F60);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CA0C0);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CA158);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CA210);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CA2F0);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CA410);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CA4A8);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CA508);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CA598);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CA6B0);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CA708);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CA778);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CA858);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CA8F0);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CA988);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CAA20);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CAAC8);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CAC60);

void func_002CACD8(u8 *work) {
    if (work != NULL) {
        func_002CAFE0();
        (*(void (**)(s32, u32))(work + 0x18))(-1, *(u32 *)(work + 0x10));
        func_002D0918(*(u32 *)work);
    }
}

void func_002CAD20(s32 arg0, s32 arg1) {
    if (arg1 != 0) {
        *(s32 *)(arg0 + 0x18) = (s32)arg1;
    }
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CAD30);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CADD0);

void func_002CAEB8(s32 arg0, s32 arg1) {
    if (arg1 != 0) {
        *(s32 *)(arg0 + 0x14) = (s32)arg1;
    }
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CAEC8);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CAF78);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CAFE0);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB058);

void *func_002CB0F8(void *head, s32 key) {
    void *n;

    n = *(void **)((s32)head + 8);
    while (*(s32 *)((s32)n + 4) != key) {
        n = *(void **)((s32)n + 8);
        if (n == NULL) {
            break;
        }
    }
    return n;
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB120);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB1C8);

void func_002CB278(u8 *work) {
    if (work != NULL) {
        kwlnTaskDestroyWithHierarchyByName(*(char **)(work + 4), 1);
        kwlnTaskDestroyWithHierarchyByName(*(char **)(work + 8), 0);
    }
}

u8 func_002CB2B8(s32 arg0) {
    u8 temp_v0;
    s64 temp_v1;

    temp_v0 = 0;
    if (arg0 != 0) {
        temp_v1 = kwlnTaskGetTaskByName(*(u32 *)((s32)arg0 + 4));
        temp_v0 = temp_v1 != 0;
    }
    return temp_v0;
}

s32 func_002CB2E0(u32 name) {
    return kwlnTaskGetTaskByName(name) != 0;
}

void func_002CB300(u8 *work, u32 *item) {
    u32 result = func_002CAD30(*(u32 *)(work + 0xc), *item, func_002CB5F0(item));
    if (*(u32 *)(work + 0x10) == 0) {
        *(u32 *)(work + 0x10) = result;
    }
}

void func_002CB358(u8 *work, s32 key) {
    void *item = func_002CB0F8(*(void **)(work + 0xc), key);
    if (item != NULL) {
        func_002CAF78(*(void **)(work + 0xc), item);
    }
}

s32 func_002CB390(void *p, s32 key) {
    void *r;

    r = func_002CB0F8(*(void **)((s32)p + 0xC), key);
    if (r != NULL) {
        return *(s32 *)((s32)r + 0x10);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB3B8);

INCLUDE_RODATA(const s32, "game/code_002C97E8", D_003B3DC0);

INCLUDE_RODATA(const s32, "game/code_002C97E8", D_003B3E00);

INCLUDE_RODATA(const s32, "game/code_002C97E8", D_003B3E40);

INCLUDE_RODATA(const s32, "game/code_002C97E8", D_003B3EE0);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB3F8);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB4B8);

void func_002CB5A8(u8 *work) {
    if (work != NULL) {
        func_002CACD8(*(u8 **)(work + 0x0C));
        func_002CFF98(*(void **)(work + 4));
        func_002CFF98(*(void **)(work + 8));
        func_002D0918(*(u32 *)work);
    }
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB5F0);

void func_002CB6B8(u8 *work) {
    if (work != NULL) {
        void (*destroy)(u32, u32) = *(void (**)(u32, u32))(work + 0x0C);
        destroy(*(u32 *)(work + 4), *(u32 *)(work + 0x18));
        func_002CFF98(work);
    }
}

void func_002CB6F8(u32 unused, u8 *work) {
    func_002CB6B8(work);
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB718);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB850);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB8E0);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB938);

void func_002CB990(void) {
    func_002CB5A8(func_00101A70());
}

void func_002CB9B8(void) {
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CB9C0);

void func_002CBAF0(ShortPair2C *p, s32 a, s32 b) {
    p->h2C = a;
    p->h2E = b;
}

void func_002CBB00(u8 *work) {
    if (work != NULL) {
        func_002CC570();
        (*(void (**)(s32, u32))(work + 0x20))(0, *(u32 *)(work + 0x30));
        func_002D0918(*(u32 *)work);
    }
}

void func_002CBB48(void) {
    func_002CC570();
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CBB68);

void func_002CBBA0(void *p, u32 *mod, u32 *div) {
    u32 *t = *(u32 **)((s32)p + 8);
    *mod = *t % *(u32 *)((s32)p + 0x14);
    *div = *t / *(u32 *)((s32)p + 0x14);
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CBBD8);

void func_002CBC10(s32 arg0, s32 arg1, s32 arg2, u32 arg3) {
    u32 temp_v0;

    temp_v0 = arg2 * *(s32 *)(arg0 + 0x14) + arg1;
    if (temp_v0 < *(u32 *)(arg0 + 0x10)) {
        *(u32 *)(temp_v0 * 8 + *(s32 *)(arg0 + 4) + 4) = arg3;
    }
}

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CBC48);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CBC98);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CBCF0);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CBD48);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CBDA8);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CBE18);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CBF60);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CC0D0);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CC238);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CC3A8);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CC430);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CC570);

INCLUDE_ASM(const s32, "game/code_002C97E8", func_002CC5F0);

float func_002CC728(float arg0, float arg1, float arg2) {
    return arg0 + arg1 * arg2;
}

u32 func_002CC738(void) {
    return 0;
}

void func_002CC740(void) {
}

u32 func_002CC748(void) {
    return 0;
}
