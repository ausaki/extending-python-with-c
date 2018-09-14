# 使用 C 语言编写 Python 模块

## 错误处理

当函数在运行过程中发生了错误，应该设置异常信息（好比 Python 层面的 raise），并且返回 NULL 或者 - 1。

Python 使用三个全局变量来保存异常信息，这三个全局变量分别对应 `sys.exc_type`, `sys.exc_value`, `sys.exc_traceback`。

Python 提供了三个设置异常信息的 API：

- `void PyErr_SetString(PyObject *type, const char *message)`

- `PyObject* PyErr_SetFromErrno(PyObject *type)`

- `void PyErr_SetObject(PyObject *type, PyObject *value)`

检测是否发生了异常的 API：

- `PyErr_Occurred()`

清除异常信息的 API：

- `PyErr_Clear()`

大部分 Python 提供的 API 都有进行错误处理，所有作为调用者就不必再进行错误处理了。

## 引用计数

当创建了新的 object 时，记得增加引用计数 (Py_INCREF)，当发生异常函数退出时，记得减少引用计数 (Py_XDECREF or Py_DECREF)。


## 模块函数表和模块初始化

模块函数表定义了模块中包含的函数，例如：

```C
static PyMethodDef SpamMethods[] = {
    ...
    {"system",  spam_system, METH_VARARGS,
     "Execute a shell command."},
    ...
    {NULL, NULL, 0, NULL}        /* Sentinel */
};
```

在定义了模块函数表之后，在模块初始化函数中将模块函数表传给 Py_InitModule。

```C
PyMODINIT_FUNC
initspam(void)
{
    (void) Py_InitModule("spam", SpamMethods);
}
```

按照约定，模块初始化函数的命名遵循 initname 格式，其中 name 为模块名。

## 编译和链接

### 动态链接

使用 distutils 编译动态模块。

### 内嵌至 Python 解释器

将 spammodule.c 放到 Modules / 目录，并在 Modules/Setup.local 文件中添加 `spam spammodule.o`，最后重新 make。

## 解析 Python 传来的参数

```C
int ok;
int i, j;
long k, l;
const char *s;
int size;

ok = PyArg_ParseTuple(args, ""); /* No arguments */
    /* Python call: f() */
ok = PyArg_ParseTuple(args, "s", &s); /* A string */
    /* Possible Python call: f('whoops!') */
ok = PyArg_ParseTuple(args, "lls", &k, &l, &s); /* Two longs and a string */
    /* Possible Python call: f(1, 2, 'three') */
ok = PyArg_ParseTuple(args, "(ii)s#", &i, &j, &s, &size);
    /* A pair of ints and a string, whose size is also returned */
    /* Possible Python call: f((1, 2), 'three') */
{
    const char *file;
    const char *mode = "r";
    int bufsize = 0;
    ok = PyArg_ParseTuple(args, "s|si", &file, &mode, &bufsize);
    /* A string, and optionally another string and an integer */
    /* Possible Python calls:
       f('spam')
       f('spam', 'w')
       f('spam', 'wb', 100000) */
}
{
    int left, top, right, bottom, h, v;
    ok = PyArg_ParseTuple(args, "((ii)(ii))(ii)",
             &left, &top, &right, &bottom, &h, &v);
    /* A rectangle and a point */
    /* Possible Python call:
       f(((0, 0), (400, 300)), (10, 10)) */
}
{
    Py_complex c;
    ok = PyArg_ParseTuple(args, "D:myfunction", &c);
    /* a complex, also providing a function name for errors */
    /* Possible Python call: myfunction(1+2j) */
}
```

## 创建 Python 值

```C
Py_BuildValue("")                        None
Py_BuildValue("i", 123)                  123
Py_BuildValue("iii", 123, 456, 789)      (123, 456, 789)
Py_BuildValue("s", "hello")              'hello'
Py_BuildValue("ss", "hello", "world")    ('hello', 'world')
Py_BuildValue("s#", "hello", 4)          'hell'
Py_BuildValue("()")                      ()
Py_BuildValue("(i)", 123)                (123,)
Py_BuildValue("(ii)", 123, 456)          (123, 456)
Py_BuildValue("(i,i)", 123, 456)         (123, 456)
Py_BuildValue("[i,i]", 123, 456)         [123, 456]
Py_BuildValue("{s:i,s:i}",
              "abc", 123, "def", 456)    {'abc': 123, 'def': 456}
Py_BuildValue("((ii)(ii)) (ii)",
              1, 2, 3, 4, 5, 6)          (((1, 2), (3, 4)), (5, 6))
```


