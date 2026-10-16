#ifndef DDS3_WORLD_PRIMARY_H
#define DDS3_WORLD_PRIMARY_H

struct EffWorldNode;

/* Return the borrowed primary world node, or NULL when no world is active. */
struct EffWorldNode *dds3GetWorldObject(void);

#endif
