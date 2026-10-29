#ifndef EFF_EVENT_SETUP_READINESS_H
#define EFF_EVENT_SETUP_READINESS_H

#include "common.h"

/* Setup remains ready after processing until its control byte resets it. */
typedef s8 EffEventSetupReadiness;
enum {
    EFF_EVENT_SETUP_NOT_READY = 0,
    EFF_EVENT_SETUP_READY = 1
};

EffEventSetupReadiness effUpdateCh72Params(void);
EffEventSetupReadiness effEventAdvanceBlurTemplateSetup(void);
EffEventSetupReadiness effEventAdvanceScatterBlurSetup(void);
EffEventSetupReadiness effEventAdvanceScreenDrawSetup(void);
EffEventSetupReadiness evtViewerCheckDirty(void);
EffEventSetupReadiness mfilterRun(void);
EffEventSetupReadiness rblurProcess(void);

#endif /* EFF_EVENT_SETUP_READINESS_H */
