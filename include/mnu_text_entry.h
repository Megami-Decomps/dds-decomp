#ifndef MNU_TEXT_ENTRY_H
#define MNU_TEXT_ENTRY_H

#include "common.h"

/* One fixed-width atlas row consumed as encoded text. Its encoding and
 * termination are left opaque. */
typedef struct MenuTextEntry {
    u8 encodedText[0x20];
} MenuTextEntry;

typedef char MenuTextEntrySizeCheck[(sizeof(MenuTextEntry) == 0x20) ? 1 : -1];

#endif /* MNU_TEXT_ENTRY_H */
