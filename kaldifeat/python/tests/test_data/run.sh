#!/usr/bin/env bash
#
# Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

KALDI_ROOT=/root/fangjun/open-source/kaldi
export PATH=${KALDI_ROOT}/src/featbin:$PATH

sox -n -r 16000 -b 16 abc.wav synth 320 sine 300
echo "1 abc.wav" > abc.scp
compute-fbank-feats --dither=0 scp:abc.scp ark,t:abc.txt

# Output
#
# compute-fbank-feats --dither=0 scp:abc.scp ark,t:abc.txt
# Time difference = 0.304916[s]
# LOG (compute-fbank-feats[5.5.880~4-3e446]:main():compute-fbank-feats.cc:195)  Done 1 out of 1 utterances.
