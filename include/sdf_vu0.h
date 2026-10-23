#ifndef SDF_VU0_H
#define SDF_VU0_H

#include "common.h"

/* VU0 and MMI operations under the names SMT3HD keeps for them: the
 * transpiler could not inline the engine's macros, so its metadata has
 * each as a function (SDF_VU0__NORMALIZE) and smt3-decomp's
 * include/ps2asm.h adopted those names. One macro per concrete operation,
 * every register literal in the body, which is the form the engine used.
 * Bodies here are byte-identical to the register-parameterised macros in
 * pcp_vu0.h they replaced. */

#define EE_MMI_RGBA_TO_VF8(rgba) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "pextlb $2, $0, %0\n\t" \
    "pextlh $2, $0, $2\n\t" \
    "qmtc2.ni $2, vf8\n\t" \
    "vitof0.xyzw vf8, vf8\n\t" \
    ".set reorder" \
    : : "r" (rgba) : "$2")

#define EE_MMI_RGBA_TO_VF9(rgba) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "pextlb $2, $0, %0\n\t" \
    "pextlh $2, $0, $2\n\t" \
    "qmtc2.ni $2, vf9\n\t" \
    "vitof0.xyzw vf9, vf9\n\t" \
    ".set reorder" \
    : : "r" (rgba) : "$2")

/* An sq offset is a literal, so the unrolled clear needs one name per step. */
#define PS2_STORE_ZERO_QUAD_AT16(dst) __asm__ volatile ( \
    "sq $0, 0x10(%0)" : : "r" (dst) : "memory")

#define PS2_STORE_ZERO_QUAD_AT32(dst) __asm__ volatile ( \
    "sq $0, 0x20(%0)" : : "r" (dst) : "memory")

#define PS2_STORE_ZERO_QUAD_AT48(dst) __asm__ volatile ( \
    "sq $0, 0x30(%0)" : : "r" (dst) : "memory")

#define SDF_VU0_ADDVECTOR() __asm__ volatile ( \
    ".set noreorder\n\tvadd.xyzw vf10, vf10, vf11\n\t.set reorder")

/* Additional working-register operations used by the trail interpolator. */
#define SDF_VU0_ADDVECTOR_A_C() __asm__ volatile ( \
    ".set noreorder\n\tvadd.xyzw vf10, vf10, vf12\n\t.set reorder")

/* The extended form, as the trail builder spells it. */
#define SDF_VU0_ADDVECTOR_B_A() __asm__ volatile ( \
    ".set noreorder\n\tvadd.xyzw vf11, vf11, vf10\n\t.set reorder" : :)

#define SDF_VU0_ADDVECTOR_B_C() __asm__ volatile ( \
    ".set noreorder\n\tvadd.xyzw vf11, vf11, vf12\n\t.set reorder")

#define SDF_VU0_ADDVECTOR_C_A() __asm__ volatile ( \
    ".set noreorder\n\tvadd.xyzw vf12, vf12, vf10\n\t.set reorder")

/* vf10 = matrix (vf28-vf31) * vf10: the core of libvu0 sceVu0ApplyMatrix.
 * _POINT takes the source w as 1; _ROTATE uses the 3x3 part only. */
#define SDF_VU0_APPLYMATRIX() __asm__ volatile ( \
    ".set noreorder\n\tvmulax.xyzw ACC, vf28, vf10x\n\tvmadday.xyzw ACC, vf29, vf10y\n\t" \
    "vmaddaz.xyzw ACC, vf30, vf10z\n\tvmaddw.xyzw vf10, vf31, vf10w\n\t.set reorder")

/* Ring geometry matrix and working-register operations. */

#define SDF_VU0_APPLYMATRIX_B() __asm__ volatile ( \
    ".set noreorder\n\tvmulax.xyzw ACC, vf28, vf11x\n\tvmadday.xyzw ACC, vf29, vf11y\n\t" \
    "vmaddaz.xyzw ACC, vf30, vf11z\n\tvmaddw.xyzw vf11, vf31, vf11w\n\t.set reorder")

