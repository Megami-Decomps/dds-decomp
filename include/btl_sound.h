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

typedef struct BattleEffect BattleEffect;
struct EffBattleEntryList;
struct EffParamWork;

typedef struct SoundMixer {
    SoundBank banks[2];
    u8 pad0C30[8];
    void *resource;
    u32 active;
    BattleEffect *voiceList;
} SoundMixer;

/* DDS1 00160958 / DDS2 00168548 allocate and clear exactly 0x128 bytes. */
struct BattleEffect {
    void *owner; /* 0x00 */
    SoundMixer *state; /* 0x04 */
    u32 color; /* 0x08 */
    u16 flags; /* 0x0C */
    u8 pad0E[2];
    u32 currentFrame; /* 0x10 */
    u32 triggerFrame; /* 0x14 */
    u32 colorRampEndFrame; /* 0x18 */
    u16 kind; /* 0x1C */
    u8 pad1E[2];
    s32 unk20;
    struct EffBattleEntryList *list; /* 0x24 */
    struct EffParamWork *parameterWorks[0x3C]; /* 0x28 */
    u32 sourceFrameCallbackAddress; /* 0x118 */
    u32 triggerFrameCallbackAddress; /* 0x11C */
    u32 unk120;
    BattleEffect *next; /* 0x124 */
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
    BattleEffect *effect;
    ActorEffectOwner sourceOwner;
    s32 sourceSelector;
    s32 targetSelector;
    ActorEffectOwner targetOwner;
    u32 duration;
} SoundEffectReferenceArgs;

typedef struct SoundEffectSourceArgs {
    SoundResourceNode *source;
    BattleEffect *effect;
    ActorEffectOwner owner;
    u8 pad0C[4];
    u64 resource;
    s32 frameCount;
    s32 fadeOutFrame;
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
BattleEffect *func_00160958(SoundMixer *mixer, u16 kind, void *owner, s32 value);
BattleEffect *func_00168548(SoundMixer *mixer, u16 kind, void *owner, s32 value);
void sndUnlinkVoice(BattleEffect *effect);
void effReleaseBattleVoiceOwner(BattleEffect *effect);
u32 effBattleGetCurrentFrame(BattleEffect *effect);
void effBattleUpdateSelectedValue(BattleEffect *effect, u32 endFrame);

#endif /* BTL_SOUND_H */
