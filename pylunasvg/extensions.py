from urllib import request

from .pylunasvg import Document


def loadFromUrl(url: str) -> Document:
    urlContents = request.urlopen(url).read()
    document = Document.loadFromData(urlContents)
    return document
