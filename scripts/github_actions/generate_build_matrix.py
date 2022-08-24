#!/usr/bin/env python3
# Copyright    2022  Xiaomi Corp.        (authors: Fangjun Kuang)

import argparse
import json


def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--enable-cuda",
        action="store_true",
        default=False,
        help="True to enable CUDA",
    )

    parser.add_argument(
        "--for-windows",
        action="store_true",
        default=False,
        help="True for windows",
    )

    parser.add_argument(
        "--test-only-latest-torch",
        action="store_true",
        default=False,
        help="""If True, we test only the latest PyTroch
        to reduce CI running time.""",
    )
    return parser.parse_args()


def generate_build_matrix(enable_cuda, for_windows, test_only_latest_torch):
    matrix = {
        # 1.5.x is removed because there are compilation errors.
        #  See
        #  https://github.com/csukuangfj/k2/runs/2533830771?check_suite_focus=true
        #  and
        #  https://github.com/NVIDIA/apex/issues/805
        #  "1.5.0": {
        #      "python-version": ["3.6", "3.7", "3.8"],
        #      "cuda": ["10.1", "10.2"],
        #  },
        #  "1.5.1": {
        #      "python-version": ["3.6", "3.7", "3.8"],
        #      "cuda": ["10.1", "10.2"],
        #  },
        "1.6.0": {
            "python-version": ["3.6", "3.7", "3.8"],
            "cuda": ["10.1", "10.2"]
            if not for_windows
            else ["10.1.243", "10.2.89"],
        },
        "1.7.0": {
            "python-version": ["3.6", "3.7", "3.8"],
            "cuda": ["10.1", "10.2", "11.0"]
            if not for_windows
            else ["10.1.243", "10.2.89", "11.0.3"],
        },
        "1.7.1": {
            "python-version": ["3.6", "3.7", "3.8", "3.9"],
            "cuda": ["10.1", "10.2", "11.0"]
            if not for_windows
            else ["10.1.243", "10.2.89", "11.0.3"],
        },
        "1.8.0": {
            "python-version": ["3.6", "3.7", "3.8", "3.9"],
            "cuda": ["10.1", "10.2", "11.1"]
            if not for_windows
            else ["10.1.243", "10.2.89", "11.1.1"],
        },
        "1.8.1": {
            "python-version": ["3.6", "3.7", "3.8", "3.9"],
            "cuda": ["10.1", "10.2", "11.1"]
            if not for_windows
            else ["10.1.243", "10.2.89", "11.1.1"],
        },
        "1.9.0": {
            "python-version": ["3.6", "3.7", "3.8", "3.9"],
            "cuda": ["10.2", "11.1"]
            if not for_windows
            else ["10.2.89", "11.1.1"],
        },
        "1.9.1": {
            "python-version": ["3.6", "3.7", "3.8", "3.9"],
            "cuda": ["10.2", "11.1"]
            if not for_windows
            else ["10.2.89", "11.1.1"],
        },
        "1.10.0": {
            "python-version": ["3.6", "3.7", "3.8", "3.9"],
            "cuda": ["10.2", "11.1", "11.3"]
            if not for_windows
            else ["10.2.89", "11.1.1", "11.3.1"],
        },
        "1.10.1": {
            "python-version": ["3.6", "3.7", "3.8", "3.9"],
            "cuda": ["10.2", "11.1", "11.3"]
            if not for_windows
            else ["10.2.89", "11.1.1", "11.3.1"],
        },
        "1.10.2": {
            "python-version": ["3.6", "3.7", "3.8", "3.9"],
            "cuda": ["10.2", "11.1", "11.3"]
            if not for_windows
            else ["10.2.89", "11.1.1", "11.3.1"],
        },
        "1.11.0": {
            "python-version": ["3.7", "3.8", "3.9", "3.10"],
            "cuda": ["10.2", "11.3", "11.5"]
            if not for_windows
            else ["11.3.1", "11.5.2"],
        },
        "1.12.0": {
            "python-version": ["3.7", "3.8", "3.9", "3.10"],
            "cuda": ["10.2", "11.3", "11.6"]
            if not for_windows
            else ["11.3.1", "11.6.2"],
        },
        "1.12.1": {
            "python-version": ["3.7", "3.8", "3.9", "3.10"],
            "cuda": ["10.2", "11.3", "11.6"]
            if not for_windows
            else ["11.3.1", "11.6.2"],
        },
    }
    if test_only_latest_torch:
        latest = "1.12.1"
        matrix = {latest: matrix[latest]}

    ans = []
    for torch, python_cuda in matrix.items():
        python_versions = python_cuda["python-version"]
        cuda_versions = python_cuda["cuda"]
        if enable_cuda:
            for p in python_versions:
                for c in cuda_versions:
                    ans.append({"torch": torch, "python-version": p, "cuda": c})
        else:
            for p in python_versions:
                ans.append({"torch": torch, "python-version": p})

    print(json.dumps({"include": ans}))


def main():
    args = get_args()
    generate_build_matrix(
        enable_cuda=args.enable_cuda,
        for_windows=args.for_windows,
        test_only_latest_torch=args.test_only_latest_torch,
    )


if __name__ == "__main__":
    main()
