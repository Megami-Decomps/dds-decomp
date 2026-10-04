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
/* The same pack without the "$3" clobber, as the effect draw templates wrote
 * it (DDS2 func_002E6CB8 keeps a mask constant in $3 across the block). */
#define EE_MMI_RGBA_PACK_UNCLOBBERED(out) __asm__ volatile ( \
    ".set noreorder\n" \
    "mfc1 $3, %1\n" \
    "qmtc2.ni $3, vf2\n" \
    "vmulx.xyzw vf10, vf10, vf2x\n" \
    "vftoi0.xyzw vf10, vf10\n" \
    "qmfc2.ni %0, vf10\n" \
    "ppach %0, $0, %0\n" \
    "ppacb %0, $0, %0\n" \
    ".set reorder" \
    : "=r"(out) : "f"(128.0f))

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
 * Two unaligned three-float vectors at a and b -> VU registers vf_a/vf_b:
 *     ldr/ldl/lw a into $2/$3; ldr/ldl/lw b into $6/$7
 *     pcpyld $2,$3,$2; pcpyld $6,$7,$6
 *     qmtc2.ni $2,vf_a; qmtc2.ni $6,vf_b
 * Plain C vec3 loads do not emit this MMI packing sequence; the paired block
 * keeps both shuffles and transfers after the six retail loads. Scratch
 * registers and the loads' memory effects are declared explicitly.
 */
#define EE_MMI_LOAD_VEC3_PAIR(vf_a, a, vf_b, b) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "ldr $2, 0(%0)\n\t" \
    "ldl $2, 7(%0)\n\t" \
    "lw $3, 8(%0)\n\t" \
    "ldr $6, 0(%1)\n\t" \
    "ldl $6, 7(%1)\n\t" \
    "lw $7, 8(%1)\n\t" \
    "pcpyld $2, $3, $2\n\t" \
    "pcpyld $6, $7, $6\n\t" \
    "qmtc2.ni $2, " #vf_a "\n\t" \
    "qmtc2.ni $6, " #vf_b "\n\t" \
    ".set reorder" \
    : : "r"(a), "r"(b) : "$2", "$3", "$6", "$7", "memory")

/* Adjacent vec3 pair at base and base+12. Keeping one address operand lets
 * the assembler encode the second vector directly at 0xC/0x13/0x14 instead
 * of making gcc materialize base+12 in another GPR. */
#define EE_MMI_LOAD_VEC3_PAIR_12(vf_a, vf_b, base) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "ldr $2, 0(%0)\n\t" \
    "ldl $2, 7(%0)\n\t" \
    "lw $3, 8(%0)\n\t" \
    "ldr $6, 0xC(%0)\n\t" \
    "ldl $6, 0x13(%0)\n\t" \
    "lw $7, 0x14(%0)\n\t" \
    "pcpyld $2, $3, $2\n\t" \
    "pcpyld $6, $7, $6\n\t" \
    "qmtc2.ni $2, " #vf_a "\n\t" \
    "qmtc2.ni $6, " #vf_b "\n\t" \
    ".set reorder" \
    : : "r"(base) : "$2", "$3", "$6", "$7", "memory")

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

/*
 * Blend two RGBA8888 words: out = a + (b - a) * t per channel, rounded
 * (bias 0.5 through vf3x into ACC):
 *     qmtc2.ni half,vf3; mfc1 $8,t; pextlb/pextlh a->$4, b->$5 (bytes -> words)
 *     vf5w = 1 - t; ACC = 0.5 + a*vf5w; vf5 = ACC + b*t; vftoi0
 *     qmfc2.ni $4,vf4; ppach $5,$0,$4; ppacb out,$0,$5
 * `half` is 0.5f; scratch $4/$5/$8 are hard-coded, and the block declares
 * $4-$8 clobbered (retail never places an operand in $6/$7). The
 * `vsubx.w vf5` is issued twice in retail. Users: sdfMotion colour-key lerp
 * and its five weighted-binding callers, both games.
 */
#define EE_MMI_RGBA_LERP(out, a, b, t, half) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "qmtc2.ni %4, vf3\n\t" \
    "mfc1 $8, %3\n\t" \
    "pextlb $4, $0, %1\n\t" \
    "pextlb $5, $0, %2\n\t" \
    "vsub.xyzw vf2, vf0, vf0\n\t" \
    "qmtc2.ni $8, vf4\n\t" \
    "pextlh $4, $0, $4\n\t" \
    "pextlh $5, $0, $5\n\t" \
    "vaddax.xyzw ACC, vf2, vf3x\n\t" \
    "vsubx.w vf5, vf0, vf4x\n\t" \
    "qmtc2.ni $4, vf2\n\t" \
    "qmtc2.ni $5, vf3\n\t" \
    "vsubx.w vf5, vf0, vf4x\n\t" \
    "vitof0.xyzw vf2, vf2\n\t" \
    "vitof0.xyzw vf3, vf3\n\t" \
    "vmaddaw.xyzw ACC, vf2, vf5w\n\t" \
    "vmaddx.xyzw vf5, vf3, vf4x\n\t" \
    "vftoi0.xyzw vf4, vf5\n\t" \
    "qmfc2.ni $4, vf4\n\t" \
    "ppach $5, $0, $4\n\t" \
    "ppacb %0, $0, $5\n\t" \
    ".set reorder" \
    : "=r"(out) : "r"(a), "r"(b), "f"(t), "r"(half) : "$4", "$5", "$6", "$7", "$8")

