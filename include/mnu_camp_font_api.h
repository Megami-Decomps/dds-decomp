#ifndef MNU_CAMP_FONT_API_H
#define MNU_CAMP_FONT_API_H

struct EvtRuntime;

/* Move the viewer's completed glyph into the current draw-buffer queue. */
void mnuCampLinkFontGlyph(struct EvtRuntime *scene);

#endif /* MNU_CAMP_FONT_API_H */
