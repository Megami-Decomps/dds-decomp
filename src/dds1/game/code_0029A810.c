#include "common.h"

extern void (*dds3IndexedCallbacks[])(void);

/* Persona 4 func_00492e30 @ 00492E30 (src/promoted/code1_0049.c), recompiled unchanged */
void dds3DispatchIndexedCallback(u16 *index) {
    dds3IndexedCallbacks[*index * 4]();
}
