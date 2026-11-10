#ifndef EFF_EVENT_DRAW_H
#define EFF_EVENT_DRAW_H

#include "eff_blur.h"


effScreenDrawParams_t *evtGetUnityCalcArea(void);
effScreenDrawParams_t *effGetLoadDescD(void);
effScreenDrawParams_t *Eve_BlurGetParam(void);
effScreenDrawParams_t *effGetCh73Params(void);
dds3Effect_Filter_t *Eve_FilterGetParams(void);
dds3Effect_Filter_t *dblurGetParam2(void);
void dblurSetParam2(dds3Effect_Filter_t *parameters);
void Eve_FilterSetParam(dds3Effect_Filter_t *parameters);

/* Enable or disable each event-effect draw family. */
void Eve_BlurON(void);
void Eve_BlurOFF(void);
void Eve_PartialMultiBlurON(void);
void Eve_PartialMultiBlurOFF(void);
void Eve_DistortionBlurON(void);
void Eve_DistortionBlurOFF(void);
void Eve_RippleBlurON(void);
void Eve_RippleBlurOFF(void);
void effEnableFramebufferQuad(void);
void effDisableFramebufferQuad(void);
void Eve_FilterON(void);
void Eve_FilterOFF(void);
void Eve_PartialMultiFilterON(void);
void Eve_PartialMultiFilterOFF(void);

#endif