#define SDF_VU0_APPLYMATRIX_XYZ() __asm__ volatile ( \
    ".set noreorder\n\tvmulax.xyzw ACC, vf28, vf10x\n\tvmadday.xyzw ACC, vf29, vf10y\n\t" \
    "vmaddz.xyzw vf10, vf30, vf10z\n\t.set reorder")

#define SDF_VU0_APPLYMATRIX_XYZ1() __asm__ volatile ( \
    ".set noreorder\n\tvmulax.xyzw ACC, vf28, vf10x\n\tvmadday.xyzw ACC, vf29, vf10y\n\t" \
    "vmaddaz.xyzw ACC, vf30, vf10z\n\tvmaddw.xyzw vf10, vf31, vf0w\n\t.set reorder")

/* Accumulate the xyz clip tests of a transformed vector against its own w,
 * then read or reset the accumulated flags in vi18. */
#define SDF_VU0_CLIPTEST() __asm__ volatile ( \
    ".set noreorder\n\tvclipw.xyz vf10, vf10w\n\t.set reorder")

#define SDF_VU0_CLIPTEST_B() __asm__ volatile ( \
    ".set noreorder\n\tvclipw.xyz vf11, vf11w\n\t.set reorder")

/* ---- VU0 macro-mode arithmetic on the fixed working pair ---- */

#define SDF_VU0_COPYVECTOR_A_TO_B() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf11, vf10\n\t.set reorder")

/* The extended form: an empty operand list, not a basic asm with no colons.
 * cc1 2.96 schedules the two differently, and mdlSetBlur only matches with
 * this spelling. */
#define SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf11, vf10\n\t.set reorder" : :)

/* vf12 is the spare the effect code parks a vector in across a call. */
#define SDF_VU0_COPYVECTOR_A_TO_C() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf12, vf10\n\t.set reorder")

#define SDF_VU0_COPYVECTOR_B_TO_A() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf10, vf11\n\t.set reorder")

#define SDF_VU0_COPYVECTOR_B_TO_C() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf12, vf11\n\t.set reorder")

#define SDF_VU0_COPYVECTOR_C_TO_A() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf10, vf12\n\t.set reorder")

#define SDF_VU0_COPYVECTOR_C_TO_B() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf11, vf12\n\t.set reorder")

#define SDF_VU0_FTOI0() __asm__ volatile ( \
    ".set noreorder\n\tvftoi0.xyzw vf10, vf10\n\t.set reorder")

#define SDF_VU0_FTOI4() __asm__ volatile ( \
    ".set noreorder\n\tvftoi4.xyzw vf10, vf10\n\t.set reorder")

#define SDF_VU0_GET_ROW2() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf10, vf30\n\t.set reorder")

#define SDF_VU0_GET_ROW3() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf10, vf31\n\t.set reorder")

/* out = vf10 . vf11 over xyz as a C float; $2 is scratch. */
#define SDF_VU0_INNERPRODUCT(out) __asm__ volatile ( \
    ".set noreorder\n\tvmul.xyz vf2, vf10, vf11\n\tvaddy.x vf2, vf2, vf2y\n\t" \
    "vaddz.x vf2, vf2, vf2z\n\tqmfc2.ni $2, vf2\n\tmtc1 $2, %0\n\t.set reorder" \
    : "=f" (out) : : "$2")

/* An identity matrix in the bank: vmr32 rotates the one in vf0.w into
 * place for each row. */
#define SDF_VU0_LOADIDENTITY() __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyzw vf28, vf0, vf0\n\tvmr32.xyzw vf30, vf0\n\t" \
    "vmove.xyzw vf31, vf0\n\tvaddw.x vf28, vf28, vf0w\n\tvmr32.xyzw vf29, vf30\n\t.set reorder")

/* vf10 = vf0, so xyz go to zero and w to one. */
#define SDF_VU0_LOADIDENTITYVECTOR() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf10, vf0\n\t.set reorder")

#define SDF_VU0_LOADIDENTITY_B() __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyzw vf24, vf0, vf0\n\tvmr32.xyzw vf26, vf0\n\t" \
    "vmove.xyzw vf27, vf0\n\tvaddw.x vf24, vf24, vf0w\n\tvmr32.xyzw vf25, vf26\n\t.set reorder")

