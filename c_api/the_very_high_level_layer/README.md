# 顶层 API

> 原文链接：[https://docs.python.org/2.7/c-api/veryhigh.html](https://docs.python.org/2.7/c-api/veryhigh.html)

`int Py_Main(int argc, char **argv)`

`int PyRun_AnyFile(FILE *fp, const char *filename)`

`int PyRun_AnyFileFlags(FILE *fp, const char *filename, PyCompilerFlags *flags)`

`int PyRun_AnyFileEx(FILE *fp, const char *filename, int closeit)`

`int PyRun_AnyFileExFlags(FILE *fp, const char *filename, int closeit, PyCompilerFlags *flags)`

`int PyRun_SimpleString(const char *command)`

`int PyRun_SimpleStringFlags(const char *command, PyCompilerFlags *flags)`

`int PyRun_SimpleFile(FILE *fp, const char *filename)`

`int PyRun_SimpleFileFlags(FILE *fp, const char *filename, PyCompilerFlags *flags)`

`int PyRun_SimpleFileEx(FILE *fp, const char *filename, int closeit)`

`int PyRun_SimpleFileExFlags(FILE *fp, const char *filename, int closeit, PyCompilerFlags *flags)`

`int PyRun_InteractiveOne(FILE *fp, const char *filename)`

`int PyRun_InteractiveOneFlags(FILE *fp, const char *filename, PyCompilerFlags *flags)`

`int PyRun_InteractiveLoop(FILE *fp, const char *filename)`

`int PyRun_InteractiveLoopFlags(FILE *fp, const char *filename, PyCompilerFlags *flags)`

`struct _node* PyParser_SimpleParseString(const char *str, int start)`

`struct _node* PyParser_SimpleParseStringFlags(const char *str, int start, int flags)`

`struct _node* PyParser_SimpleParseStringFlagsFilename(const char *str, const char *filename, int start, int flags)`


`struct _node* PyParser_SimpleParseFile(FILE *fp, const char *filename, int start)`

`struct _node* PyParser_SimpleParseFileFlags(FILE *fp, const char *filename, int start, int flags)`

`PyObject* PyRun_String(const char *str, int start, PyObject *globals, PyObject *locals)`

`PyObject* PyRun_StringFlags(const char *str, int start, PyObject *globals, PyObject *locals, PyCompilerFlags *flags)`

`PyObject* PyRun_File(FILE *fp, const char *filename, int start, PyObject *globals, PyObject *locals)`

`PyObject* PyRun_FileEx(FILE *fp, const char *filename, int start, PyObject *globals, PyObject *locals, int closeit)`

`PyObject* PyRun_FileFlags(FILE *fp, const char *filename, int start, PyObject *globals, PyObject *locals, PyCompilerFlags *flags)`

`PyObject* PyRun_FileExFlags(FILE *fp, const char *filename, int start, PyObject *globals, PyObject *locals, int closeit, PyCompilerFlags *flags)`

`PyObject* Py_CompileString(const char *str, const char *filename, int start)`

`PyObject* Py_CompileStringFlags(const char *str, const char *filename, int start, PyCompilerFlags *flags)`

`PyObject* PyEval_EvalCode(PyCodeObject *co, PyObject *globals, PyObject *locals)`

`PyObject* PyEval_EvalCodeEx(PyCodeObject *co, PyObject *globals, PyObject *locals, PyObject **args, int argcount, PyObject **kws, int kwcount, PyObject **defs, int defcount, PyObject *closure)`

`PyObject* PyEval_EvalFrame(PyFrameObject *f)`

`PyObject* PyEval_EvalFrameEx(PyFrameObject *f, int throwflag)`

`int PyEval_MergeCompilerFlags(PyCompilerFlags *cf)`

`int Py_eval_input`

`int Py_file_input`

`int Py_single_input`
