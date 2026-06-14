import os
import numpy as np
from PIL import Image
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

def svg2png(
    svg_file: str, width: int = None, height: int = None, scale: float = 1.0, output_file: str = None
):
    doc = Document.loadFromFile(svg_file)
    w = width if width is not None else doc.width()
    h = height if height is not None else doc.height()
    render_w = int(w * scale)
    render_h = int(h * scale)
    if render_w <= 0 or render_h <= 0:
        raise ValueError(
            f"Cannot render {svg_file!r} to a {render_w}x{render_h} bitmap; "
            "the SVG has no intrinsic size, so pass explicit width/height."
        )
    bitmap = doc.renderToBitmap(render_w, render_h)
    bitmap.convertToRGBA() # inplace
    svgArray = np.array(bitmap, copy=False)
    out = output_file or (os.path.splitext(svg_file)[0] + ".png")
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