#ifndef INCLUDE_ASM_H
#define INCLUDE_ASM_H

/*
 * INCLUDE_ASM(const s32, "unit/path", func) pulls a not-yet-decompiled function
 * from the split assembly into the C translation unit, so the object keeps the
 * original function order. ee-gcc 2.96 emits top-level asm in source order (no
 * unit-at-a-time), and the same ee-as assembles the compiler output and the
 * included body.
 *
 * ee-as cannot read spimdisasm's syntax directly, so configure.py rewrites each
 * body into build/eeasm/ (tools/eeas_compat.py); ASM_ROOT is that tree for the
 * version being compiled (e.g. "build/eeasm/asm/dds1/nonmatchings/").
 *
 * M2CTX/PERMUTER (context and permuter builds) and SKIP_ASM (objdiff progress
 * objects: an asm fallback must not count as matched code) drop the asm.
 */
#if !defined(M2CTX) && !defined(PERMUTER) && !defined(SKIP_ASM)

#ifndef ASM_ROOT
#error "ASM_ROOT must be defined by the build (see configure.py)"
#endif

#define INCLUDE_ASM(TYPE, FOLDER, NAME)                        \
    __asm__(".section .text\n"                                 \
            "\t.set noat\n"                                    \
            "\t.set noreorder\n"                               \
            "\t.include \"" ASM_ROOT FOLDER "/" #NAME ".s\"\n" \
            "\t.set reorder\n"                                 \
            "\t.set at\n")

#define INCLUDE_RODATA(TYPE, FOLDER, NAME)                     \
    __asm__(".section .rodata\n"                               \
            "\t.include \"" ASM_ROOT FOLDER "/" #NAME ".s\"\n" \
            ".section .text\n")

__asm__(".include \"macro.inc\"\n");

#else

#define INCLUDE_ASM(TYPE, FOLDER, NAME)
#define INCLUDE_RODATA(TYPE, FOLDER, NAME)

#endif

#endif /* INCLUDE_ASM_H */
