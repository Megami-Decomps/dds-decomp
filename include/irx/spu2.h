#ifndef IRX_SPU2_H
#define IRX_SPU2_H

/* SPU2 and IOP DMA registers, as LIBSD writes them.
 *
 * The addresses are not guesses: LIBSD carries a table of all 41 of them in
 * .rodata (sceSdRegAddrTbl, which the entry word indexes), and these agree
 * with it entry for entry.
 *
 * SPU2_FOO(core) is the register, an lvalue with the volatile cast built in.
 * SPU2_FOO_HI(core) is its upper halfword, which is how the 24 bit masks and
 * the address pairs are written. SPU2_FOO_A(core) is the bare address.
 *
 * Which one to use is not a matter of taste. Where the retail code holds an
 * address in a register and loads at offset zero, the C has to say so, with
 * the _A form in a pointer or in arithmetic; inlining the address there lets
 * gcc fold the low half into the load offset and the function stops matching.
 * A pointer to a register is &SPU2_FOO(core). The volatile is load-bearing
 * for the same reason, which is why it lives in the macro.
 *
 * Two core strides: a core's 0x000..0x3FF block repeats every 0x400, the
 * volume coefficients at 0x760 repeat every 40 bytes, and the entry word's
 * bit 7 selects between them. The shift and the multiply are spelled the way
 * the retail code computes them.
 */

/* One base each, so a host build can aim these at a register file of its own
 * by defining just these two ahead of this header. */
#ifndef SPU2_BASE
#define SPU2_BASE           0xBF900000
#endif
#ifndef IOP_HW_BASE
#define IOP_HW_BASE         0xBF800000
#endif
#define SPU2_CORE_STRIDE    0x400
#define SPU2_COEF_STRIDE    40

#define SPU2_VOICE_A(core, v) (SPU2_BASE + ((core) << 10) + (v) * 16)