## The ice

There are a few situations where seemingly harmless use of a borrowed reference can lead to problems. These all have to do with implicit invocations of the interpreter, which can cause the owner of a reference to dispose of it.

The first and most important case to know about is using Py_DECREF() on an unrelated object while borrowing a reference to a list item. For instance:

```C
void
bug(PyObject *list)
{
    PyObject *item = PyList_GetItem(list, 0);

    PyList_SetItem(list, 1, PyInt_FromLong(0L));
    PyObject_Print(item, stdout, 0); /* BUG! */
}
```

This function first borrows a reference to list[0], then replaces list[1] with the value 0, and finally prints the borrowed reference. Looks harmless, right? But it’s not!

Let’s follow the control flow into PyList_SetItem(). The list owns references to all its items, so when item 1 is replaced, it has to dispose of the original item 1. Now let’s suppose the original item 1 was an instance of a user-defined class, and let’s further suppose that the class defined a `__del__()` method. If this class instance has a reference count of 1, disposing of it will call its `__del__()` method.

Since it is written in Python, the `__del__()` method can execute arbitrary Python code. Could it perhaps do something to invalidate the reference to item in bug()? You bet! Assuming that the list passed into bug() is accessible to the `__del__()` method, it could execute a statement to the effect of `del list[0]`, and assuming this was the last reference to that object, it would free the memory associated with it, thereby invalidating item.

The solution, once you know the source of the problem, is easy: temporarily increment the reference count. The correct version of the function reads:

```C
void
no_bug(PyObject *list)
{
    PyObject *item = PyList_GetItem(list, 0);

    Py_INCREF(item);
    PyList_SetItem(list, 1, PyInt_FromLong(0L));
    PyObject_Print(item, stdout, 0);
    Py_DECREF(item);
}
```

This is a true story. An older version of Python contained variants of this bug and someone spent a considerable amount of time in a C debugger to figure out why his `__del__()` methods would fail…

The second case of problems with a borrowed reference is a variant involving threads. Normally, multiple threads in the Python interpreter can’t get in each other’s way, because there is a global lock protecting Python’s entire object space. However, it is possible to temporarily release this lock using the macro Py_BEGIN_ALLOW_THREADS, and to re-acquire it using Py_END_ALLOW_THREADS. This is common around blocking I/O calls, to let other threads use the processor while waiting for the I/O to complete. Obviously, the following function has the same problem as the previous one:

```C
void
bug(PyObject *list)
{
    PyObject *item = PyList_GetItem(list, 0);
    Py_BEGIN_ALLOW_THREADS
    ...some blocking I/O call...
    Py_END_ALLOW_THREADS
    PyObject_Print(item, stdout, 0); /* BUG! */
}
```


## 完整实例

### C 模块

```C
#include "Python.h"

static PyObject *SpamError;

static PyObject *
spam_system(PyObject *self, PyObject *args)
{
    const char *command;
    int sts;

    if (!PyArg_ParseTuple(args, "s", &command))
        return NULL;
    sts = system(command);
    if(sts < 0){
        PyErr_SetString(SpamError, "System command failed");
    }
    return Py_BuildValue("i", sts);
}

static PyMethodDef SpamMethods[] = {
    {"system", spam_system, METH_VARARGS,
     "Execute a shell command."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC
initspam(void)
{
    PyObject *m;

    m = Py_InitModule("spam", SpamMethods);
    if (m == NULL)
        return;

    SpamError = PyErr_NewException("spam.error", NULL, NULL);
    Py_INCREF(SpamError);
    PyModule_AddObject(m, "error", SpamError);
}
```

### setup.py

```python
from distutils.core import setup, Extension

setup(
    name='spam',
    version='0.1',
    ext_modules=[
        Extension('spam', ['spam.c']),
    ],
)
```

### 安装

`python setup.py install`

### 运行

```python
>>> import spam
>>> spam.system('ls -lh')
```



