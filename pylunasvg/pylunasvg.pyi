"""
pylunasvg is python bindings for lunasvg library
"""
from __future__ import annotations
import collections.abc
import typing
__all__: list[str] = ['Bitmap', 'Box', 'Document', 'Element', 'Matrix', 'Node', 'TextNode', 'lunasvg_add_font_face_from_data', 'lunasvg_add_font_face_from_file', 'lunasvg_version', 'lunasvg_version_string']
class Bitmap:
    @typing.overload
    def __init__(self) -> None:
        """
        Construct a null bitmap.
        """
    @typing.overload
    def __init__(self, width: typing.SupportsInt | typing.SupportsIndex, height: typing.SupportsInt | typing.SupportsIndex) -> None:
        """
        Construct a bitmap of the given size. Yields a null bitmap if allocation fails.
        """
    @typing.overload
    def __init__(self, bitmap: Bitmap) -> None:
        """
        Construct a bitmap that shares storage with another bitmap.
        """
    def assign(self, arg0: Bitmap) -> Bitmap:
        """
        Replace this bitmap's contents with another bitmap.
        """
    def clear(self, value: typing.SupportsInt | typing.SupportsIndex) -> None:
        """
        Fill the bitmap with a 32-bit ARGB premultiplied color.
        """
    def convertToRGBA(self) -> None:
        """
        Convert the pixels in place from premultiplied ARGB to non-premultiplied RGBA.
        """
    def data(self) -> int:
        """
        Return a pointer to the pixel data.
        """
    def height(self) -> int:
        """
        Return the height in pixels.
        """
    def isNull(self) -> bool:
        """
        Return True if the bitmap has no data.
        """
    def stride(self) -> int:
        """
        Return the number of bytes per row.
        """
    def swap(self, arg0: Bitmap) -> None:
        """
        Swap the contents of two bitmaps.
        """
    def valid(self) -> bool:
        """
        Return True if the bitmap has data.
        """
    def width(self) -> int:
        """
        Return the width in pixels.
        """
    @typing.overload
    def writeToPng(self, filename: str) -> bool:
        """
        Encode the bitmap as a PNG and write it to a file. Returns True on success.
        """
    @typing.overload
    def writeToPng(self, callback: collections.abc.Callable) -> bool:
        """
        Encode the bitmap as a PNG, passing each chunk of bytes to a callable (e.g. io.BytesIO().write or a binary file's .write). Returns True on success.
        """
class Box:
    @typing.overload
    def __init__(self) -> None:
        """
        Construct an empty box.
        """
    @typing.overload
    def __init__(self, x: typing.SupportsFloat | typing.SupportsIndex, y: typing.SupportsFloat | typing.SupportsIndex, w: typing.SupportsFloat | typing.SupportsIndex, h: typing.SupportsFloat | typing.SupportsIndex) -> None:
        """
        Construct a box from its origin and size.
        """
    def transform(self, matrix: Matrix) -> Box:
        """
        Transform this box in place by a matrix and return it.
        """
    def transformed(self, matrix: Matrix) -> Box:
        """
        Return a copy of this box transformed by a matrix.
        """
    @property
    def h(self) -> float:
        ...
    @h.setter
    def h(self, arg0: typing.SupportsFloat | typing.SupportsIndex) -> None:
        ...
    @property
    def w(self) -> float:
        ...
    @w.setter
    def w(self, arg0: typing.SupportsFloat | typing.SupportsIndex) -> None:
        ...
    @property
    def x(self) -> float:
        ...
    @x.setter
    def x(self, arg0: typing.SupportsFloat | typing.SupportsIndex) -> None:
        ...
    @property
    def y(self) -> float:
        ...
    @y.setter
    def y(self, arg0: typing.SupportsFloat | typing.SupportsIndex) -> None:
        ...