/* The per core block, stride 0x400. */
#define SPU2_PMON_A(core)  (SPU2_BASE + 0x180 + ((core) << 10))
#define SPU2_PMON(core)     (*(volatile u16 *)SPU2_PMON_A(core))
#define SPU2_PMON_HI(core)  (*(volatile u16 *)(SPU2_PMON_A(core) + 2))
#define SPU2_NON_A(core)  (SPU2_BASE + 0x184 + ((core) << 10))
#define SPU2_NON(core)     (*(volatile u16 *)SPU2_NON_A(core))
#define SPU2_NON_HI(core)  (*(volatile u16 *)(SPU2_NON_A(core) + 2))
#define SPU2_VMIXL_A(core)  (SPU2_BASE + 0x188 + ((core) << 10))
#define SPU2_VMIXL(core)     (*(volatile u16 *)SPU2_VMIXL_A(core))
#define SPU2_VMIXL_HI(core)  (*(volatile u16 *)(SPU2_VMIXL_A(core) + 2))
#define SPU2_VMIXEL_A(core)  (SPU2_BASE + 0x18C + ((core) << 10))
#define SPU2_VMIXEL(core)     (*(volatile u16 *)SPU2_VMIXEL_A(core))
#define SPU2_VMIXEL_HI(core)  (*(volatile u16 *)(SPU2_VMIXEL_A(core) + 2))
#define SPU2_VMIXR_A(core)  (SPU2_BASE + 0x190 + ((core) << 10))
#define SPU2_VMIXR(core)     (*(volatile u16 *)SPU2_VMIXR_A(core))
#define SPU2_VMIXR_HI(core)  (*(volatile u16 *)(SPU2_VMIXR_A(core) + 2))
#define SPU2_VMIXER_A(core)  (SPU2_BASE + 0x194 + ((core) << 10))
#define SPU2_VMIXER(core)     (*(volatile u16 *)SPU2_VMIXER_A(core))
#define SPU2_VMIXER_HI(core)  (*(volatile u16 *)(SPU2_VMIXER_A(core) + 2))
#define SPU2_MMIX_A(core)  (SPU2_BASE + 0x198 + ((core) << 10))
#define SPU2_MMIX(core)     (*(volatile u16 *)SPU2_MMIX_A(core))
#define SPU2_MMIX_HI(core)  (*(volatile u16 *)(SPU2_MMIX_A(core) + 2))
#define SPU2_ATTR_A(core)  (SPU2_BASE + 0x19A + ((core) << 10))
#define SPU2_ATTR(core)     (*(volatile u16 *)SPU2_ATTR_A(core))
#define SPU2_ATTR_HI(core)  (*(volatile u16 *)(SPU2_ATTR_A(core) + 2))
#define SPU2_IRQA_A(core)  (SPU2_BASE + 0x19C + ((core) << 10))
#define SPU2_IRQA(core)     (*(volatile u16 *)SPU2_IRQA_A(core))
#define SPU2_IRQA_HI(core)  (*(volatile u16 *)(SPU2_IRQA_A(core) + 2))
#define SPU2_KON_A(core)  (SPU2_BASE + 0x1A0 + ((core) << 10))
#define SPU2_KON(core)     (*(volatile u16 *)SPU2_KON_A(core))
#define SPU2_KON_HI(core)  (*(volatile u16 *)(SPU2_KON_A(core) + 2))
#define SPU2_KOFF_A(core)  (SPU2_BASE + 0x1A4 + ((core) << 10))
#define SPU2_KOFF(core)     (*(volatile u16 *)SPU2_KOFF_A(core))
#define SPU2_KOFF_HI(core)  (*(volatile u16 *)(SPU2_KOFF_A(core) + 2))
#define SPU2_TSA_A(core)  (SPU2_BASE + 0x1A8 + ((core) << 10))
#define SPU2_TSA(core)     (*(volatile u16 *)SPU2_TSA_A(core))
#define SPU2_TSA_HI(core)  (*(volatile u16 *)(SPU2_TSA_A(core) + 2))
#define SPU2_DATA0_A(core)  (SPU2_BASE + 0x1AC + ((core) << 10))
#define SPU2_DATA0(core)     (*(volatile u16 *)SPU2_DATA0_A(core))
#define SPU2_DATA0_HI(core)  (*(volatile u16 *)(SPU2_DATA0_A(core) + 2))
#define SPU2_DATA1_A(core)  (SPU2_BASE + 0x1AE + ((core) << 10))
#define SPU2_DATA1(core)     (*(volatile u16 *)SPU2_DATA1_A(core))
#define SPU2_DATA1_HI(core)  (*(volatile u16 *)(SPU2_DATA1_A(core) + 2))
#define SPU2_ADMAS_A(core)  (SPU2_BASE + 0x1B0 + ((core) << 10))
#define SPU2_ADMAS(core)     (*(volatile u16 *)SPU2_ADMAS_A(core))
#define SPU2_ADMAS_HI(core)  (*(volatile u16 *)(SPU2_ADMAS_A(core) + 2))
#define SPU2_SSA_A(core)  (SPU2_BASE + 0x1C0 + ((core) << 10))
#define SPU2_SSA(core)     (*(volatile u16 *)SPU2_SSA_A(core))
#define SPU2_SSA_HI(core)  (*(volatile u16 *)(SPU2_SSA_A(core) + 2))
#define SPU2_LSAX_A(core)  (SPU2_BASE + 0x1C4 + ((core) << 10))
#define SPU2_LSAX(core)     (*(volatile u16 *)SPU2_LSAX_A(core))
#define SPU2_LSAX_HI(core)  (*(volatile u16 *)(SPU2_LSAX_A(core) + 2))
#define SPU2_NAX_A(core)  (SPU2_BASE + 0x1C8 + ((core) << 10))
#define SPU2_NAX(core)     (*(volatile u16 *)SPU2_NAX_A(core))
#define SPU2_NAX_HI(core)  (*(volatile u16 *)(SPU2_NAX_A(core) + 2))
#define SPU2_ESA_A(core)  (SPU2_BASE + 0x2E0 + ((core) << 10))
#define SPU2_ESA(core)     (*(volatile u16 *)SPU2_ESA_A(core))
#define SPU2_ESA_HI(core)  (*(volatile u16 *)(SPU2_ESA_A(core) + 2))
#define SPU2_EEA_A(core)  (SPU2_BASE + 0x33C + ((core) << 10))
#define SPU2_EEA(core)     (*(volatile u16 *)SPU2_EEA_A(core))
#define SPU2_EEA_HI(core)  (*(volatile u16 *)(SPU2_EEA_A(core) + 2))
#define SPU2_ENDX_A(core)  (SPU2_BASE + 0x340 + ((core) << 10))
#define SPU2_ENDX(core)     (*(volatile u16 *)SPU2_ENDX_A(core))
#define SPU2_ENDX_HI(core)  (*(volatile u16 *)(SPU2_ENDX_A(core) + 2))
#define SPU2_STATX_A(core)  (SPU2_BASE + 0x344 + ((core) << 10))
#define SPU2_STATX(core)     (*(volatile u16 *)SPU2_STATX_A(core))
#define SPU2_STATX_HI(core)  (*(volatile u16 *)(SPU2_STATX_A(core) + 2))

