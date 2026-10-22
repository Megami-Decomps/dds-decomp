#ifndef EFF_PCP_EVENT_EMITTER_STATE_H
#define EFF_PCP_EVENT_EMITTER_STATE_H

#include "common.h"
#include "eff.h"
#include "eff_pcp_drift_event_row.h"
#include "eff_pcp_paired_event_row.h"

struct SoundMixer;
struct sdfMemHandle_t;

/* Serialized drift ranges copied into an owner before its event rows. */
typedef struct EffPCPDriftEventParams {
    f32 position[4];
    u8 pad10[0x44];
    s32 duration;
    u32 count;
    s32 delaySpread;
    s32 fadeIn;
    s32 fadeOut;
    f32 startPosition, endPosition, startJitter, endJitter;
    f32 angleStep;
    f32 angleDecay;
    f32 angleJitter;
    u8 pad84[0x80];
} EffPCPDriftEventParams;

/* Fixed 0x11C-byte owner followed by count * 0x18 drift rows. */
typedef struct EffPCPDriftEventWork {
    EffPCPDriftEventParams params;
    EffPCPDriftEventRow *entries;
    struct SoundMixer *owner;
    u8 unk10C;
    u8 pad10D[3];
    f32 scale;
    u32 color;
    struct sdfMemHandle_t *handle;
} EffPCPDriftEventWork;

/* Serialized fragment parameters for the paired event emitter. */
typedef struct EffPCPPairedEventParams {
    f32 origin[4];
    u8 repeat;
    u8 pad11[3];
    s32 duration;
    u32 count;
    s32 delaySpread;
    s32 fadeIn;
    s32 fadeOut;
    f32 baseRadius, radiusJitter, baseTiltStep, tiltJitter;
    EffThunderFragmentParams fragmentParams;
} EffPCPPairedEventParams;

/* Fixed 0xA4-byte owner followed by count * 0x20 paired-event rows. */
typedef struct EffPCPPairedEventWork {
    EffPCPPairedEventParams params;
    EffPCPPairedEventRow *entries;
    struct SoundMixer *ownerA, *ownerB;
    u8 unk98;
    u8 pad99[3];
    u32 color;
    struct sdfMemHandle_t *handle;
} EffPCPPairedEventWork;

typedef char EffPCPDriftEventParams_size_must_be_0x104[
    (sizeof(EffPCPDriftEventParams) == 0x104) ? 1 : -1];
typedef char EffPCPDriftEventWork_size_must_be_0x11C[
    (sizeof(EffPCPDriftEventWork) == 0x11C) ? 1 : -1];
typedef char EffPCPDriftEventWork_entries_at_0x104[
    ((u32)&((EffPCPDriftEventWork *)0)->entries == 0x104) ? 1 : -1];
typedef char EffPCPDriftEventWork_handle_at_0x118[
    ((u32)&((EffPCPDriftEventWork *)0)->handle == 0x118) ? 1 : -1];

typedef char EffPCPPairedEventParams_size_must_be_0x8C[
    (sizeof(EffPCPPairedEventParams) == 0x8C) ? 1 : -1];
typedef char EffPCPPairedEventWork_size_must_be_0xA4[
    (sizeof(EffPCPPairedEventWork) == 0xA4) ? 1 : -1];
typedef char EffPCPPairedEventWork_entries_at_0x8C[
    ((u32)&((EffPCPPairedEventWork *)0)->entries == 0x8C) ? 1 : -1];
typedef char EffPCPPairedEventWork_handle_at_0xA0[
    ((u32)&((EffPCPPairedEventWork *)0)->handle == 0xA0) ? 1 : -1];

#endif
