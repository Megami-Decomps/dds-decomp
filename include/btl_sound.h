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

/* The allocated 0x20-byte resource is also the system-effect task owner. */
typedef struct SoundResourceNode {
    u32 flags;
    s32 referenceCount; /* 0x04: retained effect tasks, not a separate prefix object. */
    u32 activeCount; /* 0x08: timed effects still using this resource. */
    s32 fadeCountdown;
    SoundMixer *resourceHandle; /* Owned clone, released by sndFreeResourceNode. */
    void *sourceHandle; /* Borrowed archive data kept by indexed nodes. */
    struct SoundResourceNode *previous;
    struct SoundResourceNode *next;
} SoundResourceNode;

/* Effect callbacks dereference these words as units, while the selector
 * provider stores their encoded keys (DDS1 001F1588 / DDS2 0020220C). */
typedef union ActorEffectOwner {
    struct BtlUnit *unit;
    s32 selectorKey;
} ActorEffectOwner;

typedef struct SoundEffectReferenceArgs {
    SoundResourceNode *source;
    u16 option;
    u8 pad06[2];
    SoundVoice *effect;
    ActorEffectOwner sourceOwner;
    s32 sourceSelector;
    s32 targetSelector;
    ActorEffectOwner targetOwner;
    u32 duration;
} SoundEffectReferenceArgs;

typedef struct SoundEffectSourceArgs {
    SoundResourceNode *source;
    SoundVoice *effect;
    struct BtlUnit *unit;
    u8 pad0C[4];
    u64 resource;
    u32 duration;
    s32 counter;
} SoundEffectSourceArgs;

typedef struct TimedUnitEffectArgs {
    SoundResourceNode *source;
    u16 option;
    u8 pad06[2];
    struct BtlUnit *unit;
    s32 channel;
    u32 volume;
    s32 frame;
} TimedUnitEffectArgs;

typedef struct EffectLoadArgs {
    SoundResourceNode *effect;
    void *loadHandle;
    const char *name;
} EffectLoadArgs;

struct ActiveSoundNode;
struct SoundResourceLink;
struct SoundLink;

SoundMixer *sndMixerClone(SoundMixer *source);
s32 sndReadSelectedMixerBankValue(SoundMixer *mixer, u16 kind);
void sndReleaseAllVoices(SoundMixer *mixer);
void sndCreateSystemEffect(SoundResourceNode *effect);
void sndDeleteSystemEffect(SoundResourceNode *effect);
s32 sndSetEffectNodeParameter(SoundResourceNode *effect, u16 option);
s32 sndGetEffectNodeParameter(SoundResourceNode *effect, u16 option);
s32 sndIsResourceNodeReferencedOrActive(SoundResourceNode *effect);
void btlExtendTaskFrameLimit(SoundResourceNode *effect, s32 frames);
u32 sndGetResourceStatus(SoundResourceNode *effect);
s32 sndHasResourceFlagsOneOrEight(struct ActiveSoundNode *node);
struct SoundResourceLink *sndAllocResourceLink(struct BtlUnit *owner);
struct SoundLink *sndAllocLink(struct BtlUnit *owner);
struct BtlRuntimeTask *sndCreateEffectSourceTask(SoundResourceNode *source, struct BtlUnit *owner, u64 resource);
void effReleaseBattleVoiceOwner(void *voice);

#endif /* BTL_SOUND_H */
