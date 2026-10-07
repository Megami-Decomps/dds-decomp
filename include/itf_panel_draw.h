#ifndef ITF_PANEL_DRAW_H
#define ITF_PANEL_DRAW_H
#include "sdf.h"

/* Native two-word integer draw point. */
typedef struct DrawVertex {
    s32 x;
    s32 y;
} DrawVertex;

/* Four representation words: RGBA, or two UV pairs for sprite packets. */
typedef struct DrawColorRec {
    u32 components[4];
} DrawColorRec;

/* Kind 7's allocation is exactly this texture pointer; its factory transports
 * the pointer through the existing u32 value argument. */
typedef struct UiSpriteTexturePayload {
    SdfTex *texture;
} UiSpriteTexturePayload;
typedef char UiSpriteTexturePayload_size[(sizeof(UiSpriteTexturePayload) == 4) ? 1 : -1];
#endif
