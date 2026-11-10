#include "sdk_ps2.h"
#include "eff_packet_params.h"
#include "sdf_packet_header.h"
#include "common.h"
#include "bill_work.h"
#include "eff_blur.h"
#include "kwln_draw_buffer.h"
#include "kwln_frame_draw_buffer.h"
#include "sdf_packet_list.h"
#include "sdf_column_packet_api.h"
#include "sdf_heap_api.h"
#include "btl_sound.h"
#include "eff_param.h"
#include "kwln_sprite.h"
#include "pcp_vu0.h"
#include "sdf_vu_matrix.h"
#include "ee_mmi.h"
#include "eff.h"
#include "par_cell_api.h"
#include "eff_dispatch_state.h"
#include "eff_offset_triangle.h"
#include "sdf_packet_alloc.h"
#include "kwln_texture_reference_flag_api.h"
#include "sdf_debug_api.h"

struct BtlUnit;

/* Node restarts and scales: retail leaves the dispatch state in a0 (and the
 * factor in f12) for them. */
extern void (*dds3SpPolyResetTable[])(EffectDispatchState *);

extern void (*dds3SpPolySetScaleTable[])(f32, EffectDispatchState *);

extern void effBTLPartDelete(SoundMixer *);

extern void effBTLPartCreate(SoundMixer *dst, SoundMixer *src);

/* Clone a mixer: copy its banks, rebuild the voice state from the original and start with no voices. */
SoundMixer *sndMixerClone(SoundMixer *src) {
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(sizeof(SoundMixer));
    SoundMixer *mixer = (void *)sdfMemLock(allocation);

    mixer->resource = allocation;
    memcpy(mixer, src, 0xC38);
    effBTLPartCreate(mixer, src);
    mixer->activeVoiceCount = 0;
    mixer->voiceList = NULL;
    return mixer;
}

void sndMixerDestroy(SoundMixer *mixer) {
    effBTLCutHandle_t *voice = mixer->voiceList;
    effBTLCutHandle_t *next;

    while (voice != NULL) {
        next = voice->next;
        effBTLCutDelete(voice);
        voice = next;
    }
    effBTLPartDelete(mixer);
    sdfMemFreeDirect(mixer->resource);
}

s32 effBTLGetMotionChangeFrame(const SoundMixer *mixer, EffBattleVoiceKind kind) {
    int bank = kind >= 2;
    return mixer->banks[bank].list.callbackFrame;
}

s32 effBTLGetCutEndFrame(const SoundMixer *mixer, EffBattleVoiceKind kind) {
    int bank = kind >= 2;
    return mixer->banks[bank].list.triggerFrame;
}

s32 effBTLGetDrawEndFrame(const SoundMixer *mixer, EffBattleVoiceKind kind) {
    int bank = kind >= 2;
    if (kind == 2) {
        return mixer->banks[bank].list.triggerFrame;
    }
    return mixer->banks[bank].list.colorRampEndFrame;
}

void effBTLCutListRelease(effBTLCutHandle_t *voice) {
    effBTLCutHandle_t **link = &voice->state->voiceList;
    effBTLCutHandle_t *cur;

    while ((cur = *link) != NULL) {
        if (cur == voice) {
            *link = cur->next;
            return;
        }
        link = &cur->next;
    }
}

extern f32 effGetActorScaleForParameterWork(struct BtlUnit *unit);

/* Create a voice on the mixer bank selected by kind (0/1: first bank, 2+:
 * second): link it into the mixer, duplicate each parameter row's work (with
 * the optional unit-scale callback) and count it as active. */
extern void * sdfChipAllocDirect(s32 size);

effBTLCutHandle_t *effCreateBattleVoiceOwner(SoundMixer *mixer, EffBattleVoiceKind kind, struct BtlUnit *owner, s32 value) {
    EffBattleEntryList *bank;
    EffBattleEntryList *parameters;
    effBTLCutHandle_t *effect;
    EffBattleEntry *entry;
    struct effPartHandle_t **workSlot;
    s32 remaining;
    u16 entryCount;
    u32 callbackEnabled;

    effect = sdfChipAllocDirect(sizeof(effBTLCutHandle_t));
    memset(effect, 0, sizeof(effBTLCutHandle_t));
    effect->color = 0x80808080;

    {
        EffBattleEntryList *upper = &mixer->banks[1].list;
        EffBattleEntryList *lower = &mixer->banks[0].list;

        bank = kind < 2 ? lower : upper;
    }
    effect->next = mixer->voiceList;
    effect->flags = 0xE;
    effect->triggerFrame = bank->triggerFrame;
    mixer->voiceList = effect;
    effect->state = mixer;
    effect->kind = kind;
    effect->list = bank;
    if (kind == 2) {
        effect->colorRampEndFrame = bank->triggerFrame;
    } else {
        effect->colorRampEndFrame = bank->colorRampEndFrame;
    }
    effect->unk20 = value;
    effect->owner = owner;

    parameters = effect->list;
    entry = parameters->entries;
    workSlot = effect->parameterWorks;
    entryCount = parameters->count;
    callbackEnabled = parameters->actorScaleEnabled;
    if (entryCount != 0) {
        remaining = entryCount;
        do {
            if (entry->parameterKind != EFF_BATTLE_PARAMETER_KIND_NO_WORK) {
                *workSlot = dds3EffectPartCopy(entry->value.work);
                if (callbackEnabled != 0 && owner != NULL &&
                    (entry->aimParams.kind == 0 || entry->aimParams.kind == 4)) {
                    f32 scale = effGetActorScaleForParameterWork((struct BtlUnit *)effect->owner);
                    dds3EffectPartSetScale(*workSlot, scale);
                }
            } else {
                *workSlot = NULL;
            }

            workSlot++;
            entry++;
            remaining--;
        } while (remaining != 0);
    }

    effect->currentFrame = 0;
    printf("effBTL:cut %d handle create(%p)[%d]\n",
                  effect->kind, effect->state,
                  effect->state->activeVoiceCount);
    effect->state->activeVoiceCount++;
    return effect;
}

INCLUDE_RODATA(const s32, "game/code_001606C0", D_003A0DB8);

