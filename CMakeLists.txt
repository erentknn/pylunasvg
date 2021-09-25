cmake_minimum_required (VERSION 3.13...3.18)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)

project ("pylunasvg")
add_subdirectory(pybind11)
add_subdirectory(lunasvg EXCLUDE_FROM_ALL)

pybind11_add_module(pylunasvg src/pylunasvg.cpp)
target_link_libraries(pylunasvg PUBLIC pybind11::module)
target_link_libraries(pylunasvg PRIVATE lunasvg)
target_compile_definitions(pylunasvg PRIVATE)