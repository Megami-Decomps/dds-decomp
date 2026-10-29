#ifndef SDF_FILE_EXISTENCE_API_H
#define SDF_FILE_EXISTENCE_API_H

#include "common.h"

typedef s32 SdfFileExistenceResult;
enum {
    SDF_FILE_NOT_FOUND = 0,
    SDF_FILE_EXISTS = 1
};

SdfFileExistenceResult sdfFileCheckExist(const char *path);

#endif /* SDF_FILE_EXISTENCE_API_H */
