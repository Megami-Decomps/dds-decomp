#ifndef BTL_SOUND_H
#define BTL_SOUND_H

#include "common.h"

typedef struct SoundBank {
    u8 pad00[8];
    s32 value08;
    u8 pad0C[0x44];
    s32 value50;
    u8 pad54[0x24];
    s32 value78;
    u8 pad7C[0x59C];
} SoundBank;

typedef struct SoundVoice SoundVoice;

typedef struct SoundMixer {
    SoundBank banks[2];
    u8 pad0C30[8];
    void *resource;
    u32 active;
    SoundVoice *voiceList;
} SoundMixer;

struct SoundVoice {
    void *owner;
    SoundMixer *mixer;
    u32 color; /* DDS1 001609B8 / DDS2 001685A8: initialized color. */
    u16 flags; /* DDS1 001609DC / DDS2 001685CC: initialized flags. */
    u8 pad0E[0x116];
    SoundVoice *next;
};


/* Shared 0x18-byte system-effect node prefix. Actor tasks maintain the
 * reference count at +4; timed effects maintain the active count at +8. */
typedef struct SoundEffectNode {
    u32 flags;
    s32 referenceCount;     /* 0x04 */
    u32 activeCount;        /* 0x08 */
    u8 padC[4];
    SoundMixer *handle;
    void *source; /* DDS1/DDS2 system-effect creation reads the borrowed +14 source. */
} SoundEffectNode;

typedef struct SoundResourceNode {
    u32 flags;
    u32 unk_04;
    u32 unk_08;
    s32 fadeCountdown;
    SoundMixer *resourceHandle; /* Owned clone, released by sndFreeResourceNode. */
    void *sourceHandle; /* Borrowed archive data kept by indexed nodes. */
    struct SoundResourceNode *previous;
    struct SoundResourceNode *next;
} SoundResourceNode;

SoundMixer *sndMixerClone(SoundMixer *source);
void sndReleaseAllVoices(SoundMixer *mixer);
void effReleaseBattleVoiceOwner(void *voice);

#endif /* BTL_SOUND_H */
