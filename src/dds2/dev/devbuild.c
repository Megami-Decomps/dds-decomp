#include "common.h"

typedef struct DevBuildState {
    u32 magic;
    u32 entryCount;
} DevBuildState;

const char devBuildIdentifier[] = "DDS2 development ELF";
DevBuildState devBuildState = { 0x44455632, 0 };

extern s32 __real_sdfInitializeRuntimeAndStartMainLoop(void);

s32 __wrap_sdfInitializeRuntimeAndStartMainLoop(void) {
    devBuildState.entryCount++;
    return __real_sdfInitializeRuntimeAndStartMainLoop();
}
