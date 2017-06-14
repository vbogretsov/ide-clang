# -*- coding:utf-8 -*-
import os

from distutils.core import setup, Extension


DESCRIPTION = """Contains objects and functions to interoperate with
synchronization primitives missing in stanard library.
"""

PREFIX = "pyvimclang"

main_module_kwargs = {
    "sources": [
        os.path.join(PREFIX, "hashmap.c"),
        os.path.join(PREFIX, "ide.c"),
        os.path.join(PREFIX, "libclang.c"),
        os.path.join(PREFIX, "pyvimclang.c")
    ],
    "include_dirs": [
        PREFIX
    ]
}

main = Extension("pyvimclang", **main_module_kwargs)

setup(
    name="pyvimclang",
    version="0.1.0",
    description=DESCRIPTION,
    author="Vladimir Bogretsov",
    author_email="bogrecov@gmail.com",
    ext_modules=[main],
    license="BSD")
