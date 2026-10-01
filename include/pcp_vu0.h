#ifndef PCP_VU0_H
#define PCP_VU0_H

/*
 * 128-bit vector copy in the style of libvu0's sceVu0CopyVector, which the
 * Sony SDK implements as an asm statement (`lq` into a scratch register, `sq`
 * out of it). The PCP effect setters compile to exactly `lq $2,0(src);
 * sq $2,0(dst); jr $31; nop`: plain C u128 copies give `lq; jr; sq` (the sq
 * moves into the jr delay slot). `.set noreorder` keeps ee-as from filling
 * the slot too, as for fsqrtf. The destination is operand 0 so its address is formed first.
 */
#define PCP_COPY_VECTOR(dst, src) __asm__ volatile ( \
    ".set noreorder\n\tlq $2, 0(%1)\n\tsq $2, 0(%0)\n\t.set reorder" \
    : : "r" (dst), "r" (src) : "$2", "memory")

/*
 * Fill the x, y and z components of a float vector with one value (plain C;
 * the w component is left alone). Used by the PCP particle and strip
 * builders to set up per-axis scales before the VU0 multiply.
 */
#define VEC3_SPLAT(v, x) ((v)[0] = (x), (v)[1] = (x), (v)[2] = (x))

/*
 * 64-byte (4-quadword) copy through vf28-vf31, as the field/effect matrix
 * setters do. COP2 has no plain-C form; .set noreorder keeps ee-as from
 * moving the sqc2 into a delay slot.
 */
#define VU0_COPY_MATRIX(dst, src) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "lqc2 vf28, 0(%1)\n\t" \
    "lqc2 vf29, 16(%1)\n\t" \
    "lqc2 vf30, 32(%1)\n\t" \
    "lqc2 vf31, 48(%1)\n\t" \
    "sqc2 vf28, 0(%0)\n\t" \
    "sqc2 vf29, 16(%0)\n\t" \
    "sqc2 vf30, 32(%0)\n\t" \
    "sqc2 vf31, 48(%0)\n\t" \
    ".set reorder" \
    : : "r" (dst), "r" (src) : "memory")

/* Load the same four quadwords into vf28-vf31 (no store). */
#define VU0_LOAD_MATRIX(src) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "lqc2 vf28, 0(%0)\n\t" \
    "lqc2 vf29, 16(%0)\n\t" \
    "lqc2 vf30, 32(%0)\n\t" \
    "lqc2 vf31, 48(%0)\n\t" \
    ".set reorder" \
    : : "r" (src))
/* Same load when the source asm also barred compiler memory motion; retail
 * uses this form where the matrix is written by a callee in between. */
#define VU0_LOAD_MATRIX_MEMORY(src) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "lqc2 vf28, 0(%0)\n\t" \
    "lqc2 vf29, 16(%0)\n\t" \
    "lqc2 vf30, 32(%0)\n\t" \
    "lqc2 vf31, 48(%0)\n\t" \
    ".set reorder" \
    : : "r" (src) : "memory")
/* Same store without a compiler memory clobber, for the effect-allocator sites
 * where retail keeps the matrix address CSE'd across the store. */
#define VU0_STORE_MATRIX_NOCLOBBER(dst) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "sqc2 vf28, 0(%0)\n\t" \
    "sqc2 vf29, 16(%0)\n\t" \
    "sqc2 vf30, 32(%0)\n\t" \
    "sqc2 vf31, 48(%0)\n\t" \
    ".set reorder" \
    : : "r" (dst))

/* Store vf28-vf31 to four quadwords (pairs with VU0_LOAD_MATRIX). */
#define VU0_STORE_MATRIX(dst) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "sqc2 vf28, 0(%0)\n\t" \
    "sqc2 vf29, 16(%0)\n\t" \
    "sqc2 vf30, 32(%0)\n\t" \
    "sqc2 vf31, 48(%0)\n\t" \
    ".set reorder" \
    : : "r" (dst) : "memory")
/* Same store for existing asm without a compiler memory clobber (retail keeps
 * the destination address CSE'd across it). */
