#ifndef EVT_POLYGON_MOVIE_FLAGS_API_H
#define EVT_POLYGON_MOVIE_FLAGS_API_H

#include "common.h"

struct dds3ProcessID_t;

/* Script start holds viewer setup until the script releases this flag. */
typedef u32 EvtPolygonMovieWorkFlags;
enum {
    EVT_POLY_MOVIE_SCRIPT_STARTUP_HOLD = 0x01
};

void evtPolygonMovieSetFlagBits(struct dds3ProcessID_t *task,
                                EvtPolygonMovieWorkFlags flags);
void evtPolygonMovieClearFlagBits(struct dds3ProcessID_t *task,
                                  EvtPolygonMovieWorkFlags flags);

#endif /* EVT_POLYGON_MOVIE_FLAGS_API_H */
