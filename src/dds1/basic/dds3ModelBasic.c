#include "common.h"

typedef struct {
    u8 pad[0x18];
    void *unk18;
} ModelObj;

void func_002CFF98(void *arg);

void func_00116860(ModelObj *arg) {
    func_002CFF98(arg->unk18);
}
