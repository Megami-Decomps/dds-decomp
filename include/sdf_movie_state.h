#ifndef SDF_MOVIE_STATE_H
#define SDF_MOVIE_STATE_H

#include "common.h"

struct sdfFileCmdHandle_t;
struct movieStream_t;

/* Values stored in movieStream_t.state. State 6 is entered by the device-release
 * callback and observed by the owner while it waits for shutdown. */
enum {
    SDF_MOVIE_STATE_INITIAL = 0,
    SDF_MOVIE_STATE_CONTROL_REQUEST = 1,
    SDF_MOVIE_STATE_PAC_HEADER_READ = 2,
    SDF_MOVIE_STATE_PAC_MASK_READ = 3,
    SDF_MOVIE_STATE_DATA_READ = 4,
    SDF_MOVIE_STATE_WAITING_FOR_BUFFER_SPACE = 5,
    SDF_MOVIE_STATE_DEVICE_RELEASE_CALLBACK = 6,
    SDF_MOVIE_STATE_STOP_REQUESTED = 7
};

/* Completion event passed by the device layer to movie callbacks. */
typedef s32 SdfMovieDeviceEvent;

s32 movieGetStreamSize(
    struct sdfFileCmdHandle_t *deviceState, SdfMovieDeviceEvent deviceEvent,
    void *data, s32 bytesRead, struct movieStream_t *movie);
s32 movieReadChunk(
    struct sdfFileCmdHandle_t *deviceState, SdfMovieDeviceEvent deviceEvent,
    const void *readBuffer, s32 bytesRead, struct movieStream_t *movie);

#endif /* SDF_MOVIE_STATE_H */
