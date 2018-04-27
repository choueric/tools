#pragma once

#include <dlfcn.h>
#include <stdlib.h>
#include <errno.h>

/*
 * 1. define a structure, e.g. VendorFuncs, which must has a member whose name
 *    is 'lib' and the type is void *. This lib is the library handle.
 *    Then this structure can contains other funtion pointers you want to load.
 *
 * 2. call GENERIC_LOAD_FUNC_PREAMBLE with arguments which are the  structure
 *    type 'VendorFuncs', pointer of pointer to the VendorFuncs variable and 
 *    the library name.
 *    This macro will dynamically allocate a VendorFuncs and set to the pointer.
 *    Then call dlopen to open the library.
 *
 * 3. Call LOAD_SYMBOL to load functions from library with arguments: functions
 *    name(func), function pointer type(tp) and the symbol name (symbol)
 *
 * 4. After loading all functions, call GENERIC_LOAD_FUNC_FINALE with the name
 *    of pointer.
 *
 * If all goes well, the allocated structure VendorFuncs will not be freed and 
 * dlclose will not be called.
 */

#define LOAD_LIBRARY(l, path)                                     \
    do {                                                          \
        if (!((l) = dlopen(path, RTLD_NOW))) {                   \
            printf("!! Cannot load %s (%s)\n", path, dlerror()); \
            ret = EAGAIN;                                \
            goto error;                                           \
        }                                                         \
        printf(">> Loaded lib: %s\n", path);     \
    } while (0)

#define GENERIC_LOAD_FUNC_PREAMBLE(T, p, N)  \
    T *f;                                    \
    int ret = 0;                             \
                                             \
    f = *p = malloc(sizeof(*f)); \
    if (!f)                                  \
        return ENOMEM;                       \
                                             \
    LOAD_LIBRARY(f->lib, N);

#define LOAD_SYMBOL(fun, tp, symbol)                                \
    do {                                                            \
        if (!((f->fun) = (tp*)dlsym(f->lib, symbol))) {             \
            printf("!! Cannot load %s(%s)\n", symbol, dlerror());   \
            ret = EAGAIN;                                  \
            goto error;                                             \
        }                                                           \
        printf(">> Loaded sym: %s\n", symbol);     \
    } while (0)

#define GENERIC_FREE_FUNC(p)              \
    if (!*p)                      \
        return ret;                          \
    if (*p && (*p)->lib) \
        dlclose((*p)->lib);      \
    free(*p);

#define GENERIC_LOAD_FUNC_FINALE(p) \
    return 0;                       \
error:                              \
    GENERIC_FREE_FUNC(p);            \
    return ret;

