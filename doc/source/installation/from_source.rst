.. _from source:

Install kaldifeat from source
=============================

You have to install ``cmake`` and `PyTorch`_ first.

  - ``cmake`` 3.11 is known to work. Other CMake versions may also work.
  - `PyTorch`_ >= 1.5.0  is known to work. Other PyTorch versions may also work.
  - Python >= 3.6
  - A compiler that supports C++ 14


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
