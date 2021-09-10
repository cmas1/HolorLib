#!/bin/sh

###############################################################################
# This file is part of Holor, a C++ template library for multi-dimensional containers

# Copyright 2020 Carlo Masone

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to 
# deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the Software is 
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or suholorantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
# DEALINGS IN THE SOFTWARE.
###############################################################################


set -e

HOLOR_ROOT_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd -P)"
HOLOR_SCRIPTS_PATH="${HOLOR_ROOT_PATH}/scripts"
HOLOR_SPHINX_PATH="${HOLOR_ROOT_PATH}/sphinx"
HOLOR_DOXYGEN_PATH="${HOLOR_ROOT_PATH}/doxygen"
HOLOR_DOCS_PATH="${HOLOR_ROOT_PATH}/docs"
source "${HOLOR_SCRIPTS_PATH}/holor.bashrc"

info "Start building the documentation."

cd ${HOLOR_SPHINX_PATH}
make clean

cd ${HOLOR_DOXYGEN_PATH}
doxygen # > /dev/null

cd ${HOLOR_SPHINX_PATH}
make html # > /dev/null

cd ${HOLOR_DOXYGEN_PATH} 
rm -rf xml

cd ${HOLOR_DOCS_PATH}
rm -rf doctrees

cd ${HOLOR_ROOT_PATH}

info "Documentation built and available in docs/".
