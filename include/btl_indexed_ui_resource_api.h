#ifndef BTL_INDEXED_UI_RESOURCE_API_H
#define BTL_INDEXED_UI_RESOURCE_API_H

#include "common.h"

struct BtlUnit;

/*
 * Select the unit's gun-type text from its party-record ID. The returned
 * read-only string is borrowed from the global name table.
 */
const u8 *btlGetIndexedUiResource(const struct BtlUnit *unit);

#endif
