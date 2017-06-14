/**
 * Multiplatform (POSIX.1 + WinddowsNT) library for import functions from a
 * shared library.
 * 
 * May 30 2017 Vladimir Bogretsov <bogrecov@gmail.com>
 */

#ifndef IMPORT_H
#define IMPORT_H

#if defined(__GNUC__) || (defined(__APPLE__) && defined(__MACH__)) // POSIX
#define IMPORT_POSIX
#include <dlfcn.h>
#else // WinddowsNT
#define IMPORT_WINDOWS
#include <Windows.h>
#endif

/**
 * Load library from the path provided. If the function returns 0, see errno
 * to get the exact error code.
 *
 * @param path Lilbrary file name.
 * @return     Pointer to library if it was loaded, otherwise NULL.
 */
void* load_library(const char* path)
{
#ifdef IMPORT_POSIX
    return dlopen(path, RTLD_NOW | RTLD_LOCAL);
#else
    // not implemented yet
    return 0;
#endif
}

/**
 * Close the library provided.
 */
void close_library(void* library)
{
#ifdef IMPORT_POSIX
    dlclose(library);
#else
    // not implemented yet
#endif
}

/**
 * Import name from the library provided. If the function returns 0, see errno
 * to get the exact error code.
 *
 * @param library 
 * @param name    Name to be exported.
 * @return        Pointer to library if it was loaded, otherwise NULL.
 */
void* import_name(void* library, const char* name)
{
#ifdef IMPORT_POSIX
    return dlsym(library, name);
#else
    // not implemented yet
    return 0;
#endif
}

#endif // !IMPORT_H