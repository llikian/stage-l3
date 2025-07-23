/***************************************************************************************************
 * @file  Node.hpp
 * @brief Declaration of the Node struct
 **************************************************************************************************/

#pragma once

#include <string>
#include <vector>

enum class DataType : unsigned char;

constexpr unsigned int MAX_NODE_DATA_COUNT = 4;

struct Node {
    enum class Type : unsigned char {
        SIMPLE,           // NO DATA
        MESH,             // 0: MESH, 1: SHADER, (2: MATERIAL)
        FLAT_SHADED_MESH, // 0: MESH, 1: SHADER, 2: COLOR
        MODEL,            // 0: MESH, 1: SHADER
        SCENE,            // 0: SCENE
        TERRAIN,          // 0: TERRAIN
    };

    struct Data {
        Data();
        DataType type; ///< The type of data. DataType::NONE when there is no data.
        int index;     ///< The index of the data. -1 when there is no data.
    };

    Node(const std::string& name, int parent, unsigned int transform_index, Type type);

    void add_data(DataType type, unsigned int index);

    Type type;        ///< The type of the node.
    std::string name; ///< The name of the node.

    int parent;                         ///< The index of the node's parent. -1 if it is the root.
    std::vector<unsigned int> children; ///< The indices of the node's children.

    unsigned int transform_index; ///< The index of the node's transform.

    Data data[MAX_NODE_DATA_COUNT]; ///< The node's data information

    bool is_visible;
    bool is_selected;
};