/*
 * Pack an aligned four-word vector into an unaligned three-word stream:
 * lq $3,0(src); pcpyud $2,$3,$0; sdr $3,0(dst); sdl $3,7(dst); sw $2,8(dst).
 * Retail: DDS1 func_0015FE20/func_002E21A0 and DDS2
 * func_00167A10/func_0033B050, twice per builder (positions and normals).
 * Tried u128 C extraction: cc1 rejects >>64 as an unsupported wide operation;
 * union half/word extraction spills the quadword rather than emitting pcpyud.
 * Stores to the macro's own destination are part of the packing idiom.
 */
#define EE_MMI_STORE_VEC3_FROM_QUAD(dst, src) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "lq $3, 0(%1)\n\t" \
    "pcpyud $2, $3, $0\n\t" \
    "sdr $3, 0(%0)\n\t" \
    "sdl $3, 7(%0)\n\t" \
    "sw $2, 8(%0)\n\t" \
    ".set reorder" \
    : : "r"(dst), "r"(src) : "$2", "$3", "memory")

/*
 * Pack a loaded u128 value's first three words into an unaligned stream.
 * The aligned quadword load is ordinary C, not part of this store primitive.
 * Retail: DDS1 func_002E27D8/func_002E3390 and DDS2
 * func_0033B688/func_0033C240. Their short loops have one compiler padding
 * nop; the pointer-taking primitive above has two.
 * cc1 cannot extract the upper half with a C >>64 operation.
 */
#define EE_MMI_STORE_VEC3_VALUE(dst, value) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "pcpyud $2, %1, $0\n\t" \
    "sdr %1, 0(%0)\n\t" \
    "sdl %1, 7(%0)\n\t" \
    "sw $2, 8(%0)\n\t" \
    ".set reorder" \
    : : "r"(dst), "r"(value) : "$2", "memory")

/*
 * vf10 (floats) -> RGBA8888 word, scale 255.0f through `mfc1 $2` (the draw
 * colour setters; EE_MMI_RGBA_PACK_F128 is the 128.0f twin):
 *     mfc1 $2,255.0f; qmtc2.ni $2,vf2; vmulx.xyzw vf10,vf10,vf2x;
 *     vftoi0.xyzw vf10,vf10; qmfc2.ni out,vf10; ppach out,$0,out; ppacb out,$0,out
 * $2 is not declared clobbered, like EE_MMI_RGBA_PACK_F128. Users: DDS1
 * func_001082D8 and DDS2 func_001081F8.
 */
#define EE_MMI_RGBA_PACK_F255(out) __asm__ volatile ( \
    ".set noreorder\n" \
    "mfc1 $2, %1\n" \
    "qmtc2.ni $2, vf2\n" \
    "vmulx.xyzw vf10, vf10, vf2x\n" \
    "vftoi0.xyzw vf10, vf10\n" \
    "qmfc2.ni %0, vf10\n" \
    "ppach %0, $0, %0\n" \
    "ppacb %0, $0, %0\n" \
    ".set reorder" \
    : "=r"(out) : "f"(255.0f))

/*
 * Gather one column of four 16-byte matrix rows into a VU register.
 * Retail: four branches each in DDS1 func_0021E450/func_0023A7A0 and DDS2
 * func_00238FC0/func_00255650 repeat the fixed $2-$5 SDK scratch sequence:
 * lw $2/$3/$4/$5 at 0/0x10/0x20/0x30(src); pextlw $2,$3,$2;
 * pextlw $4,$5,$4; pcpyld $2,$4,$2; qmtc2.ni $2,vf.
 * Plain C float gathering was compiled first: four lwc1/swc1 pairs, a
 * 16-byte stack temporary, then lqc2, rather than this MMI packing.
 */
#define EE_MMI_LOAD_MATRIX_COLUMN(vf, src) __asm__ volatile ( \
    ".set noreorder\n\tlw $2, 0(%0)\n\tlw $3, 0x10(%0)\n\t" \
    "lw $4, 0x20(%0)\n\tlw $5, 0x30(%0)\n\t" \
    "pextlw $2, $3, $2\n\tpextlw $4, $5, $4\n\t" \
    "pcpyld $2, $4, $2\n\tqmtc2.ni $2, " #vf "\n\t.set reorder" \
    : : "r" (src) : "$2", "$3", "$4", "$5", "memory")

/*
 * Count the leading sign bits (minus one) of a word: plzcw out,in. Retail:
 * DDS1 func_002CFEB8 / DDS2 func_00328D68 (power-of-two size class of an
 * allocation: class = 27 - (plzcw(size - 1) & 0xFF) for sizes above 16).
 * C has no equivalent (cc1 has no clz builtin), so the instruction is
 * written directly; `out` may be the same variable as `in`.
 */
#define EE_MMI_PLZCW(out, in) __asm__ volatile ("plzcw %0, %1" : "=r"(out) : "r"(in))

/*
 * Bare `sync` (sync.l): orders the preceding stores before the next access.
 * Retail: the DMA channel kick in DDS1 func_002E4228 / DDS2 func_0033D0D8
 * (`*chcr |= 0x40; sync;` before sceDmaSendN), as in Sony's libkernel
 * inline usage.
 */
#define EE_SYNC() __asm__ volatile ("sync")

/* Expand packed RGB5 channels into byte lanes (PEXT5), before replicating
 * their high bits into the low three bits. DDS1 func_002D32B8 and DDS2
 * func_0032C168 share this texture-intensity conversion instruction.
 * Registers are selected by gcc; the output may reuse the input register. */
#define EE_MMI_PEXT5(out, in) __asm__ volatile ("pext5 %0, %1" : "=r" (out) : "r" (in))

#endif
