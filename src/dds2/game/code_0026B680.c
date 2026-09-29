#include "common.h"

/* Pass the object's +0x60 word to the second operation after setup. */
void func_0026B680(s32 objectAddress) {
    func_0026C728();
    func_0026C538(*(u32 *)(objectAddress + 0x60));
}
