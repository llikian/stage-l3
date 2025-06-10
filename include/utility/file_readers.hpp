/***************************************************************************************************
 * @file  file_readers.hpp
 * @brief Declaration of function aimed at reading specific file formats.
 **************************************************************************************************/

#pragma once

#include "mesh/TriangleMesh.hpp"

#include <fstream>

void read_obj_file(const std::string& path, TriangleMesh& mesh, bool verbose = false);
