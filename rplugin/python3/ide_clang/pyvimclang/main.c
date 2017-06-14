#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "ide.h"

#define LIBCLANG_PATH                                                          \
    "/Applications/Xcode.app/Contents/Developer/Toolchains/"                   \
    "XcodeDefault.xctoolchain/usr/lib/libclang.dylib"


static void print_completion(void* ctx, completion_t* completion)
{
    printf(
        "[%c] abbr: %s menu: %s word: %s\n",
        completion->kind,
        completion->abbr,
        completion->menu,
        completion->word);
}

int main(int argc, char const* argv[])
{
    if (argc != 4)
    {
        printf("%s\n", "usage: completer <filename> <line> <col>");
        return 1;
    }

    const char* path = argv[1];
    int line = atoi(argv[2]);
    int col = atoi(argv[3]);

    printf("%s\n", "initializing IDE  ...");
    const char* flags[1] = {"-Iext"};
    ide_t* ide = ide_alloc(LIBCLANG_PATH, flags, 1);
    if (!ide)
    {
        printf("%s\n", "initialize IDE  failed");
        return -1;
    }
    printf("%s\n", "initializing IDE  done");

    printf("open file %s ...\n", path);
    ide_on_file_open(ide, path);
    printf("open file %s done\n", path);

    printf("getting completions in location %d:%d ...\n", line, col);
    ide_find_completions(ide, path, line, col, NULL, &print_completion);
    printf("getting completions in location %d:%d done\n", line, col);

    return 0;
}