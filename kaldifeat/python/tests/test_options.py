#!/usr/bin/env python3
#
# Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

import sys
from pathlib import Path

cur_dir = Path(__file__).resolve().parent
kaldi_feat_dir = cur_dir.parent.parent.parent


import torch

sys.path.insert(0, f"{kaldi_feat_dir}/build/lib")

import kaldifeat


def test_spectogram_options():
    opts = kaldifeat.SpectrogramOptions()
    opts.energy_floor = 0.0
    opts.raw_energy = True

    frame_opts = opts.frame_opts
    frame_opts.blackman_coeff = 0.42
    frame_opts.dither = 1
    frame_opts.frame_length_ms = 25
    frame_opts.frame_shift_ms = 10
    frame_opts.preemph_coeff = 0.97
    frame_opts.remove_dc_offset = True
    frame_opts.round_to_power_of_two = True
    frame_opts.samp_freq = 16000
    frame_opts.snip_edges = True
    frame_opts.window_type = "povey"

    print(opts)


def test_plp_options():
    opts = kaldifeat.PlpOptions()
    opts.lpc_order = 12
    opts.num_ceps = 13
    opts.use_energy = True
    opts.energy_floor = 0.0
    opts.raw_energy = True
    opts.compress_factor = 0.33333
    opts.cepstral_lifter = 22
    opts.cepstral_scale = 1.0
    opts.htk_compat = False
    opts.device = torch.device("cpu")

    frame_opts = opts.frame_opts
    frame_opts.blackman_coeff = 0.42
    frame_opts.dither = 1
    frame_opts.frame_length_ms = 25
    frame_opts.frame_shift_ms = 10
    frame_opts.preemph_coeff = 0.97
    frame_opts.remove_dc_offset = True
    frame_opts.round_to_power_of_two = True
    frame_opts.samp_freq = 16000
    frame_opts.snip_edges = True
    frame_opts.window_type = "povey"

    mel_opts = opts.mel_opts
    mel_opts.debug_mel = True
    mel_opts.high_freq = 0
    mel_opts.low_freq = 20
    mel_opts.num_bins = 23
    mel_opts.vtln_high = -500
    mel_opts.vtln_low = 100

    print(opts)


def main():
    test_spectogram_options()
    test_plp_options()


if __name__ == "__main__":
    main()
