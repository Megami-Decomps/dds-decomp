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

#endif
