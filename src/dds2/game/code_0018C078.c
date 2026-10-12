#include "common.h"
#include "pcp_vu0.h"

/* Partial PCP event-work view; effPCPMisc's owner confirms scale and color. */
typedef struct {
    u8 pad00[0x2C];
    f32 scale;   /* 0x2C */
    u8 pad30[0xC];
    u32 color;  /* 0x3C */
} EffPCPEventWorkFields;

/* vu0 routine: copy the PCP event-work position vector, including its fourth lane. */
void effPcpEventWorkSetPosition(void *work, void *position) {
    PCP_COPY_VECTOR(work, position);
}

/* Set the scale consumed by the PCP map-event update. */
void effPcpEventWorkSetScale(EffPCPEventWorkFields *work, f32 scale) {
    work->scale = scale;
}

/* Set the packed color used to tint the map event's children. */
void effPcpEventWorkSetColor(EffPCPEventWorkFields *work, u32 color) {
    work->color = color;
}

u32 func_0018C098(void) {
    return 0;
}

u32 func_0018C0A0(void) {
    return 0;
}

u32 func_0018C0A8(void) {
    return 0;
}

void func_0018C0B0(void) {
}

void func_0018C0B8(void) {
}

void func_0018C0C0(void) {
}

void func_0018C0C8(void) {
}

u32 func_0018C0D0(void) {
    return 0;
}

u32 func_0018C0D8(void) {
    return 0;
}

u32 func_0018C0E0(void) {
    return 0;
}

void func_0018C0E8(void) {
}

void func_0018C0F0(void) {
}

void func_0018C0F8(void) {
}

u32 func_0018C100(void) {
    return 0;
}

void func_0018C108(void) {
}

void func_0018C110(void) {
}

u32 func_0018C118(void) {
    return 0;
}

u32 func_0018C120(void) {
    return 0;
}

void func_0018C128(void) {
}

void func_0018C130(void) {
}

u32 func_0018C138(void) {
    return 0;
}

u32 func_0018C140(void) {
    return 0;
}

void func_0018C148(void) {
}

void func_0018C150(void) {
}

void func_0018C158(void) {
}

void func_0018C160(void) {
}

u32 func_0018C168(void) {
    return 0x18;
}

u32 func_0018C170(void) {
    return 0;
}

void func_0018C178(void) {
}

void func_0018C180(void) {
}

u32 func_0018C188(void) {
    return 0;
}
