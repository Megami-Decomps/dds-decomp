#include "eff.h"
#include "common.h"
#include "sdf_resource.h"
#include "mnu_shooting.h"

#include "mdl.h"
#include "sdf_motion.h"
#include "eff_random.h"

extern Motion *mdlFindNodeById(MdlCtx *model, s32 id);
extern u32 effMiscRand(EffRandState *state);

/* Negative frames request a random sample; NaN and nonnegative frames pass through.
 * Post-call reset exception: retail 0031C9F0 is JAL + restores, whereas
 * 0031C9E4 is J. Qualified cc1probe (--version dds2 --func func_0031C940
 * --cse), verify: OK: 01.sibling retains direct call uid139 followed by
 * frame reset uid143; random call uid89 is already a sibling. The reset
 * survives through 11.cfg and disappears in 13.life. Resetting this
 * function's frame after direct sampling blocks only the early direct-arm
 * sibling decision; no reset instruction survives in the emitted code.
 */
void func_0031C940(MnuModelNode *node, f32 frame) {
    Motion *motion = mdlFindNodeById(node->model, 0);
    if (frame < 0.0f) {
        u32 random = effMiscRand(NULL);
        sdfMotionSampleAtFrame(motion, (f32)(random % node->model->first->frameCount));
        return;
    }
    sdfMotionSampleAtFrame(motion, frame);
    frame = 0.0f;
}
