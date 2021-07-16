Installation
============

.. _from source:

Install kaldifeat from source
-----------------------------

You have to install ``cmake`` and ``PyTorch`` first.

  - ``cmake`` 3.11 is known to work. Other CMake versions may also work.
  - ``PyTorch`` 1.8.1 is known to work. Other PyTorch versions may also work.
  - Python >= 3.6


The commands to install ``kaldifeat`` from source are:

.. code-block:: bash

  git clone https://github.com/csukuangfj/kaldifeat
  cd kaldifeat
  python3 setup.py install

To test that you have installed ``kaldifeat`` successfully, please run:

.. code-block:: bash

  python3 -c "import kaldifeat; print(kaldifeat.__version__)"

It should print the version, e.g., ``1.0``.

Install kaldifeat from PyPI
---------------------------

The pre-built ``kaldifeat`` hosted on PyPI uses PyTorch 1.8.1.
If you install ``kaldifeat`` using pip, it will replace your locally
installed PyTorch automatically with PyTorch 1.8.1.

If you don't want this happen, please `Install kaldifeat from source`_.

The command to install ``kaldifeat`` from PyPI is:

.. code-block:: bash

  pip install kaldifeat


To test that you have installed ``kaldifeat`` successfully, please run:

.. code-block:: bash

  python3 -c "import kaldifeat; print(kaldifeat.__version__)"

It should print the version, e.g., ``1.0``.
