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

    static constexpr uint8_t history_frames_between = 5;
    uint8_t frames_since_last_history = history_frames_between; // must to start above history_frames_between
    static constexpr uint8_t max_history_length = 20;
    std::deque<sf::Vector2f> history;

    sf::Color color = sf::Color::Red;

    // Particle() = default;

    Particle(sf::Vector2f position_) // constructor
        : position{position_}, position_last{position_}, acceleration{0.0f, 0.0f}
    {
        history.push_back(position);
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
        if (frames_since_last_history >= history_frames_between)
        {
            history.push_back(position);

            // Limit the history length to control the tail length
            while (history.size() > max_history_length)
            {
                history.pop_front();
            }
            frames_since_last_history = 0;
        }
        else
        {
            frames_since_last_history++;
        }
    }

    void clearHistory()
    {
        history.clear();
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
