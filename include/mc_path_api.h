#ifndef MC_PATH_API_H
#define MC_PATH_API_H

#include "common.h"

void mcChangeCurrentDirectory(u32 context, const char *path);
void mcMakeDirectory(u32 context, const char *path);
void mcReadDirectoryEntries(u32 context, const char *path, void *entries, s32 entryLimit);
void mcDeleteFilePath(u32 context, const char *path);
void mcOpenFilePath(u32 context, const char *path, s32 openFlags);

#endif
