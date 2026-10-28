#ifndef DDS3_BASIC_ARRAY_SEARCH_H
#define DDS3_BASIC_ARRAY_SEARCH_H

#include "dds3obj.h"

/* Search results distinguish a found address from an empty slot or exhausted cursor. */
typedef s32 Dds3BasicArrayMatchResult;
enum {
    DDS3_BASIC_ARRAY_NO_MATCH = 0,
    DDS3_BASIC_ARRAY_MATCH_FOUND = 1
};

/* targetNode is borrowed and compared by identity; the search never writes it. */
Dds3BasicArrayMatchResult dds3SearchDeleteBasicArray(
    dds3BasicArrayHandle_t *indexNode, const dds3Basic_t *targetNode,
    s32 processAllMatches);
Dds3BasicArrayMatchResult dds3SetSearchNextBasicArray(
    dds3BasicArrayHandle_t *indexNode, const dds3Basic_t *targetNode);

#endif