#define VU0_STORE_MATRIX_UNCLOBBERED(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf28, 0(%0)\n\tsqc2 vf29, 16(%0)\n\tsqc2 vf30, 32(%0)\n\t" \
    "sqc2 vf31, 48(%0)\n\t.set reorder" \
    : : "r" (dst))

/* Load four quadwords into the second matrix bank vf24-vf27 (the right-hand
 * operand of the sdf matrix-multiply routines). */
#define VU0_LOAD_MATRIX_B(src) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "lqc2 vf24, 0(%0)\n\t" \
    "lqc2 vf25, 16(%0)\n\t" \
    "lqc2 vf26, 32(%0)\n\t" \
    "lqc2 vf27, 48(%0)\n\t" \
    ".set reorder" \
    : : "r" (src) : "memory")

/* Store the second matrix bank vf24-vf27 to four quadwords. */
#define VU0_STORE_MATRIX_B(dst) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "sqc2 vf24, 0(%0)\n\t" \
    "sqc2 vf25, 16(%0)\n\t" \
    "sqc2 vf26, 32(%0)\n\t" \
    "sqc2 vf27, 48(%0)\n\t" \
    ".set reorder" \
    : : "r" (dst) : "memory")

/* One quadword between memory and a named VU0 register. The load has no
 * memory clobber; the store does, matching the standalone COP2 asm idiom. */
#define VU0_LOAD_VF(vf, src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 " #vf ", 0(%0)\n\t.set reorder" \
    : : "r" (src))
/* Same load when the source asm also barred compiler memory motion. */
#define VU0_LOAD_VF_MEMORY(vf, src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 " #vf ", 0(%0)\n\t.set reorder" \
    : : "r" (src) : "memory")
#define VU0_STORE_VF(vf, dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 " #vf ", 0(%0)\n\t.set reorder" \
    : : "r" (dst) : "memory")
/* Store variant for existing asm without a compiler memory clobber. */
#define VU0_STORE_VF_UNCLOBBERED(vf, dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 " #vf ", 0(%0)\n\t.set reorder" \
    : : "r" (dst))
/* vf = (1, 1, 1, 0) from vf0 = (0, 0, 0, 1): the unit scale vector of an
 * object's transform (stored next to the zero rotation/translation and the
 * unit matrix). */
#define VU0_SET_ONES_XYZ(vf) __asm__ volatile ( \
    ".set noreorder\n\tvaddw.xyz " #vf ", vf0, vf0w\n\tvmulx.w " #vf ", vf0, vf0x\n\t.set reorder" \
    : : : "memory")
/* Broadcast a float into vf2 through the SDK scratch $2 and apply one VU op
 * that reads vf2x, e.g. VU0_SCALAR_OP(t, "vmulx.xyzw vf10, vf10, vf2x").
 * $2 is not declared clobbered, as in the Sony samples (docs/idioms.md). */
#define VU0_SCALAR_OP(f, insn) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\t" insn "\n\t.set reorder" \
    : : "f" (f))
/* The same block with $2 declared clobbered: gcc then keeps live values out of
 * $2 around it (DDS2 func_00206C18 addresses D_003BE0A0 through $3). */
#define VU0_SCALAR_OP_CLOBBER(f, insn) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\t" insn "\n\t.set reorder" \
    : : "f" (f) : "$2")
/* vf10.<axis> = f and vf10.w = 0 in one block: a direction on one axis for the
 * unit's path/aim vector (retail keeps the w clear right after the vaddx). */
#define VU0_SET_AXIS_CLEAR_W(f, axis) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 $2, %0\n\tqmtc2.ni $2, vf2\n\tvaddx." #axis " vf10, vf0, vf2x\n\t" \
    "vmulx.w vf10, vf10, vf0x\n\t.set reorder" \
    : : "f" (f))
/* vf *= f through a gcc-chosen GPR (qmtc2.ni reg,vf2; vmulx.xyzw vf,vf,vf2x):
 * the scale bits stay in a saved register across a loop, unlike VU0_SCALAR_OP. */
