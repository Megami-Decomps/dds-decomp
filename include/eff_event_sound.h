#ifndef EFF_EVENT_SOUND_H
#define EFF_EVENT_SOUND_H

struct SoundMixer;

/* Create an owned mixer clone, then destroy it with its active effect state. */
struct SoundMixer *effEveBEDCreate(const struct SoundMixer *source);
void effEveBEDDelete(struct SoundMixer *mixer);

#endif
