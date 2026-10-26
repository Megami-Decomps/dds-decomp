#ifndef DDS3_BASIC_LIFECYCLE_H
#define DDS3_BASIC_LIFECYCLE_H

#include "common.h"

/* dds3AllocBasic accepts a node only when its create callback returns exactly 1. */
typedef s32 Dds3BasicCreateResult;

#define DDS3_BASIC_CREATE_ACCEPTED ((Dds3BasicCreateResult)1)

#endif /* DDS3_BASIC_LIFECYCLE_H */
