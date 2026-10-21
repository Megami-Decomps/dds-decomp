#include "sdk_ps2.h"
#include "eff.h"
#include "common.h"
#include "sdf_resource.h"
#include "mnu_shooting.h"
#include "mdl.h"
#include "mdl_broadcast_callback.h"
#include "sdf_motion.h"
#include "eff_random.h"


extern f32 D_0037F5EC[];

extern f32 sdfViewEyeVector[4];

extern f32 sdfViewTargetVector[4];

extern Motion *mdlFindNodeById(MdlCtx *model, s32 id);

/* Replace only the high byte of the model's broadcast word. */
void mnuSetNodeModelBroadcastByte(MnuModelNode *node, u8 highByte) {
    u32 broadcastLowBytes = mdlGetBroadcastValue(node->model) & 0xFFFFFF;

    mdlApplyBroadcastVia(mdlBroadcastMasked, node->model, broadcastLowBytes | ((u32)highByte << 24));
}

void mnuSetModelNodeVisibility(u8 *node, s8 selector) {
    MnuModelNode *entry = (MnuModelNode *)node;
    if (selector == 1) {
        entry->model->flags &= ~MDL_SKIP_TRANSFORMS;
    } else {
        entry->model->flags |= MDL_SKIP_TRANSFORMS;
    }
}

/* Negative frames request a random sample; NaN and nonnegative frames pass through.
 * Post-call reset exception: retail 0031C9F0 is JAL + restores, whereas
 * 0031C9E4 is J. Qualified cc1probe (--version dds2 --func mnuSampleModelMotionAtFrame
 * --cse), verify: OK: 01.sibling retains direct call uid139 followed by
 * frame reset uid143; random call uid89 is already a sibling. The reset
 * survives through 11.cfg and disappears in 13.life. Resetting this
 * function's frame after direct sampling blocks only the early direct-arm
 * sibling decision; no reset instruction survives in the emitted code.
 */
void mnuSampleModelMotionAtFrame(MnuModelNode *node, f32 frame) {
    Motion *motion = mdlFindNodeById(node->model, 0);
    if (frame < 0.0f) {
        u32 random = sdfGetRand(NULL);
        sdfMotionSampleAtFrame(motion, (f32)(random % node->model->first->frameCount));
        return;
    }
    sdfMotionSampleAtFrame(motion, frame);
    frame = 0.0f;
}

/* Map a screen point onto the view plane using the eye/target Z separation. */
void itfProjectScreenPointToViewPlane(f32 *position, s32 screenX, s32 screenY) {
    f32 halfViewAngle;
    f32 eyeTargetDepth;

    halfViewAngle = D_0037F5EC[0] * 0.5f;
    eyeTargetDepth = sdfViewEyeVector[2] - sdfViewTargetVector[2];
    position[0] = eyeTargetDepth * tanf(halfViewAngle * 1.3f) *
        ((f32)(screenX - 256) * 0.00390625f);
    position[1] = eyeTargetDepth * tanf(halfViewAngle) *
        ((f32)(screenY - 224) / 224.0f);
    position[2] = 0.0f;
}

/* Variant of the screen-point projection with a fixed -300 eye depth and Z. */
void itfProjectScreenPointToFixedDepth(f32 *position, s32 screenX, s32 screenY) {
    f32 halfViewAngle;
    f32 depth;

    halfViewAngle = D_0037F5EC[0] * 0.5f;
    depth = -300.0f - sdfViewTargetVector[2];
    position[0] = depth * tanf(halfViewAngle * 1.3f) *
        ((f32)(screenX - 256) * 0.00390625f);
    position[1] = depth * tanf(halfViewAngle) *
        ((f32)(screenY - 224) / 224.0f);
    position[2] = -300.0f;
}

extern struct SdfPoolNode *D_00380788[13][4];
extern void mnuDeactivateModelNode(MnuModelNode *);
extern void mnuTranslateNodePrimaryVector(MnuModelNode *, f32, f32, f32);

void mnuUpdateSectionModelWork(MnuSectionModelWork *work, u32 flags) {
    s16 listIndex;
    s16 nodeIndex;
    MnuNodeList *list;
    MnuModelNode *node;
    f32 ratio;
    u8 alpha;

    for (listIndex = 0; listIndex < work->count; listIndex++) {
        list = &work->groups[listIndex];
        node = list->nodes;
        for (nodeIndex = 0; nodeIndex < list->count; nodeIndex++, node++) {
            if (!(node->flags & 1) || node->value4A == 0) {
                continue;
            }
            if (flags & 1) {
                if (node->value48 != 0) {
                    ratio = (f32)node->value48 / (f32)node->value4A;
                    alpha = ratio * 128.0f;
                    mnuSetNodeModelBroadcastByte(node, alpha);
                }
                mdlProcessContextNodesAndTransforms(node->model, D_00380788[0]);
            } else {
                mnuTranslateNodePrimaryVector(node, node->x, node->y, node->z);
                mdlProcessContextNodesAndTransforms(node->model, D_00380788[0]);
                if (node->value48 != 0 && --node->value48 != 0) {
                    ratio = (f32)node->value48 / (f32)node->value4A;
                    alpha = ratio * 128.0f;
                    mnuSetNodeModelBroadcastByte(node, alpha);
                } else {
                    mnuDeactivateModelNode(node);
                }
            }
        }
    }
}

