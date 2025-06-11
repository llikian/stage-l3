/***************************************************************************************************
 * @file  Model.cpp
 * @brief Implementation of the Model class
 **************************************************************************************************/

#include "mesh/Model.hpp"

Model::Model(const std::filesystem::path& obj_file_path, bool verbose) {
    std::string extension = obj_file_path.extension().string();

    if(extension != ".obj") {
        throw std::runtime_error("Cannot load '" + extension + "' file, file formats supported by Model are: .obj");
    }

    std::ifstream file(obj_file_path);
    if(!file.is_open()) { throw std::runtime_error("Couldn't open file '" + obj_file_path.string() + '\''); }

    if(verbose) { std::cout << "Reading Model from file '" << obj_file_path.filename().string() << "':\n"; }

    if(extension == ".obj") { parse_obj_file(file, verbose); }
}

void Model::parse_obj_file(std::ifstream& file, bool verbose) {

}

void Model::draw(Shader& shader) {
    shader.use();
    for(TriangleMesh& mesh : meshes) { mesh.draw(); }
}