class Document:
    @staticmethod
    @typing.overload
    def loadFromData(string: str) -> Document:
        """
        Load an SVG document from a string or bytes. Returns None on failure.
        """
    @staticmethod
    @typing.overload
    def loadFromData(data: str, size: typing.SupportsInt | typing.SupportsIndex) -> Document:
        """
        Load an SVG document from a buffer of the given size. Returns None on failure.
        """
    @staticmethod
    def loadFromFile(filename: str) -> Document:
        """
        Load an SVG document from a file. Returns None on failure.
        """
    def applyStyleSheet(self, content: str) -> None:
        """
        Apply an additional CSS stylesheet to the document.
        """
    def boundingBox(self) -> Box:
        """
        Return the document's bounding box.
        """
    def documentElement(self) -> Element:
        """
        Return the root element.
        """
    def elementFromPoint(self, x: typing.SupportsFloat | typing.SupportsIndex, y: typing.SupportsFloat | typing.SupportsIndex) -> Element:
        """
        Return the topmost element at the given point.
        """
    def forceLayout(self) -> None:
        """
        Force a full relayout of the document.
        """
    def getElementById(self, id: str) -> Element:
        """
        Return the element with the given id, or a null element.
        """
    def height(self) -> float:
        """
        Return the intrinsic height in pixels.
        """
    def querySelectorAll(self, content: str) -> list[Element]:
        """
        Return all elements matching a CSS selector.
        """
    def render(self, bitmap: Bitmap, matrix: Matrix = ...) -> None:
        """
        Render the document onto an existing bitmap using a transform.
        """
    def renderToBitmap(self, width: typing.SupportsInt | typing.SupportsIndex = -1, height: typing.SupportsInt | typing.SupportsIndex = -1, backgroundColor: typing.SupportsInt | typing.SupportsIndex = 0) -> Bitmap:
        """
        Render the document into a new bitmap. A width or height of -1 uses the intrinsic size. backgroundColor is 32-bit ARGB premultiplied.
        """
    def updateLayout(self) -> None:
        """
        Update the layout if it is dirty.
        """
    def width(self) -> float:
        """
        Return the intrinsic width in pixels.
        """
class Element(Node):
    def __init__(self) -> None:
        """
        Construct a null element.
        """
    def children(self) -> list[Node]:
        """
        Return the element's child nodes.
        """
    def getAttribute(self, name: str) -> str:
        """
        Return the value of the given attribute, or an empty string.
        """
    def getBoundingBox(self) -> Box:
        """
        Return the element's bounding box.
        """
    def getGlobalBoundingBox(self) -> Box:
        """
        Return the bounding box in the global coordinate system.
        """
    def getGlobalMatrix(self) -> Matrix:
        """
        Return the element's global transform.
        """
    def getLocalBoundingBox(self) -> Box:
        """
        Return the bounding box in the element's local coordinate system.
        """
    def getLocalMatrix(self) -> Matrix:
        """
        Return the element's local transform.
        """
    def hasAttribute(self, name: str) -> bool:
        """
        Return True if the element has the given attribute.
        """
    def render(self, bitmap: Bitmap, matrix: Matrix = ...) -> None:
        """
        Render this element onto an existing bitmap using a transform.
        """
    def renderToBitmap(self, width: typing.SupportsInt | typing.SupportsIndex = -1, height: typing.SupportsInt | typing.SupportsIndex = -1, backgroundColor: typing.SupportsInt | typing.SupportsIndex = 0) -> Bitmap:
        """
        Render this element into a new bitmap. A width or height of -1 uses the intrinsic size. backgroundColor is 32-bit ARGB premultiplied.
        """
    def setAttribute(self, name: str, value: str) -> None:
        """
        Set the value of the given attribute.
        """
