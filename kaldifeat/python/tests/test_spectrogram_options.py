#!/usr/bin/env python3
#
# Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)


import torch

import kaldifeat


def test_default():
    opts = kaldifeat.SpectrogramOptions()

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

    assert opts.energy_floor == 0
    assert opts.raw_energy is True
    assert opts.device.type == "cpu"


def test_set_get():
    opts = kaldifeat.SpectrogramOptions()

    opts.energy_floor = 1
    assert opts.energy_floor == 1

    opts.raw_energy = False
    assert opts.raw_energy is False

    opts.device = torch.device("cuda", 1)
    assert opts.device.type == "cuda"
    assert opts.device.index == 1


def test_set_get_frame_opts():
    opts = kaldifeat.SpectrogramOptions()

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


def test_from_empty_dict():
    opts = kaldifeat.SpectrogramOptions.from_dict({})
    opts2 = kaldifeat.SpectrogramOptions()

    assert str(opts) == str(opts2)


def test_from_dict_partial():
    d = {
        "energy_floor": 10.5,
        "frame_opts": {"window_type": "hanning"},
        "device": "cuda:2",
    }
    opts = kaldifeat.SpectrogramOptions.from_dict(d)
    assert opts.energy_floor == 10.5
    assert opts.device == torch.device("cuda", 2)
    assert opts.frame_opts.window_type == "hanning"

    frame_opts = kaldifeat.FrameExtractionOptions.from_dict(d["frame_opts"])
    assert str(opts.frame_opts) == str(frame_opts)


def test_from_dict_full_and_as_dict():
    opts = kaldifeat.SpectrogramOptions()
    opts.frame_opts.samp_freq = 12
    opts.device = "cuda:3"

    d = opts.as_dict()
    assert d["frame_opts"]["samp_freq"] == 12
    assert d["device"] == torch.device("cuda:3")

    frame_opts = kaldifeat.FrameExtractionOptions()
    frame_opts.samp_freq = 12
    assert d["frame_opts"] == frame_opts.as_dict()

    opts2 = kaldifeat.SpectrogramOptions.from_dict(d)
    assert str(opts2) == str(opts)

    d["device"] = torch.device("cuda", 10)
    opts3 = kaldifeat.SpectrogramOptions.from_dict(d)
    assert opts3.device == torch.device("cuda", 10)

    opts.device = "cuda:10"
    assert str(opts3) == str(opts)


def main():
    test_default()
    test_set_get()
    test_set_get_frame_opts()
    test_from_empty_dict()
    test_from_dict_partial()
    test_from_dict_full_and_as_dict()


if __name__ == "__main__":
    main()
