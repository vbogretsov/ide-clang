import logging

import ide

from ide_clang import pyvimclang


LOG = logging.getLogger("ide_clang")

FILE_HANDLER = logging.FileHandler("/tmp/ide_clang.log")
FILE_HANDLER.setLevel(logging.DEBUG)

LOG.addHandler(FILE_HANDLER)
LOG.setLevel(logging.DEBUG)

LIG_CLANG_PATH = "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/libclang.dylib"


class ClangIde(ide.Plugin):

    filetypes = ("c", "cpp")

    def __init__(self, nvim):
        super().__init__(nvim)
        self.ide = pyvimclang.Ide(LIG_CLANG_PATH)
        self.triggers = "((\s+)|(\t+)|(\w+\.)|(\=\s*)|(\w+->)|(\w+::)|(^))$"

    def on_file_open(self, filename):
        self.ide.on_file_open(filename)
        self.nvim.command(":echo 'file parsed'")

    def on_file_save(self, filename):
        self.ide.on_file_save(self, filename)

    def on_file_close(self, filename):
        self.ide.on_file_close(filename)

    def find_completions(self, filename, line, column, content):
        LOG.debug("getting completions for [%d, %d] in\n%s", line, column, content)
        completions = self.ide.find_completions(
            filename,
            line,
            column,
            content)
        LOG.debug("got %d completions", len(completions))
        return completions


IDE_PLUGIN = ClangIde
