#ifndef EFF_PCP_RING_BLOCK_H
#define EFF_PCP_RING_BLOCK_H

#include "eff.h"

/* Ring header followed immediately by its inline particle array. */
typedef struct EffPcpRingBlock {
    EffRingWork header;
    EffRingParticle vertices[1];
} EffPcpRingBlock;

#endif
