#ifndef FLD_LABEL_ROW_H
#define FLD_LABEL_ROW_H

#include "common.h"

/* DDS1 FLDALL.TBL has three 0x1C-byte label records: signed X/Y followed by
 * 24 bytes of text. */
typedef struct FldLabelRow28 {
    s16 x;
    s16 y;
    u8 name[24];
} FldLabelRow28;

/* DDS2 FLDALL.TBL label records store signed X/Y before their text. */
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

typedef char FldLabelRow28SizeCheck[(sizeof(FldLabelRow28) == 28) ? 1 : -1];
typedef char FldLabelRow30SizeCheck[(sizeof(FldLabelRow30) == 30) ? 1 : -1];
typedef char FldLabelRow34SizeCheck[(sizeof(FldLabelRow34) == 34) ? 1 : -1];

#endif /* FLD_LABEL_ROW_H */