#define VU0_SCALE_VF(vf, f) __asm__ volatile ( \
    ".set noreorder\n\tqmtc2.ni %0, vf2\n\tvmulx.xyzw " #vf ", " #vf ", vf2x\n\t.set reorder" \
    : : "r" (f))
/* vf10 = vf10 * (1 - t) + vf11 * t (vf0.w is 1.0): the two-vector lerp of the
 * particle, motion and battle tween code (45 retail sites). */
#define VU0_LERP_VF10(t) __asm__ volatile ( \
    ".set noreorder\n\tqmtc2.ni %0, vf2\n\tvsubx.w vf3, vf0, vf2x\n\t" \
    "vmulax.xyzw ACC, vf11, vf2x\n\tvmaddw.xyzw vf10, vf10, vf3w\n\t.set reorder" \
    : : "r" (t))
/* Register-to-register vector copy between calls (vmove.xyzw). */
#define VU0_MOVE_VF(dst, src) __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw " #dst ", " #src "\n\t.set reorder")
/* dst = primary matrix (vf28-vf31) * src: the core of libvu0
 * sceVu0ApplyMatrix, used with the sdf matrix-bank convention. */
#define VU0_APPLY_MATRIX(dst, src) __asm__ volatile ( \
    ".set noreorder\n\tvmulax.xyzw ACC, vf28, " #src "x\n\tvmadday.xyzw ACC, vf29, " #src "y\n\t" \
    "vmaddaz.xyzw ACC, vf30, " #src "z\n\tvmaddw.xyzw " #dst ", vf31, " #src "w\n\t.set reorder")
/* dst = 3x3 of the primary matrix (vf28-vf30) times src.xyz: a rotation, no
 * translation (the vmaddz form of the apply above). */
#define VU0_ROTATE_VEC(dst, src) __asm__ volatile ( \
    ".set noreorder\n\tvmulax.xyzw ACC, vf28, " #src "x\n\tvmadday.xyzw ACC, vf29, " #src "y\n\t" \
    "vmaddz.xyzw " #dst ", vf30, " #src "z\n\t.set reorder")
/* dst = primary matrix times the point src (w taken as 1: vmaddw with vf0w). */
#define VU0_TRANSFORM_POINT(dst, src) __asm__ volatile ( \
    ".set noreorder\n\tvmulax.xyzw ACC, vf28, " #src "x\n\tvmadday.xyzw ACC, vf29, " #src "y\n\t" \
    "vmaddaz.xyzw ACC, vf30, " #src "z\n\tvmaddw.xyzw " #dst ", vf31, vf0w\n\t.set reorder")
/* vf10 /= vf10.w, then w = 1: the perspective divide after a point transform. */
#define VU0_PERSPECTIVE_DIVIDE_VF10() __asm__ volatile ( \
    ".set noreorder\n\tvdiv Q, vf0w, vf10w\n\tvmove.w vf10, vf0\n\tvwaitq\n\t" \
    "vmulq.xyzw vf10, vf10, Q\n\t.set reorder")
/* Scale the rows of the primary matrix by src.xyz (vf28 *= x, vf29 *= y, vf30 *= z). */
#define VU0_SCALE_MATRIX_ROWS(src) __asm__ volatile ( \
    ".set noreorder\n\tvmulx.xyzw vf28, vf28, " #src "x\n\tvmuly.xyzw vf29, vf29, " #src "y\n\t" \
    "vmulz.xyzw vf30, vf30, " #src "z\n\t.set reorder")
/* dst = a - b, dst = a + b, dst = a * b on all four components
 * (vsub/vadd/vmul.xyzw between calls, e.g. the difference of two positions). */
#define VU0_SUB(dst, a, b) __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyzw " #dst ", " #a ", " #b "\n\t.set reorder")
#define VU0_ADD(dst, a, b) __asm__ volatile ( \
    ".set noreorder\n\tvadd.xyzw " #dst ", " #a ", " #b "\n\t.set reorder")
