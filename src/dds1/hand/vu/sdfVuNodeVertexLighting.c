#include "common.h"

#ifdef NON_MATCHING
#include "common.h"
#include "sdf_vu_work.h"

extern u32 corePktCurrLight;  /* the selected block; its light vector at 0xD0 */
extern u128 sdfVuObjectBasisCache[3];
extern u64 sdf3DRefMapParam;
extern void corePktLerpUVColorChain(VuBlendNode *node);
extern void corePktEmitTexTrisCtx2(corePktCb_t *work, u64 samplingState, u64 textureState, u64 clampState,
                                          u64 alphaState, s32 clearPrimitiveFlags);

/* Light the work's vertex records by their normals and send them: each
 * record's colour is the node colour times the packed colour (both /128),
 * and its 0x30 quadword holds the normal's dot with the light (through the
 * rows of sdfVuObjectBasisCache, or the light itself with parameter 0x800) and the
 * node alpha / 128. Then the blend nodes, and the chunked transfer.
 * Upstream: hand-written assembly (hand/vu/corePktEmitRefMapPass.s),
 * retail's instructions on the EE interpreter. Retail's 0x30 quadword
 * carries leftover register bits in z and w, which this leaves zero. */
void corePktEmitRefMapPass(corePktCb_t *work) {
    u8 *record = (u8 *)work->dmaBase;
    u16 count = work->param1;
    const u8 *light = (const u8 *)corePktCurrLight + 0xD0;
    const u32 nodeColor = *(const u32 *)((const u8 *)work->node + 0x10);
    const u32 packedColor = work->packedColor;
    const u64 scale = 0x3C0000003C000000ULL; /* (1/128, 1/128) */

    __asm__ volatile("lqc2 vf10, 0(%0)" : : "r"(light));
    if ((work->nextParam & 0x800) == 0) {
        __asm__ volatile(
            "lqc2 vf24, 0(%0)\n\tlqc2 vf25, 16(%0)\n\tlqc2 vf26, 32(%0)\n\t"
            "vmul.xyz vf24, vf24, vf10\n\tvmul.xyz vf25, vf25, vf10\n\tvmul.xyz vf26, vf26, vf10\n\t"
            "vaddy.x vf24, vf24, vf24y\n\tvaddy.x vf25, vf25, vf25y\n\tvaddy.x vf26, vf26, vf26y\n\t"
            "vaddz.x vf24, vf24, vf24z\n\tvaddz.x vf25, vf25, vf25z\n\tvaddz.x vf26, vf26, vf26z"
            : : "r"(sdfVuObjectBasisCache));
    } else {
        __asm__ volatile("vaddx.x vf24, vf0, vf10x\n\tvaddy.x vf25, vf0, vf10y\n\tvaddz.x vf26, vf0, vf10z");
    }
    __asm__ volatile(
        "pextlb $2, $0, %0\n\tpextlh $2, $0, $2\n\tqmtc2.ni $2, vf4\n\tvitof0.xyzw vf4, vf4\n\t"
        "pextlb $2, $0, %1\n\tpextlh $2, $0, $2\n\tqmtc2.ni $2, vf6\n\tvitof0.xyzw vf6, vf6\n\t"
        "qmtc2.ni %2, vf5\n\tvmulx.xyz vf6, vf6, vf5x\n\tvmulw.y vf5, vf5, vf4w\n\tvmul.xyz vf6, vf6, vf4"
        : : "r"(nodeColor), "r"(packedColor), "r"(scale) : "$2");
    do {
        __asm__ volatile(
            "lqc2 vf2, 16(%0)\n\t"
            "vmulax.x ACC, vf24, vf2x\n\tvmadday.x ACC, vf25, vf2y\n\tvmaddz.x vf5, vf26, vf2z\n\t"
            "sqc2 vf6, 32(%0)\n\tsqc2 vf5, 48(%0)"
            : : "r"(record) : "memory");
        record += sizeof(VuBlendNode);
    } while (--count != 0);
    corePktLerpUVColorChain(work->blendList);
    corePktEmitTexTrisCtx2(work, 0, sdf3DRefMapParam, 5, 0x48, 0x20);
}
#else
INCLUDE_ASM(const s32, "hand/vu/sdfVuNodeVertexLighting", corePktEmitRefMapPass);
#endif
