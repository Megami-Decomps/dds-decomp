#ifndef MDL_DONE_JOB_H
#define MDL_DONE_JOB_H

#include "file_pac.h"

#define MDL_DONE_JOB_BYTES 0x14

/* Allocated by mdlRequestLoadWithCallback and consumed by the completion path. */
typedef struct MdlDoneJob {
    u16 group;            /* 0x00 */
    u16 id;               /* 0x02 */
    u32 arg;              /* 0x04 */
    FilePacRequest *owner; /* 0x08 */
    void (*done)(u32);    /* 0x0C */
    u32 doneArg;          /* 0x10 */
} MdlDoneJob;

typedef char MdlDoneJob_size_must_be_0x14[
    sizeof(MdlDoneJob) == MDL_DONE_JOB_BYTES ? 1 : -1];
typedef char MdlDoneJob_owner_at_0x08[
    ((u32)&((MdlDoneJob *)0)->owner == 0x08) ? 1 : -1];
typedef char MdlDoneJob_done_at_0x0C[
    ((u32)&((MdlDoneJob *)0)->done == 0x0C) ? 1 : -1];
typedef char MdlDoneJob_done_arg_at_0x10[
    ((u32)&((MdlDoneJob *)0)->doneArg == 0x10) ? 1 : -1];

#endif /* MDL_DONE_JOB_H */
