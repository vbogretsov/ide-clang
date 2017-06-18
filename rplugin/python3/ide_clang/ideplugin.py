import ide
from ide_clang import pyvimclang


TRIGGERS = r"((::)|(\.)|(->))$"


class ClangIde(ide.Plugin):

    filetypes = ("c", "cpp")

    def __init__(self, nvim):
        super().__init__(nvim)
        self.ide = pyvimclang.Ide(nvim.eval("g:ide_clang_libclang"))
        self.triggers = TRIGGERS

    def on_file_open(self, filename):
        self.ide.on_file_open(filename)
        self.nvim.command(":echo 'parsed'")

    def on_file_save(self, filename):
        self.ide.on_file_save(self, filename)

    def on_file_close(self, filename):
        self.ide.on_file_close(filename)

    def find_completions(self, filename, line, column, content):
        return self.ide.find_completions(
            filename,
            line,
            column,
            content)


IDE_PLUGIN = ClangIde
