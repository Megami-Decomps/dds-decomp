#include "common.h"
#include "pcp_vu0.h"

/* Partial view of the two offsets accessed here; the full layout is unknown. */
typedef struct {
    u8 pad00[0x2C];
    f32 scalar;   /* 0x2C */
    u8 pad30[0xC];
    u32 word3C;  /* 0x3C */
} WorkSlots;

void func_00184420(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00184430(WorkSlots *work, f32 value) {
    work->scalar = value;
}

void func_00184438(WorkSlots *work, u32 value) {
    work->word3C = value;
}

u32 func_00184440(void) {
    return 0;
}

u32 func_00184448(void) {
    return 0;
}

u32 func_00184450(void) {
    return 0;
}

void func_00184458(void) {
}

void func_00184460(void) {
}

void func_00184468(void) {
}

void func_00184470(void) {
}

u32 func_00184478(void) {
    return 0;
}

u32 func_00184480(void) {
    return 0;
}

u32 func_00184488(void) {
    return 0;
}

void func_00184490(void) {
}

void func_00184498(void) {
}

void func_001844A0(void) {
}

u32 func_001844A8(void) {
    return 0;
}

void func_001844B0(void) {
}

void func_001844B8(void) {
}

s32 func_001844C0(void) {
    return 0;
}

s32 func_001844C8(void) {
    return 0;
}

void func_001844D0(void) {
}

void func_001844D8(void) {
}

s32 func_001844E0(void) {
    return 0;
}

s32 func_001844E8(void) {
    return 0;
}

void func_001844F0(void) {
}

void func_001844F8(void) {
}

void func_00184500(void) {
}

void func_00184508(void) {
}

s32 func_00184510(void) {
    return 0x18;
}

s32 func_00184518(void) {
    return 0;
}

void func_00184520(void) {
}

void func_00184528(void) {
}

s32 func_00184530(void) {
    return 0;
}
