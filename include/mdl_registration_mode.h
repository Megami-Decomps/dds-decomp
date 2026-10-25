#ifndef MDL_REGISTRATION_MODE_H
#define MDL_REGISTRATION_MODE_H

#include "common.h"

/* Bit flags shared by grouped model registration and its motion records. */
typedef u32 MdlRegistrationMode;

/* When set, group teardown releases the resource list, request allocation,
 * and motion-slot handles. */
#define MDL_REGISTRATION_OWNS_GROUP_RESOURCES 1

/* Create a motion controller for each new model context when this is set. */
#define MDL_MOTION_REGISTRATION_CREATE_ON_MODEL_LOAD 0x100

#endif /* MDL_REGISTRATION_MODE_H */
