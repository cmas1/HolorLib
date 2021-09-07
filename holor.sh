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


HOLOR_ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"
SCRIPTS_DIR="${HOLOR_ROOT_DIR}/scripts"
source "${SCRIPTS_DIR}/holor.bashrc"


AVAILABLE_COMMANDS="build build_examples build_tests build_docs build_all clean clean_examples clean_tests clean_docs clean_all -h --help"



function _usage() {
    echo -e "\n${RED}Usage${NO_COLOR}:
    .${BOLD}/holor.sh${NO_COLOR} [OPTION]"
    echo -e "\n${RED}Options${NO_COLOR}:
    ${BLUE}install [path]${NO_COLOR}: install the library header files at the path specified. If no path is provided, the library is installed at 
    ${BLUE}build_tests${NO_COLOR}: build test units.
    ${BLUE}build_examples${NO_COLOR}: build examples.
    ${BLUE}build_benchmarks${NO_COLOR}: build benchmarks.
    ${BLUE}build_docs${NO_COLOR}: build documentation.
    ${BLUE}clean${NO_COLOR}: remove installation, compiled units and clean build files
    ${BLUE}-h|--help ${NO_COLOR}: show this message and exit
    "
}




function main() {
    if [ "$#" -eq 0 ]; then
        _usage
        exit 0
    fi

    local cmd="$1"
    shift
    case "${cmd}" in
        install)
            bash "${SCRIPTS_DIR}/holor_install.sh" "$@"
            ;;
        -h|--help)
            _usage
            ;;
        *)
            error "Invalid Command. Check usage by calling ${GREEN}./holor.sh --help${NO_COLOR}"
            ;;
    esac
}

main "$@"