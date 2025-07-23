/***************************************************************************************************
 * @file  Cubemap.hpp
 * @brief Declaration of the Cubemap class
 **************************************************************************************************/

#pragma once
#include <filesystem>

/**
 * @class Cubemap
 * @brief
 */
class Cubemap {
public:
    /**
     * @brief
     * Paths order: PX, NX, PY, NY, PZ, NZ
     * @param paths
     */
    explicit Cubemap(const std::initializer_list<std::filesystem::path>& paths);
    ~Cubemap();

    /**
     * @brief Binds the texture to a specifc texture unit.
     * @param texUnit The opengl texture unit ID.
     */
    void bind(unsigned int texUnit = 0) const;

    /**
     * @return The texture's id.
     */
    unsigned int get_id() const;

private:
    unsigned int id; ///< Texture id.
};
