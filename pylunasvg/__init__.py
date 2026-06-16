import os
from typing import Optional

from .exceptions import PylunasvgError
from .extensions import loadFromUrl
from .pylunasvg import (
    Bitmap,
    Box,
    Document,
    Element,
    Matrix,
    Node,
    TextNode,
    lunasvg_add_font_face_from_data,
    lunasvg_add_font_face_from_file,
    lunasvg_version,
    lunasvg_version_string,
)


def svg2png(
    svg_file: str,
    width: Optional[int] = None,
    height: Optional[int] = None,
    scale: float = 1.0,
    output_file: Optional[str] = None,
) -> None:
    try:
        import numpy as np
        from PIL import Image
    except ImportError as e:
        raise ImportError("svg2png() requires numpy and Pillow. Install them with: pip install pylunasvg[png]") from e

    if not os.path.isfile(svg_file):
        raise PylunasvgError(f"SVG file not found: {svg_file!r}")

    doc = Document.loadFromFile(svg_file)
    if doc is None:
        raise PylunasvgError(f"Failed to parse SVG: {svg_file!r}")

    w = width if width is not None else doc.width()
    h = height if height is not None else doc.height()
    render_w = int(w * scale)
    render_h = int(h * scale)
    if render_w <= 0 or render_h <= 0:
        raise PylunasvgError(
            f"Cannot render {svg_file!r} to a {render_w}x{render_h} bitmap; "
            "the SVG has no intrinsic size, so pass explicit width/height."
        )
    bitmap = doc.renderToBitmap(render_w, render_h)
    if bitmap.isNull():
        raise PylunasvgError(f"Failed to render {svg_file!r} to a {render_w}x{render_h} bitmap.")
    bitmap.convertToRGBA()  # inplace
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
    "PylunasvgError",
]
