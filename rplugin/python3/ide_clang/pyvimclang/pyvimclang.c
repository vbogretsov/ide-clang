/**
 * Python bindings for clang based C/C++ IDE functions.
 *
 * Jun 11 2017 Vladimir Bogretsov <bogrecov@gmail.com>
 */
#include <errno.h>
#include <string.h>

#include <Python.h>
#include "ide.h"

#define IDE_DOC "IDE object."

#define MODULE_DOC "Python bindings for libclang based ide functions \
implementations."

#define EINVALID_FLAG "expected flag of type 'str' at index %d"
#define ELIBCLANG_LOAD "unable to load libclang: %s"
#define EARGS_INIT_FALGS_ITER "flags expected to be iterable"
#define EARGS_INIT_FLAGS_ITER_FAILED "unexpected error when iterating flags"
#define EARGS_ON_FILE_OPEN "expected arguments: 'str', 'str'"
#define EARGS_ON_FILE_CLOSE "expected arguments: 'str'"
#define EARGS_ON_FILE_SAVE "expected arguments: 'str', 'str'"
#define EARGS_FIND_COMPLETIONS "expected arguments: 'str', 'int', 'int', 'str'"

typedef struct {
    PyObject_HEAD
    ide_t* ide;
    char const** flags;
    int nflags;
} pyvimclang_Ide;

static PyObject* TAG_MENU;
static PyObject* TAG_WORD;
static PyObject* TAG_ABBR;
static PyObject* TAG_KIND;

static void
Ide_dealloc(pyvimclang_Ide* self)
{
    if (self->flags)
    {
        for (int i = 0; i < self->nflags; ++i)
        {
            free((void*)self->flags[i]);
        }
        free(self->flags);
    }
    if (self->ide)
    {
        ide_free(self->ide);
    }
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *
Ide_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    pyvimclang_Ide* self;
    self = (pyvimclang_Ide*)type->tp_alloc(type, 0);
    if (self != NULL)
    {
        self->ide = NULL;
    }
    return (PyObject*)self;
}

static int
_Ide_read_flags(pyvimclang_Ide* self, PyObject* flags)
{
    PyObject *iterator = PyObject_GetIter(flags);
    PyObject *item;

    self->nflags = 0;
    self->flags = (char const**)malloc(sizeof(char*) * PyObject_Length(flags));

    if (iterator == NULL)
    {
        PyErr_SetString(PyExc_TypeError, EARGS_INIT_FALGS_ITER);
        return -1;
    }

    int i = 0;
    while ((item = PyIter_Next(iterator)))
    {
        if (strcmp(item->ob_type->tp_name, "str") != 0)
        {
            PyErr_Format(PyExc_TypeError, EINVALID_FLAG, i);
            return -1;
        }
        self->flags[i++] = PyUnicode_AS_DATA(item);
        Py_DECREF(item);
    }

    Py_DECREF(iterator);

    if (PyErr_Occurred())
    {
        PyErr_SetString(PyExc_TypeError, EARGS_INIT_FLAGS_ITER_FAILED);
        return -1;
    }

    return 0;
}

static int
Ide_init(
    pyvimclang_Ide* self,
    PyObject* args,
    PyObject* kwargs)
{
    char* libclang_path;
    PyObject* flags_tuple = NULL;

    if (!PyArg_ParseTuple(args, "s|O", &libclang_path, &flags_tuple))
    {
        return -1;
    }

    if (flags_tuple == NULL)
    {
        self->flags = NULL;
        self->nflags = 0;
    }
    else if (_Ide_read_flags(self, flags_tuple) != 0)
    {
        return -1;
    }

    self->ide = ide_alloc(
        libclang_path, (const char* const*)self->flags, self->nflags);

    if (self->ide == NULL)
    {
        PyErr_Format(PyExc_ValueError, ELIBCLANG_LOAD, strerror(errno));
        return -1;
    }

    return 0;
}

static PyObject*
Ide_on_file_open(pyvimclang_Ide* self, PyObject* args)
{
    if (self->ide)
    {
        char* path;

        if (!PyArg_ParseTuple(args, "s", &path))
        {
            PyErr_SetString(
                PyExc_TypeError, EARGS_ON_FILE_OPEN);
            Py_RETURN_NONE;
        }

        ide_on_file_open(self->ide, path);
    }
    Py_RETURN_NONE;
}

static PyObject*
Ide_on_file_save(pyvimclang_Ide* self, PyObject* args)
{
    if (self->ide)
    {
        char* path;

        if (!PyArg_ParseTuple(args, "s", &path))
        {
            PyErr_SetString(PyExc_TypeError, EARGS_ON_FILE_SAVE);
            Py_RETURN_NONE;
        }

        ide_on_file_save(self->ide, path);
    }
    Py_RETURN_NONE;
}

static PyObject*
Ide_on_file_close(pyvimclang_Ide* self, PyObject* args)
{
    if (self->ide)
    {
        char* path;
        if (!PyArg_ParseTuple(args, "s", &path))
        {
            PyErr_SetString(PyExc_TypeError, EARGS_ON_FILE_CLOSE);
            Py_RETURN_NONE;
        }

        ide_on_file_close(self->ide, path);
    }
    Py_RETURN_NONE;
}

static void insert_completion(void* ctx, completion_t* completion)
{
    PyObject* item = PyDict_New();
    char kind[] = {completion->kind, '\0'};
    PyDict_SetItem(item, TAG_KIND, PyUnicode_FromString(kind));
    PyDict_SetItem(item, TAG_MENU, PyUnicode_FromString(completion->menu));
    PyDict_SetItem(item, TAG_ABBR, PyUnicode_FromString(completion->abbr));
    PyDict_SetItem(item, TAG_WORD, PyUnicode_FromString(completion->word));
    PyList_Append((PyObject*)ctx, item);
}

