#ifndef EVT_VIEWER_ROWS_H
#define EVT_VIEWER_ROWS_H

#include "evt_polygon_movie.h"

/* The viewer reads these fixed row strides from borrowed PMD section data.
 * Only the observed payload prefixes are described here. */
typedef struct EvtCompactRow {
    u16 value;
    u16 parameter;
    u16 flags;
    s16 variant;
    u8 payload[8];
} EvtCompactRow;

typedef struct EvtExtendedRow {
    u16 value;
    u16 parameter;
    u16 flags;
    s16 variant;
    u8 reserved08[4];
    u8 payload[0x20];
} EvtExtendedRow;

typedef char EvtCompactRow_size[(sizeof(EvtCompactRow) == 0x10) ? 1 : -1];
typedef char EvtExtendedRow_size[(sizeof(EvtExtendedRow) == 0x2C) ? 1 : -1];

u16 evtGetRowValue(PolyMovieWork *work, s32 row);
s16 evtGetRowVariant(PolyMovieWork *work, s32 row);
u16 evtGetRowParameter(PolyMovieWork *work, s32 row);
u16 evtGetRowFlags(PolyMovieWork *work, s32 row);
u8 *evtGetRowPayloadAddress(PolyMovieWork *work, s32 row);

#endif
