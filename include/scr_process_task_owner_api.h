#ifndef DDS_SCR_PROCESS_TASK_OWNER_API_H
#define DDS_SCR_PROCESS_TASK_OWNER_API_H

#include "common.h"

struct scrProcessWork_s;

/* The scheduler user-value stores this owner: script updates borrow it and
 * the shutdown callback destroys it. */
typedef struct scrProcessWork_s *ScrProcessTaskOwner;
typedef char ScrProcessTaskOwner_must_be_32_bits[
    (sizeof(ScrProcessTaskOwner) == sizeof(u32)) ? 1 : -1];

static inline u32 scrProcessTaskOwnerToUserValue(ScrProcessTaskOwner owner)
{
    return (u32)owner;
}

static inline ScrProcessTaskOwner scrProcessTaskOwnerFromUserValue(u32 userValue)
{
    return (ScrProcessTaskOwner)userValue;
}

#endif /* DDS_SCR_PROCESS_TASK_OWNER_API_H */
