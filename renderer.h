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
            circle.setPosition(particle.position);
            circle.setScale(system.standard_radius, system.standard_radius);
            circle.setFillColor(particle.color);
            m_target.draw(circle);
            for (int i = 0; i < particle.history.size(); ++i)
            {
                sf::Vertex line[] = {
                    sf::Vertex(particle.history[i]),
                    sf::Vertex(particle.history[i + 1])};
                m_target.draw(line, 2, sf::Lines);
            }
        }
    }

private:
    sf::RenderTarget &m_target;
};
