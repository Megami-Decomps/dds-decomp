#include "common.h"

typedef struct DspUnit {
    u8 pad00[0x20];
    s32 displayMode;
    u8 pad24[0xC];
    s32 *selectedValue;
} DspUnit;

typedef struct DspWindowContext {
    u8 pad00[0xC];
    DspUnit *unit;
} DspWindowContext;

extern void func_0024E260(s32, s32, s32, s32, s32, s32);

void func_002549F0(s32 x, s32 y, s32 layer, s32 scale,
                   DspWindowContext *window, s32 context) {
    switch (window->unit->displayMode) {
    case 3:
        func_0024E260(x, y, layer, scale, 0x50, context);
        func_0024E260(x, y, layer, scale, 0x53, context);
        return;
    case 4:
        func_0024E260(x, y, layer, scale, 0x51, context);
        func_0024E260(x, y, layer, scale, 0x54, context);
        return;
    case 5:
        func_0024E260(x, y, layer, scale, 0x52, context);
        func_0024E260(x, y, layer, scale, 0x55, context);
        break;
    }
}
