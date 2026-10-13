#include "common.h"
#include "dds3_vu_transform.h"
#include "pcp_vu0.h"

extern void effMiscQuaternionToMatrixVU(void);

/* vu0 routine: build the world matrix in vf28-vf31 from rotation (quaternion),
   per-row scale and translation. */
void dds3BuildVuTransformFromComponents(const f32 scale[4],
                                       const f32 rotation[4],
                                       const f32 translation[4]) {
    VU0_LOAD_VF(vf10, rotation);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, scale);
    VU0_SCALE_MATRIX_ROWS(vf10);
    VU0_LOAD_VF(vf10, translation);
    VU0_SET_W_ONE(vf10);
    VU0_MOVE_VF(vf31, vf10);
}
