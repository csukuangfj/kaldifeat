kaldifeat.FbankOptions
======================

If you want to construct an instance of `kaldifeat.Fbank`_ or
`kaldifeat.OnlineFbank`_, you have to provide an instance of
`kaldifeat.FbankOptions`_.

The following code shows how to construct an instance of `kaldifeat.FbankOptions`_.

.. literalinclude:: ./code/fbank_options-1.txt
   :caption: Usage of `kaldifeat.FbankOptions`_
   :emphasize-lines: 6,8,22,37
   :language: python

Note that we reuse the same option name with `compute-fbank-feats`_ from `Kaldi`_:

.. code-block:: bash

   $ compute-fbank-feats --help


.. literalinclude:: ./code/compute-fbank-feats-help.txt
   :caption: Output of ``compute-fbank-feats --help``

Please refer to the output of ``compute-fbank-feats --help`` for the meaning
of each field of `kaldifeat.FbankOptions`_.

One thing worth noting is that `kaldifeat.FbankOptions`_ has a field ``device``,
which is an instance of ``torch.device``. You can assign it either a string, e.g.,
``"cpu"`` or ``"cuda:0"``, or an instance of ``torch.device``, e.g., ``torch.device("cpu")`` or
``torch.device("cuda", 1)``.

.. hint::

   You can use this field to control whether the feature computer
   constructed from it performs computation on CPU or CUDA.

.. caution::

   If you use a CUDA device, make sure that you have installed a CUDA version
   of `PyTorch`_.

Example usage
-------------

The following code from
`<https://github.com/csukuangfj/kaldifeat/blob/master/kaldifeat/python/tests/test_fbank_options.py>`_
demonstrate the usage of `kaldifeat.FbankOptions`_:

.. literalinclude:: ./code/test_fbank_options.py
   :caption: Example usage of `kaldifeat.FbankOptions`_
   :language: python
