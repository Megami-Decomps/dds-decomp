#ifndef EFF_PARTICLE_VIEW_H
#define EFF_PARTICLE_VIEW_H

#include "eff.h"

/* Single-particle callbacks interpret selected fields in the shared emitter
 * owner differently; color/alpha/lastSpeed remain aliases inside its fade span. */
typedef struct EffParticleOverlay {
    f32 x;
    f32 y;
    f32 z;
    u8 pad0C[4];
    f32 speed;
    u8 pad14[0x10];
    u32 unk24;
    u8 pad28[0x24];
    u32 color;
    u8 pad50[8];
    u32 alpha;
    u8 pad5C[0x30];
    f32 lastSpeed;
    u8 pad90[4];
    f32 speedJitter;
    f32 angleJitter;
    u8 pad9C[0x54];
    u32 colorMask;
    u8 padF4[4];
    EffectBufferTail *buffer;
} EffParticleOverlay;

/* The single-particle clone is a 0x150-byte emitter owner; callbacks use
 * the specialized 0xFC prefix and leave the owner's remaining bytes opaque. */
typedef union EffParticleView {
    EffEmitterHead emitter;
    EffParticleOverlay particle;
} EffParticleView;

typedef char EffParticleOverlay_size_check[
    sizeof(EffParticleOverlay) == 0xFC ? 1 : -1];
typedef char EffParticleOverlay_x_offset_check[
    (u32)&((EffParticleOverlay *)0)->x == 0x00 ? 1 : -1];
typedef char EffParticleOverlay_y_offset_check[
    (u32)&((EffParticleOverlay *)0)->y == 0x04 ? 1 : -1];
typedef char EffParticleOverlay_z_offset_check[
    (u32)&((EffParticleOverlay *)0)->z == 0x08 ? 1 : -1];
typedef char EffParticleOverlay_speed_offset_check[
    (u32)&((EffParticleOverlay *)0)->speed == 0x10 ? 1 : -1];
typedef char EffParticleOverlay_unk24_offset_check[
    (u32)&((EffParticleOverlay *)0)->unk24 == 0x24 ? 1 : -1];
typedef char EffParticleOverlay_color_offset_check[
    (u32)&((EffParticleOverlay *)0)->color == 0x4C ? 1 : -1];
typedef char EffParticleOverlay_alpha_offset_check[
    (u32)&((EffParticleOverlay *)0)->alpha == 0x58 ? 1 : -1];
typedef char EffParticleOverlay_lastSpeed_offset_check[
    (u32)&((EffParticleOverlay *)0)->lastSpeed == 0x8C ? 1 : -1];
typedef char EffParticleOverlay_speedJitter_offset_check[
    (u32)&((EffParticleOverlay *)0)->speedJitter == 0x94 ? 1 : -1];
typedef char EffParticleOverlay_angleJitter_offset_check[
    (u32)&((EffParticleOverlay *)0)->angleJitter == 0x98 ? 1 : -1];
typedef char EffParticleOverlay_colorMask_offset_check[
    (u32)&((EffParticleOverlay *)0)->colorMask == 0xF0 ? 1 : -1];
typedef char EffParticleOverlay_buffer_offset_check[
    (u32)&((EffParticleOverlay *)0)->buffer == 0xF8 ? 1 : -1];
typedef char EffParticleView_emitter_offset_check[
    (u32)&((EffParticleView *)0)->emitter == 0 ? 1 : -1];
typedef char EffParticleView_particle_offset_check[
    (u32)&((EffParticleView *)0)->particle == 0 ? 1 : -1];
typedef char EffParticleView_size_check[
    sizeof(EffParticleView) == 0x150 ? 1 : -1];

#endif
