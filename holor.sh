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
HOLOR_SCRIPTS_DIR="${HOLOR_ROOT_DIR}/scripts"
source "${HOLOR_SCRIPTS_DIR}/holor.bashrc"



function _usage() {
    echo -e "\n${RED}Usage${NO_COLOR}:
    .${BOLD}/holor.sh${NO_COLOR} [OPTION]"
    echo -e "\n${RED}Options${NO_COLOR}:
    ${BLUE}install${NO_COLOR}: install the library header files
    ${BLUE}build_examples${NO_COLOR}: and optionally build examples, benchmarks and tests
    ${BLUE}docs${NO_COLOR}: build documentation.
    ${BLUE}clean${NO_COLOR}: remove installation, compiled units and clean build files
    ${BLUE}-h|--help ${NO_COLOR}: show this message and exit
    "
}


function main() {
    #if no arguments are given, show the help and exit
    if [ "$#" -eq 0 ]; then
        _usage
        exit 0
    fi

    local cmd="$1"
    shift
    case "${cmd}" in
        install)
            info "Installing the Holor library"
            bash "${HOLOR_SCRIPTS_DIR}/holor_install.sh" "$@"
            ;;
        build_examples)
            info "Build the examples provided with the Holor library."
            bash "${HOLOR_SCRIPTS_DIR}/holor_build_examples.sh" "$@"
            ;;
        build_benchmarks)
            info "Build the benchmarks provided with the Holor library. Requires "
            bash "${HOLOR_SCRIPTS_DIR}/holor_build_examples.sh" "$@"
            ;;
        build_tests)
            info "Build the tests provided with the Holor library. Requires gtest"
            bash "${HOLOR_SCRIPTS_DIR}/holor_build_examples.sh" "$@"
            ;;
        clean)
            bash "${HOLOR_SCRIPTS_DIR}/holor_clean.sh"
            ;;
        docs)
            bash "${HOLOR_SCRIPTS_DIR}/holor_install.sh"
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