#ifndef EFF_PCP_VECTOR_COPY_API_H
#define EFF_PCP_VECTOR_COPY_API_H

/* Shared effect PCP callback contract: paired DDS1/DDS2 providers receive
 * destination work/storage first and the source parameter vector second. */
void effPcpViewAlignedRingSetPosition(void *dst, void *src);
void effPcpCopyTwinVector(void *dst, void *src);
void effPcpCopyStaggerVector(void *dst, void *src);
void effPcpCopyCrossVector(void *dst, void *src);
void effPcpCopyDelayedPairVector(void *dst, void *src);
void effPcpCopyVectorAF0(void *work, void *src);
void effPcpCopySpawnOnceVector(void *dst, void *src);
void effPcpCopyRadialParticleVector(void *dst, void *src);
void effPcpCopyNarrowConeVector(void *dst, void *src);
void effPcpCopyVariableHeightVector(void *dst, void *src);
void effPcpCopyWideConeVector(void *dst, void *src);
void effPcpCopyFixedOriginVector(void *dst, void *src);
void effPcpCopySideBurstVector(void *dst, void *src);
void effPcpCopyThunderHandleVector(void *dst, void *src);
void effPcpCopyDenseConeVector(void *dst, void *src);
void effPcpCopyGrowingThunderFadeVector(void *dst, void *src);
void effPcpCopySharedTrailVector(void *dst, void *src);
void effPcpCopyCompactResourceRectVector(void *dst, void *src);
void effPcpCopyCompactTexturedBlurVector(void *dst, void *src);
void effPcpCopyCompactBlurVector(void *dst, void *src);
void effPcpCopyShortThunderFadeVector(void *dst, void *src);
void effPcpCopyScalingThunderFadeVector(void *dst, void *src);
void effPcpCopyThunderScaleFadeVector(void *dst, void *src);
void effPcpCopySpanVector(void *dst, void *src);
void effPcpCopySpinSingleVector(void *dst, void *src);
void effPcpCopySpinPairVector(void *dst, void *src);
void effPcpCopySpinningPairVector(void *dst, void *src);
void effPcpCopyThunderBurstVector(void *dst, void *src);
void effPcpCopyScaledThunderVector(void *dst, void *src);
void effPcpCopyBeamVector(void *dst, void *src);
void effPcpCopyLinkedBeamVector(void *dst, void *src);
void effPcpCopyGroupSetVector(void *dst, void *src);
void effPcpCopyCaptureNodeVector(void *dst, void *src);
void effPcpCopyDriftVector(void *dst, void *src);
void effPcpCopyRandomizedVector(void *dst, void *src);
void effPcpCopySpawnRangeVector(void *dst, void *src);

#endif /* EFF_PCP_VECTOR_COPY_API_H */
