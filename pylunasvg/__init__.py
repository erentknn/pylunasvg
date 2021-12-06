from .pylunasvg import Bitmap, Box, Matrix, Document
from .extensions import loadFromUrl

import os
import numpy as np
from PIL import Image


def svg2png(
    svg_file: str, width: int, height: int, scale: float = 1.0, output_file: str = None
):
    doc = Document.loadFromFile(svg_file).scale(scale, scale)
    if width is None:
        width = int(doc.width())
    if height is None:
        height = int(doc.height())
    bitmap = doc.renderToBitmap(width, height)
    svgArray = np.array(bitmap, copy=False)
    if output_file is None:
        Image.fromarray(svgArray).save(svg_file.replace(".svg", ".png"))
    else:
        Image.fromarray(svgArray).save(output_file, format="png")
