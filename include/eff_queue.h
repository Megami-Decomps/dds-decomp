#ifndef EFF_QUEUE_H
#define EFF_QUEUE_H

#include "common.h"

/* Native 0x70-byte output of the effect resource-name chooser in DDS1/2.
 * The producer fills both 50-byte strings: nameWithExtension is the selected
 * name followed by its extension, and name is retained without the extension.
 * Callers prepend the directory path when forming the full file path.
 * DDS1 reads completion unsigned and DDS2 also compares its signed view. */
typedef struct EffQueueRecord {
    char nameWithExtension[0x32];
    char name[0x32];
    union {
        s32 signedState; /* +0x64 */
        u32 state;
    } completion;
    u8 pad68[8];
} EffQueueRecord;

extern void effQueueResource(const char *extension, const char *resourceName);
extern void effUpdateResourceQueue(const char *directoryPath, const char *extension, EffQueueRecord *);

#endif /* EFF_QUEUE_H */
