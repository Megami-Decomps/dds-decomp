#ifndef DDS3_WORLD_LIFECYCLE_H
#define DDS3_WORLD_LIFECYCLE_H

struct EffWorldNode;

/* Clear active-world endpoint references and destroy node when a world exists. */
void dds3DeleteScene(struct EffWorldNode *worldNode);

#endif /* DDS3_WORLD_LIFECYCLE_H */
