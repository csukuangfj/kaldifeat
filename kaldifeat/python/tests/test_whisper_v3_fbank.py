#!/usr/bin/env python3
# Copyright      2023  Xiaomi Corporation (authors: Fangjun Kuang)

import librosa
import torch
import kaldifeat


def get_ground_truth(x):
    N_FFT = 400
    HOP_LENGTH = 160

    m = librosa.filters.mel(sr=16000, n_fft=400, n_mels=128)
    m = torch.from_numpy(m)
    #  print(m.shape)  # [128, 201]
    window = torch.hann_window(N_FFT)
    stft = torch.stft(x, N_FFT, HOP_LENGTH, window=window, return_complex=True)
    #  print(stft.shape)  # [201, 301]
    magnitudes = stft[..., :-1].abs() ** 2
    #  print(magnitudes.shape)  # [201, 300]

    mel_spec = m @ magnitudes
    #  print(mel_spec.shape)  # [128, 300]

    log_spec = torch.clamp(mel_spec, min=1e-10).log10()
    log_spec = torch.maximum(log_spec, log_spec.max() - 8.0)
    log_spec = (log_spec + 4.0) / 4.0
    return log_spec.t()


def test_whisper_v3_fbank():
    x = torch.rand(16000 * 3)

    gt = get_ground_truth(x)
    print(gt.shape)  # [300, 128]

    opts = kaldifeat.WhisperFbankOptions(num_mels=128, device="cpu")
    print(opts)
    whisper_fbank = kaldifeat.WhisperFbank(opts)
    y = whisper_fbank(x)  # [298, 128]
    print(y.shape)  # [298, 128]

    print(gt[:5, :5])
    print(y[:5, :5])


if __name__ == "__main__":
    torch.manual_seed(20231109)
    test_whisper_v3_fbank()
