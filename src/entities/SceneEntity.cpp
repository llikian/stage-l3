/***************************************************************************************************
 * @file  SceneEntity.cpp
 * @brief Implementation of the SceneEntity class
 **************************************************************************************************/

#include "entities/SceneEntity.hpp"

SceneEntity::SceneEntity(const std::string& name, const std::filesystem::path& path)
    : Entity(name), scene(path) { }
