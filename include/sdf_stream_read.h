#ifndef SDF_STREAM_READ_H
#define SDF_STREAM_READ_H

/* Operations passed through the SdfStreamRead callback contract. */
enum {
    SDF_STREAM_READ_QUERY = 0,
    SDF_STREAM_READ_COPY = 1,
    SDF_STREAM_READ_RESUME = 2
};

#endif /* SDF_STREAM_READ_H */
