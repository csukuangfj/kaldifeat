Usage
=====

Let us first see the help message of kaldi's ``compute-fbank-feats``:

.. code-block:: bash

    $ compute-fbank-feats

    Create Mel-filter bank (FBANK) feature files.
    Usage:  compute-fbank-feats [options...] <wav-rspecifier> <feats-wspecifier>

    Options:
      --allow-downsample          : If true, allow the input waveform to have a higher frequency than the specified --sample-frequency (and we'll downsample). (bool, default = false)
      --allow-upsample            : If true, allow the input waveform to have a lower frequency than the specified --sample-frequency (and we'll upsample). (bool, default = false)
      --blackman-coeff            : Constant coefficient for generalized Blackman window. (float, default = 0.42)
      --channel                   : Channel to extract (-1 -> expect mono, 0 -> left, 1 -> right) (int, default = -1)
      --debug-mel                 : Print out debugging information for mel bin computation (bool, default = false)
      --dither                    : Dithering constant (0.0 means no dither). If you turn this off, you should set the --energy-floor option, e.g. to 1.0 or 0.1 (float, default = 1)
      --energy-floor              : Floor on energy (absolute, not relative) in FBANK computation. Only makes a difference if --use-energy=true; only necessary if --dither=0.0.  Suggested values: 0.1 or 1.0 (float, default = 0)
      --frame-length              : Frame length in milliseconds (float, default = 25)
      --frame-shift               : Frame shift in milliseconds (float, default = 10)
      --high-freq                 : High cutoff frequency for mel bins (if <= 0, offset from Nyquist) (float, default = 0)
      --htk-compat                : If true, put energy last.  Warning: not sufficient to get HTK compatible features (need to change other parameters). (bool, default = false)
      --low-freq                  : Low cutoff frequency for mel bins (float, default = 20)
      --max-feature-vectors       : Memory optimization. If larger than 0, periodically remove feature vectors so that only this number of the latest feature vectors is retained. (int, default = -1)
      --min-duration              : Minimum duration of segments to process (in seconds). (float, default = 0)
      --num-mel-bins              : Number of triangular mel-frequency bins (int, default = 23)
      --output-format             : Format of the output files [kaldi, htk] (string, default = "kaldi")
      --preemphasis-coefficient   : Coefficient for use in signal preemphasis (float, default = 0.97)
      --raw-energy                : If true, compute energy before preemphasis and windowing (bool, default = true)
      --remove-dc-offset          : Subtract mean from waveform on each frame (bool, default = true)
      --round-to-power-of-two     : If true, round window size to power of two by zero-padding input to FFT. (bool, default = true)
      --sample-frequency          : Waveform data sample frequency (must match the waveform file, if specified there) (float, default = 16000)
      --snip-edges                : If true, end effects will be handled by outputting only frames that completely fit in the file, and the number of frames depends on the frame-length.  If false, the number of frames depends only on the frame-shift, and we reflect the data at the ends. (bool, default = true)
      --subtract-mean             : Subtract mean of each feature file [CMS]; not recommended to do it this way.  (bool, default = false)
      --use-energy                : Add an extra dimension with energy to the FBANK output. (bool, default = false)
      --use-log-fbank             : If true, produce log-filterbank, else produce linear. (bool, default = true)
      --use-power                 : If true, use power, else use magnitude. (bool, default = true)
      --utt2spk                   : Utterance to speaker-id map (if doing VTLN and you have warps per speaker) (string, default = "")
      --vtln-high                 : High inflection point in piecewise linear VTLN warping function (if negative, offset from high-mel-freq (float, default = -500)
      --vtln-low                  : Low inflection point in piecewise linear VTLN warping function (float, default = 100)
      --vtln-map                  : Map from utterance or speaker-id to vtln warp factor (rspecifier) (string, default = "")
      --vtln-warp                 : Vtln warp factor (only applicable if vtln-map not specified) (float, default = 1)
      --window-type               : Type of window ("hamming"|"hanning"|"povey"|"rectangular"|"sine"|"blackmann") (string, default = "povey")
      --write-utt2dur             : Wspecifier to write duration of each utterance in seconds, e.g. 'ark,t:utt2dur'. (string, default = "")

    Standard options:
      --config                    : Configuration file to read (this option may be repeated) (string, default = "")
      --help                      : Print out usage message (bool, default = false)
      --print-args                : Print the command line arguments (to stderr) (bool, default = true)
      --verbose                   : Verbose level (higher->more logging) (int, default = 0)

FbankOptions
------------

``kaldifeat`` reuses the same options from kaldi's ``compute-fbank-feats``.

The following shows the default values of ``kaldifeat.FbankOptions``:

.. code-block:: python

  >>> import kaldifeat
  >>> fbank_opts = kaldifeat.FbankOptions()
  >>> print(fbank_opts)
  frame_opts:
  samp_freq: 16000
  frame_shift_ms: 10
  frame_length_ms: 25
  dither: 1
  preemph_coeff: 0.97
  remove_dc_offset: 1
  window_type: povey
  round_to_power_of_two: 1
  blackman_coeff: 0.42
  snip_edges: 1


  mel_opts:
  num_bins: 23
  low_freq: 20
  high_freq: 0
  vtln_low: 100
  vtln_high: -500
  debug_mel: 0
  htk_mode: 0

  use_energy: 0
  energy_floor: 0
  raw_energy: 1
  htk_compat: 0
  use_log_fbank: 1
  use_power: 1
  device: cpu

It consists of three parts:

  - ``frame_opts``

    Options in this part are accessed by ``frame_opts.xxx``. That is, to access
    the sample rate, you use:

      .. code-block:: python

        >>> fbank_opts = kaldifeat.FbankOptions()
        >>> print(fbank_opts.frame_opts.samp_freq)
        16000.0

  - ``mel_opts``

    Options in this part are accessed by ``mel_opts.xxx``. That is, to access
    the number of mel bins, you use:

      .. code-block:: python

        >>> fbank_opts = kaldifeat.FbankOptions()
        >>> print(fbank_opts.mel_opts.num_bins)
        23

  - fbank related

    Options in this part are accessed directly. That is, to access the device
    field, you use:

      .. code-block::

        >>> print(fbank_opts.device)
        cpu
        >>> fbank_opts.device = 'cuda:0'
        >>> print(fbank_opts.device)
        cuda:0
        >>> import torch
        >>> fbank_opts.device = torch.device('cuda', 0)
        >>> print(fbank_opts.device)
        cuda:0



To change the sample rate to 8000, you can use:

.. code-block:: python

  >>> fbank_opts = kaldifeat.FbankOptions()
  >>> print(fbank_opts.frame_opts.samp_freq)
  16000.0
  >>> fbank_opts.frame_opts.samp_freq = 8000
  >>> print(fbank_opts.frame_opts.samp_freq)
  8000.0

To change ``snip_edges`` to ``False``, you can use:

.. code-block:: python

  >>> fbank_opts.frame_opts.snip_edges = False
  >>> print(fbank_opts.frame_opts.snip_edges)
  False

To change number of mel bins to 80, you can use:

.. code-block:: python

  >>> print(fbank_opts.mel_opts.num_bins)
  23
  >>> fbank_opts.mel_opts.num_bins = 80
  >>> print(fbank_opts.mel_opts.num_bins)
  80

To change the device to ``cuda``, you can use:


Fbank
-----

The following shows how to use ``kaldifeat.Fbank`` to compute
the fbank features of sound files.

First, let us generate two sound files using ``sox``:

.. code-block:: bash

  # generate a wav of two seconds, containing a sine-wave
  # swept from 300 Hz to 3300 Hz
  sox -n -r 16000 -b 16 test.wav synth 1.2 sine 300-3300

  # another sound file with 0.5 seconds
  sox -n -r 16000 -b 16 test2.wav synth 0.5 sine 300-3300

.. hint::

  You can find the above two files by visiting the following two links:

    - `test.wav <https://github.com/csukuangfj/kaldifeat/blob/master/kaldifeat/python/tests/test_data/test.wav>`_
    - `test2.wav <https://github.com/csukuangfj/kaldifeat/blob/master/kaldifeat/python/tests/test_data/test2.wav>`_

The `following code <https://github.com/csukuangfj/kaldifeat/blob/master/kaldifeat/python/tests/test_fbank.py>`_
shows the usage of ``kaldifeat.Fbank``.

It shows:

  - How to read a sound file. Note that audio samples are scaled to the range [-32768, 32768].
    The intention is to produce the same output as kaldi. You don't need to scale it if
    you don't care about the compatibility with kaldi

  - ``kaldifeat.Fbank`` supports CUDA as well as CPU

  - ``kaldifeat.Fbank`` supports processing sound file in a batch as well as accepting
    a single sound file


.. literalinclude:: ./code/test_fbank.py
   :caption: Demo of ``kaldifeat.Fbank``
   :language: python
