#!/usr/bin/env python3
#
# Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

import kaldifeat


def test_default():
    opts = kaldifeat.MelBanksOptions()
    assert opts.num_bins == 25
    assert opts.low_freq == 20
    assert opts.high_freq == 0
    assert opts.vtln_low == 100
    assert opts.vtln_high == -500
    assert opts.debug_mel is False
    assert opts.htk_mode is False


def test_set_get():
    opts = kaldifeat.MelBanksOptions()
    opts.num_bins = 100
    assert opts.num_bins == 100

    opts.low_freq = 22
    assert opts.low_freq == 22

    opts.high_freq = 1
    assert opts.high_freq == 1

    opts.vtln_low = 101
    assert opts.vtln_low == 101

    opts.vtln_high = -100
    assert opts.vtln_high == -100

    opts.debug_mel = True
    assert opts.debug_mel is True

    opts.htk_mode = True
    assert opts.htk_mode is True


def test_from_empty_dict():
    opts = kaldifeat.MelBanksOptions.from_dict({})
    opts2 = kaldifeat.MelBanksOptions()

    assert str(opts) == str(opts2)


def test_from_dict_partial():
    d = {"num_bins": 10, "debug_mel": True}

    opts = kaldifeat.MelBanksOptions.from_dict(d)

    opts2 = kaldifeat.MelBanksOptions()
    assert str(opts) != str(opts2)

    opts2.num_bins = 10
    assert str(opts) != str(opts2)

    opts2.debug_mel = True
    assert str(opts) == str(opts2)

    opts2.debug_mel = False
    assert str(opts) != str(opts2)


def test_from_dict_full_and_as_dict():
    opts = kaldifeat.MelBanksOptions()
    opts.num_bins = 80
    opts.vtln_high = 2

    d = opts.as_dict()
    for key, value in d.items():
        assert value == getattr(opts, key)

    opts2 = kaldifeat.MelBanksOptions.from_dict(d)
    assert str(opts2) == str(opts)

    d["htk_mode"] = True
    opts3 = kaldifeat.MelBanksOptions.from_dict(d)
    assert opts3.htk_mode is True


def main():
    test_default()
    test_set_get()
    test_from_empty_dict()
    test_from_dict_partial()
    test_from_dict_full_and_as_dict()


if __name__ == "__main__":
    main()
