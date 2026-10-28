#ifndef FONT_MEM_CONTROL_API_H
#define FONT_MEM_CONTROL_API_H

#include "common.h"

typedef s32 FntMemControlDestroyResult;

/* Distinguish a no-op on an inactive list from freeing both owned handles. */
enum {
    FNT_MEM_CONTROL_DESTROY_ALREADY_INACTIVE = 0,
    FNT_MEM_CONTROL_DESTROY_RELEASED = 1
};

FntMemControlDestroyResult fmDestroyFontMemControl(void);

#endif /* FONT_MEM_CONTROL_API_H */
