Installation
============

    - |os_types|
    - |python_versions|
    - |pytorch_versions|
    - |cuda_versions|

.. caution::

   `kaldifeat`_ depends on `PyTorch`_. `PyTorch`_ >= 1.5.0 is known to work.

   Please first install `PyTorch`_ before you install `kaldifeat`_.

.. hint::

   To install a CPU version of `kaldifeat`_, please install a CPU version
   of `PyTorch`_.

   To install a CUDA version of `kaldifeat`_, please install a CUDA version
   of `PyTorch`_. CUDA >= 10.1 is known to work.

.. _from source:

Install kaldifeat from source
-----------------------------

You have to install ``cmake`` and `PyTorch`_ first.

  - ``cmake`` 3.11 is known to work. Other CMake versions may also work.
  - `PyTorch`_ >= 1.5.0  is known to work. Other PyTorch versions may also work.
  - Python >= 3.6


The commands to install `kaldifeat`_ from source are:

.. code-block:: bash

  git clone https://github.com/csukuangfj/kaldifeat
  cd kaldifeat
  python3 setup.py install

To test that you have installed `kaldifeat`_ successfully, please run:

.. code-block:: bash

  python3 -c "import kaldifeat; print(kaldifeat.__version__)"

It should print the version, e.g., ``1.0``.

.. _from PyPI:

Install kaldifeat from PyPI
---------------------------

The command to install `kaldifeat`_ from PyPI is:

.. code-block:: bash

  pip install --verbose kaldifeat

To test that you have installed `kaldifeat`_ successfully, please run:

.. code-block:: bash

  python3 -c "import kaldifeat; print(kaldifeat.__version__)"

It should print the version, e.g., ``1.0``.

Install kaldifeat from conda (Only for Linux)
---------------------------------------------

.. hint::

   Installation using ``conda`` supports only Linux. For macOS and Windows,
   please use either :ref:`from source` or :ref:`from PyPI`.

The command to install `kaldifeat` using ``conda`` is

.. code-block:: bash

  conda install -c kaldifeat -c pytorch -c conda-forge kaldifeat python=3.8 cudatoolkit=11.1 pytorch=1.8.1

You can select the supported Python version, CUDA toolkit version and `PyTorch`_ version as you wish.

To install a CPU version of `kaldifeat`, use:

.. code-block:: bash

  conda install -c kaldifeat -c pytorch cpuonly kaldifeat python=3.8 pytorch=1.8.1

.. caution::

   If you encounter issues about missing GLIBC after installing `kaldifeat`_
   with ``conda``, please consider :ref:`from source` or :ref:`from PyPI`.
   The reason is that the package was built using Ubuntu 18.04 and your system's
   GLIBC is older.


.. |os_types| image:: ./images/os-green.svg
  :alt: Supported operating systems

.. |python_versions| image:: ./images/python_ge_3.6-blue.svg
  :alt: Supported python versions

.. |cuda_versions| image:: ./images/cuda_ge_10.1-orange.svg
  :alt: Supported cuda versions

.. |pytorch_versions| image:: ./images/pytorch_ge_1.5.0-green.svg
  :alt: Supported pytorch versions

To test that you have installed `kaldifeat`_ successfully, please run:

.. code-block:: bash

  python3 -c "import kaldifeat; print(kaldifeat.__version__)"

It should print the version, e.g., ``1.0``.

FAQs
----

How to install a CUDA version of kaldifeat
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

You need to first install a CUDA version of `PyTorch`_ and then install `kaldifeat`_.

.. note::

   You can use a CUDA version of `kaldifeat`_ on machines with no GPUs.

How to install a CPU version of kaldifeat
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

You need to first install a CPU version of `PyTorch`_ and then install `kaldifeat`_.

How to fix `Caffe2: Cannot find cuDNN library`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

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
