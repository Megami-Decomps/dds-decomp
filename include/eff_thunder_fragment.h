#ifndef EFF_THUNDER_FRAGMENT_H
#define EFF_THUNDER_FRAGMENT_H

#include "eff.h"

typedef struct EffThunderFragmentWork EffThunderFragmentWork;

EffThunderFragmentWork *effThunderFragCreate(EffThunderFragmentParams *parameters);
void effThunderReleaseFragmentWork(EffThunderFragmentWork *work);

#endif
