#ifndef MDL_OPTION_LABEL_H
#define MDL_OPTION_LABEL_H

#include "common.h"

/* Menu label coordinates, draw style, and format-string pointer. */
typedef struct MdlOptionLabel {
    u16 x;              /* 0x00 */
    u16 y;              /* 0x02 */
    s32 style;          /* 0x04 */
    const char *format; /* 0x08 */
} MdlOptionLabel;

typedef char MdlOptionLabel_size[(sizeof(MdlOptionLabel) == 0x0C) ? 1 : -1];
typedef char MdlOptionLabel_x_offset[((unsigned long)&((MdlOptionLabel *)0)->x == 0x00) ? 1 : -1];
typedef char MdlOptionLabel_y_offset[((unsigned long)&((MdlOptionLabel *)0)->y == 0x02) ? 1 : -1];
typedef char MdlOptionLabel_style_offset[((unsigned long)&((MdlOptionLabel *)0)->style == 0x04) ? 1 : -1];
typedef char MdlOptionLabel_format_offset[((unsigned long)&((MdlOptionLabel *)0)->format == 0x08) ? 1 : -1];

#endif
