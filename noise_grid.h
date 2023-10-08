#include "noise_cell.h"
#include "perlin_noise.h"
#include <iostream>

class NoiseGrid
{
public:
    std::vector<std::vector<NoiseCell>> data;
    uint32_t height;
    uint32_t width;
    uint16_t seed = 250;

    NoiseGrid() = default;

    NoiseGrid(uint32_t x, uint32_t y)
    {
        width = x;
        height = y;
        data.resize(x, std::vector<NoiseCell>(y));
    }

    void setSize(uint32_t x, uint32_t y)
    {
        width = x;
        height = y;
        data.resize(x, std::vector<NoiseCell>(y));
    }

    void setSeed(uint16_t seed_)
    {
        seed = seed_;
    }

    void generateNoise()
    {
        VectorField field();
    }

    [[nodiscard]] sf::Vector2f getVector(sf::Vector2i position)
    {
        uint32_t x = position.x;
        uint32_t y = position.y;

        NoiseCell cell = data[x][y];
        return cell.vec * cell.acceleration;
    }
};