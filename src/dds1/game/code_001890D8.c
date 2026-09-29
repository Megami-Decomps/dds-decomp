#include "common.h"

typedef struct {
    u8 pad0[0x1C];
    u32 handle;
} SceneResource;

INCLUDE_ASM(const s32, "game/code_001890D8", func_001890D8);

void effReleaseSceneResource(SceneResource *resource) {
    effMagatuhiReleaseResource(resource->handle);
    func_002CFF98(resource);
}

void func_001891A8(SceneResource *resource) {
    func_001893D8(resource->handle);
}

INCLUDE_ASM(const s32, "game/code_001890D8", func_001891C0);

INCLUDE_ASM(const s32, "game/code_001890D8", func_00189220);