class Matrix:
    @staticmethod
    def rotated(angle: typing.SupportsFloat | typing.SupportsIndex, cx: typing.SupportsFloat | typing.SupportsIndex = 0.0, cy: typing.SupportsFloat | typing.SupportsIndex = 0.0) -> Matrix:
        """
        Return a rotation matrix (angle in degrees) about a point.
        """
    @staticmethod
    def scaled(sx: typing.SupportsFloat | typing.SupportsIndex, sy: typing.SupportsFloat | typing.SupportsIndex) -> Matrix:
        """
        Return a scaling matrix.
        """
    @staticmethod
    def sheared(shx: typing.SupportsFloat | typing.SupportsIndex, shy: typing.SupportsFloat | typing.SupportsIndex) -> Matrix:
        """
        Return a shearing matrix.
        """
    @staticmethod
    def translated(tx: typing.SupportsFloat | typing.SupportsIndex, ty: typing.SupportsFloat | typing.SupportsIndex) -> Matrix:
        """
        Return a translation matrix.
        """
    def __imul__(self, arg0: Matrix) -> Matrix:
        ...
    @typing.overload
    def __init__(self) -> None:
        """
        Construct an identity matrix.
        """
    @typing.overload
    def __init__(self, a: typing.SupportsFloat | typing.SupportsIndex, b: typing.SupportsFloat | typing.SupportsIndex, c: typing.SupportsFloat | typing.SupportsIndex, d: typing.SupportsFloat | typing.SupportsIndex, e: typing.SupportsFloat | typing.SupportsIndex, f: typing.SupportsFloat | typing.SupportsIndex) -> None:
        """
        Construct a matrix from its six components.
        """
    def __mul__(self, arg0: Matrix) -> Matrix:
        ...
    def inverse(self) -> Matrix:
        """
        Return the inverse of this matrix.
        """
    def invert(self) -> Matrix:
        """
        Invert this matrix in place. Returns True on success.
        """
    def multiply(self, arg0: Matrix) -> Matrix:
        """
        Post-multiply this matrix by another, in place.
        """
    def reset(self) -> None:
        """
        Reset this matrix to the identity.
        """
    def rotate(self, angle: typing.SupportsFloat | typing.SupportsIndex, cx: typing.SupportsFloat | typing.SupportsIndex = 0.0, cy: typing.SupportsFloat | typing.SupportsIndex = 0.0) -> Matrix:
        """
        Apply a rotation (in degrees) about a point to this matrix, in place.
        """
    def scale(self, sx: typing.SupportsFloat | typing.SupportsIndex, sy: typing.SupportsFloat | typing.SupportsIndex) -> Matrix:
        """
        Apply a scale to this matrix, in place.
        """
    def shear(self, shx: typing.SupportsFloat | typing.SupportsIndex, shy: typing.SupportsFloat | typing.SupportsIndex) -> Matrix:
        """
        Apply a shear to this matrix, in place.
        """
    def translate(self, tx: typing.SupportsFloat | typing.SupportsIndex, ty: typing.SupportsFloat | typing.SupportsIndex) -> Matrix:
        """
        Apply a translation to this matrix, in place.
        """
    @property
    def a(self) -> float:
        ...
    @a.setter
    def a(self, arg0: typing.SupportsFloat | typing.SupportsIndex) -> None:
        ...
    @property
    def b(self) -> float:
        ...
    @b.setter
    def b(self, arg0: typing.SupportsFloat | typing.SupportsIndex) -> None:
        ...
    @property
    def c(self) -> float:
        ...
    @c.setter
    def c(self, arg0: typing.SupportsFloat | typing.SupportsIndex) -> None:
        ...
    @property
    def d(self) -> float:
        ...
    @d.setter
    def d(self, arg0: typing.SupportsFloat | typing.SupportsIndex) -> None:
        ...
    @property
    def e(self) -> float:
        ...
    @e.setter
    def e(self, arg0: typing.SupportsFloat | typing.SupportsIndex) -> None:
        ...
    @property
    def f(self) -> float:
        ...
    @f.setter
    def f(self, arg0: typing.SupportsFloat | typing.SupportsIndex) -> None:
        ...
class Node:
    __hash__: typing.ClassVar[None] = None
    def __eq__(self, node: Node) -> bool:
        ...
    def __init__(self) -> None:
        """
        Construct a null node.
        """
    def __ne__(self, node: Node) -> bool:
        ...
    def isElement(self) -> bool:
        """
        Return True if this node is an element.
        """
    def isNull(self) -> bool:
        """
        Return True if this node is null.
        """
    def isTextNode(self) -> bool:
        """
        Return True if this node is a text node.
        """
    def parentElement(self) -> Element:
        """
        Return the parent element of this node.
        """
    def toElement(self) -> Element:
        """
        Return this node as an Element.
        """
    def toTextNode(self) -> TextNode:
        """
        Return this node as a TextNode.
        """
class TextNode(Node):
    def __init__(self) -> None:
        """
        Construct a null text node.
        """
    def data(self) -> str:
        """
        Return the text content.
        """
    def setData(self, data: str) -> None:
        """
        Set the text content.
        """
def lunasvg_add_font_face_from_data(family: str, bold: bool, italic: bool, data: bytes) -> bool:
    """
    Register a font face from in-memory font data (bytes). An empty family matches any family name (used as a fallback font).
    """
def lunasvg_add_font_face_from_file(family: str, bold: bool, italic: bool, filename: str) -> bool:
    """
    Register a font face loaded from a file. An empty family matches any family name (used as a fallback font).
    """
def lunasvg_version() -> int:
    """
    Return the lunasvg version as an integer.
    """
def lunasvg_version_string() -> str:
    """
    Return the lunasvg version as a string.
    """
