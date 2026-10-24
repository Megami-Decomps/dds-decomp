#ifndef EFF_PCP_VECTOR_COPY_API_H
#define EFF_PCP_VECTOR_COPY_API_H

#include "common.h"

struct EffPCPChargeWork;

/* Shared effect PCP callback contract: paired DDS1/DDS2 providers receive
 * destination work/storage first and the source parameter vector second. */
void effPCPSparkleSphereSetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPHama01SetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPHama02SetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPHamaonSetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPMHanma01SetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPMudoSetPosition(struct EffPCPChargeWork *work, const f32 sourcePosition[4]);
void effPCPHwave01SetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPZiodin01SetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPZiodin02SetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPZiodin03SetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPZiodin04SetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPMZiodin01SetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPMZiodin02SetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPShockWave01SetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPShockWave02SetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPHouden01SetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPMultiBlurExpSetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPMultiFilterExpSetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPMultiBlurExp2SetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPDistortionBlurExpSetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPRippleBlurExpSetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPMegidora01SetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPMegidoraon01SetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPAgiraoFireSetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPBufuIceSetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPBufuraIceSetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPBufudnIceSetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPEyeThunderSetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPThunderSphereExpSetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPRingPolyExpSetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPRingPolyExp2SetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPRadiateThunderSetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPMakkaSetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPRadiateBEDSetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPRadiateTBSetPosition(void *dst, const f32 sourcePosition[4]);
void effPCPSpiralBEDSetPosition(void *dst, const f32 sourcePosition[4]);

#endif /* EFF_PCP_VECTOR_COPY_API_H */
