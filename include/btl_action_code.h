#ifndef BTL_ACTION_CODE_H
#define BTL_ACTION_CODE_H

#include "common.h"

/* Camera action-table selectors preserve each title's command-field width. */
#ifdef VERSION_DDS1
typedef s32 BtlActionCode;
#else
typedef u32 BtlActionCode;
#endif

#endif /* BTL_ACTION_CODE_H */
