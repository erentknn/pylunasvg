#include <string>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include "lunasvg.h"

namespace py = pybind11;
using namespace lunasvg;

PYBIND11_MODULE(pylunasvg, m)
{
	m.doc() = "pylunasvg is python bindings for lunasvg library";
    py::class_<Matrix> matrix(m, "Matrix");
	py::class_<Box>(m, "Box")
		.def(py::init<>())
		.def(py::init<double, double, double, double>(),
			py::arg("x"), py::arg("y"), py::arg("w"), py::arg("h"))
        .def("transform", &Box::transform, py::arg("matrix"))
        .def("transformed", &Box::transformed, py::arg("matrix"))
        .def_readwrite("x", &Box::x)
        .def_readwrite("y", &Box::y)
        .def_readwrite("w", &Box::w)
        .def_readwrite("h", &Box::h);

    py::class_<Matrix>(matrix, "Matrix")
        .def(py::init<>())
        .def(py::init<double, double, double, double, double, double>(),
             py::arg("a"), py::arg("b"), py::arg("c"), py::arg("d"), py::arg("e"), py::arg("f"))
        //.def(py::init<const Transform&>(), py::arg("transform"))
        .def("rotate", py::overload_cast<double>(&Matrix::rotate),
            py::arg("angle"))
        .def("rotate", py::overload_cast<double, double, double>(&Matrix::rotate),
             py::arg("angle"), py::arg("cx"), py::arg("cy"))
        .def("scale", &Matrix::scale,
             py::arg("sx"), py::arg("sy"))
        .def("shear", &Matrix::shear,
             py::arg("shx"), py::arg("shy"))
        .def("translate", &Matrix::translate,
             py::arg("tx"), py::arg("ty"))
        .def("transform", &Matrix::transform,
             py::arg("a"), py::arg("b"), py::arg("c"), py::arg("d"), py::arg("e"), py::arg("f"))
        .def("identity", &Matrix::identity)
        .def("invert", &Matrix::invert)
        .def(py::self *= Matrix())
        .def("premultiply", &Matrix::premultiply)
        .def("postmultiply", &Matrix::postmultiply)
        .def("inverted", &Matrix::inverted)
        .def(py::self * Matrix())
        .def_static("rotated", py::overload_cast<double>(&Matrix::rotated))
        .def_static("rotated", py::overload_cast<double, double, double>(&Matrix::rotated))
        .def_static("scaled", &Matrix::scaled, py::arg("sx"), py::arg("sy"))
        .def_static("sheared", &Matrix::scaled, py::arg("shx"), py::arg("shy"))
        .def_static("translated", &Matrix::scaled, py::arg("tx"), py::arg("ty"))
        .def_readwrite("a", &Matrix::a)
        .def_readwrite("b", &Matrix::b)
        .def_readwrite("c", &Matrix::c)
        .def_readwrite("d", &Matrix::d)
        .def_readwrite("e", &Matrix::e)
        .def_readwrite("f", &Matrix::f);

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