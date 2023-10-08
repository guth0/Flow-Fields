#include <iostream>
#include <SFML/Graphics.hpp>

struct NoiseCell
{
public:
    sf::Vector2f vec = {0.0f, 0.0f};
    float acceleration = 1.0f;

    NoiseCell() = default;

    NoiseCell(sf::Vector2f vec_)
        : vec{vec_}
    {
    }
};