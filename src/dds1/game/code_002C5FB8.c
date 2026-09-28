#include "common.h"
#include "fpu.h"

extern u64 func_00197760(s32, s32, u64, u64, u64, u64);

extern u32 D_003BD978;

extern s32 D_003BD97C;

extern u32 D_003BD980;

extern s32 func_002C4A10(void);

typedef struct Vec3 {
    float x; // 0x00
    float y; // 0x04
    float z; // 0x08
} Vec3; // 0x0C

extern float func_002C8588(float *, float *);
extern void func_002D0A10(void *);
typedef struct MapResource {
    u32 image;
    u32 handle;
    u32 descriptor;
    u32 unkC;
} MapResource;
extern MapResource D_00390710[10];
extern MapResource D_003906F0;
extern MapResource D_00390700;
extern u32 func_002C7D80(s32 *);
extern u32 func_002EB028(const char *, void *, s32);
extern u32 func_002D3288(u32);

extern float func_002FA1C0(float);

extern u32 D_003BD984;

extern u32 D_003DFED0[];

extern u32 D_003DFEE0[];

void func_002C5FB8(u32 arg0) {
    func_00195CD8(arg0, 1, 3);
}

s32 func_002C5FD8(s32 x, s32 n) {
    s32 i = 0;
    s32 cnt = 0;
    s32 ni;

    do {
        ni = i + 1;
        if (n == ni) {
            break;
        }
        cnt += (x >> i) & 1;
        i = ni;
    } while (i < 0x1F);
    return cnt;
}

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C6010);

void func_002C6098(s32 arg0, s32 arg1) {
    if ((arg1 <= *(s32 *)(arg0 + 0x20)) && (arg1 != 0)) {
        *(s32 *)(arg0 + 0xc) = arg1;
    }
}

void func_002C60B8(s32 arg0) {
    if (*(s32 *)(arg0 + 0xc) < 10) {
        *(s32 *)(arg0 + 0xc) = *(s32 *)(arg0 + 0xc) + 1;
    }
}

void func_002C60D8(s32 arg0) {
    if (1 < *(s32 *)(arg0 + 0xc)) {
        *(s32 *)(arg0 + 0xc) = *(s32 *)(arg0 + 0xc) - 1;
    }
}

s32 func_002C60F8(void *p) {
    void *n;
    s32 mask;

    n = *(void **)((s32)p + 0x10);
    mask = 0;
    do {
        void *m = *(void **)((s32)n + 0x70);
        n = *(void **)((s32)n + 0x58);
        mask |= 1 << (*(s16 *)((s32)m + 8) - 1);
    } while (n != NULL);
    return mask;
}

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C6130);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C62D8);

s32 func_002C6370(void) {
    s32 i = 9;
    MapResource *item = D_00390710;
    do {
        func_002C7D80((s32 *)item);
        item++;
        --i;
    } while (i >= 0);
    func_002C7D80((s32 *)&D_003906F0);
    func_002C7D80((s32 *)&D_00390700);
    return 1;
}

void func_002C63D8(void) {
    s32 temp_v0;

    D_003BD978 = 0;
    temp_v0 = func_002C4A10();
    D_003BD97C = temp_v0 - 1;
    D_003BD980 = 0x3c;
}

void func_002C6408(void) {
    if ((s32)D_003BD978 < 0x3C) {
        D_003BD978++;
    }
}

void func_002C6428(void) {
    if ((s32)D_003BD978 > 0) {
        D_003BD978 -= 2;
    } else {
        D_003BD978 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C6448);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C6948);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C6EC8);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C7000);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C7058);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C7080);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C7180);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C7430);

void func_002C7700(void) {
    D_003DFEE0[0] = D_003DFED0[0];
    D_003DFEE0[1] = D_003DFED0[1];
    D_003DFEE0[2] = D_003DFED0[2];
    D_003BD984 = 1;
}

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C7738);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C7950);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C7A60);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C7B38);

void func_002C7B58(s32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    u32 *puVar1;

    puVar1 = *(u32 **)(arg0 + 8);
    if (*(s16 *)(arg0 + 0x16) == *(s16 *)(arg0 + 0x14)) {
        if (puVar1[3] == 0) {
            *puVar1 = arg1;
            puVar1[1] = arg2;
            puVar1[2] = arg3;
            *(u32 *)(arg0 + 8) = puVar1[4];
            puVar1[3] = 1;
        }
        *(u16 *)(arg0 + 0x16) = 0;
        return;
    }
    *(s16 *)(arg0 + 0x16) = *(s16 *)(arg0 + 0x16) + 1;
}

void func_002C7BA8(s32 arg0, u16 arg1) {
    *(u16 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C7BB0);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C7C58);


