#ifndef EFF_EVENT_SOUND_H
#define EFF_EVENT_SOUND_H

struct SoundMixer;

/* Clone a mixer owner or release all of its active voices. */
struct SoundMixer *effEventCloneSoundMixer(struct SoundMixer *source);
void effEventReleaseSoundMixerVoices(struct SoundMixer *mixer);

#endif
