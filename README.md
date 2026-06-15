# pylunasvg

[![Build](https://github.com/erentknn/pylunasvg/actions/workflows/build_wheels.yml/badge.svg)](https://github.com/erentknn/pylunasvg/actions/workflows/build_wheels.yml)
[![PyPI version](https://img.shields.io/pypi/v/pylunasvg.svg)](https://pypi.org/project/pylunasvg/)
[![PyPI pyversions](https://img.shields.io/pypi/pyversions/pylunasvg.svg)](https://pypi.org/project/pylunasvg/)

pylunasvg provides Python bindings for [LunaSVG](https://github.com/sammycage/lunasvg), an SVG rendering library in C++, designed to be lightweight and portable, offering efficient rendering and manipulation of Scalable Vector Graphics (SVG) files. The bindings are built with [pybind11](https://github.com/pybind/pybind11), and the entire public API of LunaSVG is available in Python.

If you have any questions or problems, feel free to open an issue!

## Basic Usage

```python
import pylunasvg as pl

document = pl.Document.loadFromFile("tiger.svg")
if document is None:
    raise SystemExit("failed to load document")

bitmap = document.renderToBitmap()
if bitmap.isNull():
    raise SystemExit("failed to render")

bitmap.writeToPng("tiger.png")
```

![tiger.png](https://github.com/user-attachments/assets/b87bbf92-6dd1-4b29-a890-99cfffce66b8)

You can also render directly into a NumPy array without touching disk:

```python
import pylunasvg as pl
import numpy as np

document = pl.Document.loadFromFile("tiger.svg")
bitmap = document.renderToBitmap()
bitmap.convertToRGBA()

arr = np.array(bitmap, copy=False)  # zero-copy view of the pixel data
# Do something useful
```

---

## Dynamic Styling

```python
import pylunasvg as pl

landscape_content = """
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 800 600" width="800" height="600">
  <!-- Background (Sky) -->
  <rect width="800" height="600" class="sky"/>

  <!-- Sun -->
  <circle cx="650" cy="150" r="80" class="sun" />

  <!-- Clouds -->
  <ellipse cx="200" cy="150" rx="100" ry="40" class="cloud" />
  <ellipse cx="250" cy="200" rx="120" ry="50" class="cloud" />
  <ellipse cx="500" cy="80" rx="150" ry="60" class="cloud" />
  <ellipse cx="550" cy="120" rx="120" ry="50" class="cloud" />

  <!-- Mountains -->
  <polygon points="0,450 200,200 400,450" class="mountain" />
  <polygon points="200,450 400,100 600,450" class="mountain" />
  <polygon points="400,450 600,250 800,450" class="mountain" />

  <!-- Foreground (Ground) -->
  <rect y="450" width="800" height="150" class="ground" />
</svg>
"""

summer_style = """
.sky { fill: #4A90E2; }
.sun { fill: #FF7F00; }
.mountain { fill: #2E3A59; }
.cloud { fill: #FFFFFF; opacity: 0.8; }
.ground { fill: #2E8B57; }
"""

winter_style = """
.sky { fill: #87CEEB; }
.sun { fill: #ADD8E6; }
.mountain { fill: #2F4F4F; }
.cloud { fill: #FFFFFF; opacity: 0.8; }
.ground { fill: #FFFAFA; }
"""

document = pl.Document.loadFromData(landscape_content)

document.applyStyleSheet(summer_style)
document.renderToBitmap().writeToPng("summer.png")

document.applyStyleSheet(winter_style)
document.renderToBitmap().writeToPng("winter.png")
```

| `summer.png` | `winter.png` |
| --- | --- |
| ![summer.png](https://github.com/user-attachments/assets/c7f16780-23f8-4acd-906a-2242f2d0d33b) | ![winter.png](https://github.com/user-attachments/assets/fdd65288-11c7-4e16-bb5a-2bf28de57145) |

---

## Hit Testing

This example demonstrates SVG element hit testing using `elementFromPoint(x, y)`. It loads an SVG containing three shapes, performs point-based hit detection, and applies a skew transform with a black stroke to each matched element. The results are saved as `original.png` and `modified.png` for visual comparison.

```python
import pylunasvg as pl

svg_content = """
<svg width="400" height="200" xmlns="http://www.w3.org/2000/svg">
  <rect id="red-rect" x="20" y="20" width="100" height="100" fill="red"/>
  <circle id="blue-circle" cx="200" cy="70" r="50" fill="blue"/>
  <rect id="green-rect" x="300" y="30" width="70" height="130" fill="green"/>
</svg>
"""

document = pl.Document.loadFromData(svg_content)

document.renderToBitmap().writeToPng("original.png")

points = [
    (30, 30),    # inside red-rect
    (200, 70),   # center of blue-circle
    (310, 50),   # inside green-rect
    (0, 0),      # outside all shapes
]

for x, y in points:
    element = document.elementFromPoint(x, y)
    if not element.isNull():
        print(f"Element at ({x}, {y}): {element.getAttribute('id')}")
        element.setAttribute("stroke", "black")
        element.setAttribute("stroke-width", "3")
        element.setAttribute("transform", "skewX(9)")
    else:
        print(f"No element found at ({x}, {y})")

document.renderToBitmap().writeToPng("modified.png")
```

| `original.png` | `modified.png` |
| --- | --- |
| ![original.png](https://github.com/user-attachments/assets/bbffbd84-6311-484b-bfe3-219d7aec055b) | ![modified.png](https://github.com/user-attachments/assets/a7f6e502-a64f-48d5-8a01-901ad15b108b) |

```log
Element at (30, 30): red-rect
Element at (200, 70): blue-circle
Element at (310, 50): green-rect
No element found at (0, 0)
```

## Features

LunaSVG supports nearly all graphical features outlined in the SVG 1.1 and SVG 1.2 Tiny specifications. The primary exceptions are animation, filters, and scripts. As LunaSVG is designed for static rendering, animation is unlikely to be supported in the future. However, support for filters may be added. It currently handles a wide variety of elements, including:

`<a>` `<circle>` `<clipPath>` `<defs>` `<ellipse>` `<g>` `<image>` `<line>` `<linearGradient>` `<marker>` `<mask>` `<path>` `<pattern>` `<polygon>` `<polyline>` `<radialGradient>` `<rect>` `<stop>` `<style>` `<svg>` `<symbol>` `<text>` `<tspan>` `<use>`

## Installation

```bash
pip install pylunasvg
```

Prebuilt wheels are published for common platforms. If a wheel is not available for yours, pip will build from source (see below).

### Building From Source

1. Clone this repository with the `--recursive` flag. This is needed because the project uses LunaSVG and pybind11 as git submodules:

   ```bash
   git clone --recursive https://github.com/erentknn/pylunasvg.git
   cd pylunasvg
   ```

2. Install from the repository directory. This builds LunaSVG and then pylunasvg:

   ```bash
   pip install .
   ```

After this completes you can try the examples above. If you run into any errors, please submit an issue.

## Demo

pylunasvg ships with a command-line tool for converting SVG files to PNG. Unless otherwise specified, the output is saved next to the SVG file.

### Usage:
```bash
pylunasvg [filename] [options]
```

### Examples:
```bash
pylunasvg input.svg                 # input.png next to the source
pylunasvg input.svg -W 512 -H 512   # explicit output size
pylunasvg input.svg -s 2.0 -o out.png   # 2x scale, custom output path
```

| Option | Description |
| --- | --- |
| `-W`, `--width` | Width of the output in pixels |
| `-H`, `--height` | Height of the output in pixels |
| `-s`, `--scale` | Scale factor applied to the output |
| `-o`, `--output` | Output file name (defaults to the input name with `.png`) |

## License

pylunasvg is licensed under the MIT License, see [LICENSE](https://github.com/erentknn/pylunasvg/blob/main/LICENSE) for more information.
</content>
</invoke>
