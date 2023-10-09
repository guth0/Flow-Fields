#pragma once
#include <SFML/Graphics.hpp>

class VectorField
{
public:
    uint16_t cell_size;
    uint16_t height;
    uint16_t width;
    sf::Vector2i window_resolution;
    std::vector<float> data;

    VectorField() = default;

    VectorField(uint16_t cell_size_, sf::Vector2i window_resolution)
    {
        cell_size = cell_size_;
        width = floor(window_resolution.x / cell_size_);
        height = floor(window_resolution.y / cell_size_);

        data.resize(width * height);
    }

    void setSize(uint16_t cell_size_)
    {

        cell_size = cell_size_;
        width = floor(window_resolution.x / cell_size_);
        height = floor(window_resolution.y / cell_size_);

        data.resize((width + 2) * (height + 2));
    }

    void createField(float zoom, float curve)
    {
        for (int x = 0; x < width + 2; x++)
        {
            for (int y = 0; y < height + 2; y++)
            {
                float angle = (pow(cos(x * zoom), 2) + sin(y * zoom)) * curve;
                data[(x * height) + y] = angle;
            }
        }
    }
};