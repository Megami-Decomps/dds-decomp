#include "common.h"

typedef struct {
    u8 pad[0x18];
    void *unk18;
    void *unk1C;
} EventObj;

s32 func_0010F878(void *arg, s32 arg1);

void func_0010F860(void *arg, s32 arg1);

void func_0010F8B0(void *arg);

INCLUDE_ASM(const s32, "basic/dds3EventObjectBasic", func_001163B0);

s32 func_001163F0(EventObj *arg) {
    void *data;

    data = arg->unk1C;
    if (func_0010F878(data, 1) == 1) {
        func_0010F860(data, 1);
        func_0010F8B0(data);
    }
    return 1;
}
