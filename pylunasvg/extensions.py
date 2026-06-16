from urllib import error, request

from .exceptions import PylunasvgError
from .pylunasvg import Document


def loadFromUrl(url: str, timeout: float = 30.0) -> Document:
    """Load an SVG document from a URL.

    Raises:
        PylunasvgError: if the URL cannot be fetched or the response is not a
            parseable SVG document.
    """
    try:
        with request.urlopen(url, timeout=timeout) as response:
            urlContents = response.read()
    except (error.URLError, OSError) as e:
        raise PylunasvgError(f"Failed to fetch SVG from {url!r}: {e}") from e

    document = Document.loadFromData(urlContents)
    if document is None:
        raise PylunasvgError(f"Failed to parse SVG from {url!r}")
    return document
