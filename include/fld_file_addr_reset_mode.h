#ifndef FLD_FILE_ADDR_RESET_MODE_H
#define FLD_FILE_ADDR_RESET_MODE_H

#include "common.h"

typedef u32 FldFileAddrResetMode;
enum {
    FLD_FILE_ADDR_RESET_MODE_DISABLED = 0,
    FLD_FILE_ADDR_RESET_MODE_RELOCATE_ON_RELEASE = 1
};

/* Controls address relocation for retained transfer chunks at field release. */
void fldFileAddrResetMode(FldFileAddrResetMode mode);

#endif /* FLD_FILE_ADDR_RESET_MODE_H */
