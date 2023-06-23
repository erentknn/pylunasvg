#include <string>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include "lunasvg.h"

namespace py = pybind11;
using namespace lunasvg;

template <typename T>
py::capsule cleanup_callback = [T cls]() {
    return cls
};

PYBIND11_MODULE(pylunasvg, m)
{
	m.doc() = "pylunasvg is python bindings for lunasvg library";
	py::class_<Box>(m, "Box")
		.def(py::init<>())
		.def(py::init<double, double, double, double>(), 
			py::arg("x"), py::arg("y"), py::arg("w"), py::arg("h"))
		.def(py::init<wrap(Rect)>(),
			py::arg("rect"));

	py::class_<Matrix>(m, "Matrix")
	.def(py::init<>())
	.def(py::init<double, double, double, double, double, double>(), 
		py::arg("a"), py::arg("b"), py::arg("c"), py::arg("d"), py::arg("e"), py::arg("f"))
	.def(py::init<py::capsule(Transform&)>(),
		py::arg("transform"))
	.def("rotate", py::overload_cast<double>(&Matrix::rotate),
		py::arg("angle"))
	.def("rotate", py::overload_cast<double, double, double>(&Matrix::rotate),
		py::arg("angle"), py::arg("cx"), py::arg("cy"))
	.def("scale", &Matrix::scale,
		py::arg("sx"), py::arg("sy"))
	.def("shear", &Matrix::shear,
		py::arg("shx"), py::arg("shy"))
	.def("translate", &Matrix::translate, 
		py::arg("tx"), py::arg("ty"), 
		"Translates the document matrix by tx horizontally and ty vertically")
	.def("transform", &Matrix::transform, 
		py::arg("a"), py::arg("b"), py::arg("c"), py::arg("d"), py::arg("e"), py::arg("f"),
			"Multiplies the document matrix by Matrix(a, b, c, d, e, f)")
	.def("identity", &Matrix::identity, "Resets the document matrix to identity")
	.def("invert", &Matrix::invert)
	.def(py::self *= py::self)
	.def("premultiply", &Matrix::premultiply, py::arg("matrix"))
	.def("postmultiply", &Matrix::postmultiply, py::arg("matrix"))
	.def("inverted", &Matrix::inverted)
	.def(py::self * py::self)
	.def_static("rotated", py::overload_cast<double>(&Matrix::rotated),
		py::arg("angle"))
	.def_static("rotated", py::overload_cast<double, double, double>(&Matrix::rotated),
		py::arg("angle"), py::arg("cx"), py::arg("cy"))
	.def_static("scaled", &Matrix::scaled,
		py::arg("sx"), py::arg("sy"))
	.def_static("sheared", &Matrix::sheared,
		py::arg("shx"), py::arg("shy"))
	.def_static("translated", &Matrix::translated, 
		py::arg("tx"), py::arg("ty"), 
		"Translates the document matrix by tx horizontally and ty vertically");

	py::class_<Bitmap>(m, "Bitmap", py::buffer_protocol())
		.def(py::init<>())
		.def(py::init<std::uint8_t*, std::uint32_t, std::uint32_t, std::uint32_t>(), 
			py::arg("data"), py::arg("width"), py::arg("height"), py::arg("stride"))
		.def(py::init<std::uint32_t, std::uint32_t>(), 
			py::arg("width"), py::arg("height"))
		.def("reset", py::overload_cast<std::uint8_t*, std::uint32_t, std::uint32_t, std::uint32_t>(&Bitmap::reset),
			 py::arg("data"), py::arg("width"), py::arg("height"), py::arg("stride"))
		.def("reset", py::overload_cast<std::uint32_t, std::uint32_t>(&Bitmap::reset), 
			py::arg("width"), py::arg("height"))
		.def("width", &Bitmap::width)
		.def("height", &Bitmap::height)
		.def("stride", &Bitmap::stride)
		.def("valid", &Bitmap::valid)
		.def("clear", &Bitmap::clear, py::arg("color"))
		.def("convert", &Bitmap::convert, py::arg("ri"), py::arg("gi"), py::arg("bi"), py::arg("ai"), py::arg("premultiply"))
		.def("convertToRGBA", &Bitmap::convertToRGBA)
		.def_buffer([](Bitmap& img) -> py::buffer_info {
			return py::buffer_info(
				img.data(), 
				sizeof(std::uint8_t), 
				py::format_descriptor<std::uint8_t>::format(), 
				3,
				{ img.height(), img.width(), static_cast<std::uint32_t>(4) },
				{
					sizeof(std::uint8_t) * 4 * img.width(),
					sizeof(std::uint8_t) * 4,
					sizeof(std::uint8_t) });
	});

	py::class_<Document>(m, "Document")
		.def_static("loadFromFile", &Document::loadFromFile,
			py::arg("filename"), 
			"Creates a document from a file")
		.def_static("loadFromData", py::overload_cast<const std::string&>(&Document::loadFromData),
			py::arg("string"), 
			"Creates a document from a string")
		.def_static("loadFromData", py::overload_cast<const char*, std::size_t>(&Document::loadFromData), 
			py::arg("data"), py::arg("size"), 
			"Creates a document from a string data and size")
		.def_static("loadFromData", py::overload_cast<const char*>(&Document::loadFromData),
			py::arg("data"), 
			"Creates a document from a null terminated string data")
		.def("setMatrix", &Document::setMatrix, py::arg("matrix"))
		.def("matrix", &Document::matrix, "Returns the current transformation matrix of the document")
		.def("box", &Document::box, "Returns the smallest rectangle in which the document fits")
		.def("width", &Document::width, "Returns width of the document")
		.def("height", &Document::height, "Returns the height of the document")
		.def("render", &Document::render, 
			py::arg("bitmap"), py::arg("matrix") = Matrix{}, 
			"Renders the document to a bitmap")
		.def("renderToBitmap", &Document::renderToBitmap, 
			py::arg("width") = 0, py::arg("height") = 0, py::arg("backgroundColor") = 0x00000000, 
			"Renders the document to a bitmap");
}