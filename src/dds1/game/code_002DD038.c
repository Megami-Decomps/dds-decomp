#include "common.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"

extern s32 (*D_003982D0[])(void *a0, s32 a1);

extern s32 (*D_00398360[])(void *a0, s32 a1);

void func_002DD038(void) {
}

s32 func_002DD040(void *object, s32 command) {
    return D_00398360[(u16)command](object, command);
}

/* Select a 16-byte entry from the source object's motion pointer table. */
void sdfSelectMotionPointerEntry(s32 destination, s32 source, u32 unused, s32 entryIndex) {
    sdfSetMotionPointerPair();
    *(s32 *)(destination + 0xc) = *(s32 *)(*(s32 *)(*(s32 *)(source + 4) + 0x10) + 0xc) + entryIndex * 0x10;
}

INCLUDE_ASM(const s32, "game/code_002DD038", func_002DD0C8);

INCLUDE_ASM(const s32, "game/code_002DD038", sdfBlendMotionKeys);

INCLUDE_ASM(const s32, "game/code_002DD038", func_002DD1B8);

typedef struct {
    u8 data[0x10];
} Block16;

typedef struct PoseCopy {
    u8 pad00[0xC];
    Block16 *src;
    Block16 dst;
} PoseCopy;

void func_002DD348(PoseCopy *pose) {
    pose->dst = *pose->src;
}

/* Matrix registers: vf28-vf31 are the primary matrix, vf24-vf27 its
 * alternate bank, and vf20-vf23 a third bank copied between the two. */

void func_002DD378(void *matrix) {
    VU0_LOAD_MATRIX(matrix);
}

void func_002DD390(void *matrix) {
    VU0_LOAD_MATRIX_B(matrix);
}

void func_002DD3A8(void *matrix) {
    VU0_STORE_MATRIX(matrix);
}

void func_002DD3C0(void *matrix) {
    VU0_STORE_MATRIX_B(matrix);
}

void func_002DD3D8(void) {
    VU0_MOVE_VF(vf24, vf28);
    VU0_MOVE_VF(vf25, vf29);
    VU0_MOVE_VF(vf26, vf30);
    VU0_MOVE_VF(vf27, vf31);
}

void func_002DD3F0(void) {
    VU0_MOVE_VF(vf20, vf28);
    VU0_MOVE_VF(vf21, vf29);
    VU0_MOVE_VF(vf22, vf30);
    VU0_MOVE_VF(vf23, vf31);
}

void func_002DD408(void) {
    VU0_MOVE_VF(vf28, vf24);
    VU0_MOVE_VF(vf29, vf25);
    VU0_MOVE_VF(vf30, vf26);
    VU0_MOVE_VF(vf31, vf27);
}

void func_002DD420(void) {
    VU0_MOVE_VF(vf20, vf24);
    VU0_MOVE_VF(vf21, vf25);
    VU0_MOVE_VF(vf22, vf26);
    VU0_MOVE_VF(vf23, vf27);
}

void func_002DD438(void) {
    VU0_MOVE_VF(vf28, vf20);
    VU0_MOVE_VF(vf29, vf21);
    VU0_MOVE_VF(vf30, vf22);
    VU0_MOVE_VF(vf31, vf23);
}

void func_002DD450(void) {
    VU0_MOVE_VF(vf24, vf20);
    VU0_MOVE_VF(vf25, vf21);
    VU0_MOVE_VF(vf26, vf22);
    VU0_MOVE_VF(vf27, vf23);
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
void func_002DD4A8(void *dst) {
    EE_MMI_UNIT_MATRIX(dst);
}

/* Transpose the four VU rows; EE MMI interleave is needed for packed vectors. */
/* libvu0: sceVu0TransposeMatrix, register form (vf28-vf31 in and out) */
void func_002DD4D8(void) {
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
void func_002DD520(void) {
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
void func_002DD578(void) {
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

INCLUDE_ASM(const s32, "game/code_002DD038", func_002DD608);

INCLUDE_ASM(const s32, "game/code_002DD038", func_002DD688);

INCLUDE_ASM(const s32, "game/code_002DD038", func_002DD708);

INCLUDE_ASM(const s32, "game/code_002DD038", func_002DD788);
