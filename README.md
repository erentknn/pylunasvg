# pylunasvg - Python bindings for lunasvg

[![Build](https://github.com/erentknn/pylunasvg/actions/workflows/build_wheels.yml/badge.svg)](https://github.com/erentknn/pylunasvg/actions/workflows/build_wheels.yml)

Pylunasvg is a simple wrapper around [lunasvg](https://github.com/sammycage/lunasvg) that uses [pybind11](https://github.com/pybind/pybind11) to create python bindings. All public API of the lunasvg is available in Python.

Example usage is below, if you have any questions or problems, feel free to open an issue!

## Usage Example

```python
import pylunasvg as pl
import numpy as np

document = pl.Document.loadFromFile("FSL_Micelle.svg")
bitmap = document.renderToBitmap()

svgArray = np.array(bitmap, copy=False)

# Do something useful
```

## Convert SVG to PNG

To convert an SVG file to PNG (Unless otherwise specified, output is saved to same directory as SVG file), use the following command:

```bash
pylunasvg --svg FSL_Micelle.svg
```

## Install

```Python
pip install pylunasvg
```

## Build From Source

1-) Clone this repo with `--recursive` flag. This is needed because project uses lunasvg and pybind11 as git submodules.

2-) Run `pip install .` inside repo directory. (Make sure you are on the right conda environment.) This command will build lunasvg and then build pylunasvg. After this is over you can try the example above. If you get any errors, please submit an issue.
