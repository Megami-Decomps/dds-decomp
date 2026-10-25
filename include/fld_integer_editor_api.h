#ifndef FLD_INTEGER_EDITOR_API_H
#define FLD_INTEGER_EDITOR_API_H

#include "common.h"

/* The selector encodes both integer storage width and signedness. */
typedef enum FldIntegerStorageType {
    FLD_INTEGER_STORAGE_UNSIGNED_BYTE = 1,
    FLD_INTEGER_STORAGE_UNSIGNED_HALFWORD = 2,
    FLD_INTEGER_STORAGE_UNSIGNED_WORD = 4,
    FLD_INTEGER_STORAGE_SIGNED_BYTE = -1,
    FLD_INTEGER_STORAGE_SIGNED_HALFWORD = -2,
    FLD_INTEGER_STORAGE_SIGNED_WORD = -4
} FldIntegerStorageType;

typedef char FldIntegerStorageType_size_must_be_4[
    (sizeof(FldIntegerStorageType) == 4) ? 1 : -1];

void fldStepIntByPad(void *valueAddress, FldIntegerStorageType storageType, s64 min, s64 max,
                     s64 step, s64 bigStep, s8 *padState);
void dds3DebugIntEdit(void *valueAddress, FldIntegerStorageType storageType, s64 min, s64 max,
                      s64 step, s64 bigStep);

#endif /* FLD_INTEGER_EDITOR_API_H */
