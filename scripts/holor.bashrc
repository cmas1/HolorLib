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


HOLOR_ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd -P)"

export TAB="    " # 4 spaces

: ${VERBOSE:=yes}

BOLD='\033[1m'
RED='\033[0;31m'
BLUE='\033[0;34m'
GREEN='\033[32m'
WHITE='\033[34m'
YELLOW='\033[33m'
NO_COLOR='\033[0m'

function info() {
  (echo >&2 -e "[${WHITE}${BOLD}INFO${NO_COLOR}] $*")
}

function error() {
  (echo >&2 -e "[${RED}ERROR${NO_COLOR}] $*")
}

function warning() {
  (echo >&2 -e "${YELLOW}[WARNING] $*${NO_COLOR}")
}

function ok() {
  (echo >&2 -e "[${GREEN}${BOLD} OK ${NO_COLOR}] $*")
}

function print_delim() {
  echo
  echo "=============================================="
  echo
}

function get_now() {
  date +%s
}

function time_elapsed_s() {
  local start="${1:-$(get_now)}"
  local end="$(get_now)"
  echo "$end - $start" | bc -l
}

function success() {
  print_delim
  ok "$1"
  print_delim
}

function fail() {
  print_delim
  error "$1"
  print_delim
  exit 1
}
