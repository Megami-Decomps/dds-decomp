#ifndef DDS3_ADMIN_DISPATCH_H
#define DDS3_ADMIN_DISPATCH_H

#include "common.h"

/* One row per administrative mode. The entry receives the restore flag and
 * mode data; destroy and cleanup return scheduler results. Each row is 12 bytes
 * on the EE. */
typedef struct Dds3AdminModeDispatch {
    void (*entry)(s32 restoring, void *data);
    s32 (*destroy)(void);
    s32 (*cleanup)(void);
} Dds3AdminModeDispatch;

extern Dds3AdminModeDispatch ddsAdminModeCallbacks[];

#endif /* DDS3_ADMIN_DISPATCH_H */
