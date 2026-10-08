#ifndef MNU_MOVIE_TRANSFER_H
#define MNU_MOVIE_TRANSFER_H

#include "common.h"

/* Shared scratch state used by the movie draw task to inspect an IPU word
 * and a 0x40-byte block. The source fields are transported address words. */
typedef struct MnuMovieTransfer {
    u8 started;
    u8 cursor;
    u8 wordPending;
    u8 blockPending;
    u32 wordSource;
    u32 blockSource;
    s32 word;
    u8 block[0x40];
} MnuMovieTransfer;

typedef char MnuMovieTransfer_size_must_be_0x50[
    (sizeof(MnuMovieTransfer) == 0x50) ? 1 : -1];
typedef char MnuMovieTransfer_wordSource_must_be_at_0x04[
    ((u32)&((MnuMovieTransfer *)0)->wordSource == 0x04) ? 1 : -1];
typedef char MnuMovieTransfer_blockSource_must_be_at_0x08[
    ((u32)&((MnuMovieTransfer *)0)->blockSource == 0x08) ? 1 : -1];
typedef char MnuMovieTransfer_block_must_be_at_0x10[
    ((u32)&((MnuMovieTransfer *)0)->block == 0x10) ? 1 : -1];

#endif /* MNU_MOVIE_TRANSFER_H */
