#ifndef FLD_LABEL_ROWS_H
#define FLD_LABEL_ROWS_H

#include "common.h"

/* DDS2 FLDALL.TBL loads 512 records of each kind (0x3C00/0x4400 bytes).
 * The coordinate lookups read signed X/Y halfwords at 0/2; label text is +4. */
typedef struct FldLabelRow30 {
    s16 x;
    s16 y;
    u8 name[26];
} FldLabelRow30;

typedef struct FldLabelRow34 {
    s16 x;
    s16 y;
    u8 name[30];
} FldLabelRow34;

typedef char FldLabelRow30SizeCheck[(sizeof(FldLabelRow30) == 30) ? 1 : -1];
typedef char FldLabelRow34SizeCheck[(sizeof(FldLabelRow34) == 34) ? 1 : -1];

extern FldLabelRow30 D_0039A5A8[];
extern FldLabelRow34 D_0039E1A8[];

#endif
