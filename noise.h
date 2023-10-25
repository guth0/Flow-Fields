#pragma once
#include "PerlinNoise.hpp"
#include <iostream>

class PerlinField
{
public:
    std::vector<float> data;
    uint16_t width;
    uint16_t height;

    PerlinField(uint16_t seed_)
    {
        seed = seed_;
        perlin = siv::PerlinNoise{seed_};
    }

    void setSize(uint16_t w, uint16_t h)
    {
        width = w;
        height = h;

        data.resize(w * h);
    }

    void generateField()
    {
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                data[(x * height) + y] = perlin.octave2D_01((x * 3.0f), (y * 3.0f), ocateves);
            }
        }
    }

private:
    siv::PerlinNoise::seed_type seed;
    siv::PerlinNoise perlin;
    static constexpr uint8_t ocateves = 3;
};