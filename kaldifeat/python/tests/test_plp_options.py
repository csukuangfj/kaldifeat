#!/usr/bin/env python3
#
# Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)


import torch

import kaldifeat


def test_default():
    opts = kaldifeat.PlpOptions()
    assert opts.frame_opts.samp_freq == 16000
    assert opts.frame_opts.frame_shift_ms == 10.0
    assert opts.frame_opts.frame_length_ms == 25.0
    assert opts.frame_opts.dither == 1.0
    assert abs(opts.frame_opts.preemph_coeff - 0.97) < 1e-6
    assert opts.frame_opts.remove_dc_offset is True
    assert opts.frame_opts.window_type == "povey"
    assert opts.frame_opts.round_to_power_of_two is True
    assert abs(opts.frame_opts.blackman_coeff - 0.42) < 1e-6
    assert opts.frame_opts.snip_edges is True

    assert opts.mel_opts.num_bins == 23
    assert opts.mel_opts.low_freq == 20
    assert opts.mel_opts.high_freq == 0
    assert opts.mel_opts.vtln_low == 100
    assert opts.mel_opts.vtln_high == -500
    assert opts.mel_opts.debug_mel is False
    assert opts.mel_opts.htk_mode is False

    assert opts.lpc_order == 12
    assert opts.num_ceps == 13
    assert opts.use_energy is True
    assert opts.energy_floor == 0.0
    assert opts.raw_energy is True
    assert abs(opts.compress_factor - 0.33333) < 1e-6
    assert opts.cepstral_lifter == 22
    assert opts.cepstral_scale == 1.0
    assert opts.htk_compat is False
    assert opts.device.type == "cpu"


def test_set_get():
    opts = kaldifeat.PlpOptions()

    opts.lpc_order = 11
    assert opts.lpc_order == 11

    opts.num_ceps = 1
    assert opts.num_ceps == 1

    opts.use_energy = False
    assert opts.use_energy is False

    opts.energy_floor = 1
    assert opts.energy_floor == 1

    opts.raw_energy = False
    assert opts.raw_energy is False

    opts.compress_factor = 0.5
    assert opts.compress_factor == 0.5

    opts.cepstral_lifter = 2
    assert opts.cepstral_lifter == 2

    opts.cepstral_scale = 3
    assert opts.cepstral_scale == 3

    opts.htk_compat = True
    assert opts.htk_compat is True

    opts.device = "cuda:10"
    assert opts.device == torch.device("cuda", 10)


def test_set_get_frame_opts():
    opts = kaldifeat.PlpOptions()

    opts.frame_opts.samp_freq = 44100
    assert opts.frame_opts.samp_freq == 44100

    opts.frame_opts.frame_shift_ms = 20.5
    assert opts.frame_opts.frame_shift_ms == 20.5

    opts.frame_opts.frame_length_ms = 1
    assert opts.frame_opts.frame_length_ms == 1

    opts.frame_opts.dither = 0.5
    assert opts.frame_opts.dither == 0.5

    opts.frame_opts.preemph_coeff = 0.25
    assert opts.frame_opts.preemph_coeff == 0.25

    opts.frame_opts.remove_dc_offset = False
    assert opts.frame_opts.remove_dc_offset is False

    opts.frame_opts.window_type = "hanning"
    assert opts.frame_opts.window_type == "hanning"

    opts.frame_opts.round_to_power_of_two = False
    assert opts.frame_opts.round_to_power_of_two is False

    opts.frame_opts.blackman_coeff = 0.25
    assert opts.frame_opts.blackman_coeff == 0.25

    opts.frame_opts.snip_edges = False
    assert opts.frame_opts.snip_edges is False


def test_set_get_mel_opts():
    opts = kaldifeat.PlpOptions()

    opts.mel_opts.num_bins = 100
    assert opts.mel_opts.num_bins == 100

    opts.mel_opts.low_freq = 22
    assert opts.mel_opts.low_freq == 22

    opts.mel_opts.high_freq = 1
    assert opts.mel_opts.high_freq == 1

    opts.mel_opts.vtln_low = 101
    assert opts.mel_opts.vtln_low == 101

    opts.mel_opts.vtln_high = -100
    assert opts.mel_opts.vtln_high == -100

    opts.mel_opts.debug_mel = True
    assert opts.mel_opts.debug_mel is True

    opts.mel_opts.htk_mode = True
    assert opts.mel_opts.htk_mode is True


def test_from_empty_dict():
    opts = kaldifeat.PlpOptions.from_dict({})
    opts2 = kaldifeat.PlpOptions()

    assert str(opts) == str(opts2)


def test_from_dict_partial():
    d = {
        "energy_floor": 10.5,
        "htk_compat": True,
        "mel_opts": {"num_bins": 80, "vtln_low": 1},
        "frame_opts": {"window_type": "hanning"},
    }
    opts = kaldifeat.PlpOptions.from_dict(d)
    assert opts.energy_floor == 10.5
    assert opts.htk_compat is True
    assert opts.mel_opts.num_bins == 80
    assert opts.mel_opts.vtln_low == 1
    assert opts.frame_opts.window_type == "hanning"

    mel_opts = kaldifeat.MelBanksOptions.from_dict(d["mel_opts"])
    assert str(opts.mel_opts) == str(mel_opts)

    frame_opts = kaldifeat.FrameExtractionOptions.from_dict(d["frame_opts"])
    assert str(opts.frame_opts) == str(frame_opts)


def test_from_dict_full_and_as_dict():
    opts = kaldifeat.PlpOptions()
    opts.htk_compat = True
    opts.mel_opts.num_bins = 80
    opts.frame_opts.samp_freq = 10

    d = opts.as_dict()
    assert d["htk_compat"] is True
    assert d["mel_opts"]["num_bins"] == 80
    assert d["frame_opts"]["samp_freq"] == 10

    mel_opts = kaldifeat.MelBanksOptions()
    mel_opts.num_bins = 80
    assert d["mel_opts"] == mel_opts.as_dict()

    frame_opts = kaldifeat.FrameExtractionOptions()
    frame_opts.samp_freq = 10
    assert d["frame_opts"] == frame_opts.as_dict()

    opts2 = kaldifeat.PlpOptions.from_dict(d)
    assert str(opts2) == str(opts)

    d["htk_compat"] = False
    d["device"] = torch.device("cuda", 2)
    opts3 = kaldifeat.PlpOptions.from_dict(d)
    assert opts3.htk_compat is False
    assert opts3.device == torch.device("cuda", 2)


def main():
    test_default()
    test_set_get()
    test_set_get_frame_opts()
    test_set_get_mel_opts()
    test_from_empty_dict()
    test_from_dict_partial()
    test_from_dict_full_and_as_dict()


if __name__ == "__main__":
    main()
