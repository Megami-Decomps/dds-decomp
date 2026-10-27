#ifndef EVT_POLYGON_MOVIE_RESOURCE_MODE_H
#define EVT_POLYGON_MOVIE_RESOURCE_MODE_H

/* Both modes load PMD1. File mode loads PMD2 and uses PMD3 when present; the
 * viewer reload wrapper requires PMD2 in this mode. Synthesize mode creates
 * auxiliary PMD2 and PMD3 headers. */
typedef enum EvtPolygonMovieResourceMode {
    EVT_POLYGON_MOVIE_RESOURCE_MODE_LOAD_FILES = 0,
    EVT_POLYGON_MOVIE_RESOURCE_MODE_SYNTHESIZE = 1,
} EvtPolygonMovieResourceMode;

typedef char EvtPolygonMovieResourceMode_size_must_be_4[
    (sizeof(EvtPolygonMovieResourceMode) == 4) ? 1 : -1];

#endif
