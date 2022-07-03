# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
import re

import sphinx_rtd_theme

# import sys
# sys.path.insert(0, os.path.abspath('.'))


# -- Project information -----------------------------------------------------

project = "kaldifeat"
copyright = "2021, Fangjun Kuang"
author = "Fangjun Kuang"


def get_version():
    cmake_file = "../../CMakeLists.txt"
    with open(cmake_file) as f:
        content = f.read()

    version = re.search(r"set\(kaldifeat_VERSION (.*)\)", content).group(1)
    return version.strip('"')


version = get_version()
release = version


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    "recommonmark",
    "sphinx.ext.autodoc",
    "sphinx.ext.githubpages",
    "sphinx.ext.napoleon",
    "sphinx_autodoc_typehints",
    "sphinx_rtd_theme",
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ["_templates"]

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ["images/*.md"]

source_suffix = {
    ".rst": "restructuredtext",
    ".md": "markdown",
}
master_doc = "index"


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = "sphinx_rtd_theme"
html_theme_path = [sphinx_rtd_theme.get_html_theme_path()]
html_show_sourcelink = True

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ["_static"]

pygments_style = "sphinx"

numfig = True

html_context = {
    "display_github": True,
    "github_user": "csukuangfj",
    "github_repo": "kaldifeat",
    "github_version": "master",
    "conf_py_path": "/kaldifeat/docs/source/",
}

# refer to
# https://sphinx-rtd-theme.readthedocs.io/en/latest/configuring.html
html_theme_options = {
    "logo_only": False,
    "display_version": True,
    "prev_next_buttons_location": "bottom",
    "style_external_links": True,
}

rst_epilog = """
.. _kaldifeat: https://github.com/csukuangfj/kaldifeat
.. _Kaldi: https://github.com/kaldi-asr/kaldi
.. _PyTorch: https://pytorch.org/
.. _kaldifeat.Fbank: https://github.com/csukuangfj/kaldifeat/blob/master/kaldifeat/python/kaldifeat/fbank.py#L10
.. _kaldifeat.Mfcc: https://github.com/csukuangfj/kaldifeat/blob/master/kaldifeat/python/kaldifeat/mfcc.py#L10
.. _kaldifeat.Plp: https://github.com/csukuangfj/kaldifeat/blob/master/kaldifeat/python/kaldifeat/plp.py#L10
.. _kaldifeat.Spectrogram: https://github.com/csukuangfj/kaldifeat/blob/master/kaldifeat/python/kaldifeat/spectrogram.py#L9
.. _kaldifeat.OnlineFbank: https://github.com/csukuangfj/kaldifeat/blob/master/kaldifeat/python/kaldifeat/fbank.py#L16
.. _kaldifeat.OnlineMfcc: https://github.com/csukuangfj/kaldifeat/blob/master/kaldifeat/python/kaldifeat/mfcc.py#L16
.. _kaldifeat.OnlinePlp: https://github.com/csukuangfj/kaldifeat/blob/master/kaldifeat/python/kaldifeat/plp.py#L16
.. _compute-fbank-feats: https://github.com/kaldi-asr/kaldi/blob/master/src/featbin/compute-fbank-feats.cc
.. _compute-mfcc-feats: https://github.com/kaldi-asr/kaldi/blob/master/src/featbin/compute-mfcc-feats.cc
.. _compute-plp-feats: https://github.com/kaldi-asr/kaldi/blob/master/src/featbin/compute-plp-feats.cc
.. _compute-spectrogram-feats: https://github.com/kaldi-asr/kaldi/blob/master/src/featbin/compute-spectrogram-feats.cc
.. _kaldi::OnlineFbank: https://github.com/kaldi-asr/kaldi/blob/master/src/feat/online-feature.h#L160
.. _kaldi::OnlineMfcc: https://github.com/kaldi-asr/kaldi/blob/master/src/feat/online-feature.h#L158
.. _kaldi::OnlinePlp: https://github.com/kaldi-asr/kaldi/blob/master/src/feat/online-feature.h#L159
.. _kaldifeat.FbankOptions: https://github.com/csukuangfj/kaldifeat/blob/master/kaldifeat/csrc/feature-fbank.h#L19
.. _kaldi::FbankOptions: https://github.com/kaldi-asr/kaldi/blob/master/src/feat/feature-fbank.h#L41
.. _kaldifeat.MfccOptions: https://github.com/csukuangfj/kaldifeat/blob/master/kaldifeat/csrc/feature-mfcc.h#L22
.. _kaldi::MfccOptions: https://github.com/kaldi-asr/kaldi/blob/master/src/feat/feature-mfcc.h#L38
.. _kaldifeat.PlpOptions: https://github.com/csukuangfj/kaldifeat/blob/master/kaldifeat/csrc/feature-plp.h#L24
.. _kaldi::PlpOptions: https://github.com/kaldi-asr/kaldi/blob/master/src/feat/feature-plp.h#L42
.. _kaldifeat.SpectrogramOptions: https://github.com/csukuangfj/kaldifeat/blob/master/kaldifeat/csrc/feature-spectrogram.h#L18
.. _kaldi::SpectrogramOptions: https://github.com/kaldi-asr/kaldi/blob/master/src/feat/feature-spectrogram.h#L38
.. _kaldifeat.FrameExtractionOptions: https://github.com/csukuangfj/kaldifeat/blob/master/kaldifeat/csrc/feature-window.h#L30
.. _kaldi::FrameExtractionOptions: https://github.com/kaldi-asr/kaldi/blob/master/src/feat/feature-window.h#L35
.. _kaldifeat.MelBanksOptions: https://github.com/csukuangfj/kaldifeat/blob/master/kaldifeat/csrc/mel-computations.h#L17
.. _kaldi::MelBanksOptions: https://github.com/kaldi-asr/kaldi/blob/master/src/feat/mel-computations.h#L43
"""
