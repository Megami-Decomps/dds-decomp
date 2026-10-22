#ifndef MNU_MOVIE_LIST_H
#define MNU_MOVIE_LIST_H

#include "common.h"
#include "mnu_movie_list_node.h"

struct sdfDrawTag_t;
struct dds3ProcessID_t;

/* Movie-viewer list state; the task, movie-list node and packet-list owners
 * are external. */
typedef struct MnuMovieList {
    struct dds3ProcessID_t *task;
    MnuMovieListNode *head;
    s16 top;
    s16 cursor;
    s16 total;
    s8 playing;
    u8 pad0F;
    struct sdfDrawTag_t *packets;
} MnuMovieList;

typedef char MnuMovieList_size_must_be_0x14[
    (sizeof(MnuMovieList) == 0x14) ? 1 : -1];
typedef char MnuMovieList_head_must_be_at_0x04[
    ((u32)&((MnuMovieList *)0)->head == 0x04) ? 1 : -1];
typedef char MnuMovieList_playing_must_be_at_0x0E[
    ((u32)&((MnuMovieList *)0)->playing == 0x0E) ? 1 : -1];
typedef char MnuMovieList_packets_must_be_at_0x10[
    ((u32)&((MnuMovieList *)0)->packets == 0x10) ? 1 : -1];

#endif /* MNU_MOVIE_LIST_H */
