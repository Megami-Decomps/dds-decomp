#include "common.h"
#include "sdf_resource.h"

void mnuReleaseOptionalResourceSlot(u32 *resourceSlot) {
    if (resourceSlot != NULL) {
        sdfMemFreeDirect((struct sdfMemHandle_t *)(u32)(*resourceSlot));
        return;
    }
}