#define VU0_MUL(dst, a, b) __asm__ volatile ( \
    ".set noreorder\n\tvmul.xyzw " #dst ", " #a ", " #b "\n\t.set reorder")
/* vf.xyz = -vf.xyz, w kept (vsub.xyz vf,vf0,vf; vf0.xyz is 0): the reversed
 * direction after a matrix apply (95 retail sites, battle and effects). */
#define VU0_NEGATE_XYZ(vf) __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyz " #vf ", vf0, " #vf "\n\t.set reorder")
/* vf.w = 0 (vmulx.w vf,vf,vf0x) and vf.w = 1 (vmove.w vf,vf0): the w fix-up
 * retail does before packing a colour or storing a point/direction. */
#define VU0_CLEAR_W(vf) __asm__ volatile ( \
    ".set noreorder\n\tvmulx.w " #vf ", " #vf ", vf0x\n\t.set reorder")
#define VU0_SET_W_ONE(vf) __asm__ volatile ( \
    ".set noreorder\n\tvmove.w " #vf ", vf0\n\t.set reorder")
/* out = |vf10.xyz| as a C float:
 *   vmul.xyz vf2,vf10,vf10; vaddy.x vf2,vf2,vf2y; vaddz.x vf2,vf2,vf2z;
 *   vsqrt Q,vf2x; vwaitq; cfc2.ni $2,vi22; mtc1 $2,out
 * $2 is declared clobbered, as in the matched blocks this replaces. */
#define VU0_LENGTH_VF10(out) __asm__ volatile ( \
    ".set noreorder\n\tvmul.xyz vf2, vf10, vf10\n\tvaddy.x vf2, vf2, vf2y\n\t" \
    "vaddz.x vf2, vf2, vf2z\n\tvsqrt Q, vf2x\n\tvwaitq\n\tcfc2.ni $2, $vi22\n\t" \
    "mtc1 $2, %0\n\t.set reorder" \
    : "=f" (out) : : "$2")
/* vf10 = vf10 / |vf10.xyz| (w untouched):
 *   vmul.xyz vf2,vf10,vf10; vmulax.w ACC,vf0,vf2x; vmadday.w ACC,vf0,vf2y;
 *   vmaddz.w vf2,vf0,vf2z; vrsqrt Q,vf0w,vf2w; vwaitq; vmulq.xyz vf10,vf10,Q */
#define VU0_NORMALIZE_VF10() __asm__ volatile ( \
    ".set noreorder\n\tvmul.xyz vf2, vf10, vf10\n\tvmulax.w ACC, vf0, vf2x\n\t" \
    "vmadday.w ACC, vf0, vf2y\n\tvmaddz.w vf2, vf0, vf2z\n\tvrsqrt Q, vf0w, vf2w\n\t" \
    "vwaitq\n\tvmulq.xyz vf10, vf10, Q\n\t.set reorder")
/* dst = a x b (xyz cross product): vopmula.xyz ACC,a,b; vopmsub.xyz dst,b,a */
#define VU0_CROSS_XYZ(dst, a, b) __asm__ volatile ( \
    ".set noreorder\n\tvopmula.xyz ACC, " #a ", " #b "\n\tvopmsub.xyz " #dst ", " #b ", " #a "\n\t.set reorder")
/* out = a . b (xyz dot product) as a C float:
 *   vmul.xyz vf2,a,b; vaddy.x vf2,vf2,vf2y; vaddz.x vf2,vf2,vf2z;
 *   qmfc2.ni $2,vf2; mtc1 $2,out
 * $2 is declared clobbered, like VU0_LENGTH_VF10. */
#define VU0_DOT_XYZ(out, a, b) __asm__ volatile ( \
    ".set noreorder\n\tvmul.xyz vf2, " #a ", " #b "\n\tvaddy.x vf2, vf2, vf2y\n\t" \
    "vaddz.x vf2, vf2, vf2z\n\tqmfc2.ni $2, vf2\n\tmtc1 $2, %0\n\t.set reorder" \
    : "=f" (out) : : "$2")


