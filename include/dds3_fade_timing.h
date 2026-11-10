#ifndef DDS3_FADE_TIMING_H
#define DDS3_FADE_TIMING_H

#include "common.h"

/* Configure the foreground fade timing mode and frame duration. */
void dds3SetQuake(s32 mode, s32 frames);
/* Disable configured timing while retaining the stored parameters. */
void dds3StopQuake(void);

#endif /* DDS3_FADE_TIMING_H */
