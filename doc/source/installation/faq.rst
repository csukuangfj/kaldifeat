FAQs
====

How to install a CUDA version of kaldifeat from source
------------------------------------------------------

You need to first install a CUDA version of `PyTorch`_ and then install `kaldifeat`_.

.. note::

   You can use a CUDA version of `kaldifeat`_ on machines with no GPUs.

How to install a CPU version of kaldifeat from source
-----------------------------------------------------

You need to first install a CPU version of `PyTorch`_ and then install `kaldifeat`_.

How to fix `Caffe2: Cannot find cuDNN library`
----------------------------------------------

.. code-block::

  Your installed Caffe2 version uses cuDNN but I cannot find the cuDNN
  libraries.  Please set the proper cuDNN prefixes and / or install cuDNN.

You will have such an error when you want to install a CUDA version of `kaldifeat`_
by ``pip install kaldifeat`` or from source.

You need to first install cuDNN. Assume you have installed cuDNN to the
path ``/path/to/cudnn``. You can fix the error by using ``one`` of the following
commands.

(1) Fix for installation using ``pip install``

.. code-block:: bash

    export KALDIFEAT_CMAKE_ARGS="-DCMAKE_BUILD_TYPE=Release -DCUDNN_LIBRARY_PATH=/path/to/cudnn/lib/libcudnn.so -DCUDNN_INCLUDE_PATH=/path/to/cudnn/include"
    pip install --verbose kaldifeat

(2) Fix for installation from source

.. code-block:: bash

    mkdir /some/path
    git clone https://github.com/csukuangfj/kaldifeat.git
    cd kaldifeat
    export KALDIFEAT_CMAKE_ARGS="-DCMAKE_BUILD_TYPE=Release -DCUDNN_LIBRARY_PATH=/path/to/cudnn/lib/libcudnn.so -DCUDNN_INCLUDE_PATH=/path/to/cudnn/include"
    python setup.py install
