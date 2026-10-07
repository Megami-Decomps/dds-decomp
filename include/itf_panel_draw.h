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
/* Kind 8 owns a distinct 0x64-byte buffer, independently selected by the
 * native allocation, layout-initializer and color-initializer tables. */
typedef struct UiSpriteBandPayload {
    SdfTex *texture;
    DrawVertex vertices[8];
    DrawColorRec colors[2];
} UiSpriteBandPayload;
typedef char UiSpriteBandPayload_size[(sizeof(UiSpriteBandPayload) == 0x64) ? 1 : -1];
typedef char UiSpriteBandPayload_vertices[((u32)&((UiSpriteBandPayload *)0)->vertices == 4) ? 1 : -1];
typedef char UiSpriteBandPayload_colors[((u32)&((UiSpriteBandPayload *)0)->colors == 0x44) ? 1 : -1];
#endif
