#include "libclang.h"

#include <errno.h>
#include <stdlib.h>

#include "import.h"


static void* load_function(void* lib, const char* name, int* num_not_loaded)
{
    void* ptr = import_name(lib, name);
    if (!ptr)
    {
        ++*num_not_loaded;
    }
    return ptr;
}

// TODO: more details for error.
libclang_t* libclang_load(const char* path)
{
    void* handle = load_library(path);

    if (!handle)
    {
        errno = ENOENT;
        return NULL;
    }

    libclang_t* libclang = (libclang_t*)malloc(sizeof(libclang_t));

    int num_not_loaded = 0;

    libclang->create_index = (clang_create_index_t)load_function(
        handle, "clang_createIndex", &num_not_loaded);

    libclang->dispose_index = (clang_dispose_index_t)load_function(
        handle, "clang_disposeIndex", &num_not_loaded);

    libclang->parse_tu = (clang_parse_tu_t)load_function(
        handle, "clang_parseTranslationUnit", &num_not_loaded);

    libclang->reparse_tu = (clang_reparse_tu_t)load_function(
        handle, "clang_reparseTranslationUnit", &num_not_loaded);

    libclang->dispose_tu = (clang_dispose_tu_t)load_function(
        handle, "clang_disposeTranslationUnit", &num_not_loaded);

    libclang->complete_at = (clang_complete_at_t)load_function(
        handle, "clang_codeCompleteAt", &num_not_loaded);

    libclang->dispose_completion = (clang_dsipose_completion_t)load_function(
        handle, "clang_disposeCodeCompleteResults", &num_not_loaded);

    libclang->get_string = (clang_get_cstring_t)load_function(
        handle, "clang_getCString", &num_not_loaded);

    libclang->dispose_string = (clang_dispose_string_t)load_function(
        handle, "clang_disposeString", &num_not_loaded);

    libclang->get_completion_priority =
        (clang_get_completion_priority_t)load_function(
            handle, "clang_getCompletionPriority", &num_not_loaded);

    libclang->get_completion_brief_comment =
        (clang_get_completion_brief_comment_t)load_function(
            handle, "clang_getCompletionBriefComment", &num_not_loaded);

    libclang->get_num_completion_chunks =
        (clang_get_num_completion_chunks_t)load_function(
            handle, "clang_getNumCompletionChunks", &num_not_loaded);

    libclang->get_completion_chunk_text =
        (clang_get_completion_chunk_text_t)load_function(
            handle, "clang_getCompletionChunkText", &num_not_loaded);

    libclang->get_completion_chunk_kind =
        (clang_get_completion_chunk_kind_t)load_function(
            handle, "clang_getCompletionChunkKind", &num_not_loaded);

    libclang->default_code_complete_options =
        (clang_default_code_complete_options_t)load_function(
            handle, "clang_defaultCodeCompleteOptions", &num_not_loaded);

    if (num_not_loaded)
    {
        close_library(libclang);
        errno = EBADF;
        return NULL;
    }

    return libclang;
}

void libclang_close(libclang_t* libclang)
{
    close_library(libclang->handle);
}
