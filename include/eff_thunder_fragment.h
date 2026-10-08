#ifndef EFF_THUNDER_FRAGMENT_H
#define EFF_THUNDER_FRAGMENT_H

#include "eff.h"

typedef struct EffThunderFragmentWork EffThunderFragmentWork;

EffThunderFragmentWork *effThunderFragCreate(EffThunderFragmentParams *parameters);
/* Borrow the parameter block stored at the beginning of the owner. */
EffThunderFragmentParams *effThunderGetFragmentParameters(EffThunderFragmentWork *work);
void effThunderReleaseFragmentWork(EffThunderFragmentWork *work);
void effThunderSetFragmentColor(EffThunderFragmentWork *work, u32 color);
void effThunderUpdateFragments(EffThunderFragmentWork *work);

#endif
