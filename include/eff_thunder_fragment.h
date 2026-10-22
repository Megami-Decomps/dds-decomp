#ifndef EFF_THUNDER_FRAGMENT_H
#define EFF_THUNDER_FRAGMENT_H

#include "eff.h"

typedef struct EffThunderFragmentWork EffThunderFragmentWork;

EffThunderFragmentWork *effPCP_ThunderPolyCreate(EffThunderFragmentParams *parameters);
/* Borrow the parameter block stored at the beginning of the owner. */
EffThunderFragmentParams *effPCP_ThunderPolyGetParam(EffThunderFragmentWork *work);
void effPCP_ThunderPolyDelete(EffThunderFragmentWork *work);
void effPCP_ThunderPolySetColor(EffThunderFragmentWork *work, u32 color);
void effPCP_ThunderPolyDraw(EffThunderFragmentWork *work);

#endif
