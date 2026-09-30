#include "common.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"

extern s32 (*D_0040B480[])(void *a0, s32 a1);

extern s32 (*D_0040B510[])(void *a0, s32 a1);

extern u8 D_0040B518[];

extern s32 func_00328D68(s32);

typedef struct MotionKey {
    s32 id;
    f32 value;
} MotionKey;

typedef struct MotionKeySample {
    MotionKey *first;
    MotionKey *second;
    f32 weight;
} MotionKeySample;

typedef struct MotionBlend {
    u8 pad00[0xC];
    MotionKey *out;
} MotionBlend;

extern void func_00334678(void *, void *);

void func_00335EE8(void) {
}

s32 func_00335EF0(void *object, s32 command) {
    return D_0040B510[(u16)command](object, command);
}

/* Select a 16-byte entry from the source object's motion pointer table. */
void sdfSelectMotionPointerEntry(s32 destination, s32 source, void *unused, s32 entryIndex) {
    sdfSetMotionPointerPair();
    *(s32 *)(destination + 0xc) = *(s32 *)(*(s32 *)(*(s32 *)(source + 4) + 0x10) + 0xc) + entryIndex * 0x10;
}

s32 func_00335F78(s32 source, s32 unused, s32 entryIndex) {
    s32 entry = func_00328D68(0x20);

    sdfSelectMotionPointerEntry(entry, source, D_0040B518, entryIndex);
    return entry;
}

void func_00335FD8(MotionBlend *motion) {
    MotionKeySample sample;
    MotionKey *out;
    s32 firstId;
    s32 secondId;
    f32 firstValue;
    f32 secondValue;
    f32 weight;
    f32 inverse;

    func_00334678(motion, &sample);
    firstId = sample.first->id;
    secondId = sample.second->id;
    firstValue = sample.first->value;
    secondValue = sample.second->value;
    weight = sample.weight;
    inverse = 1.0f - weight;
    out = motion->out;
    if (firstId == secondId) {
        out[0].id = firstId;
        out[0].value = firstValue * inverse + secondValue * weight;
        out[1].id = secondId;
        out[1].value = 0;
    } else {
        out[0].id = firstId;
        out[0].value = firstValue * inverse;
        out[1].id = secondId;
        out[1].value = secondValue * weight;
    }
}

INCLUDE_ASM(const s32, "game/code_00335EE8", func_00336068);

typedef struct Block16 {
    u8 data[0x10];
} Block16;

typedef struct PoseCopy {
    u8 pad00[0xC];
    Block16 *src;
    Block16 dst;
} PoseCopy;

void func_003361F8(PoseCopy *pose) {
    pose->dst = *pose->src;
}

void func_00336228(void *matrix) {
    VU0_LOAD_MATRIX(matrix);
}

void func_00336240(void *matrix) {
    VU0_LOAD_MATRIX_B(matrix);
}

void func_00336258(void *dst) {
    VU0_STORE_MATRIX(dst);
}

void func_00336270(void *dst) {
    VU0_STORE_MATRIX_B(dst);
}

/* vu0 routine: vf24-vf27 = vf28-vf31 */
void func_00336288(void) {
    __asm__ volatile (
        ".set noreorder\n"
        "vmove.xyzw vf24, vf28\n"
        "vmove.xyzw vf25, vf29\n"
        "vmove.xyzw vf26, vf30\n"
        "vmove.xyzw vf27, vf31\n"
        ".set reorder\n");
}

/* vu0 routine: vf20-vf23 = vf28-vf31 */
void func_003362A0(void) {
    __asm__ volatile (
        ".set noreorder\n"
        "vmove.xyzw vf20, vf28\n"
        "vmove.xyzw vf21, vf29\n"
        "vmove.xyzw vf22, vf30\n"
        "vmove.xyzw vf23, vf31\n"
        ".set reorder\n");
}

/* vu0 routine: vf28-vf31 = vf24-vf27 */
void func_003362B8(void) {
    __asm__ volatile (
        ".set noreorder\n"
        "vmove.xyzw vf28, vf24\n"
        "vmove.xyzw vf29, vf25\n"
        "vmove.xyzw vf30, vf26\n"
        "vmove.xyzw vf31, vf27\n"
        ".set reorder\n");
}

/* vu0 routine: vf20-vf23 = vf24-vf27 */
void func_003362D0(void) {
    __asm__ volatile (
        ".set noreorder\n"
        "vmove.xyzw vf20, vf24\n"
        "vmove.xyzw vf21, vf25\n"
        "vmove.xyzw vf22, vf26\n"
        "vmove.xyzw vf23, vf27\n"
        ".set reorder\n");
}

/* vu0 routine: vf28-vf31 = vf20-vf23 */
void func_003362E8(void) {
    __asm__ volatile (
        ".set noreorder\n"
        "vmove.xyzw vf28, vf20\n"
        "vmove.xyzw vf29, vf21\n"
        "vmove.xyzw vf30, vf22\n"
        "vmove.xyzw vf31, vf23\n"
        ".set reorder\n");
}

/* vu0 routine: vf24-vf27 = vf20-vf23 */
void func_00336300(void) {
    __asm__ volatile (
        ".set noreorder\n"
        "vmove.xyzw vf24, vf20\n"
        "vmove.xyzw vf25, vf21\n"
        "vmove.xyzw vf26, vf22\n"
        "vmove.xyzw vf27, vf23\n"
        ".set reorder\n");
}

