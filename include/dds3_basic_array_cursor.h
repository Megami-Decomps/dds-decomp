#ifndef DDS3_BASIC_ARRAY_CURSOR_H
#define DDS3_BASIC_ARRAY_CURSOR_H

#include "dds3obj.h"

/* Cursor advancement returns whether cursorIndex refers to a linked entry. */
typedef u32 Dds3BasicArrayCursorResult;
enum {
    DDS3_BASIC_ARRAY_CURSOR_EXHAUSTED = 0,
    DDS3_BASIC_ARRAY_CURSOR_HAS_ENTRY = 1
};

/* Compatible redeclarations keep the broad object header unchanged. */
Dds3BasicArrayCursorResult dds3SetFirstBasicArray(dds3BasicArrayHandle_t *object);
Dds3BasicArrayCursorResult dds3SetNextBasicArray(dds3BasicArrayHandle_t *object);

#endif /* DDS3_BASIC_ARRAY_CURSOR_H */