static PyObject*
Ide_find_completions(pyvimclang_Ide* self, PyObject* args)
{
    if (!self->ide)
    {
        Py_RETURN_NONE;
    }

    char* path;
    unsigned line;
    unsigned column;
    char* content;
    unsigned size;

    if (!PyArg_ParseTuple(
        args, "siis#", &path, &line, &column, &content, &size))
    {
        PyErr_SetString(PyExc_TypeError, EARGS_FIND_COMPLETIONS);
        Py_RETURN_NONE;
    }

    PyObject* res = PyList_New(0);
    ide_find_completions(
        self->ide, path, line, column, content, size, res, &insert_completion);
    return res;
}

static PyObject*
Ide_find_definition(pyvimclang_Ide* self, PyObject* args)
{
    // Not implemented.
    Py_RETURN_NONE;
}

static PyObject*
Ide_find_declaration(pyvimclang_Ide* self, PyObject* args)
{
    // Not implemented.
    Py_RETURN_NONE;
}

static PyObject*
Ide_find_assingments(pyvimclang_Ide* self, PyObject* args)
{
    // Not implemented.
    Py_RETURN_NONE;
}

static PyObject*
Ide_find_references(pyvimclang_Ide* self, PyObject* args)
{
    // Not implemented.
    Py_RETURN_NONE;
}

static PyMethodDef Ide_methods[] =
{
    {
        "on_file_open",
        (PyCFunction)Ide_on_file_open,
        METH_VARARGS,
        "Open file."
    },
    {
        "on_file_save",
        (PyCFunction)Ide_on_file_save,
        METH_VARARGS,
        "Save file."
    },
    {
        "on_file_close",
        (PyCFunction)Ide_on_file_close,
        METH_VARARGS,
        "Close file."
    },
    {
        "find_completions",
        (PyCFunction)Ide_find_completions,
        METH_VARARGS,
        "Find completions."
    },
    {
        "find_definition",
        (PyCFunction)Ide_find_definition,
        METH_VARARGS,
        "Find definition."
    },
    {
        "find_declaration",
        (PyCFunction)Ide_find_declaration,
        METH_VARARGS,
        "Find declaration."
    },
    {
        "find_assingments",
        (PyCFunction)Ide_find_assingments,
        METH_VARARGS,
        "Find assingments."
    },
    {
        "find_references",
        (PyCFunction)Ide_find_references,
        METH_VARARGS,
        "Find references."
    },
    {
        NULL
    }
};

static PyTypeObject pyvimclang_IdeType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pyvimclang.Ide",                           /* tp_name */
    sizeof(pyvimclang_Ide),                     /* tp_basicsize */
    0,                                          /* tp_itemsize */
    (destructor)Ide_dealloc,                    /* tp_dealloc */
    0,                                          /* tp_print */
    0,                                          /* tp_getattr */
    0,                                          /* tp_setattr */
    0,                                          /* tp_reserved */
    0,                                          /* tp_repr */
    0,                                          /* tp_as_number */
    0,                                          /* tp_as_sequence */
    0,                                          /* tp_as_mapping */
    0,                                          /* tp_hash  */
    0,                                          /* tp_call */
    0,                                          /* tp_str */
    0,                                          /* tp_getattro */
    0,                                          /* tp_setattro */
    0,                                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   /* tp_flags */
    IDE_DOC,                                    /* tp_doc */
    0,                                          /* tp_traverse */
    0,                                          /* tp_clear */
    0,                                          /* tp_richcompare */
    0,                                          /* tp_weaklistoffset */
    0,                                          /* tp_iter */
    0,                                          /* tp_iternext */
    Ide_methods,                                /* tp_methods */
    0,                                          /* tp_members */
    0,                                          /* tp_getset */
    0,                                          /* tp_base */
    0,                                          /* tp_dict */
    0,                                          /* tp_descr_get */
    0,                                          /* tp_descr_set */
    0,                                          /* tp_dictoffset */
    (initproc)Ide_init,                         /* tp_init */
    0,                                          /* tp_alloc */
    Ide_new,                                    /* tp_new */
};

static PyModuleDef pyvimclangmodule = {
    PyModuleDef_HEAD_INIT,
    "pyvimclang",
    MODULE_DOC,
    -1,
    NULL, NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC
PyInit_pyvimclang(void)
{
    PyObject* module = NULL;
    pyvimclang_IdeType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&pyvimclang_IdeType) >= 0)
    {
        module = PyModule_Create(&pyvimclangmodule);
        if (module != NULL)
        {
            Py_INCREF(&pyvimclang_IdeType);
            PyModule_AddObject(
                module,
                "Ide",
                (PyObject*)&pyvimclang_IdeType);

            TAG_MENU = PyUnicode_FromString("menu");
            Py_INCREF(TAG_MENU);
            PyModule_AddObject(module, "TAG_MENU", TAG_MENU);

            TAG_ABBR = PyUnicode_FromString("abbr");
            Py_INCREF(TAG_ABBR);
            PyModule_AddObject(module, "TAG_ABBR", TAG_ABBR);

            TAG_WORD = PyUnicode_FromString("word");
            Py_INCREF(TAG_WORD);
            PyModule_AddObject(module, "TAG_WORD", TAG_WORD);

            TAG_KIND = PyUnicode_FromString("kind");
            Py_INCREF(TAG_KIND);
            PyModule_AddObject(module, "TAG_KIND", TAG_KIND);
        }
    }
    return module;
}
