"""pylunasvg is python bindings for lunasvg library"""
import typing

__all__ = [
    "Bitmap",
    "Box",
    "Document",
    "Matrix"
]


class Bitmap():
    @typing.overload
    def __init__(self) -> None: ...
    @typing.overload
    def __init__(self, data: int, width: int, height: int, stride: int) -> None: ...
    @typing.overload
    def __init__(self, width: int, height: int) -> None: ...
    def height(self) -> int: ...
    @typing.overload
    def reset(self, data: int, width: int, height: int, stride: int) -> None: ...
    @typing.overload
    def reset(self, width: int, height: int) -> None: ...
    def stride(self) -> int: ...
    def valid(self) -> bool: ...
    def width(self) -> int: ...
    pass
class Box():
    @typing.overload
    def __init__(self) -> None: ...
    @typing.overload
    def __init__(self, x: float, y: float, w: float, h: float) -> None: ...
    pass
class Document():
    def box(self) -> Box: 
        """
        Returns the smallest rectangle in which the document fits
        """
    def height(self) -> float: 
        """
        Returns the height of the document
        """
    def identity(self) -> Document: 
        """
        Resets the document matrix to identity
        """
    @staticmethod
    @typing.overload
    def loadFromData(data: str) -> Document: 
        """
        Creates a document from a string

        Creates a document from a string data and size

        Creates a document from a null terminated string data
        """
    @staticmethod
    @typing.overload
    def loadFromData(data: str, size: int) -> Document: ...
    @staticmethod
    @typing.overload
    def loadFromData(string: str) -> Document: ...
    @staticmethod
    def loadFromFile(filename: str) -> Document: 
        """
        Creates a document from a file
        """
    def matrix(self) -> Matrix: 
        """
        Returns the current transformation matrix of the document
        """
    def render(self, bitmap: Bitmap, matrix: Matrix = ..., backgroundColor: int = 0) -> None: 
        """
        Renders the document to a bitmap
        """
    def renderToBitmap(self, width: int = 0, height: int = 0, backgroundColor: int = 0) -> Bitmap: 
        """
        Renders the document to a bitmap
        """
    @typing.overload
    def rotate(self, angle: float) -> Document: 
        """
        Rotates the document matrix clockwise around the current origin

        Rotates the document matrix clockwise around the given point
        """
    @typing.overload
    def rotate(self, angle: float, cx: float, cy: float) -> Document: ...
    def scale(self, sx: float, sy: float) -> Document: 
        """
        Scales the document matrix by sx horizontally and sy vertically
        """
    def shear(self, shx: float, shy: float) -> Document: 
        """
        Shears the document matrix by shx horizontally and shy vertically
        """
    def transform(self, a: float, b: float, c: float, d: float, e: float, f: float) -> Document: 
        """
        Multiplies the document matrix by Matrix(a, b, c, d, e, f)
        """
    def translate(self, tx: float, ty: float) -> Document: 
        """
        Translates the document matrix by tx horizontally and ty vertically
        """
    def width(self) -> float: 
        """
        Returns width of the document
        """
    pass
class Matrix():
    @typing.overload
    def __init__(self) -> None: ...
    @typing.overload
    def __init__(self, a: float, b: float, c: float, d: float, e: float, f: float) -> None: ...
    pass