#define SDF_VU0_LOADSHORTVECTOR(p) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "ldr $2, 0(%0)\n\t" \
    "ldl $2, 7(%0)\n\t" \
    "pextlh $2, $2, $0\n\t" \
    "psraw $2, $2, 16\n\t" \
    "qmtc2.ni $2, " "vf10" "\n\t" \
    "vitof12.xyzw " "vf10" ", " "vf10" "\n\t" \
    ".set reorder" \
    : : "r"(p) : "$2")

#define SDF_VU0_LOADSHORTVECTOR_B(p) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "ldr $2, 0(%0)\n\t" \
    "ldl $2, 7(%0)\n\t" \
    "pextlh $2, $2, $0\n\t" \
    "psraw $2, $2, 16\n\t" \
    "qmtc2.ni $2, " "vf11" "\n\t" \
    "vitof12.xyzw " "vf11" ", " "vf11" "\n\t" \
    ".set reorder" \
    : : "r"(p) : "$2")

/* The _TO/_FROM forms take a C lvalue and let cc1 pick the addressing mode;
 * the plain forms take an address already in a register. */
#define SDF_VU0_LOADVECTOR(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r" (src))

#define SDF_VU0_LOADVECTOR_B(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf11, 0(%0)\n\t.set reorder" : : "r" (src))

#define SDF_VU0_LOADVECTOR_BARRIER(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" \
    : : "r" (src) : "memory")

/* Cell gradient vector operations. */
#define SDF_VU0_LOADVECTOR_B_BARRIER(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf11, 0(%0)\n\t.set reorder" \
    : : "r" (src) : "memory")

#define SDF_VU0_LOADVECTOR_B_MEMOPS(lvalue) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf11, %0\n\t.set reorder" : : "m" (lvalue))

#define SDF_VU0_LOADVECTOR_C(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf12, 0(%0)\n\t.set reorder" \
    : : "r" (src))

#define SDF_VU0_LOADVECTOR_MEMOPS(lvalue) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf10, %0\n\t.set reorder" : : "m" (lvalue))

/* ---- fixed-register forms, one name per register pair ---- */

#define SDF_VU0_LOADVECTOR_MTXCOL(src) __asm__ volatile ( \
    ".set noreorder\n\tlw $2, 0(%0)\n\tlw $3, 0x10(%0)\n\t" \
    "lw $4, 0x20(%0)\n\tlw $5, 0x30(%0)\n\t" \
    "pextlw $2, $3, $2\n\tpextlw $4, $5, $4\n\t" \
    "pcpyld $2, $4, $2\n\tqmtc2.ni $2, vf10\n\t.set reorder" \
    : : "r" (src) : "$2", "$3", "$4", "$5", "memory")

#define SDF_VU0_LOADVECTOR_UNALIGN_B(p) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "ldr $2, 0(%0)\n\t" \
    "ldl $2, 7(%0)\n\t" \
    "lw $3, 8(%0)\n\t" \
    "pcpyld $2, $3, $2\n\t" \
    "qmtc2.ni $2, " "vf11" "\n\t" \
    ".set reorder" \
    : : "r"(p) : "$2", "$3")

#define SDF_VU0_LOAD_ROW0_BARRIER(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf28, 0(%0)\n\t.set reorder" \
    : : "r" (src) : "memory")

#define SDF_VU0_LOAD_ROW1_BARRIER(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf29, 0(%0)\n\t.set reorder" \
    : : "r" (src) : "memory")

#define SDF_VU0_LOAD_ROW2_BARRIER(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf30, 0(%0)\n\t.set reorder" \
    : : "r" (src) : "memory")

#define SDF_VU0_LOAD_ROW3(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf31, 0(%0)\n\t.set reorder" \
    : : "r" (src))

#define SDF_VU0_LOAD_ROW3_BARRIER(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf31, 0(%0)\n\t.set reorder" \
    : : "r" (src) : "memory")

#define SDF_VU0_MATRIX_ROW3_W_SET_ONE() __asm__ volatile ( \
    ".set noreorder\n\tvmove.w vf31, vf0\n\t.set reorder")

#define SDF_VU0_MULVECTOR() __asm__ volatile ( \
    ".set noreorder\n\tvmul.xyzw vf10, vf10, vf11\n\t.set reorder")

