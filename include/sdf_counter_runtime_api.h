#ifndef SDF_COUNTER_RUNTIME_API_H
#define SDF_COUNTER_RUNTIME_API_H

struct MenuList;

extern struct MenuList *sdfActiveCounterRuntime;
void sdfCounterDestroyRuntime(struct MenuList *runtime);

#endif
