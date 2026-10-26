#ifndef FLD_TITLE_BANNER_MENU_H
#define FLD_TITLE_BANNER_MENU_H

#include "common.h"

struct dds3ProcessID_t;

typedef s32 FldTitleBannerResult;

/* The panel holds a one-shot draw request until its update callback consumes it. */
enum {
    FLD_TITLE_BANNER_DRAW_NOT_REQUESTED = 0,
    FLD_TITLE_BANNER_DRAW_REQUESTED = 1,
};

typedef s32 FldTitleBannerDrawRequest;
typedef u16 FldTitleBannerDrawRequestState;

enum {
    FLD_TITLE_BANNER_UNRESOLVED = 0,
    FLD_TITLE_BANNER_RESOLVED = 1,
    FLD_TITLE_BANNER_NO_PENDING = -1,
};

FldTitleBannerResult fldResolvePendingTitleBanner(void);

/* Task initializer used by the field title-banner panel in both games. */
s32 fldInputPanelProcess1(struct dds3ProcessID_t *task);

/* Title-banner consumer confirms these four halfwords and their offsets. */
typedef struct FldTitleBannerMenu {
    u16 position; /* 0x00 */
    u16 choice;   /* 0x02 */
    FldTitleBannerDrawRequestState drawRequestState; /* 0x04: cleared when consumed. */
    u16 reserved; /* 0x06 */
} FldTitleBannerMenu;

typedef char FldTitleBannerMenu_size_must_be_8[
    (sizeof(FldTitleBannerMenu) == 8) ? 1 : -1];

void fldInputPanelSetDraw(FldTitleBannerDrawRequest request);
void fldInputPanelSetMessage(s32 choice);

#endif /* FLD_TITLE_BANNER_MENU_H */
