#!/usr/bin/env bash
#
# Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

KALDI_ROOT=/root/fangjun/open-source/kaldi
export PATH=${KALDI_ROOT}/src/featbin:$PATH

if [ ! -f test.wav ]; then
  # generate a wav of 1.2 seconds, containing a sine-wave
  # swept from 300 Hz to 3300 Hz
  sox -n -r 16000 -b 16 test.wav synth 1.2 sine 300-3300
fi

if [ ! -f test2.wav ]; then
  # generate a wav of 0.5 seconds, containing a sine-wave
  # swept from 300 Hz to 3300 Hz
  sox -n -r 16000 -b 16 test2.wav synth 0.5 sine 300-3300
fi

echo "1 test.wav" > test.scp

# We disable dither for testing

if [ ! -f test.txt ]; then
  compute-fbank-feats --dither=0 scp:test.scp ark,t:test.txt
fi

if [ ! -f test-mfcc.txt ]; then
  compute-mfcc-feats --dither=0 scp:test.scp ark,t:test-mfcc.txt
fi

if [ ! -f test-spectrogram.txt ]; then
  compute-spectrogram-feats --dither=0 scp:test.scp ark,t:test-spectrogram.txt
fi

if [ ! -f test-plp.txt ]; then
  compute-plp-feats --dither=0 scp:test.scp ark,t:test-plp.txt
fi

if [ ! -f test-plp-no-snip-edges.txt ]; then
  compute-plp-feats --dither=0 --snip-edges=0 scp:test.scp ark,t:test-plp-no-snip-edges.txt
fi

if [ ! -f test-plp-htk-10-cpes.txt ]; then
  compute-plp-feats --dither=0 --htk-compat=1 --num-ceps=10 scp:test.scp ark,t:test-plp-htk-10-ceps.txt
fi

if [ ! -f test-spectrogram-no-snip-edges.txt ]; then
  compute-spectrogram-feats --dither=0 --snip-edges=0 scp:test.scp ark,t:test-spectrogram-no-snip-edges.txt
fi

if [ ! -f test-mfcc-no-snip-edges.txt ]; then
  compute-mfcc-feats --dither=0 --snip-edges=0 scp:test.scp ark,t:test-mfcc-no-snip-edges.txt
fi

if [ ! -f test-htk.txt ]; then
  compute-fbank-feats --dither=0 --use-energy=1 --htk-compat=1 scp:test.scp ark,t:test-htk.txt
fi

if [ ! -f test-with-energy.txt ]; then
  compute-fbank-feats --dither=0 --use-energy=1 scp:test.scp ark,t:test-with-energy.txt
fi

if [ ! -f test-40.txt ]; then
  compute-fbank-feats --dither=0 --num-mel-bins=40 scp:test.scp ark,t:test-40.txt
fi

if [ ! -f test-40-no-snip-edges.txt ]; then
  compute-fbank-feats --dither=0 --num-mel-bins=40 --snip-edges=0 \
    scp:test.scp ark,t:test-40-no-snip-edges.txt
fi

if [ ! -f test-1hour.wav ]; then
  # generate a wav of one hour, containing a sine-wave
  # swept from 300 Hz to 3300 Hz
  sox -n -r 16000 -b 16 test-1hour.wav synth 3600 sine 300-3300
fi
