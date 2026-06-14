from .pylunasvg import (
    Bitmap,
    Box,
    Matrix,
    Node,
    TextNode,
    Element,
    Document,
    lunasvg_version,
    lunasvg_version_string,
    lunasvg_add_font_face_from_file,
    lunasvg_add_font_face_from_data,
)
from .extensions import loadFromUrl

import numpy as np
from PIL import Image


def svg2png(
    svg_file: str, width: int = None, height: int = None, scale: float = 1.0, output_file: str = None
):
    doc = Document.loadFromFile(svg_file)
    w = width if width is not None else doc.width()
    h = height if height is not None else doc.height()
    bitmap = doc.renderToBitmap(int(w * scale), int(h * scale))
    bitmap.convertToRGBA() # inplace
    svgArray = np.array(bitmap, copy=False)
    out = output_file or svg_file.replace(".svg", ".png")
    Image.fromarray(svgArray).save(out, format="png")


__all__ = [
    "Bitmap",
    "Box",
    "Matrix",
    "Node",
    "TextNode",
    "Element",
    "Document",
    "lunasvg_version",
    "lunasvg_version_string",
    "lunasvg_add_font_face_from_file",
    "lunasvg_add_font_face_from_data",
    "loadFromUrl",
    "svg2png",
]