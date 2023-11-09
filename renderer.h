#pragma once
#include "system.h"

class Renderer
{

public:
    explicit Renderer(sf::RenderTarget &target, const sf::Vector2i &world_size)
        : m_target{target}, m_world_size{world_size}, m_origin{m_world_size / 2}
    {
    }

    void render(std::array<Particle, PARTICLE_CAP> &particles) const
    {

        //\\// Render objects //\\//
        // sf::CircleShape circle{1.0f};
        // circle.setPointCount(32);
        // circle.setOrigin(1.0f, 1.0f);
        // const std::vector<Particle> &particles = system.getParticles();

        sf::Color color;
        sf::Vertex line[2];

        for (const Particle &particle : particles)
        {
            //\\// Render Point //\\//
            // circle.setPosition(particle.position);
            // circle.setScale(system.standard_radius, system.standard_radius);
            // circle.setFillColor(particle.color);
            // m_target.draw(circle);
            //\\// Render Point //\\//

            //\\// Render Tail //\\//
            uint8_t history_size = particle.history.size();

            if (history_size > 1)
            {
                for (int i = 0; i < history_size - 1; ++i)
                {
                    sf::Vector2f pos = particle.history[i];
                    if (pos.x > 1 && pos.x < m_world_size.x - 1 &&
                        pos.y > 1 && pos.y < m_world_size.y - 1)
                    {

                        line[0] = sf::Vertex(particle.history[i]);
                        line[1] = sf::Vertex(particle.history[i + 1]);

                        color = particle.color;
                        line[0].color = color;
                        line[1].color = color;

                        m_target.draw(line, 2, sf::Lines);
                    }
                }
            }
            //\\// Render Tail //\\//
        }
    }

private:
    sf::RenderTarget &m_target;
    const sf::Vector2i &m_world_size;
    const sf::Vector2i m_origin;

    // float getSlope(const sf::Vertex line[]) const
    // {
    //     const sf::Vector2f v1 = line[0].position;
    //     const sf::Vector2f v2 = line[1].position;

    //     return ((v1.x - v2.x) / (v1.y - v2.y));
    // }

    // uint16_t getDist(const sf::Vertex line[]) const
    // {
    //     const sf::Vector2f v1 = line[0].position;
    //     const sf::Vector2f v2 = line[1].position;

    //     return (v1.x + v1.y + v2.x + v2.y);
    // }

    // uint16_t getOriginDist(const sf::Vector2f &v1) const
    // {
    //     const uint16_t dx = m_origin.x - v1.x;
    //     const uint16_t dy = m_origin.y - v1.y;
    //     return abs(dx * dx - dy * dy);
    // }

    // sf::Color getRainbow(const float &t) const
    // {
    //     const float r = sin(t);
    //     const float g = sin(t + 0.33f * 2.0f * M_PI);
    //     const float b = sin(t + 0.66f * 2.0f * M_PI);
    //     return {static_cast<uint8_t>(255.0f * r * r),
    //             static_cast<uint8_t>(255.0f * g * g),
    //             static_cast<uint8_t>(255.0f * b * b)};
    // }
};