/* ---- shared idioms for the remaining hand-written COP2 blocks ----
 * Each macro is one retail pattern; two consecutive calls assemble to the same
 * text as one fused block (every macro brackets itself with .set noreorder /
 * reorder, which the assembler reduces to nothing between two of them). */

/* vf2.x source for the vmulx/vmaddx family: qmtc2.ni of a GPR that already
 * holds the float bits (retail loads 1.0f or a saved scale into a register
 * first); VU0_MUL_VF2X is the vmulx that consumes it. VU0_SCALE_VF is the two
 * fused. */
#define VU0_SET_VF2X(r) __asm__ volatile ( \
    ".set noreorder\n\tqmtc2.ni %0, vf2\n\t.set reorder" \
    : : "r" (r))
/* dst = src * vf2.x (all four components). */
#define VU0_MUL_VF2X(dst, src) __asm__ volatile ( \
    ".set noreorder\n\tvmulx.xyzw " #dst ", " #src ", vf2x\n\t.set reorder")
/* The same broadcast through the SDK scratch $3 (mfc1 $3; qmtc2.ni $3,vf2;
 * insn), for blocks that run while $2 holds a live value. */
#define VU0_SCALAR_OP_R3(f, insn) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 $3, %0\n\tqmtc2.ni $3, vf2\n\t" insn "\n\t.set reorder" \
    : : "f" (f))
#define VU0_SCALAR_OP_R3_CLOBBER(f, insn) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 $3, %0\n\tqmtc2.ni $3, vf2\n\t" insn "\n\t.set reorder" \
    : : "f" (f) : "$3")
/* The broadcast through a gcc-chosen early-clobber GPR (mfc1 tmp,f; qmtc2.ni
 * tmp,vf2; insn), as the effect model scalers write it. */
#define VU0_SCALAR_OP_TMP(tmp, f, insn) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 %0, %1\n\tqmtc2.ni %0, vf2\n\t" insn "\n\t.set reorder" \
    : "=&r" (tmp) : "f" (f))
#define VU0_SCALAR_OP_TMP_MEMORY(tmp, f, insn) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 %0, %1\n\tqmtc2.ni %0, vf2\n\t" insn "\n\t.set reorder" \
    : "=&r" (tmp) : "f" (f) : "memory")
/* Two scalars broadcast at once: vf2.x = f2, vf3.x = f3 (the mfc1 pair comes
 * first, then the qmtc2 pair), followed by VU0_WEIGHTED_SUM_VF2X_VF3X or the
 * rotation steps of the sdf angle routines. */
#define VU0_SET_SCALARS_VF2_VF3(f2, f3) __asm__ volatile ( \
    ".set noreorder\n\tmfc1 $2, %0\n\tmfc1 $3, %1\n\tqmtc2.ni $2, vf2\n\tqmtc2.ni $3, vf3\n\t.set reorder" \
    : : "f" (f2), "f" (f3))
/* dst = a * vf2.x + b * vf3.x (vmulax ACC; vmaddx): the nlerp mix of two
 * quaternions weighted by remaining/amount. */
#define VU0_WEIGHTED_SUM_VF2X_VF3X(dst, a, b) __asm__ volatile ( \
    ".set noreorder\n\tvmulax.xyzw ACC, " #a ", vf2x\n\tvmaddx.xyzw " #dst ", " #b ", vf3x\n\t.set reorder")
/* vf10.<axis> = r (a GPR holding the float) with the plain, non-.ni qmtc2 the
 * script-command setters use: qmtc2 r,vf02; vaddx.axis vf10,vf00,vf02x. */
#define VU0_SET_AXIS_GPR(axis, r) __asm__ volatile ( \
    ".set noreorder\n\tqmtc2 %0, vf2\n\tvaddx." #axis " vf10, vf0, vf2x\n\t.set reorder" \
    : : "r" (r))
