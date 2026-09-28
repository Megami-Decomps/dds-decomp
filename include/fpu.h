#ifndef FPU_H
#define FPU_H

/*
 * Square root in one FPU instruction.
 *
 * Retail computes every square root with a bare `sqrt.s`: 33 of them in 11
 * DDS1 units, and never the errno-checking sqrtf() call that gcc emits by
 * default. With -fno-math-errno, ee-gcc 2.96's own sqrt.s comes after two
 * hazard `nop`s, which retail never has. So the games used an asm helper like
 * this one. `.set noreorder` keeps ee-as from moving it into a jr delay slot
 * (retail: `sqrt.s; jr $31; nop`).
 */
static inline float fsqrtf(float x) {
    float r;
    __asm__(".set noreorder\n\tsqrt.s %0, %1\n\t.set reorder" : "=f"(r) : "f"(x));
    return r;
}

#endif
