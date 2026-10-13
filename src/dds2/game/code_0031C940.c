#include "mdl.h"
#include "sdf_motion.h"
#include "eff.h"
#include "common.h"
#include "sdf_resource.h"
#include "mnu_shooting.h"

extern u32 effMiscRand(struct EffRandState *);
extern Motion *mdlFindNodeById(MdlCtx *, s32);

void func_0031C940(MnuModelNode *node, f32 frame) {
    Motion *motion = mdlFindNodeById(node->model, 0);

    if (frame < 0.0f) {
        u32 sampleFrame = effMiscRand(NULL) % node->model->first->frameCount;
        sdfMotionSampleAtFrame(motion, sampleFrame);
        return;
    }
    sdfMotionSampleAtFrame(motion, frame);
    /* Retail 0x0031C9F0 keeps JAL; reset dies after sibling selection.
     * See docs/idioms.md, "Deleted sampling workspace: asymmetric final calls". */
    motion = NULL;
}
