#ifndef FILE_PROMPT_REVEAL_RESULT_H
#define FILE_PROMPT_REVEAL_RESULT_H

#include "common.h"

/* Prompt text stays hidden during the panel reveal, then becomes ready. */
typedef s32 FilePromptRevealResult;

enum {
    FILE_PROMPT_REVEAL_PENDING = 0,
    FILE_PROMPT_REVEAL_READY = 1,
};

#endif /* FILE_PROMPT_REVEAL_RESULT_H */
