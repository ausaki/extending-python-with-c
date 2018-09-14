from distutils.core import setup, Extension

setup(
    name='noddy',
    version='0.1',
    ext_modules=[
        Extension('noddy', ['noddyobject.c']),
    ],
)