#ifndef BTL_INDEXED_UI_RESOURCE_API_H
#define BTL_INDEXED_UI_RESOURCE_API_H

#include "common.h"

struct BtlUnit;

/* Select the gun-type text for the unit's party-record ID. */
const u8 *btlGetIndexedUiResource(struct BtlUnit *unit);

#endif
