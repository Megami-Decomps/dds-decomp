#include "common.h"

typedef struct DevBuildState {
    u32 magic;
    u32 entryCount;
} DevBuildState;

const char devBuildIdentifier[] = "DDS1 development ELF";
DevBuildState devBuildState = { 0x44455631, 0 };

extern s32 __real_sdfInitializeRuntimeAndStartMainLoop(u32 startupValue, void *startupData);

s32 __wrap_sdfInitializeRuntimeAndStartMainLoop(u32 startupValue, void *startupData) {
    devBuildState.entryCount++;
    return __real_sdfInitializeRuntimeAndStartMainLoop(startupValue, startupData);
}
