# 阅读《Extending and Embedding the Python Interpreter》和《Python/C API Reference Manual》的笔记

[《Extending and Embedding the Python Interpreter》](https://docs.python.org/2.7/extending/index.html) 是一份关于如何用 C 语言编写 Python 模块的文档。

[《Python/C API Reference Manual》](https://docs.python.org/2.7/c-api/index.html) 是一份关于 Python C API 的手册。

这里记录了一些阅读过程的笔记，包括代码。

## 目录

- 《Extending and Embedding the Python Interpreter》

    - [如何使用 C 编写一个模块](extending_and_embedding/extending_python_with_c)

    - [自定义类型（类）](extending_and_embedding/define_new_type)

    - [使用 distutils 编译模块](extending_and_embedding/building_c_extensions_with_distutils)

    - [在其它应用中嵌入 Python 解释器](extending_and_embedding/embendding_python_in_another_application)

- 《Python/C API Reference Manual》

    - [简介](c_api/intro)
    
    - [顶层API](c_api/the_very_high_level_layer)
    
    - 其余的API直接看文档比较好，需要用的时候查文档就行。