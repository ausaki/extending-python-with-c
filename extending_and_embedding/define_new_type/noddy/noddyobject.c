#include <Python.h>

typedef struct {
    PyObject_HEAD
    /* Type-specific fields go here. */
} noddy_NoddyObject;

static PyTypeObject noddy_NoddyType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "noddy.Noddy",             /* tp_name */
    sizeof(noddy_NoddyObject), /* tp_basicsize */
    0,                         /* tp_itemsize */
    0,                         /* tp_dealloc */
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
    Py_TPFLAGS_DEFAULT,        /* tp_flags */
    "Noddy objects",           /* tp_doc */
};

static PyMethodDef noddy_methods[] = {
    {NULL}  /* Sentinel */
};

#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
initnoddy(void) 
{
    PyObject* m;

    noddy_NoddyType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&noddy_NoddyType) < 0)
        return;
    printf("after PyType_Ready \n");
    printf("type.tp_dict: \n");
    PyObject_Print(noddy_NoddyType.tp_dict, stdout, 0);
    printf("\n");
    printf("type.tp_flags: \n");
    printf("%x", noddy_NoddyType.tp_flags);
    printf("\n");
    printf("type.tp_itemsize: \n");
    printf("%x", noddy_NoddyType.tp_itemsize);
    printf("\n");
    printf("type.tp_dictoffset: \n");
    printf("%x", noddy_NoddyType.tp_dictoffset);
    printf("\n");
    printf("type.tp_as_number: \n");
    printf("%x\n", noddy_NoddyType.tp_as_number);
    printf("%x", PyBaseObject_Type.tp_as_number);
    printf("\n");
    printf("type.tp_as_sequence: \n");
    printf("%x\n", noddy_NoddyType.tp_as_sequence);
    printf("%x", PyBaseObject_Type.tp_as_sequence);
    printf("\n");
    printf("type.tp_as_mapping: \n");
    printf("%x\n", noddy_NoddyType.tp_as_mapping);
    printf("%x", PyBaseObject_Type.tp_as_mapping);
    printf("\n");
    printf("type.tp_print: \n");
    printf("%x\n", noddy_NoddyType.tp_print);
    printf("%x", PyBaseObject_Type.tp_print);
    printf("\n");
    printf("type.tp_str: \n");
    printf("%x\n", noddy_NoddyType.tp_str);
    printf("%x", PyBaseObject_Type.tp_str);
    printf("\n");
    printf("type.tp_init: \n");
    printf("%x\n", noddy_NoddyType.tp_init);
    printf("%x", PyBaseObject_Type.tp_init);
    printf("\n");
    printf("type.tp_new: \n");
    printf("%x\n", noddy_NoddyType.tp_new);
    printf("%x", PyBaseObject_Type.tp_new);
    printf("\n");
    printf("type.tp_getattro: \n");
    printf("%x\n", noddy_NoddyType.tp_getattro);
    printf("%x", PyBaseObject_Type.tp_getattro);
    printf("\n");
    printf("type.tp_setattro: \n");
    printf("%x\n", noddy_NoddyType.tp_setattro);
    printf("%x", PyBaseObject_Type.tp_setattro);
    printf("\n");
    
    m = Py_InitModule3("noddy", noddy_methods,
                       "Example module that creates an extension type.");

    Py_INCREF(&noddy_NoddyType);
    PyModule_AddObject(m, "Noddy", (PyObject *)&noddy_NoddyType);
}