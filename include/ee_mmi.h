#ifndef EE_MMI_H
#define EE_MMI_H

/*
 * EE multimedia (MMI) idioms that retail code gets from Sony's libvu0/SDK
 * inline asm. gcc 2.96 never emits these instructions itself. Each macro
 * reproduces one retail pattern and says which plain-C form was tried
 * (docs/idioms.md, "Inline asm: COP2 and MMI"). `.set noreorder` keeps ee-as
 * from moving a COP2/MMI op into a delay slot, where retail has a nop.
 */

/*
 * sceVu0UnitMatrix as libvu0 expands it (scratch registers hard-coded by the
 * SDK):
 *     qmfc2.ni $5, vf0            ; (0,0,0,1)
 *     pextuw   $4, $0, $5         ; (0,0,1,0) row 2
 *     pextuw   $2, $0, $4         ; (1,0,0,0) row 0
 *     pextuw   $3, $4, $0         ; (0,1,0,0) row 1
 *     sq $2,0(dst); sq $3,0x10(dst); sq $4,0x20(dst); sq $5,0x30(dst)
 * The four `sq` are part of the idiom: retail stores the identity rows from
 * the registers the shuffle left them in. Tried: sixteen u32 stores of 0/1
 * (gcc 2.96 emits sw of $0 and a constant, never qmfc2/pextuw or sq).
 */
#define EE_MMI_UNIT_MATRIX(dst) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "qmfc2.ni $5, $vf0\n\t" \
    "pextuw $4, $0, $5\n\t" \
    "pextuw $2, $0, $4\n\t" \
    "pextuw $3, $4, $0\n\t" \
    "sq $2, 0(%0)\n\t" \
    "sq $3, 0x10(%0)\n\t" \
    "sq $4, 0x20(%0)\n\t" \
    "sq $5, 0x30(%0)\n\t" \
    ".set reorder" \
    : : "r" (dst) : "$2", "$3", "$4", "$5", "memory")

/*
 * RGBA8888 word at *src -> four floats in vf10, scaled by `unit` (a GPR
 * holding the float bit pattern, 0x3C000000 = 1/128) through vf2x:
 *     lw $2,0(src); pextlb $2,$0,$2; pextlh $2,$0,$2   ; bytes -> words
 *     qmtc2.ni $2,vf10; vitof0 vf10,vf10                ; words -> floats
 *     qmtc2.ni unit,vf2; vmulx vf10,vf10,vf2x
 * The `lw` of the operand's own storage is part of the idiom (retail spills
 * the colour argument to the stack, then reloads it here). Tried: a C
 * shift/mask unpack `(float)(c & 255)`, ... (gcc gives andi/srl per byte and
 * cvt.s.w, no pextlb/pextlh or COP2).
 */
#define EE_MMI_RGBA_UNPACK(src, unit) __asm__ volatile ( \
    ".set noreorder\n" \
    "lw $2, 0(%1)\n" \
    "pextlb $2, $0, $2\n" \
    "pextlh $2, $0, $2\n" \
    "qmtc2.ni $2, vf10\n" \
    "vitof0.xyzw vf10, vf10\n" \
    "qmtc2.ni %0, vf2\n" \
    "vmulx.xyzw vf10, vf10, vf2x\n" \
    ".set reorder" \
    : : "r"(unit), "r"(src) : "$2", "memory")

/*
 * vf10 (floats, 0..1) -> RGBA8888 word: scale by 128.0 through vf2x, then
 *     vftoi0 vf10,vf10; qmfc2.ni out,vf10; ppach out,$0,out; ppacb out,$0,out
 * (words -> halfwords -> bytes). Tried: `(u32)(int)f[0] | ... << 8 ...` in C
 * (gcc gives cvt.w.s/mfc1/sll/or per channel, no qmfc2 or ppach).
 */
#define EE_MMI_RGBA_PACK(out) __asm__ volatile ( \
    ".set noreorder\n" \
    "mfc1 $3, %1\n" \
    "qmtc2.ni $3, vf2\n" \
    "vmulx.xyzw vf10, vf10, vf2x\n" \
    "vftoi0.xyzw vf10, vf10\n" \
    "qmfc2.ni %0, vf10\n" \
    "ppach %0, $0, %0\n" \
    "ppacb %0, $0, %0\n" \
    ".set reorder" \
    : "=r"(out) : "f"(128.0f) : "$3")

