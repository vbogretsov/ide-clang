import datetime
import pyvimclang

LIBCLANG_PATH = r"/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/libclang.dylib"

FILE = "/Users/vova/projects/deoplete-clang/samples/main.cpp"

CONTENT = """
#include <iostream>
#include <vector>
#include <string>

class Person
{
public:
    char* Name;
    int Age;
};

struct Completion
{
    std::string s1;
    std::string s2;
};


void test_func(int n1, int n2, int n3)
{
    int k = n1 + n2 + n3;
}


void ShowPerson(Person& person)
{
}

int main(int argc, char const* argv[])
{
    Person person;
    per
    return 0;
}

"""

ide = pyvimclang.Ide(LIBCLANG_PATH, ["-I/Users/vova/ports/usr/include"])

t0 = datetime.datetime.now()
ide.on_file_open(FILE)
t1 = datetime.datetime.now()
print("parsed in", t1 - t0)

t0 = datetime.datetime.now()
completions = ide.find_completions(FILE, 33, 5, CONTENT)
t1 = datetime.datetime.now()
print("completed in", t1 - t0)

for c in completions:
    print(c)

ide.on_file_save(FILE)

# t0 = datetime.datetime.now()
# completions = ide.find_completions(FILE, 17, 7, CONTENT)
# t1 = datetime.datetime.now()
# print("completed in", t1 - t0)

# for c in completions:
#     print(c)

ide.on_file_close(FILE)