/* The volume coefficients, stride 40. */
#define SPU2_MVOLL_A(core)  (SPU2_BASE + 0x760 + (core) * SPU2_COEF_STRIDE)
#define SPU2_MVOLL(core)     (*(volatile u16 *)SPU2_MVOLL_A(core))
#define SPU2_MVOLL_HI(core)  (*(volatile u16 *)(SPU2_MVOLL_A(core) + 2))
#define SPU2_MVOLR_A(core)  (SPU2_BASE + 0x762 + (core) * SPU2_COEF_STRIDE)
#define SPU2_MVOLR(core)     (*(volatile u16 *)SPU2_MVOLR_A(core))
#define SPU2_MVOLR_HI(core)  (*(volatile u16 *)(SPU2_MVOLR_A(core) + 2))
#define SPU2_EVOLL_A(core)  (SPU2_BASE + 0x764 + (core) * SPU2_COEF_STRIDE)
#define SPU2_EVOLL(core)     (*(volatile u16 *)SPU2_EVOLL_A(core))
#define SPU2_EVOLL_HI(core)  (*(volatile u16 *)(SPU2_EVOLL_A(core) + 2))
#define SPU2_EVOLR_A(core)  (SPU2_BASE + 0x766 + (core) * SPU2_COEF_STRIDE)
#define SPU2_EVOLR(core)     (*(volatile u16 *)SPU2_EVOLR_A(core))
#define SPU2_EVOLR_HI(core)  (*(volatile u16 *)(SPU2_EVOLR_A(core) + 2))
#define SPU2_AVOLL_A(core)  (SPU2_BASE + 0x768 + (core) * SPU2_COEF_STRIDE)
#define SPU2_AVOLL(core)     (*(volatile u16 *)SPU2_AVOLL_A(core))
#define SPU2_AVOLL_HI(core)  (*(volatile u16 *)(SPU2_AVOLL_A(core) + 2))
#define SPU2_AVOLR_A(core)  (SPU2_BASE + 0x76A + (core) * SPU2_COEF_STRIDE)
#define SPU2_AVOLR(core)     (*(volatile u16 *)SPU2_AVOLR_A(core))
#define SPU2_AVOLR_HI(core)  (*(volatile u16 *)(SPU2_AVOLR_A(core) + 2))
#define SPU2_BVOLL_A(core)  (SPU2_BASE + 0x76C + (core) * SPU2_COEF_STRIDE)
#define SPU2_BVOLL(core)     (*(volatile u16 *)SPU2_BVOLL_A(core))
#define SPU2_BVOLL_HI(core)  (*(volatile u16 *)(SPU2_BVOLL_A(core) + 2))
#define SPU2_BVOLR_A(core)  (SPU2_BASE + 0x76E + (core) * SPU2_COEF_STRIDE)
#define SPU2_BVOLR(core)     (*(volatile u16 *)SPU2_BVOLR_A(core))
#define SPU2_BVOLR_HI(core)  (*(volatile u16 *)(SPU2_BVOLR_A(core) + 2))
#define SPU2_MVOLXL_A(core)  (SPU2_BASE + 0x770 + (core) * SPU2_COEF_STRIDE)
#define SPU2_MVOLXL(core)     (*(volatile u16 *)SPU2_MVOLXL_A(core))
#define SPU2_MVOLXL_HI(core)  (*(volatile u16 *)(SPU2_MVOLXL_A(core) + 2))
#define SPU2_MVOLXR_A(core)  (SPU2_BASE + 0x772 + (core) * SPU2_COEF_STRIDE)
#define SPU2_MVOLXR(core)     (*(volatile u16 *)SPU2_MVOLXR_A(core))
#define SPU2_MVOLXR_HI(core)  (*(volatile u16 *)(SPU2_MVOLXR_A(core) + 2))

