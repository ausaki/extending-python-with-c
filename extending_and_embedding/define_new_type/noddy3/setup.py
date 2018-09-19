from distutils.core import setup, Extension

setup(
    name='noddy3',
    version='0.1',
    ext_modules=[
        Extension('noddy3', ['noddyobject.c']),
    ],
)