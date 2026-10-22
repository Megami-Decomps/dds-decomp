#ifndef KWLN_FADE_TIMING_H
#define KWLN_FADE_TIMING_H

#include "common.h"

/* Configure the foreground fade timing mode and frame duration. */
void dds3SetQuake(s32 mode, s32 frames);
/* Disable configured timing while retaining the stored parameters. */
void dds3StopQuake(void);

#endif /* KWLN_FADE_TIMING_H */
