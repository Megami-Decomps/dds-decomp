#include "common.h"

INCLUDE_ASM(const s32, "mc/mcManager", func_00289D68);

void func_00289DA8(u32 port, u32 request) {
    func_002F6D50(port, 0, request, 0);
}

INCLUDE_ASM(const s32, "mc/mcManager", func_00289DC8);

void mcMakeDirectory(u32 port, u32 path) {
    sceMcMkdir(port, 0, path);
}

INCLUDE_ASM(const s32, "mc/mcManager", func_00289E38);

void func_00289E80(u32 port, u32 path, u32 mode, u32 flags) {
    func_002F6B80(port, 0, path, 0, flags, mode);
}

INCLUDE_ASM(const s32, "mc/mcManager", func_00289EB0);

void func_00289F10(u32 port, u32 request) {
    func_002F6F60(port, 0, request);
}

INCLUDE_ASM(const s32, "mc/mcManager", func_00289F30);

void func_00289F80(u32 port, u32 request, u32 buffer) {
    func_002F61D8(port, 0, request, buffer);
}

INCLUDE_ASM(const s32, "mc/mcManager", func_00289FA8);

void func_0028A008(void) {
    func_002F6338();
}

INCLUDE_ASM(const s32, "mc/mcManager", func_0028A020);

void func_0028A070(void) {
    func_002F6558();
}

INCLUDE_ASM(const s32, "mc/mcManager", func_0028A088);
