#ifndef DDS3_WORLD_SECONDARY_H
#define DDS3_WORLD_SECONDARY_H

struct EffWorldNode;

/* Return the borrowed secondary world node, or NULL when no world is active. */
struct EffWorldNode *dds3GetWorldSecondaryObject(void);

#endif
