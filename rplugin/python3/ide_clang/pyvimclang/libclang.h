/**
 * Interface for libclang loaded dynamically.
 *
 * Jun 7 2017 Vladimir Bogretsov <bogrecov@gmail.com>
 */

#ifndef LIBCLANG_H
#define LIBCLANG_H

#include <clang-c/Index.h>

/**
 * https://clang.llvm.org/doxygen/group__CINDEX.html#ga51eb9b38c18743bf2d824c6230e61f93
 */
typedef CXIndex (*clang_create_index_t)(int, int);

/**
 * https://clang.llvm.org/doxygen/group__CINDEX.html#ga166ab73b14be73cbdcae14d62dbab22a
 */
typedef void (*clang_dispose_index_t)(CXIndex);

/**
 * https://clang.llvm.org/doxygen/group__CINDEX__TRANSLATION__UNIT.html#ga2baf83f8c3299788234c8bce55e4472e
 */
typedef CXTranslationUnit (*clang_parse_tu_t)(
    CXIndex,
    const char*,
    const char* const*,
    int,
    struct CXUnsavedFile*,
    unsigned,
    unsigned);

/**
 * https://clang.llvm.org/doxygen/group__CINDEX__TRANSLATION__UNIT.html#ga524e76bf2a809d037934d4be51ea448a
 */
typedef int (*clang_reparse_tu_t)(
    CXTranslationUnit,
    unsigned,
    struct CXUnsavedFile*,
    unsigned);

/**
 * https://clang.llvm.org/doxygen/group__CINDEX__TRANSLATION__UNIT.html#gaee753cb0036ca4ab59e48e3dff5f530a
 */
typedef void (*clang_dispose_tu_t)(CXTranslationUnit);

/**
 * https://clang.llvm.org/doxygen/group__CINDEX__CODE__COMPLET.html#ga50fedfa85d8d1517363952f2e10aa3bf
 */
typedef CXCodeCompleteResults* (*clang_complete_at_t)(
    CXTranslationUnit,
    const char*,
    unsigned,
    unsigned,
    struct CXUnsavedFile*,
    unsigned,
    unsigned);

/**
 * https://clang.llvm.org/doxygen/group__CINDEX__CODE__COMPLET.html#ga206cc6ea7be311537bb0fab584ebc6c1
 */
typedef void (*clang_dsipose_completion_t)(CXCodeCompleteResults*);

/**
 * https://clang.llvm.org/doxygen/group__CINDEX__STRING.html#gafd043aa189e990b9e327e9f95a1da8a5
 */
typedef const char* (*clang_get_cstring_t)(CXString);

/**
 * https://clang.llvm.org/doxygen/group__CINDEX__STRING.html#gaeff715b329ded18188959fab3066048f
 */
typedef void (*clang_dispose_string_t)(CXString);

/**
 * https://clang.llvm.org/doxygen/group__CINDEX__CODE__COMPLET.html#ga46e843acdf63d9a7a0c7341a2d222c49
 */
typedef unsigned (*clang_get_completion_priority_t)(CXCompletionString);

/**
 * https://clang.llvm.org/doxygen/group__CINDEX__CODE__COMPLET.html#ga32163145c7f0013e5f2ac7176a8ee0ed
 */
typedef CXString (*clang_get_completion_brief_comment_t)(CXCompletionString);

/**
 * https://clang.llvm.org/doxygen/group__CINDEX__CODE__COMPLET.html#ga76018aa1a7225268546e4d75dca5dbce
 */
typedef unsigned (*clang_get_num_completion_chunks_t)(CXCompletionString);

/**
 * https://clang.llvm.org/doxygen/group__CINDEX__CODE__COMPLET.html#ga98d4c869dda8fd4b5386f62d02d6ba0b
 */
typedef CXString (*clang_get_completion_chunk_text_t)(
    CXCompletionString,
    unsigned);

/**
 * https://clang.llvm.org/doxygen/group__CINDEX__CODE__COMPLET.html#gac61e18c6d895d85f1476c6091d486091
 */
typedef enum CXCompletionChunkKind (*clang_get_completion_chunk_kind_t)(
    CXCompletionString,
    unsigned);

/**
 * https://clang.llvm.org/doxygen/group__CINDEX__CODE__COMPLET.html#gadb669685b9ef1f8ca62b2a044b846ac1
 */
typedef unsigned (*clang_default_code_complete_options_t)();
//clang_defaultCodeCompleteOptions


/**
 * Functions imported from libclang.
 */
typedef struct
{
    void* handle;
    clang_create_index_t create_index;
    clang_dispose_index_t dispose_index;
    clang_parse_tu_t parse_tu;
    clang_reparse_tu_t reparse_tu;
    clang_dispose_tu_t dispose_tu;
    clang_complete_at_t complete_at;
    clang_dsipose_completion_t dispose_completion;
    clang_get_cstring_t get_string;
    clang_dispose_string_t dispose_string;
    clang_get_completion_priority_t get_completion_priority;
    clang_get_completion_brief_comment_t get_completion_brief_comment;
    clang_get_num_completion_chunks_t get_num_completion_chunks;
    clang_get_completion_chunk_text_t get_completion_chunk_text;
    clang_get_completion_chunk_kind_t get_completion_chunk_kind;
    clang_default_code_complete_options_t default_code_complete_options;

} libclang_t;

/**
 * Load libclang shared library from the path provided.
 * @param path  Location of the libclang shared library.
 * @return      Pointer to shared library loaded.
 */
libclang_t* libclang_load(const char* path);

/**
 * Close the shared library libclang.
 */
void libclang_close(libclang_t* libclang);

#endif // !LIBCLANG_H