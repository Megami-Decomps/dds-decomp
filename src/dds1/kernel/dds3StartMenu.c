#include "common.h"
#include "kernel.h"
#include "libsdf.h"

/* SMT3's version spins forever; DDS's returns at once. */
s32 dds3StartMenuProcess(dds3ProcessID_t *id) {
    return 0;
}
