#include "common.h"

#ifdef NON_MATCHING
#include "common.h"
#include "sdfCorePacketCb_t.h"

/* Colour each of the work's vertex records from its normal's light term w
 * (2d - 1, from the lighting kernels): colour = parameters[1] +
 * parameters[2] * clamp(0.5 + 0.5w, 0, 1), on VU0 as retail computes it.
 * Then each blend node takes its first source's colour. Upstream:
 * hand-written assembly (hand/vu/corePktApplyFaceDivColor.s), retail's
 * instructions per vertex on the EE interpreter. */
void corePktApplyFaceDivColor(sdfCorePacketCb_t *work, const u128 *parameters) {
    u8 *record = (u8 *)work->pointBuff;
    s16 count = work->pointCount;
    sdfCorePacketPointBuff_tag *node;

    /* vf12 = 0.5 in every lane. */
    __asm__ volatile(
        "vsub.xyzw vf12, vf0, vf0\n\tlqc2 vf10, 16(%0)\n\tlqc2 vf11, 32(%0)\n\tqmtc2.ni %1, vf2\n\t"
        "vaddx.xyzw vf12, vf12, vf2x"
        : : "r"(parameters), "r"(0x3F000000u));
    do {
        __asm__ volatile(
            "lqc2 vf2, 16(%0)\n\tvmulaw.w ACC, vf12, vf0w\n\tvmaddw.w vf3, vf2, vf12w\n\t"
            "vmaxx.w vf4, vf3, vf0x\n\tvminiw.w vf5, vf4, vf0w\n\t"
            "vmulaw.xyzw ACC, vf10, vf0w\n\tvmaddw.xyzw vf6, vf11, vf5w\n\tsqc2 vf6, 32(%0)"
            : : "r"(record) : "memory");
        record += sizeof(sdfCorePacketPointBuff_tag);
    } while (--count != 0);
    for (node = work->extPointTop; node != NULL; node = node->next) {
        *(u128 *)node->color = *(const u128 *)node->sourceA->color;
    }
}
#else
INCLUDE_ASM(const s32, "hand/vu/sdfVuVertexAlphaClamp", corePktApplyFaceDivColor);
#endif
