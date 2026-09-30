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

#endif
