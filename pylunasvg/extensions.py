from .pylunasvg import Document
from urllib import request


def loadFromUrl(url: str) -> Document:
    urlContents = request.urlopen(url).read()
    document = Document.loadFromData(urlContents)
    return document
