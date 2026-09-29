#include "common.h"

void func_002DD038(void) {
}

INCLUDE_ASM(const s32, "game/code_002DD038", func_002DD040);

void func_002DD070(s32 arg0, s32 arg1, u32 arg2, s32 arg3) {
    func_002DB220();
    *(s32 *)(arg0 + 0xc) = *(s32 *)(*(s32 *)(*(s32 *)(arg1 + 4) + 0x10) + 0xc) + arg3 * 0x10;
}

INCLUDE_ASM(const s32, "game/code_002DD038", func_002DD0C8);

INCLUDE_ASM(const s32, "game/code_002DD038", func_002DD128);

INCLUDE_ASM(const s32, "game/code_002DD038", func_002DD1B8);

INCLUDE_ASM(const s32, "game/code_002DD038", func_002DD348);

/* Matrix registers: vf28-vf31 are the primary matrix, vf24-vf27 its
 * alternate bank, and vf20-vf23 a third bank copied between the two. */

void func_002DD378(void *matrix) {
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf28, 0(%0)\n"
        "lqc2 vf29, 16(%0)\n"
        "lqc2 vf30, 32(%0)\n"
        "lqc2 vf31, 48(%0)\n"
        ".set reorder\n"
        : : "r"(matrix) : "memory");
}

void func_002DD390(void *matrix) {
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf24, 0(%0)\n"
        "lqc2 vf25, 16(%0)\n"
        "lqc2 vf26, 32(%0)\n"
        "lqc2 vf27, 48(%0)\n"
        ".set reorder\n"
        : : "r"(matrix) : "memory");
}

void func_002DD3A8(void *matrix) {
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf28, 0(%0)\n"
        "sqc2 vf29, 16(%0)\n"
        "sqc2 vf30, 32(%0)\n"
        "sqc2 vf31, 48(%0)\n"
        ".set reorder\n"
        : : "r"(matrix) : "memory");
}

void func_002DD3C0(void *matrix) {
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf24, 0(%0)\n"
        "sqc2 vf25, 16(%0)\n"
        "sqc2 vf26, 32(%0)\n"
        "sqc2 vf27, 48(%0)\n"
        ".set reorder\n"
        : : "r"(matrix) : "memory");
}

void func_002DD3D8(void) {
    __asm__ volatile (
        ".set noreorder\n"
        "vmove.xyzw vf24, vf28\n"
        "vmove.xyzw vf25, vf29\n"
        "vmove.xyzw vf26, vf30\n"
        "vmove.xyzw vf27, vf31\n"
        ".set reorder\n");
}

void func_002DD3F0(void) {
    __asm__ volatile (
        ".set noreorder\n"
        "vmove.xyzw vf20, vf28\n"
        "vmove.xyzw vf21, vf29\n"
        "vmove.xyzw vf22, vf30\n"
        "vmove.xyzw vf23, vf31\n"
        ".set reorder\n");
}

void func_002DD408(void) {
    __asm__ volatile (
        ".set noreorder\n"
        "vmove.xyzw vf28, vf24\n"
        "vmove.xyzw vf29, vf25\n"
        "vmove.xyzw vf30, vf26\n"
        "vmove.xyzw vf31, vf27\n"
        ".set reorder\n");
}

void func_002DD420(void) {
    __asm__ volatile (
        ".set noreorder\n"
        "vmove.xyzw vf20, vf24\n"
        "vmove.xyzw vf21, vf25\n"
        "vmove.xyzw vf22, vf26\n"
        "vmove.xyzw vf23, vf27\n"
        ".set reorder\n");
}

void func_002DD438(void) {
    __asm__ volatile (
        ".set noreorder\n"
        "vmove.xyzw vf28, vf20\n"
        "vmove.xyzw vf29, vf21\n"
        "vmove.xyzw vf30, vf22\n"
        "vmove.xyzw vf31, vf23\n"
        ".set reorder\n");
}

void func_002DD450(void) {
    __asm__ volatile (
        ".set noreorder\n"
        "vmove.xyzw vf24, vf20\n"
        "vmove.xyzw vf25, vf21\n"
        "vmove.xyzw vf26, vf22\n"
        "vmove.xyzw vf27, vf23\n"
        ".set reorder\n");
}

void func_002DD468(void) {
    __asm__ volatile (
        ".set noreorder\n"
        "vsub.xyzw vf28, vf0, vf0\n"
        "vmr32.xyzw vf30, vf0\n"
        "vmove.xyzw vf31, vf0\n"
        "vaddw.x vf28, vf28, vf0w\n"
        "vmr32.xyzw vf29, vf30\n"
        ".set reorder\n");
}

void func_002DD488(void) {
    __asm__ volatile (
        ".set noreorder\n"
        "vsub.xyzw vf24, vf0, vf0\n"
        "vmr32.xyzw vf26, vf0\n"
        "vmove.xyzw vf27, vf0\n"
        "vaddw.x vf24, vf24, vf0w\n"
        "vmr32.xyzw vf25, vf26\n"
        ".set reorder\n");
}

INCLUDE_ASM(const s32, "game/code_002DD038", func_002DD4A8);

/* Transpose the four VU rows; EE MMI interleave is needed for packed vectors. */
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
