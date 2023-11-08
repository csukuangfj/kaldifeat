#!/usr/bin/env python3

# Copyright (c)  2023  Xiaomi Corporation (authors: Fangjun Kuang)

import librosa
import numpy as np


def main():
    m = librosa.filters.mel(sr=16000, n_fft=400, n_mels=80)
    assert m.shape == (80, 201)
    s = "// Auto-generated. Do NOT edit!\n\n"
    s += "// Copyright (c)  2023  Xiaomi Corporation (authors: Fangjun Kuang)\n\n"
    s += "\n"
    s += "#ifndef KALDIFEAT_CSRC_WHISPER_MEL_BANK_H_\n"
    s += "#define KALDIFEAT_CSRC_WHISPER_MEL_BANK_H_\n"
    s += "namespace kaldifeat {\n\n"
    s += f"constexpr int32_t kWhisperMelRows = {m.shape[0]};\n"
    s += f"constexpr int32_t kWhisperMelCols = {m.shape[1]};\n"
    s += "\n"
    s += "constexpr float kWhisperMelArray[] = {\n"
    sep = ""
    for i, f in enumerate(m.reshape(-1).tolist()):
        s += f"{sep}{f:.8f}"
        sep = ", "
        if i and i % 7 == 0:
            s += ",\n"
            sep = ""

    s += "};\n\n"
    s += "}  // namespace kaldifeat\n\n"
    s += "#endif  // KALDIFEAT_CSRC_WHISPER_MEL_BANK_H_\n"

    with open("whisper-mel-bank.h", "w") as f:
        f.write(s)


if __name__ == "__main__":
    main()
