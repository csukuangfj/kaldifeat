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

# The following environment variables are supposed to be set by users
#
# - KALDIFEAT_TORCH_VERSION
#     The PyTorch version. Example:
#
#       export KALDIFEAT_TORCH_VERSION=1.7.1
#
#     Defaults to 1.7.1 if not set.
#
# - KALDIFEAT_CONDA_TOKEN
#     If not set, auto upload to anaconda.org is disabled.
#
#     Its value is from https://anaconda.org/kaldifeat/settings/access
#      (You need to login as user kaldifeat to see its value)
#
# - KALDIFEAT_BUILD_TYPE
#     If not set, defaults to Release.

set -e
export CONDA_BUILD=1

cur_dir=$(cd $(dirname $BASH_SOURCE) && pwd)
kaldifeat_dir=$(cd $cur_dir/.. && pwd)

cd $kaldifeat_dir

export KALDIFEAT_ROOT_DIR=$kaldifeat_dir
echo "KALDIFEAT_ROOT_DIR: $KALDIFEAT_ROOT_DIR"

KALDIFEAT_PYTHON_VERSION=$(python -c "import sys; print('.'.join(sys.version.split('.')[:2]))")

if [ -z $KALDIFEAT_TORCH_VERSION ]; then
  echo "env var KALDIFEAT_TORCH_VERSION is not set, defaults to 1.7.1"
  KALDIFEAT_TORCH_VERSION=1.7.1
fi

if [ -z $KALDIFEAT_BUILD_TYPE ]; then
  echo "env var KALDIFEAT_BUILD_TYPE is not set, defaults to Release"
  KALDIFEAT_BUILD_TYPE=Release
fi

export KALDIFEAT_IS_FOR_CONDA=1

# Example value: 3.8
export KALDIFEAT_PYTHON_VERSION

# Example value: 1.7.1
export KALDIFEAT_TORCH_VERSION

export KALDIFEAT_BUILD_TYPE

if [ ! -z $KALDIFEAT_IS_GITHUB_ACTIONS ]; then
  export KALDIFEAT_IS_GITHUB_ACTIONS
  conda remove -q pytorch
  conda clean -q -a
else
  export KALDIFEAT_IS_GITHUB_ACTIONS=0
fi

if [ -z $KALDIFEAT_CONDA_TOKEN ]; then
  echo "Auto upload to anaconda.org is disabled since KALDIFEAT_CONDA_TOKEN is not set"
  conda build --no-test --no-anaconda-upload -c pytorch ./scripts/conda-cpu/kaldifeat
else
  conda build --no-test -c pytorch --token $KALDIFEAT_CONDA_TOKEN ./scripts/conda-cpu/kaldifeat
fi