s32 func_002C7D18(const char *name, MapResource *record) {
    u32 handle = func_002EB028(name, &record->descriptor, 0);
    u32 descriptor = record->descriptor;
    record->handle = handle;
    record->image = func_002D3288(descriptor);
    if (record->handle != 0) {
        func_002D0A10((void *)record->handle);
        record->handle = 0;
        record->descriptor = 0;
    }
    return 1;
}

u32 func_002C7D80(s32 *arg0) {
    if (*arg0 != 0) {
        func_002D2D00(*arg0);
        *arg0 = 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C7DC0);

void func_002C7EC8(s32 arg0, s32 arg1, u64 arg2, u64 arg3) {
    u64 temp_v0;

    temp_v0 = func_00197760(arg0 << 4, arg1 << 3, 0, arg2, arg3, 0);
    func_00195880(temp_v0, 1);
    func_00194920(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C7F18);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C7FB0);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C8040);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C80D8);

void func_002C8420(float *arg0, float *arg1) {
    *arg0 = *arg0 + *arg1;
    arg0[1] = arg0[1] + arg1[1];
    arg0[2] = arg0[2] + arg1[2];
}

void func_002C8458(float *arg0, float *arg1) {
    *arg0 = *arg0 - *arg1;
    arg0[1] = arg0[1] - arg1[1];
    arg0[2] = arg0[2] - arg1[2];
}

void func_002C8490(float arg0, float arg1, float arg2, float *arg3) {
    *arg3 = *arg3 + arg0;
    arg3[1] = arg3[1] + arg1;
    arg3[2] = arg3[2] + arg2;
}

void func_002C84B8(Vec3 *v, float x, float y, float z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

void func_002C84C8(float arg0, float *arg1) {
    *arg1 = *arg1 * arg0;
    arg1[1] = arg1[1] * arg0;
    arg1[2] = arg1[2] * arg0;
}

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C84F0);

float func_002C8558(float *vector) {
    return fsqrtf(vector[0] * vector[0] + vector[1] * vector[1] +
                  vector[2] * vector[2]);
}

float func_002C8588(float *left, float *right) {
    struct Vector4 { float x, y, z, w; } a, b;
    a = *(struct Vector4 *)left;
    b = *(struct Vector4 *)right;
    func_002C84F0(&a.x);
    func_002C84F0(&b.x);
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float func_002C8628(float *left, float *right) {
    return func_002FA1C0(func_002C8588(left, right));
}

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C8648);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C8710);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C8970);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C8BD0);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C8E30);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C8F40);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C8FE8);

float func_002C9228(float x, float y) {
    float p = 1.0f;
    s32 i = 1;

    if (y >= 1.0f) {
        do {
            i++;
            p *= x;
        } while ((float)i <= y);
    }
    return p;
}

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C9268);

INCLUDE_ASM(const s32, "game/code_002C5FB8", func_002C94A8);

void func_002C95F0(float *arg0, float *arg1, float *arg2) {
    *arg0 = *arg1 + *arg2;
    arg0[1] = arg1[1] + arg2[1];
    arg0[2] = arg1[2] + arg2[2];
    arg0[3] = arg1[3] + arg2[3];
}

void func_002C9638(float *arg0, float *arg1, float *arg2) {
    *arg0 = (arg1[3] * *arg2 + *arg1 * arg2[3] + arg1[1] * arg2[2]) -
                          arg1[2] * arg2[1];
    arg0[1] = (arg1[3] * arg2[1] + arg1[1] * arg2[3] + arg1[2] * *arg2) -
                              *arg1 * arg2[2];
    arg0[2] = (arg1[3] * arg2[2] + arg1[2] * arg2[3] + *arg1 * arg2[1]) -
                              arg1[1] * *arg2;
    arg0[3] = ((arg1[3] * arg2[3] - *arg1 * *arg2) - arg1[1] * arg2[1]) -
                              arg1[2] * arg2[2];
}

float func_002C9740(float *arg0, float *arg1) {
    return *arg0 * *arg1 + arg0[1] * arg1[1] + arg0[2] * arg1[2] +
                  arg0[3] * arg1[3];
}

float func_002C9780(float *arg0, float *arg1) {
    return (arg0[1] * arg1[2] - arg0[2] * arg1[1]) +
                  (arg0[2] * *arg1 - *arg0 * arg1[2]) +
                  (*arg0 * arg1[1] - arg0[1] * *arg1);
}

float func_002C97C8(float *left, float *right) {
    return func_002FA1C0(func_002C9740(left, right));
}







INCLUDE_RODATA(const s32, "game/code_002C5FB8", D_003B3DC0);

INCLUDE_RODATA(const s32, "game/code_002C5FB8", D_003B3E00);

INCLUDE_RODATA(const s32, "game/code_002C5FB8", D_003B3E40);


INCLUDE_SDATA(const s32, "game/code_002C5FB8", D_003BD281);

