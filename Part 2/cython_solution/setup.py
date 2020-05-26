#! /usr/bin/env python3

from setuptools import setup
from Cython.Build import cythonize
from Cython.Distutils import build_ext
from distutils import sysconfig
import os

# https://stackoverflow.com/questions/60284403/change-output-filename-in-setup-py-distutils-extension
class NoSuffixBuilder(build_ext):
    def get_ext_filename(self, ext_name):
        filename = super().get_ext_filename(ext_name)
        suffix = sysconfig.get_config_var('EXT_SUFFIX')
        ext = os.path.splitext(filename)[1]
        return filename.replace(suffix, "") + ext

setup(
    cmdclass={"build_ext": NoSuffixBuilder},
    ext_modules = cythonize("Exchange.pyx")
)
