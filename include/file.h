#ifndef FILE_H
#define FILE_H

#include "common.h"

/* File manager job states, request entry size and transfer chunk. */
#define FILE_JOB_READY 3
#define FILE_JOB_TRANSFERRING 4
#define FILE_REQ_WORDS_PER_ENTRY 0x19
#define FILE_IO_MAX_CHUNK_BYTES 0x8000

#endif /* FILE_H */
