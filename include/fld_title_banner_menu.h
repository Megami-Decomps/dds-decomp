#ifndef FLD_TITLE_BANNER_MENU_H
#define FLD_TITLE_BANNER_MENU_H

#include "common.h"

struct KwlnTask;

/* Task initializer used by the field title-banner panel in both games. */
s32 fldInitializeTitleBannerTask(struct KwlnTask *task);

/* Title-banner consumer confirms these four halfwords and their offsets. */
typedef struct FldTitleBannerMenu {
    u16 position; /* 0x00 */
    u16 choice;   /* 0x02 */
    u16 pending;  /* 0x04 */
    u16 reserved; /* 0x06 */
} FldTitleBannerMenu;

typedef char FldTitleBannerMenu_size_must_be_8[
    (sizeof(FldTitleBannerMenu) == 8) ? 1 : -1];

FldTitleBannerMenu *fldSetTitleBannerPending(s32 isPending);
FldTitleBannerMenu *fldSetTitleBannerChoice(s32 choice);

#endif /* FLD_TITLE_BANNER_MENU_H */