/* The reverb coefficients, in the same stride 40 block as the volumes above.
 * sceSdApplyEffectAttrMask reaches these through the runtime sceSdRegBase
 * rather than a literal base, so they are offsets: the retail code loads that
 * global and adds, and naming the whole address here would change the code.
 * Names are the SPU2 reverb coefficients the recovered mask bits select. */
#define SPU2_O_VIIR     0x774
#define SPU2_O_VCOMB1   0x776
#define SPU2_O_VCOMB2   0x778
#define SPU2_O_VCOMB3   0x77A
#define SPU2_O_VCOMB4   0x77C
#define SPU2_O_VWALL    0x77E
#define SPU2_O_VAPF1    0x780
#define SPU2_O_VAPF2    0x782
#define SPU2_O_VLIN     0x784
#define SPU2_O_VRIN     0x786

/* The two IOP DMA channels the SPU2 transfers run on: channel 4 at 0xBF8010C0
 * and channel 7 at 0xBF801500, which is why the stride between them is 0x440.
 * MADR is the host address, BCR holds the block size and the block count in
 * its two halfwords, CHCR is the control word. Every site adds its own channel
 * offset, in one of several forms, so these are addresses and not lvalues. */
#define IOP_DMA_SPU_STRIDE  0x440
#define IOP_DMA_MADR_A      (IOP_HW_BASE + 0x10C0)
#define IOP_DMA_BCR_A       (IOP_HW_BASE + 0x10C4)
#define IOP_DMA_BCR_HI_A    (IOP_HW_BASE + 0x10C6)
#define IOP_DMA_CHCR_A      (IOP_HW_BASE + 0x10C8)

/* Not per core. */
#define SPU2_SPDIF_OUT_A        (SPU2_BASE + 0x7C0)
#define SPU2_SPDIF_OUT          (*(volatile u16 *)SPU2_SPDIF_OUT_A)
/* Read for two flag bits in bits 2..3; the name is from that use, not from
 * the register table, which does not cover the SPDIF block. */
#define SPU2_SPDIF_STAT_A        (SPU2_BASE + 0x7C2)
#define SPU2_SPDIF_STAT          (*(volatile u16 *)SPU2_SPDIF_STAT_A)
#define SPU2_SPDIF_MODE_A        (SPU2_BASE + 0x7C6)
#define SPU2_SPDIF_MODE          (*(volatile u16 *)SPU2_SPDIF_MODE_A)
#define SPU2_SPDIF_MEDIA_A        (SPU2_BASE + 0x7C8)
#define SPU2_SPDIF_MEDIA          (*(volatile u16 *)SPU2_SPDIF_MEDIA_A)
#define SPU2_SPDIF_PROTECT_A        (SPU2_BASE + 0x7CA)
#define SPU2_SPDIF_PROTECT          (*(volatile u16 *)SPU2_SPDIF_PROTECT_A)
#define IOP_SPU_DELAY_A        (IOP_HW_BASE + 0x1014)
#define IOP_SPU_DELAY          (*(volatile u32 *)IOP_SPU_DELAY_A)
#define IOP_SPU2_DELAY_A        (IOP_HW_BASE + 0x1414)
#define IOP_SPU2_DELAY          (*(volatile u32 *)IOP_SPU2_DELAY_A)
#define IOP_DPCR_A        (IOP_HW_BASE + 0x10F0)
#define IOP_DPCR          (*(volatile u32 *)IOP_DPCR_A)
#define IOP_DPCR2_A        (IOP_HW_BASE + 0x1570)
#define IOP_DPCR2          (*(volatile u32 *)IOP_DPCR2_A)
#define IOP_SPU2_DMA0_BANK_A        (IOP_HW_BASE + 0x1404)
#define IOP_SPU2_DMA0_BANK          (*(volatile u32 *)IOP_SPU2_DMA0_BANK_A)
#define IOP_SPU2_DMA1_BANK_A        (IOP_HW_BASE + 0x140C)
#define IOP_SPU2_DMA1_BANK          (*(volatile u32 *)IOP_SPU2_DMA1_BANK_A)
#define IOP_DMA_CH4_ENABLE  0x80000
#define IOP_DMA_CH7_ENABLE  0x8

#endif /* IRX_SPU2_H */
