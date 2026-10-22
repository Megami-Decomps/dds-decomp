#ifndef DDS3_WORLD_LIFECYCLE_H
#define DDS3_WORLD_LIFECYCLE_H

struct dds3Basic_t;

/* Clear active-world endpoint references and destroy node when a world exists. */
void dds3DeleteScene(struct dds3Basic_t *worldNode);

#endif /* DDS3_WORLD_LIFECYCLE_H */
