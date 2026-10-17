#include "common.h"
#include "eff_emitter_type.h"

/* Persona 4 func_00492e30 @ 00492E30 (src/promoted/code1_0049.c), recompiled unchanged */
void dds3DispatchIndexedCallback(u16 *record, f32 scale) {
    effEmitterTypeDescriptors[*record].scaleParameters(record, scale);
}
