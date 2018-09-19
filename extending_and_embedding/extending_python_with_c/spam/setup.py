from distutils.core import setup, Extension

setup(
    name='spam',
    version='0.1',
    ext_modules=[
        Extension('spam', ['spam.c']),
    ],
)