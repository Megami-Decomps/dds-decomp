#include "common.h"
#include "pcp_vu0.h"

extern void effMiscQuaternionToMatrixVU(void);

/* vu0 routine: build the world matrix in vf28-vf31 from rotation (quaternion),
   per-row scale and translation. */
void func_0010FAA8(void *scale, void *rotation, void *translation) {
    VU0_LOAD_VF(vf10, rotation);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, scale);
    VU0_SCALE_MATRIX_ROWS(vf10);
    VU0_LOAD_VF(vf10, translation);
    VU0_SET_W_ONE(vf10);
    VU0_MOVE_VF(vf31, vf10);
}
