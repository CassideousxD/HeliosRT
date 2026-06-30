#pragma once
#include "primitives/Mesh.h"
#include "material/Material.h"
#include <string>
#include <memory>

class OBJLoader
{
public:
    static std::shared_ptr<Mesh> load(
        const std::string& filepath,
        std::shared_ptr<Material> material);
};