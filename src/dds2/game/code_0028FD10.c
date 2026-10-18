#include "common.h"
#include "mnu_mantra.h"

extern MenuMantraRecord *mnuFindPendingMantraPanelRecord(MnuStatusResource *resource, s16 kind, s16 element);

/* Query the fallback panel record with both filters unrestricted. Native A0
 * supplies the resource object and the tail call forwards the selected V0. */
MenuMantraRecord *mnuGetDefaultMantraPanelRecord(MnuStatusResource *object) {
    return mnuFindPendingMantraPanelRecord(object, -1, -1);
}
