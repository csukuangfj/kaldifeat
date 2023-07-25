From pre-compiled wheels (Recommended)
=======================================

You can find pre-compiled wheels at

  - CPU wheels: `<https://csukuangfj.github.io/kaldifeat/cpu.html>`_
  - CUDA wheels: `<https://csukuangfj.github.io/kaldifeat/cuda.html>`_

We give a few examples below to show you how to install `kaldifeat`_ from
pre-compiled wheels.

Linux (CPU)
-----------

Suppose you want to install the following wheel:

.. code-block:: bash

   https://huggingface.co/csukuangfj/kaldifeat/resolve/main/ubuntu-cpu/kaldifeat-1.24.dev20230724+cpu.torch2.0.1-cp311-cp311-manylinux_2_17_x86_64.manylinux2014_x86_64.whl

you can use one of the following methods:

.. code-block:: bash

   # method 1
   pip install torch==2.0.1+cpu -f https://download.pytorch.org/whl/torch_stable.html
   pip install kaldifeat==1.24.dev20230724+cpu.torch2.0.1 -f https://csukuangfj.github.io/kaldifeat/cpu.html

   # method 2
   pip install torch==2.0.1+cpu -f https://download.pytorch.org/whl/torch_stable.html
   wget https://huggingface.co/csukuangfj/kaldifeat/resolve/main/ubuntu-cpu/kaldifeat-1.24.dev20230724+cpu.torch2.0.1-cp311-cp311-manylinux_2_17_x86_64.manylinux2014_x86_64.whl
   pip install ./kaldifeat-1.24.dev20230724+cpu.torch2.0.1-cp311-cp311-manylinux_2_17_x86_64.manylinux2014_x86_64.whl


Windows (CPU)
--------------

Suppose you want to install the following wheel:

.. code-block:: bash

   https://huggingface.co/csukuangfj/kaldifeat/resolve/main/windows-cpu/kaldifeat-1.24.dev20230724+cpu.torch2.0.1-cp311-cp311-win_amd64.whl

you can use one of the following methods:

.. code-block:: bash

   # method 1
   pip install torch==2.0.1+cpu -f https://download.pytorch.org/whl/torch_stable.html
   pip install kaldifeat==1.24.dev20230724+cpu.torch2.0.1 -f https://csukuangfj.github.io/kaldifeat/cpu.html

   # method 2
   pip install torch==2.0.1+cpu -f https://download.pytorch.org/whl/torch_stable.html
   wget https://huggingface.co/csukuangfj/kaldifeat/resolve/main/windows-cpu/kaldifeat-1.24.dev20230724+cpu.torch2.0.1-cp311-cp311-win_amd64.whl
   pip install ./kaldifeat-1.24.dev20230724+cpu.torch2.0.1-cp311-cp311-win_amd64.whl

macOS (CPU)
-----------

Suppose you want to install the following wheel:

.. code-block:: bash

   https://huggingface.co/csukuangfj/kaldifeat/resolve/main/macos/kaldifeat-1.24.dev20230724+cpu.torch2.0.1-cp311-cp311-macosx_10_9_x86_64.whl

you can use one of the following methods:

.. code-block:: bash

   # method 1
   pip install torch==2.0.1
   pip install kaldifeat==1.24.dev20230724+cpu.torch2.0.1 -f https://csukuangfj.github.io/kaldifeat/cpu.html

   # method 2
   pip install torch==2.0.1+cpu -f https://download.pytorch.org/whl/torch_stable.html
   wget https://huggingface.co/csukuangfj/kaldifeat/resolve/main/macos/kaldifeat-1.24.dev20230724+cpu.torch2.0.1-cp311-cp311-macosx_10_9_x86_64.whl
   pip install ./kaldifeat-1.24.dev20230724+cpu.torch2.0.1-cp311-cp311-macosx_10_9_x86_64.whl

Linux (CUDA)
------------

Suppose you want to install the following wheel:

.. code-block:: bash

   https://huggingface.co/csukuangfj/kaldifeat/resolve/main/ubuntu-cuda/kaldifeat-1.24.dev20230724+cuda11.8.torch2.0.1-cp311-cp311-manylinux_2_17_x86_64.manylinux2014_x86_64.whl

you can use one of the following methods:

.. code-block:: bash

   # method 1
   pip install torch==2.0.1+cu118 -f https://download.pytorch.org/whl/torch_stable.html
   pip install kaldifeat==1.24.dev20230724+cuda11.8.torch2.0.1  -f https://csukuangfj.github.io/kaldifeat/cuda.html

   # method 2
   pip install torch==2.0.1+cu118 -f https://download.pytorch.org/whl/torch_stable.html
   wget https://huggingface.co/csukuangfj/kaldifeat/resolve/main/ubuntu-cuda/kaldifeat-1.24.dev20230724+cuda11.8.torch2.0.1-cp311-cp311-manylinux_2_17_x86_64.manylinux2014_x86_64.whl
   pip install ./kaldifeat-1.24.dev20230724+cuda11.8.torch2.0.1-cp311-cp311-manylinux_2_17_x86_64.manylinux2014_x86_64.whl
