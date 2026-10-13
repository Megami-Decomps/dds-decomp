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

u32 func_00184440(void) {
    return 0;
}

/* Kind 88's registered factory returns zero; its semantic effect class is unknown. */
u32 effPcpKind88CreateDisabled(void) {
    return 0;
}

/* Kind 88's registered duplicate operation returns zero. */
u32 effPcpKind88DuplicateDisabled(void) {
    return 0;
}

/* Kind 88's registered release operation does nothing. */
void effPcpKind88ReleaseNoOp(void) {
}

/* Kind 88's registered per-work dispatch does nothing. */
void effPcpKind88DispatchNoOp(void) {
}

/* Kind 88's optional scale operation is present but does nothing. */
void effPcpKind88ScaleNoOp(void) {
}

/* Kind 88's optional value operation is present but does nothing. */
void effPcpKind88ValueNoOp(void) {
}

u32 func_00184478(void) {
    return 0;
}

/* Kind 95's registered factory returns zero; its semantic effect class is unknown. */
u32 effPcpKind95CreateDisabled(void) {
    return 0;
}

/* Kind 95's registered duplicate operation returns zero. */
u32 effPcpKind95DuplicateDisabled(void) {
    return 0;
}

/* Kind 95's registered release operation does nothing. */
void effPcpKind95ReleaseNoOp(void) {
}

/* Kind 95's registered per-work dispatch does nothing. */
void effPcpKind95DispatchNoOp(void) {
}

/* Kind 95's optional value operation is present but does nothing. */
void effPcpKind95ValueNoOp(void) {
}

u32 func_001844A8(void) {
    return 0;
}

void func_001844B0(void) {
}

void func_001844B8(void) {
}

s32 func_001844C0(void) {
    return 0;
}

s32 func_001844C8(void) {
    return 0;
}

void func_001844D0(void) {
}

void func_001844D8(void) {
}

s32 func_001844E0(void) {
    return 0;
}

s32 func_001844E8(void) {
    return 0;
}

void func_001844F0(void) {
}

void func_001844F8(void) {
}

void func_00184500(void) {
}

void func_00184508(void) {
}

s32 func_00184510(void) {
    return 0x18;
}

s32 func_00184518(void) {
    return 0;
}

void func_00184520(void) {
}

void func_00184528(void) {
}

s32 func_00184530(void) {
    return 0;
}
