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

/* Panel payloads borrow the manager-owned texture; panel release frees only
 * the payload allocation. Kinds 6/9 store its address as a raw word, while
 * kinds 7/8 retain a typed, read-only view. */
typedef struct UiSpriteTexturePayload {
    const sdfTexHandle_t *borrowedTexture;
} UiSpriteTexturePayload;
typedef char UiSpriteTexturePayload_size[(sizeof(UiSpriteTexturePayload) == 4) ? 1 : -1];
/* Kind 8 owns a distinct 0x64-byte buffer, independently selected by the
 * native allocation, layout-initializer and color-initializer tables. */
typedef struct UiSpriteBandPayload {
    const sdfTexHandle_t *borrowedTexture;
    DrawVertex vertices[8];
    DrawColorRec colors[2];
} UiSpriteBandPayload;
typedef char UiSpriteBandPayload_size[(sizeof(UiSpriteBandPayload) == 0x64) ? 1 : -1];
typedef char UiSpriteBandPayload_vertices[((u32)&((UiSpriteBandPayload *)0)->vertices == 4) ? 1 : -1];
typedef char UiSpriteBandPayload_colors[((u32)&((UiSpriteBandPayload *)0)->colors == 0x44) ? 1 : -1];

/* Bind a texture and emit a sprite from two vertices and packed UV/color pairs. */
void itfMiscDrawSpritePrim(DrawVertex *vertices, DrawColorRec *uv,
                           DrawColorRec *colors, s32 tail,
                           const sdfTexHandle_t *borrowedTexture, s32 flag,
                           sdfDrawTag_t *command);
#endif
