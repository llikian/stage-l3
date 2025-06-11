/***************************************************************************************************
 * @file  parsers.hpp
 * @brief Declaration of function aimed at parsing specific file formats.
 **************************************************************************************************/

#pragma once

#include "mesh/TriangleMesh.hpp"

void parse_obj_file(const std::string& path, TriangleMesh& mesh, bool verbose = false);
