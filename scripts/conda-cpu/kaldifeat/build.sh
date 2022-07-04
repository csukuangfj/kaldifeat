#!/usr/bin/env bash
#
# Copyright      2021  Xiaomi Corp.       (author: Fangjun Kuang)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set -ex

CONDA_ENV_DIR=$CONDA_PREFIX

echo "KALDIFEAT_PYTHON_VERSION: $KALDIFEAT_PYTHON_VERSION"
echo "KALDIFEAT_TORCH_VERSION: $KALDIFEAT_TORCH_VERSION"
echo "KALDIFEAT_BUILD_TYPE: $KALDIFEAT_BUILD_TYPE"
echo "KALDIFEAT_BUILD_VERSION: $KALDIFEAT_BUILD_VERSION"

export KALDIFEAT_CMAKE_ARGS="-DCMAKE_BUILD_TYPE=${KALDIFEAT_BUILD_TYPE}"
export KALDIFEAT_MAKE_ARGS="-j1 VERBOSE=1"

export LIBRARY_PATH="/usr/local/miniconda/envs/kaldifeat/lib":$LIBRARY_PATH
export LD_LIBRARY_PATH="/usr/local/miniconda/envs/kaldifeat/lib":$LD_LIBRARY_PATH
export DYLD_LIBRARY_PATH="/usr/local/miniconda/envs/kaldifeat/lib":$DYLD_LIBRARY_PATH

python3 setup.py install --single-version-externally-managed --record=record.txt
