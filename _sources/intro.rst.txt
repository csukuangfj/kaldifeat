Introduction
============

`kaldifeat`_ implements
speech feature extraction algorithms **compatible** with `Kaldi`_ using `PyTorch`_,
supporting CUDA as well as autograd.

`kaldifeat`_ has the following features:

  - Fully compatible with `Kaldi`_

    .. note::

      The underlying C++ code is copied & modified from `Kaldi`_ directly.
      It is rewritten with `PyTorch` C++ APIs.

  - Provide not only ``C++ APIs`` but also ``Python APIs``

    .. note::

      You can access `kaldifeat`_ from ``Python``.

  - Support autograd
  - Support ``CUDA`` and ``CPU``

    .. note::

      You can use CUDA for feature extraction.

  - Support ``online`` (i.e., ``streaming``) and ``offline`` (i.e., ``non-streaming``)
    feature extraction
  - Support chunk-based processing

    .. note::

      This is especially usefull if you want to process audios of several
      hours long, which may cause OOM if you send them for computation at once.
      With chunk-based processing, you can process audios of arbirtray length.

  - Support batch processing

    .. note::

      With `kaldifeat`_ you can extract features for a batch of audios


.. see https://sublime-and-sphinx-guide.readthedocs.io/en/latest/tables.html

Currently implemented speech features and their counterparts in `Kaldi`_ are
listed in the following table.

.. list-table:: Supported speech features
   :widths: 50 50
   :header-rows: 1

   * - Supported speech features
     - Counterpart in `Kaldi`_
   * - `kaldifeat.Fbank`_
     - `compute-fbank-feats`_
   * - `kaldifeat.Mfcc`_
     - `compute-mfcc-feats`_
   * - `kaldifeat.Plp`_
     - `compute-plp-feats`_
   * - `kaldifeat.Spectrogram`_
     - `compute-spectrogram-feats`_
   * - `kaldifeat.OnlineFbank`_
     - `kaldi::OnlineFbank`_
   * - `kaldifeat.OnlineMfcc`_
     - `kaldi::OnlineMfcc`_
   * - `kaldifeat.OnlinePlp`_
     - `kaldi::OnlinePlp`_

Each feature computer needs an option. The following table lists the options
for each computer and the corresponding options in `Kaldi`_.

.. hint::

   Note that we reuse the parameter names from `Kaldi`_.

   Also, both online feature computers and offline feature computers share the
   same option.

.. list-table:: Feature computer options
   :widths: 50 50
   :header-rows: 1

   * - Options in `kaldifeat`_
     - Corresponding options in `Kaldi`_
   * - `kaldifeat.FbankOptions`_
     - `kaldi::FbankOptions`_
   * - `kaldifeat.MfccOptions`_
     - `kaldi::MfccOptions`_
   * - `kaldifeat.PlpOptions`_
     - `kaldi::PlpOptions`_
   * - `kaldifeat.SpectrogramOptions`_
     - `kaldi::SpectrogramOptions`_
   * - `kaldifeat.FrameExtractionOptions`_
     - `kaldi::FrameExtractionOptions`_
   * - `kaldifeat.MelBanksOptions`_
     - `kaldi::MelBanksOptions`_

Read more to learn how to install `kaldifeat`_ and how to use each feature
computer.
