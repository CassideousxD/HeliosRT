#pragma once

#include "material/Texture.h"
#include <string>
#include <vector>

class ImageTexture : public Texture
{
public:
    explicit ImageTexture(const std::string& filepath);

    Vector3 sample(float u, float v, const Vector3& point) const override;
    bool isLoaded() const;

private:
    int width;
    int height;
    std::vector<unsigned char> pixels;
};
