#ifndef DDS_SCR_PROCESS_HANDLE_API_H
#define DDS_SCR_PROCESS_HANDLE_API_H

#include "kwln.h"

/* Script command operands and return values encode scheduler task pointers in
 * signed 32-bit VM integer slots. Preserve all pointer bits at that boundary. */
typedef dds3ProcessID_t *ScrProcessHandle;
typedef char ScrProcessHandle_must_be_32_bits[
    (sizeof(ScrProcessHandle) == sizeof(u32)) ? 1 : -1];

static inline ScrProcessHandle scrVmIntegerToProcessHandle(s32 value)
{
    return (ScrProcessHandle)(u32)value;
}

static inline s32 scrProcessHandleToVmInteger(ScrProcessHandle handle)
{
    return (s32)(u32)handle;
}

#endif /* DDS_SCR_PROCESS_HANDLE_API_H */
