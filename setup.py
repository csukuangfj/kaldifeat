#!/usr/bin/env python3
#
# Copyright (c)  2021  Xiaomi Corporation (author: Fangjun Kuang)

import glob
import os
import re
import setuptools
import shutil
import subprocess

from setuptools.command.build_ext import build_ext


def read_long_description():
    with open('README.md', encoding='utf8') as f:
        readme = f.read()
    return readme


def get_package_version():
    with open('CMakeLists.txt') as f:
        content = f.read()

    latest_version = re.search(r'set\(kaldifeat_VERSION (.*)\)',
                               content).group(1)
    latest_version = latest_version.strip('"')
    return latest_version


package_name = 'kaldifeat'

setuptools.setup(
    name=package_name,
    version=get_package_version(),
    author='Fangjun Kuang',
    author_email='csukuangfj@gmail.com',
    data_files=[('', ['LICENSE', 'README.md'])],
    package_dir={
        package_name: 'kaldifeat/python/kaldifeat',
    },
    packages=[package_name],
    url='https://github.com/csukuangfj/kaldifeat',
    long_description=read_long_description(),
    long_description_content_type='text/markdown',
    #  ext_modules=[cmake_extension('_kaldifeat')],
    #  cmdclass={'build_ext': BuildExtension},
    zip_safe=False,
    classifiers=[
        'Programming Language :: C++',
        'Programming Language :: Python',
        'Topic :: Scientific/Engineering :: Artificial Intelligence',
    ],
    license='Apache licensed, as found in the LICENSE file',
)