/* vf10.xyz = -vf10.xyz (vf0.xyz is zero), vf10.w = 0, vf10.w = 1. */
#define SDF_VU0_NEGVECTOR_XYZ() __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyz vf10, vf0, vf10\n\t.set reorder")

/* vf10 = vf10 x vf11 over xyz: the outer-product pair. */
#define SDF_VU0_OUTERPRODUCT() __asm__ volatile ( \
    ".set noreorder\n\tvopmula.xyz ACC, vf10, vf11\n\t" \
    "vopmsub.xyz vf10, vf11, vf10\n\t.set reorder")

#define SDF_VU0_RESET_ROW3() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf31, vf0\n\t.set reorder")

#define SDF_VU0_SCALE_XYZ() __asm__ volatile ( \
    ".set noreorder\n\tvmulx.xyzw vf28, vf28, vf10x\n\tvmuly.xyzw vf29, vf29, vf10y\n\t" \
    "vmulz.xyzw vf30, vf30, vf10z\n\t.set reorder")

#define SDF_VU0_SETSCALE1VECTOR() __asm__ volatile ( \
    ".set noreorder\n\tvaddw.xyz vf10, vf0, vf0w\n\t" \
    "vmulx.w vf10, vf0, vf0x\n\t.set reorder" : : : "memory")

#define SDF_VU0_SET_ROW0() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf28, vf10\n\t.set reorder")

#define SDF_VU0_SET_ROW1() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf29, vf10\n\t.set reorder")

#define SDF_VU0_SET_ROW2() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf30, vf10\n\t.set reorder")

/* ---- operations that were written out at a call site ---- */

#define SDF_VU0_SET_ROW3() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf31, vf10\n\t.set reorder")

#define SDF_VU0_STOREVECTOR(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r" (dst) : "memory")

#define SDF_VU0_STOREVECTOR_B(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf11, 0(%0)\n\t.set reorder" : : "r" (dst) : "memory")

#define SDF_VU0_STOREVECTOR_B_NOMEM(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf11, 0(%0)\n\t.set reorder" \
    : : "r" (dst))

/* from effect/parManager.c */
#define SDF_VU0_STOREVECTOR_C(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 " "vf12" ", 0(%0)\n\t.set reorder" \
    : : "r" (dst) : "memory")

#define SDF_VU0_STOREVECTOR_MEMOPS(lvalue) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf10, %0\n\t.set reorder" : "=m" (lvalue))

/* SDK input-address-only store: audited VU pipelines only. Unlike the
 * normal store it does not invalidate values cached from unrelated memory. */
#define SDF_VU0_STOREVECTOR_NOMEM(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" \
    : : "r" (dst))

#define SDF_VU0_STORE_ROW0(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf28, 0(%0)\n\t.set reorder" : : "r" (dst) : "memory")

#define SDF_VU0_STORE_ROW0_NOMEM(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 " "vf28" ", 0(%0)\n\t.set reorder" \
    : : "r" (dst))

#define SDF_VU0_STORE_ROW1(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf29, 0(%0)\n\t.set reorder" : : "r" (dst) : "memory")

#define SDF_VU0_STORE_ROW1_NOMEM(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 " "vf29" ", 0(%0)\n\t.set reorder" \
    : : "r" (dst))

#define SDF_VU0_STORE_ROW2(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf30, 0(%0)\n\t.set reorder" : : "r" (dst) : "memory")

#define SDF_VU0_STORE_ROW2_NOMEM(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 " "vf30" ", 0(%0)\n\t.set reorder" \
    : : "r" (dst))

#define SDF_VU0_SUBVECTOR() __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyzw vf10, vf10, vf11\n\t.set reorder")

/* from code_001DEA58.c */
#define SDF_VU0_SUBVECTOR_A_C() __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyzw vf10, vf10, vf12\n\t.set reorder")

#define SDF_VU0_SUBVECTOR_B_A() __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyzw vf11, vf11, vf10\n\t.set reorder")

#define SDF_VU0_SUBVECTOR_B_C() __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyzw vf11, vf11, vf12\n\t.set reorder")

#define SDF_VU0_SUBVECTOR_EXTENDED() __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyzw vf10, vf10, vf11\n\t.set reorder" : :)