/* Identity matrix in four VU registers built from vf0 = (0,0,0,1): row order
 * a,b,c,d; (vf28,vf29,vf30,vf31) is the primary bank, (vf24..vf27) the second.
 *   vsub a,vf0,vf0; vmr32 c,vf0; vmove d,vf0; vaddw.x a,a,vf0w; vmr32 b,c */
#define VU0_SET_UNIT_MATRIX(a, b, c, d) __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyzw " #a ", vf0, vf0\n\tvmr32.xyzw " #c ", vf0\n\t" \
    "vmove.xyzw " #d ", vf0\n\tvaddw.x " #a ", " #a ", vf0w\n\tvmr32.xyzw " #b ", " #c "\n\t.set reorder")
/* vf24-vf27 = vf28-vf31: the primary matrix becomes the second bank before the
 * point projection overwrites it. */
#define VU0_MOVE_MATRIX_TO_B() __asm__ volatile ( \
    ".set noreorder\n\tvmove.xyzw vf24, vf28\n\tvmove.xyzw vf25, vf29\n\t" \
    "vmove.xyzw vf26, vf30\n\tvmove.xyzw vf27, vf31\n\t.set reorder")
/* A quadword between a VU0 register and a C lvalue (the "m" operand form,
 * `lqc2 vf,%0` / `sqc2 vf,%0`, without the register-offset spelling). */
#define VU0_LOAD_VF_FROM(vf, lvalue) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 " #vf ", %0\n\t.set reorder" \
    : : "m" (lvalue))
#define VU0_STORE_VF_TO(vf, lvalue) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 " #vf ", %0\n\t.set reorder" \
    : "=m" (lvalue))
#define VU0_STORE_VF_TO_MEMORY(vf, lvalue) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 " #vf ", %0\n\t.set reorder" \
    : "=m" (lvalue) : : "memory")
/* Quadword at src + off (a byte offset); sdf node structs keep several vectors
 * in one record. */
#define VU0_LOAD_VF_AT(vf, off, src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 " #vf ", " #off "(%0)\n\t.set reorder" \
    : : "r" (src))
#define VU0_STORE_VF_AT_UNCLOBBERED(vf, off, dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 " #vf ", " #off "(%0)\n\t.set reorder" \
    : : "r" (dst))
/* dst = a * b on xyz only (w kept): the scale of the three matrix rows by the
 * object scale vector. */
#define VU0_MUL_XYZ(dst, a, b) __asm__ volatile ( \
    ".set noreorder\n\tvmul.xyz " #dst ", " #a ", " #b "\n\t.set reorder")
/* dst = -src on all four components (ACC = vf0 * vf0x = 0, then 0 - src*1):
 * the negated quaternion for the shorter-arc nlerp. */
#define VU0_NEGATE_VF(dst, src) __asm__ volatile ( \
    ".set noreorder\n\tvmulax.xyzw ACC, vf0, vf0x\n\tvmsubw.xyzw " #dst ", " #src ", vf0w\n\t.set reorder")
/* out = a . b over all four components as a C float (quaternion dot): vf1 =
 * (1,1,1,x); vmul.xyzw vf2,a,b; vadday.x/vmaddaz.x/vmaddw.x fold y, z, w into
 * x; qmfc2.ni $2,vf2; mtc1 $2,out. Unlike VU0_DOT_XYZ it declares no clobber. */
#define VU0_DOT_XYZW(out, a, b) __asm__ volatile ( \
    ".set noreorder\n\tvaddw.xyz vf1, vf0, vf0w\n\tvmul.xyzw vf2, " #a ", " #b "\n\t" \
    "vadday.x ACC, vf2, vf2y\n\tvmaddaz.x ACC, vf1, vf2z\n\tvmaddw.x vf2, vf1, vf2w\n\t" \
    "qmfc2.ni $2, vf2\n\tmtc1 $2, %0\n\t.set reorder" \
    : "=f" (out))
