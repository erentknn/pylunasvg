#include <string>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "lunasvg.h"

namespace py = pybind11;
using namespace lunasvg;

PYBIND11_MODULE(pylunasvg, m)
{
	m.doc() = "pylunasvg is python bindings for lunasvg library";
	m.def("lunasvg_version", &lunasvg_version,
		"Return the lunasvg version as an integer.");
	m.def("lunasvg_version_string", &lunasvg_version_string,
		"Return the lunasvg version as a string.");
	m.def("lunasvg_add_font_face_from_file", &lunasvg_add_font_face_from_file,
		py::arg("family"), py::arg("bold"), py::arg("italic"), py::arg("filename"),
		"Register a font face loaded from a file. An empty family matches any "
		"family name (used as a fallback font).");
		m.def("lunasvg_add_font_face_from_data",
		[](const std::string& family, bool bold, bool italic, py::bytes data) {
			char* src = nullptr;
			py::ssize_t length = 0;
			// borrow the raw bytes buffer (no decode, binary-safe)
			if (PyBytes_AsStringAndSize(data.ptr(), &src, &length) != 0)
				throw py::error_already_set();

			// plutovg keeps a reference to this buffer after we return,
			// so give it a heap copy it will own and free via destroy_func.
			void* copy = std::malloc(static_cast<size_t>(length));
			if (copy == nullptr && length > 0)
				throw std::bad_alloc();
			std::memcpy(copy, src, static_cast<size_t>(length));

			// destroy_func is invoked exactly once in every path (success or
			// parse-failure), so we never free `copy` ourselves.
			return lunasvg_add_font_face_from_data(
				family.c_str(), bold, italic,
				copy, static_cast<size_t>(length),
				[](void* closure) { std::free(closure); },  // destroy_func
				copy);                                       // closure
      },
      py::arg("family"), py::arg("bold"), py::arg("italic"), py::arg("data"),
      "Register a font face from in-memory font data (bytes). An empty family "
      "matches any family name (used as a fallback font).");

    py::class_<Bitmap>(m, "Bitmap", py::buffer_protocol())
		.def(py::init<>(), "Construct a null bitmap.")
		.def(py::init<int, int>(), py::arg("width"), py::arg("height"),
			"Construct a bitmap of the given size. Yields a null bitmap if "
			"allocation fails.")
		.def(py::init<const Bitmap&>(), py::arg("bitmap"),
			"Construct a bitmap that shares storage with another bitmap.")
		.def("assign", py::overload_cast<const Bitmap&>(&Bitmap::operator=),
			"Replace this bitmap's contents with another bitmap.")
		.def("swap", &Bitmap::swap, "Swap the contents of two bitmaps.")
		.def("data", &Bitmap::data, "Return a pointer to the pixel data.")
		.def("width", &Bitmap::width, "Return the width in pixels.")
		.def("height", &Bitmap::height, "Return the height in pixels.")
		.def("stride", &Bitmap::stride, "Return the number of bytes per row.")
		.def("clear", &Bitmap::clear, py::arg("value"),
			"Fill the bitmap with a 32-bit ARGB premultiplied color.")
		.def("convertToRGBA", &Bitmap::convertToRGBA,
			"Convert the pixels in place from premultiplied ARGB to "
			"non-premultiplied RGBA.")
		.def("isNull", &Bitmap::isNull, "Return True if the bitmap has no data.")
		.def("valid", &Bitmap::valid, "Return True if the bitmap has data.")
		.def("writeToPng", py::overload_cast<const std::string&>(&Bitmap::writeToPng, py::const_),
			py::arg("filename"),
			"Encode the bitmap as a PNG and write it to a file. Returns True on "
			"success.")
		.def("writeToPng",
			[](const Bitmap& self, py::function callback) {
				auto writer = [](void* closure, void* data, int size) {
					auto& fn = *reinterpret_cast<py::function*>(closure);
					fn(py::bytes(reinterpret_cast<const char*>(data),
						static_cast<size_t>(size)));
				};
				return self.writeToPng(writer, &callback);
			},
			py::arg("callback"),
			"Encode the bitmap as a PNG, passing each chunk of bytes to a "
			"callable (e.g. io.BytesIO().write or a binary file's .write). "
			"Returns True on success.")
		.def_buffer([](Bitmap& b) -> py::buffer_info {
		return py::buffer_info(
			b.data(),
			sizeof(uint8_t),
			py::format_descriptor<uint8_t>::format(),
			3,
			{ (size_t)b.height(), (size_t)b.width(), (size_t)4 },
			{ (size_t)b.stride(), (size_t)4, (size_t)1 }
		);
		});

	py::class_<Box>(m, "Box")
		.def(py::init<>(), "Construct an empty box.")
		.def(py::init<float, float, float, float>(),
			py::arg("x"), py::arg("y"), py::arg("w"), py::arg("h"),
			"Construct a box from its origin and size.")
        .def("transform", &Box::transform, py::arg("matrix"),
			"Transform this box in place by a matrix and return it.")
        .def("transformed", &Box::transformed, py::arg("matrix"),
			"Return a copy of this box transformed by a matrix.")
        .def_readwrite("x", &Box::x)
        .def_readwrite("y", &Box::y)
        .def_readwrite("w", &Box::w)
        .def_readwrite("h", &Box::h);

    py::class_<Matrix>(m, "Matrix")
        .def(py::init<>(), "Construct an identity matrix.")
        .def(py::init<float, float, float, float, float, float>(),
             py::arg("a"), py::arg("b"), py::arg("c"), py::arg("d"), py::arg("e"), py::arg("f"),
             "Construct a matrix from its six components.")
        .def(py::self * Matrix())
        .def(py::self *= Matrix())
        .def("multiply", &Matrix::multiply, "Post-multiply this matrix by another, in place.")
        .def("translate", &Matrix::translate, py::arg("tx"), py::arg("ty"),
			"Apply a translation to this matrix, in place.")
        .def("scale", &Matrix::scale, py::arg("sx"), py::arg("sy"),
			"Apply a scale to this matrix, in place.")
        .def("rotate", &Matrix::rotate, py::arg("angle"), py::arg("cx") = 0.f, py::arg("cy") = 0.f,
			"Apply a rotation (in degrees) about a point to this matrix, in place.")
        .def("shear", &Matrix::shear, py::arg("shx"), py::arg("shy"),
			"Apply a shear to this matrix, in place.")
        .def("invert", &Matrix::invert, "Invert this matrix in place. Returns True on success.")
        .def("inverse", &Matrix::inverse, "Return the inverse of this matrix.")
		.def("reset", &Matrix::reset, "Reset this matrix to the identity.")
        .def_static("translated", &Matrix::translated, py::arg("tx"), py::arg("ty"),
			"Return a translation matrix.")
        .def_static("scaled", &Matrix::scaled, py::arg("sx"), py::arg("sy"),
			"Return a scaling matrix.")
        .def_static("rotated", &Matrix::rotated, py::arg("angle"), py::arg("cx") = 0.f, py::arg("cy") = 0.f,
			"Return a rotation matrix (angle in degrees) about a point.")
        .def_static("sheared", &Matrix::sheared, py::arg("shx"), py::arg("shy"),
			"Return a shearing matrix.")
        .def_readwrite("a", &Matrix::a)
        .def_readwrite("b", &Matrix::b)
        .def_readwrite("c", &Matrix::c)
        .def_readwrite("d", &Matrix::d)
        .def_readwrite("e", &Matrix::e)
        .def_readwrite("f", &Matrix::f);

	py::class_<Node>(m, "Node")
		.def(py::init<>(), "Construct a null node.")
		.def("isTextNode", &Node::isTextNode, "Return True if this node is a text node.")
		.def("isElement", &Node::isElement, "Return True if this node is an element.")
		.def("toTextNode", &Node::toTextNode, "Return this node as a TextNode.")
		.def("toElement", &Node::toElement, "Return this node as an Element.")
		.def("parentElement", &Node::parentElement, "Return the parent element of this node.")
		.def("isNull", &Node::isNull, "Return True if this node is null.")
		.def("__eq__", &Node::operator==, py::arg("node"))
		.def("__ne__", &Node::operator!=, py::arg("node"));

	py::class_<TextNode, Node>(m, "TextNode")
		.def(py::init<>(), "Construct a null text node.")
		.def("data", &TextNode::data, "Return the text content.")
		.def("setData", &TextNode::setData, py::arg("data"), "Set the text content.");

	py::class_<Element, Node>(m, "Element")
		.def(py::init<>(), "Construct a null element.")
		.def("hasAttribute", &Element::hasAttribute, py::arg("name"),
			"Return True if the element has the given attribute.")
		.def("getAttribute", &Element::getAttribute, py::arg("name"),
			"Return the value of the given attribute, or an empty string.")
		.def("setAttribute", &Element::setAttribute, py::arg("name"), py::arg("value"),
			"Set the value of the given attribute.")
		.def("render", &Element::render, py::arg("bitmap"), py::arg("matrix") = Matrix(),
			"Render this element onto an existing bitmap using a transform.")
		.def("renderToBitmap", &Element::renderToBitmap,
			py::arg("width") = -1, py::arg("height") = -1, py::arg("backgroundColor") = 0x00000000,
			"Render this element into a new bitmap. A width or height of -1 uses "
			"the intrinsic size. backgroundColor is 32-bit ARGB premultiplied.")
		.def("getLocalMatrix", &Element::getLocalMatrix, "Return the element's local transform.")
		.def("getGlobalMatrix", &Element::getGlobalMatrix, "Return the element's global transform.")
		.def("getLocalBoundingBox", &Element::getLocalBoundingBox,
			"Return the bounding box in the element's local coordinate system.")
		.def("getGlobalBoundingBox", &Element::getGlobalBoundingBox,
			"Return the bounding box in the global coordinate system.")
		.def("getBoundingBox", &Element::getBoundingBox, "Return the element's bounding box.")
		.def("children", &Element::children, "Return the element's child nodes.");

	py::class_<Document>(m, "Document")
		.def_static("loadFromFile", &Document::loadFromFile, py::arg("filename"),
			"Load an SVG document from a file. Returns None on failure.")
		.def_static("loadFromData", py::overload_cast<const std::string&>(&Document::loadFromData),
			py::arg("string"),
			"Load an SVG document from a string or bytes. Returns None on failure.")
		.def_static("loadFromData", py::overload_cast<const char*, std::size_t>(&Document::loadFromData),
			py::arg("data"), py::arg("size"),
			"Load an SVG document from a buffer of the given size. Returns None on failure.")
		.def("width", &Document::width, "Return the intrinsic width in pixels.")
		.def("height", &Document::height, "Return the intrinsic height in pixels.")
		.def("boundingBox", &Document::boundingBox, "Return the document's bounding box.")
		.def("updateLayout", &Document::updateLayout, "Update the layout if it is dirty.")
		.def("render", &Document::render, py::arg("bitmap"), py::arg("matrix") = Matrix(),
			"Render the document onto an existing bitmap using a transform.")
		.def("renderToBitmap", &Document::renderToBitmap,
			py::arg("width") = -1, py::arg("height") = -1, py::arg("backgroundColor") = 0x00000000,
			"Render the document into a new bitmap. A width or height of -1 uses "
			"the intrinsic size. backgroundColor is 32-bit ARGB premultiplied.")
		.def("getElementById", &Document::getElementById, py::arg("id"),
			"Return the element with the given id, or a null element.")
		.def("documentElement", &Document::documentElement, "Return the root element.")
		.def("applyStyleSheet", &Document::applyStyleSheet, py::arg("content"),
			"Apply an additional CSS stylesheet to the document.")
		.def("querySelectorAll", &Document::querySelectorAll, py::arg("content"),
			"Return all elements matching a CSS selector.")
		.def("forceLayout", &Document::forceLayout, "Force a full relayout of the document.")
		.def("elementFromPoint", &Document::elementFromPoint, py::arg("x"), py::arg("y"),
			"Return the topmost element at the given point.");
}