/* from basic/dds3EffectObjectBasic.c */
#define SDF_VU0_TRANSLATE() __asm__ volatile ( \
    ".set noreorder\n\tvmulax.xyzw ACC, vf28, vf10x\n\t" \
    "vmadday.xyzw ACC, vf29, vf10y\n\tvmaddaz.xyzw ACC, vf30, vf10z\n\t" \
    "vmaddw.xyzw vf31, vf31, vf10w\n\t.set reorder")

#define SDF_VU0_VECTOR_W_SET_ONE() __asm__ volatile ( \
    ".set noreorder\n\tvmove.w vf10, vf0\n\t.set reorder")

#define SDF_VU0_VECTOR_W_SET_ZERO() __asm__ volatile ( \
    ".set noreorder\n\tvmulx.w vf10, vf10, vf0x\n\t.set reorder")

#define VU0_ADD_VF31_VF10_XYZ() __asm__ volatile ( \
    ".set noreorder\n\tvadd.xyz vf31, vf31, vf10\n\t.set reorder")

#define VU0_CLIP_RATIO_X() __asm__ volatile ( \
    ".set noreorder\n\tvnop\n\tvnop\n\tvdiv Q, vf3x, vf4x\n\t.set reorder")

#define VU0_CLIP_RATIO_Y() __asm__ volatile ( \
    ".set noreorder\n\tvnop\n\tvnop\n\tvdiv Q, vf3y, vf4y\n\t.set reorder")

#define VU0_CLIP_RATIO_Z() __asm__ volatile ( \
    ".set noreorder\n\tvnop\n\tvnop\n\tvdiv Q, vf3z, vf4z\n\t.set reorder")

#define VU0_CLIP_SEGMENT_PREP_FIRST() __asm__ volatile ( \
    ".set noreorder\n\t" \
    "vmulw.xyzw vf3, vf2, vf10w\n\t" \
    "vmulw.xyzw vf4, vf2, vf11w\n\t" \
    "vsub.xyzw vf5, vf11, vf10\n\t" \
    "vsub.xyzw vf6, vf9, vf8\n\t" \
    "vsub.xyzw vf3, vf10, vf3\n\t" \
    "vsub.xyzw vf4, vf11, vf4\n\t" \
    "vsub.xyzw vf4, vf4, vf3\n\t.set reorder")

#define VU0_CLIP_SEGMENT_PREP_SECOND() __asm__ volatile ( \
    ".set noreorder\n\t" \
    "vmulw.xyzw vf3, vf2, vf11w\n\t" \
    "vmulw.xyzw vf4, vf2, vf10w\n\t" \
    "vsub.xyzw vf5, vf10, vf11\n\t" \
    "vsub.xyzw vf6, vf8, vf9\n\t" \
    "vsub.xyzw vf3, vf11, vf3\n\t" \
    "vsub.xyzw vf4, vf10, vf4\n\t" \
    "vsub.xyzw vf4, vf4, vf3\n\t.set reorder")

#define VU0_CLIP_SEGMENT_UPDATE_FIRST() __asm__ volatile ( \
    ".set noreorder\n\tvwaitq\n\tvaddq.x vf2, vf0, Q\n\tvabs.x vf2, vf2\n\t" \
    "vmulaw.xyzw ACC, vf10, vf0w\n\t" \
    "vmaddx.xyzw vf10, vf5, vf2x\n\t" \
    "vmulaw.xyzw ACC, vf8, vf0w\n\t" \
    "vmaddx.xyzw vf8, vf6, vf2x\n\t" \
    "vclipw.xyz vf10, vf10w\n\t.set reorder")

#define VU0_CLIP_SEGMENT_UPDATE_SECOND() __asm__ volatile ( \
    ".set noreorder\n\tvwaitq\n\tvaddq.x vf2, vf0, Q\n\tvabs.x vf2, vf2\n\t" \
    "vmulaw.xyzw ACC, vf11, vf0w\n\t" \
    "vmaddx.xyzw vf11, vf5, vf2x\n\t" \
    "vmulaw.xyzw ACC, vf9, vf0w\n\t" \
    "vmaddx.xyzw vf9, vf6, vf2x\n\t" \
    "vclipw.xyz vf11, vf11w\n\t.set reorder")

