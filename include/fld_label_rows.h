#ifndef FLD_LABEL_ROWS_H
#define FLD_LABEL_ROWS_H

#include "fld_label_row.h"

/* DDS2 FLDALL.TBL loads 512 records of each kind (0x3C00/0x4400 bytes).
 * The coordinate lookups read signed X/Y halfwords at 0/2; label text is +4. */

extern FldLabelRow30 D_0039A5A8[];
extern FldLabelRow34 D_0039E1A8[];

#endif