/* out = |vf|^2 in the retail quaternion form (vmulay.x in place of vadday.x). */
#define VU0_LENGTH_SQ_XYZW(out, vf) __asm__ volatile ( \
    ".set noreorder\n\tvaddw.xyz vf1, vf0, vf0w\n\tvmul.xyzw vf2, " #vf ", " #vf "\n\t" \
    "vmulay.x ACC, vf2, vf2y\n\tvmaddaz.x ACC, vf1, vf2z\n\tvmaddw.x vf2, vf1, vf2w\n\t" \
    "qmfc2.ni $2, vf2\n\tmtc1 $2, %0\n\t.set reorder" \
    : "=f" (out))
/* vf10 /= |vf10| over xyzw (quaternion normalise):
 *   vmul.xyzw vf2,vf10,vf10; vaddax.w; vmadday.w; vmaddz.w vf3; vrsqrt Q,vf0w,vf3w;
 *   vwaitq; vmulq.xyzw vf10,vf10,Q  (retail leaves the component letters off
 *   the vf2 operands of the first two ACC steps) */
#define VU0_NORMALIZE_XYZW_VF10() __asm__ volatile ( \
    ".set noreorder\n\tvmul.xyzw vf2, vf10, vf10\n\tvaddax.w ACC, vf2, vf2\n\t" \
    "vmadday.w ACC, vf0, vf2\n\tvmaddz.w vf3, vf0, vf2\n\tvrsqrt Q, vf0w, vf3w\n\t" \
    "vwaitq\n\tvmulq.xyzw vf10, vf10, Q\n\t.set reorder")

/* One component of vf10 read back into a C float through the SDK scratch $2
 * (declared clobbered, like VU0_DOT_XYZ). Retail stores a result vector to
 * four separate floats this way (evtPolygonMovie vector blend):
 *   x: qmfc2.ni $2,vf10; mtc1 $2,out
 *   y: qmfc2.ni $2,vf10; prot3w $2,$2; mtc1 $2,out   (prot3w rotates y into word 0)
 *   z: qmfc2.ni $2,vf10; pexew $2,$2; mtc1 $2,out    (pexew swaps words 0 and 2)
 *   w: vaddw.x vf2,vf0,vf10w; qmfc2.ni $2,vf2; mtc1 $2,out */
#define VU0_GET_VF10_X(out) __asm__ volatile ( \
    ".set noreorder\n\tqmfc2.ni $2, vf10\n\tmtc1 $2, %0\n\t.set reorder" \
    : "=f" (out) : : "$2")
#define VU0_GET_VF10_Y(out) __asm__ volatile ( \
    ".set noreorder\n\tqmfc2.ni $2, vf10\n\tprot3w $2, $2\n\tmtc1 $2, %0\n\t.set reorder" \
    : "=f" (out) : : "$2")
#define VU0_GET_VF10_Z(out) __asm__ volatile ( \
    ".set noreorder\n\tqmfc2.ni $2, vf10\n\tpexew $2, $2\n\tmtc1 $2, %0\n\t.set reorder" \
    : "=f" (out) : : "$2")
#define VU0_GET_VF10_W(out) __asm__ volatile ( \
    ".set noreorder\n\tvaddw.x vf2, vf0, vf10w\n\tqmfc2.ni $2, vf2\n\tmtc1 $2, %0\n\t.set reorder" \
    : "=f" (out) : : "$2")
/* One component of vf10 set from a C float: the broadcast through vf2x of
 * VU0_SCALAR_OP_CLOBBER followed by vaddx.<axis> vf10,vf0,vf2x (x, y, z) or,
 * for w, vmulx.w vf10,vf0,vf2x (vf0.w is 1, so w = f):
 *   mfc1 $2,f; qmtc2.ni $2,vf2; vaddx.y vf10,vf0,vf2x
 * $2 is declared clobbered so a table address kept across the four sets lives
 * in $3 (retail's default-vector arm). */
#define VU0_SET_VF10_COMPONENT(axis, f) \
    VU0_SCALAR_OP_CLOBBER(f, "vaddx." #axis " vf10, vf0, vf2x")
#define VU0_SET_VF10_W(f) \
    VU0_SCALAR_OP_CLOBBER(f, "vmulx.w vf10, vf0, vf2x")

#endif