void sdfSetPrimaryIdentityMatrixVU(void) {
    __asm__ volatile (
        ".set noreorder\n"
        "vsub.xyzw vf28, vf0, vf0\n"
        "vmr32.xyzw vf30, vf0\n"
        "vmove.xyzw vf31, vf0\n"
        "vaddw.x vf28, vf28, vf0w\n"
        "vmr32.xyzw vf29, vf30\n"
        ".set reorder\n");
}

void sdfSetAlternateIdentityMatrixVU(void) {
    __asm__ volatile (
        ".set noreorder\n"
        "vsub.xyzw vf24, vf0, vf0\n"
        "vmr32.xyzw vf26, vf0\n"
        "vmove.xyzw vf27, vf0\n"
        "vaddw.x vf24, vf24, vf0w\n"
        "vmr32.xyzw vf25, vf26\n"
        ".set reorder\n");
}

/* libvu0: sceVu0UnitMatrix */
void func_00336358(void *dst) {
    EE_MMI_UNIT_MATRIX(dst);
}

/* Transpose the four VU rows; EE MMI interleave is needed for packed vectors. */
/* libvu0: sceVu0TransposeMatrix, register form (vf28-vf31 in and out) */
void func_00336388(void) {
    __asm__ volatile (
        ".set noreorder\n"
        "qmfc2.ni $8, vf28\n"
        "qmfc2.ni $9, vf29\n"
        "qmfc2.ni $10, vf30\n"
        "qmfc2.ni $11, vf31\n"
        "pextlw $12, $9, $8\n"
        "pextuw $13, $9, $8\n"
        "pextlw $14, $11, $10\n"
        "pextuw $15, $11, $10\n"
        "pcpyld $8, $14, $12\n"
        "pcpyud $9, $12, $14\n"
        "pcpyld $10, $15, $13\n"
        "pcpyud $11, $13, $15\n"
        "qmtc2.ni $8, vf28\n"
        "qmtc2.ni $9, vf29\n"
        "qmtc2.ni $10, vf30\n"
        "qmtc2.ni $11, vf31\n"
        ".set reorder\n");
}

/* vu0 routine: rigid inverse of vf28-vf31 (transpose the 3x3, translation = -(R^T * t)) */
void func_003363D0(void) {
    __asm__ volatile (
        ".set noreorder\n"
        "qmfc2.ni $8, vf28\n"
        "qmfc2.ni $9, vf29\n"
        "qmfc2.ni $10, vf30\n"
        "qmfc2.ni $11, vf0\n"
        "pextlw $12, $9, $8\n"
        "pextuw $13, $9, $8\n"
        "pextlw $14, $11, $10\n"
        "pextuw $15, $11, $10\n"
        "pcpyld $8, $14, $12\n"
        "pcpyud $9, $12, $14\n"
        "pcpyld $10, $15, $13\n"
        "vmove.w vf31, vf0\n"
        "qmtc2.ni $8, vf28\n"
        "qmtc2.ni $9, vf29\n"
        "qmtc2.ni $10, vf30\n"
        "vsuba.xyz ACC, vf0, vf0\n"
        "vmsubax.xyz ACC, vf28, vf31x\n"
        "vmsubay.xyz ACC, vf29, vf31y\n"
        "vmsubz.xyz vf31, vf30, vf31z\n"
        ".set reorder\n");
}

/* vu0 routine: inverse of vf28-vf31 with per-axis scale removed (transpose, rows / |row|^2, translation = -(R^T * t)) */
void func_00336428(void) {
    __asm__ volatile (
        ".set noreorder\n"
        "vmula.xyz ACC, vf28, vf28\n"
        "vmadda.xyz ACC, vf29, vf29\n"
        "vmadd.xyz vf2, vf30, vf30\n"
        "vaddw.xyz vf3, vf0, vf0w\n"
        "qmfc2.ni $8, vf28\n"
        "qmfc2.ni $9, vf29\n"
        "qmfc2.ni $10, vf30\n"
        "qmfc2.ni $11, vf0\n"
        "vdiv Q, vf0w, vf2x\n"
        "pextlw $12, $9, $8\n"
        "pextuw $13, $9, $8\n"
        "pextlw $14, $11, $10\n"
        "pextuw $15, $11, $10\n"
        "pcpyld $8, $14, $12\n"
        "pcpyud $9, $12, $14\n"
        "pcpyld $10, $15, $13\n"
        "vwaitq\n"
        "vmulq.x vf3, vf3, Q\n"
        "vdiv Q, vf0w, vf2y\n"
        "qmtc2.ni $8, vf28\n"
        "qmtc2.ni $9, vf29\n"
        "qmtc2.ni $10, vf30\n"
        "vwaitq\n"
        "vmulq.y vf3, vf3, Q\n"
        "vdiv Q, vf0w, vf2z\n"
        "vwaitq\n"
        "vmulq.z vf3, vf3, Q\n"
        "vmul.xyz vf28, vf28, vf3\n"
        "vmul.xyz vf29, vf29, vf3\n"
        "vmul.xyz vf30, vf30, vf3\n"
        "vmula.xyz ACC, vf0, vf0\n"
        "vmsubax.xyz ACC, vf28, vf31x\n"
        "vmsubay.xyz ACC, vf29, vf31y\n"
        "vmsubz.xyz vf31, vf30, vf31z\n"
        ".set reorder\n");
}

INCLUDE_ASM(const s32, "game/code_00335EE8", func_003364B8);

INCLUDE_ASM(const s32, "game/code_00335EE8", func_00336538);

INCLUDE_ASM(const s32, "game/code_00335EE8", func_003365B8);

INCLUDE_ASM(const s32, "game/code_00335EE8", func_00336638);
