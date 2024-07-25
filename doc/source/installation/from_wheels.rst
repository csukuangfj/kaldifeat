From pre-compiled wheels (Recommended)
=======================================

You can find pre-compiled wheels at

  - CPU wheels: `<https://csukuangfj.github.io/kaldifeat/cpu.html>`_
  - CUDA wheels: `<https://csukuangfj.github.io/kaldifeat/cuda.html>`_

We give a few examples below to show you how to install `kaldifeat`_ from
pre-compiled wheels.

.. hint::

   The following lists only some examples. We suggest that you always select the
   latest version of ``kaldifeat``.

Linux (CPU)
-----------

Suppose you want to install the following wheel:

.. code-block:: bash

   https://huggingface.co/csukuangfj/kaldifeat/resolve/main/ubuntu-cpu/kaldifeat-1.25.4.dev20240725+cpu.torch2.4.0-cp312-cp312-manylinux_2_17_x86_64.manylinux2014_x86_64.whl

you can use one of the following methods:

.. code-block:: bash

   # method 1
   pip install torch==2.4.0+cpu -f https://download.pytorch.org/whl/torch/
   pip install kaldifeat==1.25.4.dev20240725+cpu.torch2.4.0 -f https://csukuangfj.github.io/kaldifeat/cpu.html

   # For users from China
   # 中国国内用户，如果访问不了 huggingface, 请使用
   # pip install kaldifeat==1.25.4.dev20240725+cpu.torch2.4.0 -f https://csukuangfj.github.io/kaldifeat/cpu-cn.html

   # method 2
   pip install torch==2.4.0+cpu -f https://download.pytorch.org/whl/torch/
   wget https://huggingface.co/csukuangfj/kaldifeat/resolve/main/ubuntu-cpu/kaldifeat-1.25.4.dev20240725+cpu.torch2.4.0-cp312-cp312-manylinux_2_17_x86_64.manylinux2014_x86_64.whl

   # For users from China
   # 中国国内用户，如果访问不了 huggingface, 请使用
   # wget https://hf-mirror.com/csukuangfj/kaldifeat/resolve/main/ubuntu-cpu/kaldifeat-1.25.4.dev20240725+cpu.torch2.4.0-cp312-cp312-manylinux_2_17_x86_64.manylinux2014_x86_64.whl

   pip install ./kaldifeat-1.25.4.dev20240725+cpu.torch2.4.0-cp312-cp312-manylinux_2_17_x86_64.manylinux2014_x86_64.whl

Windows (CPU)
--------------

Suppose you want to install the following wheel:

.. code-block:: bash

   https://huggingface.co/csukuangfj/kaldifeat/resolve/main/windows-cpu/kaldifeat-1.25.4.dev20240725+cpu.torch2.4.0-cp312-cp312-win_amd64.whl

you can use one of the following methods:

.. code-block:: bash

   # method 1
   pip install torch==2.4.0+cpu -f https://download.pytorch.org/whl/torch/
   pip install kaldifeat==1.25.4.dev20240725+cpu.torch2.4.0 -f https://csukuangfj.github.io/kaldifeat/cpu.html

   # For users from China
   # 中国国内用户，如果访问不了 huggingface, 请使用
   # pip install kaldifeat==1.25.4.dev20240725+cpu.torch2.4.0 -f https://csukuangfj.github.io/kaldifeat/cpu-cn.html

   # method 2
   pip install torch==2.4.0+cpu -f https://download.pytorch.org/whl/torch/
   wget https://huggingface.co/csukuangfj/kaldifeat/resolve/main/windows-cpu/kaldifeat-1.25.4.dev20240725+cpu.torch2.4.0-cp312-cp312-win_amd64.whl

   # For users from China
   # 中国国内用户，如果访问不了 huggingface, 请使用
   # wget https://hf-mirror.com/csukuangfj/kaldifeat/resolve/main/windows-cpu/kaldifeat-1.25.4.dev20240725+cpu.torch2.4.0-cp312-cp312-win_amd64.whl

   pip install ./kaldifeat-1.25.4.dev20240725+cpu.torch2.4.0-cp312-cp312-win_amd64.whl

macOS (CPU)
-----------

Suppose you want to install the following wheel:

.. code-block:: bash

   https://huggingface.co/csukuangfj/kaldifeat/resolve/main/macos/kaldifeat-1.25.4.dev20240725+cpu.torch2.4.0-cp311-cp311-macosx_11_0_arm64.whl

you can use one of the following methods:

.. code-block:: bash

   # method 1
   pip install torch==2.4.0
   pip install kaldifeat==1.25.4.dev20240725+cpu.torch2.4.0 -f https://csukuangfj.github.io/kaldifeat/cpu.html

   # For users from China
   # 中国国内用户，如果访问不了 huggingface, 请使用
   # pip install kaldifeat==1.25.4.dev20240725+cpu.torch2.4.0 -f https://csukuangfj.github.io/kaldifeat/cpu-cn.html

   # method 2
   pip install torch==2.4.0 -f https://download.pytorch.org/whl/torch/
   wget https://huggingface.co/csukuangfj/kaldifeat/resolve/main/macos/kaldifeat-1.25.4.dev20240725+cpu.torch2.4.0-cp311-cp311-macosx_11_0_arm64.whl

   # For users from China
   # 中国国内用户，如果访问不了 huggingface, 请使用
   # wget https://hf-mirror.com/csukuangfj/kaldifeat/resolve/main/macos/kaldifeat-1.25.4.dev20240725+cpu.torch2.4.0-cp311-cp311-macosx_11_0_arm64.whl

   pip install ./kaldifeat-1.25.4.dev20240725+cpu.torch2.4.0-cp311-cp311-macosx_11_0_arm64.whl

Linux (CUDA)
------------

Suppose you want to install the following wheel:

.. code-block:: bash

   https://huggingface.co/csukuangfj/kaldifeat/resolve/main/ubuntu-cuda/kaldifeat-1.25.4.dev20240725+cuda12.4.torch2.4.0-cp312-cp312-manylinux_2_17_x86_64.manylinux2014_x86_64.whl

you can use one of the following methods:

.. code-block:: bash

   # method 1
   pip install torch==2.4.0+cu124 -f https://download.pytorch.org/whl/torch/
   pip install kaldifeat==1.25.4.dev20240725+cuda12.4.torch2.4.0 -f https://csukuangfj.github.io/kaldifeat/cuda.html

   # For users from China
   # 中国国内用户，如果访问不了 huggingface, 请使用
   # pip install kaldifeat==1.25.4.dev20240725+cuda12.4.torch2.4.0 -f https://csukuangfj.github.io/kaldifeat/cuda-cn.html

   # method 2
   pip install torch==2.4.0+cu124 -f https://download.pytorch.org/whl/torch/
   wget https://huggingface.co/csukuangfj/kaldifeat/resolve/main/ubuntu-cuda/kaldifeat-1.25.4.dev20240725+cuda12.4.torch2.4.0-cp312-cp312-manylinux_2_17_x86_64.manylinux2014_x86_64.whl

   # For users from China
   # 中国国内用户，如果访问不了 huggingface, 请使用
   # wget https://hf-mirror.com/csukuangfj/kaldifeat/resolve/main/ubuntu-cuda/kaldifeat-1.25.4.dev20240725+cuda12.4.torch2.4.0-cp312-cp312-manylinux_2_17_x86_64.manylinux2014_x86_64.whl

   pip install ./kaldifeat-1.25.4.dev20240725+cuda12.4.torch2.4.0-cp312-cp312-manylinux_2_17_x86_64.manylinux2014_x86_64.whl
