# 简介

> 原文链接：[https://docs.python.org/2.7/c-api/intro.html](https://docs.python.org/2.7/c-api/intro.html)

## include files

```c
#include "Python.h"
```

在编写 Python 模块时，必须在代码最开始处包含 `Python.h` 头文件。

`Python.h` 已经引入了以下头文件：<stdio.h>, <string.h>, <errno.h>, <limits.h>, <assert.h> and <stdlib.h>。

`Python.h` 提供的 API 的命名规则：

- 统一由 `Py` 或者 `_Py` 开头

- `_Py` 开头的 API 专为 Python 内部使用

## Objects, Types

在 Python 中基本上所有变量都是存放在堆（heap）上的，使用 `PyObject *` 引用。

Python 的内部类型都已对应的宏用于检测其类型，例如 `PyList_Check(a)`。

## 引用计数

- `Py_INCREF or Py_XINCREF` 引用计数 + 1

- `Py_DECREF or Py_XDECREF` 减少引用计数 - 1

如果引用计数变为 0，则会调用对象类型的 `dealloctor`，`dealloctor` 会释放对象占用的内存，如果对象是复合类型（如 list，dict），`dealloctor` 还会减少其子元素的引用计数，如果子元素的引用计数也变为 0，同样会调用子元素的 `dealloctor`。

当将一个对象赋值给一个变量时，不一定必须增加引用计数。理论上，赋值时增加引用计数，变量使用完毕后减少引用计数，这两个操作正好互相抵消，引用计数没有变化。因此，只要能够确保在处理过程中对象的引用计数不会变为 0，则不用增加引用计数。

当一个对象作为参数传递给 C 模块中的函数时，Python 的函数调用机制保证在函数执行过程中保留一个对该对象的引用。


## 异常处理

```python
def incr_item(dict, key):
    try:
        item = dict[key]
    except KeyError:
        item = 0
    dict[key] = item + 1
```

对应的 C 代码：

```C
int
incr_item(PyObject *dict, PyObject *key)
{
    /* Objects all initialized to NULL for Py_XDECREF */
    PyObject *item = NULL, *const_one = NULL, *incremented_item = NULL;
    int rv = -1; /* Return value initialized to -1 (failure) */

    item = PyObject_GetItem(dict, key);
    if (item == NULL) {
        /* Handle KeyError only: */
        if (!PyErr_ExceptionMatches(PyExc_KeyError))
            goto error;

        /* Clear the error and use zero: */
        PyErr_Clear();
        item = PyInt_FromLong(0L);
        if (item == NULL)
            goto error;
    }
    const_one = PyInt_FromLong(1L);
    if (const_one == NULL)
        goto error;

    incremented_item = PyNumber_Add(item, const_one);
    if (incremented_item == NULL)
        goto error;

    if (PyObject_SetItem(dict, key, incremented_item) < 0)
        goto error;
    rv = 0; /* Success */
    /* Continue with cleanup code */

 error:
    /* Cleanup code, shared by success and failure path */

    /* Use Py_XDECREF() to ignore NULL references */
    Py_XDECREF(item);
    Py_XDECREF(const_one);
    Py_XDECREF(incremented_item);

    return rv; /* -1 for error, 0 for success */
}
```

## 嵌入 Python

流程：

- 初始化 Python 解释器：`Py_Initialize()`

- 设置 sys.argv：`PySys_SetArgvEx(argc, argv, updatepath)`

- 销毁 Python 解释器：`Py_Finalize()`

## 编译 debug 版本的 Python 解释器

`./configure --with-pydebug`

详情参考 `Misc/SpecialBuilds.txt`。


