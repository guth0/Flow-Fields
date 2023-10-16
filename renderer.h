#pragma once
#include "system.h"

class Renderer
{

public:
    explicit Renderer(sf::RenderTarget &target)
        : m_target{target}
    {
    }

    void render(const ParticleSystem &system) const
    {

        // Render objects
        sf::CircleShape circle{1.0f};
        circle.setPointCount(32);
        circle.setOrigin(1.0f, 1.0f);
        const std::vector<Particle> &particles = system.getParticles();
        for (const Particle &particle : particles)
        {
            // circle.setPosition(particle.position);
            // circle.setScale(system.standard_radius, system.standard_radius);
            // circle.setFillColor(particle.color);
            // m_target.draw(circle);
            uint8_t history_size = particle.history.size();

            if (history_size >= 2)
            {
                for (int i = 0; i < history_size - 1; ++i)
                {
                    sf::Vector2f pos = particle.history[i];
                    if (pos.x > 1 && pos.x < system.m_world_size.x - 1 &&
                        pos.y > 1 && pos.y < system.m_world_size.y - 1)
                    {

                        sf::Vertex line[] = {
                            sf::Vertex(particle.history[i]),
                            sf::Vertex(particle.history[i + 1])};

                        sf::Color color = particle.color;
                        color.a = static_cast<uint8_t>((color.a / (history_size - i)) + 100);

                        line[0].color = color;
                        line[1].color = color;

                        m_target.draw(line, 2, sf::Lines);
                    }
                }
            }
        }
    }

private:
    sf::RenderTarget &m_target;
};
