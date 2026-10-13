#ifndef BTL_SOUND_NODE_API_H
#define BTL_SOUND_NODE_API_H

struct ActiveSoundNode;
struct SoundResourceNode;

void sndFreeResourceNode(struct SoundResourceNode *node);
void sndFreeListNode(struct ActiveSoundNode *node);

#endif /* BTL_SOUND_NODE_API_H */
