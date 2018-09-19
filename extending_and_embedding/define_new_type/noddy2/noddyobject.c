#include <Python.h>
#include "structmember.h"

typedef struct {
    PyObject_HEAD
    PyObject *first; /* first name */
    PyObject *last;  /* last name */
    int number;
} Noddy;

static void
Noddy_dealloc(Noddy* self)
{
    Py_XDECREF(self->first);
    Py_XDECREF(self->last);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *
Noddy_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    Noddy *self;

    self = (Noddy *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->first = PyString_FromString("");
        if (self->first == NULL) {
            Py_DECREF(self);
            return NULL;
        }

        self->last = PyString_FromString("");
        if (self->last == NULL) {
            Py_DECREF(self);
            return NULL;
        }

        self->number = 0;
    }

    return (PyObject *)self;
}

static int
Noddy_init(Noddy *self, PyObject *args, PyObject *kwds)
{
    PyObject *first=NULL, *last=NULL, *tmp;

    static char *kwlist[] = {"first", "last", "number", NULL};

    if (! PyArg_ParseTupleAndKeywords(args, kwds, "|OOi", kwlist,
                                      &first, &last,
                                      &self->number))
        return -1;

    if (first) {
        tmp = self->first;
        Py_INCREF(first);
        self->first = first;
        Py_XDECREF(tmp);
    }

    if (last) {
        tmp = self->last;
        Py_INCREF(last);
        self->last = last;
        Py_XDECREF(tmp);
    }

    return 0;
}


static PyMemberDef Noddy_members[] = {
    {"first", T_OBJECT_EX, offsetof(Noddy, first), 0,
     "first name"},
    {"last", T_OBJECT_EX, offsetof(Noddy, last), 0,
     "last name"},
    {"number", T_INT, offsetof(Noddy, number), 0,
     "noddy number"},
    {NULL}  /* Sentinel */
};

static PyObject *
Noddy_name(Noddy* self)
{
    static PyObject *format = NULL;
    PyObject *args, *result;

    if (format == NULL) {
        format = PyString_FromString("%s %s");
        if (format == NULL)
            return NULL;
    }

    if (self->first == NULL) {
        PyErr_SetString(PyExc_AttributeError, "first");
        return NULL;
    }

    if (self->last == NULL) {
        PyErr_SetString(PyExc_AttributeError, "last");
        return NULL;
    }

    args = Py_BuildValue("OO", self->first, self->last);
    if (args == NULL)
        return NULL;

    result = PyString_Format(format, args);
    Py_DECREF(args);
    return result;
}

static PyObject*
Noddy_test(Noddy* self)
{
    PyStringObject *n = PyString_FromString("first");
    descrgetfunc f;
    PyObject *res = NULL;
    PyObject *descr = _PyType_Lookup(self->ob_type, n);
    
    printf("test\n");

    printf("descr: ");
    PyObject_Print(descr, stdout, 0);
    printf("\n");

    if (descr != NULL &&
	    PyType_HasFeature(descr->ob_type, Py_TPFLAGS_HAVE_CLASS)) {
		f = descr->ob_type->tp_descr_get;
		if (f != NULL && PyDescr_IsData(descr)) {
			res = f(descr, self, (PyObject *)self->ob_type);
		}
	}

    printf("res: ");
    PyObject_Print(res, stdout, 0);
    printf("\n");

    printf("attr: ");
    PyObject* v = PyObject_GenericGetAttr((PyObject*)self, n);
    PyObject_Print(v, stdout, 0);
    printf("\n");
    
    return Py_None;
}

static PyMethodDef Noddy_methods[] = {
    {"name", (PyCFunction)Noddy_name, METH_NOARGS,
     "Return the name, combining the first and last name"
    },
    {"test", (PyCFunction)Noddy_test, METH_NOARGS, "test"},
    {NULL}  /* Sentinel */
};

static PyTypeObject NoddyType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "noddy.Noddy",             /* tp_name */
    sizeof(Noddy),             /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)Noddy_dealloc, /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_compare */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "Noddy objects",           /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    Noddy_methods,             /* tp_methods */
    Noddy_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Noddy_init,      /* tp_init */
    0,                         /* tp_alloc */
    Noddy_new,                 /* tp_new */
};

static PyMethodDef module_methods[] = {
    {NULL}  /* Sentinel */
};

#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
initnoddy2(void)
{
    PyObject* m;

    if (PyType_Ready(&NoddyType) < 0)
        return;
    printf("after PyType_Ready \n");
    printf("type.tp_dict: \n");
    PyObject_Print(NoddyType.tp_dict, stdout, 0);
    printf("\n");
    printf("type.tp_flags: \n");
    printf("%x", NoddyType.tp_flags);
    printf("\n");
    printf("type.tp_itemsize: \n");
    printf("%x", NoddyType.tp_itemsize);
    printf("\n");
    printf("type.tp_dictoffset: \n");
    printf("%x", NoddyType.tp_dictoffset);
    printf("\n");
    printf("type.tp_as_number: \n");
    printf("%x\n", NoddyType.tp_as_number);
    printf("%x", PyBaseObject_Type.tp_as_number);
    printf("\n");
    printf("type.tp_as_sequence: \n");
    printf("%x\n", NoddyType.tp_as_sequence);
    printf("%x", PyBaseObject_Type.tp_as_sequence);
    printf("\n");
    printf("type.tp_as_mapping: \n");
    printf("%x\n", NoddyType.tp_as_mapping);
    printf("%x", PyBaseObject_Type.tp_as_mapping);
    printf("\n");
    printf("type.tp_print: \n");
    printf("%x\n", NoddyType.tp_print);
    printf("%x", PyBaseObject_Type.tp_print);
    printf("\n");
    printf("type.tp_str: \n");
    printf("%x\n", NoddyType.tp_str);
    printf("%x", PyBaseObject_Type.tp_str);
    printf("\n");
    printf("type.tp_init: \n");
    printf("%x\n", NoddyType.tp_init);
    printf("%x", PyBaseObject_Type.tp_init);
    printf("\n");
    printf("type.tp_new: \n");
    printf("%x\n", NoddyType.tp_new);
    printf("%x", PyBaseObject_Type.tp_new);
    printf("\n");
    printf("type.tp_getattro: \n");
    printf("%x\n", NoddyType.tp_getattro);
    printf("%x", PyBaseObject_Type.tp_getattro);
    printf("\n");
    printf("type.tp_setattro: \n");
    printf("%x\n", NoddyType.tp_setattro);
    printf("%x", PyBaseObject_Type.tp_setattro);
    printf("\n");
    m = Py_InitModule3("noddy2", module_methods,
                       "Example module that creates an extension type.");

    if (m == NULL)
        return;

    Py_INCREF(&NoddyType);
    PyModule_AddObject(m, "Noddy", (PyObject *)&NoddyType);
}