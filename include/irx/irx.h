#ifndef IRX_H
#define IRX_H

/* Shared declarations for the three IOP modules in USERIRX/.
 * The build is described in docs/irx-modules.md.
 *
 * A build that compiles this C for the host rather than the IOP defines
 * IRX_HOST. It gets the same typedefs, the platform's own stdarg, and the IRX
 * container macros as nothing: there is no loader to patch an import stub, no
 * import table is read, and nothing depends on .text alignment. That keeps
 * such a build from needing a parallel copy of this header, which would
 * silently drift from this one.
 */

#ifdef IRX_HOST
#include <stdarg.h>
#include <stddef.h>
#endif

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef short s16;
typedef int s32;
typedef unsigned long long u64;
typedef long long s64;

#ifndef IRX_HOST
typedef char *va_list;
#define va_start(ap, last) ((ap) = (va_list)__builtin_next_arg(last))
#define va_end(ap) ((void)0)
#endif

#ifndef NULL
#define NULL 0
#endif

#if !defined(IRX_HOST) && !defined(ASM_ROOT)
#error "ASM_ROOT must be defined by the build (see tools/irx_configure.py)"
#endif

#ifdef IRX_HOST

/* The container means nothing off the IOP; the host links the real
 * implementation under the same name instead. */
#define INCLUDE_ASM(MODULE, NAME)
#define IRX_IMPORT_TABLE(LIB, MAJOR, MINOR)
#define IRX_IMPORT_TABLE_END
#define IRX_IMPORT(LIB, NAME, ORDINAL)
#define ALIGN_TEXT(n)

#else

/* Pull a chunk of .text that is still assembly. The chunks appear in the C
 * file in link order; replacing one with a C function keeps that order. */
#define INCLUDE_ASM(MODULE, NAME)                              \
    __asm__(".section .text\n"                                 \
            "\t.set noat\n"                                    \
            "\t.set noreorder\n"                               \
            "\t.include \"" ASM_ROOT MODULE "/text/" #NAME ".s\"\n" \
            "\t.set reorder\n"                                 \
            "\t.set at\n")

/* The header of one IOP import table: the magic, the library version, and an
 * eight byte name field the loader matches against a resident library. The
 * IRX_IMPORT stubs for that library follow it, then IRX_IMPORT_TABLE_END. */
#define IRX_IMPORT_TABLE(LIB, MAJOR, MINOR)                 \
    __asm__(".section .text\n"                              \
            "\t.word 0x41e00000\n"                          \
            "\t.word 0\n"                                   \
            "\t.word (" #MAJOR " << 8) | " #MINOR "\n"      \
            ".L__irx_name_" #LIB ":\n"                      \
            "\t.ascii \"" #LIB "\"\n"                       \
            "\t.fill 8 - (. - .L__irx_name_" #LIB "), 1, 0\n")

/* The zero entry that ends an import table's stub list. */
#define IRX_IMPORT_TABLE_END                                \
    __asm__(".section .text\n"                              \
            "\t.word 0\n"                                   \
            "\t.word 0\n")

/* An IOP import stub: the loader rewrites these two instructions into a jump to
 * the resolved export, and the ordinal tells it which one. The symbol keeps the
 * library prefix, so two libraries exporting the same short name stay distinct.
 * Same shape as ps2sdk's DECLARE_IMPORT. */
#define IRX_IMPORT(LIB, NAME, ORDINAL)            \
    __asm__(".section .text\n"                    \
            "\t.set noreorder\n"                  \
            "\t.globl " #LIB "_" #NAME "\n"       \
            #LIB "_" #NAME ":\n"                   \
            "\tjr $31\n"                          \
            "\taddiu $0, $0, " #ORDINAL "\n"      \
            "\t.set reorder\n")

/* Alignment the retail linker inserted at a translation unit boundary. The
 * argument is a power of two, as MIPS gas takes it. */
#define ALIGN_TEXT(n) __asm__("\t.align " #n)

#endif /* IRX_HOST */

/* A function whose C is equivalent but does not reproduce the retail
 * instructions sits under #ifdef NON_MATCHING, with the INCLUDE_ASM the
 * default build uses in the #else. */

#endif /* IRX_H */
