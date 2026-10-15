#ifndef MNU_VOICE_RESOURCE_PATH_API_H
#define MNU_VOICE_RESOURCE_PATH_API_H

#include "common.h"

/* Copy the title's voice-path prefix into caller-owned pathBuffer, append the
 * read-only filename, and return the resulting path buffer. */
char *mnuBuildVoiceResourcePath(char *pathBuffer, const char *filename);

#endif /* MNU_VOICE_RESOURCE_PATH_API_H */
