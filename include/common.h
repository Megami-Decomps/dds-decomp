#ifndef COMMON_H
#define COMMON_H

/* ee-gcc: int/pointers are 32-bit, long long is 64-bit, TImode is 128-bit. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int s128 __attribute__((mode(TI)));
typedef unsigned int u128 __attribute__((mode(TI)));

/* Marks a DMA tag or packet type: the DMAC reads it at a quadword (16-byte)
 * boundary. The PS2 layouts already place these types there, so this build
 * adds nothing. A build whose layouts differ (8-byte pointers, say) defines
 * QWORD_ALIGNED as __attribute__((aligned(16))). */
#ifndef QWORD_ALIGNED
#define QWORD_ALIGNED
#endif
/* Stores a zero quadword. ee-gcc materialises the 128-bit zero in a
 * register and stores that, which is what retail does, so the operation stays
 * written as an assignment here. A build with no 128-bit integer type defines
 * this as a 16-byte clear instead. */
#ifndef QUAD_ZERO
#define QUAD_ZERO(lvalue) ((lvalue) = 0)
#endif

/* A pointer in the EE's 32-bit address representation, which is how this
 * engine passes a packet or buffer address through a u32 field or argument.
 * It lives here rather than in eeregs.h because 60 units need it and almost
 * every unit already includes this header. On target the cast is the whole
 * of it; a build with wider pointers redefines both of these and must keep
 * them inverse to each other. */
#ifndef EE_ADDR
#define EE_ADDR(ptr) ((u32)(ptr))
#endif
/* The same address back as a pointer. The (u32) is kept because the sites
 * spell it: dropping it would change the target conversion, not only the
 * host one. */
#ifndef EE_ADDR_PTR
#define EE_ADDR_PTR(addr) ((void *)(u32)(addr))
#endif
/* The same narrowing where the site spells the signed type. 422 sites do, so
 * the signedness is kept rather than folded into EE_ADDR: on the EE the two
 * differ in later arithmetic and comparisons, not in the address. */
#ifndef EE_ADDR_S
#define EE_ADDR_S(ptr) ((s32)(ptr))
#endif
/* An EE address widened back to a typed pointer. Unlike EE_ADDR_PTR there is
 * no (u32) in the expansion, because these sites spell none: the cast to the
 * pointer type is the whole of the target conversion and adding a step could
 * change it. */
#ifndef EE_ADDR_AS
#define EE_ADDR_AS(type, addr) ((type)(addr))
#endif

typedef float f32;
typedef double f64;

/* size_t, spelled here because the retail build compiles with -nostdinc and
 * has no system header to take it from. ee-gcc is invoked with
 * -D__SIZE_TYPE__=unsigned int, so it is a 32-bit unsigned here and follows
 * the compiler on a target whose own is wider. The C library declarations in
 * this tree spell their length parameter with it, the one width that differs
 * between targets with no cast at the call site to mark it.
 *
 * The right-hand side has to stay __SIZE_TYPE__ rather than a concrete type.
 * Where a host does see <stddef.h>, this typedef is then a redundant
 * identical one, which gcc and clang both accept; a concrete spelling
 * conflicts instead. This build cannot tell the two apart, because on MIPS
 * o32 unsigned int and unsigned long are both 32 bits and the image is
 * byte-identical either way. Measured on i386: unsigned long gives
 * "conflicting types for size_t" out of stddef.h under both compilers. */
typedef __SIZE_TYPE__ size_t;

/* A guest address held in an integer, which is how this engine passes a
 * packet, buffer or callback address through a field, parameter or return
 * type rather than through a pointer. EE_ADDR produces one and EE_ADDR_PTR
 * takes one back. On this target it is exactly u32, so every layout, every
 * promotion and every byte of output is what it was when these declarations
 * spelled u32; a build with wider pointers defines ADDR_TYPE to an
 * integer that holds its own pointers.
 *
 * It is deliberately not spelled on a hardware packet word. A DMA tag, VIF
 * code or GIF register field is 32 bits because the hardware reads 32 bits
 * there, and widening one would describe the packet wrongly. Those stay u32
 * and a host truncates into them on purpose. */
#ifndef ADDR_TYPE
#define ADDR_TYPE u32
#endif
typedef ADDR_TYPE addr_t;

/* The same, where the declaration spells the signed type. The signedness is
 * kept rather than folded into addr_t for the reason EE_ADDR_S exists:
 * unsigned is not a free substitution, it changes comparisons, division and
 * promotion, which on this target is codegen. */
#ifndef SADDR_TYPE
#define SADDR_TYPE s32
#endif
typedef SADDR_TYPE saddr_t;

