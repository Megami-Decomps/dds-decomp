#ifndef PTY_REMOVE_UNIT_ITEM_DISPOSITION_API_H
#define PTY_REMOVE_UNIT_ITEM_DISPOSITION_API_H

#include "pty_roster_api.h"

/* Zero returns the removed unit's assigned item to inventory before saving
 * its template; every nonzero value retains the assignment in that template. */
typedef s32 PtyRemoveUnitItemDisposition;
enum {
    PTY_REMOVE_UNIT_ITEM_RETURN_TO_INVENTORY = 0,
    PTY_REMOVE_UNIT_ITEM_RETAIN_IN_TEMPLATE = 1,
};

PtyRemoveUnitResult ptyRemoveUnitWithFlags(
    DatUnitId unitId, PtyRemoveUnitItemDisposition itemDisposition);

#endif /* PTY_REMOVE_UNIT_ITEM_DISPOSITION_API_H */
