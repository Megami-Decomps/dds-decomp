#ifndef PRF_CURRENT_RECORD_VIEW_H
#define PRF_CURRENT_RECORD_VIEW_H

struct datUnitWork_t;
struct DatProfileRecord;

/* Borrow the selected record from live game state as a read-only view. */
const struct DatProfileRecord *ptyGetCurrentProfileRecord(struct datUnitWork_t *unit);

#endif
