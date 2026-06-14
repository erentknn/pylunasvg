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
	m.def("lunasvg_version", &lunasvg_version);
	m.def("lunasvg_version_string", &lunasvg_version_string);
	m.def("lunasvg_add_font_face_from_file", &lunasvg_add_font_face_from_file);
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
      py::arg("family"), py::arg("bold"), py::arg("italic"), py::arg("data"));

    py::class_<Bitmap>(m, "Bitmap", py::buffer_protocol())
		.def(py::init<>())
		.def(py::init<int, int>(), py::arg("width"), py::arg("height"))
		.def(py::init<const Bitmap&>(), py::arg("bitmap"))
		.def("assign", py::overload_cast<const Bitmap&>(&Bitmap::operator=))
		.def("swap", &Bitmap::swap)
		.def("data", &Bitmap::data)
		.def("width", &Bitmap::width)
		.def("height", &Bitmap::height)
		.def("stride", &Bitmap::stride)
		.def("clear", &Bitmap::clear, py::arg("value"))
		.def("convertToRGBA", &Bitmap::convertToRGBA)
		.def("isNull", &Bitmap::isNull)
		.def("valid", &Bitmap::valid)
		.def("writeToPng", py::overload_cast<const std::string&>(&Bitmap::writeToPng, py::const_), py::arg("filename"))
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
		.def(py::init<>())
		.def(py::init<float, float, float, float>(),
			py::arg("x"), py::arg("y"), py::arg("w"), py::arg("h"))
        .def("transform", &Box::transform, py::arg("matrix"))
        .def("transformed", &Box::transformed, py::arg("matrix"))
        .def_readwrite("x", &Box::x)
        .def_readwrite("y", &Box::y)
        .def_readwrite("w", &Box::w)
        .def_readwrite("h", &Box::h);

    py::class_<Matrix>(m, "Matrix")
        .def(py::init<>())
        .def(py::init<float, float, float, float, float, float>(),
             py::arg("a"), py::arg("b"), py::arg("c"), py::arg("d"), py::arg("e"), py::arg("f"))
        .def(py::self * Matrix())
        .def(py::self *= Matrix())
        .def("multiply", &Matrix::multiply)
        .def("translate", &Matrix::translate, py::arg("tx"), py::arg("ty"))
        .def("scale", &Matrix::scale, py::arg("sx"), py::arg("sy"))
        .def("rotate", &Matrix::rotate, py::arg("angle"), py::arg("cx") = 0.f, py::arg("cy") = 0.f)
        .def("shear", &Matrix::shear, py::arg("shx"), py::arg("shy"))
        .def("invert", &Matrix::invert)
        .def("inverse", &Matrix::inverse)
		.def("reset", &Matrix::reset)
        .def_static("translated", &Matrix::translated, py::arg("tx"), py::arg("ty"))
        .def_static("scaled", &Matrix::scaled, py::arg("sx"), py::arg("sy"))
        .def_static("rotated", &Matrix::rotated, py::arg("angle"), py::arg("cx") = 0.f, py::arg("cy") = 0.f)
        .def_static("sheared", &Matrix::sheared, py::arg("shx"), py::arg("shy"))
        .def_readwrite("a", &Matrix::a)
        .def_readwrite("b", &Matrix::b)
        .def_readwrite("c", &Matrix::c)
        .def_readwrite("d", &Matrix::d)
        .def_readwrite("e", &Matrix::e)
        .def_readwrite("f", &Matrix::f);

	py::class_<Node>(m, "Node")
		.def(py::init<>())
		.def("isTextNode", &Node::isTextNode)
		.def("isElement", &Node::isElement)
		.def("toTextNode", &Node::toTextNode)
		.def("toElement", &Node::toElement)
		.def("parentElement", &Node::parentElement)
		.def("isNull", &Node::isNull)
		.def("__eq__", &Node::operator==, py::arg("node"))
		.def("__ne__", &Node::operator!=, py::arg("node"));

	py::class_<TextNode, Node>(m, "TextNode")
		.def(py::init<>())
		.def("data", &TextNode::data)
		.def("setData", &TextNode::setData, py::arg("data"));

	py::class_<Element, Node>(m, "Element")
		.def(py::init<>())
		.def("hasAttribute", &Element::hasAttribute, py::arg("name"))
		.def("getAttribute", &Element::getAttribute, py::arg("name"))
		.def("setAttribute", &Element::setAttribute, py::arg("name"), py::arg("value"))
		.def("render", &Element::render, py::arg("bitmap"), py::arg("matrix") = Matrix())
		.def("renderToBitmap", &Element::renderToBitmap, py::arg("width") = -1, py::arg("height") = -1, py::arg("backgroundColor") = 0x00000000)
		.def("getLocalMatrix", &Element::getLocalMatrix)
		.def("getGlobalMatrix", &Element::getGlobalMatrix)
		.def("getLocalBoundingBox", &Element::getLocalBoundingBox)
		.def("getGlobalBoundingBox", &Element::getGlobalBoundingBox)
		.def("getBoundingBox", &Element::getBoundingBox)
		.def("children", &Element::children);

	py::class_<Document>(m, "Document")
		.def_static("loadFromFile", &Document::loadFromFile, py::arg("filename"))
		.def_static("loadFromData", py::overload_cast<const std::string&>(&Document::loadFromData),
			py::arg("string"))
		.def_static("loadFromData", py::overload_cast<const char*, std::size_t>(&Document::loadFromData), 
			py::arg("data"), py::arg("size"))
		.def("width", &Document::width)
		.def("height", &Document::height)
		.def("boundingBox", &Document::boundingBox)
		.def("updateLayout", &Document::updateLayout)
		.def("render", &Document::render, py::arg("bitmap"), py::arg("matrix") = Matrix())
		.def("renderToBitmap", &Document::renderToBitmap, py::arg("width") = -1, py::arg("height") = -1, py::arg("backgroundColor") = 0x00000000)
		.def("getElementById", &Document::getElementById, py::arg("id"))
		.def("documentElement", &Document::documentElement)
		.def("applyStyleSheet", &Document::applyStyleSheet, py::arg("content"))
		.def("querySelectorAll", &Document::querySelectorAll, py::arg("content"))
		.def("forceLayout", &Document::forceLayout)
		.def("elementFromPoint", &Document::elementFromPoint, py::arg("x"), py::arg("y"));
}