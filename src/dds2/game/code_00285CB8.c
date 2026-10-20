#include "common.h"
#include "sdf_resource.h"

void mnuReleaseOptionalResourceSlot(u32 *resourceSlot) {
    if (resourceSlot != NULL) {
        sdfMemFreeDirect((struct SdfMemBlock *)(u32)(*resourceSlot));
        return;
    }
}
