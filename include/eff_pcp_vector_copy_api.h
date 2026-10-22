#ifndef EFF_PCP_VECTOR_COPY_API_H
#define EFF_PCP_VECTOR_COPY_API_H

/* Shared effect PCP callback contract: paired DDS1/DDS2 providers receive
 * destination work/storage first and the source parameter vector second. */
void effPCPSparkleSphereSetPosition(void *dst, void *src);
void effPCPHama01SetPosition(void *dst, void *src);
void effPCPHama02SetPosition(void *dst, void *src);
void effPCPHamaonSetPosition(void *dst, void *src);
void effPCPMHanma01SetPosition(void *dst, void *src);
void effPCPMudoSetPosition(void *work, void *src);
void effPCPHwave01SetPosition(void *dst, void *src);
void effPCPZiodin01SetPosition(void *dst, void *src);
void effPCPZiodin02SetPosition(void *dst, void *src);
void effPCPZiodin03SetPosition(void *dst, void *src);
void effPCPZiodin04SetPosition(void *dst, void *src);
void effPCPMZiodin01SetPosition(void *dst, void *src);
void effPCPMZiodin02SetPosition(void *dst, void *src);
void effPCPShockWave01SetPosition(void *dst, void *src);
void effPCPShockWave02SetPosition(void *dst, void *src);
void effPCPHouden01SetPosition(void *dst, void *src);
void effPCPMultiBlurExpSetPosition(void *dst, void *src);
void effPCPMultiFilterExpSetPosition(void *dst, void *src);
void effPCPMultiBlurExp2SetPosition(void *dst, void *src);
void effPCPDistortionBlurExpSetPosition(void *dst, void *src);
void effPCPRippleBlurExpSetPosition(void *dst, void *src);
void effPCPMegidora01SetPosition(void *dst, void *src);
void effPCPMegidoraon01SetPosition(void *dst, void *src);
void effPCPAgiraoFireSetPosition(void *dst, void *src);
void effPCPBufuIceSetPosition(void *dst, void *src);
void effPCPBufuraIceSetPosition(void *dst, void *src);
void effPCPBufudnIceSetPosition(void *dst, void *src);
void effPCPEyeThunderSetPosition(void *dst, void *src);
void effPCPThunderSphereExpSetPosition(void *dst, void *src);
void effPCPRingPolyExpSetPosition(void *dst, void *src);
void effPCPRingPolyExp2SetPosition(void *dst, void *src);
void effPCPRadiateThunderSetPosition(void *dst, void *src);
void effPCPMakkaSetPosition(void *dst, void *src);
void effPCPRadiateBEDSetPosition(void *dst, void *src);
void effPCPRadiateTBSetPosition(void *dst, void *src);
void effPCPSpiralBEDSetPosition(void *dst, void *src);

#endif /* EFF_PCP_VECTOR_COPY_API_H */
