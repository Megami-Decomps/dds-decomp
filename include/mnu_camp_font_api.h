#ifndef MNU_CAMP_FONT_API_H
#define MNU_CAMP_FONT_API_H

struct evtViewerInfo_t;

/* Move the viewer's completed glyph into the current draw-buffer queue. */
void mnuCampLinkFontGlyph(struct evtViewerInfo_t *scene);

#endif /* MNU_CAMP_FONT_API_H */
