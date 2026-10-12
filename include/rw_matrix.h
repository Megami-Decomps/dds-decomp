#ifndef RW_MATRIX_H
#define RW_MATRIX_H

#include "sdf_quaternion.h"

/* RenderWare-style three-axis plus position matrix consumed by VU0 loaders. */
typedef struct RwMatrix {
    RwV3d right; /* 0x00 */
    u32 flags; /* 0x0C */
    RwV3d up; /* 0x10 */
    u32 pad1; /* 0x1C */
    RwV3d at; /* 0x20 */
    u32 pad2; /* 0x2C */
    RwV3d pos; /* 0x30 */
    u32 pad3; /* 0x3C */
} RwMatrix;

typedef char RwMatrix_size_must_be_0x40[
    (sizeof(RwMatrix) == 0x40) ? 1 : -1];
typedef char RwMatrix_right_offset_must_be_0[
    ((u32)&((RwMatrix *)0)->right == 0) ? 1 : -1];
typedef char RwMatrix_flags_offset_must_be_0x0C[
    ((u32)&((RwMatrix *)0)->flags == 0x0C) ? 1 : -1];
typedef char RwMatrix_up_offset_must_be_0x10[
    ((u32)&((RwMatrix *)0)->up == 0x10) ? 1 : -1];
typedef char RwMatrix_pad1_offset_must_be_0x1C[
    ((u32)&((RwMatrix *)0)->pad1 == 0x1C) ? 1 : -1];
typedef char RwMatrix_at_offset_must_be_0x20[
    ((u32)&((RwMatrix *)0)->at == 0x20) ? 1 : -1];
typedef char RwMatrix_pad2_offset_must_be_0x2C[
    ((u32)&((RwMatrix *)0)->pad2 == 0x2C) ? 1 : -1];
typedef char RwMatrix_pos_offset_must_be_0x30[
    ((u32)&((RwMatrix *)0)->pos == 0x30) ? 1 : -1];
typedef char RwMatrix_pad3_offset_must_be_0x3C[
    ((u32)&((RwMatrix *)0)->pad3 == 0x3C) ? 1 : -1];

#endif /* RW_MATRIX_H */
