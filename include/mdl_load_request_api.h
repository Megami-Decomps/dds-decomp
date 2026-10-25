#ifndef MDL_LOAD_REQUEST_API_H
#define MDL_LOAD_REQUEST_API_H

#include "mdl.h"
#include "mdl_registration_mode.h"

static inline MdlRegistrationMode mdlLoadRequestGetRegistrationMode(
    const MdlLoadRequest *request) {
    return request->options;
}

static inline void mdlLoadRequestSetRegistrationMode(
    MdlLoadRequest *request, MdlRegistrationMode registrationMode) {
    request->options = registrationMode;
}

#endif /* MDL_LOAD_REQUEST_API_H */
