#include "common.h"

typedef struct DevBuildState {
    u32 magic;
    u32 entryCount;
} DevBuildState;

const char devBuildIdentifier[] = "DDS1 development ELF";
DevBuildState devBuildState = { 0x44455631, 0 };

extern s32 __real_func_00101BD8(u32 startupValue, void *startupData);

s32 __wrap_func_00101BD8(u32 startupValue, void *startupData) {
    devBuildState.entryCount++;
    return __real_func_00101BD8(startupValue, startupData);
}
