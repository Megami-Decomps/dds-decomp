#ifndef PAR_OBJECT_CLIENT_VIEW_H
#define PAR_OBJECT_CLIENT_VIEW_H

#include "par_object.h"

/* The battle clients see the shared ParObj prefix followed by two kind-mode
 * bytes. The remaining allocated tail has no established meaning here. */
typedef struct ParObjClientView {
    ParObj owner;
    u8 mode150; /* 0x150 */
    u8 mode151; /* 0x151 */
    u8 opaque152[0x26];
} ParObjClientView;

typedef char ParObjClientView_size_must_be_0x178[
    (sizeof(ParObjClientView) == 0x178) ? 1 : -1];
typedef char ParObjClientView_mode150_offset_must_be_0x150[
    ((u32)&((ParObjClientView *)0)->mode150 == 0x150) ? 1 : -1];
typedef char ParObjClientView_mode151_offset_must_be_0x151[
    ((u32)&((ParObjClientView *)0)->mode151 == 0x151) ? 1 : -1];

#endif /* PAR_OBJECT_CLIENT_VIEW_H */
