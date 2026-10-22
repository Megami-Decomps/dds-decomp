#include "common.h"
#include "sdf_vu0.h"
#include "dds3_vu_transform.h"
#include "pcp_vu0.h"
#include "sdf_vu_matrix.h"


/* vu0 routine: build the world matrix in vf28-vf31 from rotation (quaternion),
   per-row scale and translation. */
void dds3MakeMatrix(const f32 scale[4],
                                       const f32 rotation[4],
                                       const f32 translation[4]) {
    SDF_VU0_LOADVECTOR(rotation);
    effMiscQuaternionToMatrixVU();
    SDF_VU0_LOADVECTOR(scale);
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(translation);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
}
