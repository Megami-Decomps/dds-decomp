#ifndef SDF_COUNTER_RUNTIME_API_H
#define SDF_COUNTER_RUNTIME_API_H

struct SdfCounterRuntime;

extern struct SdfCounterRuntime *sdfActiveCounterRuntime;
void sdfCounterDestroyRuntime(struct SdfCounterRuntime *runtime);

#endif
