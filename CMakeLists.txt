cmake_minimum_required (VERSION 3.18)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

project (pylunasvg)

add_subdirectory(pybind11)
add_subdirectory(lunasvg)

pybind11_add_module(pylunasvg src/pylunasvg.cpp)
target_link_libraries(pylunasvg PRIVATE pybind11::module)
target_link_libraries(pylunasvg PRIVATE lunasvg)
target_include_directories(pylunasvg PRIVATE lunasvg/source)
target_include_directories(pylunasvg PRIVATE plutovg/include)
target_compile_definitions(pylunasvg PRIVATE)