/* Asserts a structure's retail size, by the negative-array-size trick ee-gcc
 * 2.96 accepts. The typedef emits nothing, so this costs no code. The check
 * applies only where a pointer is four bytes: a wider pointer moves every
 * member after one, so the retail size is the wrong question there and the
 * members' own types stay the record of the layout. */
#ifndef SIZE_CHECK
#define SIZE_CHECK(name, type, size) \
    typedef char name[(sizeof(void *) != 4 || sizeof(type) == (size)) ? 1 : -1]
#endif


/* Binds NAME as an assembler alias for BASE + OFFSET, for a view into
 * another object that has no symbol of its own. The assembler does the
 * arithmetic, so nothing is allocated and the reference stays gp-relative.
 * A build whose assembler cannot resolve the expression, which is every
 * target but the EE, defines this differently. */
#ifndef SYMBOL_AT
#define SYMBOL_AT(type, name, base, offset) \
    extern type name; \
    __asm__(#name " = " #base " + " #offset)
#endif

/* The same for a symbol naming a region rather than an object, so the
 * declaration is an array and a difference against another such symbol stays
 * pointer arithmetic. SYMBOL_AT cannot serve: it puts the type before the
 * name and an array's brackets go after it.
 *
 * Structured rather than a raw asm string, for a reason measured on a
 * supported compiler. LLVM's integrated assembler refuses symbol arithmetic
 * against an undefined extern, with "expected relocatable expression", as
 * soon as the symbol is referenced; GNU as defers the relocation and accepts
 * it. A site that spells the alias as a raw string gives a host no way to
 * neutralise that one alias without neutralising every raw asm string in the
 * tree, so the alias belongs behind its own name. A build whose assembler
 * cannot resolve the expression redefines this to the declaration alone and
 * supplies the symbol itself. */
#ifndef SYMBOL_AT_REGION
#define SYMBOL_AT_REGION(type, name, base, offset) \
    extern type name[]; \
    __asm__(#name " = " #base " + " #offset)
#endif

/* Localized compiler constraints on a real live value. None of them emits an
 * instruction: the binding only fixes which register cc1 keeps the value in,
 * and the empty template only forces it into a register, or into its storage,
 * at that point. docs/CONTRIBUTING.md says when these are permitted. */
/* Declares NAME while the symbol it binds to keeps its retail name, for a
 * second view of one object or a definition the linker must see under another
 * name. It emits nothing: the label only names the symbol. */
#ifndef FAKEMATCH_ASM_NAME
#define FAKEMATCH_ASM_NAME(symbol) __asm__(symbol)
#endif
#ifndef FAKEMATCH_ASM_REG
#define FAKEMATCH_ASM_REG(type, name, reg) register type name __asm__(reg)
#endif
#ifndef FAKEMATCH_ASM_LIVE
#define FAKEMATCH_ASM_LIVE(value) __asm__("" : "+r" (value))
#endif
#ifndef FAKEMATCH_ASM_LIVE_PAIR
#define FAKEMATCH_ASM_LIVE_PAIR(first, second) __asm__("" : "+r" (first), "+r" (second))
#endif
#ifndef FAKEMATCH_ASM_LIVE_F32
#define FAKEMATCH_ASM_LIVE_F32(value) __asm__("" : "+f" (value))
#endif
/* The _VOLATILE forms hold the constraint where cc1 would otherwise drop it. */
#ifndef FAKEMATCH_ASM_LIVE_VOLATILE
#define FAKEMATCH_ASM_LIVE_VOLATILE(value) __asm__ volatile ("" : "+r" (value))
#endif
#ifndef FAKEMATCH_ASM_LIVE_F32_VOLATILE
#define FAKEMATCH_ASM_LIVE_F32_VOLATILE(value) __asm__ volatile ("" : "+f" (value))
#endif
/* Input-only extended asm is implicitly volatile; it emits no instruction. */
#ifndef FAKEMATCH_ASM_READ
#define FAKEMATCH_ASM_READ(value) __asm__("" : : "r" (value))
#endif
#ifndef FAKEMATCH_ASM_READ_PAIR
#define FAKEMATCH_ASM_READ_PAIR(first, second) __asm__("" : : "r" (first), "r" (second))
#endif
#ifndef FAKEMATCH_ASM_LIVE_MEM
#define FAKEMATCH_ASM_LIVE_MEM(lvalue) __asm__ volatile ("" : "+m" (lvalue))
#endif
#ifndef FAKEMATCH_ASM_READ_MEM
#define FAKEMATCH_ASM_READ_MEM(lvalue) __asm__ volatile ("" : : "m" (lvalue))
#endif

typedef volatile u8 vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;

#ifndef NULL
#define NULL ((void *)0)
#endif

#define TRUE 1
#define FALSE 0

#define ARRAY_COUNT(arr) (s32)(sizeof(arr) / sizeof(arr[0]))

#include "include_asm.h"

#endif /* COMMON_H */
