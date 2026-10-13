#ifndef MNU_SOUND_RESOURCE_PATH_API_H
#define MNU_SOUND_RESOURCE_PATH_API_H

#include "common.h"

/* Copy the title's sound-path prefix into caller-owned pathBuffer, append
 * filename, and return the resulting path buffer. */
char *mnuBuildSoundResourcePath(char *pathBuffer, char *filename);

#endif /* MNU_SOUND_RESOURCE_PATH_API_H */
