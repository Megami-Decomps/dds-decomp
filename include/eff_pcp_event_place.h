#ifndef EFF_PCP_EVENT_PLACE_H
#define EFF_PCP_EVENT_PLACE_H

#include "common.h"

/* Misc clients use payload[0..6] as position and [7..10] as scale values;
 * Thunder callers retain the same layout with event-specific slots unknown. */
typedef struct EffPCPEventPlace {
    f32 payload[11]; /* 0x00 */
    u32 color;       /* 0x2C */
} EffPCPEventPlace;

typedef char EffPCPEventPlaceSizeCheck[(sizeof(EffPCPEventPlace) == 0x30) ? 1 : -1];
typedef char EffPCPEventPlaceColorOffset[
    ((unsigned int)&((EffPCPEventPlace *)0)->color == 0x2C) ? 1 : -1
];

#endif /* EFF_PCP_EVENT_PLACE_H */
