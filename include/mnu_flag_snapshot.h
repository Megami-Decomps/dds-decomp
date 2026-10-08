#ifndef MNU_FLAG_SNAPSHOT_H
#define MNU_FLAG_SNAPSHOT_H

struct SdfMemBlock;

/* The snapshot handle owns the allocation; its retained data holds flag pairs. */
struct SdfMemBlock *mnuCreateFlagEntries(void);
#ifdef VERSION_DDS1
void mnuApplyFlagEntries(struct SdfMemBlock *snapshot);
#else
void mnuApplyCampResourceFlagEntries(struct SdfMemBlock *snapshot);
#endif

#endif