/*
 * vf10 (floats) -> RGBA8888 word with the scale in a GPR:
 *     qmtc2.ni unit,vf2; vmulx.xyzw vf10,vf10,vf2x; vftoi0.xyzw vf10,vf10;
 *     qmfc2.ni out,vf10; ppach out,$0,out; ppacb out,$0,out
 * Retail keeps `unit` (0x43000000 = 128.0f) in one register for several packs.
 * out is earlyclobber: retail never shares it with unit.
 */
#define EE_MMI_RGBA_PACK_UNIT(out, unit) __asm__ volatile ( \
    ".set noreorder\n" \
    "qmtc2.ni %1, vf2\n" \
    "vmulx.xyzw vf10, vf10, vf2x\n" \
    "vftoi0.xyzw vf10, vf10\n" \
    "qmfc2.ni %0, vf10\n" \
    "ppach %0, $0, %0\n" \
    "ppacb %0, $0, %0\n" \
    ".set reorder" \
    : "=&r"(out) : "r"(unit))

/*
 * vf10 (floats) -> RGBA8888 word, scale 128.0f through `mfc1 $2` (the event
 * opcodes' pack; EE_MMI_RGBA_PACK uses `$3`):
 *     mfc1 $2,128.0f; qmtc2.ni $2,vf2; vmulx.xyzw vf10,vf10,vf2x;
 *     vftoi0.xyzw vf10,vf10; qmfc2.ni out,vf10; ppach out,$0,out; ppacb out,$0,out
 * $2 is not declared clobbered, like VU0_SCALAR_OP.
 */
#define EE_MMI_RGBA_PACK_F128(out) __asm__ volatile ( \
    ".set noreorder\n" \
    "mfc1 $2, %1\n" \
    "qmtc2.ni $2, vf2\n" \
    "vmulx.xyzw vf10, vf10, vf2x\n" \
    "vftoi0.xyzw vf10, vf10\n" \
    "qmfc2.ni %0, vf10\n" \
    "ppach %0, $0, %0\n" \
    "ppacb %0, $0, %0\n" \
    ".set reorder" \
    : "=r"(out) : "f"(128.0f))

/*
 * Unaligned three-float vector at p -> vf register (w undefined):
 *     ldr $2,0(p); ldl $2,7(p)      ; low 8 bytes
 *     lw $3,8(p)                    ; third float
 *     pcpyld $2,$3,$2; qmtc2.ni $2,vfN
 * Scratch $2/$3 are hard-coded, as in the SDK macros. Tried: a packed-struct
 * s64 load (gcc emits ldl before ldr, retail has ldr first) and u128 C
 * shifts (no pcpyld).
 */
#define EE_MMI_LOAD_VEC3(vf, p) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "ldr $2, 0(%0)\n\t" \
    "ldl $2, 7(%0)\n\t" \
    "lw $3, 8(%0)\n\t" \
    "pcpyld $2, $3, $2\n\t" \
    "qmtc2.ni $2, " #vf "\n\t" \
    ".set reorder" \
    : : "r"(p) : "$2", "$3")

/*
 * Four s16 at p (unaligned) -> vf register as floats with 12 fractional bits
 * (quaternion keys):
 *     ldr $2,0(p); ldl $2,7(p)
 *     pextlh $2,$2,$0; psraw $2,$2,16   ; halfwords -> sign-extended words
 *     qmtc2.ni $2,vfN; vitof12.xyzw vfN,vfN
 * Scratch $2 is hard-coded as in EE_MMI_LOAD_VEC3.
 */
#define EE_MMI_LOAD_S16X4_FIXED12(vf, p) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "ldr $2, 0(%0)\n\t" \
    "ldl $2, 7(%0)\n\t" \
    "pextlh $2, $2, $0\n\t" \
    "psraw $2, $2, 16\n\t" \
    "qmtc2.ni $2, " #vf "\n\t" \
    "vitof12.xyzw " #vf ", " #vf "\n\t" \
    ".set reorder" \
    : : "r"(p) : "$2")

#endif
