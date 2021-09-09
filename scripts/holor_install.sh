#! /usr/bin/env bash

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
source "${HOLOR_SCRIPTS_PATH}/holor.bashrc"


PARSED_OPTIONS=


function main() {
    cd ${HOLOR_ROOT_PATH}
    if [ ! -d "${HOLOR_ROOT_PATH}/build" ] 
    then
        warning "The build directory does not exist and will be created." 
        mkdir build
    else 
        warning "The build directory already exists and its content may be overwritten." 
    fi
    cd build

    cmake ${HOLOR_ROOT_PATH} -DCMAKE_BUILD_TYPE=Release -DHOLOR_BUILD_TESTS=OFF -DHOLOR_BUILD_BENCHMARKS=OFF -DHOLOR_BUILD_EXAMPLES=OFF -DHOLOR_INSTALL_LIBRARY=ON
    sudo cmake --install .
    cd ${HOLOR_ROOT_PATH}

    echo
    ok "Installation completed"
}



main "$@"