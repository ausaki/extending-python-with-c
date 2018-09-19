from distutils.core import setup, Extension

setup(
    name='noddy2',
    version='0.1',
    ext_modules=[
        Extension('noddy2', ['noddyobject.c']),
    ],
)