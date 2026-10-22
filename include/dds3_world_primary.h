#ifndef DDS3_WORLD_PRIMARY_H
#define DDS3_WORLD_PRIMARY_H

struct dds3Basic_t;

/* Return the borrowed primary world node, or NULL when no world is active. */
struct dds3Basic_t *dds3GetActiveScene(void);

#endif
