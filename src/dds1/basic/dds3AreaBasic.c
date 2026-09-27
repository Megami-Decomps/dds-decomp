#include "common.h"

typedef struct {
    u8 pad[0xC];
    s32 (*unkC)(void);
} AreaSub;

typedef struct {
    u8 pad[0x10];
    AreaSub *unk10;
} AreaObj;

void func_00111A68(void *arg0, s32 arg1, s32 arg2);

s32 func_00111108(void *arg) {
    func_00111A68(arg, 0, 5);
    return 1;
}

s32 func_00111130(AreaObj *arg) {
    s32 (*func)(void);

    func = arg->unk10->unkC;
    if (func == NULL) {
        return 1;
    }
    return func();
}
