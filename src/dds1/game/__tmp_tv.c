#include "common.h"
typedef struct EffectMapping { u8 pad_00[0x0C]; u32 field0C; u32 field10; u8 *table; u32 count; } EffectMapping;
extern EffectMapping D_0038F898;
extern u32 D_003BD11C; extern u32 D_003BD120; extern u32 D_003BD124; extern u32 D_003BD128;
void func_002B92F0(void) {
    D_003BD11C = 0;
    D_003BD128 = 0;
    D_003BD120 = 0;
    D_003BD124 = 1;
    D_0038F898.field0C = 1;
    D_0038F898.field10 |= 0x10;
}
