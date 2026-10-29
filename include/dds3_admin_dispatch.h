#ifndef DDS3_ADMIN_DISPATCH_H
#define DDS3_ADMIN_DISPATCH_H

#include "common.h"

typedef s32 Dds3AdminModeCleanupResult;

#define DDS3_ADMIN_MODE_CLEANUP_CONTINUE 0

/* One row per administrative mode. The entry receives the restore flag and
 * mode data. A zero cleanup result keeps the mode active; a nonzero result
 * requests that it exit. Each row is 12 bytes on the EE. */
typedef struct dds3AdminiList_t {
    void (*start)(s32 restoring, void *data);
    s32 (*end)(void);
    Dds3AdminModeCleanupResult (*check)(void);
} dds3AdminiList_t;

extern dds3AdminiList_t ddsAdminModeCallbacks[];

#endif /* DDS3_ADMIN_DISPATCH_H */
