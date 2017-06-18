/**
 * Interface for IDE C/C++ plugin based on libclang.
 *
 * Jun 7 2017 Vladimir Bogretsov <bogrecov@gmail.com>
 */

#ifndef IDE_H
#define IDE_H

#define ABBR_SIZE 128
#define MENU_SIZE 128  // TODO: remove menu member.
#define SORT_SIZE 128
#define WORD_SIZE 128
#define INFO_SIZE 1024


typedef struct ide ide_t;

typedef struct
{
    char abbr[ABBR_SIZE];
    char menu[MENU_SIZE];  // TODO: remove menu member.
    char sort[SORT_SIZE];
    char word[WORD_SIZE];
    char info[INFO_SIZE];
    char kind;
    unsigned priority;
} completion_t;

typedef struct
{
    const char* filename;
    unsigned line;
    unsigned column;
} location_t;

/**
 * Allocate and initialize new ide instance.
 * @param  libclang_path Path to libclang library.
 * @param  flags         Compiler flags.
 * @param  nflags        Number of compiler flags.
 * @return               Initialized ide instance.
 */
ide_t* ide_alloc(
    const char* libclang_path,
    const char* const* flags,
    unsigned nflags);

/**
 * Deallocate the ide instance provided.
 * @param ide Instance to be deallocated.
 */
void ide_free(ide_t* ide);

/**
 * Notify IDE about opening a file.
 * @param ide      IDE instance.
 * @param filename Opened file name.
 */
void ide_on_file_open(ide_t* ide, const char* filename);

/**
 * Notifgy IDE about file save.
 * @param ide      IDE instance.
 * @param filename Saved file name.
 */
void ide_on_file_save(ide_t* ide, const char* filename);

/**
 * Notify IDE about closing a file.
 * @param ide      IDE instance.
 * @param filename Closed file name.
 */
void ide_on_file_close(ide_t* ide, const char* filename);

/**
 * Find completions for the position in the file.
 * @param ide         IDE instance.
 * @param filename    File where completions deisred.
 * @param line        Line number where completions desired.
 * @param column      Column number where completions desired.
 * @param content     Content of the file.
 * @param size        Content size.
 * @param ctx         Enclosure context.
 * @param ncompletion Single completion handler.
 */
void ide_find_completions(
    ide_t* ide,
    const char* filename,
    unsigned line,
    unsigned column,
    const char* content,
    unsigned size,
    void* ctx,
    void (*oncompletion)(void*, completion_t*));

/**
 * Find symbol declaration.
 * @param ide          IDE instance.
 * @param filename     File where symbol desired is located.
 * @param line         Line number where symbol desired is located.
 * @param column       Column number where symbol desired is located.
 * @param ctx          Enclosure context.
 * @param ondefinition Definition handler.
 */
void ide_find_definition(
    ide_t* ide,
    const char* filename,
    unsigned line,
    unsigned column,
    void* ctx,
    void (*ondefinition)(void*, location_t*));

/**
 * Find symbol declaration.
 * @param ide          IDE instance.
 * @param filename     File where symbol desired is located.
 * @param line         Line number where symbol desired is located.
 * @param column       Column number where symbol desired is located.
 * @param ctx          Enclosure context.
 * @param ondefinition Single declaration handler.
 */
void ide_find_declaration(
    ide_t* ide,
    const char* filename,
    unsigned line,
    unsigned column,
    void* ctx,
    void (*ondeclaration)(void*, location_t*));

/**
 * Find symbol assingment.
 * @param ide          IDE instance.
 * @param filename     File where symbol desired is located.
 * @param line         Line number where symbol desired is located.
 * @param column       Column number where symbol desired is located.
 * @param ctx          Enclosure context.
 * @param onassingment Single assingment handler.
 */
void ide_find_assingments(
    ide_t* ide,
    const char* filename,
    unsigned line,
    unsigned column,
    void* ctx,
    void (*onassingment)(void*, location_t*));

/**
 * Find symbol reference.
 * @param ide          IDE instance.
 * @param filename     File where symbol desired is located.
 * @param line         Line number where symbol desired is located.
 * @param column       Column number where symbol desired is located.
 * @param ctx          Enclosure context.
 * @param onreference  Single reference handler.
 */
void ide_find_references(
    ide_t* ide,
    const char* filename,
    unsigned line,
    unsigned column,
    void* ctx,
    void (*onreference)(void*, location_t*));


#endif // !IDE_H