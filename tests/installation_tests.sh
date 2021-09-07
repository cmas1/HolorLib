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

#!/bin/bash
# assert.sh
# file: test/installation_tests.sh

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
ROOT_DIR=$(realpath ${SCRIPT_DIR}/../)
OS_NAME=$(uname)

echo "Running on operating system: ${OS_NAME}"


setUp() {
    # SI_BUILD_DIR=$(mktemp -d)
    # INSTALL_PATH=$(mktemp -d)
    BUILD_DIR=$(mktemp -d)
}

tearDown() {
    # if [ -d ${SI_BUILD_DIR} ]; then
    #     rm -rf ${SI_BUILD_DIR}
    # fi
    
    # if [ -d ${INSTALL_PATH} ]; then
    #     rm -rf ${INSTALL_PATH}
    # fi
    
    if [ -d ${BUILD_DIR} ]; then
        rm -rf ${BUILD_DIR}
    fi
}



# test building a project with an executable linked against the Holor library installation
testPureCmakeInstallation() {
    # cmake ${ROOT_DIR} -B${SI_BUILD_DIR} -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_PATH} -DBUILD_TESTING=off -DCMAKE_BUILD_TYPE=Release
    # cmake --build ${SI_BUILD_DIR} --config Release --target install
    # assertEquals "Installation build successful" 0 $?
    mkdir build
    cmake ${ROOT_DIR}/tests/installation_tests -B ./build  -DCMAKE_CXX_COMPILER=clang++-12 -DCMAKE_BUILD_TYPE=Release
    cmake --build ./build
    # assertEquals "build against installation successful" 0 $?
    
}

testPureCmakeInstallation

# testPureCmakeInstallation() {
#     # install SI
#     cmake ${ROOT_DIR} -B${SI_BUILD_DIR} -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_PATH} -DBUILD_TESTING=off -DCMAKE_BUILD_TYPE=Release
#     cmake --build ${SI_BUILD_DIR} --config Release --target install
#     assertEquals "Installation build successful" 0 $?
#     cmake ${ROOT_DIR}/test/installation-tests -B${BUILD_DIR} -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_PATH}
#     cmake --build ${BUILD_DIR}
#     assertEquals "build against installation successful" 0 $?
    
# }


# testPureCmakeInstallationWithInstallOptionTurnedOff() {
#     cmake ${ROOT_DIR} -B${SI_BUILD_DIR} -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_PATH} -DBUILD_TESTING=off -DSI_INSTALL_LIBRARY=OFF -DCMAKE_BUILD_TYPE=Release
#     cmake --build ${SI_BUILD_DIR} --config Release --target install
#     assertNotEquals "Installation build successful" 0 $?
    
#     DIRECTORY_CONTENTS=$(ls -A ${INSTALL_PATH})
#     EMPTY=""
#     assertEquals "Installation directory is empty" "${EMPTY}" "${DIRECTORY_CONTENTS}"
    
# }