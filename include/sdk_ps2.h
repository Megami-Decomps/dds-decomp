#ifndef SDK_PS2_H
#define SDK_PS2_H

/* PS2 SDK entry points the game links, one canonical prototype each.
 *
 * Every prototype sits in an `#ifndef SDK_SKIP_<name>` guard. A unit whose retail code
 * depends on a different declaration of the same function (a K&R `void memcpy();` or an
 * `s32` size parameter drops gcc 2.96's builtin expansion; a declaration placed after the first
 * use leaves earlier calls implicitly declared; `__attribute__((malloc))`) defines the
 * guard before including sdk_ps2.h and keeps its own declaration. See docs/idioms.md,
 * "memset as a builtin or a library call". */

#include "sdk_libkernl.h"
#include "sdk_libc.h"
#include "sdk_libm.h"
#include "sdk_libcdvd.h"
#include "sdk_libdma.h"
#include "sdk_libgraph.h"
#include "sdk_libipu.h"
#include "sdk_libpad.h"
#include "sdk_libmc.h"
#include "sdk_libsdr.h"

#endif
