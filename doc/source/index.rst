.. kaldifeat documentation master file, created by
   sphinx-quickstart on Fri Jul 16 20:15:27 2021.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

kaldifeat
=========

`kaldifeat <https://github.com/csukuangfj/kaldifeat>`_ implements
feature extraction algorithms **compatible** with kaldi using PyTorch, supporting CUDA
as well as autograd.

Currently, only fbank features are supported.
It can produce the same feature output as ``compute-fbank-feats`` (from kaldi)
when given the same options.



.. toctree::
   :maxdepth: 2
   :caption: Contents:

   installation
   usage
