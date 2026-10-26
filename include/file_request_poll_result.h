#ifndef FILE_REQUEST_POLL_RESULT_H
#define FILE_REQUEST_POLL_RESULT_H

#include "common.h"

/* This reports completion of the card-info query, not the card's FileReqStatus. */
typedef s32 FileReqPollResult;
enum {
    FILE_REQ_POLL_PENDING = 0,
    FILE_REQ_POLL_COMPLETE = 1
};

#endif /* FILE_REQUEST_POLL_RESULT_H */
