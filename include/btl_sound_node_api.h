#ifndef BTL_SOUND_NODE_API_H
#define BTL_SOUND_NODE_API_H

struct ActiveSoundNode;
struct SoundResourceNode;

/* Allocate the two independently linked sound-node kinds. */
struct SoundResourceNode *sndAllocResourceNode(void);
struct ActiveSoundNode *sndAllocListNode(void);

void sndFreeResourceNode(struct SoundResourceNode *node);
void sndFreeListNode(struct ActiveSoundNode *node);

#endif /* BTL_SOUND_NODE_API_H */
