#ifndef SDF_DEV_START_LOAD_API_H
#define SDF_DEV_START_LOAD_API_H

/* Both paths are read without modification. The disc path is retained in
 * global device state after this call, so it must have static lifetime; the
 * preload path is consumed synchronously while loading the file. */
void sdfDevStartLoad(const char *discFileName, const char *preloadFileName);

#endif
