#ifndef MNU_FLAG_SNAPSHOT_H
#define MNU_FLAG_SNAPSHOT_H

#include "mnu_event_flag_entries.h"

struct sdfMemHandle_t;

#define MNU_FLAG_SNAPSHOT_BYTES 0x140

typedef struct MnuFlagSnapshotEntry {
    s32 firstFlag;
    s32 firstOn;
    s32 secondFlag;
    s32 secondOn;
} MnuFlagSnapshotEntry;

typedef char MnuFlagSnapshotEntry_size[(sizeof(MnuFlagSnapshotEntry) == 0x10) ? 1 : -1];
typedef char MnuFlagSnapshotEntry_offsets[
    (((u32)&((MnuFlagSnapshotEntry *)0)->firstFlag == 0x00) &&
     ((u32)&((MnuFlagSnapshotEntry *)0)->firstOn == 0x04) &&
     ((u32)&((MnuFlagSnapshotEntry *)0)->secondFlag == 0x08) &&
     ((u32)&((MnuFlagSnapshotEntry *)0)->secondOn == 0x0C)) ? 1 : -1];
typedef char MnuFlagSnapshotEntry_allocation_extent[
    (sizeof(MnuFlagSnapshotEntry) * (MNU_SCENE_FLAG_ENTRY_COUNT + MNU_PARTY_FLAG_ENTRY_COUNT) ==
     MNU_FLAG_SNAPSHOT_BYTES) ? 1 : -1];

/* The snapshot handle owns the allocation; its retained data holds flag pairs.
 * Apply reads the retained entries but leaves the allocation with its caller. */
struct sdfMemHandle_t *mnuCreateFlagEntries(void);
#ifdef VERSION_DDS1
void mnuApplyFlagEntries(struct sdfMemHandle_t *snapshot);
#else
void mnuApplyCampResourceFlagEntries(struct sdfMemHandle_t *snapshot);
#endif

#endif
