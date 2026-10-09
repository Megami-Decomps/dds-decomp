#ifndef MDL_RESOURCE_TABLE_H
#define MDL_RESOURCE_TABLE_H

#include "common.h"

/* The viewer writes the selection suffix as two independent halfwords. */
typedef struct MdlResourceSelection {
    u16 pathTable;
    u16 pathIndex;
    u16 unk4;
    u16 unk6;
} MdlResourceSelection;

typedef struct MdlResourcePath {
    const char *resourceListPath;
    const char *path;
    const char *motionPath;
} MdlResourcePath;

/* Shared descriptor layout for the separate selection and filename tables. */
typedef struct MdlResourceTable {
    void *entries;
    s32 count;
} MdlResourceTable;

#ifdef VERSION_DDS2
s32 mdlSetViewerSlotResourceHandles(s32 table, s32 slot, const char *resourceListPath,
                                  const char *path, const char *motionPath);
#endif

#endif /* MDL_RESOURCE_TABLE_H */
