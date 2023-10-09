// reused from particle physics engine

#pragma once
#include <SFML/Graphics.hpp>
#include <deque>

struct Particle
{
public:
    sf::Vector2f position;
    sf::Vector2f position_last;
    sf::Vector2f acceleration;

    std::deque<sf::Vector2f> history; // For tails
    uint8_t max_history_length = 10;

    sf::Color color = sf::Color::Red;

    // Particle() = default;

    Particle(sf::Vector2f position_) // constructor
        : position{position_}, position_last{position_}, acceleration{0.0f, 0.0f}
    {
    }

    void update(float dt)
    {
        // Compute how much particle moved
        const sf::Vector2f displacement = position - position_last;
        // Update pos

        position_last = position;
        position = position + displacement + acceleration * (dt * dt);

        // Reset acceleration
        acceleration = {};
    }

    void updateHistory()
    {
        history.push_back(position);

        // Limit the history length to control the tail length
        while (history.size() > max_history_length)
        {
            history.pop_front();
        }
    }

    void accelerate(sf::Vector2f a)
    {
        acceleration += a;
    }

    void setVelocity(sf::Vector2f v, float dt)
    {
        position_last = position - (v * dt);
    }

    void addVelocity(sf::Vector2f v, float dt)
    {
        position_last -= v * dt;
    }

    void slowdown(float ratio)
    {
        position_last = position_last + ratio * (position - position_last);
    }

    [[nodiscard]] sf::Vector2f getVelocity(float dt) const
    {
        return (position - position_last) / dt;
    }

    [[nodiscard]] float getSpeed() const
    {
        sf::Vector2f v = position - position_last;
        return sqrt(v.x * v.x + v.y * v.y);
    }
};
