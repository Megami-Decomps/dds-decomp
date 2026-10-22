#ifndef DDS3_WORLD_SECONDARY_H
#define DDS3_WORLD_SECONDARY_H

struct dds3Basic_t;

/* Return the borrowed secondary world node, or NULL when no world is active. */
struct dds3Basic_t *dds3GetControlScene(void);

#endif
