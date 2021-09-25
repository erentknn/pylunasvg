#include <string>

#include <pybind11/pybind11.h>
#include "lunasvg.h"

namespace py = pybind11;
using namespace lunasvg;

PYBIND11_MODULE(pylunasvg, m)
{
	m.doc() = "pylunasvg is python bindings for lunasvg library";
	py::class_<Box>(m, "Box")
		.def(py::init<>())
		.def(py::init<double, double, double, double>(), 
			py::arg("x"), py::arg("y"), py::arg("w"), py::arg("h"));

	py::class_<Matrix>(m, "Matrix")
	.def(py::init<>())
	.def(py::init<double, double, double, double, double, double>(), 
		py::arg("a"), py::arg("b"), py::arg("c"), py::arg("d"), py::arg("e"), py::arg("f"));

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
		.def("rotate", py::overload_cast<double>(&Document::rotate), 
			py::arg("angle"), 
			"Rotates the document matrix clockwise around the current origin")
		.def("rotate", py::overload_cast<double, double, double>(&Document::rotate), 
			py::arg("angle"), py::arg("cx"), py::arg("cy"), 
			"Rotates the document matrix clockwise around the given point")
		.def("scale", &Document::scale, 
			py::arg("sx"), py::arg("sy"), 
			"Scales the document matrix by sx horizontally and sy vertically")
		.def("shear", &Document::shear, 
			py::arg("shx"), py::arg("shy"), 
			"Shears the document matrix by shx horizontally and shy vertically")
		.def("translate", &Document::translate, 
			py::arg("tx"), py::arg("ty"), 
			"Translates the document matrix by tx horizontally and ty vertically")
		.def("transform", &Document::transform, 
			py::arg("a"), py::arg("b"), py::arg("c"), py::arg("d"), py::arg("e"), py::arg("f"),
			 "Multiplies the document matrix by Matrix(a, b, c, d, e, f)")
		.def("identity", &Document::identity, "Resets the document matrix to identity")
		.def("matrix", &Document::matrix, "Returns the current transformation matrix of the document")
		.def("box", &Document::box, "Returns the smallest rectangle in which the document fits")
		.def("width", &Document::width, "Returns width of the document")
		.def("height", &Document::height, "Returns the height of the document")
		.def("render", &Document::render, 
			py::arg("bitmap"), py::arg("matrix") = Matrix{}, py::arg("backgroundColor") = 0x00000000, 
			"Renders the document to a bitmap")
		.def("renderToBitmap", &Document::renderToBitmap, 
			py::arg("width") = 0, py::arg("height") = 0, py::arg("backgroundColor") = 0x00000000, 
			"Renders the document to a bitmap");
}