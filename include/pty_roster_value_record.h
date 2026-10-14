#ifndef PTY_ROSTER_VALUE_RECORD_H
#define PTY_ROSTER_VALUE_RECORD_H

#include "common.h"

/* Shared roster-indexed table entry; only the leading value is recovered. */
typedef struct PtyRosterValueRecord {
    u16 value;
    u16 opaque02;
} PtyRosterValueRecord;

typedef char PtyRosterValueRecord_size_must_be_0x04[
    sizeof(PtyRosterValueRecord) == 0x04 ? 1 : -1];

#endif