/* vf12 = -vf11, taking the long way: the accumulator is zeroed by vf0.x and
 * vf11 is then subtracted from it. */
#define VU0_NEGATE_VF11_TO_VF12() __asm__ volatile ( \
    ".set noreorder\n" \
    "\tvmulax.xyzw ACC, vf0, vf0x\n" \
    "\tvmsubw.xyzw vf12, vf11, vf0w\n" \
    "\t.set reorder")

#define VU0_NEGATIVE_CLIP_PLANE_VF2() __asm__ volatile ( \
    ".set noreorder\n\tvsubw.xyzw vf2, vf0, vf0w\n\t" \
    "vsubw.w vf2, vf0, vf0w\n\t.set reorder")

#define VU0_POSITIVE_CLIP_PLANE_VF2() __asm__ volatile ( \
    ".set noreorder\n\tvaddw.xyz vf2, vf0, vf0w\n\t" \
    "vmulw.w vf2, vf0, vf0w\n\t.set reorder")

/* vf10 . vf11 over all four lanes, read back as a C float. vf1.xyz is set to
 * one first because vf0.x is a hardwired zero and cannot carry the y and z
 * terms. */
#define VU0_QUAT_DOT_VF10_VF11(out) __asm__ volatile ( \
    ".set noreorder\n" \
    "\tvaddw.xyz vf1, vf0, vf0w\n" \
    "\tvmul.xyzw vf2, vf10, vf11\n" \
    "\tvadday.x ACC, vf2, vf2y\n" \
    "\tvmaddaz.x ACC, vf1, vf2z\n" \
    "\tvmaddw.x vf2, vf1, vf2w\n" \
    "\tqmfc2.ni $2, vf2\n" \
    "\tmtc1 $2, %0\n" \
    "\t.set reorder" \
    : "=f" (out))

/* From src/smt3/code_002BD340.c. */
#define VU0_SEQ_EULER_ANGLE_AXIS_Y(rotatedZ, rotatedX) __asm__ volatile ( \
    ".set noreorder\n" \
    "\tqmfc2.ni $2, vf10\n" \
    "\tpexew $2, $2\n" \
    "\tmtc1 $2, %0\n" \
    "\tqmfc2.ni $2, vf10\n" \
    "\tmtc1 $2, %1\n" \
    "\t.set reorder" \
    : "=f" (rotatedZ), "=f" (rotatedX) : : "$2", "memory")

/* From src/smt3/code_002BD340.c. */
#define VU0_SEQ_SDF_2_C0510(componentMetric) __asm__ volatile ( \
    ".set noreorder\n" \
    "\tvaddw.xyz vf1, vf0, vf0w\n" \
    "\tvmul.xyzw vf2, vf10, vf10\n" \
    "\tvmulay.x ACC, vf2, vf2y\n" \
    "\tvmaddaz.x ACC, vf1, vf2z\n" \
    "\tvmaddw.x vf2, vf1, vf2w\n" \
    "\tqmfc2.ni $2, vf2\n" \
    "\tmtc1 $2, %0\n" \
    "\t.set reorder" \
    : "=f" (componentMetric))

#define VU0_SET_VF10_Y_CLEAR_W(f) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\tvaddx.y vf10, vf0, vf2x\n\t" \
    "vmulx.w vf10, vf10, vf0x\n\t.set reorder" \
    : : "f" (f))

#define VU0_TRANSFORM_POINT_VF11() __asm__ volatile ( \
    ".set noreorder\n\tvmulax.xyzw ACC, vf28, vf11x\n\tvmadday.xyzw ACC, vf29, vf11y\n\t" \
    "vmaddaz.xyzw ACC, vf30, vf11z\n\tvmaddw.xyzw vf11, vf31, vf0w\n\t.set reorder")

/* vf10 = vf10 * vf2.x + vf12 * vf3.x. */
#define VU0_WEIGHTED_SUM_VF10_VF12() __asm__ volatile ( \
    ".set noreorder\n" \
    "\tvmulax.xyzw ACC, vf10, vf2x\n" \
    "\tvmaddx.xyzw vf10, vf12, vf3x\n" \
    "\t.set reorder")

#endif
