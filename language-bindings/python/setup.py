# -*- coding: utf-8 -*-
#!/usr/bin/env python3
#
# Copyright (C) 2019 Intel Corporation.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#
# pylint: disable=missing-class-docstring
# pylint: disable=missing-function-docstring
# pylint: disable=missing-module-docstring

import pathlib
<<<<<<< HEAD
from setuptools import setup, find_packages
from setuptools.command.develop import develop
from setuptools.command.install import install
from setuptools.command.egg_info import egg_info
from subprocess import check_call
=======
from setuptools import setup
from setuptools.command.develop import develop
from setuptools.command.install import install
from subprocess import check_call


def build_library():
    cur_path = pathlib.Path(__file__).parent
    check_call(f"{cur_path}/utils/create_lib.sh".split())

class PreDevelopCommand(develop):
    """Pre-installation for development mode."""
    def run(self):
        build_library()
        develop.run(self)

class PreInstallCommand(install):
    """Pre-installation for installation mode."""
    def run(self):
        build_library()
        install.run(self)
>>>>>>> 3cc132e8... Add WAMR API bindings in Python (#1959)


def read_file(file: str) -> str:
    with open(file) as f:
        return f.read()


def build_library():
    cur_path = pathlib.Path(__file__).parent
    check_call(f"{cur_path}/utils/create_lib.sh".split())


class PreDevelopCommand(develop):
    def run(self):
        build_library()
        develop.run(self)


class PreInstallCommand(install):
    def run(self):
        build_library()
        install.run(self)


<<<<<<< HEAD
<<<<<<< HEAD
=======
class PreEggInfoCommand(egg_info):
    def run(self):
        build_library()
        egg_info.run(self)


>>>>>>> b4f02284... Python WAMR API binding: Add malloc/free and register_native (#1989)
with open("README.md") as f:
    readme = f.read()

with open("LICENSE") as f:
    license = f.read()
=======
class PreEggInfoCommand(egg_info):
    def run(self):
        build_library()
        egg_info.run(self)

>>>>>>> 8b47308a... Improve setup

setup(
    name="wamr-python",
    version="0.1.0",
    description="A WebAssembly runtime powered by WAMR",
<<<<<<< HEAD
    long_description=readme,
    packages=find_packages(where="src"),
    package_dir={"": "src"},
    author="The WAMR Project Developers",
    author_email="hello@bytecodealliance.org",
    url="https://github.com/bytecodealliance/wasm-micro-runtime",
    license=license,
<<<<<<< HEAD
=======
    long_description=read_file("README.md"),
    packages=find_packages(where="src"),
    package_dir={"": "src"},
    author="The WAMR Project Developers",
    author_email="hello@bytecodealliance.org",
    url="https://github.com/bytecodealliance/wasm-micro-runtime",
    license=read_file("LICENSE"),
    python_requires='>=3.8',
>>>>>>> 8b47308a... Improve setup
=======
>>>>>>> 3cc132e8... Add WAMR API bindings in Python (#1959)
    include_package_data=True,
    cmdclass={
        'develop': PreDevelopCommand,
        'install': PreInstallCommand,
<<<<<<< HEAD
        'egg_info': PreEggInfoCommand,
=======
>>>>>>> 3cc132e8... Add WAMR API bindings in Python (#1959)
    },
)
