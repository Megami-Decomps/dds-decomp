#ifndef SDF_NAMED_RESOURCE_REQUEST_API_H
#define SDF_NAMED_RESOURCE_REQUEST_API_H

#include "common.h"

/* Copy a nul-terminated name and optional payload into a named-resource RPC.
 * Returns the transport error or server status; on transport success, optional
 * outSize receives the server-reported response size. */
s32 sdfSendNamedResourceRequest(const char *name, s32 dataSize,
                                const void *data, s32 *outSize);

#endif /* SDF_NAMED_RESOURCE_REQUEST_API_H */
