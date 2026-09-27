#include "common.h"

extern void (*D_003E95C4[])(void);

/* Persona 4 func_00492e30 @ 00492E30 (src/promoted/code1_0049.c), recompiled unchanged */
void func_002DC108(u16 *arg0) {
    D_003E95C4[*arg0 * 4]();
}
